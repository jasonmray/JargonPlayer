#pragma once

#include "Jargon/BinaryWriterObject.h"
#include <ostream>

namespace Jargon{

	class BinaryStreamWriter : public BinaryWriterObject {
		public:
			BinaryStreamWriter(std::ostream& outputStream, bool byteSwappingEnabled = false);
			virtual ~BinaryStreamWriter();

			std::ostream& getStream();

			bool isAtEnd() const override;
			bool seekToPosition(int64_t position, SeekMode seekMode) override;
			uint64_t getCurrentPosition() const override;
			uint64_t getBytesWritten() const override;

		private:
			std::ostream& outputStream;
			bool byteSwappingEnabled;

			void writeByte(uint8_t byte) override;
			void writeBytes(const uint8_t* data, size_t numBytes) override;
	};

}
