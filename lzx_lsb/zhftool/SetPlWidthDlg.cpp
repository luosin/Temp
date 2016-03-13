// SetPlWidthDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetPlWidthDlg.h"

// CSetPlWidthDlg �Ի���

IMPLEMENT_DYNAMIC(CSetPlWidthDlg, CDialog)
CSetPlWidthDlg::CSetPlWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPlWidthDlg::IDD, pParent)
	, m_dFrom(0)
	, m_dTo(0)
	, m_bIncludeBlk(TRUE)
{
}

CSetPlWidthDlg::~CSetPlWidthDlg()
{
}

void CSetPlWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SETPLWIDTH_FROM, m_dFrom);
	DDV_MinMaxDouble(pDX, m_dFrom, 0, 10);
	DDX_Text(pDX, IDC_EDIT_SETPLWIDTH_TO, m_dTo);
	DDV_MinMaxDouble(pDX, m_dTo, 0, 1);
	DDX_Check(pDX, IDC_CHECK_SETPLWIDTH_INCLUDEBLK, m_bIncludeBlk);
}


BEGIN_MESSAGE_MAP(CSetPlWidthDlg, CDialog)
END_MESSAGE_MAP()


// CSetPlWidthDlg ��Ϣ�������

void CSetPlWidthDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialog::OnOK();
}
