#pragma once

#include "Jargon/ScopedPointer.h"
#include "Jargon/System/WindowsDefines.h"

#include <windows.h>

struct IUnknown;

namespace Jargon{
namespace System{

	class ComInitializer {
		public:
			ComInitializer(DWORD flags);

			~ComInitializer();

			operator bool() const;
		private:
			bool initialized = false;
	};

	class ComPtrReleasePolicy {
		public:
			static void Release(IUnknown* toRelease);
	};

	template<class ComClass>
	class ComPtr : public Jargon::ScopedPointer<ComClass, ComPtrReleasePolicy> {
	};


}
}

