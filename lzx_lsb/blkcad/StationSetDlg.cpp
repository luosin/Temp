// StationSetDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "StationSetDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStationSetDlg dialog


CStationSetDlg::CStationSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStationSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStationSetDlg)
	m_strDwgNum = _T("");
	m_strStationName = _T("");
	m_strSavePath = _T("");
	//}}AFX_DATA_INIT
}


void CStationSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStationSetDlg)
	DDX_Text(pDX, IDC_EDIT_DWGNUM, m_strDwgNum);
	DDX_Text(pDX, IDC_EDIT_StationName, m_strStationName);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStationSetDlg, CDialog)
	//{{AFX_MSG_MAP(CStationSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnButtonSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStationSetDlg message handlers

void CStationSetDlg::OnButtonSel() 
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

BOOL CStationSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(m_strTitle) ;

	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\STATION") ))
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
		if (ERROR_SUCCESS == cReg.QueryValue(chSN2, _T("StationName"), &lSize2)) 
		{
			m_strStationName = chSN2 ;
		}
		else
		{
			cReg.SetValue(_T(""), _T("StationName")) ;
		}
		TCHAR  chSN3[200] ;
		chSN3[0] = _T('\0') ;
		DWORD lSize3 = sizeof(chSN3) ;
		if (ERROR_SUCCESS == cReg.QueryValue(chSN3, _T("DwgNum"), &lSize3)) 
		{
			m_strDwgNum = chSN3 ;
		}
		else
		{
			cReg.SetValue(_T(""), _T("DwgNum")) ;
		}

	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\STATION")))
		{
		}
	}
	
	cReg.Close() ;
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStationSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;
	
	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\STATION") ))
	{
		cReg.SetValue(m_strStationName,_T("StationName")) ;
		cReg.SetValue(m_strDwgNum,_T("DwgNum")) ;
		cReg.SetValue(m_strSavePath,_T("SavePath")) ;

	}	
	cReg.Close() ;
	
	CDialog::OnOK();
}
