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
	void init(){
		system("chcp 65001");
	}
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
	void init(){
		
	}
	void cls(){
		cout << "\033[2J\033[1;1H";
	}
#endif

map<string, string> longSyl = {{"ā", "a"}, {"ī", "i"}};

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

struct ansgroup{
	bool mandatory;
	vector<string> ans;
};
ostream& operator<<(ostream& stream, const ansgroup& a){
	if(!a.mandatory)
		stream<<'*';
	stream<<a.ans[0]<<", ";
	for(int i =1; i<a.ans.size(); i++)
		stream<<'|'<<a.ans[i]<<", ";
	return stream;
}

struct entry{
	char state;
	int lineNum;
	string key;
	vector<ansgroup> ans;
	const category* cat;
	
	entry(int lineNum, const string &line, const category* cat, char state = 0):
		lineNum(lineNum), cat(cat){
		
		vector<string> s = split(line, ";");
		this->key = s[1];
		trim(this->key);
		
		s = split(s[0], ",");
		for(string& i : s){
			trim(i);
			if(startsWith(i, "|"))
				this->ans.back().ans.pb(i.substr(1));
			else{
				this->ans.pb(ansgroup());
				if(startsWith(i, "*")){
					this->ans.back().mandatory=0;
					i=i.substr(1);
				}
				else
					this->ans.back().mandatory=1;
				this->ans.back().ans.pb(i);
			}
		}
	}
	
	bool check(const vector<string>& ans){
		bool c1 = 0, c2;
		int c=0;
		for(ansgroup& i : this->ans){
			c2=0;
			for(const string& a : ans){
//				cout<<a<<" : "<<i.ans<<endl;
				if(find(i.ans.begin(), i.ans.end(), a) != i.ans.end()){
					c2=1;
					c++;
					break;
				}
			}
			if(c2)
				c1 = 1;
			else if(i.mandatory){
				c1 = 0;
				break;
			}
				
		}
		if(c<ans.size())
			return 0;	
		
		return c1;		
		
	}
};
ostream& operator<<(ostream& stream, const entry& e){
	stream<<e.lineNum<<' '<<e.cat->name<<": "<<e.key<<"; ";
	for(ansgroup i : e.ans)
		stream<<i;
	return stream;
}


class set{
public:
	string name;
	vector<entry> entries;
	map<string, category> categories;
	
	set(string name) : name(name) {
		categories["default"] = {0, "default"};
	}
	
	//settings: b0 - randomize; b1 - group terms; b2 - repeat wrongs;
	void train(uint8_t settings = 0b00000000){
//		for(entry& e : entries){
//			cout<<e<<endl;
			
		if(settings & 0b00000001){
			random_shuffle(entries.begin(), entries.end());
			if(settings & 0b00000010){
				sort(entries.begin(), entries.end(), [](const entry& a, const entry& b){
					return a.cat->num<b.cat->num;
				});
			}
		}
		else
			sort(entries.begin(), entries.end(), [](const entry& a, const entry& b){
				return a.lineNum<b.lineNum;
			});
			
//		for(const entry &e : this->entries){
//			cout<<e<<endl;
//		}
			
			
		for(entry &e : this->entries){
			if(e.state == '^')
				continue;
			cout<<this->name<<endl;
			cout<<e.key<<'\n';
			bool cr = 0;
			string a;
			getline(cin, a);
			
			vector<string> ans = split(a, ",");
			for(string& i : ans)
				trim(i);
			cr = e.check(ans);
			if(cr)
				cout<<"Correct\n";
			else
				cout<<"Incorrect\n";
			cout<<e.ans<<'\n';
			do{
				getline(cin, a);
				trim(a);
				if(a == "!")
					cout<<"Correct\n";
				if(a == "!!"){
					e.state = '!';
				}
				if(a == "^")
					e.state = '^';
				
			}while(a != "");
			cls();
		}
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
	
	vector<set> *read(){
		vector<set> *sets = new vector<set>();
		sets->pb({"default"});
		string curCat;
		for(int i=0; i<lines.size(); i++){
//			clog<<i<<' '<<lines[i]<<'\n';
			if(lines[i].empty())
				continue;
			if(startsWith(lines[i], "#"))
				continue;
			if(startsWith(lines[i], "@")){
				sets->pb({trim_c(lines[i].substr(1))});
				continue;
			}
			if(startsWith(lines[i], "$")){
				curCat = trim_c(lines[i].substr(1));
				sets->back().categories[curCat] = {(int)sets->back().categories.size(), curCat};
				continue;
			}
			if(startsWith(lines[i], "^")){
				sets->back().entries.pb( {i, lines[i].substr(1), &(sets->back().categories[curCat]), '^' } );
				continue;
			}
			sets->back().entries.pb( {i, lines[i], &(sets->back().categories[curCat]) } );
		}
		
		return sets;
	}
};

int32_t main(){
	init();
	
	string path;
	getline(cin, path);
	sourcefile sf(path);
	
	
//	cout<<sf.lines.size()<<endl;
//	for(string line : sf.lines){
//		cout<<line<<'\n';
//	}
//	cout<<"************************************************************************\n";
	
	vector<set> &sets = *sf.read();
	
	for(set& s : sets){
//		cout<<s.name<<endl;
//		for(entry& e : s.entries){
//			cout<<e<<endl;
//		}
		s.train(0);
	}

	return 0;
}

