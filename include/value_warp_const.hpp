#pragma once

#include "value/value_base.hpp"
#include "value/null.hpp"
#include "value/bool.hpp"
#include "value/object.hpp"
#include "value/string.hpp"
#include "value/number.hpp"
#include "value/array.hpp"

#define STATIC_CAST_TO(T,PPtr) (*static_cast<T*>((PPtr)->get()))

namespace wjson {

class ValueWarpConst{

protected:
	ValueBasePPtr pptr;

	void RightTypeOrThrow(ValueType t)const;
public:
	class ObjectConstIterator : public Map::const_iterator{
		using Base = Map::const_iterator;

	public:
		using KVPair = std::pair<const std::string&,ValueWarpConst>;
		ObjectConstIterator(const Map::const_iterator it):Map::const_iterator(it){}
		const std::string& Key(){ return Base::operator*().first; }
		ValueWarpConst Value(){ return ValueWarpConst(Base::operator*().second); }
		KVPair operator*(){ return {Key(),Value()}; }
		std::unique_ptr<KVPair> operator->(){ return std::make_unique<KVPair>(Key(),Value()); }
	};

	class ArrayConstIterator : public Vector::const_iterator{
	public:
		ArrayConstIterator(const Vector::const_iterator it):Vector::const_iterator(it){}
		ValueWarpConst operator*(){ return ValueWarpConst(Vector::const_iterator::operator*()); }
		std::unique_ptr<ValueWarpConst> operator->(){ return std::make_unique<ValueWarpConst>(Vector::const_iterator::operator*()); }
	};

public:
	ValueWarpConst(ValueBasePPtr);
	ValueWarpConst();

	ValueType GetType()const;
	bool IsBool()const;
	bool IsNumber()const;
	bool IsNull()const;
	bool IsObject()const;
	bool IsString()const;
	bool IsArray()const;

	bool operator==(const bool _value)const;
	bool operator==(const std::string s)const;
	bool operator==(const char *s)const;
	bool operator==(const LL _value)const;
	bool operator==(const double _value)const;
	bool operator==(const int _value)const;
	bool operator==(const std::nullptr_t nptr)const;

	bool GetBool()const;
	int GetInteger()const;
	long long GetLongLong()const;
	double GetDouble()const;

	const std::string& GetString()const;
	const Map& GetObject()const;
	const Vector& GetArray()const;

	size_t Size()const;

	ObjectConstIterator ObjectBegin()const;
	ObjectConstIterator ObjectEnd()const;
	ArrayConstIterator ArrayBegin()const;
	ArrayConstIterator ArrayEnd()const;

