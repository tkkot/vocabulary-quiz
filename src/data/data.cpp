#include <types.h>
#include "data.h"
#include <functions.h>

#include <fstream>
#include <map>
#include <set>

#ifndef __SETS_LOCATION__
	#define __SETS_LOCATION__ "~/.config/vocab"
#endif


std::string data_location = __SETS_LOCATION__;

std::map<const std::string, set*> loadedSets;
std::set<std::string> loadableSets;

inline std::string unwrapPath(std::string path){
	std::string newpath="";
	std::string home = getenv("HOME");
	for(char c : path){
		if(c=='~')
			newpath+=home;
		else 
			newpath+=c;
	}
	return newpath;
}

uint8_t getSetList(const std::string &location){
	data_location = location;
	return getSetList();
}
uint8_t getSetList(){
	// ui->writeText(data_location+"\n");
	std::system(std::string("mkdir -p " + data_location).data());
	std::ifstream fs(unwrapPath(data_location)+"/.setlist");
	if(!fs)
		return 'F';
	
	std::string setName;
	loadableSets.clear();
	while(std::getline(fs, setName))
		loadableSets.insert(setName);
	fs.close();
	return 0;
}


//TEMPORARILY USING OLD ENTRY FORMAT. TO BE CHANGED!!!
uint8_t loadSet(const std::string &setname){
	std::ifstream fs(unwrapPath(data_location)+"/"+setname);
	if(!fs)
		return 'F';

	set*& s = loadedSets[setname];
	if(!s)
		delete s;
	s = new set(setname);
	std::string line;

	std::getline(fs, line);
	int nCat = std::stoi(line);
	int e=0;

	for(int i=0; i<nCat; i++){
		std::string cat;
		std::getline(fs, cat);
		s->categories[cat] = {i, cat};

		std::getline(fs, line);
		int nE = std::stoi(line);
		for(int j=0; j<nE; j++){
			std::getline(fs, line);
			if(startsWith(line, "$")){
				s->entries.push_back({j, line.substr(1), &s->categories[cat], '^'});
				continue;
			}
			if(startsWith(line, "!")){
				s->entries.push_back({j, line.substr(1), &s->categories[cat], '!'});
				continue;
			}
				s->entries.push_back({j, line, &s->categories[cat]});
		}

	}

	return 0;
}

uint8_t saveSet(const std::string &set){



	// if(loadableSets.insert(set).second){
	// 	//update set list
	// }

	
	// for(const set i : sets){
	// 	std::fstream fs(data_location+"/"+i.name);
	// 	for(const entry j : i.entries){
	// 		fs<<j;
	// 	}
	// }
	return 0;
}


uint8_t importSets(const std::string &path){
	std::string line;
	//Open file
	std::ifstream is(unwrapPath(path));
	if(!is)
		return 'F';
	set* curSet = new set(split(path, "/").back());
	loadedSets[curSet->name] = curSet;	//Create default set		TODO: only if not specufied
	std::string curCat;
	//PARSE FILE
	while(std::getline(is, line)){
		if(line.empty())
			continue;
		if(startsWith(line, "#"))
			continue;
		if(startsWith(line, "@")){
			curSet= new set(trim_c(line.substr(1)));
			loadedSets[curSet->name]= curSet;
			continue;
		}
		if(startsWith(line, "$")){
			curCat = trim_c(line.substr(1));
			curSet->categories[curCat] = {(int)curSet->categories.size(), curCat};
			continue;
		}
		if(startsWith(line, "^")){
			curSet->entries.push_back( {0, line.substr(1), &(curSet->categories[curCat]), '^' } );
			continue;
		}
		if(startsWith(line, "!")){
			curSet->entries.push_back( {0, line.substr(1), &(curSet->categories[curCat]), '!'} );
			continue;
		}
		curSet->entries.push_back( {0, line, &(curSet->categories[curCat]) } );
	}
	curSet = loadedSets[split(path, "/").back()];
	if(curSet->entries.empty()){
		loadedSets.erase(curSet->name);
		delete curSet;
	}
	is.close();
	return 0;
}