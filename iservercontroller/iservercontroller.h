
// iservercontroller.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CiservercontrollerApp:
// See iservercontroller.cpp for the implementation of this class
//
namespace IServerController
{
	class CiservercontrollerApp : public CWinApp
	{
	public:
		CiservercontrollerApp();

		// Overrides
	public:
		virtual BOOL InitInstance();

		// Implementation

		DECLARE_MESSAGE_MAP()
	};
}

extern IServerController::CiservercontrollerApp theApp;