// DmhOptionsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DmhOptionsDlg.h"

// CDmhOptionsDlg 对话框

IMPLEMENT_DYNAMIC(CDmhOptionsDlg, CDialog)
CDmhOptionsDlg::CDmhOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDmhOptionsDlg::IDD, pParent)
	, m_i2Or4Line(0)
	, m_iFlOn(0)
	, m_iFxpFrom(0)
	, m_strFxpStart(_T(""))
	, m_strPreZ(_T(""))
	, m_strPreZY(_T(""))
	, m_strPreZZH(_T(""))
	, m_strNPlusZYLoc(_T(""))
	, m_strFitFor(_T(""))
	, m_bUseHLP(FALSE)
	, m_strProjectName(_T(""))
	, m_strDwgSn(_T(""))
{
}

CDmhOptionsDlg::~CDmhOptionsDlg()
{
}

void CDmhOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DMH_OPTION_TWOLINE, m_i2Or4Line);
	DDX_Radio(pDX, IDC_RADIO_DMH_OPTION_FLONZH, m_iFlOn);
	DDX_Radio(pDX, IDC_RADIO_DMH_OPTION_FLFROM1, m_iFxpFrom);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_FXPSTART, m_strFxpStart);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_PREZ, m_strPreZ);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_PREZY, m_strPreZY);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_PREZZH, m_strPreZZH);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_NPLUSLOC, m_strNPlusZYLoc);
	DDX_CBString(pDX, IDC_COMBO_DMH_OPTION_FITFOR, m_strFitFor);
	DDX_Check(pDX, IDC_CHECK_DMH_OPTION_USEHLP, m_bUseHLP);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_PROJECTNAME, m_strProjectName);
	DDX_Text(pDX, IDC_EDIT_DMH_OPTION_DWGSN, m_strDwgSn);
}


BEGIN_MESSAGE_MAP(CDmhOptionsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_DMH_OPTION_FLFROM1, OnBnClickedRadioDmhOptionFlfrom1)
	ON_BN_CLICKED(IDC_RADIO_DMH_OPTION_FLFROM2, OnBnClickedRadioDmhOptionFlfrom2)
END_MESSAGE_MAP()


// CDmhOptionsDlg 消息处理程序

BOOL CDmhOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD i2Or4Line = 0, iFlOn = 0, iFlFrom = 0, iUserHLP = 0 ;
	//CString strFxpStart ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("TwoOrFourLine"), i2Or4Line) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FlOnZhOrFxp"), iFlOn) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FlFrom"), iFlFrom) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FxpStart"), m_strFxpStart) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("PreZ"), m_strPreZ) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("PreZY"), m_strPreZY) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("PreZZH"), m_strPreZZH) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("NPlusZYLoc"), m_strNPlusZYLoc) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FitFor"), m_strFitFor) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("UseHLP"), iUserHLP) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("ProjectName"), m_strProjectName) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("DwgSn"), m_strDwgSn) ;
	m_i2Or4Line = i2Or4Line ;
	m_iFlOn = iFlOn ;
	m_iFxpFrom = iFlFrom ;
	m_bUseHLP = (iUserHLP==0?FALSE:TRUE) ;
	//m_strFxpStart = strFxpStart ;
	if (iFlFrom==1)
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DMH_OPTION_FXPSTART) ;
		pEdit->SetReadOnly(FALSE) ;
	}

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CDmhOptionsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;

	DWORD i2Or4Line = 0, iFlOn = 0, iFlFrom = 0, iUserHLP = 0 ;
	i2Or4Line = (DWORD)m_i2Or4Line ;
	iFlOn = (DWORD)m_iFlOn ;
	iFlFrom = (DWORD)m_iFxpFrom ;
	iUserHLP = (DWORD)m_bUseHLP ;

	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("TwoOrFourLine"), i2Or4Line) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("FlOnZhOrFxp"), iFlOn) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("FlFrom"), iFlFrom) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("FxpStart"), m_strFxpStart) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("PreZ"), m_strPreZ) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("PreZY"), m_strPreZY) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("PreZZH"), m_strPreZZH) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("NPlusZYLoc"), m_strNPlusZYLoc) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("FitFor"), m_strFitFor) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("UseHLP"), iUserHLP) ;
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("ProjectName"), m_strProjectName);
	CBlkUtility::SetValueReg(_T("DmhOptions"), _T("DwgSn"), m_strDwgSn) ;

	OnOK();
}

void CDmhOptionsDlg::OnBnClickedRadioDmhOptionFlfrom1()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DMH_OPTION_FXPSTART) ;
	pEdit->SetReadOnly(TRUE) ;
}

void CDmhOptionsDlg::OnBnClickedRadioDmhOptionFlfrom2()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DMH_OPTION_FXPSTART) ;
	pEdit->SetReadOnly(FALSE) ;
}
