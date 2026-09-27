#pragma once

#include "Jargon/ErrorContext.h"

#include <string>
#include <string_view>

#include "Jargon/System/WindowsDefines.h"
#include <windows.h>


namespace Jargon{
namespace FileSystem{

	class WindowsFileFinder : public ErrorContext {
		public:
			WindowsFileFinder();
			~WindowsFileFinder();

			bool start(const char* pathExpression);
			bool start(const std::string& pathExpression);
			bool start(const std::string_view& pathExpression);
			void stop();

			// returns false on error or if there is no current file.
			// check hasError() to see if an error occurred.
			bool getCurrentFilename(std::string& filenameOut);

			// returns false on error or if there is no next file.
			// check hasError() to see if an error occurred.
			bool advance();

		private:
			std::string pathExpression;
			WIN32_FIND_DATAW findData;
			HANDLE findHandle;

			bool skipDotDirectories();
			bool findNextFile();
			void setWindowsError(const char* message, DWORD errorCode);

	};

}
}

