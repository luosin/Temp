#pragma once


// CZnOptionDlg �Ի���

class CZnOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CZnOptionDlg)

public:
	CZnOptionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZnOptionDlg();

// �Ի�������
	enum { IDD = IDD_ZNCADOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();

public:
	double m_dCableCoeff;
	int m_iCableIntoXhl;
	int m_iCablePerGd;
	int m_iCablePerBetweenGd;
	int m_iCablePerBox;
	int m_iCablePerBoxF;
	int m_iCablePerBetweenBoxF;
	double m_dTolChekGd;
	// �����ƣ�0��������ƣ�1��
	int m_i4Or2Line;
	// �źŻ���˿������Ĭ��0:2о��1��3о
	int m_iDsNumXhj;
	// ����ȱ�ڱ���������Ĭ��0:2о��1,4о
	int m_iQkNum;
	// ������˿������Ĭ��0���޵�˿��1,2о��˿��2,3о��˿
	int m_iDcDsNum;
};
