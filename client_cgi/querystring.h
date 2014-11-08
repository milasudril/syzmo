#ifdef __WAND__
target[name[querystring.h] type[include]]
dependency[querystring.o]
#endif

#ifndef SYZMO_CLIENTCGI_QUERYSTRING_H
#define SYZMO_CLIENTCGI_QUERYSTRING_H

#include "../paramreader.h"

namespace SyZmO
	{
	class Buffer;
	namespace ClientCgi
		{
		class QueryString:public ParamReader
			{
			public:
				QueryString();
				bool paramGet(Buffer& key,Buffer& value);
			private:
				const char* ptr;
			};
		}
	}
	
#endif
