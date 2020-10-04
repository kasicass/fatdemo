// Stack-based fixed-size String, similar to TString

#pragma once

#include "Kernel/Common/Assertion.h"
#include "Kernel/String/TCharTraits.h"
#include <stdarg.h>

namespace Fat {

//
// TStackString
//

template <typename T, size_t S>
class TStackString
{
public:
	// Types compatible with STL string.
	typedef TStackString<T, S> MyType;
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
	TStackString();
	TStackString(const MyType& rhs);
	TStackString(const MyType& rhs, size_type offset, size_type count);
	explicit TStackString(value_type ch, size_type repeat = 1);
	TStackString(const_str s);
	TStackString(const_str s, size_type length);
	TStackString(const_iterator first, const_iterator last);
	~TStackString();

	//
	// STL string like interface.
	//

	size_type length() const { return length_; }
	size_type size() const { return length_; }
	bool empty() const { return length_ == 0; }
	void clear();

	inline size_type capacity() const { return S - 1; }  // the storage currently allocated to hold the string, is always == (S - 1)
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

	// void swap(MyType& rhs);

	// overloaded operators.
	value_type& operator[](size_type i);
	const value_type& operator[](size_type i) const;

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

private:
	// helpers
	void _Initialize();
	void _AllocData(size_type length);

