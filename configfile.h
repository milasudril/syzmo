#ifdef __WAND__
target[name[configfile.h] type[include]]
dependency[configfile.o]
#endif

#ifndef SYZMO_CONFIGFILE_H
#define SYZMO_CONFIGFILE_H

namespace SyZmO
	{
	class Buffer;
	class ConfigFile
		{
		public:
			ConfigFile(const char* filename);
			~ConfigFile();

			bool paramGet(Buffer& key,Buffer& value);

		private:
			void* src;
		};
	}

#endif

