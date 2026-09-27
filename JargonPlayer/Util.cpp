#include "Util.h"

#include "Jargon/DebugLog.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/System/WindowsDefines.h"

#include <mpv/client.h>
#include <ctime>
#include <windows.h>


namespace Util{

	std::string createDateTimeSecondString() {
		std::time_t t = std::time(nullptr);
		char timeStringBuffer[100] = {0};
		std::tm localTime = {0};
		localtime_s(&localTime, &t);
		std::strftime(timeStringBuffer, sizeof(timeStringBuffer), "%Y%m%d_%H%M%S", &localTime);
		return std::string(timeStringBuffer);
	}

	std::string formatSeconds_HMS(int totalSeconds) {
		if (totalSeconds < 60) {
			return Jargon::StringUtilities::format("%is", totalSeconds);
		} else if (totalSeconds < 60 * 60) {
			int seconds = totalSeconds % 60;
			int minutes = totalSeconds / 60;
			return Jargon::StringUtilities::format("%im%is", minutes, seconds);
		} else {
			int seconds = totalSeconds % 60;
			int minutes = totalSeconds / 60;
			int hours = minutes / 60;
			minutes = minutes % 60;

			return Jargon::StringUtilities::format("%ih%im%is", hours, minutes, seconds);
		}
	}

	std::string formatSeconds_Colon(int totalSeconds) {
		if (totalSeconds < 60) {
			return Jargon::StringUtilities::format("00:%02i", totalSeconds);
		} else if (totalSeconds < 60 * 60) {
			int seconds = totalSeconds % 60;
			int minutes = totalSeconds / 60;
			return Jargon::StringUtilities::format("%02i:%02i", minutes, seconds);
		} else {
			int seconds = totalSeconds % 60;
			int minutes = totalSeconds / 60;
			int hours = minutes / 60;
			minutes = minutes % 60;

			return Jargon::StringUtilities::format("%02i:%02i:%02i", hours, minutes, seconds);
		}
	}

	void debugLog(const char* name, const mpv_node& val) {
		switch (val.format) {
			case mpv_format::MPV_FORMAT_STRING:
				Jargon::debugLog("%s : %s\n", name, val.u.string);
				break;
			case mpv_format::MPV_FORMAT_FLAG:
				Jargon::debugLog("%s : %i\n", name, val.u.flag);
				break;
			case mpv_format::MPV_FORMAT_INT64:
				Jargon::debugLog("%s : %ull\n", name, val.u.int64);
				break;
			case mpv_format::MPV_FORMAT_DOUBLE:
				Jargon::debugLog("%s : %i\n", name, val.u.double_);
				break;
			default:
				break;
		}
	}

}