#pragma once
#include "value/value_base.hpp"
#include <cstdio>
#include <math.h>
#include <iomanip>
#include <sstream>



namespace wjson {

using LL = long long;

class Number : public ValueBase{

	long double value;
	int precision;
	int GetPrecision(long double num);
public:
	Number();
	Number(long double v);

	double GetValue()const;
	void SetValue(const long double v_);
	//void SetValue(const LL v_);
	//void SetValue(const int v_);

	constexpr ValueType GetType()const;
	
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;

};

inline Number::Number():Number(0){}

inline int Number::GetPrecision(long double num){
	num -= LL(num);
	int ret = 0;
	int l = 0,r= 16;
	constexpr double threshold = 1e-16;
	while(l <= r){
		int mid = (l+r)/2;
		long double p = pow(10,mid);
		long double tmp = num * p;
		tmp -= (LL)tmp;
		if(fabs(tmp)<threshold * p){
			ret = mid;
			r = mid-1;
		}
		else l = mid+1;
	}
	return ret+1;
}


inline Number::Number(long double v):ValueBase(ValueType::Number),value(v),precision(GetPrecision(v)){}

inline double Number::GetValue()const{ return value; }

inline void Number::SetValue(const long double v_){
	value = v_;
	precision = GetPrecision(v_);
}
//inline void Number::SetValue(const LL v_){ value = v_; isInteger = true;}
//inline void Number::SetValue(const int v_){ value = v_; isInteger = true;}

inline void Number::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	// stream.precision(precision);
	stream << std::setprecision(precision) << value;
	// stream <<value;
}

}

