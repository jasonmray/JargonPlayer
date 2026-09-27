
#include "Jargon/FileSystem/WindowsFileFinder.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/System/WindowsUtilities.h"


namespace Jargon{
namespace FileSystem{

	WindowsFileFinder::WindowsFileFinder() {
		findData = {};
		findHandle = INVALID_HANDLE_VALUE;
	}

	bool WindowsFileFinder::start(const char* pathExpression) {
		return start(std::string_view(pathExpression));
	}

	bool WindowsFileFinder::start(const std::string& pathExpression) {
		return start(std::string_view(pathExpression));
	}

	bool WindowsFileFinder::start(const std::string_view& pathExpression){
		stop();

		this->pathExpression = pathExpression;

		std::wstring findPath = Jargon::StringUtilities::utf8ToWide(pathExpression);
		findHandle = ::FindFirstFileW(findPath.c_str(), &findData);

		if (findHandle == INVALID_HANDLE_VALUE) {
			DWORD error = GetLastError();
			if (error != ERROR_NO_MORE_FILES) {
				setWindowsError("FindFirstFile() failed", error);
				return false;
			}
		} else {
			skipDotDirectories();
		}

		return true;
	}

	void WindowsFileFinder::stop() {
		if (findHandle != INVALID_HANDLE_VALUE) {
			FindClose(findHandle);
		}

		pathExpression.clear();
		findData = {};
		findHandle = INVALID_HANDLE_VALUE;
		clearLastError();
	}

	WindowsFileFinder::~WindowsFileFinder(){
		stop();
	}

	bool WindowsFileFinder::getCurrentFilename(std::string& filenameOut) {
		if (findHandle == INVALID_HANDLE_VALUE) {
			return false;
		}
		filenameOut = Jargon::StringUtilities::wideToUtf8(findData.cFileName);
		return true;
	}

	bool WindowsFileFinder::advance() {
		if (findHandle == INVALID_HANDLE_VALUE) {
			return false;
		}

		if (!findNextFile()) {
			return false;
		}

		if (!skipDotDirectories()) {
			return false;
		}

		return true;
	}

	bool WindowsFileFinder::skipDotDirectories() {
		if (findHandle == INVALID_HANDLE_VALUE) {
			return false;
		}

		while (Jargon::StringUtilities::stringEqual(L".", findData.cFileName) || Jargon::StringUtilities::stringEqual(L"..", findData.cFileName)) {
			if (!findNextFile()) {
				return false;
			}
		}

		return true;
	}

	bool WindowsFileFinder::findNextFile() {
		if (FindNextFileW(findHandle, &findData) == 0) {
			const DWORD error = GetLastError();

			if (error != ERROR_NO_MORE_FILES) {
				setWindowsError("FindNextFile() failed", error);
			}

			if (findHandle != INVALID_HANDLE_VALUE) {
				FindClose(findHandle);
			}

			findData = {};
			findHandle = INVALID_HANDLE_VALUE;

			return false;
		}

		return true;
	}

	void WindowsFileFinder::setWindowsError(const char* message, DWORD errorCode) {
		std::string systemMessage = Jargon::System::getWindowsErrorMessage(errorCode);
		setLastError("%s reading %s : %s", message, pathExpression.c_str(), systemMessage.c_str());
		
	}

}
}
