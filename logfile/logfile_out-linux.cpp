#ifdef __WAND__
target[name[logfile_out.o] type[object] platform[;GNU/Linux]]
#endif

#include "logfile_out.h"
#include "../buffer.h"
#include "../exception_missing.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdarg>

SyZmO::LogfileOut::LogfileOut(const char* filename)
	{
	struct passwd* pw = getpwuid(getuid());
	const char* homedir = pw->pw_dir;
	if(homedir==nullptr)
		{throw ExceptionMissing(__FILE__,__LINE__);}

		{
		Buffer temp(256,homedir);
		temp.append('\\').append(filename).append(".txt").terminate();
		dest=fopen(temp.begin(),"ab");
		if(dest==NULL)
			{throw ExceptionMissing(__FILE__,__LINE__);}
		}

		{
		Buffer temp(256,homedir);
		temp.append('\\').append(filename).append(".idx").terminate();
		dest_index=fopen(temp.begin(),"ab");
		if(dest_index==NULL)
			{
			fclose((FILE*)dest);
			throw ExceptionMissing(__FILE__,__LINE__);
			}
		}
	}

SyZmO::LogfileOut::~LogfileOut()
	{
	fclose((FILE*)dest_index);
	fclose((FILE*)dest);
	}

void SyZmO::LogfileOut::entryWrite(const char* address,const char* message,...)
	{
	tm now;
	time_t t_system=time(NULL);
	gmtime_r(&t_system,&now);

	int l_tot=fprintf((FILE*)dest,"%d-%02d-%02d %d:%02d:%02d;"
		,now.tm_year+1900,now.tm_mon+1,now.tm_mday,now.tm_hour,now.tm_min
		,now.tm_sec);

	l_tot+=strlen(address)+1;
	fputs(address,(FILE*)dest);
	putc(';',(FILE*)dest);

	va_list args;
	va_start(args,message);
	l_tot+=vfprintf((FILE*)dest,message,args)+1;
	va_end(args);
	putc('\n',(FILE*)dest);
	fflush((FILE*)dest);
	
	fprintf((FILE*)dest_index,"%d ",l_tot);
	fflush((FILE*)dest_index);
	}
