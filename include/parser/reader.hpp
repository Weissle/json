#pragma once

#include <set>
#include <array>

namespace wjson {

constexpr std::array<char,4> whiterSpaces = {'\n','\t',' ','\r'};
class ReaderInterface{

public:
	bool IsWhiteSpace(const char c);

	// just look but not take.
	bool LookChar(char &c);
	char LookChar();

	// Get char include white space
	bool GetChar(char &c);
	char GetChar();

	// Get visible char
	bool GetVChar(char &c);
	char GetVChar();
};

inline bool ReaderInterface::IsWhiteSpace(const char c){
	for (const auto &ws:whiterSpaces){
		if(c==ws) return true;
	}	
	return false;
}

class CharPtrReader : ReaderInterface{
	const char *ptr;
	int idx;
public:
	CharPtrReader(const char *p_);

	// just look but not take.
	bool LookChar(char &c);
	char LookChar();

	// Get char include white space
	bool GetChar(char &c);
	char GetChar();

	// Get visible char
	bool GetVChar(char &c);
	char GetVChar();

};

inline CharPtrReader::CharPtrReader(const char *p_):ptr(p_),idx(0){}

// return true means c is not '\0'
inline bool CharPtrReader::LookChar(char &c){
	c = *(ptr+idx);
	if(c == 0) return false;
	else return true;
}

inline char CharPtrReader::LookChar(){
	return *(ptr+idx);
}

// return true means c is not '\0'
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
	while( GetChar(c) ){
		if(IsWhiteSpace(c) == false) return true;
	}
	return false;
}

inline char CharPtrReader::GetVChar(){
	char tmp;
	GetVChar(tmp);
	return tmp;
}

}
