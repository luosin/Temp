#pragma once
#include "afxwin.h"


// CConvertTextDlg �Ի���

class CConvertTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CConvertTextDlg)

public:
	CConvertTextDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConvertTextDlg();

// �Ի�������
	enum { IDD = IDD_CONVERTTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrColorIndex;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonConvAtt();
	virtual BOOL OnInitDialog();
	int m_iTransRangeIndex;
};
