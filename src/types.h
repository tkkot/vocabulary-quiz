#pragma once

#include <string>
#include <vector>
#include <map>

//Training types

struct category{
	int num;
	std::string name;
};

struct ansgroup{
	uint8_t type;	// 0 - mandatory, '*' - not-mandatory, '-' - ending
	std::vector<std::string> ans;
};

struct entry{
	char state;
	int lineNum;
	std::string key;
	std::vector<ansgroup> ans;
	const category* cat;

	entry(int lineNum, const std::string &line, const category* cat, char state = 0);

	bool check(const std::vector<std::string>& ans);
};

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

class sourcefile{
public:
	std::vector<std::string> lines;
	std::string path;

	sourcefile(std::string path);

	std::vector<set> *read();

	void update(const std::vector<set>& sets);

	void write();
};
