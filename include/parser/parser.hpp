#pragma once

#include "value/object.hpp"
#include "value/string.hpp"
#include "value/number.hpp"
#include "value/bool.hpp"
#include "value/null.hpp"
#include "value/array.hpp"

#include <string>

namespace wjson {

// The first char of value have been read in all functions below.
// such as string is "<char><char><char>" , when we call functions readStr or stringParser, the first '"' have benn read. 
// Thus it will get <char><char><char>" and finish it's work when it meet '"'.

template <typename R>
std::string readStr(R &reader);

template <typename R>
Object* objectParser(R &reader);

template <typename R>
String* stringParser(R &reader);

template <typename R>
Number* numberParser(R &reader);

template <typename R>
Bool* boolParser(R &reader);

template <typename R>
Null* nullParser(R &reader);

template <typename R>
Array* arrayParser(R &reader);

template <typename R>
Object* objectParser(R &reader){
	Object* ret = new Object;
		
	return ret;
}

}
