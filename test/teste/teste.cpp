// teste.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "../../log/message.h"

/*
*	support dll based testing such as: teste.ext TestModule1.dll TestCase1
*	only support single module testing
*/

// begin: these API is coming from gtest.cc
enum STDColor {
	COLOR_DEFAULT,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW
};

WORD GetColorAttribute(STDColor color) {
	switch (color) {
	case COLOR_RED:    return FOREGROUND_RED;
	case COLOR_GREEN:  return FOREGROUND_GREEN;
	case COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
	default:           return 0;
	}
}

void ColoredPrintf(STDColor color, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Gets the current text color.
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
	const WORD old_color_attrs = buffer_info.wAttributes;

	// We need to flush the stream buffers into the console before each
	// SetConsoleTextAttribute call lest it affect the text that is already
	// printed but has not yet reached the console.
	fflush(stdout);
	SetConsoleTextAttribute(stdout_handle,
		GetColorAttribute(color) | FOREGROUND_INTENSITY);
	vprintf(fmt, args);

	fflush(stdout);
	// Restores the text color.
	SetConsoleTextAttribute(stdout_handle, old_color_attrs);

	va_end(args);
}

// unicode
void ColoredWPrintf(STDColor color, const wchar_t* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Gets the current text color.
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
	const WORD old_color_attrs = buffer_info.wAttributes;

	// We need to flush the stream buffers into the console before each
	// SetConsoleTextAttribute call lest it affect the text that is already
	// printed but has not yet reached the console.
	fflush(stdout);
	SetConsoleTextAttribute(stdout_handle,
		GetColorAttribute(color) | FOREGROUND_INTENSITY);
	vwprintf(fmt, args);

	fflush(stdout);
	// Restores the text color.
	SetConsoleTextAttribute(stdout_handle, old_color_attrs);

	va_end(args);
}

// end

typedef struct _testingenv
{
	std::vector<std::string> mTestingMoudle;
	std::string mTestingCase;
} testingenv;
testingenv gtestingenv;

bool hanldeArguments(int argc, char **argv)
{
	if (argc < 2)
	{
		ColoredPrintf(COLOR_RED, "%s", "[parameters error] ");
		ColoredPrintf(COLOR_YELLOW, "%s", "teste.exe module.dll [testcase]");
		return false;
	}
	if (argc > 3)
	{
		ColoredPrintf(COLOR_YELLOW, "%s", "[parameters warning] only supports two paramters and extra ones will be ignored\n");
	}
	gtestingenv.mTestingMoudle.push_back(argv[1]);

	/*
	Running a Subset of the Tests

	By default, a Google Test program runs all tests the user has defined. Sometimes, you want to run only a subset of the tests (e.g. for debugging or quickly verifying a change). If you set the GTEST_FILTER environment variable or the --gtest_filter flag to a filter string, Google Test will only run the tests whose full names (in the form of TestCaseName.TestName) match the filter.

	The format of a filter is a ':'-separated list of wildcard patterns (called the positive patterns) optionally followed by a '-' and another ':'-separated pattern list (called the negative patterns). A test matches the filter if and only if it matches any of the positive patterns but does not match any of the negative patterns.

	A pattern may contain '*' (matches any string) or '?' (matches any single character). For convenience, the filter '*-NegativePatterns' can be also written as '-NegativePatterns'.

	For example:

	./foo_test Has no flag, and thus runs all its tests.
	./foo_test --gtest_filter=* Also runs everything, due to the single match-everything * value.
	./foo_test --gtest_filter=FooTest.* Runs everything in test case FooTest.
	./foo_test --gtest_filter=*Null*:*Constructor* Runs any test whose full name contains either "Null" or "Constructor".
	./foo_test --gtest_filter=-*DeathTest.* Runs all non-death tests.
	./foo_test --gtest_filter=FooTest.*-FooTest.Bar Runs everything in test case FooTest except FooTest.Bar.

	Availability: Linux, Windows, Mac.

	refer: http://code.google.com/p/googletest/wiki/AdvancedGuide
	*/
	gtestingenv.mTestingCase = "--gtest_filter=";
	argc > 2 ? gtestingenv.mTestingCase += argv[2] :
		gtestingenv.mTestingCase += "*";
	if (argc > 2) gtestingenv.mTestingCase += ".*";

	return true;
}

bool loadMoudules(std::vector<std::string> & irModules)
{
	// dont show error message boxs
	::SetErrorMode(SEM_FAILCRITICALERRORS |
		SEM_NOGPFAULTERRORBOX |
		SEM_NOOPENFILEERRORBOX
		);

	bool lAnyone = false;
	for (int index = 0; index < irModules.size(); ++index)
	{
		try
		{
			// load each testing module which testing cases will be registered into google testing framework during initalization stage
			HMODULE lTestingModule = ::LoadLibrary(irModules[index].c_str());
			if (lTestingModule == NULL)
			{
				ColoredWPrintf(COLOR_RED, L"loading library %s error: %s\n", 
					irModules[index].c_str(), LOG::ConvertErrorCode(::GetLastError()));
			}
			lAnyone = true;
			ColoredPrintf(COLOR_GREEN, "module %s is loaded successfully!\n", irModules[index].c_str());
		}
		catch (...)
		{
			ColoredWPrintf(COLOR_RED, L"unknown fatal error\n");
		}
	}

	::SetErrorMode(0);
	return lAnyone;
}

int main(int argc, char **argv) {
	
	if (!hanldeArguments(argc, argv))
	{
		return -1;
	}

	if (!loadMoudules(gtestingenv.mTestingMoudle))
	{
		ColoredPrintf(COLOR_RED, "well, none of these modules is loaded correctly\n");
		return -1;
	}

	// set goolge testing environment
	int gargc = 2;
	const int argvbufsize = 64 > gtestingenv.mTestingCase.length() ? 64 : gtestingenv.mTestingCase.length();
	char** gargv = new char*[gargc];
	for (int iter = 0; iter < gargc; ++iter)
	{
		gargv[iter] = new char[argvbufsize];
		::memset(gargv[iter], '\0', argvbufsize);
	}
	strcpy_s(gargv[1], argvbufsize, gtestingenv.mTestingCase.c_str());

	// init google test env
	testing::InitGoogleTest(&gargc, gargv);

	// run all test
	int res = RUN_ALL_TESTS();

	if (gargv)
	{
		for (int iter = 0; iter < gargc; ++iter)
		{
			delete[] gargv[iter];
			gargv[iter] = NULL;
		}
		delete[] gargv;
	}

	return res;
}
