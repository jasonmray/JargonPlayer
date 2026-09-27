
#include "Jargon/BufferReader.h"

#include <cassert>
#include <cstring>

namespace Jargon{

	BufferReader::BufferReader(const ByteBuffer& sourceBuffer) :
		buffer(sourceBuffer.asConstBuffer()),
		readPosition(0)
	{
	}

	BufferReader::BufferReader(const ConstByteBuffer& sourceBuffer) :
		buffer(sourceBuffer),
		readPosition(0) {
	}

	BufferReader::~BufferReader(){
	}

	size_t BufferReader::getDataSizeBytes() const {
		return buffer.getSizeBytes();
	}

	bool BufferReader::peekAhead(size_t numBytesToSkip, uint8_t* valueOut) const {
		return peekAt(getCurrentPosition() + numBytesToSkip, valueOut);
	}

	bool BufferReader::peekAt(size_t position, uint8_t* valueOut) const {
		if (position >= buffer.getSizeBytes()) {
			return false;
		}
		*valueOut = buffer[position];
		return true;
	}

	bool BufferReader::canSeekToPosition(int64_t position, SeekMode seekMode) const {
		if (seekMode == SeekMode_Beginning && position > 0) {
			return (uint64_t)position <= buffer.getSizeBytes();
		} else if (seekMode == SeekMode_End && position < 0) {
			return (uint64_t)-position <= buffer.getSizeBytes();
		} else if (seekMode == SeekMode_CurrentPosition) {
			if (position < 0) {
				return (uint64_t)(-position) <= readPosition;
			} else {
				return (buffer.getSizeBytes() - readPosition) < (uint64_t)position;
			}
		}
		return false;
	}

	bool BufferReader::readBytes(std::string_view& destination, size_t numBytes) {
		if (isAtEnd()) {
			return false;
		}

		size_t bytesRemaining = buffer.getSizeBytes() - readPosition;
		if (numBytes > bytesRemaining) {
			return false;
		}
		destination = std::string_view((char *)buffer.getBuffer() + readPosition, numBytes);

		readPosition += numBytes;

		return true;
	}

	bool BufferReader::skipForward(uint64_t numBytes) {
		if (buffer.getSizeBytes() - readPosition < numBytes) {
			return false;
		}
		readPosition += numBytes;
		return true;
	}

	bool BufferReader::seekToPosition(int64_t position, SeekMode seekMode) {
		if (seekMode == SeekMode_Beginning && position > 0) {
			if ((uint64_t)position <= buffer.getSizeBytes()) {
				readPosition = (size_t)position;
				return true;
			}
		} else if (seekMode == SeekMode_CurrentPosition) {
			if (position < 0) {
				if ((uint64_t)(-position) > readPosition) {
					return false;
				}
				readPosition += position;
				return true;
			} else {
				return skipForward(position);
			}
		} else if (seekMode == SeekMode_End && position < 0) {
			uint64_t posPosition = (uint64_t)-position;
			if (posPosition > buffer.getSizeBytes()) {
				return false;
			}
			readPosition = buffer.getSizeBytes() - posPosition;
			return true;
		}
		return false;
	}

	uint64_t BufferReader::getCurrentPosition() const {
		return readPosition;
	}

	uint64_t BufferReader::getNumAvailableBytes() const {
		return buffer.getSizeBytes();
	}

	bool BufferReader::isAtEnd() const {
		return getCurrentPosition() == getNumAvailableBytes();
	}

	bool BufferReader::peekByte(uint8_t* value) const {
		if (isAtEnd()) {
			return false;
		}
		*value = buffer[readPosition];
		return true;
	}

	bool BufferReader::readBytes(uint8_t* destination, size_t numBytes) {
		if (isAtEnd()) {
			return false;
		}
		const size_t bytesRemaining = buffer.getSizeBytes() - readPosition;
		if (numBytes > bytesRemaining) {
			return false;
		}
		errno_t result = memcpy_s(destination, numBytes, buffer.getBuffer() + readPosition, numBytes);
		assert(result == 0);
		readPosition += numBytes;
		return true;
	}

}
