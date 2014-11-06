#ifdef __WAND__
target[name[buffer.h] type[include]]
dependency[buffer.o]
#endif

#ifndef SYZMO_BUFFER_H
#define SYZMO_BUFFER_H

#include <cstddef>

namespace SyZmO
	{
	class Buffer
		{
		public:
			Buffer(size_t length_init,const char* buff);
			explicit Buffer(size_t length_init);
			~Buffer();

			Buffer& append(char ch)
				{
				if(length()>=length_alloc)
					{alloc();}
				*ptr_end=ch;
				++ptr_end;
				return *this;
				}

			Buffer& append(const char* buffer);

			void terminate()
				{append('\0');}

			void clear()
				{
				*ptr_begin='\0';
				ptr_end=ptr_begin;
				}

			size_t length() const
				{return ptr_end-ptr_begin;}

			const char* begin() const
				{return ptr_begin;}

			const char* end() const
				{return ptr_end;}

		private:
			/**Hopefully not needed.
			*/
			Buffer(const Buffer& buffer);

			/**Hopefully not needed.
			*/
			Buffer& operator=(const Buffer& buffer);

			char* ptr_begin;
			char* ptr_end;
			size_t length_alloc;

			void alloc();
		};
	}
#endif
