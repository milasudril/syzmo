#ifdef __WAND__
target[name[connection.h] type[include]]
dependency [connection.o]
#endif

#ifndef SYZMO_CLIENTSTUDIO_CONNECTION_H
#define SYZMO_CLIENTSTUDIO_CONNECTION_H

#include "../bridge/socket_datagram.h"
#include <mustudio/midi_input_exported.h>
#include <mustudio/midi_event.h>

namespace SyZmO
	{
	class Client;

	namespace ClientStudio
		{
		class MidiPump;

		class Connection
			{
			public:
				Connection(MuStudio::Client& studio_client,const char* name
					,const char* server,uint32_t device_id);

				void eventFirstGet(size_t n_frames)
					{
					event_has=port_in.eventFirstGet(event_next,n_frames);
					time_prev=0;
					}

				size_t eventTime() const
					{return event_next.time;}

				bool eventHas() const
					{return event_has;}

				void eventPost(MidiPump& pump,size_t time);

				bool match(const char* server) const;

				bool match(const char* server,uint32_t device_id) const
					{return (match(server) && m_device_id==device_id);}

			private:
				MuStudio::MIDI::InputExported port_in;
				char m_server[SocketDatagram::ADDRBUFF_LENGTH];
				uint32_t m_device_id;

				MuStudio::MIDI::Event event_next;
				size_t time_prev;
				bool event_has;
			};
		}
	}

#endif
