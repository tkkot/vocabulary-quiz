#include <iostream>
#include <cctype>
#include <ctime>

#include "functions.h"

#ifdef WIN32
	#include <windows.h>

	void init(){
		srand(time(0));
//		system("chcp 65001");
	}

#else
	void init(){
		srand(time(0));
	}
#endif

using namespace std;

sourcefile *sf;

int32_t main(int argc, char *argv[]){

	init();

	//TODO - proper argument parsing
	if (argc>1){
		UIfunction(vector<string>(argv+1, argv+argc));
	}
	else
		UIstart();

	if (sf)
		delete sf;

	return 0;
}
