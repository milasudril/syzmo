#ifdef __WAND__
target[name[syzmo_client_cgi.exe] type[application] platform[;Windows]]
#endif

#include "event_handler.h"
#include "../client/client.h"
#include "../buffer.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

void keywordProcess(SyZmO::Client& client,const char* word)
	{
	if(strcmp(word,"hostname")==0)
		{
		client.serverHostnameRequest("127.0.0.1");
		client.run();
		}
	else
	if(strcmp(word,"devices")==0)
		{
		client.deviceCountRequest("127.0.0.1");
		client.run();
		}
	else
	if(strcmp(word,"setup_get")==0)
		{
		client.serverSetupGetRequest("127.0.0.1");
		client.run();
		}
	else
		{printf("<p class=\"error\">Unknown keyword: <code>%s</code></p>",word);}
	}


int main()
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
		
	SyZmO::ClientCgi::EventHandler eh;
	SyZmO::Client client(params,eh);

//	const char* args=getenv("QUERY_STRING");
	FILE* src=fopen("info.html","r");
	if(src==NULL)
		{
		printf("Content-type: text/plain\r\n\r\nFile not found\r\n");
		return -1;
		}

	printf("Content-type: text/html\r\n\r\n");
	SyZmO::Buffer word(16);

	int ch_in;
	while( (ch_in=getc(src)) != EOF)
		{
		if((ch_in>=0 && ch_in<=32) || ch_in=='>' || ch_in=='<' || ch_in=='='
			|| ch_in=='"' || ch_in=='&' || ch_in==';')
			{
			word.terminate();
			if(*word.begin()=='$')
				{keywordProcess(client,word.begin()+1);}
			else
				{printf("%s",word.begin());}
			putchar(ch_in);
			word.clear();
			}
		else
			{word.append(ch_in);}
		}
	fclose(src);
	return 0;
	}
