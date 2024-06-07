#ifndef B2_STACK_ALLOCATOR_H
#define B2_STACK_ALLOCATOR_H

#include "./b2Settings.h"

const int32 b2_stackSize = 100 * 1024;	// 100k
const int32 b2_maxStackEntries = 32;

struct b2StackEntry
{
	char* data;
	int32 size;
	bool usedMalloc;
};

class b2StackAllocator
{
public:
	b2StackAllocator();
	~b2StackAllocator();

	void* Allocate(int32 size);
	void Free(void* p);

	int32 GetMaxAllocation() const;

private:

	char m_data[b2_stackSize];
	int32 m_index;

	int32 m_allocation;
	int32 m_maxAllocation;

	b2StackEntry m_entries[b2_maxStackEntries];
	int32 m_entryCount;
};

#endif
