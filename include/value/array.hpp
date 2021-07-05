#pragma once

#include "value/value_base.hpp"
#include "value/value_base_pptr.hpp"

#include <map>
#include <sstream>
#include <string>

namespace wjson {
using Vector = std::vector<ValueBasePPtr>;

class Array : public ValueBase{
	Vector value_;
public:
	Array();
	ValueBase** operator[](int idx);

	size_t Size()const;

	Vector& GetValue();
	const Vector& GetValue()const;

	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;
	void Resize(int s_);

};

inline Array::Array():ValueBase(ValueType::Array){}

inline void Array::Resize(int s_){
	value_.resize(s_);
}

inline ValueBase** Array::operator[](int idx){ 
	return value_[idx];
}

inline size_t Array::Size()const{ return value_.size(); }

void Array::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	stream << "[\n";
	for(auto it=value_.begin(); it!=value_.end();++it){
		if(it != value_.begin()) stream<<",\n";
		Indent(stream,indent_num,indent_char,indent_level+1);
		auto &tmp = *it;
		if(*tmp == nullptr) stream<<"null";
		else (*tmp)->Dump(stream, indent_num, indent_char, indent_level+1);
	}
	stream << '\n';
	Indent(stream,indent_num,indent_char,indent_level);
	stream << ']';
	return;
}

inline Vector& Array::GetValue(){ return value_; }

inline const Vector& Array::GetValue()const{ return value_; }

}
