
#ifndef MCONTL_H
#define MCONTL_H

#include "config.h"
#include "../auto/atuo_buffer.h"
#include "../ipc/critical_section.h"

namespace MemoryMgt
{
	class MEMORYMGT_API CMemorymgt {
	public:
		static CMemorymgt* CreateInstace();
		static void DeleteInstance();

		// enable windows low fragmentation heap
		void EnableLFH();

	private:
		CMemorymgt();
		~CMemorymgt();

	private:
		int mHeapHandleCount;
		Auto::auto_buffer<HANDLE> mHeapHandle;

		static IPC::CCriticalSection mCS;
		static CMemorymgt* sMemorymgt;
	};
}

#endif
