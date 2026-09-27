#include "Jargon/DebugLog.h"

#include "Jargon/StringUtilities.h"

#include <cstdarg>
#include <string>

#include "Jargon/System/WindowsDefines.h"
#include <windows.h>


namespace Jargon {

	void debugLog(const wchar_t* formatString, ...) {
#ifdef _DEBUG
		va_list args;
		va_start(args, formatString);
		std::wstring message = Jargon::StringUtilities::formatWideVarArgs(formatString, args);
		OutputDebugStringW(message.c_str());
		va_end(args);
#endif
	}

	void debugLog(const char* formatString, ...) {
#ifdef _DEBUG
		va_list args;
		va_start(args, formatString);
		std::string message = Jargon::StringUtilities::formatVarArgs(formatString, args);
		OutputDebugStringA(message.c_str());
		va_end(args);
#endif
	}

}
