// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MODULE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODULE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MODULE_EXPORTS
#define MODULE_API __declspec(dllexport)
#else
#define MODULE_API __declspec(dllimport)
#endif

// This class is exported from the module.dll
class MODULE_API Cmodule {
public:
	Cmodule();
	
	~Cmodule();

	void TestAlloc();

private:
	unsigned char* mTestNew;
};

extern MODULE_API int nmodule;

MODULE_API int fnmodule(void);
