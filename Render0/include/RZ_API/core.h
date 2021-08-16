#pragma once

#ifdef RZ_PLATFORM_WINDOWS
	#ifdef RZ_BUILD_DLL
		#define	RZ_API __declspec(dllexport)
	#else
		#define	RZ_API __declspec(dllimport)
	#endif
#else
	#error falta suporte pra linux ainda : /
#endif

#ifdef DEBUG
#define CONFIG_NAME "Debug"
#else
#define CONFIG_NAME "Release"
#endif

#ifdef Win64
#define PLATFORM_NAME "Win-64"
#else
#define PLATFORM_NAME "Win-32"
#endif


