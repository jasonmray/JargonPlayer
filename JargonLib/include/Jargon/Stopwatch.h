#ifndef JARGON_STOPWATCH_H
#define JARGON_STOPWATCH_H

#include <chrono>
#include <cassert>


namespace Jargon{

	/**
		A basic stopwatch
	*/
	class Stopwatch{
		public:
			typedef std::chrono::high_resolution_clock ClockType;
			Stopwatch():
				m_startTime(ClockType::time_point::max())
			{
			}

			void start(){
				m_startTime = m_clock.now();
			}

			void restart(){
				m_startTime = m_clock.now();
			}

			void stop(){
				m_startTime = ClockType::time_point::max();
			}

			uint64_t getElapsedMilliseconds() const{
				ClockType::time_point currentTime = m_clock.now();
				auto timeDiff = currentTime - m_startTime;
				return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count());
			}

			uint64_t getElapsedMicroseconds() const {
				ClockType::time_point currentTime = m_clock.now();
				auto timeDiff = currentTime - m_startTime;
				return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(timeDiff).count());
			}

		private:
			ClockType m_clock;
			ClockType::time_point m_startTime;
	};
}

#endif
