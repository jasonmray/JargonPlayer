
#ifndef JARGON_STRINGUTILITIES_H
#define JARGON_STRINGUTILITIES_H

#include <string>
#include <cstdarg>

namespace Jargon{
namespace StringUtilities{

	std::string format(const char * formatString, ...);
	std::string formatVarArgs(const char * formatString, va_list args);

	size_t stringLength(const wchar_t* s);
	std::string wideToUtf8(const wchar_t *wString, size_t length);
	std::string wideToUtf8(const wchar_t *wString);
	std::string wideToUtf8(const std::wstring& wString);
	std::wstring utf8ToWide(const char* utf8string);

	void tolower(std::string& s);

	// use for case-insensitive sort:
	//    std::sort(x.begin(), x.end(), Jargon::StringUtilities::caseInsensitiveSortFunctor);
	bool caseInsensitiveSortFunctor(const std::string& s1, const std::string& s2);

	bool caseInsensitiveSortFunctorReverse(const std::string& s1, const std::string& s2);
}
}

#endif
