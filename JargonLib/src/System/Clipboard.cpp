#include "Jargon/System/Clipboard.h"
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/StringUtilities.h"

#ifdef _WIN32
	#include "Jargon/System/WindowsDefines.h"
	#include <windows.h>
	#include <Shlobj.h>
	#include <shellapi.h>
	#include <tchar.h>

#endif

#include <algorithm>
#include <thread>

namespace Jargon{
namespace System{

	#ifdef _WIN32
		enum ClipboardMode {
			Copy,
			Cut
		};

		bool placeFileOnClipboard(const std::wstring& filepath, ClipboardMode clipboardMode) {
			const size_t pathSize = filepath.length();

			// The structure sent to the clipboard will be a DROPFILES struct followed
			// by a double-null terminated wide char path. The double-null is needed 
			// because the mechanism supports multiple null-terminated strings, with a
			// final terminator at the end of the list. But we're only using 1 path here.
			size_t clpSize = sizeof(DROPFILES);
			clpSize += sizeof(wchar_t) * (pathSize + 2); // two \0 needed at the end

			// allocate the zero initialized memory
			HDROP hDrop   = (HDROP)GlobalAlloc(GHND, clpSize);
			if (!hDrop) {
				return false;
			}

			// lock a pointer to write to the memory
			DROPFILES* df = (DROPFILES*)GlobalLock(hDrop);
			if (df == NULL) {
				GlobalFree(hDrop);
				return false;
			}

			// set the byte offset to the list if paths
			df->pFiles    = sizeof(DROPFILES);
			// indicate we're using wide chars
			df->fWide     = TRUE;

			// copy the file path to the allocated memory after the DROPFILES struct
			wchar_t* dstStart = (wchar_t*)&df[1];
			wcscpy_s(dstStart, pathSize + 1, filepath.data());
			GlobalUnlock(hDrop);

			HGLOBAL hDropEffect = NULL;

			if (clipboardMode == ClipboardMode::Cut) {
				// allocate memory for CFSTR_PREFERREDDROPEFFECT
				hDropEffect = GlobalAlloc(GHND, sizeof(DWORD));
				if (!hDropEffect) {
					GlobalFree(hDrop);
					return false;
				}

				// lock a pointer to write to the memory
				DWORD* pDropEffect = (DWORD*)GlobalLock(hDropEffect);
				if (!pDropEffect) {
					GlobalFree(hDrop);
					GlobalFree(hDropEffect);
					return false;
				}

				// indicate to the shell that we want a cut action
				*pDropEffect = DROPEFFECT_MOVE;
				GlobalUnlock(hDropEffect);
			}

			// prepare the clipboard
			if (!OpenClipboard(NULL)) {
				GlobalFree(hDrop);
				if (hDropEffect != NULL) {
					GlobalFree(hDropEffect);
				}
				return false;
			}

			// clear existing contents
			EmptyClipboard();

			// place the handle on the clipboard
			if (!SetClipboardData(CF_HDROP, hDrop)) {
				CloseClipboard();
				GlobalFree(hDrop);
				if (hDropEffect != NULL) {
					GlobalFree(hDropEffect);
				}
				return false;
			}

			if (clipboardMode == ClipboardMode::Cut) {
				UINT format = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
				if (!SetClipboardData(format, hDropEffect)) {
					CloseClipboard();
					// hDrop already owned by system, only free hDropEffect
					GlobalFree(hDropEffect);
					return false;
				}
			}

			CloseClipboard();
			return true;
		}

		bool placeFileOnClipboard(const char* filepath, ClipboardMode clipboardMode) {
			filepath = Jargon::FileSystem::skipLongPathPrefix(filepath);

			std::wstring widePath = Jargon::StringUtilities::utf8ToWide(filepath);

			std::replace(widePath.begin(), widePath.end(), '/', '\\');

			return placeFileOnClipboard(widePath, clipboardMode);
		}

		bool placeFileOnClipboard(const wchar_t* filepath, ClipboardMode clipboardMode) {
			filepath = Jargon::FileSystem::skipLongPathPrefix(filepath);

			std::wstring filepathString = filepath;

			std::replace(filepathString.begin(), filepathString.end(), L'/', L'\\');

			return placeFileOnClipboard(filepathString, clipboardMode);
		}

		bool copyFileToClipboard(const char * filepath) {
			return placeFileOnClipboard(filepath, ClipboardMode::Copy);
		}

		bool copyFileToClipboard(const wchar_t* filepath) {
			return placeFileOnClipboard(filepath, ClipboardMode::Copy);
		}

		bool cutFileToClipboard(const char * filepath) {
			return placeFileOnClipboard(filepath, ClipboardMode::Cut);
		}

		bool cutFileToClipboard(const wchar_t* filepath) {
			return placeFileOnClipboard(filepath, ClipboardMode::Cut);
		}

		bool setClipboardText(const char* text) {
			if (text == nullptr) {
				return false;
			}

			const size_t textLengthWithNull = Jargon::StringUtilities::stringLength(text) + 1;

			// allocate global memory for the text
			HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, textLengthWithNull);
			if (!hGlobal) {
				return false;
			}

			// lock a pointer to write to the memory
			char* pGlobal = (char*)GlobalLock(hGlobal);
			if (!pGlobal) {
				GlobalFree(hGlobal);
				return false;
			}

			// copy the text to the allocated memory
			Jargon::StringUtilities::stringCopy(pGlobal, textLengthWithNull, text);
			GlobalUnlock(hGlobal);

			// open and prepare the clipboard
			if (!OpenClipboard(NULL)) {
				GlobalFree(hGlobal);
				return false;
			}

			// clear existing contents
			EmptyClipboard();

			// place the text on the clipboard
			if (!SetClipboardData(CF_TEXT, hGlobal)) {
				CloseClipboard();
				GlobalFree(hGlobal);
				return false;
			}

			CloseClipboard();
			return true;
		}

		bool setClipboardTextUtf8(const char* text) {
			std::wstring textWide = Jargon::StringUtilities::utf8ToWide(text);
			return setClipboardText(textWide.c_str());
		}

		bool setClipboardText(const wchar_t* text) {
			if (text == nullptr) {
				return false;
			}

			const size_t textLengthWithNull = Jargon::StringUtilities::stringLength(text) + 1;
			size_t bytesNeeded = sizeof(wchar_t) * textLengthWithNull;

			// allocate global memory for the text
			HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, bytesNeeded);
			if (!hGlobal) {
				return false;
			}

			// lock a pointer to write to the memory
			wchar_t* pGlobal = (wchar_t*)GlobalLock(hGlobal);
			if (!pGlobal) {
				GlobalFree(hGlobal);
				return false;
			}

			// copy the text to the allocated memory
			Jargon::StringUtilities::stringCopy(pGlobal, textLengthWithNull, text);
			GlobalUnlock(hGlobal);

			// open and prepare the clipboard
			if (!OpenClipboard(NULL)) {
				GlobalFree(hGlobal);
				return false;
			}

			// clear existing contents
			EmptyClipboard();

			// place the text on the clipboard
			if (!SetClipboardData(CF_UNICODETEXT, hGlobal)) {
				CloseClipboard();
				GlobalFree(hGlobal);
				return false;
			}

			CloseClipboard();
			return true;
		}

	#else
		bool copyFileToClipboard(const char * filepath) {
			return false;
		}
		bool cutFileToClipboard(const char * filepath) {
			return false;
		}
	#endif
}
}
