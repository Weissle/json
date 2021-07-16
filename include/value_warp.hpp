#pragma once

#include <iostream>
#include <string>

#include "parser/reader.hpp"
#include "parser/parser.hpp"
#include "value_warp_const.hpp"


namespace wjson {


class ValueWarp : public ValueWarpConst{

	template<typename T1,typename T2>
	void SetValue(const ValueType type,const T2 &value_);
	

protected:

public:
	class ObjectIterator : public Map::iterator{
		using Base = Map::iterator;

	public:
		using KVPair = std::pair<const std::string&,ValueWarp>;
		ObjectIterator(const Map::iterator it):Map::iterator(it){}
		const std::string& Key(){ return Base::operator*().first; }
		ValueWarp Value(){ return ValueWarp(Base::operator*().second); }
		KVPair operator*(){ return {Key(),Value()}; }
		std::unique_ptr<KVPair> operator->(){ return std::make_unique<KVPair>(Key(),Value()); }
	};

	class ArrayIterator : public Vector::iterator{
	public:
		ArrayIterator(const Vector::iterator it):Vector::iterator(it){}
		ValueWarp operator*(){ return ValueWarp(Vector::iterator::operator*()); }
		std::unique_ptr<ValueWarp> operator->(){ return std::make_unique<ValueWarp>(Vector::iterator::operator*()); }
	};


public:
	ValueWarp() = default;
	ValueWarp(ValueBasePPtr &pptr_);


	ValueWarp& operator=(ValueBase* ptr_);
	ValueWarp& operator=(const bool _value);
	ValueWarp& operator=(const std::string s);
	ValueWarp& operator=(const char *s);
	ValueWarp& operator=(const LL _value);
	ValueWarp& operator=(const double _value);
	ValueWarp& operator=(const long double _value);
	ValueWarp& operator=(const int _value);
	ValueWarp& operator=(const std::nullptr_t nptr);


	ValueWarp operator[](const std::string s);
	ValueWarp operator[](const int idx);


	std::string& GetString();

	Map& GetObject();

	Vector& GetArray();


	ValueWarp& ToNull();
	ValueWarp& ToObject();
	ValueWarp& ToNumber();
	ValueWarp& ToString();
	ValueWarp& ToArray();
	ValueWarp& ToBool();
	ValueWarp& ToType(const ValueType type);

	void Resize(int s);
	void Remove(const std::string s);


	ObjectIterator ObjectBegin();
	ObjectIterator ObjectEnd();
	ArrayIterator ArrayBegin();
	ArrayIterator ArrayEnd();

	ObjectConstIterator ObjectBegin()const;
	ObjectConstIterator ObjectEnd()const;
	ArrayConstIterator ArrayBegin()const;
	ArrayConstIterator ArrayEnd()const;

