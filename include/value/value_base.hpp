#pragma once

#include <map>
#include <string>
#include <stdio.h>
#include <memory>
#include <assert.h>



namespace wjson {


enum class ValueType{ None,Bool,Number,Object,Array,String };
class ValueBase{

protected:
	ValueType type_;
public:
	ValueBase(ValueType t);
	virtual ~ValueBase() = default;

	ValueType GetType()const;
	virtual void Print(int indent=0)const=0;

};

inline ValueBase::ValueBase(ValueType t):type_(t){}

inline ValueType ValueBase::GetType()const{ return type_; }

inline void ValueBase::Print(int indent)const{ return; }

using Map = std::map<std::string,ValueBase**>;

}
