#include <vector>
#include <iostream>
#include <algorithm>
#include "ArgParser.h"
#include <string_view>

namespace ArgumentParser {
ArgParser::ArgParser(const std::string& name) : instance_name_(name) {}

std::string ArgParser::GetParameter(const std::string_view & str) {
    auto pos = str.rfind('-');
    if (pos == std::string::npos) {
        std::cerr << "Error: Invalid parameter " << str << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string_view param = str.substr(pos + 1);
    if (param.length() == 1)
        return arguments_.multi_names.find(std::string(param))->second;
    return std::string(param);
}

int ArgParser::GetValueInt(const std::string_view& str) {
    try {
        return std::stoi(std::string(str));
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error: Invalid argument " << str << std::endl;
        exit(EXIT_FAILURE);
    } catch (const std::out_of_range &ex) {
        std::cerr << "Error: Invalid argument " << str << " dont match int 32 bits" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool ArgParser::IsNumeric(const std::string_view& str) {
    return std::ranges::all_of(str.begin(), str.end(), isdigit);
}

void ArgParser::FlagProcessing(const std::string_view& str) {
    if (str[0] == '-' && str[1] == '-') {
        std::string_view param = str.substr(2);
        if (param == help_.name)
            help_.help_added = true;
        else
            arguments_.flag_arguments.find(std::string(param))->second->SetValue(true);
    } else if (str[0] == '-' && str[1] != '-') {
        std::string_view param = str.substr(1);
        if (param == help_.short_name)
            help_.help_added = true;
        else
            for (auto c: param) {
                std::string value{c};
                std::string split = arguments_.multi_names.find(value)->second;
                arguments_.flag_arguments.find(split)->second->SetValue(true);
            }
    }
}

bool ArgParser::PutPositional(const std::string_view& el) {
    for (const auto &arg: arguments_.int_arguments) {
        if (arg.second->IsPositional()) {
            if (IsNumeric(el)) {
                int valueInt = GetValueInt(el);
                arg.second->SetValue(valueInt);
                return true;
            }
        }
    }
    for (const auto &arg: arguments_.string_arguments) {
        if (arg.second->IsPositional()) {
            if (!IsNumeric(el)) {
                arg.second->SetValue(std::string(el));
                return true;
            }
        }
    }
    return false;
}

bool ArgParser::CheckAddedArguments() {
    if (help_.help_added) {
        return true;
    }
    for (auto& arg: arguments_.int_arguments) {
        if (!arg.second->IsFilled()) {
            return false;
        }
    }
    for (auto& arg: arguments_.string_arguments) {
        if (!arg.second->IsFilled()) {
            return false;
        }
    }
    return true;
}

bool ArgParser::Parse(const std::vector<std::string_view>& args) {
    if (args.empty()) {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    arguments_.program_name = args[0];
    int i = 1;
    while (i < args.size()) {
        const std::string_view &el = args[i];
        auto pos = el.find('=');
        if (pos == std::string::npos) {
            if (el[0] == '-')
                FlagProcessing(std::string(el));
            else if (!PutPositional(std::string(el))) {
                std::cerr << "Error: " << el << " is not correct positional argument" << std::endl;
                exit(EXIT_FAILURE);
            }
            i++;
            continue;
        }
        std::string key = GetParameter(std::string(el.substr(0, pos)));
        std::string_view value = el.substr(pos + 1);
        int valueInt;
        std::string valueStr;
        if (IsNumeric(std::string(value))) {
            valueInt = GetValueInt(std::string(value));
            arguments_.int_arguments.find(key)->second->SetValue(valueInt);
        } else {
            arguments_.string_arguments.find(key)->second->SetValue(std::string(value));
        }
        ++i;
    }
    return CheckAddedArguments();
}

ArgumentString& ArgParser::AddStringArgument(const std::string& name,
                                                       const std::string& description) {
    auto* arg = new ArgumentString(name, description);
    arguments_.string_arguments.insert({name, arg});

    return *arg;
}

ArgumentString& ArgParser::AddStringArgument(char el, const std::string& name,
                                                       const std::string& description) {
    auto* arg = new ArgumentString(name, description);
    std::string s{el};
    arg->SetShortName(s);
    arguments_.multi_names.insert({s, name});
    arguments_.string_arguments.insert({name, arg});

    return *arg;
}

std::string ArgParser::GetStringValue(const std::string& name, int index) {
    return std::vector<std::string>(arguments_.string_arguments.find(name)->second->GetArg())[index];
}

ArgumentInt& ArgParser::AddIntArgument(const std::string& name, const std::string& description) {
    auto *arg = new ArgumentInt(name, description);
    arguments_.int_arguments.insert({name, arg});

    return *arg;
}

ArgumentInt& ArgParser::AddIntArgument(char el, const std::string& name,
                                                    const std::string& description) {
    auto* arg = new ArgumentInt(name, description);
    std::string s{el};
    arg->SetShortName(s);
    arguments_.multi_names.insert({s, name});
    arguments_.int_arguments.insert({name, arg});

    return *arg;
}

int ArgParser::GetIntValue(const std::string& name, int index) {
    return arguments_.int_arguments.find(name)->second->GetArg()[index];
}

ArgumentFlag& ArgParser::AddFlag(char el, const std::string &name,
                                             const std::string &description) {
    auto* arg = new ArgumentFlag(name, description);
    std::string s{el};
    arguments_.multi_names.insert({s, name});
    arg->SetShortName(s);
    arguments_.flag_arguments.insert({name, arg});

    return *arg;
}

bool ArgParser::GetFlag(const std::string& name) {
    return arguments_.flag_arguments.find(name)->second->GetArg();
}

ArgumentFlag& ArgParser::AddFlag(const std::string& name, const std::string& description) {
    auto* arg = new ArgumentFlag(name, description);
    arguments_.flag_arguments.insert({name, arg});

    return *arg;
}

void ArgParser::AddHelp(char el, const std::string& name,
                                        const std::string& description) {
    std::string val{el};
    help_.short_name = val;
    help_.name = name;
    help_.help_description = description;
}

void ArgParser::AddHelp(const std::string& name, const std::string& description) {
    help_.name = name;
    help_.help_description = description;
}

std::string ArgParser::HelpDescription() const {
    std::stringstream accStr;
    accStr << instance_name_ << "\n" << help_.help_description << "\n\n";
    for (auto& arg: arguments_.int_arguments) {
        accStr << arg.second->GetInfo();
    }
    for (auto& arg: arguments_.string_arguments) {
        accStr << arg.second->GetInfo();
    }
    for (auto& arg: arguments_.flag_arguments) {
        accStr << arg.second->GetInfo();
    }
    accStr << "\n";
    if (help_.help_added)
        accStr << "-" << help_.short_name << ",  " << "--" << help_.name << ",  " << help_.help_description;
    return accStr.str();
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string_view> res;
    for (int i = 1; i < argc; i++) {
        res.emplace_back(argv[i]);
    }
    return Parse(res);
}
bool ArgParser::Parse(const std::vector<std::string>& args) {
    std::vector<std::string_view> res;
    for (auto& arg: args) {
        res.emplace_back(arg);
    }
    return Parse(res);
}

ArgParser::~ArgParser() {
    for (auto& arg: arguments_.string_arguments) {
        delete arg.second;
    }
    for (auto& arg: arguments_.int_arguments) {
        delete arg.second;
    }
    for (auto& arg: arguments_.flag_arguments) {
        delete arg.second;
    }
}
} // namespace ArgumentParser
