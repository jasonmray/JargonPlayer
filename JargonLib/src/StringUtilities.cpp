
#include "Jargon/Null.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/System/WindowsDefines.h"

#include <algorithm>
#include <cassert>

#include <strsafe.h>
#include <windows.h>


namespace Jargon{
namespace StringUtilities{

	const char* PosixUpperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const char* PosixLowerChars = "abcdefghijklmnopqrstuvwxyz";
	const char* PosixAlphaChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	const char* PosixDigitChars = "0123456789";
	const char* PosixHexDigitChars = "0123456789ABCDEFabcdef";
	const char* PosixAlphaNumericChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	const char* PosixPunctuationChars = "!\"#$ % &'()*+,-./:;<=>?@[\\]^_`{|}~";
	const char* PosixBlankChars = " \t";
	const char* PosixSpaceChars = "\t\n\v\f\r ";
	const char* PosixControlChars = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x7f";
	const char* PosixGraphChars = "\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e";
	const char* PosixPrintableChars = "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e";
	const char* ReasonableAsciiChars = "\t\n\r\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e";


	size_t stringLength(const char* s) {
		return strlen(s);
	}

	int stringCompare(const char* s1, const char* s2) {
		return strcmp(s1, s2);
	}

	int stringCompare(const wchar_t* s1, const wchar_t* s2) {
		return wcscmp(s1, s2);
	}

	int stringCompareCaseInsensitive(const char* s1, const char* s2) {
		return _stricmp(s1, s2);
	}

	int stringCompareCaseInsensitive(const wchar_t* s1, const wchar_t* s2) {
		return _wcsicmp(s1, s2);
	}

	int stringNCompare(const char* s1, const char* s2, size_t length) {
		return strncmp(s1, s2, length);
	}

	int stringNCompare(const wchar_t* s1, const wchar_t* s2, size_t length) {
		return wcsncmp(s1, s2, length);
	}

	int stringNCompareCaseInsensitive(const char* s1, const char* s2, size_t length) {
		return _strnicmp(s1, s2, length);
	}

	int stringNCompareCaseInsensitive(const wchar_t* s1, const wchar_t* s2, size_t length) {
		return _wcsnicmp(s1, s2, length);
	}

	bool stringEqual(const char* s1, const char* s2) {
		return stringCompare(s1, s2) == 0;
	}

	bool stringEqual(const wchar_t* s1, const wchar_t* s2) {
		return stringCompare(s1, s2) == 0;
	}

	bool stringEqualCaseInsensitive(const char* s1, const char* s2) {
		return stringCompareCaseInsensitive(s1, s2) == 0;
	}

	bool stringEqualCaseInsensitive(const wchar_t* s1, const wchar_t* s2) {
		return stringCompareCaseInsensitive(s1, s2) == 0;
	}

	bool stringEqualCaseInsensitive(const std::string& s1, const std::string& s2){
		return stringEqualCaseInsensitive(s1.c_str(), s2.c_str());
	}

	bool stringEqualCaseInsensitive(const std::wstring& s1, const std::wstring& s2){
		return stringEqualCaseInsensitive(s1.c_str(), s2.c_str());
	}

	bool isSpace(char c) {
		return isspace((unsigned char)c) != 0;
	}

	bool isSpace(wchar_t c) {
		return iswspace(c) != 0;
	}

	bool stringContains(const char* s, char c){
		std::string_view sv(s);
		return sv.find_first_of(c) != std::string_view::npos;
	}

	bool stringContains(const wchar_t* s, wchar_t c){
		std::wstring_view sv(s);
		return sv.find_first_of(c) != std::wstring_view::npos;
	}

	bool stringContainsAnyOf(const char* s, const char* toFind){
		std::string_view sv(s);
		return stringContainsAnyOf(sv, toFind);
	}

	bool stringContainsAnyOf(const wchar_t* s, const wchar_t* toFind){
		std::wstring_view sv(s);
		return stringContainsAnyOf(sv, toFind);
	}

	bool stringContainsOnly(const char* s, const char* expected) {
		std::string_view sv(s);
		return sv.find_first_not_of(expected) == std::string_view::npos;
	}

	bool stringContainsOnly(const wchar_t* s, const wchar_t* expected) {
		std::wstring_view sv(s);
		return sv.find_first_not_of(expected) == std::wstring_view::npos;
	}

