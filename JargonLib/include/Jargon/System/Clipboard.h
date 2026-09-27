
#ifndef JARGON_SYSTEM_CLIPBOARD_H
#define JARGON_SYSTEM_CLIPBOARD_H


namespace Jargon{
namespace System{

	bool copyFileToClipboard(const char * filepath);
	bool copyFileToClipboard(const wchar_t* filepath);
	bool cutFileToClipboard(const char * filepath);
	bool cutFileToClipboard(const wchar_t* filepath);
	bool setClipboardText(const char* text);
	bool setClipboardTextUtf8(const char* text);
	bool setClipboardText(const wchar_t* text);
}
}

#endif
