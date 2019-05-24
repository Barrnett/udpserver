#ifndef	LIBCHEN_MEMCACHE_H_
#define LIBCHEN_MEMCACHE_H_

#ifndef WIN32
	#include "Linux_Common.h"     
#endif

#define MAX_MEMCACHE_SIZE		0xFFFFFFFE

namespace chen{

	class MemCache
	{
	public:
		MemCache(void);
		~MemCache(void);
		MemCache(unsigned int size);
		MemCache(const MemCache &mc);
		MemCache &operator=(const MemCache &mc);
		bool AllocMem(unsigned int size);
		char *GetRawMemory();
		void ZeroMem(unsigned int size=0);

		unsigned int Length();
		void RemoveAll();
		bool ReAllocMem(unsigned int size);

		operator char *();
		operator const char *() const;
		operator void *();

	protected:
		char *m_pData;
		unsigned int m_size;

	};
}

#endif
