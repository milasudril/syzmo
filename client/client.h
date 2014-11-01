#ifdef __WAND__
target[name[client.h] type[include]]
dependency[client.o]
#endif

#ifndef SYZMO_CLIENT_H
#define SYZMO_CLIENT_H

#include "../bridge/socket_datagram.h"

namespace SyZmO
	{
	class EventHandler;
	
	class Client
		{
		public:
			struct Parameters
				{
				uint16_t port_in;
				uint16_t port_out;
				uint32_t flags;
				char server_ip[SocketDatagram::ADDRBUFF_LENGTH];
				static const uint32_t SERVER_ANY=0x1;
				};

			Client(const Parameters& params,EventHandler& eh);
			~Client();
			
			void deviceCountRequest(const char* server);
			void deviceNameRequest(const char* server,uint32_t id);
				
			int run();

		private:
			Parameters m_params;
			SocketDatagram socket_in;
			SocketDatagram socket_out;
			EventHandler* m_handler;
		};
	}

#endif
