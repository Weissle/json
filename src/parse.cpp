#include "value/number.hpp"
#include "value/value_base.hpp"
#include "parser/reader.hpp"
#include "parser/parse.hpp"


#include <cstdlib>
#include <memory>
#include <string>
#include <cctype>

namespace wjson {


inline int HexToInt(char c){
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

JsonBase Parse(const std::string &s){
	return std::move(Parse(s.c_str()));
}

JsonBase Parse(const char* ptr){
	Reader reader(ptr);
	return std::move(Parse(reader));
}

JsonBase Parse(Reader &reader){
	JsonBase value;
	char c = reader.LookVCharF();
	switch (c) {
		case 'n': reader.MoveNext(); value = NullParse(reader); break;
		case 't': reader.MoveNext(); value = BoolParse(reader,true); break;
		case 'f': reader.MoveNext(); value = BoolParse(reader,false); break;
		case '\"': reader.MoveNext(); value = StringParse(reader); break;
		case '{': reader.MoveNext(); value = ObjectParse(reader); break;
		case '[': reader.MoveNext(); value = ArrayParse(reader); break;
		case '\0': throw "there is no value for this key?";
		default: 
			if(c == '-' || std::isdigit(c)) value = NumberParse(reader);
			else throw "unknow value type";
	}
	return std::move(value);

}

JsonBase ObjectParse(Reader &reader){
	JsonBase ret(ValueType::Object);
	char c = reader.GetVChar();
	while(c == '\"'){
		//parser key
		std::string key = ReadStr(reader);
		if(reader.GetVChar() != ':') throw " : should after a key";

		//parser value
		ret[key] = std::move(Parse(reader));

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


JsonBase StringParse(Reader &reader){
	return std::move(JsonBase(ReadStr(reader)));
}



JsonBase NumberParse(Reader &reader){
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

	return std::move(JsonBase(num,ptr,reader.GetPtr()));

}

JsonBase BoolParse(Reader &reader,bool exp){
	if(exp){
		if( reader.GetChar() != 'r' || reader.GetChar() != 'u' || reader.GetChar() != 'e' ) throw "strange value type";
	}
	else{
		if( reader.GetChar() != 'a' || reader.GetChar() != 'l' || reader.GetChar() != 's' ||reader.GetChar() != 'e' ) throw "strange value type";
	}
	Bool *ret = new Bool(exp);
	return std::move(JsonBase(exp));
	
}

JsonBase NullParse(Reader &reader){
	if( reader.GetChar() != 'u' || reader.GetChar() != 'l' || reader.GetChar() != 'l' ) throw "strange value type";
	return std::move(JsonBase(nullptr));
}

JsonBase ArrayParse(Reader &reader){
	JsonBase ret(ValueType::Array);
	char c = reader.LookVCharF();
	if(c == ']') {reader.MoveNext(); return std::move(ret);}
	do{
		ret.PushBack(Parse(reader));
		c = reader.GetVChar();
		if(c == ']') break;
		else if(c == ',') continue;
		else throw "error, expect , or ]";
	}while(c != ']');
	return std::move(ret);
}

}
