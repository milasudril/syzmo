#ifdef __WAND__
target[name[midi_out.o] type[object] platform[;Windows] dependency[winmm;external]]
#endif

#include "midi_out.h"
#include "../bridge/exception_missing.h"
#include <windows.h>
#include <cassert>

SyZmO::uint32_t SyZmO::MidiOut::deviceCount()
	{return midiOutGetNumDevs();}

void SyZmO::MidiOut::deviceNameGet(uint32_t id,char* name)
	{
	MIDIOUTCAPSA caps;
	midiOutGetDevCapsA(id,&caps,sizeof(caps));
	memset(name,0,DEVNAME_LENGTH*sizeof(char));
	strncpy(name,caps.szPname,DEVNAME_LENGTH);
	name[DEVNAME_LENGTH-1]=0;
	}

SyZmO::MidiOut::MidiOut(uint32_t id)
	{
	assert(sizeof(handle)==sizeof(HMIDIOUT));
	
	uint32_t res=midiOutOpen((HMIDIOUT*)(&handle),id,0,0,CALLBACK_NULL);
	if(res!=MMSYSERR_NOERROR)
		{throw ExceptionMissing(__FILE__,__LINE__);}
	}

SyZmO::MidiOut::~MidiOut()
	{
	midiOutClose((HMIDIOUT)handle);
	}

void SyZmO::MidiOut::messageSend(MessageMidi msg_out)
	{
	msg_out.bytes[3]=0;
	midiOutShortMsg((HMIDIOUT)handle,msg_out.dword);
	}
