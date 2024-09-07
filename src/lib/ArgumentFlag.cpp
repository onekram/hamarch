//
// Created by onekram on 11/18/23.
//

#include "ArgumentFlag.h"

ArgumentFlag::ArgumentFlag(const std::string& name) : Argument(name) {filled = true;}

ArgumentFlag::ArgumentFlag(const std::string& name, const std::string& description) : Argument(name, description) {filled = true;}

ArgumentFlag& ArgumentFlag::Default(bool value) {
    defaulted = true;
    SetValue(value);
    return *this;
}

ArgumentFlag& ArgumentFlag::StoreValue(bool& value) {
    is_stored = true;
    stored_value_ = &value;
    return *this;
}

void ArgumentFlag::SetValue(bool value) {
    value_arg_ = value;
    filled = true;

    if (is_stored)
        *stored_value_ = value;
}

std::string ArgumentFlag::GetInfo() {
    std::stringstream acc;
    if (has_short_name)
        acc << "-" << short_name << ",  " << "--" << name_ << ",  " << description_;
    else
        acc << "     --" << name_ << ",  " << description_;
    if (defaulted) {
        if (value_arg_)
            acc << " [default = true]";
        else
            acc << " [default = false]";
    }
    acc << "\n";
    return acc.str();
}
