#pragma once
#include "value/value_base.hpp"
#include <cstdio>
#include <stdio.h>


namespace wjson {

class Null : public ValueBase{

public:
	Null();
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;
};

inline Null::Null():ValueBase(ValueType::Null){}

inline void Null::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	stream << "null";
}

}
