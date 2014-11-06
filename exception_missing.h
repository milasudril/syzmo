#ifdef __WAND__
target[name[exception_missing.h] type[include]]
dependency[exception_missing.o]
#endif

#ifndef SYZMO_EXCEPTIONMISSING_H
#define SYZMO_EXCEPTIONMISSING_H

namespace SyZmO
	{
	class ExceptionMissing
		{
		public:
			ExceptionMissing(const char* file,unsigned int line);

			void print() const;

		private:
			unsigned int m_line;
			char m_file[1020];
		};
	}

#endif
