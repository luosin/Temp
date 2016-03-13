#include "afxwin.h"
#if !defined(AFX_BATCHFINDREPLACEDLG_H__0E3A0D01_84E0_4013_924F_BBEC191BC0D7__INCLUDED_)
#define AFX_BATCHFINDREPLACEDLG_H__0E3A0D01_84E0_4013_924F_BBEC191BC0D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchFindReplaceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatchFindReplaceDlg dialog

class CBatchFindReplaceDlg : public CDialog
{
// Construction
public:
	int GetMapStrToStrAtt();
	CMapStringToString m_mapStrToStrAtt;
	CBatchFindReplaceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBatchFindReplaceDlg)
	enum { IDD = IDD_FINDREPLACEBLK };
	BOOL	m_bUseAtt;
	CString	m_strAtt;
	CString	m_strFold;
	CString	m_strNewBlk;
	CString	m_strOldBlk;
	int		m_iSelOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchFindReplaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBatchFindReplaceDlg)
	afx_msg void OnRadioFrbSel(UINT nID);
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckFrbAtt();
	afx_msg void OnButtonFrbSelnewdwg();
	afx_msg void OnButtonFrbSelfold();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboOldBlk;
	CComboBox m_comboNewBlk;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHFINDREPLACEDLG_H__0E3A0D01_84E0_4013_924F_BBEC191BC0D7__INCLUDED_)
