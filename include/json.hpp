#pragma once

#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include "value/object.hpp"

namespace wjson {

// using Json = ValueWarp;

class Json : public ValueWarp{
public:
	Json();

};

inline Json::Json():ValueWarp(){}

}
