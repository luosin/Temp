#if !defined(AFX_STEXTSET_H__7BC00C9B_DBD0_4357_8797_980E3A014F84__INCLUDED_)
#define AFX_STEXTSET_H__7BC00C9B_DBD0_4357_8797_980E3A014F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STextSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSTextSet dialog

class CSTextSet : public CDialog
{
// Construction
public:
	CSTextSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSTextSet)
	enum { IDD = IDD_STEXTSET };
	int		m_iStartNum;
	CString	m_strFrontTxt;
	CString	m_strBackTxt;
	int		m_iIncreNum;
	UINT	m_iRepeat;
	int		m_iSelectMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTextSet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSTextSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// 框选时横向排序，还是纵向排序
	int m_iSortBy;
	afx_msg void OnBnClickedRadioStextWindow();
	afx_msg void OnBnClickedRadioStextPick();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEXTSET_H__7BC00C9B_DBD0_4357_8797_980E3A014F84__INCLUDED_)
