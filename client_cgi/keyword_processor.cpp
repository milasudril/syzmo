#ifdef __WAND__
target[name[keyword_processor.o] type[object]]
#endif

#include "keyword_processor.h"
#include "event_handler.h"
#include "parameters.h"
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
	else
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
	if(strcmp(word,"logfile_print")==0)
		{
		log_fieldprinter.tableBegin();
		log_fieldprinter.rowBegin();
		log_fieldprinter.fieldHeaderPrint("#");
		log_fieldprinter.fieldHeaderPrint("Time");
		log_fieldprinter.fieldHeaderPrint("Event source");
		log_fieldprinter.fieldHeaderPrint("Message");
		log_fieldprinter.rowEnd();
		size_t n_pages=logfile->recordsCount()/64 + 1;
		if(m_params.page>=n_pages)
			{m_params.page=n_pages-1;}
		size_t index_start=logfile->recordsCount() - m_params.page*64;
		size_t i=index_start;
		while(i!=0)
			{
			log_fieldprinter.rowBegin();
			--i;
			char buffer[16];
			sprintf(buffer,"%u",i);
			log_fieldprinter.fieldPrint(buffer);
			logfile->recordGet(i,log_fieldprinter);
			log_fieldprinter.rowEnd();
			if(index_start-i==64)
				{break;}
			}
		log_fieldprinter.tableEnd();
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
		size_t n_pages=logfile->recordsCount()/64 + 1;
		if(m_params.page+1<n_pages)
			{printf("%u",m_params.page+1);}
		else
			{printf("%u",m_params.page);}
		}
	else
	if(strcmp(word,"page_last")==0)
		{
		size_t n_pages=logfile->recordsCount()/64 + 1;
		printf("%u",n_pages-1);
		}
	else
		{
		printf("Unknown keyword: %s",word);
		}
	}
