#ifdef __WAND__
target
	[
	name[configfile_in.o] type[object] platform[;GNU/Linux]
	]
#endif

#include "configfile_in.h"
#include "buffer.h"
#include "exception_missing.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstdio>

SyZmO::ConfigFileIn::ConfigFileIn(const char* filename)
	{
	struct passwd* pw = getpwuid(getuid());
	const char* homedir = pw->pw_dir;
	if(homedir==nullptr)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	Buffer temp(256,homedir);
	temp.append('/').append(filename).terminate();
	src=fopen(temp.begin(),"r");
	if(src==nullptr)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	}

bool SyZmO::ConfigFileIn::paramGet(Buffer& key,Buffer& value)
	{
	if(feof((FILE*)src))
		{return 0;}
	int ch_in;
	key.clear();
	value.clear();
	int field=0;
	while( (ch_in=getc( (FILE*)src) )!=EOF )
		{
		if(ch_in>=0 && ch_in<=32 && ch_in!=10)
			{continue;}

		switch(field)
			{
			case 0:
				{
				switch(ch_in)
					{
					case '\n':
						key.terminate();
						value.terminate();
						return 1;
					case ':':
						key.terminate();
						++field;
						break;
					default:
						key.append(ch_in);
					}
				}
				break;

			case 1:
				if(ch_in=='\n')
					{
					value.terminate();
					return 1;
					}
				value.append(ch_in);
				break;
			}
		}
	key.terminate();
	value.terminate();
	return 1;
	}

SyZmO::ConfigFileIn::~ConfigFileIn()
	{
	fclose((FILE*)src);
	}
