#pragma once

#include <string>
#include <vector>

#include "Jargon/System/WindowsDefines.h"
#include <windows.h>


namespace Jargon{
namespace System{

	std::string getWindowsErrorMessage(DWORD errorCode);

	class AutoHandle {
		public:
			AutoHandle();
			AutoHandle(HANDLE handle);
			~AutoHandle();

			HANDLE get();

			operator HANDLE ();
			operator bool();

			AutoHandle& operator=(HANDLE handle);

		private:
			HANDLE handle;

			AutoHandle(AutoHandle&) = delete;
			AutoHandle& operator=(AutoHandle&) = delete;
	};

}
}
