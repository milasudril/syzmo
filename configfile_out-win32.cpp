#ifdef __WAND__
target
	[
	name[configfile_out.o] type[object] platform[;Windows]
	]
#endif

#include "configfile_out.h"
#include "buffer.h"
#include "exception_missing.h"
#define _WIN32_IE 0x0400
#include <windows.h>
#include <shlobj.h>
#include <cstdio>

SyZmO::ConfigFileOut::ConfigFileOut(const char* filename)
	{
	char homedir[MAX_PATH];
	if(!SHGetSpecialFolderPath(NULL,homedir,CSIDL_APPDATA,TRUE))
		{throw ExceptionMissing(__FILE__,__LINE__);}
	homedir[MAX_PATH-1]=0;

	Buffer temp(MAX_PATH,homedir);
	temp.append('\\').append(filename).terminate();
	dest=fopen(temp.begin(),"w");
	if(dest==NULL)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	}

void SyZmO::ConfigFileOut::paramSet(const char* key,const char* value)
	{
	fprintf((FILE*)dest,"%s: %s\n",key,value);
	}

SyZmO::ConfigFileOut::~ConfigFileOut()
	{
	fclose((FILE*)dest);
	}
