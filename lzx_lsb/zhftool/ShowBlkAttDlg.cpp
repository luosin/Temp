// ShowBlkAttDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowBlkAttDlg.h"

// CShowBlkAttDlg �Ի���

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


// CShowBlkAttDlg ��Ϣ�������

void CShowBlkAttDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;
	m_strBlkName.Trim() ;
	m_strAtts.Trim() ;

	if (m_strBlkName.IsEmpty()||m_strAtts.IsEmpty())
	{
		AfxMessageBox(_T("������������Ϊ�գ����������ȡ�����β�����")) ;
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("BlockName"), m_strBlkName) ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("BlockAtts"), m_strAtts) ;
	DWORD dwShow = 0 ;
	CBlkUtility::QueryValueReg(_T("SHOWBLKATT"), _T("ShowOrHide"), dwShow) ;
	m_iShowOrHide = static_cast<int>(dwShow) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
