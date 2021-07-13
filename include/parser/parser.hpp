#pragma once

#include "value/object.hpp"
#include "value/string.hpp"
#include "value/number.hpp"
#include "value/bool.hpp"
#include "value/null.hpp"
#include "value/array.hpp"
#include "value/value_base.hpp"
#include "value/value_warp.hpp"

#include <array>
#include <string>

namespace wjson {

bool isDigit(const char c);

// The first char of value have been read in all functions below except numberParser.
// such as string is "<char><char><char>" , when we call functions readStr or stringParser, the first '\"' have benn read. 
// Thus it will get <char><char><char>" and finish it's work when it meet '\"'.

template <typename R>
std::string readStr(R &reader);

template <typename R>
ValueBase* anyParser(R &reader);

template <typename R>
Object* objectParser(R &reader);

template <typename R>
String* stringParser(R &reader);

template <typename R>
long long readerInteger(R &reader);

template <typename R>
Number* numberParser(R &reader);

template <typename R>
Bool* boolParser(R &reader,bool exp);

template <typename R>
Null* nullParser(R &reader);

template <typename R>
Array* arrayParser(R &reader);

inline bool isDigit(const char c){
	return c>='0' && c<='9';
}

template <typename R>
std::string readStr(R &reader){
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
				case '\\': c = '\\'; break;
				default: throw "unknow escape character";
			}
		}
		ret.push_back(c);
	}
	throw "error when read a string, all char have been read and do not meet a \"";
	return ret;
}

template <typename R>
ValueBase* anyParser(R &reader){
	ValueBase *value;
	char c = reader.LookVCharF();
	switch (c) {
		case 'n': reader.GetChar(); value = nullParser(reader); break;
		case 't': reader.GetChar(); value = boolParser(reader,true); break;
		case 'f': reader.GetChar(); value = boolParser(reader,false); break;
		case '\"': reader.GetChar(); value = stringParser(reader); break;
		case '{': reader.GetChar(); value = objectParser(reader); break;
		case '[': reader.GetChar(); value = arrayParser(reader); break;
		case '\0': throw "there is no value for this key?";
		default: try {
			value = numberParser(reader);
		} catch (std::exception e) {
			throw "unknow value type";
		}
	}
	return value;

}

template <typename R>
Object* objectParser(R &reader){
	Object* ret = new Object;
	auto &obj = *ret;

	char c;
	while(reader.GetVChar(c)){
		if(c == '}') return ret;
		else if(c == '\"'){
			do{
				//parser key
				std::string key = readStr(reader);
				if(reader.GetVChar() != ':') throw " : should after a key";

				//parser value
				auto pptr_ = obj[key];
				delete *pptr_;
				*pptr_ = anyParser(reader);

				//is more ? 
				if(reader.LookVCharF() == ','){
					reader.GetChar();
					if(reader.GetVChar() != '\"') throw " key should after the ',' ";
				}
				else break;
			}while(true);
		}
		else {
			throw "A object's key should start with \"";
			break;
		}
	}
	throw "error when parser a object, all char have been read and do not meet a }";
	return ret;
}


template <typename R>
String* stringParser(R &reader){
	return new String(readStr(reader));
}

template <typename R>
long long readerInteger(R &reader){
	char c = reader.GetVChar();
	bool neg = (c == '-');
	long long num = 0;
	if(c != '-' && c!='+'){
		if(isDigit(c) == false) throw "error when parser a number, should start with - or digit";
		else num = c-'0';
	}
	while(isDigit(reader.LookChar())){
		c = reader.GetChar();
		num *= 10;
		num += c - '0';
	}
	if(neg) num *= -1;
	return num;
}

template <typename R>
Number* numberParser(R &reader){
	long long int_part = readerInteger(reader);
	bool neg = int_part < 0;
	char c = reader.GetChar();

	double decimal_part = 0;
	if(c == '.'){
		double tmp = 0.1;
		while(reader.GetChar(c) && isDigit(c)){
			decimal_part += tmp * (c - '0');
			tmp *= 0.1;
		}
		if(neg) decimal_part *= -1;
	}
	int pow_ten = 1;
	if(c == 'e' || c == 'E'){
		pow_ten = std::pow(10,readerInteger(reader));
	}
	return new Number((decimal_part + int_part) * pow_ten);

}

template <typename R>
Bool* boolParser(R &reader,bool exp){
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
	return new Bool(exp);
	
}

template <typename R>
Null* nullParser(R &reader){
	char c;
	static const std::array<char,3> null_str{'u','l','l'};
	const std::array<char,3> tmp{reader.GetChar(),reader.GetChar(),reader.GetChar()};
	if(tmp != null_str) throw "strange value type";
	return new Null();
}

template <typename R>
Array* arrayParser(R &reader){
	Array *ret = new Array();
	Array &arr = *ret;
	char c = reader.LookVCharF();
	while(c != ']'){
		arr.PushBack(anyParser(reader));
		c = reader.GetVChar();
		if(c != ',' && c!=']') throw "strange char in array";
	}
	return ret;
}

}
