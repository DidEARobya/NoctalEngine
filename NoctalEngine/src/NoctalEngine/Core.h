#pragma once

#ifdef NE_PLATFORM_WINDOWS
	#ifdef NE_BUILD_DLL
		#define NOCTAL_ENGINE_API __declspec(dllexport)
	#else
		#define NOCTAL_ENGINE_API __declspec(dllimport)
	#endif // NE_BUILD_DLL
#else
	#error NoctalEngine only supports Windows
#endif // DEBUG

#ifdef NE_ENABLE_ASSERTS
	#define NE_ASSERT(x, ...) { if(!(x)) { NE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugBreak(); }}
	#define NE_ENGINE_ASSERT(x, ...) { if(!(x)) { NE_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugBreak(); }}
#else
	#define NE_ASSERT(x, ...)
	#define NE_ENGINE_ASSERT(x, ...)
#endif // NE_ENABLE_ASSERTS

#define BIT(x) (1 << x)