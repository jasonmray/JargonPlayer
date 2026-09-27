#include "DeviceStatus.h"
#include "VideoWindow.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/System/Power.h"
#include "Jargon/System/Utilities.h"


std::string DeviceStatus::BuildDeviceStatusString(const VideoWindow& videoWindow) {
	const std::string filename = videoWindow.getActiveFilename();
	const std::string time = Jargon::System::getClockTimeForCurrentUserLocale();
	const std::string power = BuildPowerStatusString();

	const int playbackTimeRemainingSeconds = (int)videoWindow.getPlaybackTimeRemaining();
	const std::string remainingTimeString = Util::formatSeconds_HMS(playbackTimeRemainingSeconds);
	return Jargon::StringUtilities::format("\\N\\N%s\n%s\n%s remaining in current file\n%s", filename.c_str(), time.c_str(), remainingTimeString.c_str(), power.c_str());
}

std::string DeviceStatus::BuildPowerStatusString() {

	Jargon::System::DevicePowerState devicePowerState;
	if (Jargon::System::getDevicePowerState(devicePowerState)) {

		std::string acStatus = "";
		if (devicePowerState.isAcConnected == true) {
			if (devicePowerState.isBatteryCharging == true) {
				acStatus = " On AC (charging)";
			}
			else {
				acStatus = " On AC";
			}
		}

		std::string percentLeft = "";
		if (devicePowerState.batteryLifePercent != -1) {
			percentLeft = Jargon::StringUtilities::format(" %i%%", devicePowerState.batteryLifePercent);
		}

		std::string timeStatus = "";
		if (devicePowerState.batteryRemainingSeconds != -1) {
			std::string remainingString = Util::formatSeconds_HMS(devicePowerState.batteryRemainingSeconds);
			timeStatus = Jargon::StringUtilities::format(" %s remaining", remainingString.c_str());
		}

		std::string powerSave = devicePowerState.isBatterySaverOn.isTrue() ? " PowerSave on" : "";
		std::string powerStatus = Jargon::StringUtilities::format("Power:%s%s%s%s", acStatus.c_str(), percentLeft.c_str(), timeStatus.c_str(), powerSave.c_str());

		return powerStatus;
	}

	return "";
}
