#pragma once

#include "value/value_base_pptr.hpp"
#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include "value/object.hpp"

namespace wjson {

// using Json = ValueWarp;

template<typename T = Object>
class JsonAny : public ValueWarp{
	ValueBasePPtr vpptr;
public:
	JsonAny();

};

template<typename T>
inline JsonAny<T>::JsonAny(){
	*vpptr = new T();
	ValueWarp::Set(vpptr);
}

using Json = JsonAny<Object>;

}
