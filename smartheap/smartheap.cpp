// smartheap.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "smartheap.h"

#include <stdlib.h>
#include <stdio.h>
//#include "../SmartHeap9/shmalloc.h"

// This is an example of an exported variable
SMARTHEAP_API int nsmartheap=0;

// This is an example of an exported function.
SMARTHEAP_API int fnsmartheap(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see smartheap.h for the class definition
Csmartheap::Csmartheap()
{
	return;
}

Csmartheap::~Csmartheap()
{
	if (mTestSmrtNew)
	{
		free(mTestSmrtNew);
	}
}

void Csmartheap::TestSmartHeap()
{
	const int l64MB = 64 * 1024 * 1024;
	mTestSmrtNew = malloc(l64MB);

}