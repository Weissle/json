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

	double get()const;
	void set(const double v_);
	Number& operator=(const double v);
	operator double()const;	
	void Dump(std::stringstream &stream)const;

};

}

