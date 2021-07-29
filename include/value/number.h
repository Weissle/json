#pragma once
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <utility>
#include <variant>
#include <string>



namespace wjson {

class Number{
	std::variant<double,std::string> value;
public:
	Number();
	template<class T>
	Number(T && v);
	Number(const char *ptr);
	Number(const char* ori_begin,const char* ori_end);

	double get()const;

	template<class T>
	void set(T && v);
	void set(const char *ptr);

	template<class T>
	Number& operator=(T &&v);
	Number& operator=(const char *ptr);
	bool operator==(const Number &_rhs)const;
	bool operator==(const double &_rhs)const;

	operator double()const;	
	void Dump(std::stringstream &stream)const;

};

template<class T>
Number::Number(T && v):value(std::forward<T>(v)){};

template<class T>
void Number::set(T && v){
	value = std::forward<T>(v);
}

template<class T>
Number& Number::operator=(T &&v){
	value = std::forward<T>(v);
	return *this;
}

}

