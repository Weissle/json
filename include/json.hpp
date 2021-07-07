#pragma once

#include "value/value_base_pptr.hpp"
#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include "value/object.hpp"
#include "parser/reader.hpp"


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
void Json::Parse(const char *s){

}

inline void Json::Parse(const std::string s){
	Parse(s.c_str());
}

}
