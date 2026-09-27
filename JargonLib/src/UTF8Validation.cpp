
#include "Jargon/UTF8Validation.h"

#include <cstdint>
#include <cstring>

namespace Jargon{
namespace UTF8Validation {

	/*
	The following was adapted from the is_utf8 library
	https://github.com/simdutf/is_utf8/



	Original license text:
	============================================================================
	Copyright 2022 The is_utf8 authors

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	============================================================================

	*/
	bool isValidUtf8(const uint8_t* byteBuffer, size_t numBytes) {

		const uint8_t* data = byteBuffer;

		uint64_t pos = 0;
		uint32_t code_point = 0;

		while (pos < numBytes) {

			// check if the next 8 bytes are ascii.
			uint64_t next_pos = pos + 16;
			if (next_pos <= numBytes) { // if it is safe to read 8 more bytes, check that they are ascii
				uint64_t v1;
				std::memcpy(&v1, data + pos, sizeof(uint64_t));

				uint64_t v2;
				std::memcpy(&v2, data + pos + sizeof(uint64_t), sizeof(uint64_t));

				uint64_t v{ v1 | v2 };
				if ((v & 0x8080808080808080) == 0) {
					pos = next_pos;
					continue;
				}
			}

			unsigned char byte = data[pos];

			while (byte < 0b10000000) {
				if (++pos == numBytes) {
					return true;
				}
				byte = data[pos];
			}

			if ((byte & 0b11100000) == 0b11000000) {
				next_pos = pos + 2;
				if (next_pos > numBytes) {
					return false;
				}

				if ((data[pos + 1] & 0b11000000) != 0b10000000) {
					return false;
				}

				// range check
				code_point = (byte & 0b00011111) << 6 | (data[pos + 1] & 0b00111111);
				if ((code_point < 0x80) || (0x7ff < code_point)) {
					return false;
				}
			} else if ((byte & 0b11110000) == 0b11100000) {
				next_pos = pos + 3;
				if (next_pos > numBytes) {
					return false;
				}

				if ((data[pos + 1] & 0b11000000) != 0b10000000) {
					return false;
				}

				if ((data[pos + 2] & 0b11000000) != 0b10000000) {
					return false;
				}

				// range check
				code_point =
					(byte & 0b00001111) << 12 |
					(data[pos + 1] & 0b00111111) << 6 |
					(data[pos + 2] & 0b00111111)
				;

				if (
					(code_point < 0x800) || (0xffff < code_point) ||
					(0xd7ff < code_point && code_point < 0xe000)
				) {
					return false;
				}

			} else if ((byte & 0b11111000) == 0b11110000) { // 0b11110000
				next_pos = pos + 4;
				if (next_pos > numBytes) {
					return false;
				}

				if ((data[pos + 1] & 0b11000000) != 0b10000000) {
					return false;
				}

				if ((data[pos + 2] & 0b11000000) != 0b10000000) {
					return false;
				}

				if ((data[pos + 3] & 0b11000000) != 0b10000000) {
					return false;
				}

				// range check
				code_point =
					(byte & 0b00000111) << 18 |
					(data[pos + 1] & 0b00111111) << 12 |
					(data[pos + 2] & 0b00111111) << 6 |
					(data[pos + 3] & 0b00111111)
				;

				if (code_point <= 0xffff || 0x10ffff < code_point) {
					return false;
				}
			} else {
				// we may have a continuation
				return false;
			}
			pos = next_pos;
		}
		return true;
	}


}
}
