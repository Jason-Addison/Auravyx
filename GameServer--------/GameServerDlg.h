
// GameServerDlg.h : header file
//

#pragma once


// CGameServerDlg dialog
class CGameServerDlg : public CDialogEx
{
// Construction
public:
	CGameServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMESERVER_DIALOG };
#endif

	protected:


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnOK();
public:
	CEdit ServerCommandControl;
	CString ServerCommandString;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEnChangeEditservercmd();
	afx_msg void OnEnChangeServerlog();
};