	void Parse(const char *s);
	void Parse(const std::string s);
	void ParseAny(const char *s);
	void ParseAny(const std::string s);
	void ParseAny(CharPtrReader &reader);

};

inline ValueWarp::ObjectIterator ValueWarp::ObjectBegin(){
	RightTypeOrThrow(ValueType::Object);
	return STATIC_CAST_TO(Object,pptr).GetValue().begin();
}

inline ValueWarp::ObjectIterator ValueWarp::ObjectEnd(){
	RightTypeOrThrow(ValueType::Object);
	return STATIC_CAST_TO(Object,pptr).GetValue().end();
}

inline ValueWarp::ArrayIterator ValueWarp::ArrayBegin(){
	RightTypeOrThrow(ValueType::Array);
	return STATIC_CAST_TO(Array,pptr).GetValue().begin();
}

inline ValueWarp::ArrayIterator ValueWarp::ArrayEnd(){
	RightTypeOrThrow(ValueType::Array);
	return STATIC_CAST_TO(Array,pptr).GetValue().end();
}

inline ValueWarpConst::ObjectConstIterator ValueWarp::ObjectBegin()const{
	return ValueWarpConst::ObjectBegin();
}

inline ValueWarpConst::ObjectConstIterator ValueWarp::ObjectEnd()const{
	return ValueWarpConst::ObjectEnd();
}

inline ValueWarpConst::ArrayConstIterator ValueWarp::ArrayBegin()const{
	return ValueWarpConst::ArrayBegin();
}

inline ValueWarpConst::ArrayConstIterator ValueWarp::ArrayEnd()const{
	return ValueWarpConst::ArrayEnd();
}

inline ValueWarp::ValueWarp(ValueBasePPtr &pptr_):ValueWarpConst(pptr_){
	if(pptr == nullptr) pptr = std::make_shared<ValueBasePtr>();
}

inline ValueWarp& ValueWarp::ToType(const ValueType type){
	if ((*pptr) == nullptr || (*pptr) -> GetType() != type){
		switch (type) {
			case ValueType::Array : *pptr = std::make_unique<Array>(); break;
			case ValueType::Object : *pptr = std::make_unique<Object>(); break;
			case ValueType::Number : *pptr = std::make_unique<Number>(); break;
			case ValueType::String : *pptr = std::make_unique<String>(); break;
			case ValueType::Bool : *pptr = std::make_unique<Bool>(); break;
			case ValueType::Null : *pptr = nullptr; break;
			default: throw "unknow value type";
		}
	}
	return *this;
}

inline ValueWarp& ValueWarp::ToNull(){
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
		*pptr = std::make_unique<T1>();
	}
	auto ptr = static_cast<T1*>(pptr->get());
	ptr->SetValue(value_);
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

inline ValueWarp& ValueWarp::operator=(const long double _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const int _value){
	SetValue<Number>(ValueType::Number,_value);
	return *this;
}

inline ValueWarp& ValueWarp::operator=(const std::nullptr_t nptr){
	*pptr = nullptr;
	return *this;
}


inline ValueWarp ValueWarp::operator[](const std::string s){
	if (*pptr == nullptr) *pptr = std::make_unique<Object>();
	else RightTypeOrThrow(ValueType::Object);
	return ValueWarp(STATIC_CAST_TO(Object,pptr)[s]);
}


inline ValueWarp ValueWarp::operator[](const int idx){
	RightTypeOrThrow(ValueType::Array);
	return ValueWarp(STATIC_CAST_TO(Array,pptr)[idx]);
}

inline void ValueWarp::Resize(const int s){
	RightTypeOrThrow(ValueType::Array);
	STATIC_CAST_TO(Array,pptr).Resize(s);
}

inline void ValueWarp::Remove(const std::string s){
	RightTypeOrThrow(ValueType::Object);
	STATIC_CAST_TO(Object,pptr).Remove(s);
}



inline std::string& ValueWarp::GetString(){
	RightTypeOrThrow(ValueType::String);
	return STATIC_CAST_TO(String,pptr).GetValue();
}

inline Map& ValueWarp::GetObject(){
	RightTypeOrThrow(ValueType::Object);
	return STATIC_CAST_TO(Object,pptr).GetValue();
}


inline Vector& ValueWarp::GetArray(){
	RightTypeOrThrow(ValueType::Array);
	return STATIC_CAST_TO(Array,pptr).GetValue();
}

inline void ValueWarp::Parse(const char *s){
	CharPtrReader reader(s);
	if( reader.LookVCharF() != '{' ) throw "json should start with { ";
	ParseAny(reader);
}

inline void ValueWarp::Parse(const std::string s){
	Parse(s.c_str());
}

inline void ValueWarp::ParseAny(const char *s){
	CharPtrReader reader(s);
	ParseAny(reader);
}

inline void ValueWarp::ParseAny(const std::string s){
	ParseAny(s.c_str());
}

inline void ValueWarp::ParseAny(CharPtrReader &reader){
	*pptr = nullptr;
	*pptr = AnyParser(reader);
	if(reader.GetVChar() != '\0') throw "unexpected char";
}

}
