
#include "Jargon/System/Semaphore.h"
#include "Jargon/System/WindowsUtilities.h"

#include "Jargon/System/WindowsDefines.h"
#include <Windows.h>

#include <limits>

namespace Jargon{
namespace System{

	class Semaphore::SemaphoreImpl {
		public:
			static const uint32_t MaxSemaphoreValue = (uint32_t)std::numeric_limits<LONG>::max();

			SemaphoreImpl(uint32_t initialValue, uint32_t maxValue) {
				assert(initialValue < MaxSemaphoreValue);
				assert(maxValue < MaxSemaphoreValue);
				assert(maxValue > 0);
				assert(maxValue >= initialValue);
				semaphoreHandle = ::CreateSemaphoreW(nullptr, initialValue, maxValue, nullptr);
			}

			~SemaphoreImpl() {
			}

			bool releaseOne() {
				return ::ReleaseSemaphore(semaphoreHandle, 1, nullptr) != 0;
			}

			bool release(uint32_t count) {
				return ::ReleaseSemaphore(semaphoreHandle, count, nullptr) != 0;
			}

			void acquireOne() {
				::WaitForSingleObject(semaphoreHandle, INFINITE);
			}

			bool tryAcquireOne(uint32_t millisecondsToWait) {
				return ::WaitForSingleObject(semaphoreHandle, millisecondsToWait) == WAIT_OBJECT_0;
			}
		private:
			Jargon::System::AutoHandle semaphoreHandle;
	};

	Semaphore::Semaphore(uint32_t initialValue, uint32_t maxValue) {
		semaphoreImpl = new SemaphoreImpl(initialValue, maxValue);
	}

	Semaphore::Semaphore(uint32_t initialAndMaxValue) {
		semaphoreImpl = new SemaphoreImpl(initialAndMaxValue, initialAndMaxValue);
	}

	Semaphore::~Semaphore() {
	}

	bool Semaphore::releaseOne() {
		return semaphoreImpl->releaseOne();
	}

	bool Semaphore::release(uint32_t count) {
		return semaphoreImpl->release(count);
	}

	void Semaphore::acquireOne() {
		return semaphoreImpl->acquireOne();
	}

	bool Semaphore::tryAcquireOne(uint32_t millisecondsToWait) {
		return semaphoreImpl->tryAcquireOne(millisecondsToWait);
	}
}
}
