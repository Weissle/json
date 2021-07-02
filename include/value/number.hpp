#pragma once
#include "value/value_base.hpp"
#include <cstdio>



namespace wjson {

using LL = long long;

class Number : public ValueBase{

	double value;
	bool isInteger;
public:
	Number();
	Number(double v);
	Number(int v);
	Number(LL v);

	double GetValue()const;
	void SetValue(const double v_);
	void SetValue(const LL v_);
	void SetValue(const int v_);

	constexpr ValueType GetType()const;
	
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Number::Number():Number(0){}

inline Number::Number(double v):ValueBase(ValueType::Number),value(v),isInteger(false){}
inline Number::Number(LL v):ValueBase(ValueType::Number),value(v),isInteger(true){}
inline Number::Number(int v):ValueBase(ValueType::Number),value(v),isInteger(true){}

inline double Number::GetValue()const{ return value; }

inline void Number::SetValue(const double v_){ value = v_; isInteger = false;}
inline void Number::SetValue(const LL v_){ value = v_; isInteger = true;}
inline void Number::SetValue(const int v_){ value = v_; isInteger = true;}

inline void Number::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	if(isInteger) stream << LL(value);
	else stream << value;
}

}

