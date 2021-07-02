#pragma once

#include "value/value_base.hpp"

#include <cstdio>
#include <map>
#include <string>

namespace wjson {


class Object : public ValueBase{
	Map value_;
public:
	Object();

	ValueBase** operator[](std::string s);
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Object::Object():ValueBase(ValueType::Object){}

inline ValueBase** Object::operator[](std::string s){ 
	auto &tmp = value_[s];
	if(tmp == nullptr) tmp = new ValueBase*();
	return tmp;
}

void Object::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	stream << "{\n";
	for(auto it=value_.begin(); it!=value_.end();++it){
		if(it != value_.begin()) stream << ",\n";
		Indent(stream,indent_num,indent_char,indent_level+1);
		stream << '\"' << it->first << "\" : ";

		auto &tmp = *(it->second);
		if(tmp == nullptr) stream<<"none";
		else tmp->Dump(stream, indent_num, indent_char, indent_level+1);
	}
	stream<<'\n';
	Indent(stream,indent_num,indent_char,indent_level);
	stream<<'}';
}

}
