
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdafx.h"

namespace TestController
{
	/* 
	* create a console event, if user input the correct command, the event will be triggered
	* initial status is unsignaled and manual reset.
	*/
	HANDLE createConsoleEvent(std::wstring iTriggerConsoleCmd);


}

#endif
