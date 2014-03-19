#include "stdafx.h"
#include "error.h"

void CError::setMessage(wchar_t* iErrorMessage)
{
	mErrorMessage = iErrorMessage;
}

std::wstring CError::getMessage()
{
	return mErrorMessage;
}