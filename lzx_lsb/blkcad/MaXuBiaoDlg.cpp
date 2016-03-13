// MaXuBiaoDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "MaXuBiaoDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaXuBiaoDlg dialog


CMaXuBiaoDlg::CMaXuBiaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaXuBiaoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaXuBiaoDlg)
	m_strStationName = _T("");
	m_dSYT_Height = 0.0;
	m_dFrameHeight = 0.0;
	m_dBetweenTable = 0.0;
	m_strSavePath = _T("");
	m_iVersionFor = 0;
	m_dWidthGuiDao = 0.7;
	m_dWidthBianMa = 0.7;
	m_dHeightGuiDao = 4.0;
	m_dHeightBianMa = 4.0;
	m_strProjName = _T("");
	m_strDrawNum = _T("");
	//}}AFX_DATA_INIT
}


void CMaXuBiaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaXuBiaoDlg)
	DDX_Text(pDX, IDC_EDIT_StationName, m_strStationName);
	DDX_Text(pDX, IDC_EDIT_SYT_HEIGHT, m_dSYT_Height);
	DDX_Text(pDX, IDC_EDIT_FRAMEHEIGHT, m_dFrameHeight);
	DDX_Text(pDX, IDC_EDIT_BETWEENTABLE, m_dBetweenTable);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strSavePath);
	DDX_CBIndex(pDX, IDC_COMBO_MAXUBIAO_TYPE, m_iVersionFor);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTWIDTH_GUIDAO, m_dWidthGuiDao);
	DDV_MinMaxDouble(pDX, m_dWidthGuiDao, 0.1, 1.);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTWIDTH_BIANMA, m_dWidthBianMa);
	DDV_MinMaxDouble(pDX, m_dWidthBianMa, 0.1, 1.);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTHEIGHT_GUIDAO, m_dHeightGuiDao);
	DDV_MinMaxDouble(pDX, m_dHeightGuiDao, 1., 10.);
	DDX_Text(pDX, IDC_EDIT_MXB_FONTHEIGHT_BIANMA, m_dHeightBianMa);
	DDV_MinMaxDouble(pDX, m_dHeightBianMa, 1., 10.);
	DDX_Text(pDX, IDC_EDIT_MXB_PROJNAME, m_strProjName);
	DDX_Text(pDX, IDC_EDIT_MXB_DRAWNUM, m_strDrawNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaXuBiaoDlg, CDialog)
	//{{AFX_MSG_MAP(CMaXuBiaoDlg)
	ON_BN_CLICKED(IDC_BUTTON_SelectDir, OnBUTTONSelectDir)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaXuBiaoDlg message handlers

BOOL CMaXuBiaoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText(m_strTitle) ;
	
	CString strInitDir = _T("c:\\") ;
	int iVersionFor = 0 ;
	CString strSetFile ;
	CString strBasePath ;
	CBlkUtility::GetBasePath(strBasePath) ;
	strSetFile.Format(_T("%s\\myset_2002.txt"), strBasePath) ;
	CString strStation, strProjName, strDrawNum ;
	CFile file(strSetFile, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate) ;
	int iLength = file.GetLength() ;
	if (iLength==0)
	{
		CArchive saveAr(&file, CArchive::store) ;
		saveAr<<strInitDir<<iVersionFor<<strStation<<strProjName<<strDrawNum ;
		saveAr.Flush() ;
		saveAr.Close() ;
	}
	file.Close() ;
	
	file.Open(strSetFile, CFile::modeRead) ;	
	CArchive readAr(&file, CArchive::load) ;
	readAr>>strInitDir>>iVersionFor>>strStation>>strProjName>>strDrawNum;
	readAr.Close() ;	
	file.Close() ;
	
	m_strSavePath = strInitDir ;
	m_iVersionFor = iVersionFor ;
	m_strStationName = strStation ;
	m_strProjName = strProjName ;
	m_strDrawNum = strDrawNum ;
	
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMaXuBiaoDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData() ;
	CFile file ;
	CString strSetFile ;
	CString strBasePath ;
	CBlkUtility::GetBasePath(strBasePath) ;
	strSetFile.Format(_T("%s\\myset_2002.txt"), strBasePath) ;
	file.Open(strSetFile, CFile::modeWrite) ;	
	CArchive saveAr2(&file, CArchive::store) ;
	saveAr2<<m_strSavePath<<m_iVersionFor<<m_strStationName<<m_strProjName<<m_strDrawNum ;
	saveAr2.Close() ;	
	file.Close() ;
	
	CDialog::OnOK();
}

void CMaXuBiaoDlg::OnBUTTONSelectDir() 
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
