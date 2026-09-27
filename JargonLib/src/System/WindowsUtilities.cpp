#include "Jargon/System/WindowsUtilities.h"
#include "Jargon/StringUtilities.h"

#include "Jargon/System/WindowsDefines.h"
#include <windows.h>


namespace Jargon{
namespace System{

	std::string getWindowsErrorMessage(DWORD errorCode) {
		std::string errorMessage;

		const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		LPWSTR messageBuffer = nullptr;

		if (FormatMessageW(flags, nullptr, errorCode, 0, (LPWSTR)&messageBuffer, 0, nullptr) != 0) {
			errorMessage = Jargon::StringUtilities::wideToUtf8(messageBuffer);
		}

		LocalFree(messageBuffer);
		return errorMessage;
	}

	AutoHandle::AutoHandle() :
		handle(INVALID_HANDLE_VALUE)
	{
	}

	AutoHandle::AutoHandle(HANDLE handle) :
		handle(handle)
	{
	}

	AutoHandle::~AutoHandle() {
		if (handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
	}

	HANDLE AutoHandle::get() {
		return handle;
	}

	AutoHandle::operator HANDLE () {
		return handle;
	}

	AutoHandle::operator bool() {
		return handle != INVALID_HANDLE_VALUE;
	}

	AutoHandle& AutoHandle::operator=(HANDLE handle) {
		if (this->handle != INVALID_HANDLE_VALUE) {
			CloseHandle(this->handle);
		}
		this->handle = handle;
		return *this;
	}


}
}
