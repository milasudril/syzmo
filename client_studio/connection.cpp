#ifdef __WAND__
target[name[connection.o] type[object]]
#endif

#include "connection.h"
#include "midi_pump.h"
#include "../client/client.h"
#include <mustudio/midi_event.h>
#include <cstring>

SyZmO::ClientStudio::Connection::Connection(MuStudio::Client& studio_client
	,const char* name,const char* server,uint32_t device_id):
	port_in(studio_client,name)
	{
	memset(m_server,0,SyZmO::SocketDatagram::ADDRBUFF_LENGTH*sizeof(char));
	strncpy(m_server,server,SyZmO::SocketDatagram::ADDRBUFF_LENGTH);
	m_server[SyZmO::SocketDatagram::ADDRBUFF_LENGTH-1]=0;
	m_device_id=device_id;
	time_prev=0;
	}

bool SyZmO::ClientStudio::Connection::match(const char* server) const
	{
	return strcmp(m_server,server)==0;
	}

void SyZmO::ClientStudio::Connection::eventPost(MidiPump& pump,size_t time)
	{
	if(time>=event_next.time && event_has)
		{
		MessageMidi msg;
		msg.dword=event_next.data.dwords[0];
		pump.eventPost(m_server,m_device_id,msg,event_next.time-time_prev);
		time_prev=time;
		event_has=port_in.eventNextGet(event_next);
		}
	}