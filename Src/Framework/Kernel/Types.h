#pragma once

typedef unsigned char UInt8;
typedef char Int8;
typedef unsigned short UInt16;
typedef short Int16;
typedef unsigned int UInt32;
typedef int Int32;
typedef unsigned long long UInt64;
typedef long long Int64;
typedef float F32;
typedef double F64;
typedef bool Bool;

// Tricky here~
// Windows - sizeof(unsigned long) = 4 (DWORD)
// OpenBSD - sizeof(unsigned long) = 4 or 8 (32/64bit, equals to pthread_t)
typedef unsigned long ThreadId;
#define THREADID_NULL (0)
