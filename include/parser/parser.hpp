#pragma once

#include "value/object.hpp"
#include "value/string.hpp"
#include "value/number.hpp"
#include "value/bool.hpp"
#include "value/null.hpp"
#include "value/array.hpp"
#include "value/value_base.hpp"

#include <cstdlib>
#include <memory>
#include <string>
#include <cctype>

namespace wjson {


int HexToInt(const char c);

template <typename R>
unsigned ReadHex4(std::string &s,R &reader);

template <typename R>
void ToUTF8(R &reader);

// The first char of value have been read in all functions below except numberParser.
// such as string is "<char><char><char>" , when we call functions readStr or stringParser, the first '\"' have benn read. 
// Thus it will get <char><char><char>" and finish it's work when it meet '\"'.

template <typename R>
std::string ReadStr(R &reader);

template <typename R>
ValueBasePtr AnyParser(R &reader);

template <typename R>
ValueBasePtr ObjectParser(R &reader);

template <typename R>
ValueBasePtr StringParser(R &reader);

template <typename R>
ValueBasePtr NumberParser(R &reader);

template <typename R>
ValueBasePtr BoolParser(R &reader,bool exp);

template <typename R>
ValueBasePtr NullParser(R &reader);

template <typename R>
ValueBasePtr ArrayParser(R &reader);


inline int HexToInt(char c){
	if(isdigit(c)) return c - '0';
	else if(c >='a' && c<='f') return 10+c-'a';
	else if(c >='A' && c<='F') return 10+c-'A';
	else throw "unknow hex";
	return 0;
}

template <typename R>
unsigned ReadHex4(R &reader){
	unsigned ret=0;
	for (int i = 3; i >= 0 ; --i){ 
		ret |= HexToInt(reader.GetChar()) << (i<<2);
	}
	return ret;
}

template <typename R>
void ToUTF8(std::string &s,R &reader){
	unsigned t = ReadHex4(reader);
	if(t >= 0xD800 && t <= 0xDBFF){
		if(reader.GetChar()!= '\\' || reader.GetChar()!='u') throw "error when parse unicode";
		unsigned tmp = ReadHex4(reader);
		if(tmp < 0xDC00 || tmp > 0xDFFF) throw "error when parse unicode";
		t = 0x10000 + (t - 0xD800) * 0x400 + (tmp - 0xDC00);
	}
	if(t <= 0x7f) s.push_back(t);
	else if(t <= 0x7ff){
		s.push_back( (0xc0) | ((t >> 6) & 0x1f) );
		s.push_back( (0x80) | (t & 0x3f) );
	}
	else if((t<=0xd7ff) || (t >=0xE000 && t <= 0xFFFF)){
		s.push_back( 0xe0 | ((t>>12) & 0xf) );
		s.push_back( 0x80 | ((t>>6) & 0x3f) );
		s.push_back( 0x80 | (t & 0x3f) );
	}
	else if(t >= 0x10000 && t <= 0x10FFFF){
		s.push_back( 0xF0 | ((t>>18) & 0x7) );
		s.push_back( 0x80 | ((t>>12) & 0x3f) );
		s.push_back( 0x80 | ((t>>6) & 0x3f) );
		s.push_back( 0x80 | (t & 0x3f) );
	}
	else throw "error when parse unicode";
}

template <typename R>
std::string ReadStr(R &reader){
	std::string ret;
	char c;
	while(reader.GetChar(c)){
		if(c == '\"') return ret;
		else if(isspace(c) && c != ' ') throw "not allow space except ' ',please use escape char";
		else if( c == '\\' ){
			char tmp = reader.GetChar();
			switch (tmp) {
				case 'b': c = '\b'; break;
				case 'f': c = '\f'; break;
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				case '"': c = '\"'; break;
				case '/': c = '/'; break;
				case '\\': c = '\\'; break;
				case 'u' : c = 0; ToUTF8(ret, reader); break;
				default: throw "unknow escape character";
			}
			if(c) ret.push_back(c);
		}
		else ret.push_back(c);
	}
	throw "error when read a string, all char have been read and do not meet a \"";
	return ret;
}

template <typename R>
ValueBasePtr AnyParser(R &reader){
	ValueBasePtr value;
	char c = reader.LookVCharF();
	switch (c) {
		case 'n': reader.MoveNext(); value = NullParser(reader); break;
		case 't': reader.MoveNext(); value = BoolParser(reader,true); break;
		case 'f': reader.MoveNext(); value = BoolParser(reader,false); break;
		case '\"': reader.MoveNext(); value = StringParser(reader); break;
		case '{': reader.MoveNext(); value = ObjectParser(reader); break;
		case '[': reader.MoveNext(); value = ArrayParser(reader); break;
		case '\0': throw "there is no value for this key?";
		default: 
			if(c == '-' || std::isdigit(c)) value = NumberParser(reader);
			else throw "unknow value type";
	}
	return std::move(value);

}

template <typename R>
ValueBasePtr ObjectParser(R &reader){
	ValueBasePtr ret(new Object());
	auto &obj = *static_cast<Object*>(ret.get());
	char c = reader.GetVChar();
	while(c == '\"'){
		//parser key
		std::string key = ReadStr(reader);
		if(reader.GetVChar() != ':') throw " : should after a key";

		//parser value
		auto &pptr_ = obj[key];
		*pptr_ = AnyParser(reader);

		//is more ? 
		c = reader.GetVChar();
		if(c == ','){
			c = reader.GetVChar();
			if(c != '\"') throw "a key should after a comma";
		}
	}
	if(c != '}') throw "need the end of object '}'";
	return std::move(ret);
}


template <typename R>
ValueBasePtr StringParser(R &reader){
	String *ret = new String(ReadStr(reader));
	return std::move(ValueBasePtr(ret));
}



template <typename R>
ValueBasePtr NumberParser(R &reader){
	//Check number is valid
	const char *ptr = reader.GetPtr();
	{
		char c = reader.GetChar();
		if(c == '-') c = reader.GetChar();
		if(isdigit(c) == false) throw "interger part should have at least one digit";
		if(c == '0' && isdigit(reader.LookChar())) throw "if interger part is 0, it should be one 0";
		while(isdigit(c = reader.LookChar())) reader.MoveNext();

		if(c == '.') {
			reader.MoveNext();
			while(isdigit(c = reader.LookChar())) reader.MoveNext();
		}
		if(c == 'e' || c =='E'){
			reader.MoveNext();
			c = reader.GetChar();
			if(c == '-' || c == '+') c = reader.GetChar();
			if(!isdigit(c)) throw "at least one digit should after e or E";
			while(isdigit(c = reader.LookChar())) reader.MoveNext();
		}
	}
	const double num = strtod(ptr, nullptr);
	Number *ret = new Number(num,ptr,reader.GetPtr());
	return std::move(ValueBasePtr(ret));

}

template <typename R>
ValueBasePtr BoolParser(R &reader,bool exp){
	if(exp){
		if( reader.GetChar() != 'r' || reader.GetChar() != 'u' || reader.GetChar() != 'e' ) throw "strange value type";
	}
	else{
		if( reader.GetChar() != 'a' || reader.GetChar() != 'l' || reader.GetChar() != 's' ||reader.GetChar() != 'e' ) throw "strange value type";
	}
	Bool *ret = new Bool(exp);
	return std::move(ValueBasePtr(ret));
	
}

template <typename R>
ValueBasePtr NullParser(R &reader){
	if( reader.GetChar() != 'u' || reader.GetChar() != 'l' || reader.GetChar() != 'l' ) throw "strange value type";
	return std::move(ValueBasePtr(nullptr));
}

template <typename R>
ValueBasePtr ArrayParser(R &reader){
	ValueBasePtr ret(new Array());
	auto &arr = *static_cast<Array*>(ret.get());
	char c = reader.LookVCharF();
	if(c == ']') {reader.MoveNext(); return std::move(ret);}
	do{
		auto tmp = std::make_shared<ValueBasePtr>(AnyParser(reader));
		arr.PushBack(tmp);
		c = reader.GetVChar();
		if(c == ']') break;
		else if(c == ',') continue;
		else throw "error, expect , or ]";
	}while(c != ']');
	return std::move(ret);
}

}
