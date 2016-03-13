#if !defined(AFX_GETHEIGHTWIDTHDLG_H__B37D82DF_9E54_4626_A223_EFD8CB52EFA3__INCLUDED_)
#define AFX_GETHEIGHTWIDTHDLG_H__B37D82DF_9E54_4626_A223_EFD8CB52EFA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetHeightWidthDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetHeightWidthDlg dialog

class CGetHeightWidthDlg : public CDialog
{
// Construction
public:
	CGetHeightWidthDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetHeightWidthDlg)
	enum { IDD = IDD_GETXLSHEIGHT };
	double	m_dCadHeight;
	double	m_dCadWidth;
	double	m_dExcelHeight;
	double	m_dExcelWidth;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetHeightWidthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetHeightWidthDlg)
	afx_msg void OnButtonGethwDone();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETHEIGHTWIDTHDLG_H__B37D82DF_9E54_4626_A223_EFD8CB52EFA3__INCLUDED_)
