#include "Jargon/TextReader.h"

#include <cassert>


namespace Jargon{

	TextReader::TextReader(BinaryReaderObject& dataSource):
		dataSource(dataSource)
	{
	}

	TextReader::~TextReader(){
	}

	bool TextReader::readLine(std::string* lineOut) {
		lineOut->clear();

		if (dataSource.isAtEnd()) {
			return false;
		}

		bool status = false;
		while (!dataSource.isAtEnd()) {
			char c = 0;
			if (dataSource.read8(&c)) {

				// report success if we read at least one byte
				status = true;

				if (c == '\n') {
					break;
				}

				if (c == '\r') {
					char next = 0;
					if (dataSource.peek8(&next)) {
						if (next == '\n') {
							bool result = dataSource.read8(&c);
							assert(result);
						}
					}
					break;
				} else {
					lineOut->push_back(c);
				}
			}
		}

		return status;
	}

	bool TextReader::readLineWithEndings(std::string* lineOut) {
		lineOut->clear();

		if (dataSource.isAtEnd()) {
			return false;
		}

		bool status = false;
		while (!dataSource.isAtEnd()) {
			char c = 0;
			if (dataSource.read8(&c)) {
				lineOut->push_back(c);

				// report success if we read at least one byte
				status = true;

				if (c == '\n') {
					break;
				}

				if (c == '\r') {
					char next = 0;
					if (dataSource.peek8(&next)) {
						if (next == '\n') {
							bool result = dataSource.read8(&c);
							assert(result);
							lineOut->push_back(c);
						}
					}
					break;
				}
			}
		}

		return status;
	}

	
}
