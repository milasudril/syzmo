#ifdef __WAND__
target[name[client.h] type[include]]
dependency[client.o]
#endif

#ifndef SYZMO_CLIENT_H
#define SYZMO_CLIENT_H

#include "../bridge/socket_datagram.h"

namespace SyZmO
	{
	class Client
		{
		public:
			struct Parameters
				{
				uint16_t port_in;
				uint16_t port_out;
				};

			Client(const Parameters& params);
			~Client();
			
			int run();

		private:
			Parameters m_params;
			SocketDatagram socket_in;
			SocketDatagram socket_out;

		};
	}

#endif
