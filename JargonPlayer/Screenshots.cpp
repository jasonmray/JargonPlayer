
#include "Screenshots.h"
#include "ConfigFile.h"
#include "VideoWindow.h"
#include "Util.h"

#include "Jargon/DebugLog.h"
#include "Jargon/FileSystem/Utilities.h"
#include "Jargon/StringUtilities.h"
#include "Jargon/System/Utilities.h"

#include <chrono>


static void cleanInvalidPathCharacters(std::string& s) {
	static std::string InvalidChars = "\\/:*?\"<>|";
	for (size_t i = 0; i < s.size(); i++) {
		if (InvalidChars.find_first_of(s[i]) != std::string::npos) {
			s[i] = '_';
		}
	}
}

static std::string formatTimestamp(double timePos) {
	std::chrono::milliseconds ms((long long)(timePos * 1000));

	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ms);
	ms -= std::chrono::duration_cast<std::chrono::milliseconds>(seconds);

	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
	seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

	auto hours = std::chrono::duration_cast<std::chrono::hours>(minutes);
	minutes -= std::chrono::duration_cast<std::chrono::minutes>(hours);


	if (hours > std::chrono::hours::zero()) {
		return Jargon::StringUtilities::format("%02dh%02dm%02d.%03ds", hours.count(), minutes.count(), seconds.count(), ms.count());
	} else {
		return Jargon::StringUtilities::format("%02dm%02d.%03ds", minutes.count(), seconds.count(), ms.count());
	}
}

void Screenshots::SaveScreenshotToUserFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType) {
	std::string filename = videoWindow->getActiveFilename();
	std::string baseFilename(Jargon::FileSystem::getBaseFilename(filename.c_str()));
	cleanInvalidPathCharacters(baseFilename);
	double mediaTime = videoWindow->getCurrentPlaybackTime();
	std::string mediaTimeStr = formatTimestamp(mediaTime);

	std::string destinationFolder;
	if (!ConfigFile::Instance.getSettingAsString("ScreenshotsFolder", &destinationFolder)) {
		destinationFolder = Jargon::System::getUserScreenshotsFolderPath();
	}

	std::string screenshotFiletype;
	if (!ConfigFile::Instance.getSettingAsString("ScreenshotFiletype", &screenshotFiletype)) {
		screenshotFiletype = "png";
	}
	

	std::string clockTimeStr = Util::createDateTimeSecondString();
	std::string screenshotFilename = Jargon::StringUtilities::format("%s\\%s_%s_%s.%s", destinationFolder.c_str(), baseFilename.c_str(), mediaTimeStr.c_str(), clockTimeStr.c_str(), screenshotFiletype.c_str());

	SaveScreenshotToSourceFolder(mpv, videoWindow, screenshotType, screenshotFilename.c_str());
}

void Screenshots::SaveScreenshotToSourceFolder(mpv_handle* mpv, VideoWindow* videoWindow, ScreenshotType screenshotType) {
	std::string filename = videoWindow->getActiveFilename();
	std::string baseFilename(Jargon::FileSystem::removeFileExtension(filename.c_str()));
	//cleanInvalidPathCharacters(baseFilename);
	double mediaTime = videoWindow->getCurrentPlaybackTime();
	std::string mediaTimeStr = formatTimestamp(mediaTime);
	std::string clockTimeStr = Util::createDateTimeSecondString();

	std::string screenshotFiletype;
	if (!ConfigFile::Instance.getSettingAsString("ScreenshotFiletype", &screenshotFiletype)) {
		screenshotFiletype = "png";
	}

	std::string screenshotFilename = Jargon::StringUtilities::format("%s_%s_%s.%s", baseFilename.c_str(), mediaTimeStr.c_str(), clockTimeStr.c_str(), screenshotFiletype.c_str());
	
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
		Jargon::debugLog("saving screenshot failed: %s", mpv_error_string(result));
	}
	videoWindow->showMessage(destinationFilename);
}