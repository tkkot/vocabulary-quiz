#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include "../functions.h"

#include "messages.h"

using namespace std;

typedef void (*cmd_t)(const vector<string>&);

uint8_t flags;

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
	vector<string> a = split(args[0], " ");
	if(a.size()<2)
		f_noargError({"import"});
	//quiz.readFile(a[1])
}

map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit}, {"i", f_import} };


//Asks user for train file path and return said path
string askPath(){
	string path, line;
	cout<<"Enter path to training file:\n"; //TODO
	getline(cin, line);
	trim(line);
	return line;
}


///Start UI loop
void UIstart(){

	cout<<mes::welcome;

	flags |= 1;
	string line;
	cmd_t c;
	while(flags & 1){
		getline(cin, line);
		trim(line);
		c = COMMANDS[split(line, " ")[0]];
		if(c)
			c(split(line, " "));
		else
			f_wrongFunc(split(line, " "));
	}

	cout<<mes::exit;
}
