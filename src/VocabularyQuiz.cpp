#include <iostream>
#include <cctype>
#include <ctime>

// #include <functions.h>
#include <UI/console/consoleUI.h>

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

//Currently imported file
// sourcefile *sf;


/**
 *	Desing principles:
 *		Backend functions indepentent from frontend
 *		Backend has control, sends commands to frontend, which is responsible
 *	   for their implementation.
 *		
*/

UI *ui;




//STOP WHILE TRAINING

//HELP SHOULD CLS



int32_t main(int argc, char *argv[]){
	/*
	Parser file should have variables of set types - flags
		Store boolean flags in bitset, varaibles are references to bits

		1. [-X] <commands>
			start program and immidiately execute <commands> in order
			-E | --exit	exit program after all commands finish execution
		[-s <settings>]
			set training settings
		[-l <language code>]
			set language
		[-Tf <filepath>]
			import file from <filepath> and train it
	*/

	parse({argv, argv+argc});	//parse commandline arguments

	init();
	//cmdCommand(vector<string>(argv+1, argv+argc));
	
	//TODO: UI system selecting
	ui= new consoleUI;
	
	ui->init();
	getSetList();

	// if(par::flags[1])
	// 	ui->cmdCommand({"i", par::f});
	// if(par::flags[0])
	// 	ui->cmdCommand({"t"});
	if(!par::flags[4])
		ui->start();	//Start the UI loop

	//Cleanup

	for(const std::pair<const std::string, set*> &i : loadedSets){
		if(i.second)
			delete i.second;
	}

	// if (sf)
	// 	delete sf;

	return 0;
}
