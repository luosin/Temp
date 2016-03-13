// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDlg.h"


// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)
CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect ;
	GetClientRect(rect) ;	
	rect.DeflateRect(0,0,0,50) ;
	//m_grid.CreateGrid( WS_CHILD|WS_VISIBLE, rect, this, 23422 );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
