
#include "Jargon/TextWriter.h"
#include "Jargon/StringUtilities.h"

namespace Jargon{

	const char* TextWriter::LineEndings[] = {
		"\n",
		"\r\n"
	};

	const size_t TextWriter::LineEndingLengths[] = {
		1,
		2
	};

	TextWriter::TextWriter(BinaryWriterObject& destination, LineEndingType lineEndingType) :
		destination(destination),
		lineEndingType(lineEndingType)
	{
	}

	TextWriter::~TextWriter(){
	}

	void TextWriter::write(const char* text) {
		destination.writeUnswapped((const uint8_t*)text, strlen(text));
	}

	void TextWriter::write(const std::string& text) {
		destination.writeUnswapped((const uint8_t*)text.data(), text.length());
	}

	void TextWriter::write(char c) {
		destination.writeChar(c);
	}

	void TextWriter::writeLine(const char* line) {
		write(line);
		newLine();
	}

	void TextWriter::writeLine(const std::string& text) {
		write(text);
		newLine();
	}

	void TextWriter::writeFormatted(const char* format, ...) {
		va_list args;
		va_start(args, format);

		writeFormattedVarArg(format, args);

		va_end(args);
	}

	void TextWriter::writeFormattedVarArg(const char* format, va_list args) {
		std::string line;
		line = Jargon::StringUtilities::formatVarArgs(format, args);
		write(line);
	}

	void TextWriter::writeLineFormatted(const char* format, ...) {
		va_list args;
		va_start(args, format);

		writeLineFormattedVarArg(format, args);

		va_end(args);
	}

	void TextWriter::writeLineFormattedVarArg(const char* format, va_list args) {
		std::string line;
		line = Jargon::StringUtilities::formatVarArgs(format, args);
		write(line);
		newLine();
	}

	void TextWriter::newLine() {
		destination.writeUnswapped((const uint8_t*)LineEndings[lineEndingType], LineEndingLengths[lineEndingType]);
	}

	void TextWriter::flush()
	{
		destination.flush();
	}
}
