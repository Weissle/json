#pragma once
#include <cstdio>
#include <math.h>
#include <iomanip>
#include <sstream>



namespace wjson {

class Number{

	double value;
	std::string origin;
public:
	Number();
	Number(double v);
	Number(double v,const char* ori_begin,const char* ori_end);

	double GetValue()const;
	void SetValue(const double v_);
	operator double()const;	
	void Dump(std::stringstream &stream)const;

};

inline Number::Number():Number(0){}

inline Number::Number(double v):Number(v,nullptr,nullptr){};

inline Number::Number(double v,const char* ori_begin,const char* ori_end):value(v){
	if(ori_begin && ori_end){
		origin.assign(ori_begin,ori_end);
	}
}

inline double Number::GetValue()const{ return value; }

inline void Number::SetValue(const double v_){
	value = v_;
	origin.clear();
}

inline Number::operator double()const{ return value; }

inline void Number::Dump(std::stringstream &stream)const{
	if(origin.size()) stream << origin;
	else stream << value;
}

}

