#pragma once

#include <cstdint>

namespace Jargon{

	class BinaryReaderObject {
		public:
			enum SeekMode {
				SeekMode_Beginning,
				SeekMode_CurrentPosition,
				SeekMode_End
			};

			BinaryReaderObject();
			virtual ~BinaryReaderObject();

			bool peek8(char* value) const;
			bool peek8(int8_t* value) const;
			bool peek8(uint8_t* value) const;

			bool read8(char* value);
			bool read8(int8_t* value);
			bool read8(uint8_t* value);

			bool read16(int16_t* value);
			bool read16(uint16_t* value);

			bool read32(int32_t* value);
			bool read32(uint32_t* value);

			bool read64(int64_t* value);
			bool read64(uint64_t* value);

			bool readFloat(float* value);
			bool readDouble(double* value);

			bool readRaw(uint8_t* destination, size_t numBytes);

			void setByteSwappingEnabled(bool byteSwappingEnabled);
			bool isByteSwappingEnabled() const;

			virtual bool skipForward(uint64_t numBytes) = 0;
			virtual bool seekToPosition(int64_t position, SeekMode seekMode = SeekMode_Beginning) = 0;
			virtual uint64_t getCurrentPosition() const = 0;
			virtual uint64_t getNumAvailableBytes() const = 0;
			virtual bool isAtEnd() const = 0;

		protected:
			virtual bool peekByte(uint8_t* value) const = 0;
			virtual bool readBytes(uint8_t* destination, size_t numBytes) = 0;

		private:
			bool byteSwappingEnabled;
	};

}
