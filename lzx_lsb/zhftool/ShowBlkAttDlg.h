#pragma once


// CShowBlkAttDlg �Ի���

class CShowBlkAttDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowBlkAttDlg)

public:
	CShowBlkAttDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowBlkAttDlg();

// �Ի�������
	enum { IDD = IDD_SHOWBLKATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBlkName;
	CString m_strAtts;
	int m_iShowOrHide;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
