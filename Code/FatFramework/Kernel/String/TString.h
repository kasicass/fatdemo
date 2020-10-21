// Custom reference counted string class.
// Can easily be substituted instead of std::string.

#pragma once

#include "FatFramework/Kernel/Common/Assertion.h"
#include "FatFramework/Kernel/String/TCharTraits.h"
#include <stdarg.h>

namespace Fat {

//
// TString
//

template <typename T>
class TString
{
public:
	// Types compatible with STL string.
	typedef TString<T> MyType;
	typedef size_t size_type;
	typedef T value_type;
	typedef const value_type* const_str;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef pointer iterator;
	typedef const_pointer const_iterator;

	enum npos_type : size_type {
		npos = (size_type)~0,
	};

public:
	TString();
	TString(const MyType& rhs);
	TString(const MyType& rhs, size_type offset, size_type count);
	explicit TString(value_type ch, size_type repeat = 1);
	TString(const_str s);
	TString(const_str s, size_type length);
	TString(const_iterator first, const_iterator last);
	~TString();

	//
	// STL string like interface.
	//

	size_type length() const;
	size_type size() const;
	bool empty() const;
	void clear();

	inline size_type capacity() const;
	void reserve(size_type count = 0);
	void shrink_to_fit();

	MyType& append(const_str s);
	MyType& append(const_str s, size_type count);
	MyType& append(const MyType& s, size_type offset, size_type count);
	MyType& append(const MyType& s);
	MyType& append(size_type count, value_type ch);
	MyType& append(const_iterator first, const_iterator last);

	MyType& assign(const_str s);
	MyType& assign(const_str s, size_type count);
	MyType& assign(const MyType& s, size_type offset, size_type count);
	MyType& assign(const MyType& s);
	MyType& assign(size_type count, value_type ch);
	MyType& assign(const_iterator first, const_iterator last);

	value_type at(size_type index) const;

	inline const_iterator begin() const { return str_; }
	inline const_iterator end() const { return str_ + length(); }

	inline iterator begin() { return str_; }
	inline iterator end() { return str_ + length(); }

	// operator const_str() const [ return str_; }

	inline const value_type* c_str() const { return str_; }
	inline const value_type* data() const { return str_; }

	int compare(const MyType& rhs) const;
	int compare(const_str s) const;

	int compareNoCase(const MyType& rhs) const;
	int compareNoCase(const_str s) const;

	// copy at most a specified number of characters from an indexed position to "dest"
	size_type copy(value_type* dest, size_type count, size_type offset = 0) const;

	void pop_back();
	void push_back(value_type ch);
	void resize(size_type count, value_type ch = ' ');

	MyType substr(size_type pos, size_type count = npos) const;

	// replace part of string.
	MyType& replace(value_type chOld, value_type chNew);
	MyType& replace(const_str strOld, const_str strNew);
	MyType& replace(size_type pos, size_type count, const_str strNew);
	MyType& replace(size_type pos, size_type count, const_str strNew, size_type count2);
	MyType& replace(size_type pos, size_type count, size_type numChars, value_type chNew);

	// insert new elements to strings.
	MyType& insert(size_type index, value_type ch);
	MyType& insert(size_type index, size_type count, value_type ch);
	MyType& insert(size_type index, const_str s);
	MyType& insert(size_type index, const_str s, size_type count);

	// delete count characters starting at zoer-based index
	MyType& erase(size_type index, size_type count = npos);

	// searching (return starting index, or "npos" if not found)
	size_type find(value_type ch, size_type pos = 0) const; // like strchr()
	size_type find(const_str subs, size_type pos = 0) const; // like strstr()
	size_type rfind(value_type ch, size_type pos = npos) const;

	size_type find_first_of(const MyType& s, size_type offset = 0) const;
	size_type find_first_of(const_str charSet, size_type offset = 0) const;
	size_type find_first_of(value_type ch, size_type offset = 0) const;

	void swap(MyType& rhs);

	// overloaded operators.
	value_type operator[](size_type i) const;

	MyType& operator=(const MyType& rhs);
	MyType& operator=(const_str s);

	MyType& operator+=(const MyType& rhs);
	MyType& operator+=(const_str s);
	MyType& operator+=(value_type ch);

	//
	// ATL CSTring interfaces. (not in STL)
	//

