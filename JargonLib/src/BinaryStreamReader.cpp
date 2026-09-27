
#include "Jargon/BinaryStreamReader.h"
#include "Jargon/System/ByteSwapping.h"

#include <cassert>

namespace Jargon{

	BinaryStreamReader::BinaryStreamReader(std::istream& istr, bool byteSwappingEnabled):
		inputStream(istr)
	{
		setByteSwappingEnabled(byteSwappingEnabled);
	}

	BinaryStreamReader::~BinaryStreamReader() {

	}

	bool BinaryStreamReader::readCString(std::string& stringOut) {
		if (inputStream.fail() || inputStream.eof()){
			return false;
		}

		bool status = true;
		while (true){
			char c;
			inputStream.read(&c, 1);

			if (inputStream.fail()){
				break;
			}

			// if we read at least one char sucessfully, we'll return true
			status = true;

			if (c == '\0'){
				break;
			}

			stringOut += c;
		}

		return status;
	}

	std::istream& BinaryStreamReader::getStream() const {
		return inputStream;
	}

	bool BinaryStreamReader::skipForward(uint64_t numBytes) {
		inputStream.seekg((std::streamoff)numBytes, std::ios_base::cur);
		return !inputStream.fail();
	}

	bool BinaryStreamReader::seekToPosition(int64_t position, SeekMode seekMode) {
		assert(position < std::numeric_limits<std::streamoff>::max());

		inputStream.clear();

		std::ios_base::seekdir seekDir;

		if (seekMode == SeekMode_Beginning) {
			seekDir = std::ostream::beg;
		} else if (seekMode == SeekMode_CurrentPosition) {
			seekDir = std::ostream::cur;
		} else {
			seekDir = std::ostream::end;
		}
		inputStream.seekg((std::streamoff)position, seekDir);

		return !inputStream.fail();
	}

	uint64_t BinaryStreamReader::getCurrentPosition() const {
		return (uint64_t)inputStream.tellg();
	}

	uint64_t BinaryStreamReader::getNumAvailableBytes() const {
		return (uint64_t)inputStream.rdbuf()->in_avail();
	}

	bool BinaryStreamReader::isAtEnd() const {
		return inputStream.eof();
	}

	bool BinaryStreamReader::peekByte(uint8_t* value) const {
		if (inputStream.fail() || inputStream.eof()) {
			return false;
		}

		*value = (uint8_t)inputStream.peek();
		return !inputStream.fail();
	}

	bool BinaryStreamReader::readBytes(uint8_t* destination, size_t numBytes){
		inputStream.read((char*)destination, (std::streamsize)numBytes);
		return !inputStream.fail();
	}
}
