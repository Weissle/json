#pragma once

#include <cstring>
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
	
	void RightTypeOrThrow(ValueType t)const;

protected:
	ValueWarp() = default;

	ValueBase** Get();
	void Set(ValueBase**);


public:
	ValueWarp(ValueBase**);

	bool IsBool()const;
	bool IsNumber()const;
	bool IsNull()const;
	bool IsObject()const;
	bool IsString()const;
	bool IsArray()const;

	ValueWarp& operator=(ValueBase* ptr_);
	ValueWarp& operator=(const bool _value);
	ValueWarp& operator=(const std::string s);
	ValueWarp& operator=(const char *s);
	ValueWarp& operator=(const LL _value);
	ValueWarp& operator=(const double _value);
	ValueWarp& operator=(const int _value);
	ValueWarp& operator=(const std::nullptr_t nptr);

	bool operator==(const bool _value)const;
	bool operator==(const std::string s)const;
	bool operator==(const char *s)const;
	bool operator==(const LL _value)const;
	bool operator==(const double _value)const;
	bool operator==(const int _value)const;
	bool operator==(const std::nullptr_t nptr)const;

	ValueWarp operator[](const std::string s);
	ValueWarp operator[](const int idx);

	bool GetBool()const;
	int GetInteger()const;
	long long GetLongLong()const;
	double GetDouble()const;

	std::string& GetString();
	const std::string& GetString()const;
	Map& GetObject();
	const Map& GetObject()const;
	Vector& GetArray();
	const Vector& GetArray()const;



	ValueWarp& ToNull();
	ValueWarp& ToObject();
	ValueWarp& ToNumber();
	ValueWarp& ToString();
	ValueWarp& ToArray();
	ValueWarp& ToBool();
	ValueWarp& ToType(const ValueType type);

	void Resize(int s);
	void Remove(const std::string s);

	size_t Size()const;
	void Dump(std::stringstream &stream, const int indent_num=2, const char indent_char=' ');

};

inline void ValueWarp::RightTypeOrThrow(ValueType t)const{
	if(((*pptr) == nullptr && t != ValueType::Null) || (*pptr && t != (*pptr)->GetType())) throw "Value type is not right";
}

inline ValueWarp::ValueWarp(ValueBase **ptr):pptr(ptr){}

inline ValueBase** ValueWarp::Get() { return pptr; }
inline void ValueWarp::Set(ValueBase** pp_) { pptr = pp_;}

inline bool ValueWarp::IsBool()const{ return (*pptr) && (*pptr)->GetType() == ValueType::Bool; }
inline bool ValueWarp::IsNumber()const{ return (*pptr) && (*pptr)->GetType() == ValueType::Number; }
inline bool ValueWarp::IsNull()const{ return !(*pptr) || (*pptr)->GetType() == ValueType::Null; }
inline bool ValueWarp::IsObject()const{ return (*pptr) && (*pptr)->GetType() == ValueType::Object; }
inline bool ValueWarp::IsString()const{ return (*pptr) && (*pptr)->GetType() == ValueType::String; }
inline bool ValueWarp::IsArray()const{ return (*pptr) && (*pptr)->GetType() == ValueType::Array; }

inline ValueWarp& ValueWarp::ToType(const ValueType type){
	if ((*pptr) == nullptr || (*pptr) -> GetType() != type){
		delete *pptr;
		switch (type) {
			case ValueType::Array : *pptr = new Array(); break;
			case ValueType::Object : *pptr = new Object(); break;
			case ValueType::Number : *pptr = new Number(); break;
			case ValueType::String : *pptr = new String(); break;
			case ValueType::Bool : *pptr = new Bool(); break;
			case ValueType::Null : *pptr = new Null(); break;
			default: throw "unknow value type";
		}
	}
	return *this;
}

