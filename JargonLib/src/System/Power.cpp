
#include "Jargon/System/Power.h"

#ifdef _WIN32
	#include "Jargon/System/WindowsDefines.h"
	#include <windows.h>
#endif

namespace Jargon{
namespace System{

	#ifdef _WIN32
		bool getDevicePowerState(DevicePowerState& devicePowerStateOut){
			SYSTEM_POWER_STATUS systemPowerStatus = { 0 };
			if (GetSystemPowerStatus(&systemPowerStatus) == FALSE) {
				return false;
			}

			switch (systemPowerStatus.ACLineStatus) {
				case 0:
					devicePowerStateOut.isAcConnected = false;
					break;
				case 1:
					devicePowerStateOut.isAcConnected = true;
					break;
				default:
					devicePowerStateOut.isAcConnected.setUnknown();
					break;
			}

			const BYTE BatteryFlag_Unknown = 255;
			const BYTE BatteryFlag_NoBattery = 128;

			if (systemPowerStatus.BatteryFlag == BatteryFlag_Unknown || systemPowerStatus.BatteryFlag == BatteryFlag_NoBattery) {
				devicePowerStateOut.isBatteryCharging.setUnknown();
			} else if (systemPowerStatus.BatteryFlag & 8) {
				devicePowerStateOut.isBatteryCharging = true;
			} else {
				devicePowerStateOut.isBatteryCharging = false;
			}

			devicePowerStateOut.isBatterySaverOn.setUnknown();
			if (systemPowerStatus.SystemStatusFlag == 1) {
				devicePowerStateOut.isBatterySaverOn = true;
			}

			devicePowerStateOut.batteryRemainingSeconds = (signed long)systemPowerStatus.BatteryLifeTime;
			devicePowerStateOut.batteryFullLifetimeSeconds = (signed long)systemPowerStatus.BatteryFullLifeTime;

			devicePowerStateOut.batteryLifePercent = -1;
			if (systemPowerStatus.BatteryLifePercent >= 0 && systemPowerStatus.BatteryLifePercent <= 100) {
				devicePowerStateOut.batteryLifePercent = systemPowerStatus.BatteryLifePercent;
			}

			return true;
		}
	#else
		bool getDevicePowerState(DevicePowerState& devicePowerStateOut) {
			return false;
		}
	#endif
}
}
