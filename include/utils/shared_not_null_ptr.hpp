#pragma once

#include <memory>
namespace wjson {

/**
 * This class make sure ptr is not null.
*/
template<class T>
class SharedNotNullPtr :public std::shared_ptr<T> {
public:
	SharedNotNullPtr();
	SharedNotNullPtr(std::shared_ptr<T> p_);
};

template<class T>
inline SharedNotNullPtr<T>::SharedNotNullPtr():std::shared_ptr<T>(std::make_shared<T>()){}

template<class T>
inline SharedNotNullPtr<T>::SharedNotNullPtr(std::shared_ptr<T> p_):std::shared_ptr<T>(std::move(p_)){}

}
