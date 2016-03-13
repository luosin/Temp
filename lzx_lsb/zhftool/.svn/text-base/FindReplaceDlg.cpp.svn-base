// FindReplaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindReplaceDlg.h"
#include "../blkuserdll/UserSystem.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_iColorSel = 0 ; 
CString g_strFindTxt = _T("") ;
CString g_strReplaceTxt = _T("") ;
BOOL g_bCaseSensitive = FALSE ;

/////////////////////////////////////////////////////////////////////////////
// CFindReplaceDlg dialog


CFindReplaceDlg::CFindReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindReplaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindReplaceDlg)
	m_strFindTxt = g_strFindTxt ;
	m_strReplaceTxt = g_strReplaceTxt ;
	m_strFilePath = _T("") ;
	m_bCase = g_bCaseSensitive ;
	m_bReg = 0 ;
	m_iRange = 0 ;
	//}}AFX_DATA_INIT
}


void CFindReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindReplaceDlg)
	DDX_Control(pDX, IDC_COMBO_COLOR, m_colorIndex);
	DDX_Text(pDX, IDC_EDIT_FIND, m_strFindTxt);
	DDX_Text(pDX, IDC_EDIT_REPLACE, m_strReplaceTxt);
	DDX_Check(pDX, IDC_CHECK_CASE, m_bCase);
	DDX_Check(pDX, IDC_CHECK_REG, m_bReg);
	DDX_Radio(pDX, IDC_RADIO_SELECTION, m_iRange) ;
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CFindReplaceDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELDIR, OnButtonSelDir)
	ON_COMMAND_RANGE(IDC_RADIO_SELECTION, IDC_RADIO_FILEPATH, OnRadioFilePath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindReplaceDlg message handlers

void CFindReplaceDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData(TRUE) ;
	
	g_iColorSel = this->m_colorIndex.GetCurSel() ;
	g_strFindTxt = this->m_strFindTxt ;
	g_strReplaceTxt = this->m_strReplaceTxt ;
	g_bCaseSensitive = m_bCase ;

	CDialog::OnOK();
}

BOOL CFindReplaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_colorIndex.SetCurSel(g_iColorSel) ;
	if (m_iRange!=2)
	{
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_SELDIR) ;
		pButton->EnableWindow(FALSE) ;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFindReplaceDlg::OnRadioFilePath(UINT nID)
{
	CButton *pButton = (CButton *)this->GetDlgItem(IDC_BUTTON_SELDIR) ;
	pButton->EnableWindow(nID==IDC_RADIO_FILEPATH?TRUE:FALSE) ;

}
void CFindReplaceDlg::OnButtonSelDir()
{
	this->UpdateData() ;
	CSBDestination sb( m_hWnd , IDS_BFF_TITLE ) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(sb.SelectFolder())
	{
		this->m_strFilePath = sb.GetSelectedFolder() ;	
		this->UpdateData(FALSE) ;
	}
}