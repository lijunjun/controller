
#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#include "ipc.h"
#include "stdafx.h"

namespace IPC
{
	class IPC_API  CCriticalSection
	{
	public:
		CCriticalSection();
		~CCriticalSection();

		bool TryToEnter();
		void Enter();
		void Leave();

	protected:
		CCriticalSection(CCriticalSection& iCCriticalSection);
		CCriticalSection* operator = (CCriticalSection& iCCriticalSection);

	private:
		CRITICAL_SECTION mCS;
	};
}

#endif
