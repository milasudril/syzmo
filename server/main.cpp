#ifdef __WAND__
target[name[syzmo_server.exe] type[application] platform[;Windows]]
target[name[syzmo_server] type[application]]
#endif

#include "server.h"
#include "../buffer.h"
#include "../bridge/server_setup.h"
#include "../exception_missing.h"

#include <windows.h>

int main()
	{
	try
		{
		SyZmO::ServerSetup params;
		params.port_out=49152;
		params.port_in=params.port_out+1;
		params.flags=SyZmO::ServerSetup::STARTUP_BROADCAST;

		SyZmO::load(params);
			
		int status=SyZmO::Server::RUN_STATUS_CONTINUE;
		while(!status)
			{
			SyZmO::Server server(params);
			status=server.run();
			}
		switch(status)
			{
			case SyZmO::Server::RUN_STATUS_SHUTDOWN:
				ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF,0);
				break;
			case SyZmO::Server::RUN_STATUS_REBOOT:
				ExitWindowsEx(EWX_REBOOT,0);
				break;
			}
		}
	catch(const SyZmO::ExceptionMissing& e)
		{
		e.print();
		}
	return 0;
	}
