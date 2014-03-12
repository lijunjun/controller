// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MEMORYMGT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MEMORYMGT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef MEMORYMGT_H
#define MEMORYMGT_H

#ifdef MEMORYMGT_EXPORTS
#define MEMORYMGT_API __declspec(dllexport)
#else
#define MEMORYMGT_API __declspec(dllimport)
#endif

/*
* simple testing overload new and delete
*/

namespace MemoryMgt
{
	class MEMORYMGT_API Cmemoryhandler
	{
	public:
		Cmemoryhandler();

		void* alloc(size_t iSize);

		void free(void* iPtr);

	private:


	};

	class MEMORYMGT_API Cmemorymgt {
	public:
		Cmemorymgt(void);
	};

}

MEMORYMGT_API void* operator new (size_t iSize, MemoryMgt::Cmemoryhandler* iMemoryHdl);

MEMORYMGT_API void operator delete(void* iPtr, MemoryMgt::Cmemoryhandler* iMemoryHdl) throw();

MEMORYMGT_API void* operator new[](size_t iSize, MemoryMgt::Cmemoryhandler* iMemoryHdl);

MEMORYMGT_API void operator delete[](void* iPtr, MemoryMgt::Cmemoryhandler* iMemoryHdl) throw();

#endif
