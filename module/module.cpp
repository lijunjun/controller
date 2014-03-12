// module.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "module.h"


// This is an example of an exported variable
MODULE_API int nmodule=0;

// This is an example of an exported function.
MODULE_API int fnmodule(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see module.h for the class definition
Cmodule::Cmodule() : mTestNew(0)
{
	return;
}

Cmodule::~Cmodule()
{
	if (mTestNew != NULL)
	{
		delete[] mTestNew;
	}

}

void Cmodule::TestAlloc()
{
	const int l64MB = 64 * 1024 * 1024;

	/*
	Heaps represent private memory managed by the user-mode heap manager and, like the Private memory type, is charged against the system commit limit and contains application data. Application memory allocations using the C runtime malloc library, HeapAlloc and LocalAlloc, use Heap memory
	*/

	// c++: new
	mTestNew = new unsigned char[l64MB];

	if (mTestNew)
	{
		memset(mTestNew, 0, l64MB);
		std::cout << &(mTestNew[0]) << std::endl;
	}
	else
		_ASSERT(0);
	
}