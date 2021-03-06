#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#if FAT_ENABLE_LOG

namespace Fat {

void LogPrintf(const wchar_t* format, ...);

}

#define FatLog(...) Fat::LogPrintf(__VA_ARGS__)

#else

#define FatLog(...)

#endif
