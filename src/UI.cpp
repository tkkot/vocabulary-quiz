#include <iostream>
#include <map>
#include <functional>

#include "util.h"

#include "messages.h"

using namespace std;

typedef void (*cmd_t)(const string[]);

uint8_t flags;

/// Program util functions
void f_help(const string args[]){
	cout<<mes::help;
}
void f_lang(const string args[]){
	cout<<mes::lang_0;
}
void f_exit(const string args[]){
	flags &= 0B11111110;
}
///Errors
void f_wrongFunc(const string func[]){
	cout<<mes::err_com_0<<func<<mes::err_com_1<<endl;
}
void f_argError(const string data[]){
	cout<<mes::err_arg_0<<data[0]<<mes::err_arg_1<<endl;
}
void f_noargError(const string data[]){
	cout<<"Error, command "<<data[0]<<" requires arguments\n";		///TEMP
}
/// App functions
void f_import(const string args[]){
	vector<string> a = split(args, " ");
	if(a.size()<2)
		f_noargError("import");
	//quiz.readFile(a[1])
}

map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit}, {"i", f_import} };

int32_t main(){
	ios_base::sync_with_stdio(0);
	cin.tie();
	cout.tie();

	cout<<mes::welcome;

	flags |= 1;
	string line;
	cmd_t c;
	while(flags & 1){
		getline(cin, line);
		trim(line);
		c = COMMANDS[split(line, " ")[0]];
		if(c)
			c({line});
		else
			f_wrongFunc(line);
	}

	cout<<mes::exit;
	return 0;
}
