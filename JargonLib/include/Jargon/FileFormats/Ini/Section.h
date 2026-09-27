#pragma once

#include "Jargon/FileFormats/Ini/IniElement.h"
#include "Jargon/FileFormats/Ini/Setting.h"

#include <map>
#include <string>


namespace Jargon{
namespace FileFormats{
namespace Ini{

	class Section : public IniElement {
		public:
			static const char SectionNameBeginChar;
			static const char SectionNameEndChar;

			Section();
			~Section();

			bool parse(const std::string& line, Jargon::ErrorContext& errorContext) override;

			Setting* findSetting(const char* settingName);
			const Setting* findSetting(const char* settingName) const;

			Setting* findOrCreateSetting(const char* settingName);

			const std::string& getName() const;
			void setName(const std::string& name);

			const std::string& getLeadingWhiteSpace() const;
			void setLeadingWhiteSpace(const std::string& leadingWhiteSpace);

			const std::string& getTrailingComment() const;
			void setTrailingComment(const std::string& trailingComment);

			bool getAllSettings(std::map<std::string, std::string>& settingsOut, bool includeSectionName = true) const;

			bool canAttachChild(ElementType elementType) const override;
			bool writeTo(TextWriter& destination) const override;

		private:
			std::string leadingWhiteSpace;
			std::string name;
			std::string trailingComment;
	};

}
}
}
