
// ServerHostDlg.cpp : implementation file
//

#include "targetver.h"
#include "pch.h"
#include "framework.h"
#include "ServerHost.h"
#include "ServerHostDlg.h"
#include "afxdialogex.h"
#include "ServerManager.h"
#include <string>
#include <thread>
#include "Server.h"
#include <sstream>
#include <strstream>
#include "Log.h"
#include <resource.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define ON 1
#define CRASH 0
#define LOADING 3
#define OFF 2

// CServerHostDlg dialog



CServerHostDlg::CServerHostDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERHOST_DIALOG, pParent)
	, ServerCmdString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVERCMD, ServerCmdControl);
	DDX_Text(pDX, IDC_SERVERCMD, ServerCmdString);
	DDX_Control(pDX, IDC_SERVERLOG, ServerLogControl);
	DDX_Text(pDX, IDC_SERVERLOG, ServerLogString);
	DDX_Control(pDX, IDC_SERVERLOG, ServerLogControl);
	DDX_Control(pDX, IDC_BUTTONSEND, ServerCmdButtonControl);
	//DDX_Control(pDX, IDC_SERVERINFO, ServerInfoControl);
	DDX_Control(pDX, IDC_SERVERINFO, ServerInfoControl);
}

BEGIN_MESSAGE_MAP(CServerHostDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_SERVERCMD, &CServerHostDlg::OnEnChangeServercmd)
	ON_BN_CLICKED(IDC_BUTTONSEND, &CServerHostDlg::OnBnClickedButtonsend)
	ON_EN_CHANGE(IDC_SERVERLOG, &CServerHostDlg::OnEnChangeServerlog)
	ON_COMMAND(IDCANCEL, &CServerHostDlg::OnIdcancel)
END_MESSAGE_MAP()


// CServerHostDlg message handlers
void serverStart()
{
	ServerManager s;
	s.start();
}
std::thread CServerHostDlg::serverThread;
BOOL CServerHostDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	ServerManager::ready = true;
	serverThread = std::thread(serverStart);
	// TODO: Add extra initialization here
	m_brushm.CreateSolidBrush(bgm);
	m_brushc.CreateSolidBrush(bgc);
	ServerLogControl.SetBackgroundColor(false, bgc);
	ServerInfoControl.SetBackgroundColor(false, bgm);
	ServerCmdButtonControl.SetFaceColor(bgc);

	ServerCmdButtonControl.EnableWindowsTheming(FALSE);
	ServerCmdButtonControl.m_nFlatStyle = CMFCButton::BUTTONSTYLE_FLAT;
	ServerCmdButtonControl.m_bTransparent = false;
	ServerCmdButtonControl.m_bDrawFocus = false;
	//ServerCmdButtonControl.border
	ServerCmdButtonControl.SetFaceColor(bgc, true);
	ServerCmdButtonControl.SetTextColor(RGB(255, 255, 255));

	Log::serverStatus(ON);
	GetDlgItem(IDC_SERVERINFO)->ModifyStyle(0, WS_DISABLED);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerHostDlg::OnPaint()
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
std::wstring CServerHostDlg::text = L"";
CRichEditCtrl CServerHostDlg::ServerLogControl;
CRichEditCtrl CServerHostDlg::ServerInfoControl;
CString CServerHostDlg::ServerLogString = L"";
int CServerHostDlg::line = 0;
int CServerHostDlg::iline = 0;
void CServerHostDlg::log(std::wstring msg, COLORREF color, bool bold, bool italics)
{
	logNNL(msg + L"\r\n", color, bold, italics);
	line++;
}
void CServerHostDlg::logNNL(std::wstring msg, COLORREF color, bool bold, bool italics)
{
	CHARFORMAT cf = { 0 };
	int txtLen = ServerLogControl.GetTextLength();
	cf.cbSize = sizeof(cf);
	cf.yHeight = 200;
	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = (bold ? CFE_BOLD : 0) | (italics ? CFE_ITALIC : 0);// | ~CFE_AUTOCOLOR;
	cf.crTextColor = color;
	
	//ServerLogControl.SetWindowText(msg.c_str());
	ServerLogControl.SetSel(txtLen, -1); // Set the cursor to the end of the text area and deselect everything.
	ServerLogControl.ReplaceSel(msg.c_str()); // Inserts when nothing is selected.

	// Apply formating to the just inserted text.
	ServerLogControl.SetSel(txtLen - line, ServerLogControl.GetTextLength() - line);
	ServerLogControl.SetSelectionCharFormat(cf);
	//info(L"Server Status", RGB(255, 0, 0), false, false);
}
void CServerHostDlg::info(std::wstring msg, COLORREF color, bool bold, bool italics)
{
	info(msg, 300, color, bold, italics);
	iline -= 1;
}
void CServerHostDlg::info(std::wstring msg, int size, COLORREF color, bool bold, bool italics)
{
	info(msg, 300, color, bold, italics, 1);
}
void CServerHostDlg::info(std::wstring msg, int size, COLORREF color, bool bold, bool italics, int nl)
{
	CHARFORMAT cf = { 0 };
	int txtLen = ServerInfoControl.GetTextLength();
	cf.cbSize = sizeof(cf);
	cf.yHeight = size;
	cf.dwMask = CFM_BOLD | CFM_ITALIC | CFM_COLOR | CFM_SIZE | 0;
	cf.dwEffects = (bold ? CFE_BOLD : 0) | (italics ? CFE_ITALIC : 0);// | ~CFE_AUTOCOLOR;
	cf.crTextColor = color;

	//ServerLogControl.SetWindowText(msg.c_str());
	ServerInfoControl.SetSel(txtLen, -1); // Set the cursor to the end of the text area and deselect everything.
	ServerInfoControl.ReplaceSel(msg.c_str()); // Inserts when nothing is selected.

	// Apply formating to the just inserted text.
	ServerInfoControl.SetSel(txtLen - iline, ServerInfoControl.GetTextLength() - iline);
	ServerInfoControl.SetSelectionCharFormat(cf);
	iline += nl;
}
void CServerHostDlg::updateLog()
{
	/*ServerLogControl
		SetWindowTextW(
		(LPCTSTR)text.
			c_str());*/
}

