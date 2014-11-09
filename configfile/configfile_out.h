#ifdef __WAND__
target[name[configfile_out.h] type[include]]
dependency[configfile_out.o]
#endif

#ifndef SYZMO_CONFIGFILEOUT_H
#define SYZMO_CONFIGFILEOUT_H

#include "../paramwriter.h"

namespace SyZmO
	{
	class ConfigFileOut:public ParamWriter
		{
		public:
			ConfigFileOut(const char* filename);
			~ConfigFileOut();

			void paramSet(const char* key,const char* value);

		private:
			void* dest;
		};
	}

#endif