	void Dump(std::stringstream &stream, const int indent_num=2, const char indent_char=' ')const;

};

inline ValueWarpConst::ValueWarpConst():pptr(std::make_shared<ValueBasePtr>()){}

inline ValueWarpConst::ValueWarpConst(ValueBasePPtr pptr__):pptr(pptr__){}

inline void ValueWarpConst::RightTypeOrThrow(ValueType t)const{
	if(((*pptr) == nullptr && t != ValueType::Null) || (*pptr && t != (*pptr)->GetType())) throw "Value type is not right";
}

inline ValueType ValueWarpConst::GetType()const{
	if(*pptr == nullptr) return ValueType::Null;
	else return (*pptr)->GetType();
}

inline bool ValueWarpConst::IsBool()const{ return GetType() == ValueType::Bool; }
inline bool ValueWarpConst::IsNumber()const{ return GetType() == ValueType::Number; }
inline bool ValueWarpConst::IsNull()const{ return GetType() == ValueType::Null; }
inline bool ValueWarpConst::IsObject()const{ return GetType() == ValueType::Object; }
inline bool ValueWarpConst::IsString()const{ return GetType() == ValueType::String; }
inline bool ValueWarpConst::IsArray()const{ return GetType() == ValueType::Array; }

inline bool ValueWarpConst::operator==(const bool _value)const{
	RightTypeOrThrow(ValueType::Bool);
	return STATIC_CAST_TO(Bool,pptr).GetValue() == _value;
}

inline bool ValueWarpConst::operator==(const std::string s)const{
	RightTypeOrThrow(ValueType::String);
	return STATIC_CAST_TO(String,pptr).GetValue() == s;
}

inline bool ValueWarpConst::operator==(const char *s)const{
	RightTypeOrThrow(ValueType::String);
	return STATIC_CAST_TO(String,pptr).GetValue() == s;
}

inline bool ValueWarpConst::operator==(const LL _value)const{
	RightTypeOrThrow(ValueType::Number);
	return STATIC_CAST_TO(Number,pptr).GetValue() == _value;
}

inline bool ValueWarpConst::operator==(const double _value)const{
	RightTypeOrThrow(ValueType::Number);
	return STATIC_CAST_TO(Number,pptr).GetValue() == _value;
}

inline bool ValueWarpConst::operator==(const int _value)const{
	RightTypeOrThrow(ValueType::Number);
	return STATIC_CAST_TO(Number,pptr).GetValue() == _value;
}

inline bool ValueWarpConst::operator==(const std::nullptr_t nptr)const{
	return IsNull();
}

inline bool ValueWarpConst::GetBool()const{
	RightTypeOrThrow(ValueType::Bool);
	return STATIC_CAST_TO(Bool,pptr).GetValue();
}

inline int ValueWarpConst::GetInteger()const{
	RightTypeOrThrow(ValueType::Number);
	return STATIC_CAST_TO(Number,pptr).GetValue();
}

inline long long ValueWarpConst::GetLongLong()const{
	RightTypeOrThrow(ValueType::Number);
	return STATIC_CAST_TO(Number,pptr).GetValue();
}

inline double ValueWarpConst::GetDouble()const{
	RightTypeOrThrow(ValueType::Number);
	return STATIC_CAST_TO(Number,pptr).GetValue();
}

inline const std::string& ValueWarpConst::GetString()const{
	RightTypeOrThrow(ValueType::String);
	return STATIC_CAST_TO(String,pptr).GetValue();
}

inline const Map& ValueWarpConst::GetObject()const{
	RightTypeOrThrow(ValueType::Object);
	return STATIC_CAST_TO(Object,pptr).GetValue();
}

inline const Vector& ValueWarpConst::GetArray()const{
	RightTypeOrThrow(ValueType::Array);
	return STATIC_CAST_TO(Array,pptr).GetValue();
}

inline size_t ValueWarpConst::Size()const{
	if (!(*pptr)) throw "This type has no Size() function"; 
	ValueType t = (*pptr)->GetType();
	switch (t) {
		case ValueType::Array: return STATIC_CAST_TO(Array,pptr).Size();
		case ValueType::Object: return STATIC_CAST_TO(Object,pptr).Size();
		case ValueType::String: return STATIC_CAST_TO(String,pptr).Size();
		default: throw  "This type has no Size() function"; 
	}
	return 0;
}

inline ValueWarpConst::ObjectConstIterator ValueWarpConst::ObjectBegin()const{
	RightTypeOrThrow(ValueType::Object);
	return STATIC_CAST_TO(const Object,pptr).GetValue().begin();
}

inline ValueWarpConst::ObjectConstIterator ValueWarpConst::ObjectEnd()const{
	RightTypeOrThrow(ValueType::Object);
	return STATIC_CAST_TO(const Object,pptr).GetValue().end();
}

inline ValueWarpConst::ArrayConstIterator ValueWarpConst::ArrayBegin()const{
	RightTypeOrThrow(ValueType::Array);
	return STATIC_CAST_TO(const Array,pptr).GetValue().begin();
}

inline ValueWarpConst::ArrayConstIterator ValueWarpConst::ArrayEnd()const{
	RightTypeOrThrow(ValueType::Array);
	return STATIC_CAST_TO(const Array,pptr).GetValue().end();
}

inline void ValueWarpConst::Dump(std::stringstream &stream, const int indent_num, const char indent_char)const{
	if(*pptr) (*pptr)->Dump(stream,indent_num,indent_char,0);
}

}
