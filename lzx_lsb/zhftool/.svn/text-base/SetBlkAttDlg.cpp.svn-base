// SetBlkAttDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SetBlkAttDlg.h"


// CSetBlkAttDlg 对话框

IMPLEMENT_DYNAMIC(CSetBlkAttDlg, CDialog)
CSetBlkAttDlg::CSetBlkAttDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBlkAttDlg::IDD, pParent)
	, m_strBlkName(_T(""))
{
}

CSetBlkAttDlg::~CSetBlkAttDlg()
{
}

void CSetBlkAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SETBLKATT_BLKNAME, m_comboxBlkName);
}


BEGIN_MESSAGE_MAP(CSetBlkAttDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_SETBLKATT_BLKNAME, OnCbnSelchangeComboSetblkattBlkname)
END_MESSAGE_MAP()


// CSetBlkAttDlg 消息处理程序

BOOL CSetBlkAttDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CBlkUtility::QueryValueReg(_T("SetBlkAtt"), _T("BlockName"), m_strBlkName) ;
// 	CBlkUtility::QueryValueReg(_T("SetBlkAtt"), _T("NewBlockName"), m_strNewBlk) ;
// 	CBlkUtility::QueryValueReg(_T("SetBlkAtt"), _T("NewBlockFold"), m_strFold) ;
// 	CBlkUtility::QueryValueReg(_T("SetBlkAtt"), _T("Att"), m_strAtt) ;
	this->UpdateData(FALSE) ;
	CUserSystem mysys ;
	CStringArray strArrayBlkName ;
	mysys.GetAllBlockTable(strArrayBlkName, 1) ;
	for (int i=0; i<strArrayBlkName.GetCount(); i++)
	{
		CString strBlkName = strArrayBlkName.GetAt(i) ;
		m_comboxBlkName.AddString(strBlkName) ;
		if (m_strBlkName==strBlkName)
		{
			m_comboxBlkName.SetCurSel(i) ;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CSetBlkAttDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData(TRUE) ;
	m_comboxBlkName.GetLBText(m_comboxBlkName.GetCurSel(), m_strBlkName) ;
	if (m_strBlkName==_T(""))
	{
		AfxMessageBox(_T("块名称不能为空！")) ;
		return ;
	}
	CBlkUtility::SetValueReg(_T("SetBlkAtt"), _T("BlockName"), m_strBlkName) ;


	CDialog::OnOK();
}

void CSetBlkAttDlg::OnCbnSelchangeComboSetblkattBlkname()
{
	// TODO: 在此添加控件通知处理程序代码
	m_comboxBlkName.GetLBText(m_comboxBlkName.GetCurSel(), m_strBlkName) ;
	acutPrintf(_T("\nyou selected %s"), m_strBlkName) ;
}