void CServerHostDlg::OnOK()
{
	//info(L"Server Status", RGB(255, 0, 0), false, false);
	if (GetFocus() == &ServerCmdControl)
	{
		Log::serverStatus(LOADING);
		CString c;
		ServerCmdControl.GetWindowTextW(c);
		if (c.GetLength() != 0)
		{
			ServerCmdControl.SetWindowTextW(L"");
			std::wstringstream w;
			w << c.GetString();

			Log::out(SERVER, w);
			CT2CA pszConvertedAnsiString(c);
			// construct a std::string using the LPCSTR input
			std::string strStd(pszConvertedAnsiString);
			for (auto& p : Server::players)
			{
				p->send((char)1 + strStd);
			}
		}
	}
	// Default behavior: Close the dialog.
	//CDialog::OnOK();
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerHostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerHostDlg::OnEnChangeServercmd()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CServerHostDlg::log(std::wstring msg)
{
	log(msg, RGB(255, 255, 255), false, false);
}

void CServerHostDlg::log(std::wstring msg, COLORREF color)
{
	log(msg, color, false, false);
}

void CServerHostDlg::OnBnClickedButtonsend()
{
	CString c;
	ServerCmdControl.GetWindowTextW(c);
	log((LPCTSTR)c);
}

HBRUSH CServerHostDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if ((CTLCOLOR_EDIT == nCtlColor) &&
		(IDC_SERVERCMD == pWnd->GetDlgCtrlID()))
	{
		pDC->SetBkColor(bgc);
		pDC->SetTextColor(RGB(255, 255, 255));
		return m_brushc; //Create this brush in OnInitDialog() and destroy in destructor
	}
	if ((CTLCOLOR_EDIT == nCtlColor) &&
		(IDC_SERVERLOG == pWnd->GetDlgCtrlID()))
	{
		pDC->SetBkColor(bgc);
		return m_brushc; //Create this brush in OnInitDialog() and destroy in destructor
	}
	if ((CTLCOLOR_EDIT == nCtlColor) &&
		(IDC_SERVERINFO == pWnd->GetDlgCtrlID()))
	{
		pDC->SetBkColor(bgc);
		return m_brushc; //Create this brush in OnInitDialog() and destroy in destructor
	}
	return m_brushm;
}


void CServerHostDlg::OnEnChangeServerlog()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
BOOL CServerHostDlg::PreTranslateMessage(MSG* pMsg)
{
	//MessageBox(L"WM_CLOSE");
	TRACE("\n MSG %d", pMsg->message);
	if (pMsg->message == WM_CLOSE)
		MessageBox(L"WM_CLOSE");
	else if (pMsg->message == WM_QUIT)
		MessageBox(L"WM_QUIT");
	else if (pMsg->message == WM_DESTROY)
		MessageBox(L"WM_DESTROY");
	return __super::PreTranslateMessage(pMsg); // allow default behavior
}


void CServerHostDlg::OnIdcancel()
{
	ServerManager::running = false;
	while (!ServerManager::done)
	{

	}
	//PostQuitMessage(0);
	//ASSERT(AfxGetMainWnd() != NULL);
	SendMessage(WM_CLOSE);
	DestroyWindow();
	serverThread.join();
}
int CServerHostDlg::serverStatus = -1;
void CServerHostDlg::renderInfo()
{
	iline = 0;
	ServerInfoControl.SetSel(0, -1); //all text
	ServerInfoControl.ReplaceSel(L"");
	info(L"Server\n", 400, RGB(255, 255, 255), false, false);
	info(L"\n", 300, RGB(255, 255, 255), false, false);
	switch (serverStatus)
	{
		/*if (1)
			break;
		case(ON):
		{
			//log(std::to_wstring(ServerInfoControl.GetTextLength()));
			info(L"⬛", RGB(0, 255, 0), false, false);
			info(L" On", RGB(255, 255, 255), false, false);
			break;
		};
		case(LOADING):
		{
			info(L"⬛", RGB(255, 127, 0), false, false);
			info(L" Loading", RGB(255, 255, 255), false, false);
			break;
		};
		case(OFF):
		{
			info(L"⬛", RGB(255, 0, 0), false, false);
			info(L" Off", RGB(255, 255, 255), false, false);
			break;
		};*/
	}
	//info(L"\n", 300, RGB(255, 255, 255), false, false);
	//info(L"\n", 300, RGB(255, 255, 255), false, false);
	info(L"Players\n", 300, RGB(255, 255, 255), false, false);
	for (auto p : Server::players)
	{
		std::wstringstream ws;
		ws << p->name.c_str() << "\n";
		info(L"⬛", 200, RGB(0, 255, 0), false, false, 0);
		info(ws.str(), 200, RGB(255, 255, 255), false, false, 1);
	}
	//info(L"\n", 300, RGB(255, 255, 255), false, false);
	//log(std::to_wstring(ServerInfoControl.GetTextLength()));
}
