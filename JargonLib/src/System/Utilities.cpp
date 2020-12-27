
#include "Jargon/System/Utilities.h"

#ifdef _WIN32
	#include "Jargon/System/WindowsDefines.h"
	#include <windows.h>
#endif

#include <thread>

namespace Jargon{
namespace System{

	#ifdef _WIN32
		void sleep(unsigned int milliseconds){
			Sleep(milliseconds);
		}
		unsigned int getHardwareConcurrencyCount(){
			return std::thread::hardware_concurrency();
		}
	#endif
}
}
