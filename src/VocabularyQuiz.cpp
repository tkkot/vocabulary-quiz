#include <iostream>
#include <fstream>
#include <cctype>
#include <ctime>

#include "functions.h"
#define pb push_back

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

//map<string, string> longSyl = {{"ā", "a"}, {"ī", "i"}};
vector<set> *sets;

int32_t main(int argc, char *argv[]){

	string filepath;

	init();

	//Parse parameters
	//Default: start UI loop
	//train [path] - start training sets from path


	//TODO - proper argument parsing
	if (argc>1){
		if(! strcmp(argv[1], "train")){
			//Call functions in UI outside loop
			if (argc>2)
				filepath=argv[2];
			else
				filepath=askPath();
			trainFile(filepath);
		}
	}
	UIstart();

/*
	sourcefile sf(filepath);	//And this as function
	vector<set> &sets = *sf.read();		//store sets in THIS file
	if(!&sets){
		cerr<<"File not found\n"<<filepath;
		getline(cin, filepath);
		return 2;
	}

	//All to UI
	cout<<"List of training sets in sourcefile:\n";
	for(int i = 0; i<sets.size(); i++){
		cout<<i+1<<". "<<sets[i].name<<endl;
	}

	//training activated by UI or argument
	for(set& s : sets){
		s.train(3);
	}

	sf.update(sets);	//dynamic updating of data?
	sf.write();

	delete &sets;
*/
	if (sets){
//		cout<<"Deleting\n";
		delete sets;
	}
//	clog<<"END\n";
	return 0;
}

