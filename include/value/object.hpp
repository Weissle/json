#pragma once
#include "value/none.hpp"
#include "value/value_base.hpp"
#include "value/value_warp.hpp"

#include <cstdio>
#include <map>
#include <string>

namespace wjson {


class Object : public ValueBase{
	Map value_;
	void PrintIndent(int t,const char* sp="  ")const;
public:
	Object();

	ValueBase** operator[](std::string s);
	void Print(int indent = 0)const;

};

inline Object::Object():ValueBase(ValueType::Object){}

inline void Object::PrintIndent(int t,const char* sp)const{
	for (int i = 0; i < t; ++i){ 
		std::printf("%s",sp);
	}
	return;
}


inline ValueBase** Object::operator[](std::string s){ 
	auto &tmp = value_[s];
	if(tmp == nullptr) tmp = new ValueBase*();
	return tmp;
}

void Object::Print(int indent)const{
	std::printf("{\n");
	for(auto it=value_.begin(); it!=value_.end();++it){
		if(it != value_.begin()) std::printf(",\n");
		PrintIndent(indent+1);		
		std::printf("\"%s\" : ",it->first.c_str());

		auto &tmp = *(it->second);
		if(tmp == nullptr) std::printf("none");
		else tmp->Print(indent+1);
	}
	std::printf("\n");
	PrintIndent(indent);		
	std::printf("}");
	return;
}

}
