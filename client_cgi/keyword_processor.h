#ifdef __WAND__
target[name[keyword_processor.h] type[include]]
dependency[keyword_processor.o]
#endif

#ifndef SYZMO_CLIENTCGI_KEYWORDPROCESSOR_H
#define SYZMO_CLIENTCGI_KEYWORDPROCESSOR_H

#include "template_reader.h"
#include "log_fieldprinter.h"

namespace SyZmO
	{
	class LogfileIn;
	class Client;

	namespace ClientCgi
		{
		class EventHandler;
		class Parameters;

		class KeywordProcessor:public TemplateReader::KeywordProcessor
			{
			public:
				KeywordProcessor(Client& client,EventHandler& eh
					,Parameters& params);

				void keywordProcess(const char* word);

			private:
				Client& m_client;
				ClientCgi::EventHandler& m_eh;
				ClientCgi::Parameters& m_params;
				LogfileIn* logfile;
				LogFieldprinter log_fieldprinter;
			};
		}
	}

#endif
