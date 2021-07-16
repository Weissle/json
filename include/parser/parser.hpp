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
double ReadInteger(R &reader);

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
double ReadInteger(R &reader){
	char c = reader.LookChar();
	if(isdigit(c)==false) throw "at least one digit";
	double ret = 0;
	while(isdigit(c)){
		ret *= 10;
		ret += c - '0';
		reader.MoveNext();
		c = reader.LookChar();
	}
	return ret;
}

template <typename R>
ValueBasePtr NumberParser(R &reader){
	char c;
	bool neg = false;
	double num_part = 0;
	{
		c = reader.LookVCharF();
		if(c == '+') throw "now allow char '+' at the beginning of number";
		else if(c == '-') {neg = true; reader.MoveNext(); c = reader.LookChar();}

		if(c == '0'){
			reader.MoveNext();
			c = reader.LookChar();
			if(isdigit(c)) throw  "number start with digit 0 must is '0'";
		}
		else num_part = ReadInteger(reader);
		
		if(neg) num_part *= -1;
	}


	{
		c = reader.LookChar();
		if (c == '.'){
			double decimal_part = 0;
			reader.GetChar();
			double tmp = 0.1;
			while (reader.LookChar(c) && isdigit(c)){
				reader.GetChar();
				decimal_part += tmp * (c - '0');
				tmp *= 0.1;
			}
			if (neg) decimal_part *= -1;
			num_part += decimal_part;
		}
	}

	double pow_ten = 1;
	{
		if(c == 'e' || c == 'E'){
			reader.MoveNext();
			c = reader.LookChar();
			int exp_neg = 1;
			if(c=='+' || c=='-') reader.MoveNext();
			if(c == '-') exp_neg = -1;

			int exp = ReadInteger(reader) * exp_neg;
			if (std::abs(exp) > 307){
				int dis = std::abs(exp) - 307;
				dis *= (exp<0)? -1:1;
				exp -= dis;
				num_part *= std::pow(10,dis);
			}

			pow_ten = std::pow(10,exp);
		}
	}
	Number *ret = new Number((num_part) * pow_ten);
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
		if(c == ']') break;
		else if(c == ','){
			if(reader.LookVCharF() == ']') break;
		}
		else throw "error, expect , or ]";
	}
	return std::move(ret);
}

}
