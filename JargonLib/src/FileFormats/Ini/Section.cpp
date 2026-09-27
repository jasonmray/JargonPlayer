#include "Jargon/FileFormats/Ini/Section.h"

#include "Jargon/StringParser.h"


namespace Jargon{
namespace FileFormats{
namespace Ini{

	const char Section::SectionNameBeginChar = '[';
	const char Section::SectionNameEndChar = ']';

	Section::Section() :
		IniElement(ElementType_Section)
	{
	}

	Section::~Section(){
	}

	bool Section::parse(const std::string& line, Jargon::ErrorContext& errorContext) {
		Jargon::StringParser<char> parser(line);

		parser.readWhitespace(&leadingWhiteSpace);

		if (!parser.readCharLiteral('[')) {
			errorContext.setLastError("INI section line must start with '['");
			return false;
		}

		if (!parser.readUntil(']', &name) || name.empty()) {
			errorContext.setLastError("Invalid INI section name.");
			return false;
		}

		if (!parser.readCharLiteral(']')) {
			errorContext.setLastError("INI section line must end with ']'");
			return false;
		}

		parser.readUntilEnd(&trailingComment);

		return true;
	}

	Setting* Section::findSetting(const char* settingName){
		IniElement* childElement;
		childElement = this->getFirstChild();

		while (childElement != nullptr) {
			if (childElement->getElementType() == ElementType_Setting) {
				Setting* setting = (Setting*)childElement;
				if (setting->getName() == settingName) {
					return setting;
				}
			}
			childElement = childElement->getNextSibling();
		}

		return nullptr;
	}

	const Setting* Section::findSetting(const char* settingName) const{
		const IniElement* childElement;
		childElement = this->getFirstChild();

		while (childElement != nullptr) {
			if (childElement->getElementType() == ElementType_Setting) {
				Setting* setting = (Setting*)childElement;
				if (setting->getName() == settingName) {
					return setting;
				}
			}
			childElement = childElement->getNextSibling();
		}

		return nullptr;
	}

	Setting* Section::findOrCreateSetting(const char* settingName){
		Setting* setting = findSetting(settingName);

		if (setting == nullptr) {
			setting = new Setting();
			setting->setName(settingName);
			setting->setLeadingWhiteSpace("\t");
			appendChild(setting);
		}

		return setting;
	}

	const std::string& Section::getName() const{
		return name;
	}

	void Section::setName(const std::string& name){
		this->name = name;
	}

	const std::string& Section::getLeadingWhiteSpace() const{
		return leadingWhiteSpace;
	}

	void Section::setLeadingWhiteSpace(const std::string& leadingWhiteSpace){
		this->leadingWhiteSpace = leadingWhiteSpace;
	}

	const std::string& Section::getTrailingComment() const{
		return trailingComment;
	}

	void Section::setTrailingComment(const std::string& trailingComment){
		this->trailingComment = trailingComment;
	}

	bool Section::getAllSettings(std::map<std::string, std::string>& settingsOut, bool includeSectionNameInKey) const{
		const IniElement* child = this->getFirstChild();

		std::string prefix;
		if (includeSectionNameInKey) {
			prefix = this->getName();
			prefix += "::";
		}

		while (child != nullptr) {
			if (child->getElementType() == ElementType_Setting) {
				auto setting = (const Setting*)child;

				if (includeSectionNameInKey) {
					std::string name = prefix;
					name += setting->getName();
					settingsOut[name] = setting->getValue();
				} else {
					settingsOut[setting->getName()] = setting->getValue();
				}
			}
			child = child->getNextSibling();
		}

		return true;
	}

	bool Section::canAttachChild(ElementType elementType) const{
		return elementType == ElementType_Setting || elementType == ElementType_Comment || elementType == ElementType_Whitespace;
	}

	bool Section::writeTo(TextWriter& destination) const
	{
		destination.write(leadingWhiteSpace);
		destination.write('[');
		destination.write(name);
		destination.write(']');
		destination.write(trailingComment);

		const IniElement* childElement;
		childElement = this->getFirstChild();

		while (childElement != nullptr) {
			if (!childElement->writeTo(destination)) {
				return false;
			}
			childElement = childElement->getNextSibling();
		}

		return true;
	}

}
}
}
