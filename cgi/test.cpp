#ifdef __WAND__
target[name[syzmo_client] type[application] platform[;GNU/Linux]]
#endif

#include "client.h"
#include "event_handler.h"

#include <cstdio>

class EventHandlerTest:public SyZmO::EventHandler
	{
	public:
		virtual void clientStartup(SyZmO::Client& client,const char* server)
			{
			printf("Client created. Asking for number of devices\n");
			client.deviceCountRequest(server);
			}
			
		virtual void clientShutdown(SyZmO::Client& client,const char* server)
			{
			printf("Client is beeing destroyed\n");
			}
	
		virtual bool serverStartup(SyZmO::Client& client,const char* server)
			{
			printf("%s has started. Asking for number of devices\n",server);
			client.deviceCountRequest(server);
			return 1;
			}
			
		virtual bool serverShutdown(SyZmO::Client& client,const char* server)
			{
			printf("%s has been shutdown\n",server);
			return 0;
			}
		
		virtual bool deviceCountSet(SyZmO::Client& client,const char* server
			,SyZmO::uint32_t n_devs)
			{
			printf("%s has %u devices. They are\n",server,n_devs);
			dev_count=n_devs;
			for(SyZmO::uint32_t k=0;k<n_devs;++k)
				{
				client.deviceNameRequest(server,k);
				}
			return 1;
			}
			
		virtual bool deviceAdd(SyZmO::Client& client,const char* server
			,SyZmO::uint32_t id,const char* device)
			{
			printf(" %u %s\n",id,device);
			if(id==dev_count-1)
				{return 0;}
			return 1;
			}
	private:
		SyZmO::uint32_t dev_count;
	};

int main()
	{
	SyZmO::Client::Parameters params;
	params.port_in=49152;
	params.port_out=params.port_in+1;
	params.flags=SyZmO::Client::Parameters::SERVER_ANY;

	EventHandlerTest test;
	
	SyZmO::Client client(params,test);
	client.run();

	return 0;
	}
