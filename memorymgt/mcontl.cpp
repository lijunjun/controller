#include "stdafx.h"
#include "mcontl.h"


namespace MemoryMgt
{
	CMemorymgt* CMemorymgt::sMemorymgt = NULL;
	IPC::CCriticalSection CMemorymgt::mCS;

	CMemorymgt* CMemorymgt::CreateInstace()
	{
		mCS.Enter();
		if (sMemorymgt == NULL)
		{
			sMemorymgt = new CMemorymgt();
		}
		mCS.Leave();
		return sMemorymgt;
	}

	void CMemorymgt::DeleteInstance()
	{
		mCS.Enter();
		if (sMemorymgt != NULL)
		{
			delete sMemorymgt;
		}
		sMemorymgt = NULL;
		mCS.Leave();
	}

	CMemorymgt::CMemorymgt() : mHeapHandleCount(256),
		mHeapHandle(mHeapHandleCount)
	{

	}

	CMemorymgt::~CMemorymgt()
	{

	}

	/*
	* http://msdn.microsoft.com/en-us/library/windows/desktop/aa366750(v=vs.85).aspx
	*/
	void CMemorymgt::EnableLFH()
	{
		// get default heaps
		while (true)
		{
			DWORD lTotalHeapNum = ::GetProcessHeaps(mHeapHandleCount, mHeapHandle.begin());
			if (lTotalHeapNum == 0)
			{
				return;
			}
			if (lTotalHeapNum != mHeapHandleCount)
			{
				mHeapHandleCount = lTotalHeapNum;
				mHeapHandle.reset(mHeapHandleCount);
			}
			else 
				break;
		}
		
		// enable low fragmentation heap
		unsigned long lHeapFragValue = 2;
		for (int iter = 0; iter < mHeapHandleCount; ++iter)
		{
			if (!HeapSetInformation(
				mHeapHandle[iter],
				HeapCompatibilityInformation,
				&lHeapFragValue,
				sizeof(lHeapFragValue)
				))
			{

			}
		}

	}

}