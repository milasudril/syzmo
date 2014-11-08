#ifdef __WAND__
target[name[exception_missing.o] type[object]]
#endif

#include "exception_missing.h"
#include <cstdio>
#include <cstring>

SyZmO::ExceptionMissing::ExceptionMissing(const char* file,unsigned int line)
	{
	m_line=line;
	memset(m_file,0,1020*sizeof(char));
	strncpy(m_file,file,1020);
	m_file[1019]=0;
	}

void SyZmO::ExceptionMissing::print() const
	{
	printf("%s:%u: Unknown error\n",m_file,m_line);
	}
