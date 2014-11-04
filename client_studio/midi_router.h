#ifdef __WAND__
target[name[midi_router.h] type[include]]
dependency[midi_router.o]
#endif

#ifndef SYZMO_CLIENTSTUDIO_MIDIROUTER_H
#define SYZMO_CLIENTSTUDIO_MIDIROUTER_H

#include "midi_pump.h"
#include <mustudio/client.h>
#include <herbs/array/array.h>

namespace Herbs
	{
	class Thread;
	}

namespace SyZmO
	{
	namespace ClientStudio
		{
		class Connection;

		class MidiRouter:public MuStudio::Client
			{
			public:
				MidiRouter(Herbs::LogWriter& writer,size_t length_buffer);
				~MidiRouter();

				int onProcess(size_t n_frames);
				int onSampleRateChanged(size_t fs_new);

				void connectionAdd(const char* name,const char* server
					,uint32_t device_id);
				void connectionsRemove(const char* server);
				void connectionRemove(const char* server,uint32_t device_id);
				void connectionsIsAliveRequest(SyZmO::Client& client);
				void connectionsIsAlive(const char* server);

				void pumpClientSet(SyZmO::Client& client_new)
					{pump.clientSet(client_new);}
				void pumpStop();
				void pumpStart();

			private:
				Herbs::Array<Connection*> connections;
				MidiPump pump;
				Herbs::Thread* pump_thread;
			};
		}
	}

#endif
