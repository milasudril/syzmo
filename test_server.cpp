#ifdef __WAND__
target[name[syzmo_test_server] platform[;GNU/Linux] type[application]]
#endif

#include "ackmessage.h"
#include <herbs/network/socket_datagram.h>

int main()
	{
	Herbs::Network::SocketDatagram sd;
	sd.broadcastEnable();
	Syzmo::AckMessage ack(1);
	sd.send(&ack,sizeof(ack),49152,"255.255.255.255");
	return 0;
	}
