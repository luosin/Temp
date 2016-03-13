#if !defined(AFX_TEXTDLG_H__AD572776_9EDC_4599_B2E7_F6E006073910__INCLUDED_)
#define AFX_TEXTDLG_H__AD572776_9EDC_4599_B2E7_F6E006073910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextDlg.h : header file
//

//#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog

class CTextDlg : public CDialog
{
// Construction
public:
	int iFlag;
	CTextDlg(CWnd* pParent = NULL);   // standard constructor
	AcDbEntity * pEnt;
	AcDbText * pText;

// Dialog Data
	//{{AFX_DATA(CTextDlg)
	enum { IDD = IDD_EDITTEXT };
	CString	m_strText;
	int		m_iSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextDlg)
	afx_msg void OnConFirm();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTDLG_H__AD572776_9EDC_4599_B2E7_F6E006073910__INCLUDED_)
