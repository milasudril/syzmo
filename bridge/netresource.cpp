#ifdef __WAND__
target[name[netresource.o] type[object] platform[;Windows]
dependency[ws2_32;external]]
#endif

#include "netresource.h"

#include <winsock2.h>
#include <windows.h>

SyZmO::NetResource SyZmO::resource;

void SyZmO::NetResource::acquire()
	{
	if(n_refs==0)
		{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		}
	++n_refs;
	}

void SyZmO::NetResource::release()
	{
	--n_refs;
	if(n_refs==0)
		{
		WSACleanup();
		}
	}
