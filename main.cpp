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
	//cout<<stream.rdbuf()<<endl;

	stream.clear();
	json["array"].Resize(1);
	json.Dump(stream,4,' ');
	//cout<<stream.rdbuf()<<endl;

	cout<< json.IsObject()<<endl;
	cout<< json["happy"].IsBool()<<endl;
	cout<< json["happy"].IsNull()<<endl;
	cout<< json["country"].IsObject()<<endl;
	cout<< json["none"].IsNull()<<endl;
	cout<< json["none"].IsNumber()<<endl;

	wjson::JsonAny<wjson::Array> arr;
	arr.Resize(5);
	arr[1] = "first";
	arr[2] = "second";
	auto obj = arr[3].ToObject();
	obj["pgs"] = true;
	obj["pos"] = 3.3;
	arr[4] = nullptr;
	auto subarr = arr[0].ToArray();
	subarr.Resize(2);
	subarr[0] = "ags";
	subarr[1] = 3.5;

	stream.clear();
	arr.Dump(stream);
	cout<<stream.rdbuf()<<endl;

	return 0;
}

