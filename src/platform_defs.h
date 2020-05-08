#pragma once

//
// platform_defs.h - Useful platform-dependent macros and utilities.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

// set up "CURRENT_PLATFORM_XYZ" defines
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#undef CURRENT_PLATFORM_MICROSOFT_WINDOWS
#define CURRENT_PLATFORM_POSIX 1
#elif defined(_WIN32)
#define CURRENT_PLATFORM_MICROSOFT_WINDOWS 1
#undef CURRENT_PLATFORM_POSIX
#else
#undef CURRENT_PLATFORM_MICROSOFT_WINDOWS
#undef CURRENT_PLATFORM_POSIX
#endif

// set up macros for disabling compiler warnings
#if defined(_WIN32)
#define PUSH_WARN_DISABLE __pragma(warning(push))
#define POP_WARN_DISABLE __pragma(warning(pop))
#define WARN_DISABLE(WARN_CODE_MS, WARN_CODE_GCC) __pragma(warning(disable \
                                                                   : WARN_CODE_MS))
#define WARN_DISABLE_MS(WARN_CODE_MS) __pragma(warning(disable \
                                                       : WARN_CODE_MS))
#define WARN_DISABLE_GCC(WARN_CODE_GCC)
#elif defined(__GNUG__)
#define DO_PRAGMA_(x) _Pragma(#x)
#define DO_PRAGMA(x) DO_PRAGMA_(x)
#define PUSH_WARN_DISABLE DO_PRAGMA_(GCC diagnostic push)
#define POP_WARN_DISABLE DO_PRAGMA_(GCC diagnostic pop)
#define WARN_DISABLE(WARN_CODE_MS, WARN_CODE_GCC) DO_PRAGMA_(GCC diagnostic ignored WARN_CODE_GCC)
#define WARN_DISABLE_MS(WARN_CODE_MS)
#define WARN_DISABLE_GCC(WARN_CODE_GCC) DO_PRAGMA_(GCC diagnostic ignored WARN_CODE_GCC)
#else
#define PUSH_WARN_DISABLE
#define POP_WARN_DISABLE
#define WARN_DISABLE(WARN_CODE_MS, WARN_CODE_GCC)
#define WARN_DISABLE_MS(WARN_CODE_MS)
#define WARN_DISABLE_GCC(WARN_CODE_GCC)
#endif

// include posix specific defs
#if defined(CURRENT_PLATFORM_POSIX)
#include "platform_defs_posix.h"
#endif

// include windows specific defs
#if defined(CURRENT_PLATFORM_MICROSOFT_WINDOWS)
#include "platform_defs_windows.h"
#endif
