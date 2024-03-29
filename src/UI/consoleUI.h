#pragma once
#include <functions.h>

class consoleUI : public UI{
public:
    void writeText(const std::string& text) override;
    void writeText(std::initializer_list<std::string> text) override;
    std::string readLine() override;
    void cls() override;
};