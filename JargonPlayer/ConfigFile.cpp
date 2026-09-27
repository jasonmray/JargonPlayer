#include "ConfigFile.h"
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/FileFormats/Ini/IniFile.h"
#include "Jargon/System/Utilities.h"

#include <fstream>
#include <string>

const char* ConfigFile::DefaultFilename = "JargonPlayer.ini";

ConfigFile ConfigFile::Instance;

ConfigFile::ConfigFile():
	appSection(nullptr)
{
}

ConfigFile::~ConfigFile(){
}

bool ConfigFile::loadFromDefaultFile() {
	std::string applicationFolder;
	if (!Jargon::System::getApplicationFolder(&applicationFolder)) {
		return false;
	}

	std::string configFilename(applicationFolder);
	configFilename += DefaultFilename;

	return loadFromFile(configFilename.c_str());
}

bool ConfigFile::loadFromFile(const char* filename) {
	if (!iniFile.parse(filename)) {
		return false;
	}

	appSection = iniFile.findSection("JargonPlayer");
	if (appSection == nullptr) {
		iniFile.clear();
		return false;
	}

	return true;
}

bool ConfigFile::getSettingAsString(const char* settingName, std::string* stringOut) const{
	if (appSection == nullptr) {
		return false;
	}

	const Jargon::FileFormats::Ini::Setting* setting = appSection->findSetting(settingName);
	if (setting == nullptr) {
		return false;
	}
	*stringOut = setting->getValue();
	return true;
}

bool ConfigFile::getSettingAsBool(const char* settingName, bool* valueOut) const{
	if (appSection == nullptr) {
		return false;
	}

	const Jargon::FileFormats::Ini::Setting* setting = appSection->findSetting(settingName);
	if (setting == nullptr) {
		return false;
	}
	return setting->getValueAsBoolean(valueOut);
}


