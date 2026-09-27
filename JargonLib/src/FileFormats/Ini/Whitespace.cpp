#include "Jargon/FileFormats/Ini/Whitespace.h"

#include "Jargon/StringUtilities.h"


namespace Jargon{
namespace FileFormats{
namespace Ini{

	Whitespace::Whitespace():
		IniElement(ElementType_Whitespace)
	{
	}

	Whitespace::~Whitespace(){
	}

	bool Whitespace::parse(const std::string& line, Jargon::ErrorContext& errorContext) {
		std::string::const_iterator it = Jargon::StringUtilities::skipWhitespace(line, line.begin());
		if (it != line.end()) {
			errorContext.setLastError("INI Whitespace line contains non-whitespace");
			return false;
		}

		this->contents = line;
		return true;
	}

	const std::string& Whitespace::getContents() const{
		return contents;
	}

	void Whitespace::setContents(const std::string& contents){
		this->contents = contents;
	}

	bool Whitespace::canAttachChild(ElementType elementType) const{
		return false;
	}

	bool Whitespace::writeTo(TextWriter& destination) const{
		destination.write(contents);
		return true;
	}

}
}
}
