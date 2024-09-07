//
// Created by onekram on 11/18/23.
//
#pragma once

#include <vector>
#include "Argument.h"

class ArgumentInt : public Argument{
public:
    explicit ArgumentInt(const std::string& name);
    ArgumentInt(const std::string& name, const std::string& description);

    ArgumentInt& StoreValue(int& value);
    ArgumentInt& StoreValues(std::vector<int>& values);

    ArgumentInt& Default(int value);
    void SetValue(int value);

    ArgumentInt& Positional();
    ArgumentInt& MultiValue(int min = 0);

    std::string GetInfo() override;
    [[nodiscard]] std::vector<int> GetArg() const { return value_arg_; };
private:
    int* stored_value_{};
    std::vector<int>* stored_vector_{};
    std::vector<int> value_arg_;
};
