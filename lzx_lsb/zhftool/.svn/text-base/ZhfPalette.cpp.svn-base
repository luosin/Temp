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
//----- BoxPalette.cpp : Implementation of CZhfPalette
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//#include "resource.h"
#include "ZhfPalette.h"
#include "..\blkdll\BlkUtitily.h"

extern CAcExtensionModule zhftoolDLL ;
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CZhfPalette, CAdUiPalette)

BEGIN_MESSAGE_MAP(CZhfPalette, CAdUiPalette)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(ZHFMSG_PREVIEWCTRL_LBDOWN, OnCtrlIsSelected)
//	ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CZhfPalette::CZhfPalette () 
: m_iOldPosY(0)
, m_iCtrlWidth(240)
, m_iCtrlHeight(160)
, m_iCurSel(-1)
// ,m_strFilePath(_T(""))
, m_iFilterMode(0)
, m_iMarginX(10)
, m_iMarginY(30)
// , m_bIsActive(FALSE)
, m_bFirstActive(true)
{

}
// CZhfPalette::CZhfPalette (const TCHAR* szFilePath)
// : m_iOldPosY(0)
// , m_iCtrlWidth(240)
// , m_iCtrlHeight(160)
// , m_iCurSel(-1)
// , m_strFilePath(szFilePath)
// , m_iFilterMode(0)
// {
// 	 //CZhfPalette() ; 构造函数不能被构造函数调用
// }

//-----------------------------------------------------------------------------
CZhfPalette::~CZhfPalette () 
{
	//acutPrintf(_T("\n~CZhfPalette")) ;
	DeleteControls() ;
}

LRESULT CZhfPalette::OnCtrlIsSelected(WPARAM wparam,LPARAM lparam)
{
	int iIndexSelected = (int)wparam ;
	CGsPreviewCtrl* pCtrl =m_pArrayPreviewCtrl.GetAt(iIndexSelected) ; //当前点击的gsctrl
	if (g_pCtrlGsPreview!=NULL)
	{
		CZhfPalette* pPalette = (CZhfPalette*)g_pCtrlGsPreview->GetParent() ; //原选中gsctrl的面板
		if (pCtrl!=g_pCtrlGsPreview)
		{
			g_pCtrlGsPreview->SetSelect(false) ;
			pPalette->m_iCurSel = -1 ; //先置-1，若pPalette就是本面板，下面会重新赋值

			m_iCurSel = iIndexSelected ;
			g_pCtrlGsPreview = pCtrl ;
		}
		else
		{
			g_pCtrlGsPreview->SetSelect(false) ;
			pPalette->m_iCurSel = -1 ;

			g_pCtrlGsPreview = NULL ;
		}
	}
	else
	{
		m_iCurSel = iIndexSelected ;
		g_pCtrlGsPreview = pCtrl ;
	}

// 	if (m_iCurSel!=iIndexSelected)
// 	{
// 		if (m_iCurSel!=-1)
// 		{
// 			m_pArrayPreviewCtrl.GetAt(m_iCurSel)->SetSelect(false) ;
// 		}
// 		m_iCurSel = iIndexSelected ;
// 	}
// 	else
// 	{
// 		m_iCurSel = -1 ;
// 		g_pCtrlGsPreview = NULL ;
// 	}

	return 0 ;
}

void CZhfPalette::AddControls()
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


	int iIndex = 0 ;
	for (int i=0; i<m_strArrayFile.GetCount(); i++)
	{
		CString strFile = m_strArrayFile.GetAt(i) ;

		CGsPreviewCtrl* pCtrl = new CGsPreviewCtrl() ;
		pCtrl->m_iIndex = iIndex ;
		pCtrl->m_strDwgFile = strFile ;
		m_pArrayPreviewCtrl.Add(pCtrl) ;
		pCtrl->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_CENTER|SS_SUNKEN, CRect(10, 10, 110, 110), this) ;
		//pCtrl->SetDwgFile(strFile) ;
		//pCtrl->Init(zhftoolDLL.ModuleResourceInstance(), true);
// 		if (m_iFilterMode==1)
// 		{
// 			pCtrl->SetDwgFile(m_strArrayFile.GetAt(i)) ;
// 		}
// 		else
		
// 		{			
			AcDbDatabase* pDbSrc = new AcDbDatabase(false) ;
			if(Acad::eOk==pDbSrc->readDwgFile(m_strArrayFile.GetAt(i)))
			{
				AcDbDatabase* pDbTo = new AcDbDatabase() ;
				this->Wblock(pDbSrc, pDbTo) ;
				this->FilterDb(pDbTo, m_iFilterMode) ;

				pCtrl->SetDatabase(pDbTo);
			}
			delete pDbSrc ;
