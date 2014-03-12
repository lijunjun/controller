
#include "stdafx.h"
#include "message.h"
#include "convertor.h"
#include <stdlib.h>

namespace LOG
{

	std::wstring ConvertErrorCode(size_t iErrorCode)
	{
		std::wstring lMessage = L"";
#ifdef WIN32
		LPVOID lpMsgBuf = NULL;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			iErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		wchar_t lMessageWrapper[1024];
		_snwprintf_s(lMessageWrapper, 1024, 1024, L"ERROR: %s\n", lpMsgBuf);

		lMessage = lMessageWrapper;
		::LocalFree(lpMsgBuf);

#endif
		return lMessage;
	}

}
