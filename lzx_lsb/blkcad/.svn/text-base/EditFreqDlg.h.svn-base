#if !defined(AFX_EDITFREQDLG_H__2DD35AC4_DD57_46D1_8ABD_0F1E2BB17F76__INCLUDED_)
#define AFX_EDITFREQDLG_H__2DD35AC4_DD57_46D1_8ABD_0F1E2BB17F76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditFreqDlg.h : header file
//
#include "NeuListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEditFreqDlg dialog

class CEditFreqDlg : public CDialog
{
// Construction
public:
	CStringArray m_strArrayFreq;
	CStringArray m_strArrayGuiDao;
	CEditFreqDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditFreqDlg)
	enum { IDD = IDD_EDITFREQ };
	CNeuListCtrl  m_listCtrFreq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFreqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//HICON m_hIcon;
	 CImageList m_imlNull;
	afx_msg void OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult);


	// Generated message map functions
	//{{AFX_MSG(CEditFreqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnEndlabeleditListFreq(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITFREQDLG_H__2DD35AC4_DD57_46D1_8ABD_0F1E2BB17F76__INCLUDED_)
