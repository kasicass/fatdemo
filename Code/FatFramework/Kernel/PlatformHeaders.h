#pragma once

#if FAT_OS_WINDOWS
// can't define it, or <dsound.h> compiles error in uber-build
// #  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>
#  include <process.h>
#else
#  include <signal.h>
#  include <pthread.h>
#endif

// thread local
#if FAT_OS_WINDOWS
#  define FAT_THREAD_LOCAL __declspec(thread)
#else
#  define FAT_THREAD_LOCAL __thread
#endif

// debug break
#if FAT_DEBUG_BUILD
#  if FAT_OS_WINDOWS
#    define fat_debugbreak()  __debugbreak()
#  else
#    define fat_debugbreak()  raise(SIGTRAP)
#  endif
#else
#  define fat_debugbreak()
#endif
