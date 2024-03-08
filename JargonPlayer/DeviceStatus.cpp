#include "DeviceStatus.h"
#include "VideoWindow.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/System/Power.h"
#include "Jargon/System/Utilities.h"


std::string DeviceStatus::BuildDeviceStatusString(VideoWindow* videoWindow) {
	std::string filename = videoWindow->getActiveFilename();
	std::string time = Jargon::System::getClockTimeForCurrentUserLocale();
	std::string power = BuildPowerStatusString();
	return Jargon::StringUtilities::format("%s\n%s\n%s", filename.c_str(), time.c_str(), power.c_str());
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
			std::string remainingString = FormatSeconds(devicePowerState.batteryRemainingSeconds);
			timeStatus = Jargon::StringUtilities::format(" %s remaining", remainingString.c_str());
		}

		std::string powerSave = devicePowerState.isBatterySaverOn.isTrue() ? " PowerSave on" : "";
		std::string powerStatus = Jargon::StringUtilities::format("Power:%s%s%s%s", acStatus.c_str(), percentLeft.c_str(), timeStatus.c_str(), powerSave.c_str());

		return powerStatus;
	}

	return "";
}

std::string DeviceStatus::FormatSeconds(int totalSeconds) {
	if (totalSeconds < 60) {
		return Jargon::StringUtilities::format("%is", totalSeconds);
	}
	else if (totalSeconds < 60 * 60) {
		int seconds = totalSeconds % 60;
		int minutes = totalSeconds / 60;
		return Jargon::StringUtilities::format("%im%is", minutes, seconds);
	}
	else {
		int seconds = totalSeconds % 60;
		int minutes = totalSeconds / 60;
		int hours = minutes / 60;
		minutes = minutes % 60;

		return Jargon::StringUtilities::format("%ih%im%is", hours, minutes, seconds);
	}
}