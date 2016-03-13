// EntPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EntPropertyDlg.h"
#include "XhjDATA.h"
#include "DcDATA.h"
#include "GdDATA.h"


// CEntPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CEntPropertyDlg, CDialog)
CEntPropertyDlg::CEntPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEntPropertyDlg::IDD, pParent)
{
}

CEntPropertyDlg::~CEntPropertyDlg()
{
}

void CEntPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEntPropertyDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEntPropertyDlg 消息处理程序

BOOL CEntPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int nRow = 1 ;
	int nRowHeight = 20 ;
	CString strDlgTitle = _T("数据编辑") ;

// 	const char *szClassName = typeid(*(m_grid.m_pEntDATA)).name() ;
// 	CString strClassName ;
// 	CBlkUtility::ConvMultiToWide(szClassName, strClassName) ;
	CString strClassName  = m_grid.m_pEntDATA->GetClassType() ;
	if (strClassName==_T("CXhjDATA"))
	{
		nRow = 12 ;
		strDlgTitle.Append(_T("-信号机")) ;
	}
	else if (strClassName==_T("CGdDATA"))
	{
		nRow = 13 ;
		strDlgTitle.Append(_T("-轨道区段")) ;
	}
	else if (strClassName==_T("CDcDATA"))
	{
		nRow = 19 ;
		strDlgTitle.Append(_T("-道岔")) ;
	}

	this->SetWindowText(strDlgTitle) ;

	CRect rectClient ;
	GetClientRect(rectClient) ;		
	//rectClient.DeflateRect(5,0,5,0) ;
	rectClient.bottom = nRowHeight*nRow+5 ;
	m_grid.CreateGrid( WS_CHILD|WS_VISIBLE, rectClient, this, IDC_LIST_GRID );

	//acutPrintf(_T("\nrectclient %d-%d-%d-%d"), rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height()) ;

	CButton* pButton = (CButton*)GetDlgItem(IDOK) ;
	pButton->MoveWindow(rectClient.Width()/2-80, rectClient.Height()+10, 80, 20) ;
	pButton = (CButton*)GetDlgItem(IDCANCEL) ;
	pButton->MoveWindow(rectClient.Width()/2, rectClient.Height()+10, 80, 20) ;
	CRect rectWnd ;
	GetWindowRect(rectWnd) ;
	//acutPrintf(_T("\nbefore rectwnd %d-%d-%d-%d-%d-%d"), rectWnd.left, rectWnd.top, rectWnd.right, rectWnd.bottom, rectWnd.Width(), rectWnd.Height()) ;
	rectWnd.bottom = rectWnd.top+rectClient.Height()+80 ;
	//acutPrintf(_T("\nafter rectwnd %d-%d-%d-%d-%d-%d"), rectWnd.left, rectWnd.top, rectWnd.right, rectWnd.bottom, rectWnd.Width(), rectWnd.Height()) ;
	this->SetWindowPos(NULL, rectWnd.left, rectWnd.top, rectWnd.Width(), rectWnd.Height(), SWP_NOMOVE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CEntPropertyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此添加消息处理程序代码
}

void CEntPropertyDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::OnCancel();
}

BOOL CEntPropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if (pMsg->message == WM_KEYDOWN)  
// 	{  
// 		switch(pMsg->wParam)  
// 		{  
// 		case VK_ESCAPE: 
// 			{
// 			}
// 		case VK_RETURN: 
// 			{
// 
// 			}
// 		default:  
// 			;  
// 		}  
// 	}  

	return CDialog::PreTranslateMessage(pMsg);
}

void CEntPropertyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCellText ;

	CString strClassName  = m_grid.m_pEntDATA->GetClassType() ;
	if (strClassName==_T("CXhjDATA"))
	{
		CXhjDATA *pDATA = dynamic_cast<CXhjDATA*>(m_grid.m_pEntDATA) ;

		pDATA->m_strName  = m_grid.QuickGetText(0, 0) ;
		m_grid.QuickGetNumber(0, 1, pDATA->m_nLoc) ;			
		pDATA->m_bIsTrain = m_grid.QuickGetBool(0, 2) ;
		pDATA->m_strBoxType = m_grid.QuickGetText(0, 3) ;
		m_grid.QuickGetNumber(0, 4, pDATA->m_nCoreNumI) ;
		m_grid.QuickGetNumber(0, 5, pDATA->m_nCoreNumII) ;	
		pDATA->m_strBoxPos = m_grid.QuickGetText(0, 6) ;							
		pDATA->m_bHasDDUnit = m_grid.QuickGetBool(0, 7) ;
		pDATA->m_bIsAlu = m_grid.QuickGetBool(0, 8) ;
		pDATA->m_bHasPhone = m_grid.QuickGetBool(0, 9) ;
		m_grid.QuickGetNumber(0, 10, pDATA->m_nDSNum) ;
		m_grid.QuickGetNumber(0, 11, pDATA->m_nSpeedLimit) ;
	}
	else if (strClassName==_T("CGdDATA"))
	{
		CGdDATA *pDATA = dynamic_cast<CGdDATA*>(m_grid.m_pEntDATA) ;

		pDATA->m_strName = m_grid.QuickGetText(0, 0) ;
		pDATA->m_strCircuitType = m_grid.QuickGetText(0, 1) ;							
		pDATA->m_str25HzDev = m_grid.QuickGetText(0, 2) ;							
		pDATA->m_bIsDH = m_grid.QuickGetBool(0, 3) ;
		pDATA->m_bIsDMH = m_grid.QuickGetBool(0, 4) ;	
		pDATA->m_bIs4Line = m_grid.QuickGetBool(0, 5) ;	
		pDATA->m_bIsShareXB = m_grid.QuickGetBool(0, 6) ;							
		pDATA->m_bIs1FnJ = m_grid.QuickGetBool(0, 7) ;
		pDATA->m_bIsMainSec = m_grid.QuickGetBool(0, 8) ;
		strCellText = m_grid.QuickGetText(0, 9) ;
		pDATA->m_strLeftFOrS = _T("N") ;
		if (strCellText==_T("送电"))
		{
			pDATA->m_strLeftFOrS = _T("F") ;
		}
		else if (strCellText==_T("受电"))
		{
			pDATA->m_strLeftFOrS = _T("S") ;
		}
		strCellText = _T("") ;
		strCellText = m_grid.QuickGetText(0, 10) ;
		pDATA->m_strRightFOrS = _T("N") ;
		if (strCellText==_T("送电"))
		{
			pDATA->m_strRightFOrS = _T("F") ;
		}
		else if (strCellText==_T("受电"))
		{
			pDATA->m_strRightFOrS = _T("S") ;
		}
		pDATA->m_strFreqLeft = m_grid.QuickGetText(0, 11) ;
		pDATA->m_strFreqRight = m_grid.QuickGetText(0, 12) ;
	}
	else if (strClassName==_T("CDcDATA"))
	{
		CDcDATA *pDATA = dynamic_cast<CDcDATA*>(m_grid.m_pEntDATA) ;

		pDATA->m_strName = m_grid.QuickGetText(0, 0) ;
		m_grid.QuickGetNumber(0, 1, pDATA->m_nLoc) ;				
		m_grid.QuickGetNumber(0, 2, pDATA->m_nJG_QyNum) ;
		pDATA->m_strJG_QyType = m_grid.QuickGetText(0, 3) ;
		pDATA->m_bJG_HasMT = m_grid.QuickGetBool(0, 4) ;
		pDATA->m_strJG_QyPos = m_grid.QuickGetText(0, 5) ;
		m_grid.QuickGetNumber(0, 6, pDATA->m_nXG_QyNum) ;
		pDATA->m_strXG_QyType = m_grid.QuickGetText(0, 7) ;
		pDATA->m_bXG_HasMT = m_grid.QuickGetBool(0, 8) ;
		pDATA->m_strXG_QyPos = m_grid.QuickGetText(0, 9) ;
		pDATA->m_strBoxType = m_grid.QuickGetText(0, 10) ;
		pDATA->m_bIsSingleAct = m_grid.QuickGetBool(0, 11) ;
		pDATA->m_bIsComplex = m_grid.QuickGetBool(0, 12) ;
		pDATA->m_strRelationDc = m_grid.QuickGetText(0, 13) ;
		m_grid.QuickGetNumber(0, 14, pDATA->m_nActIndex) ;
		pDATA->m_bHasPhone = m_grid.QuickGetBool(0, 15) ;
		pDATA->m_bHasDCQKBJ = m_grid.QuickGetBool(0, 16) ;
		m_grid.QuickGetNumber(0, 17, pDATA->m_nSpeedLimit) ;
		CString strText = m_grid.QuickGetText(0, 18) ;
		pDATA->m_iCutJYJPos =(strText==_T("弯股切割")?0:1) ;		
	}

	OnOK();
}
