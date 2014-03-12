// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SMARTHEAP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SMARTHEAP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SMARTHEAP_EXPORTS
#define SMARTHEAP_API __declspec(dllexport)
#else
#define SMARTHEAP_API __declspec(dllimport)
#endif

// This class is exported from the smartheap.dll

/*
* Test smart heap to figure out how the memory is managed
*/
class SMARTHEAP_API Csmartheap {
public:
	Csmartheap(void);
	~Csmartheap();

	void TestSmartHeap();

private:
	void* mTestSmrtNew;

};

extern SMARTHEAP_API int nsmartheap;

SMARTHEAP_API int fnsmartheap(void);
