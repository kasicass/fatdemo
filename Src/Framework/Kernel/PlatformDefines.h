// https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html

#pragma once

// 32/64-bit
#if defined(_WIN64) || defined(__x86_64__)
#  define FAT_CPU_AMD64
#elif defined(_WIN32) || defined(__i386__)
#  define FAT_CPU_X86
#elif defined(__x86_64__)
#  error Unsupport CPU
#endif

// Operating System
#if defined(_WIN64) || defined(_WIN32)
#  define FAT_OS_WINDOWS
#elif defined(__OpenBSD__)
#  define FAT_OS_OPENBSD
#elif defined(__ANDROID__)
#  define FAT_OS_ANDROID
#elif defined(__linux__)
#  define FAT_OS_LINUX
#else
#  error Unsupport Operating System
#endif

// Compiler
#if defined(_WIN64) || defined(_WIN32)
#  define FAT_COMPILER_MSVC
#elif defined(__clang__)
#  define FAT_COMPILER_CLANG
#elif defined(__GNUC__)
#  define FAT_COMPILER_GCC
#else
#  error Unsupport Compiler
#endif

// Build Configuration
#if !defined(FAT_DEBUG_BUILD) && !defined(FAT_RELEASE_BUILD)
#  error Where is the build configuration?
#endif

// Preprocessor definition
#if defined(FAT_DEBUG_BUILD)
#  define FAT_ENABLE_ASSERT
#  define FAT_ENABLE_UNITTEST
#  define FAT_ENABLE_LOG
#else
#  define FAT_ENABLE_LOG
#endif

// Memory Leak Detection
#if defined(FAT_OS_WINDOWS) && defined(FAT_DEBUG_BUILD)
#  define FAT_ENABLE_MEMORY_LEAK_DETECTION
#endif
