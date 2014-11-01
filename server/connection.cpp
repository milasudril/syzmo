#ifdef __WAND__
target[name[connection.o] type[object]]
#endif

#include "connection.h"
#include <cstring>

SyZmO::Connection::Connection(const char* client,uint32_t device_id):
	midi_out(new MidiOut(device_id))
	{
	strncpy(m_client,client,SocketDatagram::ADDRBUFF_LENGTH);
	}

SyZmO::Connection::~Connection()
	{
	delete midi_out;
	}
	
bool SyZmO::Connection::clientMatch(const char* client) const
	{
	if(strncmp(client,m_client,SocketDatagram::ADDRBUFF_LENGTH)==0)
		{return 1;}
	return 0;
	}
