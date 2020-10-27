#pragma once

typedef unsigned char      UInt8;
typedef char               SInt8;
typedef unsigned short     UInt16;
typedef short              SInt16;
typedef unsigned int       UInt32;
typedef int                SInt32;
typedef unsigned long long UInt64;
typedef long long          SInt64;
typedef float              F32;
typedef double             F64;

// Tricky here~
// Windows - sizeof(unsigned long) = 4 (DWORD)
// OpenBSD - sizeof(unsigned long) = 4 or 8 (32/64bit, equals to pthread_t)
typedef unsigned long ThreadId;
#define THREADID_NULL (0)
