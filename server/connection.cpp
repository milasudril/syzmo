#ifdef __WAND__
target[name[connection.o] type[object]]
#endif

#include "connection.h"
#include <cstring>

SyZmO::Connection::Connection(const char* client,uint32_t device_id):
	midi_out(device_id)
	{
	memset(m_client,0,SocketDatagram::ADDRBUFF_LENGTH);
	strncpy(m_client,client,SocketDatagram::ADDRBUFF_LENGTH);
	m_client[SocketDatagram::ADDRBUFF_LENGTH-1]=0;
	}

bool SyZmO::Connection::clientMatch(const char* client) const
	{
	if(strcmp(client,m_client)==0)
		{return 1;}
	return 0;
	}
