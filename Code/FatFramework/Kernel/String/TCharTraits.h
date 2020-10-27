#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#include <ctype.h>   // isspace, toupper
#include <string.h>  // sprintf
#include <wchar.h>   // wcsrchr, wmemset
#include <stdio.h>   // _vsnprintf

// c-string functions
#if FAT_OS_WINDOWS

#define fat_sprintf    sprintf_s
#define fat_snprintf   _snprintf_s
#define fat_vsnprintf  _vsnprintf
#define fat_vsnwprintf _vsnwprintf
#define fat_stricmp    _stricmp
#define fat_strnicmp   _strnicmp
#define fat_wcsicmp    _wcsicmp
#define fat_wcsnicmp   _wcsnicmp

#else

#define fat_sprintf    sprintf
#define fat_snprintf   snprintf
#define fat_vsnprintf  vsnprintf
#define fat_vsnwprintf vswprintf
#define fat_stricmp    strcasecmp
#define fat_strnicmp   strncasecmp
#define fat_wcsicmp    wcscasecmp
#define fat_wcsnicmp   wcsncasecmp

#endif

namespace Fat {

//
// CharTraits, used by TString & TStackString
//

template <typename T> struct CharTraits;

// traits for char
template <>
struct CharTraits<char>
{
	typedef size_t size_type;
	typedef char value_type;
	typedef const value_type* const_str;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	static inline int isspace(value_type ch)
	{
		return ::isspace((int)ch);
	}

	static inline value_type toupper(value_type ch)
	{
		return (value_type)::toupper((int)ch);
	}

	static inline value_type tolower(value_type ch)
	{
		return (value_type)::tolower((int)ch);
	}

	static inline value_type tolower_ascii(value_type ch)
	{
		return ( (((ch) >= 'A') && ((ch) <= 'Z')) ? ((ch) - 'A' + 'a') : (ch) );
	}

	static inline int strcmp(const_str a, const_str b)
	{
		return ::strcmp(a, b);
	}

	static inline int strncmp(const_str a, const_str b, size_type n)
	{
		return ::strncmp(a, b, n);
	}

	static inline int stricmp(const_str a, const_str b)
	{
		return ::fat_stricmp(a, b);
	}

	static inline int strnicmp(const_str a, const_str b, size_type n)
	{
		return ::fat_strnicmp(a, b, n);
	}

	static inline size_type strspn(const_str str, const_str charSet)
	{
		return (str == NULL) ? 0 : (size_type)::strspn(str, charSet);
	}

	static inline size_type strcspn(const_str str, const_str charSet)
	{
		return (str == NULL) ? 0 : (size_type)::strcspn(str, charSet);
	}

	static inline size_type strlen(const_str str)
	{
		return (str == NULL) ? 0 : (size_type)::strlen(str);
	}

	static inline const_str strchr(const_str str, value_type ch)
	{
		return (str == NULL) ? 0 : ::strchr(str, ch);
	}

	static inline const_str strrchr(const_str str, value_type ch)
	{
		return (str == NULL) ? 0 : ::strrchr(str, ch);
	}

	static inline const_str strstr(const_str str, const_str strSearch)
	{
		return (str == NULL) ? 0 : ::strstr(str, strSearch);
	}

	static inline const_str strpbrk(const_str str, const_str charSet)
	{
		return (str == NULL) ? 0 : ::strpbrk(str, charSet);
	}

	static inline void copy(value_type* dest, const value_type* src, size_type count)
	{
		::memcpy(dest, src, count*sizeof(value_type));
	}

	static inline void move(value_type* dest, const value_type* src, size_type count)
	{
		::memmove(dest, src, count*sizeof(value_type));
	}

	static inline void set(value_type* dest, value_type ch, size_type count)
	{
		::memset(dest, ch, count);
	}

	static inline int vsnprintf(value_type* str, size_type size, const_str format, va_list ap)
	{
		return ::fat_vsnprintf(str, size, format, ap);
	}
};

// traits for wchar_t
template <>
struct CharTraits<wchar_t>
{
	typedef size_t size_type;
	typedef wchar_t value_type;
	typedef const value_type* const_str;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	static inline int isspace(value_type ch)
	{
		return ::iswspace(ch);
	}

	static inline value_type toupper(value_type ch)
	{
		return ::towupper(ch);
	}

	static inline value_type tolower(value_type ch)
	{
		return ::towlower(ch);
	}

	static inline value_type tolower_ascii(value_type ch)
	{
		return ::towlower(ch);
	}

	static inline int strcmp(const_str a, const_str b)
	{
		return ::wcscmp(a, b);
	}

	static inline int strncmp(const_str a, const_str b, size_type n)
	{
		return ::wcsncmp(a, b, n);
	}

	static inline int stricmp(const_str a, const_str b)
	{
		return ::fat_wcsicmp(a, b);
	}

	static inline int strnicmp(const_str a, const_str b, size_type n)
	{
		return ::fat_wcsnicmp(a, b, n);
	}

	static inline size_type strspn(const_str str, const_str strCharSet)
	{
		return (str == NULL) ? 0 : (size_type)::wcsspn(str, strCharSet);
	}

	static inline size_type strcspn(const_str str, const_str strCharSet)
	{
		return (str == NULL) ? 0 : (size_type)::wcscspn(str, strCharSet);
	}

	static inline size_type strlen(const_str str)
	{
		return (str == NULL) ? 0 : (size_type)::wcslen(str);
	}

	static inline const_str strchr(const_str str, value_type ch)
	{
		return (str == NULL) ? 0 : ::wcschr(str, ch);
	}

	static inline const_str strrchr(const_str str, value_type ch)
	{
		return (str == NULL) ? 0 : ::wcsrchr(str, ch);
	}

	static inline const_str strstr(const_str str, const_str strSearch)
	{
		return (str == NULL) ? 0 : ::wcsstr(str, strSearch);
	}

	static inline const_str strpbrk(const_str str, const_str charSet)
	{
		return (str == NULL) ? 0 : ::wcspbrk(str, charSet);
	}

	static inline void copy(value_type* dest, const value_type* src, size_type count)
	{
		::memcpy(dest, src, count*sizeof(value_type));
	}

	static inline void move(value_type* dest, const value_type* src, size_type count)
	{
		::memmove(dest, src, count*sizeof(value_type));
	}

	static inline void set(value_type* dest, value_type ch, size_type count)
	{
		::wmemset(dest, ch, count);
	}

	static inline int vsnprintf(value_type* str, size_type size, const_str format, va_list ap)
	{
		return ::fat_vsnwprintf(str, size, format, ap);
	}
};

}
