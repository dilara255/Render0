#pragma once

//definições para controle dos projetos, sistemas, compilação, linkagem, etc

#ifdef RZ_PLATFORM_WINDOWS
	#define SYSTEM_NAME "Windows"
	#ifdef RENDER0
		#define	RZ_API __declspec(dllexport)
	#else
		#define	RZ_API __declspec(dllimport)
	#endif
#else
	#error falta suporte pra linux ainda : /
#endif

#ifdef DEBUG
	#define CONFIG_NAME "Debug"
#elif RELEASE 
	#define CONFIG_NAME "Release"
#elif DISTRIB 
	#define CONFIG_NAME "Distribution"
#endif

#ifdef X64
	#define PLATFORM_NAME "64 bits"
#elif X86
	#define PLATFORM_NAME "32 bits"
#endif

#define BIT(x) (1 << x)


