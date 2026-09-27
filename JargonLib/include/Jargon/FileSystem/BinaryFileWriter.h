#pragma once

#include "Jargon/BinaryStreamWriter.h"

#include <fstream>

namespace Jargon{
namespace FileSystem {

	class BinaryFileWriter : public BinaryStreamWriter {
		public:
			enum OpenMode {
				OpenMode_Append,
				OpenMode_Overwrite
			};

			// returns nullptr on failure
			static BinaryFileWriter* OpenFile(const wchar_t* filename, OpenMode mode);
			static BinaryFileWriter* OpenFile(const char* filename, OpenMode mode);

			BinaryFileWriter();
			virtual ~BinaryFileWriter();

			virtual bool openFile(const wchar_t* filename, OpenMode mode);
			virtual bool openFile(const char* filename, OpenMode mode);
			virtual bool isOpen() const;
			virtual bool closeFile();

			bool isAtEnd() const override;

		private:
			std::ofstream outputFile;
	};

}
}
