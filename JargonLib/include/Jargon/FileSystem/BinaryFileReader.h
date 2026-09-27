#pragma once

#include "Jargon/BinaryStreamReader.h"

#include <fstream>

namespace Jargon {
namespace FileSystem {

	class BinaryFileReader : public BinaryStreamReader {
		public:
			BinaryFileReader();
			~BinaryFileReader();

			virtual bool openFile(const char* filename);
			virtual bool closeFile();
			virtual bool isOpen() const;

			virtual uint64_t getDataSizeBytes() const;

		private:
			mutable std::ifstream inputFile;
	};

}
}