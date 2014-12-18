#ifdef __WAND__
target[name[message_ctrl.o] type[object]]
#endif

#include "message_ctrl.h"
#include <cstring>

#include <cstdio>

const unsigned int SyZmO::MessageCtrl::Midi::MESSAGE_COUNT_MAX;

void SyZmO::MessageCtrl::valuesSet(uint32_t length,const void* vals)
	{
	strncpy(header.magic.ascii,"SyZmOmsg",8);
	memcpy(data,vals,length);
	}

bool SyZmO::MessageCtrl::validIs() const
	{
	if(header.magic.value==0x67736d4f6d5a7953LLu)
		{return 1;}
	return 0;
	}
