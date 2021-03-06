#ifdef __WAND__
target[name[logfile_out.h] type[include]]
dependency[logfile_out.o]
#endif

#ifndef SYZMO_LOGFILEOUT_H
#define SYZMO_LOGFILEOUT_H

#include "../buffer.h"

namespace SyZmO
	{
	class LogfileOut
		{
		public:
			LogfileOut(const char* filename);
			~LogfileOut();
			void entryWrite(const char* address,const char* message,...);
			void clear();

		private:
			void* dest;
			void* dest_index;

			Buffer filename_log;
			Buffer filename_index;
		};
	}

#endif
