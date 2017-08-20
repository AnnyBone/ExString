#pragma once
#pragma once

#ifndef EXSTRING_H
#define EXSTRING_H

#include "Windows.h"

template<class TCHAR>
void g_str_move_chars(const TCHAR *Str, DWORD StrLen, DWORD Position, int Count)
{
	const TCHAR* _start = Str + Position;
	const TCHAR *_end = Str + StrLen;

	if (Count > 0)
	{
		TCHAR *new_str = const_cast<TCHAR*>(Str + (StrLen + Count));
		do
		{
			*new_str = *_end;
			--new_str;
			--_end;
		} while (_end >= _start);

	}
	else
	{

		TCHAR *new_str = const_cast<TCHAR*>(Str + (Position + Count));
		if (new_str < Str)
			new_str = const_cast<TCHAR*>(Str);
		do
		{
			*new_str = *_start;
			++new_str;
			++_start;
		} while (_start <= _end);
	}
}

template<class TCHAR>
void g_str_insert(const TCHAR *Str, DWORD StrLen, DWORD Position, const TCHAR *SubStr, DWORD SubStrLen)
{
	g_str_move_chars<TCHAR>(Str, StrLen, Position, SubStrLen);
	memcpy(LPVOID(Str + Position), LPVOID(SubStr), SubStrLen * sizeof(TCHAR));
}


template <typename TCHAR>
class ExBaseString
{
public:

	
	typedef size_t size_type;
	typedef TCHAR value_type;
	typedef TCHAR* value_iterator;
	ExBaseString();
	ExBaseString(TCHAR string, size_type size);;
	~ExBaseString();


	// Modificadores;
	void add(const TCHAR string);
	void free();

	//acesores 
	value_type c_string() const;

	// Operaciones
	ExBaseString substr(size_type position, size_type count);


private:
	void _allocate(size_type value);
	void _deallocate();
	void _cpy(TCHAR dest, const TCHAR src, size_type count);

	static void   strcpy(TCHAR dest,  TCHAR src);
	static size_t strlen(TCHAR src);

public:
	operator TCHAR();

	bool operator ==(const TCHAR string) const;
	bool operator ==(const ExBaseString& string) const;

	bool operator !=(const TCHAR string) const;
	bool operator !=(const ExBaseString& string) const;

	bool operator >(const TCHAR string) const;
	bool operator >(const ExBaseString& string) const;

	bool operator <(const TCHAR string) const;
	bool operator <(const ExBaseString& string) const;

	bool operator >=(const TCHAR string) const;
	bool operator >=(ExBaseString& string) const;

	bool operator <=(const TCHAR string) const;
	bool operator <=(const ExBaseString& string) const;

	ExBaseString& operator +(const TCHAR string);
	ExBaseString& operator + (const ExBaseString& string);

	void operator +=(const TCHAR string);
	void operator +=(const ExBaseString& string);


	ExBaseString& operator =(const TCHAR string);
	ExBaseString& operator =(const ExBaseString& string);

private:
	value_type StringBuffer;
	size_type _length; // esto es el tamño de la cadena
	size_type _size; // esto es el tamaño de la cadena incluyendo la terminacion null

protected:
};


template <typename TCHAR>
ExBaseString<TCHAR>::ExBaseString()
{
	RtlSecureZeroMemory(this, sizeof(*this));
}

template <typename TCHAR>
ExBaseString<TCHAR>::ExBaseString(TCHAR string, size_type size)
{
	RtlSecureZeroMemory(this, sizeof(*this));
	_allocate(size);
	strcpy(StringBuffer, string);
}

template <typename TCHAR>
void ExBaseString<TCHAR>::add(const TCHAR string)
{
	size_type c_length = strlen(string);
	size_type position = _length;
	if (_size == 0 && _length == 0)
	{
		_allocate(c_length);
		StringBuffer[_length] = 0;
		strcpy(StringBuffer, string);
		return;
	}

	{

		size_type NewBufSize = c_length + _length;

		value_type *temp = new value_type[NewBufSize];
		_cpy(value_type(temp), StringBuffer, _length);
		_allocate(NewBufSize);
		_cpy(StringBuffer, value_type(temp), _size);
		delete[]temp;
	}

	g_str_insert(StringBuffer, _length, position, string, c_length);
	StringBuffer[_length] = 0;
	

}

template <typename TCHAR>
void ExBaseString<TCHAR>::free()
{
	_deallocate();
}

template <typename TCHAR>
typename ExBaseString<TCHAR>::value_type ExBaseString<TCHAR>::c_string() const
{
	return StringBuffer;
}

template <typename TCHAR>
ExBaseString<TCHAR> ExBaseString<TCHAR>::substr(size_type position, size_type count)
{
	ExBaseString temp("\0", count);
	for (size_type i = position; i < position + count; i++)
		temp[i - position] = StringBuffer[i];

	return temp;
}

