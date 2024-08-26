#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <bitset>

//Training types

struct dictionary{
	std::string id;
	std::string name;
	std::string ver;
	std::string desc;
	struct entry{
		std::string id;
		std::vector<std::string> alts;
		//TODO: INFLECTIONS
		std::vector<std::string> defs;
		// entry(std::string &id, std::string &inf, std::string &def);
	};
	std::map<std::string, entry> entries;
	dictionary(const std::string &id);
};



//A set of entries grouped in categories
class set{
public:

	//User-defined entry category
	struct category{
		int num;
		std::string name;
	};

	//One set of answers, mutually alternative
	struct ansgroup{
		uint8_t type;	// 0 - mandatory, '*' - not-mandatory, '-' - ending
		std::vector<std::string> ans;
		operator std::string();
	};

	//A single entry, or line in sourcefile - key and answer
	struct entry{
		dictionary::entry e;


		char state;
		int lineNum;	//deprecated
		std::string key;
		std::vector<ansgroup> ans;
		const category* cat;

		entry(int lineNum, const std::string &line, const category* cat, char state = 0);

		bool check(const std::vector<std::string>& ans);

		operator std::string() const;
	};

	std::string name;
	std::vector<entry> entries;
	int activeEntries;
	std::map<std::string, category> categories;

	set(std::string name);

	//settings: b0 - randomize; b1 - group terms; b2 - repeat wrongs;
	void train(uint8_t settings = 0b00000000);
};

//One file from which sets are read
class sourcefile{
public:
	std::vector<std::string> lines;
	std::string path;

	std::vector<set> sets;

	sourcefile(std::string path);

	uint8_t read();

	void update();

	uint8_t  write();
};
