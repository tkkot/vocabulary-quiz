#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include "../functions.h"

#include "messages.h"

using namespace std;

typedef void (*cmd_t)(const vector<string>&);

uint8_t flags;
string line;

/// Program util functions
void f_help(const vector<string>& args){
	cout<<mes::help;
}
void f_lang(const vector<string>& args){
	cout<<mes::lang_0;
}
void f_exit(const vector<string>& args){
	flags &= 0B11111110;
}
///Errors
void f_wrongFunc(const vector<string>& func){
	cout<<mes::err_com_0<<func[0]<<mes::err_com_1<<endl;
}
void f_argError(const vector<string>& data){
	cout<<mes::err_arg_0<<data[0]<<mes::err_arg_1<<endl;
}
void f_noargError(const vector<string>& data){
	cout<<"Error, command "<<data[0]<<" requires arguments\n";		//TODO
}
/// App functions
void f_import(const vector<string>& args){

	if(args.size()>1)
		sf = new sourcefile(args[1]);
	else{
		cout<<"Enter path to training file:\n"; //TODO
		getline(cin, line);
		trim(line);
		sf = new sourcefile(line);
	}

	if(!sf->read()){
		cout<<"File not found: \n"<<sf->path<<"\n";
		cls();
		return;
	}
	return;
}
void f_train(const vector<string>& args){

	if (!sf | args.size()>1){
		f_import(args);
	}

	for(set &s : sf->sets)
		s.train(3);	//settings as parameter
	sf->update();	//dynamic updating of data?
	sf->write();
}

map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit}, {"i", f_import}, {"t", f_train} };


int UIfunction(const vector<string> &args){

	for (const string &i : args)
		cout<<i<<" ";
	cout<<endl;

	if(args.size() == 0)
		return 1;
	cmd_t c = COMMANDS[args[0]];
	if(c)
		c(args);
	else
		f_wrongFunc(args);
	return 0;
}

///Start UI loop
void UIstart(){
	cls();
	cout<<mes::welcome;

	flags |= 1;
	cmd_t c;
	while(flags & 1){
		getline(cin, line);
		trim(line);
		UIfunction(split(line, " "));
	}

	cout<<mes::exit;
}
