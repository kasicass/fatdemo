#pragma once

#include "Kernel/PlatformConfig.h"

#if defined(FAT_ENABLE_LOG)

namespace Fat {

void LogPrintf(const wchar_t* format, ...);

}

#define FatLog(...) Fat::LogPrintf(__VA_ARGS__)

#else

#define FatLog(...)

#endif
