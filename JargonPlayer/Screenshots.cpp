
#include "Screenshots.h"
#include "VideoWindow.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/System/Utilities.h"

void Screenshots::SaveScreenshotToUserFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType) {
	std::string filename = videoWindow->getActiveFilename();
	std::string baseFilename = Util::getBaseFilenameNoExt(filename.c_str());
	std::string picturesFolder = Jargon::System::getUserScreenshotsFolderPath();
	std::string timestamp = Util::createDateTimeSecondString();
	std::string screenshotFilename = Jargon::StringUtilities::format("%s\\%s_%s.png", picturesFolder.c_str(), baseFilename.c_str(), timestamp.c_str());

	SaveScreenshotToSourceFolder(mpv, videoWindow, screenshotType, screenshotFilename.c_str());
}

void Screenshots::SaveScreenshotToSourceFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType) {
	std::string filename = videoWindow->getActiveFilename();
	std::string baseFilename = Util::getPathAndFilenameNoExt(filename.c_str());
	std::string timestamp = Util::createDateTimeSecondString();
	std::string screenshotFilename = Jargon::StringUtilities::format("%s_%s.png", baseFilename.c_str(), timestamp.c_str());
	
	SaveScreenshotToSourceFolder(mpv, videoWindow, screenshotType, screenshotFilename.c_str());
}

void Screenshots::SaveScreenshotToSourceFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType, const char* destinationFilename) {
	const char* params = "";
	if (screenshotType == ScreenshotType_OriginalVideo) {
		params = "subtitles";
	}
	else if (screenshotType == ScreenshotType_WindowDisplay) {
		params = "window";
	}

	const char* command[] = { "screenshot-to-file", destinationFilename, params, 0 };
	int result = mpv_command(mpv, command);
	if (result < 0) {
		Util::log("saving screenshot failed: %s", mpv_error_string(result));
	}
	videoWindow->showMessage(destinationFilename);
}