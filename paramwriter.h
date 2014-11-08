#ifdef __WAND__
target[name[paramwriter.h] type[include]]
#endif

#ifndef SYZMO_PARAMWRITER_H
#define SYZMO_PARAMWRITER_H

namespace SyZmO
	{
	class ParamWriter
		{
		public:
			virtual void paramSet(const char* key,const char* value)=0;
			virtual ~ParamWriter(){}
		};
	}

#endif
