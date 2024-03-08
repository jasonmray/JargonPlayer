
#ifndef JARGON_SYSTEM_UTILITIES_H
#define JARGON_SYSTEM_UTILITIES_H

#include <string>
#include <vector>

namespace Jargon{
namespace System{

	void sleep(unsigned int milliseconds);
	unsigned int getHardwareConcurrencyCount();
	void showFileInExplorer(const char * path);
	void notifyDisplayInUse(bool inUse);
	void globFiles(const char * pattern, std::vector<std::string>& files);
	std::string getClockTimeForCurrentUserLocale();
	std::string getUserScreenshotsFolderPath();
}
}

#endif
