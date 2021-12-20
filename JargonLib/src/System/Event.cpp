
#include "Jargon/System/Event.h"
#include <windows.h>
#include <system_error>

namespace Jargon{
namespace System{

	Event::Event(bool autoReset){
		eventHandle = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
	}

	Event::~Event(){
		CloseHandle(eventHandle);
	}

	void Event::wait() {
		switch (WaitForSingleObject(eventHandle, INFINITE)) {
			case WAIT_OBJECT_0:
				return;
			default:
				throw new std::system_error(std::make_error_code(std::errc::timed_out), "wait for event failed");
		}
	}

	bool Event::tryWait(uint32_t milliseconds) {
		switch (WaitForSingleObject(eventHandle, milliseconds)) {
			case WAIT_TIMEOUT:
				return false;
			case WAIT_OBJECT_0:
				return true;
			default:
				throw new std::system_error(std::make_error_code(std::errc::timed_out), "wait for event failed");
		}
	}

	void Event::set() {
		if (!SetEvent(eventHandle)) {
			throw new std::system_error(std::make_error_code(std::errc::timed_out), "wait for event failed");
		}
	}

	void Event::reset() {
		if (!ResetEvent(eventHandle)) {
			throw new std::system_error(std::make_error_code(std::errc::timed_out), "wait for event failed");
		}
	}
}
}
