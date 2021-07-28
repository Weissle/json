#include "utils/reader.h"
#include "json.h"
#include <cstring>

namespace wjson {

int JsonBase::hex_to_int(char c){
	if(isdigit(c)) return c - '0';
	else if(c >='a' && c<='f') return 10+c-'a';
	else if(c >='A' && c<='F') return 10+c-'A';
	else throw "unknow hex";
	return 0;
}

unsigned JsonBase::read_4_hex(Reader &reader){
	unsigned ret=0;
	for (int i = 3; i >= 0 ; --i){ 
		ret |= hex_to_int(reader.get_char()) << (i<<2);
	}
	return ret;
}

void JsonBase::unicode_to_utf8(std::string &s,Reader &reader){
	unsigned t = read_4_hex(reader);
	if(t >= 0xD800 && t <= 0xDBFF){
		if(reader.get_char()!= '\\' || reader.get_char()!='u') throw "error when parse unicode";
		unsigned tmp = read_4_hex(reader);
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

void JsonBase::read_str(Reader &reader,std::string &ret){
	char c;
	{
		const char *ptr = reader.get_ptr();
		int len = 0;
		while(ptr[len] != '\"' || ptr[len-1]=='\\' ) ++len;
		ret.reserve(len);
	}
	while(true){
		const char *head = reader.get_ptr();
		const char *ptr = head;
		char c;
		do{
			c = *ptr;
			if(isspace(static_cast<unsigned char>(c)) && c != ' ') throw "not allow space except ' ',please use escape char";
			if(c != '\"' && c != '\\') ++ptr;
			else break;
		}while(true);
		ret.append(head,ptr);
		reader.move_next(ptr-head+1);
		if( c == '\\' ){
			char tmp = reader.get_char();
			switch (tmp) {
				case 'b': c = '\b'; break;
				case 'f': c = '\f'; break;
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				case '"': c = '\"'; break;
				case '/': c = '/'; break;
				case '\\': c = '\\'; break;
				case 'u' : c = 0; unicode_to_utf8(ret, reader); break;
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

std::string JsonBase::read_str(Reader &reader){
	std::string ret;
	read_str(reader,ret);
	return ret;
}

void JsonBase::parse(const char* ptr,JsonBase &ret){
	ret.clear();
	Reader reader(ptr);
	__parse(reader,ret);
	if(reader.get_vchar()) throw "There are chars after the parse process which is not allowed";
}


void JsonBase::__parse(Reader &reader,JsonBase &ret){
	char c = reader.look_vchar_f();
	switch (c) {
		case 'n': null_parse(reader,ret); break;
		case 't': bool_parse(reader,true,ret); break;
		case 'f': bool_parse(reader,false,ret); break;
		case '\"': reader.move_next(); string_parse(reader,ret); break;
		case '{': reader.move_next(); object_parse(reader,ret); break;
		case '[': reader.move_next(); array_parse(reader,ret); break;
		default: 
			if(c == '-' || std::isdigit(c)) number_parse(reader,ret);
			else throw "unknow value type";
	}

}

void JsonBase::parse(const std::string &s){
	parse(s.c_str());
}

void JsonBase::parse(const char* ptr){
	parse(ptr,*this);
}

void JsonBase::object_parse(Reader &reader,JsonBase &ret){
	char c = reader.get_vchar();
	ret.to<Object>();
	while(c == '\"'){
		//parser key
		auto &tmp = ret[read_str(reader)];
		if(reader.get_vchar() != ':') throw " : should after a key";

		//parser value
		__parse(reader,tmp);

		//is more ? 
		c = reader.get_vchar();
		if(c == ','){
			c = reader.get_vchar();
			if(c != '\"') throw "a key should after a comma";
		}
	}
	if(c != '}') throw "need the end of object '}'";
}


void JsonBase::string_parse(Reader &reader,JsonBase &ret){
	read_str(reader,ret.to<String>().get<String>());
}

void JsonBase::number_parse(Reader &reader,JsonBase &ret){
	//Check number is valid
	const char *ptr = reader.get_ptr();
	{
		char c = reader.get_char();
		if(c == '-') c = reader.get_char();
		if(isdigit(c) == false) throw "interger part should have at least one digit";
		if(c == '0' && isdigit(reader.look_char())) throw "if interger part is 0, it should be one 0";
		while(isdigit(c = reader.look_char())) reader.move_next();

		if(c == '.') {
			reader.move_next();
			while(isdigit(c = reader.look_char())) reader.move_next();
		}
		if(c == 'e' || c =='E'){
			reader.move_next();
			c = reader.get_char();
			if(c == '-' || c == '+') c = reader.get_char();
			if(!isdigit(c)) throw "at least one digit should after e or E";
			while(isdigit(reader.look_char())) reader.move_next();
		}
	}
	//const double num = strtod(ptr, nullptr);
	ret.to<Number>();
	ret.get<Number>() = Number(ptr,reader.get_ptr());

}

void JsonBase::bool_parse(Reader &reader,bool exp,JsonBase &ret){
	if(!exp) reader.move_next();
	const char *ptr = reader.get_ptr();

	static constexpr char* true_chars = "true";
	static constexpr char* false_chars = "alse"; //ignore f;
	static const unsigned false_unsigned = *(static_cast<const unsigned*>(static_cast<const void*>(false_chars)));
	static const unsigned true_unsigned = *(static_cast<const unsigned*>(static_cast<const void*>(true_chars)));
	const unsigned tmp = *(static_cast<const unsigned*>(static_cast<const void*>(ptr)));
	if(exp){
		if(tmp != true_unsigned) throw "strange value type";
	}
	else{
		if(tmp != false_unsigned) throw "strange value type";
	}
	reader.move_next(4);
	ret = exp;
}

void JsonBase::null_parse(Reader &reader,JsonBase &ret){
	const char *ptr = reader.get_ptr();
	static const char* null_chars = "null";
	static const unsigned null_unsigned = *(static_cast<const unsigned*>(static_cast<const void*>(null_chars)));
	const unsigned tmp = *(static_cast<const unsigned*>(static_cast<const void*>(ptr)));
	if(tmp != null_unsigned) throw "strange value type";
	reader.move_next(4);
	ret = nullptr;
}

void JsonBase::array_parse(Reader &reader,JsonBase &ret){
	ret.to<Array>();
	char c = reader.look_vchar_f();
	if(c == ']') reader.move_next();
	else {
		auto &arr = ret.get<Array>();
		do{
			arr.emplace_back();
			// ret.PushBack(Parse(reader));
			__parse(reader,arr.back());
			c = reader.get_vchar();
			if(c == ']') break;
			else if(c == ',') continue;
			else throw "error, expect , or ]";
		}while(true);
	}	
}

}
