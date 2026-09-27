#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace Jargon{
namespace Math{

	template<class Target, class Source>
	Target numericCast(Source source) {
		if constexpr (std::is_floating_point_v<Source> && std::is_integral_v<Target>) {
			return static_cast<Target>(std::round(source));
		} else {
			return static_cast<Target>(source);
		}
	}

	template<class T>
	T clamp(T val, T min, T max){
		return std::max(min, std::min(val, max));
	}

	template<class IntType, class FloatType>
	IntType lerp(IntType min, IntType max, FloatType t) {
		return min + (IntType)( (FloatType)(max - min) * t + (FloatType)0.5);
	}

	double lerp(double min, double max, double t);
	double rangeMap(double val, double sourceMin, double sourceMax, double destMin, double destMax);
	int16_t rangeMap(int16_t val, int16_t sourceMin, int16_t sourceMax, int16_t destMin, int16_t destMax);

	template<class SrcType, class DestType, class FloatType = double>
	DestType rangeMap(SrcType val, SrcType sourceMin, SrcType sourceMax, DestType destMin, DestType destMax) {
		return numericCast<DestType>((val - sourceMin) * (destMax - destMin) / (FloatType)(sourceMax - sourceMin) + destMin);
	}
}
}
