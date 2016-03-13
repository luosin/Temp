// ConNode.cpp : 实现文件
//

#include "stdafx.h"
#include "ConNode.h"


// CConNode 对话框

IMPLEMENT_DYNAMIC(CConNode, CDialog)
CConNode::CConNode(CWnd* pParent /*=NULL*/)
	: CDialog(CConNode::IDD, pParent)
	, m_iType(0)
	, m_strPrompt1(_T(""))
	, m_strPrompt2(_T(""))
	, m_strNode(_T(""))
	, m_iVal(1)
{
}

CConNode::~CConNode()
{
}

void CConNode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CON_NODE_TIE, m_strNode);
	DDX_Radio(pDX, IDC_RADIO_CON_NODE_1, m_iVal);
}


BEGIN_MESSAGE_MAP(CConNode, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CConNode 消息处理程序

void CConNode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE ) ;	
	OnOK();
}

BOOL CConNode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton * pBut = NULL ;

	pBut = ( CButton * ) this->GetDlgItem( IDC_RADIO_CON_NODE_1) ;
	pBut->SetWindowText( m_strPrompt1 ) ;
	pBut = ( CButton * ) this->GetDlgItem( IDC_RADIO_CON_NODE_2 ) ;
	pBut->SetWindowText( m_strPrompt2 ) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
