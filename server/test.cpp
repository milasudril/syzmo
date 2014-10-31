#ifdef __WAND__
target[name[syzmo_server.exe] type[application] platform[;Windows]]
target[name[syzmo_server] type[application]]
#endif

#include "server.h"
#include <cstdio>

int main()
	{
	SyZmO::Server::Parameters params;
	params.port_out=49152;
	params.port_in=params.port_out+1;
	params.flags=SyZmO::Server::Parameters::STARTUP_BROADCAST
		|SyZmO::Server::Parameters::SHUTDOWN_BROADCAST;

	SyZmO::Server server(params);
	server.run();
	return 0;
	}
