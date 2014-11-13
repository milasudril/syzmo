#ifdef __WAND__
target[name[server_setup.o] type[object]]
#endif

#include "server_setup.h"
#include "../buffer.h"
#include "../configfile/configfile_in.h"
#include "../configfile/configfile_out.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

bool SyZmO::load(ServerSetup& params)
	{
	if(ConfigFileIn::exists("syzmo_config_server.txt"))
		{
		ConfigFileIn config("syzmo_config_server.txt");
		load(config,params);
		return 1;
		}
	return 0;
	}

void SyZmO::store(const ServerSetup& params)
	{
	ConfigFileOut config("syzmo_config_server.txt");
	store(config,params);
	}

void SyZmO::store(ParamWriter& writer,const ServerSetup& params)
	{
	char num[16];
	sprintf(num,"%u",params.port_in);
	writer.paramSet("port_in",num);
	sprintf(num,"%u",params.port_out);
	writer.paramSet("port_out",num);

	if(params.flags&ServerSetup::STARTUP_BROADCAST)
		{writer.paramSet("startup_broadcast","yes");}
	else
		{writer.paramSet("startup_broadcast","no");}
	}

void SyZmO::load(ParamReader& reader,ServerSetup& params)
	{
	SyZmO::Buffer key(16);
	SyZmO::Buffer value(16);
	params.flags=0;
	while(reader.paramGet(key,value))
		{
		if(strcmp(key.begin(),"port_in")==0)
			{params.port_in=atol(value.begin());}
		else
		if(strcmp(key.begin(),"port_out")==0)
			{params.port_out=atol(value.begin());}
		else
		if(strcmp(key.begin(),"startup_broadcast")==0)
			{
			if(strcmp(value.begin(),"yes")==0)
				{params.flags|=ServerSetup::STARTUP_BROADCAST;}
			else
			if(strcmp(value.begin(),"no")==0)
				{params.flags&=~ServerSetup::STARTUP_BROADCAST;}
			}
		}
	}
