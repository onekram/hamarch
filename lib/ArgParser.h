#pragma once

#include <string>
#include <vector>
#include <map>
#include "ArgumentInt.h"
#include "ArgumentString.h"
#include "ArgumentFlag.h"


namespace ArgumentParser {
struct Arguments {
    std::string program_name;
    std::map<std::string, ArgumentInt*> int_arguments;
    std::map<std::string, ArgumentString*> string_arguments;
    std::map<std::string, ArgumentFlag*> flag_arguments;
    std::map<std::string, std::string> multi_names;
};
struct HelpInfo {
    std::string short_name;
    std::string name;
    std::string help_description;
    bool help_added = false;
};
class ArgParser {
public:
    explicit ArgParser(const std::string& name);
    ~ArgParser();

    bool Parse(const std::vector<std::string_view >& args);
    bool Parse(int argc, char** argv);
    bool Parse(const std::vector<std::string>& args);

    ArgumentString& AddStringArgument(const std::string& name, const std::string& description = "  ");
    ArgumentString& AddStringArgument(char el, const std::string& name, const std::string& description = "  ");
    std::string GetStringValue(const std::string& name, int index = 0);

    ArgumentInt& AddIntArgument(const std::string &name, const std::string& description = "  ");
    ArgumentInt& AddIntArgument(char el, const std::string& name, const std::string& description = "  ");
    int GetIntValue(const std::string& name, int index = 0);

    ArgumentFlag& AddFlag(char el, const std::string& name, const std::string& description = "  ");
    ArgumentFlag& AddFlag(const std::string& name, const std::string& description = "  ");
    bool GetFlag(const std::string& name);

    void AddHelp(char el, const std::string& name, const std::string& description = "  ");
    void AddHelp(const std::string& name, const std::string& description = "  ");

    [[nodiscard]] bool Help() const {return help_.help_added;}
    [[nodiscard]] std::string HelpDescription() const;
private:
    std::string instance_name_;
    Arguments arguments_;
    HelpInfo help_;
    std::string GetParameter(const std::string_view& str);
    static int GetValueInt(const std::string_view& str);
    void FlagProcessing(const std::string_view & str);
    static bool IsNumeric(const std::string_view& str);
    bool CheckAddedArguments();
    bool PutPositional(const std::string_view& el);
};

} // namespace ArgumentParser
