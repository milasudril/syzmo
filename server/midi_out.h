#ifdef __WAND__
target[name[midi_out.h] type[include]]
dependency[midi_out.o]
#endif

#ifndef SYZMO_MIDIOUT_H
#define SYZMO_MIDIOUT_H

#include "../bridge/message_midi.h"
#include <cstddef>

namespace SyZmO
	{
	class MidiOut
		{
		public:
			static const size_t DEVNAME_LENGTH=32;
			static uint32_t deviceCount();
			static void deviceNameGet(unsigned int id,char* name);
			
			MidiOut(uint32_t id);
			~MidiOut();
			void messageSend(MessageMidi msg_out);
			
		private:
			MidiOut& operator=(const MidiOut&);
			MidiOut(const MidiOut&);
		
			void* handle;
		};
	}

#endif
