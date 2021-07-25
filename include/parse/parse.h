#pragma once

#include "value/number.h"
#include "json.h"
#include "parse/reader.h"


#include <cstdlib>
#include <memory>
#include <string>
#include <cctype>

namespace wjson {
// a hex char to int '0'~'9' 'a'~'f' 'A'~'F'
int HexToInt(const char c);

// read a hex number which consist of 4 hex char
unsigned ReadHex4(Reader &reader);

// From unicode to utf-8
void ToUTF8(Reader &reader);

// Below four parse function are used for the user, allow of them will call Parse(const char*,JsonBase &) function.
// It will check that is any characters rest after the parse process is done.
// If it is, throw a exception.
void Parse(const std::string &s,JsonBase &ret);

void Parse(const char* ptr,JsonBase &ret);

JsonBase Parse(const std::string &s);

JsonBase Parse(const char* ptr);

void __Parse(Reader &reader,JsonBase &ret);

std::string ReadStr(Reader &reader);

void ReadStr(Reader &reader,std::string &ret);

void ObjectParse(Reader &reader,JsonBase &ret);

void StringParse(Reader &reader,JsonBase &ret);

void NumberParse(Reader &reader,JsonBase &ret);

void BoolParse(Reader &reader,bool exp,JsonBase &ret);

void NullParse(Reader &reader,JsonBase &ret);

void ArrayParse(Reader &reader,JsonBase &ret);

}