// 		}
		

		CStatic* pCtrlStatic = new CStatic() ;
		CString strFileName ;
		strFileName.Format(_T("(%d)\n%s"), i+1, m_strArrayFileName.GetAt(i)) ;		
		//CBlkUtility::GetFileName(strFile, strFileName) ;
		m_pArrayStatic.Add(pCtrlStatic) ;
		pCtrlStatic->Create(strFileName, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(10, 10, 40, 110), this) ;
		pCtrlStatic->SetFont(pFont) ;
		iIndex++ ;
	}
	acDocManager->unlockDocument(curDoc()) ;
	this->OnSize(0, iWidth, iHeight) ;
	//this->ResizeCtrl(iWidth, iHeight) ;
}
Acad::ErrorStatus CZhfPalette::Wblock(AcDbDatabase*& pDbFrom, AcDbDatabase*& pDbTo, /*const AcGePoint3d& pt_3d_base,*/ const AcGeMatrix3d & xform)
{
	Acad::ErrorStatus es ;

	AcDbObjectIdArray objIdsOut;

	AcDbBlockTable* pBT = NULL ;
	pDbFrom->getBlockTable(pBT, AcDb::kForRead);
	AcDbBlockTableRecord* pBTR = NULL;
	es = pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForRead);
	pBT->close();

	AcDbBlockTableRecordIterator* pIT;
	es = pBTR->newIterator(pIT) ;
	for (; !pIT->done(); pIT->step()) 
	{ 
		AcDbEntity* pEnt = NULL ;
		if (Acad::eOk==pIT->getEntity(pEnt, AcDb::kForRead))
		{
// 			if (!pEnt->isKindOf(AcDbAttributeDefinition::desc()))
// 			{
				AcDbObjectId objId;
				if (Acad::eOk == pIT->getEntityId(objId))
				{
					objIdsOut.append(objId);
				}
// 			}
			pEnt->close() ;
		}
	}

	delete pIT;
	pBTR->close();

	if (!objIdsOut.isEmpty()) 
	{
		for (int i=0; i<objIdsOut.length(); i++)
		{
			AcDbObjectId objId = objIdsOut.at(i);
			AcDbEntity *pEnt = NULL ;
			if(Acad::eOk==acdbOpenAcDbEntity(pEnt,objId, AcDb::kForRead))
			{
				AcDbObjectId objIdDic ;
				if ((objIdDic = pEnt->extensionDictionary())!= AcDbObjectId::kNull)
				{
					AcDbDictionary *pDict = NULL;				
					if (Acad::eOk==acdbOpenObject(pDict, objIdDic, AcDb::kForWrite))
					{
						pDict->setTreatElementsAsHard(Adesk::kTrue);
						pDict->close();
					}
				}
				pEnt->close() ;
			}
		}

		AcDbDatabase *pTempDb ;
		es = pDbFrom->wblock( pTempDb, objIdsOut, AcGePoint3d::kOrigin ) ;
		if (Acad::eOk != es )
		{
			acedAlert( _T("wblock failed!") );
			return es ;
		}
		es = pDbTo->insert( xform, pTempDb ) ;
		if (Acad::eOk != es)
			acedAlert( _T("insert failed!" ));
		delete pTempDb;
	}
	else
	{
		acutPrintf(_T("\nNo entities found in model space of DWG"));
	}

	return es ;
}
void CZhfPalette::DeleteControls(void)
{
	//delete m_pEdit1 ;
	int iCount = m_pArrayPreviewCtrl.GetCount() ;
	for (int i=iCount-1; i>=0; i--)
	{
		CGsPreviewCtrl*& pCtrl = m_pArrayPreviewCtrl.GetAt(i) ;
		m_pArrayPreviewCtrl.RemoveAt(i) ;
		delete pCtrl ;

		CStatic*& pCtrlStatic = m_pArrayStatic.GetAt(i) ;
		m_pArrayStatic.RemoveAt(i) ;
		delete pCtrlStatic ;
	}
}

