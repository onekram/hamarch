//
// Created by onekram on 11/16/23.
//
#include "Argument.h"

#include <string>

Argument::Argument(const std::string& name) : name_(name) {}
Argument::Argument(const std::string& name, const std::string& description) : name_(name), description_(description) {}

bool Argument::IsFilled() const {
    return filled;
}

bool Argument::IsPositional() const {
    return positional;
}

void Argument::SetShortName(const std::string& value) {
    short_name = value;
    has_short_name = true;
}
