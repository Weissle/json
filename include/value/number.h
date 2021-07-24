#pragma once
#include <cstdio>
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

}

