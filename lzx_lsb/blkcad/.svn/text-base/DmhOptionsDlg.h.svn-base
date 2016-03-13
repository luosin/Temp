#pragma once


// CDmhOptionsDlg 对话框

class CDmhOptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CDmhOptionsDlg)

public:
	CDmhOptionsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDmhOptionsDlg();

// 对话框数据
	enum { IDD = IDD_DMH_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 0:二线制，1：四线制
	int m_i2Or4Line;
	// 0：防雷上综合柜，1：防雷上分线盘
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
	// 适用单位：通号院、电化局
	CString m_strFitFor;
	// 是否采用汇流排
	BOOL m_bUseHLP;
	CString m_strProjectName;
	CString m_strDwgSn;
};
