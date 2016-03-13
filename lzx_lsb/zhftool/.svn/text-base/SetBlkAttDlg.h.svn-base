#pragma once
#include "afxwin.h"


// CSetBlkAttDlg 对话框

class CSetBlkAttDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetBlkAttDlg)

public:
	CSetBlkAttDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetBlkAttDlg();

// 对话框数据
	enum { IDD = IDD_SETBLKATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strBlkName;
	CComboBox m_comboxBlkName;
protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeComboSetblkattBlkname();
};
