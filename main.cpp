#include "include/json.hpp"
#include "parse/parse.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

std::string ReadFromFile(const std::string path);

int main(){

	wjson::Json json;

	// wjson::Parse(ReadFromFile("citm_catalog.json"),json);
	const char *case1 = "{\"abc\":true, \"ase\":null, \"sfel\":\"Sfefl\" }";
	wjson::Parse(case1,json);
	std::stringstream stream;
	json.Dump(stream,false);
	cout<<stream.rdbuf()<<endl;

	return 0;
}

std::string ReadFromFile(const std::string path){
	fstream f(path.c_str(),ios_base::in);
	std::string ret;

	f.seekg(0, std::ios::end);   
	ret.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	ret.assign((std::istreambuf_iterator<char>(f)),
			std::istreambuf_iterator<char>());
	return ret;
}
