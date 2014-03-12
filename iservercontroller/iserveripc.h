
#ifndef ISERVERIPC_H
#define ISERVERIPC_H

#include "afxwin.h"
#include "../ipc/critical_section.h"

namespace IServerController
{
	class CIServerIPC
	{
	public:
		static CIServerIPC* CreateInstance();
		static void DeleteInstance();

		// if error, return error message, else return empty string
		std::wstring TriggerEvent(CString iEventName);

	protected:
		typedef struct _TriggerEventPara
		{
			 std::wstring mEventName;
		} TriggerEventPara;

		static VOID CALLBACK TriggerEventAPI(
			_Inout_      PTP_CALLBACK_INSTANCE Instance,
			_Inout_opt_  PVOID Context,
			_Inout_      PTP_WORK Work);

	private:
		CIServerIPC();
		~CIServerIPC();

	private:
		/*
		* using windows thread pool to hadnle server controller
		*/
		TP_CALLBACK_ENVIRON mCallBackEvn;
		PTP_POOL mWorkingPool;
		PTP_CLEANUP_GROUP mCleanUpGroup;

		size_t mMaxWorkingThreads;
		size_t mMinWorkingThreads;

		static CIServerIPC* sCIServerIPCInstance;
		static IPC::CCriticalSection sInstanceCS;
	};
}

#endif
