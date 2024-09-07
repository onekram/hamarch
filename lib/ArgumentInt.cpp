//
// Created by onekram on 11/18/23.
//
#include "ArgumentInt.h"

ArgumentInt::ArgumentInt(const std::string& name) : Argument(name) {}

ArgumentInt::ArgumentInt(const std::string& name, const std::string& description) : Argument(name, description) {}

ArgumentInt& ArgumentInt::StoreValue(int& value) {
    is_stored = true;
    stored_value_ = &value;
    return *this;
}

ArgumentInt& ArgumentInt::StoreValues(std::vector<int>& values) {
    is_stored_vector = true;
    stored_vector_ = &values;
    return *this;
}


ArgumentInt& ArgumentInt::Default(int value) {
    defaulted = true;
    SetValue(value);
    return *this;
}

ArgumentInt& ArgumentInt::Positional() {
    positional = true;
    return *this;
}

void ArgumentInt::SetValue(int value) {
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

std::string ArgumentInt::GetInfo() {
    std::stringstream acc;
    if (has_short_name)
        acc << "-" + short_name << ",  " << "--" << name_ << "=<int>,  " << description_;
    else
        acc << "   --" << name_ << "=<int>,  " << description_;

    if (defaulted) {
        acc << " [default = ";
        acc << std::to_string(value_arg_[0]);
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

ArgumentInt& ArgumentInt::MultiValue(int min) {
    multi_value = true;
    min_amount = min;
    return *this;
}
