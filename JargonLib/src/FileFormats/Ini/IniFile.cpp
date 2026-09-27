#include "Jargon/FileFormats/Ini/IniFile.h"
#include "Jargon/FileSystem/BinaryFileReader.h"
#include "Jargon/StringParser.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/TextReader.h"


namespace Jargon{
namespace FileFormats{
namespace Ini{

	static bool parseSettingPath(const char* settingPath, std::string* sectionNameOut, std::string* settingNameOut) {
		Jargon::StringParser<char> parser(settingPath);

		if (!parser.readUntil(':', sectionNameOut)) {
			return false;
		}

		if (!parser.readCharLiteral(':') || !parser.readCharLiteral(':')) {
			return false;
		}

		if (!parser.readUntilEnd(settingNameOut)) {
			return false;
		}

		return true;
	}

	IniFile::IniFile() :
		IniElement(ElementType_File)
	{
	}

	IniFile::~IniFile(){
	}

	bool IniFile::parse(const char* filename){
		Jargon::FileSystem::BinaryFileReader inputFile;
		if( !inputFile.openFile(filename) ){
			return false;
		}

		return parse(inputFile);
	}

	bool IniFile::parse(Jargon::BinaryReaderObject& dataSource){
		this->clear();

		Jargon::TextReader textReader(dataSource);
		Jargon::ErrorContext& errorContext = *this;
		IniElement* currentParent = this;
		std::string line;

		while (textReader.readLine(&line)) {

			IniElement* newElement;
			bool isNextParent = false;

			std::string::iterator afterWhitespace = Jargon::StringUtilities::skipWhitespace(line);

			if (afterWhitespace == line.end()) {
				newElement = new Whitespace();
			} else if (Comment::IsCommentStartChar(*afterWhitespace)) {
				newElement = new Comment();
			} else if (*afterWhitespace == Section::SectionNameBeginChar) {
				newElement = new Section();
				currentParent = this;
				isNextParent = true;
			} else {
				newElement = new Setting();
			}

			if (!newElement->parse(line, errorContext)) {
				delete newElement;
				this->clear();
				return false;
			}

			// currentParent assumes ownership of pointer on suceess
			if (!currentParent->appendChild(newElement)) {
				errorContext.setLastError("Bad INI structure");
				delete newElement;
				this->clear();
				return false;
			}

			if (isNextParent) {
				currentParent = newElement;
			}
		}

		return true;
	}

	bool IniFile::writeTo(TextWriter& destination) const {
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

	Setting* IniFile::findGlobalSetting(const char* settingName) {
		IniElement* child = this->getFirstChild();

		while (child != nullptr) {
			if (child->getElementType() == ElementType_Setting) {
				auto setting = (Setting*)child;
				if (setting->getName() == settingName) {
					return setting;
				}
			}
			child = child->getNextSibling();
		}

		return nullptr;
	}

	const Setting* IniFile::findGlobalSetting(const char* settingName) const {
		const IniElement* child = this->getFirstChild();

		while (child != nullptr) {
			if (child->getElementType() == ElementType_Setting) {
				auto setting = (const Setting*)child;
				if (setting->getName() == settingName) {
					return setting;
				}
			}
			child = child->getNextSibling();
		}

		return nullptr;
	}

	Section* IniFile::findSection(const char* sectionName) {
		IniElement* child = this->getFirstChild();

		while (child != nullptr) {
			if (child->getElementType() == ElementType_Section) {
				auto section = (Section*)child;
				if (section->getName() == sectionName) {
					return section;
				}
			}
			child = child->getNextSibling();
		}

		return nullptr;
	}

	const Section* IniFile::findSection(const char* sectionName) const {
		const IniElement* child = this->getFirstChild();

		while (child != nullptr) {
			if (child->getElementType() == ElementType_Section) {
				auto section = (const Section*)child;
				if (section->getName() == sectionName) {
					return section;
				}
			}
			child = child->getNextSibling();
		}

		return nullptr;
	}

	Setting* IniFile::findSetting(const char* settingPath) {
		std::string sectionName;
		std::string settingName;

		if (!parseSettingPath(settingPath, &sectionName, &settingName)) {
			return nullptr;
		}

		if (sectionName.empty()) {
			return findGlobalSetting(settingName.c_str());
		} else {
			return findSetting(sectionName.c_str(), settingName.c_str());
		}
	}

	const Setting* IniFile::findSetting(const char* settingPath) const {
		std::string sectionName;
		std::string settingName;

		if (!parseSettingPath(settingPath, &sectionName, &settingName)) {
			return nullptr;
		}

		if (sectionName.empty()) {
			return findGlobalSetting(settingName.c_str());
		} else {
			return findSetting(sectionName.c_str(), settingName.c_str());
		}
	}

	Setting* IniFile::findSetting(const char* sectionName, const char* settingName) {
		Section* section = findSection(sectionName);

		if (section == nullptr) {
			return nullptr;
		}

		return section->findSetting(settingName);
	}

	const Setting* IniFile::findSetting(const char* sectionName, const char* settingName) const {
		const Section* section = findSection(sectionName);

		if (section == nullptr) {
			return nullptr;
		}

		return section->findSetting(settingName);
	}

	bool IniFile::getValue(const char* settingPath, std::string* valueOut) const {
		const Setting* setting = findSetting(settingPath);

		if (setting == nullptr) {
			return false;
		}

		*valueOut = setting->getValue();
		return true;
	}

	bool IniFile::setValue(const char* settingPath, const char* value) {
		Setting* setting = findSetting(settingPath);

		if (setting == nullptr) {
			return false;
		}

		setting->setValue(value);
		return true;
	}

	Setting* IniFile::findOrCreateGlobalSetting(const char* settingName) {
		Setting* setting = findGlobalSetting(settingName);

		if (setting == nullptr) {
			setting = new Setting();
			setting->setName(settingName);

			this->prependChild(setting);
		}

		return setting;
	}

	Section* IniFile::findOrCreateSection(const char* sectionName) {
		Section* section = findSection(sectionName);

		if (section == nullptr) {
			section = new Section();
			section->setName(sectionName);
			this->appendChild(section);
		}

		return section;
	}

	Setting* IniFile::findOrCreateSetting(const char* settingPath) {
		std::string sectionName;
		std::string settingName;

		if (!parseSettingPath(settingPath, &sectionName, &settingName)) {
			return nullptr;
		}

		if (sectionName.empty()) {
			return findOrCreateGlobalSetting(settingName.c_str());
		} else {
			Section* section = findOrCreateSection(sectionName.c_str());
			return section->findOrCreateSetting(settingName.c_str());
		}
	}

	bool IniFile::getAllSettings(std::map<std::string, std::string>& settingsOut) const{
		const IniElement* child = this->getFirstChild();

		while (child != nullptr) {
			if (child->getElementType() == ElementType_Setting) {
				auto setting = (const Setting*)child;
				settingsOut[setting->getName()] = setting->getValue();
			} else if (child->getElementType() == ElementType_Section) {
				auto section = (const Section*)child;
				if (!section->getAllSettings(settingsOut)) {
					return false;
				}
			}
			child = child->getNextSibling();
		}

		return true;
	}

	void IniFile::clear(){
		IniElement* child = this->getFirstChild();

		while (child != nullptr) {
			child->detachFromTree();
			delete child;
			child = this->getFirstChild();
		}
	}

	bool IniFile::canAttachChild(ElementType elementType) const{
		return elementType != ElementType_File;
	}

	bool IniFile::parse(const std::string& line, Jargon::ErrorContext& errorContext) {
		return false;
	}

}
}
}
