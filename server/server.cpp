#ifdef __WAND__
target[name[server.o] type[object] dependency[winmm;external] platform[;Windows]]
#endif

#include "server.h"
#include "connection.h"
#include "../bridge/socket_datagram.h"
#include "../bridge/message_ctrl.h"
#include "../bridge/server_setup.h"
#include "../logfile/logfile_out.h"
#include "../exception_missing.h"

#include <windows.h>

SyZmO::Server::Server(ServerSetup& params,LogfileOut& logfile):
	m_params(params),m_log(logfile)
	{
	timeBeginPeriod(1);
	n_devs=MidiOut::deviceCount();
	m_log.entryWrite("127.0.0.1","Server has %u devices",n_devs);
	connections=new Connection*[n_devs];
	for(size_t k=0;k<n_devs;++k)
		{connections[k]=NULL;}

	socket_in.recvTimeoutSet(2);
	socket_in.bind(m_params.port_in);
	m_log.entryWrite("127.0.0.1","Server is listening to port %u"
		,m_params.port_in);
	if(m_params.flags&ServerSetup::STARTUP_BROADCAST)
		{
		socket_out.broadcastEnable();
		MessageCtrl::ServerStartupResponse msg_startup;
		MessageCtrl msg(msg_startup);
		socket_out.send(&msg,sizeof(msg),m_params.port_out,"255.255.255.255");
		socket_out.broadcastDisable();
		logfile.entryWrite("127.0.0.1","Startup message broadcasted to port %u"
			,m_params.port_out);
		}
	time_activity=timeGetTime();
	m_log.entryWrite("127.0.0.1","Server has started. "
		"Time since latest activity is %u ms.",time_activity);
	}

SyZmO::Server::~Server()
	{
	MessageCtrl::ServerExitResponse msg_shutdown;
	MessageCtrl msg(msg_shutdown);
	socket_out.broadcastEnable();
	socket_out.send(&msg,sizeof(msg),m_params.port_out,"255.255.255.255");
	m_log.entryWrite("127.0.0.1","Shutdown message broadcasted to port %u"
		,m_params.port_out);
	socket_out.broadcastDisable();

	m_log.entryWrite("127.0.0.1","Cleaning up connections",m_params.port_out);
	for(size_t k=0;k<n_devs;++k)
		{
		if(connections[k]!=NULL)
			{delete connections[k];}
		}
	delete[] connections;
	timeEndPeriod(1);
	m_log.entryWrite("127.0.0.1","Server has exited",m_params.port_out);
	}


void SyZmO::Server::midiMessageSend(const char* client,uint32_t dev_id
	,const MessageMidi* msg,size_t message_count)
	{
	if(dev_id < n_devs)
		{
		if(connections[dev_id]!=NULL)
			{
			while(message_count!=0)
				{
				connections[dev_id]->messageSend(client,*msg);
				--message_count;
				++msg;
				}				
			}
		}
	}

