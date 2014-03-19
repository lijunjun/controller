#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include "basic.h"


class BASIC_API CError
{
public:
	typedef enum _ErrorType
	{
		ERROR_LOG,
		ERROR_CONSOLE,
		ERROR_WINDOW
	} ErrorType;

	void setMessage(wchar_t* iErrorMessage);

	std::wstring getMessage();

	virtual void Print() = 0;

private:
	std::wstring mErrorMessage;
};

#endif