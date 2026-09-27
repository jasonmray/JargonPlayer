
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/StringBuffer.h"
#include "Jargon/StringUtilities.h"

#include <cassert>
#include <string>
#include <string_view>
#include <fstream>

#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include "Jargon/System/WindowsDefines.h"
#include <windows.h>
#include <fileapi.h>


// for PathAllocCombine(), PathAllocCanonicalize()
#include <PathCch.h>
#pragma comment(lib, "Pathcch.lib")

#endif

namespace Jargon{
namespace FileSystem{

	const int WindowsMaxPathLength = 260;

	const char* WindowsLongNamePrefix = "\\\\?\\";
	const wchar_t* WindowsLongNamePrefixWide = L"\\\\?\\";

	const char* WindowsIllegalFilepathChars = "<>\"|?*";
	const wchar_t* WindowsIllegalFilepathCharsWide = L"<>\"|?*";

	const char* WindowsIllegalFilenameChars = "<>\"|?*:\\/";
	const wchar_t* WindowsIllegalFilenameCharsWide = L"<>\"|?*:\\/";

	bool filenameHasExtension(const char* filename, const char* extension){
		assert(filename != nullptr);
		assert(extension != nullptr);

		if (extension[0] != '.') {
			return Jargon::StringUtilities::stringEndsWith<char>(filename, extension);
		}
		
		size_t filenameLength = strlen(filename);
		size_t extensionLength = strlen(extension);
		size_t dottedExtensionLength = extensionLength + 1;

		if (dottedExtensionLength > filenameLength) {
			return false;
		}

		size_t expectedDotLocation = filenameLength - dottedExtensionLength;
		size_t expectedExtensionStart = expectedDotLocation + 1;
		if (filename[expectedDotLocation] != '.') {
			return false;
		}
		return(_strnicmp(&filename[expectedExtensionStart], extension, extensionLength) == 0);
	}

	bool getFileType(const char* filename, FileType& fileTypeOut) {
		std::wstring filenameWide = Jargon::StringUtilities::utf8ToWide(filename);
		return getFileType(filenameWide.c_str(), fileTypeOut);
	}

	bool getFileType(const wchar_t* filename, FileType& fileTypeOut) {
		const BOOL result = GetFileAttributesW(filename);

		if (result == INVALID_FILE_ATTRIBUTES) {
			fileTypeOut = FileType::Unknown;
			return false;
		}

		if (result & FILE_ATTRIBUTE_DIRECTORY) {
			fileTypeOut = FileType::Folder;
		} else {
			fileTypeOut = FileType::File;
		}

		return true;
	}

	bool fileExists(const char* filename) {
		assert(filename != nullptr);
		return (_access(filename, 4) == 0);
	}

	bool fileExists(const wchar_t* filename) {
		assert(filename != nullptr);
		return (_waccess(filename, 4) == 0);
	}

	bool isDirectory(const char* filename){
		FileType fileType = FileType::Unknown;
		if (!getFileType(filename, fileType)) {
			return false;
		}
		return fileType == FileType::Folder;
	}

	bool isDirectory(const char* filename, bool& existsFlagOut) {
		FileType fileType = FileType::Unknown;
		if (!getFileType(filename, fileType)) {
			existsFlagOut = false;
			return false;
		}
		existsFlagOut = true;
		return fileType == FileType::Folder;
	}

	bool isDirectory(const wchar_t* filename) {
		FileType fileType = FileType::Unknown;
		if (!getFileType(filename, fileType)) {
			return false;
		}
		return fileType == FileType::Folder;
	}

	bool isDirectory(const wchar_t* filename, bool& existsFlagOut) {
		FileType fileType = FileType::Unknown;
		if (!getFileType(filename, fileType)) {
			existsFlagOut = false;
			return false;
		}
		existsFlagOut = true;
		return fileType == FileType::Folder;
	}

	bool getFileSize(const char* filename, int64_t* sizeOut) {
		const std::wstring filenameWide = Jargon::StringUtilities::utf8ToWide(filename);

		WIN32_FILE_ATTRIBUTE_DATA attributeData = {};
		if (!GetFileAttributesExW(filenameWide.c_str(), GetFileExInfoStandard, &attributeData)) {
			return false;
		}

		*sizeOut = ((int64_t)attributeData.nFileSizeHigh << 32) | attributeData.nFileSizeLow;

		return true;
	}

