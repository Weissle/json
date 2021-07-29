#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <deque>
#include <stdio.h>
#include <memory>
#include <assert.h>
#include <sstream>
#include <utility>
#include <variant>
#include <unordered_map>
#include <iostream>

#include "utils/reader.h"
#include "value/number.h"

namespace wjson {


enum class ValueType:int{ Null=0,Bool=1,Number=2,String=3,Array=4,Object=5};

using Null = std::nullptr_t;
using Bool = bool;
//using Number = number;
using String = std::string;

class JsonBase;
//using Array = std::vector<JsonBase>;
using Array = std::deque<JsonBase>;
using Object = std::map<std::string,JsonBase>;
using ObjectConstIterator = typename Object::const_iterator;
using ArrayConstIterator = typename Array::const_iterator;
using ObjectIterator = typename Object::iterator;
using ArrayIterator = typename Array::iterator;

struct DumpPackage{
public:
	void operator()(const Null &,std::stringstream *stream_ptr);
	void operator()(const Bool &b,std::stringstream *stream_ptr);
	void operator()(const String &s,std::stringstream *stream_ptr);
	void operator()(const Number &n,std::stringstream *stream_ptr);
	void operator()(const Array &arr,std::stringstream *stream_ptr);
	void operator()(const Object &obj,std::stringstream *stream_ptr);
};

struct PrettyPackage : public DumpPackage{
	char indent_char,indent_num;
	void operator()(const Array &arr,std::stringstream *stream_ptr,int indent_level);
	void operator()(const Object &obj,std::stringstream *stream_ptr,int indent_level);
	void operator()(const JsonBase &json,std::stringstream *stream_ptr,int indent_level);
	void Indent(std::stringstream *stream_ptr,const int indent_level);
};

class JsonBase{

	// a hex char to int '0'~'9' 'a'~'f' 'A'~'F'
	int hex_to_int(const char c);

	// read a hex number which consist of 4 hex char
	unsigned read_4_hex(Reader &reader);

	// From unicode to utf-8
	void unicode_to_utf8(std::string &s, Reader &reader);


	void __parse(Reader &reader,JsonBase &ret);

	std::string read_str(Reader &reader);

	void read_str(Reader &reader,std::string &ret);

	void object_parse(Reader &reader,JsonBase &ret);

	void string_parse(Reader &reader,JsonBase &ret);

	void number_parse(Reader &reader,JsonBase &ret);

	void bool_parse(Reader &reader,bool exp,JsonBase &ret);

	void null_parse(Reader &reader,JsonBase &ret);

	void array_parse(Reader &reader,JsonBase &ret);

	// Below two parse function are used for the user, allow of them will call Parse(const char*,JsonBase &) function.
	// It will check that is any characters rest after the parse process is done.
	// If it is, throw a exception.

	void parse(const char* ptr,JsonBase &ret);

protected:
	using Variant = std::variant<Null,Bool,Number,String,Array,Object>;
	Variant value_;

// Simple function, just write in header.
public:
	JsonBase(){}
	JsonBase(JsonBase &&_rv):value_(std::move(_rv.value_)){}
	JsonBase(const std::nullptr_t b):JsonBase(){}
	JsonBase(const bool b):value_(b){}
	JsonBase(const double d):value_(d){}
	JsonBase(const char *s):value_(s){}
	JsonBase(const std::string &s):value_(s){}

	ValueType type()const { return (ValueType)value_.index(); }

	JsonBase& operator=(const double d){ value_.emplace<Number>(d); return *this;}
	JsonBase& operator=(const int d){ value_.emplace<Number>(d); return *this;}
	JsonBase& operator=(const long long d){ value_.emplace<Number>(d); return *this;}
	JsonBase& operator=(const char *s){ value_.emplace<String>(s); return *this;}
	JsonBase& operator=(JsonBase&& _rv){ value_ = std::move(_rv.value_); return *this;}

	JsonBase(ValueType _type);

// common function
public:

	template<class T>
	JsonBase& operator=(T&& _v);

	size_t size()const;

	template<class T>
	bool is()const;

	template<class T>
	JsonBase& to();

	template<class T>
	const T& get()const;

	template<class T>
	T& get();

	bool operator==(const JsonBase& _rv)const;

// For Object 
public:
	JsonBase& operator[](const std::string &s);
	JsonBase& operator[](std::string &&s);
	void remove(const std::string &s);

	ObjectConstIterator object_begin()const;
	ObjectConstIterator object_end()const;
	ObjectIterator object_begin();
	ObjectIterator object_end();


	void clear();

// For Array
public:
	JsonBase& operator[](const int idx);
	void resize(int s);
	void push_back(JsonBase &&_other );
	ArrayConstIterator array_begin()const;
	ArrayConstIterator array_end()const;

	ArrayIterator array_begin();
	ArrayIterator array_end();

public:

	void dump(std::stringstream &stream)const;

	std::string dump()const;

	void pretty(std::stringstream &stream,int indent_num=2,int indent_char=' ')const;

	std::string pretty(int indent_num=2,int indent_char=' ')const;

	void parse(const std::string &s);

	void parse(const char* ptr);
};

template<class T>
JsonBase& JsonBase::operator=(T&& _v){
	value_ = _v;
	return *this;
}

template<class T>
bool JsonBase::is()const{
	return std::holds_alternative<T>(value_);
}

template<class T>
JsonBase& JsonBase::to(){
	if(is<T>() == false) value_.emplace<T>();
	return *this;
}

template<class T>
const T& JsonBase::get()const{
	return std::get<T>(value_);
}

template<class T>
T& JsonBase::get(){
	return std::get<T>(value_);
}

using Json = JsonBase;

}


