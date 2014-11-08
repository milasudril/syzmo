#ifdef __WAND__
target[name[parameters.h] type[include]]
dependency[parameters.o]
#endif

#ifndef SYZMO_CLIENTCGI_PARAMETERS_H
#define SYZMO_CLIENTCGI_PARAMETERS_H

#include "../buffer.h"

namespace SyZmO
	{
	namespace ClientCgi
		{
		struct Parameters
			{
			Parameters():view(16){}
			
			Buffer view;
			size_t record_begin;
			size_t record_end;
			
			bool setup_set;
			};
			
		void load(Parameters& params);
		}
	}

#endif
