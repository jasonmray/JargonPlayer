
#include "Jargon/System/Clipboard.h"

#ifdef _WIN32
	#include "Jargon/System/WindowsDefines.h"
	#include <windows.h>
	#include <Shlobj.h>
	#include <shellapi.h>
	#include <tchar.h>

#endif

#include <thread>

namespace Jargon{
namespace System{

	#ifdef _WIN32
		bool copyFileToClipboard(const char * filepath){
			const size_t pathSize = strlen(filepath);

			size_t clpSize = sizeof(DROPFILES);
			clpSize += sizeof(char) * (pathSize + 2); // two \0 needed at the end

			// allocate the zero initialized memory
			HDROP hdrop   = (HDROP)GlobalAlloc(GHND, clpSize);
			DROPFILES* df = (DROPFILES*)GlobalLock(hdrop);
			df->pFiles    = sizeof(DROPFILES); // string offset
			df->fWide     = FALSE;

			// copy the command line args to the allocated memory
			char* dstStart = (char*)&df[1];
			strcpy_s(dstStart, pathSize + 1, filepath);
			GlobalUnlock(hdrop);

			// prepare the clipboard
			OpenClipboard(NULL);
			EmptyClipboard();
			SetClipboardData(CF_HDROP, hdrop);
			CloseClipboard();

			return true;
		}
	#else
		bool copyFileToClipboard(const char * filepath) {
			return false;
		}
	#endif
}
}
