#include <exception>
#include <ios>
#include <iomanip>
#include <json.h>
#include <iostream>
#include <fstream>
using namespace wjson;
using namespace std;
std::string ReadFromFile(const std::string path);

int main(){
	Json json;
	// 解析json，解析失败会抛出异常
	json.parse("{  \"info\": \"假设我们有这么一个json字符串\",\
			\"array\": [false, \"abcd\", null, 123456]}");

	//输出准备
	cout<<boolalpha << setiosflags(ios::fixed) << setprecision(6);

	// json.is<T>() T 可以是六种类型之一
	cout << json.is<Object>() << endl; // true
	cout << json["array"].is<Array>() << endl; // true
	cout << json["info"].is<Null>() << endl; // false

	// json.get<T>() T 可以是六种类型之一
	//如果类型不对会抛出异常
	cout << json["array"][0].get<Bool>() << endl; //false
	cout << json["array"][1].get<String>() << endl; // abcd
	cout << json["array"][2].get<Null>() << endl; // nullptr ; 在dump和pretty函数中是null;
	cout << json["array"][3].get<Number>() <<endl; // 123456

	{
		try{
			auto &tmp = json["array"][2].get<Array>();
		}catch(exception e){
			cout<< e.what() << endl;
		}
	}

	// 默认类型为Null, 使用to<T>()改变类型
	cout<< json["unknow"].is<String>() << endl; // false
	cout<< json["unknow"].is<Null>() << endl; // true
	json["unknow"].to<String>();
	cout<< json["unknow"].is<String>() << endl; // true

	// 改变值; 如果类型不对，会自动改变类型
	json["unknow"] = 123456.789;
	cout << json["unknow"].is<Number>() << endl; // true
	cout << json["unknow"].get<Number>() << endl;// 123456
	

	// dump and pretty
	cout << json.dump() << endl;
	cout << json.pretty(2,' ') << endl;

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
