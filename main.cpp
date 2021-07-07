#include "json.hpp"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
int main(){

	wjson::Json json;
	std::string case1 = "{\"true\":true,   \"false\":false,   \"null\":null }";
	json.Parse(case1);

	stringstream stream;
	json.Dump(stream,4,' ');
	cout<<stream.rdbuf()<<endl;

	return 0;
}

