#pragma once


// CDmhOptionsDlg �Ի���

class CDmhOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CDmhOptionsDlg)

public:
	CDmhOptionsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDmhOptionsDlg();

// �Ի�������
	enum { IDD = IDD_DMH_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// 0:�����ƣ�1��������
	int m_i2Or4Line;
	// 0���������ۺϹ�1�������Ϸ�����
	int m_iFlOn;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int m_iFxpFrom;
	CString m_strFxpStart;
	afx_msg void OnBnClickedRadioDmhOptionFlfrom1();
	afx_msg void OnBnClickedRadioDmhOptionFlfrom2();
	CString m_strPreZ;
	CString m_strPreZY;
	CString m_strPreZZH;
	CString m_strNPlusZYLoc;
	// ���õ�λ��ͨ��Ժ���绯��
	CString m_strFitFor;
	// �Ƿ���û�����
	BOOL m_bUseHLP;
	CString m_strProjectName;
	CString m_strDwgSn;
};
