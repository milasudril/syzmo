#ifdef __WAND__
target[name[server_setup.o] type[object]]
#endif

#include "server_setup.h"
#include "../configfile.h"
#include "../buffer.h"
#include <cstring>
#include <cstdlib>

void SyZmO::load(SyZmO::ServerSetup& params)
	{
	SyZmO::ConfigFile config("syzmo_config_server.txt");
	SyZmO::Buffer key(16);
	SyZmO::Buffer value(16);
	while(config.paramGet(key,value))
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
				{params.flags|=SyZmO::ServerSetup::STARTUP_BROADCAST;}
			else
			if(strcmp(value.begin(),"no")==0)
				{params.flags&=~SyZmO::ServerSetup::STARTUP_BROADCAST;}
			}
		}
	}
