#pragma once
#include "value/value_base.hpp"
#include <string>
#include <sstream>




namespace wjson {

class String : public ValueBase{
	std::string str;
public:
	String();
	String(const std::string &s);
	String(std::string &&s);
	String(const char *s);

	const std::string& GetValue()const;
	std::string& GetValue();

	void SetValue(const std::string s);
	void SetValue(std::string &&s);
	void SetValue(const char* s);
	size_t Size()const;

	constexpr ValueType GetType()const;
	void Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const;

	void Print(int _)const;
};

inline String::String():ValueBase(ValueType::String){}

inline String::String(const std::string &s):ValueBase(ValueType::String),str(s){}
inline String::String(std::string&& s):ValueBase(ValueType::String),str(s){}
inline String::String(const char*s):ValueBase(ValueType::String),str(s){}

inline const std::string& String::GetValue()const{ return str; }
inline std::string& String::GetValue(){ return str; }
inline void String::SetValue(std::string &&s){ str = s; }

inline void String::SetValue(const std::string s){ str = s; }
inline void String::SetValue(const char* s){ str = std::string(s); }

inline size_t String::Size()const{ return str.size(); }

inline void String::Print(int _)const { 
	std::printf("\"%s\"",str.c_str());
};
inline void String::Dump(std::stringstream &stream,const int indent_num,const int indent_char,const int indent_level)const{
	stream <<'\"'<<str<<'\"';
}

}
