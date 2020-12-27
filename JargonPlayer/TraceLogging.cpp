#include "TraceLogging.h"

#include <memory>


#ifdef JARGONPLAYER_TRACELOGGING_ENABLED
	#include <windows.h>
	#include <TraceLoggingProvider.h>  // The native windows TraceLogging API
#else
	#define TRACELOGGING_DECLARE_PROVIDER(...)
	#define TRACELOGGING_DEFINE_PROVIDER(...)
	#define TraceLoggingRegister(...)
	#define TraceLoggingUnregister(...)
	#define TraceLoggingWrite(...)
	#define TraceLoggingWrite(...) 
#endif


TRACELOGGING_DECLARE_PROVIDER(g_jargonPlayerProvider);

// Define the GUID to use in TraceLoggingProviderRegister 
// {E5800F1E-6090-46B8-907D-A6D3A34FEBFE}
TRACELOGGING_DEFINE_PROVIDER(
	g_jargonPlayerProvider,
	"JargonPlayer",
	(0xe5800f1e, 0x6090, 0x46b8, 0x90, 0x7d, 0xa6, 0xd3, 0xa3, 0x4f, 0xeb, 0xfe));



static std::unique_ptr<TraceLogging> g_instance = nullptr;

TraceLogging* TraceLogging::Instance() {
	if (g_instance == nullptr) {
		g_instance = std::make_unique<TraceLogging>();
	}

	return g_instance.get();
}

TraceLogging::TraceLogging(){
	TraceLoggingRegister(g_jargonPlayerProvider);
}

TraceLogging::~TraceLogging(){
	TraceLoggingUnregister(g_jargonPlayerProvider);
}

void TraceLogging::logSdlEvent(uint32_t type, const char* eventName) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"SdlEvent",
		TraceLoggingValue(type, "EventType"),
		TraceLoggingString(eventName, "EventName"));
}

void TraceLogging::logSdlKeydown(uint32_t keycode, uint32_t scancode, uint32_t mod) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"SdlKeydown",
		TraceLoggingValue(keycode, "Keycode"),
		TraceLoggingValue(scancode, "Scancode"),
		TraceLoggingValue(mod, "ModifierFlags")
	);
}

void TraceLogging::logSdlKeyIgnored(uint32_t keycode, uint32_t scancode, uint32_t mod) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"SdlKeyIgnored",
		TraceLoggingValue(keycode, "Keycode"),
		TraceLoggingValue(scancode, "Scancode"),
		TraceLoggingValue(mod, "ModifierFlags")
	);
}

void TraceLogging::logSdlWindowEvent(void* window, uint32_t type) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"SdlWindowEvent",
		TraceLoggingValue(type, "EventType"),
		TraceLoggingPointer(window, "WindowPtr"));
}

void TraceLogging::logMpvRedrawEvent(void* window) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"MpvRedrawEvent",
		TraceLoggingPointer(window, "WindowPtr"));
}

void TraceLogging::logMpvWakeupEvent(void* window) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"MpvWakeupEvent",
		TraceLoggingPointer(window, "WindowPtr"));
}

void TraceLogging::logMpvEvent(void* window, uint32_t eventType, const char * eventName) {
	TraceLoggingWrite(
		g_jargonPlayerProvider,
		"MpvEvent",
		TraceLoggingValue(eventType, "EventType"),
		TraceLoggingString(eventName, "EventName"),
		TraceLoggingPointer(window, "WindowPtr"));
}
