#ifdef __WAND__
target[name[../syzmo_client_cgi.exe] type[application] platform[;Windows]]
target[name[../syzmo_client_cgi] type[application] ]
#endif

#include "event_handler.h"
#include "parameters.h"
#include "template_reader.h"
#include "querystring.h"

#include "../client/client.h"
#include "../bridge/server_setup.h"
#include "../bridge/message_midi.h"
#include "../buffer.h"
#include "../exception_missing.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <windows.h>

class TemplateKeywordProcessor:
	public SyZmO::ClientCgi::TemplateReader::KeywordProcessor
	{
	public:
		TemplateKeywordProcessor(SyZmO::Client& client
			,SyZmO::ClientCgi::EventHandler& eh
			,SyZmO::ClientCgi::Parameters& params):
			m_client(client),m_eh(eh),m_params(params)
			{}

		void keywordProcess(const char* word)
			{
			if(*word=='!')
				{printf("%s",getenv(word+1));}
			else
			if(strcmp(word,"cdata_begin")==0)
				{m_eh.cdataBegin();}
			else
			if(strcmp(word,"cdata_end")==0)
				{m_eh.cdataEnd();}
			else
			if(strcmp(word,"input_begin")==0)
				{m_eh.inputBegin();}
			else
			if(strcmp(word,"input_end")==0)
				{m_eh.inputEnd();}
			else
			if(strcmp(word,"hostname")==0)
				{
				m_client.serverHostnameRequest("127.0.0.1");
				m_client.run();
				}
			else
			if(strcmp(word,"devices")==0)
				{
				m_client.deviceCountRequest("127.0.0.1");
				m_client.run();
				}
			else
			if(strcmp(word,"setup_get")==0)
				{
				m_client.serverSetupGetRequest("127.0.0.1");
				m_client.run();
				}
			else
				{
				printf("<p class=\"error\">Unknown keyword: <code>%s</code></p>"
					,word);
				}
			}

	private:
		SyZmO::Client& m_client;
		SyZmO::ClientCgi::EventHandler& m_eh;
		SyZmO::ClientCgi::Parameters& m_params;
	};

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
			SyZmO::load(params_server);
			params.port_in=params_server.port_out;
			params.port_out=params_server.port_in;
			}

		SyZmO::ClientCgi::Parameters params_cgi;
		params_cgi.record_begin=0;
		params_cgi.record_end=128;
		params_cgi.action=SyZmO::ClientCgi::Parameters::ACTION_NORMAL;
		SyZmO::ClientCgi::load(params_cgi);

		SyZmO::ClientCgi::EventHandler eh;
		SyZmO::Client client(params,eh);

		switch(params_cgi.action)
			{
			case SyZmO::ClientCgi::Parameters::ACTION_NORMAL:
				{
				TemplateKeywordProcessor keyp(client,eh,params_cgi);
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
					"Location: http://%s\r\n\r\n",getenv("HTTP_HOST"));
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
				SyZmO::MessageMidi msg;
				msg.bytes[0]=0xc0;
				msg.bytes[1]=81;
				msg.bytes[2]=0;
				client.messageMidiSend(params.server_ip,device_id,msg);
				msg.bytes[0]=0xb0;
				msg.bytes[1]=7;
				msg.bytes[2]=127;
				client.messageMidiSend(params.server_ip,device_id,msg);
				msg.bytes[0]=0x90;
				msg.bytes[1]=63;
				msg.bytes[2]=127;
				Sleep(500);
				client.messageMidiSend(params.server_ip,device_id,msg);
				msg.bytes[0]=0x80;
				msg.bytes[1]=63;
				msg.bytes[2]=127;
				client.messageMidiSend(params.server_ip,device_id,msg);

				printf("HTTP 1.1/301 Moved Permanently\r\n"
					"Location: http://%s\r\n\r\n",getenv("HTTP_HOST"));
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
