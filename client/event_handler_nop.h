#ifdef __WAND__
target[name[event_handler_nop.h] type[include]]
dependency[event_handler_nop.o]
#endif

#ifndef SYZMO_EVENTHANDLERNOP_H
#define SYZMO_EVENTHANDLERNOP_H

#include "event_handler.h"

namespace SyZmO
	{
	class EventHandlerNop:public EventHandler
		{
		public:
			virtual void clientStartup(Client& client,const char* server){}
			virtual void clientShutdown(Client& client,const char* server){}
		
			virtual bool serverStartup(Client& client,const char* server)
				{return 1;}
				
			virtual bool serverShutdown(Client& client,const char* server)
				{return 1;}
			
			virtual bool deviceCountSet(Client& client,const char* server
				,uint32_t n_devs)
				{return 1;}
				
			virtual bool deviceAdd(Client& client,const char* server
				,uint32_t id,const char* device)
				{return 1;}
		};
		
	extern EventHandlerNop event_handler_null;
	}
	
#endif
