
#include "Jargon/FileSystem/BinaryFileReader.h"

namespace Jargon {
namespace FileSystem {

	BinaryFileReader::BinaryFileReader():
		BinaryStreamReader(inputFile)
	{
	}

	BinaryFileReader::~BinaryFileReader(){
		closeFile();
	}

	bool BinaryFileReader::openFile(const char* filename){
		if (isOpen()) {
			return false;
		}

		inputFile.open(filename, std::ios::binary);

		return isOpen();
	}

	bool BinaryFileReader::closeFile(){
		if (!isOpen()) {
			return false;
		}
		inputFile.close();
		return true;
	}

	bool BinaryFileReader::isOpen() const{
		return inputFile.is_open();
	}

	uint64_t BinaryFileReader::getDataSizeBytes() const{
		const std::streamoff currentPosition = inputFile.tellg();

		inputFile.seekg(0, std::ostream::beg);
		const std::streamoff startPosition = inputFile.tellg();

		inputFile.seekg(0, std::ostream::end);
		const std::streamoff endPosition = inputFile.tellg();

		inputFile.seekg(currentPosition, std::ostream::beg);

		return (uint64_t)(endPosition - startPosition);
	}

}
}