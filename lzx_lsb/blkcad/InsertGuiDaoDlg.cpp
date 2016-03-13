// InsertGuiDaoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InsertGuiDaoDlg.h"
#include "dwgthumbnail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertGuiDaoDlg dialog


CInsertGuiDaoDlg::CInsertGuiDaoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertGuiDaoDlg::IDD, pParent)
	,m_iSelectID(-1)
{
	//{{AFX_DATA_INIT(CInsertGuiDaoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInsertGuiDaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertGuiDaoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertGuiDaoDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertGuiDaoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertGuiDaoDlg message handlers

BOOL CInsertGuiDaoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strBlockName[] = {
			"samp_GUIDAO_TG_X_WFG.dwg",
			"samp_GUIDAO_TG_X_FG.dwg",
			"samp_GUIDAO_JZ_X_Z.dwg",
			"samp_GUIDAO_JZ_X_F.dwg",
			"samp_GUIDAO_LQ_X_0.dwg",
			"samp_GUIDAO_TG_S_WFG.dwg",
			"samp_GUIDAO_TG_S_FG.dwg",
			"samp_GUIDAO_JZ_S_Z.dwg",
			"samp_GUIDAO_JZ_S_F.dwg",
			"samp_GUIDAO_LQ_S_0.dwg",
			"samp_GUIDAO_GD_X_ALL.dwg",
			"samp_GUIDAO_GD_X_LEFT.dwg",
			"samp_GUIDAO_GD_X_RIGHT.dwg",
			"samp_GUIDAO_GD_S_ALL.dwg",
			"samp_GUIDAO_GD_S_LEFT.dwg",
			"samp_GUIDAO_GD_S_RIGHT.dwg",
			"samp_GUIDAO_GD_C_ALL.dwg",
			"samp_GUIDAO_GD_C_LEFT.dwg",
			"samp_GUIDAO_GD_C_RIGHT.dwg",
			"samp_GUIDAO_ZN_0_0.dwg"
	} ;

	for (int i=0; i<20; i++)
	{
		CString strFileName ;
		CString strBasePath ;
		g_cSystem.GetBasePath(strBasePath) ;
		strFileName.Format(_T("%s\\support\\maxubiao\\%s"), strBasePath, strBlockName[i]) ;
		CDwgThumbnail * pDwg = (CDwgThumbnail*)this->GetDlgItem(IDC_DWGTHUMBNAILCTRL1+i) ;
		pDwg->SetDwgFileName(strFileName) ;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CInsertGuiDaoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CInsertGuiDaoDlg)
	ON_EVENT_RANGE(CInsertGuiDaoDlg, IDC_DWGTHUMBNAILCTRL1,IDC_DWGTHUMBNAILCTRL20, -600 /* Click */, OnClickDwgthumbnailctrl1, VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CInsertGuiDaoDlg::OnClickDwgthumbnailctrl1(UINT nID) 
{
	// TODO: Add your control notification handler code here
	m_iSelectID = nID - IDC_DWGTHUMBNAILCTRL1 ;
// 	CString strMsg ;
// 	strMsg.Format("%d", nID) ;
// 	AfxMessageBox(strMsg) ;
	
}
