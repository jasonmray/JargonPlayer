
#ifndef JARGON_MATH_UTILITIES_H
#define JARGON_MATH_UTILITIES_H

#include <algorithm>
#include <cstdint>

namespace Jargon{
namespace Math{

	template<class T>
	T clamp(T val, T min, T max){
		return std::max(min, std::min(val, max));
	}

	double lerp(double min, double max, double t);
	double rangeMap(double val, double sourceMin, double sourceMax, double destMin, double destMax);
	int16_t rangeMap(int16_t val, int16_t sourceMin, int16_t sourceMax, int16_t destMin, int16_t destMax);
}
}

#endif
