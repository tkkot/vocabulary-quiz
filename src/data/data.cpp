#include <types.h>
#include "data.h"
#include <functions.h>

#include <filesystem>
#include <fstream>
#include <map>
#include <set>

#ifndef __SETS_LOCATION__
	// #define __SETS_LOCATION__ "~/.config/vocab"
	#define __SETS_LOCATION__ "/home/tkkot/dev/projects/vocabulary-quiz/data/"
#endif

namespace fs = std::filesystem;

std::string data_location;

std::vector<std::string> dList, sList;
std::vector<dictionary*> dictionaries;
std::vector<set*> sets;
// std::map<const std::string, set*> loadedSets;
// std::set<std::string> loadableSets;

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

void (*init)() = [](){
	data_location=unwrapPath(__SETS_LOCATION__);
	// std::clog<<"Setting initial path to:"; //LOGGER!!!
	init = [](){};
	return;
};

uint8_t reloadData(const std::string &location){
	init();
	data_location = unwrapPath(location);
	return reloadData();
}

uint8_t reloadData(){
	init();
	ui->writeText(data_location+"\n");

	if(!fs::exists(data_location+"dictionaries")){
		fs::create_directories(data_location+"dictionaries");
	}
	else{
		dList.clear();
		for(const auto & set : fs::recursive_directory_iterator(data_location+"dictionaries")){
			ui->writeText((std::string)set.path()+"\n");
			if (set.is_regular_file()){
				dList.push_back(set.path());
			}
		}
	}
	if(!fs::exists(data_location+"sets"))
		fs::create_directories(data_location+"sets");
	else{
		sList.clear();
		for(const auto & dic : fs::recursive_directory_iterator(data_location+"sets")){
			ui->writeText((std::string)dic.path()+"\n");
			if (dic.is_regular_file()){
				sList.push_back(dic.path());
			}
		}
	}
	return 0;
	// std::system(std::string("mkdir -p " + data_location).data());
}

///Load functions should NOT call reloadData() - may mess up ids. Instead, check with file_exists().
///Maybe move these functions to their classes?
uint8_t loadDictionary(int id){
	std::string path = data_location+dList[id];
	
	if(!fs::exists(path)){
		(*ui)<<"File "<<path<<" seems to no longer exist. Please reload data.";
		return 'f';
	}
	std::ifstream file(path);
	if(!file)
		return 'F';
	
	dictionary *d = new dictionary(dList[id]);
	dictionaries.push_back(d);		//Add test for whether dict is already loaded / duplicacte removal

	std::getline(file, d->name);
	std::getline(file, d->ver);
	std::getline(file, d->desc);

	return 0;
}

uint8_t loadSet(int id){
	return 0;
}






// uint8_t getSetList(const std::string &location){
// 	data_location = location;
// 	return getSetList();
// }
// uint8_t getSetList(){
// 	// ui->writeText(data_location+"\n");
// 	std::system(std::string("mkdir -p " + data_location).data());
// 	std::ifstream fs(unwrapPath(data_location)+"/.setlist");
// 	if(!fs)
// 		return 'F';
	
// 	std::string setName;
// 	loadableSets.clear();
// 	while(std::getline(fs, setName))
// 		loadableSets.insert(setName);
// 	fs.close();
// 	return 0;
// }


//TEMPORARILY USING OLD ENTRY FORMAT. TO BE CHANGED!!!
// uint8_t loadSet(const std::string &setname){
// 	std::ifstream fs(unwrapPath(data_location)+"/"+setname);
// 	if(!fs)
// 		return 'F';

// 	set*& s = loadedSets[setname];
// 	if(!s)
// 		delete s;
// 	s = new set(setname);
// 	std::string line;

// 	std::getline(fs, line);
// 	int nCat = std::stoi(line);
// 	int e=0;

// 	for(int i=0; i<nCat; i++){
// 		std::string cat;
// 		std::getline(fs, cat);
// 		s->categories[cat] = {i, cat};

// 		std::getline(fs, line);
// 		int nE = std::stoi(line);
// 		for(int j=0; j<nE; j++){
// 			std::getline(fs, line);
// 			if(startsWith(line, "$")){
// 				s->entries.push_back({j, line.substr(1), &s->categories[cat], '^'});
// 				continue;
// 			}
// 			if(startsWith(line, "!")){
// 				s->entries.push_back({j, line.substr(1), &s->categories[cat], '!'});
// 				continue;
// 			}
// 				s->entries.push_back({j, line, &s->categories[cat]});
// 		}

// 	}

// 	return 0;
// }

uint8_t saveSet(const set *s){

	// // getSetList();
	// if(loadableSets.insert(s->name).second){
	// 	std::ofstream fs(unwrapPath(data_location)+"/.setlist");
	// 	for(const std::string &i : loadableSets){
	// 		fs<<i<<'\n';
	// 	}
	// 	fs.close();
	// }
	// std::ofstream fs(unwrapPath(data_location)+"/"+s->name);
	// for(const set::entry &e : s->entries){
	// 	fs<<e<<'\n';
	// }
	// fs.close();
	// for(const set i : sets){
	// 	std::fstream fs(data_location+"/"+i.name);
	// 	for(const entry j : i.entries){
	// 		fs<<j;
	// 	}
	// }
	return 0;
}

///LEGACY
uint8_t importSets(const std::string &path){
	std::string line;
	//Open file
	std::ifstream is(unwrapPath(path));
	if(!is)
		return 'F';
	set* curSet = new set(split(path, "/").back());
	int n=sets.size();
	sets.push_back(curSet);	//Create default set		TODO: only if not specufied
	std::string curCat;
	//PARSE FILE
	while(std::getline(is, line)){
		if(line.empty())
			continue;
		if(startsWith(line, "#"))
			continue;
		if(startsWith(line, "@")){
			curSet= new set(trim_c(line.substr(1)));
			sets.push_back(curSet);
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
	curSet = sets[n];
	if(curSet->entries.empty()){
		sets.erase(sets.begin()+n);
		delete curSet;
	}
	is.close();
	return 0;
}

dictionary::dictionary(const std::string &id): id(id){

}