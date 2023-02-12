#pragma once
#ifndef vector
	#include <vector>
#endif
#ifndef string
	#include <string>
#endif

void cls();

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vect);

void trim(std::string &s);
std::string trim_c(std::string s);

bool startsWith(const std::string& s, const std::string& prefix);

std::vector<std::string> split(std::string s, const std::string& c);
