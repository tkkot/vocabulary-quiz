#include <iostream>
#include <map>
#include <vector>
#include <functional>

#include <functions.h>
#include "consoleUI.h"

#define __MES_UI__
#include "messages.h"


//Platform-specific functions
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>

	//Cleaning screen on windows (code from the internet somewhere)
	void consoleUI::cls(){
		COORD tl = {0,0};
		CONSOLE_SCREEN_BUFFER_INFO s;
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(console, &s);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;
		FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
		FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
		SetConsoleCursorPosition(console, tl);
		std::cout << "=============================" << "\n\n";
	}
#else
	//Cleaning terminal (Also  from internet)
	void consoleUI::cls(){
		std::cout << "\033[2J\033[1;1H";
		std::cout << "=============================" << "\n\n";
	}
#endif

using namespace std;

void consoleUI::writeText(const string& text){
	cout<<text;
}
void consoleUI::writeText(std::initializer_list<std::string> text){
	for (string i : text){
		ui.writeText(i);
	}
};
string consoleUI::readLine(){
	string line;
	getline(cin, line);
	return line;
}


typedef void (*cmd_t)(const vector<string>&);	//Funcion pointers to executed commands
typedef const vector<string> &arg_t;		//Macro for command arguments

uint8_t flags;	//Program flags (for now only [0]=running)
string line;	//Container for user input from std::cin


// Program util functions
///Display help message
void f_help(arg_t args){
	ui.cls();
	ui.writeText(mes::help);
}
///Choose language (for now not working)
void f_lang(arg_t args){
	ui.writeText(mes::lang);
}
///Call the clean screen function
void f_clear(arg_t args){
	ui.cls();
}
///Set running flag to 0, exit program
void f_exit(arg_t args){
	flags &= 0B11111110;
}
//Errors
///Display error message for unknown command
void f_wrongFunc(arg_t& func){
	ui.writeText({mes::err_com[0], func[0], mes::err_com[1], "\n"});
}
///Display error message for wrong arguments for function (or lack of them)
void f_argError(arg_t data){
	if(data.size()>1){
		ui.writeText(mes::err_arg[0]);
		for(auto i = data.begin()+1; i<data.end(); i++)
			ui.writeText({*i, " "});
		ui.writeText({mes::err_arg[1], data[0], mes::err_arg[4]});
	}
	else
		ui.writeText({mes::err_arg[2], data[0], mes::err_arg[3], mes::err_arg[4]});
}
// App functions
///Open sourcefile and import sets from it
void f_import(arg_t args){
	if(args.size()>1){
		sf = new sourcefile(args[1]);
	}
	else{
		ui.writeText(mes::import[0]);
		line = ui.readLine();
		trim(line);
		sf = new sourcefile(line);
	}

	if(sf->read()){
		ui.writeText({mes::import[2], sf->path, "\n"});
		delete sf;
		sf = nullptr;
		return;
	}
	ui.writeText(mes::import[1]);
	UIwait();
	ui.cls();
	return;
}
///Train current sets
void f_train(arg_t args){

	if (!sf | args.size()>1)
		f_import(args);
	if(!sf)
		return;

	ui.writeText({mes::start_sf[0], sf->path, mes::start_sf[1]});
	for(int i=0; i<sf->sets.size(); i++)
		ui.writeText({to_string(i), ". ", sf->sets[i].name, "\n"});
	UIwait();

	for(set &s : sf->sets)
		s.train(par::s);	//settings as parameter
	ui.writeText(mes::fin);

	sf->update();	//dynamic updating of data?
	if(sf->write())
		ui.writeText({mes::err_fnf, sf->path, "\n"});
	UIwait();
	ui.cls();
}
void f_settings(arg_t args){
	if(args.size()<2){	//TODO ask user for specifying settings
		f_argError({"s"});
		return;
	}
	par::s = stoi(args[1], nullptr, 2);
	cout<<"New settings: "<<0+par::s<<"\n";
	return;
}

//Map assigning shortcuts to commands, used to interpret user input
map<string, cmd_t> COMMANDS = { {"h", f_help}, {"l", f_lang}, {"q", f_exit}, {"i", f_import}, {"t", f_train}, {"c", f_clear}, {"s", f_settings} };

//Wait until user presses ENTER
void UIwait(){
	ui.writeText(mes::wait);
	line = ui.readLine();
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
	ui.cls();
	ui.writeText(mes::welcome);

	flags |= 1;
	cmd_t c;
	while(flags & 1){
		line = ui.readLine();
		trim(line);
		if (line.size()>0)
			UIfunction(split(line, " "));
	}

	ui.writeText(mes::exit);
}
