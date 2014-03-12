
#ifndef MESSAGE_H
#define MESSAGE_H

#include "log.h"
#include <iostream>
#include <string>

namespace LOG
{
	// convert error code to error message with system api
	LOG_API std::wstring ConvertErrorCode(size_t iErrorCode);


}

#endif
