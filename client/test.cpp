#ifdef __WAND__
target[name[syzmo_client] type[application]]
#endif

#include "../bridge/socket_datagram.h"
#include "../bridge/message_ctrl.h"
#include <cstdio>

int main()
	{
	SyZmO::uint16_t port_in=49152;
	SyZmO::uint16_t port_out=port_in+1;
	const char* ip="192.168.1.66";

	SyZmO::SocketDatagram socket_in;
	socket_in.bind(port_in);

	SyZmO::SocketDatagram socket_out;
		{
		SyZmO::MessageCtrl::DeviceCountRequest msg_temp;
		SyZmO::MessageCtrl msg_out(msg_temp);
		socket_out.send(&msg_out,sizeof(msg_out),port_out,ip);
		}

	SyZmO::MessageCtrl msg;
	char source[SyZmO::SocketDatagram::ADDRBUFF_LENGTH];
	while(socket_in.receive(&msg,sizeof(msg),source)==sizeof(msg))
		{
		switch(msg.message_type)
			{
			case SyZmO::MessageCtrl::DeviceCountResponse::ID:
				{
				const SyZmO::MessageCtrl::DeviceCountResponse* msg_in
					=(const SyZmO::MessageCtrl::DeviceCountResponse*)msg.data;
				printf("%s has %u MIDI devices availible\n",source,msg_in->n_devs);
				for(SyZmO::uint32_t k=0;k<msg_in->n_devs;++k)
					{
					SyZmO::MessageCtrl::DeviceNameRequest devreq;
					devreq.id=k;
					SyZmO::MessageCtrl msg_out(devreq);
					socket_out.send(&msg_out,sizeof(msg_out),port_out,source);
					}
				}
				break;
			case SyZmO::MessageCtrl::DeviceNameResponse::ID:
				{
				const SyZmO::MessageCtrl::DeviceNameResponse* msg_in
					=(const SyZmO::MessageCtrl::DeviceNameResponse*)msg.data;
				printf(" * %s\n",msg_in->name);
				}
				break;

			default:
				printf("%u\t%s\t%u\n",msg.timestamp,source,msg.message_type);
			}
		}
	}