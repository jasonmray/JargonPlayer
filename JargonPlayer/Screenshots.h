#pragma once

#include <string>

struct mpv_handle;
class VideoWindow;

enum ScreenshotType {
	ScreenshotType_OriginalVideo,
	ScreenshotType_WindowDisplay
};

class Screenshots{
	public:
		// save screenshot to a system-defined user folder, like Screenshots, Pictures, or My Documents
		static void SaveScreenshotToUserFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType);

		// save screenshot to same folder as source file
		static void SaveScreenshotToSourceFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType);

		static void SaveScreenshotToSourceFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType, const char * destinationFilename);

	private:

};
