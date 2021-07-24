#include "value/number.h"

namespace wjson {

Number::Number():Number(0){}

Number::Number(double v):Number(v,nullptr,nullptr){};

Number::Number(double v,const char* ori_begin,const char* ori_end):value(v){
	if(ori_begin && ori_end){
		origin.assign(ori_begin,ori_end);
	}
}

double Number::GetValue()const{ return value; }

void Number::SetValue(const double v_){
	value = v_;
	origin.clear();
}

Number::operator double()const{ return value; }

void Number::Dump(std::stringstream &stream)const{
	if(origin.size()) stream << origin;
	else stream << value;
}

}
