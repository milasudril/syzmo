#include <windows.h>
#include <cstdio>

int main()
	{
	SYSTEMTIME now;
	GetSystemTime(&now);
	printf("%d:%02d\n",now.wHour,now.wMinute);
	
	while(1)
		{
		GetLocalTime(&now);
		printf("%d:%02d:%02d\n",now.wHour,now.wMinute,now.wSecond);
		Sleep(1000);
		}
	return 0;
	}
