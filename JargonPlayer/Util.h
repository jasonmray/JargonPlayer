#pragma once

#include <string>

struct mpv_node;

namespace Util{

	std::string createDateTimeSecondString();
	std::string formatSeconds_HMS(int totalSeconds);
	std::string formatSeconds_Colon(int totalSeconds);

	void debugLog(const char* name, const mpv_node& val);
}