
#include "stdafx.h"
#include "convertor.h"
#include <stdlib.h>

namespace LOG
{
	LOG_API wchar_t* char2wchar(const char* iSrc, size_t iLen)
	{
		wchar_t* lDes = NULL;
		if (iLen > 0 && iSrc)
		{
			int length = std::strlen(iSrc);
			length = length > iLen ? iLen : length;
			lDes = new wchar_t[length + 1];
			_ASSERT(lDes);
			swprintf(lDes, length, L"%hs", iSrc);
		}
		return lDes;
	}

	LOG_API char* wchar2char(const wchar_t* iSrc, size_t iLen)
	{
		char* lDes = NULL;
		if (iLen > 0 && iSrc)
		{
			int length = std::wcslen(iSrc);
			length = length > iLen ? iLen : length;
			lDes = new char[iLen + 1];
			_ASSERT(lDes);
			size_t lConvertedLen = 0;
			wcstombs_s(&lConvertedLen, lDes, iLen, iSrc, iLen);
		}
		return lDes;
	}

}