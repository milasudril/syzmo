#ifdef __WAND__
target[name[netresource.h] type[include]]
dependency[netresource.o]
#endif

#ifndef SYZMO_NETRESOURCE_H
#define SYZMO_NETRESOURCE_H

#include <cstddef>

namespace SyZmO
	{
	class NetResource
		{
		public:
			NetResource():n_refs(0){}
			~NetResource(){}

			class Block;

			void acquire();
			void release();

		private:
			size_t n_refs;
		};
	extern NetResource resource;

	class NetResource::Block
		{
		public:
			Block()
				{resource.acquire();}
			~Block()
				{resource.release();}
		};
	}

#endif
