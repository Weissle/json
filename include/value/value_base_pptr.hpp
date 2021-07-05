#pragma once

#include "value/value_base.hpp"
// #include <iostream>
namespace wjson {
// int count = 0;
class ValueBasePPtr{
	ValueBase **pptr;
public:
	ValueBasePPtr();
	ValueBasePPtr(ValueBasePPtr &other) = delete;
	ValueBasePPtr(ValueBasePPtr &&other);
	~ValueBasePPtr();

	ValueBase** Get();
	void Set(ValueBase** pptr_);

	ValueBase*& operator*()const;
	operator ValueBase** ()const;

	ValueBasePPtr& operator=(ValueBasePPtr &other) = delete;
	ValueBasePPtr& operator=(ValueBasePPtr &&other);
	
};

inline ValueBasePPtr::ValueBasePPtr(){
	pptr = new ValueBase*();
}

inline ValueBasePPtr::ValueBasePPtr(ValueBasePPtr&& other){
	pptr = other.pptr;
	other.pptr = nullptr;
	
}


inline ValueBase** ValueBasePPtr::Get() { return pptr; }

inline void ValueBasePPtr::Set(ValueBase** pptr_) {
	delete *pptr;
	if(pptr_){
		delete pptr;
		pptr = pptr_;
	}
}

inline ValueBasePPtr::~ValueBasePPtr(){
	// std::cout<<pptr<<" delete id: "<<++count<<std::endl;
	if(pptr) delete *pptr;
	delete pptr;
}

inline ValueBasePPtr::operator ValueBase** ()const{ return pptr; }

inline ValueBase*& ValueBasePPtr::operator*()const{
	return *pptr;
}

inline ValueBasePPtr& ValueBasePPtr::operator=(ValueBasePPtr &&other){
	Set(other.pptr);
	other.pptr = nullptr;
	return *this;
}

}
