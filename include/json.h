#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <memory>
#include <assert.h>
#include <sstream>
#include <variant>
#include <unordered_map>
#include <iostream>

#include "value/number.h"

namespace wjson {


enum class ValueType:int{ Null=0,Bool=1,Number=2,String=3,Array=4,Object=5};

using Null = std::nullptr_t;
using Bool = bool;
//using Number = number;
using String = std::string;

class JsonBase;
using Array = std::vector<JsonBase>;
using Object = std::map<std::string,JsonBase>;
using ObjectConstIterator = typename Object::const_iterator;
using ArrayConstIterator = typename Array::const_iterator;
using ObjectIterator = typename Object::iterator;
using ArrayIterator = typename Array::iterator;


class JsonBase{
protected:
	using Variant = std::variant<Null,Bool,Number,String,Array,Object>;
	Variant value_;
	

	void Indent(std::stringstream &stream,const int indent_num,const char indent_char,const int indent_level)const;

	void Dump(std::stringstream &stream,const std::string &s)const;

	void Dump(std::stringstream &stream,const Array &arr,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const;

	void Dump(std::stringstream &stream,const Object &obj,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const;

public:
	JsonBase(){}
	JsonBase(JsonBase &&_rv){
		value_ = std::move(_rv.value_);
	}
	JsonBase(const std::nullptr_t b):JsonBase(){}
	JsonBase(const bool b):value_(b){}
	JsonBase(const double d):value_(d){}
	JsonBase(const double d,const char *beg,const char *end):value_(Number(d,beg,end)){}
	JsonBase(const char *s):value_(s){}
	JsonBase(const std::string &s):value_(s){}

	JsonBase(ValueType _type);
	~JsonBase(){ 
		// static int release_count = 0;
		// release_count++;
		// if(release_count % 1000 == 0) 
		// std::cout<<"release : "<<release_count<<std::endl; 
	}

	ValueType GetType()const { return (ValueType)value_.index(); }

	template<class T>
	bool Is()const;

	template<class T>
	JsonBase& To();

	template<class T>
	const T& Get()const;

	template<class T>
	T& Get();

	JsonBase& operator=(const std::nullptr_t b){ value_.emplace<Null>(nullptr); return *this;}
	JsonBase& operator=(const bool b){ value_.emplace<Bool>(b); return *this;}
	JsonBase& operator=(const double d){ value_.emplace<Number>(d); return *this;}
	JsonBase& operator=(const int d){ value_.emplace<Number>(d); return *this;}
	JsonBase& operator=(const long long d){ value_.emplace<Number>(d); return *this;}
	JsonBase& operator=(const char *s){ value_.emplace<String>(s); return *this;}
	JsonBase& operator=(const std::string &s){ value_.emplace<String>(s); return *this;}
	JsonBase& operator=(JsonBase&& _rv){ value_ = std::move(_rv.value_); return *this;}

	bool IsBool()const{ return Is<bool>(); }
	bool IsNumber()const{ return Is<Number>(); }
	bool IsNull()const{ return Is<std::nullptr_t>(); }
	bool IsObject()const{ return Is<Object>(); }
	bool IsString()const{ return Is<String>(); }
	bool IsArray()const{ return Is<Array>(); }


	JsonBase& operator[](const std::string &s);
	JsonBase& operator[](std::string &&s);
	void Remove(const std::string &s);

	JsonBase& operator[](const int idx);
	void Resize(int s);
	void PushBack(JsonBase &&_other );

	void Clear();

public:
	ObjectConstIterator ObjectBegin()const;
	ObjectConstIterator ObjectEnd()const;
	ArrayConstIterator ArrayBegin()const;
	ArrayConstIterator ArrayEnd()const;

	ObjectIterator ObjectBegin();
	ObjectIterator ObjectEnd();
	ArrayIterator ArrayBegin();
	ArrayIterator ArrayEnd();

public:
	size_t Size()const;
	void Dump(std::stringstream &stream,const bool pretty=true,const int indent_num=4,const int indent_char=' ',const int indent_level=0)const;
};

template<class T>
bool JsonBase::Is()const{
	return std::holds_alternative<T>(value_);
}

template<class T>
JsonBase& JsonBase::To(){
	if(Is<T>() == false) value_.emplace<T>();
	return *this;
}

template<class T>
const T& JsonBase::Get()const{
	return std::get<T>(value_);
}

template<class T>
T& JsonBase::Get(){
	return std::get<T>(value_);
}

using Json = JsonBase;

}


