// GetHeightWidthDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "GetHeightWidthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetHeightWidthDlg dialog


CGetHeightWidthDlg::CGetHeightWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetHeightWidthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetHeightWidthDlg)
	m_dCadHeight = 0.0;
	m_dCadWidth = 0.0;
	m_dExcelHeight = 0.0;
	m_dExcelWidth = 0.0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGetHeightWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetHeightWidthDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_GETHW_CAD_H, m_dCadHeight);
	DDX_Text(pDX, IDC_EDIT_GETHW_CAD_W, m_dCadWidth);
	DDX_Text(pDX, IDC_EDIT_GETHW_EXCEL_H, m_dExcelHeight);
	DDX_Text(pDX, IDC_EDIT_GETHW_EXCEL_W, m_dExcelWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetHeightWidthDlg, CDialog)
	//{{AFX_MSG_MAP(CGetHeightWidthDlg)
		// NOTE: the ClassWizard will add message map macros here
		ON_BN_CLICKED(IDC_BUTTON_GETHW_DONE, OnButtonGethwDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetHeightWidthDlg message handlers
void CGetHeightWidthDlg::OnButtonGethwDone() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE) ;
	this->m_dExcelHeight = this->m_dCadHeight/0.328 ;
	this->m_dExcelWidth = this->m_dCadWidth/2.41 ;
	this->UpdateData(FALSE) ;
	
}