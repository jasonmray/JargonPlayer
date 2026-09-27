#pragma once

#include <string>

class VideoWindow;

class DeviceStatus {
	public:
		static std::string BuildDeviceStatusString(const VideoWindow& videoWindow);
		static std::string BuildPowerStatusString();

	private:

};
