// ModifyTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModifyTextStyleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyTextStyleDlg dialog


CModifyTextStyleDlg::CModifyTextStyleDlg(CWnd* pParent /*=NULL*/)
: CDialog(CModifyTextStyleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyTextStyleDlg)
	m_dHeight = 2.00;
	m_dRotation = 0;
	m_dWidthFactor = 0.70 ;
	m_bSet = FALSE;

	m_iTextJustifyCurSel = 0 ;
	m_iTextStyleCurSel = 0 ;
	//}}AFX_DATA_INIT
}


void CModifyTextStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyTextStyleDlg)
	DDX_Control(pDX, IDC_COMBO_STYLE, m_TextStyleIndex);
	DDX_Control(pDX, IDC_COMBO_JUSTIFY, m_TextJustifyIndex);
	DDX_Text(pDX, IDC_EDIT_Height, m_dHeight);
	DDX_Text(pDX, IDC_EDIT_Rotation, m_dRotation);
	DDX_Text(pDX, IDC_EDIT_WidthFactor, m_dWidthFactor);
	DDX_Check(pDX, IDC_CHECK_SET, m_bSet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyTextStyleDlg, CDialog)
//{{AFX_MSG_MAP(CModifyTextStyleDlg)
ON_BN_CLICKED(IDC_CHECK_SET, OnCheckSet)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyTextStyleDlg message handlers

void CModifyTextStyleDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData(TRUE) ;
	this->m_iTextJustifyCurSel = this->m_TextJustifyIndex.GetCurSel()  ;
	this->m_iTextStyleCurSel = this->m_TextStyleIndex.GetCurSel()  ;
	this->m_TextStyleIndex.GetLBText(m_iTextStyleCurSel, m_strTextStyle) ;					
	
	CDialog::OnOK();
}

BOOL CModifyTextStyleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_TextJustifyIndex.SetCurSel(m_iTextJustifyCurSel) ;
	this->m_TextStyleIndex.AddString(_T("Ä¬ÈÏ²»±ä")) ;
	for(int i=0; i<m_strArrayTextStyle.GetSize(); i++)
	{
		this->m_TextStyleIndex.AddString(m_strArrayTextStyle.GetAt(i)) ;
	}
	this->m_TextStyleIndex.SetCurSel(m_iTextStyleCurSel) ;

    this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyTextStyleDlg::OnCheckSet() 
{
	// TODO: Add your control notification handler code here
	CEdit * pEdit_Height = (CEdit *)GetDlgItem(IDC_EDIT_Height) ;
	CEdit * pEdit_WidthFactor = (CEdit *)GetDlgItem(IDC_EDIT_WidthFactor) ;
	CEdit * pEdit_Rotation = (CEdit *)GetDlgItem(IDC_EDIT_Rotation) ;
	
	pEdit_Height->EnableWindow(!pEdit_Height->IsWindowEnabled()) ;
	pEdit_WidthFactor->EnableWindow(!pEdit_WidthFactor->IsWindowEnabled()) ;
	pEdit_Rotation->EnableWindow(!pEdit_Rotation->IsWindowEnabled()) ;	
	
	
}
