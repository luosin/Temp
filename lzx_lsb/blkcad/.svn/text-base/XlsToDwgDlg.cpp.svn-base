// XlsToDwgDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "XlsToDwgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXlsToDwgDlg dialog


CXlsToDwgDlg::CXlsToDwgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXlsToDwgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXlsToDwgDlg)
	m_strEndCol = _T("Z");
	m_iEndRow = 30;
	m_strExcelFileLoc = _T("");
	m_strMark = _T("");
	m_strStartCol = _T("A");
	m_iStartRow = 1;
	m_iConvMode = -1;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CXlsToDwgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXlsToDwgDlg)
	DDX_Text(pDX, IDC_EDIT_XLSTODWG_ENDCOL, m_strEndCol);
	DDV_MaxChars(pDX, m_strEndCol, 2);
	DDX_Text(pDX, IDC_EDIT_XLSTODWG_ENDROW, m_iEndRow);
	DDV_MinMaxInt(pDX, m_iEndRow, 2, 5000);
	DDX_Text(pDX, IDC_EDIT_XLSTODWG_FILEPATH, m_strExcelFileLoc);
	DDX_Text(pDX, IDC_EDIT_XLSTODWG_MARKTEXT, m_strMark);
	DDX_Text(pDX, IDC_EDIT_XLSTODWG_STARTCOL, m_strStartCol);
	DDV_MaxChars(pDX, m_strStartCol, 2);
	DDX_Text(pDX, IDC_EDIT_XLSTODWG_STARTROW, m_iStartRow);
	DDV_MinMaxInt(pDX, m_iStartRow, 1, 1000);
	DDX_Radio(pDX, IDC_RADIO_XLSTODWG_TRANSMODE_ALL, m_iConvMode);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXlsToDwgDlg, CDialog)
	//{{AFX_MSG_MAP(CXlsToDwgDlg)
	ON_BN_CLICKED(IDC_BUTTON_XLSTODWG_SELECTFILE, OnButtonXlstodwgSelectfile)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXlsToDwgDlg message handlers
void CXlsToDwgDlg::OnButtonXlstodwgSelectfile() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(文件(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("选择Excel文件") ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strExcelFileLoc = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
	
}

BOOL CXlsToDwgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iConvMode = 0 ;
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CXlsToDwgDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData(TRUE) ;
	
	CDialog::OnOK();
}
