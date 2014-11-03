#ifdef __WAND__
target[name[syzmo_client_studio] type[application] platform[;GNU/Linux]]
#endif

#include "midi_router.h"
#include "event_handler.h"
#include "../client/client.h"
#include <herbs/logwriterdefault/logwriterdefault.h>
#include <herbs/thread/thread.h>

int main()
	{
	Herbs::LogWriterDefault logwriter;

	SyZmO::ClientStudio::MidiRouter router(logwriter,65536);

	SyZmO::ClientStudio::EventHandler handler(router);

	SyZmO::Client::Parameters params;
	params.port_in=49152;
	params.port_out=params.port_in+1;
	params.flags=SyZmO::Client::Parameters::SERVER_ANY;

	SyZmO::Client client(params,handler);
	router.pumpClientSet(client);
	router.pumpStart();
	client.run();


	return 0;
	}
