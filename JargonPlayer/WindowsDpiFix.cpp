#include "WindowsDpiFix.h"

#include <ShellScalingApi.h> // Windows header for DPI functions

#pragma comment (lib, "Shcore.lib")


void WindowsDpiFix::FixWindowsDpiHandling(){
	// Tell windows that this process will monitor and react to all DPI changes.
	// We'll do nothing, but Windows won't try to do things for us. This will prevent 
	// scaling of the SDL rendering.

	// Without this, the Windows setting that says "Change the size of text, apps, and other items"
	// will do ugly pixel scaling of the render window.

	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
};
