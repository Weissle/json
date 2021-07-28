#include "json.h"
#include <sstream>

namespace wjson {

void JsonBase::Pretty(std::stringstream &stream,int indent_num,int indent_char)const{
	PrettyPackage pp;
	pp.indent_char = indent_char;
	pp.indent_num = indent_num;
	pp(*this,&stream,0);
}

std::string JsonBase::Pretty(int indent_num,int indent_char)const{
	std::stringstream stream;
	Pretty(stream,indent_num,indent_char);
	return stream.str();
}

void PrettyPackage::operator()(const Array &arr,std::stringstream *stream_ptr,int indent_level){
	*stream_ptr << "[\n";
	for(auto it=arr.begin(); it!=arr.end();++it){
		if(it != arr.begin()) *stream_ptr<<",\n";
		Indent(stream_ptr,indent_level+1);
		auto &tmp = *it;
		this->operator()(*it,stream_ptr,indent_level+1);
	}
	*stream_ptr << '\n';
	Indent(stream_ptr,indent_level);
	*stream_ptr << ']';
	return;

}

void PrettyPackage::operator()(const Object &obj,std::stringstream *stream_ptr,int indent_level){
	*stream_ptr << "{\n";
	for(auto it=obj.begin(); it!=obj.end();++it){
		if(it != obj.begin()) *stream_ptr<<",\n";
		Indent(stream_ptr,indent_level+1);
		DumpPackage::operator()(it->first,stream_ptr);
		*stream_ptr<<':';
		(*this)(it->second,stream_ptr,indent_level+1);
	}
	*stream_ptr << '\n';
	Indent(stream_ptr,indent_level);
	*stream_ptr << '}';
	return;

}

void PrettyPackage::operator()(const JsonBase &json,std::stringstream *stream_ptr,int indent_level){
	switch (json.GetType()) {
		case ValueType::Null: DumpPackage::operator()(json.Get<Null>(),stream_ptr); return;
		case ValueType::Bool: DumpPackage::operator()(json.Get<Bool>(),stream_ptr); return;
		case ValueType::String: DumpPackage::operator()(json.Get<String>(),stream_ptr); return;
		case ValueType::Number: DumpPackage::operator()(json.Get<Number>(),stream_ptr); return;
		case ValueType::Array: (*this)(json.Get<Array>(),stream_ptr,indent_level); return;
		case ValueType::Object: (*this)(json.Get<Object>(),stream_ptr,indent_level); return;
	}
}

void PrettyPackage::Indent(std::stringstream *stream_ptr,const int indent_level){
	for (int i = 0; i < indent_level * indent_num; ++i){ 
		*stream_ptr<<indent_char;
	}
}

}
