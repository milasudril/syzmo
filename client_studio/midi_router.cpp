#ifdef __WAND__
target[name[midi_router.o] type[object]]
#endif

#include "midi_router.h"
#include "connection.h"
#include <herbs/thread/thread.h>

#include <cstdio>

SyZmO::ClientStudio::MidiRouter::MidiRouter(Herbs::LogWriter& writer
	,size_t length_buffer):
	MuStudio::Client("SyZmO Client"),pump(writer,length_buffer)
	{
	activate();
	pump.sampleRateSet(sampleRateGet());
	}

SyZmO::ClientStudio::MidiRouter::~MidiRouter()
	{
	deactivate();
	auto ptr=connections.begin();
	while(ptr!=connections.end())
		{
		if(*ptr!=nullptr)
			{delete *ptr;}
		++ptr;
		}
	}

int SyZmO::ClientStudio::MidiRouter::onProcess(size_t n_frames)
	{
	auto ptr=connections.begin();
	size_t t=0;

	while(ptr!=connections.end())
		{
		if(*ptr!=nullptr)
			{(*ptr)->eventFirstGet(n_frames);}
		++ptr;
		}

	while(n_frames)
		{
		ptr=connections.begin();
		while(ptr!=connections.end())
			{
			if(*ptr!=nullptr)
				{(*ptr)->eventPost(pump,t);}
			++ptr;
			}
		--n_frames;
		++t;
		}
	pump.eventsFlush();
	return 0;
	}

int SyZmO::ClientStudio::MidiRouter::onSampleRateChanged(size_t fs_new)
	{
	pump.sampleRateSet(fs_new);
	return 0;
	}

void SyZmO::ClientStudio::MidiRouter::connectionsIsAliveRequest(SyZmO::Client& client)
	{
	auto ptr=connections.begin();
	while(ptr!=connections.end())
		{
		if(*ptr!=nullptr)
			{
			if(!(*ptr)->isAliveRequest(client))
				{
				deactivate();
				delete *ptr;
				*ptr=nullptr;
				activate();
				}
			}
		++ptr;
		}
	}
void SyZmO::ClientStudio::MidiRouter::connectionsIsAlive(const char* server)
	{
	auto ptr=connections.begin();
	while(ptr!=connections.end())
		{
		if(*ptr!=nullptr)
			{
			if((*ptr)->match(server))
				{
				(*ptr)->isAlive();
				}
			}
		++ptr;
		}
	}

void SyZmO::ClientStudio::MidiRouter::connectionAdd(const char* name
	,const char* server,uint32_t device_id)
	{
	deactivate();
	auto connection=new Connection(*this,name,server,device_id);
	auto ptr=connections.begin();
	while(ptr!=connections.end())
		{
		if(*ptr==nullptr)
			{
			*ptr=connection;
			activate();
			return;
			}
		++ptr;
		}
	connections.append(connection);
	activate();
	}

void SyZmO::ClientStudio::MidiRouter::connectionsRemove(const char* server)
	{
	deactivate();
	auto ptr=connections.begin();
	while(ptr!=connections.end())
		{
		if(*ptr!=nullptr)
			{
			if((*ptr)->match(server))
				{
				delete *ptr;
				*ptr=nullptr;
				return;
				}
			}
		++ptr;
		}
	activate();
	}

void SyZmO::ClientStudio::MidiRouter::connectionRemove(const char* server
	,uint32_t device_id)
	{
	deactivate();
	auto ptr=connections.begin();
	while(ptr!=connections.end())
		{
		if(*ptr!=nullptr)
			{
			if((*ptr)->match(server,device_id))
				{
				delete *ptr;
				*ptr=nullptr;
				return;
				}
			}
		++ptr;
		}
	activate();
	}

void SyZmO::ClientStudio::MidiRouter::pumpStop()
	{
	pump.stop();
	delete pump_thread;
	}

void SyZmO::ClientStudio::MidiRouter::pumpStart()
	{
	pump.start();
	pump_thread=new Herbs::Thread(pump);
	}
