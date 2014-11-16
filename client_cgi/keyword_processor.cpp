#ifdef __WAND__
target[name[keyword_processor.o] type[object]]
#endif

#include "keyword_processor.h"
#include "event_handler.h"
#include "../client/client.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

SyZmO::ClientCgi::KeywordProcessor::KeywordProcessor(Client& client
	,EventHandler& eh,Parameters& params):
	m_client(client),m_eh(eh),m_params(params),logfile(NULL)
	{}

void SyZmO::ClientCgi::KeywordProcessor::keywordProcess(const char* word)
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
/*	else
	if(strcmp(word,"logfile_aquire")==0)
		{
		if(logfile!=NULL)
			{delete logfile;}
		logfile=new SyZmO::LogfileIn("syzmo_log_server");
		}
	else
	if(strcmp(word,"logfile_release")==0)
		{
		if(logfile!=NULL)
			{delete logfile;}
		logfile=NULL;
		}
	else
	if(strcmp(word,"page_prev")==0)
		{
		if(m_params.page>0)
			{printf("%u",m_params.page-1);}
		else
			{printf("%u",0);}
		}
	else
	if(strcmp(word,"page_next")==0)
		{
		printf("<p class=\"error\">Unimplemented keyword: <code>%s</code></p>"
			,word);
		}
	else
	if(strcmp(word,"page_last")==0)
		{
		printf("<p class=\"error\">Unimplemented keyword: <code>%s</code></p>"
			,word);
		}*/
	else
		{
		printf("Unknown keyword: %s",word);
		}
	}
