
#include "Jargon/BinaryStreamWriter.h"

#include <cassert>

namespace Jargon{

	BinaryStreamWriter::BinaryStreamWriter(std::ostream& outputStream, bool byteSwappingEnabled):
		outputStream(outputStream),
		byteSwappingEnabled(byteSwappingEnabled)
	{
	}

	BinaryStreamWriter::~BinaryStreamWriter(){
	}

	std::ostream& BinaryStreamWriter::getStream(){
		return outputStream;
	}

	bool BinaryStreamWriter::isAtEnd() const
	{
		return outputStream.eof();
	}

	bool BinaryStreamWriter::seekToPosition(int64_t position, SeekMode seekMode)
	{
		assert(position < std::numeric_limits<std::streamoff>::max());

		outputStream.clear();

		std::ios_base::seekdir seekDir;

		if (seekMode == SeekMode_Beginning) {
			seekDir = std::ostream::beg;
		} else if (seekMode == SeekMode_CurrentPosition) {
			seekDir = std::ostream::cur;
		} else {
			seekDir = std::ostream::end;
		}
		outputStream.seekp((std::streamoff)position, seekDir);

		return outputStream.good();
	}

	uint64_t BinaryStreamWriter::getCurrentPosition() const
	{
		return (uint64_t)outputStream.tellp();
	}

	uint64_t BinaryStreamWriter::getBytesWritten() const
	{
		return getCurrentPosition();
	}

	void BinaryStreamWriter::writeByte(uint8_t byte)
	{
		outputStream.put(byte);
	}

	void BinaryStreamWriter::writeBytes(const uint8_t* data, size_t numBytes)
	{
		outputStream.write((const char*)data, (std::streamsize)numBytes);
	}

}
