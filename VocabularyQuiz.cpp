#include <iostream>
#include <fstream>
#include <vector>
#define pb push_back
#include <algorithm>
#include <cctype>
#include <map>

using namespace std;

#ifdef _WIN32
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
		v.pb(s.substr(0, i));
		s.erase(0, i + c.size());
	}
	v.pb(s);
	return v;
}


struct category{
	int num;
	string name;
};

struct entry{
	int line;
	string key;
	vector<string> ans;
	category cat;
	
	entry(int line, string key, vector<string> a, category cat):
	line(line), key(key), ans(a), cat(cat){
		for(int i=0; i<ans.size(); i++){
			trim(ans[i]);
		}	
	}
	
};

class set{
public:
	string name;
	vector<entry> entries;
	map<string, category> categories;
	
	set(string name) : name(name) {
		categories["default"] = {0, "default"};
	}
};

class sourcefile{
public:
	vector<string> lines;

	sourcefile(string path){
		string s;
		ifstream is(path);
		if(!is){
			cerr<<"File not found\n";
			return;
		}
		while(getline(is, s)){
			trim(s);
			lines.pb(s);
		}
		is.close();
	}
	
	vector<set> read(){
		vector<set> sets;
		sets.pb({"default"});
		string curCat;
		for(int i=0; i<lines.size(); i++){
//			clog<<i<<' '<<lines[i]<<'\n';
			if(lines[i].empty())
				continue;
			if(startsWith(lines[i], "#"))
				continue;
			if(startsWith(lines[i], "@")){
				sets.pb({trim_c(lines[i].substr(1))});
				continue;
			}
			if(startsWith(lines[i], "$")){
				curCat = trim_c(lines[i].substr(1));
				sets.back().categories[curCat] = {sets.back().categories.size(), curCat};
				continue;
			}
			vector<string> line = split(lines[i], ";");
//			cout<<'\t';
//			for(string s : line)
//				cout<<s<<';';
//			cout<<"T"<<endl;
			sets.back().entries.pb( {i, trim_c(line[1]), split(line[0], ","), sets.back().categories[curCat]} );
		}
		
		return sets;
		
	}
	
};

int32_t main(){
	string path;

	sourcefile sf("Latin words.txt");
	
//	cout<<sf.lines.size()<<endl;
//	for(string line : sf.lines){
//		cout<<line<<'\n';
//	}
//	cout<<"************************************************************************\n";
	
	vector<set> sets = sf.read();
	
	for(set s : sets){
		cout<<s.name<<endl;
		for(entry e : s.entries){
			cout<<e.line<<' '<<e.cat.name<<'\t'<<e.key<<";\t";
			for(string ans : e.ans){
				cout<<ans<<",\t";
			}
			cout<<'\n';
		}
	}

	return 0;
}

