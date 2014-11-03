#ifdef __WAND__
target[name[syzmo_client] type[application] platform[;GNU/Linux]]
#endif

#include "client.h"
#include "event_handler_nop.h"

#include <cstdio>

class EventHandlerTest:public SyZmO::EventHandlerNop
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
			,const SyZmO::MessageCtrl::DeviceCountResponse& message)
			{
			dev_count=message.n_devs;
			printf("%s has %u devices. They are\n",server,dev_count);
			for(SyZmO::uint32_t k=0;k<dev_count;++k)
				{
				client.deviceNameRequest(server,k);
				}
			return 1;
			}
			
		virtual bool deviceAdd(SyZmO::Client& client,const char* server
			,const SyZmO::MessageCtrl::DeviceNameResponse& message)
			{
			printf(" %u. %s %u\n",message.device_id,message.name,message.status);
			if(message.device_id==dev_count-1)
				{client.serverExitRequest(server);}
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
