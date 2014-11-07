#ifdef __WAND__
target[name[event_handler.o] type[object]]
#endif

#include "event_handler.h"
#include "midi_router.h"
#include "../client/client.h"

SyZmO::ClientStudio::EventHandler::EventHandler(MidiRouter& router):
	m_router(router)
	{}

void SyZmO::ClientStudio::EventHandler::clientStartup(Client& client
	,const char* server)
	{
	client.deviceCountRequest(server);
	}

void SyZmO::ClientStudio::EventHandler::clientShutdown(Client& client)
	{

	}

bool SyZmO::ClientStudio::EventHandler::timeout(Client& client)
	{
	m_router.connectionsIsAliveRequest(client);
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::isAlive(Client& client,const char* server)
	{
	m_router.connectionsIsAlive(server);
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::deviceCount(Client& client
	,const char* server,const MessageCtrl::DeviceCountResponse& message)
	{
	for(size_t k=0;k<message.n_devs;++k)
		{client.deviceNameRequest(server,k);}
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::deviceName(Client& client
	,const char* server,const MessageCtrl::DeviceNameResponse& message)
	{
	if(message.status==MessageCtrl::DeviceNameResponse::STATUS_READY)
		{client.connectionOpenRequest(server,message.device_id);}
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::connectionOpened(Client& client
	,const char* server
	,const MessageCtrl::ConnectionOpenResponsePrivate& message)
	{
	if(message.status==MessageCtrl::ConnectionOpenResponsePrivate::STATUS_OK)
		{m_router.connectionAdd(message.name,server,message.device_id);}
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::connectionOpened(Client& client
	,const char* server
	,const MessageCtrl::ConnectionOpenResponsePublic& message)
	{
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::connectionClosed(Client& client
	,const char* server
	,const MessageCtrl::ConnectionCloseResponsePrivate& message)
	{
	if(message.status==MessageCtrl::ConnectionCloseResponsePrivate::STATUS_OK)
		{m_router.connectionRemove(server,message.device_id);}
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::connectionClosed(Client& client
	,const char* server
	,const MessageCtrl::ConnectionCloseResponsePublic& message)
	{
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::serverStartup(Client& client
	,const char* server)
	{
	client.deviceCountRequest(server);
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::serverShutdown(Client& client
	,const char* server)
	{
	m_router.connectionsRemove(server);
	return 1;
	}



bool SyZmO::ClientStudio::EventHandler::serverHostname(Client& client
	,const char* server	,const MessageCtrl::ServerHostnameResponse& message)
	{
	return 1;
	}

bool SyZmO::ClientStudio::EventHandler::serverSetupGet(Client& client
	,const char* server	,const MessageCtrl::ServerSetupGetResponse& message)
	{
	return 1;
	}
