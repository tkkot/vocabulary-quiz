#include "functions.h"
#include <iostream>
#include <vector>
#include <algorithm>

//Platform-specific functions
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>

	//Cleaning screen on windows (code from the internet somewhere)
	void cls(){
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
	void cls(){
		std::cout << "\033[2J\033[1;1H";
		std::cout << "=============================" << "\n\n";
	}
#endif

using namespace std;	//Must be included AFTER windows.h, or it messes up (conflict between std::byte and definition of byte in header)

///Outputting vectors with stream
template<typename T>
ostream& operator<<(ostream& stream, const vector<T>& vect){
	for(const T& i : vect){
		stream<<i;
		stream<<", ";
	}
	return stream;
}

//Remove whitespaces from beginning and end of string s
void trim(string &s){
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
//Same as trim, but copies string rather than referencing
string trim_c(string s){
	trim(s);
	return s;
}

//Checks if string starts with prefix
bool startsWith(const string& s, const string& prefix){
	return prefix == s.substr(0, prefix.size());
}

//Split string into vector of strings by c
vector<string> split(string s, const string& c){
	int i=0;
	vector<string> v;
	while( (i = s.find(c)) != string::npos){
		v.push_back(s.substr(0, i));
		s.erase(0, i + c.size());
	}
	v.push_back(s);
	return v;
}

//For streaming ansgroups
ostream& operator<<(ostream& stream, const ansgroup& a){
	if(a.type == '*')
		stream<<'*';
	else if(a.type == '-')
		stream<<'-';
	stream<<a.ans[0]<<", ";
	for(int i =1; i<a.ans.size(); i++)
		stream<<'|'<<a.ans[i]<<", ";
	return stream;
}
//For streaming entries
ostream& operator<<(ostream& stream, const entry& e){
	stream<<e.lineNum<<' '<<e.cat->name<<": "<<e.key<<"; ";
	for(ansgroup i : e.ans)
		stream<<i;
	return stream;
}
