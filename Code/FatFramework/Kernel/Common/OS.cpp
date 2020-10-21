#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/String/TCharTraits.h"

#if defined(FAT_OS_ANDROID)
#  include <android/log.h>
#endif

namespace Fat { namespace OS {

void Println(const wchar_t* msg)
{
#if defined(FAT_OS_ANDROID)

	// __android_log_print() add '\n' for us
	__android_log_print(ANDROID_LOG_INFO, "FAT", "%ls", msg);

#else

	const UInt32 bufSize = 512;
	wchar_t buf[bufSize];
	size_t n = wcslen(msg);
	if (n >= bufSize-1)
		n = bufSize-2;
	CharTraits<wchar_t>::copy(buf, msg, n);
	buf[n] = L'\n';
	buf[n+1] = L'\0';
	wprintf(buf);

#endif
}

}}
