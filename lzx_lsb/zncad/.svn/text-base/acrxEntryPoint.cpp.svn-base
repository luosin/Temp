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
#include "znsxt.h"
#include "ZnUtility.h"
#include "ZhfPlCableJig.h"
#include "CableNet.h"
#include "ZnOptionDlg.h"
#include "../tcl/tcl/sequential_tree.h"
#include "CableCore.h"
#include "cableboxnode.h"
#include "../blkuserdll/BlkExcelTool.h"
#include "boxnodeent.h"
#include "DistToXhlPalette.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("zhf")

static TCHAR pDWT[] = _T("acad.dwt") ;

CAdUiPaletteSet* g_pDistToXhlSet = NULL ;
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CzncadApp : public AcRxArxApp {

public:
	CzncadApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		// TODO: Add your initialization code here
		CAcModuleResourceOverride myResources ;	
		if (!InitSystem())
		{
			return AcRx::kRetError ;
		}
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		// TODO: Add your code here
		if (g_pDistToXhlSet)
		{
			g_pDistToXhlSet->DestroyWindow() ;
			delete g_pDistToXhlSet ;
		}

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadDwgMsg(void * pkt)
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadDwgMsg (pkt) ;
		// TODO: Add your code here
		CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();
		if (g_pDistToXhlSet)
		{
			if (g_pDistToXhlSet->IsWindowVisible())
			{
				pAcadFrame->ShowControlBar(g_pDistToXhlSet, FALSE, FALSE);
			}
		}
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	// ----- zhfzncad._test command (do not rename)
	static void zhfzncad_test(void)
	{
		// Add your code for command zhfzncad._test here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CUserSystem mysys ;
		mysys.m_pDb = pDb ;

		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;

		//mysys.ZoomView() ;
		int iInput = 0 ;
		if (acedGetInt( _T("\n按照先转换、读入计算轨道名、编辑数据、重新读入并增加直弯切割和极性等信息、计算移动坐标、绘制双线\nchoose an option 1)调整图纸 2)读入数据库（未分段pmt） 3)读入数据库（分段平面图） 4)计算轨道 5)计算坐标偏移 6)绘制双线 7)转换平面图 8)编辑实体数据 9)增加切割绝缘节 10)绘制轨道线......:"), &iInput)==RTNORM)
		{
			if (iInput==1)
			{
				sxt.AdjustDwg() ;				
			}
			else if (iInput==2)
			{
				//读入未转换的pmt
				sxt.ReadAllEntToDB() ;
			}
			else if (iInput==3)
			{
				//读入转换成段的pmt
				sxt.ReadAllEntToDB2() ;			
			}
			else if (iInput==4)
			{
				sxt.FindAllGuiDao() ;
			}
			else if (iInput==5)
			{
				sxt.PreDoGetDcCenter() ;	
				sxt.AddVertexOfCutJYJ() ;
			}
			else if (iInput==6)
			{
				ads_point pt ;
				if (RTNORM==acedGetPoint(NULL, _T("\ninput a point:"), pt))
				{
					AcGePoint3d pt_3d_base(0,0,0) ;
					mysys.ConvertPtAdsTo3D(pt, pt_3d_base) ;
					sxt.DrawSxt(2, 2, pt_3d_base) ;
				}
			}
			else if (iInput==7)
			{
				sxt.ConvertPMT() ;
			}
			else if (iInput==8)
			{
				sxt.ModEntData() ;
			}
			else if (iInput==9)
			{
				sxt.AddCutJYJ() ;
			}
			else if (iInput==10)
			{
				sxt.DrawGuiDao() ;
			}
			else if (iInput==11)
			{
				int i = 0 ;
				if (RTNORM==acedGetInt(_T("\n显示隐藏信息:箱盒（1）/多段线handle（2）/轨道名称（3）"), &i))
				{
					sxt.ShowInfo(i) ;
				}
			}
			else if (iInput==12)
			{
				sxt.m_AdoDb.Execute(_T("update sxt_line set sxt_line_polar=0")) ;
				ads_name en ;
				ads_point pt ;
				long nHandle = 0 ;
				BOOL bRet = FALSE ;
				BOOL bSelectOk = FALSE ;
				BOOL bTestMode = FALSE ;
				while(!bRet)
				{
					int iRetSel = acedEntSel(_T("\n选择左侧一根起点轨道开始计算极性："), en, pt) ;

					if(iRetSel==RTNORM)
					{
						TCHAR szInput[10] ;
						if (RTNORM==	acedGetString(0, _T("\n是否显示过程,[不显示(N)/显示(Y)] <N>:"), szInput) )
						{
							if (_tcscmp(szInput, _T("Y"))==0 || _tcscmp(szInput, _T("y"))==0)
							{
								bTestMode = TRUE ;
							}
						}

						AcDbEntity *pEnt = NULL ;
						if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
						{
							int iIsGuiDao = 0 ;
							mysys.GetXdata(pEnt, _T("ISGUIDAO"), iIsGuiDao) ;
							if (iIsGuiDao==1&&pEnt->isKindOf(AcDbPolyline::desc()))
							{
								nHandle = mysys.GetHandle(pEnt) ;	
								bSelectOk = TRUE ;
								bRet = TRUE ;
							}
							pEnt->close() ;
						}
					}
					else if (iRetSel==RTERROR)
					{
						acedAlert(_T("选择错误，请重新选择一根左侧起点轨道！"))  ;
						continue; 
					}
					else if (iRetSel==RTCAN)
					{
						bRet = TRUE ;
					}
				}
				if (bSelectOk)
				{
					sxt.FillPolar2(nHandle, -1, 0, true, bTestMode) ;
				}

			}
			else if (iInput==13)
			{
				sxt.ReadAllEntToDB2_DT() ;
				sxt.PreDoGetDcCenter() ;	
				sxt.DrawSxt_DT(1.5, 2, AcGePoint3d(0, 1000, 0), 8, 10) ;
			}
			else if (iInput==14)
			{
				sxt.ReadJltToDb() ;
			}
			else if (iInput==15)
			{
				sxt.DrawCableNet() ;
			}
			else if (iInput==16)
			{
				CCableNet cablenet ;
				cablenet.m_iXhjDcSet = 0 ;
				cablenet.m_pCurDb = pDb ;				
				cablenet.ReadAndFillDB(pDb) ;	
			}
			else if (iInput==17)
			{
				CCableNet cablenet ;
				//cablenet.m_strSavePath =_T("d:\\test\\1") ;
				cablenet.ComputeCoreAndDraw() ;
			}
			else if (iInput==18)
			{
				//sxt.FillDefineCoreDetail() ;
				//sxt.ModEntData(2) ;
				sxt.m_iCurJob = 1 ;
				sxt.ReadJltToDb() ;
				double dCoeff = 1.05 ;
				vector<int > vecParam ;
				sxt.GetCableParam(dCoeff, vecParam) ;
				int iSize = sxt.m_vecCableData[0].size() ;
				for (int i=0; i<iSize; i++)
				{
					acutPrintf(_T("\n%s:%d-%d"), sxt.m_strArrayCableSn.GetAt(i), sxt.m_vecCableData[0][i], sxt.m_vecCableData[1][i]) ;
				}
				CBlkExcelTool::WriteCableInfoToXls(_T("c:\\1.xls"), sxt.m_strArrayCableSn, sxt.m_strArrayCableAll, dCoeff, vecParam, sxt.m_vecCableData) ;
				
			}
			else if (iInput==19)
			{	
				ads_name en ;
				ads_point pt ;
				
				int iRetSel = acedEntSel(_T("\n选择多段线"), en, pt) ;
				if(iRetSel==RTNORM)
				{	
					AcGePoint2dArray pt2dArray ;
					int iNumVertex = CZnUtility::GetVertex(en, pt2dArray) ;
					mysys.setVisibility(en, AcDb::kInvisible) ; //先隐藏原多线
					CZhfPlCableJig* pJig = new CZhfPlCableJig(pt2dArray) ;
					int iRet = pJig->doIt() ;
					//用pjig记录的点重新设置原多线
					AcDbEntity *pEnt = NULL ;
					if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForWrite))
					{
						if (pEnt->isKindOf(AcDbPolyline::desc()))
						{
							AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
							int iNumVertexNew = pJig->m_pt2dArray.length() ;
							if (iNumVertexNew>=iNumVertex)
							{
								Acad::ErrorStatus es = pPolyLine->setPointAt(iNumVertex-1, pJig->m_pt2dArray.at(iNumVertex-1)) ;
								for (int i=iNumVertex; i<iNumVertexNew; i++)
								{
									es = pPolyLine->addVertexAt(i, pJig->m_pt2dArray.at(i)) ;
								}
							}

							pPolyLine->setVisibility(AcDb::kVisible) ;
						}
						pEnt->close() ;
					}

					delete pJig ;

				}

			}
			else if(iInput==20)
			{
				sxt.Test2();

			}
			else if(iInput==21)
			{
				sxt.LsbTest2();

			}
			else if(iInput=22)
			{
				sxt.LsbTest();
			}
		}

	}

	// ----- zhfzncad._sxdl command (do not rename)
	static void zhfzncad_sxdl(void)
	{
		// Add your code for command zhfzncad._sxdl here
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		CUserSystem mysys ;
		sxt.SetAcDbDB(pDb) ;		
		mysys.m_pDb = pDb ;

		//保存当前图层，设置要绘制的图层
		AcDbObjectId ojbIdLayerOld = pDb->clayer() ;

		long nHandleBoxStart=0, nHandleBoxEnd=0, nHandleCable=0;
		CZhfPlCableJig* pJig = new CZhfPlCableJig() ;
		int iRet = pJig->doIt() ;
		nHandleBoxStart = pJig->m_nHandleBoxStart ;
		nHandleBoxEnd = pJig->m_nHandleBoxEnd ;	
		nHandleCable = mysys.GetHandle(pJig->entity()) ;
// 		AcDbHandle handleLine ;
// 		pJig->entity()->getAcDbHandle(handleLine) ;
// 		nHandleCable = handleLine.low() ;
		int iCableType = pJig->m_iType ;
		delete pJig ;

		//当前绘制的

		if (iRet==1) //画线ok
		{
			int iColorIndex = 256 ;
			mysys.SetXdata(nHandleCable, _T("IS_CABLE"), 1) ;
			if (iCableType==1)
			{
				mysys.SetXdata(nHandleCable, _T("IS_XHLCABLE"), 1) ;
			}
			mysys.SetEntColorIndex(nHandleCable, 1) ;
			mysys.Highlight(nHandleCable) ;
			actrTransactionManager->flushGraphics();
			acedUpdateDisplay();

			if (iCableType==1) //1，信号楼主电缆；
			{
				sxt.ComputeCableInfo(nHandleCable) ;
				iColorIndex = 1 ;
			}
			else //0，箱盒之间的电缆；2，起始连接盒子，末端连在信号楼主电缆
			{
				sxt.ComputeCableInfo(nHandleBoxStart, nHandleBoxEnd, nHandleCable, iCableType) ;				
			}

			mysys.UnHighlight(nHandleCable) ;

			mysys.SetEntColorIndex(nHandleCable, iColorIndex) ;
			if (iCableType==1)
			{
				mysys.SetPLineWidth(nHandleCable, 0.5) ;
			}
			sxt.SetLayer(nHandleCable, sxt.m_iCurDljlType) ;
			if (iCableType==0)
			{
				sxt.SetLayer(nHandleBoxEnd, sxt.m_iCurDljlType) ;
			}
		}

		pDb->setClayer(ojbIdLayerOld) ; //恢复当前图层
	}

	// ----- zhfzncad._pmtzh command (do not rename)
	static void zhfzncad_pmtzh(void)
	{
		// Add your code for command zhfzncad._pmtzh here
		if(AfxMessageBox(_T("将平面图轨道转换为一段段的轨道，此操作会清除整张图所有扩展数据！"), MB_OKCANCEL)==IDOK)
		{
			CZnSxt sxt ;
			sxt.ConvertPMT() ;
		}
	}

	// ----- zhfzncad._pmtgdname command (do not rename)
	static void zhfzncad_pmtgdname(void)
	{
		// Add your code for command zhfzncad._pmtgdname here
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;		
		sxt.FindAllGuiDao() ;
	}

	// ----- zhfzncad._pmtaddjyj command (do not rename)
	static void zhfzncad_pmtaddjyj(void)
	{
		// Add your code for command zhfzncad._pmtaddjyj here
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;		
		sxt.AddCutJYJ() ;
	}

	// ----- zhfzncad._pmttestjx command (do not rename)
	static void zhfzncad_pmttestjx(void)
	{
		// Add your code for command zhfzncad._pmttestjx here
		AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		CUserSystem mysys ;
		sxt.SetAcDbDB(pDb) ;		
		mysys.m_pDb = pDb ;

		ads_name en ;
		ads_point pt ;
		long nHandle = 0 ;
		BOOL bRet = FALSE ;
		BOOL bSelectOk = FALSE ;
		//BOOL bTestMode = FALSE ;
		int iTestMode = 0 ;
		while(!bRet)
		{
			int iRetSel = acedEntSel(_T("\n选择左侧一根起点轨道开始计算极性："), en, pt) ;

			if(iRetSel==RTNORM)
			{
				//TCHAR szInput[10] ;
				if (acedGetInt(_T("\n请输入参数：[0，仅提示极性冲突段/1，显示辅助极性线条/2，显示计算过程] <0>:"), &iTestMode) )
				{
				}

				AcDbEntity *pEnt = NULL ;
				if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
				{
					int iIsGuiDao = 0 ;
					mysys.GetXdata(pEnt, _T("ISGUIDAO"), iIsGuiDao) ;
					if (iIsGuiDao==1&&pEnt->isKindOf(AcDbPolyline::desc()))
					{
						nHandle = mysys.GetHandle(pEnt) ;	
						bSelectOk = TRUE ;
						bRet = TRUE ;
					}
					pEnt->close() ;
				}
			}
			else if (iRetSel==RTERROR)
			{
				acedAlert(_T("选择错误，请重新选择一根左侧起点轨道！"))  ;
				continue; 
			}
			else if (iRetSel==RTCAN)
			{
				bRet = TRUE ;
			}
		}
		if (bSelectOk)
		{
			//CViewExtents viewExt ;
			//mysys.GetViewExtents(viewExt) ;

			sxt.ReadAllEntToDB2() ;
			//偏移
			sxt.PreDoGetDcCenter() ;	
			sxt.AddVertexOfCutJYJ() ;

			sxt.m_AdoDb.Execute(_T("update sxt_line set sxt_line_polar=0")) ;
			sxt.m_bFillPolarErro = false ;
			sxt.FillPolar2(nHandle, -1, 0, true, iTestMode) ;

			//mysys.SetViewExtents(viewExt) ;
		}
	}

	// ----- zhfzncad._pmttosxt command (do not rename)
	static void zhfzncad_pmttosxt(void)
	{
		// Add your code for command zhfzncad._pmttosxt here
		ads_point pt ;
		if (RTNORM==acedGetPoint(NULL, _T("\n在平面图上方选择一个点(尽量和平面图左侧对齐):"), pt))
		{
			AcGePoint3d pt_3d_base(0,0,0) ;
			AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
			CZnSxt sxt ;
			CUserSystem mysys ;
			sxt.SetAcDbDB(pDb) ;		
			mysys.m_pDb = pDb ;

			mysys.ConvertPtAdsTo3D(pt, pt_3d_base) ;
			sxt.DrawSxt(2, 2, pt_3d_base) ;
		}
	}

	// ----- zhfzncad._znoption command (do not rename)
	static void zhfzncad_znoption(void)
	{
		// Add your code for command zhfzncad._znoption here
		CZnOptionDlg dlg ;
		dlg.DoModal() ;
	}

	// ----- zhfzncad._editent command (do not rename)
	static void zhfzncad_editent(void)
	{
		// Add your code for command zhfzncad._editent here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;
		sxt.ModEntData() ;
	}

	// ----- zhfzncad._sxtbox command (do not rename)
	static void zhfzncad_sxtbox(void)
	{
		// Add your code for command zhfzncad._sxtbox here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;

		sxt.DrawZzj() ;
		sxt.DrawBoxFsE() ;
		sxt.DrawBoxDMH() ;
		sxt.DrawBoxXhj() ;
	}

	// ----- zhfzncad._showpmtinfo command (do not rename)
	static void zhfzncad_showpmtinfo(void)
	{
		// Add your code for command zhfzncad._showpmtinfo here
		int i = 0 ;
		if (RTNORM==acedGetInt(_T("\n显示隐藏信息:箱盒（1）/多段线handle（2）/轨道名称（3）"), &i))
		{
			AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
			CZnSxt sxt ;
			sxt.SetAcDbDB(pDb) ;
			sxt.ShowInfo(i) ;
		}
	}

	// ----- zhfzncad._checkgdline command (do not rename)
	static void zhfzncad_checkgdline(void)
	{
		// Add your code for command zhfzncad._checkgdline here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;
		sxt.CheckGdLine() ;
	}

	// ----- zhfzncad._editgd command (do not rename)
	static void zhfzncad_editgd(void)
	{
		// Add your code for command zhfzncad._editgd here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;
		sxt.ModEntData(2) ;		
	}

	// ----- zhfzncad._pmtComputeLsb command (do not rename)
	static void zhfzncad_pmtComputeLsb(void)
	{
		// Add your code for command zhfzncad._pmtComputeLsb here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CUserSystem mysys ;
		mysys.m_pDb = pDb ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;
		//CZnSxt sxt ;
		sxt.LsbTest2();
	}
	
	// ----- zhfzncad._checkoverlap command (do not rename)
	static void zhfzncad_checkoverlap(void)
	{
		// Add your code for command zhfzncad._checkoverlap here
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;
		sxt.CheckJyjXhj() ;
	}

	// ----- zhfzncad._recomputezzj command (do not rename)
	static void zhfzncad_recomputezzj(void)
	{
		// Add your code for command zhfzncad._recomputezzj here
		ads_name en ;
		ads_point pt ;
		if(acedEntSel(_T("\n选择一根连接到主电缆的转辙机电缆:"), en, pt)==RTNORM)
		{
			AcDbObjectId objId ;
			acdbGetObjectId(objId, en) ;
			AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
			CZnSxt sxt ;
			sxt.SetAcDbDB(pDb) ;	
			sxt.ReComputeZzjCable(sxt.m_mysys.GetHandle(objId)) ;
		}
	}

	// ----- zhfzncad._disttoxhl command (do not rename)
	static void zhfzncad_disttoxhl(void)
	{
		// Add your code for command zhfzncad._disttoxhl here
		CMDIFrameWnd* pAcadFrame  = acedGetAcadFrame();
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
		CZnSxt sxt ;
		sxt.SetAcDbDB(pDb) ;	

		if (g_pDistToXhlSet==NULL)
		{					
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

// 				acutPrintf(_T("\ncount:%d"), vecDist.size()) ;
// 				for (int i=0; i<vecDist.size(); i++)
// 				{
// 					for (int j=0; j<vecDist[i].size(); j++)
// 					{
// 						acutPrintf(_T("\n%d-%d:%s-%s-%s"), i, j, vecDist[i][j][0], vecDist[i][j][1], vecDist[i][j][2]) ;
// 					}
// 				}

				g_pDistToXhlSet = new CAdUiPaletteSet ;
				CRect rect(0, 0, 310, 300);
				g_pDistToXhlSet->Create(_T("平面图信息"), WS_OVERLAPPED|WS_DLGFRAME, rect, pAcadFrame,	PSS_EDIT_NAME | PSS_PROPERTIES_MENU | PSS_AUTO_ROLLUP |	PSS_CLOSE_BUTTON) ;

				CDistToXhlPalette* pDistToXhlPalette = new CDistToXhlPalette() ;
				pDistToXhlPalette->m_grid.m_vecToXhl = vecDist ;
				pDistToXhlPalette->Create(WS_CHILD | WS_VISIBLE|WS_VSCROLL,_T("距离(公里标)"), g_pDistToXhlSet, PS_EDIT_NAME);

				g_pDistToXhlSet->AddPalette(pDistToXhlPalette) ;
				// Finally show the palette set
				g_pDistToXhlSet->EnableDocking(CBRS_ALIGN_ANY);
				g_pDistToXhlSet->RestoreControlBar();

				pAcadFrame->ShowControlBar(g_pDistToXhlSet, TRUE, FALSE);
			}
		}
		else
		{
			if (g_pDistToXhlSet->IsWindowVisible())
			{
				pAcadFrame->ShowControlBar(g_pDistToXhlSet, FALSE, FALSE);
			}
			else
			{
				pAcadFrame->ShowControlBar(g_pDistToXhlSet, TRUE, FALSE);
				CDistToXhlPalette* pPalette = (CDistToXhlPalette*)(g_pDistToXhlSet->GetPalette(0)) ;
				pPalette->PostMessage(WM_COMMAND, MAKEWPARAM(IDS_BUTTON_DWGTOGRID, BN_CLICKED), NULL);
			}	
		}
	}
	// -----------------------------------------------------------------------------
	//virtual AcRx::AppRetCode On_kUnloadDwgMsg(void * pkt);
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CzncadApp)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _test, test, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _sxdl, sxdl, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _pmtzh, pmtzh, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _pmtgdname, pmtgdname, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _pmtaddjyj, pmtaddjyj, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _pmttestjx, pmttestjx, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _pmttosxt, pmttosxt, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _znoption, znoption, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _editent, editent, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _sxtbox, sxtbox, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _showpmtinfo, showpmtinfo, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _checkgdline, checkgdline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _editgd, editgd, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _pmtComputeLsb, pmtComputeLsb, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _checkoverlap, checkoverlap, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _recomputezzj, recomputezzj, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CzncadApp, zhfzncad, _disttoxhl, disttoxhl, ACRX_CMD_TRANSPARENT, NULL)
