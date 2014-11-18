#ifdef __WAND__
target[name[logfile_in.o] type[object] platform[;Windows]]
#endif

#include "logfile_in.h"
#include "../buffer.h"
#include "../exception_missing.h"
#include "../bridge/inttypes.h"

#define _WIN32_IE 0x0400
#include <windows.h>
#include <shlobj.h>

#include <cstdlib>
#include <cstdio>

SyZmO::LogfileIn::LogfileIn(const char* filename)
	{
	char homedir[MAX_PATH];
	if(!SHGetSpecialFolderPath(NULL,homedir,CSIDL_APPDATA,TRUE))
		{throw ExceptionMissing(__FILE__,__LINE__);}
	homedir[MAX_PATH-1]=0;

		{
		Buffer temp(MAX_PATH,homedir);
		temp.append('\\').append(filename).append(".txt").terminate();
		src=fopen(temp.begin(),"rb");
		if(src==NULL)
			{throw ExceptionMissing(__FILE__,__LINE__);}
		}

		{
		Buffer temp(MAX_PATH,homedir);
		temp.append('\\').append(filename).append(".idx").terminate();

		HANDLE h=CreateFile(temp.begin(),GENERIC_READ
			,FILE_SHARE_READ|FILE_SHARE_WRITE
			,NULL,OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
		if(h==INVALID_HANDLE_VALUE)
			{
			fclose((FILE*)src);
			throw ExceptionMissing(__FILE__,__LINE__);
			}

		record_count=GetFileSize(h,NULL)/sizeof(uint16_t);
		if(record_count>1048576)
			{
			fclose((FILE*)src);
			CloseHandle(h);
			throw ExceptionMissing(__FILE__,__LINE__);
			}
		if(record_count==0)
			{
			CloseHandle(h);
			return;
			}

		record_index=(size_t*)malloc(record_count*sizeof(size_t));
		if(record_index==NULL)
			{
			fclose((FILE*)src);
			CloseHandle(h);
			throw ExceptionMissing(__FILE__,__LINE__);
			}

		uint16_t buffer[4096];
		DWORD N=4096*sizeof(uint16_t);
		DWORD n;
		size_t i=1;
		record_index[0]=0;
		do
			{
			ReadFile(h,buffer,N,&n,NULL);
			for(size_t j=0;j<n/sizeof(uint16_t) && i!=record_count;++j)
				{
				record_index[i]=buffer[j]+record_index[i-1];
				++i;
				}
			}
		while(n==N);
		CloseHandle(h);
		}
	}

SyZmO::LogfileIn::~LogfileIn()
	{
	free(record_index);
	fclose((FILE*)src);
	}

void SyZmO::LogfileIn::recordGet(size_t index,FieldPrinter& printer)
	{
	size_t pos=record_index[index];
	fseek((FILE*)src,pos,SEEK_SET);
	int ch_in;
	Buffer data(64);
	while((ch_in=getc((FILE*)src))!=EOF)
		{
		switch(ch_in)
			{
			case '\n':
				data.terminate();
				printer.fieldPrint(data.begin());
				data.clear();
				return;
			case ';':
				data.terminate();
				printer.fieldPrint(data.begin());
				data.clear();
				break;
			default:
				data.append(ch_in);
			}
		}
	data.terminate();
	printer.fieldPrint(data.begin());
	}
