
// ServerHostDlg.h : header file
//

#pragma once
#include <string>
#include <thread>

// CServerHostDlg dialog
class CServerHostDlg : public CDialogEx
{
// Construction
public:
	CServerHostDlg(CWnd* pParent = nullptr);	// standard constructor
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERHOST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CBrush m_brush;
	COLORREF bgm = RGB(15, 15, 15);
	COLORREF bgc = RGB(30, 30, 30);
	CBrush m_brushm = RGB(30, 30, 30);
	CBrush m_brushc = RGB(15, 15, 15);
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void OnOK();
public:
	CEdit ServerCmdControl;
	CString ServerCmdString;
	static CString ServerLogString;
	static std::thread serverThread;
	static std::wstring text;
	static CRichEditCtrl ServerLogControl;
	static CRichEditCtrl ServerInfoControl;
	//static CRichEditCtrl ServerInfoControl;
	afx_msg void OnEnChangeServercmd();

	static void log(std::wstring msg);
	static void log(std::wstring msg, COLORREF color);
	static void log(std::wstring msg, COLORREF color, bool bold, bool italics);
	static void logNNL(std::wstring msg, COLORREF color, bool bold, bool italics);
	static void info(std::wstring msg, COLORREF color, bool bold, bool italics);
	static void info(std::wstring msg, int size, COLORREF color, bool bold, bool italics);
	static void info(std::wstring msg, int size, COLORREF color, bool bold, bool italics, int nl);
	static void updateLog();
	afx_msg void OnBnClickedButtonsend();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeServerlog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnIdcancel();
	CMFCButton ServerCmdButtonControl;
	static int serverStatus;
	static void renderInfo();
	static int line;
	static int iline;
};
