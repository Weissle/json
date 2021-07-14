#pragma once

#include "value/value_base_pptr.hpp"
#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include "value/object.hpp"
#include "parser/reader.hpp"
#include "parser/parser.hpp"


#include <string>

namespace wjson {

// using Json = ValueWarp;

class Json: public ValueWarp{
	ValueBasePPtr vpptr;
public:
	std::string parserInfo;
	Json();
	void Parse(const char *s);
	void Parse(const std::string s);
	void ParseAny(const char *s);
	void ParseAny(const std::string s);
	void ParseAny(CharPtrReader &reader);

};

inline Json::Json(){
	*vpptr = new Object();
	ValueWarp::Set(vpptr);
}
inline void Json::Parse(const char *s){
	CharPtrReader reader(s);
	if( reader.LookVCharF() != '{' ) throw "json should start with { ";
	ParseAny(reader);
}

inline void Json::Parse(const std::string s){
	Parse(s.c_str());
}

inline void Json::ParseAny(const char *s){
	CharPtrReader reader(s);
	ParseAny(reader);
}

inline void Json::ParseAny(const std::string s){
	ParseAny(s.c_str());
}

inline void Json::ParseAny(CharPtrReader &reader){
	delete *vpptr;
	parserInfo.clear();
	try{
		*vpptr = AnyParser(reader);
		parserInfo = "ok";
	}catch(std::exception e){
		parserInfo = e.what();
	}
}

}
