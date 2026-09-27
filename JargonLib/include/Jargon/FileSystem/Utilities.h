#pragma once

#include "Jargon/Buffer.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>


namespace Jargon{
namespace FileSystem{

	extern const int WindowsMaxPathLength;

	extern const char* WindowsLongNamePrefix;
	extern const wchar_t* WindowsLongNamePrefixWide;

	extern const char* WindowsIllegalFilepathChars;
	extern const wchar_t* WindowsIllegalFilepathCharsWide;

	extern const char* WindowsIllegalFilenameChars;
	extern const wchar_t* WindowsIllegalFilenameCharsWide;

	bool filenameHasExtension(const char* filename, const char* extension);

	enum FileType {
		Unknown,
		File,
		Folder,
		Other
	};
	bool getFileType(const char* filename, FileType& fileTypeOut);
	bool getFileType(const wchar_t* filename, FileType& fileTypeOut);

	bool fileExists(const char* filename);
	bool fileExists(const wchar_t* filename);
	bool isDirectory(const char* filename);
	bool isDirectory(const char* filename, bool& existsFlagOut);
	bool isDirectory(const wchar_t* filename);
	bool isDirectory(const wchar_t* filename, bool& existsFlagOut);

	bool getFileSize(const char* filename, int64_t* sizeOut);
	bool getFileSize(const wchar_t* filename, int64_t* sizeOut);

	std::string addLongPathPrefixIfNeeded(const std::string& filename);
	std::wstring addLongPathPrefixIfNeeded(const std::wstring& filename);

	std::string ensureLongPathPrefix(const char* filename);
	std::string ensureLongPathPrefix(const std::string& filename);
	std::wstring ensureLongPathPrefixWide(const char* filename);
	std::wstring ensureLongPathPrefixWide(const wchar_t* filename);
	std::wstring ensureLongPathPrefixWide(const std::wstring& filename);
	const char* skipLongPathPrefix(const char* filename);
	const char* skipLongPathPrefix(const std::string& filename);
	const wchar_t* skipLongPathPrefix(const wchar_t* filename);
	const wchar_t* skipLongPathPrefix(const std::wstring& filename);

	void globFiles(const char* pattern, std::vector<std::string>& files, bool returnAbsolutePaths = true);
	void globFiles(const wchar_t* pattern, std::vector<std::wstring>& files, bool returnAbsolutePaths = true);

	// filename without path or extension
	std::string_view getBaseFilename(const char* sourceFilename);

	// filename without path, with extension
	std::string_view getFilenameComponent(const char* filePath);

	// includes trailing slash if present
	std::string_view getPathComponent(const char* filePath);

	std::string_view getFileExtension(const char* filename);

	// remove dot and extension
	std::string_view removeFileExtension(const char* filename);


	// extract the path and filename+extension.
	// if a path is present, it will be included in filenameOut.
	template<class StringClass>
	void extractPathAndFilename(const std::string_view& filenameView, StringClass& pathOut, StringClass& filenameAndExtensionOut) {
		const size_t stringEnd = filenameView.length();
		const size_t pathStart = 0;
		size_t filenameStart = pathStart;

		const size_t slashLocation = filenameView.find_last_of("\\/");
		if (slashLocation != std::string_view::npos) {
			filenameStart = slashLocation + 1;
		}

		const size_t pathLength = filenameStart - pathStart;
		const size_t filenameLength = stringEnd - filenameStart;


		pathOut = filenameView.substr(0, filenameStart);
		filenameAndExtensionOut = filenameView.substr(filenameStart, filenameLength);
	}
	template<class StringClass>
	void extractPathAndFilename(const char* filename, StringClass& pathOut, StringClass& filenameAndExtensionOut) {
		std::string_view filenameView(filename);

		extractPathAndFilename(filenameView, pathOut, filenameAndExtensionOut);
	}
	void extractPathAndFilename(const char* filename, std::string& pathOut, std::string& filenameAndExtensionOut);
	void extractPathAndFilename(const char* filename, std::string_view& pathOut, std::string_view& filenameAndExtensionOut);

	template<class WStringClass>
	void extractPathAndFilename(const std::wstring_view& filenameView, WStringClass& pathOut, WStringClass& filenameAndExtensionOut) {
		const size_t stringEnd = filenameView.length();
		const size_t pathStart = 0;
		size_t filenameStart = pathStart;

		const size_t slashLocation = filenameView.find_last_of(L"\\/");
		if (slashLocation != std::string_view::npos) {
			filenameStart = slashLocation + 1;
		}

		const size_t pathLength = filenameStart - pathStart;
		const size_t filenameLength = stringEnd - filenameStart;


		pathOut = filenameView.substr(0, filenameStart);
		filenameAndExtensionOut = filenameView.substr(filenameStart, filenameLength);
	}
	template<class WStringClass>
	void extractPathAndFilename(const wchar_t* filename, WStringClass& pathOut, WStringClass& filenameAndExtensionOut) {
		std::wstring_view filenameView(filename);

		extractPathAndFilename(filenameView, pathOut, filenameAndExtensionOut);
	}
	void extractPathAndFilename(const wchar_t* filename, std::wstring& pathOut, std::wstring& filenameAndExtensionOut);
	void extractPathAndFilename(const wchar_t* filename, std::wstring_view& pathOut, std::wstring_view& filenameAndExtensionOut);


