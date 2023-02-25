#pragma once

#include "types.h"

//Output
//Util stuff - streaming containers and structs
std::ostream& operator<<(std::ostream& stream, const ansgroup& a);
std::ostream& operator<<(std::ostream& stream, const entry& e);
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vect);

//Globally accessible UI functions
void UIwait();						//Wait until user presses ENTER
void UIstart();						//Start UI loop
int UIfunction(const std::vector<std::string>& args);	//Execute UI function args[0] with arguments args

//util
void cls();	//clear screen

//Text functions
void trim(std::string &s);
std::string trim_c(std::string s);
bool startsWith(const std::string& s, const std::string& prefix);
std::vector<std::string> split(std::string s, const std::string& c);


//training
extern sourcefile *sf;
