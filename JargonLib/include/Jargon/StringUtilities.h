#pragma once

#include <string>
#include <string_view>
#include <cstdarg>
#include <vector>

namespace Jargon{
namespace StringUtilities{

	extern const char* PosixUpperChars;        // [:upper:]  or [A-Z]
	extern const char* PosixLowerChars;        // [:lower:]  or [a-z]
	extern const char* PosixAlphaChars;        // [:alpha:]  or [A-Za-z]
	extern const char* PosixDigitChars;        // [:digit:]  or [0-9]
	extern const char* PosixHexDigitChars;     // [:xdigit:] or [0-9A-Fa-f]
	extern const char* PosixAlphaNumericChars; // [:alnum:]  or [A-Za-z0-9]
	extern const char* PosixPunctuationChars;  // [:punct:]
	extern const char* PosixBlankChars;        // [:blank:]  or [ \t]
	extern const char* PosixSpaceChars;        // [:space:]  or [ \t\n\r\f\v]
	extern const char* PosixControlChars;      // [:cntrl:]  or [\x00-\x1F\x7f]
	extern const char* PosixGraphChars;        // [:graph:]  or [^ [:cntrl:]]
	extern const char* PosixPrintableChars;    // [:print:]  or [[:graph:] ]
	extern const char* ReasonableAsciiChars;   // [\t\n\r\x20-\x7E]


	size_t stringLength(const char* s);
	size_t stringLength(const wchar_t* s);

	int stringCompare(const char* s1, const char* s2);
	int stringCompare(const wchar_t* s1, const wchar_t* s2);

	int stringCompareCaseInsensitive(const char* s1, const char* s2);
	int stringCompareCaseInsensitive(const wchar_t* s1, const wchar_t* s2);

	int stringNCompare(const char* s1, const char* s2, size_t length);
	int stringNCompare(const wchar_t* s1, const wchar_t* s2, size_t length);

	template<typename CharType>
	int stringNCompare(const std::basic_string<CharType>& s1, const std::basic_string<CharType>& s2, size_t length) {
		return stringNCompare(s1.c_str(), s2.c_str(), length);
	}

	int stringNCompareCaseInsensitive(const char* s1, const char* s2, size_t length);
	int stringNCompareCaseInsensitive(const wchar_t* s1, const wchar_t* s2, size_t length);

	bool stringEqual(const char* s1, const char* s2);
	bool stringEqual(const wchar_t* s1, const wchar_t* s2);
	bool stringEqualCaseInsensitive(const char* s1, const char* s2);
	bool stringEqualCaseInsensitive(const wchar_t* s1, const wchar_t* s2);
	bool stringEqualCaseInsensitive(const std::string& s1, const std::string& s2);
	bool stringEqualCaseInsensitive(const std::wstring& s1, const std::wstring& s2);

	std::string format(const char * formatString, ...);
	std::string formatVarArgs(const char * formatString, va_list args);

	std::wstring formatWide(const wchar_t* formatString, ...);
	std::wstring formatWideVarArgs(const wchar_t* formatString, va_list args);

	std::string wideToUtf8(const wchar_t *wString, size_t length);
	std::string wideToUtf8(const wchar_t *wString);
	std::string wideToUtf8(const std::wstring& wString);
	std::wstring utf8ToWide(const char* utf8string);
	std::wstring utf8ToWide(const std::string_view& utf8string);

	// returns number of characters copied to dest
	size_t stringCopy(char* dest, size_t destLengthIncludingNull, const char* source);
	size_t stringCopy(char* dest, size_t destLengthIncludingNull, const char* source, size_t sourceLengthWithoutNull);
	size_t stringCopy(wchar_t* dest, size_t destLengthIncludingNull, const wchar_t* source);
	size_t stringCopy(wchar_t* dest, size_t destLengthIncludingNull, const wchar_t* source, size_t sourceLengthWithoutNull);

	void toLower(std::string* s);
	void toUpper(std::string* s);
	std::string toLower(const std::string& s);
	std::string toUpper(const std::string& s);
	bool isSpace(char c);
	bool isSpace(wchar_t c);

	bool stringContains(const char* s, char c);
	bool stringContains(const wchar_t* s, wchar_t c);

	template<class CharT>
	bool stringContainsAnyOf(const std::basic_string_view<CharT>& sv, const CharT* toFind) {
		return sv.find_first_of(toFind) != std::string_view::npos;
	}
	template<class CharT>
	bool stringContainsAnyOf(const std::basic_string<CharT>& s, const CharT* toFind) {
		std::string_view sv(s);
		return stringContainsAnyOf(sv, toFind);
	}
	bool stringContainsAnyOf(const char* s, const char* toFind);
	bool stringContainsAnyOf(const wchar_t* s, const wchar_t* toFind);

