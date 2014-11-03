#ifdef __WAND__
target[name[server.o] type[object]]
#endif

#include "server.h"
#include "connection.h"
#include "../bridge/socket_datagram.h"
#include "../bridge/message_ctrl.h"


SyZmO::Server::Server(const Parameters& params):
	m_params(params)
	{
	n_devs=MidiOut::deviceCount();
	connections=new Connection*[n_devs];
	for(size_t k=0;k<n_devs;++k)
		{connections[k]=NULL;}

	socket_in.bind(m_params.port_in);
	if(m_params.flags&Parameters::STARTUP_BROADCAST)
		{
		socket_out.broadcastEnable();
		MessageCtrl::ServerStartupResponse msg_startup;
		MessageCtrl msg(msg_startup);
		socket_out.send(&msg,sizeof(msg),m_params.port_out,"255.255.255.255");
		socket_out.broadcastDisable();
		}
	}

SyZmO::Server::~Server()
	{
	MessageCtrl::ServerExitResponse msg_shutdown;
	MessageCtrl msg(msg_shutdown);
	if(m_params.flags&Parameters::SHUTDOWN_BROADCAST)
		{
		socket_out.broadcastEnable();
		socket_out.send(&msg,sizeof(msg),m_params.port_out,"255.255.255.255");
		socket_out.broadcastDisable();
		}

	for(size_t k=0;k<n_devs;++k)
		{
		if(connections[k]!=NULL)
			{delete connections[k];}
		}
	delete[] connections;
	}


void SyZmO::Server::midiMessageSend(const char* client,uint32_t dev_id
	,MessageMidi msg)
	{
	if(dev_id < n_devs)
		{
		if(connections[dev_id]!=NULL)
			{connections[dev_id]->messageSend(client,msg);}
		}
	}

void SyZmO::Server::deviceCountSend(const char* client)
	{
	MessageCtrl::DeviceCountResponse resp;
	resp.n_devs=MidiOut::deviceCount();
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::deviceNameSend(const char* client,uint32_t dev_id)
	{
	MessageCtrl::DeviceNameResponse resp;
	if(dev_id < n_devs)
		{
		resp.device_id=dev_id;
		resp.status=MessageCtrl::DeviceNameResponse::STATUS_BUSY;
		if(connections[dev_id]==NULL)
			{resp.status=MessageCtrl::DeviceNameResponse::STATUS_READY;}
		MidiOut::deviceNameGet(dev_id,resp.name);
		}
	else
		{
		resp.device_id=(uint32_t)-1;
		resp.status=MessageCtrl::DeviceNameResponse::STATUS_INVALID;
		}
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::clientConnect(const char* client,uint32_t dev_id)
	{
	MessageCtrl::ConnectionOpenResponsePrivate resp;
	resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_OK;
	resp.device_id=dev_id;
	MidiOut::deviceNameGet(dev_id,resp.name);
	if(connections[dev_id]==NULL)
		{
		try
			{
			connections[dev_id]=new Connection(client,dev_id);

			MessageCtrl::ConnectionOpenResponsePublic resp_public;
			resp_public.device_id=dev_id;
			MessageCtrl msg_ret(resp_public);
			socket_out.broadcastEnable();
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out
				,"255.255.255.255");
			socket_out.broadcastDisable();
			}
		catch(...)
			{resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_ERROR;}
		}
	else
		{
		if(!connections[dev_id]->clientMatch(client))
			{resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_BUSY;}
		}
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::clientDisconnect(const char* client,uint32_t dev_id)
	{
	MessageCtrl::ConnectionCloseResponsePrivate resp;
	resp.device_id=dev_id;
	if(connections[dev_id]==NULL)
		{resp.status=MessageCtrl::ConnectionCloseResponsePrivate::NOT_CONNECTED;}
	else
		{
		if(!connections[dev_id]->clientMatch(client))
			{resp.status=MessageCtrl::ConnectionCloseResponsePrivate::NOT_OWNER;}
		else
			{
			MessageCtrl::ConnectionCloseResponsePublic resp_public;
			resp_public.device_id=dev_id;
			MessageCtrl msg_ret(resp_public);
			socket_out.broadcastEnable();
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out
				,"255.255.255.255");
			socket_out.broadcastDisable();
			delete connections[dev_id];
			connections[dev_id]=NULL;
			}
		}
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}


int SyZmO::Server::run()
	{
	MessageCtrl msg;
	char source[SyZmO::SocketDatagram::ADDRBUFF_LENGTH];
	while(socket_in.receive(&msg,sizeof(msg),source)==sizeof(msg))
		{
		if(msg.validIs())
			{
			switch(msg.message_type)
				{
				case MessageCtrl::Midi::ID:
					{
					const MessageCtrl::Midi* m=(const MessageCtrl::Midi*)msg.data;
					midiMessageSend(source,m->device_id,m->midi);
					};
					break;

				case MessageCtrl::IsAlive::ID:
					{
					MessageCtrl::NoOp nop;
					MessageCtrl msg_ret(nop);
					socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
					}
					break;

				case MessageCtrl::DeviceCountRequest::ID:
					deviceCountSend(source);
					break;

				case MessageCtrl::DeviceNameRequest::ID:
					{
					const MessageCtrl::DeviceNameRequest* msg_in
						=(const MessageCtrl::DeviceNameRequest*)msg.data;
					deviceNameSend(source,msg_in->device_id);
					}
					break;

				case MessageCtrl::ConnectionOpenRequest::ID:
					{
					const MessageCtrl::ConnectionOpenRequest* msg_in
						=(const MessageCtrl::ConnectionOpenRequest*)msg.data;
					clientConnect(source,msg_in->device_id);
					}
					break;

				case MessageCtrl::ConnectionCloseRequest::ID:
					{
					const MessageCtrl::ConnectionCloseRequest* msg_in
						=(const MessageCtrl::ConnectionCloseRequest*)msg.data;
					clientDisconnect(source,msg_in->device_id);
					}
					break;

				case MessageCtrl::ServerExitRequest::ID:
					return 0;
				}
			}
		}
	return 0;
	}
