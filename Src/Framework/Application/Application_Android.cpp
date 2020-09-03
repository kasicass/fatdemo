#include "FatFramework.h"

#if defined(FAT_OS_ANDROID)

#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "FAT", __VA_ARGS__))

using namespace Fat;

#endif
