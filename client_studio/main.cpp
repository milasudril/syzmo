#ifdef __WAND__
target[name[syzmo_client_studio] type[application] platform[;GNU/Linux]]
#endif

#include "midi_router.h"
#include "event_handler.h"
#include "../client/client.h"
#include "../configfile.h"
#include "../buffer.h"
#include <herbs/logwriterdefault/logwriterdefault.h>
#include <herbs/thread/thread.h>

#include <cstring>

int main()
	{
	Herbs::LogWriterDefault logwriter;

	SyZmO::ClientStudio::MidiRouter router(logwriter);

	SyZmO::ClientStudio::EventHandler handler(router);

	SyZmO::Client::Parameters params;
	SyZmO::ConfigFile config("syzmo_config_client.txt");
	SyZmO::Buffer key(16);
	SyZmO::Buffer value(16);
	while(config.paramGet(key,value))
		{
		if(strcmp(key.begin(),"port_in")==0)
			{params.port_in=atol(value.begin());}
		else
		if(strcmp(key.begin(),"port_out")==0)
			{params.port_out=atol(value.begin());}
		}

	params.flags=SyZmO::Client::Parameters::SERVER_ANY;

	SyZmO::Client client(params,handler);
	router.pumpClientSet(client);
	router.pumpStart();
	client.run();


	return 0;
	}
