#ifdef __WAND__
target[name[syzmo_server.exe] type[application] platform[;Windows]]
target[name[syzmo_server] type[application]]
#endif

#include "server.h"
//#include "../configfile.h"

int main()
	{
	SyZmO::ServerSetup params;
	
	params.port_out=49152;
	params.port_in=params.port_out+1;
	params.flags=SyZmO::ServerSetup::STARTUP_BROADCAST;

	SyZmO::Server server(params);
	server.run();
	return 0;
	}
