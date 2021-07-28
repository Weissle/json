#include "utils/reader.h"
#include <cctype>

namespace wjson{


void Reader::move_next(int s){ ptr += s; }

Reader::Reader(const char *p_):ptr(p_){}

const char * Reader::get_ptr()const{ return ptr; }



// return true means c is not '\0'

char Reader::look_char()const{
	return *ptr;
}

char Reader::look_vchar_f(){
	int idx=0;
	while(isspace(ptr[idx])) ++idx;
	move_next(idx);
	return *ptr;
}


char Reader::get_char(){
	return *(ptr++);
}

char Reader::get_vchar(){
	while(isspace(*ptr)) ++ptr;
	return get_char();
}

}
