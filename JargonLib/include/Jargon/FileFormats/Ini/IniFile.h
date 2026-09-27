#pragma once

#include "Jargon/FileFormats/Ini/Comment.h"
#include "Jargon/FileFormats/Ini/IniElement.h"
#include "Jargon/FileFormats/Ini/Section.h"
#include "Jargon/FileFormats/Ini/Setting.h"
#include "Jargon/FileFormats/Ini/Whitespace.h"

#include "Jargon/ErrorContext.h"
#include "Jargon/BinaryReaderObject.h"
#include "Jargon/TextWriter.h"

#include <map>
#include <string>


namespace Jargon{
namespace FileFormats{
namespace Ini{

	class IniFile : public IniElement, public Jargon::ErrorContext {
		public:
			IniFile();
			~IniFile();

			bool parse(const char* filename);
			bool parse(Jargon::BinaryReaderObject& dataSource);
			bool writeTo(TextWriter& destination) const override;

			Setting* findGlobalSetting(const char* settingName);
			const Setting* findGlobalSetting(const char* settingName) const;

			Section* findSection(const char* sectionName);
			const Section* findSection(const char* sectionName) const;

			Setting* findSetting(const char* settingPath);
			const Setting* findSetting(const char* settingPath) const;

			Setting* findSetting(const char* sectionName, const char* settingName);
			const Setting* findSetting(const char* sectionName, const char* settingName) const;

			bool getValue(const char* settingPath, std::string* valueOut) const;
			bool setValue(const char* settingPath, const char* value);

			Setting* findOrCreateGlobalSetting(const char* settingName);
			Section* findOrCreateSection(const char* sectionName);
			Setting* findOrCreateSetting(const char* settingPath);

			bool getAllSettings(std::map<std::string, std::string>& settingsOut) const;

			void clear();

			bool canAttachChild(ElementType elementType) const override;

		protected:
			bool parse(const std::string& line, Jargon::ErrorContext& errorContext) override;
	};

}
}
}
