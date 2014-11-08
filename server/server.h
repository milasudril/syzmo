#ifdef __WAND__
target[name[server.h] type[include]]
dependency[server.o]
#endif

#ifndef SYZMO_SERVER_H
#define SYZMO_SERVER_H

#include "../bridge/inttypes.h"
#include "../bridge/socket_datagram.h"
#include "../bridge/message_midi.h"

#include <map>

namespace SyZmO
	{
	class SocketDatagram;
	class Connection;
	class ServerSetup;

	class Server
		{
		public:
			Server(ServerSetup& params);
			~Server();

			void midiMessageSend(const char* client,uint32_t dev_id,MessageMidi msg);
			void isAliveRequest(const char* client);
			void deviceCountSend(const char* client);
			void deviceNameSend(const char* client,uint32_t dev_id);
			void clientConnect(const char* client,uint32_t dev_id);
			void clientDisconnect(const char* client,uint32_t dev_id);

			void hostnameSend(const char* client);
			void setupGetSend(const char* client);
			void setupSet(const char* client,const ServerSetup& setup);


			void connectionsIsAliveRequest();
			void connectionsIsAlive(const char* server);

			int run();
			void stop()
				{running=0;}

		private:
			ServerSetup& m_params;
			SocketDatagram socket_in;
			SocketDatagram socket_out;
			size_t n_devs;
			Connection** connections;
			uint32_t time_activity;
			volatile bool running;
		};
	}

#endif
