#ifdef __WAND__
target[name[querystring.o] type[object]]
#endif

#include "querystring.h"
#include "../exception_missing.h"
#include "../buffer.h"
#include <cstdlib>

SyZmO::ClientCgi::QueryString::QueryString()
	{
	ptr=getenv("QUERY_STRING");
	if(ptr==NULL)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	}

bool SyZmO::ClientCgi::QueryString::paramGet(Buffer& key,Buffer& value)
	{
	if(*ptr=='\0')
		{return 0;}
	int field=0;
	key.clear();
	value.clear();
	do
		{
		char ch_in=*ptr;
		++ptr;
		switch(field)
			{
			case 0:
				{
				switch(ch_in)
					{
					case '&':
						key.terminate();
						value.terminate();
						return 1;
					case '=':
						key.terminate();
						++field;
						break;
					default:
						key.append(ch_in);
					}
				}
				break;

			case 1:
				if(ch_in=='&')
					{
					value.terminate();
					return 1;
					}
				value.append(ch_in);
				break;
			}
		}
	while(*ptr!='\0');
	key.terminate();
	value.terminate();
	return 1;
	}
