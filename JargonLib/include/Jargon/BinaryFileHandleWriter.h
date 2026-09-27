#pragma once

#include "Jargon/BinaryWriterObject.h"

#include <cstdio>

namespace Jargon{

	class BinaryFileHandleWriter : public BinaryWriterObject {
		public:
			BinaryFileHandleWriter(std::FILE* outputFile, bool byteSwappingEnabled = false);
			virtual ~BinaryFileHandleWriter();

			std::FILE* getFile();

			bool isAtEnd() const override;
			bool seekToPosition(int64_t position, SeekMode seekMode) override;
			uint64_t getCurrentPosition() const override;
			uint64_t getBytesWritten() const override;

		private:
			std::FILE* outputFile;
			bool byteSwappingEnabled;

			void writeByte(uint8_t byte) override;
			void writeBytes(const uint8_t* data, size_t numBytes) override;
	};
}

