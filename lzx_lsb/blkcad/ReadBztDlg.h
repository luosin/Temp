#pragma once


// CReadBztDlg �Ի���

class CReadBztDlg : public CDialog
{
	DECLARE_DYNAMIC(CReadBztDlg)

public:
	CReadBztDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReadBztDlg();

// �Ի�������
	enum { IDD = IDD_READBZT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelpath();
	afx_msg void OnBnClickedButtonSelz();
	afx_msg void OnBnClickedButtonSely();
	afx_msg void OnBnClickedButtonSelzh();
	afx_msg void OnBnClickedButtonSelg();
	afx_msg void OnBnClickedButtonSelf();
	CString m_strDir;
	// ��Ϲ��ļ�
	CString m_strFileZ;
	// ��Ƶ���ļ�
	CString m_strFileY;
	// �ۺϹ��ļ�
	CString m_strFileZH;
	// ������ļ�
	CString m_strFileG;
	// ���߹��ļ�
	CString m_strFileF;
	afx_msg void OnBnClickedOk();
};
