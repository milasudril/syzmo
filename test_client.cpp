#ifdef __WAND__
target[name[syzmo_test_client] platform[;GNU/Linux] type[application]]
#endif

#include "ackmessage.h"
#include <herbs/network/socket_datagram.h>

#include <cstdio>

int main()
	{
	Herbs::Network::SocketDatagram sd;
	sd.bind(49152);
	Syzmo::AckMessage ack;
	char source[Herbs::Network::SocketDatagram::ADDRBUFF_LENGTH];
	auto n=sd.receive(&ack,sizeof(ack),source);
	if(n==sizeof(ack))
		{
		if(ack.validIs())
			{
			printf("%s %u\n",source,ack.msgcode);
			}
		}
	return 0;
	}
