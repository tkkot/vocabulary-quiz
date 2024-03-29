#include "../types.h"
#include "data.h"
#include "../functions.h"
#include <fstream>

void writeSetList(){
}


void writeSets(const std::vector<set>& sets){
	for(const set i : sets){
		std::fstream fs(DATA_PATH+"/"+i.name);
		for(const entry j : i.entries){
			fs<<j;
		}
	}
}
