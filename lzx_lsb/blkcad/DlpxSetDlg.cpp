// DlpxSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DlpxSetDlg.h"

// CDlpxSetDlg 对话框

IMPLEMENT_DYNAMIC(CDlpxSetDlg, CDialog)
CDlpxSetDlg::CDlpxSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlpxSetDlg::IDD, pParent)
	, m_iXHJ_DC(0)
{
}

CDlpxSetDlg::~CDlpxSetDlg()
{
}

void CDlpxSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_DLPX_SET_XHJ_DC, m_iXHJ_DC);
}


BEGIN_MESSAGE_MAP(CDlpxSetDlg, CDialog)
END_MESSAGE_MAP()


// CDlpxSetDlg 消息处理程序

void CDlpxSetDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData(TRUE) ;

	DWORD iXhjDcSet = 0 ;
	iXhjDcSet = (DWORD)m_iXHJ_DC ;
	CBlkUtility::SetValueReg(_T("DlpxSet"), _T("XHJ_DC"), iXhjDcSet) ;

	CDialog::OnOK();
}

BOOL CDlpxSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD iXhjDcSet = 0 ;
	CBlkUtility::QueryValueReg(_T("DlpxSet"), _T("XHJ_DC"), iXhjDcSet) ;
	m_iXHJ_DC = iXhjDcSet ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
