#include <iostream>
#include <fstream>
#include <vector>
#define pb push_back
#include <algorithm>
#include <cctype>
#include <map>

#ifdef WIN32
	#include <windows.h>
	
	void init(){
//		system("chcp 65001");
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
		std::cout << "\033[2J\033[1;1H";
	}
#endif

using namespace std;

//map<string, string> longSyl = {{"ā", "a"}, {"ī", "i"}};

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
	uint8_t type;	// 0 - mandatory, '*' - not-mandatory, '-' - ending
	vector<string> ans;	 
};
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

struct entry{
	char state;
	int lineNum;
	string key;
	vector<ansgroup> ans;
	const category* cat;
	
	entry(int lineNum, const string &line, const category* cat, char state = 0):
		lineNum(lineNum), cat(cat), state(state){
		
		vector<string> s = split(line, ";");
		if(s.size() != 2) {
			cerr<<"Line "<<lineNum<<" is not formatted correctly:\n"<<line<<'\n';
			return;
		}
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
					this->ans.back().type='*';
					i=i.substr(1);
				}
				else if(startsWith(i, "-")){
					this->ans.back().type='-';
					i=i.substr(1);
				}
				else
					this->ans.back().type=0;
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
			else if(i.type == 0){
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
		int correct = 0;
//		for(entry& e : entries){
//			cout<<e<<endl;

		if(entries.empty())
			return;	
			
		cout<<"Train set "<<name<<"? y/n\n";
		string a;
		getline(cin, a);
		trim(a);
		
		if(a == "y" || a=="Y")
			goto __START_TRAINING__;
		return;
	
	__START_TRAINING__:
		cls();
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
			cout<<e.cat->name<<endl;
			cout<<e.key<<'\n';
			bool cr = 0;
			getline(cin, a);
			
			vector<string> ans = split(a, ",");
			for(string& i : ans)
				trim(i);
			cr = e.check(ans);
			cout<<(cr ? "Correct\n" : "Incorrect\n");
			for(ansgroup i : e.ans){
				cout<<i;
			}
			cout<<'\n';
			do{
				getline(cin, a);
				trim(a);
				if(a == "!"){
					cr = !cr;
					cout<<(cr ? "Correct\n" : "Incorrect\n");
				}
				if(a == "!!"){
					e.state = '!';
				}
				if(a == "^")
					e.state = '^';
				
			}while(a != "");
			correct += cr;
			cls();
		}
		cout<<correct<<" / "<<entries.size()<<" : "<<(entries.size() ? to_string(100 * correct / entries.size()) : "~")<<"%\n";
	}
	
};

class sourcefile{
public:
	vector<string> lines;
	string path;

	sourcefile(string path) : path(path) {}
	
	vector<set> *read(){
		string s;
		ifstream is(path);
		if(!is)
			return nullptr;
		while(getline(is, s)){
			trim(s);
			lines.pb(s);
		}
		is.close();
	
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
			if(startsWith(lines[i], "!")){
				sets->back().entries.pb( {i, lines[i].substr(1), &(sets->back().categories[curCat]), '!'} );
				continue;
			}
			sets->back().entries.pb( {i, lines[i], &(sets->back().categories[curCat]) } );
		}
		cout<<"File read successfully\nPress ENTER to continue\n";
		getline(cin, curCat);
		cls();
		
		return sets;
	}
	
	void update(const vector<set>& sets){
		for(const set& set : sets){
			for(const entry& e : set.entries){
				if(lines[e.lineNum][0] != e.state && e.state>0)
					lines[e.lineNum] = e.state + lines[e.lineNum];
			}
		}
	}
	
	void write(){
		ofstream os(path);
		if(!os){
			cerr<<"File not found\n"<<path;
			return;
		}
		for(const string& line : lines){
			os<<line<<'\n';
			cout<<line<<'\n';
		}
		os.close();
	}
};

int32_t main(){
	init();
	
	string path;
	cout<<"Enter file path:\n";
	getline(cin, path);
//	sourcefile sf("C:\\Users\\Tobiasz\\OneDrive\\Szko�a\\test.txt");
	sourcefile sf(path);
	
//	cout<<sf.lines.size()<<endl;
//	for(string line : sf.lines){
//		cout<<line<<'\n';
//	}
//	cout<<"************************************************************************\n";
	
	vector<set> &sets = *sf.read();
	
	if(!&sets){
		cerr<<"File not found\n"<<path;
		getline(cin, path);
		return 2;
	}
	
	cout<<"List of training sets if sourcefile:\n";
	
	for(int i = 0; i<sets.size(); i++){
		cout<<i+1<<". "<<sets[i].name<<endl;
	}
	
	for(set& s : sets){
//		cout<<s.name<<endl;
//		for(entry& e : s.entries){
//			cout<<e<<endl;
//		}
		s.train(0);
	}
	
	sf.update(sets);
//	sf.write();
	
	delete &sets;

	return 0;
}

