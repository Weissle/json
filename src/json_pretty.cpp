#include "json.h"
#include <sstream>
#include <iomanip>
#include <ios>
namespace wjson {

void JsonBase::pretty(std::stringstream &stream,int indent_num,int indent_char)const{
	PrettyPackage pp;
	pp.indent_char = indent_char;
	pp.indent_num = indent_num;
	pp(*this,&stream,0);
}

std::string JsonBase::pretty(int indent_num,int indent_char)const{
	std::stringstream stream;
	stream  << std::setiosflags(std::ios::fixed) << std::setprecision(6);
	pretty(stream,indent_num,indent_char);
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
	switch (json.type()) {
		case ValueType::Null: DumpPackage::operator()(json.get<Null>(),stream_ptr); return;
		case ValueType::Bool: DumpPackage::operator()(json.get<Bool>(),stream_ptr); return;
		case ValueType::String: DumpPackage::operator()(json.get<String>(),stream_ptr); return;
		case ValueType::Number: DumpPackage::operator()(json.get<Number>(),stream_ptr); return;
		case ValueType::Array: (*this)(json.get<Array>(),stream_ptr,indent_level); return;
		case ValueType::Object: (*this)(json.get<Object>(),stream_ptr,indent_level); return;
	}
}

void PrettyPackage::Indent(std::stringstream *stream_ptr,const int indent_level){
	for (int i = 0; i < indent_level * indent_num; ++i){ 
		*stream_ptr<<indent_char;
	}
}

}
