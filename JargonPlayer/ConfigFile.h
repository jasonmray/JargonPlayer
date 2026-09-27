#pragma once

#include "Jargon/FileFormats/Ini/IniFile.h"

#include <string>

class ConfigFile{
	public:
		static ConfigFile Instance;

		ConfigFile();
		~ConfigFile();

		bool loadFromDefaultFile();
		bool loadFromFile(const char* filename);
		bool getSettingAsString(const char* settingName, std::string* stringOut) const;
		bool getSettingAsBool(const char* settingName, bool* valueOut) const;

	private:
		static const char* DefaultFilename;

		Jargon::FileFormats::Ini::IniFile iniFile;
		const Jargon::FileFormats::Ini::Section* appSection;
};
