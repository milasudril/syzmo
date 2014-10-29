#ifdef __WAND__
target
	[
	name[ackmessage.h] type[include]
	]
dependency[ackmessage.o]
#endif

#ifndef SYZMO_ACKMESSAGE_H
#define SYZMO_ACKMESSAGE_H

#include <cstdint>

namespace Syzmo
	{
	struct AckMessage
		{
		AckMessage(uint32_t code);
		AckMessage(uint32_t code,uint32_t param);
		AckMessage(){}

		bool validIs() const;

		char magic[8];
		uint32_t msgcode;
		uint32_t msgparam;
		};
	}

#endif
