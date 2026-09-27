#include "Jargon/BinaryFileHandleWriter.h"

#include <cassert>


namespace Jargon {

	BinaryFileHandleWriter::BinaryFileHandleWriter(std::FILE* outputFile, bool byteSwappingEnabled) :
		outputFile(outputFile),
		byteSwappingEnabled(byteSwappingEnabled)
	{
	}

	BinaryFileHandleWriter::~BinaryFileHandleWriter() {
	}

	FILE* BinaryFileHandleWriter::getFile() {
		return outputFile;
	}

	bool BinaryFileHandleWriter::isAtEnd() const {
		return std::feof(outputFile) != 0;
	}

	bool BinaryFileHandleWriter::seekToPosition(int64_t position, SeekMode seekMode) {
		int seekDir;

		if (seekMode == SeekMode_Beginning) {
			seekDir = SEEK_SET;
		} else if (seekMode == SeekMode_CurrentPosition) {
			seekDir = SEEK_CUR;
		} else {
			seekDir = SEEK_END;
		}

		return _fseeki64(outputFile, position, seekDir) == 0;
	}

	uint64_t BinaryFileHandleWriter::getCurrentPosition() const {
		return (uint64_t)_ftelli64(outputFile);
	}

	uint64_t BinaryFileHandleWriter::getBytesWritten() const {
		return getCurrentPosition();
	}

	void BinaryFileHandleWriter::writeByte(uint8_t byte) {
		fputc(byte, outputFile);
	}

	void BinaryFileHandleWriter::writeBytes(const uint8_t* data, size_t numBytes) {
		fwrite(data, 1, numBytes, outputFile);
	}

}
