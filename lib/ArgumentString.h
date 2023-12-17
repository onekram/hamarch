//
// Created by onekram on 11/18/23.
//
#include "Argument.h"

class ArgumentString : public Argument {
public:
    explicit ArgumentString(const std::string& name);
    ArgumentString(const std::string& name, const std::string& description);

    ArgumentString& StoreValue(std::string& value);
    ArgumentString& StoreValues(std::vector<std::string>& values);

    void SetValue(const std::string& value);
    ArgumentString& Default(const char* value);

    ArgumentString& Positional();
    ArgumentString& MultiValue(int min = 0);

    std::string GetInfo() override;
    [[nodiscard]] std::vector<std::string> GetArg() const { return value_arg_; };
private:
    std::string* stored_value_{};
    std::vector<std::string>* stored_vector_{};
    std::vector<std::string> value_arg_{};
};
