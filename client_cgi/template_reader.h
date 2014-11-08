#ifdef __WAND__
target[name[template_reader.h] type[include]]
dependency[template_reader.o]
#endif

#ifndef SYZMO_CLIENTCGI_TEMPLATEREADER_H
#define SYZMO_CLIENTCGI_TEMPLATEREADER_H

namespace SyZmO
	{
	namespace ClientCgi
		{
		class TemplateReader
			{
			public:
				class KeywordProcessor
					{
					public:
						virtual void keywordProcess(const char* word)=0;
					};
				
				TemplateReader(const char* filename,KeywordProcessor& keyp);
				~TemplateReader();
				int run();
				
			private:
				KeywordProcessor& m_keyp;
				void* handle;
			};
		}
	}

#endif