inline ValueWarp& ValueWarp::ToNull(){
	delete *pptr;
	*pptr = nullptr;
	return *this;
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

inline ValueWarp& ValueWarp::operator=(ValueBase* ptr_){
	delete *pptr;
	*pptr = ptr_;
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const bool _value){
	SetValue<Bool>(ValueType::Bool, _value);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const char *s){
	SetValue<String>(ValueType::String, s);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const std::string s){
	SetValue<String>(ValueType::String, s);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const LL _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const double _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const int _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const std::nullptr_t nptr){
	delete *pptr;
	*pptr = nullptr;
	return *this;
}

inline bool ValueWarp::operator==(const bool _value)const{
	RightTypeOrThrow(ValueType::Bool);
	return (*static_cast<Bool*>(*pptr)).GetValue() == _value;
}

inline bool ValueWarp::operator==(const std::string s)const{
	RightTypeOrThrow(ValueType::String);
	return (*static_cast<String*>(*pptr)).GetValue() == s;
}

inline bool ValueWarp::operator==(const char *s)const{
	RightTypeOrThrow(ValueType::String);
	return (*static_cast<String*>(*pptr)).GetValue() == s;
}

inline bool ValueWarp::operator==(const LL _value)const{
	RightTypeOrThrow(ValueType::Number);
	return (*static_cast<Number*>(*pptr)).GetValue() == _value;
}

inline bool ValueWarp::operator==(const double _value)const{
	RightTypeOrThrow(ValueType::Number);
	return (*static_cast<Number*>(*pptr)).GetValue() == _value;
}

inline bool ValueWarp::operator==(const int _value)const{
	RightTypeOrThrow(ValueType::Number);
	return (*static_cast<Number*>(*pptr)).GetValue() == _value;
}

inline bool ValueWarp::operator==(const std::nullptr_t nptr)const{
	return IsNull();
}

inline ValueWarp ValueWarp::operator[](const std::string s){
	if (*pptr == nullptr) *pptr = new Object();
	else RightTypeOrThrow(ValueType::Object);
	auto ptr = static_cast<Object*>(*pptr);
	return ValueWarp((*ptr)[s]);
}


inline ValueWarp ValueWarp::operator[](const int idx){
	RightTypeOrThrow(ValueType::Array);
	auto ptr = static_cast<Array*>(*pptr);
	return ValueWarp((*ptr)[idx]);
}

inline void ValueWarp::Resize(const int s){
	RightTypeOrThrow(ValueType::Array);
	auto ptr = static_cast<Array*>(*pptr);
	ptr->Resize(s);
}

inline void ValueWarp::Remove(const std::string s){
	RightTypeOrThrow(ValueType::Object);
	auto ptr = static_cast<Object*>(*pptr);
	ptr -> Remove(s);
}

inline void ValueWarp::Dump(std::stringstream &stream, const int indent_num, const char indent_char){
	if(*pptr) (*pptr)->Dump(stream,indent_num,indent_char,0);
}

inline bool ValueWarp::GetBool()const{
	RightTypeOrThrow(ValueType::Bool);
	return static_cast<Bool*>(*pptr)->GetValue();
}

inline int ValueWarp::GetInteger()const{
	RightTypeOrThrow(ValueType::Number);
	return static_cast<Number*>(*pptr)->GetValue();
}

inline long long ValueWarp::GetLongLong()const{
	RightTypeOrThrow(ValueType::Number);
	return static_cast<Number*>(*pptr)->GetValue();
}

inline double ValueWarp::GetDouble()const{
	RightTypeOrThrow(ValueType::Number);
	return static_cast<Number*>(*pptr)->GetValue();
}

inline const std::string& ValueWarp::GetString()const{
	RightTypeOrThrow(ValueType::String);
	return static_cast<const String*>(*pptr)->GetValue();
}

inline std::string& ValueWarp::GetString(){
	RightTypeOrThrow(ValueType::String);
	return static_cast<String*>(*pptr)->GetValue();
}

inline Map& ValueWarp::GetObject(){
	RightTypeOrThrow(ValueType::Object);
	return static_cast<Object*>(*pptr)->GetValue();
}

inline const Map& ValueWarp::GetObject()const{
	RightTypeOrThrow(ValueType::Object);
	return static_cast<const Object*>(*pptr)->GetValue();
}

inline const Vector& ValueWarp::GetArray()const{
	RightTypeOrThrow(ValueType::Array);
	return static_cast<const Array*>(*pptr)->GetValue();
}

inline Vector& ValueWarp::GetArray(){
	RightTypeOrThrow(ValueType::Array);
	return static_cast<Array*>(*pptr)->GetValue();
}

inline size_t ValueWarp::Size()const{
	if (!(*pptr)) throw "This type has no Size() function"; 
	ValueType t = (*pptr)->GetType();
	switch (t) {
		case ValueType::Array: return static_cast<Array*>(*pptr)->Size();
		case ValueType::Object: return static_cast<Object*>(*pptr)->Size();
		case ValueType::String: return static_cast<String*>(*pptr)->Size();
		default: throw  "This type has no Size() function"; 
	}
	return 0;
}


}
