#ifdef __WAND__
target[name[configfile_in.h] type[include]]
dependency[configfile_in.o]
#endif

#ifndef SYZMO_CONFIGFILEIN_H
#define SYZMO_CONFIGFILEIN_H

#include "paramreader.h"

namespace SyZmO
	{
	class Buffer;
	class ConfigFileIn:public ParamReader
		{
		public:
			ConfigFileIn(const char* filename);
			~ConfigFileIn();

			bool paramGet(Buffer& key,Buffer& value);

		private:
			void* src;
		};
	}

#endif

