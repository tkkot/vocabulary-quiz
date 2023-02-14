#pragma once

#include "types.h"

//Output
std::ostream& operator<<(std::ostream& stream, const ansgroup& a);
std::ostream& operator<<(std::ostream& stream, const entry& e);
template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vect);

//UI
void UIstart();
int UIfunction(const std::vector<std::string>& args);

//util
void cls();

void trim(std::string &s);
std::string trim_c(std::string s);

bool startsWith(const std::string& s, const std::string& prefix);

std::vector<std::string> split(std::string s, const std::string& c);


//training
//extern std::vector<set> *sets;
//extern std::string filepath;
extern sourcefile *sf;
