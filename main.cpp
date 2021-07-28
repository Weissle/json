#include "include/json.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstring>
using namespace std;
using namespace wjson;
std::string ReadFromFile(const std::string path);

int main(){
	wjson::Json json;
	// wjson::Parse(ReadFromFile("citm_catalog.json"),json);
	auto citm = ReadFromFile("data/twitter.json");
	json.parse(citm);
	stringstream stream;
	json.dump(stream);
	cout<<stream.rdbuf()<<endl;
	cout << json.pretty();
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
