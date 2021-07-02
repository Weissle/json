#pragma once
#include "value/value_warp.hpp"
#include "value/value_base.hpp"
#include <cstdio>



namespace wjson {

class Bool : public ValueBase{
	bool value_;
public:
	Bool();
	Bool(bool v_);

	bool GetValue()const;
	void SetValue(bool v_);

	constexpr ValueType GetType()const;
	

	void Print(int _)const;
};

inline Bool::Bool():Bool(false){}

inline Bool::Bool(bool _v):ValueBase(ValueType::Bool),value_(_v){}


inline bool Bool::GetValue()const{ return value_; }
inline void Bool::SetValue(bool v_){ value_ = v_; }

inline void Bool::Print(int _)const { 
	if(value_) std::printf("true");
	else std::printf("false");
	return;
};

}
