#ifdef __WAND__
target[name[client.o] type[object]]
#endif

#include "client.h"
#include "event_handler.h"
#include "../bridge/message_ctrl.h"
#include <cstring>

SyZmO::Client::Client(const SyZmO::Client::Parameters& params,EventHandler& eh):
	m_params(params),m_handler(eh)
	{
	socket_in.bind(m_params.port_in);
	MessageCtrl::DeviceCountRequest msg_temp;
	MessageCtrl msg_out(msg_temp);
	if(m_params.flags & Parameters::SERVER_ANY)
		{
		socket_out.broadcastEnable();
		m_handler.clientStartup(*this,"255.255.255.255");
		socket_out.broadcastDisable();
		}
	else
		{m_handler.clientStartup(*this,m_params.server_ip);}
	}

SyZmO::Client::~Client()
	{
	m_handler.clientShutdown(*this);
	}

void SyZmO::Client::messageMidiSend(const char* server,uint32_t device_id
	,MessageMidi msg)
	{
	MessageCtrl::Midi msg_temp;
	msg_temp.device_id=device_id;
	msg_temp.midi=msg;

	MessageCtrl msg_out(msg_temp);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::deviceCountRequest(const char* server)
	{
	MessageCtrl::DeviceCountRequest msg_temp;
	MessageCtrl msg_out(msg_temp);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::deviceNameRequest(const char* server,uint32_t id)
	{
	MessageCtrl::DeviceNameRequest devreq;
	devreq.device_id=id;
	MessageCtrl msg_out(devreq);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverExitRequest(const char* server)
	{
	MessageCtrl::ServerExitRequest exitreq;
	MessageCtrl msg_out(exitreq);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::connectionOpenRequest(const char* server,uint32_t device_id)
	{
	MessageCtrl::ConnectionOpenRequest req;
	req.device_id=device_id;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::connectionCloseRequest(const char* server,uint32_t device_id)
	{
	MessageCtrl::ConnectionCloseRequest req;
	req.device_id=device_id;
	MessageCtrl msg_out(req);
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
			&& (strcmp(source,m_params.server_ip)==0
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
					running=m_handler.serverStartup(*this,source);
					break;

				case MessageCtrl::ServerExitResponse::ID:
					running=m_handler.serverShutdown(*this,source);
					break;

				case MessageCtrl::DeviceCountResponse::ID:
					{
					const MessageCtrl::DeviceCountResponse* msg_in
						=(const MessageCtrl::DeviceCountResponse*)msg.data;
					running=m_handler.deviceCount(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::DeviceNameResponse::ID:
					{
					const MessageCtrl::DeviceNameResponse* msg_in
						=(const MessageCtrl::DeviceNameResponse*)msg.data;
					running=m_handler.deviceName(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ConnectionOpenResponsePrivate::ID:
					{
					const MessageCtrl::ConnectionOpenResponsePrivate* msg_in
						=(const MessageCtrl::ConnectionOpenResponsePrivate*)msg.data;
					running=m_handler.connectionOpened(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ConnectionOpenResponsePublic::ID:
					{
					const MessageCtrl::ConnectionOpenResponsePublic* msg_in
						=(const MessageCtrl::ConnectionOpenResponsePublic*)msg.data;
					running=m_handler.connectionOpened(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ConnectionCloseResponsePrivate::ID:
					{
					const MessageCtrl::ConnectionCloseResponsePrivate* msg_in
						=(const MessageCtrl::ConnectionCloseResponsePrivate*)msg.data;
					running=m_handler.connectionClosed(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ConnectionCloseResponsePublic::ID:
					{
					const MessageCtrl::ConnectionCloseResponsePublic* msg_in
						=(const MessageCtrl::ConnectionCloseResponsePublic*)msg.data;
					running=m_handler.connectionClosed(*this,source,*msg_in);
					}
					break;
				}
			}
		}
	return 0;
	}
