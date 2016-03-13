// DLPXDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "DLPXDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLPXDlg dialog


CDLPXDlg::CDLPXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLPXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLPXDlg)
	m_strSavePath = _T("");
	m_strMode = _T("");
	//}}AFX_DATA_INIT
}


void CDLPXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLPXDlg)
	DDX_Text(pDX, IDC_EDIT_DLPX_SAVEPATH, m_strSavePath);
	DDX_CBString(pDX, IDC_COMBO_MODE, m_strMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLPXDlg, CDialog)
	//{{AFX_MSG_MAP(CDLPXDlg)
	ON_BN_CLICKED(IDC_BUTTON_DLPX_SELPATH, OnButtonDlpxSelpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLPXDlg message handlers

void CDLPXDlg::OnButtonDlpxSelpath() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData() ;
	
	CSBDestination sb( NULL , IDS_BFF_TITLE_MXB ) ;
	sb.SetInitialSelection(m_strSavePath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strSavePath = sb.GetSelectedFolder() ;	
	
	this->UpdateData(FALSE) ;	
}

void CDLPXDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;

	CBlkUtility::SetValueReg(_T("DLPX"),  _T("SavePath"), m_strSavePath) ;
	CBlkUtility::SetValueReg(_T("DLPX"),  _T("Mode"), m_strMode) ;


	CButton *pButton = (CButton *)this->GetDlgItem(IDOK) ;
	pButton->EnableWindow(FALSE) ;
	CButton *pButton2 = (CButton *)this->GetDlgItem(IDCANCEL) ;
	pButton2->EnableWindow(FALSE) ;
	
	CDialog::OnOK();
}

BOOL CDLPXDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBlkUtility::QueryValueReg(_T("DLPX"),  _T("SavePath"), m_strSavePath) ;
	CBlkUtility::QueryValueReg(_T("DLPX"),  _T("Mode"), m_strMode) ;

	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