	bool stringContainsOnly(const std::string_view& sv, const char* expected) {
		return sv.find_first_not_of(expected) == std::string_view::npos;
	}

	bool stringContainsOnly(const std::wstring_view& sv, const wchar_t* expected) {
		return sv.find_first_not_of(expected) == std::wstring_view::npos;
	}

	std::string format(const char * formatString, ...) {
		va_list args;
		va_start(args, formatString);
		std::string s(formatVarArgs(formatString, args));
		va_end(args);
		return s;
	}

	std::string formatVarArgs(const char * formatString, va_list args) {
		size_t stringLength = _vscprintf(formatString, args);
		std::string result;
		result.resize(stringLength);

		vsnprintf(&result[0], stringLength + 1, formatString, args);
		
		return result;
	}

	std::wstring formatWide(const wchar_t* formatString, ...) {
		va_list args;
		va_start(args, formatString);
		std::wstring s(formatWideVarArgs(formatString, args));
		va_end(args);
		return s;
	}

	std::wstring formatWideVarArgs(const wchar_t* formatString, va_list args) {
		size_t stringLength = _vscwprintf(formatString, args);
		std::wstring result;
		result.resize(stringLength);

		_vsnwprintf_s(&result[0], stringLength + 1, stringLength, formatString, args);

		return result;
	}

	size_t stringLength(const wchar_t* s) {
		return wcslen(s);
	}

	std::string wideToUtf8(const wchar_t *wString, size_t length) {
		assert(length == stringLength(wString));
		const int destSize = WideCharToMultiByte(CP_UTF8, 0, &wString[0], (int)length, NULL, 0, NULL, NULL);
		assert(destSize > 0);

		std::string utf8String(destSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wString[0], (int)length, &utf8String[0], destSize, NULL, NULL);

		utf8String.resize(destSize);

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
		const int destSize = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8string, -1, NULL, 0);
		assert(destSize > 0);

		std::wstring wideString(destSize, 0);
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8string, -1, &wideString[0], destSize);

		wideString.resize(destSize - 1);

