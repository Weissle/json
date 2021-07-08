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

// The first char of value have been read in all functions below.
// such as string is "<char><char><char>" , when we call functions readStr or stringParser, the first '\"' have benn read. 
// Thus it will get <char><char><char>" and finish it's work when it meet '\"'.


template <typename R>
std::string readStr(R &reader);

template <typename R>
ValueBase* valueParser(R &reader);

template <typename R>
Object* objectParser(R &reader);

template <typename R>
String* stringParser(R &reader);

template <typename R>
Number* numberParser(R &reader);

template <typename R>
Bool* boolParser(R &reader,bool exp);

template <typename R>
Null* nullParser(R &reader);

template <typename R>
Array* arrayParser(R &reader);

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
	char c = reader.GetVChar();
	switch (c) {
		case 'n': value = nullParser(reader); break;
		case 't': value = boolParser(reader,true); break;
		case 'f': value = boolParser(reader,false); break;
		case '\"': value = stringParser(reader); break;
		case '{': value = objectParser(reader); break;
		case '[': value = arrayParser(reader); break;
		case '\0': throw "there is no value for this key?";
		default: throw "unknow value type";
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
