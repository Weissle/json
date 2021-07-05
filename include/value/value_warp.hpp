#pragma once

#include <map>
#include <string>
#include "value/value_base.hpp"
#include "value/null.hpp"
#include "value/bool.hpp"
#include "value/object.hpp"
#include "value/string.hpp"
#include "value/number.hpp"
#include "value/array.hpp"


namespace wjson {

class ValueWarp{
	ValueBase **pptr;

	template<typename T1,typename T2>
	void SetValue(const ValueType type,const T2 &value_);

protected:
	ValueWarp() = default;

	ValueBase** Get();
	void Set(ValueBase**);


public:
	ValueWarp(ValueBase**);

	bool IsBool();
	bool IsNumber();
	bool IsNull();
	bool IsObject();
	bool IsString();
	bool IsArray();

	ValueWarp& operator=(const bool _value);
	ValueWarp& operator=(const std::string s);
	ValueWarp& operator=(const char *s);
	ValueWarp& operator=(const LL _value);
	ValueWarp& operator=(const double _value);
	ValueWarp& operator=(const int _value);
	ValueWarp& operator=(const nullptr_t nptr);

	ValueWarp operator[](const std::string s);
	ValueWarp operator[](const int idx);

	ValueWarp& ToNull();
	ValueWarp& ToObject();
	ValueWarp& ToNumber();
	ValueWarp& ToString();
	ValueWarp& ToArray();
	ValueWarp& ToBool();
	ValueWarp& ToType(const ValueType type);

	void Resize(int s);
	void Dump(std::stringstream &stream, const int indent_num=2, const char indent_char=' ');

};


inline ValueWarp::ValueWarp(ValueBase **ptr):pptr(ptr){}

inline ValueBase** ValueWarp::Get() { return pptr; }
inline void ValueWarp::Set(ValueBase** pp_) { pptr = pp_;}

inline bool ValueWarp::IsBool(){ return (*pptr) && (*pptr)->GetType() == ValueType::Bool; }
inline bool ValueWarp::IsNumber(){ return (*pptr) && (*pptr)->GetType() == ValueType::Number; }
inline bool ValueWarp::IsNull(){ return !(*pptr) || (*pptr)->GetType() == ValueType::Null; }
inline bool ValueWarp::IsObject(){ return (*pptr) && (*pptr)->GetType() == ValueType::Object; }
inline bool ValueWarp::IsString(){ return (*pptr) && (*pptr)->GetType() == ValueType::String; }
inline bool ValueWarp::IsArray(){ return (*pptr) && (*pptr)->GetType() == ValueType::Array; }

ValueWarp& ValueWarp::ToType(const ValueType type){
	if ((*pptr) == nullptr || (*pptr) -> GetType() != type){
		delete *pptr;
		switch (type) {
			case ValueType::Array : *pptr = new Array(); break;
			case ValueType::Object : *pptr = new Object(); break;
			case ValueType::Number : *pptr = new Number(); break;
			case ValueType::String : *pptr = new String(); break;
			case ValueType::Bool : *pptr = new Bool(); break;
			case ValueType::Null : *pptr = new Null(); break;
			default: throw "Unknown value type";
		}
	}
	return *this;
}

inline ValueWarp& ValueWarp::ToNull(){
	return ToType(ValueType::Null);
}
inline ValueWarp& ValueWarp::ToObject(){
	return ToType(ValueType::Object);
}
inline ValueWarp& ValueWarp::ToNumber(){
	return ToType(ValueType::Number);
}
inline ValueWarp& ValueWarp::ToString(){
	return ToType(ValueType::String);
}
inline ValueWarp& ValueWarp::ToArray(){
	return ToType(ValueType::Array);
}
inline ValueWarp& ValueWarp::ToBool(){
	return ToType(ValueType::Bool);
}

template<typename T1,typename T2>
void ValueWarp::SetValue(const ValueType type,const T2 &value_){
	if((*pptr) == nullptr || (*pptr) -> GetType() != type) {
		delete *pptr;
		*pptr = new T1();
	}
	auto ptr = static_cast<T1*>(*pptr);
	ptr->SetValue(value_);
}

ValueWarp& ValueWarp::operator=(const bool _value){
	SetValue<Bool>(ValueType::Bool, _value);
	return *this;
}

ValueWarp& ValueWarp::operator=(const char *s){
	SetValue<String>(ValueType::String, s);
	return *this;
}

ValueWarp& ValueWarp::operator=(const std::string s){
	SetValue<String>(ValueType::String, s);
	return *this;
}

ValueWarp& ValueWarp::operator=(const LL _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

ValueWarp& ValueWarp::operator=(const double _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

ValueWarp& ValueWarp::operator=(const int _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

ValueWarp& ValueWarp::operator=(const nullptr_t nptr){
	delete *pptr;
	*pptr = nullptr;
	return *this;
}

ValueWarp ValueWarp::operator[](const std::string s){
	if (*pptr == nullptr) *pptr = new Object();
	if (IsObject() == false) throw "Value type is not right";
	auto ptr = static_cast<Object*>(*pptr);
	return ValueWarp((*ptr)[s]);
}


ValueWarp ValueWarp::operator[](const int idx){
	if (*pptr == nullptr) *pptr = new Array();
	if (IsArray() == false) throw "Value type is not right";
	auto ptr = static_cast<Array*>(*pptr);
	return ValueWarp((*ptr)[idx]);
}

void ValueWarp::Resize(const int s){
	if (IsArray() == false) throw "Value type is not right";
	auto ptr = static_cast<Array*>(*pptr);
	ptr->Resize(s);
}

inline void ValueWarp::Dump(std::stringstream &stream, const int indent_num, const char indent_char){
	if(*pptr) (*pptr)->Dump(stream,indent_num,indent_char,0);
}

}
