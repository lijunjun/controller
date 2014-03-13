
// iservercontrollerDlg.cpp : implementation file
//

#include "stdafx.h"
#include <cstring>
#include "iservercontroller.h"
#include "iservercontrollerDlg.h"
#include "afxdialogex.h"
#include "iserveripc.h"
#include "json/json.h"
#include "../auto/atuo_buffer.h"

namespace IServerController
{

	// CAboutDlg dialog used for App About

	class CAboutDlg : public CDialogEx
	{
	public:
		CAboutDlg();

		// Dialog Data
		enum { IDD = IDD_ABOUTBOX };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// Implementation
	protected:
		DECLARE_MESSAGE_MAP()
	};

	CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
	{
	}

	void CAboutDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	END_MESSAGE_MAP()


	// CiservercontrollerDlg dialog



	CiservercontrollerDlg::CiservercontrollerDlg(CWnd* pParent /*=NULL*/)
		: CDialogEx(CiservercontrollerDlg::IDD, pParent),
		mFileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, NULL, this)
	{
		wchar_t lCurrentPath[256] = { L'\0' };
		::GetCurrentDirectory(256, lCurrentPath);
		mCurrentPathName = lCurrentPath;
		if (mCurrentPathName.GetLength() == 0)
		{
			mCurrentPathName = L"c:\\";
		}
		
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	}

	CiservercontrollerDlg::~CiservercontrollerDlg()
	{
		
	}

	void CiservercontrollerDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_ISERVER_EVENTLIST, mIServerEvents);
	}

	BEGIN_MESSAGE_MAP(CiservercontrollerDlg, CDialogEx)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		ON_BN_CLICKED(IDCANCEL, &CiservercontrollerDlg::OnBnClickedCancel)
		ON_LBN_SELCHANGE(IDC_ISERVER_EVENTLIST, &CiservercontrollerDlg::OnLbnSelchangeIserverEventlist)
		ON_BN_CLICKED(IDOK, &CiservercontrollerDlg::OnBnClickedOk)
		ON_BN_CLICKED(IDC_BUTTON1, &CiservercontrollerDlg::OnBnClickedButton1)
	END_MESSAGE_MAP()


	// CiservercontrollerDlg message handlers

	BOOL CiservercontrollerDlg::OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		// Add "About..." menu item to system menu.

		// IDM_ABOUTBOX must be in the system command range.
		ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
		ASSERT(IDM_ABOUTBOX < 0xF000);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			BOOL bNameValid;
			CString strAboutMenu;
			bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
			ASSERT(bNameValid);
			if (!strAboutMenu.IsEmpty())
			{
				pSysMenu->AppendMenu(MF_SEPARATOR);
				pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			}
		}

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here

		return TRUE;  // return TRUE  unless you set the focus to a control
	}

	void CiservercontrollerDlg::OnSysCommand(UINT nID, LPARAM lParam)
	{
		if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
		}
		else
		{
			CDialogEx::OnSysCommand(nID, lParam);
		}
	}

	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.

	void CiservercontrollerDlg::OnPaint()
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialogEx::OnPaint();
		}
	}

	// The system calls this function to obtain the cursor to display while the user drags
	//  the minimized window.
	HCURSOR CiservercontrollerDlg::OnQueryDragIcon()
	{
		return static_cast<HCURSOR>(m_hIcon);
	}



	void CiservercontrollerDlg::OnBnClickedCancel()
	{
		// TODO: Add your control notification handler code here
		CDialogEx::OnCancel();
	}


	void CiservercontrollerDlg::OnLbnSelchangeIserverEventlist()
	{
		// TODO: Add your control notification handler code here

	}


	void CiservercontrollerDlg::OnBnClickedOk()
	{
		UpdateData();

		CString lSelectedData;
		mIServerEvents.GetText(mIServerEvents.GetCurSel(), lSelectedData);
		std::wstring lRet = CIServerIPC::CreateInstance()->TriggerEvent(lSelectedData);
		if (lRet.length() > 0)
		{
			::MessageBox(NULL, lRet.c_str(), L"error", MB_OK);
		}
	}

	bool CiservercontrollerDlg::ParseIServerConfig(CString iConfigFile)
	{
		try 
		{
			CFile lConfigFile(iConfigFile.GetBuffer(), CFile::modeRead);
			lConfigFile.SeekToBegin();

			ULONGLONG lTotalSize = lConfigFile.GetLength();
			std::string lConfigData;

			Auto::auto_buffer<char> lConfigBuf(lTotalSize + 1, &mMemoryhandler);
			if (lConfigBuf.empty())
			{
				return false;
			}
			lConfigFile.Read(lConfigBuf.begin(), lTotalSize);
			lConfigData = lConfigBuf.begin();

			Json::Features features = Json::Features::strictMode();
			Json::Value lRoot;
			Json::Reader lReader(features);
			if (!lReader.parse(lConfigData, lRoot, false))
			{
				std::string lError = lReader.getFormattedErrorMessages();

				return false;
			}

			/*
			*	{
			*	"iserver" :
			*	{
			*		"events":
			*		[
			*			"event1", "event2"	
			*		],
			*		"semaphore":
			*		[
			*			"sema1", "sema2"
			*		]
			*	}
			*	}
			*/

			// show events
			const Json::Value lEventsConfig = lRoot["iserver"]["events"];
			std::string lEventsNameString = "";
			for (int iter = 0; iter < lEventsConfig.size(); ++iter)
			{
				// dbgheap pFirstBlock == pHead
				// lEventsNameString = lEventsConfig[iter].asString();

				if (lEventsConfig[iter].isString())
				{
					lEventsNameString = lEventsConfig[iter].asCString();
				}
				Auto::auto_buffer<wchar_t> lEventsName(lEventsNameString.length() + 1);
				if (!::MultiByteToWideChar(
					CP_UTF8,
					MB_ERR_INVALID_CHARS,
					lEventsNameString.c_str(),
					lEventsNameString.length(),
					lEventsName.begin(),
					lEventsName.size()
					))
				{
					// handle error
					::GetLastError();
					continue;
				}
				lEventsName[lEventsNameString.length()] = L'\0';
				mIServerEvents.InsertString(iter, lEventsName.begin());

			}
		}
		catch (...)
		{

		}
		return true;
	}

	void CiservercontrollerDlg::OnBnClickedButton1()
	{
		const int lFileBufferSize = 256;
		wchar_t lFileBuffer[lFileBufferSize] = { L'\0' };
		wcscpy_s(lFileBuffer, mCurrentPathName.GetBuffer());

		// buffer to init path and store selected file
		mFileDialog.m_ofn.lpstrFile = lFileBuffer;
		mFileDialog.m_ofn.nMaxFile = lFileBufferSize;

		if (mFileDialog.DoModal() == IDOK)
		{
			mCurrentPathName = mFileDialog.GetFolderPath();
			mConfigFile = mFileDialog.GetPathName();
			ParseIServerConfig(mConfigFile);
		}

	}

}
