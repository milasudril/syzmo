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
	class LogfileOut;

	class Server
		{
		public:
			Server(ServerSetup& params,LogfileOut& logfile);
			~Server();

			void midiMessageSend(const char* client,uint32_t dev_id
				,const MessageMidi* msg,size_t message_count);
			void isAliveRequest(const char* client);
			void deviceCountSend(const char* client);
			void deviceNameSend(const char* client,uint32_t dev_id);
			void clientConnect(const char* client,uint32_t dev_id);
			void clientDisconnect(const char* client,uint32_t dev_id);

			void hostnameSend(const char* client);
			void setupGetSend(const char* client);
			void setupSet(const char* client,const ServerSetup& setup);
			void deviceTest(const char* client, uint32_t dev_id);
			void logClear(const char* client);


			void connectionsIsAliveRequest();
			void connectionsIsAlive(const char* server);

			static const int RUN_STATUS_CONTINUE=0;
			static const int RUN_STATUS_SHUTDOWN=1;
			static const int RUN_STATUS_REBOOT=2;

			int run();

			void stop()
				{running=0;}



		private:
			ServerSetup& m_params;
			LogfileOut& m_log;
			SocketDatagram socket_in;
			SocketDatagram socket_out;
			size_t n_devs;
			Connection** connections;
			uint32_t time_activity;
			volatile bool running;
		};
	}

#endif
