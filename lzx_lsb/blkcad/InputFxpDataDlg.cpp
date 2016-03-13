// InputFxpDataDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "InputFxpDataDlg.h"

#include "CableNetForJY.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputFxpDataDlg dialog


CInputFxpDataDlg::CInputFxpDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputFxpDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputFxpDataDlg)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
}


void CInputFxpDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputFxpDataDlg)
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputFxpDataDlg, CDialog)
	//{{AFX_MSG_MAP(CInputFxpDataDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELFILE, OnButtonSelfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputFxpDataDlg message handlers

void CInputFxpDataDlg::OnButtonSelfile() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData() ;

	CFileDialog filedlg(false,NULL,NULL,NULL,_T("(分线盘数据(*.xls)|*.xls|"),NULL);
	filedlg.m_ofn.lpstrTitle = _T( "选择分线盘数据文件");
	if(filedlg.DoModal() == IDOK)
	{
		m_strFilePath = filedlg.GetPathName( ) ;
	}
	
	this->UpdateData(FALSE) ;	
}

void CInputFxpDataDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;
	
	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\DLPX") ))
	{
		cReg.SetValue(m_strFilePath,_T("FxpPath")) ;
		
	}	
	cReg.Close() ;
	
	CButton *pButton = (CButton *)this->GetDlgItem(IDOK) ;
	pButton->EnableWindow(FALSE) ;
	CButton *pButton2 = (CButton *)this->GetDlgItem(IDCANCEL) ;
	pButton2->EnableWindow(FALSE) ;
	
	
	if (this->m_strFilePath!="")
	{
		CCableNetForJY cablenet ;
		int iRet = cablenet.ReadFxpDataToDb(m_strFilePath) ;
		CString strMsg = _T("") ;
		strMsg.Format(_T("导入分线盘数据 %d 条，请核对!"), iRet) ;
		AfxMessageBox(strMsg) ;
	}
	else
	{
		AfxMessageBox(_T("请指定分线盘数据所在的excel文件位置!")) ;
	}


	
	CDialog::OnOK();
}

BOOL CInputFxpDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\DLPX") ))
	{		
		TCHAR  chSN[100] ;
		chSN[0] = _T( '\0') ;
		DWORD lSize = sizeof(chSN) ;
		if (ERROR_SUCCESS == cReg.QueryValue(chSN, _T("FxpPath"), &lSize)) 
		{
			m_strFilePath = chSN ;
		}
		else
		{
			cReg.SetValue(_T(""), _T( "FxpPath")) ;
		}
	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD\\DLPX")))
		{
		}
	}
	
	cReg.Close() ;
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
