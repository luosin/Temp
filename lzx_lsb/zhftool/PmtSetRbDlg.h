#pragma once
//#include "Resource.h"


// CPmtSetRbDlg �Ի���

class CPmtSetRbDlg : public CDialog
{
	DECLARE_DYNAMIC(CPmtSetRbDlg)

public:
	CPmtSetRbDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPmtSetRbDlg();

// �Ի�������
	enum { IDD = IDD_PMT_SET_REPLACEBLOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_iType;
	CString m_strXhjBlkPath;
	afx_msg void OnBnClickedButtonPmtrbXhjSel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioPmtrbXhj(UINT id);
	int m_iJyjIndex;
// ��Ե�ڴ��������ԣ����룩
	BOOL m_bJyjWithDist;
	BOOL m_bXhjWithJyj;
};
