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

	//Temporary argument parsing
	if (argc>1){
		UIfunction(vector<string>(argv+1, argv+argc));	//Arguments sent to be interpreted as UI command
	}
	else
		UIstart();	//Start the UI loop

	//Cleanup
	if (sf)
		delete sf;

	return 0;
}
