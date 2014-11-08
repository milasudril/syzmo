#ifdef __WAND__
target[name[template_reader.o] type[object]]
#endif

#include "template_reader.h"
#include "../exception_missing.h"
#include "../buffer.h"
#include <cstdio>

SyZmO::ClientCgi::TemplateReader::TemplateReader(const char* filename
	,KeywordProcessor& keyp):m_keyp(keyp)
	{
	handle=fopen(filename,"r");
	if(handle==NULL)
		{
		printf("Content-type: text/plain\r\n\r\n");
		throw ExceptionMissing(__FILE__,__LINE__);
		}
	printf("Content-type: text/html\r\n\r\n");
	}

SyZmO::ClientCgi::TemplateReader::~TemplateReader()
	{
	fclose((FILE*)handle);
	}

int SyZmO::ClientCgi::TemplateReader::run()
	{
	SyZmO::Buffer word(16);
	FILE* src=(FILE*)handle;
	int ch_in;
	while( (ch_in=getc(src)) != EOF)
		{
		if((ch_in>=0 && ch_in<=32) || ch_in=='>' || ch_in=='<' || ch_in=='='
			|| ch_in=='"' || ch_in=='&' || ch_in==';' || ch_in=='?')
			{
			word.terminate();
			if(*word.begin()=='$')
				{m_keyp.keywordProcess(word.begin()+1);}
			else
				{printf("%s",word.begin());}
			putchar(ch_in);
			word.clear();
			}
		else
			{word.append(ch_in);}
		}
	return 0;
	}
