#ifdef __WAND__
target[name[event_handler.h] type[include]]
dependency[event_handler.o]
#endif

#ifndef SYZMO_CLIENTSTUDIO_EVENTHANDLER_H
#define SYZMO_CLIENTSTUDIO_EVENTHANDLER_H

#include "../client/event_handler.h"

namespace SyZmO
	{
	namespace ClientStudio
		{
		class MidiRouter;

		class EventHandler:public SyZmO::EventHandler
			{
			public:
				EventHandler(MidiRouter& router);

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

				bool serverStartup(Client& client,const char* server);
				bool serverShutdown(Client& client,const char* server);

			private:
				MidiRouter& m_router;
			};
		}
	}

#endif
