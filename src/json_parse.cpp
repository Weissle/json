#include "parse/reader.h"
#include "json.h"
#include <cstring>

namespace wjson {

int JsonBase::HexToInt(char c){
	if(isdigit(c)) return c - '0';
	else if(c >='a' && c<='f') return 10+c-'a';
	else if(c >='A' && c<='F') return 10+c-'A';
	else throw "unknow hex";
	return 0;
}

unsigned JsonBase::ReadHex4(Reader &reader){
	unsigned ret=0;
	for (int i = 3; i >= 0 ; --i){ 
		ret |= HexToInt(reader.GetChar()) << (i<<2);
	}
	return ret;
}

void JsonBase::ToUTF8(std::string &s,Reader &reader){
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

void JsonBase::ReadStr(Reader &reader,std::string &ret){
	char c;
	{
		const char *ptr = reader.GetPtr();
		int len = 0;
		while(ptr[len] != '\"' || ptr[len-1]=='\\' ) ++len;
		ret.reserve(len);
	}
	while(true){
		const char *head = reader.GetPtr();
		const char *ptr = head;
		char c;
		do{
			c = *ptr;
			if(isspace(static_cast<unsigned char>(c)) && c != ' ') throw "not allow space except ' ',please use escape char";
			if(c != '\"' && c != '\\') ++ptr;
			else break;
		}while(true);
		ret.append(head,ptr);
		reader.MoveNext(ptr-head+1);
		if( c == '\\' ){
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
		// c == '\"'
		else {
			return;
		}
	}
}

std::string JsonBase::ReadStr(Reader &reader){
	std::string ret;
	ReadStr(reader,ret);
	return ret;
}

void JsonBase::Parse(const std::string &s,JsonBase &ret){
	Parse(s.c_str(),ret);
}

void JsonBase::Parse(const char* ptr,JsonBase &ret){
	ret.Clear();
	Reader reader(ptr);
	__Parse(reader,ret);
	if(reader.GetVChar()) throw "There are chars after the parse process which is not allowed";
}


void JsonBase::__Parse(Reader &reader,JsonBase &ret){
	char c = reader.LookVCharF();
	switch (c) {
		case 'n': NullParse(reader,ret); break;
		case 't': BoolParse(reader,true,ret); break;
		case 'f': BoolParse(reader,false,ret); break;
		case '\"': reader.MoveNext(); StringParse(reader,ret); break;
		case '{': reader.MoveNext(); ObjectParse(reader,ret); break;
		case '[': reader.MoveNext(); ArrayParse(reader,ret); break;
		default: 
			if(c == '-' || std::isdigit(c)) NumberParse(reader,ret);
			else throw "unknow value type";
	}

}

void JsonBase::Parse(const std::string &s){
	Parse(s.c_str());
}

void JsonBase::Parse(const char* ptr){
	Parse(ptr,*this);
}

void JsonBase::ObjectParse(Reader &reader,JsonBase &ret){
	char c = reader.GetVChar();
	ret.To<Object>();
	while(c == '\"'){
		//parser key
		auto &tmp = ret[ReadStr(reader)];
		if(reader.GetVChar() != ':') throw " : should after a key";

		//parser value
		__Parse(reader,tmp);

		//is more ? 
		c = reader.GetVChar();
		if(c == ','){
			c = reader.GetVChar();
			if(c != '\"') throw "a key should after a comma";
		}
	}
	if(c != '}') throw "need the end of object '}'";
}


void JsonBase::StringParse(Reader &reader,JsonBase &ret){
	ReadStr(reader,ret.To<String>().Get<String>());
}

void JsonBase::NumberParse(Reader &reader,JsonBase &ret){
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
			while(isdigit(reader.LookChar())) reader.MoveNext();
		}
	}
	//const double num = strtod(ptr, nullptr);
	ret.To<Number>();
	ret.Get<Number>() = Number(ptr,reader.GetPtr());

}

void JsonBase::BoolParse(Reader &reader,bool exp,JsonBase &ret){
	if(!exp) reader.MoveNext();
	const char *ptr = reader.GetPtr();

	static const char* true_chars = "true";
	static const char* false_chars = "alse"; //ignore f;
	if(exp){
		if(memcmp(ptr,true_chars,4) != 0) throw "strange value type";
	}
	else{
		if(memcmp(ptr,false_chars,4) != 0) throw "strange value type";
	}
	reader.MoveNext(4);
	ret = exp;
}

void JsonBase::NullParse(Reader &reader,JsonBase &ret){
	const char *ptr = reader.GetPtr();
	static const char* null_chars = "null";
	if(memcmp(ptr,null_chars,4) != 0) throw "strange value type";
	reader.MoveNext(4);
	ret = nullptr;
}

void JsonBase::ArrayParse(Reader &reader,JsonBase &ret){
	ret.To<Array>();
	char c = reader.LookVCharF();
	if(c == ']') reader.MoveNext();
	else {
		auto &arr = ret.Get<Array>();
		do{
			arr.emplace_back();
			// ret.PushBack(Parse(reader));
			__Parse(reader,arr.back());
			c = reader.GetVChar();
			if(c == ']') break;
			else if(c == ',') continue;
			else throw "error, expect , or ]";
		}while(true);
	}	
}

}