// ChangeFileNameDlg.cpp : implementation file
//

#include "StdAfx.h"
//#include "resource.h"
#include "ChangeFileNameDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeFileNameDlg dialog


CChangeFileNameDlg::CChangeFileNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeFileNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeFileNameDlg)
	m_strFileExt = _T("");
	m_strFilePath = _T("");
	m_strNew = _T("");
	m_strOld = _T("");
	//}}AFX_DATA_INIT
}


void CChangeFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeFileNameDlg)
	DDX_CBString(pDX, IDC_COMBO_CHANGEFILENAME_EXT, m_strFileExt);
	DDX_Text(pDX, IDC_EDIT_CHANGEFILENAME_FILEPATH, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT_CHANGEFILENAME_NEW, m_strNew);
	DDX_Text(pDX, IDC_EDIT_CHANGEFILENAME_OLD, m_strOld);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeFileNameDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeFileNameDlg)
	ON_BN_CLICKED(IDC_BUTTON_CHANGEFILENAME_SELECT, OnButtonChangefilenameSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeFileNameDlg message handlers

void CChangeFileNameDlg::OnButtonChangefilenameSelect() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData() ;
	CSBDestination sb( m_hWnd , IDS_BFF_TITLE ) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(sb.SelectFolder())
	{
		this->m_strFilePath = sb.GetSelectedFolder() ;	
		this->UpdateData(FALSE) ;
	}
	
}

void CChangeFileNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;
	if (m_strOld.IsEmpty())
	{
		AfxMessageBox(_T("请输入要替换的字符串！")) ;
		return ;
	}
	else if (m_strFilePath.IsEmpty())
	{
		AfxMessageBox(_T("请选择路径！")) ;
		return ;
	}
	
	CUserSystem mysys ;
	if(CBlkUtility::IsStrMatch(_T("^\\*\\.[\\w,\\*]+$"), m_strFileExt)==FALSE)
	{
		AfxMessageBox(_T("请检查输入的后缀名是否是 *.后缀名 形式")) ;
		return ;
	}
	
	CDialog::OnOK();
}

BOOL CChangeFileNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_strFileExt = _T("*.*") ;
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
