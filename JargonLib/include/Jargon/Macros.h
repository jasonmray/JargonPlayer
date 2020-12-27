
#ifndef JARGON_MACROS_H
#define JARGON_MACROS_H

#define JARGON_DISABLE_COPY(clazz) \
	clazz(const clazz&) = delete;\
	clazz(clazz &&) = delete;\
	clazz & operator=(const clazz &) = delete;

#if defined(_DEBUG) || defined(DEBUG)
	#define JARGON_DEBUG
#endif

#if defined(_WIN32)
	#define JARGON_PLATFORM_WINDOWS

	#if defined(JARGON_DEBUG)
		#define JARGON_WINDOWS_DEBUG
	#endif

	#if defined(_WIN64)
		#define JARGON_PLATFORM_WIN64
	#else
		#define JARGON_PLATFORM_WIN32
	#endif
#endif




#endif
