#pragma once

#include "Jargon/Buffer.h"
#include "Jargon/BinaryReaderObject.h"

#include <cstdint>
#include <string_view>

namespace Jargon{

	class BufferReader : public BinaryReaderObject{
		public:
			BufferReader(const ByteBuffer& sourceBuffer);
			BufferReader(const ConstByteBuffer& sourceBuffer);
			~BufferReader();

			size_t getDataSizeBytes() const;
			bool peekAhead(size_t numBytesToSkip, uint8_t* valueOut) const;
			bool peekAt(size_t position, uint8_t* valueOut) const;
			bool canSeekToPosition(int64_t position, SeekMode seekMode) const;
			bool readBytes(std::string_view& destination, size_t numBytes);

			bool skipForward(uint64_t numBytes) override;
			bool seekToPosition(int64_t position, SeekMode seekMode) override;
			uint64_t getCurrentPosition() const override;
			uint64_t getNumAvailableBytes() const override;
			bool isAtEnd() const override;

		protected:
			bool peekByte(uint8_t* value) const override;
			bool readBytes(uint8_t* destination, size_t numBytes) override;

		private:
			const ConstByteBuffer& buffer;
			size_t readPosition;
	};

}

