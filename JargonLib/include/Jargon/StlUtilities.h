
#ifndef JARGON_STLUTILITIES_H
#define JARGON_STLUTILITIES_H


namespace Jargon{
namespace StlUtilities{


	template<typename MapType>
	const typename MapType::mapped_type & GetOrDefault(const MapType & map, const typename MapType::key_type & key, const typename MapType::mapped_type & defaultValue){
		typename MapType::const_iterator it = map.find(key);
		if( it == map.end() ){
			return defaultValue;
		}
		return it->second;
	}
}
}

#endif
