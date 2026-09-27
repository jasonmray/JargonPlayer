#pragma once

#include "Jargon/ScopedPointer.h"
#include "Jargon/Macros.h"

#include <cstdint>


namespace Jargon{
namespace System{

	class Semaphore{
		public:
			Semaphore(uint32_t initialValue, uint32_t maxValue);
			explicit Semaphore(uint32_t initialAndMaxValue);
			~Semaphore();

			bool releaseOne();
			bool release(uint32_t count);

			void acquireOne();
			bool tryAcquireOne(uint32_t millisecondsToWait = 0);

		private:
			class SemaphoreImpl;
			Jargon::ScopedPointer<SemaphoreImpl> semaphoreImpl;

			JARGON_DISABLE_COPY(Semaphore);
	};

}
}

