#ifdef __WAND__
target[name[server.o] type[object]]
#endif

#include "server.h"
#include "midi_out.h"
#include "../bridge/socket_datagram.h"
#include "../bridge/message_ctrl.h"

SyZmO::Server::Server(const Parameters& params):
	m_params(params)
	{
	socket_in.bind(m_params.port_in);
	if(m_params.flags&Parameters::STARTUP_BROADCAST)
		{
		socket_out.broadcastEnable();
		MessageCtrl::ServerStarted msg_startup;
		MessageCtrl msg(msg_startup);
		socket_out.send(&msg,sizeof(msg),m_params.port_out,"255.255.255.255");
		socket_out.broadcastDisable();
		}
	}

int SyZmO::Server::run()
	{
	SyZmO::MessageCtrl msg;
	char source[SyZmO::SocketDatagram::ADDRBUFF_LENGTH];
	while(socket_in.receive(&msg,sizeof(msg),source)==sizeof(msg))
		{
		switch(msg.message_type)
			{
			case MessageCtrl::IsAlive::ID:
				{
				MessageCtrl::NoOp nop;
				MessageCtrl msg_ret(nop);
				socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
				}
				break;
				
			case MessageCtrl::DeviceCountRequest::ID:
				{
				MessageCtrl::DeviceCountResponse resp;
				resp.n_devs=MidiOut::deviceCount();
				MessageCtrl msg_ret(resp);
				socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
				}
				break;
				
			case MessageCtrl::DeviceNameRequest::ID:
				{
				const MessageCtrl::DeviceNameRequest* msg_in
					=(const MessageCtrl::DeviceNameRequest*)msg.data;
				MessageCtrl::DeviceNameResponse resp;
				MidiOut::deviceNameGet(msg_in->id,resp.name);
				MessageCtrl msg_ret(resp);
				socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
				}
				break;
			}
		}
	return 0;
	}

SyZmO::Server::~Server()
	{
	MessageCtrl::ServerShutdown msg_shutdown;
	MessageCtrl msg(msg_shutdown);
	if(m_params.flags&Parameters::SHUTDOWN_BROADCAST)
		{
		socket_out.broadcastEnable();
		socket_out.send(&msg,sizeof(msg),m_params.port_out,"255.255.255.255");
		socket_out.broadcastDisable();
		}
	}
