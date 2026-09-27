#include "Jargon/Stopwatch.h"


namespace Jargon{

	Stopwatch::Stopwatch() :
		m_startTime(ClockType::time_point::max()) {
	}

	Stopwatch::~Stopwatch() {
	}

	void Stopwatch::start() {
		m_startTime = m_clock.now();
	}

	void Stopwatch::restart() {
		m_startTime = m_clock.now();
	}

	void Stopwatch::stop() {
		m_startTime = ClockType::time_point::max();
	}

	uint64_t Stopwatch::getElapsedMilliseconds() const {
		ClockType::time_point currentTime = m_clock.now();
		auto timeDiff = currentTime - m_startTime;
		return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count());
	}

	uint64_t Stopwatch::getElapsedMicroseconds() const {
		ClockType::time_point currentTime = m_clock.now();
		auto timeDiff = currentTime - m_startTime;
		return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(timeDiff).count());
	}
}
