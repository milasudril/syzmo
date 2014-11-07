#ifdef __WAND__
target[name[server_setup.h] type[include]]
dependency[server_setup.o]
#endif

#ifndef SYZMO_SERVERSETUP_H
#define SYZMO_SERVERSETUP_H

#include "inttypes.h"

namespace SyZmO
	{
	struct ServerSetup
		{
		static const uint32_t STARTUP_BROADCAST=0x1;
		uint32_t flags;
		uint16_t port_in;
		uint16_t port_out;
		};
		
	void load(ServerSetup& setup);
	}

#endif
