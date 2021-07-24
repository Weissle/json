#include "parse/reader.h"
#include <cctype>

namespace wjson{


void Reader::MoveNext(int s){ ptr += s; }

Reader::Reader(const char *p_):ptr(p_){}

const char * Reader::GetPtr()const{ return ptr; }



// return true means c is not '\0'

char Reader::LookChar()const{
	return *ptr;
}

char Reader::LookVCharF(){
	int idx=0;
	while(isspace(ptr[idx])) ++idx;
	MoveNext(idx);
	return *ptr;
}


char Reader::GetChar(){
	return *(ptr++);
}

char Reader::GetVChar(){
	while(isspace(*ptr)) ++ptr;
	return GetChar();
}

}
