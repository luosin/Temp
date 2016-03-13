#pragma once
#include "Resource.h"
#include "xhentugctrl.h"


// CEntPropertyDlg 对话框

class CEntPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CEntPropertyDlg)

public:
	CEntPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntPropertyDlg();

// 对话框数据
	enum { IDD = IDD_MOD_ENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CXhEntUGCtrl m_grid;
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
};
