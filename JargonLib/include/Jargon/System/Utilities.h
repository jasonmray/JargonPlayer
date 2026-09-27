
#ifndef JARGON_SYSTEM_UTILITIES_H
#define JARGON_SYSTEM_UTILITIES_H

#include <string>
#include <vector>

namespace Jargon{
namespace System{

	void waitForDebugger();
	void sleep(unsigned int milliseconds);
	unsigned int getHardwareConcurrencyCount();
	void showFileInExplorer(const char * path);
	void notifyDisplayInUse(bool inUse);

	std::string getClockTimeForCurrentUserLocale();
	std::string getUserScreenshotsFolderPath();

	// gets path & exe name of current application
	bool getApplicationPath(std::string* pathOut);

	// gets folder location of current application
	bool getApplicationFolder(std::string* folderOut);

	std::string getHumanReadableSizeBytes(uint64_t sizeBytes);
	void getHumanReadableSizeBytes(uint64_t sizeBytes, std::string& stringOut);
}
}

#endif
