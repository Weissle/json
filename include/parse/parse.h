#pragma once

#include "value/number.hpp"
#include "json_base.hpp"
#include "parse/reader.hpp"


#include <cstdlib>
#include <memory>
#include <string>
#include <cctype>

namespace wjson {


int HexToInt(const char c);

unsigned ReadHex4(std::string &s,Reader &reader);

void ToUTF8(Reader &reader);

// The first char of value have been read in all functions below except numberParse.
// such as string is "<char><char><char>" , when we call functions readStr or stringParse, the first '\"' have been read. 
// Thus it will get <char><char><char>" and finish it's work when it meet '\"'.

std::string ReadStr(Reader &reader);

JsonBase Parse(const std::string &s);

JsonBase Parse(const char* ptr);

JsonBase Parse(Reader &reader);

JsonBase ObjectParse(Reader &reader);

JsonBase StringParse(Reader &reader);

JsonBase NumberParse(Reader &reader);

JsonBase BoolParse(Reader &reader,bool exp);

JsonBase NullParse(Reader &reader);

JsonBase ArrayParse(Reader &reader);

}