	MyType& Format(const_str format, ...);
	MyType& FormatV(const_str format, va_list argList);

	MyType& MakeLower();
	MyType& MakeUpper();

	MyType& Trim();
	MyType& Trim(value_type ch);
	MyType& Trim(const value_type* charSet);

	MyType& TrimLeft();
	MyType& TrimLeft(value_type ch);
	MyType& TrimLeft(const value_type* charSet);

	MyType& TrimRight();
	MyType& TrimRight(value_type ch);
	MyType& TrimRight(const value_type* charSet);

	MyType SpanIncluding(const_str charSet) const;
	MyType SpanExcluding(const_str charSet) const;
	MyType Tokenize(const_str charSet, int& start) const;
	MyType Mid(size_type first, size_type count = npos) const { return substr(first, count); }

	MyType Left(size_type count) const;
	MyType Right(size_type count) const;

	size_t GetAllocatedMemory() const;

private:
	struct SStrHeader
	{
		int refcount;           // Reference count of this string.
		int length;             // Current length of string.
		//T data[length+1]      // Here in memory starts character buffer of size length+1.

		inline T* GetStr() { return (T*)(this+1); }
		inline void AddRef() { ++refcount; }
		inline int Release() { return --refcount; }
		inline int GetLength() { return length; }
		inline int GetRefcount() { return refcount; }
	};

	static size_t _CalcAllocSize(size_t length)
	{
		return sizeof(SStrHeader) + (length+1)*sizeof(value_type);
	}

	static SStrHeader* _EmptyHeader()
	{
		// the 2nd is a dummy object to hold a single empty string.
		static SStrHeader s_emptyString[2] = { {-1, 0}, {0, 0} };
		return &s_emptyString[0];
	}

	static void _FreeData(SStrHeader* pData)
	{
		if (pData->refcount >= 0) // not empty string.
		{
			FatAssertNoText(pData->refcount != 0);
			if (pData->Release() <= 0)
			{
				::operator delete((void*)pData);
			}
		}
	}

private:
	// helpers
	SStrHeader* _Header() const;

	void _Initialize();
	void _AllocData(size_type length);
	void _Free();

	void _Assign(const_str s, size_type length);
	void _Concat(const_str s, size_type length);
	void _Concat(const_str s1, size_type len1, const_str s2, size_type len2);
	void _MakeUnique(); // copy-on-write

private:
	value_type* str_; // pointer to the ref-counted string data
};

//
// Helper Functions
//

template <typename T>
inline typename TString<T>::SStrHeader* TString<T>::_Header() const
{
	FatAssertNoText(str_ != NULL);
	return ((SStrHeader*)str_) - 1;
}

template <typename T>
inline void TString<T>::_Initialize()
{
	str_ = _EmptyHeader()->GetStr();
}

template <typename T>
inline void TString<T>::_AllocData(size_type length)
{
	// always allocate one extra character for '\0'
	// length() always equal capacity()

	FatAssertNoText(length <= 500*1024*1024); // 500M

	if (length == 0)
	{
		_Initialize();
		return;
	}

	size_type allocLen = _CalcAllocSize(length);
	SStrHeader* pData = (SStrHeader*)FatMalloc(allocLen);
	pData->refcount  = 1;
	pData->length    = (int)length;

	str_ = pData->GetStr();
	str_[length] = '\0';
}

template <typename T>
inline void TString<T>::_Free()
{
	if (_Header()->GetRefcount() >= 0) // not empty string.
	{
		_FreeData(_Header());
		_Initialize();
	}
}

template <typename T>
inline void TString<T>::_Assign(const_str s, size_type slen)
{
	// refcount > 1 means the string is shared
	if (_Header()->refcount > 1 || slen > capacity())
	{
		_Free();
		_AllocData(slen);
	}

	CharTraits<T>::copy(str_, s, slen);
	_Header()->length = (int)slen;
	str_[slen] = '\0';
}

template <typename T>
inline void TString<T>::_Concat(const_str s1, size_type len1, const_str s2, size_type len2)
{
	size_type newLength = len1 + len2;
	size_type newCapacity = newLength;
	if (len1*2 > newCapacity)
		newCapacity = len1*2;

	if (newCapacity != 0)
	{
		if (newCapacity < 8)
			newCapacity = 8;

		_AllocData(newCapacity);
		CharTraits<T>::copy(str_, s1, len1);
		CharTraits<T>::copy(str_+len1, s2, len2);
		_Header()->length = (int)newLength;
		str_[newLength] = '\0';
	}
}

template <typename T>
inline void TString<T>::_Concat(const_str s, size_type slen)
{
	if (slen != 0)
	{
		// shared string or capacity is not fit for new string
		if (_Header()->refcount > 1 || length()+slen > capacity())
		{
			SStrHeader* pOld = _Header();
			_Concat(str_, length(), s, slen);
			_FreeData(pOld);
		}
		else
		{
			CharTraits<T>::copy(str_+length(), s, slen);
			size_type newLength = length() + slen;
			_Header()->length = (int)newLength;
			str_[newLength] = '\0';
		}
	}
}

template <typename T>
inline void TString<T>::_MakeUnique()
{
	if (_Header()->refcount > 1)
	{
		SStrHeader* pOldData = _Header();
		_Free();  // this will not free header as refcount > 1
		_AllocData(pOldData->GetLength());
		CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength()+1);
	}
}

