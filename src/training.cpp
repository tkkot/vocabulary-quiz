#include "functions.h"
#include <iostream>
#include <algorithm>
#include <fstream>

#define __MES_TRAIN__
#include "UI/messages.h"

#define pb push_back

using namespace std;

entry::entry(int lineNum, const string &line, const category* cat, char state):
	lineNum(lineNum), cat(cat), state(state){

	vector<string> s = split(line, ";");
	if(s.size() != 2) {
		cout<<mes::err_line[0]<<lineNum<<mes::err_line[1]<<line<<'\n';	//TODO move handling to UI
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

bool entry::check(const vector<string>& ans){
	bool c1 = 0, c2;
	int c=0;
	for(ansgroup& i : this->ans){
		c2=0;
		for(const string& a : ans){
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



set::set(string name) : name(name) {
	categories["default"] = {0, "default"};
}

//settings: b0 - randomize; b1 - group terms; b2 - repeat wrongs;
void set::train(uint8_t settings){
	string a;	//Placeholder variable for user input

	cls();

	activeEntries = 0;
	for(entry& e : entries){
		if(e.state != '^')
			activeEntries++;
	}

	if(activeEntries == 0){
		cout<<mes::no_ent[0]<<name<<mes::no_ent[1];
		return;		//TODO flags
	}

	cout<<mes::q_train[0]<<name<<mes::q_train[1];	//Train set [name]? y/N\n

	getline(cin, a);
	trim(a);
	if(a != "y" & a!="Y")
		return;

	cls();
	cout<<"\t"<<name<<"\n\n";
	int correct = 0;

	//sorting and grouping entries
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
		cout<<e.cat->name<<"\n\n";
		cout<<e.key<<'\n';
		bool cr = 0;
		getline(cin, a);

		vector<string> ans = split(a, ",");
		for(string& i : ans)
			trim(i);
		cr = e.check(ans);
		cout<<(cr ? mes::cor : mes::inc);
		for(ansgroup i : e.ans){
			cout<<i;
		}
		cout<<'\n';
		do{
			cout<<mes::ovr;
			getline(cin, a);
			trim(a);
			if(a == "!"){
				cr = !cr;
				cout<<mes::ovrd<<(cr ? mes::cor : mes::inc);
			}
			if(a == "!!"){
				e.state = '!';
			}
			if(a == "^")
				e.state = '^';

		}while(a != "");
		correct += cr;
		cls();
		cout<<"\t"<<name<<"\n\n";
	}
	cout<<mes::set_end;
	cout<<correct<<" / "<<activeEntries<<" : "<<(activeEntries ? to_string(100 * correct / activeEntries) : "~")<<"%\n";
	UIwait();
	cls();
}


sourcefile::sourcefile(string path) : path(path) {}

uint8_t sourcefile::read(){
	string s;
	ifstream is(this->path);
	if(!is)
		return 1;
	while(getline(is, s)){
		trim(s);
		lines.pb(s);
	}
	is.close();

	this->sets.pb({"default"});
	string curCat;
	for(int i=0; i<lines.size(); i++){
//			clog<<i<<' '<<lines[i]<<'\n';
		if(lines[i].empty())
			continue;
		if(startsWith(lines[i], "#"))
			continue;
		if(startsWith(lines[i], "@")){
			this->sets.pb({trim_c(lines[i].substr(1))});
			continue;
		}
		if(startsWith(lines[i], "$")){
			curCat = trim_c(lines[i].substr(1));
			this->sets.back().categories[curCat] = {(int)this->sets.back().categories.size(), curCat};
			continue;
		}
		if(startsWith(lines[i], "^")){
			this->sets.back().entries.pb( {i, lines[i].substr(1), &(this->sets.back().categories[curCat]), '^' } );
			continue;
		}
		if(startsWith(lines[i], "!")){
			this->sets.back().entries.pb( {i, lines[i].substr(1), &(this->sets.back().categories[curCat]), '!'} );
			continue;
		}
		this->sets.back().entries.pb( {i, lines[i], &(this->sets.back().categories[curCat]) } );
	}
	return 0;
}

void sourcefile::update(){
	for(const set& set : this->sets){
		for(const entry& e : set.entries){
			if(lines[e.lineNum][0] != e.state && e.state>0)
				lines[e.lineNum] = e.state + lines[e.lineNum];
		}
	}
}

uint8_t sourcefile::write(){
	ofstream os(this->path);
	if(!os){
		return 1;
	}
	for(const string& line : lines){
		os<<line<<'\n';
//		cout<<line<<'\n';
	}
	os.close();
	return 0;
}
