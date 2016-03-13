#pragma once


// CReadBztDlg 对话框

class CReadBztDlg : public CDialog
{
	DECLARE_DYNAMIC(CReadBztDlg)

public:
	CReadBztDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReadBztDlg();

// 对话框数据
	enum { IDD = IDD_READBZT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelpath();
	afx_msg void OnBnClickedButtonSelz();
	afx_msg void OnBnClickedButtonSely();
	afx_msg void OnBnClickedButtonSelzh();
	afx_msg void OnBnClickedButtonSelg();
	afx_msg void OnBnClickedButtonSelf();
	CString m_strDir;
	// 组合柜文件
	CString m_strFileZ;
	// 移频柜文件
	CString m_strFileY;
	// 综合柜文件
	CString m_strFileZH;
	// 轨道柜文件
	CString m_strFileG;
	// 分线柜文件
	CString m_strFileF;
	afx_msg void OnBnClickedOk();
};
