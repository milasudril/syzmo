#ifdef __WAND__
target[name[log_fieldprinter.o] type[object]]
#endif

#include "log_fieldprinter.h"
#include <cstdio>

void SyZmO::ClientCgi::LogFieldprinter::tableBegin()
	{
	printf("<table>\n");
	}

void SyZmO::ClientCgi::LogFieldprinter::tableEnd()
	{
	printf("</table>\n");
	}

void SyZmO::ClientCgi::LogFieldprinter::rowBegin()
	{
	printf("<tr>");
	}

void SyZmO::ClientCgi::LogFieldprinter::rowEnd()
	{
	printf("</tr>\n");
	}

void SyZmO::ClientCgi::LogFieldprinter::fieldPrint(const char* value)
	{
	printf("<td>%s</td>",value);
	}

void SyZmO::ClientCgi::LogFieldprinter::fieldPrint(const char* value
	,const char* classname)
	{
	printf("<td class=\"%s\">%s</td>",classname,value);
	}

void SyZmO::ClientCgi::LogFieldprinter::fieldHeaderPrint(const char* value)
	{
	printf("<th>%s</th>",value);
	}
