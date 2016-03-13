#pragma once


// CConNode 对话框

class CConNode : public CDialog
{
	DECLARE_DYNAMIC(CConNode)

public:
	CConNode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConNode();

// 对话框数据
	enum { IDD = IDD_CON_NODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iType;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strPrompt1;
	CString m_strPrompt2;
	CString m_strNode;
	int m_iVal;
};
