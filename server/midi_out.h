#ifdef __WAND__
target[name[midi_out.h] type[include]]
dependency[midi_out.o]
#endif

#ifndef SYZMO_MIDIOUT_H
#define SYZMO_MIDIOUT_H

#include <cstddef>

namespace SyZmO
	{
	class MidiOut
		{
		public:
			static const size_t DEVNAME_LENGTH=32;
			static unsigned int deviceCount();
			static void deviceNameGet(unsigned int id,char* name);
		};
	}

#endif
