//
// Created by onekram on 11/16/23.
//
#pragma once
#include <vector>
#include <string>
#include <sstream>
class Argument {
public:
    explicit Argument(const std::string& name);
    Argument(const std::string& name, const std::string& description);

    void SetShortName(const std::string& value);

    [[nodiscard]] bool IsFilled() const;
    [[nodiscard]] bool IsPositional() const;

    virtual std::string GetInfo() = 0;
protected:
    bool multi_value = false;
    int min_amount = 0;
    std::string short_name;
    bool has_short_name = false;
    std::string name_;
    std::string description_;
    bool is_stored = false;
    bool is_stored_vector = false;
    bool positional = false;
    bool defaulted = false;
    bool filled = false;
};
