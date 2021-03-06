#ifdef __WAND__
target[name[connection.h] type[include]]
dependency[connection.o]
#endif

#ifndef SYZMO_CONNECTION_H
#define SYZMO_CONNECTION_H

#include "midi_out.h"
#include "../bridge/socket_datagram.h"

namespace SyZmO
	{
	class Server;

	class Connection
		{
		public:
			Connection(const char* client,uint32_t device_id);

			bool clientMatch(const char* client) const;
			void messageSend(const char* client,MessageMidi msg)
				{
				if(clientMatch(client))
					{midi_out.messageSend(msg);}
				}

			bool isAliveRequest(Server& server);

			void isAlive()
				{isalive_sent=0;}
			
			const char* clientNameGet() const
				{return m_client;}

		private:
			char m_client[SocketDatagram::ADDRBUFF_LENGTH];
			MidiOut midi_out;
			bool isalive_sent;
		};
	};

#endif
