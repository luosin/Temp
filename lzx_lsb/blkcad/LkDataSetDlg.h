#if !defined(AFX_LKDATASETDLG_H__688B126A_A49A_4396_B341_682647A9BD24__INCLUDED_)
#define AFX_LKDATASETDLG_H__688B126A_A49A_4396_B341_682647A9BD24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LkDataSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLkDataSetDlg dialog

class CLkDataSetDlg : public CDialog
{
// Construction
public:
	CString m_strTitle;
	CLkDataSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLkDataSetDlg)
	enum { IDD = IDD_LKDATA_SET };
	CString	m_strLineName;
	CString	m_strSavePath;
	UINT	m_iBaOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLkDataSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLkDataSetDlg)
	afx_msg void OnButtonSelpath();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LKDATASETDLG_H__688B126A_A49A_4396_B341_682647A9BD24__INCLUDED_)
