#pragma once

#include "Jargon/BinaryReaderObject.h"
#include <string>

namespace Jargon{

	class TextReader{
		public:
			TextReader(BinaryReaderObject& dataSource);
			~TextReader();

			bool readLine(std::string* lineOut);
			bool readLineWithEndings(std::string* lineOut);

		private:
			BinaryReaderObject& dataSource;
	};

}
