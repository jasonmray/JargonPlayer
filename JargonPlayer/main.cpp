
extern int appmain(int argc, const char *argv[]);

#include "WindowsDpiFix.h"

#include <windows.h>

#include <string>
#include <vector>
#include <Jargon/StringUtilities.h>


INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	int argc = 0;
	LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);

	std::vector<std::string> argvS(argc);
	std::vector<const char*> argv(argc);

	for(int i = 0; i < argc; i++){
		argvS[i] = Jargon::StringUtilities::wideToUtf8(argvW[i]);
		argv[i] = argvS[i].c_str();
	}

	LocalFree(argvW);

	WindowsDpiFix::FixWindowsDpiHandling();

	int returnCode = appmain(argc, argv.data());

	return returnCode;
}
