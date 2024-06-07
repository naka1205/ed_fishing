#ifndef B2_GROWABLE_STACK_H
#define B2_GROWABLE_STACK_H

#include "./b2Settings.h"

#include <memory.h>
#include <string.h>

/// This is a growable LIFO stack with an initial capacity of N.
/// If the stack size exceeds the initial capacity, the heap is used
/// to increase the size of the stack.
template <typename T, int32 N>
class b2GrowableStack
{
public:
	b2GrowableStack()
	{
		m_stack = m_array;
		m_count = 0;
		m_capacity = N;
	}

	~b2GrowableStack()
	{
		if (m_stack != m_array)
		{
			b2Free(m_stack);
			m_stack = NULL;
		}
	}

	void Push(const T& element)
	{
		if (m_count == m_capacity)
		{
			T* old = m_stack;
			m_capacity *= 2;
			m_stack = (T*)b2Alloc(m_capacity * sizeof(T));
			memcpy(m_stack, old, m_count * sizeof(T));
			if (old != m_array)
			{
				b2Free(old);
			}
		}

		m_stack[m_count] = element;
		++m_count;
	}

	T Pop()
	{
		b2Assert(m_count > 0);
		--m_count;
		return m_stack[m_count];
	}

	int32 GetCount()
	{
		return m_count;
	}

private:
	T* m_stack;
	T m_array[N];
	int32 m_count;
	int32 m_capacity;
};


#endif
