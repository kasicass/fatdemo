#pragma once

// helper macros
#define FAT_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define FAT_SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define _FAT_CONCAT(x, y) x##y
#define FAT_CONCAT(x, y) _FAT_CONCAT(x, y)

// helper for enum
#define FAT_ENUM_BEGIN(x_enumType) \
	struct x_enumType { \
		enum EValue {

#define FAT_ENUM_END() \
			eValuesCount, \
		}; \
	};

#define FAT_ENUM_FOREACH(x_itValue, x_enumType) \
	for (x_enumType::EValue x_itValue = x_enumType::EValue(0); \
		 x_itValue < x_enumType::eValuesCount; \
		 x_itValue = x_enumType::EValue(x_itValue+ 1))

// c-string functions
#if defined(FAT_OS_WINDOWS)

#if defined(FAT_DEBUG_BUILD)
#define fat_debugbreak()                __debugbreak()
#else
#define fat_debugbreak()
#endif

#else

#if defined(FAT_DEBUG_BUILD)
#define fat_debugbreak()                raise(SIGTRAP)
#else
#define fat_debugbreak()
#endif

#endif
