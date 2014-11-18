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

				bool serverHostname(Client& client,const char* server
					,const MessageCtrl::ServerHostnameResponse& message);
				bool serverSetupGet(Client& client,const char* server
					,const MessageCtrl::ServerSetupGetResponse& message);
				bool serverSetupSet(Client& client,const char* server
					,const MessageCtrl::ServerSetupSetResponse& message);
				bool serverTest(Client& client,const char* server
					,const MessageCtrl::ServerTestResponse& message);
				bool serverLogCleared(Client& client,const char* server
					,const MessageCtrl::ServerLogClearResponse& message);



				bool serverStartup(Client& client,const char* server);
				bool serverShutdown(Client& client,const char* server);

				void cdataBegin()
					{mode_output|=MODE_OUTPUT_CDATA;}
				void cdataEnd()
					{mode_output&= ~MODE_OUTPUT_CDATA;}
				void inputBegin()
					{mode_output|=MODE_OUTPUT_INPUT;}
				void inputEnd()
					{mode_output&= ~MODE_OUTPUT_INPUT;}

			private:
				size_t dev_count;
				uint32_t mode_output;
				static const uint32_t MODE_OUTPUT_CDATA=1;
				static const uint32_t MODE_OUTPUT_INPUT=2;
			};
		}
	}

#endif
