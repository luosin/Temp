// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDlg.h"


// CTestDlg �Ի���

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


// CTestDlg ��Ϣ�������

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect ;
	GetClientRect(rect) ;	
	rect.DeflateRect(0,0,0,50) ;
	//m_grid.CreateGrid( WS_CHILD|WS_VISIBLE, rect, this, 23422 );


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
