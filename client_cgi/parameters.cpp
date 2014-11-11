#ifdef __WAND__
target[name[parameters.o] type[object]]
#endif

#include "parameters.h"
#include "querystring.h"
#include <cstring>
#include <cstdlib>

void SyZmO::ClientCgi::load(Parameters& params)
	{
	SyZmO::ClientCgi::QueryString args;
	SyZmO::Buffer key(16);
	SyZmO::Buffer value(16);
	while(args.paramGet(key,value))
		{
		if(strcmp(key.begin(),"view")==0)
			{
			params.view.clear();
			params.view.append(value.begin()).append(".html");
			}
		else
		if(strcmp(key.begin(),"record_begin")==0)
			{
			params.record_begin=atol(value.begin());
			}
		else
		if(strcmp(key.begin(),"record_end")==0)
			{
			params.record_end=atol(value.begin());
			}
		else
		if(strcmp(key.begin(),"device")==0)
			{
			params.device=atol(value.begin());
			}
		else
		if(strcmp(key.begin(),"action")==0)
			{
			if(strcmp(value.begin(),"Normal")==0)
				{params.action=Parameters::ACTION_NORMAL;}
			else
			if(strcmp(value.begin(),"Setup")==0)
				{params.action=Parameters::ACTION_SETUP;}
			else
			if(strcmp(value.begin(),"Shutdown")==0)
				{params.action=Parameters::ACTION_SHUTDOWN;}
			else
			if(strcmp(value.begin(),"Reboot")==0)
				{params.action=Parameters::ACTION_REBOOT;}
			else
			if(strcmp(value.begin(),"Test")==0)
				{params.action=Parameters::ACTION_TEST;}
			}
		}
	}
