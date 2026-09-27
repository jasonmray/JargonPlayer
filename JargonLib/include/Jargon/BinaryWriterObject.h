#pragma once

#include <cstdint>
#include <string>

namespace Jargon{

	class BinaryWriterObject{
		public:
			enum SeekMode {
				SeekMode_Beginning,
				SeekMode_CurrentPosition,
				SeekMode_End
			};

			BinaryWriterObject();
			virtual ~BinaryWriterObject();

			virtual void setByteSwappingEnabled(bool setting);
			virtual bool getByteSwappingEnabled() const;
			virtual void write(const uint8_t* sourceBuffer, size_t numBytesToWrite);
			virtual void write(const char* sourceBuffer, size_t numBytesToWrite);
			virtual void writeUnswapped(const uint8_t* sourceBuffer, size_t numBytesToWrite);
			virtual void writeChar(char c);
			virtual void write8(uint8_t c);
			virtual void write16(uint16_t value);
			virtual void write32(uint32_t value);
			virtual void write64(uint64_t value);
			virtual void write8(int8_t c);
			virtual void write16(int16_t value);
			virtual void write32(int32_t value);
			virtual void write64(int64_t value);
			virtual void writeFloat(float value);
			virtual void writeDouble(double value);
			virtual void write(const std::string& value);
			virtual void writeFormatted(const char* format, ...);

			virtual void reserve(uint64_t totalBytes);
			virtual void flush();

			virtual bool isAtEnd() const = 0;

			virtual bool seekToPosition(int64_t position, SeekMode seekMode = SeekMode_Beginning) = 0;
			virtual uint64_t getCurrentPosition() const = 0;
			virtual uint64_t getBytesWritten() const = 0;

		protected:
			virtual void writeByte(uint8_t byte) = 0;
			virtual void writeBytes(const uint8_t* data, size_t numBytes) = 0;

		private:
			bool byteSwappingEnabled;
	};

}
