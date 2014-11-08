#ifdef __WAND__
target[name[syzmo_client_cgi.exe] type[application] platform[;Windows]]
#endif

#include "event_handler.h"
#include "parameters.h"
#include "template_reader.h"
#include "querystring.h"

#include "../client/client.h"
#include "../bridge/server_setup.h"
#include "../buffer.h"
#include "../exception_missing.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

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
		params_cgi.setup_set=0;
		SyZmO::ClientCgi::load(params_cgi);
		
		SyZmO::ClientCgi::EventHandler eh;
		SyZmO::Client client(params,eh);	
		
		if(params_cgi.setup_set)
			{
			SyZmO::ServerSetup server_setup;
			SyZmO::ClientCgi::QueryString qs;
			SyZmO::load(qs,server_setup);
			client.serverSetupSetRequest(params.server_ip,server_setup);
			client.run();
			printf("HTTP 1.1/301 Moved Permanently\r\n"
				"Location: http://%s\r\n\r\n",getenv("HTTP_HOST"));
			return 0;
			}
	
		TemplateKeywordProcessor keyp(client,eh,params_cgi);
		SyZmO::ClientCgi::TemplateReader reader(params_cgi.view.begin(),keyp);
		reader.run();
		}
	catch(const SyZmO::ExceptionMissing& e)
		{
		e.print();
		}

	return 0;
	}
