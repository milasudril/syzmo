#ifdef __WAND__
target[name[midi_out.o] type[object] platform[;Windows] dependency[winmm;external]]
#endif

#include "midi_out.h"
#include <windows.h>

unsigned int SyZmO::MidiOut::deviceCount()
	{return midiOutGetNumDevs();}

void SyZmO::MidiOut::deviceNameGet(unsigned int id,char* name)
	{
	MIDIOUTCAPSA caps;
	midiOutGetDevCapsA(id,&caps,sizeof(caps));
	memset(name,0,DEVNAME_LENGTH*sizeof(char));
	strncpy(name,caps.szPname,DEVNAME_LENGTH);
	name[DEVNAME_LENGTH-1]=0;
	}

