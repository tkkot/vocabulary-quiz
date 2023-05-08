#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

#ifdef WIN32
	#include <Windows.h>
#endif

int main(){
	std::string str;

#ifdef WIN32
	SetConsoleOutputCP(CP_UTF8);
	setvbuf(stdout, nullptr, _IOFBF, 1000);		//Is it needed??? A fix for a VS bug. Might cause issues?
#endif

	{	//Console (iostream)
		std::getline(std::cin, str);
		std::cout<<str<<std::endl;
		std::cout<<str.size()<<std::endl;
		
		for(unsigned char i : str){
			std::cout<<0+i<<'\t';
		}
		std::cout<<"\n\n==-------------------------------==\n\n";
	}
	
	{	//File
		std::ifstream is("test.vcb");
		std::vector<std::string> v;
		
		while(std::getline(is, str)){
			v.push_back(str);
			std::cout<<str<<'\n';
		}
		for(std::string i : v){
			std::cout<<i.size()<<'\n';
			for(unsigned char j : i)
				std::cout<<0+j<<'\t';
			std::cout<<'\n';
		}
		
		is.close();
		
		std::ofstream os("output.vcb");
		for(std::string i : v)
			os<<i<<'\n';
		os.close();
		
		std::cout<<"\n\n==-------------------------------==\n\n";
	}

	{	//In-code string
		str = u8"test ĄĘłćóź αβπφΔΦΩ кот äÖß áéí āēī\nמלךֳזָסבּנ";
		std::cout<<str<<'\n'<<str.size()<<'\n';
		for(unsigned char i : str){
			std::cout<<0+i<<'\t';
		}
	}
	
}
