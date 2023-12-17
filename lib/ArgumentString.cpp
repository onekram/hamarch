//
// Created by onekram on 11/18/23.
//
#include "ArgumentString.h"

ArgumentString::ArgumentString(const std::string& name) : Argument(name) {}

ArgumentString::ArgumentString(const std::string &name, const std::string& description) : Argument(name, description) {}

ArgumentString& ArgumentString::StoreValue(std::string& value) {
    is_stored = true;
    stored_value_ = &value;

    return *this;
}

ArgumentString& ArgumentString::StoreValues(std::vector<std::string>& values) {
    is_stored_vector = true;
    stored_vector_ = &values;

    return *this;
}

ArgumentString& ArgumentString::Default(const char* valueCh) {
    defaulted = true;
    std::string value { valueCh};
    SetValue(value);

    return *this;
}

ArgumentString& ArgumentString::Positional() {
    positional = true;
}

void ArgumentString::SetValue(const std::string& value) {
    size_t size = value_arg_.size();
    if (!multi_value) {
        if (size == 0)
            value_arg_.push_back(value);
        else
            value_arg_[0] = value;
        filled = true;
    } else {
        value_arg_.push_back(value);
        if (value_arg_.size() >= min_amount)
            filled = true;
    }
    if (is_stored)
        *stored_value_ = value;
    if (is_stored_vector)
        stored_vector_->push_back(value);
}

std::string ArgumentString::GetInfo() {
    std::stringstream acc;
    if (has_short_name)
        acc << "-" << short_name << ",  " << "--" << name_ << "<string>,  " << description_;
    else
        acc << "     --" << name_ << "<string>,  " << description_;

    if (defaulted) {
        acc << " [default = ";
        acc << value_arg_[0];
        acc << "] ";
    }
    if (multi_value) {
        acc << " [repeated, min args = ";
        acc << std::to_string(min_amount);
        acc << "] ";
    }
    acc << "\n";
    return acc.str();
}

ArgumentString& ArgumentString::MultiValue(int min) {
    multi_value = true;
    min_amount = min;
}
