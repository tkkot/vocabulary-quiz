#pragma once
#include <functions.h>

class consoleUI : public UI{
public:
    void init() override;
    void start() override;
    void writeText(const std::string& text) override;
    void writeText(std::initializer_list<std::string> text) override;
    UI& operator<<(const std::string &text) override;
    std::string readLine() override;
    void cls() override;

    // void listList(const std::vector<std::string> &list) override;
    // void listTree(const std::vector<std::string> &list) override;

    // //UI has access to global state
    // void trainingStart() override;
    // std::string trainingAsk(const std::string &q) override;
    // void trainingEnd() override;



    int consoleCommand(const std::vector<std::string>& args);	//Execute UI function args[0] with arguments args
};

extern uint8_t flags;	//Program flags (for now only [0]=running)