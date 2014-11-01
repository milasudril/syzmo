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
		struct
			{
			int16_t year;
			uint16_t month;
			uint16_t day;
			uint16_t hour;
			uint16_t minute;
			uint16_t seconds;
			uint16_t milliseconds;
			uint16_t padding;
			} time;
		};
	}

#endif
