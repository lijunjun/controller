
#ifndef CONVERTOR_H
#define CONVERTOR_H

#include "log.h"
#include <iostream>

namespace LOG
{
	// caller should manage allocated memory
	LOG_API wchar_t* char2wchar(const char* iSrc, size_t iLen);

	// caller should manage allocated memory
	LOG_API char* wchar2char(const wchar_t* iSrc, size_t iLen);

}

#endif
