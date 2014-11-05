#ifdef __WAND__
target[name[event_handler.o] type[object]]
#endif

#include "event_handler.h"
#include "../client/client.h"

#include <cstdio>
#include <algorithm>


SyZmO::ClientCgi::EventHandler::EventHandler()
	{}

void SyZmO::ClientCgi::EventHandler::clientStartup(Client& client
	,const char* server)
	{}

void SyZmO::ClientCgi::EventHandler::clientShutdown(Client& client)
	{}

bool SyZmO::ClientCgi::EventHandler::timeout(Client& client)
	{
	printf("<p class=\"error\">Server did not respond</p>");
	return 0;
	}

bool SyZmO::ClientCgi::EventHandler::isAlive(Client& client,const char* server)
	{return 1;}

bool SyZmO::ClientCgi::EventHandler::deviceCount(Client& client
	,const char* server,const MessageCtrl::DeviceCountResponse& message)
	{
	printf("<table>\n"
		"<tr><th>Id</th><th>Name</th><th>Status</th></tr>\n");
	dev_count=message.n_devs;
	for(size_t k=0;k<message.n_devs;++k)
		{client.deviceNameRequest(server,k);}
	return 1;
	}

namespace
	{
	static const char* dev_status[]={"Ready","Busy","Invalid id","Unknown"};
	}

bool SyZmO::ClientCgi::EventHandler::deviceName(Client& client
	,const char* server,const MessageCtrl::DeviceNameResponse& message)
	{
	printf("<tr><td class=\"number\">%u</td><td>%s</td><td>%s</td></tr>\n"
		,message.device_id,message.name,dev_status[std::min(message.status,3u)]);

	if(message.device_id==dev_count-1)
		{
		printf("</table>\n");
		return 0;
		}
	return 1;
	}

bool SyZmO::ClientCgi::EventHandler::connectionOpened(Client& client
	,const char* server
	,const MessageCtrl::ConnectionOpenResponsePrivate& message)
	{
	return 1;
	}

bool SyZmO::ClientCgi::EventHandler::connectionOpened(Client& client
	,const char* server
	,const MessageCtrl::ConnectionOpenResponsePublic& message)
	{
	return 1;
	}

bool SyZmO::ClientCgi::EventHandler::connectionClosed(Client& client
	,const char* server
	,const MessageCtrl::ConnectionCloseResponsePrivate& message)
	{
	return 1;
	}

bool SyZmO::ClientCgi::EventHandler::connectionClosed(Client& client
	,const char* server
	,const MessageCtrl::ConnectionCloseResponsePublic& message)
	{
	return 1;
	}

bool SyZmO::ClientCgi::EventHandler::serverStartup(Client& client
	,const char* server)
	{
	return 1;
	}

bool SyZmO::ClientCgi::EventHandler::serverShutdown(Client& client
	,const char* server)
	{
	return 1;
	}