// BtMxbDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BtMxbDlg.h"
#include "SBDestination.h"


// CBtMxbDlg 对话框

IMPLEMENT_DYNAMIC(CBtMxbDlg, CDialog)
CBtMxbDlg::CBtMxbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBtMxbDlg::IDD, pParent)
	, m_strStation1(_T(""))
	, m_strStation2(_T(""))
	, m_strSavePath(_T(""))
{
}

CBtMxbDlg::~CBtMxbDlg()
{
}

void CBtMxbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BTMXB_STATION1, m_strStation1);
	DDX_Text(pDX, IDC_EDIT_BTMXB_STATION2, m_strStation2);
	DDX_Text(pDX, IDC_EDIT_BTMXB_SAVEPATH, m_strSavePath);
}


BEGIN_MESSAGE_MAP(CBtMxbDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BTMXB_SELPATH, OnBnClickedButtonBtmxbSelpath)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CBtMxbDlg 消息处理程序

void CBtMxbDlg::OnBnClickedButtonBtmxbSelpath()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;

	CSBDestination sb( NULL , IDS_BFF_TITLE_MXB ) ;
	sb.SetInitialSelection(m_strSavePath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strSavePath = sb.GetSelectedFolder() ;


	this->UpdateData(FALSE) ;
}

void CBtMxbDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码	
	OnOK();
}
