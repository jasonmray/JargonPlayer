#pragma once

#include <string>
#include <vector>

#include "Jargon/System/WindowsDefines.h"
#include <windows.h>


namespace Jargon{
namespace FileSystem{
namespace WindowShell {

	class CompletedOperations {
		public:
			std::vector<std::wstring> deletedFiles;
	};

	bool moveFilesToRecycleBin(const std::vector<const char*>& filenames, bool allowUserInteraction = true);
	bool moveFilesToRecycleBin(const std::vector<const char*>& filenames, CompletedOperations& completedOperations, bool allowUserInteraction = true);
	bool moveFilesToRecycleBin(HWND parentWindow, const std::vector<const char*>& filenames, bool allowUserInteraction = true);
	bool moveFilesToRecycleBin(HWND parentWindow, const std::vector<const char*>& filenames, CompletedOperations& completedOperations, bool allowUserInteraction = true);

	bool moveFilesToFolder(const std::vector<const char*>& filenames, const std::wstring& destinationFolder, bool allowUserInteraction = true);
	bool moveFilesToFolder(const std::vector<const char*>& filenames, const std::wstring& destinationFolder, CompletedOperations& completedOperations, bool allowUserInteraction = true);
	bool moveFilesToFolder(HWND parentWindow, const std::vector<const char*>& filenames, const std::wstring& destinationFolder, bool allowUserInteraction = true);
	bool moveFilesToFolder(HWND parentWindow, const std::vector<const char*>& filenames, const std::wstring& destinationFolder, CompletedOperations& completedOperations, bool allowUserInteraction = true);
}
}
}

