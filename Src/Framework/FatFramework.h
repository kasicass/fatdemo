#pragma once

// Platform-specific
#include "Kernel/PlatformConfig.h"

#if defined(FAT_OS_WINDOWS)
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <Windows.h>
#  include <process.h>
#  define FAT_THREAD_LOCAL __declspec(thread)
#else
#  include <signal.h>
#  include <pthread.h>
#  define FAT_THREAD_LOCAL __thread
#  if defined(FAT_OS_ANDROID)
#    include <android/log.h>
#  endif
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
#include <atomic>

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

#if defined(FAT_DEBUG_BUILD)
#define fat_debugbreak()                __debugbreak()
#else
#define fat_debugbreak()
#endif

#else

#define fat_sprintf    sprintf
#define fat_snprintf   snprintf
#define fat_vsnprintf  vsnprintf
#define fat_vsnwprintf vswprintf
#define fat_stricmp    strcasecmp
#define fat_strnicmp   strncasecmp
#define fat_wcsicmp    wcscasecmp
#define fat_wcsnicmp   wcsncasecmp

#if defined(FAT_DEBUG_BUILD)
#define fat_debugbreak()                raise(SIGTRAP)
#else
#define fat_debugbreak()
#endif

#endif

// FatFramework stuff
#include "Kernel/Types.h"
#include "Kernel/Common/OS.h"
#include "Kernel/Common/Log.h"
#include "Kernel/Common/Assertion.h"
#include "Kernel/Common/NonCopyable.h"
#include "Kernel/Common/Memory.h"
#include "Kernel/Common/Time.h"
#include "Kernel/String/TCharTraits.h"
#include "Kernel/String/TString.h"
#include "Kernel/String/TStackString.h"
#include "Kernel/UnitTest/UnitTest.h"
#include "Kernel/Thread/AtomicInt.h"
#include "Kernel/Thread/TAutoLock.h"
#include "Kernel/Thread/MutexFast.h"
#include "Kernel/Thread/ConditionVariable.h"
#include "Kernel/Common/SmartPtr.h"
#include "Kernel/Thread/Thread.h"
#include "Driver/Client/FactorySelector.h"
#include "Driver/Server/Interface/IServerObject.h"
#include "Driver/Server/Interface/IServerFactory.h"
#include "Driver/Server/Interface/IServerDevice.h"
#include "Driver/Server/Interface/IServerCommand.h"
#include "Driver/Server/Interface/IServerState.h"
#include "Driver/Client/IObject.h"
#include "Driver/Client/CommandLocker.h"
#include "Driver/Client/CommandBuffer.h"
#include "Driver/Client/ICommand.h"
#include "Driver/Client/EStateType.h"
#include "Driver/Client/IState.h"
#include "Driver/Client/Device.h"
#include "Driver/Client/Command/DrawCommand.h"
#include "Driver/Client/Command/SwapCommand.h"
#include "Application/IApplication.h"
#include "Application/Application.h"
