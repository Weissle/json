#pragma once
#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include <cstdio>



namespace wjson {

class Bool : public ValueBase{
	bool value_;
public:
	Bool();
	Bool(bool v_);

	bool GetValue()const;
	void SetValue(bool v_);

	constexpr ValueType GetType()const;
	
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Bool::Bool():Bool(false){}

inline Bool::Bool(bool _v):ValueBase(ValueType::Bool),value_(_v){}


inline bool Bool::GetValue()const{ return value_; }
inline void Bool::SetValue(bool v_){ value_ = v_; }

inline void Bool::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	if(value_) stream<<"true";
	else stream<<"false";
}

}
