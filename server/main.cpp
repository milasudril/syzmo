#ifdef __WAND__
target[name[../syzmo_server.exe] type[application] platform[;Windows]]
target[name[../syzmo_server] type[application]]
#endif

#include "server.h"
#include "../buffer.h"
#include "../bridge/server_setup.h"
#include "../logfile/logfile_out.h"
#include "../exception_missing.h"

#include <windows.h>

int main()
	{
	try
		{
		SyZmO::LogfileOut logfile("syzmo_log_server");
		SyZmO::ServerSetup params;
		params.port_out=49152;
		params.port_in=params.port_out+1;
		params.flags=SyZmO::ServerSetup::STARTUP_BROADCAST;

		SyZmO::load(params);

		int status;
		do
			{
			SyZmO::Server server(params,logfile);
			status=server.run();
			SyZmO::store(params);
			}
		while(status==SyZmO::Server::RUN_STATUS_CONTINUE);

		switch(status)
			{
			case SyZmO::Server::RUN_STATUS_SHUTDOWN:
				Sleep(5000);
				ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF,0);
				break;

			case SyZmO::Server::RUN_STATUS_REBOOT:
				Sleep(5000);
				ExitWindowsEx(EWX_REBOOT,0);
				break;
			}
		}
	catch(const SyZmO::ExceptionMissing& e)
		{
		SyZmO::LogfileOut logfile("syzmo_log_server");
		e.print(logfile);
		}
	return 0;
	}
