#pragma once
/*
* SEH LIST made by konmio616, 2024-01
* Using alloc and seh, I made it possible to use lists without critical section exceptions.
* Please turn on EHa in project properties.
* this is for x64 project. if you use x86, change QWORD to DWORD
*/
#ifndef SEH_LIST_HEADER
#define SEH_LIST_HEADER
#include <Windows.h>
typedef unsigned long long QWORD;

template <typename T>
class SEHIterator
{
private:
	T* ptr;

public:
	SEHIterator(T* ptr) : ptr(ptr) {}

	T& operator*() const { return *ptr; }
	T* operator->() { return ptr; }

	SEHIterator& operator++()
	{
		ptr++;
		return *this;
	}

	SEHIterator operator++(int)
	{
		SEHIterator temp = *this;
		++(*this);
		return temp;
	}

	friend bool operator==(const SEHIterator& a, const SEHIterator& b) { return a.ptr == b.ptr; };
	friend bool operator!=(const SEHIterator& a, const SEHIterator& b) { return a.ptr != b.ptr; };
};

template <typename T>
class SEHList
{
private:
	QWORD allocAddress;
	size_t m_size = 0, maxSize;

public:
	SEHList(size_t _maxSize) : maxSize(_maxSize)
	{
		allocAddress = reinterpret_cast<QWORD>(VirtualAlloc(NULL, _maxSize * sizeof(T), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
	}

	SEHList(const SEHList& other) : m_size(other.m_size), maxSize(other.maxSize)
	{
		allocAddress = reinterpret_cast<QWORD>(VirtualAlloc(NULL, maxSize * sizeof(T), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		memcpy(reinterpret_cast<void*>(allocAddress), reinterpret_cast<void*>(other.allocAddress), m_size * sizeof(T));
	}

	~SEHList()
	{
		VirtualFree(reinterpret_cast<LPVOID>(allocAddress), maxSize * sizeof(T), MEM_RELEASE);
	}

	T& operator[](size_t index)
	{
		__try
		{
			return *reinterpret_cast<T*>(allocAddress + index * sizeof(T));
		}
		__except (1) { return T{}; }
	}

	operator bool() const
	{
		return allocAddress != 0;
	}

	bool operator==(const SEHList& other) const
	{
		return other.allocAddress == this->allocAddress;
	}

	SEHList& operator=(const SEHList& other)
	{
		if (this != &other)
		{
			VirtualFree(reinterpret_cast<void*>(allocAddress), maxSize * sizeof(T), MEM_RELEASE);
			m_size = other.m_size;
			maxSize = other.maxSize;
			allocAddress = reinterpret_cast<QWORD>(VirtualAlloc(NULL, maxSize * sizeof(T), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
			memcpy(reinterpret_cast<void*>(allocAddress), reinterpret_cast<void*>(other.allocAddress), m_size * sizeof(T));
		}
		return *this;
	}

	size_t size()
	{
		return m_size;
	}

	bool empty() const
	{
		return m_size == 0;
	}
	
	bool add(T value)
	{
		bool exceptionOccur = false;
		__try
		{
			*reinterpret_cast<T*>(allocAddress + m_size * sizeof(T)) = value;
		}
		__except (1) 
		{
			exceptionOccur = true;
		}

		if (!exceptionOccur) m_size++;
		return exceptionOccur;
	}

	bool exchange(size_t index, T value)
	{
		__try
		{
			*reinterpret_cast<T*>(allocAddress + index * sizeof(T)) = value;
		}
		__except (1)
		{
			return false;
		}

		return true;
	}

	bool remove(size_t index)
	{
		bool exceptionOccur = false;
		__try
		{
			*reinterpret_cast<T*>(allocAddress + m_size * sizeof(T)) = 0;
		}
		__except (1)
		{
			exceptionOccur = true;
		}

		if (!exceptionOccur) m_size--;
		return exceptionOccur;
	}

	void clear()
	{
		__try
		{
			for (size_t i = 0; i < m_size; i++)
			{
				*reinterpret_cast<T*>(allocAddress + i * sizeof(T)) = 0;
			}
			m_size = 0;
		}
		__except (1) {}
	}

	bool find(T value)
	{
		for (size_t i = 0; i < m_size; i++)
		{
			if (this[i] == value)
				return true;
		}
		return false;
	}

	SEHIterator<T> begin()
	{
		return SEHIterator<T>(reinterpret_cast<T*>(allocAddress));
	}

	SEHIterator<T> end()
	{
		return SEHIterator<T>(reinterpret_cast<T*>(allocAddress + m_size * sizeof(T)));
	}

	SEHIterator<const T> begin() const
	{
		return SEHIterator<const T>(reinterpret_cast<T*>(allocAddress));
	}

	SEHIterator<const T> end() const
	{
		return SEHIterator<const T>(reinterpret_cast<T*>(allocAddress + m_size * sizeof(T)));
	}
};

#endif