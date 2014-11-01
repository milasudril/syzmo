#ifdef __WAND__
target[name[message_midi.h] type[include]]
#endif

#ifndef SYZMO_MESSAGEMIDI_H
#define SYZMO_MESSAGEMIDI_H

#include "inttypes.h"

namespace SyZmO
	{
	union MessageMidi
		{
		uint32_t dword;
		uint8_t bytes[4];
		};
	}

#endif
