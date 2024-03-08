#pragma once

#include <string>

class VideoWindow;

class DeviceStatus {
	public:
		static std::string BuildDeviceStatusString(VideoWindow* videoWindow);
		static std::string BuildPowerStatusString();
		static std::string FormatSeconds(int totalSeconds);

	private:

};
