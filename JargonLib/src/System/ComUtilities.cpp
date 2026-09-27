#include "Jargon/System/ComUtilities.h"

#include <objbase.h>


namespace Jargon{
namespace System{

	ComInitializer::ComInitializer(DWORD flags) {
		HRESULT hr = CoInitializeEx(nullptr, flags);
		initialized = SUCCEEDED(hr);
	}

	ComInitializer::~ComInitializer() {
		if (initialized) {
			CoUninitialize();
		}
	}

	ComInitializer::operator bool() const {
		return initialized;
	}

	void ComPtrReleasePolicy::Release(IUnknown* toRelease) {
		if (toRelease) {
			toRelease->Release();
		}
	}
}
}
