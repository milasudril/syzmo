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
					virtual void rowEnd()=0;
					virtual void fieldPrint(const char* value)=0;
					virtual void fieldHeaderPrint(const char* value)=0;
				};

			LogfileIn(const char* filename);
			~LogfileIn();

			void recordGet(size_t index,FieldPrinter& printer);

			size_t recordsCount() const
				{return record_count;}

		private:
			size_t* record_index;
			size_t record_count;
			void* src;
		};
	}

#endif
