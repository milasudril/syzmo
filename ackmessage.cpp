#ifdef __WAND__
target[name[ackmessage.o] type[object]]
#endif

#include "ackmessage.h"
#include <cstring>

Syzmo::AckMessage::AckMessage(uint32_t code)
	{
	strcpy(magic,"syzmoack");
	msgcode=code;
	msgparam=0;
	}

Syzmo::AckMessage::AckMessage(uint32_t code,uint32_t param)
	{
	strcpy(magic,"syzmoack");
	msgcode=code;
	msgparam=param;
	}


bool Syzmo::AckMessage::validIs() const
	{
	if(strncmp(magic,"syzmoack",8)==0)
		{return 1;}
	return 0;
	}