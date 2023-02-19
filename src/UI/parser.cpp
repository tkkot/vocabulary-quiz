#include "../functions.h"

namespace par{
std::vector<bool> flags(4, 0);//	[T, f, l, s, E]

uint8_t s = 3; //settings: b0 - randomize; b1 - group terms; b2 - repeat wrongs;
std::string l = "en";
std::string f;

}

uint8_t parse(const std::vector<std::string> &args){
	for (int i=1; i<args.size(); i++){
		if(args[i][0] == '-'){
			int n = 1;
			for(int j=1; j<args[i].size(); j++){
				switch(args[i][j]){
				case 'T':
					par::flags[0]=1;
					break;
				case 'f':
					par::flags[1]=1;
					par::f=args[i+n];
					n++;
				case 'l':
					par::flags[2]=1;
					par::l=args[i+n];
					n++;
					break;
				case 's':
					par::flags[3]=1;
					par::s=stoi(args[i+n], nullptr, 2);
					n++;
					break;
				case 'E':
					par::flags[4]=1;
					break;
				}
			}
		}
	}
	return 0;
}
