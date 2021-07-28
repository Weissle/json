#include "json.h"
#include "value/number.h"
#include <variant>


namespace wjson {

JsonBase::JsonBase(ValueType _type){
	switch (_type) {
		case ValueType::Null: to<Null>(); break;
		case ValueType::Bool: to<Bool>(); break;
		case ValueType::Number: to<Number>(); break;
		case ValueType::String: to<String>(); break;
		case ValueType::Object: to<Object>(); break;
		case ValueType::Array: to<Array>(); break;
	}
}


size_t JsonBase::size()const{
	switch (value_.index()) {
		case (int)ValueType::Array : return get<Array>().size();
		case (int)ValueType::Object : return get<Object>().size();
		case (int)ValueType::String : return get<String>().size();
		default: throw "This value has no size.";
	}
}

bool JsonBase::operator==(const JsonBase& _rv)const{
	if(value_.index() != _rv.value_.index()) return false;
	switch (value_.index()) {
		case (int)ValueType::Null : return true;
		case (int)ValueType::Bool : return get<Bool>() == _rv.get<Bool>();
		case (int)ValueType::String : return get<String>() == _rv.get<String>();
		case (int)ValueType::Number : return get<Number>() == _rv.get<Number>();
		case (int)ValueType::Array : return get<Array>() == _rv.get<Array>();
		case (int)ValueType::Object : return get<Object>() == _rv.get<Object>();
		default: return false;
	}
}


ObjectConstIterator JsonBase::object_begin()const{
	return get<Object>().begin();
}

ObjectConstIterator JsonBase::object_end()const{
	return get<Object>().end();
}

ArrayConstIterator JsonBase::array_begin()const{
	return get<Array>().begin();
}

ArrayConstIterator JsonBase::array_end()const{
	return get<Array>().end();
}

ObjectIterator JsonBase::object_begin(){
	return get<Object>().begin();
}

ObjectIterator JsonBase::object_end(){
	return get<Object>().end();
}

ArrayIterator JsonBase::array_begin(){
	return get<Array>().begin();
}

ArrayIterator JsonBase::array_end(){ 
	return get<Array>().end();
}

JsonBase& JsonBase::operator[](const std::string &s){ 
	if( value_.index() == int(ValueType::Null) ) { to<Object>(); }
	return get<Object>()[s];
}

JsonBase& JsonBase::operator[](std::string &&s){ 
	if( value_.index() == int(ValueType::Null) ) { to<Object>(); }
	return get<Object>()[std::move(s)];
}

void JsonBase::clear(){
	value_.emplace<0>(nullptr);
}

void JsonBase::remove(const std::string &s){ get<Object>().erase(s); }

JsonBase& JsonBase::operator[](const int idx){ return get<Array>()[idx]; }

void JsonBase::resize(int s){ get<Array>().resize(s); }

void JsonBase::push_back(JsonBase &&_other ){ get<Array>().emplace_back(std::move(_other)); }

}
