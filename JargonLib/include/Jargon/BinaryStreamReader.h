#pragma once

#include "Jargon/BinaryReaderObject.h"

#include <istream>
#include <cstdint>

namespace Jargon{

	class BinaryStreamReader : public BinaryReaderObject {
		public:

			BinaryStreamReader(std::istream& istr, bool byteSwappingEnabled = false);

			virtual ~BinaryStreamReader();

			bool readCString(std::string& stringOut);
			std::istream& getStream() const;

			bool skipForward(uint64_t numBytes) override;
			bool seekToPosition(int64_t position, SeekMode seekMode) override;
			uint64_t getCurrentPosition() const override;
			uint64_t getNumAvailableBytes() const override;
			bool isAtEnd() const override;

		protected:
			bool peekByte(uint8_t* value) const;
			bool readBytes(uint8_t* destination, size_t numBytes);

		private:
			std::istream& inputStream;
	};

}
