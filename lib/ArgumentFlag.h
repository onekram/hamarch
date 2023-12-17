//
// Created by onekram on 11/18/23.
//
#pragma once
#include "Argument.h"

class ArgumentFlag : public Argument {
public:
    explicit ArgumentFlag(const std::string &name);
    ArgumentFlag(const std::string& name, const std::string& description);

    ArgumentFlag& Default(bool value);
    void SetValue(bool value);
    ArgumentFlag& StoreValue(bool &value);

    std::string GetInfo() override;
    [[nodiscard]] bool GetArg() const { return value_arg_; };
private:
    bool* stored_value_;
    bool value_arg_;
};
