#include "Jargon/NumberParsing.h"

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cwchar>


namespace Jargon{
namespace NumberParsing{

	bool parse(const char* string, int* valueOut, const char** parseEndOut) {
		long value;
		char* parseEnd;

		value = strtol(string, &parseEnd, 10);

		if (errno == ERANGE || value > INT_MAX || value < INT_MIN || parseEnd == string) {
			return false;
		}

		*valueOut = (int)value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const char* string, unsigned int* valueOut, const char** parseEndOut) {
		unsigned long value;
		char* parseEnd;

		value = strtoul(string, &parseEnd, 10);

		if (errno == ERANGE || value > UINT_MAX || parseEnd == string) {
			return false;
		}

		*valueOut = (unsigned int)value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const char* string, int64_t* valueOut, const char** parseEndOut) {
		int64_t value;
		char* parseEnd;

		value = _strtoi64(string, &parseEnd, 10);

		if (errno == ERANGE || parseEnd == string) {
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const char* string, uint64_t* valueOut, const char** parseEndOut) {
		uint64_t value;
		char* parseEnd;

		value = _strtoui64(string, &parseEnd, 10);

		if (errno == ERANGE || parseEnd == string) {
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const char* string, float* valueOut, const char** parseEndOut) {
		float value;
		char* parseEnd;

		value = strtof(string, &parseEnd);

		if (errno == ERANGE || parseEnd == string) {
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const char* string, double* valueOut, const char** parseEndOut) {
		double value;
		char* parseEnd;

		value = strtod(string, &parseEnd);

		if (errno == ERANGE || parseEnd == string) {
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parseHex(const char* string, unsigned int* valueOut, const char** parseEndOut) {
		unsigned long value;
		char* parseEnd;

		value = strtoul(string, &parseEnd, 16);

		if (errno == ERANGE || value > UINT_MAX || parseEnd == string) {
			return false;
		}

		*valueOut = (unsigned int)value;
		*parseEndOut = parseEnd;
		return true;
	}



	bool parsePartial(const char* string, int* valueOut) {
		const char* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const char* string, unsigned int* valueOut) {
		const char* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const char* string, int64_t* valueOut) {
		const char* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const char* string, uint64_t* valueOut) {
		const char* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const char* string, float* valueOut) {
		const char* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const char* string, double* valueOut) {
		const char* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parseHexPartial(const char* string, unsigned int* valueOut) {
		const char* parseEnd = nullptr;
		return parseHex(string, valueOut, &parseEnd);
	}



	bool parseComplete(const char* string, int* valueOut) {
		const char* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const char* string, unsigned int* valueOut) {
		const char* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const char* string, int64_t* valueOut) {
		const char* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const char* string, uint64_t* valueOut) {
		const char* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const char* string, float* valueOut) {
		const char* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const char* string, double* valueOut) {
		const char* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseHexComplete(const char* string, unsigned int* valueOut) {
		const char* parseEnd = nullptr;
		if (!parseHex(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}



	bool parse(const wchar_t* string, int* valueOut, const wchar_t** parseEndOut){
		long value;
		wchar_t* parseEnd;

		value = wcstol(string, &parseEnd, 10);

		if( errno == ERANGE || value > INT_MAX || value < INT_MIN || parseEnd == string ){
			return false;
		}

		*valueOut = (int)value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const wchar_t* string, unsigned int* valueOut, const wchar_t** parseEndOut){
		unsigned long value;
		wchar_t* parseEnd;

		value = wcstoul(string, &parseEnd, 10);

		if( errno == ERANGE || value > UINT_MAX || parseEnd == string ){
			return false;
		}

		*valueOut = (unsigned int)value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const wchar_t* string, int64_t* valueOut, const wchar_t** parseEndOut){
		int64_t value;
		wchar_t* parseEnd;

		value = _wcstoi64(string, &parseEnd, 10);

		if( errno == ERANGE || parseEnd == string ){
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const wchar_t* string, uint64_t* valueOut, const wchar_t** parseEndOut){
		uint64_t value;
		wchar_t* parseEnd;

		value = _wcstoui64(string, &parseEnd, 10);

		if( errno == ERANGE || parseEnd == string ){
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const wchar_t* string, float* valueOut, const wchar_t** parseEndOut){
		float value;
		wchar_t* parseEnd;

		value = wcstof(string, &parseEnd);

		if (errno == ERANGE || parseEnd == string){
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parse(const wchar_t* string, double* valueOut, const wchar_t** parseEndOut){
		double value;
		wchar_t* parseEnd;

		value = wcstod(string, &parseEnd);

		if( errno == ERANGE || parseEnd == string ){
			return false;
		}

		*valueOut = value;
		*parseEndOut = parseEnd;
		return true;
	}

	bool parseHex(const wchar_t* string, unsigned int* valueOut, const wchar_t** parseEndOut){
		unsigned long value;
		wchar_t* parseEnd;

		value = wcstoul(string, &parseEnd, 16);

		if (errno == ERANGE || value > UINT_MAX || parseEnd == string){
			return false;
		}

		*valueOut = (unsigned int)value;
		*parseEndOut = parseEnd;
		return true;
	}



	bool parsePartial(const wchar_t* string, int* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const wchar_t* string, unsigned int* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const wchar_t* string, int64_t* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const wchar_t* string, uint64_t* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const wchar_t* string, float* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parsePartial(const wchar_t* string, double* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parse(string, valueOut, &parseEnd);
	}

	bool parseHexPartial(const wchar_t* string, unsigned int* valueOut) {
		const wchar_t* parseEnd = nullptr;
		return parseHex(string, valueOut, &parseEnd);
	}



	bool parseComplete(const wchar_t* string, int* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const wchar_t* string, unsigned int* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const wchar_t* string, int64_t* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const wchar_t* string, uint64_t* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const wchar_t* string, float* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseComplete(const wchar_t* string, double* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parse(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

	bool parseHexComplete(const wchar_t* string, unsigned int* valueOut) {
		const wchar_t* parseEnd = nullptr;
		if (!parseHex(string, valueOut, &parseEnd)) {
			return false;
		}
		return *parseEnd == '\0';
	}

}
}
