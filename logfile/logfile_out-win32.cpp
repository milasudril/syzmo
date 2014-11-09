#ifdef __WAND__
target[name[logfile_out.o] type[object] platform[;Windows]]
#endif

#include "logfile_out.h"
#include "../buffer.h"
#include "../exception_missing.h"
#define _WIN32_IE 0x0400
#include <windows.h>
#include <shlobj.h>
#include <cstdio>

SyZmO::LogfileOut::LogfileOut(const char* filename)
	{
	char homedir[MAX_PATH];
	if(!SHGetSpecialFolderPath(NULL,homedir,CSIDL_APPDATA,TRUE))
		{throw ExceptionMissing(__FILE__,__LINE__);}
	homedir[MAX_PATH-1]=0;

		{
		Buffer temp(MAX_PATH,homedir);
		temp.append('\\').append(filename).append(".txt").terminate();
		dest=fopen(temp.begin(),"ab");
		if(dest==NULL)
			{throw ExceptionMissing(__FILE__,__LINE__);}
		}

		{
		Buffer temp(MAX_PATH,homedir);
		temp.append('\\').append(filename).append(".idx").terminate();
		dest_index=fopen(temp.begin(),"ab");
		if(dest_index==NULL)
			{
			fclose((FILE*)dest);
			throw ExceptionMissing(__FILE__,__LINE__);
			}
		}
	}

SyZmO::LogfileOut::~LogfileOut()
	{
	fclose((FILE*)dest_index);
	fclose((FILE*)dest);
	}

void SyZmO::LogfileOut::entryWrite(const char* address,const char* message,...)
	{
	SYSTEMTIME now;
	GetSystemTime(&now);
	int l_tot=fprintf((FILE*)dest,"%u-%02u-%02u %u:%02u:%02u;"
		,now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond);

	l_tot+=strlen(address)+1;
	fputs(address,(FILE*)dest);
	putc(';',(FILE*)dest);

	va_list args;
	va_start(args,message);
	l_tot+=vfprintf((FILE*)dest,message,args)+1;
	va_end(args);
	putc('\n',(FILE*)dest);
	fflush((FILE*)dest);

	fprintf((FILE*)dest_index,"%d ",l_tot);
	fflush((FILE*)dest_index);
	}
