#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include "../functions.h"

#include "messages.h"

using namespace std;

typedef void (*cmd_t)(const vector<string>&);
typedef const vector<string> &arg_t;

uint8_t flags;
string line;

/// Program util functions
void f_help(arg_t args){
	cout<<mes::help;
}
void f_lang(arg_t args){
	cout<<mes::lang;
}
void f_clear(arg_t args){
	cls();
}
void f_exit(arg_t args){
	flags &= 0B11111110;
}
///Errors
void f_wrongFunc(arg_t& func){
	cout<<mes::err_com[0]<<func[0]<<mes::err_com[1]<<endl;
}
void f_argError(arg_t data){
	if(data.size()>1){
		cout<<mes::err_arg[0];
		for(auto i = data.begin()+1; i<data.end(); i++)
			cout<<*i<<" ";
		cout<<mes::err_arg[1]<<data[0]<<mes::err_arg[4];
	}
	else
		cout<<mes::err_arg[2]<<data[0]<<mes::err_arg[3]<<mes::err_arg[4];
}
/// App functions
void f_import(arg_t args){
	for (const string& i : args)
		cout<<i<<" ";
	cout<<endl;
	if(args.size()>1)
		sf = new sourcefile(args[1]);
	else{
		cout<<mes::import[0]; //TODO
		getline(cin, line);
		trim(line);
		sf = new sourcefile(line);
	}

	if(sf->read()){
		cout<<mes::import[2]<<sf->path<<"\n";
		delete sf;
		sf = nullptr;
		return;
	}
	cout<<mes::import[1];
	return;
}
void f_train(arg_t args){

	if (!sf | args.size()>1)
		f_import(args);
	if(!sf)
		return;

	for(set &s : sf->sets)
		s.train(3);	//settings as parameter
	sf->update();	//dynamic updating of data?
	if(sf->write())
		cout<<mes::err_fnf<<sf->path<<"\n";
}

map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit}, {"i", f_import}, {"t", f_train}, {"c", f_clear} };


int UIfunction(const vector<string> &args){

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
		if (line.size()>0)
			UIfunction(split(line, " "));
	}

	cout<<mes::exit;
}