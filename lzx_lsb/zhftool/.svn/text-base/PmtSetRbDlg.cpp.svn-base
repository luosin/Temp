// PmtSetRbDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PmtSetRbDlg.h"


// CPmtSetRbDlg 对话框

IMPLEMENT_DYNAMIC(CPmtSetRbDlg, CDialog)
CPmtSetRbDlg::CPmtSetRbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPmtSetRbDlg::IDD, pParent)
	, m_iType(0)
	, m_strXhjBlkPath(_T("C:\\block_zhf\\support\\xhj\\XHJ_CZ_HLB_0_1_Z_A_0.dwg"))
	, m_iJyjIndex(0)
	, m_bJyjWithDist(FALSE)
	, m_bXhjWithJyj(FALSE)
{
}

CPmtSetRbDlg::~CPmtSetRbDlg()
{
}

void CPmtSetRbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_PMTRB_XHJ, m_iType);
	DDX_Text(pDX, IDC_EDIT_PMTRB_XHJ_BLKNAME, m_strXhjBlkPath);
	DDX_Radio(pDX, IDC_RADIO_PMTRB_JYJ1, m_iJyjIndex);
	DDX_Check(pDX, IDC_CHECK_JYJWITHDIST, m_bJyjWithDist);
	DDX_Check(pDX, IDC_CHECK_RBSET_XHJWITHJYJ, m_bXhjWithJyj);
}


BEGIN_MESSAGE_MAP(CPmtSetRbDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PMTRB_XHJ_SEL, OnBnClickedButtonPmtrbXhjSel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_PMTRB_XHJ, IDC_RADIO_PMTRB_JCB, OnBnClickedRadioPmtrbXhj)
// 	ON_CONTROL_RANGE(IDC_RADIO_PMTRB_JYJ1, IDC_RADIO_PMTRB_JYJ4, OnBnClickedRadioPmtrbJyj1)
END_MESSAGE_MAP()


// CPmtSetRbDlg 消息处理程序

void CPmtSetRbDlg::OnBnClickedButtonPmtrbXhjSel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(文件(*.dwg)|*.dwg|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("选择图块文件") ;
	filedlg.m_ofn.lpstrInitialDir = m_strXhjBlkPath ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strXhjBlkPath = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

BOOL CPmtSetRbDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD iType = 0 ;
	CBlkUtility::QueryValueReg(_T("PMTRBSET"),  _T("ToRbType"), iType) ;
	m_iType = iType ;
	CBlkUtility::QueryValueReg(_T("PMTRBSET"), _T("XhjBlkPath"), m_strXhjBlkPath) ;
	DWORD iJyjIndex = 0 ;
	CBlkUtility::QueryValueReg(_T("PMTRBSET"),  _T("JyjIndex"), iJyjIndex) ;
	m_iJyjIndex = iJyjIndex ;
	DWORD iJyjWithDist = 0 ;
	CBlkUtility::QueryValueReg(_T("PMTRBSET"),  _T("JyjWithDist"), iJyjWithDist) ;
	m_bJyjWithDist = (iJyjWithDist==0?FALSE:TRUE) ;
	DWORD iXhjWithJyj = 1 ;
	CBlkUtility::QueryValueReg(_T("ZHFTOOLS"), _T("AddJyjWhenInsertXhj"), iXhjWithJyj) ;
	m_bXhjWithJyj = (iXhjWithJyj==0?FALSE:TRUE) ;
	if (m_iType!=0)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PMTRB_XHJ_BLKNAME) ;
		pEdit->EnableWindow(FALSE) ;
		CButton* pButton =  (CButton*)GetDlgItem(IDC_BUTTON_PMTRB_XHJ_SEL) ;
		pButton->EnableWindow(FALSE) ;
	}
	if (m_iType!=2)
	{
		for (int i=0; i<4; i++)
		{
			CButton* pButton =  (CButton*)GetDlgItem(IDC_RADIO_PMTRB_JYJ1+i) ;
			pButton->EnableWindow(FALSE) ;
		}
	}
	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPmtSetRbDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;
	CBlkUtility::SetValueReg(_T("PMTRBSET"),  _T("ToRbType"), (DWORD)m_iType) ;
	CBlkUtility::SetValueReg(_T("PMTRBSET"), _T("XhjBlkPath"), m_strXhjBlkPath) ;
	CBlkUtility::SetValueReg(_T("PMTRBSET"), _T("JyjIndex"), (DWORD)m_iJyjIndex) ;
	CBlkUtility::SetValueReg(_T("PMTRBSET"),  _T("JyjWithDist"), (DWORD)m_bJyjWithDist) ;
	g_bAddJyjWhenInsertXhj = m_bXhjWithJyj ;
	CBlkUtility::SetValueReg(_T("ZHFTOOLS"), _T("AddJyjWhenInsertXhj"), (DWORD)m_bXhjWithJyj) ;

	OnOK();
}

void CPmtSetRbDlg::OnBnClickedRadioPmtrbXhj(UINT id)
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bXhjCtrlEnable = FALSE ;
	if (id==IDC_RADIO_PMTRB_XHJ)
	{
		bXhjCtrlEnable = TRUE ;
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PMTRB_XHJ_BLKNAME) ;
	pEdit->EnableWindow(bXhjCtrlEnable) ;
	CButton* pButton =  (CButton*)GetDlgItem(IDC_BUTTON_PMTRB_XHJ_SEL) ;
	pButton->EnableWindow(bXhjCtrlEnable) ;

	BOOL bJyjCtrlEnable = FALSE ;
	if (id==IDC_RADIO_PMTRB_JYJ)
	{
		bJyjCtrlEnable = TRUE ;
	}
	for (int i=0; i<4; i++)
	{
		CButton* pButton =  (CButton*)GetDlgItem(IDC_RADIO_PMTRB_JYJ1+i) ;
		pButton->EnableWindow(bJyjCtrlEnable) ;
	}

}

// void CPmtSetRbDlg::OnBnClickedRadioPmtrbJyj1(UINT id)
// {
// 	// TODO: 在此添加控件通知处理程序代码
// }
