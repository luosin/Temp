#if !defined(AFX_BATCHDTEXTDLG_H__0CB25286_CA4D_4002_B79B_83880D5198A5__INCLUDED_)
#define AFX_BATCHDTEXTDLG_H__0CB25286_CA4D_4002_B79B_83880D5198A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchDTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatchDTextDlg dialog

class CBatchDTextDlg : public CDialog
{
// Construction
public:
	CBatchDTextDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBatchDTextDlg)
	enum { IDD = IDD_SUPERTEXT };
	CString	m_strFront;
	CString	m_strBack;
	int		m_iStart;
	int		m_iEnd;
	int		m_iIncrement;
	int		m_iRepeat;
	int		m_iTextDirect;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchDTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBatchDTextDlg)
	afx_msg void OnBUTTONOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
public:
	ads_point endp_2d;
	ads_point startp_2d;
	
	BOOL WriteBatchTextHor();
	BOOL WriteBatchTextVer();
	//AcDbDatabase * pDb;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHDTEXTDLG_H__0CB25286_CA4D_4002_B79B_83880D5198A5__INCLUDED_)
