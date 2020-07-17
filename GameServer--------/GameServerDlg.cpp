
// GameServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GameServer.h"
#include "GameServerDlg.h"
#include "afxdialogex.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameServerDlg dialog



CGameServerDlg::CGameServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAMESERVER_DIALOG, pParent)
	, ServerCommandString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// CGameServerDlg message handlers

BOOL CGameServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	//std::wstring title(L"Server");
	//SetWindowText(title.c_str());
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGameServerDlg::OnPaint()
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
HCURSOR CGameServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGameServerDlg::OnOK()
{
	std::basic_string<TCHAR> s(ServerCommandString);
	if (GetFocus() == &ServerCommandControl)
	{
		// TODO: Add your handling of the Return key here.
		TRACE0("Return key in edit control pressed\n");

		// Call `return` to leave the dialog open.
		//return;
	}
	CWnd* pWnd = GetFocus();
	if (pWnd == &ServerCommandControl)
	{
		TRACE0("A\n");
	}
	else if (pWnd == &ServerCommandControl)
	{
		TRACE0("B\n");
	}
	else
	{
		//if (s.length() > 0)
		{
			TRACE0("out\n");
		}
		//SetWindowText(GetDlgItem(), (LPCTSTR) " lol");
	}
	// Default behavior: Close the dialog.
	//CDialog::OnOK();
}BEGIN_MESSAGE_MAP(CGameServerDlg, CDialogEx)
ON_EN_CHANGE(IDC_EDITSERVERCMD, &CGameServerDlg::OnEnChangeEditservercmd)
ON_EN_CHANGE(IDC_SERVERLOG, &CGameServerDlg::OnEnChangeServerlog)
END_MESSAGE_MAP()


void CGameServerDlg::OnEnChangeEditservercmd()
{
	
}


void CGameServerDlg::OnEnChangeServerlog()
{
	TRACE0("a\n");
}
