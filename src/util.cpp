#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#ifdef WIN32
	#include <windows.h>

	void cls(){ 
	    COORD tl = {0,0};
	    CONSOLE_SCREEN_BUFFER_INFO s;
	    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);   
	    GetConsoleScreenBufferInfo(console, &s);
	    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	    FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
	    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	    SetConsoleCursorPosition(console, tl);
	}
#else
	void cls(){
		cout << "\033[2J\033[1;1H";
	}
#endif

template<typename T>
ostream& operator<<(ostream& stream, const vector<T>& vect){
	for(const T& i : vect){
		stream<<i;
		stream<<", ";
	}
	return stream;
}

void trim(string &s){
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
string trim_c(string s){
	trim(s);
	return s;
}

bool startsWith(const string& s, const string& prefix){
	return prefix == s.substr(0, prefix.size());
}

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
