#ifdef __WAND__
target[name[buffer.o] type[object]]
#endif

#include "buffer.h"
#include <cstdlib>

SyZmO::Buffer::Buffer(size_t length_init)
	{
	ptr_begin=(char*)malloc(length_init*sizeof(char));
	ptr_end=ptr_begin;
	length_alloc=length_init;
	}

SyZmO::Buffer::~Buffer()
	{
	free(ptr_begin);
	}

void SyZmO::Buffer::alloc()
	{
	size_t length_new=2*length();
	size_t length_old=length();
	ptr_begin=(char*)realloc(ptr_begin,sizeof(char*)*length_new);
	ptr_end=ptr_begin+length_old;
	}
