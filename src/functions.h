#pragma once

#include "types.h"

//Output
//Util stuff - streaming containers and structs
std::ostream& operator<<(std::ostream& stream, const entry& e);
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vect);

/**
 *  UI command interface:
 *      each UI implementation should... well implement these
 */
class UI{
public:
//Temporary UI stuff, in lieu of proper interface
   virtual void writeText(const std::string& text)=0;
   virtual void writeText(std::initializer_list<std::string> text)=0;
   virtual std::string readLine()=0;
   virtual void cls()=0;   //clear screen
};

extern UI &ui;

//Globally accessible UI functions
void UIwait();						//Wait until user presses ENTER
void UIstart();						//Start UI loop
int UIfunction(const std::vector<std::string>& args);	//Execute UI function args[0] with arguments args
extern std::string line;

//console
uint8_t parse(const std::vector<std::string> &args);

namespace par{
extern std::vector<bool> flags;//	[T, f, l, s, E]

extern uint8_t s; //settings: b0 - randomize; b1 - group terms; b2 - repeat wrongs;
extern std::string l;
extern std::string f;
}

//Text functions
void trim(std::string &s);
std::string trim_c(std::string s);
bool startsWith(const std::string& s, const std::string& prefix);
std::vector<std::string> split(std::string s, const std::string& c);


//training
extern sourcefile *sf;
