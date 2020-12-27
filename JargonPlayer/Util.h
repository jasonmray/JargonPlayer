#pragma once

#include <string>

struct mpv_node;

namespace Util{

	std::string getBaseFilename(const char* path);
	std::string getFileExtension(const char* path);
	void log(const char * formatString, ...);
	bool stringEqualCaseInsensitive(const char* a, const char *b);
	void debugLog(const char* name, const mpv_node& val);
}