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
			params.view.terminate();
			}
		else
		if(strcmp(key.begin(),"page")==0)
			{params.page=atol(value.begin());}
		else
		if(strcmp(key.begin(),"device")==0)
			{params.device=atol(value.begin());}
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
			else
			if(strcmp(value.begin(),"Clear+logfile")==0 ||
				strcmp(value.begin(),"Clear%20logfile")==0)
				{params.action=Parameters::ACTION_LOGFILE_CLEAR;}

			}
		}
	}
