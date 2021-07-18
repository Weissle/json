#pragma once

#include <set>
#include <cctype>
#include <array>

namespace wjson {


class Reader {
	const char *ptr;
	const char *ori;
public:
	// Look and Get function return false means there is no more char.

	//just look the next visible char, return '\0' if it not exists.
	//rarely used
	//bool LookVChar(char &c)const;
	//char LookVChar()const;

	//just look the next visible char, return '\0' if it not exists. 
	//And white space char will be skip; We can think they have been read.
	// for example, "\t\n a"
	// If we use LookVChar() then get GetChar(), we will get 'a' and '\t' respectively.
	// If we use LookVCharF() then get GetChar(), we will get two 'a'.
	Reader(const char *p_);
	void MoveNext();
	const char * GetPtr()const;
	int GetIdx()const;

	bool LookChar(char &c)const;
	char LookChar()const;
	bool LookVCharF(char &c);
	char LookVCharF();
	bool GetChar(char &c);
	char GetChar();
	bool GetVChar(char &c);
	char GetVChar();

};
inline void Reader::MoveNext(){ ++ptr; }

inline Reader::Reader(const char *p_):ptr(p_),ori(p_){}

inline const char * Reader::GetPtr()const{ return ptr; }

inline int Reader::GetIdx()const{ return ptr - ori; }


// return true means c is not '\0'
inline bool Reader::LookChar(char &c)const{
	c = *ptr;
	if(c == 0) return false;
	else return true;
}

inline char Reader::LookChar()const{
	return *ptr;
}

inline bool Reader::LookVCharF(char &c){
	while(LookChar(c) && isspace(c)) ++ptr;
	return c !='\0';
}

inline char Reader::LookVCharF(){
	char tmp;
	LookVCharF(tmp);
	return tmp;
}

inline bool Reader::GetChar(char &c){
	bool tmp = LookChar(c);
	if(tmp) { ++ptr; return true; }
	else return false;
}

inline char Reader::GetChar(){
	char tmp;
	GetChar(tmp);
	return tmp;
}

inline bool Reader::GetVChar(char &c){
	c = LookVCharF();
	if(c) ++ptr;
	return c != '\0';
}

inline char Reader::GetVChar(){
	char tmp;
	GetVChar(tmp);
	return tmp;
}

}
