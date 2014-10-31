#ifdef __WAND__
target[name[client.o] type[object]]
#endif

#include "client.h"
#include "../bridge/message_ctrl.h"

SyZmO::Client::Client(const SyZmO::Client::Parameters& params):
	m_params(params)
	{
	socket_in.bind(m_params.port_in);
	MessageCtrl::DeviceCountRequest msg_temp;
	MessageCtrl msg_out(msg_temp);
	socket_out.broadcastEnable();
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,"255.255.255.255");
	socket_out.broadcastDisable();
	}

SyZmO::Client::~Client()
	{}

int SyZmO::Client::run()
	{
	MessageCtrl msg;
	char source[SocketDatagram::ADDRBUFF_LENGTH];
	while(socket_in.receive(&msg,sizeof(msg),source)==sizeof(msg))
		{
		switch(msg.message_type)
			{
			case MessageCtrl::ServerStartup::ID:
				{
				MessageCtrl::DeviceCountRequest msg_temp;
				MessageCtrl msg_out(msg_temp);
				socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,source);
			//	Notify a local object that a new server is started
				}
				break;

			case MessageCtrl::IsAlive::ID:
				{
				MessageCtrl::NoOp nop;
				MessageCtrl msg_ret(nop);
				socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
				}
				break;

			case MessageCtrl::DeviceCountResponse::ID:
				{
				const SyZmO::MessageCtrl::DeviceCountResponse* msg_in
					=(const SyZmO::MessageCtrl::DeviceCountResponse*)msg.data;

				for(SyZmO::uint32_t k=0;k<msg_in->n_devs;++k)
					{
					SyZmO::MessageCtrl::DeviceNameRequest devreq;
					devreq.id=k;
					SyZmO::MessageCtrl msg_out(devreq);
					socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out
						,source);
					}
				}
				break;

			case SyZmO::MessageCtrl::DeviceNameResponse::ID:
				{
				const SyZmO::MessageCtrl::DeviceNameResponse* msg_in
					=(const SyZmO::MessageCtrl::DeviceNameResponse*)msg.data;
			//	devices[msg_in->name].routeAdd(source);
				}
				break;
			}
		}
	return 0;
	}
