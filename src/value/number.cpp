#include "value/number.h"
#include <cmath>
#include <string>
#include <variant>
#include <math.h>

namespace wjson {

Number::Number():value(0){}

Number::Number(double v):value(v){}

Number::Number(const char *ptr){
	value.emplace<1>(ptr);
}

Number::Number(const char* ori_begin,const char* ori_end){
	value.emplace<1>(ori_begin,ori_end);
}

double Number::get()const{ 
	switch (value.index()){
		case 0: return std::get<0>(value);
		case 1: return std::strtod(std::get<1>(value).data(),nullptr);
	}
	return 0;
}

Number& Number::operator=(const char *ptr){
	value.emplace<1>(ptr);
	return *this;
}

bool Number::operator==(const Number &_rhs)const{
	return fabs(get() - _rhs.get()) < 2e-16;
}
bool Number::operator==(const double &_rhs)const{
	return fabs(get() - _rhs) < 2e-16;
}

void Number::set(const char *ptr){
	value.emplace<1>(ptr);
}
Number::operator double()const{ return get(); }

void Number::Dump(std::stringstream &stream)const{
	if(std::holds_alternative<std::string>(value)) stream << std::get<1>(value);
	else stream << std::get<0>(value);
}

}
