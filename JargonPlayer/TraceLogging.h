#pragma once

#include <cstdint>

#define JARGONPLAYER_TRACELOGGING_ENABLED 0

#define TRACE_LITERAL(x) "##x##"

class TraceLogging{
	public:
		static TraceLogging* Instance();

		TraceLogging();
		~TraceLogging();

		void logSdlEvent(uint32_t type, const char* eventName);
		void logSdlKeydown(uint32_t keycode, uint32_t scancode, uint32_t mod);
		void logSdlKeyIgnored(uint32_t keycode, uint32_t scancode, uint32_t mod);
		void logSdlWindowEvent(void* window, uint32_t type);
		void logMpvRedrawEvent(void* window);
		void logMpvWakeupEvent(void* window);
		void logMpvEvent(void* window, uint32_t eventType, const char* eventName);

	private:
};
