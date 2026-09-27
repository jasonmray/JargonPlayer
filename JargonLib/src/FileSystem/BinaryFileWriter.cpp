#include "Jargon/FileSystem/BinaryFileWriter.h"
#include "Jargon/ScopedPointer.h"

#include <cassert>


namespace Jargon {
namespace FileSystem {
	BinaryFileWriter* BinaryFileWriter::OpenFile(const wchar_t* filename, OpenMode mode) {
		Jargon::ScopedPointer<BinaryFileWriter> fileWriter(new BinaryFileWriter());

		if (!fileWriter->openFile(filename, mode)) {
			return nullptr;
		}

		return fileWriter.releaseOwnership();
	}

	BinaryFileWriter* BinaryFileWriter::OpenFile(const char* filename, OpenMode mode) {
		Jargon::ScopedPointer<BinaryFileWriter> fileWriter(new BinaryFileWriter());

		if (!fileWriter->openFile(filename, mode)) {
			return nullptr;
		}

		return fileWriter.releaseOwnership();
	}

	BinaryFileWriter::BinaryFileWriter() :
		BinaryStreamWriter(outputFile)
	{
	}

	BinaryFileWriter::~BinaryFileWriter(){
		closeFile();
	}

	bool BinaryFileWriter::openFile(const wchar_t* filename, OpenMode mode) {
		if (isOpen()) {
			return false;
		}

		std::ios_base::openmode stdOpenMode;
		stdOpenMode = std::ios::binary | std::ios::out;

		if (mode == OpenMode_Append) {
			stdOpenMode |= std::ios::ate;
		}

		outputFile.open(filename, stdOpenMode);

		return isOpen();
	}

	bool BinaryFileWriter::openFile(const char* filename, OpenMode mode) {
		if (isOpen()) {
			return false;
		}

		std::ios_base::openmode stdOpenMode;
		stdOpenMode = std::ios::binary | std::ios::out;

		if (mode == OpenMode_Append) {
			stdOpenMode |= std::ios::app;
		}

		outputFile.open(filename, stdOpenMode);

		return isOpen();
	}

	bool BinaryFileWriter::isOpen() const {
		return outputFile.is_open();
	}

	bool BinaryFileWriter::closeFile()
	{
		if (outputFile.is_open()) {
			outputFile.close();
			outputFile.clear();
		}
		return !outputFile.is_open();
	}

	bool BinaryFileWriter::isAtEnd() const {
		return false;
	}

}
}