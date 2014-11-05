#ifdef __WAND__
target[name[event_handler.h] type[include]]
dependency[event_handler.o]
#endif

#ifndef SYZMO_CLIENTCGI_EVENTHANDLER_H
#define SYZMO_CLIENTCGI_EVENTHANDLER_H

#include "../client/event_handler.h"

#include <cstddef>

namespace SyZmO
	{
	namespace ClientCgi
		{
		class EventHandler:public SyZmO::EventHandler
			{
			public:
				EventHandler();

				void clientStartup(Client& client,const char* server);
				void clientShutdown(Client& client);

				bool timeout(Client& client);
				bool isAlive(Client& client,const char* server);
				bool deviceCount(Client& client,const char* server
					,const MessageCtrl::DeviceCountResponse& message);
				bool deviceName(Client& client,const char* server
					,const MessageCtrl::DeviceNameResponse& message);
				bool connectionOpened(Client& client,const char* server
					,const MessageCtrl::ConnectionOpenResponsePrivate& message);
				bool connectionOpened(Client& client,const char* server
					,const MessageCtrl::ConnectionOpenResponsePublic& message);
				bool connectionClosed(Client& client,const char* server
					,const MessageCtrl::ConnectionCloseResponsePrivate& message);
				bool connectionClosed(Client& client,const char* server
					,const MessageCtrl::ConnectionCloseResponsePublic& message);

				bool serverStartup(Client& client,const char* server);
				bool serverShutdown(Client& client,const char* server);

			private:
				size_t dev_count;
			};
		}
	}

#endif
