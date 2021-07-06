#pragma once

#include "value/value_base.hpp"
#include "value/value_base_pptr.hpp"

#include <cstdio>
#include <map>
#include <string>
#include <assert.h>

namespace wjson {

using Map = std::map<std::string,ValueBasePPtr>;

class Object : public ValueBase{
	Map value_;
public:
	Object();

	ValueBase** operator[](std::string s);
	Map& GetValue();
	const Map& GetValue()const;
	size_t Size()const;
	void Remove(const std::string s);
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Object::Object():ValueBase(ValueType::Object){}

inline ValueBase** Object::operator[](std::string s){ 
	return value_[s];
}

void Object::Remove(const std::string s){
	value_.erase(s);
}

inline size_t Object::Size()const{
	return value_.size();
}

void Object::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	stream << "{\n";
	for(auto it=value_.begin(); it!=value_.end();++it){
		if(it != value_.begin()) stream << ",\n";
		Indent(stream,indent_num,indent_char,indent_level+1);
		stream << '\"' << it->first << "\" : ";

		const auto tmp = *(it->second);
		if(tmp == nullptr) stream<<"none";
		else tmp->Dump(stream, indent_num, indent_char, indent_level+1);
	}
	stream<<'\n';
	Indent(stream,indent_num,indent_char,indent_level);
	stream<<'}';
}

inline Map& Object::GetValue(){ return value_; }

inline const Map& Object::GetValue()const{ return value_; }

}
