#pragma once


// CSetPlWidthDlg 对话框

class CSetPlWidthDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPlWidthDlg)

public:
	CSetPlWidthDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetPlWidthDlg();

// 对话框数据
	enum { IDD = IDD_SETPLWIDTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dFrom;
	double m_dTo;
protected:
	virtual void OnOK();
public:
	BOOL m_bIncludeBlk;
};
