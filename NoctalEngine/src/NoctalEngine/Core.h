#pragma once

#ifdef NE_PLATFORM_WINDOWS

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
	#define NE_ASSERT(x, fmt, ...) { if(!(x)) { NE_ENGINE_ERROR("Assertion Failed: " fmt, ##__VA_ARGS__); DEBUG_BREAK(); }}	
	#define NE_ENGINE_ASSERT(x, fmt, ...) { if(!(x)) { NE_ENGINE_ERROR("Assertion Failed: " fmt, ##__VA_ARGS__); DEBUG_BREAK(); }}	
	#define NE_ENGINE_SDL_ASSERT(x, fmt, ...) { if(!(x)) { NE_ENGINE_ERROR("Assertion Failed: " fmt, SDL_GetError()); DEBUG_BREAK(); }}	
#else
	#define NE_ASSERT(x, ...)
	#define NE_ENGINE_ASSERT(x, ...)
#endif // NE_ENABLE_ASSERTS

#define BIT(x) (1 << x)