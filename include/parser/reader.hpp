#pragma once

#include <set>
#include <cctype>
#include <array>

namespace wjson {

class ReaderInterface{

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
	bool LookVCharF(char &c);
	char LookVCharF();

	// just look but not take.
	bool LookChar(char &c)const;
	char LookChar()const;

	// Get char include white space
	bool GetChar(char &c);
	char GetChar();

	// Get visible char
	bool GetVChar(char &c);
	char GetVChar();
};


class CharPtrReader : ReaderInterface{
	const char *ptr;
	int idx;
public:
	CharPtrReader(const char *p_);
	void MoveNext();

	bool LookChar(char &c)const;
	char LookChar()const;
	bool LookVCharF(char &c);
	char LookVCharF();
	bool GetChar(char &c);
	char GetChar();
	bool GetVChar(char &c);
	char GetVChar();

};
inline void CharPtrReader::MoveNext(){ ++idx; }

inline CharPtrReader::CharPtrReader(const char *p_):ptr(p_),idx(0){}


// return true means c is not '\0'
inline bool CharPtrReader::LookChar(char &c)const{
	c = *(ptr+idx);
	if(c == 0) return false;
	else return true;
}

inline char CharPtrReader::LookChar()const{
	return *(ptr+idx);
}

inline bool CharPtrReader::LookVCharF(char &c){
	while(LookChar(c) && isspace(c)) ++idx;
	return c !='\0';
}

inline char CharPtrReader::LookVCharF(){
	char tmp;
	LookVCharF(tmp);
	return tmp;
}

inline bool CharPtrReader::GetChar(char &c){
	bool tmp = LookChar(c);
	if(tmp) { ++idx; return true; }
	else return false;
}

inline char CharPtrReader::GetChar(){
	char tmp;
	GetChar(tmp);
	return tmp;
}

inline bool CharPtrReader::GetVChar(char &c){
	c = LookVCharF();
	if(c) ++idx;
	return c != '\0';
}

inline char CharPtrReader::GetVChar(){
	char tmp;
	GetVChar(tmp);
	return tmp;
}

}
