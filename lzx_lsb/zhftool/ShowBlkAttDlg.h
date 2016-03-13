#pragma once


// CShowBlkAttDlg 对话框

class CShowBlkAttDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowBlkAttDlg)

public:
	CShowBlkAttDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowBlkAttDlg();

// 对话框数据
	enum { IDD = IDD_SHOWBLKATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBlkName;
	CString m_strAtts;
	int m_iShowOrHide;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
