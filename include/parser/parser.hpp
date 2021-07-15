#pragma once

#include "value/object.hpp"
#include "value/string.hpp"
#include "value/number.hpp"
#include "value/bool.hpp"
#include "value/null.hpp"
#include "value/array.hpp"
#include "value/value_base.hpp"

#include <array>
#include <memory>
#include <string>
#include <cctype>

namespace wjson {


int HexToInt(const char c);
template <typename R>
void Read4hex(std::string &s,R &reader);

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
long long ReadInteger(R &reader,bool allow_plus=true);

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
	c = tolower(c);
	if(c >='a' && c<='f') return 10+c-'a';
	else throw "unknow hex";
	return 0;
}

template <typename R>
void Read4hex(std::string &s,R &reader){
	const char c1 = (HexToInt(reader.GetChar())<< 4) | HexToInt(reader.GetChar());
	const char c2 = (HexToInt(reader.GetChar())<< 4) | HexToInt(reader.GetChar());
	s.push_back(c1);
	s.push_back(c2);
}

template <typename R>
std::string ReadStr(R &reader){
	std::string ret;
	char c;
	while(reader.GetChar(c)){
		if(c == '\"') return ret;
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
				case 'u' : break;
				default: throw "unknow escape character";
			}
		}
		ret.push_back(c);
	}
	throw "error when read a string, all char have been read and do not meet a \"";
	return ret;
}

template <typename R>
ValueBasePtr AnyParser(R &reader){
	ValueBasePtr value;
	char c = reader.LookVCharF();
	switch (c) {
		case 'n': reader.GetChar(); value = NullParser(reader); break;
		case 't': reader.GetChar(); value = BoolParser(reader,true); break;
		case 'f': reader.GetChar(); value = BoolParser(reader,false); break;
		case '\"': reader.GetChar(); value = StringParser(reader); break;
		case '{': reader.GetChar(); value = ObjectParser(reader); break;
		case '[': reader.GetChar(); value = ArrayParser(reader); break;
		case '\0': throw "there is no value for this key?";
		default: try {
			value = NumberParser(reader);
		} catch (std::exception e) {
			throw "unknow value type";
		}
	}
	return std::move(value);

}

template <typename R>
ValueBasePtr ObjectParser(R &reader){
	ValueBasePtr ret(new Object());
	auto &obj = *static_cast<Object*>(ret.get());

	char c = reader.GetVChar();
	if(c == '}') return std::move(ret);
	else if(c == ',') {
		if(reader.GetVChar(c) == '}') return std::move(ret);
		else throw "If the first char in object is comma, the object should be empty";
	}
	else if(c == '\"'){
		do{
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
				if(c == '}')break;
				else if(c=='\"') continue;
				else throw "error occur.";
			}
			else if (c == '}') break;
			else throw "error occur.";
		}while(true);
	}
	else {
		throw "A object's key should start with \"";
	}
	return std::move(ret);
}


template <typename R>
ValueBasePtr StringParser(R &reader){
	String *ret = new String(ReadStr(reader));
	return std::move(ValueBasePtr(ret));
}

template <typename R>
long long ReadInteger(R &reader,bool allow_plus){
	char c = reader.GetVChar();
	if(c == '+' && allow_plus == false) throw "now allow char '+' at the beginning of number";
	bool neg = (c == '-');
	long long num = 0;
	if(c != '-' && c!='+'){
		if(isdigit(c) == false) throw "error when parser a number, should start with - or digit";
		else num = c-'0';
	}
	if(c == '0' && isdigit(reader.LookChar())) throw "number start with digit 0 must is 0";
	while(isdigit(reader.LookChar())){
		c = reader.GetChar();
		num *= 10;
		num += c - '0';
	}
	if(neg) num *= -1;
	return num;
}

template <typename R>
ValueBasePtr NumberParser(R &reader){
	long long int_part = ReadInteger(reader,false);
	bool neg = int_part < 0;
	char c = reader.LookChar();

	long double decimal_part = 0;
	if(c == '.'){
		reader.GetChar();
		double tmp = 0.1;
		while(reader.LookChar(c) && isdigit(c)){
			reader.GetChar();
			decimal_part += tmp * (c - '0');
			tmp *= 0.1;
		}
		if(neg) decimal_part *= -1;
	}
	long double pow_ten = 1;
	if(c == 'e' || c == 'E'){
		reader.GetChar();
		pow_ten = std::pow(10,ReadInteger(reader));
	}
	Number *ret = new Number((decimal_part + int_part) * pow_ten);
	return std::move(ValueBasePtr(ret));

}

template <typename R>
ValueBasePtr BoolParser(R &reader,bool exp){
	static const std::array<char, 3> true_str{'r','u','e'};
	static const std::array<char, 4> false_str{'a','l','s','e'};
	char c;
	if(exp){
		const std::array<char,3> tmp{reader.GetChar(),reader.GetChar(),reader.GetChar()};
		if(tmp != true_str) throw "strange value type";
	}
	else{
		const std::array<char,4> tmp{reader.GetChar(),reader.GetChar(),reader.GetChar(),reader.GetChar()};
		if(tmp != false_str) throw "strange value type";
	}
	Bool *ret = new Bool(exp);
	return std::move(ValueBasePtr(ret));
	
}

template <typename R>
ValueBasePtr NullParser(R &reader){
	char c;
	static const std::array<char,3> null_str{'u','l','l'};
	const std::array<char,3> tmp{reader.GetChar(),reader.GetChar(),reader.GetChar()};
	if(tmp != null_str) throw "strange value type";
	return std::move(ValueBasePtr(nullptr));
}

template <typename R>
ValueBasePtr ArrayParser(R &reader){
	ValueBasePtr ret(new Array());
	auto &arr = *static_cast<Array*>(ret.get());
	char c = reader.LookVCharF();
	if(c == ']'){
		reader.GetChar();
		return std::move(ret);
	}
	else if(c == ',') {
		reader.GetChar();
		if(reader.GetVChar() != ']'){
			throw "A array have only one comma should be empty";
		}
		return std::move(ret);
	}
	while(c != ']'){
		auto tmp = std::make_shared<ValueBasePtr>(AnyParser(reader));
		arr.PushBack(tmp);
		c = reader.GetVChar();
		if(c == ',' && reader.LookVCharF() != ']') continue;
		else if(c == ']') break;
	}
	return std::move(ret);
}

}
