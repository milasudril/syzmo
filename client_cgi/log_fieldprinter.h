#ifdef __WAND__
target[name[log_fieldprinter.h] type[include]]
dependency[log_fieldprinter.o]
#endif

#ifndef SYZMO_CLIENTCGI_LOGFIELDPRINTER_H
#define SYZMO_CLIENTCGI_LOGFIELDPRINTER_H

#include "../logfile/logfile_in.h"

namespace SyZmO
	{
	namespace ClientCgi
		{
		class LogFieldprinter:public LogfileIn::FieldPrinter
			{
			public:
				void tableBegin();
				void tableEnd();
				void rowBegin();
				void rowEnd();
				void fieldPrint(const char* value);
				void fieldPrint(const char* value,const char* classname);
				void fieldHeaderPrint(const char* value);

			};
		}
	}

#endif
