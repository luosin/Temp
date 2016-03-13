#pragma once


// CReadLkForPmtDlg 对话框

class CReadLkForPmtDlg : public CDialog
{
	DECLARE_DYNAMIC(CReadLkForPmtDlg)

public:
	CReadLkForPmtDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReadLkForPmtDlg();

// 对话框数据
	enum { IDD = IDD_READLKFORPMT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPath;
	CString m_strXhFile;
	CString m_strYdqFile;
	int m_nPmtOption;
	afx_msg void OnBnClickedButtonReadlkforpmtSelpath();
	afx_msg void OnBnClickedButtonReadlkforpmtSelxh();
	afx_msg void OnBnClickedButtonReadlkforpmtSelydq();
protected:
	virtual void OnOK();
public:
	BOOL m_bSelXH;
	BOOL m_bSelYDQ;
//	afx_msg void OnBnClickedCheckReadlkforpmtSelxh();
};
