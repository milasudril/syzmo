#ifdef __WAND__
target[name[event_handler.h] type[include]]
#endif

#ifndef SYZMO_EVENTHANDLER_H
#define SYZMO_EVENTHANDLER_H

#include "../bridge/message_ctrl.h"

namespace SyZmO
	{
	class Client;

	class EventHandler
		{
		public:
			virtual void clientStartup(Client& client,const char* server)=0;
			virtual void clientShutdown(Client& client)=0;

			virtual bool timeout(Client& client)=0;
			virtual bool isAlive(Client& client,const char* server)=0;

			virtual bool deviceCount(Client& client,const char* server
				,const MessageCtrl::DeviceCountResponse& message)=0;
			virtual bool deviceName(Client& client,const char* server
				,const MessageCtrl::DeviceNameResponse& message)=0;
			virtual bool connectionOpened(Client& client,const char* server
				,const MessageCtrl::ConnectionOpenResponsePrivate& message)=0;
			virtual bool connectionOpened(Client& client,const char* server
				,const MessageCtrl::ConnectionOpenResponsePublic& message)=0;
			virtual bool connectionClosed(Client& client,const char* server
				,const MessageCtrl::ConnectionCloseResponsePrivate& message)=0;
			virtual bool connectionClosed(Client& client,const char* server
				,const MessageCtrl::ConnectionCloseResponsePublic& message)=0;

			virtual bool serverHostname(Client& client,const char* server
				,const MessageCtrl::ServerHostnameResponse& message)=0;
			virtual bool serverSetupGet(Client& client,const char* server
				,const MessageCtrl::ServerSetupGetResponse& message)=0;
			virtual bool serverSetupSet(Client& client,const char* server
				,const MessageCtrl::ServerSetupSetResponse& message)=0;
			virtual bool serverTest(Client& client,const char* server
				,const MessageCtrl::ServerTestResponse& message)=0;


			virtual bool serverStartup(Client& client,const char* server)=0;
			virtual bool serverShutdown(Client& client,const char* server)=0;


		};
	}

#endif
