#include "FatFramework.h"

namespace Fat { namespace OS {

void Println(const wchar_t* msg)
{
#if defined(FAT_OS_ANDROID)

	// Android NDK, as typical for Linux, uses 32 bits to represent wide chars
	// https://stackoverflow.com/questions/27464835/android-ndk-log-unicode-wide-characters
#if 0
	const UInt32 bufSize = 512;
	UInt32 buf[bufSize];
	size_t n = wcslen(msg);
	if (n >= bufSize)
		n = bufSize-1;
	for (size_t i = 0; i < n; ++i)
		buf[i] = msg[i];
	buf[n] = '\0';
#endif

	// __android_log_print() add '\n' for us
	// __android_log_print(ANDROID_LOG_INFO, "FAT", "%ls", buf);
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
