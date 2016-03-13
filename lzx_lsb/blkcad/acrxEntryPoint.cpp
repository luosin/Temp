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
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "InsertGuiDaoDlg.h"
#include "CableNetForKZ.h"
#include "DLPXDlg.h"
#include "StationSystem.h"
#include "StationSetDlg.h"
#include "LKDataTable.h"
#include "LkDataSetDlg.h"
#include "MaXuBiaoDlg.h"
#include "MaXuBiaoForHD.h"
#include "LocToNameDlg.h"
#include "CableNetForBT.h"
#include "CableNetForJY.h"
#include "MaXuBiaoForBT.h"
#include "ZnDmh.h"
#include "UserUtility.h"
#include "InputFxpDataDlg.h"
#include "MaXuBiaoForJH.h"
#include "BtMxbDlg.h"
#include "ReadBztDlg.h"
#include "ReadLKDataDlg.h"
#include "readlkforpmtdlg.h"
#include "BtTools.h"
#include "DmhSetDlg.h"
#include "DmhOptionsDlg.h"
#include "DlpxSetDlg.h"
#include "CzhfEntReactor.h"
#include "CTestReactor.h"
#include "TestDlg.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("zhf")

// Gets a rectangular window in viewport's x-y plane
// that represents drawing's extents
static void getUcsExts(AcGePoint2d &maxExt,	AcGePoint2d &minExt,	AcGeMatrix3d UcsMat )
{
	AcDbDatabase* pDb	= acdbHostApplicationServices()->workingDatabase();
	AcGePoint3d max = pDb->extmax(), min = pDb->extmin();

	// Make extents box
	AcDbExtents ext(min, max);

	// Transform extents to UCS
	ext.transformBy(UcsMat);
	max = ext.maxPoint();
	min = ext.minPoint();
	maxExt[X] = max[X];
	maxExt[Y] = max[Y];
	minExt[X] = min[X];
	minExt[Y] = min[Y];
}

static void SetViewportExtents(AcDbViewport *pVp,	AcGePoint2d &max,	AcGePoint2d &min, 	double height)
{
	// Set View center
	AcGePoint2d ViewCenter;
	ViewCenter[X] = (max[X]+min[X])/2.0;
	ViewCenter[Y] = (max[Y]+min[Y])/2.0;
	pVp->setViewCenter(ViewCenter);

	//set View's height + 0.5
	pVp->setViewHeight((max[Y]-min[Y])+0.5);

	// Get width proportional to height
	double WidthHeightRatio = (max[X]-min[X])/(max[Y]-min[Y]);
	pVp->setHeight(height);

	//The viewport's width maintains the extent windows proportions
	pVp->setWidth(height * WidthHeightRatio);
}

// Get WCS to UCS transformation matrix
static void getTrnsMatrix (AcGeMatrix3d &ucsMatrix, AcGeVector3d ViewDirection, AcGePoint3d origin)
{
	AcGePlane XYPlane(origin, ViewDirection);
	ucsMatrix.setToWorldToPlane(XYPlane);
}

void AddViewPort(AcDbObjectId idToBtr,	AcGePoint3d centerPoint,	AcGeVector3d vecVPoint)
{
	AcDbDatabase* pDb	= acdbHostApplicationServices()->workingDatabase();
	AcDbViewport* pVp = new AcDbViewport;

	// Append new viewport to paper space
	AcDbBlockTableRecord *pBTR;
	if (acdbOpenObject(pBTR, idToBtr, AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(_T("\nCannot access paper space."));
		delete pVp;
		return;
	}

	if (pBTR->appendAcDbEntity( pVp) != Acad::eOk)
	{
		acutPrintf(_T("\nCannot append viewport to paper space."));
		pBTR->close();
		delete pVp;
		return;
	}

	pBTR->close();

	pVp->setCenterPoint(centerPoint);

	//Set View direction
	pVp->setViewDirection(vecVPoint);

	//Assume target point is WCS (0,0,0)
	AcGeMatrix3d ucsMatrix;
	getTrnsMatrix (ucsMatrix, vecVPoint,AcGePoint3d(0,0,0));

	// Get a rectangular window in viewport's x-y plane
	// that represents drawing's extents
	AcGePoint2d maxExt, minExt;
	getUcsExts(maxExt, minExt, ucsMatrix);

	// Here 2 is the view's height. You may change it to any height
	SetViewportExtents(pVp, maxExt, minExt, 2);
	pVp->setOn();
	pVp->close();
} 

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
void OpenDoc( void *pData)
{
	AcApDocument* pDoc = acDocManager->curDocument();
	if (acDocManager->isApplicationContext())
	{
		acDocManager->appContextOpenDocument((const TCHAR *)pData);
		acDocManager->activateDocument(pDoc) ;
	}
	else
	{
		acutPrintf(_T("\nERROR To Open Doc!\n"));
	}
}
////合成多段线
static void AppendPLinePoint(const AcDbObjectId& id,const bool& gotoNext,AcDbPolyline* pLine,int& plIndex)
{
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt,id,AcDb::OpenMode::kForRead);
	if(es != Acad::eOk)
	{
		acutPrintf(_T("open object failed in combine pline"));
		return;
	}
	if(!pEnt->isKindOf(AcDbPolyline::desc()))
	{
		pEnt->close();
		return;
	}

	AcDbPolyline* pPoly = NULL;
	pPoly = (AcDbPolyline*)pEnt;
	AcGePoint2dArray ptArr;
	int count = pPoly->numVerts();
	AcGePoint2d pt ;
	double bulge = 0.0;
	if(gotoNext)
	{
		for(int i = 0;i < count ; i++)
		{
			pPoly->getPointAt(i,pt);
			pPoly->getBulgeAt(i,bulge);
			pLine->addVertexAt(plIndex,pt,bulge);
			plIndex++;
		}
	}
	else
	{
		for(int i = count - 1;i > 0; i--)
		{
			pPoly->getPointAt(i,pt);
			if(i > 0)
			{
				pPoly->getBulgeAt(i - 1,bulge);
			}
			else
			{
				pPoly->getBulgeAt(0,bulge);
			}
			pLine->addVertexAt(plIndex,pt,-bulge);
			plIndex++;
		}
	}

	pEnt->close() ;
}

class CblkcadApp : public AcRxArxApp {

public:
	CblkcadApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		ads_printf(_T("\n**************************************************************\n")) ;
		ads_printf(_T("\n\t正在载入信号所辅助设计工具......\n\t有问题和建议请联系信号所系统室\n\t电话:51865540\n\tEmail:zhfeng@crscd.com.cn")) ;
		ads_printf(_T("\n**************************************************************\n")) ;

		if (!InitSystem())
		{
			return AcRx::kRetError ;
		}

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}


	// ----- zhfblkcad._test2 command (do not rename)
	
// 	static void zhfblkcad_test2(void)
// 	{
// 		// Add your code for command zhfblkcad._test2 here
// 		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
// 		g_cSystem.m_pDb = pDb ;
// 		g_pCurDb = pDb ;
// 		g_cSystem.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;



		
// 		ads_name en ;
// 		ads_point pt ;		
// 		int iIndex = 4 ;
// 		if(RTNORM == acedEntSel (_T("\nselect an ent") , en , pt) )
// 		{
// 			AcDbEntity * pEnt = NULL ;
// 			if(g_cSystem.OpenAcDbEntity(en , pEnt))
// 			{
// 			
				
