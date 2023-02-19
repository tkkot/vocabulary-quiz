#include <iostream>
#include <cctype>
#include <ctime>

#include "functions.h"

//Platform-dependent functions
#ifdef _WIN32
	#include <windows.h>

	//initialize randomizer
	void init(){
		srand(time(0));
		SetConsoleOutputCP(CP_UTF8);
//		setvbuf(stdout, nullptr, _IOFBF, 1000);		//Is it needed??? A fix for a VS bug. Might cause issues?
	}

#else
	void init(){
		srand(time(0));
	}
#endif

using namespace std;

//Currently imported file
sourcefile *sf;

int32_t main(int argc, char *argv[]){

	init();

	//STOP WHILE TRAINING

	//HELP SHOULD CLS
	/*
	Parser file should have variables of set types - flags
		Store boolean flags in bitset, varaibles are references to bits

		1. [-E] <command>
			start program and immidiately execute <command>
			-E | --exit	exit program after command finishes execution
		[-s <settings>]
			set training settings
		[-l <language code>]
			set language
		[-Tf <filepath>]
			import file from <filepath> and train it
	*/

	parse({argv, argv+argc});
		//UIfunction(vector<string>(argv+1, argv+argc));
	if(par::flags[1])
		UIfunction({"i", par::f});
	if(par::flags[0]);
		UIfunction({"t"});
	if(!par::flags[4])
		UIstart();	//Start the UI loop

	//Cleanup
	if (sf)
		delete sf;

	return 0;
}
