#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include "../functions.h"

#define __MES_UI__
#include "messages.h"

using namespace std;


typedef void (*cmd_t)(const vector<string>&);	//Funcion pointers to executed commands
typedef const vector<string> &arg_t;		//Macro for command arguments

uint8_t flags;	//Program flags (for now only [0]=running)
string line;	//Container for user input from std::cin

// Program util functions
///Display help message
void f_help(arg_t args){
	cout<<mes::help;
}
///Choose language (for now not working)
void f_lang(arg_t args){
	cout<<mes::lang;
}
///Call the clean screen function
void f_clear(arg_t args){
	cls();
}
///Set running flag to 0, exit program
void f_exit(arg_t args){
	flags &= 0B11111110;
}
//Errors
///Display error message for unknown command
void f_wrongFunc(arg_t func){
	cout<<mes::err_com[0]<<func[0]<<mes::err_com[1]<<endl;
}
///Display error message for wrong arguments for function (or lack of them)
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
// App functions
///Open sourcefile and import sets from it
void f_import(arg_t args){
	if(args.size()>1){
		sf = new sourcefile(args[1]);
	}
	else{
		cout<<mes::import[0];
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
	UIwait();
	cls();
	return;
}
///Train current sets
void f_train(arg_t args){

	if (!sf | args.size()>1)
		f_import(args);
	if(!sf)
		return;

	cout<<mes::start_sf[0]<<sf->path<<mes::start_sf[1];
	for(int i=0; i<sf->sets.size(); i++)
		cout<<i<<". "<<sf->sets[i].name<<"\n";
	UIwait();

	for(set &s : sf->sets)
		s.train(7);	//settings as parameter
	cout<<mes::fin;

	sf->update();	//dynamic updating of data?
	if(sf->write())
		cout<<mes::err_fnf<<sf->path<<"\n";
	UIwait();
	cls();
}
void f_settings(arg_t args){
	if(args.size()<2){	//TODO ask user for specifying settings
		f_argError({"s"});
		return;
	}
	//TODO
}

//Map assigning shortcuts to commands, used to interpret user input
map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit}, {"i", f_import}, {"t", f_train}, {"c", f_clear}, {"s", f_settings} };

//Wait until user presses ENTER
void UIwait(){
	cout<<mes::wait;
	getline(cin, line);
}

/*
 * Execute function args[0] with parameter args
 */
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
