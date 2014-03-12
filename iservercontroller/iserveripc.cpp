
#include "stdafx.h"
#include "iserveripc.h"
#include "../ipc/critical_section.h"
#include "../log/message.h"

namespace IServerController
{
	IPC::CCriticalSection CIServerIPC::sInstanceCS;
	CIServerIPC* CIServerIPC::sCIServerIPCInstance = NULL;

	CIServerIPC* CIServerIPC::CreateInstance()
	{
		sInstanceCS.Enter();
		if (sCIServerIPCInstance == NULL)
		{
			sCIServerIPCInstance = new CIServerIPC();
		}
		sInstanceCS.Leave();
		return sCIServerIPCInstance;
	}

	void CIServerIPC::DeleteInstance()
	{
		sInstanceCS.Enter();
		if (sCIServerIPCInstance != NULL)
		{
			delete sCIServerIPCInstance;
			sCIServerIPCInstance = NULL;
		}
		sInstanceCS.Leave();
	}

	CIServerIPC::CIServerIPC()
	{
		try {
			mMaxWorkingThreads = 8;
			mMinWorkingThreads = 1;

			// init callback environment
			::InitializeThreadpoolEnvironment(&mCallBackEvn);

			// create a new thread pool
			mWorkingPool = ::CreateThreadpool(NULL);
			if (mWorkingPool == NULL) throw;

			// init pool environment
			::SetThreadpoolThreadMaximum(mWorkingPool, mMaxWorkingThreads);
			::SetThreadpoolThreadMinimum(mWorkingPool, mMinWorkingThreads);

			mCleanUpGroup = ::CreateThreadpoolCleanupGroup();
			if (mCleanUpGroup == NULL) throw;

			// Associate the callback environment with our thread pool.
			::SetThreadpoolCallbackPool(&mCallBackEvn, mWorkingPool);

			//
			// Associate the cleanup group with our thread pool.
			// Objects created with the same callback environment
			// as the cleanup group become members of the cleanup group.
			//
			::SetThreadpoolCallbackCleanupGroup(&mCallBackEvn, mCleanUpGroup, NULL);


		}
		catch (...)
		{
			CString lErrorCode = L"";
			lErrorCode.FormatMessage(L"%d", ::GetLastError());
			::MessageBox(NULL, lErrorCode.GetBuffer(), L"failed to init iserver ipc environment", MB_OK);
		}

	}

	CIServerIPC::~CIServerIPC()
	{
		if (mCleanUpGroup)
		{
			// Clean up the cleanup group members.
			CloseThreadpoolCleanupGroupMembers(mCleanUpGroup,
				FALSE, NULL);
			// Clean up the cleanup group.
			CloseThreadpoolCleanupGroup(mCleanUpGroup);
		}
		if (mWorkingPool)
		{
			// Clean up the pool.
			CloseThreadpool(mWorkingPool);
		}
	}

	VOID CALLBACK CIServerIPC::TriggerEventAPI(
		_Inout_      PTP_CALLBACK_INSTANCE Instance,
		_Inout_opt_  PVOID Context,
		_Inout_      PTP_WORK Work)
	{
		TriggerEventPara* lEventName = (TriggerEventPara*)Context;
		if (lEventName == NULL)
		{
			return;
		}
		HANDLE lEventHandle = ::OpenEvent( EVENT_MODIFY_STATE, false, lEventName->mEventName.c_str());
		if (lEventHandle != NULL)
		{
			::SetEvent(lEventHandle);
			::CloseHandle(lEventHandle);
			lEventName->mEventName = L"";
			return;
		}

		lEventName->mEventName = LOG::ConvertErrorCode(::GetLastError());
	}

	std::wstring CIServerIPC::TriggerEvent(CString iEventName)
	{
		TriggerEventPara lTriggerEventPara;
		lTriggerEventPara.mEventName = iEventName.GetBuffer();

		// Create a new job with callback envrionment
		PTP_WORK lNewEventJob = ::CreateThreadpoolWork(TriggerEventAPI, (PVOID)&lTriggerEventPara, &mCallBackEvn);
		if (lNewEventJob == NULL)
		{
			return LOG::ConvertErrorCode(::GetLastError());
		}

		// submit job
		::SubmitThreadpoolWork(lNewEventJob);

		// wait until job being done
		::WaitForThreadpoolWorkCallbacks(lNewEventJob, false);

		return lTriggerEventPara.mEventName;
	}

}