	bool getFileSize(const wchar_t* filename, int64_t* sizeOut) {
		WIN32_FILE_ATTRIBUTE_DATA attributeData = {};
		if (!GetFileAttributesExW(filename, GetFileExInfoStandard, &attributeData)) {
			return false;
		}

		*sizeOut = ((int64_t)attributeData.nFileSizeHigh << 32) | attributeData.nFileSizeLow;

		return true;
	}

	std::string addLongPathPrefixIfNeeded(const std::string& filename) {
		if (isAbsolutePath(filename.c_str()) && filename.length() >= WindowsMaxPathLength) {
			return ensureLongPathPrefix(filename);
		}

		return filename;
	}

	std::wstring addLongPathPrefixIfNeeded(const std::wstring& filename) {
		if (isAbsolutePath(filename.c_str()) && filename.length() >= WindowsMaxPathLength) {
			return ensureLongPathPrefixWide(filename);
		}

		return filename;
	}

	std::string ensureLongPathPrefix(const char* filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, WindowsLongNamePrefix)) {
			return filename;
		}

		return Jargon::StringUtilities::format("%s%s", WindowsLongNamePrefix, filename);
	}

	std::string ensureLongPathPrefix(const std::string& filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, WindowsLongNamePrefix)) {
			return filename;
		}

		return std::string(WindowsLongNamePrefix) + filename;
	}

	std::wstring ensureLongPathPrefixWide(const char* filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, WindowsLongNamePrefix)) {
			return Jargon::StringUtilities::utf8ToWide(filename);
		}

		return Jargon::StringUtilities::formatWide(L"%ls%s", WindowsLongNamePrefixWide, filename);
	}

	std::wstring ensureLongPathPrefixWide(const wchar_t* filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, WindowsLongNamePrefixWide)) {
			return filename;
		}

		return Jargon::StringUtilities::formatWide(L"%ls%ls", WindowsLongNamePrefixWide, filename);
	}

	std::wstring ensureLongPathPrefixWide(const std::wstring& filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, WindowsLongNamePrefixWide)) {
			return filename;
		}

		return std::wstring(WindowsLongNamePrefixWide) + filename;
	}

	const char* skipLongPathPrefix(const char* filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, Jargon::FileSystem::WindowsLongNamePrefix)) {
			filename += Jargon::StringUtilities::stringLength(Jargon::FileSystem::WindowsLongNamePrefix);
		}

		return filename;
	}

	const char* skipLongPathPrefix(const std::string& filename) {
		const char* result = filename.c_str();

		if (Jargon::StringUtilities::stringBeginsWith(filename, Jargon::FileSystem::WindowsLongNamePrefix)) {
			result += Jargon::StringUtilities::stringLength(Jargon::FileSystem::WindowsLongNamePrefix);
		}

		return result;
	}

	const wchar_t* skipLongPathPrefix(const wchar_t* filename) {
		if (Jargon::StringUtilities::stringBeginsWith(filename, Jargon::FileSystem::WindowsLongNamePrefixWide)) {
			filename += Jargon::StringUtilities::stringLength(Jargon::FileSystem::WindowsLongNamePrefixWide);
		}

		return filename;
	}

	const wchar_t* skipLongPathPrefix(const std::wstring& filename) {
		const wchar_t* result = filename.c_str();

		if (Jargon::StringUtilities::stringBeginsWith(filename, Jargon::FileSystem::WindowsLongNamePrefixWide)) {
			result += Jargon::StringUtilities::stringLength(Jargon::FileSystem::WindowsLongNamePrefixWide);
		}

		return result;
	}

	void globFiles(const char* pattern, std::vector<std::string>& files, bool returnAbsolutePaths) {
		std::wstring patternWide = Jargon::StringUtilities::utf8ToWide(pattern);

		std::wstring directory;
		{
			wchar_t directoryBuffer[MAX_PATH] = { 0 };
			wchar_t* filepart = nullptr;
			if (GetFullPathNameW(patternWide.c_str(), MAX_PATH, directoryBuffer, &filepart) != 0) {
				directory = std::wstring(directoryBuffer, filepart);
			}
		}

		WIN32_FIND_DATAW findFileData = { 0 };
		HANDLE findHandle = FindFirstFileExW(patternWide.c_str(), FindExInfoStandard, &findFileData, FindExSearchNameMatch, NULL, 0);

		if (findHandle == INVALID_HANDLE_VALUE) {
			return;
		}

		do {
			if (Jargon::StringUtilities::stringEqual(findFileData.cFileName, L".") || Jargon::StringUtilities::stringEqual(findFileData.cFileName, L"..")) {
				continue;
			}

			if (returnAbsolutePaths) {
				const ULONG combineFlags = PATHCCH_ALLOW_LONG_PATHS;
				PWSTR fullPath = nullptr;

				HRESULT hr = PathAllocCombine(directory.c_str(), findFileData.cFileName, combineFlags, &fullPath);
				if (SUCCEEDED(hr)) {
					files.push_back(Jargon::StringUtilities::wideToUtf8(fullPath));
					LocalFree(fullPath);
				}
			} else {
				files.push_back(Jargon::StringUtilities::wideToUtf8(findFileData.cFileName));
			}
		} while (FindNextFileW(findHandle, &findFileData));

		FindClose(findHandle);
	}
	
	void globFiles(const wchar_t* pattern, std::vector<std::wstring>& files, bool returnAbsolutePaths) {

		std::wstring directory;
		{
			wchar_t directoryBuffer[MAX_PATH] = { 0 };
			wchar_t* filepart = nullptr;
			if (GetFullPathNameW(pattern, MAX_PATH, directoryBuffer, &filepart) != 0) {
				directory = std::wstring(directoryBuffer, filepart);
			}
		}

		WIN32_FIND_DATAW findFileData = { 0 };
		HANDLE findHandle = FindFirstFileExW(pattern, FindExInfoStandard, &findFileData, FindExSearchNameMatch, NULL, 0);

		if (findHandle == INVALID_HANDLE_VALUE) {
			return;
		}

		do {
			if (Jargon::StringUtilities::stringEqual(findFileData.cFileName, L".") || Jargon::StringUtilities::stringEqual(findFileData.cFileName, L"..")) {
				continue;
			}

			if (returnAbsolutePaths) {
				const ULONG combineFlags = PATHCCH_ALLOW_LONG_PATHS;
				PWSTR fullPath = nullptr;

				HRESULT hr = PathAllocCombine(directory.c_str(), findFileData.cFileName, combineFlags, &fullPath);
				if (SUCCEEDED(hr)) {
					files.push_back(fullPath);
					LocalFree(fullPath);
				}
			} else {
				files.push_back(findFileData.cFileName);
			}
		} while (FindNextFileW(findHandle, &findFileData));

		FindClose(findHandle);
	}

	std::string_view getBaseFilename(const char* sourceFilename){
		std::string_view filenameView(sourceFilename);

		size_t filenameStart = 0;
		size_t filenameEnd = 0;

		size_t slashLocation = filenameView.find_last_of("\\/");
		if (slashLocation != std::string_view::npos) {
			filenameStart = slashLocation + 1;
		}

		size_t dotLocation = std::string_view(sourceFilename + filenameStart).find_last_of('.');
		if (dotLocation == std::string_view::npos) {
			filenameEnd = filenameView.length();
		} else {
			filenameEnd = filenameStart + dotLocation;
		}

		return filenameView.substr(filenameStart, filenameEnd - filenameStart);
	}

	std::string_view getFileExtension(const char* filename) {
		std::string_view filenameView(filename);

		size_t extensionStart = 0;

		size_t dotLocation = filenameView.find_last_of('.');
		if (dotLocation == std::string_view::npos) {
			extensionStart = 0;
		} else {
			extensionStart = dotLocation + 1;
		}

		return filenameView.substr(extensionStart, filenameView.length());
	}

	std::string_view removeFileExtension(const char* filename) {
		std::string_view filenameView(filename);

		size_t dotLocation = filenameView.find_last_of('.');
		if (dotLocation == std::string_view::npos) {
			return filenameView;
		} else {
			return filenameView.substr(0, dotLocation);
		}
	}

	void extractPathAndFilename(const char* filename, std::string& pathOut, std::string& filenameAndExtensionOut) {
		extractPathAndFilename<std::string>(filename, pathOut, filenameAndExtensionOut);
	}

	void extractPathAndFilename(const char* filename, std::string_view& pathOut, std::string_view& filenameAndExtensionOut) {
		extractPathAndFilename<std::string_view>(filename, pathOut, filenameAndExtensionOut);
	}

	void extractPathAndFilename(const wchar_t* filename, std::wstring& pathOut, std::wstring& filenameAndExtensionOut) {
		extractPathAndFilename<std::wstring>(filename, pathOut, filenameAndExtensionOut);
	}

	void extractPathAndFilename(const wchar_t* filename, std::wstring_view& pathOut, std::wstring_view& filenameAndExtensionOut) {
		extractPathAndFilename<std::wstring_view>(filename, pathOut, filenameAndExtensionOut);
	}

	void extractFilenameAndExtension(const char* filename, std::string_view& filenameOut, std::string_view& dottedExtensionOut) {
		extractFilenameAndExtension<std::string_view>(filename, filenameOut, dottedExtensionOut);
	}

	void extractFilenameAndExtension(const char* filename, std::string& filenameOut, std::string& dottedExtensionOut) {
		extractFilenameAndExtension<std::string>(filename, filenameOut, dottedExtensionOut);
	}

	void extractPathFilenameAndExtension(const char* filename, std::string_view& pathOut, std::string_view& filenameOut, std::string_view& dottedExtensionOut) {
		extractPathFilenameAndExtension<std::string_view>(filename, pathOut, filenameOut, dottedExtensionOut);
	}

	void extractPathFilenameAndExtension(const char* filename, std::string& pathOut, std::string& filenameOut, std::string& dottedExtensionOut) {
		extractPathFilenameAndExtension<std::string>(filename, pathOut, filenameOut, dottedExtensionOut);
	}

	std::string changeFileExtension(const char* filename, const char* newExtension) {
		std::string changed;
		std::string::size_type dotLocation;

		changed = filename;
		dotLocation = changed.rfind('.');

		if (dotLocation == std::string::npos) {
			changed.erase(dotLocation);
		}

		changed += '.';
		changed += newExtension;

		return changed;
	}

	std::wstring changeFileExtension(const wchar_t* filename, const wchar_t* newExtension) {
		std::wstring changed;
		std::wstring::size_type dotLocation;

		changed = filename;
		dotLocation = changed.rfind(L'.');

		if (dotLocation == std::wstring::npos) {
			changed.erase(dotLocation);
		}

		changed += '.';
		changed += newExtension;

		return changed;
	}

	std::string_view getFilenameComponent(const char* filePath){
		assert(filePath != nullptr);

		std::string_view filePathView(filePath);
		size_t slashLocation = filePathView.find_last_of("\\/");

		if (slashLocation != std::string::npos) {
			return filePathView.substr(slashLocation + 1);
		} else {
			return filePathView;
		}
	}

	std::string_view getPathComponent(const char* filePath)
	{
		assert(filePath != nullptr);

		std::string_view filePathView(filePath);
		size_t slashLocation = filePathView.find_last_of("\\/");

		if (slashLocation != std::string::npos) {
			return filePathView.substr(0, slashLocation + 1);
		}
		else {
			return std::string_view();
		}
	}

	bool isAbsolutePath(const char* path){
		size_t length = Jargon::StringUtilities::stringLength(path);

		if (length > 0 && path[0] == '\\') {
			return true;
		}

		if (length > 0 && path[0] == '/') {
			return true;
		}

		if (length > 1 && path[1] == ':') {
			return true;
		}

		return false;
	}

	bool isAbsolutePath(const wchar_t* path) {
		size_t length = Jargon::StringUtilities::stringLength(path);

		if (length > 0 && path[0] == L'\\') {
			return true;
		}

		if (length > 0 && path[0] == L'/') {
			return true;
		}

		if (length > 1 && path[1] == L':') {
			return true;
		}

		return false;
	}

	bool isRelativePath(const char* path) {
		return !isAbsolutePath(path);
	}

	bool isRelativePath(const wchar_t* path) {
		return !isAbsolutePath(path);
	}

	std::string mergeRelativePath(const char* startDirectory, const char* relativePath)
	{
		const size_t startDirectoryLength = Jargon::StringUtilities::stringLength(startDirectory);
		const size_t relativePathLength = Jargon::StringUtilities::stringLength(relativePath);

		if (startDirectoryLength == 0 || relativePathLength == 0) {
			std::string mergedPath = startDirectory;
			mergedPath += relativePath;
			return mergedPath;
		}

		std::vector<std::string> mergedPathFolders;
		Jargon::StringUtilities::tokenizeString(startDirectory, "\\/", mergedPathFolders);
		Jargon::StringUtilities::tokenizeString(relativePath, "\\/", mergedPathFolders);

		char folderSeparator = '\\';
		const bool startDirectoryIsAbsolute = isAbsolutePath(startDirectory);
		std::string rootDevice;
		size_t sourceIndex = 0;

		if (startDirectoryIsAbsolute) {
			if (startDirectoryLength > 1) {
				if (startDirectory[1] == ':') {
					// drive location like 'c:'
					rootDevice = mergedPathFolders[0];
					rootDevice += "\\";

					sourceIndex++;
				} else if (startDirectory[0] == '\\' && startDirectory[1] == '\\') {
					// path starting with '\\'
					rootDevice = "\\\\";
					rootDevice += mergedPathFolders[0];
					rootDevice += "\\";

					sourceIndex++;
				} else if (startDirectory[0] == '/' && startDirectory[1] == '/') {
					// path starting with '//'
					rootDevice = "//";
					rootDevice += mergedPathFolders[0];
					rootDevice += "/";

					folderSeparator = '/';

					sourceIndex++;
				} else {
					rootDevice = startDirectory[0];
					folderSeparator = startDirectory[0];
				}
			} else {
				rootDevice = startDirectory[0];
				folderSeparator = startDirectory[0];
			}
		}

		size_t destinationIndex = 0;

		while( sourceIndex < mergedPathFolders.size() ){

			if ( mergedPathFolders[sourceIndex] == "" ){
				sourceIndex++;
			} else if ( mergedPathFolders[sourceIndex] == "." ){
				sourceIndex++;
			} else if ( mergedPathFolders[sourceIndex] == ".." ){
				if( destinationIndex == 0 ){
					if( destinationIndex != sourceIndex ){
						mergedPathFolders[destinationIndex] = mergedPathFolders[sourceIndex];
					}
					destinationIndex++;
				}else{
					if( mergedPathFolders[destinationIndex-1] == ".." ){
						if( destinationIndex != sourceIndex ){
							mergedPathFolders[destinationIndex] = mergedPathFolders[sourceIndex];
						}
						destinationIndex++;
					}else{
						destinationIndex--;
					}
				}
				sourceIndex++;
			}else{
				if( destinationIndex != sourceIndex ){
					mergedPathFolders[destinationIndex] = mergedPathFolders[sourceIndex];
				}
				destinationIndex++;
				sourceIndex++;
			}
		}

		mergedPathFolders.erase(mergedPathFolders.begin() + destinationIndex, mergedPathFolders.end());

		if (startDirectoryIsAbsolute) {
			while (mergedPathFolders.empty() == false && mergedPathFolders[0] == "..") {
				mergedPathFolders.erase(mergedPathFolders.begin());
			}
		}

		std::string mergedPath = rootDevice;

		if (mergedPathFolders.empty() == false) {
			size_t i;
			for (i = 0; i < mergedPathFolders.size() - 1; i++) {
				mergedPath += mergedPathFolders[i];
				mergedPath += folderSeparator;
			}
			mergedPath += mergedPathFolders[i];
		}

		return mergedPath;
	}

	std::string createRelativePath(const char* startDirectory, const char* endDirectory){
		
		char separator = '\\';

		if (strchr(startDirectory, '/') != nullptr || strchr(endDirectory, '/') != nullptr) {
			separator = '/';
		}

		std::vector<std::string> startDirectoryTokens;
		Jargon::StringUtilities::tokenizeString(startDirectory, "\\/", startDirectoryTokens);
		if (startDirectoryTokens.empty()) {
			return endDirectory;
		}

		std::vector<std::string> endDirectoryTokens;
		Jargon::StringUtilities::tokenizeString(endDirectory, "\\/", endDirectoryTokens);
		if (endDirectoryTokens.empty()) {
			return startDirectory;
		}

		if (!Jargon::StringUtilities::stringEqualCaseInsensitive(startDirectoryTokens[0], endDirectoryTokens[0])) {
			return endDirectory;
		}

		while (!startDirectoryTokens.empty() && !endDirectoryTokens.empty()){
			if (Jargon::StringUtilities::stringEqualCaseInsensitive(startDirectoryTokens[0], endDirectoryTokens[0])) {
				startDirectoryTokens.erase(startDirectoryTokens.begin());
				endDirectoryTokens.erase(endDirectoryTokens.begin());
			} else {
				break;
			}
		}

		std::vector <std::string> relativePathTokens;
		for (size_t i = 0; i < startDirectoryTokens.size(); i++){
			relativePathTokens.push_back("..");
		}
		for (size_t i = 0; i < endDirectoryTokens.size(); i++){
			relativePathTokens.push_back(endDirectoryTokens[i]);
		}

		if (relativePathTokens.empty()) {
			return ".";
		}

		std::string relativePath;
		for (size_t i = 0; i < relativePathTokens.size(); i++){
			if (i != 0){
				relativePath += separator;
			}

			relativePath += relativePathTokens[i];
		}
		return relativePath;
	}

	bool getFullPathName(const wchar_t* filePath, std::wstring& fullPathOut) {
		const DWORD requiredSize = GetFullPathNameW(filePath, 0, nullptr, nullptr);

		std::vector<wchar_t> buffer(requiredSize);
		if (buffer.size() > MAXDWORD) {
			return false;
		}

		const DWORD result = GetFullPathNameW(filePath, (DWORD)buffer.size(), buffer.data(), nullptr);
		const DWORD numCharsWritten = result + 1;

		if (result == 0) {
			return false;
		}

		fullPathOut.assign(buffer.data());
		return true;
	}

	bool getFullPathName(const char* filePath, std::wstring& fullPathOut) {
		const std::wstring filePathWide = Jargon::StringUtilities::utf8ToWide(filePath);
		return getFullPathName(filePathWide.c_str(), fullPathOut);
	}

	bool canonicalizePathName(const wchar_t* filePath, std::wstring& canonicalizedOut) {
		std::wstring fullPathWide;
		if (!getFullPathName(filePath, fullPathWide)) {
			return false;
		}

		const ULONG flags = PATHCCH_ALLOW_LONG_PATHS | PATHCCH_CANONICALIZE_SLASHES;
		PWSTR finalPath;
		HRESULT hr = PathAllocCanonicalize(filePath, flags, &finalPath);
		if (SUCCEEDED(hr)) {
			canonicalizedOut = finalPath;
			LocalFree(finalPath);
			return true;
		}

		return false;
	}

	bool canonicalizePathName(const char* filePath, std::string& canonicalizedOut) {
		const std::wstring filePathWide = Jargon::StringUtilities::utf8ToWide(filePath);
		return canonicalizePathName(filePathWide, canonicalizedOut);
	}

	bool canonicalizePathName(const std::wstring& filePath, std::string& canonicalizedOut) {
		std::wstring fullPathWide;
		if(!getFullPathName(filePath.c_str(), fullPathWide)){
			return false;
		}

		const ULONG flags = PATHCCH_ALLOW_LONG_PATHS | PATHCCH_CANONICALIZE_SLASHES;
		PWSTR finalPath;
		HRESULT hr = PathAllocCanonicalize(fullPathWide.c_str(), flags, &finalPath);
		if (SUCCEEDED(hr)) {
			canonicalizedOut = Jargon::StringUtilities::wideToUtf8(finalPath);
			LocalFree(finalPath);
			return true;
		}

		return false;
	}

	bool getCurrentDirectory(std::string& currentDirectoryOut) {
		std::wstring currentDirectoryWide;
		if (!getCurrentDirectory(currentDirectoryWide)) {
			return false;
		}

		currentDirectoryOut = Jargon::StringUtilities::wideToUtf8(currentDirectoryWide.c_str());
		return true;
	}

	bool getCurrentDirectory(std::wstring& currentDirectoryOut) {
		const DWORD requiredSizeWChars = GetCurrentDirectoryW(0, nullptr);
		currentDirectoryOut.resize((size_t)requiredSizeWChars);
		GetCurrentDirectoryW((DWORD)currentDirectoryOut.size() + 1, currentDirectoryOut.data());
		return true;
	}

	bool createFolder(const char* path) {
		std::wstring pathWide = Jargon::StringUtilities::utf8ToWide(path);
		return createFolder(pathWide.c_str());
	}

	bool createFolder(const wchar_t* path) {
		const BOOL result = CreateDirectoryW(path, nullptr);
		if (result != 0) {
			return true;
		}

		const DWORD lastError = GetLastError();
		if (lastError == ERROR_ALREADY_EXISTS) {
			return true;
		}

		return false;
	}

	bool createNestedFolders(const char* path) {
		std::wstring pathWide = Jargon::StringUtilities::utf8ToWide(path);
		return createNestedFolders(pathWide.c_str());
	}

	bool createNestedFolders(const wchar_t* path) {

		FileType fileType = FileType::Unknown;
		if (getFileType(path, fileType)) {
			// file with this name already exists.
			// return true if it is a folder
			return fileType == FileType::Folder;
		}

		std::basic_string_view<wchar_t> pathView(path);

		// trim trailing slash if present
		if (Jargon::StringUtilities::stringEndsWith(pathView, L'\\') || Jargon::StringUtilities::stringEndsWith(pathView, L'/')) {
			pathView = pathView.substr(0, pathView.length() - 1);
		}

		// extract the parent folder
		std::basic_string<wchar_t> parentPath;
		std::basic_string<wchar_t> folder;
		extractPathAndFilename<std::basic_string<wchar_t>>(pathView, parentPath, folder);

		// make sure the parent folder exists
		if (!parentPath.empty()){
			if (!createNestedFolders(parentPath.c_str())) {
				return false;
			}
		}

		// try to create this folder
		if (!createFolder(path)) {
			return false;
		}

		return true;
	}

	bool moveFile(const char* oldFilename, const char* newFilename, bool flush) {
		const std::wstring oldFilenameWide = Jargon::StringUtilities::utf8ToWide(oldFilename);
		const std::wstring newFilenameWide = Jargon::StringUtilities::utf8ToWide(newFilename);
		return moveFile(oldFilenameWide.c_str(), newFilenameWide.c_str(), flush);
	}

	bool moveFile(const wchar_t* oldFilename, const wchar_t* newFilename, bool flush) {
		DWORD flags = 0;
		if (flush) {
			flags |= MOVEFILE_WRITE_THROUGH;
		}
		return MoveFileExW(oldFilename, newFilename, flags) == TRUE;
	}

	bool canonicalizePathName(const std::string_view& filePath, std::string& canonicalizedOut) {
		const std::wstring filePathWide = Jargon::StringUtilities::utf8ToWide(filePath);
		return canonicalizePathName(filePathWide, canonicalizedOut);
	}

	bool readEntireFile(const char* filename, ByteBuffer& fileBufferOut) {
		std::ifstream inputStream(filename, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
		if (!inputStream.is_open()) {
			return false;
		}

		std::ifstream::pos_type fileSizeBytes = inputStream.tellg();
		inputStream.seekg(0, std::ios::beg);

		if (fileSizeBytes > MAXSIZE_T) {
			return false;
		}

		fileBufferOut.createNewBuffer(fileSizeBytes);

		inputStream.read((char *)fileBufferOut.getBuffer(), fileBufferOut.getSizeBytes());

		return true;
	}

}
}
