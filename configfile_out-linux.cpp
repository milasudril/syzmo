#ifdef __WAND__
target
	[
	name[configfile_out.o] type[object] platform[;GNU/Linux]
	]
#endif

#include "configfile_out.h"
#include "buffer.h"
#include "exception_missing.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstdio>

SyZmO::ConfigFileOut::ConfigFileOut(const char* filename)
	{
	struct passwd* pw = getpwuid(getuid());
	const char* homedir = pw->pw_dir;
	if(homedir==nullptr)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	Buffer temp(256,homedir);
	temp.append('/').append(filename).terminate();
	dest=fopen(temp.begin(),"w");
	if(dest==nullptr)
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
