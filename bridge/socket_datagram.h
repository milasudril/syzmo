#ifdef __WAND__
target[name[socket_datagram.h] type[include]]
dependency[socket_datagram.o]
#endif

#ifndef SYZMO_SOCKETDATAGRAM_H
#define SYZMO_SOCKETDATAGRAM_H

#include "inttypes.h"
#include <cstddef>

namespace SyZmO
	{
	class SocketDatagram
		{
		public:
			static const size_t ADDRBUFF_LENGTH=32;

			SocketDatagram();
			~SocketDatagram();

			void bind(uint16_t port);
			void bind(uint16_t port,const char* address);
			uint16_t receive(void* buffer,uint16_t length);
			uint16_t receive(void* buffer,uint16_t length,char* addr_from);
			uint16_t send(const void* buffer,uint16_t length,uint16_t port
				,const char* address);

			void broadcastEnable();
			void broadcastDisable();

		private:
			class Socket;
			Socket* impl;
		};
	}

#endif
