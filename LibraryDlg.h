// LibraryDlg.h : header file
//

#if !defined(AFX_LIBRARYDLG_H__D6C8A98A_29FB_4B4E_A9C2_B7F77D45045C__INCLUDED_)
#define AFX_LIBRARYDLG_H__D6C8A98A_29FB_4B4E_A9C2_B7F77D45045C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLibraryDlg dialog

class CLibraryDlg : public CDialog
{
// Construction
public:
	CLibraryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLibraryDlg)
	enum { IDD = IDD_LIBRARY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLibraryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLibraryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIBRARYDLG_H__D6C8A98A_29FB_4B4E_A9C2_B7F77D45045C__INCLUDED_)
