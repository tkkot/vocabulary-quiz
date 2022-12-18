#include <iostream>
#include <map>
#include <functional>

using namespace std;

typedef void (*cmd_t)(const string&);

uint8_t flags;
void f_help(const string& args){
	cout<<"HELP\n";
}
void f_lang(const string& args){
	
}
void f_exit(const string& args){
	flags &= 0B11111110;
}
void f_wrongFunc(const string& func){
	cout<<"Unkonw function: "<<func<<endl;
}

map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit} };

int32_t main(){
	ios_base::sync_with_stdio(0);
	cin.tie();
	cout.tie();

	flags |= 1;
	string line;
	cmd_t c;
	while(flags & 1){
		getline(cin, line);
		c = COMMANDS[line];
		if(c)
			c("");
		else
			f_wrongFunc(line);
	}
	
	return 0;
}

