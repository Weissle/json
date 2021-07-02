#include "json.hpp"
#include <iostream>

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

	cout<< json.IsObject()<<endl;
	cout<< json["happy"].IsBool()<<endl;
	cout<< json["happy"].IsNone()<<endl;
	cout<< json["country"].IsObject()<<endl;
	cout<< json["none"].IsNone()<<endl;
	cout<< json["none"].IsNumber()<<endl;
	json.Print();
	return 0;
}

