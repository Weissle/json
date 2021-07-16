#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <memory>
#include <assert.h>
#include <sstream>
#include "utils/shared_not_null_ptr.hpp"




namespace wjson {


enum class ValueType{ Null,Bool,Number,Object,Array,String };
class ValueBase{

protected:
	ValueType type_;
	void Indent(std::stringstream &stream,const int indent_num, const char indent_char, const int indent_level)const;
public:
	ValueBase(ValueType t);
	virtual ~ValueBase() = default;

	ValueType GetType()const;

	virtual void Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const = 0;

};
inline void ValueBase::Indent(std::stringstream &stream,const int indent_num, const char indent_char, const int indent_level)const{
	for (int i = 0; i < indent_level * indent_num; ++i){ 
		stream<<indent_char;
	}
}

inline ValueBase::ValueBase(ValueType t):type_(t){}

inline ValueType ValueBase::GetType()const{ return type_; }

inline void ValueBase::Dump(std::stringstream  &stream,const bool pretty, const int indent, const int indent_char, const int indent_level)const{ return; }

using ValueBasePtr = std::unique_ptr<ValueBase>;
using ValueBasePPtr = SharedNotNullPtr<ValueBasePtr>;

}
