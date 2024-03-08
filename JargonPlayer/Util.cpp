#include "Util.h"

#include "Jargon/StringUtilities.h"

#include <libmpv/include/client.h>
#include <ctime>
#include <windows.h>


namespace Util{

	std::string getBaseFilename(const char* path){
		std::string pathString = path;
		size_t slashIndex = pathString.find_last_of("/\\");
		if(slashIndex != std::string::npos){
			pathString.erase(0, slashIndex + 1);
		}
		return pathString;
	}

	std::string getBaseFilenameNoExt(const char* path) {
		std::string pathString = path;
		size_t slashIndex = pathString.find_last_of("/\\");
		if (slashIndex != std::string::npos) {
			pathString.erase(0, slashIndex + 1);
		}

		size_t dotIndex = pathString.find_last_of(".");
		if (dotIndex != std::string::npos) {
			pathString.erase(dotIndex, pathString.length());
		}

		return pathString;
	}

	std::string getPathAndFilenameNoExt(const char* path) {
		std::string pathString = path;

		size_t dotIndex = pathString.find_last_of(".");
		if (dotIndex != std::string::npos) {
			pathString.erase(dotIndex, pathString.length());
		}

		return pathString;
	}

	std::string getFileExtension(const char* path) {
		std::string pathString = path;
		size_t dotIndex = pathString.find_last_of(".");
		if (dotIndex != std::string::npos) {
			pathString.erase(0, dotIndex + 1);
		}
		return pathString;
	}

	std::string createDateTimeSecondString() {
		std::time_t t = std::time(nullptr);
		char timeStringBuffer[100] = {0};
		std::tm localTime = {0};
		localtime_s(&localTime, &t);
		std::strftime(timeStringBuffer, sizeof(timeStringBuffer), "%Y%m%d_%H%M%S", &localTime);
		return std::string(timeStringBuffer);
	}

	void log(const char * formatString, ...) {
		va_list args;
		va_start(args, formatString);
		std::string s(Jargon::StringUtilities::formatVarArgs(formatString, args));
		va_end(args);

		OutputDebugString(s.c_str());
	}

	bool stringEqualCaseInsensitive(const char* a, const char *b){
		return _stricmp(a, b) == 0;
	}

	void debugLog(const char* name, const mpv_node& val) {
		switch (val.format) {
			case mpv_format::MPV_FORMAT_STRING:
				Util::log("%s : %s\n", name, val.u.string);
				break;
			case mpv_format::MPV_FORMAT_FLAG:
				Util::log("%s : %i\n", name, val.u.flag);
				break;
			case mpv_format::MPV_FORMAT_INT64:
				Util::log("%s : %ull\n", name, val.u.int64);
				break;
			case mpv_format::MPV_FORMAT_DOUBLE:
				Util::log("%s : %i\n", name, val.u.double_);
				break;
			default:
				break;
		}
	}

}