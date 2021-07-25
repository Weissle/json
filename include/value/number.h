#pragma once
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <variant>
#include <string>



namespace wjson {

class Number{
	std::variant<double,std::string> value;
public:
	Number();
	Number(double v);
	Number(const char* ori_begin,const char* ori_end);
	Number(const std::string &s);

	double get()const;
	void set(const double v_);
	template<class T>
	void set(const T from,const T to);
	Number& operator=(const double v);
	Number& operator=(const std::string &s);
	operator double()const;	
	void Dump(std::stringstream &stream)const;

};

}