void CZhfPalette::OnSize(UINT nType, int cx, int cy)
{
	CAdUiPalette::OnSize(nType, cx, cy);
	// TODO: 在此添加消息处理程序代码
	if (cx<m_iCtrlWidth+m_iMarginX||cy<m_iCtrlHeight+m_iMarginY)
	{
		return ;
	}
	int iCtrlNum = m_pArrayPreviewCtrl.GetCount() ;
	int iCol = cx/(m_iCtrlWidth+m_iMarginX) ;
	int iRow = iCtrlNum/iCol + (iCtrlNum%iCol==0?0:1);

	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;
	si.nMax = iRow-1 ;
	si.nPage = cy/(m_iCtrlHeight+m_iMarginY) ;
	SetScrollInfo(SB_VERT,&si,TRUE);
	ScrollWindow(0,0);

	m_iOldPosY = 0;

	Invalidate(TRUE);

	if (m_pArrayPreviewCtrl.GetCount()>0&&cx>(m_iCtrlWidth+m_iMarginX))
	{
		TRACE(_T("\ncx:%d, cy:%d"),cx, cy) ;
		ResizeCtrl(cx, cy) ;
	}
}

void CZhfPalette::ResizeCtrl(int iX, int iY)
{
	//int iCtrlNum = m_pArrayPreviewCtrl.GetCount() ;
	int iCol = iX/(m_iCtrlWidth+m_iMarginX) ;
	//int iRow = iCtrlNum/iCol + (iCtrlNum%iCol==0?0:1);
	for (int i=0; i<m_pArrayPreviewCtrl.GetCount(); i++)
	{
		int iColCur = i%iCol ;
		int iRowCur = (i+1)/iCol+((i+1)%iCol==0?0:1);
		int iXTmp = iColCur*(m_iCtrlWidth+m_iMarginX)+m_iMarginX ;
		int iYTmp = (iRowCur-1)*(m_iCtrlHeight+m_iMarginY) ;
		CGsPreviewCtrl* pCtrl = m_pArrayPreviewCtrl.GetAt(i) ;
		pCtrl->MoveWindow(iXTmp, iYTmp, m_iCtrlWidth, m_iCtrlHeight) ;
		pCtrl->Invalidate(TRUE) ;

		CStatic* pCtrlStatic = m_pArrayStatic.GetAt(i) ;
		pCtrlStatic->MoveWindow(iXTmp, iYTmp+m_iCtrlHeight, m_iCtrlWidth, m_iMarginY) ;
		pCtrlStatic->Invalidate(TRUE) ;
	}
}

BOOL CZhfPalette::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	//cs.style |=WS_VSCROLL ;
	return CAdUiPalette::PreCreateWindow(cs);
}

void CZhfPalette::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int minpos = 0;
	int maxpos = 0;
	GetScrollRange(SB_VERT, &minpos, &maxpos); 
	maxpos = GetScrollLimit(SB_VERT);

	// Get the current position of scroll box.
	int curpos = GetScrollPos(SB_VERT);

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_TOP:      // Scroll to far left.
		curpos = minpos;
		break;

	case SB_BOTTOM:      // Scroll to far right.
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;

	case SB_LINEUP:      // Scroll left.
		if (curpos > minpos)
			curpos--;
		break;

	case SB_LINEDOWN:   // Scroll right.
		if (curpos < maxpos)
			curpos++;
		break;

	case SB_PAGEUP:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_VERT, &info, SIF_ALL);

			if (curpos > minpos)
				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;

	case SB_PAGEDOWN:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_VERT, &info, SIF_ALL);

			if (curpos < maxpos)
				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}

	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_VERT, curpos);
	ScrollWindow(0,(m_iOldPosY-curpos)*(m_iCtrlHeight+m_iMarginY));

	m_iOldPosY = curpos;
	UpdateWindow();

	CAdUiPalette::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CZhfPalette::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int curpos = GetScrollPos(SB_VERT);
	int minpos = 0;
	int maxpos = 0;
	GetScrollRange(SB_VERT, &minpos, &maxpos); 
	maxpos = GetScrollLimit(SB_VERT);

	if (zDelta<0)
	{
		if (curpos < maxpos)
			curpos++;
	}
	else 
	{
		if (curpos > minpos)
			curpos-- ;
	}
	SetScrollPos(SB_VERT, curpos);
	ScrollWindow(0, (m_iOldPosY-curpos)*(m_iCtrlHeight+m_iMarginY));

	m_iOldPosY = curpos;

	return CAdUiPalette::OnMouseWheel(nFlags, zDelta, pt);
}

void CZhfPalette::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CAdUiPalette::OnMouseMove(nFlags, point);
}

