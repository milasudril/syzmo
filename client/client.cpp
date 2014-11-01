#ifdef __WAND__
target[name[client.o] type[object]]
#endif

#include "client.h"
#include "event_handler.h"
#include "../bridge/message_ctrl.h"
#include <cstring>

SyZmO::Client::Client(const SyZmO::Client::Parameters& params,EventHandler& eh):
	m_params(params),m_handler(&eh)
	{
	socket_in.bind(m_params.port_in);
	MessageCtrl::DeviceCountRequest msg_temp;
	MessageCtrl msg_out(msg_temp);
	if(m_params.flags & Parameters::SERVER_ANY)
		{
		socket_out.broadcastEnable();
		m_handler->clientStartup(*this,"255.255.255.255");
		socket_out.broadcastDisable();
		}
	else
		{m_handler->clientStartup(*this,m_params.server_ip);}
	}

SyZmO::Client::~Client()
	{
	m_handler->clientShutdown(*this,m_params.server_ip);
	}

void SyZmO::Client::deviceCountRequest(const char* server)
	{
	MessageCtrl::DeviceCountRequest msg_temp;
	MessageCtrl msg_out(msg_temp);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::deviceNameRequest(const char* server,uint32_t id)
	{
	SyZmO::MessageCtrl::DeviceNameRequest devreq;
	devreq.device_id=id;
	SyZmO::MessageCtrl msg_out(devreq);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}
	
int SyZmO::Client::run()
	{
	MessageCtrl msg;
	char source[SocketDatagram::ADDRBUFF_LENGTH];
	bool running=1;
	while(running)
		{
		socket_in.receive(&msg,sizeof(msg),source);
		
		if(msg.validIs() 
			&& (strncmp(source,m_params.server_ip,SocketDatagram::ADDRBUFF_LENGTH)==0
			|| m_params.flags&Parameters::SERVER_ANY))
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
				
				case MessageCtrl::ServerStartupResponse::ID:
					running=m_handler->serverStartup(*this,source);
					break;
					
				case MessageCtrl::ServerExitResponse::ID:
					running=m_handler->serverShutdown(*this,source);
					break;

				case MessageCtrl::DeviceCountResponse::ID:
					{
					const SyZmO::MessageCtrl::DeviceCountResponse* msg_in
						=(const SyZmO::MessageCtrl::DeviceCountResponse*)msg.data;
					running=m_handler->deviceCountSet(*this,source,msg_in->n_devs);
					}
					break;

				case SyZmO::MessageCtrl::DeviceNameResponse::ID:
					{
					const SyZmO::MessageCtrl::DeviceNameResponse* msg_in
						=(const SyZmO::MessageCtrl::DeviceNameResponse*)msg.data;
					running=m_handler->deviceAdd(*this,source,msg_in->device_id
						,msg_in->name);
					}
					break;
				}
			}
		}
	return 0;
	}
