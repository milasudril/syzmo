#ifdef __WAND__
target[name[../syzmo_client_cgi.exe] type[application] platform[;Windows]]
target[name[../syzmo_client_cgi] type[application] ]
#endif

#include "keyword_processor.h"
#include "event_handler.h"
#include "querystring.h"
#include "parameters.h"

#include "../client/client.h"
#include "../logfile/logfile_in.h"
#include "../bridge/server_setup.h"
#include "../bridge/message_midi.h"
#include "../buffer.h"
#include "../exception_missing.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

int main()
	{
	try
		{
		if(getenv("QUERY_STRING")==NULL)
			{
			printf("HTTP 1.1/301 Moved Permanently\r\n"
				"Location: http://%s%s?view=info\r\n\r\n"
				,getenv("HTTP_HOST")
				,getenv("SCRIPT_NAME"));
			return 0;
			}

		SyZmO::Client::Parameters params;
		params.port_in=49152;
		params.port_out=49153;
		params.flags=0;
		strcpy(params.server_ip,"127.0.0.1");

			{
			SyZmO::ServerSetup params_server;
			if(SyZmO::load(params_server))
				{
				params.port_in=params_server.port_out;
				params.port_out=params_server.port_in;
				}
			}

		SyZmO::ClientCgi::Parameters params_cgi;
		params_cgi.page=0;
		params_cgi.action=SyZmO::ClientCgi::Parameters::ACTION_NORMAL;
		SyZmO::ClientCgi::load(params_cgi);

		SyZmO::ClientCgi::EventHandler eh;
		SyZmO::Client client(params,eh);

		switch(params_cgi.action)
			{
			case SyZmO::ClientCgi::Parameters::ACTION_NORMAL:
				{
				SyZmO::ClientCgi::KeywordProcessor keyp(client,eh,params_cgi);
				SyZmO::ClientCgi::TemplateReader reader(params_cgi.view.begin(),keyp);
				reader.run();
				}
				return 0;

			case SyZmO::ClientCgi::Parameters::ACTION_SETUP:
				{
				SyZmO::ServerSetup server_setup;
				SyZmO::ClientCgi::QueryString qs;
				SyZmO::load(qs,server_setup);
				client.serverSetupSetRequest(params.server_ip,server_setup);
				client.run();
				printf("HTTP 1.1/301 Moved Permanently\r\n"
					"Location: http://%s%s?view=actions\r\n\r\n"
					,getenv("HTTP_HOST"),getenv("SCRIPT_NAME"));
				}
				return 0;

			case SyZmO::ClientCgi::Parameters::ACTION_SHUTDOWN:
				{
				printf("HTTP 1.1/301 Moved Permanently\r\n"
					"Location: http://%s%s?view=shutdown\r\n\r\n"
					,getenv("HTTP_HOST"),getenv("SCRIPT_NAME"));
				client.serverShutdownRequest(params.server_ip);
				}
				return 0;

			case SyZmO::ClientCgi::Parameters::ACTION_REBOOT:
				{
				printf("HTTP 1.1/301 Moved Permanently\r\n"
					"Location: http://%s%s?view=reboot\r\n\r\n"
					,getenv("HTTP_HOST"),getenv("SCRIPT_NAME"));
				client.serverRebootRequest(params.server_ip);
				}
				return 0;

			case SyZmO::ClientCgi::Parameters::ACTION_TEST:
				{
				SyZmO::uint32_t device_id=params_cgi.device;
				client.serverTestRequest(params.server_ip,device_id);
				client.run();

				printf("HTTP 1.1/301 Moved Permanently\r\n"
					"Location: http://%s\r\n\r\n",getenv("HTTP_HOST"));
				}
				return 0;

			case SyZmO::ClientCgi::Parameters::ACTION_LOGFILE_CLEAR:
				{
				client.serverLogClearRequest(params.server_ip);
				client.run();
				printf("HTTP 1.1/301 Moved Permanently\r\n"
					"Location: http://%s%s?view=actions\r\n\r\n"
					,getenv("HTTP_HOST"),getenv("SCRIPT_NAME"));
				}
				return 0;

			default:
				printf("Content-type: text/plain\r\n\r\nBad Action=%u\n"
					,params_cgi.action);
				return 0;
			}
		}
	catch(const SyZmO::ExceptionMissing& e)
		{
		e.print();
		}

	return 0;
	}
