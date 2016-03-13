#include "afxcoll.h"
#if !defined(AFX_GETDIANLANSUMDLG_H__64B169C7_71C7_4992_86AD_B5232122D38D__INCLUDED_)
#define AFX_GETDIANLANSUMDLG_H__64B169C7_71C7_4992_86AD_B5232122D38D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetDianlanSumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetDianlanSumDlg dialog

class CGetDianlanSumDlg : public CDialog
{
// Construction
public:
	CStringArray m_strArrayAllStr;
	int GetStrFromDwg(AcDbDatabase *pDb, CStringArray &strArrayLength1, CStringArray &strArrayType1,CStringArray &strArrayLength2, CStringArray &strArrayType2);
	CStringArray m_strArrayType2;
	CStringArray m_strArrayLength2;
	CStringArray m_strArrayType1;
	CStringArray m_strArrayLength1;

	CStringArray m_strArrayHaveType1;
	CStringArray m_strArrayHaveTypeLen1 ;
	CStringArray m_strArrayHaveType2;
	CStringArray m_strArrayHaveTypeLen2 ;

	CStringArray m_strArray1S1S ;
	CStringArray m_strArray1S2S ;
	CStringArray m_strArray1S3S ;

	CStringArray m_strArrayFilePath;
	CGetDianlanSumDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CGetDianlanSumDlg)
	enum { IDD = IDD_GETDIANLAN };
	CString	m_strDianLanDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetDianlanSumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetDianlanSumDlg)
	afx_msg void OnBUTTONDo();
	afx_msg void OnBUTTONGetDianlanDir();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strReg;
	BOOL m_bMoReg;
	afx_msg void OnBnClickedCheckGetdianlanMoreg();
	virtual BOOL OnInitDialog();
	void WriteToExcel(const CString &strSaveFileName);
	void GetHaveTypeLen(void);
	// 按目录/文件统计方式
	int m_nTongJiMode;
	// 统计箱盒数量
	int SumBoxFromDwg(AcDbDatabase *pDb, CMapStringToString & mapBoxTypeAndNum);
	CMapStringToString m_mapBoxTypeAndNum;
	// 统计轨道电路（1送1受、1送2受、1送3受）
	int SumGuiDaoDianLu(AcDbDatabase *pDb, CStringArray &strArray1S1S, CStringArray &strArray1S2S, CStringArray &strArray1S3S);
	CString m_strRegLen;
	CString m_strRegType;
	int m_iTwoOrFourLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETDIANLANSUMDLG_H__64B169C7_71C7_4992_86AD_B5232122D38D__INCLUDED_)
