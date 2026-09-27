#pragma once

#include <cstdint>

namespace Jargon{
namespace NumberParsing{

	// may only parse partial string when returning true
	bool parse(const char* string, int* valueOut, const char** parseEndOut);
	bool parse(const char* string, unsigned int* valueOut, const char** parseEndOut);
	bool parse(const char* string, int64_t* valueOut, const char** parseEndOut);
	bool parse(const char* string, uint64_t* valueOut, const char** parseEndOut);
	bool parse(const char* string, float* valueOut, const char** parseEndOut);
	bool parse(const char* string, double* valueOut, const char** parseEndOut);
	bool parseHex(const char* string, unsigned int* valueOut, const char** parseEndOut);

	// may only parse partial string when returning true
	bool parsePartial(const char* string, int* valueOut);
	bool parsePartial(const char* string, unsigned int* valueOut);
	bool parsePartial(const char* string, int64_t* valueOut);
	bool parsePartial(const char* string, uint64_t* valueOut);
	bool parsePartial(const char* string, float* valueOut);
	bool parsePartial(const char* string, double* valueOut);
	bool parseHexPartial(const char* string, unsigned int* valueOut);

	// only returns true if whole string was parsed
	bool parseComplete(const char* string, int* valueOut);
	bool parseComplete(const char* string, unsigned int* valueOut);
	bool parseComplete(const char* string, int64_t* valueOut);
	bool parseComplete(const char* string, uint64_t* valueOut);
	bool parseComplete(const char* string, float* valueOut);
	bool parseComplete(const char* string, double* valueOut);
	bool parseHexComplete(const char* string, unsigned int* valueOut);


	// may only parse partial string when returning true
	bool parse(const wchar_t* string, int* valueOut, const wchar_t** parseEndOut);
	bool parse(const wchar_t* string, unsigned int* valueOut, const wchar_t** parseEndOut);
	bool parse(const wchar_t* string, int64_t* valueOut, const wchar_t** parseEndOut);
	bool parse(const wchar_t* string, uint64_t* valueOut, const wchar_t** parseEndOut);
	bool parse(const wchar_t* string, float* valueOut, const wchar_t** parseEndOut);
	bool parse(const wchar_t* string, double* valueOut, const wchar_t** parseEndOut);
	bool parseHex(const wchar_t* string, unsigned int* valueOut, const wchar_t** parseEndOut);

	// may only parse partial string when returning true
	bool parsePartial(const wchar_t* string, int* valueOut);
	bool parsePartial(const wchar_t* string, unsigned int* valueOut);
	bool parsePartial(const wchar_t* string, int64_t* valueOut);
	bool parsePartial(const wchar_t* string, uint64_t* valueOut);
	bool parsePartial(const wchar_t* string, float* valueOut);
	bool parsePartial(const wchar_t* string, double* valueOut);
	bool parseHexPartial(const wchar_t* string, unsigned int* valueOut);

	// only returns true if whole string was parsed
	bool parseComplete(const wchar_t* string, int* valueOut);
	bool parseComplete(const wchar_t* string, unsigned int* valueOut);
	bool parseComplete(const wchar_t* string, int64_t* valueOut);
	bool parseComplete(const wchar_t* string, uint64_t* valueOut);
	bool parseComplete(const wchar_t* string, float* valueOut);
	bool parseComplete(const wchar_t* string, double* valueOut);
	bool parseHexComplete(const wchar_t* string, unsigned int* valueOut);

}
}