	bool stringContainsOnly(const char* s, const char* expected);
	bool stringContainsOnly(const wchar_t* s, const wchar_t* expected);
	bool stringContainsOnly(const std::string_view& sv, const char* expected);
	bool stringContainsOnly(const std::wstring_view& sv, const wchar_t* expected);

	template<class CharT>
	bool isEmptyOrWhitespace(const std::basic_string<CharT>& s) {
		if (s.empty()) {
			return true;
		}
		return stringContainsOnly(s, PosixSpaceChars);
	}

	template<class CharT>
	bool isEmptyOrWhitespace(const std::basic_string_view<CharT>& s) {
		if (s.empty()) {
			return true;
		}
		return stringContainsOnly(s, PosixSpaceChars);
	}

	bool isEmptyOrWhitespace(const char* s);

	// use for case-insensitive sort:
	//    std::sort(x.begin(), x.end(), Jargon::StringUtilities::caseInsensitiveSortFunctor);
	bool caseInsensitiveSortFunctor(const std::string& s1, const std::string& s2);

	bool caseInsensitiveSortFunctorReverse(const std::string& s1, const std::string& s2);

	template <typename CharType>
	int trimLeadingWhitespace(std::basic_string<CharType>* stringToTrim) {
		typename std::basic_string<CharType>::iterator position;
		int count;

		position = stringToTrim->begin();

		if (position == stringToTrim->end()) {
			return 0;
		}

		while (position != stringToTrim->end() && isSpace(*position)) {
			++position;
		}

		count = (int)(position - stringToTrim->begin());
		stringToTrim->erase(stringToTrim->begin(), position);
		return count;
	}

	template <typename CharType>
	int trimTrailingWhitespace(std::basic_string<CharType>* stringToTrim) {
		typename std::basic_string<CharType>::reverse_iterator position;
		int count = 0;

		position = stringToTrim->rbegin();

		if (position == stringToTrim->rend()) {
			return 0;
		}

		while (position != stringToTrim->rend() && isSpace(*position)) {
			++position;
			++count;
		}

		stringToTrim->erase(position.base(), stringToTrim->end());
		return count;
	}

	template <typename CharType>
	int trimWhitespace(std::basic_string<CharType>* stringToTrim) {
		return trimLeadingWhitespace(stringToTrim) + trimTrailingWhitespace(stringToTrim);
	}

	template <typename CharType>
	typename std::basic_string<CharType>::iterator skipWhitespace(std::basic_string<CharType>& stringIn, typename std::basic_string<CharType>::iterator position) {
		while (position != stringIn.end() && isSpace(*position)) {
			++position;
		}
		return position;
	}

	template <typename CharType>
	typename std::basic_string<CharType>::iterator skipWhitespace(std::basic_string<CharType>& stringIn) {
		return skipWhitespace(stringIn, stringIn.begin());
	}

	template <typename CharType>
	typename std::basic_string<CharType>::const_iterator skipWhitespace(const std::basic_string<CharType>& stringIn, typename std::basic_string<CharType>::const_iterator position) {
		while (position != stringIn.end() && isSpace(*position)) {
			++position;
		}
		return position;
	}

	template <typename CharType>
	typename std::basic_string<CharType>::const_iterator skipWhitespace(const std::basic_string<CharType>& stringIn) {
		return skipWhitespace(stringIn, stringIn.cbegin());
	}

	const char* skipWhitespace(const char* string);
	const wchar_t* skipWhitespace(const wchar_t* string);

	template <typename CharType>
	int trimLeadingCharacters(std::basic_string<CharType>* stringToTrim, const CharType* charactersToRemove) {
		size_t count = stringToTrim->find_first_not_of(charactersToRemove);
		if (count == std::basic_string<CharType>::npos) {
			return 0;
		}
		stringToTrim->erase(0, count);
		return (int)count;
	}

	template <typename CharType>
	int trimTrailingCharacters(std::basic_string<CharType>* stringToTrim, const CharType* charactersToRemove) {
		size_t size = stringToTrim->size();
		size_t count = stringToTrim->find_last_not_of(charactersToRemove);
		stringToTrim->erase(count + 1);
		return (int)(size - count - 1);
	}

	template <typename CharType>
	int trimCharacters(std::basic_string<CharType>* stringToTrim, const CharType* charactersToRemove) {
		return trimLeadingCharacters(stringToTrim, charactersToRemove) + trimTrailingCharacters(stringToTrim, charactersToRemove);
	}

	template <typename CharType>
	bool trimSurroundingChars(std::basic_string_view<CharType>* stringView, CharType startChar, CharType endChar) {
		if (stringView->length() >= 2) {
			if (stringView->front() == startChar && stringView->back() == endChar) {
				*stringView = stringView->substr(1, stringView->length() - 2);
				return true;
			}
		}
		return false;
	}