	// extract filename and extension components.
	// if a path is present, it will be included in filenameOut.
	// if an extension is present, dottedExtensionOut will include its dot
	template<class StringClass>
	void extractFilenameAndExtension(const std::string_view& filenameView, StringClass& filenameOut, StringClass& dottedExtensionOut) {
		const size_t stringEnd = filenameView.length();
		const size_t filenameStart = 0;

		size_t extensionStart = stringEnd;

		const size_t dotLocation = filenameView.find_last_of('.');
		if (dotLocation != std::string_view::npos) {
			extensionStart = dotLocation;
		}

		const size_t filenameLength = dotLocation - filenameStart;
		const size_t extensionLength = stringEnd - extensionStart;

		filenameOut = filenameView.substr(filenameStart, filenameLength);
		dottedExtensionOut = filenameView.substr(extensionStart, extensionLength);
	}
	template<class StringClass>
	void extractFilenameAndExtension(const char* filename, StringClass& filenameOut, StringClass& dottedExtensionOut) {
		std::string_view filenameView(filename);

		extractFilenameAndExtension(filenameView, filenameOut, dottedExtensionOut);
	}
	void extractFilenameAndExtension(const char* filename, std::string_view& filenameOut, std::string_view& dottedExtensionOut);
	void extractFilenameAndExtension(const char* filename, std::string& filenameOut, std::string& dottedExtensionOut);


	// extract path, filename and extension components.
	// if a path is present, pathOut will include its trailing slash.
	// filenameOut will receive the base filename
	// if an extension is present, dottedExtensionOut will include its dot
	template<class StringClass>
	void extractPathFilenameAndExtension(const std::string_view& filenameView, StringClass& pathOut, StringClass& filenameOut, StringClass& dottedExtensionOut) {
		const size_t stringEnd = filenameView.length();
		const size_t pathStart = 0;
		size_t filenameStart = pathStart;

		const size_t slashLocation = filenameView.find_last_of("\\/");
		if (slashLocation != std::string_view::npos) {
			filenameStart = slashLocation + 1;
		}

		const size_t pathLength = filenameStart - pathStart;

		pathOut = filenameView.substr(pathStart, pathLength);

		const std::string_view afterPathView = filenameView.substr(filenameStart);
		extractFilenameAndExtension<StringClass>(afterPathView, filenameOut, dottedExtensionOut);
	}
	template<class StringClass>
	void extractPathFilenameAndExtension(const char* filename, StringClass& pathOut, StringClass& filenameOut, StringClass& dottedExtensionOut) {
		std::string_view filenameView(filename);

		extractPathFilenameAndExtension(filenameView, pathOut, filenameOut, dottedExtensionOut);
	}
	void extractPathFilenameAndExtension(const char* filename, std::string_view& pathOut, std::string_view& filenameOut, std::string_view& dottedExtensionOut);
	void extractPathFilenameAndExtension(const char* filename, std::string& pathOut, std::string& filenameOut, std::string& dottedExtensionOut);

	std::string changeFileExtension(const char* filename, const char* newExtension);
	std::wstring changeFileExtension(const wchar_t* filename, const wchar_t* newExtension);
	
	bool isAbsolutePath(const char* path);
	bool isAbsolutePath(const wchar_t* path);
	bool isRelativePath(const char* path);
	bool isRelativePath(const wchar_t* path);
	std::string mergeRelativePath(const char* startDirectory, const char* relativePath);
	std::string createRelativePath(const char* startDirectory, const char* endDirectory);

	bool getFullPathName(const wchar_t* filePath, std::wstring& fullPathOut);
	bool getFullPathName(const char* filePath, std::wstring& fullPathOut);

	bool canonicalizePathName(const wchar_t* filePath, std::wstring& canonicalizedOut);
	bool canonicalizePathName(const char* filePath, std::string& canonicalizedOut);
	bool canonicalizePathName(const std::string_view& filePath, std::string& canonicalizedOut);
	bool canonicalizePathName(const std::wstring& filePath, std::string& canonicalizedOut);

	bool getCurrentDirectory(std::string& currentDirectoryOut);
	bool getCurrentDirectory(std::wstring& currentDirectoryOut);

	// returns true if folder was created or already exists
	bool createFolder(const char* path);
	bool createFolder(const wchar_t* path);
	bool createNestedFolders(const char* path);
	bool createNestedFolders(const wchar_t* path);

	bool moveFile(const char* oldFilename, const char* newFilename, bool flush = true);
	bool moveFile(const wchar_t* oldFilename, const wchar_t* newFilename, bool flush = true);

	bool readEntireFile(const char* filename, ByteBuffer& fileBufferOut);
}
}
