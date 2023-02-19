#include "functions.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>

#define __MES_TRAIN__	//Only messages used in this file will be loaded
#include "UI/messages.h"

#define pb push_back	//vector macro

using namespace std;

//Defining functions declared in "types.h" and "functions.h"

/* Constructor for entry
 *
 * lineNum - number of line in sourcefile, used when writing
 * line - raw text input from sourcefile
 * cat - entry's category
 * state - state to set for entry (read from sourcefile)
 */
entry::entry(int lineNum, const string &line, const category* cat, char state):
	lineNum(lineNum), cat(cat), state(state){

	vector<string> s = split(line, ";");	//Split line into two halves by ';' separator
	if(s.size() != 2) {	//Line should be split into two parts, if not there is a formatting mistake
		cout<<mes::err_line[0]<<lineNum<<mes::err_line[1]<<line<<'\n';
		return;
	}
	this->key = s[1];	//The second half of the line is the key (word / phrase asked to the user)
	trim(this->key);

	s = split(s[0], ",");	//The first half of the line is the accepted answers, which are aplit by ','
	for(string& i : s){
		trim(i);
		//Testing for answer types
		if(startsWith(i, "|"))	//If answer alternative to previous, add it to previous ansgroup
			this->ans.back().ans.pb(i.substr(1));
		else{	//Else create new ansgroup
			this->ans.pb(ansgroup());
			//Set ansgroup type
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
			this->ans.back().ans.pb(i);	//Add cyrrent answer to newly made ansgroup
		}
	}
}

/*
 * Check whether answer ans matches this entry
 * Answer string should contain every mandatory (default) answer and no wrong (not matching) answers
 * 
 */
bool entry::check(const vector<string>& ans){
	bool c1 = 0, c2;
	int c=0;	//counts how many strings in ans match a correct answer in the entry
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


/*
 * Constructor for set
 */
set::set(string name) : name(name) {
	categories["default"] = {0, "default"};
}

bool processEntry(entry& e, queue<entry*> &wrng, string &name){
	if(e.state == '^')
		return 0;
	
	//Ask question and request answer
	cout<<e.cat->name<<"\n\n";
	cout<<e.key<<'\n';
	bool cr = 0;
	getline(cin, line);

	//Process answer
	vector<string> ans = split(line, ",");
	for(string& i : ans)
		trim(i);
	cr = e.check(ans);
	
	//Respond to user
	cout<<(cr ? mes::cor : mes::inc);
	for(ansgroup i : e.ans)
		cout<<i;
	cout<<'\n';
	
	//Ask for override
	do{
		cout<<mes::ovr;
		getline(cin, line);
		trim(line);
		if(line == "!"){
			cr = !cr;
			cout<<mes::ovrd<<(cr ? mes::cor : mes::inc);
		}
		if(line == "!!")
			e.state = '!';
		if(line == "^")
			e.state = '^';

	}while(line != "");
	if (!cr)
		wrng.push(&e);
	cls();
	cout<<"\t"<<name<<"\n\n";
	return cr;
}

/*
 * Train this set.
 * settings:
 *	b0 - randomize - should entries be randomized?
 *	b1 - group entries - should entries from the same category be grouped together (after being randomized)?
 * 	b2 - repeat wrongs - should wrong answers be repeated after training ends?
 */
void set::train(uint8_t settings){
	cls();

	//Count trainable entries in set
	activeEntries = 0;
	for(entry& e : entries){
		if(e.state != '^')
			activeEntries++;
	}
	if(activeEntries == 0){
		cout<<mes::no_ent[0]<<name<<mes::no_ent[1];
		return;		//TODO flags
	}

	//Ask user whether to train this set
	cout<<mes::q_train[0]<<name<<mes::q_train[1];	//Train set [name]? y/N\n

	getline(cin, line);
	trim(line);
	if(line != "y" & line!="Y")
		return;

	cls();
	cout<<"\t"<<name<<"\n\n";
	int correct = 0;

	//sorting/randomizing and grouping entries
	if(settings & 1){	//Randomize
		random_shuffle(entries.begin(), entries.end());
		if(settings & 2){	//Group terms
			sort(entries.begin(), entries.end(), [](const entry& a, const entry& b){
				return a.cat->num<b.cat->num;
			});
		}
	}
	else
		sort(entries.begin(), entries.end(), [](const entry& a, const entry& b){
			return a.lineNum<b.lineNum;
		});

	queue<entry*> wrng;

	//TRAINING
	for(entry &e : this->entries){
		correct+=processEntry(e, wrng, name);
	}

	if(settings & 4){	//repeat wrongs
		entry* e;
		while(!wrng.empty()){
			e=wrng.front();
			processEntry(*e, wrng, name);
			wrng.pop();
		}
	}

	cout<<mes::set_end;
	cout<<correct<<" / "<<activeEntries<<" : "<<(activeEntries ? to_string(100 * correct / activeEntries) : "~")<<"%\n";	//Calculating score on set
	UIwait();
	cls();
}

///Constructor for sourcefile
sourcefile::sourcefile(string path) : path(path) {}

/*
 * Read this sourcefile and parse sets
 */
uint8_t sourcefile::read(){
	string s;
	//Open file
	ifstream is(this->path);
	if(!is)
		return 1;
	//Read and close file
	while(getline(is, s)){
		trim(s);
		lines.pb(s);
	}
	is.close();

	//PARSE FILE
	this->sets.pb({"default"});	//Create default set		TODO: only if not specufied
	string curCat;
	for(int i=0; i<lines.size(); i++){
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

/*
 * Update raw sourcefile with data from program
 */
void sourcefile::update(){
	for(const set& set : this->sets){
		for(const entry& e : set.entries){
			if(lines[e.lineNum][0] != e.state && e.state>0)
				lines[e.lineNum] = e.state + lines[e.lineNum];
		}
	}
}

/*
 * Write raw source to file
 */
uint8_t sourcefile::write(){
	ofstream os(this->path);
	if(!os){
		return 1;
	}
	for(const string& line : lines){
		os<<line<<'\n';
	}
	os.close();
	return 0;
}
