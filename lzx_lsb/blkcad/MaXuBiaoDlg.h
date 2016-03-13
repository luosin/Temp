#if !defined(AFX_MAXUBIAODLG_H__3492274E_6A41_4096_A128_594C681AC95F__INCLUDED_)
#define AFX_MAXUBIAODLG_H__3492274E_6A41_4096_A128_594C681AC95F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaXuBiaoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaXuBiaoDlg dialog

class CMaXuBiaoDlg : public CDialog
{
// Construction
public:
	CString m_strTitle;
	CMaXuBiaoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMaXuBiaoDlg)
	enum { IDD = IDD_MAXUBIAO };
	CString	m_strStationName;
	double	m_dSYT_Height;
	double	m_dFrameHeight;
	double	m_dBetweenTable;
	CString	m_strSavePath;
	int		m_iVersionFor;
	double	m_dWidthGuiDao;
	double	m_dWidthBianMa;
	double	m_dHeightGuiDao;
	double	m_dHeightBianMa;
	CString	m_strProjName;
	CString	m_strDrawNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaXuBiaoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaXuBiaoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBUTTONSelectDir();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAXUBIAODLG_H__3492274E_6A41_4096_A128_594C681AC95F__INCLUDED_)
