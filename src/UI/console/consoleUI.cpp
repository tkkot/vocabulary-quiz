#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include <functions.h>
#include "consoleUI.h"

#define __MES_UI__
#define __MES_CONSOLE_UI__
#include <UI/messages.h>


//Platform-specific functions
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>

	//Cleaning screen on windows (code from the internet somewhere)
	void consoleUI::cls(){
		COORD tl = {0,0};
		CONSOLE_SCREEN_BUFFER_INFO s;
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(console, &s);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;
		FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
		FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
		SetConsoleCursorPosition(console, tl);
		std::cout << "=============================" << "\n\n";
	}
#else
	//Cleaning terminal (Also  from internet)
	void consoleUI::cls(){
		std::cout << "\033[2J\033[1;1H";
		std::cout << "=============================" << "\n\n";
	}
#endif

typedef void (*cmd_t)(const std::vector<std::string>&);	//Funcion pointers to executed commands
typedef const std::vector<std::string> &arg_t;		//Macro for command arguments


void consoleUI::init(){
	this->cls();
}
void consoleUI::start(){
	this->cls();
	this->writeText(mes::welcome);

	flags |= 1;
	cmd_t c;
	std::string line;
	while(flags & 1){
		line = this->readLine();
		trim(line);
		if (line.size()>0)
			this->consoleCommand(split(line, " "));
	}

	this->writeText(mes::exit);
}
void consoleUI::writeText(const std::string &text){
    std::cout<<text;
}
void consoleUI::writeText(std::initializer_list<std::string> text){
	for (std::string i : text){
		this->writeText(i);
	}
}
UI& consoleUI::operator<<(const std::string &text){
	this->writeText(text);
    return *this;
}
std::string consoleUI::readLine(){
    std::string line;
	std::getline(std::cin, line);
	return line;
}

//Wait until user presses ENTER
void UIwait(){
	ui->writeText(mes::wait);
	ui->readLine();
}