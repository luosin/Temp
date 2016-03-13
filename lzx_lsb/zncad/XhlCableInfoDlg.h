#pragma once
#include "Resource.h"

// CXhlCableInfoDlg 对话框

class CXhlCableInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CXhlCableInfoDlg)

public:
	CXhlCableInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXhlCableInfoDlg();

// 对话框数据
	enum { IDD = IDD_XHLCABLEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnTextChanged(WPARAM wparam,LPARAM lparam);
	afx_msg void OnEnChangeEditXhlcableIntoxhl();
	afx_msg void OnEnChangeEditXhlcableCrossgd();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void OnOK();
public:
	double m_dSumOld;
	int m_iCableLenIntoXhl;
	int UpdateCableLen(void);
	AcDbEntity* m_pEnt;
	int m_iCrossGd;
	int m_iPerGd;
	int m_iPerBwtweenGd;
	afx_msg void OnBnClickedOk();
};
