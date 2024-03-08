#ifndef JARGON_SYSTEM_POWER_H
#define JARGON_SYSTEM_POWER_H

#include "../Tri.h"


namespace Jargon{
namespace System{

	struct DevicePowerState {
		Tri isAcConnected = Tri::Unknown;
		Tri isBatteryCharging = Tri::Unknown;
		Tri isBatterySaverOn = Tri::Unknown;
		int batteryLifePercent = -1; // -1 if unknown
		int batteryRemainingSeconds = -1; // -1 if unknown
		int batteryFullLifetimeSeconds = -1; // -1 if unknown
	};

	bool getDevicePowerState(DevicePowerState& devicePowerStateOut);
}
}

#endif
