#ifdef __WAND__
target[name[inttypes.h] type[include]]
#endif

#ifndef SYZMO_INTTYPES_H
#define SYZMO_INTTYPES_H

namespace SyZmO
	{
	typedef char int8_t;
	typedef unsigned char uint8_t;

	typedef short int16_t;
	typedef unsigned short uint16_t;

	typedef int int32_t;
	typedef unsigned int uint32_t;

	typedef long long int int64_t;
	typedef  unsigned long long int uint64_t;
	}

#endif