template <typename TCHAR>
void ExBaseString<TCHAR>::_allocate(size_type value)
{
	value_type *temp = nullptr;
	if (value >= 0)
	{
		if (_size > 0) _deallocate();

		if (value != 0)
		{
			_length = value;
			_size = value + 1;
			temp = new value_type[_size * sizeof(value_type)];
			StringBuffer = value_type(temp);
			//StringBuffer = value_type(Mem::Malloc(_size));
		}
	}
}

template <typename TCHAR>
void ExBaseString<TCHAR>::_deallocate()
{
	// deallocate
	if (_length > 0 && StringBuffer != NULL)
	{
		_size = 0;
		_length = 0;
		delete[] StringBuffer;
		StringBuffer = nullptr;
	}
}

template <typename TCHAR>
void ExBaseString<TCHAR>::_cpy(TCHAR dest, const TCHAR src, size_type count)
{
	TCHAR tmp = TCHAR(dest);
	TCHAR s = TCHAR(src);
	while (count--) *tmp++ = *s++;
}

template <typename TCHAR>
void ExBaseString<TCHAR>::strcpy(TCHAR dest,  TCHAR src)
{
	TCHAR tmp = dest;
	while ((*dest++ = *src++) != '\0');
	return;
}

template <typename TCHAR>
size_t ExBaseString<TCHAR>::strlen(TCHAR src)
{
	TCHAR sc;

	for (sc = src; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - src;
}

template <typename TCHAR>
ExBaseString<TCHAR>::~ExBaseString()
{
	_deallocate();
}

template <class TCHAR>
ExBaseString<TCHAR>::operator TCHAR()
{
	return static_cast<TCHAR>(StringBuffer);
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator==(const TCHAR string) const
{
	int szStr = Strlen(string);

	// comprobamos  el tamaño  del string
	if (this->_length != szStr) return false;

	// ahora comprabamos  cada  caracter
	for (int i = 0; i < this->_size && this->_length == szStr; i++)
		if (this->StringBuffer[i] != string[i]) return false;

	//  si todo  esta correcto  devolvemos true
	return true;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator ==(const ExBaseString& string) const
{
	if ((this->_length != string._length))
		return false;

	for (int iter = 0; iter < this->_size && this->_length == string._length; iter++)
		if (this->StringBuffer[iter] != string.StringBuffer[iter]) return false;

	return true;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator !=(const TCHAR string) const
{
	int szStr = Strlen(string);
	if (this->_length != szStr) return true;

	for (int i = 0; i < this->_size && this->_length == szStr; i++)
		if (this->StringBuffer[i] != string[i]) return true;

	return false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator!=(const ExBaseString& string) const
{
	int is_ok = false;
	// comprabamos el tamaño
	if (this->_length != string._length) return true;

	for (int i = 0; i < this->_size && string._length != NULL; i++)
		if (StringBuffer[i] != string.StringBuffer[i]) return true;

	return false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator>(const TCHAR string) const
{
	int length = Strlen(string);
	return (this->_length > length) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator>(const ExBaseString& string) const
{
	return (this->_length > string._length) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator<(const TCHAR string) const
{
	int length = Strlen(string);
	return (this->_length < length) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator<(const ExBaseString& string) const
{
	return (this->_length < string._length) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator>=(const TCHAR string) const
{
	int _len = Strlen(string);
	return (this->_length >= _len) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator>=(ExBaseString& string) const
{
	return (this->_length >= string._length) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator<=(const TCHAR string) const
{
	int _len = Strlen(string);
	return (this->_length <= _len) ? true : false;
}

template <class TCHAR>
bool ExBaseString<TCHAR>::operator<=(const ExBaseString& string) const
{
	return (this->_length <= string._length) ? true : false;
}

template <class TCHAR>
ExBaseString<TCHAR>& ExBaseString<TCHAR>::operator+(const TCHAR string)
{
	this->add(string);
	return *this;
}

template <class TCHAR>
ExBaseString<TCHAR>& ExBaseString<TCHAR>::operator+(const ExBaseString& string)
{
	return (*this + string.StringBuffer);
}

template <class TCHAR>
void ExBaseString<TCHAR>::operator+=(const TCHAR string)
{
	this->add(string);
}

template <class TCHAR>
void ExBaseString<TCHAR>::operator+=(const ExBaseString& string)
{
	return (*this += string.StringBuffer);
}

template <class TCHAR>
ExBaseString<TCHAR>& ExBaseString<TCHAR>::operator=(const TCHAR string)
{
	_deallocate();
	add(string);
	return *this;
}

template <class TCHAR>
ExBaseString<TCHAR>& ExBaseString<TCHAR>::operator=(const ExBaseString& string)
{
	if (this != &string)
		return (*this = string.StringBuffer);

	return *this;
}


typedef ExBaseString<LPSTR> ExStringA;



#endif // EXSTRING_H
