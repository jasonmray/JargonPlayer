
#include "Jargon/Null.h"
#include "Jargon/StringUtilities.h"

#include <algorithm>

#include <windows.h>

namespace Jargon{
namespace StringUtilities{

	std::string format(const char * formatString, ...){
		va_list args;
		va_start(args, formatString);
		std::string s(formatVarArgs(formatString, args));
		va_end(args);
		return s;
	}

	std::string formatVarArgs(const char * formatString, va_list args){
		size_t bufferLength = vsnprintf(NULL, 0, formatString, args) + 1;
		char * buffer = (char *)alloca(bufferLength * sizeof(char));
		vsnprintf(buffer, bufferLength, formatString, args);

		return std::string(buffer, 0, bufferLength);
	}

	size_t stringLength(const wchar_t* s) {
		return wcslen(s);
	}

	std::string wideToUtf8(const wchar_t *wString, size_t length) {
		int destSize = WideCharToMultiByte(CP_UTF8, 0, &wString[0], (int)length, NULL, 0, NULL, NULL);

		std::string utf8String(destSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wString[0], (int)length, &utf8String[0], destSize, NULL, NULL);
		return utf8String;
	}

	std::string wideToUtf8(const wchar_t *wString) {
		const size_t length = stringLength(wString);
		return wideToUtf8(wString, length);
	}

	std::string wideToUtf8(const std::wstring& wString) {
		return wideToUtf8(wString.c_str(), wString.size());
	}

	std::wstring utf8ToWide(const char* utf8string) {
		int destSize = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8string, -1, NULL, 0);

		std::wstring wideString(destSize, 0);
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8string, -1, &wideString[0], destSize);

		return wideString;
	}

	void tolower(std::string& s) {
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	}

	bool caseInsensitiveSortFunctor(const std::string& s1, const std::string& s2) {
		return _stricmp(s1.c_str(), s2.c_str()) < 0;
	}

	bool caseInsensitiveSortFunctorReverse(const std::string& s1, const std::string& s2) {
		return !caseInsensitiveSortFunctor(s1, s2);
	}

}
}
