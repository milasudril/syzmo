#ifdef __WAND__
target[name[server_setup.h] type[include]]
#endif

#ifndef SYZMO_SERVERSETUP_H
#define SYZMO_SERVERSETUP_H

#include "inttypes.h"

namespace SyZmO
	{
	struct ServerSetup
		{
		uint16_t port_client;
		uint16_t port_server;
		};
	}

#endif
