#pragma once

#include "Jargon/BinaryWriterObject.h"

#include <cstdarg>
#include <string>

namespace Jargon{

	class BinaryWriterObject;

	class TextWriter{
		public:
			static const char* LineEndings[];
			static const size_t LineEndingLengths[];

			enum LineEndingType {
				LineEndingType_LF,
				LineEndingType_CRLF
			};

			TextWriter(BinaryWriterObject& destination, LineEndingType lineEndingType = LineEndingType_CRLF);

			virtual ~TextWriter();

			void write(const char* text);
			void write(const std::string& text);
			void write(char c);

			void writeLine(const char* line);
			void writeLine(const std::string& text);

			void writeFormatted(const char* format, ...);
			void writeFormattedVarArg(const char* format, va_list args);

			void writeLineFormatted(const char* format, ...);
			void writeLineFormattedVarArg(const char* format, va_list args);


			void newLine();

			void flush();

		private:
			LineEndingType lineEndingType;
			BinaryWriterObject& destination;
	};

}
