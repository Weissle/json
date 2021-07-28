#pragma once

namespace wjson {


class Reader {
	const char *ptr;
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
	void move_next(int s=1);
	const char * get_ptr()const;

	char look_char()const;
	char look_vchar_f();
	char get_char();
	char get_vchar();

};

}
