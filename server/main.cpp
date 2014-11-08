#ifdef __WAND__
target[name[syzmo_server.exe] type[application] platform[;Windows]]
target[name[syzmo_server] type[application]]
#endif

#include "server.h"
#include "../buffer.h"
#include "../bridge/server_setup.h"
#include "../exception_missing.h"

int main()
	{
	try
		{
		SyZmO::ServerSetup params;
		params.port_out=49152;
		params.port_in=params.port_out+1;
		params.flags=SyZmO::ServerSetup::STARTUP_BROADCAST;

		SyZmO::load(params);
			
		int status=0;
		while(!status)
			{
			SyZmO::Server server(params);
			status=server.run();
			}
		}
	catch(const SyZmO::ExceptionMissing& e)
		{
		e.print();
		}
	return 0;
	}
