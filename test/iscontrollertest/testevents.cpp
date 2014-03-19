
#include "stdafx.h"
#include "gtest/gtest.h"
#include <Windows.h>
#include "../../log/message.h"

/*
* create an event and wait the controller triggering it to signaled status
*/
bool TestEvent(std::wstring iEventName)
{
	
	HANDLE lEventHandle = ::CreateEvent(NULL, TRUE, FALSE, iEventName.c_str());
	if (lEventHandle == NULL)
	{
		std::wcout << ::GetLastError() << L" : " <<  LOG::ConvertErrorCode(::GetLastError()) << std::endl;
		return false;
	}
	// _ASSERT(lEventHandle);
	
	DWORD lRet = ::WaitForSingleObject(lEventHandle, INFINITE);
	switch (lRet)
	{
		case WAIT_ABANDONED :
		{
					    std::wcout << L"event is abandoned\n";
					    return false;
		}
		case WAIT_OBJECT_0:
		{
					  std::wcout << L"event is signaled\n";
					  return true;
		}
		case WAIT_TIMEOUT :
		{
					  std::wcout << L"wait is timeout";
					  return false;
		}
	}
	return true;
}

TEST(IServerEventTest)
{
	while (true)
	{
		std::wstring lEventName;
		std::wcout << L"input event's name (end): ";
		std::wcin >> lEventName;
		if (lEventName.compare(L"end") == 0)
		{
			break;
		}
		EXPECT_TRUE(TestEvent(lEventName));
	}
	
}
