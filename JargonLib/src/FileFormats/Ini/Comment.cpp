#include "Jargon/FileFormats/Ini/Comment.h"
#include "Jargon/StringParser.h"


namespace Jargon {
namespace FileFormats {
namespace Ini {

	const char* Comment::CommentStartChars = ";#";

	bool Comment::IsCommentStartChar(char c) {
		return (c == '#' || c == ';');
	}

	Comment::Comment() :
		IniElement(ElementType_Comment),
		commentStartChar(';') {
	}

	Comment::~Comment() {
	}

	bool Comment::parse(const std::string& line, Jargon::ErrorContext& errorContext) {
		Jargon::StringParser<char> parser(line);

		parser.readWhitespace(&leadingWhiteSpace);
		if (!parser.readAnyOf(CommentStartChars, &commentStartChar)) {
			errorContext.setLastError("INI Comment must start with a comment character (; or #)");
			return false;
		}
		parser.readUntilEnd(&comment);
		return true;
	}

	const std::string& Comment::getComment() const {
		return comment;
	}

	void Comment::setComment(const char* comment) {
		this->comment = comment;
	}

	const std::string& Comment::getLeadingWhiteSpace() const {
		return leadingWhiteSpace;
	}

	void Comment::setLeadingWhiteSpace(const std::string& leadingWhiteSpace) {
		this->leadingWhiteSpace = leadingWhiteSpace;
	}

	char Comment::getCommentStartChar() const {
		return commentStartChar;
	}

	void Comment::setCommentStartChar(char commentStartChar) {
		this->commentStartChar = commentStartChar;
	}

	bool Comment::canAttachChild(ElementType elementType) const {
		return false;
	}

	bool Comment::writeTo(TextWriter& destination) const {
		destination.write(leadingWhiteSpace);
		destination.write(commentStartChar);
		destination.write(comment);

		return true;
	}

}
}
}
