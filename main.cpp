#include "json.hpp"
#include <iostream>
#include <sstream>
using namespace std;
int main(){

	wjson::Json json;
	json["happy"] = true;
	json["sad"] = false;
	json["drunk"] = false;
	json["country"]["China"] = true;
	json["country"]["USA"] = false;
	json["none"];
	json["str_c"] = "string_c";
	json["str"] = std::string("string");

	json["int"] = 500;
	json["double"] = 5.03;
	json["array"].ToArray().Resize(3);
	json["array"][2] = "idx2";
	json["array"][0] = 1245364;

	stringstream stream;
	json.Dump(stream,4,' ');
	cout<<stream.rdbuf()<<endl;

	return 0;
}

