#if !defined(AFX_FINDREPLACEDLG_H__E2B8C227_AB2D_49D8_9A6B_88F071B61674__INCLUDED_)
#define AFX_FINDREPLACEDLG_H__E2B8C227_AB2D_49D8_9A6B_88F071B61674__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindReplaceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindReplaceDlg dialog

class CFindReplaceDlg : public CDialog
{
// Construction
public:
	CFindReplaceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindReplaceDlg)
	enum { IDD = IDD_FINDREPLACE };
	CComboBox	m_colorIndex;
	CString	m_strFindTxt;
	CString	m_strReplaceTxt;
	CString m_strFilePath ;
	BOOL	m_bCase;
	BOOL	m_bReg ;
	int		m_iRange ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindReplaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindReplaceDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelDir();
	afx_msg void OnRadioFilePath(UINT nID);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDREPLACEDLG_H__E2B8C227_AB2D_49D8_9A6B_88F071B61674__INCLUDED_)
