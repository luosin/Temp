// LocToNameDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "LocToNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocToNameDlg dialog


CLocToNameDlg::CLocToNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLocToNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocToNameDlg)
	m_strInCol = _T("D");
	m_iInRow = 3;
	m_strOutCol = _T("G");
	m_iOutRow = 3;
	m_iUpOrDown = 0;
	m_strXlsFile = _T("");
	m_iSheet = 1;
	//}}AFX_DATA_INIT
}


void CLocToNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocToNameDlg)
	DDX_Text(pDX, IDC_LOCTONAME_INCOL, m_strInCol);
	DDV_MaxChars(pDX, m_strInCol, 2);
	DDX_Text(pDX, IDC_LOCTONAME_INROW, m_iInRow);
	DDV_MinMaxInt(pDX, m_iInRow, 1, 10000);
	DDX_Text(pDX, IDC_LOCTONAME_OUTCOL, m_strOutCol);
	DDV_MaxChars(pDX, m_strOutCol, 2);
	DDX_Text(pDX, IDC_LOCTONAME_OUTROW, m_iOutRow);
	DDV_MinMaxInt(pDX, m_iOutRow, 1, 10000);
	DDX_Radio(pDX, IDC_LOCTONAME_RADIO1, m_iUpOrDown);
	DDX_Text(pDX, IDC_XLSFILENAME, m_strXlsFile);
	DDX_Text(pDX, IDC_LOCTONAME_SHEET, m_iSheet);
	DDV_MinMaxInt(pDX, m_iSheet, 1, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocToNameDlg, CDialog)
	//{{AFX_MSG_MAP(CLocToNameDlg)
	ON_BN_CLICKED(IDC_SELFILE, OnSelfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocToNameDlg message handlers

void CLocToNameDlg::OnSelfile() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(文件(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("选择Excel文件") ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strXlsFile = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
	
}

void CLocToNameDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData(TRUE) ;
	CDialog::OnOK();
}
