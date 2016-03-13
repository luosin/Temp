// LkDataSetDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "LkDataSetDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLkDataSetDlg dialog


CLkDataSetDlg::CLkDataSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLkDataSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLkDataSetDlg)
	m_strLineName = _T("");
	m_strSavePath = _T("");
	m_iBaOffset = 0;
	//}}AFX_DATA_INIT
}


void CLkDataSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLkDataSetDlg)
	DDX_Text(pDX, IDC_EDIT_LINENAME, m_strLineName);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	DDX_Text(pDX, IDC_EDIT_BAOFFSET, m_iBaOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLkDataSetDlg, CDialog)
	//{{AFX_MSG_MAP(CLkDataSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELPATH, OnButtonSelpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLkDataSetDlg message handlers

void CLkDataSetDlg::OnButtonSelpath() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData() ;
	CSBDestination sb( m_hWnd , IDS_BFF_TITLE ) ;
	sb.SetInitialSelection(m_strSavePath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(sb.SelectFolder())
	{
		this->m_strSavePath = sb.GetSelectedFolder() ;	
		this->UpdateData(FALSE) ;
	}
	
}

BOOL CLkDataSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(m_strTitle) ;
	if (m_strTitle==_T("生成列控数据表-信号数据表"))
	{
		CButton *pButton = (CButton*)this->GetDlgItem(IDC_STATIC_BAOFFSET) ;
		pButton->ShowWindow(SW_SHOW) ;
		CEdit *pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT_BAOFFSET) ;
		pEdit->ShowWindow(SW_SHOW) ;	
	}
	if (m_strTitle==_T("生成列控数据表-进路信息表"))
	{
		this->SetDlgItemText(IDC_STATIC_LKDATASET_NAME, _T("车站名称")) ;	
	}
	
	
	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\LKDATA") ))
	{		
		TCHAR  chSN[200] ;
		chSN[0] = _T('\0') ;
		
		DWORD lSize = sizeof(chSN) ;
		
		if (ERROR_SUCCESS == cReg.QueryValue(chSN, _T("SavePath"), &lSize)) 
		{
			m_strSavePath = chSN ;
		}
		else
		{
			cReg.SetValue(_T(""), _T("SavePath")) ;
		}
		TCHAR  chSN2[200] ;
		chSN2[0] = _T('\0') ;		
		DWORD lSize2 = sizeof(chSN2) ;
		if (ERROR_SUCCESS == cReg.QueryValue(chSN2, _T("LineName"), &lSize2)) 
		{
			m_strLineName = chSN2 ;
		}
		else
		{
			cReg.SetValue(_T(""), _T("LineName")) ;
		}

		DWORD iBaOffset = 0 ;	
		if (ERROR_SUCCESS == cReg.QueryValue(iBaOffset, _T("BaOffset"))) 
		{
			m_iBaOffset = iBaOffset ;
		}
		else
		{
			cReg.SetValue(iBaOffset, _T("BaOffset")) ;
		}
		
	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\LKDATA")))
		{
		}
	}
	
	cReg.Close() ;
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLkDataSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;
	
	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\LKDATA") ))
	{
		cReg.SetValue(m_strLineName,_T("LineName")) ;
		// cReg.SetValue(m_strDwgNum,"DwgNum") ;
		cReg.SetValue(m_strSavePath,_T("SavePath")) ;
		cReg.SetValue(m_iBaOffset,_T("BaOffset")) ;
		
	}	
	cReg.Close() ;

	
	CDialog::OnOK();
}
