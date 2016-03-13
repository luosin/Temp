#if !defined(AFX_INPUTTEXTDLG_H__95E38099_934E_47C7_A378_41CEAD88808A__INCLUDED_)
#define AFX_INPUTTEXTDLG_H__95E38099_934E_47C7_A378_41CEAD88808A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputTextDlg dialog

class CInputTextDlg : public CDialog
{
// Construction
public:
	int m_iJustifyIndex;
	CString m_strTextStyle;
	CStringArray m_strArrayTextStyle;
	CInputTextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputTextDlg)
	enum { IDD = IDD_INPUTTEXT };
	CComboBox	m_ColorIndex;
	CComboBox	m_JustifyIndex;
	CComboBox	m_StyleIndex;
	double	m_dHeight;
	double	m_dRotation;
	CString	m_strText;
	double	m_dWidth;
	double	m_dXOffSet;
	double	m_dYOffSet;
	BOOL	m_bSet;
	CString	m_strMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputTextDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckInputtextSet();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTTEXTDLG_H__95E38099_934E_47C7_A378_41CEAD88808A__INCLUDED_)