		return wideString;
	}

	std::wstring utf8ToWide(const std::string_view& utf8string) {
		const int destSize = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8string.data(), (int)utf8string.length(), NULL, 0);
		assert(destSize > 0);

		std::wstring wideString(destSize, 0);
		MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, utf8string.data(), (int)utf8string.length(), &wideString[0], destSize);

		// note: no resize(destSize - 1) here! MultiByteToWideChar behaves differently when the length you give
		// it does not end on a null terminator.

		return wideString;
	}

	size_t stringCopy(char* dest, size_t destLengthIncludingNull, const char* source) {
		return stringCopy(dest, destLengthIncludingNull, source, stringLength(source));
	}

	size_t stringCopy(char* dest, size_t destLengthIncludingNull, const char* source, size_t sourceLengthWithoutNull) {
		assert(source != nullptr);
		assert(dest != nullptr);
		assert(stringLength(source) == sourceLengthWithoutNull);

		if (source == nullptr || dest == nullptr) {
			return 0;
		}

		HRESULT result = StringCchCopyNA(dest, destLengthIncludingNull, source, sourceLengthWithoutNull);

		if (SUCCEEDED(result) || result == STRSAFE_E_INSUFFICIENT_BUFFER) {
			return stringLength(dest);
		}

		dest[0] = '\0';
		return 0;
	}

	size_t stringCopy(wchar_t* dest, size_t destLengthIncludingNull, const wchar_t* source) {
		return stringCopy(dest, destLengthIncludingNull, source, stringLength(source));
	}

	size_t stringCopy(wchar_t* dest, size_t destLengthIncludingNull, const wchar_t* source, size_t sourceLengthWithoutNull) {
		assert(source != nullptr);
		assert(dest != nullptr);
		assert(stringLength(source) == sourceLengthWithoutNull);

		if (source == nullptr || dest == nullptr) {
			return 0;
		}

		HRESULT result = StringCchCopyNW(dest, destLengthIncludingNull, source, sourceLengthWithoutNull);

		if (SUCCEEDED(result) || result == STRSAFE_E_INSUFFICIENT_BUFFER) {
			return stringLength(dest);
		}

		dest[0] = L'\0';
		return 0;
	}

	void toLower(std::string* s) {
		std::transform(s->begin(), s->end(), s->begin(), [](unsigned char c) { return std::tolower(c); });
	}

	void toUpper(std::string* s) {
		std::transform(s->begin(), s->end(), s->begin(), [](unsigned char c) { return std::toupper(c); });
	}

	std::string toLower(const std::string& s) {
		std::string result(s);
		std::transform(result.begin(), result.end(), result.begin(), std::tolower);
		return result;
	}

	std::string toUpper(const std::string& s) {
		std::string result(s);
		std::transform(result.begin(), result.end(), result.begin(), std::toupper);
		return result;
	}

	bool isEmptyOrWhitespace(const char* s) {
		if (*s == '\0') {
			return true;
		}
		return stringContainsOnly(s, PosixSpaceChars);
	}

	bool caseInsensitiveSortFunctor(const std::string& s1, const std::string& s2) {
		return _stricmp(s1.c_str(), s2.c_str()) < 0;
	}

	bool caseInsensitiveSortFunctorReverse(const std::string& s1, const std::string& s2) {
		return !caseInsensitiveSortFunctor(s1, s2);
	}

	const char* skipWhitespace(const char* string) {
		if (string == nullptr) {
			return nullptr;
		}

		while (isSpace(*string)) {
			string++;
		}

		return string;
	}

	const wchar_t* skipWhitespace(const wchar_t* string) {
		if (string == nullptr) {
			return nullptr;
		}

		while (isSpace(*string)) {
			string++;
		}

		return string;
	}

	bool unSingleQuote(std::string& string) {
		return trimSurroundingChars(string, '\'');
	}

	bool unSingleQuote(std::wstring& string) {
		return trimSurroundingChars(string, L'\'');
	}

	bool unDoubleQuote(std::string& string) {
		return trimSurroundingChars(string, '\"');
	}

	bool unDoubleQuote(std::wstring& string) {
		return trimSurroundingChars(string, L'\"');
	}

	bool unQuote(std::string& string) {
		if (trimSurroundingChars(string, '\"')) {
			return true;
		}

		return trimSurroundingChars(string, '\'');
	}

	bool unQuote(std::wstring& string) {
		if (trimSurroundingChars(string, L'\"')) {
			return true;
		}

		return trimSurroundingChars(string, L'\'');
	}

	bool stringBeginsWith(const std::string& s, const std::string& prefix) {
		return stringBeginsWith<char>(s, prefix);
	}

	bool stringBeginsWith(const std::wstring& s, const std::wstring& prefix) {
		return stringBeginsWith<wchar_t>(s, prefix);
	}

	bool stringEndsWith(const std::string& s, const std::string& suffix) {
		size_t suffixLength = suffix.length();
		size_t stringLength = s.length();

		if (stringLength < suffixLength) {
			return false;
		}

		return stringNCompare(s.c_str() + stringLength - suffixLength, suffix.c_str(), suffixLength) == 0;
	}

	bool stringEndsWith(const std::wstring& s, const std::wstring& suffix) {
		size_t suffixLength = suffix.length();
		size_t sLength = s.length();

		if (sLength < suffixLength) {
			return false;
		}

		return stringNCompare(s.c_str() + sLength - suffixLength, suffix.c_str(), suffixLength) == 0;
	}


	int tokenizeString(const char* toTokenize, const char* delimiters, std::vector<std::string>& tokens) {
		return tokenizeString<char>(toTokenize, delimiters, tokens);
	}

	int tokenizeString(const std::string& toTokenize, const char* delimiters, std::vector<std::string>& tokens) {
		return tokenizeString<char>(toTokenize, delimiters, tokens);
	}

	void hexEncode(const uint8_t* data, size_t dataSizeBytes, std::string& hexOut) {
		static const char hexDigits[] = "0123456789abcdef";

		hexOut.clear();
		hexOut.reserve(dataSizeBytes * 2);

		for (size_t i = 0; i < dataSizeBytes; i++) {
			const uint8_t value = data[i];
			const uint8_t high = (value & 0xF0) >> 4;
			const uint8_t low = value & 0xF;
			hexOut.push_back(hexDigits[high]);
			hexOut.push_back(hexDigits[low]);
		}
	}
}
}
