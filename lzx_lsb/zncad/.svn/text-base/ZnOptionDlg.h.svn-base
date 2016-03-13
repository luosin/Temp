#pragma once


// CZnOptionDlg 对话框

class CZnOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(CZnOptionDlg)

public:
	CZnOptionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CZnOptionDlg();

// 对话框数据
	enum { IDD = IDD_ZNCADOPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 四线制（0）或二线制（1）
	int m_i4Or2Line;
	// 信号机灯丝个数，默认0:2芯；1，3芯
	int m_iDsNumXhj;
	// 道岔缺口报警个数，默认0:2芯；1,4芯
	int m_iQkNum;
	// 调车灯丝数量：默认0，无灯丝；1,2芯灯丝；2,3芯灯丝
	int m_iDcDsNum;
};
