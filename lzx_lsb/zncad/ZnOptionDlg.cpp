// ZnOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZnOptionDlg.h"
#include "ZnUtility.h"

// CZnOptionDlg 对话框

IMPLEMENT_DYNAMIC(CZnOptionDlg, CDialog)
CZnOptionDlg::CZnOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZnOptionDlg::IDD, pParent)
	, m_dCableCoeff(1.05)
	, m_iCableIntoXhl(50)
	, m_iCablePerGd(10)
	, m_iCablePerBetweenGd(10)
	, m_iCablePerBox(4)
	, m_iCablePerBoxF(4)
	, m_iCablePerBetweenBoxF(500)
	, m_dTolChekGd(0.001)
	, m_i4Or2Line(0)
	, m_iDsNumXhj(0)
	, m_iQkNum(0)
	, m_iDcDsNum(0)
{
}

CZnOptionDlg::~CZnOptionDlg()
{
}

void CZnOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_COEFFICIENT, m_dCableCoeff);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERINTOXHL, m_iCableIntoXhl);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERGD, m_iCablePerGd);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERBETWEENGD, m_iCablePerBetweenGd);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERBOX, m_iCablePerBox);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERBOXF, m_iCablePerBoxF);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_CABLE_PERBETWEENBOXF, m_iCablePerBetweenBoxF);
	DDX_Text(pDX, IDC_EDIT_ZNOPTION_TOL_CHECKGD, m_dTolChekGd);
	DDX_Radio(pDX, IDC_RADIO_ZNOPTION_4LINE, m_i4Or2Line);
	DDX_Radio(pDX, IDC_RADIO_ZNOPTION_DS2, m_iDsNumXhj);
	DDX_Radio(pDX, IDC_RADIO_ZNOPTION_QK2, m_iQkNum);
	DDX_Radio(pDX, IDC_RADIO_ZNOPTION_DCDS0, m_iDcDsNum);
}


BEGIN_MESSAGE_MAP(CZnOptionDlg, CDialog)
END_MESSAGE_MAP()


// CZnOptionDlg 消息处理程序

BOOL CZnOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CZnUtility::GetCableParam(m_dCableCoeff, m_iCableIntoXhl, m_iCablePerGd, m_iCablePerBetweenGd, m_iCablePerBox, m_iCablePerBoxF, m_iCablePerBetweenBoxF) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TolCheckGd"), m_dTolChekGd) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TwoOrFourLine"), m_i4Or2Line) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("XhjDsNum"), m_iDsNumXhj) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("DcQkNum"), m_iQkNum) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("DcDsNum"), m_iDcDsNum) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CZnOptionDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData(TRUE) ;

	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CableCoeff"), m_dCableCoeff) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CableIntoXhl"), m_iCableIntoXhl) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CablePerGd"), m_iCablePerGd) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CablePerBetweenGd"), m_iCablePerBetweenGd) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CablePerBox"), m_iCablePerBox) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CablePerBoxF"), m_iCablePerBoxF) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("CablePerBetweenBoxF"), m_iCablePerBetweenBoxF) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("TolCheckGd"), m_dTolChekGd) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("TwoOrFourLine"), m_i4Or2Line) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("XhjDsNum"), m_iDsNumXhj) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("DcQkNum"), m_iQkNum) ;
	CBlkUtility::SetValueReg(_T("ZnOption"), _T("DcDsNum"), m_iDcDsNum) ;


	CDialog::OnOK();
}
