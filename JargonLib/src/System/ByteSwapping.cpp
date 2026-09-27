
#include "Jargon/System/ByteSwapping.h"

#define USE_MSVC_BYTESWAP 1

#if USE_MSVC_BYTESWAP
#include <stdlib.h>
#endif


namespace Jargon{
namespace System{

	bool isSystemBigEndian() {
		uint16_t test = 0xABCD;
		return *(uint8_t*)&test == 0xAB;
	}

	bool isSystemLittleEndian() {
		return !isSystemBigEndian();
	}

	bool isSystemNetworkEndian() {
		return isSystemBigEndian();
	}

	uint16_t swap16(uint16_t value) {
#if USE_MSVC_BYTESWAP
		return _byteswap_ushort(value);
#else
		return
			((value >> 8) & 0x00FF) |
			((value << 8) & 0xFF00)
			;
#endif
	}

	uint32_t swap32(uint32_t value) {
#if USE_MSVC_BYTESWAP
		return _byteswap_ulong(value);
#else
		return
			((value >> 24) & 0x000000FF) |
			((value >> 8)  & 0x0000FF00) |
			((value << 8)  & 0x00FF0000) |
			((value << 24) & 0xFF000000)
			;
#endif
	}

	uint64_t swap64(uint64_t value) {
#if USE_MSVC_BYTESWAP
		return _byteswap_uint64(value);
#else

		uint32_t high = uint32_t(value >> 32);
		uint32_t low  = uint32_t(value & 0xFFFFFFFF);
		return
			((uint64_t)swap32(high)) |
			((uint64_t)swap32(low) << 32)
			;
#endif
	}


	int16_t swap16(int16_t value) {
		return (int16_t)swap16((uint16_t)value);
	}

	int32_t swap32(int32_t value) {
		return (int32_t)swap32((uint32_t)value);
	}

	int64_t swap64(int64_t value) {
		return (int64_t)swap64((uint64_t)value);
	}


	void swap16(uint16_t* value) {
		*value = swap16(*value);
	}

	void swap32(uint32_t* value) {
		*value = swap32(*value);
	}

	void swap64(uint64_t* value) {
		*value = swap64(*value);
	}


	void swap16(int16_t* value) {
		swap16((uint16_t*)value);
	}

	void swap32(int32_t* value) {
		swap32((uint32_t*)value);
	}

	void swap64(int64_t* value) {
		swap64((uint64_t*)value);
	}


	void swap(uint8_t* value, size_t sizeBytes) {
		uint8_t temp;
		for (size_t i = 0; i < sizeBytes / 2; i++) {
			temp = value[i];
			value[i] = value[sizeBytes - i - 1];
			value[sizeBytes - i - 1] = temp;
		}
	}
}
}
