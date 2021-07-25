#include "value/number.h"
#include <string>
#include <variant>

namespace wjson {

Number::Number():Number(0){}

Number::Number(double v):value(v){};

Number::Number(const char* ori_begin,const char* ori_end){
	value.emplace<1>(ori_begin,ori_end);
}

Number::Number(const std::string &s):value(s){}

double Number::get()const{ 
	if(std::holds_alternative<double>(value)) return std::get<0>(value);
	else return std::strtod(std::get<std::string>(value).data(),nullptr);
}

void Number::set(const double v_){
	value = v_;
}

Number::operator double()const{ return get(); }

Number& Number::operator=(const double v){
	set(v);
	return *this;
}

void Number::Dump(std::stringstream &stream)const{
	if(std::holds_alternative<std::string>(value)) stream << std::get<1>(value);
	else stream << std::get<0>(value);
}

}
