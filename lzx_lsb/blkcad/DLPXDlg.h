#if !defined(AFX_DLPXDLG_H__D4BFE8C5_0DD8_4EA5_B5CC_9DAC948CB067__INCLUDED_)
#define AFX_DLPXDLG_H__D4BFE8C5_0DD8_4EA5_B5CC_9DAC948CB067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLPXDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLPXDlg dialog

class CDLPXDlg : public CDialog
{
// Construction
public:
	CDLPXDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLPXDlg)
	enum { IDD = IDD_DLPX };
	CString	m_strSavePath;
	CString	m_strMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLPXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLPXDlg)
	afx_msg void OnButtonDlpxSelpath();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLPXDLG_H__D4BFE8C5_0DD8_4EA5_B5CC_9DAC948CB067__INCLUDED_)
