#pragma once

//
// platform_defs.h - Useful platform-dependent macros and utilities.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//

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

#if defined(CURRENT_PLATFORM_POSIX)
#include "platform_defs_posix.h"
#endif

#if defined(CURRENT_PLATFORM_MICROSOFT_WINDOWS)
#include "platform_defs_windows.h"
#endif
