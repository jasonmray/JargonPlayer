
#include "Jargon/StringUtilities.h"
#include "Jargon/System/Utilities.h"

#ifdef _WIN32
	#include "Jargon/System/WindowsDefines.h"
	#include "Jargon/StringUtilities.h"
	#include <windows.h>
	#include <shlobj_core.h>
	#include <shlwapi.h>
	#include <WinBase.h>
#endif

#include <algorithm>
#include <thread>

namespace Jargon{
namespace System{

	#ifdef _WIN32
		void sleep(unsigned int milliseconds){
			Sleep(milliseconds);
		}

		unsigned int getHardwareConcurrencyCount(){
			return std::thread::hardware_concurrency();
		}

		void showFileInExplorer(const char * path) {
			std::wstring widePath = Jargon::StringUtilities::utf8ToWide(path);
			std::replace(widePath.begin(), widePath.end(), '/', '\\');

			PIDLIST_ABSOLUTE pidl = 0;
			SFGAOF flags = 0;
			SHParseDisplayName(widePath.c_str(), NULL, &pidl, 0, &flags);
			try {
				// Open Explorer and select the thing
				SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
			}
			catch(...)
			{
				// Use the task allocator to free to returned pidl
				CoTaskMemFree(pidl);
			}
		}

		void notifyDisplayInUse(bool inUse) {
			if (inUse) {
				SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
			} else {
				SetThreadExecutionState(ES_CONTINUOUS);
			}
		}

		void globFiles(const char * pattern, std::vector<std::string>& files) {
			std::wstring patternWide = Jargon::StringUtilities::utf8ToWide(pattern);

			std::wstring directory;
			{
				wchar_t directoryBuffer[MAX_PATH] = {0};
				wchar_t* filepart = nullptr;
				if (GetFullPathNameW(patternWide.c_str(), MAX_PATH, directoryBuffer, &filepart) != 0) {
					directory = std::wstring(directoryBuffer, filepart);
				}
			}

			WIN32_FIND_DATAW findFileData = {0};
			HANDLE findHandle = FindFirstFileExW(patternWide.c_str(), FindExInfoStandard, &findFileData, FindExSearchNameMatch, NULL, 0);

			if (findHandle == INVALID_HANDLE_VALUE) {
				return;
			}

			do {
				wchar_t fullPath[MAX_PATH] = {0};
				if(PathCombineW(fullPath, directory.c_str(), findFileData.cFileName) != nullptr){
					files.push_back(Jargon::StringUtilities::wideToUtf8(fullPath));
				}
			}while (FindNextFileW(findHandle, &findFileData));

			FindClose(findHandle);
		}

		std::string getClockTimeForCurrentUserLocale() {
			wchar_t buff[128] = { 0 };
			GetTimeFormat(LOCALE_USER_DEFAULT, 0, NULL, NULL, buff, 128);
			return Jargon::StringUtilities::wideToUtf8(buff);
		}

		std::string getUserScreenshotsFolderPath() {
			DWORD flags = 0;
			PWSTR widePath;

			HRESULT result = SHGetKnownFolderPath(FOLDERID_Screenshots, 0, NULL, &widePath);

			if (result != S_OK) {
				result = SHGetKnownFolderPath(FOLDERID_SavedPictures, 0, NULL, &widePath);
			}

			if (result != S_OK) {
				result = SHGetKnownFolderPath(FOLDERID_CameraRoll, 0, NULL, &widePath);
			}

			if (result != S_OK) {
				result = SHGetKnownFolderPath(FOLDERID_Pictures, 0, NULL, &widePath);
			}
			
			if (result != S_OK) {
				result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &widePath);
			}

			if (result != S_OK) {
				result = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &widePath);
			}

			std::string path = Jargon::StringUtilities::wideToUtf8(widePath);
			CoTaskMemFree(widePath);

			return path;
		}
	#endif
}
}
