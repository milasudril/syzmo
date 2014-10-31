#ifdef __WAND__
target[name[syzmo_client] type[application]]
#endif

#include "client.h"

int main()
	{
	SyZmO::Client::Parameters params;
	params.port_in=49152;
	params.port_out=params.port_in+1;

	SyZmO::Client client(params);
	client.run();

	return 0;

	}