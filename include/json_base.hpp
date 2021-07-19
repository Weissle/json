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
#include <iostream>

#include "value/number.hpp"



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
	std::unique_ptr<Variant> value_;
	

	void Indent(std::stringstream &stream,const int indent_num,const char indent_char,const int indent_level)const{
		for (int i = 0; i < indent_level * indent_num; ++i){ 
			stream<<indent_char;
		}
	}

	void Dump(std::stringstream &stream,const std::string &s)const{
		stream << s;
	}

	void Dump(std::stringstream &stream,const Array &arr,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{

		stream << "[";
		if(pretty) stream << '\n';
		for(auto it=arr.begin(); it!=arr.end();++it){
			if(it != arr.begin()) stream<<",";
			if(pretty) {
				stream << '\n';
				Indent(stream,indent_num,indent_char,indent_level+1);
			}
			auto &tmp = *it;
			(*it).Dump(stream,pretty,indent_num,indent_char,indent_level+1);
		}
		if(pretty) {
			stream << '\n';
			Indent(stream,indent_num,indent_char,indent_level);
		}
		stream << ']';
		return;
	}

	void Dump(std::stringstream &stream,const Object &obj,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{

		stream << "{";
		if(pretty) stream << '\n';
		for(auto it=obj.begin(); it!=obj.end();++it){
			if(it != obj.begin()) stream<<",";
			if(pretty) {
				stream << '\n';
				Indent(stream,indent_num,indent_char,indent_level+1);
			}
			(it->second).Dump(stream,pretty,indent_num,indent_char,indent_level+1);
		}
		if(pretty) {
			stream << '\n';
			Indent(stream,indent_num,indent_char,indent_level);
		}
		stream << '}';
		return;
	}

public:
	JsonBase():value_(std::make_unique<Variant>(nullptr)){}
	JsonBase(JsonBase &&_rv){
		value_ = std::move(_rv.value_);
	}
	JsonBase(const std::nullptr_t b):JsonBase(){}
	JsonBase(const bool b):value_(std::make_unique<Variant>(b)){}
	JsonBase(const double d):value_(std::make_unique<Variant>(d)){}
	JsonBase(const double d,const char *beg,const char *end):value_(std::make_unique<Variant>(Number(d,beg,end))){}
	JsonBase(const char *s):value_(std::make_unique<Variant>(s)){}
	JsonBase(const std::string &s):value_(std::make_unique<Variant>(s)){}

	JsonBase(ValueType _type);
	~JsonBase(){ 
		// static int release_count = 0;
		// release_count++;
		// if(release_count % 1000 == 0) 
		// std::cout<<"release : "<<release_count<<std::endl; 
	}

	ValueType GetType()const { return (ValueType)value_->index(); }

	template<class T>
	bool Is()const;

	template<class T>
	JsonBase& To();

	JsonBase& operator=(const std::nullptr_t b){ value_->emplace<Null>(nullptr); return *this;}
	JsonBase& operator=(const bool b){ value_->emplace<Bool>(b); return *this;}
	JsonBase& operator=(const double d){ value_->emplace<Number>(d); return *this;}
	JsonBase& operator=(const int d){ value_->emplace<Number>(d); return *this;}
	JsonBase& operator=(const long long d){ value_->emplace<Number>(d); return *this;}
	JsonBase& operator=(const char *s){ value_->emplace<String>(s); return *this;}
	JsonBase& operator=(const std::string &s){ value_->emplace<String>(s); return *this;}
	JsonBase& operator=(JsonBase&& _rv){ value_ = std::move(_rv.value_); return *this;}

	bool IsBool()const{ return Is<bool>(); }
	bool IsNumber()const{ return Is<Number>(); }
	bool IsNull()const{ return Is<std::nullptr_t>(); }
	bool IsObject()const{ return Is<Object>(); }
	bool IsString()const{ return Is<String>(); }
	bool IsArray()const{ return Is<Array>(); }

	template<class T>
	const T& Get()const;

	template<class T>
	T& Get();

	JsonBase& operator[](const std::string &s){ 
		if( value_->index() == int(ValueType::Null) ) { To<Object>(); }
		return Get<Object>()[s];
	}
	void Remove(const std::string &s){ Get<Object>().erase(s); }

	JsonBase& operator[](const int idx){ return Get<Array>()[idx]; }
	void Resize(int s){ Get<Array>().resize(s); }
	//void PushBack(const JsonBase& _other ) { Get<Array>().push_back(_other); }
	//void PushBack(JsonBase &&_other ) { Get<Array>().push_back(std::move(_other)); }
	void PushBack(JsonBase &&_other ) { Get<Array>().emplace_back(std::move(_other)); }


public:
	ObjectConstIterator ObjectBegin()const{
		return Get<Object>().begin();
	}
	ObjectConstIterator ObjectEnd()const{
		return Get<Object>().end();
	}
	ArrayConstIterator ArrayBegin()const{
		return Get<Array>().begin();
	}
	ArrayConstIterator ArrayEnd()const{
		return Get<Array>().end();
	}

	ObjectIterator ObjectBegin(){
		return Get<Object>().begin();
	}
	ObjectIterator ObjectEnd(){
		return Get<Object>().end();
	}
	ArrayIterator ArrayBegin(){
		return Get<Array>().begin();
	}
	ArrayIterator ArrayEnd(){ 
		return Get<Array>().end();
	}

public:
	size_t Size()const;
	void Dump(std::stringstream &stream,const bool pretty=true,const int indent_num=4,const int indent_char=' ',const int indent_level=0)const;
};

inline JsonBase::JsonBase(ValueType _type):JsonBase(){
	// value_ = std::make_unique<Variant>();
	switch (_type) {
		case ValueType::Null: To<Null>(); break;
		case ValueType::Bool: To<Bool>(); break;
		case ValueType::Number: To<Number>(); break;
		case ValueType::String: To<String>(); break;
		case ValueType::Object: To<Object>(); break;
		case ValueType::Array: To<Array>(); break;
	}
}

template<class T>
bool JsonBase::Is()const{
	return std::holds_alternative<T>(*value_);
}

template<class T>
JsonBase& JsonBase::To(){
	// if(Is<T>() == false) value_->emplace<T>();
	value_->emplace<T>();
	return *this;
}

inline size_t JsonBase::Size()const{
	switch (value_->index()) {
		case (int)ValueType::Array : return Get<Array>().size();
		case (int)ValueType::Object : return Get<Object>().size();
		case (int)ValueType::String : return Get<String>().size();
		default: throw "This value has no size.";
	}
}

template<class T>
const T& JsonBase::Get()const{
	return std::get<T>(*value_);
}

template<class T>
T& JsonBase::Get(){
	return std::get<T>(*value_);
}

inline void JsonBase::Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{
	switch (value_->index()) {
		case (int)ValueType::Null: stream<<"null"; break;
		case (int)ValueType::Bool: stream<<(Get<bool>()? "true":"false"); break;
		case (int)ValueType::Number: Get<Number>().Dump(stream); break;
		case (int)ValueType::String: Dump(stream,Get<String>()); break;
		case (int)ValueType::Object: Dump(stream,Get<Object>(),pretty,indent_num,indent_char,indent_level); break;
		case (int)ValueType::Array: Dump(stream,Get<Array>(),pretty,indent_num,indent_char,indent_level); break;
	}
}


}
