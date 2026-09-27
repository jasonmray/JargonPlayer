#pragma once

#include <cstdint>

namespace Jargon{
namespace UTF8Validation {

	bool isValidUtf8(const uint8_t* byteBuffer, size_t numBytes);
}
}

