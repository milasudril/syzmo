#ifdef __WAND__
target[name[paramreader.h] type[include]]
#endif

#ifndef SYZMO_PARAMREADER_H
#define SYZMO_PARAMREADER_H

namespace SyZmO
	{
	class Buffer;
	class ParamReader
		{
		public:
			virtual bool paramGet(Buffer& key,Buffer& value)=0;
			virtual ~ParamReader(){}
		};
	}

#endif