//
// Constructor
//

template <typename T>
TString<T>::TString()
{
	_Initialize();
}

template <typename T>
TString<T>::TString(const MyType& rhs)
{
	FatAssertNoText(rhs._Header()->GetRefcount() != 0);
	if (rhs._Header()->GetRefcount() >= 0)
	{
		str_ = rhs.str_;
		_Header()->AddRef();
	}
	else
	{
		_Initialize();
	}
}

template <typename T>
TString<T>::TString(const MyType& rhs, size_type offset, size_type count)
{
	_Initialize();
	assign(rhs, offset, count);
}

template <typename T>
TString<T>::TString(value_type ch, size_type repeat)
{
	_Initialize();
	if (repeat > 0)
	{
		_AllocData(repeat);
		CharTraits<T>::set(str_, ch, repeat);
	}
}

template <typename T>
TString<T>::TString(const_str s)
{
	_Initialize();
	size_type slen = CharTraits<T>::strlen(s);
	if (slen > 0)
	{
		_AllocData(slen);
		CharTraits<T>::copy(str_, s, slen);
	}
}

template <typename T>
TString<T>::TString(const_str s, size_type slen)
{
	_Initialize();
	if (slen > 0)
	{
		_AllocData(slen);
		CharTraits<T>::copy(str_, s, slen);
	}
}

template <typename T>
TString<T>::TString(const_iterator first, const_iterator last)
{
	_Initialize();
	size_type length = (size_type)(last - first);
	if (length > 0)
	{
		_AllocData(length);
		CharTraits<T>::copy(str_, first, length);
	}
}

template <typename T>
TString<T>::~TString()
{
	_FreeData(_Header());
}

template <typename T>
inline typename TString<T>::size_type TString<T>::length() const
{
	return (size_type) _Header()->GetLength();
}

template <typename T>
inline typename TString<T>::size_type TString<T>::size() const
{
	return (size_type) _Header()->GetLength();
}

template <typename T>
inline bool TString<T>::empty() const
{
	return length() == 0;
}

template <typename T>
inline void TString<T>::clear()
{
	if (length() == 0)
		return;

	_Free();

	FatAssertNoText(length() == 0);
	FatAssertNoText(_Header()->GetRefcount() < 0 || capacity() == 0);
}

template <typename T>
inline size_t TString<T>::capacity() const
{
	return (size_type) _Header()->length;
}

template <typename T>
inline void TString<T>::reserve(size_type count)
{
	if (count > capacity())
	{
		SStrHeader* pOldData = _Header();
		_AllocData(count);
		CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength());
		_Header()->length = pOldData->length;
		str_[length()] = '\0';
		_FreeData(pOldData);
	}
}

template <typename T>
inline void TString<T>::shrink_to_fit()
{
	if (length() != capacity())
	{
		SStrHeader* pOldData = _Header();
		_AllocData(length());
		CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength()+1);
		_FreeData(pOldData);
	}
}

