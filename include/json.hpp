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
	Json();
	void Parse(const char *s);
	void Parse(const std::string s);

};

inline Json::Json(){
	*vpptr = new Object();
	ValueWarp::Set(vpptr);
}
inline void Json::Parse(const char *s){
	CharPtrReader reader(s);
	if( reader.GetVChar() != '{' ) throw "json should start with { ";

	delete *vpptr;
	*vpptr = objectParser(reader);
	
}

inline void Json::Parse(const std::string s){
	Parse(s.c_str());
}

}
