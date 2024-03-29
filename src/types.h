#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <bitset>

//Training types

//User-defined entry category
struct category{
	int num;
	std::string name;
};

//One set of answers, mutually alternative
struct ansgroup{
	uint8_t type;	// 0 - mandatory, '*' - not-mandatory, '-' - ending
	std::vector<std::string> ans;
};

//A single entry, or line in sourcefile - key and answer
struct entry{
	char state;
	int lineNum;
	std::string key;
	std::vector<ansgroup> ans;
	const category* cat;

	entry(int lineNum, const std::string &line, const category* cat, char state = 0);

	bool check(const std::vector<std::string>& ans);
};

//A set of entries grouped in categories
class set{
public:
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
