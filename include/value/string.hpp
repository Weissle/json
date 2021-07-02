#pragma once
#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include <cstdio>
#include <string>



namespace wjson {

class String : public ValueBase{
	std::string str;
public:
	String();
	String(std::string s);

	std::string GetValue()const;

	void SetValue(const std::string s);
	void SetValue(const char* s);

	constexpr ValueType GetType()const;
	

	void Print(int _)const;
};

inline String::String():ValueBase(ValueType::String){}

inline String::String(const std::string s):ValueBase(ValueType::String),str(s){}

inline std::string String::GetValue()const{ return str; }
inline void String::SetValue(const std::string s){ str = s; }
inline void String::SetValue(const char* s){ str = std::string(s); }

inline void String::Print(int _)const { 
	std::printf("\"%s\"",str.c_str());
};

}
