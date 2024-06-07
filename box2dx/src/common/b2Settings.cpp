#include "./b2Settings.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

b2Version b2_version = {2, 3, 0};

// Memory allocators. Modify these to use your own allocator.
void* b2Alloc(int32 size)
{
	return malloc(size);
}

void b2Free(void* mem)
{
	free(mem);
}

// You can modify this to use your logging facility.
void b2Log(const char* string, ...)
{
	va_list args;
	va_start(args, string);
	vprintf(string, args);
	va_end(args);
}
