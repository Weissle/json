#pragma once

#include "value/value_base.hpp"

#include <map>
#include <sstream>
#include <string>

namespace wjson {


class Array : public ValueBase{
	Vector value_;
public:
	Array();
	~Array();
	ValueBase** operator[](int idx);

	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;
	void Resize(int s_);

};

inline Array::Array():ValueBase(ValueType::Array){}
Array::~Array(){
	for (const auto &pptr:value_){
		if(pptr){
			delete *pptr;
			delete pptr;
		}
	}
}

void Array::Resize(int s_){
	if(s_ < value_.size()){
		for (int i = s_; i < value_.size(); ++i){ 
			if(value_[i]){
				delete *value_[i];
				delete value_[i];
			}
		}
	}
	value_.resize(s_);
}

inline ValueBase** Array::operator[](int idx){ 
	auto &tmp = value_[idx];
	if(tmp == nullptr) tmp = new ValueBase*();
	return tmp;
}

void Array::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	stream << "[\n";
	for(auto it=value_.begin(); it!=value_.end();++it){
		if(it != value_.begin()) stream<<",\n";
		Indent(stream,indent_num,indent_char,indent_level+1);
		auto &tmp = *it;
		if(tmp == nullptr) stream << "null";
		else (*tmp)->Dump(stream, indent_num, indent_char, indent_level+1);
	}
	stream << '\n';
	Indent(stream,indent_num,indent_char,indent_level);
	stream << ']';
	return;
}

}
