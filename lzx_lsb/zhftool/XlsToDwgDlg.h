#if !defined(AFX_XLSTODWGDLG_H__B0843BFC_933F_4F97_8AC9_B3159E023B0C__INCLUDED_)
#define AFX_XLSTODWGDLG_H__B0843BFC_933F_4F97_8AC9_B3159E023B0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XlsToDwgDlg.h : header file
//
//#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CXlsToDwgDlg dialog

class CXlsToDwgDlg : public CDialog
{
// Construction
public:
	CXlsToDwgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXlsToDwgDlg)
	enum { IDD = IDD_XLSTODWG };
	CString	m_strEndCol;
	int		m_iEndRow;
	CString	m_strExcelFileLoc;
	CString	m_strMark;
	CString	m_strStartCol;
	int		m_iStartRow;
	int		m_iConvMode;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXlsToDwgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXlsToDwgDlg)
	afx_msg void OnButtonXlstodwgSelectfile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditXlstodwgStartrow();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLSTODWGDLG_H__B0843BFC_933F_4F97_8AC9_B3159E023B0C__INCLUDED_)