	void _Assign(const_str s, size_type length);
	void _Concat(const_str s, size_type length);

private:
	size_type length_;
	value_type str_[S];
};

template <typename T, size_t S>
inline void TStackString<T, S>::_Initialize()
{
	str_[0] = '\0';
	length_ = 0;
}

template <typename T, size_t S>
inline void TStackString<T, S>::_AllocData(size_type length)
{
	FatAssertNoText(length <= capacity());
	length_ = length;
	str_[length_] = '\0';
}

template <typename T, size_t S>
inline void TStackString<T, S>::_Assign(const_str s, size_type slen)
{
	_AllocData(slen);
	CharTraits<T>::copy(str_, s, slen);
}

template <typename T, size_t S>
inline void TStackString<T, S>::_Concat(const_str s, size_type slen)
{
	if (slen != 0)
	{
		FatAssertNoText(length() + slen <= capacity());
		CharTraits<T>::copy(str_ + length(), s, slen);
		length_ += slen;
		str_[length_] = '\0';
	}
}

template <typename T, size_t S>
TStackString<T, S>::TStackString()
{
	_Initialize();
}

template <typename T, size_t S>
TStackString<T, S>::TStackString(const MyType& rhs)
{
	_Initialize();
	_Assign(rhs.c_str(), rhs.length());
}

template <typename T, size_t S>
TStackString<T, S>::TStackString(const MyType& rhs, size_type offset, size_type count)
{
	_Initialize();
	assign(rhs, offset, count);
}

template <typename T, size_t S>
TStackString<T, S>::TStackString(value_type ch, size_type repeat)
{
	_Initialize();
	if (repeat > 0)
	{
		_AllocData(repeat);
		CharTraits<T>::set(str_, ch, repeat);
	}
}

template <typename T, size_t S>
TStackString<T, S>::TStackString(const_str s)
{
	_Initialize();
	size_type slen = CharTraits<T>::strlen(s);
	if (slen > 0)
	{
		_AllocData(slen);
		CharTraits<T>::copy(str_, s, slen);
	}
}

template <typename T, size_t S>
TStackString<T, S>::TStackString(const_str s, size_type slen)
{
	_Initialize();
	if (slen > 0)
	{
		_AllocData(slen);
		CharTraits<T>::copy(str_, s, slen);
	}
}

template <typename T, size_t S>
TStackString<T, S>::TStackString(const_iterator first, const_iterator last)
{
	_Initialize();
	size_type length = (size_type)(last - first);
	if (length > 0)
	{
		_AllocData(length);
		CharTraits<T>::copy(str_, first, length);
	}
}

template <typename T, size_t S>
TStackString<T, S>::~TStackString()
{
}

template <typename T, size_t S>
inline void TStackString<T, S>::clear()
{
	_Initialize();
}

template <typename T, size_t S>
inline void TStackString<T, S>::reserve(size_type count)
{
	FatAssertNoText(count <= capacity());
}

template <typename T, size_t S>
inline void TStackString<T, S>::shrink_to_fit()
{
	// do nothing
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::append(const_str s)
{
	*this += s;
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::append(const_str s, size_type count)
{
	_Concat(s, count);
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::append(const MyType& rhs, size_type offset, size_type count)
{
	size_type len = rhs.length();
	if (offset > len)
		return *this;
	if (offset + count > len)
		count = len - offset;
	_Concat(rhs.str_ + offset, count);
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::append(const MyType& rhs)
{
	*this += rhs;
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::append(size_type count, value_type ch)
{
	FatAssertNoText(length() + count <= capacity());
	CharTraits<T>::set(str_ + length_, ch, count);
	length_ += count;
	str_[length_] = '\0';
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::append(const_iterator first, const_iterator last)
{
	append(first, (last - first));
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::assign(const_str s)
{
	*this = s;
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::assign(const_str s, size_type count)
{
	size_type len = CharTraits<T>::strlen(s);
	_Assign(s, (count < len) ? count : len);
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::assign(const MyType& rhs, size_type offset, size_type count)
{
	size_type len = rhs.length();
	if (offset > len)
		return *this;
	if (offset + count > len)
		count = len - offset;
	_Assign(rhs.str_ + offset, count);
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::assign(const MyType& rhs)
{
	*this = rhs;
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::assign(size_type count, value_type ch)
{
	FatAssertNoText(count <= capacity());
	CharTraits<T>::set(str_, ch, count);
	length_ = count;
	str_[length_] = '\0';
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::assign(const_iterator first, const_iterator last)
{
	assign(first, (size_type)(last - first));
	return *this;
}

template <typename T, size_t S>
inline typename TStackString<T, S>::value_type TStackString<T, S>::at(size_type i) const
{
	FatAssertNoText(i >= 0 && i < length());
	return str_[i];
}

template <typename T, size_t S>
inline int TStackString<T, S>::compare(const MyType& rhs) const
{
	return CharTraits<T>::strcmp(str_, rhs.str_);
}

template <typename T, size_t S>
inline int TStackString<T, S>::compare(const_str s) const
{
	return CharTraits<T>::strcmp(str_, s);
}

template <typename T, size_t S>
inline int TStackString<T, S>::compareNoCase(const MyType& rhs) const
{
	return CharTraits<T>::stricmp(str_, rhs.str_);
}

template <typename T, size_t S>
inline int TStackString<T, S>::compareNoCase(const_str s) const
{
	return CharTraits<T>::stricmp(str_, s);
}

template <typename T, size_t S>
inline void TStackString<T, S>::pop_back()
{
	if (length_ > 0)
	{
		length_ -= 1;
		str_[length_] = '\0';
	}
}

template <typename T, size_t S>
inline void TStackString<T, S>::push_back(value_type ch)
{
	_Concat(&ch, 1);
}

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::copy(value_type* dest, size_type count, size_type offset) const
{
	FatAssertNoText(offset < length());

	if (offset + count > length())
		count = length() - offset;

	CharTraits<T>::copy(dest, str_+offset, count);
	return count;
}

template <typename T, size_t S>
inline void TStackString<T, S>::resize(size_type count, value_type ch)
{
	FatAssertNoText(count <= capacity());
	if (count > length())
	{
		append(count - length(), ch);
	}
	else if (count < length())
	{
		length_ = count;
		str_[length_] = '\0';
	}
}

template <typename T, size_t S>
inline TStackString<T, S> TStackString<T, S>::substr(size_type pos, size_type count) const
{
	if (pos >= length())
		return TStackString<T, S>();

	if ((count == npos) || (pos + count > length()))
		count = length() - pos;

	return TStackString<T, S>(str_ + pos, count);
}

//
// replace()
//

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::replace(value_type chOld, value_type chNew)
{
	if (chOld != chNew)
	{
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

template <typename T, size_t S>
TStackString<T, S>& TStackString<T, S>::replace(const_str strOld, const_str strNew)
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
		size_type oldLength = length();
		size_type newLength = oldLength + (replacementLen - sourceLen) * count;
		FatAssertNoText(newLength <= capacity());

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
		length_ = newLength;
	}

	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::replace(size_type pos, size_type count, const_str strNew)
{
	return replace(pos, count, strNew, CharTraits<T>::strlen(strNew));
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::replace(size_type pos, size_type count, const_str strNew, size_type count2)
{
	erase(pos, count);
	insert(pos, strNew, count2);
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::replace(size_type pos, size_type count, size_type numChars, value_type chNew)
{
	erase(pos, count);
	insert(pos, numChars, chNew);
	return *this;
}

//
// insert
//

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::insert(size_type index, value_type ch)
{
	return insert(index, 1, ch);
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::insert(size_type index, size_type count, value_type ch)
{
	if (count == 0)
		return *this;

	size_type newLength = length();
	if (index > newLength)
		index = newLength;
	newLength += count;

	FatAssertNoText(newLength <= capacity());

	CharTraits<T>::move(str_ + index + count, str_ + index, (length() - index));
	CharTraits<T>::set(str_ + index, ch, count);
	length_ = newLength;
	str_[length_] = '\0';
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::insert(size_type index, const_str s)
{
	return insert(index, s, CharTraits<T>::strlen(s));
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::insert(size_type index, const_str s, size_type count)
{
	if (count == 0)
		return *this;

	size_type newLength = length();
	if (index > newLength)
		index = newLength;
	newLength += count;

	FatAssertNoText(newLength <= capacity());

	CharTraits<T>::move(str_ + index + count, str_ + index, (length() - index));
	CharTraits<T>::copy(str_ + index, s, count);
	length_ = newLength;
	str_[length_] = '\0';
	return *this;
}

//
// erase()
//

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::erase(size_type index, size_type count)
{
	if (count > length() - index)
		count = length() - index;
	if (count > 0 && index < length())
	{
		size_type numToCopy = length() - (index + count) + 1; // including '\0'
		CharTraits<T>::move(str_ + index, str_ + index + count, numToCopy);
		length_ -= count;
	}
	return *this;
}

//
// find(), rfind()
//

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::find(value_type ch, size_type pos) const
{
	if (pos >= length())
		return npos;

	const_str str = CharTraits<T>::strchr(str_+pos, ch);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::find(const_str subs, size_type pos) const
{
	if (pos >= length())
		return npos;

	const_str str = CharTraits<T>::strstr(str_+pos, subs);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::rfind(value_type ch, size_type pos) const
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

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::find_first_of(const TStackString<T, S>& s, size_type offset) const
{
	return find_first_of(s.str_, offset);
}

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::find_first_of(const_str charSet, size_type offset) const
{
	if (offset > length())
		return npos;
	const_str str = CharTraits<T>::strpbrk(str_ + offset, charSet);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

template <typename T, size_t S>
inline typename TStackString<T, S>::size_type TStackString<T, S>::find_first_of(value_type ch, size_type offset) const
{
	if (offset > length())
		return npos;
	value_type charSet[2] = { ch, 0 };
	const_str str = CharTraits<T>::strpbrk(str_ + offset, charSet);
	return (str == NULL) ? npos : (size_type)(str - str_);
}

#if 0 // should not swap()
template <typename T, size_t S>
inline void TStackString<T, S>::swap(MyType& rhs)
{
	// *((value_type**)&) make compiler happy~
	value_type* tmp = rhs.str_;
	((value_type*)rhs.str_) = str_;
	((value_type*)str_) = tmp;

	size_type tmpLen = rhs.length_;
	rhs.length_ = length_;
	length_ = tmpLen;
}
#endif

// s[i]
template <typename T, size_t S>
inline typename TStackString<T, S>::value_type& TStackString<T, S>::operator[](size_type i)
{
	FatAssertNoText(i < length());
	return str_[i];
}

template <typename T, size_t S>
inline const typename TStackString<T, S>::value_type& TStackString<T, S>::operator[](size_type i) const
{
	FatAssertNoText(i < length());
	return str_[i];
}

//
// s1 == s2, s1 != s2, s1 < s2, ...
//

template <typename T, size_t S>
inline bool operator==(const TStackString<T, S>& s1, const TStackString<T, S>& s2)
{
	return s1.compare(s2) == 0;
}

template <typename T, size_t S>
inline bool operator==(const TStackString<T, S>& s1, const typename TStackString<T, S>::value_type* s2)
{
	return s1.compare(s2) == 0;
}

template <typename T, size_t S>
inline bool operator==(const typename TStackString<T, S>::value_type* s1, const TStackString<T, S>& s2)
{
	return s2.compare(s1) == 0;
}

template <typename T, size_t S>
inline bool operator!=(const TStackString<T, S>& s1, const TStackString<T, S>& s2)
{
	return s1.compare(s2) != 0;
}

template <typename T, size_t S>
inline bool operator!=(const TStackString<T, S>& s1, const typename TStackString<T, S>::value_type* s2)
{
	return s1.compare(s2) != 0;
}

template <typename T, size_t S>
inline bool operator!=(const typename TStackString<T, S>::value_type* s1, const TStackString<T, S>& s2)
{
	return s2.compare(s1) != 0;
}

template <typename T, size_t S>
inline bool operator<(const TStackString<T, S>& s1, const TStackString<T, S>& s2)
{
	return s1.compare(s2) < 0;
}

template <typename T, size_t S>
inline bool operator<(const TStackString<T, S>& s1, const typename TStackString<T, S>::value_type* s2)
{
	return s1.compare(s2) < 0;
}

template <typename T, size_t S>
inline bool operator<(const typename TStackString<T, S>::value_type* s1, const TStackString<T, S>& s2)
{
	return s2.compare(s1) > 0;
}

template <typename T, size_t S>
inline bool operator>(const TStackString<T, S>& s1, const TStackString<T, S>& s2)
{
	return s1.compare(s2) > 0;
}

template <typename T, size_t S>
inline bool operator>(const TStackString<T, S>& s1, const typename TStackString<T, S>::value_type* s2)
{
	return s1.compare(s2) > 0;
}

template <typename T, size_t S>
inline bool operator>(const typename TStackString<T, S>::value_type* s1, const TStackString<T, S>& s2)
{
	return s2.compare(s1) < 0;
}


template <typename T, size_t S>
inline bool operator<=(const TStackString<T, S>& s1, const TStackString<T, S>& s2)
{
	return s1.compare(s2) <= 0;
}

template <typename T, size_t S>
inline bool operator<=(const TStackString<T, S>& s1, const typename TStackString<T, S>::value_type* s2)
{
	return s1.compare(s2) <= 0;
}

template <typename T, size_t S>
inline bool operator<=(const typename TStackString<T, S>::value_type* s1, const TStackString<T, S>& s2)
{
	return s2.compare(s1) >= 0;
}

template <typename T, size_t S>
inline bool operator>=(const TStackString<T, S>& s1, const TStackString<T, S>& s2)
{
	return s1.compare(s2) >= 0;
}

template <typename T, size_t S>
inline bool operator>=(const TStackString<T, S>& s1, const typename TStackString<T, S>::value_type* s2)
{
	return s1.compare(s2) >= 0;
}

template <typename T, size_t S>
inline bool operator>=(const typename TStackString<T, S>::value_type* s1, const TStackString<T, S>& s2)
{
	return s2.compare(s1) <= 0;
}

//
// s1 + s2
//

template <typename T, size_t S>
inline TStackString<T, S> operator+(const TStackString<T, S>& lhs, typename TStackString<T, S>::value_type ch)
{
	TStackString<T, S> s(lhs);
	s.append(1, ch);
	return s;
}

template <typename T, size_t S>
inline TStackString<T, S> operator+(typename TStackString<T, S>::value_type ch, const TStackString<T, S>& rhs)
{
	TStackString<T, S> s(ch, 1);
	s.append(rhs);
	return s;
}

template <typename T, size_t S>
inline TStackString<T, S> operator+(const TStackString<T, S>& lhs, const TStackString<T, S>& rhs)
{
	TStackString<T, S> s(lhs);
	s.append(rhs);
	return s;
}

template <typename T, size_t S>
inline TStackString<T, S> operator+(const TStackString<T, S>& lhs, const typename TStackString<T, S>::value_type* rhs)
{
	TStackString<T, S> s(lhs);
	s.append(rhs);
	return s;
}

template <typename T, size_t S>
inline TStackString<T, S> operator+(const typename TStackString<T, S>::value_type* lhs, const TStackString<T, S>& rhs)
{
	TStackString<T, S> s(lhs);
	s.append(rhs);
	return s;
}

//
// s1 = s2, s1 += s2
//

template <typename T, size_t S>
TStackString<T, S>& TStackString<T, S>::operator=(const MyType& rhs)
{
	_Assign(rhs.c_str(), rhs.length());
	return *this;
}

template <typename T, size_t S>
TStackString<T, S>& TStackString<T, S>::operator=(const_str s)
{
	FatAssertNoText(s != NULL);
	_Assign(s, CharTraits<T>::strlen(s));
	return *this;
}

template <typename T, size_t S>
TStackString<T, S>& TStackString<T, S>::operator+=(const MyType& rhs)
{
	_Concat(rhs.c_str(), rhs.length());
	return *this;
}

template <typename T, size_t S>
TStackString<T, S>& TStackString<T, S>::operator+=(const_str s)
{
	FatAssertNoText(s != NULL);
	_Concat(s, CharTraits<T>::strlen(s));
	return *this;
}

template <typename T, size_t S>
TStackString<T, S>& TStackString<T, S>::operator+=(value_type ch)
{
	_Concat(&ch, 1);
	return *this;
}

//
// ATL CSTring interfaces. (not in STL)
//

template <typename T, size_t S>
inline TStackString<T,S>& TStackString<T,S>::Format(const_str format, ...)
{
	va_list argList;
	va_start(argList, format);
	FormatV(format, argList);
	va_end(argList);
	return *this;
}

template <typename T, size_t S>
inline TStackString<T,S>& TStackString<T,S>::FormatV(const_str format, va_list argList)
{
	// _vsnprintf/vsnprintf return value is different in win32/linux
	// win32 - not put '\0' for us, will return -1
	// linux - put '\0' for us, not return -1
	//
	// https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/vsnprintf-vsnprintf-vsnprintf-l-vsnwprintf-vsnwprintf-l?view=vs-2019
	// https://linux.die.net/man/3/vsnprintf
	int n = CharTraits<T>::vsnprintf(str_, S, format, argList);
	if (n < 0 || n > (int)capacity())
		n = (int)capacity();
	length_ = (size_t)n;
	str_[n] = '\0';
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::MakeLower()
{
	for (value_type* s = str_; *s; ++s)
	{
		*s = CharTraits<T>::tolower_ascii(*s);
	}
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::MakeUpper()
{
	for (value_type* s = str_; *s; ++s)
	{
		*s = CharTraits<T>::toupper(*s);
	}
	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::Trim()
{
	return TrimRight().TrimLeft();
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::Trim(value_type ch)
{
	const value_type charSet[2] = {ch, 0};
	return TrimRight(charSet).TrimLeft(charSet);
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::Trim(const value_type* charSet)
{
	return TrimRight(charSet).TrimLeft(charSet);
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::TrimLeft()
{
	const_str s = str_;
	while ((*s) && CharTraits<T>::isspace(*s))
		++s;

	if (s != str_)
	{
		size_type offset = (size_type)(s - str_);
		size_type newLength = length() - offset;
		CharTraits<T>::move(str_, str_+offset, newLength);
		length_ = newLength;
		str_[length_] = '\0';
	}

	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::TrimLeft(value_type ch)
{
	const value_type charSet[2] = {ch, 0};
	return TrimLeft(charSet);
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::TrimLeft(const value_type* charSet)
{
	FatAssertNoText(charSet && *charSet);

	const_str s = str_;
	while ((*s) && CharTraits<T>::strchr(charSet, *s))
		++s;

	if (s != str_)
	{
		size_type offset = (size_type)(s - str_);
		size_type newLength = length() - offset;
		CharTraits<T>::move(str_, str_+offset, newLength);
		length_ = newLength;
		str_[length_] = '\0';
	}

	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::TrimRight()
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
		length_ = newLength;
		str_[length_] = '\0';
	}

	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::TrimRight(value_type ch)
{
	const value_type charSet[2] = {ch, 0};
	return TrimRight(charSet);
}

template <typename T, size_t S>
inline TStackString<T, S>& TStackString<T, S>::TrimRight(const value_type* charSet)
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
		length_ = newLength;
		str_[length_] = '\0';
	}

	return *this;
}

template <typename T, size_t S>
inline TStackString<T, S> TStackString<T, S>::SpanIncluding(const_str charSet) const
{
	return Left((size_type)CharTraits<T>::strspn(str_, charSet));
}

template <typename T, size_t S>
inline TStackString<T, S> TStackString<T, S>::SpanExcluding(const_str charSet) const
{
	return Left((size_type)CharTraits<T>::strcspn(str_, charSet));
}

template <typename T, size_t S>
inline TStackString<T, S> TStackString<T, S>::Tokenize(const_str charSet, int& start) const
{
	FatAssertNoText(charSet && *charSet);

	if (start < 0)
		return TStackString<T, S>();

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
	return TStackString<T, S>();
}

template <typename T, size_t S>
inline TStackString<T, S> TStackString<T, S>::Left(size_type count) const
{
	if (count == npos)
		return TStackString<T, S>();

	if (count > length())
		return *this;

	return TStackString<T, S>(str_, count);
}

template <typename T, size_t S>
inline TStackString<T, S> TStackString<T, S>::Right(size_type count) const
{
	if (count == npos)
		return TStackString<T, S>();

	if (count > length())
		return *this;

	return TStackString<T, S>(str_ + length() - count, count);
}

//
// StackString, StackWString
// 

typedef TStackString<char, 32>     StackString32;
typedef TStackString<wchar_t, 32>  StackWString32;
typedef TStackString<char, 64>     StackString64;
typedef TStackString<wchar_t, 64>  StackWString64;
typedef TStackString<char, 128>    StackString128;
typedef TStackString<wchar_t, 128> StackWString128;
typedef TStackString<char, 256>    StackString256;
typedef TStackString<wchar_t, 256> StackWString256;
typedef TStackString<char, 512>    StackString512;
typedef TStackString<wchar_t, 512> StackWString512;
typedef StackString512             StackString;
typedef StackWString512            StackWString;

}
