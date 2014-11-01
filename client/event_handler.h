#ifdef __WAND__
target[name[event_handler.h] type[include]]
#endif

#ifndef SYZMO_EVENTHANDLER_H
#define SYZMO_EVENTHANDLER_H

#include "../bridge/inttypes.h"

namespace SyZmO
	{
	class Client;
	
	class EventHandler
		{
		public:
			virtual void clientStartup(Client& client,const char* server)=0;
			virtual void clientShutdown(Client& client,const char* server)=0;
		
			virtual bool serverStartup(Client& client,const char* server)=0;
			virtual bool serverShutdown(Client& client,const char* server)=0;
			
			virtual bool deviceCountSet(Client& client,const char* server
				,uint32_t n_devs)=0;
			virtual bool deviceAdd(Client& client,const char* server
				,uint32_t id,const char* device)=0;
			
		};
	}

#endif
