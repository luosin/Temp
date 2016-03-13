#pragma once
#include "afxwin.h"


// CConvertTextDlg 对话框

class CConvertTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertTextDlg)

public:
	CConvertTextDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConvertTextDlg();

// 对话框数据
	enum { IDD = IDD_CONVERTTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrColorIndex;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonConvAtt();
	virtual BOOL OnInitDialog();
	int m_iTransRangeIndex;
};
