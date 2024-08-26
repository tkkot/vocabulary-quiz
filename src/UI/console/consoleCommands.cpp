#include <functions.h>

#define __MES_UI__
#define __MES_CONSOLE_UI__
#include <UI/messages.h>
#include <stdexcept>
#include <algorithm>

#include "consoleUI.h"

typedef void (*cmd_t)(const std::vector<std::string>&);	//Funcion pointers to executed commands
typedef const std::vector<std::string> &arg_t;		//Macro for command arguments

uint8_t flags;	//Program flags (for now only [0]=running)
std::string line;	//Container for user input

// Program util functions
///Display help message
void f_help(arg_t args){
	ui->cls();
	ui->writeText(mes::help);
}
///Choose language (for now not working)
void f_lang(arg_t args){
	ui->writeText(mes::lang);
}
///Call the clean screen function
void f_clear(arg_t args){
	ui->cls();
}
///Set running flag to 0, exit program
void f_exit(arg_t args){
	flags &= 0B11111110;
}
//Errors
///Display error message for unknown command
void f_wrongFunc(arg_t& func){
	ui->writeText({mes::err_com[0], func[0], mes::err_com[1], "\n"});
}
///Display error message for wrong arguments for function (or lack of them)
void f_argError(arg_t data){
	if(data.size()>1){
		ui->writeText(mes::err_arg[0]);
		for(auto i = data.begin()+1; i<data.end(); i++)
			ui->writeText({*i, " "});
		ui->writeText({mes::err_arg[1], data[0], mes::err_arg[4]});
	}
	else
		ui->writeText({mes::err_arg[2], data[0], mes::err_arg[3], mes::err_arg[4]});
}
// App functions
void f_reload(arg_t args){
	ui->writeText("Reloading sets and dictionaries...\n");
	reloadData();
}
void f_list(arg_t args){
    ui->cls();

	///MOVE TO DIFFERENT FUNCTION
    // if(args.size()>1){
	// 	int n;
	// 	try{
	// 		n=std::stoi(args[1]);
	// 		if(n>=sets.size()){
	// 			(*ui)<<"Number out of range";
	// 			return;
	// 		}
	// 	}
	// 	catch(std::invalid_argument){
	// 		n=std::distance(sets.begin(), std::find_if(sets.begin(), sets.end(), [&](const set* const s){
	// 			return s->name==args[1];
	// 		}));
	// 		if(n==sets.size()){
	// 			ui->writeText("Set "+args[1]+" not loaded");
	// 			return;
	// 		}
	// 	}

	// 	for(const set::entry &i : sets[n]->entries){
	// 		ui->writeText({i,"\n"});
    //     }
    // }
    // else{

	ui->writeText("Available dictionaries:\n");
	for(int i=0; i<dList.size(); i++){
		(*ui)<<std::to_string(i)<<". "<<dList[i]<<"\n";
	}

	ui->writeText("Currently loaded dictionaries:\n");
	for(int i=0; i<dictionaries.size(); i++){
		(*ui)<<std::to_string(i)<<". "<<dictionaries[i]->name<<"\n";
	}

	ui->writeText("Available sets:\n");
	for(int i=0; i<sList.size(); i++){
		(*ui)<<std::to_string(i)<<". "<<sList[i]<<"\n";
	}

	ui->writeText("Currently loaded sets:\n");
	for(int i=0; i<sets.size(); i++){
		(*ui)<<std::to_string(i)<<". "<<sets[i]->name<<"\n";
	}
    // }
}
void f_load(arg_t args){

}
void f_save(arg_t args){
	std::string l = (
		(args.size()>1)	?		//I'm sorry. Was just curious whether this would work.
		args[1]			:		//I love the ternary operator.
		[](){					//And lambdas no longer scare me.
			(*ui)<<"Which set to save?";
			return trim_c(ui->readLine());
		}()
	);

	int n;
	try{
		n=std::stoi(l);
		if(n>=sets.size()){
			(*ui)<<"Number out of range";
			goto _END;
		}
	}
	catch(std::invalid_argument){
		n=std::distance(sList.begin(), std::find(sList.begin(), sList.end(), l));
		if(n==sList.size()){
			(*ui)<<("Set "+l+" not loaded");
			goto _END;
		}
	}
    saveSet(sets.at(n));
	
	// //DO NOT EVEN ASK.
	// try{
	// 	saveSet(sets.at(
	// 		[&](){
	// 			try{
	// 				int n = std::stoi(l);
	// 				return (
	// 					(n>=sets.size())	?
	// 					[](){
	// 						(*ui)<<"Number out of range";
	// 						return -1;
	// 					}()					:
	// 					n
	// 				);
	// 			}
	// 			catch(std::invalid_argument){
	// 				int n = std::distance(sList.begin(), std::find(sList.begin(), sList.end(), l));
	// 				return (
	// 					(n==sList.size())	?
	// 					[&](){
	// 						(*ui)<<"Set "+l+" not loaded";
	// 						return -1;
	// 					}()					:
	// 					n
	// 				);
	// 			}
	// 		}()
	// 	));
	// }
	// catch(std::out_of_range){}
	// //... yes. it does work. (compile. could not bother to test further)

_END:
    UIwait();
    ui->cls();
}
///Open sourcefile and import sets from it
void f_import(arg_t args){
    std::string path;
	if(args.size()>1){
		// sf = new sourcefile(args[1]);
        path = args[1];
	}
	else{
		ui->writeText(mes::import[0]);
		line = ui->readLine();
		trim(line);
		// sf = new sourcefile(line);
        path = line;
	}

	if(importSets(path)){
		ui->writeText({mes::import[2], path, "\n"});
		return;
	}
	ui->writeText(mes::import[1]);
	UIwait();
	ui->cls();
	return;
}
///Train current sets
void f_train(arg_t args){
    // ui->writeText("TRAINING TEMPORARILY NOT WORKING\n");
	std::string l = (
		(args.size()>1)	?		//I'm sorry. Was just curious whether this would work.
		args[1]			:		//I love the ternary operator.
		[](){					//And lambdas no longer scare me.
			(*ui)<<"Which set to train?";
			return trim_c(ui->readLine());
		}()
	);

	int n;
	try{
		n=std::stoi(l);
		if(n>=sets.size()){
			(*ui)<<"Number out of range";
			goto _END;
		}
	}
	catch(std::invalid_argument){
		n=std::distance(sList.begin(), std::find(sList.begin(), sList.end(), l));
		if(n==sList.size()){
			(*ui)<<("Set "+l+" not loaded");
			goto _END;
		}
	}

    sets.at(n)->train(par::s); 

	// if (!sf | args.size()>1)
	// 	f_import(args);
	// if(!sf)
	// 	return;

	// ui->writeText({mes::start_sf[0], sf->path, mes::start_sf[1]});
	// for(int i=0; i<sf->sets.size(); i++)
	// 	ui->writeText({std::to_string(i), ". ", sf->sets[i].name, "\n"});
	// UIwait();

	// for(set &s : sf->sets)
	// 	s.train(par::s);	//settings as parameter
	// ui->writeText(mes::fin);

	// sf->update();	//dynamic updating of data?
	// if(sf->write())
	// 	ui->writeText({mes::err_fnf, sf->path, "\n"});
_END:
	UIwait();
	ui->cls();
}
void f_settings(arg_t args){
	if(args.size()<2){	//TODO ask user for specifying settings
		f_argError({"s"});
		return;
	}
	par::s = std::stoi(args[1], nullptr, 2);
	ui->writeText({"New settings: ", std::to_string(0+par::s), "\n"});
	return;
}

//Map assigning shortcuts to commands, used to interpret user input
std::map<std::string, cmd_t> COMMANDS = { {"h", f_help}, {"lang", f_lang}, {"q", f_exit}, 
    {"r", f_reload}, {"l", f_list}, {"ld", f_load}, {"s", f_save}, {"i", f_import}, {"t", f_train},
	{"c", f_clear}, {"st", f_settings} };


/*
 * Execute function args[0] with parameter args
 */
int consoleUI::consoleCommand(const std::vector<std::string> &args){
	if(args.size() == 0)
		return 1;
	cmd_t c = COMMANDS[args[0]];
	if(c)
		c(args);
	else
		f_wrongFunc(args);
	return 0;
}

