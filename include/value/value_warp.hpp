#pragma once

#include <map>
#include <string>
#include "value/value_base.hpp"
#include "value/none.hpp"
#include "value/bool.hpp"
#include "value/object.hpp"
#include "value/string.hpp"


namespace wjson {


class ValueWarp{
	ValueBase **pptr;

	template<typename T>
	T* GetExactPointer(const ValueType type);

public:
	ValueWarp();
	ValueWarp(ValueBase**);

	bool IsBool();
	bool IsNumber();
	bool IsNone();
	bool IsObject();
	bool IsString();

	ValueWarp& operator=(bool _value);
	ValueWarp& operator=(const std::string s);
	ValueWarp& operator=(const char *s);
	ValueWarp operator[](const std::string s);


	void Print(int indent=0)const;

};


inline ValueWarp::ValueWarp(){
	pptr = new ValueBase*();
}

inline ValueWarp::ValueWarp(ValueBase **ptr):pptr(ptr){}

inline bool ValueWarp::IsBool(){ return (*pptr) && (*pptr)->GetType() == ValueType::Bool; }
inline bool ValueWarp::IsNumber(){ return (*pptr) && (*pptr)->GetType() == ValueType::Number; }
inline bool ValueWarp::IsNone(){ return !(*pptr) || (*pptr)->GetType() == ValueType::None; }
inline bool ValueWarp::IsObject(){ return (*pptr) && (*pptr)->GetType() == ValueType::Object; }
inline bool ValueWarp::IsString(){ return (*pptr) && (*pptr)->GetType() == ValueType::String; }

template<typename T>
T* ValueWarp::GetExactPointer(const ValueType type){
	if((*pptr) == nullptr || (*pptr) -> GetType() != type) {
		delete *pptr;
		*pptr = new T();
	}
	return static_cast<T*>(*pptr);
}

ValueWarp& ValueWarp::operator=(bool _value){
	auto ptr = GetExactPointer<Bool>(ValueType::Bool);
	ptr -> SetValue(_value);
	return *this;
}

ValueWarp& ValueWarp::operator=(const char *s){
	auto ptr = GetExactPointer<String>(ValueType::String);
	ptr->SetValue(s);
	return *this;
}

ValueWarp& ValueWarp::operator=(const std::string s){
	auto ptr = GetExactPointer<String>(ValueType::String);
	ptr->SetValue(s);
	return *this;
}

ValueWarp ValueWarp::operator[](const std::string s){
	auto ptr = GetExactPointer<Object>(ValueType::Object);
	return ValueWarp((*ptr)[s]);
}

inline void ValueWarp::Print(int indent)const{
	if((*pptr)) (*pptr)->Print(indent);
}

}
