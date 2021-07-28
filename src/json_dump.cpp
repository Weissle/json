#include "json.h"
#include <variant>

namespace wjson {

void JsonBase::dump(std::stringstream &stream)const{
	std::visit(DumpPackage(), value_,std::variant<std::stringstream*>(&stream));
}

std::string JsonBase::dump()const{
	std::stringstream stream;
	dump(stream);
	return stream.str();
}

void DumpPackage::operator()(const Null &,std::stringstream *stream_ptr){ *stream_ptr<<"null"; }

void DumpPackage::operator()(const Bool &b,std::stringstream *stream_ptr){ *stream_ptr<<((b)? "true":"false"); }

void DumpPackage::operator()(const String &s,std::stringstream *stream_ptr){
	*stream_ptr << '\"';
	for(unsigned char c : s){
        switch (c) {
            case '\"': (*stream_ptr)<<"\\\""; break;
            case '\\': (*stream_ptr)<<"\\\\"; break;
            case '\b': (*stream_ptr)<<"\\b"; break;
            case '\f': (*stream_ptr)<<"\\f"; break;
            case '\n': (*stream_ptr)<<"\\n"; break;
            case '\r': (*stream_ptr)<<"\\r"; break;
            case '\t': (*stream_ptr)<<"\\t"; break;
            default:
                if (c < 0x20) {
                    char buffer[7];
                    sprintf(buffer, "\\u%04X", c);
					(*stream_ptr)<<buffer;
                }
                else
                    (*stream_ptr)<<c;
        }
	}
	(*stream_ptr)<<'\"';
}

void DumpPackage::operator()(const Number &n,std::stringstream *stream_ptr){ n.Dump(*stream_ptr); }

void DumpPackage::operator()(const Array &arr,std::stringstream *stream_ptr){
	*stream_ptr << "[";
	for(auto it=arr.begin(); it!=arr.end();++it){
		if(it != arr.begin()) *stream_ptr<<",";
		auto &tmp = *it;
		(*it).dump(*stream_ptr);
	}
	*stream_ptr << ']';
	return;

}

void DumpPackage::operator()(const Object &obj,std::stringstream *stream_ptr){
	*stream_ptr << "{";
	for(auto it=obj.begin(); it!=obj.end();++it){
		if(it != obj.begin()) *stream_ptr<<",";
		(*this)(it->first,stream_ptr);
		*stream_ptr<<':';
		(it->second).dump(*stream_ptr);
	}
	*stream_ptr << '}';
	return;
}


}
