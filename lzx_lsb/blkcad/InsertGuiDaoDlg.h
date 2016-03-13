#if !defined(AFX_INSERTGUIDAODLG_H__8B8DEF80_48B4_4E92_87AA_D6AC8C88B475__INCLUDED_)
#define AFX_INSERTGUIDAODLG_H__8B8DEF80_48B4_4E92_87AA_D6AC8C88B475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertGuiDaoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsertGuiDaoDlg dialog

class CInsertGuiDaoDlg : public CDialog
{
// Construction
public:
	int m_iSelectID;
	CInsertGuiDaoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInsertGuiDaoDlg)
	enum { IDD = IDD_INSERTGUIDAO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertGuiDaoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsertGuiDaoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDwgthumbnailctrl1(UINT nID);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTGUIDAODLG_H__8B8DEF80_48B4_4E92_87AA_D6AC8C88B475__INCLUDED_)
