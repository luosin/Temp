#pragma once
//#include "Resource.h"


// CPmtSetRbDlg 对话框

class CPmtSetRbDlg : public CDialog
{
	DECLARE_DYNAMIC(CPmtSetRbDlg)

public:
	CPmtSetRbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPmtSetRbDlg();

// 对话框数据
	enum { IDD = IDD_PMT_SET_REPLACEBLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iType;
	CString m_strXhjBlkPath;
	afx_msg void OnBnClickedButtonPmtrbXhjSel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioPmtrbXhj(UINT id);
	int m_iJyjIndex;
// 绝缘节带不带属性（距离）
	BOOL m_bJyjWithDist;
	BOOL m_bXhjWithJyj;
};
