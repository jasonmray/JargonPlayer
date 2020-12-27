
#ifndef JARGON_FUNCTIONAL_H
#define JARGON_FUNCTIONAL_H

#include <algorithm>
#include <iterator>


namespace Jargon{
namespace Functional{

	template<class Src, class Dest, class Func>
	Dest map(Src & src){
		Dest dest;
		dest.reserve(src.size());
		Func func;
		std::transform(src.begin(), src.end(), std::back_inserter(dest), func);
		return dest;
	}

	template<class Src, class Dest, class Func>
	Dest map(Src & src, Func func){
		Dest dest;
		dest.reserve(src.size());
		std::transform(src.begin(), src.end(), std::back_inserter(dest), func);
		return dest;
	}

	template<class Src, class Dest, class Func>
	void map(Src & src, Dest & dest, Func func){
		dest.reserve(dest.size() + src.size());
		std::transform(src.begin(), src.end(), std::back_inserter(dest), func);
	}

	template<class T>
	struct Unity : public std::unary_function<T, T>{
		T operator()(const T & t) const{
			return (T)1;
		}
	};

	template<class T>
	struct NullTransform : public std::unary_function<T, T>{
		T operator()(const T & t) const{
			return t;
		}
	};

	template<class SourceType, class DestType>
	struct StaticCastTransform : public std::unary_function<SourceType, DestType>{
		DestType operator()(const SourceType & t){
			return static_cast<DestType>(t);
		}
	};

	template<class SourceType, class DestType>
	struct ConstructorTransform : public std::unary_function<SourceType, DestType>{
		DestType operator()(const SourceType & t){
			return DestType(t);
		}
	};

}
}

#endif
