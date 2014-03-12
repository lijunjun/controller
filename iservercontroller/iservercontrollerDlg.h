
// iservercontrollerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "../memorymgt/memorymgt.h"

namespace IServerController
{
	// CiservercontrollerDlg dialog
	class CiservercontrollerDlg : public CDialogEx
	{
		// Construction
	public:
		CiservercontrollerDlg(CWnd* pParent = NULL);	// standard constructor
		virtual ~CiservercontrollerDlg();

		// Dialog Data
		enum { IDD = IDD_ISERVERCONTROLLER_DIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


		// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedCancel();
		afx_msg void OnLbnSelchangeIserverEventlist();
		afx_msg void OnBnClickedOk();

	private:
		bool ParseIServerConfig(CString iConfigFile);

	private:
		CString mCurrentPathName;
		CString mConfigFile;
		MemoryMgt::Cmemoryhandler mMemoryhandler;

	private:
		CListBox mIServerEvents;
		CFileDialog mFileDialog;

	public:
		afx_msg void OnBnClickedButton1();
	};

}
