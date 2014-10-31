#ifdef __WAND__
target[name[message_ctrl.o] type[object]]
#endif

#include "message_ctrl.h"
#include <ctime>
#include <cstring>

void SyZmO::MessageCtrl::valuesSet(uint32_t length,const void* vals)
	{
	strncpy(magic.ascii,"SyZmOmsg",8);
	memcpy(data,vals,length);
	timestamp=time(NULL);
	}
