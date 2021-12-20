
#include "Jargon/Math/Utilities.h"

namespace Jargon{
namespace Math{

	double lerp(double min, double max, double t){
		return min + (max - min) * t;
	}

	double rangeMap(double val, double sourceMin, double sourceMax, double destMin, double destMax){
		return (val - sourceMin) / (sourceMax - sourceMin) * (destMax - destMin) + destMin;
	}

	int16_t rangeMap(int16_t val, int16_t sourceMin, int16_t sourceMax, int16_t destMin, int16_t destMax){
		return (int16_t)((val - sourceMin) * (destMax - destMin) / (float)(sourceMax - sourceMin) + destMin + 0.5f);
	}
}
}
