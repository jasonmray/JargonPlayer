#pragma once

#include <cstdint>

namespace Jargon{
namespace System{

	bool isSystemBigEndian();
	bool isSystemLittleEndian();
	bool isSystemNetworkEndian();

	uint16_t swap16(uint16_t value);
	uint32_t swap32(uint32_t value);
	uint64_t swap64(uint64_t value);

	int16_t swap16(int16_t value);
	int32_t swap32(int32_t value);
	int64_t swap64(int64_t value);

	void swap16(uint16_t* value);
	void swap32(uint32_t* value);
	void swap64(uint64_t* value);

	void swap16(int16_t* value);
	void swap32(int32_t* value);
	void swap64(int64_t* value);

	void swap(uint8_t* value, size_t sizeBytes);

}
}
