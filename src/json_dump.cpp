#include "json.h"
#include <variant>

namespace wjson {

void DumpPackage::operator()(const Null &,std::stringstream *stream_ptr){ *stream_ptr<<"null"; }

void DumpPackage::operator()(const Bool &b,std::stringstream *stream_ptr){ *stream_ptr<<((b)? "true":"false"); }

void DumpPackage::operator()(const String &s,std::stringstream *stream_ptr){ *stream_ptr<< '\"' << s << '\"'; }

void DumpPackage::operator()(const Number &n,std::stringstream *stream_ptr){ n.Dump(*stream_ptr); }

void DumpPackage::operator()(const Array &arr,std::stringstream *stream_ptr){
	*stream_ptr << "[";
	for(auto it=arr.begin(); it!=arr.end();++it){
		if(it != arr.begin()) *stream_ptr<<",";
		auto &tmp = *it;
		(*it).Dump(*stream_ptr);
	}
	*stream_ptr << ']';
	return;

}

void DumpPackage::operator()(const Object &obj,std::stringstream *stream_ptr){
	*stream_ptr << "{";
	for(auto it=obj.begin(); it!=obj.end();++it){
		if(it != obj.begin()) *stream_ptr<<",";
		(*this)(it->first,stream_ptr);
		this->operator()(it->first,stream_ptr);
		//std::visit(DumpPackage(),it->first);
		*stream_ptr<<':';
		(it->second).Dump(*stream_ptr);
	}
	*stream_ptr << '}';
	return;
}

}
