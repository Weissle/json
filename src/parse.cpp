#include "value/number.hpp"
#include "json_base.hpp"
#include "parse/reader.hpp"
#include "parse/parse.h"


#include <cstdlib>
#include <memory>
#include <string>
#include <cctype>
#include <iostream>

namespace wjson {


int HexToInt(char c){
	if(isdigit(c)) return c - '0';
	else if(c >='a' && c<='f') return 10+c-'a';
	else if(c >='A' && c<='F') return 10+c-'A';
	else throw "unknow hex";
	return 0;
}

unsigned ReadHex4(Reader &reader){
	unsigned ret=0;
	for (int i = 3; i >= 0 ; --i){ 
		ret |= HexToInt(reader.GetChar()) << (i<<2);
	}
	return ret;
}

void ToUTF8(std::string &s,Reader &reader){
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

std::string ReadStr(Reader &reader){
	std::string ret;
	char c;
	while(reader.GetChar(c)){
		if(c == '\"') return ret;
		else if(isspace(static_cast<unsigned char>(c)) && c != ' ') throw "not allow space except ' ',please use escape char";
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

void Parse(const std::string &s,JsonBase &ret){
	Parse(s.c_str(),ret);
}

void Parse(const char* ptr,JsonBase &ret){
	Reader reader(ptr);
	Parse(reader,ret);
}

void Parse(Reader &reader,JsonBase &ret){
	JsonBase value;
	char c = reader.LookVCharF();
	switch (c) {
		case 'n': reader.MoveNext(); NullParse(reader,ret); break;
		case 't': reader.MoveNext(); BoolParse(reader,true,ret); break;
		case 'f': reader.MoveNext(); BoolParse(reader,false,ret); break;
		case '\"': reader.MoveNext(); StringParse(reader,ret); break;
		case '{': reader.MoveNext(); ObjectParse(reader,ret); break;
		case '[': reader.MoveNext(); ArrayParse(reader,ret); break;
		case '\0': throw "there is no value for this key?";
		default: 
			if(c == '-' || std::isdigit(c)) NumberParse(reader,ret);
			else throw "unknow value type";
	}
}

JsonBase Parse(const std::string &s){
	return Parse(s.c_str());
}

JsonBase Parse(const char* ptr){
	Reader reader(ptr);
	return Parse(reader);
}

JsonBase Parse(Reader &reader){
	JsonBase ret;
	Parse(reader,ret);
	return std::move(ret);
}
void ObjectParse(Reader &reader,JsonBase &ret){
	char c = reader.GetVChar();
	if (ret.Is<Object>() == false) { ret.To<Object>(); }
	while(c == '\"'){
		//parser key
		std::string key = ReadStr(reader);
		if(reader.GetVChar() != ':') throw " : should after a key";

		//parser value
		Parse(reader,ret[key]);

		//is more ? 
		c = reader.GetVChar();
		if(c == ','){
			c = reader.GetVChar();
			if(c != '\"') throw "a key should after a comma";
		}
	}
	if(c != '}') throw "need the end of object '}'";
}


void StringParse(Reader &reader,JsonBase &ret){
	ret = std::move(ReadStr(reader));
}

void NumberParse(Reader &reader,JsonBase &ret){
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
	
	ret = std::move(JsonBase(num,ptr,reader.GetPtr()));

}

void BoolParse(Reader &reader,bool exp,JsonBase &ret){
	if(exp){
		if( reader.GetChar() != 'r' || reader.GetChar() != 'u' || reader.GetChar() != 'e' ) throw "strange value type";
	}
	else{
		if( reader.GetChar() != 'a' || reader.GetChar() != 'l' || reader.GetChar() != 's' ||reader.GetChar() != 'e' ) throw "strange value type";
	}
	ret = exp;
}

void NullParse(Reader &reader,JsonBase &ret){
	if( reader.GetChar() != 'u' || reader.GetChar() != 'l' || reader.GetChar() != 'l' ) throw "strange value type";
	ret = nullptr;
}

void ArrayParse(Reader &reader,JsonBase &ret){
	if (ret.Is<Array>() == false) { ret.To<Array>(); }
	char c = reader.LookVCharF();
	if(c == ']') reader.MoveNext();
	else {
		do{
			ret.PushBack(Parse(reader));
			c = reader.GetVChar();
			if(c == ']') break;
			else if(c == ',') continue;
			else throw "error, expect , or ]";
		}while(c != ']');
	}	
	ret = std::move(ret);
}

}
