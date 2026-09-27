#pragma once

#include <chrono>
#include <cassert>


namespace Jargon{

	class Stopwatch{
		public:
			typedef std::chrono::high_resolution_clock ClockType;

			Stopwatch();
			~Stopwatch();

			void start();
			void restart();
			void stop();

			uint64_t getElapsedMilliseconds() const;
			uint64_t getElapsedMicroseconds() const;

		private:
			ClockType m_clock;
			ClockType::time_point m_startTime;
	};
}
