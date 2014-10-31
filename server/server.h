#ifdef __WAND__
target[name[server.h] type[include]]
dependency[server.o]
#endif

#ifndef SYZMO_SERVER_H
#define SYZMO_SERVER_H

#include "../bridge/inttypes.h"
#include "../bridge/socket_datagram.h"

namespace SyZmO
	{
	class SocketDatagram;

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
			
			int run();

		/*	void devcountRequest();
			void devnameRequest(uint32_t device_id);*/

		private:
			Parameters m_params;
			SocketDatagram socket_in;
			SocketDatagram socket_out;
		};
	}

#endif
