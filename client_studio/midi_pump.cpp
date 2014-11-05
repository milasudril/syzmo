#ifdef __WAND__
target[name[midi_pump.o] type[object] platform[;GNU/Linux]]
#endif

#include "midi_pump.h"
#include "../client/client.h"

#include <cstdio>
#include <cstring>
#include <time.h>


SyZmO::ClientStudio::MidiPump::MidiPump(Herbs::LogWriter& writer
	,size_t length_buffer)
	:Herbs::Runnable(writer),m_client(nullptr),m_stop(0)
	{}


SyZmO::ClientStudio::MidiPump::~MidiPump()
	{}

void SyZmO::ClientStudio::MidiPump::eventPost(const char* server
	,uint32_t device_id,MessageMidi msg,size_t delay)
	{
	Event event_next;
	memset(&event_next,0,sizeof(event_next));
	strncpy(event_next.server,server,SocketDatagram::ADDRBUFF_LENGTH);
	event_next.server[SocketDatagram::ADDRBUFF_LENGTH-1]=0;
	event_next.device_id=device_id;
	event_next.midi=msg;
	event_next.delay=delay;
	buffer.push_back(event_next);
	}

namespace
	{
	void sleep(double time)
		{
		time_t secs=(time_t)time;
		long ns=long(1e9*(time-secs));

		timespec ts
			{
			 secs
			,ns
			};

		timespec dummy;
		nanosleep(&ts,&dummy);
		}

	}

int SyZmO::ClientStudio::MidiPump::run()
	{
	while(!m_stop)
		{
		while(!buffer.empty())
			{
			const Event& event_next=buffer.front();
			sleep(event_next.delay/fs);
			m_client->messageMidiSend(event_next.server,event_next.device_id
				,event_next.midi);
			buffer.pop_front();
			}
		data_has.wait();
		}
	return STATUS_OK;
	}