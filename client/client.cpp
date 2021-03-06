#ifdef __WAND__
target[name[client.o] type[object]]
#endif

#include "client.h"
#include "event_handler.h"
#include "../bridge/message_ctrl.h"
#include <cstring>

SyZmO::Client::Client(SyZmO::Client::Parameters& params,EventHandler& eh):
	m_params(params),m_handler(eh)
	{
	socket_in.bind(m_params.port_in);
	MessageCtrl::DeviceCountRequest msg_temp;
	MessageCtrl msg_out(msg_temp);
	socket_in.recvTimeoutSet(2);
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
	,const MessageMidi* msg,size_t message_count)
	{
	if(*server=='\0' || message_count==0)
		{return;}
	MessageCtrl::Midi msg_temp;
	msg_temp.device_id=device_id;
	size_t count=0;
	while(message_count!=0)
		{
		msg_temp.midi[count]=*msg;
		++count;
		++msg;
		--message_count;
			
		if(count==MessageCtrl::Midi::MESSAGE_COUNT_MAX || message_count==0)
			{
			MessageCtrl msg_out(msg_temp);
			msg_out.header.message_size=sizeof(msg_temp.device_id)
				+ count*sizeof(MessageMidi);
			socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
			count=0;
			}		
		}
	}

void SyZmO::Client::isAliveRequest(const char* server)
	{
	MessageCtrl::IsAliveRequest msg_temp;
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

void SyZmO::Client::serverHostnameRequest(const char* server)
	{
	MessageCtrl::ServerHostnameRequest req;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverSetupGetRequest(const char* server)
	{
	MessageCtrl::ServerSetupGetRequest req;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverSetupSetRequest(const char* server,const ServerSetup& setup)
	{
	MessageCtrl::ServerSetupSetRequest req;
	req.setup=setup;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverShutdownRequest(const char* server)
	{
	MessageCtrl::ServerShutdownRequest req;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverRebootRequest(const char* server)
	{
	MessageCtrl::ServerRebootRequest req;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverTestRequest(const char* server,uint32_t device_id)
	{
	MessageCtrl::ServerTestRequest req;
	req.device_id=device_id;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

void SyZmO::Client::serverLogClearRequest(const char* server)
	{
	MessageCtrl::ServerLogClearRequest req;
	MessageCtrl msg_out(req);
	socket_out.send(&msg_out,sizeof(msg_out),m_params.port_out,server);
	}

int SyZmO::Client::run()
	{
	MessageCtrl msg;
	char source[SocketDatagram::ADDRBUFF_LENGTH];
	bool running=1;
	m_stop=0;
	while(running && !m_stop)
		{
		memset(&msg,0,sizeof(msg));
		if(socket_in.receive(&msg,sizeof(msg),source)!=sizeof(msg))
			{
			running=m_handler.timeout(*this);
			continue;
			}

		if(socket_in.recvTimeout())
			{
			running=m_handler.timeout(*this);
			continue;
			}

		if(msg.validIs()
			&& (strcmp(source,m_params.server_ip)==0
			|| m_params.flags&Parameters::SERVER_ANY))
			{
			switch(msg.header.message_type)
				{
				case MessageCtrl::IsAliveRequest::ID:
					{
					MessageCtrl::IsAliveResponse nop;
					MessageCtrl msg_ret(nop);
					socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
					}
					break;

				case MessageCtrl::IsAliveResponse::ID:
					running=m_handler.isAlive(*this,source);
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
					MessageCtrl::DeviceNameResponse* msg_in
						=(MessageCtrl::DeviceNameResponse*)msg.data;
					msg_in->name[MessageCtrl::DeviceNameResponse::NAME_LENGTH-1]=0;
					running=m_handler.deviceName(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ConnectionOpenResponsePrivate::ID:
					{
					MessageCtrl::ConnectionOpenResponsePrivate* msg_in
						=(MessageCtrl::ConnectionOpenResponsePrivate*)msg.data;
					msg_in->name[MessageCtrl::ConnectionOpenResponsePrivate
						::NAME_LENGTH-1]=0;
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


				case MessageCtrl::ServerHostnameResponse::ID:
					{
					MessageCtrl::ServerHostnameResponse* msg_in
						=(MessageCtrl::ServerHostnameResponse*)msg.data;
					msg_in->hostname[MessageCtrl::ServerHostnameResponse
						::HOSTNAME_LENGTH-1]=0;
					running=m_handler.serverHostname(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ServerSetupGetResponse::ID:
					{
					MessageCtrl::ServerSetupGetResponse* msg_in
						=(MessageCtrl::ServerSetupGetResponse*)msg.data;
					running=m_handler.serverSetupGet(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ServerSetupSetResponse::ID:
					{
					MessageCtrl::ServerSetupSetResponse* msg_in
						=(MessageCtrl::ServerSetupSetResponse*)msg.data;
					running=m_handler.serverSetupSet(*this,source,*msg_in);
					m_params.port_in=msg_in->setup.port_out;
					m_params.port_out=msg_in->setup.port_in;
					if(running)
						{return RUN_STATUS_CONTINUE;}
					else
						{return RUN_STATUS_EXIT;}
					}
					break;

				case MessageCtrl::ServerTestResponse::ID:
					{
					MessageCtrl::ServerTestResponse* msg_in
						=(MessageCtrl::ServerTestResponse*)msg.data;
					running=m_handler.serverTest(*this,source,*msg_in);
					}
					break;

				case MessageCtrl::ServerLogClearResponse::ID:
					{
					MessageCtrl::ServerLogClearResponse* msg_in
						=(MessageCtrl::ServerLogClearResponse*)msg.data;
					running=m_handler.serverLogCleared(*this,source,*msg_in);
					}
					break;
				}
			}
		else
			{
			running=m_handler.timeout(*this);
			continue;
			}
			
		}
		
	return RUN_STATUS_EXIT;
	}
