#pragma once

#include "types.h"
#include <set>
//------------------UI---------------------

/**
 *  UI command interface:
 *      each UI implementation should... well implement these
 */
class UI{
public:

//Functions to be implemented
//Temporary UI stuff, in lieu of proper interface
   virtual void init()=0;
   virtual void start()=0;
   virtual void writeText(const std::string& text)=0;
   virtual void writeText(std::initializer_list<std::string> text)=0;
   //As opposed to writeText(), which should just write given text in active location, this function's behaviour may differ depending on what the program currently is doing.
   virtual UI& operator<<(const std::string &test)=0;
   virtual std::string readLine()=0;
   virtual void cls()=0;   //clear screen
};
extern UI *ui;

//To be deprecated
void UIwait();						                        //Wait until user presses ENTER
void UIstart();						                     //Start UI loop


//--------------training-------------------
// extern sourcefile *sf;

//-------------console args----------------
uint8_t parse(const std::vector<std::string> &args);

namespace par{
extern std::vector<bool> flags;//	[T, f, l, s, E]

extern uint8_t s; //settings: b0 - randomize; b1 - group terms; b2 - repeat wrongs;
extern std::string l;
extern std::string f;
}


//-----------data management---------------
extern std::map<const std::string, set*> loadedSets;
extern std::set<std::string> loadableSets;

uint8_t getSetList();
uint8_t getSetList(const std::string &location);

uint8_t loadSet(const std::string &setname);
uint8_t saveSet(const set *set);

uint8_t importSets(const std::string &path);

//----------------util---------------------
//Text functions
void trim(std::string &s);
std::string trim_c(std::string s);
bool startsWith(const std::string& s, const std::string& prefix);
std::vector<std::string> split(std::string s, const std::string& c);

//Output - streaming containers and structs
std::ostream& operator<<(std::ostream& stream, const entry& e);
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vect);
