#pragma once


// CBtMxbDlg 对话框

class CBtMxbDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtMxbDlg)

public:
	CBtMxbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBtMxbDlg();

// 对话框数据
	enum { IDD = IDD_BTMXB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strStation1;
	CString m_strStation2;
	CString m_strSavePath;
	afx_msg void OnBnClickedButtonBtmxbSelpath();
	afx_msg void OnBnClickedOk();
};
