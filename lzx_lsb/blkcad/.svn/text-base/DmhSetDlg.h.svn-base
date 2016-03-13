#pragma once
#include "afxcmn.h"

UINT ThreadFuncDrawCircuit(LPVOID pParam) ;

typedef struct tagTHREADPARMS
{
	HWND hWnd ;
	TCHAR chPath[100];
}THREADPARMS;

// CDmhSetDlg 对话框

class CDmhSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CDmhSetDlg)

public:
	CDmhSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDmhSetDlg();

// 对话框数据
	enum { IDD = IDD_DMH_DOSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg LRESULT OnSetProgressRange(WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnSetProgressPos(WPARAM wParam, LPARAM lParam) ;
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam) ;

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSavePath;
	CProgressCtrl m_ctrlProgressCur;
	CProgressCtrl m_ctrlProgressAll;
	afx_msg void OnBnClickedButtonDmhSetSel();
	int m_iDwgType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strFitFor;
};
