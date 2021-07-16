#pragma once

#include "value/value_base.hpp"
#include "utils/shared_not_null_ptr.hpp"

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

	ValueBasePPtr& operator[](std::string s);
	Map& GetValue();
	const Map& GetValue()const;
	size_t Size()const;
	void Remove(const std::string s);
	void Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Object::Object():ValueBase(ValueType::Object){}

inline ValueBasePPtr& Object::operator[](std::string s){ 
	return value_[s];
}

inline void Object::Remove(const std::string s){
	value_.erase(s);
}

inline size_t Object::Size()const{
	return value_.size();
}

inline void Object::Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{
	stream << "{";
	if(pretty) stream<<'\n';
	for(auto it=value_.begin(); it!=value_.end();++it){
		if(it != value_.begin()) stream << ",";
		if(pretty) stream<<'\n';
		Indent(stream,indent_num,indent_char,indent_level+1);
		stream << '\"' << it->first << "\":";

		const auto tmp = (*it).second->get();
		if(tmp == nullptr) stream<<"null";
		else tmp->Dump(stream, pretty,indent_num, indent_char, indent_level+1);
	}
	if(pretty)stream<<'\n';
	Indent(stream,indent_num,indent_char,indent_level);
	stream<<'}';
}

inline Map& Object::GetValue(){ return value_; }

inline const Map& Object::GetValue()const{ return value_; }

}
