#ifndef JARGON_SYSTEM_WINDOWSMEMORYLEAKHELPERS_H
#define JARGON_SYSTEM_WINDOWSMEMORYLEAKHELPERS_H

#include "Jargon/Macros.h"


#ifdef JARGON_WINDOWS_DEBUG
#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


/*
	To use this:

		#include "Jargon/System/WindowsMemoryLeakHelpers.h"


		JARGON_WINDOWS_BREAK_AT_ALLOCATION_NUMBER(322); // replace param with whatever leak you want to break on, or -1 to disable

		int main(int argc, char ** argv){

			// whatever ....

			JARGON_WINDOWS_DUMP_MEMORY_LEAKS();

			return 0;
		}
*/



#if defined(JARGON_WINDOWS_DEBUG)
#define JARGON_WINDOWS_BREAK_AT_ALLOCATION_NUMBER(x) \
static struct _jargon_allocbreaker{\
	_jargon_allocbreaker(){\
		_crtBreakAlloc = x;\
	}\
} jargon_allocbreaker
#else
#define JARGON_WINDOWS_BREAK_AT_ALLOCATION_NUMBER(x)
#endif

#if defined(JARGON_WINDOWS_DEBUG)
#define JARGON_WINDOWS_DUMP_MEMORY_LEAKS()\
	_CrtDumpMemoryLeaks()
#else
#define JARGON_WINDOWS_DUMP_MEMORY_LEAKS()
#endif


#endif
