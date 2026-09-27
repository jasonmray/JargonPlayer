
#include "Jargon/BinaryWriterObject.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/System/ByteSwapping.h"

#include <cstdarg>

namespace Jargon{

	BinaryWriterObject::BinaryWriterObject():
		byteSwappingEnabled(false)
	{
	}

	BinaryWriterObject::~BinaryWriterObject(){
	}

	void BinaryWriterObject::setByteSwappingEnabled(bool setting) {
		byteSwappingEnabled = setting;
	}

	bool BinaryWriterObject::getByteSwappingEnabled() const {
		return byteSwappingEnabled;
	}

	void BinaryWriterObject::write(const uint8_t* sourceBuffer, size_t numBytesToWrite) {
		if (byteSwappingEnabled) {
			sourceBuffer += (numBytesToWrite - 1);

			for (size_t i = 0; i < numBytesToWrite; i++) {
				writeByte(*sourceBuffer);
				sourceBuffer--;
			}
		} else {
			writeBytes(sourceBuffer, numBytesToWrite);
		}
	}

	void BinaryWriterObject::write(const char* sourceBuffer, size_t numBytesToWrite) {
		write((const uint8_t*)sourceBuffer, numBytesToWrite);
	}

	void BinaryWriterObject::writeUnswapped(const uint8_t* sourceBuffer, size_t numBytesToWrite) {
		writeBytes(sourceBuffer, numBytesToWrite);
	}

	void BinaryWriterObject::writeChar(char c) {
		writeByte(c);
	}

	void BinaryWriterObject::write8(uint8_t c) {
		writeByte(c);
	}

	void BinaryWriterObject::write16(uint16_t value) {
		if (byteSwappingEnabled) {
			Jargon::System::swap16(&value);
		}

		writeBytes((const uint8_t*)&value, 2);
	}

	void BinaryWriterObject::write32(uint32_t value) {
		if (byteSwappingEnabled) {
			Jargon::System::swap32(&value);
		}

		writeBytes((const uint8_t*)&value, 4);
	}

	void BinaryWriterObject::write64(uint64_t value) {
		if (byteSwappingEnabled) {
			Jargon::System::swap64(&value);
		}

		writeBytes((const uint8_t*)&value, 8);
	}

	void BinaryWriterObject::write8(int8_t c) {
		writeByte(c);
	}

	void BinaryWriterObject::write16(int16_t value) {
		if (byteSwappingEnabled) {
			Jargon::System::swap16(&value);
		}

		writeBytes((const uint8_t*)&value, 2);
	}

	void BinaryWriterObject::write32(int32_t value) {
		if (byteSwappingEnabled) {
			Jargon::System::swap32(&value);
		}

		writeBytes((const uint8_t*)&value, 4);
	}

	void BinaryWriterObject::write64(int64_t value) {
		if (byteSwappingEnabled) {
			Jargon::System::swap64(&value);
		}

		writeBytes((const uint8_t*)&value, 8);
	}

	void BinaryWriterObject::writeFloat(float value) {
		write32(*(uint32_t*)&value);
	}

	void BinaryWriterObject::writeDouble(double value) {
		write64(*(uint64_t*)&value);
	}

	void BinaryWriterObject::write(const std::string& value) {
		write(value.c_str(), value.size());
	}

	void BinaryWriterObject::writeFormatted(const char* format, ...) {
		va_list args;
		va_start(args, format);

		std::string line;
		line = Jargon::StringUtilities::formatVarArgs(format, args);
		write(line);

		va_end(args);
	}

	void BinaryWriterObject::reserve(uint64_t totalBytes) {
	}

	void BinaryWriterObject::flush() {
		return;
	}
}
