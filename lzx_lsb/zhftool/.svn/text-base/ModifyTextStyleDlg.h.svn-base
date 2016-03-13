#if !defined(AFX_MODIFYTEXTDLG_H__CFB99DD1_494B_456E_B6FA_19C3724DA990__INCLUDED_)
#define AFX_MODIFYTEXTDLG_H__CFB99DD1_494B_456E_B6FA_19C3724DA990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyTextStyleDlg.h : header file
//
//#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CModifyTextStyleDlg dialog

class CModifyTextStyleDlg : public CDialog
{
	// Construction
public:
	CString m_strTextStyle;
	int m_iTextStyleCurSel;
	int m_iTextJustifyCurSel;
	CStringArray m_strArrayTextStyle;
	CModifyTextStyleDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CModifyTextStyleDlg)
	enum { IDD = IDD_MODIFYTEXT };
	CComboBox	m_TextStyleIndex;
	CComboBox	m_TextJustifyIndex;
	double	m_dHeight;
	double	m_dRotation;
	double	m_dWidthFactor;
	BOOL	m_bSet;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyTextStyleDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CModifyTextStyleDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYTEXTDLG_H__CFB99DD1_494B_456E_B6FA_19C3724DA990__INCLUDED_)
