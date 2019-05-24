#ifndef LIBCHEN_EXCEPTION_H_
#define LIBCHEN_EXCEPTION_H_

#ifndef WIN32
	#include "Linux_Common.h"     
#endif

#include <string.h>

namespace chen{

	class exception
	{
#define LIBCHEN_EXECPTION_MAX_TITLE		30
#define LIBCHEN_EXECPTION_MAX_CONTENT	200
#define LIBCHEN_EXECPTION_DEFAULT_CONTENT	"Unknown exception"
#define LIBCHEN_EXECPTION_DEFAULT_TITLE		"Execption"
	public:
		exception(void)
		{
			strcpy(Title,LIBCHEN_EXECPTION_DEFAULT_TITLE);
			strcpy(Description,LIBCHEN_EXECPTION_DEFAULT_CONTENT);
		}
		~exception(void)
		{
			;
		}
		exception(const char *content,const char *tt=LIBCHEN_EXECPTION_DEFAULT_TITLE)
		{
			if(content)
			{
				if(strlen(content)<LIBCHEN_EXECPTION_MAX_CONTENT)
					strcpy(Description,content);

				else
					strcpy(Description,LIBCHEN_EXECPTION_DEFAULT_CONTENT);
			}
			else
				strcpy(Description,LIBCHEN_EXECPTION_DEFAULT_CONTENT);

			if(tt)
			{
				if(strlen(tt)<LIBCHEN_EXECPTION_MAX_TITLE)
					strcpy(Title,tt);
				else
					strcpy(Title,LIBCHEN_EXECPTION_DEFAULT_TITLE);
			}
			else
				strcpy(Title,LIBCHEN_EXECPTION_DEFAULT_TITLE);

		}

	public:
		char Title[LIBCHEN_EXECPTION_MAX_TITLE];
		char Description[LIBCHEN_EXECPTION_MAX_CONTENT];
	};
}

#endif
