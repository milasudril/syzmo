#ifdef __WAND__
target[name[logfile_in.o] type[object] platform[;Windows]]
#endif

#include "logfile_in.h"
#include "../buffer.h"
#include "../exception_missing.h"

#define _WIN32_IE 0x0400
#include <windows.h>
#include <shlobj.h>

#include <cstdio>

SyZmO::LogfileIn::LogfileIn(const char* filename,FieldPrinter& printer):
	m_printer(printer)
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
	//	read index file...	
		}
	}

SyZmO::LogfileIn::~LogfileIn()
	{
	fclose((FILE*)src);
	}
