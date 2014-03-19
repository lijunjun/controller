// malloc.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "malloc.h"
#include <iostream>
#include <new.h>

//void * operator new( size_t cb )
//{
//void *res;
//
//for (;;) {
//
////  allocate memory block
//res = _heap_alloc(cb);
//
////  if successful allocation, return pointer to memory
//
//if (res)
//break;
//
////  call installed new handler
//if (!_callnewh(cb))
//break;
//
////  new handler was successful -- try to allocate again
//}
//
//RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));
//
//return res;
//}
//#else  /* _SYSCRT */
//
//#include <cstdlib>
//#include <new>
//
//_C_LIB_DECL
//int __cdecl _callnewh(size_t size) _THROW1(_STD bad_alloc);
//_END_C_LIB_DECL
//
//void *__CRTDECL operator new(size_t size) _THROW1(_STD bad_alloc)
//{       // try to allocate size bytes
//	74AE232B  jmp         0A90BF60
//		74AE2330  sub         esp, 10h
//		void *p;
//	while ((p = malloc(size)) == 0)
//		74AE2333  push        dword ptr[ebp + 8]
//		74AE2336  call        malloc(74AE0233h)
//		74AE233B  pop         ecx
//		74AE233C  test        eax, eax
//		74AE233E  je          operator new + 0Ah (74B0F2A5h)
//}
//
//return (p);
//	}
//	74AE2344  leave
//		74AE2345  ret

namespace MemoryMgt
{
	Cmemoryhandler::Cmemoryhandler()
	{

	}

	void* Cmemoryhandler::alloc(size_t iSize)
	{
		return ::VirtualAlloc(0, iSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}

	void Cmemoryhandler::free(void* iPtr)
	{
		::VirtualFree(iPtr, 0, MEM_RELEASE);
	}

}

MEMORYMGT_API void* operator new (size_t iSize, MemoryMgt::Cmemoryhandler* iMemoryHdl)
{
	return iMemoryHdl != NULL ? iMemoryHdl->alloc(iSize) : ::operator new (iSize);
}

MEMORYMGT_API void operator delete(void* iPtr, MemoryMgt::Cmemoryhandler* iMemoryHdl) throw()
{
	if (iMemoryHdl != NULL)
	{
		iMemoryHdl->free(iPtr);
	}
	else
		delete iPtr;
}

MEMORYMGT_API void* operator new[](size_t iSize, MemoryMgt::Cmemoryhandler* iMemoryHdl)
{
	return iMemoryHdl != NULL ? iMemoryHdl->alloc(iSize) : ::operator new[] (iSize);
}

MEMORYMGT_API void operator delete[](void* iPtr, MemoryMgt::Cmemoryhandler* iMemoryHdl) throw()
{
	if (iMemoryHdl != NULL)
	{
		iMemoryHdl->free(iPtr);
	}
	else
		delete [] iPtr;
}

