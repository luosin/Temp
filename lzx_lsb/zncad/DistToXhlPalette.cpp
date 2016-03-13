// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- DistToXhlPalette.cpp : Implementation of CDistToXhlPalette
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ZnSxt.h"
#include "DistToXhlPalette.h"
#include "..\blkuserdll\BlkExcelTool.h"

extern CAcExtensionModule zncadDLL ;
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDistToXhlPalette, CAdUiPalette)

BEGIN_MESSAGE_MAP(CDistToXhlPalette, CAdUiPalette)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDS_BUTTON_DWGTOGRID, OnButtonClickDwgToGrid)
	ON_BN_CLICKED(IDS_BUTTON_APPLYTODWG, OnButtonClickApplyToDwg)
	ON_BN_CLICKED(IDS_BUTTON_TOXLS, OnButtonClickToXls)
	ON_BN_CLICKED(IDS_BUTTON_FROMXLS, OnButtonClickFromXls)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDistToXhlPalette::CDistToXhlPalette () 
: m_iCurSel(-1)
, m_bFirstActive(true)
{
// 	for (int i=0; i<3; i++)
// 	{
// 		m_pButton[i] = NULL ;
// 	}
}


//-----------------------------------------------------------------------------
CDistToXhlPalette::~CDistToXhlPalette () 
{
	//acutPrintf(_T("\n~CDistToXhlPalette")) ;
	DeleteControls() ;
}

void CDistToXhlPalette::AddControls()
{
	acDocManager->lockDocument(curDoc(), AcAp::kWrite, NULL, NULL, true) ;
	CRect rect ;
	GetClientRect(&rect) ;
	int iWidth = rect.Width() ;
	int iHeight = rect.Height() ;

	CFont * pFont = new CFont;
	pFont->CreateFont(14, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_BOLD, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("Arial")); // lpszFac

	CRect rectClient ;
	GetClientRect(rectClient) ;	
	rectClient.bottom-=50 ;
	m_grid.CreateGrid( WS_CHILD|WS_VISIBLE, rectClient, this, IDC_LIST_GRID );
	
	for (int i=0; i<4; i++)
	{
		CButton * pButton = this->NewButton(IDS_BUTTON_DWGTOGRID+i,CRect(i*76+6, rectClient.bottom-40,70,30),i==0?BS_DEFPUSHBUTTON:0);
		pButton->SetFont(pFont) ;
		m_vecButton.push_back(pButton) ;
	}

	acDocManager->unlockDocument(curDoc()) ;
	this->OnSize(0, iWidth, iHeight) ;
}

void CDistToXhlPalette::DeleteControls(void)
{
	for (int i=0; i<m_vecButton.size(); i++)
	{
		if (m_vecButton[i])
		{
			delete m_vecButton[i] ;
			m_vecButton[i] = NULL ;
		}
	}
}

void CDistToXhlPalette::OnSize(UINT nType, int cx, int cy)
{
	CAdUiPalette::OnSize(nType, cx, cy);
	// TODO: 在此添加消息处理程序代码
	if (m_grid.m_hWnd!=NULL )
	{
		m_grid.MoveWindow(0, 0, cx, cy-50) ;
		m_grid.Invalidate(TRUE) ;
	}

	for (int i=0; i<m_vecButton.size(); i++)
	{
		if (m_vecButton[i]!=NULL)
		{
			m_vecButton[i]->MoveWindow(cx/2+i*76-149, cy-40, 70, 30) ;
			//m_vecButton[i]->Invalidate(TRUE) ;
		}
	}
// 	m_buttonToDwg.MoveWindow(cx/2-110, cy-40, 100, 30) ;
// 	m_buttonImport.MoveWindow(cx/2+10, cy-40, 100, 30) ;
}

// -----------------------------------------------------------------------------
void CDistToXhlPalette::OnSetActive(void)
{
	CAdUiPalette::OnSetActive () ;
	if (m_bFirstActive)
	{
		this->AddControls() ;
		m_bFirstActive = false ;
	}	
}
BOOL CDistToXhlPalette::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style &= ~WS_HSCROLL ;
	cs.style &= ~WS_VSCROLL ;	

	return CAdUiPalette::PreCreateWindow(cs);
}

