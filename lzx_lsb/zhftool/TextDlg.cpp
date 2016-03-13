// TextDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "TextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_iSel = 7 ;
/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog


CTextDlg::CTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextDlg)
	m_strText = _T("");
	m_iSel = g_iSel ;
	iFlag = 0 ;
	//m_iSel = 0 ;
	//}}AFX_DATA_INIT
}


void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextDlg)
	DDX_Text(pDX, IDC_MODIFYTEXT_EDIT1, m_strText);
	DDX_Radio(pDX, IDC_MODIFYTEXT_RADIO1, m_iSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextDlg, CDialog)
	//{{AFX_MSG_MAP(CTextDlg)
	ON_BN_CLICKED(IDC_ConFirm, OnConFirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDlg message handlers

void CTextDlg::OnConFirm() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(true) ;
	/*
	CString str ;
	str.Format("color index: %d", m_iSel) ;
	AfxMessageBox(str) ;
	*/
	g_iSel = this->m_iSel ;
	this->pText->setTextString(this->m_strText) ;
	if (m_iSel!=7)
	{
		AcCmColor color ;
		color.setColorIndex(this->m_iSel+1) ;
		
		pEnt->setColor(color) ;

	}
	this->iFlag = 1 ;

	CDialog::OnOK() ;

	
}

BOOL CTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	this->m_strText = this->pText->textString() ;

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_MODIFYTEXT_EDIT1) ;
	//pEdit->SetActiveWindow();
	
	pEdit->SetSel(0, -1, false) ;
	//pEdit->SetTabStops(1) ;

	this->UpdateData(false) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->OnConFirm() ;
	
	//CDialog::OnOK();
}

void CTextDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
