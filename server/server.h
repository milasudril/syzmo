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

	class Server
		{
		public:
			struct Parameters
				{
				uint32_t flags;
				uint16_t port_in;
				uint16_t port_out;

				static const uint32_t STARTUP_BROADCAST=0x1;
				static const uint32_t SHUTDOWN_BROADCAST=0x2;
				};

			Server(const Parameters& params);
			~Server();
			
			void midiMessageSend(const char* client,uint32_t dev_id,MessageMidi msg);
			void deviceCountSend(const char* client);
			void deviceNameSend(const char* client,uint32_t dev_id);
			void clientConnect(const char* client,uint32_t dev_id);
			void clientDisconnect(const char* client,uint32_t dev_id);
			
			int run();

		private:
			Parameters m_params;
			SocketDatagram socket_in;
			SocketDatagram socket_out;
			std::map<uint32_t,Connection> connections;
		};
	}

#endif