CButton* CDistToXhlPalette::NewButton(int nID, CRect rect, int nStyle)
{
	CString strCaption;
	strCaption.LoadString( nID ); //取按钮标题
	CButton *p_Button = new CButton();
	ASSERT_VALID(p_Button);
	p_Button->Create( strCaption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | nStyle, rect, this, nID ); //创建按钮
	return p_Button;
}

void CDistToXhlPalette::OnButtonClickDwgToGrid()
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	AcApDocManager *pDocMgr = acDocManager ;
	pDocMgr->lockDocument(curDoc()) ;
	CZnSxt sxt ;
	sxt.SetAcDbDB(pDb) ;
	AcGePoint3d pt_3d_toxhlheader1, pt_3d_toxhlheader2 ;
	int iRet = sxt.GetToXhlHeaderPos(pt_3d_toxhlheader1, pt_3d_toxhlheader2) ;
	if(iRet>0)
	{
		vector<vector<vector<CString> > > vecDist ;
		if (iRet==1)
		{
			sxt.GetDistToXhl(pt_3d_toxhlheader1, vecDist) ;
		}
		else if (iRet==13)
		{
			sxt.GetDistToXhl_DT(pt_3d_toxhlheader1, pt_3d_toxhlheader2, vecDist) ;
		}

		m_grid.InData(vecDist) ;	
	}
	pDocMgr->unlockDocument(curDoc()) ;
}
void CDistToXhlPalette::OnButtonClickApplyToDwg()
{
	vector<vector<vector<CString> > > vecToXhl ;
	m_grid.OutData(vecToXhl) ;
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	AcApDocManager *pDocMgr = acDocManager ;
	pDocMgr->lockDocument(curDoc()) ;
	CZnSxt sxt ;
	sxt.SetAcDbDB(pDb) ;	
	sxt.ApplyDistToXhlToDwg(vecToXhl) ;
	pDocMgr->unlockDocument(curDoc()) ;
}

void CDistToXhlPalette::OnButtonClickToXls()
{
	CString strFilter = _T("(文件(*.xls)|*.xls|");
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, strFilter);
	fileDlg.m_ofn.lpstrTitle = _T("导出距离信号楼距离");
	TCHAR szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));
	GetCurrentDirectory(sizeof(szBuf), szBuf);
	fileDlg.m_ofn.lpstrInitialDir = szBuf;
	if(IDOK==fileDlg.DoModal())
	{
		CString strFilePath = fileDlg.GetPathName();
		CString strFilePathTmp = strFilePath ;
		strFilePathTmp.MakeLower() ;
		if (strFilePathTmp.Right(4)!=_T(".xls"))
		{
			strFilePath.Append(_T(".xls")) ;
		}
		vector<vector<vector<CString> > > vecToXhl ;
		m_grid.OutData(vecToXhl) ;
		CBlkExcelTool::WriteDistToXhlToXls(strFilePath, vecToXhl) ;
	}	
}

void CDistToXhlPalette::OnButtonClickFromXls()
{
	CString strFilter = _T("(文件(*.xls)|*.xls|");
	CFileDialog fileDlg(true, NULL, NULL, NULL, strFilter);
	fileDlg.m_ofn.lpstrTitle = _T("导入距离信号楼距离");
	TCHAR szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));
	GetCurrentDirectory(sizeof(szBuf), szBuf);
	fileDlg.m_ofn.lpstrInitialDir = szBuf;
	if(IDOK==fileDlg.DoModal())
	{
		CString strFilePath = fileDlg.GetPathName();
		vector<vector<vector<CString> > > vecToXhl ;
		CBlkExcelTool::ReadDistToXhlFromXls(strFilePath, vecToXhl) ;
		m_grid.InData(vecToXhl) ;		
	}

}