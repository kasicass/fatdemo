#pragma once

// Platform-specific
#include "Kernel/PlatformDefines.h"

// https://www.flipcode.com/archives/Detecting_Memory_Leaks.shtml
#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
#  define _CRTDBG_MAP_ALLOC
#  include <stdlib.h>
#  include <crtdbg.h>
#endif

#if defined(FAT_OS_WINDOWS)
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>

#  define FAT_THREAD_LOCAL __declspec(thread)
#else
#  define FAT_THREAD_LOCAL
#endif

// c headers
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <assert.h>

// c++ headers
#include <vector>
#include <list>
#include <map>

// helper macros
#define FAT_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define _FAT_CONCAT(x, y) x##y
#define FAT_CONCAT(x, y) _FAT_CONCAT(x, y)

#define FAT_ENUM_BEGIN(x_enumType) \
	struct x_enumType { \
		enum EValue {

#define FAT_ENUM_END() \
			eValuesCount, \
		}; \
	};

// c-string functions
#if defined(FAT_OS_WINDOWS)

#define fat_sprintf    sprintf_s
#define fat_snprintf   _snprintf_s
#define fat_vsnprintf  _vsnprintf
#define fat_vsnwprintf _vsnwprintf
#define fat_stricmp    _stricmp
#define fat_strnicmp   _strnicmp
#define fat_wcsicmp    _wcsicmp
#define fat_wcsnicmp   _wcsnicmp

#define fat_aligned_malloc(size, align) _aligned_malloc(size, align)
#define fat_aligned_free(p)             _aligned_free(p)

#else

#define fat_sprintf    sprintf
#define fat_snprintf   snprintf
#define fat_vsnprintf  vsnprintf
#define fat_vsnwprintf vswprintf
#define fat_stricmp    strcasecmp
#define fat_strnicmp   strncasecmp
#define fat_wcsicmp    wcscasecmp
#define fat_wcsnicmp   wcsncasecmp

#define fat_aligned_malloc(size, align) aligned_alloc(align, size)
#define fat_aligned_free(p)             free(p)

#endif

// FatFramework stuff
#include "Kernel/Types.h"
#include "Kernel/Common/Log.h"
#include "Kernel/Common/Assertion.h"
#include "Kernel/Common/Memory.h"
#include "Kernel/Common/NonCopyable.h"
#include "Kernel/Common/Time.h"
#include "Kernel/String/TCharTraits.h"
#include "Kernel/String/TString.h"
#include "Kernel/String/TStackString.h"
#include "Kernel/UnitTest/UnitTest.h"
#include "Application/IApplication.h"
#include "Application/Application.h"
