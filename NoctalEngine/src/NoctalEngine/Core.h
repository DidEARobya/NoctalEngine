#pragma once
#ifdef NE_PLATFORM_WINDOWS
	#if NE_DYNAMIC_LINK
		#ifdef NE_BUILD_DLL
			#define NOCTAL_ENGINE_API __declspec(dllexport)
		#else
			#define NOCTAL_ENGINE_API __declspec(dllimport)
		#endif // NE_BUILD_DLL
	#else
		#define NOCTAL_ENGINE_API
	#endif
#else
	#error NoctalEngine only supports Windows
#endif // DEBUG

#if defined(_MSC_VER)
	#include <intrin.h>
	#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
	#define DEBUG_BREAK() __builtin_trap()
#else
	#define DEBUG_BREAK() ((void)0)
#endif

#ifdef NE_ENABLE_ASSERTS
	#include "SDL3/SDL_error.h"
//Fix (__VA_ARGS__ not being passed properly)
	#define NE_ASSERT(x, ...) { if(!(x)) { NE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); }}
	#define NE_ENGINE_ASSERT(x, ...) { if(!(x)) { NE_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); }}
	#define NE_ENGINE_SDL_ASSERT(x) { if(!(x)) { NE_ENGINE_ERROR("Assertion Failed: SDL Error: {0}", SDL_GetError()); DEBUG_BREAK(); }}
#else
	#define NE_ASSERT(x, ...)
	#define NE_ENGINE_ASSERT(x, ...)
#endif // NE_ENABLE_ASSERTS

#define BIT(x) (1 << x)