#ifdef __WAND__
target[name[midi_pump.h] type[include]]
dependency[midi_pump.o]
#endif

#ifndef SYZMO_CLIENTSTUDIO_MIDIPUMP_H
#define SYZMO_CLIENTSTUDIO_MIDIPUMP_H

#include "../bridge/message_midi.h"
#include "../bridge/socket_datagram.h"

#include <herbs/runnable/runnable.h>
#include <herbs/eventauto/eventauto.h>
#include <cstddef>
#include <deque>

namespace SyZmO
	{
	class Client;
	namespace ClientStudio
		{
		class MidiPump:public Herbs::Runnable
			{
			public:
				MidiPump(Herbs::LogWriter& writer,size_t length_buffer);
				~MidiPump();

				int run();

				void eventPost(const char* server,uint32_t device_id
					,MessageMidi msg,size_t delay);

				void sampleRateSet(double fs_new)
					{fs=fs_new;}

				void clientSet(Client& client)
					{m_client=&client;}

				void stop()
					{m_stop=1;}

				void start()
					{m_stop=0;}

				void eventsFlush()
					{data_has.set();}

			private:
				double fs;

				struct Event
					{
					char server[SocketDatagram::ADDRBUFF_LENGTH];
					uint32_t device_id;
					MessageMidi midi;
					size_t delay;
					};

				std::deque< Event> buffer;
				Herbs::EventAuto data_has;
				Client* m_client;
				bool m_stop;

			};

		}
	}

#endif
