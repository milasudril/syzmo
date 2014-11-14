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
			size_t page;
			size_t device;

			size_t action;
			static const size_t ACTION_NORMAL=0;
			static const size_t ACTION_SETUP=1;
			static const size_t ACTION_SHUTDOWN=2;
			static const size_t ACTION_REBOOT=3;
			static const size_t ACTION_TEST=4;
			};

		void load(Parameters& params);
		}
	}

#endif
