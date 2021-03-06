#ifdef __WAND__
target[name[logfile_out.o] type[object] platform[;Windows]]
#endif

#include "logfile_out.h"
#include "../buffer.h"
#include "../exception_missing.h"
#include "../bridge/inttypes.h"
#define _WIN32_IE 0x0400
#include <windows.h>
#include <shlobj.h>
#include <cstdio>

SyZmO::LogfileOut::LogfileOut(const char* filename):
	filename_log(MAX_PATH),filename_index(MAX_PATH)
	{
	char homedir[MAX_PATH];
	if(!SHGetSpecialFolderPath(NULL,homedir,CSIDL_APPDATA,TRUE))
		{throw ExceptionMissing(__FILE__,__LINE__);}
	homedir[MAX_PATH-1]=0;

		{
		filename_log.append(homedir).append('\\').append(filename)
			.append(".txt").terminate();
		dest=fopen(filename_log.begin(),"ab");
		if(dest==NULL)
			{throw ExceptionMissing(__FILE__,__LINE__);}
		}

		{
		filename_index.append(homedir).append('\\').append(filename)
			.append(".idx").terminate();
		dest_index=fopen(filename_index.begin(),"ab");
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
	uint16_t l_tot=fprintf((FILE*)dest,"%u-%02u-%02u %u:%02u:%02u;"
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

	fwrite(&l_tot, sizeof(l_tot),1,(FILE*)dest_index);
	fflush((FILE*)dest_index);
	}

void SyZmO::LogfileOut::clear()
	{
	fclose((FILE*)dest_index);
	fclose((FILE*)dest);

	dest=fopen(filename_log.begin(),"wb");
	if(dest==NULL)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	dest_index=fopen(filename_index.begin(),"wb");
	if(dest_index==NULL)
		{
		fclose((FILE*)dest);
		throw ExceptionMissing(__FILE__,__LINE__);
		}
	}
