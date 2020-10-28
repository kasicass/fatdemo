// https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html

#pragma once

//
// 32/64-bit
//

#undef FAT_CPU_AMD64
#undef FAT_CPU_X86

#if defined(_WIN64) || defined(__x86_64__)
#  define FAT_CPU_AMD64 1
#  define FAT_CPU_X86   0
#elif defined(_WIN32) || defined(__i386__)
#  define FAT_CPU_AMD64 0
#  define FAT_CPU_X86   1
#elif defined(__x86_64__)
#  error Unsupport CPU
#endif

//
// Operating System
//

#undef FAT_OS_WINDOWS
#undef FAT_OS_OPENBSD
#undef FAT_OS_ANDROID
#undef FAT_OS_LINUX

#if defined(_WIN64) || defined(_WIN32)
#  define FAT_OS_WINDOWS 1
#  define FAT_OS_OPENBSD 0
#  define FAT_OS_ANDROID 0
#  define FAT_OS_LINUX   0
#elif defined(__OpenBSD__)
#  define FAT_OS_WINDOWS 0
#  define FAT_OS_OPENBSD 1
#  define FAT_OS_ANDROID 0
#  define FAT_OS_LINUX   0
#elif defined(__ANDROID__)
#  define FAT_OS_WINDOWS 0
#  define FAT_OS_OPENBSD 0
#  define FAT_OS_ANDROID 1
#  define FAT_OS_LINUX   0
#elif defined(__linux__)
#  define FAT_OS_WINDOWS 0
#  define FAT_OS_OPENBSD 0
#  define FAT_OS_ANDROID 0
#  define FAT_OS_LINUX   1
#else
#  error Unsupport Operating System
#endif

//
// Compiler
//

#undef FAT_COMPILER_MSVC
#undef FAT_COMPILER_CLANG
#undef FAT_COMPILER_GCC

#if defined(_WIN64) || defined(_WIN32)
#  define FAT_COMPILER_MSVC  1
#  define FAT_COMPILER_CLANG 0
#  define FAT_COMPILER_GCC   0
#elif defined(__clang__)
#  define FAT_COMPILER_MSVC  0
#  define FAT_COMPILER_CLANG 1
#  define FAT_COMPILER_GCC   0
#elif defined(__GNUC__)
#  define FAT_COMPILER_MSVC  0
#  define FAT_COMPILER_CLANG 0
#  define FAT_COMPILER_GCC   1
#else
#  error Unsupport Compiler
#endif

//
// Build Configuration
//

#if defined(FAT_DEBUG_BUILD)
#  define FAT_DEBUG_BUILD   1
#  define FAT_PROFILE_BUILD 0
#  define FAT_RELEASE_BUILD 0
#elif defined(FAT_PROFILE_BUILD)
#  define FAT_DEBUG_BUILD   0
#  define FAT_PROFILE_BUILD 1
#  define FAT_RELEASE_BUILD 0
#elif defined(FAT_RELEASE_BUILD)
#  define FAT_DEBUG_BUILD   0
#  define FAT_PROFILE_BUILD 0
#  define FAT_RELEASE_BUILD 1
#else
#  error Where is the build configuration?
#endif

//
// Preprocessor definition
//

#if FAT_RELEASE_BUILD
#  define FAT_ENABLE_LOG                   1
#  define FAT_ENABLE_ASSERT                0
#  define FAT_ENABLE_UNITTEST              0
#  define FAT_ENABLE_MEMORY_LEAK_DETECTION 0       
#else
#  define FAT_ENABLE_LOG                   1
#  define FAT_ENABLE_ASSERT                1
#  define FAT_ENABLE_UNITTEST              1
#  define FAT_ENABLE_MEMORY_LEAK_DETECTION 1
#endif
