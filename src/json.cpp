#include "json.h"
#include "value/number.h"


namespace wjson {

JsonBase::JsonBase(ValueType _type):JsonBase(){
	// value_ = std::make_unique<Variant>();
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

void JsonBase::Dump(std::stringstream &stream,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{
	switch (value_.index()) {
		case (int)ValueType::Null: stream<<"null"; break;
		case (int)ValueType::Bool: stream<<(Get<bool>()? "true":"false"); break;
		case (int)ValueType::Number: Get<Number>().Dump(stream); break;
		case (int)ValueType::String: Dump(stream,Get<String>()); break;
		case (int)ValueType::Object: Dump(stream,Get<Object>(),pretty,indent_num,indent_char,indent_level); break;
		case (int)ValueType::Array: Dump(stream,Get<Array>(),pretty,indent_num,indent_char,indent_level); break;
	}
}

void JsonBase::Indent(std::stringstream &stream,const int indent_num,const char indent_char,const int indent_level)const{
	for (int i = 0; i < indent_level * indent_num; ++i){ 
		stream<<indent_char;
	}
}

void JsonBase::Dump(std::stringstream &stream,const std::string &s)const{
	stream << '\"' << s << '\"';
}

void JsonBase::Dump(std::stringstream &stream,const Array &arr,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{

	stream << "[";
	if(pretty) stream << '\n';
	for(auto it=arr.begin(); it!=arr.end();++it){
		if(it != arr.begin()) stream<<",";
		if(pretty) {
			stream << '\n';
			Indent(stream,indent_num,indent_char,indent_level+1);
		}
		auto &tmp = *it;
		(*it).Dump(stream,pretty,indent_num,indent_char,indent_level+1);
	}
	if(pretty) {
		stream << '\n';
		Indent(stream,indent_num,indent_char,indent_level);
	}
	stream << ']';
	return;
}

void JsonBase::Dump(std::stringstream &stream,const Object &obj,const bool pretty,const int indent_num,const int indent_char,const int indent_level)const{

	stream << "{";
	if(pretty) stream << '\n';
	for(auto it=obj.begin(); it!=obj.end();++it){
		if(it != obj.begin()) stream<<",";
		if(pretty) {
			stream << '\n';
			Indent(stream,indent_num,indent_char,indent_level+1);
		}
		Dump(stream,it->first);
		stream<<':';
		(it->second).Dump(stream,pretty,indent_num,indent_char,indent_level+1);
	}
	if(pretty) {
		stream << '\n';
		Indent(stream,indent_num,indent_char,indent_level);
	}
	stream << '}';
	return;
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