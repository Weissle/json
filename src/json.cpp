#include "json.h"
#include "value/number.h"
#include <variant>


namespace wjson {

JsonBase::JsonBase(ValueType _type){
	switch (_type) {
		case ValueType::Null: To<Null>(); break;
		case ValueType::Bool: To<Bool>(); break;
		case ValueType::Number: To<Number>(); break;
		case ValueType::String: To<String>(); break;
		case ValueType::Object: To<Object>(); break;
		case ValueType::Array: To<Array>(); break;
	}
}


size_t JsonBase::Size()const{
	switch (value_.index()) {
		case (int)ValueType::Array : return Get<Array>().size();
		case (int)ValueType::Object : return Get<Object>().size();
		case (int)ValueType::String : return Get<String>().size();
		default: throw "This value has no size.";
	}
}


void JsonBase::Dump(std::stringstream &stream)const{
	std::visit(DumpPackage(), value_,std::variant<std::stringstream*>(&stream));
}
void JsonBase::Indent(std::stringstream &stream,const int indent_num,const char indent_char,const int indent_level)const{
	for (int i = 0; i < indent_level * indent_num; ++i){ 
		stream<<indent_char;
	}
}

ObjectConstIterator JsonBase::ObjectBegin()const{
	return Get<Object>().begin();
}

ObjectConstIterator JsonBase::ObjectEnd()const{
	return Get<Object>().end();
}

ArrayConstIterator JsonBase::ArrayBegin()const{
	return Get<Array>().begin();
}

ArrayConstIterator JsonBase::ArrayEnd()const{
	return Get<Array>().end();
}

ObjectIterator JsonBase::ObjectBegin(){
	return Get<Object>().begin();
}

ObjectIterator JsonBase::ObjectEnd(){
	return Get<Object>().end();
}

ArrayIterator JsonBase::ArrayBegin(){
	return Get<Array>().begin();
}

ArrayIterator JsonBase::ArrayEnd(){ 
	return Get<Array>().end();
}

JsonBase& JsonBase::operator[](const std::string &s){ 
	if( value_.index() == int(ValueType::Null) ) { To<Object>(); }
	return Get<Object>()[s];
}

JsonBase& JsonBase::operator[](std::string &&s){ 
	if( value_.index() == int(ValueType::Null) ) { To<Object>(); }
	return Get<Object>()[std::move(s)];
}

void JsonBase::Clear(){
	value_.emplace<0>(nullptr);
}

void JsonBase::Remove(const std::string &s){ Get<Object>().erase(s); }

JsonBase& JsonBase::operator[](const int idx){ return Get<Array>()[idx]; }

void JsonBase::Resize(int s){ Get<Array>().resize(s); }

void JsonBase::PushBack(JsonBase &&_other ){ Get<Array>().emplace_back(std::move(_other)); }

}