// 				if (pEnt->isKindOf(AcDbBlockReference::desc()))
// 				{
// 					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
// 					AcGeScale3d scale_3d = pRef->scaleFactors()  ;
// 					double dScaleX = scale_3d.sx ;
// 					double dScaleY = scale_3d.sy ;
// 					acutPrintf(_T("\n0-sx:%.3f, sy:%.3f"), dScaleX,  dScaleY) ;
// 					//AcGeScale3d scale_3dtmp =  scale_3d.inverse() ;
// 					//acutPrintf(_T("\n1-sx:%.3f, sy:%.3f------sxt:%.3f, syt:%.3f"), scale_3d.sx,  scale_3d.sy, scale_3dtmp.sx,  scale_3dtmp.sy) ;
// 					scale_3d =  scale_3d.invert() ;
// 					 acutPrintf(_T("\n2-sx:%.3f, sy:%.3f"), scale_3d.sx,  scale_3d.sy) ;
// 				}
				
// 				AcGeMatrix3d xform; 
// 				/*
// 				//旋转实体
// 				
// 				AcGeVector3d axis(0, 0, 1); //给出当前的旋转平面 
// 				xform.setToRotation(45 * PI/180, axis); 
// 				pEnt->transformBy(xform); 
// 				*/
// 				/*		
// 				//平移实体(xy轴同时移动5)
// 				AcGeMatrix3d xform; 
// 				AcGeVector3d vec(5, 5, 0);  
// 				xform.setToTranslation(vec); 
// 				pEnt->transformBy(xform); 
// 				*/
// 				
// 				/*
// 				//旋转实体
// 				AcGePoint3d p1(0,0,0), p2(2,1,0) , p3;
// 				AcGeMatrix3d xform; 
// 				AcGeVector3d axis(0, 0, 1); //给出当前的旋转平面 
// 				xform.setToRotation(45 * PI/180, axis, p1); 
// 				p3 = p2.transformBy(xform); 
// 				acutPrintf(_T("\np3:%.3f-%.3f-%.3f"), p3.x, p3.y, p3.z) ;
// 				*/		
// 
// 				
// 				if (pEnt->isKindOf(AcDbLine::desc()))
// 				{
// 					ads_name en2 ;
// 					ads_point pt2 ;
// 					if(RTNORM == acedEntSel (_T("\nselect the second ent") , en2 , pt2) )
// 					{
// // 						AcDbObjectId objId ;
// // 						if(Acad::eOk==acdbGetObjectId(objId, en2) )
// // 						{
// // 							AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;
// // 							AppendPLinePoint(objId, true, pPolyLine, iIndex);
// // 						}
// 						AcDbEntity * pEnt2 = NULL ;
// 						if(g_cSystem.OpenAcDbEntity(en2 , pEnt2))
// 						{
// 							if (pEnt2->isKindOf(AcDbLine::desc()))
// 							{
// 								AcDbLine *pLine1 = AcDbLine::cast(pEnt) ;
// 								AcDbLine *pLine2 = AcDbLine::cast(pEnt2) ;
// 
// 								AcGeVector3d vec1 = pLine1->endPoint()-pLine1->startPoint() ;
// 								AcGeVector3d vec2 = pLine2->endPoint()-pLine2->startPoint() ;
// 								double dAng  = vec1.angleTo(vec2) ;
// 								acutPrintf(_T("\ndAng%.3f"), dAng) ;
// 								AcGeVector3d vecZ(0.0, 0.0, 1.0) ;
// 								double dRot ;							
// 								if (dAng<PI)
// 								{
// 									dRot = 0.5*PI-dAng ;
// 								}
// 								else
// 								{
// 									dRot = -(dAng-0.5*PI) ;
// 								}
// 								acutPrintf(_T("\ndRot%.3f"), dRot) ;
// 								xform = xform.rotation(dRot, vecZ, pLine2->startPoint()) ;
// 								pLine2->transformBy(xform) ;
// 								pLine2->setColorIndex(1) ;
// 							}
// 
// 							pEnt2->close() ;
// 						}
// 					}
// 
// 				}
// 				
// 				pEnt->close() ;
// 			}
// 		}

		/*
	
		CStringArray strArrayFilePath ;
		TCHAR chPath[256] ;
		acedGetString(1, _T("\nInput Path:"),  chPath) ;
		CString strPath ;
		strPath.Format(_T("%s"), chPath) ;
		g_cSystem.FindFile(strPath, _T("dwg"), strArrayFilePath, 0, 0) ;
		for (int i=0; i<strArrayFilePath.GetCount(); i++)
		{
			CString strFilePath ;
			strFilePath = strArrayFilePath.GetAt(i) ;
			acutPrintf(_T("\nFile:%s"), strFilePath) ;
			AcDbDatabase *pNewDb = new AcDbDatabase(false) ;
			if (Acad::eOk == pNewDb->readDwgFile(strFilePath))
			{
				g_cSystem.m_pDb = pNewDb ;
				acdbHostApplicationServices()->setWorkingDatabase(pNewDb) ; //解决后台数据库写入的文本，对齐方式问题，记住，文本写入数据库后，要把当前数据库设置为原始状态
			
				BOOL bIsChanged = FALSE ;

				struct resbuf *rb ;
				rb = acutNewRb(RTSHORT) ;
				acedGetVar(_T("INSUNITS"), rb)  ;
				acutPrintf(_T("\n插入比例单位:%d"), rb->resval.rint) ;
				rb->resval.rint = 4 ;
				acedSetVar(_T("INSUNITS"), rb) ;
				acutRelRb(rb) ;
				bIsChanged = TRUE ;
				//acedCommand(RTSTR,_T("zoom"),RTSTR,_T("e"),0) ;  

				acdbHostApplicationServices()->setWorkingDatabase(pDb) ;

				AcDbBlockTable * pBlkTbl = NULL ;
				pNewDb->getBlockTable(pBlkTbl,AcDb::kForRead);

				AcDbBlockTableRecord * pBlkTblRec = NULL ;
				pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
				pBlkTbl->close() ;
				AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
				pBlkTblRec->newIterator( pBlkTblRecIter );
				pBlkTblRec->close();

				AcDbEntity * pEnt = NULL ;
				for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
				{
					//遍历并打开实体 ;
					Acad::ErrorStatus es = pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite) ;

					if (Acad::eOk == es)
					{
						acutPrintf(_T("\n%s"), pEnt->isA()->name()) ;
						if (pEnt->isKindOf(AcDbLine::desc()))
						{
							AcDbLine *pLine = AcDbLine::cast(pEnt) ;
							AcGePoint3d pt_3d_start = pLine->startPoint() ;
							AcGePoint3d pt_3d_end = pLine->endPoint() ;
							int i1 = abs(pt_3d_start.y) ;
							int i2 = abs(pt_3d_end.y) ;
							if (i1==i2 && (i1-1)<0.01)
							{
								pLine->upgradeOpen() ;
								pt_3d_start.y = 0.001 ;
								pt_3d_end.y = -0.001 ;
								pLine->setStartPoint(pt_3d_start) ;
								pLine->setEndPoint(pt_3d_end) ;
								bIsChanged = TRUE ;
							}
						}

						pEnt->close() ;
					}			

				}
				delete pBlkTblRecIter ;
				pBlkTblRecIter = NULL ;
				

				if (bIsChanged)
				{
					pNewDb->saveAs(strFilePath) ;
				}

			}
			delete pNewDb ;
			pNewDb = NULL ;
		}
        */

// 	}
	// ----- zhfblkcad._test command (do not rename)