//0，不显示属性，1，显示属性，2，显示属性默认值
void CZhfPalette::FilterDb(AcDbDatabase* pDb, int iFilterMode)
{
	if (iFilterMode==1)
	{
		return ;
	}
	Acad::ErrorStatus es ;
	AcDbBlockTable* pBT = NULL ;
	pDb->getBlockTable(pBT, AcDb::kForRead);
	AcDbBlockTableRecord* pBTR = NULL;
	es = pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForWrite);
	pBT->close();

	AcDbBlockTableRecordIterator* pIT;
	es = pBTR->newIterator(pIT) ;
	for (; !pIT->done(); pIT->step()) 
	{
		AcDbEntity* pEnt = NULL ;
		if (Acad::eOk==pIT->getEntity(pEnt, AcDb::kForWrite))
		{
			if (pEnt->isKindOf(AcDbAttributeDefinition::desc()))
			{
				AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);

				if (iFilterMode==0)
				{
					pEnt->erase() ;
				}
				else if (iFilterMode>1)
				{
					if (pAttDef != NULL && !pAttDef->isConstant())
					{
						// We have a non-constant attribute definition,
						// so build an attribute entity.
						CString strShowVal ;

						if (iFilterMode==2)
						{
							strShowVal = pAttDef->textString() ;
						}
						else if (iFilterMode==3)
						{
							strShowVal = pAttDef->prompt() ; //显示中文为乱码
						}
						pAttDef->setTag(strShowVal) ;

// 						AcDbAttribute * pAtt = new AcDbAttribute ;
// 						pAtt->setTag( pAttDef->tag() ) ;
// 
// 						AcDb::TextHorzMode attHMode = pAttDef->horizontalMode() ;
// 						AcDb::TextVertMode attVMode = pAttDef->verticalMode() ;
// 
// 						pAtt->setHorizontalMode( attHMode );
// 						pAtt->setVerticalMode ( attVMode ) ;
// 
// 						AcGePoint3d pt_3d_position( 0,0,0 ), pt_3d_align(0,0,0), Att_Def_3d ( 0,0,0 ) ;
// 						Att_Def_3d = pAttDef->position() ;
// 						AcGePoint3d pt_3d_insert = m_pBlkRef->position() ;
// 						pt_3d_position[X] = pt_3d_insert[X] + Att_Def_3d[X] ;
// 						pt_3d_position[Y] = pt_3d_insert[Y] + Att_Def_3d[Y] ;
// 
// 						Att_Def_3d = pAttDef->alignmentPoint() ;
// 						pt_3d_align[X] = pt_3d_insert[X] + Att_Def_3d[X] ;
// 						pt_3d_align[Y] = pt_3d_insert[Y] + Att_Def_3d[Y] ;
// 
// 						//////////////////////////////////////////////////////////////////////////
// 						if (attHMode==AcDb::kTextLeft&&attVMode==AcDb::kTextBase)
// 						{
// 							pAtt->setPosition( pt_3d_position ) ;
// 						}
// 						else if (attHMode==AcDb::kTextAlign||attHMode==AcDb::kTextFit)
// 						{
// 							pAtt->setPosition(pt_3d_position) ;
// 							pAtt->setAlignmentPoint(pt_3d_align) ;
// 						}
// 						else
// 						{
// 							pAtt->setAlignmentPoint(pt_3d_align) ;
// 						}
// 						//////////////////////////////////////////////////////////////////////////
// 
// 						pAtt->setRotation ( pAttDef->rotation() ) ;			
// 						pAtt->setHeight( pAttDef->height() ) ;
// 						pAtt->setWidthFactor ( pAttDef->widthFactor() );		
// 						pAtt->setTextString( pAttDef->textString() ) ;
// 						pAtt->setInvisible ( pAttDef->isInvisible() ) ;
// 						pAtt->setColorIndex(pAttDef->colorIndex()) ;
// 						pAtt->setTextStyle(pAttDef->textStyle()) ;
// 						m_pBlkRef->appendAttribute(pAtt);
// 						pAtt->close() ;
					}
				}
			}
			pEnt->close() ;
		}
	}

	delete pIT;
	pBTR->close();
}

void CZhfPalette::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	CMDIFrameWnd* pAcadFrame = acedGetAcadFrame();
// 	CWnd* pWnd = GetForegroundWindow() ;
// 	CString strText ;
// 	pWnd->GetWindowText(strText) ;
// 	AfxMessageBox(strText) ;

	CAdUiPalette::OnLButtonDown(nFlags, point);
}

// -----------------------------------------------------------------------------
void CZhfPalette::OnSetActive(void)
{
	CAdUiPalette::OnSetActive () ;
	if (m_bFirstActive)
	{
		this->AddControls() ;
		m_bFirstActive = false ;
	}	
}
