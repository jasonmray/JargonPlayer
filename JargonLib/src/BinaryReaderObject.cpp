
#include "Jargon/BinaryReaderObject.h"
#include "Jargon/System/ByteSwapping.h"

namespace Jargon{

	BinaryReaderObject::BinaryReaderObject():
		byteSwappingEnabled(false)
	{
	}

	BinaryReaderObject::~BinaryReaderObject(){
	}

	bool BinaryReaderObject::peek8(char* value) const {
		return peekByte((uint8_t*)value);
	}

	bool BinaryReaderObject::peek8(int8_t* value) const {
		return peekByte((uint8_t*)value);
	}

	bool BinaryReaderObject::peek8(uint8_t* value) const {
		return peekByte(value);
	}

	bool BinaryReaderObject::read8(char* value) {
		return readBytes((uint8_t*)value, sizeof(*value));
	}

	bool BinaryReaderObject::read8(int8_t* value) {
		return readBytes((uint8_t*)value, sizeof(*value));
	}

	bool BinaryReaderObject::read8(uint8_t* value) {
		return readBytes((uint8_t*)value, sizeof(*value));
	}

	bool BinaryReaderObject::read16(int16_t* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap16(value);
		}
		return true;
	}

	bool BinaryReaderObject::read16(uint16_t* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap16(value);
		}
		return true;
	}

	bool BinaryReaderObject::read32(int32_t* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap32(value);
		}
		return true;
	}

	bool BinaryReaderObject::read32(uint32_t* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap32(value);
		}
		return true;
	}

	bool BinaryReaderObject::read64(int64_t* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap64(value);
		}
		return true;
	}

	bool BinaryReaderObject::read64(uint64_t* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap64(value);
		}
		return true;
	}

	bool BinaryReaderObject::readFloat(float* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap32((uint32_t*)value);
		}
		return true;
	}

	bool BinaryReaderObject::readDouble(double* value) {
		if (!readBytes((uint8_t*)value, sizeof(*value))) {
			return false;
		}
		if (byteSwappingEnabled) {
			Jargon::System::swap64((uint64_t*)value);
		}
		return true;
	}

	bool BinaryReaderObject::readRaw(uint8_t* destination, size_t numBytes) {
		return readBytes(destination, numBytes);
	}

	void BinaryReaderObject::setByteSwappingEnabled(bool byteSwappingEnabled) {
		this->byteSwappingEnabled = byteSwappingEnabled;
	}

	bool BinaryReaderObject::isByteSwappingEnabled() const {
		return byteSwappingEnabled;
	}
}
