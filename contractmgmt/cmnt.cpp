#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>

#include "controller.h"
#include "../module/module.h"
#include "../smartheap/smartheap.h"
#include "../memorymgt/memorymgt.h"

typedef BOOL(FAR WINAPI *PARPGetProcessMemoryInfo)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);
typedef BOOL(FAR WINAPI *PARPEnumProcesses)(DWORD*, DWORD, DWORD*);
typedef BOOL(FAR WINAPI *PARPEnumProcessModules)(HANDLE, HMODULE*, DWORD, LPDWORD);
typedef BOOL(FAR WINAPI *PARPGetModuleBaseName)(HANDLE, HMODULE, LPTSTR, DWORD);


void TestNTEnv()
{
	HMODULE lPsAPI = ::LoadLibrary(L"psapi.dll");
	PARPGetProcessMemoryInfo lpGetProcessMemoryInfo;
	PARPEnumProcesses lpEnumProcesses;
	PARPEnumProcessModules lpEnumProcessModules;
	PARPGetModuleBaseName lpGetModuleBaseName;

	if (lPsAPI == NULL)
	{
		return;
	}

	lpGetProcessMemoryInfo = (PARPGetProcessMemoryInfo)::GetProcAddress(lPsAPI, "GetProcessMemoryInfo");
	lpEnumProcesses = (PARPEnumProcesses)GetProcAddress(lPsAPI, "EnumProcesses");
	lpEnumProcessModules = (PARPEnumProcessModules)GetProcAddress(lPsAPI, "EnumProcessModules");
	lpGetModuleBaseName = (PARPGetModuleBaseName)GetProcAddress(lPsAPI, "GetModuleBaseNameW");

	if (lpGetProcessMemoryInfo == NULL ||
		lpEnumProcesses == NULL ||
		lpEnumProcessModules == NULL ||
		lpGetModuleBaseName == NULL)
	{
		::FreeLibrary(lPsAPI);
		return;
	}

	// Memory Status
	MEMORYSTATUSEX lMemoryStatus;
	lMemoryStatus.dwLength = sizeof(lMemoryStatus);
	bool lRet = ::GlobalMemoryStatusEx(&lMemoryStatus);
	double l1MB = 1 * 1024 * 1024;
	if (lRet)
	{
		
		std::cout << "memory load: " << lMemoryStatus.dwMemoryLoad
			<< "\ntotal phys: " << lMemoryStatus.ullTotalPhys / l1MB
			<< "\navail phys: " << lMemoryStatus.ullAvailPhys / l1MB
			<< "\ntotal page file: " << lMemoryStatus.ullTotalPageFile / l1MB
			<< "\navail page file: " << lMemoryStatus.ullAvailPageFile / l1MB
			<< "\ntotal virtual: " << lMemoryStatus.ullTotalVirtual / l1MB
			<< "\navail virtual: " << lMemoryStatus.ullAvailVirtual / l1MB
			<< "\navail ex virtual: " << lMemoryStatus.ullAvailExtendedVirtual << "\n\n";
	}

	// Process Memory Counters
	PROCESS_MEMORY_COUNTERS lMemoryCounters;
	lRet = ::GetProcessMemoryInfo(GetCurrentProcess(), &lMemoryCounters, sizeof(lMemoryCounters));
	if (lRet)
	{
		std::cout << "page fault: " << lMemoryCounters.PageFaultCount
			<< "\npeak working set: " << lMemoryCounters.PeakWorkingSetSize / l1MB
			<< "\ncurrent working set: " << lMemoryCounters.WorkingSetSize / l1MB
			<< "\npeak paged pool: " << lMemoryCounters.QuotaPeakPagedPoolUsage / l1MB
			<< "\npaged pool: " << lMemoryCounters.QuotaPagedPoolUsage / l1MB
			<< "\npeak non paged pool: " << lMemoryCounters.QuotaPeakNonPagedPoolUsage / l1MB
			<< "\nnon paged pool: " << lMemoryCounters.QuotaNonPagedPoolUsage / l1MB
			<< "\npeak page file usage: " << lMemoryCounters.PeakPagefileUsage / l1MB
			<< "\npage file usage: " << lMemoryCounters.PagefileUsage / l1MB << "\n\n";
	}

	// Get largest free memory without 3GB opened
	unsigned int iBegin = 0x00010000;
	unsigned int iEnd = 0x7fff0000;
	MEMORY_BASIC_INFORMATION lMemoryBasicInfo;
	MEMORY_BASIC_INFORMATION lLargestMemoryBasicInfo;
	lLargestMemoryBasicInfo.RegionSize = 0;
	
	// DO NOT use LPCVOID
	/*
	typedef void far            *LPVOID;
	typedef CONST void far      *LPCVOID;
	*/
	LPVOID iIter = (LPVOID)iBegin;
	while ((DWORD)iIter < iEnd)
	{
		::VirtualQuery(iIter, &lMemoryBasicInfo, sizeof(lMemoryBasicInfo));
		if (lMemoryBasicInfo.State == MEM_FREE && lLargestMemoryBasicInfo.RegionSize < lMemoryBasicInfo.RegionSize)
		{
			lLargestMemoryBasicInfo = lMemoryBasicInfo;
			// wait too long, just break
			//break;
		}
		iIter = (LPVOID)((DWORD)lMemoryBasicInfo.BaseAddress + (DWORD)lMemoryBasicInfo.RegionSize);
	}
	std::cout << "largest memory: \nregion size: " << lLargestMemoryBasicInfo.RegionSize / l1MB
		<< "\nstate: " << std::hex << lLargestMemoryBasicInfo.State
		<< "\nprotect: " << std::hex << lLargestMemoryBasicInfo.Protect
		<< "\ntype: " << std::hex << lLargestMemoryBasicInfo.Type << "\n";



	::FreeLibrary(lPsAPI);
}

