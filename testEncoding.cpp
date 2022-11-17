#include <iostream>
#include <fstream>
//#include <filesystem>
#include <windows.h>
//#include <codecvt>
#include <vector>

#define pb push_back
#define SET_UTF8 SetConsoleOutputCP(CP_UTF8)
#define RESET_CP SetConsoleOutputCP(CP)

#define DEBUG_OUTPUT

using namespace std;

int CP;

void printU8(const string& s){
	SetConsoleOutputCP(CP_UTF8);
	cout<<s;
	
	clog<<dec<<' '<<s.length()<<'\t'<<hex;
	
	for(char c : s){
		clog<<+(uint8_t)c<<' ';
	}
	cout<<endl<<dec;
	SetConsoleOutputCP(CP);
}

void printU16(const wstring& s){
	wcout<<s;
	
	clog<<dec<<' '<<s.size()<<'\t'<<hex;
	for(wchar_t c : s){
		clog<<+(uint16_t)c<<' ';
	}
	cout<<endl;
	
}

string wstringToU8(const wstring& ws){
	if(ws.empty())
		return string();
	int size = WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int)ws.size(), nullptr, 0, nullptr, nullptr);
	string s(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int)ws.size(), &s[0], size, nullptr, nullptr);
	return s;
}

/**
 *	Use UTF-8 internally with std::stirng
 *	On windows use SetConsoleOutputCP(CP_UTF8), and std::cout. For input use wstring and wcin, and than convert to UTF-8
 */

int32_t main(){
	
	vector<string> file;
	
	CP = GetConsoleOutputCP();
	clog<<CP<<endl;

	ifstream ifs("UTF-8.win");
	ofstream ofs("write.UTF-8");
	
	string s;
//	SET_UTF8;
	while(getline(ifs, s)){
		printU8(s);
		ofs<<s<<endl;
		file.pb(s);
	}
	cout<<endl;
	
//	for(string i : file){
//		cout<<dec<<i.size()<<" \""<<i<<"\"\t";
//		for(int j = 0; j<i.size(); j++){
//			cout<<hex<<+(uint8_t)i[j]<<' ';
//		}
//		cout<<endl;
//	}
//	RESET_CP;

	wstring ws;
	getline(wcin, ws);
	
//	wstring_convert<codecvt_utf8<wchar_t> > conv;
//	string c = conv.to_bytes(ws);
//	string c = wstringToU8(ws);
//	
//	SET_UTF8;
//	cout<<c<<endl;
//	RESET_CP;

	//UTF=16 to UTF8
	wcout<<ws<<endl;
	printU16(ws);
//	cout<<dec<<sizeof(ws.c_str())<<endl;
//	uint16_t *p = reinterpret_cast<uint16_t *>(&ws[0]);
//	for(int i = 0; i<sizeof(ws)/2; i++){
//		cout<<hex<<i<<' '<<p<<' '<<*p<<endl;
//		p+=1;
//	}
//	cout<<endl;

	printU8("\u0444 test\n");	
	
	
	ifs.close();
	ofs.close();
//	cout<<WideCharToMultiByte(L"TEST");
	
//	wcout<<endl<<L"\u0444";
//	cout<<endl<<u8"\u0444";
//	wifstream ifs("UTF-8.win");
//	wstring s;
//	getline(ifs, s);
//	wcout<<s;
	
//	cin.get();
	return 0;
}

