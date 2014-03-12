
#include "stdafx.h"
#include "critical_section.h"


namespace IPC
{
	CCriticalSection::CCriticalSection()
	{
		::InitializeCriticalSection(&mCS);
	}

	CCriticalSection::~CCriticalSection()
	{

	}

	bool CCriticalSection::TryToEnter()
	{
		return ::TryEnterCriticalSection(&mCS);
	}

	void CCriticalSection::Enter()
	{
		::EnterCriticalSection(&mCS);
	}

	void CCriticalSection::Leave()
	{
		::LeaveCriticalSection(&mCS);
	}

	CCriticalSection::CCriticalSection(CCriticalSection& iCCriticalSection)
	{
		
	}

	CCriticalSection* CCriticalSection::operator = (CCriticalSection& iCCriticalSection)
	{
		return this;
	}

}