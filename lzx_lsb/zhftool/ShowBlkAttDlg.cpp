// ShowBlkAttDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowBlkAttDlg.h"

// CShowBlkAttDlg 对话框

IMPLEMENT_DYNAMIC(CShowBlkAttDlg, CDialog)
CShowBlkAttDlg::CShowBlkAttDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowBlkAttDlg::IDD, pParent)
	, m_strBlkName(_T(""))
	, m_strAtts(_T(""))
	, m_iShowOrHide(0)
{
}

CShowBlkAttDlg::~CShowBlkAttDlg()
{
}

void CShowBlkAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOWBLKATT_BLKNAME, m_strBlkName);
	DDX_Text(pDX, IDC_EDIT_SHOWBLKATT_ATTS, m_strAtts);
	DDX_Radio(pDX, IDC_RADIO_SHOWBLKATT_SHOW, m_iShowOrHide);
}


BEGIN_MESSAGE_MAP(CShowBlkAttDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CShowBlkAttDlg 消息处理程序

void CShowBlkAttDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;
	m_strBlkName.Trim() ;
	m_strAtts.Trim() ;

	if (m_strBlkName.IsEmpty()||m_strAtts.IsEmpty())
	{
		AfxMessageBox(_T("块名或者属性为空，请输入或者取消本次操作！")) ;
		return ;
	}

	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("BlockName"), m_strBlkName) ;
	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("BlockAtts"), m_strAtts) ;
	DWORD dwShow = static_cast<DWORD>(m_iShowOrHide) ;
	CBlkUtility::SetValueReg(_T("SHOWBLKATT"), _T("ShowOrHide"), dwShow) ;


	OnOK();
}

BOOL CShowBlkAttDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("BlockName"), m_strBlkName) ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("BlockAtts"), m_strAtts) ;
	DWORD dwShow = 0 ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("ShowOrHide"), dwShow) ;
	m_iShowOrHide = static_cast<int>(dwShow) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
