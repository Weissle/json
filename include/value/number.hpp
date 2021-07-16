#pragma once
#include "value/value_base.hpp"
#include <cstdio>
#include <math.h>
#include <iomanip>
#include <sstream>



namespace wjson {

using LL = long long;

class Number : public ValueBase{

	double value;
public:
	Number();
	Number(double v);

	double GetValue()const;
	void SetValue(const double v_);

	constexpr ValueType GetType()const;
	
	void Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Number::Number():Number(0){}

inline Number::Number(double v):ValueBase(ValueType::Number),value(v){}

inline double Number::GetValue()const{ return value; }

inline void Number::SetValue(const double v_){
	value = v_;
}

inline void Number::Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{
	stream <<value;
}

}

