#ifdef __WAND__
target[name[client.h] type[include]]
dependency[client.o]
#endif

#ifndef SYZMO_CLIENT_H
#define SYZMO_CLIENT_H

#include "../bridge/socket_datagram.h"
#include "../bridge/message_midi.h"

namespace SyZmO
	{
	class EventHandler;
	class ServerSetup;
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

			Client(Parameters& params,EventHandler& eh);
			~Client();

			void messageMidiSend(const char* server,uint32_t device_id
				,MessageMidi msg);
			void isAliveRequest(const char* server);
			void deviceCountRequest(const char* server);
			void deviceNameRequest(const char* server,uint32_t id);
			void connectionOpenRequest(const char* server,uint32_t device_id);
			void connectionCloseRequest(const char* server,uint32_t device_id);
			void serverHostnameRequest(const char* server);
			void serverSetupGetRequest(const char* server);
			void serverSetupSetRequest(const char* server,const ServerSetup& setup);
			void serverShutdownRequest(const char* server);
			void serverRebootRequest(const char* server);
			void serverTestRequest(const char* server,uint32_t device_id);

			static const int RUN_STATUS_CONTINUE=0;
			static const int RUN_STATUS_EXIT=1;

			int run();
			void stop()
				{m_stop=1;}

		private:
			Parameters& m_params;
			SocketDatagram socket_in;
			SocketDatagram socket_out;
			EventHandler& m_handler;
			volatile bool m_stop;
		};
	}

#endif