void TestMemoryAlloc()
{
	const int l64MB = 512 * 1024 * 1024;

	/*
	Heaps represent private memory managed by the user-mode heap manager and, like the Private memory type, is charged against the system commit limit and contains application data. Application memory allocations using the C runtime malloc library, HeapAlloc and LocalAlloc, use Heap memory
	*/

	// c++: new
	unsigned char* lTestNew = new unsigned char[l64MB];

	if (lTestNew)
	{
		memset(lTestNew, 0, l64MB);
		std::cout << &(lTestNew[0]) << std::endl;
	}
	else
		_ASSERT(0);

	// C: malloc
	unsigned char* lTestMalloc = (unsigned char*)malloc(l64MB);
	if (lTestMalloc != NULL)
	{
		std::cout << lTestMalloc << std::endl;
	}

	// DLL: new
	Cmodule lTestDLL;
	lTestDLL.TestAlloc();

	// DLL: smartheap new
	Csmartheap lTestSmartHeap;
	lTestSmartHeap.TestSmartHeap();

	unsigned int lTestVirtualAllocSize = 128 * 1024 * 1024;
	// Virutal Address Space
	/*
	Private memory is memory allocated by VirtualAlloc and not suballocated either by the Heap Manager or the .NET run time. It cannot be shared with other processes, is charged against the system commit limit, and typically contains application data.
	*/
	LPVOID lTestVirtualAlloc = ::VirtualAlloc(NULL, lTestVirtualAllocSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (lTestVirtualAlloc != NULL)
	{
		std::cout << lTestVirtualAlloc << std::endl;
	}

	HANDLE lHeapHandle = ::GetProcessHeap();
	_ASSERT(lHeapHandle != NULL);
	LPVOID lTestHeapAlloc = ::HeapAlloc(lHeapHandle, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, l64MB);
	if (lTestHeapAlloc != NULL)
	{
		std::cout << lTestHeapAlloc << std::endl;
	}

	CONSOLE_WAIT("end")

	if (lTestVirtualAlloc != NULL)
	{
		::VirtualFree(lTestVirtualAlloc, lTestVirtualAllocSize, MEM_RELEASE);
	}
	if (lTestNew != NULL)
	{
		delete[] lTestNew;
	}
	if (lTestHeapAlloc != NULL)
	{
		::HeapFree(lHeapHandle, HEAP_NO_SERIALIZE, lTestHeapAlloc);
	}
	if (lTestMalloc != NULL)
	{
		free(lTestMalloc);
	}
}

void TestMemoryMgt()
{
	MemoryMgt::Cmemoryhandler lCmemoryhandler;

	const int l128MB = 128 * 1024 * 1024;
	int* lTestMemoryHandler = new (&lCmemoryhandler) int[l128MB];
	if (lTestMemoryHandler)
	{
		std::cout << lTestMemoryHandler << std::endl;
	}

	int* lTestDefaultMemory = ::new int[l128MB];
	if (lTestDefaultMemory)
	{
		std::cout << lTestDefaultMemory << std::endl;
	}

	CONSOLE_WAIT("end")

	if (lTestMemoryHandler)
	{
		operator delete (lTestMemoryHandler, &lCmemoryhandler);
	}
	if (lTestDefaultMemory)
	{
		::delete[] lTestDefaultMemory;
	}
}

int main()
{
	TestMemoryMgt();
	//TestMemoryAlloc();

	//TestNTEnv();

	system("pause");
	return 1;
}