	template <typename CharType>
	bool trimSurroundingChars(std::basic_string_view<CharType>* stringView, CharType startAndEndChar) {
		return trimSurroundingChars(stringView, startAndEndChar, startAndEndChar);
	}

	template <typename CharType>
	bool trimSurroundingChars(std::basic_string<CharType>& string, CharType startChar, CharType endChar) {
		std::basic_string_view<CharType> stringView = string;
		if (trimSurroundingChars(&stringView, startChar, endChar)) {
			string = stringView;
			return true;
		}

		return false;
	}

	template <typename CharType>
	bool trimSurroundingChars(std::basic_string<CharType>& string, CharType startAndEndChar) {
		return trimSurroundingChars(string, startAndEndChar, startAndEndChar);
	}

	bool unSingleQuote(std::string& string);
	bool unSingleQuote(std::wstring& string);
	bool unDoubleQuote(std::string& string);
	bool unDoubleQuote(std::wstring& string);
	bool unQuote(std::string& string);
	bool unQuote(std::wstring& string);

	template <typename CharType>
	bool stringBeginsWith(const std::basic_string_view<CharType>& s, CharType prefix) {
		return (s.length() > 0) && s[0] == prefix;
	}

	template <typename CharType>
	bool stringBeginsWith(const std::basic_string<CharType>& s, CharType prefix) {
		return (s.length() > 0) && s[0] == prefix;
	}

	template <typename CharType>
	bool stringEndsWith(const std::basic_string_view<CharType>& s, CharType suffix) {
		return (s.length() > 0) && s[s.length() - 1] == suffix;
	}

	template <typename CharType>
	bool stringEndsWith(const std::basic_string<CharType>& s, CharType suffix) {
		return (s.length() > 0) && s[s.length() - 1] == suffix;
	}

	template <typename CharType>
	bool stringBeginsWith(const std::basic_string<CharType>& s, const std::basic_string<CharType>& prefix) {
		size_t prefixLength = prefix.length();

		if (s.length() < prefixLength) {
			return false;
		}

		return stringNCompare(s.c_str(), prefix.c_str(), prefixLength) == 0;
	}
	template <typename CharType>
	bool stringBeginsWith(const CharType* s, const CharType* prefix) {
		size_t sLength = stringLength(s);
		size_t prefixLength = stringLength(prefix);

		if (sLength < prefixLength) {
			return false;
		}

		return stringNCompare(s, prefix, prefixLength) == 0;
	}

	bool stringBeginsWith(const std::string& s, const std::string& prefix);
	bool stringBeginsWith(const std::wstring& s, const std::wstring& prefix);


	template <typename CharType>
	bool stringEndsWith(const CharType* s, const CharType* suffix) {
		size_t sLength = stringLength(s);
		size_t suffixLength = stringLength(suffix);

		if (sLength < suffixLength) {
			return false;
		}

		return stringNCompare(s + sLength - suffixLength, suffix, suffixLength) == 0;
	}

	bool stringEndsWith(const std::string& s, const std::string& suffix);
	bool stringEndsWith(const std::wstring& s, const std::wstring& suffix);


	template <typename CharType> int tokenizeString(const std::basic_string<CharType>& toTokenize, const CharType* delimiters, std::vector<std::basic_string<CharType> >& tokens) {
		std::basic_string<CharType> token;
		typename std::basic_string<CharType>::size_type tokenBegin;
		typename std::basic_string<CharType>::size_type tokenEnd;
		int tokenCount = 0;

		typename std::basic_string<CharType>::size_type size = toTokenize.size();

		tokenEnd = 0;

		do {
			tokenBegin = tokenEnd;
			tokenEnd = toTokenize.find_first_of(delimiters, tokenBegin);

			if (tokenEnd == std::string::npos) {
				tokenEnd = size;
			}

			if (tokenEnd - tokenBegin > 0) {
				token = toTokenize.substr(tokenBegin, tokenEnd - tokenBegin);
				tokens.push_back(token);
				tokenCount++;
			}

			tokenEnd++;
		} while (tokenEnd < size);

		return tokenCount;
	}

	template <typename CharType> int tokenizeString(const CharType* toTokenize, const CharType* delimiters, std::vector<std::basic_string<CharType> >& tokens) {
		std::basic_string<CharType> stringToTokenize(toTokenize);
		return tokenizeString(stringToTokenize, delimiters, tokens);
	}

	int tokenizeString(const char* toTokenize, const char* delimiters, std::vector<std::string>& tokens);
	int tokenizeString(const std::string& toTokenize, const char* delimiters, std::vector<std::string>& tokens);


	void hexEncode(const uint8_t* data, size_t dataSizeBytes, std::string& hexOut);
}
}
