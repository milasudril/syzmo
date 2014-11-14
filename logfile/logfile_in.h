#ifdef __WAND__
target[name[logfile_in.h] type[include]]
dependency[logfile_in.o]
#endif

#ifndef SYZMO_LOGFILEIN_H
#define SYZMO_LOGFILEIN_H

#include <cstddef>

namespace SyZmO
	{
	class LogfileIn
		{
		public:
			class FieldPrinter
				{
				public:
					virtual void tableBegin()=0;
					virtual void tableEnd()=0;
					virtual void rowBegin()=0;
					virtual void rowEnd();
					virtual void fieldPrint(const char* value)=0;
				};
			
			LogfileIn(const char* filename,FieldPrinter& printer);
			~LogfileIn();
			
			bool recordGet(size_t index);
			
			size_t recordsCount() const
				{return record_count;}
			
		private:
			FieldPrinter& m_printer;
			size_t* record_index;
			size_t record_count;
			void* src;
		};
	}

#endif
