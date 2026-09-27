#include "Jargon/FileFormats/Ini/Setting.h"
#include "Jargon/FileFormats/Ini/Comment.h"

#include "Jargon/NumberParsing.h"
#include "Jargon/StringParser.h"
#include "Jargon/StringUtilities.h"


namespace Jargon{
namespace FileFormats{
namespace Ini{

	static bool isQuotedString(const std::string& string) {
		if (Jargon::StringUtilities::stringBeginsWith(string, '\"') && Jargon::StringUtilities::stringEndsWith(string, '\"')) {
			return true;
		}
		if (Jargon::StringUtilities::stringBeginsWith(string, '\'') && Jargon::StringUtilities::stringEndsWith(string, '\'')) {
			return true;
		}

		return false;
	}
	static bool isTrueValue(const std::string& string) {
		return Jargon::StringUtilities::stringEqualCaseInsensitive(string.c_str(), "true") || Jargon::StringUtilities::stringEqual(string.c_str(), "1");
	}
	static bool isFalseValue(const std::string& string) {
		return Jargon::StringUtilities::stringEqualCaseInsensitive(string.c_str(), "false") || Jargon::StringUtilities::stringEqual(string.c_str(), "0");
	}

	Setting::Setting() :
		IniElement(ElementType_Setting)
	{
	}

	Setting::~Setting(){
	}

	bool Setting::parse(const std::string& line, Jargon::ErrorContext& errorContext) {
		Jargon::StringParser<char> parser(line);

		parser.readWhitespace(&leadingWhiteSpace);
		if (!parser.readUntil('=', &name) || name.empty()) {
			errorContext.setLastError("INI setting must contain a name");
			return false;
		}

		Jargon::StringUtilities::trimTrailingWhitespace(&name);

		if (!parser.readCharLiteral('=')) {
			errorContext.setLastError("INI setting must contain '='");
			return false;
		}

		parser.readUntilAnyOf(Comment::CommentStartChars, &value);

		Jargon::StringUtilities::trimWhitespace(&value);

		if (value.length() >= 2) {
			if (isQuotedString(value)) {
				value = value.substr(1, value.length() - 2);
			}
		}

		parser.readUntilEnd(&trailingComment);
		return true;
	}

	std::string Setting::getName() const {
		return name;
	}

	void Setting::setName(const std::string& name) {
		this->name = name;
	}

	std::string Setting::getValue() const {
		return value;
	}

	void Setting::setValue(const char* value) {
		this->value = value;
	}

	void Setting::setValue(const std::string& value) {
		this->value = value;
	}

	bool Setting::getValueAsInteger(int* valueOut) const {
		return Jargon::NumberParsing::parseComplete(value.c_str(), valueOut);
	}

	bool Setting::getValueAsBoolean(bool* valueOut) const {
		if (isTrueValue(value)) {
			*valueOut = true;
			return true;
		}

		if (isFalseValue(value)) {
			*valueOut = false;
			return true;
		}

		return false;
	}

	bool Setting::getValueAsFloat(float* valueOut) const {
		return Jargon::NumberParsing::parseComplete(value.c_str(), valueOut);
	}

	bool Setting::getValueAsDouble(double* valueOut) const {
		return Jargon::NumberParsing::parseComplete(value.c_str(), valueOut);
	}

	const std::string& Setting::getLeadingWhiteSpace() const{
		return leadingWhiteSpace;
	}

	void Setting::setLeadingWhiteSpace(const std::string& leadingWhiteSpace){
		this->leadingWhiteSpace = leadingWhiteSpace;
	}

	const std::string& Setting::getTrailingComment() const{
		return trailingComment;
	}

	void Setting::setTrailingComment(const std::string& trailingComment){
		this->trailingComment = trailingComment;
	}

	bool Setting::canAttachChild(ElementType elementType) const {
		return false;
	}

	bool Setting::writeTo(TextWriter& destination) const{
		destination.write(leadingWhiteSpace);
		destination.write(name);
		destination.write(" = ");

		if (value.find_first_of(" \t") != std::string::npos) {
			destination.write('\"');
			destination.write(value);
			destination.write('\"');
		} else {
			destination.write(value);
		}

		if (!trailingComment.empty()) {
			destination.write(' ');
			destination.write(trailingComment);
		}
		return true;
	}

}
}
}