void SyZmO::Server::deviceCountSend(const char* client)
	{
	m_log.entryWrite(client,"Device count request");
	MessageCtrl::DeviceCountResponse resp;
	resp.n_devs=n_devs;
	MessageCtrl msg_ret(resp);
 	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::deviceNameSend(const char* client,uint32_t dev_id)
	{
	MessageCtrl::DeviceNameResponse resp;
	m_log.entryWrite(client,"Device name request for device %u",dev_id);
	if(dev_id < n_devs)
		{
		MidiOut::deviceNameGet(dev_id,resp.name);
		resp.device_id=dev_id;
		resp.status=MessageCtrl::DeviceNameResponse::STATUS_BUSY;
		if(connections[dev_id]==NULL)
			{resp.status=MessageCtrl::DeviceNameResponse::STATUS_READY;}
		m_log.entryWrite("127.0.0.1","Device name is %s",resp.name);
		}
	else
		{
		resp.device_id=(uint32_t)-1;
		resp.status=MessageCtrl::DeviceNameResponse::STATUS_INVALID;
		m_log.entryWrite("127.0.0.1","Device does not exist");
		}
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::clientConnect(const char* client,uint32_t dev_id)
	{
	MessageCtrl::ConnectionOpenResponsePrivate resp;
	m_log.entryWrite(client,"Connection request for device %u",dev_id);
	resp.device_id=dev_id;
	if(dev_id >= n_devs)
		{
		resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_INVALID;
		MessageCtrl msg_ret(resp);
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
		m_log.entryWrite("127.0.0.1","Device does not exist");
		return;
		}

	MidiOut::deviceNameGet(resp.device_id,resp.name);

	if(connections[dev_id]==NULL)
		{
		try
			{
			connections[dev_id]=new Connection(client,dev_id);
			m_log.entryWrite("127.0.0.1","Client is connected to %u [%s]"
				,dev_id,resp.name);
			MessageCtrl::ConnectionOpenResponsePublic resp_public;
			resp_public.device_id=dev_id;
			MessageCtrl msg_ret(resp_public);
			socket_out.broadcastEnable();
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out
				,"255.255.255.255");
			socket_out.broadcastDisable();

			}
		catch(const ExceptionMissing& e)
			{
			e.print(m_log);
			resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_ERROR;
			MessageCtrl msg_ret(resp);
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
			return;
			}
		}
	else
		{
		if(!connections[dev_id]->clientMatch(client))
			{
			m_log.entryWrite("127.0.0.1","Device %u [%s] is busy"
				,dev_id,resp.name);
			resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_BUSY;
			MessageCtrl msg_ret(resp);
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
			return;
			}
		else
			{
			m_log.entryWrite("127.0.0.1","Client is already connected to %u [%s]"
				,dev_id,resp.name);
			}
		}

	resp.status=MessageCtrl::ConnectionOpenResponsePrivate::STATUS_OK;
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::clientDisconnect(const char* client,uint32_t dev_id)
	{
	MessageCtrl::ConnectionCloseResponsePrivate resp;
	m_log.entryWrite(client,"Connection close request for device %u",dev_id);
	resp.device_id=dev_id;
	if(dev_id>=n_devs)
		{
		resp.status=MessageCtrl::ConnectionCloseResponsePrivate::STATUS_INVALID;
		m_log.entryWrite("127.0.0.1","Device does not exist");
		MessageCtrl msg_ret(resp);
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
		return;
		}

	if(connections[dev_id]==NULL)
		{
		resp.status=MessageCtrl::ConnectionCloseResponsePrivate::NOT_CONNECTED;
		m_log.entryWrite("127.0.0.1","Device is not connected");
		MessageCtrl msg_ret(resp);
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
		return;
		}

	if(!connections[dev_id]->clientMatch(client))
		{
		resp.status=MessageCtrl::ConnectionCloseResponsePrivate::NOT_OWNER;
		m_log.entryWrite("127.0.0.1","Remote address does not match");
		MessageCtrl msg_ret(resp);
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
		return;
		}

	MessageCtrl::ConnectionCloseResponsePublic resp_public;
	resp_public.device_id=dev_id;
		{
		MessageCtrl msg_ret(resp_public);
		socket_out.broadcastEnable();
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out
			,"255.255.255.255");
		socket_out.broadcastDisable();
		}
	delete connections[dev_id];
	connections[dev_id]=NULL;
	resp.status=MessageCtrl::ConnectionCloseResponsePrivate::STATUS_OK;
	m_log.entryWrite("127.0.0.1","Device disconnected");
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::connectionsIsAliveRequest()
	{
	Connection** ptr=connections;
	while(ptr!=connections + n_devs)
		{
		if(*ptr!=NULL)
			{
			if(!(*ptr)->isAliveRequest(*this))
				{
				m_log.entryWrite("127.0.0.1"
					,"Client %s did not respond in time. Closing connection."
					,(*ptr)->clientNameGet());
				delete *ptr;
				*ptr=NULL;
				}
			}
		++ptr;
		}
	}

void SyZmO::Server::isAliveRequest(const char* client)
	{
	MessageCtrl::IsAliveRequest resp;
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::hostnameSend(const char* client)
	{
	MessageCtrl::ServerHostnameResponse resp;
	DWORD n=MessageCtrl::ServerHostnameResponse::HOSTNAME_LENGTH;
	memset(resp.hostname,0,n*sizeof(char));
	GetComputerName(resp.hostname,&n);

	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::setupGetSend(const char* client)
	{
	MessageCtrl::ServerSetupGetResponse resp;
	resp.setup=m_params;

	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::setupSet(const char* client,const ServerSetup& setup)
	{
	m_log.entryWrite(client,"Setup set request");
	MessageCtrl::ServerSetupSetResponse resp;
	resp.setup=setup;
	MessageCtrl msg_ret(resp);
	if(m_params.flags&ServerSetup::STARTUP_BROADCAST)
		{
		socket_out.broadcastEnable();
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out
			,"255.255.255.255");
		socket_out.broadcastDisable();
		m_log.entryWrite("127.0.0.1","Setup change broadcasted");
		}
	else
		{socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);}
	m_params=setup;
	}

void SyZmO::Server::deviceTest(const char* client, uint32_t dev_id)
	{
	MessageCtrl::ServerTestResponse resp;
	m_log.entryWrite(client,"Testing device %u",dev_id);
	resp.device_id=dev_id;
	if(dev_id > n_devs)
		{
		resp.status=MessageCtrl::ServerTestResponse::STATUS_INVALID;
		MessageCtrl msg_ret(resp);
		socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
		m_log.entryWrite("127.0.0.1","Device does not exist");
		return;
		}

	MidiOut::deviceNameGet(resp.device_id,resp.name);

	bool new_is=0;
	if(connections[dev_id]==NULL)
		{
		try
			{
			new_is=1;
			connections[dev_id]=new Connection(client,dev_id);
			}
		catch(const ExceptionMissing& e)
			{
			e.print(m_log);

			resp.status=MessageCtrl::ServerTestResponse::STATUS_ERROR;
			MessageCtrl msg_ret(resp);
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
			return;
			}
		}
	else
		{
		if(!connections[dev_id]->clientMatch(client))
			{
			m_log.entryWrite("127.0.0.1","Device %u [%s] is busy"
				,dev_id,resp.name);

			resp.status=MessageCtrl::ServerTestResponse::STATUS_BUSY;
			MessageCtrl msg_ret(resp);
			socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
			return;
			}
		else
			{
			m_log.entryWrite("127.0.0.1","Client is already connected to %u [%s]"
				,dev_id,resp.name);
			}
		}

	SyZmO::MessageMidi msg;
	msg.bytes[0]=0xc0; msg.bytes[1]=81; msg.bytes[2]=0;
	connections[dev_id]->messageSend(client,msg);

	msg.bytes[0]=0xb0; msg.bytes[1]=7; msg.bytes[2]=127;
	connections[dev_id]->messageSend(client,msg);

	msg.bytes[0]=0x90; msg.bytes[1]=69; msg.bytes[2]=127;
	connections[dev_id]->messageSend(client,msg);

	Sleep(300);

	msg.bytes[0]=0x80; msg.bytes[1]=69; msg.bytes[2]=127;
	connections[dev_id]->messageSend(client,msg);

	if(new_is)
		{
		delete connections[dev_id];
		connections[dev_id]=NULL;
		}

	resp.status=MessageCtrl::ServerTestResponse::STATUS_OK;
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::logClear(const char* client)
	{
	m_log.clear();
	MessageCtrl::ServerLogClearResponse resp;
	MessageCtrl msg_ret(resp);
	socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,client);
	}

void SyZmO::Server::connectionsIsAlive(const char* client)
	{
	Connection** ptr=connections;
	while(ptr!=connections + n_devs)
		{
		if(*ptr!=NULL)
			{
			if((*ptr)->clientMatch(client))
				{(*ptr)->isAlive();}
			}
		++ptr;
		}
	}



int SyZmO::Server::run()
	{
	MessageCtrl msg;
	char source[SyZmO::SocketDatagram::ADDRBUFF_LENGTH];
	running=1;
	while(running)
		{
		if(socket_in.receive(&msg,sizeof(msg),source)!=sizeof(msg))
			{continue;}
		uint32_t time_now=timeGetTime();
		if(socket_in.recvTimeout() || time_now-time_activity >3000)
			{connectionsIsAliveRequest();}

		if(msg.validIs())
			{
			switch(msg.header.message_type)
				{
				case MessageCtrl::Midi::ID:
					{
					const MessageCtrl::Midi* m=(const MessageCtrl::Midi*)msg.data;
					size_t message_count=
						std::min((msg.header.message_size-sizeof(m->device_id))
							/sizeof(MessageMidi)
							,MessageCtrl::Midi::MESSAGE_COUNT_MAX);
					
					midiMessageSend(source,m->device_id,m->midi,message_count);
					time_activity=timeGetTime();
					};
					break;

				case MessageCtrl::IsAliveRequest::ID:
					{
					MessageCtrl::IsAliveResponse nop;
					MessageCtrl msg_ret(nop);
					socket_out.send(&msg_ret,sizeof(msg_ret),m_params.port_out,source);
					time_activity=timeGetTime();
					}
					break;

				case MessageCtrl::IsAliveResponse::ID:
					connectionsIsAlive(source);
					time_activity=timeGetTime();
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
					time_activity=timeGetTime();
					}
					break;

				case MessageCtrl::ConnectionCloseRequest::ID:
					{
					const MessageCtrl::ConnectionCloseRequest* msg_in
						=(const MessageCtrl::ConnectionCloseRequest*)msg.data;
					clientDisconnect(source,msg_in->device_id);
					}
					break;

				case MessageCtrl::ServerHostnameRequest::ID:
					hostnameSend(source);
					break;

				case MessageCtrl::ServerSetupGetRequest::ID:
					setupGetSend(source);
					break;

				case MessageCtrl::ServerSetupSetRequest::ID:
					{
					const MessageCtrl::ServerSetupSetRequest* msg_in
						=(const MessageCtrl::ServerSetupSetRequest*)msg.data;
					setupSet(source,msg_in->setup);
					}
					return RUN_STATUS_CONTINUE;

				case MessageCtrl::ServerShutdownRequest::ID:
					return RUN_STATUS_SHUTDOWN;

				case MessageCtrl::ServerRebootRequest::ID:
					return RUN_STATUS_REBOOT;

				case MessageCtrl::ServerTestRequest::ID:
					{
					const MessageCtrl::ServerTestRequest* msg_in
						=(const MessageCtrl::ServerTestRequest*)msg.data;
					deviceTest(source,msg_in->device_id);
					}
					break;

				case MessageCtrl::ServerLogClearRequest::ID:
					logClear(source);
					break;
				}
			}
		}
	return RUN_STATUS_CONTINUE;
	}
