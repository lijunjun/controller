// mmgttest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include "../../memorymgt/mcontl.h"

TEST(MMGTTest)
{
	MemoryMgt::CMemorymgt* lMemorymgt = MemoryMgt::CMemorymgt::CreateInstace();

	// enable low fragmentation heap
	lMemorymgt->EnableLFH();

	// allocate some space from heap
	const int size = 16 * 1024 * 1024;
	void* lpHeapSpace = ::HeapAlloc(
		GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size
		);
	if (lpHeapSpace == NULL)
	{
		return;
	}

	char continuec;
	std::cin >> continuec;

	::HeapFree(GetProcessHeap(), 0, lpHeapSpace);
}
