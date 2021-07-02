#pragma once
#include "value_base.hpp"
#include <cstdio>
#include <stdio.h>


namespace wjson {

class None : public ValueBase{

public:
	None();
	void Print(int _)const;
};

inline None::None():ValueBase(ValueType::None){}

inline void None::Print(int _)const{
	std::printf("none");
}

}