// 	static void zhfblkcad_test(void)
// 	{
// 		// Add your code for command zhfblkcad._test here	
// 
// 	}



	// ----- zhfblkcad._con_guidao command (do not rename)
	static void zhfblkcad_con_guidao(void)
	{
		// Add your code for command zhfblkcad._con_guidao here
		// 关联前后轨道
		CMaXuBiaoForHD::ConGuiDao() ;
	}
	// ----- zhfblkcad._SetStartFreq command (do not rename)
	static void zhfblkcad_SetStartFreq(void)
	{
		// Add your code for command zhfblkcad._SetStartFreq here
		// 设置起始轨道频率（根据起始频率和频率布置规则自动计算其他频率）
		CMaXuBiaoForHD maxbubiao ;
		maxbubiao.SetStartFreq() ;
	}
	// ----- zhfblkcad._set_freq command (do not rename)
	static void zhfblkcad_set_freq(void)
	{
		// Add your code for command zhfblkcad._set_freq here
		// 设置轨道频率
		CMaXuBiaoForHD maxbubiao ;
		maxbubiao.SetFreq() ;
	}
	// ----- zhfblkcad._delguidaocon command (do not rename)
	static void zhfblkcad_delguidaocon(void)
	{
		// Add your code for command zhfblkcad._delguidaocon here
		// 删除轨道关联
		CMaXuBiaoForHD::DelGuiDaoCon() ;
	}
	// ----- zhfblkcad._checkcon command (do not rename)
	static void zhfblkcad_checkcon(void)
	{
		// Add your code for command zhfblkcad._checkcon here
		// 检查轨道关联
		CMaXuBiaoForHD::CheckGuiDaoCon() ;
	}
	// ----- zhfblkcad._uncheckcon command (do not rename)
	static void zhfblkcad_uncheckcon(void)
	{
		// Add your code for command zhfblkcad._uncheckcon here
		// 取消显示轨道关联
		CMaXuBiaoForHD::UnCheckGuiDaoCon() ;
	}

	// ----- zhfblkcad._mxbinput command (do not rename)
	static void zhfblkcad_mxbinput(void)
	{
		// Add your code for command zhfblkcad._mxbinput here
		g_pCurDb = acdbCurDwg() ;
		g_cSystem.m_pDb = g_pCurDb ;
		//CString strSql ;
		CMaXuBiaoForHD maxubiao ;
		TCHAR chStationName[100] ;
		if(RTNORM==acedGetString(1, _T("\n请输入车站名称："), chStationName))
		{
			maxubiao.m_strStationName.Format(_T("%s"), chStationName) ;
		}
		maxubiao.ReadDwgToDb2(g_PtrCon, g_pCurDb) ;
	}
	// ----- zhfblkcad._mxbdo command (do not rename)
	static void zhfblkcad_mxbdo(void)
	{
		// Add your code for command zhfblkcad._mxbdo here
		CMaXuBiaoForHD maxubiao;
		CMaXuBiaoDlg dlg ;
		dlg.m_strTitle = _T("生成码序表") ;
		dlg.m_dSYT_Height = maxubiao.m_dSiYiTuHeight ;
		dlg.m_dBetweenTable = maxubiao.dBetweenTable_Y ;
		dlg.m_dFrameHeight = maxubiao.m_dBetweenTableAndBottom - 16 ;
		dlg.m_iVersionFor = maxubiao.m_iVersionFor ;
		if (IDOK == dlg.DoModal())
		{
			maxubiao.m_dSiYiTuHeight = dlg.m_dSYT_Height ;
			maxubiao.m_strStationName = dlg.m_strStationName ;
			maxubiao.m_strProjName = dlg.m_strProjName ;
			maxubiao.m_strDrawNum = dlg.m_strDrawNum ;
			maxubiao.dBetweenTable_Y = dlg.m_dBetweenTable ;
			maxubiao.m_dBetweenTableAndBottom = dlg.m_dFrameHeight + 16 ;
			maxubiao.m_strDwgSaveDir = dlg.m_strSavePath ;
			maxubiao.m_iVersionFor = dlg.m_iVersionFor ;

			maxubiao.m_dWidthGuiDao = dlg.m_dWidthGuiDao ;
			maxubiao.m_dWidthBianMa = dlg.m_dWidthBianMa ;
			maxubiao.m_dHeightGuiDao = dlg.m_dHeightGuiDao ;
			maxubiao.m_dHeightBianMa = dlg.m_dHeightBianMa ;

			maxubiao.DoALL() ;
		}
	}
	// ----- zhfblkcad._mxbdozjz command (do not rename)
	static void zhfblkcad_mxbdozjz(void)
	{
		// Add your code for command zhfblkcad._mxbdozjz here
		CMaXuBiaoForHD maxubiao;
		CMaXuBiaoDlg dlg ;
		dlg.m_strTitle = _T("由导入数据表生成码序表（中继站）") ;
		dlg.m_dSYT_Height = maxubiao.m_dSiYiTuHeight ;
		dlg.m_dBetweenTable = maxubiao.dBetweenTable_Y ;
		dlg.m_dFrameHeight = maxubiao.m_dBetweenTableAndBottom - 16 ;
		dlg.m_iVersionFor = maxubiao.m_iVersionFor ;
		if (IDOK == dlg.DoModal())
		{
			maxubiao.m_dSiYiTuHeight = dlg.m_dSYT_Height ;
			maxubiao.m_strStationName = dlg.m_strStationName ;
			maxubiao.dBetweenTable_Y = dlg.m_dBetweenTable ;
			maxubiao.m_dBetweenTableAndBottom = dlg.m_dFrameHeight + 16 ;
			maxubiao.m_strDwgSaveDir = dlg.m_strSavePath ;
			maxubiao.m_iVersionFor = dlg.m_iVersionFor ;

			maxubiao.m_dWidthGuiDao = dlg.m_dWidthGuiDao ;
			maxubiao.m_dWidthBianMa = dlg.m_dWidthBianMa ;
			maxubiao.m_dHeightGuiDao = dlg.m_dHeightGuiDao ;
			maxubiao.m_dHeightBianMa = dlg.m_dHeightBianMa ;

			maxubiao.MaXuBiaoForJH() ;
		}
	}

	// ----- zhfblkcad._mxblkqc command (do not rename)
	static void zhfblkcad_mxblkqc(void)
	{
		// Add your code for command zhfblkcad._mxblkqc here
		CMaXuBiaoForHD maxubiao;
		CMaXuBiaoDlg dlg ;
		dlg.m_strTitle = _T("生成列控中心驱动采集配置表") ;
		dlg.m_dSYT_Height = maxubiao.m_dSiYiTuHeight ;
		dlg.m_dBetweenTable = maxubiao.dBetweenTable_Y ;
		dlg.m_dFrameHeight = maxubiao.m_dBetweenTableAndBottom - 16 ;
		dlg.m_iVersionFor = maxubiao.m_iVersionFor ;
		if (IDOK == dlg.DoModal())
		{
			maxubiao.m_dSiYiTuHeight = dlg.m_dSYT_Height ;
			maxubiao.m_strStationName = dlg.m_strStationName ;
			maxubiao.dBetweenTable_Y = dlg.m_dBetweenTable ;
			maxubiao.m_dBetweenTableAndBottom = dlg.m_dFrameHeight + 16 ;
			maxubiao.m_strDwgSaveDir = dlg.m_strSavePath ;
			maxubiao.m_iVersionFor = dlg.m_iVersionFor ;

			maxubiao.m_dWidthGuiDao = dlg.m_dWidthGuiDao ;
			maxubiao.m_dWidthBianMa = dlg.m_dWidthBianMa ;
			maxubiao.m_dHeightGuiDao = dlg.m_dHeightGuiDao ;
			maxubiao.m_dHeightBianMa = dlg.m_dHeightBianMa ;

			CString strSaveFile ;
			strSaveFile.Format(_T("%s\\列控中心采集驱动配置表.xls"), dlg.m_strSavePath) ;

			maxubiao.ExportLKQCTable(strSaveFile) ;		

		}
	}

	// ----- zhfblkcad._zn_inputfxp command (do not rename)
	static void zhfblkcad_zn_inputfxp(void)
	{
		// Add your code for command zhfblkcad._zn_inputfxp here
		CInputFxpDataDlg dlg ;
		dlg.DoModal() ;
	}
	// ----- zhfblkcad._znreadcablenet command (do not rename)
	static void zhfblkcad_znreadcablenet(void)
	{
		// Add your code for command zhfblkcad._znreadcablenet here
		ads_printf(_T("\n导入电缆网络图......\n")) ;
		//beging for jy
		CDlpxSetDlg dlg ;
		if (dlg.DoModal()==IDOK)
		{
			CCableNetForJY cablenet ;
			cablenet.m_iXhjDcSet = dlg.m_iXHJ_DC ;
			AcDbDatabase * pCurDb = acdbCurDwg() ;
			cablenet.m_pCurDb = pCurDb ;
			g_cSystem.m_pDb = pCurDb ;
			cablenet.ReadAndFillDB(g_PtrCon, pCurDb) ;	
		}	
		//end for jy

		ads_printf(_T("\n导入电缆网络图完成\n")) ;
	}

	// ----- zhfblkcad._znoutdlpx command (do not rename)
	static void zhfblkcad_znoutdlpx(void)
	{
		// Add your code for command zhfblkcad._znoutdlpx here
		CAcModuleResourceOverride myResources ;

		CDLPXDlg dlg ;
		if(dlg.DoModal()==IDOK)
		{
			if (dlg.m_strSavePath!=_T(""))
			{
				//begin for bt
				//CCableNetForBT cablenet ;	
				//end for bt
				CCableNetForJY cablenet ;

				cablenet.m_strSavePath = dlg.m_strSavePath ;
				if (dlg.m_strMode==_T("多图模式"))
				{
					cablenet.ComputeCoreAndDraw() ;
				}
				else if (dlg.m_strMode==_T("单图模式"))
				{
					cablenet.ComputeCoreAndDraw2() ;
				}
			}
		}
	}

	// ----- zhfblkcad._znreadbzt command (do not rename)
	static void zhfblkcad_znreadbzt(void)
	{
		// Add your code for command zhfblkcad._znreadbzt here
		CReadBztDlg dlg ;
		if (dlg.DoModal() == IDOK)
		{
			CStationSystem stationsys ;
			AcDbDatabase *pDb = new AcDbDatabase(false) ;

			int iZNum = 0 ;
			int iZGNum = 0 ;
			if (Acad::eOk == pDb->readDwgFile(dlg.m_strFileZ))
			{
				iZNum = stationsys.ReadZBztToDB(g_PtrCon, pDb) ;	
				acutPrintf(_T("\n读入%d个组合架!"), iZNum) ;
				iZGNum = stationsys.ReadZGBztToDB(g_PtrCon, pDb) ;	
				acutPrintf(_T("\n读入%d个轨道区段排列!"), iZGNum) ;	
			}
			delete pDb ;
			pDb = NULL ;
			
			pDb = new AcDbDatabase(false) ;
			int iYNum = 0 ;
			if (Acad::eOk == pDb->readDwgFile(dlg.m_strFileY))
			{				
				iYNum = stationsys.ReadYBztToDB2(g_PtrCon, pDb) ;
				acutPrintf(_T("\n读入%d个移频柜!"), iYNum) ;
			}
			delete pDb ;
			pDb = NULL ;

			pDb = new AcDbDatabase(false) ;
			int iZHNum = 0 ;
			if (Acad::eOk == pDb->readDwgFile(dlg.m_strFileZH))
			{
				iZHNum = stationsys.ReadZHBztToDB2(g_PtrCon, pDb) ;	
				acutPrintf(_T("\n读入%d个综合柜!"), iZHNum) ;
			}
			delete pDb ;
			pDb = NULL ;

			pDb = new AcDbDatabase(false) ;
			int iGNum = 0 ;
			if (Acad::eOk == pDb->readDwgFile(dlg.m_strFileG))
			{
				iGNum = stationsys.ReadGBztToDB(g_PtrCon, pDb) ;	
				acutPrintf(_T("\n读入%d个轨道柜!"), iGNum) ;
			}
			delete pDb ;
			pDb = NULL ;

			pDb = new AcDbDatabase(false) ;
			int iFxpNum = 0 ;
			if (Acad::eOk == pDb->readDwgFile(dlg.m_strFileF))
			{
				iFxpNum = stationsys.ReadFxpBztToDB(g_PtrCon, pDb) ;	
				acutPrintf(_T("\n读入%d个分线盘!"), iFxpNum) ;
			}
			delete pDb ;
			pDb = NULL ;

			CString strMsg ;
			strMsg.Format(_T("导入清单：\n%d个组合架\n%d个轨道区段排列\n%d个移频柜\n%d个综合柜\n%d个轨道柜\n%d个分线盘"), iZNum, iZGNum, iYNum, iZHNum, iGNum, iFxpNum) ;
			AfxMessageBox(strMsg) ;	
		}
	}

	// ----- zhfblkcad._znreadlsqc command (do not rename)
	static void zhfblkcad_znreadlsqc(void)
	{
		// Add your code for command zhfblkcad._znreadlsqc here
		CFileDialog filedlg(false,NULL,NULL,NULL,_T("(联锁驱采数据(*.xls)|*.xls|"),NULL);
		filedlg.m_ofn.lpstrTitle = _T("选择联锁驱采数据文件");
		CString strFileName  = _T("") ;
		if(filedlg.DoModal() == IDOK)
		{
			strFileName = filedlg.GetPathName() ;
		}
		if (!strFileName.IsEmpty())
		{
			CStationSystem stationsys ;
			stationsys.ReadQcDataToDb(strFileName) ;
		}
	}

	// ----- zhfblkcad._znreadlkqc command (do not rename)
	static void zhfblkcad_znreadlkqc(void)
	{
		// Add your code for command zhfblkcad._znreadlkqc here
		CFileDialog filedlg(false,NULL,NULL,NULL,_T("(列控驱采数据(*.xls)|*.xls|"),NULL);
		filedlg.m_ofn.lpstrTitle = _T("选择列控驱采数据文件");
		CString strFileName  = _T("") ;
		if(filedlg.DoModal() == IDOK)
		{
			strFileName = filedlg.GetPathName() ;
		}
		if (!strFileName.IsEmpty())
		{
			CStationSystem stationsys ;
			stationsys.ReadLkQcDataToDb(strFileName) ;
		}
	}

	// ----- zhfblkcad._zncomputemid command (do not rename)
	static void zhfblkcad_zncomputemid(void)
	{
		// Add your code for command zhfblkcad._zncomputemid here
		CStationSystem stationsys ;
		stationsys.ReplaceSpecialChar2() ;
		stationsys.ComputeFxData() ;
	}

	// ----- zhfblkcad._znoutfxppx command (do not rename)
	static void zhfblkcad_znoutfxppx(void)
	{
		// Add your code for command zhfblkcad._znoutfxppx here
		CStationSetDlg dlg ;
		dlg.m_strTitle = _T("生成分线盘配线") ;

		if (dlg.DoModal()==IDOK)
		{
			CStationSystem stationsys ;
			stationsys.m_strSavePath = dlg.m_strSavePath ;
			stationsys.m_strStationName = dlg.m_strStationName ;
			stationsys.m_strDwgNum = dlg.m_strDwgNum ;

			AcDbDatabase *pDb = new AcDbDatabase ;		
			stationsys.DrawFxpPx(pDb) ;	
			CString strSaveFileName = _T("") ;
			strSaveFileName.Format(_T("%s\\分线盘配线图"), dlg.m_strSavePath) ;
			pDb->saveAs(strSaveFileName) ;			
			delete pDb ;
			pDb = NULL ;

		}
	}

	// ----- zhfblkcad._znoutythdl command (do not rename)
	static void zhfblkcad_znoutythdl(void)
	{
		// Add your code for command zhfblkcad._znoutythdl here
		CStationSetDlg dlg ;
		dlg.m_strTitle = _T("生成站内一体化轨道电路") ;
		if (dlg.DoModal()==IDOK)
		{
			CStationSystem stationsys ;
			stationsys.m_strSavePath = dlg.m_strSavePath ;
			stationsys.m_strStationName = dlg.m_strStationName ;
			stationsys.m_strDwgNum = dlg.m_strDwgNum ;
			stationsys.ComputeYthCircuit() ;
		}
	}

	// ----- zhfblkcad._znout25hzdl command (do not rename)
	static void zhfblkcad_znout25hzdl(void)
	{
		// Add your code for command zhfblkcad._znout25hzdl here
		CStationSetDlg dlg ;
		dlg.m_strTitle = _T("生成25Hz轨道电路") ;

		if (dlg.DoModal()==IDOK)
		{
			CStationSystem stationsys ;
			stationsys.m_strSavePath = dlg.m_strSavePath ;
			stationsys.m_strStationName = dlg.m_strStationName ;
			stationsys.m_strDwgNum = dlg.m_strDwgNum ;

			stationsys.Compute25HzCircuit() ;
		}
	}

	// ----- zhfblkcad._znoutxhjdl command (do not rename)
	static void zhfblkcad_znoutxhjdl(void)
	{
		// Add your code for command zhfblkcad._znoutxhjdl here
		CStationSetDlg dlg ;
		dlg.m_strTitle = _T("生成信号机点灯、驱动采集电路") ;
		if (dlg.DoModal()==IDOK)
		{
			CStationSystem stationsys ;
			stationsys.m_strSavePath = dlg.m_strSavePath ;
			stationsys.m_strStationName = dlg.m_strStationName ;
			stationsys.m_strDwgNum = dlg.m_strDwgNum ;

			stationsys.ComputeJzCircuit() ;
			stationsys.ComputeJlCircuit() ;
			stationsys.ComputeCzCircuit() ;
			stationsys.ComputeDcCircuit() ;	
		}
	}

	// ----- zhfblkcad._znoutdcdl command (do not rename)
	static void zhfblkcad_znoutdcdl(void)
	{
		// Add your code for command zhfblkcad._znoutdcdl here
		CStationSetDlg dlg ;
		dlg.m_strTitle = _T("生成道岔联锁电路") ;

		if (dlg.DoModal()==IDOK)
		{
			CStationSystem stationsys ;

			stationsys.m_strSavePath = dlg.m_strSavePath ;
			stationsys.m_strStationName = dlg.m_strStationName ;
			stationsys.m_strDwgNum = dlg.m_strDwgNum ;

			//该函数用到了在列控数据表软件才进行的导入道岔信息，未导入道岔信息，道岔电路生不出来
			stationsys.ComputerSwitchToDb() ;

			stationsys.Compute_18_d_indoor_Circuit() ;	
			stationsys.Compute_18_outdoor_Circuit() ;
			stationsys.Compute_18_d_qc_Circuit() ;

			stationsys.Compute_18_s_indoor_Circuit() ;	
			stationsys.Compute_18_outdoor_Circuit(1) ;
			stationsys.Compute_18_s_qc_Circuit() ;
		}
	}

	// ----- zhfblkcad._znoutpxdl command (do not rename)
	static void zhfblkcad_znoutpxdl(void)
	{
		// Add your code for command zhfblkcad._znoutpxdl here
		CStationSetDlg dlg ;
		dlg.m_strTitle = _T("生成配线用电路") ;

		if (dlg.DoModal()==IDOK)
		{
			CStationSystem stationsys ;
			stationsys.m_strSavePath = dlg.m_strSavePath ;
			stationsys.m_strStationName = dlg.m_strStationName ;
			stationsys.m_strDwgNum = dlg.m_strDwgNum ;

			CUserSystem mysys ;
			CStringArray strArrayFile ;
			CBlkUtility::FindFile(stationsys.m_strSavePath, _T("dwg"), strArrayFile) ;
			int iFileSum = strArrayFile.GetSize() ;
			acedSetStatusBarProgressMeter(_T("生成配线用电路图"), 0, iFileSum-1) ;	

			for (int i=0; i<iFileSum; i++)
			{
				acedSetStatusBarProgressMeterPos(i) ;
				CString strDwgFile = strArrayFile.GetAt(i) ;
				stationsys.ComputeCircuitForPx(strDwgFile) ;
			}

			acedRestoreStatusBar() ;
		}
	}
    
	// ----- zhfblkcad._lkinput command (do not rename)
	static void zhfblkcad_lkinput(void)
	{
		// Add your code for command zhfblkcad._lkinput here
		CReadLKDataDlg dlg ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			int iRet[8] = {0,0,0,0,0,0,0,0} ;

			if (!dlg.m_strFileQJXH.IsEmpty())
			{
				iRet[0] = lkdata.ImportXHJTable(dlg.m_strFileQJXH) ;
				acutPrintf(_T("\n导入%d个区间"), iRet[0]) ;
			}
			if (!dlg.m_strFileCZXHJ.IsEmpty())
			{
				iRet[1] = lkdata.ImportJzCzXhjInfo(dlg.m_strFileCZXHJ) ;
				acutPrintf(_T("\n导入%d条进出站数据"), iRet[1]) ;
			}
			if (!dlg.m_strFileGDQD.IsEmpty())
			{
				iRet[2] = lkdata.ImportGdSectionInfo(dlg.m_strFileGDQD) ;
				acutPrintf(_T("\n导入%d条轨道区段数据"), iRet[2]) ;
			}
			if (!dlg.m_strFileDCXX.IsEmpty())
			{
				iRet[3] = lkdata.ImportDcInfo(dlg.m_strFileDCXX) ;
				acutPrintf(_T("\n导入%d条道岔信息数据"), iRet[3]) ;
			}
			if (!dlg.m_strFileXLSD.IsEmpty())
			{
				iRet[4] = lkdata.ImportSpeedInfo(dlg.m_strFileXLSD) ;
				acutPrintf(_T("\n导入%d条速度信息数据"), iRet[4]) ;
			}
			if (!dlg.m_strFileCZXX.IsEmpty())
			{
				iRet[5] = lkdata.ImportStationInfo(dlg.m_strFileCZXX) ;
				acutPrintf(_T("\n导入%d条车站信息数据"), iRet[5]) ;
			}
			if (!dlg.m_strFilePDXX.IsEmpty())
			{
				iRet[6] = lkdata.ImportPdTable(dlg.m_strFilePDXX) ;
				acutPrintf(_T("\n导入%d条坡度信息数据"), iRet[6]) ;
			}
			if (!dlg.m_strFileFXXX.IsEmpty())
			{
				iRet[7] = lkdata.ImportDFXTable(dlg.m_strFileFXXX) ;
				acutPrintf(_T("\n导入%d条分相数据"), iRet[7]) ;
			}

			CString strMsg ;
			strMsg.Format(_T("导入清单：\n%d个区间\n%d条进出站数据\n%d条轨道区段数据\n%d条道岔信息数据\n%d条速度信息数据\n%d条车站信息数据\n%d条坡度信息数据\n%d条分相数据"), iRet[0],iRet[1],iRet[2],iRet[3],iRet[4],iRet[5],iRet[6],iRet[7]) ;
			AfxMessageBox(strMsg) ;

		}

	}

	// ----- zhfblkcad._lkdoxhdata command (do not rename)
	static void zhfblkcad_lkdoxhdata(void)
	{
		// Add your code for command zhfblkcad._lkdoxhdata here
		CLKDataTable lkdata ;
		lkdata.m_strMileSysName = _T("DK") ;
		int iLimitToFG = 1200 ;
		if (RTNORM == acedGetInt(_T("\n输入最小区段分割长度<1200>:"), &iLimitToFG))
		{
			if (iLimitToFG>1200)
			{
				lkdata.m_iLimitToFG = iLimitToFG ;
			}
		}
		int iRet = AfxMessageBox(_T("是否自动计算信号机名称？"), MB_YESNOCANCEL) ;
		if (iRet==IDYES)
		{
			lkdata.m_bAutoComputeXhjName = TRUE ;
		}
		else if (iRet==IDNO)
		{
			lkdata.m_bAutoComputeXhjName = FALSE ;
		}
		else
		{
			return ;
		}
		lkdata.ComputeXHData() ;
		lkdata.InsertZnXhData() ;
	}

	// ----- zhfblkcad._lkdoydqdata command (do not rename)
	static void zhfblkcad_lkdoydqdata(void)
	{
		// Add your code for command zhfblkcad._lkdoydqdata here
		CLKDataTable lkdata ;	
		lkdata.ComputeYdqData() ;
	}

	// ----- zhfblkcad._lkoutxhdata command (do not rename)
	static void zhfblkcad_lkoutxhdata(void)
	{
		// Add your code for command zhfblkcad._lkoutxhdata here
		CLkDataSetDlg dlg ;
		dlg.m_strTitle = _T("生成列控数据表-信号数据表") ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			lkdata.m_strXHTitle = dlg.m_strLineName ;
			lkdata.m_nBaOffset = dlg.m_iBaOffset ;
			lkdata.ExportXHData(dlg.m_strSavePath) ;
		}
	}

	// ----- zhfblkcad._lkoutydqdata command (do not rename)
	static void zhfblkcad_lkoutydqdata(void)
	{
		// Add your code for command zhfblkcad._lkoutydqdata here
		CLkDataSetDlg dlg ;
		dlg.m_strTitle = _T("生成列控数据表-应答器位置表") ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			lkdata.m_strXHTitle = dlg.m_strLineName ;		
			lkdata.ExportYdqData(dlg.m_strSavePath) ;
		}
	}
	// ----- zhfblkcad._lkinlsb command (do not rename)
	static void zhfblkcad_lkinlsb(void)
	{
		// Add your code for command zhfblkcad._lkinlsb here
		CFileDialog filedlg(false,NULL,NULL,NULL,_T("(导入联锁表(*.xls)|*.xls|"),NULL);
		filedlg.m_ofn.lpstrTitle = _T("选择联锁表文件");
		CString strFileName  = _T("") ;
		if(filedlg.DoModal() == IDOK)
		{
			strFileName = filedlg.GetPathName() ;
			CLKDataTable lkdata ;
			int iRet = lkdata.ImportLsbTable(strFileName) ;

			acutPrintf(_T("\n导入%d条进路数据"), iRet) ;

		}
	}
	// ----- zhfblkcad._lkoutjldata command (do not rename)
	static void zhfblkcad_lkoutjldata(void)
	{
		// Add your code for command zhfblkcad._lkoutjldata here
		CLkDataSetDlg dlg ;
		dlg.m_strTitle = _T("生成列控数据表-进路信息表") ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			lkdata.m_strCurStation = dlg.m_strLineName ;
			lkdata.ComputeJLInfo() ;
			lkdata.ExportJlData(dlg.m_strSavePath) ;
		}
	}
	// ----- zhfblkcad._drawpdt command (do not rename)
	static void zhfblkcad_drawpdt(void)
	{
		// Add your code for command zhfblkcad._drawpdt here
		CLKDataTable lkdata ;
		int i = 0 ;
		if (RTNORM == acedGetInt(_T("\n生成全线平面图[按比例(0)/按相对位置(1)]:"), &i))
		{
			if (i==0)
			{
				lkdata.DrawPdt() ;
			}
			else
			{
				lkdata.DrawPdt2() ;
			}
		}
	}
	// ----- zhfblkcad._drawsyt command (do not rename)
	static void zhfblkcad_drawsyt(void)
	{
		// Add your code for command zhfblkcad._drawsyt here
		CLKDataTable lkdata ;
		lkdata.DrawSyt() ;
	}

	// ----- zhfblkcad._lkoutpmt command (do not rename)
	static void zhfblkcad_lkoutpmt(void)
	{
		// Add your code for command zhfblkcad._lkoutpmt here
		CReadLkForPmtDlg dlg ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			if (dlg.m_bSelXH==TRUE)
			{
				int iRet1 = lkdata.ImportXHJ_LKDATA(dlg.m_strXhFile) ;
				acutPrintf(_T("\n导入%d条线路数据表数据"), iRet1) ;
			}
			if (dlg.m_bSelYDQ==TRUE)
			{
				int iRet2 = lkdata.ImportYdq_LKDATA(dlg.m_strYdqFile) ;
				acutPrintf(_T("\n导入%d条应答器位置表数据"), iRet2) ;
			}
			if (dlg.m_nPmtOption==0)
			{
				lkdata.DrawPdt_LKDATA() ;
			}
			else
			{
				lkdata.DrawPdt_LKDATA_N() ;
			}			
		}

	}

	// ----- zhfblkcad._lkoutxhjtb command (do not rename)
	static void zhfblkcad_lkoutxhjtb(void)
	{
		// Add your code for command zhfblkcad._lkoutxhjtb here
		CLkDataSetDlg dlg ;
		dlg.m_strTitle = _T("导出信号机位置表") ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			lkdata.m_strXHTitle = dlg.m_strLineName ;
			lkdata.ExportXHTable(dlg.m_strSavePath) ;
		}
	}


	// ----- zhfblkcad._btpredojlt command (do not rename)
	static void zhfblkcad_btpredojlt(void)
	{
		// Add your code for command zhfblkcad._btpredojlt here
		//预处理巴铁电缆径路图
		int nInput = 0 ;
		if (RTNORM==acedGetInt(_T("\n选择巴铁工程版本[前期(0)/后期(1)]"), &nInput))
		{
			int nInput2 = 0 ;
			if (RTNORM==acedGetInt(_T("\n下行线画在上行线上方？[下行在上(0)/上行在上(1)]"), &nInput2))
			{
				AcDbDatabase *pDb = acdbCurDwg() ;
				g_cSystem.m_pDb = pDb ;

				CCableNetForBT cablenet ;
				cablenet.m_iDownLineIsUp = nInput2 ;
				cablenet.m_nForProj = nInput ;
				cablenet.m_pCurDb = pDb ;
				cablenet.ChangeTo2D() ;
				if (cablenet.DrawXhlDz()==FALSE)
				{
					AfxMessageBox(_T("绘制信号楼端子未成功！")) ;
					return ;
				}		
				cablenet.ReplaceWaySideEquipment() ;
				cablenet.ReplaceFS() ;
				cablenet.ReplaceJS() ;
				cablenet.ReplaceHz() ;
				cablenet.ReplaceXhj() ;		
				cablenet.ReplaceOther() ;
			}
		}
	}

	// ----- zhfblkcad._btreadcablenet command (do not rename)
	static void zhfblkcad_btreadcablenet(void)
	{
		// Add your code for command zhfblkcad._btreadcablenet here
		int nInput = 0 ;
		if (RTNORM==acedGetInt(_T("选择巴铁工程版本[前期(0)/后期(1)]"), &nInput))
		{
			//begin for bt
			ads_printf(_T("\n读取巴铁电缆网络图......\n")) ;
			CCableNetForBT cablenet ;
			cablenet.m_nForProj = nInput ;
			AcDbDatabase * pCurDb = acdbCurDwg() ;
			cablenet.m_pCurDb = pCurDb ;
			g_cSystem.m_pDb = pCurDb ;
			acutPrintf(_T("\nBegin ReadBtDljlToDb")) ;
			cablenet.ReadBtDljlToDb() ;
			acutPrintf(_T("\nEnd ReadBtDljlToDb")) ;
			acutPrintf(_T("\nBegin FillDB2")) ;
			cablenet.FillDB2() ;
			acutPrintf(_T("\nEnd FillDB2")) ;
			ads_printf(_T("\n读取巴铁电缆网络图完成\n")) ;
			//end for bt
		}
	}

	// ----- zhfblkcad._btdlpx command (do not rename)
	static void zhfblkcad_btdlpx(void)
	{
		// Add your code for command zhfblkcad._btdlpx here
		CAcModuleResourceOverride myResources ;

		CDLPXDlg dlg ;
		if(dlg.DoModal()==IDOK)
		{
			if (dlg.m_strSavePath!=_T(""))
			{
				//begin for bt
				CCableNetForBT cablenet ;	
				//end for bt		

				cablenet.m_strSavePath = dlg.m_strSavePath ;
				if (dlg.m_strMode==_T("多图模式"))
				{
					cablenet.ComputeCoreAndDraw() ;
				}
				else if (dlg.m_strMode==_T("单图模式"))
				{
					cablenet.ComputeCoreAndDraw2() ;
				}

				cablenet.ReplaceCoreString() ;
			}
		}
	}

	// ----- zhfblkcad._btmxbinput command (do not rename)
	static void zhfblkcad_btmxbinput(void)
	{
		// Add your code for command zhfblkcad._btmxbinput here
		CFileDialog filedlg(false,NULL,NULL,NULL,_T("(巴铁轨道区段表(*.xls)|*.xls|"),NULL);
		filedlg.m_ofn.lpstrTitle = _T("选择巴铁轨道区段表文件");
		CString strFileName  = _T("") ;
		if(filedlg.DoModal() == IDOK)
		{
			strFileName = filedlg.GetPathName() ;
		}
		if (!strFileName.IsEmpty())
		{
			CMaXuBiaoForBT mxbforbt ;
			mxbforbt.ImportGdTable(strFileName) ;
			mxbforbt.SetBelongXhj() ;
		}
	}

	// ----- zhfblkcad._btmxb command (do not rename)
	static void zhfblkcad_btmxb(void)
	{
		// Add your code for command zhfblkcad._btmxb here
		CMaXuBiaoForBT mxbforbt ;
		CBtMxbDlg dlg ;
		if (IDOK == dlg.DoModal())
		{
			mxbforbt.m_strStation = dlg.m_strStation1 ;		
			mxbforbt.m_strStation2 = dlg.m_strStation2 ;
			mxbforbt.m_strSavePath = dlg.m_strSavePath ;
			acedSetStatusBarProgressMeter(_T("巴铁码序表"), 1, 100) ;
			acedSetStatusBarProgressMeterPos(1) ;
			mxbforbt.JinLuBianMa_QJ(0) ;
			acedSetStatusBarProgressMeterPos(50) ;
			mxbforbt.JinLuBianMa_QJ(1) ;
			acedSetStatusBarProgressMeterPos(100) ;
			acedRestoreStatusBar() ;
		}
	}



	// ----- zhfblkcad._dmhdotable command (do not rename)
	static void zhfblkcad_dmhdotable(void)
	{
		// Add your code for command zhfblkcad._dmhdotable here
		CFileDialog filedlg(false,NULL,NULL,NULL,_T("(电码化输入(*.xls)|*.xls|"),NULL);
		filedlg.m_ofn.lpstrTitle = _T("选择电码化输入文件");
		CString strFileName  = _T("") ;
		if(filedlg.DoModal() == IDOK)
		{
			strFileName = filedlg.GetPathName() ;
		}
		if (!strFileName.IsEmpty())
		{
			CZnDmh::PreDoDmhImport(strFileName) ;

		}
	}

		// ----- zhfblkcad._insertguidao command (do not rename)
	static void zhfblkcad_insertguidao(void)
	{
		// Add your code for command zhfblkcad._insertguidao here
		/*
		CAcModuleResourceOverride myResources ;	

		AcDbDatabase* pDB = acdbCurDwg ( ) ;
		CUserSystem mysys ;

		mysys.m_pDb  = pDB ;
		CInsertGuiDaoDlg dlg ;
		if( IDOK == dlg.DoModal () )
		{
			CString strBlcokName[] = {		
				_T("maxubiao_GUIDAO_TG_X_WFG"),
					_T("maxubiao_GUIDAO_TG_X_FG"),
					_T("maxubiao_GUIDAO_JZ_X_Z"),
					_T("maxubiao_GUIDAO_JZ_X_F"),
					_T("maxubiao_GUIDAO_LQ_X_0"),
					_T("maxubiao_GUIDAO_TG_S_WFG"),
					_T("maxubiao_GUIDAO_TG_S_FG"),
					_T("maxubiao_GUIDAO_JZ_S_Z"),
					_T("maxubiao_GUIDAO_JZ_S_F"),
					_T("maxubiao_GUIDAO_LQ_S_0"),
					_T("maxubiao_GUIDAO_GD_X_ALL"),
					_T("maxubiao_GUIDAO_GD_X_LEFT"),
					_T("maxubiao_GUIDAO_GD_X_RIGHT"),
					_T("maxubiao_GUIDAO_GD_S_ALL"),
					_T("maxubiao_GUIDAO_GD_S_LEFT"),
					_T("maxubiao_GUIDAO_GD_S_RIGHT"),
					_T("maxubiao_GUIDAO_GD_C_ALL"),
					_T("maxubiao_GUIDAO_GD_C_LEFT"),
					_T("maxubiao_GUIDAO_GD_C_RIGHT"),
					_T("maxubiao_GUIDAO_ZN_0_0")
			} ;

			CString strSelBlockName ;
			strSelBlockName.Format(_T("%s\\support\\maxubiao\\%s.dwg"), g_strBasePath, strBlcokName[dlg.m_iSelectID]) ;
			AcDbObjectId lay_Id ;
			lay_Id = mysys.Set_Layer( _T("") , _T("轨道") ) ;
			while ( TRUE )
			{
				ads_point base_Pt = { 0,0,0 } ;
				if(RTNORM == ads_getpoint ( NULL , _T("\n点取基点") , base_Pt ) )
				{
					AcDbBlockReference * pRef = NULL ;
					AcGePoint3d base_3d ( 0,0,0 ) ;
					mysys.ConvertPtAdsTo3D ( base_Pt , base_3d ) ;
					if (mysys.InsertBlock(pRef, strSelBlockName, base_3d, FALSE, 0)) 
					{					
						pRef->close() ;
					}
				}
				else
				{
					break ;
				}
			}

			mysys.Set_Layer(_T(""), _T("0")) ;
		}
		*/
	}






	// ----- zhfblkcad._loctoname command (do not rename)
	static void zhfblkcad_loctoname(void)
	{
		// Add your code for command zhfblkcad._loctoname here
		CAcModuleResourceOverride myResources ;	

		CLocToNameDlg dlg ;
		if (dlg.DoModal()==IDOK)
		{
			CLKDataTable lkdata ;
			int iRet = lkdata.GetNameFromLoc(dlg.m_strXlsFile, dlg.m_strInCol, dlg.m_iInRow, dlg.m_strOutCol, dlg.m_iOutRow, dlg.m_iSheet, dlg.m_iUpOrDown) ;		
			acutPrintf(_T("\n共处理%d行"), iRet) ;
		}
	}



	// ----- zhfblkcad._importdmh command (do not rename)
	static void zhfblkcad_importdmh(void)
	{
		// Add your code for command zhfblkcad._importdmh here
		CFileDialog filedlg(false,NULL,NULL,NULL,_T("(电码化输入(*.xls)|*.xls|"),NULL);
		filedlg.m_ofn.lpstrTitle = _T("选择电码化输入文件");
		CString strFileName  = _T("") ;
		if(filedlg.DoModal() == IDOK)
		{
			strFileName = filedlg.GetPathName() ;
		}
		if (!strFileName.IsEmpty())
		{
			CZnDmh::ImportDmh(strFileName) ;

		}
	}

	// ----- zhfblkcad._setxdatass command (do not rename)
	static void zhfblkcad_setxdatass(void)
	{
		// Add your code for command zhfblkcad._setxdatass here
		// 批量设置扩展数据（从数据库读取）
		CUserUtility::SetXdataSS() ;
	}

	// ----- zhfblkcad._contie command (do not rename)
	static void zhfblkcad_contie(void)
	{
		// Add your code for command zhfblkcad._contie here
		CZnDmh::ConTie(0) ;
	}

	// ----- zhfblkcad._con_node command (do not rename)
	static void zhfblkcad_con_node(void)
	{
		// Add your code for command zhfblkcad._con_node here
		CZnDmh::ConTie(1) ;
	}

	// ----- zhfblkcad._set_par command (do not rename)
	static void zhfblkcad_set_par(void)
	{
		// Add your code for command zhfblkcad._set_par here
		CZnDmh::SetPar() ;
	}

	// ----- zhfblkcad._dmhjg command (do not rename)
	static void zhfblkcad_dmhjg(void)
	{
		// Add your code for command zhfblkcad._dmhjg here
		CDmhSetDlg dlg ;
		dlg.m_iDwgType = 0 ;
		dlg.DoModal() ;
	}

	// ----- zhfblkcad._dmhreadjg command (do not rename)
	static void zhfblkcad_dmhreadjg(void)
	{
		// Add your code for command zhfblkcad._dmhreadjg here
		CDmhSetDlg dlg ;
		dlg.m_iDwgType = 1 ;
		dlg.DoModal() ;
	}

	// ----- zhfblkcad._dmhcircuit command (do not rename)
	static void zhfblkcad_dmhcircuit(void)
	{
		// Add your code for command zhfblkcad._dmhcircuit here
		CDmhSetDlg dlg ;
		dlg.m_iDwgType = 2 ;
		dlg.DoModal() ;		
	}

	// ----- zhfblkcad._ReadNB command (do not rename)
	static void zhfblkcad_ReadNB(void)
	{
		// Add your code for command zhfblkcad._ReadNB here
		//begin 读内部

		TCHAR strInput[50]  ;
		if(RTNORM == acedGetString(0,_T("\n输入组合内部名称:"), strInput))
		{
			CString strZhname ;
			strZhname.Format(_T("%s"), strInput) ;
			CString strFitFor ;
			int iFitFor = 0 ; //0 通号院; 1 电化局
			CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FitFor"), strFitFor) ;
			if (strFitFor==_T("通号院"))
			{
				iFitFor = 0 ;
			}
			else if (strFitFor==_T("电化局"))
			{
				iFitFor = 1 ;
			}

			CZnDmh::ReadNB(strZhname, iFitFor) ;
		}
		//end 读内部

	}

	// ----- zhfblkcad._DmhOptions command (do not rename)
	static void zhfblkcad_DmhOptions(void)
	{
		// Add your code for command zhfblkcad._DmhOptions here
		CDmhOptionsDlg dlg ;
		dlg.DoModal() ;
	}

	// ----- zhfblkcad._dmhpx command (do not rename)
	static void zhfblkcad_dmhpx(void)
	{
		// Add your code for command zhfblkcad._dmhpx here
		CDmhSetDlg dlg ;
		dlg.m_iDwgType = 3 ;
		dlg.DoModal() ;	
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CblkcadApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _con_guidao, con_guidao, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _SetStartFreq, SetStartFreq, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _set_freq, set_freq, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _delguidaocon, delguidaocon, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _checkcon, checkcon, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _uncheckcon, uncheckcon, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _mxbinput, mxbinput, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _mxbdo, mxbdo, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _mxbdozjz, mxbdozjz, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _mxblkqc, mxblkqc, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _zn_inputfxp, zn_inputfxp, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znreadcablenet, znreadcablenet, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znoutdlpx, znoutdlpx, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znreadbzt, znreadbzt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znreadlsqc, znreadlsqc, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znreadlkqc, znreadlkqc, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _zncomputemid, zncomputemid, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znoutfxppx, znoutfxppx, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znoutythdl, znoutythdl, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znout25hzdl, znout25hzdl, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znoutxhjdl, znoutxhjdl, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znoutdcdl, znoutdcdl, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _znoutpxdl, znoutpxdl, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkinput, lkinput, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkdoxhdata, lkdoxhdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkdoydqdata, lkdoydqdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkoutxhdata, lkoutxhdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkoutydqdata, lkoutydqdata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkinlsb, lkinlsb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkoutjldata, lkoutjldata, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _drawpdt, drawpdt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _drawsyt, drawsyt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkoutpmt, lkoutpmt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _lkoutxhjtb, lkoutxhjtb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _btpredojlt, btpredojlt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _btreadcablenet, btreadcablenet, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _btdlpx, btdlpx, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _btmxbinput, btmxbinput, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _btmxb, btmxb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _dmhdotable, dmhdotable, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _insertguidao, insertguidao, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _loctoname, loctoname, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _importdmh, importdmh, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _setxdatass, setxdatass, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _contie, contie, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _con_node, con_node, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _set_par, set_par, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _dmhjg, dmhjg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _dmhreadjg, dmhreadjg, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _dmhcircuit, dmhcircuit, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _ReadNB, ReadNB, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _DmhOptions, DmhOptions, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CblkcadApp, zhfblkcad, _dmhpx, dmhpx, ACRX_CMD_TRANSPARENT, NULL)