template <typename T>
inline TString<T>& TString<T>::append(const_str s)
{
	*this += s;
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::append(const_str s, size_type count)
{
	_Concat(s, count);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::append(const MyType& rhs, size_type offset, size_type count)
{
	size_type len = rhs.length();
	if (offset > len)
		return *this;
	if (offset + count > len)
		count = len - offset;
	_Concat(rhs.str_ + offset, count);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::append(const MyType& rhs)
{
	*this += rhs;
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::append(size_type count, value_type ch)
{
	if (count == 0)
		return *this;

	_MakeUnique();

	if (length() + count > capacity())
	{
		// _MakeUnique() succeed
		SStrHeader* pOldData = _Header();
		_AllocData(length() + count);
		CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength());
		CharTraits<T>::set(str_ + pOldData->GetLength(), ch, count);
		_FreeData(pOldData);
	}
	else
	{
		// _MakeUnique() do nothing
		size_type oldLength = length();
		CharTraits<T>::set(str_ + oldLength, ch, count);
		_Header()->length = (int)(oldLength + count);
		str_[length()] = '\0';
	}

	return *this;
}

template <typename T>
inline TString<T>& TString<T>::append(const_iterator first, const_iterator last)
{
	append(first, (last - first));
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::assign(const_str s)
{
	*this = s;
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::assign(const_str s, size_type count)
{
	size_type len = CharTraits<T>::strlen(s);
	_Assign(s, (count < len) ? count : len);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::assign(const MyType& rhs, size_type offset, size_type count)
{
	size_type len = rhs.length();
	if (offset > len)
		return *this;
	if (offset + count > len)
		count = len - offset;
	_Assign(rhs.str_ + offset, count);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::assign(const MyType& rhs)
{
	*this = rhs;
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::assign(size_type count, value_type ch)
{
	if (count == 0)
		return *this;

	_AllocData(count);
	CharTraits<T>::set(str_, ch, count);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::assign(const_iterator first, const_iterator last)
{
	assign(first, (size_type)(last - first));
	return *this;
}

template <typename T>
inline typename TString<T>::value_type TString<T>::at(size_type i) const
{
	FatAssertNoText(i >= 0 && i < length());
	return str_[i];
}

template <typename T>
inline int TString<T>::compare(const MyType& rhs) const
{
	return CharTraits<T>::strcmp(str_, rhs.str_);
}

template <typename T>
inline int TString<T>::compare(const_str s) const
{
	return CharTraits<T>::strcmp(str_, s);
}

template <typename T>
inline int TString<T>::compareNoCase(const MyType& rhs) const
{
	return CharTraits<T>::stricmp(str_, rhs.str_);
}

template <typename T>
inline int TString<T>::compareNoCase(const_str s) const
{
	return CharTraits<T>::stricmp(str_, s);
}

template <typename T>
inline void TString<T>::pop_back()
{
	if (length() > 0)
		assign(begin(), length()-1);
}

template <typename T>
inline void TString<T>::push_back(value_type ch)
{
	_Concat(&ch, 1);
}

template <typename T>
inline typename TString<T>::size_type TString<T>::copy(value_type* dest, size_type count, size_type offset) const
{
	FatAssertNoText(offset < length());

	if (offset + count > length())
		count = length() - offset;

	CharTraits<T>::copy(dest, str_+offset, count);
	return count;
}

template <typename T>
inline void TString<T>::resize(size_type count, value_type ch)
{
	if (count > length())
	{
		_MakeUnique();
		append(count - length(), ch);
	}
	else if (count < length())
	{
		_MakeUnique();
		_Header()->length = (int) count;
		str_[length()] = '\0';
	}
}

template <typename T>
inline TString<T> TString<T>::substr(size_type pos, size_type count) const
{
	if (pos >= length())
		return TString<T>();

	if ((count == npos) || (pos + count > length()))
		count = length() - pos;

	return TString<T>(str_ + pos, count);
}

//
// replace()
//

template <typename T>
inline TString<T>& TString<T>::replace(value_type chOld, value_type chNew)
{
	if (chOld != chNew)
	{
		_MakeUnique();

		iterator theEnd = end();
		for (iterator it = begin(); it != theEnd; ++it)
		{
			if (*it == chOld)
			{
				*it = chNew;
			}
		}
	}
	return *this;
}

template <typename T>
TString<T>& TString<T>::replace(const_str strOld, const_str strNew)
{
	size_type sourceLen = CharTraits<T>::strlen(strOld);
	if (sourceLen == 0)
		return *this;
	size_type replacementLen = CharTraits<T>::strlen(strNew);

	size_type count = 0;
	iterator start  = begin();
	iterator theEnd = end();
	iterator target;
	while (start < theEnd)
	{
		while ((target = (value_type*)CharTraits<T>::strstr(start, strOld)) != NULL)
		{
			count++;
			start = target + sourceLen;
		}
		start += CharTraits<T>::strlen(start) + 1;
	}

	if (count > 0)
	{
		_MakeUnique();

		size_type oldLength = length();
		size_type newLength = oldLength + (replacementLen - sourceLen) * count;
		if (/* _Header()->GetRefcount() > 1 ||*/ newLength > capacity())
		{
			SStrHeader* pOldData = _Header();
			_AllocData(newLength);
			CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength());
			_FreeData(pOldData);
		}

		start  = begin();
		theEnd = end();
		while (start < theEnd)
		{
			while ((target = (value_type*)CharTraits<T>::strstr(start, strOld)) != NULL)
			{
				size_type balance = oldLength - ((size_type)(target - begin()) + sourceLen);
				CharTraits<T>::move(target + replacementLen, target + sourceLen, balance);
				CharTraits<T>::copy(target, strNew, replacementLen);
				start = target + replacementLen;
				start[balance] = '\0';
				oldLength += (replacementLen - sourceLen);
			}
			start += CharTraits<T>::strlen(start) + 1;
		}
		_Header()->length = (int)newLength;
	}

	return *this;
}

template <typename T>
inline TString<T>& TString<T>::replace(size_type pos, size_type count, const_str strNew)
{
	return replace(pos, count, strNew, CharTraits<T>::strlen(strNew));
}

template <typename T>
inline TString<T>& TString<T>::replace(size_type pos, size_type count, const_str strNew, size_type count2)
{
	erase(pos, count);
	insert(pos, strNew, count2);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::replace(size_type pos, size_type count, size_type numChars, value_type chNew)
{
	erase(pos, count);
	insert(pos, numChars, chNew);
	return *this;
}

//
// insert
//

template <typename T>
inline TString<T>& TString<T>::insert(size_type index, value_type ch)
{
	return insert(index, 1, ch);
}

template <typename T>
inline TString<T>& TString<T>::insert(size_type index, size_type count, value_type ch)
{
	if (count == 0)
		return *this;

	_MakeUnique();

	size_type oldLength = length();
	if (index > oldLength)
		index = oldLength;

	size_type newLength = oldLength + count;
	if (newLength > capacity())
	{
		SStrHeader* pOldData = _Header();
		_AllocData(newLength);
		CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength()+1);
		_FreeData(pOldData);
	}

	CharTraits<T>::move(str_ + index + count, str_ + index, (oldLength - index));
	CharTraits<T>::set(str_ + index, ch, count);
	_Header()->length = (int)newLength;
	str_[newLength] = '\0';
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::insert(size_type index, const_str s)
{
	return insert(index, s, CharTraits<T>::strlen(s));
}

template <typename T>
inline TString<T>& TString<T>::insert(size_type index, const_str s, size_type count)
{
	if (count == 0)
		return *this;

	_MakeUnique();

	size_type oldLength = length();
	if (index > oldLength)
		index = oldLength;

	size_type newLength = oldLength + count;
	if (newLength > capacity())
	{
		SStrHeader* pOldData = _Header();
		_AllocData(newLength);
		CharTraits<T>::copy(str_, pOldData->GetStr(), pOldData->GetLength()+1);
		_FreeData(pOldData);
	}

	CharTraits<T>::move(str_ + index + count, str_ + index, (oldLength - index));
	CharTraits<T>::copy(str_ + index, s, count);
	_Header()->length = (int)newLength;
	str_[newLength] = '\0';
	return *this;
}

//
// erase()
//

template <typename T>
inline TString<T>& TString<T>::erase(size_type index, size_type count)
{
	if (count > length() - index)
		count = length() - index;
	if (count > 0 && index < length())
	{
		_MakeUnique();

		size_type numToCopy = length() - (index + count) + 1; // including '\0'
		CharTraits<T>::move(str_ + index, str_ + index + count, numToCopy);
		_Header()->length -= (int)count;
	}
	return *this;
}

//
// find(), rfind()
//

template <typename T>
inline typename TString<T>::size_type TString<T>::find(value_type ch, size_type pos) const
{
	if (pos >= length())
		return npos;

	const_str str = CharTraits<T>::strchr(str_+pos, ch);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T>
inline typename TString<T>::size_type TString<T>::find(const_str subs, size_type pos) const
{
	if (pos >= length())
		return npos;

	const_str str = CharTraits<T>::strstr(str_+pos, subs);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T>
inline typename TString<T>::size_type TString<T>::rfind(value_type ch, size_type pos) const
{
	const_str s;
	if (pos == npos || pos >= length())
		s = end();
	else
		s = str_ + pos;

	while (s >= str_)
	{
		if (*s == ch)
			return (size_type)(s - str_);

		--s;
	}

	return npos;
}

//
// find_first_of()
//

template <typename T>
inline typename TString<T>::size_type TString<T>::find_first_of(const TString<T>& s, size_type offset) const
{
	return find_first_of(s.str_, offset);
}

template <typename T>
inline typename TString<T>::size_type TString<T>::find_first_of(const_str charSet, size_type offset) const
{
	if (offset > length())
		return npos;
	const_str str = CharTraits<T>::strpbrk(str_ + offset, charSet);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T>
inline typename TString<T>::size_type TString<T>::find_first_of(value_type ch, size_type offset) const
{
	if (offset > length())
		return npos;
	value_type charSet[2] = { ch, 0 };
	const_str str = CharTraits<T>::strpbrk(str_ + offset, charSet);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T>
inline void TString<T>::swap(MyType& rhs)
{
	value_type* tmp = rhs.str_;
	rhs.str_ = str_;
	str_ = tmp;
}

// s[i]
template <typename T>
inline typename TString<T>::value_type TString<T>::operator[](size_type i) const
{
	FatAssertNoText(i < length());
	return str_[i];
}

//
// s1 == s2, s1 != s2, s1 < s2, ...
//

template <typename T>
inline bool operator==(const TString<T>& s1, const TString<T>& s2)
{
	return s1.compare(s2) == 0;
}

template <typename T>
inline bool operator==(const TString<T>& s1, const typename TString<T>::value_type* s2)
{
	return s1.compare(s2) == 0;
}

template <typename T>
inline bool operator==(const typename TString<T>::value_type* s1, const TString<T>& s2)
{
	return s2.compare(s1) == 0;
}

template <typename T>
inline bool operator!=(const TString<T>& s1, const TString<T>& s2)
{
	return s1.compare(s2) != 0;
}

template <typename T>
inline bool operator!=(const TString<T>& s1, const typename TString<T>::value_type* s2)
{
	return s1.compare(s2) != 0;
}

template <typename T>
inline bool operator!=(const typename TString<T>::value_type* s1, const TString<T>& s2)
{
	return s2.compare(s1) != 0;
}

template <typename T>
inline bool operator<(const TString<T>& s1, const TString<T>& s2)
{
	return s1.compare(s2) < 0;
}

template <typename T>
inline bool operator<(const TString<T>& s1, const typename TString<T>::value_type* s2)
{
	return s1.compare(s2) < 0;
}

template <typename T>
inline bool operator<(const typename TString<T>::value_type* s1, const TString<T>& s2)
{
	return s2.compare(s1) > 0;
}

template <typename T>
inline bool operator>(const TString<T>& s1, const TString<T>& s2)
{
	return s1.compare(s2) > 0;
}

template <typename T>
inline bool operator>(const TString<T>& s1, const typename TString<T>::value_type* s2)
{
	return s1.compare(s2) > 0;
}

template <typename T>
inline bool operator>(const typename TString<T>::value_type* s1, const TString<T>& s2)
{
	return s2.compare(s1) < 0;
}


template <typename T>
inline bool operator<=(const TString<T>& s1, const TString<T>& s2)
{
	return s1.compare(s2) <= 0;
}

template <typename T>
inline bool operator<=(const TString<T>& s1, const typename TString<T>::value_type* s2)
{
	return s1.compare(s2) <= 0;
}

template <typename T>
inline bool operator<=(const typename TString<T>::value_type* s1, const TString<T>& s2)
{
	return s2.compare(s1) >= 0;
}

template <typename T>
inline bool operator>=(const TString<T>& s1, const TString<T>& s2)
{
	return s1.compare(s2) >= 0;
}

template <typename T>
inline bool operator>=(const TString<T>& s1, const typename TString<T>::value_type* s2)
{
	return s1.compare(s2) >= 0;
}

template <typename T>
inline bool operator>=(const typename TString<T>::value_type* s1, const TString<T>& s2)
{
	return s2.compare(s1) <= 0;
}

//
// s1 + s2
//

template <typename T>
inline TString<T> operator+(const TString<T>& lhs, typename TString<T>::value_type ch)
{
	TString<T> s(lhs);
	s.append(1, ch);
	return s;
}

template <typename T>
inline TString<T> operator+(typename TString<T>::value_type ch, const TString<T>& rhs)
{
	TString<T> s;
	s.reserve(rhs.size() + 1);
	s.append(1, ch);
	s.append(rhs);
	return s;
}

template <typename T>
inline TString<T> operator+(const TString<T>& lhs, const TString<T>& rhs)
{
	TString<T> s(lhs);
	s.append(rhs);
	return s;
}

template <typename T>
inline TString<T> operator+(const TString<T>& lhs, const typename TString<T>::value_type* rhs)
{
	TString<T> s(lhs);
	s.append(rhs);
	return s;
}

template <typename T>
inline TString<T> operator+(const typename TString<T>::value_type* lhs, const TString<T>& rhs)
{
	TString<T> s;
	s.reserve(CharTraits<T>::strlen(lhs) + rhs.size());
	s.append(lhs);
	s.append(rhs);
	return s;
}

//
// s1 = s2, s1 += s2
//

template <typename T>
TString<T>& TString<T>::operator=(const MyType& rhs)
{
	if (str_ == rhs.str_)
		return *this;

	if (_Header()->GetRefcount() < 0)
	{
		// empty string.
		if (rhs._Header()->GetRefcount() < 0)
		{
			// two empty strings
		}
		else
		{
			str_ = rhs.str_;
			_Header()->AddRef();
		}
	}
	else if (rhs._Header()->GetRefcount() < 0)
	{
		_Free();
		str_ = rhs.str_;
	}
	else
	{
		_Free();
		str_ = rhs.str_;
		_Header()->AddRef();
	}

	return *this;
}

template <typename T>
TString<T>& TString<T>::operator=(const_str s)
{
	FatAssertNoText(s != NULL);
	_Assign(s, CharTraits<T>::strlen(s));
	return *this;
}

template <typename T>
TString<T>& TString<T>::operator+=(const MyType& rhs)
{
	_Concat(rhs.c_str(), rhs.length());
	return *this;
}

template <typename T>
TString<T>& TString<T>::operator+=(const_str s)
{
	FatAssertNoText(s != NULL);
	_Concat(s, CharTraits<T>::strlen(s));
	return *this;
}

template <typename T>
TString<T>& TString<T>::operator+=(value_type ch)
{
	_Concat(&ch, 1);
	return *this;
}

//
// ATL CSTring interfaces. (not in STL)
//

template <typename T>
inline TString<T>& TString<T>::Format(const_str format, ...)
{
	va_list argList;
	va_start(argList, format);
	FormatV(format, argList);
	va_end(argList);
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::FormatV(const_str format, va_list argList)
{
#define BUFSIZE 4096

	value_type temp[BUFSIZE];

	// see TStackString::Format()
	int n = CharTraits<T>::vsnprintf(temp, BUFSIZE, format, argList);
	if (n < 0 || n > (BUFSIZE-1))
		n = (BUFSIZE-1);
	temp[n] = '\0';

	*this = temp;
	return *this;

#undef BUFSIZE
}

template <typename T>
inline TString<T>& TString<T>::MakeLower()
{
	_MakeUnique();

	for (value_type* s = str_; *s; ++s)
	{
		*s = CharTraits<T>::tolower_ascii(*s);
	}
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::MakeUpper()
{
	_MakeUnique();

	for (value_type* s = str_; *s; ++s)
	{
		*s = CharTraits<T>::toupper(*s);
	}
	return *this;
}

template <typename T>
inline TString<T>& TString<T>::Trim()
{
	return TrimRight().TrimLeft();
}

template <typename T>
inline TString<T>& TString<T>::Trim(value_type ch)
{
	const value_type charSet[2] = {ch, 0};
	return TrimRight(charSet).TrimLeft(charSet);
}

template <typename T>
inline TString<T>& TString<T>::Trim(const value_type* charSet)
{
	return TrimRight(charSet).TrimLeft(charSet);
}

template <typename T>
inline TString<T>& TString<T>::TrimLeft()
{
	const_str s = str_;
	while ((*s) && CharTraits<T>::isspace(*s))
		++s;

	if (s != str_)
	{
		size_type offset = (size_type)(s - str_); // str_ may change in _MakeUnique()
		_MakeUnique();
		size_type newLength = length() - offset;
		CharTraits<T>::move(str_, str_+offset, newLength);
		_Header()->length = (int)newLength;
		str_[length()] = '\0';
	}

	return *this;
}

template <typename T>
inline TString<T>& TString<T>::TrimLeft(value_type ch)
{
	const value_type charSet[2] = {ch, 0};
	return TrimLeft(charSet);
}

template <typename T>
inline TString<T>& TString<T>::TrimLeft(const value_type* charSet)
{
	FatAssertNoText(charSet && *charSet);

	const_str s = str_;
	while ((*s) && CharTraits<T>::strchr(charSet, *s))
		++s;

	if (s != str_)
	{
		size_type offset = (size_type)(s - str_); // str_ may change in _MakeUnique()
		_MakeUnique();
		size_type newLength = length() - offset;
		CharTraits<T>::move(str_, str_+offset, newLength);
		_Header()->length = (int)newLength;
		str_[length()] = '\0';
	}

	return *this;
}

template <typename T>
inline TString<T>& TString<T>::TrimRight()
{
	if (length() < 1)
		return *this;

	const value_type* last = str_ + length() - 1;
	const value_type* s = last;
	while ((s >= str_) && CharTraits<T>::isspace(*s))
		--s;

	if (s != last)
	{
		size_type newLength = (size_type)(s - str_ + 1); // s - str_ can be -1
		_MakeUnique();
		_Header()->length = (int)newLength;
		str_[length()] = '\0';
	}

	return *this;
}

template <typename T>
inline TString<T>& TString<T>::TrimRight(value_type ch)
{
	const value_type charSet[2] = {ch, 0};
	return TrimRight(charSet);
}

template <typename T>
inline TString<T>& TString<T>::TrimRight(const value_type* charSet)
{
	FatAssertNoText(charSet && *charSet);

	if (length() < 1)
		return *this;

	const value_type* last = str_ + length() - 1;
	const value_type* s = last;
	while ((s >= str_) && CharTraits<T>::strchr(charSet, *s))
		--s;

	if (s != last)
	{
		size_type newLength = (size_type)(s - str_ + 1); // s - str_ can be -1
		_MakeUnique();
		_Header()->length = (int)newLength;
		str_[length()] = '\0';
	}

	return *this;
}

template <typename T>
inline TString<T> TString<T>::SpanIncluding(const_str charSet) const
{
	return Left((size_type)CharTraits<T>::strspn(str_, charSet));
}

template <typename T>
inline TString<T> TString<T>::SpanExcluding(const_str charSet) const
{
	return Left((size_type)CharTraits<T>::strcspn(str_, charSet));
}

template <typename T>
inline TString<T> TString<T>::Tokenize(const_str charSet, int& start) const
{
	FatAssertNoText(charSet && *charSet);

	if (start < 0)
		return TString<T>();

	const_str s = str_ + start;
	const_str theEnd = end();
	if (s < theEnd)
	{
		int including = (int) CharTraits<T>::strspn(s, charSet);
		if ((s + including) < theEnd)
		{
			s += including;
			int excluding = (int) CharTraits<T>::strcspn(s, charSet);
			int from = start + including;
			start = from + excluding + 1; // next start position
			return substr(from, excluding);
		}
	}

	// -1 means ended
	start = -1;
	return TString<T>();
}

template <typename T>
inline TString<T> TString<T>::Left(size_type count) const
{
	if (count == npos)
		return TString<T>();

	if (count > length())
		return *this;

	return TString<T>(str_, count);
}

template <typename T>
inline TString<T> TString<T>::Right(size_type count) const
{
	if (count == npos)
		return TString<T>();

	if (count > length())
		return *this;

	return TString<T>(str_ + length() - count, count);
}

template <typename T>
size_t TString<T>::GetAllocatedMemory() const
{
	return _CalcAllocSize(_Header()->GetLength());
}

//
// String, WString
// 

typedef TString<char>    String;
typedef TString<wchar_t> WString;

}
