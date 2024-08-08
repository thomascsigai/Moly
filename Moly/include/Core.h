#pragma once

#ifdef ML_PLATFORM_WINDOWS
	#ifdef ML_BUILD_DLL
		#define MOLY_API __declspec(dllexport)
	#else
		#define MOLY_API __declspec(dllimport)
	#endif
#else
	#error Moly only support Windows
#endif
