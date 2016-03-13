// CableNetForJY.cpp: implementation of the CCableNetForJY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CableNetForJY.h"
#include "DBUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern BOOL bFirst ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCableNetForJY::CCableNetForJY()
: m_iXhjDcSet(0)
{
	m_pCurDb = NULL ;
	
	m_iDrawMode = 0 ;
	
	m_dCellHight = 5 ;
	m_dCellWidth = 40 ;
	m_iReadCablenetPos = 0 ;
	m_iReadCablenetCount = 100 ;
	
	CBlkUtility::GetBasePath(m_strSysBasePath) ;


}

CCableNetForJY::~CCableNetForJY()
{

}

BOOL CCableNetForJY::ReadEntToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("cablenet_ent")) ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//处理图块 AcDbBlockReference ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				//得到实体的handle（对每个dwg是唯一的标示）
				AcDbHandle handleTmp;
				Adesk::UInt32 iHandle_Low = 0, iHandle_High = 0 ;
				pEnt->getAcDbHandle(handleTmp) ;

				iHandle_Low = handleTmp  ;							

				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;	
				
				//得到块名
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;

				CStringArray strArrayBlkRet ;
				CBlkUtility::DivideString(strBlockName, _T("_"), strArrayBlkRet) ;

				CString strBoxType = _T("") ;
				CString strDevType = _T("") ;
				CString strDevName = _T("") ;
				CString strDevFor = _T("JY1") ;
				int iIsDianMaHua = 0 ;
				AcGePoint3d pt_3d_block(0,0,0) ;
				pt_3d_block = pRef->position() ;

				AcGePoint3d pt_3d_search_topleft(0,0,0) ;
				AcGePoint3d pt_3d_search_bottomright(0,0,0) ;

				CString strBlkType = strArrayBlkRet.GetAt(0) ;
				if (strBlkType==_T("BOX")) 
				{
					CString strTmp = strArrayBlkRet.GetAt(1) ;
					mysys.GetAttValue(pRef, strTmp, strBoxType) ;
					if (strTmp==_T("XB"))
					{
						pt_3d_search_topleft.x = pt_3d_block.x-6 ;
						pt_3d_search_topleft.y = pt_3d_block.y+7 ;
						pt_3d_search_bottomright.x = pt_3d_block.x+6 ;
						pt_3d_search_bottomright.y = pt_3d_block.y-7 ;
						//找出盒子对应的设备（目前只是信号机，20100120）
						this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strDevType, strDevName, strDevFor, 0) ;						
					}
					else if (strTmp==_T("HF"))
					{
						CString strHFType = strArrayBlkRet.GetAt(2) ;
						//HFTYPE=0:使用BOX_HF_0块，HFTYPE=1：使用BOX_HF_1块
						if (strHFType==_T("0"))
						{
							pt_3d_search_topleft.x = pt_3d_block.x-9 ;
							pt_3d_search_topleft.y = pt_3d_block.y+9 ;
							pt_3d_search_bottomright.x = pt_3d_block.x+9 ;
							pt_3d_search_bottomright.y = pt_3d_block.y-9 ;

							CString strDevTypeTmp ;
							this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strDevTypeTmp, strDevName, strDevFor, 1) ;						
						}
						else if (strHFType==_T("1"))
						{
							mysys.GetAttValue(pRef, _T("HFNUM"), strDevName) ;
						}
						strBoxType.Insert(0,_T("HF")) ;

					}
					else if (strTmp==_T("HZ"))
					{
						pt_3d_search_topleft.x = pt_3d_block.x-6 ;
						pt_3d_search_topleft.y = pt_3d_block.y+7 ;
						pt_3d_search_bottomright.x = pt_3d_block.x+6 ;
						pt_3d_search_bottomright.y = pt_3d_block.y-7 ;
						//找出盒子对应的设备
						this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strDevType, strDevName, strDevFor, 0) ;						
						strBoxType.Insert(0,_T("HZ")) ;
					}	

				}//end if box
// 				else if (strArrayBlkRet.GetAt(0)==_T("XHJ"))
// 				{
// 					CString strXhjTypeTmp = strArrayBlkRet.GetAt(1) ; //jz-xb
// 					CString strXhjTypeTmpPre = strXhjTypeTmp.Left(strXhjTypeTmp.Find(_T("-"))) ; //jz
// 					CString strXhjTypeTmpBack = strXhjTypeTmp.Mid(strXhjTypeTmp.Find(_T("-"))+1) ; //xb
// 					//结果形式XHJ_5_JH
// 					if (strXhjTypeTmpPre==_T("JZ") || strXhjTypeTmpPre==_T("JL"))
// 					{
// 						strDevType.Format(_T("%sB_%s_%s_%s"), strXhjTypeTmpPre, strArrayBlkRet.GetAt(2), strArrayBlkRet.GetAt(5), strArrayBlkRet.GetAt(6)) ;
// 					}
// 					else if (strXhjTypeTmpPre==_T("CZ")&&strArrayBlkRet.GetAt(2)==_T("4")&&strArrayBlkRet.GetAt(5)!=_T("0"))
// 					{
// 						strDevType.Format(_T("%sB_%s_%s_%s"), strXhjTypeTmpPre, strArrayBlkRet.GetAt(2), strArrayBlkRet.GetAt(5), strArrayBlkRet.GetAt(6)) ;
// 					}
// 					else
// 					{
// 						strDevType.Format(_T("%s_%s_%s_%s"), strXhjTypeTmpPre, strArrayBlkRet.GetAt(2), strArrayBlkRet.GetAt(5), strArrayBlkRet.GetAt(6)) ;
// 					}
// 					mysys.GetAttValue(pRef, _T("XHJNAME"), strDevName) ;
// 					CString strTmp = strXhjTypeTmpBack ;
// 					//strTmp.MakeUpper() ;
// 					mysys.GetAttValue(pRef, strTmp, strBoxType) ;
// 					if (strXhjTypeTmpBack==_T("HZ"))
// 					{
// 						strBoxType.Insert(0,_T("HZ")) ; //such as "HZ24"
// 					}
// 				}//end else if xhj
				else if (strBlkType==_T("ZZJ"))
				{					
					if (strArrayBlkRet.GetAt(2)==_T("JH"))
					{
						strDevFor = _T("JH") ;
						CString strTmp = _T("") ;
						mysys.GetAttValue(pRef, _T("DCNUM-DEV"), strTmp) ;
						strDevType.Format(_T("%s"),strTmp.Mid(strTmp.Find(_T('-'))+1)) ;
						strTmp.Delete(strTmp.Find(_T('-'))) ;
						strDevName = strTmp ;
						
						strTmp = strArrayBlkRet.GetAt(1) ;
						//strTmp.MakeUpper() ;				
						mysys.GetAttValue(pRef, strTmp, strBoxType) ;
						strBoxType.Insert(0, _T("HZ")) ;
					}
					else if (strArrayBlkRet.GetAt(2)==_T("JY"))
					{
						
						mysys.GetAttValue(pRef, _T("ZZJ_TYPE"), strDevFor) ;
						mysys.GetAttValue(pRef, _T("DC_NUM"), strDevName) ;
// 						strDevFor = strDevName.Mid(strDevName.Find(_T("-"))+1) ; //ZD6/ZYJ6-7
// 						strDevName = strDevName.Left(strDevName.Find(_T("-"))) ; //道岔编号 
						

						CString strActIndex = _T(""), strUseCoreNum = _T(""), strActNum = _T("") ;
						mysys.GetAttValue(pRef, _T("ACT_INDEX"), strActIndex) ;
						if (strActIndex==_T(""))
						{
							strActIndex = _T("1") ;
						}
						mysys.GetAttValue(pRef, _T("CORE_NUM"), strUseCoreNum) ;
						mysys.GetAttValue(pRef, _T("ACT_NUM"), strActNum) ;
						if (strDevFor==_T("SH5"))
						{
							strActNum = strActIndex = _T("0") ;
							strDevName = _T("SH5") ;
						}
						strDevType.Format(_T("ZZJ_%s_%s"), strActNum, strActIndex) ;
						mysys.GetAttValue(pRef, _T("HZ"), strBoxType) ;
						//strBoxType.Insert(0, _T("HZ")) ;

					}

				}
				//20100407-1711 here
				else if (strBlkType==_T("FJD"))
				{
					CString strFJDTypeTmp = strArrayBlkRet.GetAt(1) ; //JD-XB1
					CString strFJDTypeTmpPre = strFJDTypeTmp.Left(strFJDTypeTmp.Find(_T("-"))) ; //JD
					strBoxType = strFJDTypeTmp.Mid(strFJDTypeTmp.Find(_T("-"))+1) ; //XB1
					strDevType.Format(_T("%s_%s_%s"), strFJDTypeTmpPre, strArrayBlkRet.GetAt(2), strArrayBlkRet.GetAt(strArrayBlkRet.GetSize()-1)) ;
					mysys.GetAttValue(pRef, _T("GDNAME"), strDevName) ;					
					mysys.GetXdata(pRef, _T("IS_DIANMAHUA"), iIsDianMaHua) ;
				}
				else if (strBlkType==_T("YDQ"))
				{
					CString strYDQTypeTmp = strArrayBlkRet.GetAt(1) ; //YD-HZ
					CString strYDQTypeTmpPre = strYDQTypeTmp.Left(strYDQTypeTmp.Find(_T("-"))) ; //YD

					strBoxType = strYDQTypeTmp.Mid(strYDQTypeTmp.Find(_T("-"))+1) ; //HZ
					CString strBoxNum = _T("6") ;
					mysys.GetAttValue(pRef, strBoxType, strBoxNum) ;
					strBoxType+=strBoxNum ;
					strDevType.Format(_T("%s_%s"), strYDQTypeTmpPre, strArrayBlkRet.GetAt(strArrayBlkRet.GetSize()-1)) ;
					mysys.GetAttValue(pRef, _T("YDNAME"), strDevName) ;
				}
				else if (strBlkType==_T("DZ"))
				{
					mysys.GetAttValue(pRef, _T("XHLDZ"), strDevName) ;	
					strBoxType = _T("XHL") ;
				}
				else
				{
					pEnt->close() ;
					continue ;
				}

				//数据入库
				try
				{
				strSql.Format(_T("insert into cablenet_ent(handle_low,handle_high,devname,devtype,dev_for, boxtype,pt_x,pt_y, is_dianmahua) \
					values(%d,%d,'%s','%s','%s','%s',%f,%f, %d)"),\
					iHandle_Low, iHandle_High, strDevName, strDevType, strDevFor, strBoxType, pt_3d_block.x, pt_3d_block.y, iIsDianMaHua) ;
				ExecSqlString(strSql) ;
				}
				catch (_com_error& e)
				{
					CString sBuff = CBlkUtility::GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;	
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error in ReadEntToDB")) ;

				}
				
			}
			pEnt->close() ;
		}
		
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

// 	if (!m_bSet_XHL_X)
// 	{
// 		bRet = FALSE ;
// 		AfxMessageBox(_T("注意：没有发现信号楼节点！")) ;
// 	}

	return bRet ;

}


BOOL CCableNetForJY::FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strDevType, CString &strDevName, CString & strDevFor, int iFindType)
{
	BOOL bRet = FALSE ;
	CUserSystem mysys ;
	mysys.m_pDb = this->m_pCurDb ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pRef->position() ;
				//只处理规定区域；
				if((textp_3d.x>pt_3d_topleft.x && textp_3d.x<pt_3d_bottomright.x) && (textp_3d.y>pt_3d_bottomright.y && textp_3d.y<pt_3d_topleft.y))
				{
					//得到块名
			
					CString strBlockName ;
					mysys.GetBlockName(pRef, strBlockName) ;					
					//strBlockName.MakeLower() ;

					strBlockName.TrimLeft();
					strBlockName.TrimRight() ;
					if (!strBlockName.IsEmpty())
					{
						CStringArray strArrayRet ;
						CBlkUtility::DivideString(strBlockName, _T("_"), strArrayRet) ;
						switch (iFindType)
						{
						case 0:
							{
								if (strBlockName.Left(3)==_T("XHJ"))
								{
									CString strXhjTypeTmp = strArrayRet.GetAt(1) ; //jz
									//结果形式JZB_5_0_0
									if (strXhjTypeTmp==_T("JZ") || (strXhjTypeTmp==_T("JL")&& _tstoi(strArrayRet.GetAt(2))>3))
									{
										strDevType.Format(_T("%sB_%s_%s_%s"), strArrayRet.GetAt(1), strArrayRet.GetAt(2), strArrayRet.GetAt(3), strArrayRet.GetAt(4)) ;
									}
									else if (strXhjTypeTmp==_T("CZ")&&strArrayRet.GetAt(2)==_T("4")&&strArrayRet.GetAt(3)!=_T("0"))
									{
										strDevType.Format(_T("%sB_%s_%s_%s"), strArrayRet.GetAt(1), strArrayRet.GetAt(2), strArrayRet.GetAt(3), strArrayRet.GetAt(4)) ;
									}
									else
									{
										strDevType.Format(_T("%s_%s_%s_%s"), strArrayRet.GetAt(1), strArrayRet.GetAt(2), strArrayRet.GetAt(3), strArrayRet.GetAt(4)) ;
										if (strXhjTypeTmp==_T("DC")&&m_iXhjDcSet==1)
										{
											strDevFor = _T("JY2") ;
										}
									}

									mysys.GetAttValue(pRef, _T("XHJNAME"), strDevName) ;
									bRet = TRUE ;						
								}

							}
							break;
						case 1:
							{
								if (strBlockName.Left(3)==_T("ATT"))
								{
									mysys.GetAttValue(pRef, _T("HFNUM"), strDevName) ;
									bRet = TRUE ;						
								}

							}
							break;
						}
					}					
				}				
			}
			pEnt->close() ;
			if (bRet)
			{
				break ;
			}
		}		
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	return bRet ;
}

BOOL CCableNetForJY::ReadConCableToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;
	CString strError(_T("Error in ReadConCableToDB:")) ;

	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("connect_cable")) ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//处理AcDbPolyline ;
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				Adesk::UInt32 iLeftEntHandle_Low = 0 ;
				Adesk::UInt32 iRightEntHandle_Low = 0 ;
				
				CString strCableStr ;
				AcGePoint3d pt_3d_text(0,0,0) ;

				AcDbPolyline * pPolyLine = (AcDbPolyline *)pEnt ;	
				int iVertNum = pPolyLine->numVerts() ;
				AcGePoint3d pt_3d_ptStartEnd[2] ;
				//pt= {(0,0,0),(0,0,0)} ;
				pPolyLine->getPointAt(0,pt_3d_ptStartEnd[0]) ;
				pPolyLine->getPointAt(iVertNum-1, pt_3d_ptStartEnd[1]) ;
				//ads_printf(_T("\n线段起始端 %.2f-%.2f %.2f-%.2f"), pt_3d_ptStartEnd[0].x,pt_3d_ptStartEnd[0].y,pt_3d_ptStartEnd[1].x,pt_3d_ptStartEnd[1].y ) ;
				//将起始点按x从小到大排序
				if (pt_3d_ptStartEnd[0].x>pt_3d_ptStartEnd[1].x)
				{
					AcGePoint3d pt_3d_tmp(0,0,0) ;
					pt_3d_tmp = pt_3d_ptStartEnd[0] ;
					pt_3d_ptStartEnd[0] = pt_3d_ptStartEnd[1] ;
					pt_3d_ptStartEnd[1] = pt_3d_tmp ;
				}

				//here, 20100120-2255
				//begin polyline左边端点周边要搜索区域
				//由于可能是信号楼节点实体，比较小，所以先搜一个较小的范围
				AcGePoint3d pt1_3d_topleft(0,0,0) ;
				AcGePoint3d pt1_3d_bottomright(0,0,0) ;
				pt1_3d_topleft.x = pt_3d_ptStartEnd[0].x-1.5 ;
				pt1_3d_topleft.y = pt_3d_ptStartEnd[0].y+1 ;
				pt1_3d_bottomright.x = pt_3d_ptStartEnd[0].x+1 ;
				pt1_3d_bottomright.y = pt_3d_ptStartEnd[0].y-1 ;
				AcDbHandle left_ent_Handle ;
				BOOL bFindLeft = FALSE ;
				bFindLeft = this->FindBoxBetweenCable(pt1_3d_topleft,pt1_3d_bottomright,left_ent_Handle) ;
				if (!bFindLeft)
				{
					//如果在一个较小范围没有找到（主要是找信号楼端子），则扩大范围
					pt1_3d_topleft.x = pt_3d_ptStartEnd[0].x-6 ;
					pt1_3d_topleft.y = pt_3d_ptStartEnd[0].y+3.5 ;
					pt1_3d_bottomright.x = pt_3d_ptStartEnd[0].x+4 ;
					pt1_3d_bottomright.y = pt_3d_ptStartEnd[0].y-3.5 ;
					bFindLeft = this->FindBoxBetweenCable(pt1_3d_topleft,pt1_3d_bottomright,left_ent_Handle) ;
				}
				if (!bFindLeft)
				{
					strError.Format(_T("%s\nLeft near (%.3f-%.3f) no Box found"), strError, pt_3d_ptStartEnd[0].x, pt_3d_ptStartEnd[0].y );
					ads_printf(strError) ;
					//
					Acad::ErrorStatus es = pEnt->upgradeOpen() ;
					if(Acad::eOk == es)
					{
						pEnt->setColorIndex(3) ;
						pEnt->downgradeOpen() ;
					}					
					pEnt->close() ;
					continue ;					
				}
				iLeftEntHandle_Low = left_ent_Handle ;				
				//end polyline左边端点周边要搜索区域
				
				//begin polyline右边端点周边要搜索区域
				pt1_3d_topleft.x = pt_3d_ptStartEnd[1].x-1.5 ;
				pt1_3d_topleft.y = pt_3d_ptStartEnd[1].y+1 ;
				pt1_3d_bottomright.x = pt_3d_ptStartEnd[1].x+1 ;
				pt1_3d_bottomright.y = pt_3d_ptStartEnd[1].y-1 ;
				AcDbHandle right_ent_Handle ;
				BOOL bFindRight = FALSE ;
				bFindRight = this->FindBoxBetweenCable(pt1_3d_topleft,pt1_3d_bottomright,right_ent_Handle) ;
				if (!bFindRight)
				{
					pt1_3d_topleft.x = pt_3d_ptStartEnd[1].x-4 ;
					pt1_3d_topleft.y = pt_3d_ptStartEnd[1].y+3.5 ;
					pt1_3d_bottomright.x = pt_3d_ptStartEnd[1].x+6 ;
					pt1_3d_bottomright.y = pt_3d_ptStartEnd[1].y-3.5 ;
					bFindRight = this->FindBoxBetweenCable(pt1_3d_topleft,pt1_3d_bottomright,right_ent_Handle) ;
				}
				if (!bFindRight)
				{
					strError.Format(_T("%s\nRight near (%.3f-%.3f) no Box found"), strError, pt_3d_ptStartEnd[1].x, pt_3d_ptStartEnd[1].y );
					ads_printf(strError) ;
					//
					Acad::ErrorStatus es = pEnt->upgradeOpen() ;
					if(Acad::eOk == es)
					{
						pEnt->setColorIndex(3) ;
						pEnt->downgradeOpen() ;
					}
					pEnt->close() ;
					continue ;
				}
				iRightEntHandle_Low = right_ent_Handle ;					
				//end polyline右边端点周边要搜索区域		


				//搜索该连接线上面的电缆字符串类似100-12(6)				
				for (int i=0; i<iVertNum-1; i++)
				{
					AcGeLineSeg3d linSeg_3d ;					
					pPolyLine->getLineSegAt(i, linSeg_3d) ;
					AcGePoint3d pt1 = linSeg_3d.startPoint() ;
					AcGePoint3d pt2 = linSeg_3d.endPoint() ;
					AcGeLineSeg3d lin(pt1,pt2) ;
					
					if (this->FindCableStrInLine(lin, strCableStr, pt_3d_text))
					{
						break ;
					}					
				}
				if (strCableStr.IsEmpty())
				{
					//
					Acad::ErrorStatus es = pEnt->upgradeOpen() ;
					if(Acad::eOk == es)
					{
						pEnt->setColorIndex(1) ;
						pEnt->downgradeOpen() ;
					}
					strError.Format(_T("%s\nPolyline (%.3f-%.3f)(%.3f-%.3f) no cablestring found,It's color has been set red"), strError, pt_3d_ptStartEnd[0].x, pt_3d_ptStartEnd[0].y, pt_3d_ptStartEnd[1].x, pt_3d_ptStartEnd[1].y);
					ads_printf(strError) ;					
				}

				
				try
				{
				strSql.Format(_T("insert into connect_cable(cablestring,pt_x,pt_y,left_ent_handle,right_ent_handle) \
					values('%s',%f,%f,%d,%d)"),\
					strCableStr,pt_3d_text.x,pt_3d_text.y,iLeftEntHandle_Low, iRightEntHandle_Low) ;
				ExecSqlString(strSql) ;
				}
				catch (_com_error& e)
				{
					CString sBuff = CBlkUtility::GetErrorDescription(e) ;
					AfxMessageBox(sBuff) ;	
				}
				catch (...)
				{
					AfxMessageBox(_T("unknown error in KZ-ReadEntToDB")) ;					
				}

			}
			pEnt->close() ;
		}
		
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return bRet ;
}


BOOL CCableNetForJY::FindBoxBetweenCable(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright, AcDbHandle &entHandle)
{
	BOOL bRet = FALSE ;

	CUserSystem mysys ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			//if (_tcscmp(pEnt->isA()->name(), _T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pRef->position() ;
				//只处理规定区域；
				if((textp_3d.x>pt_3d_topleft.x && textp_3d.x<pt_3d_bottomright.x) && (textp_3d.y>pt_3d_bottomright.y && textp_3d.y<pt_3d_topleft.y))
				{
					//得到块名					
					CString strBlockName ;
					mysys.GetBlockName(pRef, strBlockName) ;					
					//strBlockName.MakeLower() ;
					
					strBlockName.TrimLeft();
					strBlockName.TrimRight() ;
					if (!strBlockName.IsEmpty())
					{
						int iFindLoc = strBlockName.Find(_T("_")) ;
						if (iFindLoc!=-1)
						{
							CString strBlockType = strBlockName.Left(iFindLoc) ;
							strBlockType.MakeUpper() ;
							//20110512:XHJ和BOX块分离后去掉此处strBlockType==_T("XHJ"),加上信号楼端子strBlockType==_T("DZ")
							//if (strBlockType==_T("BOX")||strBlockType==_T("XHJ")||strBlockType==_T("ZZJ")||strBlockType==_T("FJD")||strBlockType==_T("YDQ"))
							if (strBlockType==_T("BOX")||strBlockType==_T("ZZJ")||strBlockType==_T("FJD")||strBlockType==_T("YDQ")||strBlockType==_T("DZ"))
							{
								pEnt->getAcDbHandle(entHandle) ; 
								bRet = TRUE ;						
							}
						}						
					}
					
				}				
			}
			pEnt->close() ;
			if (bRet)
			{
				break ;
			}
		}
		
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	return bRet ;
}

BOOL CCableNetForJY::FindCableStrInLine(const AcGeLineSeg3d lin, CString &strCableStr, AcGePoint3d &pt_3d_text)
{
	BOOL bRet = FALSE ;

	CUserSystem mysys ;
	mysys.m_pDb = this->m_pCurDb ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText * pText = NULL ;
				pText = (AcDbText *)pEnt;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pText->position() ;
				double dLen = mysys.GetDistance(lin, textp_3d) ;
				BOOL bXYInArea = (textp_3d.x>min(lin.startPoint().x,lin.endPoint().x) &&
								 textp_3d.x<max(lin.startPoint().x,lin.endPoint().x)) ||
								 (textp_3d.y>min(lin.startPoint().y,lin.endPoint().y) &&
								 textp_3d.y<max(lin.startPoint().y,lin.endPoint().y)) ;
												
				if (bXYInArea && dLen<2)
				{
					CString strText ;
					strText = pText->textString() ;
					strText.TrimLeft() ;
					strText.TrimRight() ;
					if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*-([4-9]|[1-4]\\d)[A-Z]*\\(([1-9]|[1-2]\\d)\\)$"),strText))
					{
						strCableStr = strText ;
						pt_3d_text = textp_3d ;
						bRet = TRUE ;						
					}
				}
				
			}
			pEnt->close() ;
			if (bRet)
			{
				break ;
			}
		}
		
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	return bRet ;
}

void CCableNetForJY::DoAll()
{

}

//递归出一条链路
void CCableNetForJY::FindOneRoute(long iCurEntHandle, CLongArray &iArrayEntHandle)
{
	iArrayEntHandle.Add(iCurEntHandle) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		
		//标示出起始(没有前一个链接的)的ent
		strSql.Format(_T("SELECT * FROM cablenet_ent WHERE handle_low=%d and next_ent_handle_low>0"), iCurEntHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				long iHandleTmp = 0 ;
				cSet.GetFieldValue(_T("next_ent_handle_low"), iHandleTmp) ;
				if (CBlkUtility::FindLongInArray(iArrayEntHandle, iHandleTmp)!=-1)
				{
					CString strErrMsg ;
					
					double dX, dY ;
					cSet.GetFieldValue(_T("pt_x"), dX) ;
					cSet.GetFieldValue(_T("pt_y"), dY) ;

					strErrMsg.Format(_T(" Error in FindOneRoute():%d(%.3f,%.3f) and %d has next_ent_handle each other"), iCurEntHandle, dX, dY, iHandleTmp) ;
					AfxMessageBox(strErrMsg) ;
					acutPrintf(_T("\n%s"), strErrMsg) ;		
					cSet.Close() ;
					return ;
				}
				this->FindOneRoute(iHandleTmp, iArrayEntHandle) ;		
			}

			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FindOneRoute")) ;
	} 

}

//计算填充完整数据
BOOL CCableNetForJY::FillDB()
{
	BOOL bRet = TRUE ;
	CString strErrMsg ;
	try	
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin 设置cablenet_ent表的left_ent_handle和right_ent_handle
		strSql.Format(_T("select * from cablenet_ent")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				long iHandle = 0 ;
				cSet.GetFieldValue(_T("handle_low"), iHandle) ;

				CString strColName[] = {_T("right_ent_handle"), _T("left_ent_handle")} ;
				for (int i=0; i<2; i++)
				{
					strSql.Format(_T("select * from connect_cable where %s=%d"), strColName[i], iHandle) ;
					CADORecordset cSetTmp ;
					if (cSetTmp.IsOpen())
					{
						cSetTmp.Close() ;
					}
					if(cSetTmp.Open(g_PtrCon, strSql))
					{
						if (cSetTmp.GetRecordCount()>0)
						{
							cSetTmp.MoveFirst() ;

							long iEntHandle = 0 ;				
							cSetTmp.GetFieldValue(strColName[(i+1)%2], iEntHandle) ;

							cSet.Edit() ;
							cSet.SetFieldValue(strColName[(i+1)%2], iEntHandle) ;
							cSet.Update() ;
						}
						cSetTmp.Close() ;
					}
				}		

				cSet.MoveNext() ;
			}
		}
		cSet.Close() ;
		//end 设置cablenet_ent表的left_ent_handle和right_ent_handle

		//begin 设置cablenet_ent表的next_ent_handle_low, is_start, left_or_right
		CString strColName[] = {_T("left_ent_handle"), _T("right_ent_handle")} ;
		for (int i=0; i<2; i++)
		{
			strSql.Format(_T("select * from cablenet_ent where %s=0 and boxtype<>'XHL'"), strColName[i]) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if(cSet.Open(g_PtrCon, strSql))
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				while (!cSet.IsEOF())
				{
					long iHandle = 0 ;
					cSet.GetFieldValue(_T("handle_low"), iHandle) ;	

					this->FindNextEnt(iHandle, (i+1)%2) ;

					cSet.Edit() ;
					cSet.SetFieldValue(_T("is_start"),1) ;
					cSet.Update() ;

					cSet.MoveNext() ;
				}
				cSet.Close() ;
			}
		}

		//here 20100325
		//end 设置cablenet_ent表的next_ent_handle_low
		

		acedSetStatusBarProgressMeterPos(40) ;

	

		//begin 设置cablenet_ent表的branch
		strSql.Format(_T("select * from cablenet_ent")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF()) 
			{
				long iHandleLow ;			
				cSet.GetFieldValue(_T("handle_low"), iHandleLow) ;

				int iBranchNum = 0 ;
				//here 20100325-2228
				this->FindBranch(iHandleLow, iBranchNum) ;
				cSet.Edit() ;
				cSet.SetFieldValue(_T("branch_num"), iBranchNum) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

		//end 设置cablenet_ent表的branch

		acedSetStatusBarProgressMeterPos(50) ;
	

		//begin 设置cablenet_ent表的belong_xhl_handle_low
		strSql.Format(_T("select * from cablenet_ent where is_start=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iRsCount = cSet.GetRecordCount() ;
			BOOL bSetProgressMod = iRsCount>50?TRUE:FALSE ;
			int iSumTmp = 0 ;
			//BOOL bHaveSetXHLRightOrLeft = FALSE ;
			while (!cSet.IsEOF()) 
			{
				long iHandleLow ;
				cSet.GetFieldValue(_T("handle_low"), iHandleLow) ;	

				CLongArray iArrayEntHandle ;
				//递归查找以ihandleLow（实体的handle）为起始（以信号楼为结束）的链路上所有实体
				this->FindOneRoute(iHandleLow, iArrayEntHandle) ;
				int iEntSum = iArrayEntHandle.GetSize() ;
				int iXhlEntHandle = iArrayEntHandle.GetAt(iEntSum-1) ;
				for (int i=0; i<iEntSum; i++)
				{
					long iHandleTmp = iArrayEntHandle.GetAt(i) ;
					strSql.Format(_T("update cablenet_ent set belong_xhl_handle_low=%d where handle_low=%d"), iXhlEntHandle, iHandleTmp) ;
					ExecSqlString(strSql) ;	
					if (i==(iEntSum-1))
					{
						int iLeftOrRight = 0 ;
						cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
						strSql.Format(_T("update cablenet_ent set left_or_right=%d where handle_low=%d"), iLeftOrRight, iHandleTmp) ;
						ExecSqlString(strSql) ;
						//bHaveSetXHLRightOrLeft = TRUE ;
					}
				}
				//以此实体为起始的链路上的实体个数
				cSet.Edit() ;
				cSet.SetFieldValue(_T("route_num"), iEntSum) ;
				cSet.Update() ;

				iSumTmp ++ ;

				if (bSetProgressMod)
				{
					acedSetStatusBarProgressMeterPos(50+iSumTmp/2) ;
				}
				else
				{
					acedSetStatusBarProgressMeterPos(50+(50/iRsCount)*iSumTmp) ;
				}


				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end 设置cablenet_ent表的belong_xhl_handle_low

		//begin 计算某些XB箱的设备cablenet_ent:devname（针对JH线：没有设备的是JZ JL信号机使用的,20110504增加CZA/CZB）
		strSql.Format(_T("select * from cablenet_ent where left(devtype,3)='JZB' or left(devtype,3)='JLB' or left(devtype,3)='CZB' ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				long iNextEntHandle = 0 ;
				CString strDevName = _T("") ;
				CString strDevType = _T("") ;
				cSet.GetFieldValue(_T("next_ent_handle_low"), iNextEntHandle) ;
				cSet.GetFieldValue(_T("devname"), strDevName) ;
				cSet.GetFieldValue(_T("devtype"), strDevType) ;

				//begin 设置下一个盒子的设备类型和名称
				strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iNextEntHandle) ;
				CADORecordset cSetNextEnt ;
				if (cSetNextEnt.IsOpen())
				{
					cSetNextEnt.Close() ;
				}
				cSetNextEnt.Open(g_PtrCon, strSql) ;
				if (cSetNextEnt.GetRecordCount()>0)
				{
					cSetNextEnt.MoveFirst() ;

					CString strTmp = _T("") ;				
					cSetNextEnt.Edit() ;
					strTmp.Format(_T("%s-A"), strDevName) ;
					cSetNextEnt.SetFieldValue(_T("devname"), strTmp) ;
					strTmp.Format(_T("%sA%s"), strDevType.Left(2), strDevType.Mid(3)) ;
					cSetNextEnt.SetFieldValue(_T("devtype"), strTmp) ;
					cSetNextEnt.Update() ;
				}
				cSetNextEnt.Close() ;
				//end 设置下一个盒子的设备类型和名称

				cSet.Edit() ;
				strDevName+=_T("-B") ;
				cSet.SetFieldValue(_T("devname"), strDevName) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;

		}

		//end 计算某些XB箱的设备cablenet_ent:devname（针对JH线：没有设备的是JZ信号机使用的,20110504增加CZA/CZB）
		

		//begin 对ZD6 ZY4等型转辙机确定其使用芯线
		//modify:20111026,增加ZYJ6-7
		//modify:20121212:ZY4+ZD6双动组合（谢欣），重写此部分代码
		strSql.Format(_T("select * from cablenet_ent where is_start=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				long iStartHandle = 0;
				CString strDevFor ;
				cSet.GetFieldValue(_T("dev_for"), strDevFor) ;
				cSet.GetFieldValue(_T("handle_low"), iStartHandle) ;

				if (strDevFor==_T("SH5")||strDevFor==_T("SH6")) //第一个节点是锁闭器，舍弃，从下一个节点开始算加芯
				{
					cSet.GetFieldValue(_T("next_ent_handle_low"), iStartHandle) ;
					//若起点是SH5/6，则该起点不参与加芯计算，其类型为ZZJ_0_0_5
					CString strDevTypeSH ;
					cSet.GetFieldValue(_T("devtype"), strDevTypeSH) ;
					strDevTypeSH.Append(_T("_5_1")) ;
					cSet.Edit() ;
					cSet.SetFieldValue(_T("devtype"), strDevTypeSH) ;
					cSet.Update() ;
				}
				CLongArray iArrayRouteHandle ;
				this->FindOneRoute(iStartHandle, iArrayRouteHandle) ;

				CString strHandleStr = _T("") ;
				//径路最后一个是信号楼，去掉
				for (int iHandleIndex=0; iHandleIndex<iArrayRouteHandle.GetSize()-1; iHandleIndex++)
				{					
					CString strHandleTmp ;
					strHandleTmp.Format(_T("%d,"), iArrayRouteHandle.GetAt(iHandleIndex)) ;
					strHandleStr+=strHandleTmp ;					
				}
				strHandleStr.TrimRight(_T(",")) ;

				
				strSql.Format(_T("select * from cablenet_ent where handle_low in(%s) "), strHandleStr) ;
				CADORecordset cSetTmp ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				long iDistanceToXhl = 0 ;
				if(cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						//长度
						cSetTmp.MoveFirst() ;

						while (!cSetTmp.IsEOF())
						{
							CString strCableStr ;
							cSetTmp.GetFieldValue(_T("cablestring"), strCableStr) ;
							if (!strCableStr.IsEmpty())
							{
								int iFindLoc = strCableStr.Find(_T("-")) ;
								if (iFindLoc!=-1)
								{
									long iLenTmp = 0 ;
									iLenTmp = _tstoi(strCableStr.Left(iFindLoc)) ;
									iDistanceToXhl+=iLenTmp ;								
								}
							}
							else
							{
								CString strDevNameTmp ;
								cSetTmp.GetFieldValue(_T("devname"), strDevNameTmp) ;
								strErrMsg.Format(_T("注意：%s缺少电缆信息！"), strDevNameTmp) ;
								AfxMessageBox(strErrMsg) ;
							}

							cSetTmp.MoveNext() ;
						}

						//为以iStartHandle为起头的一条链路上的所有转辙机节点确定类型
						cSetTmp.MoveFirst() ;
						while(!cSetTmp.IsEOF())
						{
							CString strDevType, strDevFor2 ;
							cSetTmp.GetFieldValue(_T("devtype"), strDevType) ;
							cSetTmp.GetFieldValue(_T("dev_for"), strDevFor2) ;
							CStringArray strArrayDevType ;
							CBlkUtility::DivideString(strDevType, _T("_"), strArrayDevType) ;
							if (strArrayDevType.GetSize()>2&&strArrayDevType.GetAt(0)==_T("ZZJ"))
							{
								long iCoreNum = 0 ;
								strSql.Format(_T("SELECT top 1 * FROM define_zzj_plus where act_num=%s and act_index=%s and cable_len>=%d and dev_for='%s' "), strArrayDevType.GetAt(1), strArrayDevType.GetAt(2), iDistanceToXhl, strDevFor2) ;
								CADORecordset cSetTmp2 ;
								if (cSetTmp2.IsOpen())
								{
									cSetTmp2.Close() ;
								}
								if (cSetTmp2.Open(g_PtrCon, strSql))
								{
									if (cSetTmp2.GetRecordCount()>0)
									{
										cSetTmp2.MoveFirst() ;

										cSetTmp2.GetFieldValue(_T("core_num"), iCoreNum) ;
									}
									cSetTmp2.Close() ;
								}

								CString strCoreNumTmp ;
								strCoreNumTmp.Format(_T("_%d"), iCoreNum) ;
								strDevType.Append(strCoreNumTmp) ;

								//如果本节点是ZY4/SH5，往后移动一位，看下一节点是否是SH5/ZY4,是则该节点类型多2芯B5/B6
								int nId = 0 ;
								cSetTmp.GetFieldValue(_T("id"), nId) ;
								//if (strDevFor2==_T("ZY4") || strDevFor2==_T("SH5"))
								if (strDevFor2==_T("SH5"))
								{
									strSql.Format(_T("select * from cablenet_ent where handle_low=(select next_ent_handle_low from cablenet_ent where id =%d)"), nId) ;
									CADORecordset cSetTmp3 ;
									if (cSetTmp3.IsOpen())
									{
										cSetTmp3.Close() ;
									}
									if (cSetTmp3.Open(g_PtrCon, strSql))
									{
										if (cSetTmp3.GetRecordCount()>0)
										{
											cSetTmp3.MoveFirst() ;

											CString strDevForNext ;
											cSetTmp3.GetFieldValue(_T("dev_for"), strDevForNext) ;
											//if ((strDevFor2==_T("ZY4")&&strDevForNext == _T("SH5")) || (strDevFor2==_T("SH5")&&strDevForNext == _T("ZY4")))
											if (strDevForNext == _T("ZY4"))
											{
												strDevType.Append(_T("_1")) ;												
											}
										}

										cSetTmp3.Close() ;
									}

									/*
									cSetTmp.MoveNext() ; 
									if (!cSetTmp.IsEOF())
									{
										CString strDevForNext ;
										cSetTmp.GetFieldValue(_T("dev_for"), strDevForNext) ;
										if (strDevForNext == _T("SH5"))
										{
											strDevType.Append(_T("_1")) ;
										}
									}
									cSetTmp.MovePrevious() ;
									*/
								}
								else if (strDevFor2==_T("SH5"))
								{
									CString strSH5Name, strZY4Name ;

									cSetTmp.MoveNext() ; 
									if (!cSetTmp.IsEOF())
									{
										CString strDevForNext ;
										cSetTmp.GetFieldValue(_T("dev_for"), strDevForNext) ;
										if (strDevForNext == _T("ZY4"))
										{
											strDevType.Append(_T("_1")) ;

                                            cSetTmp.GetFieldValue(_T("devname"), strZY4Name) ;											
										}
									}
									cSetTmp.MovePrevious() ;

									if (strZY4Name.IsEmpty())
									{
										cSetTmp.MovePrevious() ;
										
										if (!cSetTmp.IsEOF())
										{
											CString strDevForNext ;
											cSetTmp.GetFieldValue(_T("dev_for"), strDevForNext) ;
											if (strDevForNext == _T("ZY4"))
											{
												cSetTmp.GetFieldValue(_T("devname"), strZY4Name) ;											
											}
										}
										
										cSetTmp.MoveNext() ;
									}

									/*
									strSH5Name.Format(_T("%s-SH5"), strZY4Name) ;

									cSetTmp.Edit() ;
									cSetTmp.SetFieldValue(_T("devname"), strSH5Name) ;									
									cSetTmp.Update() ;	
									*/
								}

								cSetTmp.Edit() ;
								cSetTmp.SetFieldValue(_T("devtype"), strDevType) ;
								cSetTmp.SetFieldValue(_T("distance_to_xhl"), iDistanceToXhl) ;
								cSetTmp.Update() ;
							}

                            cSetTmp.MoveNext() ;
						}
					}
					cSetTmp.Close() ;
				}

				cSet.MoveNext() ;

			}

			cSet.Close() ;
		}

		//end 对ZD6 ZY4等型转辙机确定其使用芯线

		strSql.Format(_T("SELECT * FROM cablenet_ent where boxtype='XHL'")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					long nXhlHandle = 0 ;
					cSet.GetFieldValue(_T("belong_xhl_handle_low"), nXhlHandle) ;
					cSet.Edit() ;
					cSet.SetFieldValue(_T("branch_index"), _T("1")) ;
					cSet.Update() ;
                    this->SetBoxBranchIndex(nXhlHandle, _T("1")) ;					
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
		

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillDB")) ;
	}  

	return bRet ;
}

//计算配线,一个如楼端一张图
BOOL CCableNetForJY::ComputeCoreAndDraw()
{
	BOOL bRet = TRUE ;
	CString strErrMsg ;

	CUserSystem mysys ;
	//mysys.m_pDb = this->m_pCurDb ;

	try
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin 一个信号楼ent产生一张图纸，循环产生所有图纸
		//////////////////////////////////////////////////////////////////////////

		strSql.Format(_T("SELECT distinct belong_xhl_handle_low FROM cablenet_ent")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		BOOL bOpenSuccess = cSet.Open(g_PtrCon, strSql) ;
		if (!bOpenSuccess)
		{
			strErrMsg.Format(_T("coun't open %s"), strSql) ;
			AfxMessageBox(strErrMsg) ;
			return FALSE ;
		}
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		int iRsCount = cSet.GetRecordCount() ;

		CString strBarMsg ;
		strBarMsg.Format(_T("电缆配线:%d张"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;

		int iIndex = 0 ;
		double dCurTotalYForOneDwg = 0 ; //绘制到一张图纸时，记录每个块绘制时的Y坐标		

		while (!cSet.IsEOF())
		{
			//先清空表
			CDBUtility::ClearTable(_T("cable_core_draw")) ;

			acedSetStatusBarProgressMeterPos(iIndex) ;			

			long iXHL_handle_low = 0 ;			
			cSet.GetFieldValue(_T("belong_xhl_handle_low"), iXHL_handle_low) ;			

			//begin 遍历以iXHL_handle_low为终点的所有起点，然后以该起点往信号楼方向走一遍，计算芯线数据
			//strSql.Format(_T("select * from cablenet_ent where is_start=1 and belong_xhl_handle_low=%d order by pt_y desc"), iXHL_handle_low) ;
			strSql.Format(_T("select * from cablenet_ent where is_start=1 and belong_xhl_handle_low=%d order by branch_index"), iXHL_handle_low) ; //20130420:增加branch_index列，用以判断起始box绘制的顺序（y方向）
			CADORecordset cSet_AllStart ;
			if (cSet_AllStart.Open(g_PtrCon, strSql))
			{
				if (!cSet_AllStart.IsBOF())
				{
					cSet_AllStart.MoveFirst() ;
				}
				//每张配线图的Y坐标从0开始
				double dCurTotalY = 0 ;
				int iAddYMode = 0 ; //此次Y的增加是增加芯线/0，还是增加底边框/1
				BOOL bIsFirstRoute = TRUE ;

				//被遍历的次数
				long iScan_Time = -1 ;

				while (!cSet_AllStart.IsEOF())
				{
					if (bIsFirstRoute)
					{
						dCurTotalY-=m_dCellHight*2 ;
						bIsFirstRoute = FALSE ;
					}
					else
					{
						dCurTotalY-=m_dCellHight*3 ;
					}
					double dCurTitleY = dCurTotalY ;

					long iStartHandle_Low = 0 ;
					int iLeftORight = -1 ;		
					cSet_AllStart.GetFieldValue(_T("handle_low"), iStartHandle_Low) ;				
					cSet_AllStart.GetFieldValue(_T("left_or_right"), iLeftORight) ;				

					CLongArray iArrayHandle_OneRoute ;
					this->FindOneRoute(iStartHandle_Low, iArrayHandle_OneRoute) ;
					double dCurX = 0 ; //在每条径路（从起始到信号楼）遍历过程中的x坐标
					double dPlusX = (iLeftORight==1?1:-1) ;//左边从信号楼往左x坐标是减少，右边反之
					int iEntNum = iArrayHandle_OneRoute.GetSize() ;
					//前一个devname
					CString strPreDevName = _T("") ;
					//begin for 遍历一条径路中的每一个ent（box、hz）计算其使用电缆芯线
					//
					CString strPassedHandle = _T("") ; //已经走过的链路节点的handle
					for (int i=0; i<iEntNum; i++)
					{
						long iHandleTmp = iArrayHandle_OneRoute.GetAt(i) ;
						strPassedHandle.AppendFormat(_T("%d,"), iHandleTmp) ;

						strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iHandleTmp) ;
						CADORecordset cSetOneEntInRoute ;
						if (cSetOneEntInRoute.IsOpen())
						{
							cSetOneEntInRoute.Close() ;
						}
						cSetOneEntInRoute.Open(g_PtrCon, strSql) ;
						cSetOneEntInRoute.MoveFirst() ;

						CString strDevType = _T("") ;
						CString strDevName = _T("") ;
						CString strDevFor = _T("") ;

						long iBranch_num = 0 ;
						cSetOneEntInRoute.GetFieldValue(_T("scan_time"), iScan_Time) ;
						cSetOneEntInRoute.GetFieldValue(_T("devtype"), strDevType) ;
						cSetOneEntInRoute.GetFieldValue(_T("devname"), strDevName) ;
						cSetOneEntInRoute.GetFieldValue(_T("dev_for"), strDevFor) ;
						cSetOneEntInRoute.GetFieldValue(_T("branch_num"), iBranch_num) ;

						if (strDevType.Left(7)==_T("ZZJ_2_1")) //modified 20110613:双动中的第1动，其devname要将2个道岔名合成一个类似47/49的形式
						{						
							//找出双动的第2动
							CString strSecActName ;
							strPassedHandle.TrimRight(_T(",")) ;
							strSql.Format(_T("select * from cablenet_ent where left(devtype,7)='ZZJ_2_2' and handle_low in(%s)"), strPassedHandle) ;
							CADORecordset cSetFindSecAct ;
							if (cSetFindSecAct.Open(g_PtrCon, strSql))
							{
								if(cSetFindSecAct.GetRecordCount()==1)
								{
									cSetFindSecAct.MoveFirst() ;
									cSetFindSecAct.GetFieldValue(_T("devname"), strSecActName) ;
								}
								else
								{
									CString strMsgFindSecAct ;
									strMsgFindSecAct.Format(_T("计算电路配线时：查找%s的第2动时，结果个数不是1"), strDevName) ;
									AfxMessageBox(strMsgFindSecAct) ;
								}

								cSetFindSecAct.Close() ;
							}
							strDevName.Insert(0, _T("/")) ;
							strDevName.Insert(0, strSecActName) ;

							strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and left(core_name,2)='MH'"), iArrayHandle_OneRoute.GetAt(i-1)) ;
							CADORecordset cSetFoundExist ;
							if (cSetFoundExist.IsOpen())
							{
								cSetFoundExist.Close() ;
							}
							if(cSetFoundExist.Open(g_PtrCon, strSql))
							{
								if (cSetFoundExist.GetRecordCount()>0)
								{
									cSetFoundExist.MoveFirst() ;
									while (!cSetFoundExist.IsEOF())
									{
										CString strCoreNameTmp ;
										cSetFoundExist.GetFieldValue(_T("core_name"), strCoreNameTmp) ;
										strCoreNameTmp.Replace(_T("MH"), _T("X4")) ;

										cSetFoundExist.Edit() ;
										cSetFoundExist.SetFieldValue(_T("dev_name"), strDevName) ;
										cSetFoundExist.SetFieldValue(_T("core_name"), strCoreNameTmp) ;
										cSetFoundExist.Update() ;

										cSetFoundExist.MoveNext() ;
									}
								}
								cSetFoundExist.Close() ;
							}
						}

						//begin if 没有被遍历过，第一次遍历要设置title，并为此盒对应设备产生新芯线,若不是起始，则还要加上延续芯线
						if (iScan_Time==0)
						{
							//title的坐标
							double dTitleX = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
							double dTitleY = dCurTitleY ;

							cSetOneEntInRoute.Edit() ;
							cSetOneEntInRoute.SetFieldValue(_T("px_title_x"), dTitleX) ;
							cSetOneEntInRoute.SetFieldValue(_T("px_title_y"), dTitleY) ;
							cSetOneEntInRoute.Update() ;

							//盒子有对应设备，不是分向盒，则必定有引出的电缆
							if (strDevType!="")
							{
								//20110504:define_dev表中dev_type由类似JZA变为类似JZA_5_0_JH

								// 							int iFindMark = strDevType.Find(_T("_")) ;
								// 							if (iFindMark!=-1)
								// 							{
								// 								strDevType = strDevType.Left(iFindMark) ;
								// 							}
								CADORecordset cSetGetDev/*, cSetGetCoredetail*/ ;
								//查表获取该类型设备使用的芯线

								strSql.Format(_T("select * from define_dev where dev_type='%s' and dev_for='%s'"), strDevType, strDevFor) ;

								CString strDevTypeTmp ;
								strDevTypeTmp.Format(_T("'%s'"), strDevType) ;
								CString strDevNameTmp[2] = {_T(""), _T("")} ;
								strDevNameTmp[0] = strDevName ;

								if (strDevType.Right(4)==_T("25_0"))
								{
									if (strDevType.Find(_T("_"))==2)
									{									
										strSql.Format(_T("select * from define_dev where dev_type='%s_25_0' and dev_for='%s' union all select * from define_dev where dev_type='%s_25_0' and dev_for='%s'"), strDevType.Left(1), strDevFor, strDevType.Mid(1,1), strDevFor) ;
										strDevNameTmp[0] = strDevName.Left(strDevName.Find(_T(" "))) ;
										strDevNameTmp[1] = strDevName.Mid(strDevName.Find(_T(" "))+1) ;
									}
								}

								if (cSetGetDev.IsOpen())
								{
									cSetGetDev.Close() ;
								}
								if(cSetGetDev.Open(g_PtrCon, strSql))
								{
									if (cSetGetDev.GetRecordCount()>0)
									{
										cSetGetDev.MoveFirst() ;

										int iDevIndex = 0 ;
										while (!cSetGetDev.IsEOF())
										{
											int iDevId = -1 ;
											cSetGetDev.GetFieldValue(_T("id"), iDevId) ;
											CADORecordset cSetGetCoreDetail ;
											strSql.Format(_T("select * from define_dev_use_cablecore where dev_id=%d"), iDevId) ;
											if (cSetGetCoreDetail.IsOpen())
											{
												cSetGetCoreDetail.Close() ;
											}
											if(cSetGetCoreDetail.Open(g_PtrCon, strSql))
											{
												if (cSetGetCoreDetail.GetRecordCount()>0)
												{
													cSetGetCoreDetail.MoveFirst() ;
													while (!cSetGetCoreDetail.IsEOF())
													{
														CString strCoreName = _T("") ; //芯线名称，例：DH DHH
														CString strCoreSn = _T("") ; //芯线编号,例如：D1-1 II2-3
														int iIsInside = 0 ;			//内部不需要引出的芯线，0/非内部，1/内部
														int iIsShare = 0 ;			//是不是共用线，比如DH DHH 0/不共用，1/共用
														CString strStartNum = _T("") ;			//芯线起点使用的盒子的端子
														CString strEndNum = _T("") ;			//芯线末点使用的盒子的端子
														int iIsVirtual = 0 ;	//0/非虚拟，1/虚拟芯，占位用
														double dCurCoreX = dTitleX ;
														double dCurCoreY = dCurTotalY ;
														int iIsNew = 1 ;
														int iIsAdd = 0 ;

														cSetGetCoreDetail.GetFieldValue(_T("is_virtual"), iIsVirtual) ;
														cSetGetCoreDetail.GetFieldValue(_T("is_add"), iIsAdd) ;

														cSetGetCoreDetail.GetFieldValue(_T("core_name"), strCoreName) ;

														cSetGetCoreDetail.GetFieldValue(_T("is_share"), iIsShare) ;
														if (iIsShare>0 && i>0) //如果是共用线，且不是第一个ent，则要检查是否前面链接的盒子里面已有，若有则不再增加该芯线，进入下一芯线
														{
															long iHandlePre = iArrayHandle_OneRoute.GetAt(i-1) ;
															strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iHandlePre,strCoreName) ;
															CADORecordset cSetFindExistCore ;
															if(cSetFindExistCore.Open(g_PtrCon, strSql))
															{
																if (cSetFindExistCore.GetRecordCount()>0)
																{
																	cSetFindExistCore.Close() ;
																	cSetGetCoreDetail.MoveNext() ;
																	continue ;
																}
																cSetFindExistCore.Close() ;
															}								
															//here 20100326
														}

														cSetGetCoreDetail.GetFieldValue(_T("is_inside"), iIsInside) ;
														if (iIsInside==1) //如果是纯内部线，则将采用默认的端点和电缆芯
														{
															cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
															cSetGetCoreDetail.GetFieldValue(_T("end_num_default"), strEndNum) ;
															cSetGetCoreDetail.GetFieldValue(_T("cable_core_sn_default"), strCoreSn) ;											
														}
														else if (iIsInside==2)//如果是半纯内部线，则将采用默认的起点
														{
															cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
														}


														BOOL bFoundExist = FALSE ;
														if ((strDevType.Left(3)==_T("JZA")||strDevType.Left(3)==_T("JLA")||strDevType.Left(3)==_T("CZA"))&& i>0)//如果是JZA则基本可以确定是第2个盒子，加上i>0起检验作用
														{
															//对于已经存在的芯线，延续
															strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iArrayHandle_OneRoute.GetAt(i-1), strCoreName) ;
															CADORecordset cSetFoundExist ;
															if (cSetFoundExist.IsOpen())
															{
																cSetFoundExist.Close() ;
															}
															if(cSetFoundExist.Open(g_PtrCon, strSql))
															{
																if (cSetFoundExist.GetRecordCount()>0)
																{
																	cSetFoundExist.MoveFirst() ;
																	cSetFoundExist.GetFieldValue(_T("pt_y"), dCurCoreY) ;
																	iIsNew = 0 ;
																	bFoundExist = TRUE ;									
																}
																cSetFoundExist.Close() ;
															}

														}
														//modified by zhf:20110613
														else if (strDevType.Left(7)==_T("ZZJ_2_1") && strCoreName.Left(2)==_T("X4")) //对于双动道岔的1动X4芯线
														{
															strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iArrayHandle_OneRoute.GetAt(i-1), strCoreName) ;
															CADORecordset cSetFoundExist ;
															if (cSetFoundExist.IsOpen())
															{
																cSetFoundExist.Close() ;
															}
															if(cSetFoundExist.Open(g_PtrCon, strSql))
															{
																if (cSetFoundExist.GetRecordCount()>0)
																{
																	cSetFoundExist.MoveFirst() ;
																	cSetFoundExist.GetFieldValue(_T("pt_y"), dCurCoreY) ;
																	iIsNew = 0 ;
																	bFoundExist = TRUE ;									
																}
																cSetFoundExist.Close() ;
															}
														}
														if (!bFoundExist)
														{
															dCurTotalY-=m_dCellHight ;
															dCurCoreY = dCurTotalY ;
														}

														iAddYMode = 0 ;
														strSql.Format(_T("insert into cable_core_draw(dev_name, core_name, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_virtual, is_add) \
																		 values('%s','%s',%d, %d, %d, %d, %f,%f, '%s', '%s', '%s', %d, %d, %d)"), strDevNameTmp[iDevIndex], strCoreName , iHandleTmp, iIsNew, iIsInside, iIsShare, dCurCoreX, dCurCoreY, strCoreSn, strStartNum, strEndNum, iLeftORight, iIsVirtual, iIsAdd) ;
														ExecSqlString(strSql) ;

														cSetGetCoreDetail.MoveNext() ;
													}
												}

											}
											cSetGetCoreDetail.Close() ;

											cSetGetDev.MoveNext() ;
											iDevIndex++ ;
										}
									}
									cSetGetDev.Close() ;
								}

							}

						}
						//end if 没有被遍历过，第一次遍历要设置title，并为此盒对应设备产生新芯线,若不是起始，则还要加上延续芯线


						//除起始box外，其他都要延续前一个盒子的芯线
						if(i>0) 
						{
							long iHandlePre = iArrayHandle_OneRoute.GetAt(i-1) ;
							// 						if (iHandlePre==351)
							// 						{
							// 							int iiii = 0 ;
							// 						}
							strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_inside<>1"), iHandlePre) ;
							CADORecordset cSetPreBoxCore ;
							if(cSetPreBoxCore.Open(g_PtrCon, strSql))
							{
								if (cSetPreBoxCore.GetRecordCount()>0)
								{
									cSetPreBoxCore.MoveFirst() ;
									
									while (!cSetPreBoxCore.IsEOF())
									{
										CString strDevName, strCoreName, strCoreSn ;
										long iBelong_Handle = 0 ;						
										int iIsShare = 0 ;
										CString strStartNum = _T("") ;
										CString strEndNum = _T("") ;
										int iLeftOrRightTmp = -1 ;
										double dPt_x = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
										double dPt_y = 0 ;
										int iIsAdd = 0 ;

										cSetPreBoxCore.GetFieldValue(_T("dev_name"), strDevName) ;
										cSetPreBoxCore.GetFieldValue(_T("core_name"), strCoreName) ;
										cSetPreBoxCore.GetFieldValue(_T("belong_box_handle_low"), iBelong_Handle) ;	
										cSetPreBoxCore.GetFieldValue(_T("is_share"), iIsShare) ;
										cSetPreBoxCore.GetFieldValue(_T("pt_y"), dPt_y) ;
										cSetPreBoxCore.GetFieldValue(_T("core_sn"), strCoreSn) ;
										cSetPreBoxCore.GetFieldValue(_T("start_num"), strStartNum) ;
										cSetPreBoxCore.GetFieldValue(_T("end_num"), strEndNum) ;
										cSetPreBoxCore.GetFieldValue(_T("left_or_right"), iLeftOrRightTmp) ;
										cSetPreBoxCore.GetFieldValue(_T("is_add"), iIsAdd) ;

										//对于已经存在的共享芯线，不再延续
										strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s' and is_share>0"), iArrayHandle_OneRoute.GetAt(i), strCoreName) ;
										CADORecordset cSetHasShared ;
										if (cSetHasShared.IsOpen())
										{
											cSetHasShared.Close() ;
										}
										cSetHasShared.Open(g_PtrCon, strSql) ;
										if (cSetHasShared.GetRecordCount()>0)
										{
											cSetHasShared.Close() ;
											cSetPreBoxCore.MoveNext() ;
											continue ;
										}
										cSetHasShared.Close() ;

										//对于已经存在的芯线，不再延续，跟共享芯线不一样
										//比如:共享芯线是只要有DH,就不再延续了，此处是即使有X1了，但不是同一个设备的X1，则一样要延续
										strSql.Format(_T("select * from cable_core_draw where [dev_name]='%s' and [core_name]='%s' and belong_box_handle_low=%d"), strDevName, strCoreName, iArrayHandle_OneRoute.GetAt(i)) ;
										CADORecordset cSetHasThisCore ;
										if (cSetHasThisCore.IsOpen())
										{
											cSetHasThisCore.Close() ;
										}
										cSetHasThisCore.Open(g_PtrCon, strSql) ;
										if (cSetHasThisCore.GetRecordCount()>0)
										{
											cSetHasThisCore.Close() ;
											cSetPreBoxCore.MoveNext() ;
											continue ;
										}
										cSetHasThisCore.Close() ;

										strSql.Format(_T("insert into cable_core_draw(dev_name,core_name, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_add) \
														 values('%s','%s',%d, %d, %d, %d, %f,%f,'%s','%s', '%s', %d, %d)"), strDevName, strCoreName, iHandleTmp, 0, 0, iIsShare, dPt_x, dPt_y, strCoreSn, strStartNum, strEndNum, iLeftOrRightTmp, iIsAdd) ;
										ExecSqlString(strSql) ;

										cSetPreBoxCore.MoveNext() ;							
									}

								}
								cSetPreBoxCore.Close() ;
							}

						}

						//每遍历一次将scan_time加1
						iScan_Time++ ;
						//strSql.Format(_T("update cablenet_ent set scan_time=%d"), iScan_Time) ;	
						//ExecSqlString(strSql) ;
						cSetOneEntInRoute.Edit() ;					
						bool b3 = cSetOneEntInRoute.SetFieldValue(_T("scan_time"), iScan_Time) ;
						bool b1 = cSetOneEntInRoute.Update() ;			


						//若遍历次数和分支（branch）相同，则需要设置bottom_title的位置，单元格合拢
						if (iBranch_num==0 || iScan_Time==iBranch_num)
						{
							double dBottom_Title_x = (iEntNum-i-1)*dPlusX*m_dCellWidth ; 
							if (iAddYMode==0)
							{
								dCurTotalY-=m_dCellHight*2 ;
							}						
							iAddYMode = 1 ;
							cSetOneEntInRoute.Edit() ;
							cSetOneEntInRoute.SetFieldValue(_T("px_bottom_x"), dBottom_Title_x) ;
							cSetOneEntInRoute.SetFieldValue(_T("px_bottom_y"), dCurTotalY) ;
							cSetOneEntInRoute.Update() ;

						}
						cSetOneEntInRoute.Close() ;

						strPreDevName = strDevName ; //保留此devname以备下一个dev使用(在双动道岔的第1动时使用,20121220已弃用，因前一个节点不一定是第2动，可能是锁闭器)
					}
					//end for 遍历一条径路中的每一个ent（box、hz）计算其使用电缆芯线

					cSet_AllStart.MoveNext() ;
				}
				cSet_AllStart.Close() ;
			}	

			//end 遍历以iXHL_handle_low为终点的所有起点，然后以该起点往信号楼方向走一遍，计算芯线数据

			
			//begin 填充端子编号,芯线名称
			//////////////////////////////////////////////////////////////////////////
			CADORecordset cSetAllBox ;
			CString strFileName = _T("未知");

			strSql.Format(_T("select distinct belong_box_handle_low from cable_core_draw")) ;
			if(cSetAllBox.IsOpen())
			{
				cSetAllBox.Close() ;
			}
			if(cSetAllBox.Open(g_PtrCon, strSql))
			{
				if (cSetAllBox.GetRecordCount()>0)
				{
					cSetAllBox.MoveFirst() ;

					while(!cSetAllBox.IsEOF())
					{
						long iBoxHandle = 0 ;
						cSetAllBox.GetFieldValue(_T("belong_box_handle_low"), iBoxHandle) ;

						//	int iBoxDzSum = 0 ; //该盒子端子总数 注意查询盒子如果是xhl，没有结果
						CString strBoxType = _T("") ;
						CString strDevType = _T("") ;//20130412:谢欣，石太，devtype为F_25_0、J_25_0时，其内非本设备引出芯线端子从11开始排
						//begin 得到该盒子类型
						CADORecordset cSetGetBoxType ;
						strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
						if(cSetGetBoxType.IsOpen())
						{
							cSetGetBoxType.Close() ;
						}
						if(cSetGetBoxType.Open(g_PtrCon, strSql) )
						{
							if (cSetGetBoxType.GetRecordCount()>0)
							{
								cSetGetBoxType.MoveFirst() ;
								cSetGetBoxType.GetFieldValue(_T("boxtype"), strBoxType) ;
								cSetGetBoxType.GetFieldValue(_T("devtype"), strDevType) ;
								if (strBoxType==_T("XHL")) //盒子是信号楼，则跳出继续下次循环
								{
									cSetGetBoxType.GetFieldValue(_T("devname"), strFileName) ;

									cSetGetBoxType.Close() ;
									cSetAllBox.MoveNext() ;
									continue ;
								}
							}
							cSetGetBoxType.Close() ;
						}
						//end 得到该盒子类型

						//begin 得到盒子的box_type_id
						int iBoxTypeId = -1 ;
						CADORecordset cSetGetBoxTypeId ;
						//strBoxType.MakeUpper() ;
						strSql.Format(_T("select * from define_box where box_type='%s'"), strBoxType) ;
						if(cSetGetBoxTypeId.IsOpen())
						{
							cSetGetBoxTypeId.Close() ;
						}
						if(cSetGetBoxTypeId.Open(g_PtrCon, strSql))
						{
							//	int ii = cSetBoxDzSum.GetRecordCount() ;
							if(cSetGetBoxTypeId.GetRecordCount()>0)
							{
								cSetGetBoxTypeId.MoveFirst() ;
								cSetGetBoxTypeId.GetFieldValue(_T("id"), iBoxTypeId) ;
								//	cSetGetBoxTypeId.GetFieldValue(_T("box_dz_num"), iBoxDzSum) ;
							}
							cSetGetBoxTypeId.Close() ;
						}

						//end 得到盒子的box_type_id

						//先将该型号盒子的所有端子的is_used重置0
						//20130412:谢欣，石太，devtype为F_25_0、J_25_0时，其内非本设备引出芯线端子从11开始排
						// 在井径站xh-5配线中，HF-7的第42个端子没有置0成功，不知何故，改用update语句后又不立即更新（已解决，并不是置0不成功，是要等record关闭后才更新最后一个）

						CADORecordset cSetUpdateBoxDetail ;
						strSql.Format(_T("select * from define_box_detail where box_type_id=%d"), iBoxTypeId) ;					
						if(cSetUpdateBoxDetail.IsOpen())
						{
							cSetUpdateBoxDetail.Close() ;
						}
						if(cSetUpdateBoxDetail.Open(g_PtrCon, strSql))
						{
							if (!cSetUpdateBoxDetail.IsBOF())
							{
								cSetUpdateBoxDetail.MoveFirst() ;
							}
							while (!cSetUpdateBoxDetail.IsEOF())
							{
								cSetUpdateBoxDetail.Edit() ;
								cSetUpdateBoxDetail.SetFieldValue(_T("is_used"), 0) ;
								cSetUpdateBoxDetail.Update() ;
								cSetUpdateBoxDetail.MoveNext() ;
							}
							cSetUpdateBoxDetail.Close() ;
						}	

						if (strDevType==_T("F_25_0")) //送电端占用1、3端子，盒内其他从11开始用
						{
							strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and ((val(box_node)<11 and val(box_node)>3) or box_node='2')"), iBoxTypeId) ;
							ExecSqlString(strSql) ;
						}
						else if (strDevType==_T("J_25_0"))//受电端占用1、2端子，盒内其他从11开始用
						{
							strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and (val(box_node)<11 and val(box_node)>2) "), iBoxTypeId) ;
							ExecSqlString(strSql) ;
						}

						/*
						strSql.Format(_T("update define_box_detail set is_used=0 where box_type_id=%d"), iBoxTypeId) ;
						ExecSqlString(strSql) ;
						*/

						//here 20100401 

						//begin 处理所有共享芯线的端子
						//////////////////////////////////////////////////////////////////////////

						//begin 先将内部占用的端子标出
						//从前面盒子引入的端子
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in(select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_inside=1"), iBoxHandle) ;
						CADORecordset cSetFindDz ;
						if (cSetFindDz.IsOpen())
						{
							cSetFindDz.Close() ;
						}
						if(cSetFindDz.Open(g_PtrCon, strSql))
						{
							if (!cSetFindDz.IsBOF())
							{
								cSetFindDz.MoveFirst() ;
							}
							while (!cSetFindDz.IsEOF())
							{
								CString strDzNode = _T("") ;
								cSetFindDz.GetFieldValue(_T("end_num"), strDzNode) ;
								//strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
								//ExecSqlString(strSql) ;
								CADORecordset cSetTmp ;
								strSql.Format(_T("select * from define_box_detail where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
								if(cSetTmp.IsOpen())
								{
									cSetTmp.Close() ;
								}
								if(cSetTmp.Open(g_PtrCon, strSql))
								{
									if (!cSetTmp.IsBOF())
									{
										cSetTmp.MoveFirst() ;
									}
									while (!cSetTmp.IsEOF())
									{
										cSetTmp.Edit() ;
										cSetTmp.SetFieldValue(_T("is_used"), 1) ;
										cSetTmp.Update() ;
										cSetTmp.MoveNext() ;
									}
									cSetTmp.Close() ;
								}

								cSetFindDz.MoveNext() ;
							}
							cSetFindDz.Close() ;
						}
						//本盒子伸出的端子 
						//20130420:不包括共享芯线，即使共享芯线是内部或半内部的（谢欣，石太提供，共享芯线基本都从最后端子往前排）
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_inside>0 and is_share=0"), iBoxHandle) ;
						if (cSetFindDz.IsOpen())
						{
							cSetFindDz.Close() ;
						}
						if(cSetFindDz.Open(g_PtrCon, strSql))
						{
							if (!cSetFindDz.IsBOF())
							{
								cSetFindDz.MoveFirst() ;
							}
							while (!cSetFindDz.IsEOF())
							{
								CString strDzNode = _T("") ;
								cSetFindDz.GetFieldValue(_T("start_num"), strDzNode) ;
								//strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
								//ExecSqlString(strSql) ;
								CADORecordset cSetTmp ;
								strSql.Format(_T("select * from define_box_detail where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
								if(cSetTmp.IsOpen())
								{
									cSetTmp.Close() ;
								}
								if(cSetTmp.Open(g_PtrCon, strSql))
								{
									if (!cSetTmp.IsBOF())
									{
										cSetTmp.MoveFirst() ;
									}
									while (!cSetTmp.IsEOF())
									{
										cSetTmp.Edit() ;
										cSetTmp.SetFieldValue(_T("is_used"), 1) ;
										cSetTmp.Update() ;
										cSetTmp.MoveNext() ;
									}
									cSetTmp.Close() ;
								}

								cSetFindDz.MoveNext() ;
							}
							cSetFindDz.Close() ;

						}
						//end 先将内部占用的端子标出

						//begin 计算共享芯线的端子号
						//typedef CMap<CString, LPCSTR, CString ,LPCSTR> CMapStrToInt ;
						//CMapStrToInt mapCoreNameToDzNum ; //for examp:<"DHH", 42>
						CMapStringToString mapCoreNameToDzNum ;
						CADORecordset cSetShareType ; 
						//将共享芯甄选出来，排序按照is_share的大小来，分配端子号时按排序来（从最后端子开始）
						//union前面/后面的select 去掉了where.... and is_inside<>1（谢欣，石太，共享芯线不管内部与否，全部端子从最后往前排）
						strSql.Format(_T("select * from (select is_share, core_name from cable_core_draw where belong_box_handle_low \
										 in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share>0 \
										 union select is_share, core_name from cable_core_draw where belong_box_handle_low=%d and is_share>0 ) order by is_share"), iBoxHandle, iBoxHandle) ;
						if(cSetShareType.IsOpen())
						{
							cSetShareType.Close() ;
						}
						if(cSetShareType.Open(g_PtrCon, strSql))
						{
							if (cSetShareType.GetRecordCount()>0)
							{
								cSetShareType.MoveFirst() ;

								while (!cSetShareType.IsEOF())
								{
									CString strShareCoreName ;
									cSetShareType.GetFieldValue(_T("core_name"), strShareCoreName) ;

									int iFoundDz = -1 ;
									CString strCurDzNum = _T("") ;

									CADORecordset cSetDzDetail ;
									strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0 order by id desc"), iBoxTypeId) ;
									if (cSetDzDetail.IsOpen())
									{
										cSetDzDetail.Close() ;
									}
									cSetDzDetail.Open(g_PtrCon, strSql) ;
									if (cSetDzDetail.GetRecordCount()>0)
									{
										cSetDzDetail.MoveFirst() ;
										cSetDzDetail.GetFieldValue(_T("box_node"), strCurDzNum) ;

										cSetDzDetail.Edit() ;
										cSetDzDetail.SetFieldValue(_T("is_used"), 1) ;
										cSetDzDetail.Update() ;
										iFoundDz = 1 ;
									}
									else
									{
										iFoundDz = 0 ;
									}
									cSetDzDetail.Close() ;

									if (iFoundDz==1)
									{
										mapCoreNameToDzNum.SetAt(strShareCoreName, strCurDzNum) ;//for examp:<"DHH", 42>		
									}

									cSetShareType.MoveNext() ;
								}
							}
							cSetShareType.Close() ;
						}
						//end 计算共享芯线的端子号

						//begin 先给从前一个盒子引入的共用芯线的末端指定端子号
						CADORecordset cSetPreShare ;
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low \
										 in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share>0 and is_inside<>1"), iBoxHandle) ;
						if(cSetPreShare.IsOpen())
						{
							cSetPreShare.Close() ;
						}
						if(cSetPreShare.Open(g_PtrCon, strSql))
						{
							if (!cSetPreShare.IsBOF())
							{
								cSetPreShare.MoveFirst() ;
							}
							//begin 此段代码为解决1个端子上接4根以上的芯，权宜之计
							int iPreShareCount = cSetPreShare.GetRecordCount() ;
							int iPreShareIndex = 0 ;
							int iPreSharePlus = mapCoreNameToDzNum.GetCount() ;
							//end 此段代码为解决1个端子上接4根以上的芯，权宜之计
							while (!cSetPreShare.IsEOF())
							{
								CString strCoreName ;
								CString strDzNum = _T("") ;
								cSetPreShare.GetFieldValue(_T("core_name"), strCoreName) ;
								mapCoreNameToDzNum.Lookup(strCoreName, strDzNum) ;
								//begin 此段代码为解决1个端子上接4根以上的芯，权宜之计
								if ((iPreShareCount/iPreSharePlus) >=5 ) //5机牵引才考虑
								{
									if((iPreShareIndex/iPreSharePlus) == 2)
									{
										int iDzNum = _tstoi(strDzNum) ;
										int iDzNum2 = iDzNum-4 ;
										strDzNum.Format(_T("%d-%d"), iDzNum, iDzNum2) ;
									}
									else if ((iPreShareIndex/iPreSharePlus) > 2)
									{
										int iDzNum = _tstoi(strDzNum) ;
										int iDzNum2 = iDzNum-4 ;
										strDzNum.Format(_T("%d"), iDzNum2) ;
									}

								}
								//end 此段代码为解决1个端子上接4根以上的芯，权宜之计
								cSetPreShare.Edit() ;
								cSetPreShare.SetFieldValue(_T("end_num"), strDzNum) ;
								cSetPreShare.Update() ;

								iPreShareIndex++ ;
								cSetPreShare.MoveNext() ;
							}
							cSetPreShare.Close() ;
						}
						//end 先给从前一个盒子引入的共用芯线的末端指定端子号

						//begin 给属于本盒子的共享芯线起始端子指定端子号
						//select 去掉 and is_inside=0 ，共享芯线不管是否内部
						CADORecordset cSetCurBoxShare ;
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_share>0 "), iBoxHandle) ;
						if(cSetCurBoxShare.IsOpen())
						{
							cSetCurBoxShare.Close() ;
						}
						if(cSetCurBoxShare.Open(g_PtrCon, strSql))
						{
							if (!cSetCurBoxShare.IsBOF())
							{
								cSetCurBoxShare.MoveFirst() ;
							}
							while (!cSetCurBoxShare.IsEOF())
							{
								CString strCoreName ;
								CString strDzNum = _T("") ;
								cSetCurBoxShare.GetFieldValue(_T("core_name"), strCoreName) ;
								mapCoreNameToDzNum.Lookup(strCoreName, strDzNum) ;
								cSetCurBoxShare.Edit() ;
								cSetCurBoxShare.SetFieldValue(_T("start_num"), strDzNum) ;
								cSetCurBoxShare.Update() ;

								cSetCurBoxShare.MoveNext() ;
							}
							cSetCurBoxShare.Close() ;
						}
						//end 给属于本盒子的共享芯线起始端子指定端子号

						//end 处理所有共享芯线的端子

						//begin 处理剩余的非共享芯线的端子:先处理该盒子新用的芯（写start端）+再处理从前面盒子引入的芯（写end端）
						CADORecordset cSetOtherCore ;
						// 					strSql.Format(_T("select * from \
						// 						(select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 \
						// 						union select * from cable_core_draw where belong_box_handle_low \
						// 						in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside=0) order by pt_y desc"), iBoxHandle,iBoxHandle) ;
						//本盒子新用芯
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 and is_add=0"), iBoxHandle) ;
						if (cSetOtherCore.IsOpen())
						{
							cSetOtherCore.Close() ;
						}
						if(cSetOtherCore.Open(g_PtrCon, strSql))
						{
							if (!cSetOtherCore.IsBOF())
							{
								cSetOtherCore.MoveFirst() ;
							}
							while (!cSetOtherCore.IsEOF())
							{
								int iFoundDz = -1 ;
								CString strCurDzNum = _T("") ;

								CADORecordset cSetDzDetail ;
								strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0"), iBoxTypeId) ;
								if (cSetDzDetail.IsOpen())
								{
									cSetDzDetail.Close() ;
								}
								cSetDzDetail.Open(g_PtrCon, strSql) ;
								if (cSetDzDetail.GetRecordCount()>0)
								{
									cSetDzDetail.MoveFirst() ;
									cSetDzDetail.GetFieldValue(_T("box_node"), strCurDzNum) ;

									cSetDzDetail.Edit() ;
									cSetDzDetail.SetFieldValue(_T("is_used"), 1) ;
									cSetDzDetail.Update() ;
									iFoundDz = 1 ;
								}
								else
								{
									iFoundDz = 0 ;
								}
								cSetDzDetail.Close() ;

								if (iFoundDz==1)
								{
									cSetOtherCore.Edit() ;
									cSetOtherCore.SetFieldValue(_T("start_num"), strCurDzNum) ;
									cSetOtherCore.Update() ;
								}

								cSetOtherCore.MoveNext() ;
							}
							cSetOtherCore.Close() ;
						}

						//从前面盒子引入的芯
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside<>1 and is_add=0 order by pt_y desc"), iBoxHandle) ;
						if (cSetOtherCore.IsOpen())
						{
							cSetOtherCore.Close() ;
						}
						if(cSetOtherCore.Open(g_PtrCon, strSql))
						{
							if (!cSetOtherCore.IsBOF())
							{
								cSetOtherCore.MoveFirst() ;
							}					
							while (!cSetOtherCore.IsEOF())
							{
								int iFoundDz = -1 ;
								CString strCurDzNum = _T("") ;

								CADORecordset cSetDzDetail ;
								strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0"), iBoxTypeId) ;
								if (cSetDzDetail.IsOpen())
								{
									cSetDzDetail.Close() ;
								}
								if(cSetDzDetail.Open(g_PtrCon, strSql))
								{
									if (cSetDzDetail.GetRecordCount()>0)
									{
										cSetDzDetail.MoveFirst() ;
										cSetDzDetail.GetFieldValue(_T("box_node"), strCurDzNum) ;

										cSetDzDetail.Edit() ;
										cSetDzDetail.SetFieldValue(_T("is_used"), 1) ;
										cSetDzDetail.Update() ;
										iFoundDz = 1 ;
									}
									else
									{
										iFoundDz = 0 ;
									}
									cSetDzDetail.Close() ;
								}

								if (iFoundDz==1)
								{
									cSetOtherCore.Edit() ;
									cSetOtherCore.SetFieldValue(_T("end_num"), strCurDzNum) ;
									cSetOtherCore.Update() ;
								}				

								cSetOtherCore.MoveNext() ;
							}
							cSetOtherCore.Close() ;

						}

						//end 处理剩余的非共享芯线的端子,先处理该盒子新用的芯（写start端）+再处理从前面盒子引入的芯（写end端）

						//begin 标出哪些不再延续的芯线，cable_core_draw:is_last，is_last的端子要绘制end端子
						CADORecordset cSetIsLastCore ; 
						strSql.Format(_T("SELECT * FROM cable_core_draw WHERE [dev_name]&[core_name] \
										 in (select [dev_name]&[core_name] from cable_core_draw where belong_box_handle_low=%d) \
										 and [dev_name]&[core_name] not in (select [dev_name]&[core_name] from cable_core_draw where \
										 belong_box_handle_low=(select next_ent_handle_low from cablenet_ent where handle_low=%d))"), iBoxHandle, iBoxHandle) ;
						if (cSetIsLastCore.IsOpen())
						{
							cSetIsLastCore.Close() ;
						}
						if(cSetIsLastCore.Open(g_PtrCon, strSql))
						{
							if (!cSetIsLastCore.IsBOF())
							{
								cSetIsLastCore.MoveFirst() ;
							}
							while (!cSetIsLastCore.IsEOF())
							{
								cSetIsLastCore.Edit() ;
								cSetIsLastCore.SetFieldValue(_T("is_last"), 1) ;
								cSetIsLastCore.Update();
								cSetIsLastCore.MoveNext() ;
							}
							cSetIsLastCore.Close() ;
						}

						//end 标出哪些不再延续的芯线，cable_core_draw:is_last，is_last的端子要绘制end端子

						//begin 计算盒子内芯线的编号 core_sn
						//////////////////////////////////////////////////////////////////////////
						//获取连接电缆的字符串，类似35-12(4)
						CString strCableStr = _T("") ;
						CADORecordset cSetGetCableStr ;
						strSql.Format(_T("select cablestring from cablenet_ent where handle_low=%d"), iBoxHandle) ;
						if (cSetGetCableStr.IsOpen())
						{
							cSetGetCableStr.Close() ;
						}
						if(cSetGetCableStr.Open(g_PtrCon, strSql))
						{
							if (!cSetGetCableStr.IsBOF())
							{
								cSetGetCableStr.MoveFirst() ;
							}
							if (cSetGetCableStr.GetRecordCount()>0)
							{
								cSetGetCableStr.GetFieldValue(_T("cablestring"), strCableStr) ;
							}
							cSetGetCableStr.Close() ;
						}

						CString strCabeStrTmp1 = strCableStr.Mid(strCableStr.Find(_T('-'))+1) ;
						CString strCabeStrTmp2 = strCabeStrTmp1.Left(strCabeStrTmp1.Find(_T('('))) ;//电缆芯数
						strCabeStrTmp2.TrimRight(_T("L")) ; //对44L,去掉L
						//取得电缆定义的id
						int iCableTypeId = -1 ;
						CADORecordset cSetGetCableTypeId ;
						strSql.Format(_T("select id from define_cable_type where cable_name='%s'"), strCabeStrTmp2) ;
						if(cSetGetCableTypeId.IsOpen())
						{
							cSetGetCableTypeId.Close() ;
						}
						if(cSetGetCableTypeId.Open(g_PtrCon, strSql))
						{
							if (!cSetGetCableTypeId.IsBOF())
							{
								cSetGetCableTypeId.MoveFirst() ;
							}
							if (cSetGetCableTypeId.GetRecordCount()>0)
							{
								cSetGetCableTypeId.GetFieldValue(_T("id"), iCableTypeId) ;
							}
							cSetGetCableTypeId.Close() ;
						}


						//先将该型号电缆的所有芯的is_used重置0

						CADORecordset cSetUpdate ;
						strSql.Format(_T("select * from define_core_detail where cable_type_id=%d"), iCableTypeId) ;
						if(cSetUpdate.IsOpen())
						{
							cSetUpdate.Close() ;
						}
						if(cSetUpdate.Open(g_PtrCon, strSql))
						{
							if (!cSetUpdate.IsBOF())
							{
								cSetUpdate.MoveFirst() ;
							}
							while (!cSetUpdate.IsEOF())
							{
								cSetUpdate.Edit() ;
								cSetUpdate.SetFieldValue(_T("is_used"), 0) ;
								cSetUpdate.Update() ;
								cSetUpdate.MoveNext() ;
							}
							cSetUpdate.Close() ;
						}

						//ExecSqlString(strSql) ;

						//将内部芯已经占用的芯线标出来					
						CADORecordset cSetComputeCoreSn ;
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and is_inside=1"), iBoxHandle) ;
						if(cSetComputeCoreSn.IsOpen())
						{
							cSetComputeCoreSn.Close() ;
						}
						if(cSetComputeCoreSn.Open(g_PtrCon,strSql))
						{
							if (!cSetComputeCoreSn.IsBOF())
							{
								cSetComputeCoreSn.MoveFirst() ;
							}					
							while (!cSetComputeCoreSn.IsEOF())
							{
								CString strCableCoreSn = _T("") ;

								cSetComputeCoreSn.GetFieldValue(_T("core_sn"), strCableCoreSn) ;
								//如果是内部芯线，则在define_core_detail里面找出这个芯线别标示为已用芯
								CADORecordset cSetUpdateCoreDeatail ;
								strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and core_sn='%s'"), iCableTypeId, strCableCoreSn) ;
								if (cSetUpdateCoreDeatail.IsOpen())
								{
									cSetUpdateCoreDeatail.Close();
								}
								if(cSetUpdateCoreDeatail.Open(g_PtrCon, strSql))
								{
									if (!cSetUpdateCoreDeatail.IsBOF())
									{
										cSetUpdateCoreDeatail.MoveFirst() ;
									}
									if (!cSetUpdateCoreDeatail.IsEOF())
									{
										cSetUpdateCoreDeatail.Edit() ;
										cSetUpdateCoreDeatail.SetFieldValue(_T("is_used"), 1) ;
										cSetUpdateCoreDeatail.Update() ;
									}
									cSetUpdateCoreDeatail.Close() ;
								}

								cSetComputeCoreSn.MoveNext() ;
							}
							cSetComputeCoreSn.Close() ;
						}
						//给非内部芯分配芯线,包括半内部芯，即is_inside=2 
						//20130408：对于is_inside=1的纯内部，假如其默认core_sn为空，则也需要给其分配芯线，比如SH5，因为其涉及加芯，不能再默认芯线
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and (is_inside<>1 or (is_inside=1 and core_sn='')) order by pt_y desc"), iBoxHandle) ;
						if(cSetComputeCoreSn.IsOpen())
						{
							cSetComputeCoreSn.Close() ;
						}
						if(cSetComputeCoreSn.Open(g_PtrCon,strSql))
						{
							if (!cSetComputeCoreSn.IsBOF())
							{
								cSetComputeCoreSn.MoveFirst() ;
							}					
							while (!cSetComputeCoreSn.IsEOF())
							{
								int iFundCoreSn = -1 ;
								CString strCableCoreSn = _T("") ;

								//得到指定型号的电缆的所有未使用的芯
								CADORecordset cSetCoreDetail ;
								strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0"), iCableTypeId) ;
								if (cSetCoreDetail.IsOpen())
								{
									cSetCoreDetail.Close() ;
								}
								if(cSetCoreDetail.Open(g_PtrCon, strSql))
								{
									if (cSetCoreDetail.GetRecordCount()>0)
									{
										cSetCoreDetail.MoveFirst() ;
										cSetCoreDetail.GetFieldValue(_T("core_sn"), strCableCoreSn) ;

										cSetCoreDetail.Edit() ;
										cSetCoreDetail.SetFieldValue(_T("is_used"),1) ;
										cSetCoreDetail.Update() ;
										iFundCoreSn = 1 ;
									}
									else
									{
										iFundCoreSn = 0 ;
									}
									cSetCoreDetail.Close() ;
								}

								if (iFundCoreSn==1) //找到芯线才更新数据，否则不更新
								{
									cSetComputeCoreSn.Edit() ;
									cSetComputeCoreSn.SetFieldValue(_T("core_sn"), strCableCoreSn) ;
									cSetComputeCoreSn.Update() ;
								}

								cSetComputeCoreSn.MoveNext() ;
							}
							cSetComputeCoreSn.Close() ;		
						}
						

						//end 计算盒子内芯线的编号 core_sn


						//begin 计算备用,20100406注：假定备用是连续的
						CString strBackupStr = _T("") ;					
						CADORecordset cSetBackup ;
						//单芯
						strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='1'"), iCableTypeId) ;
						if (cSetBackup.IsOpen())
						{
							cSetBackup.Close() ;
						}
						if(cSetBackup.Open(g_PtrCon, strSql))
						{
							int iDxRsCount = cSetBackup.GetRecordCount() ;
							if (iDxRsCount>0)
							{								
								cSetBackup.MoveFirst() ;

								CString strBackupStrTmp = _T("") ;
								cSetBackup.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
								strBackupStr+=strBackupStrTmp ;
								if (iDxRsCount>1)
								{
									cSetBackup.MoveLast() ;
									if (!cSetBackup.IsEOF())
									{
										strBackupStrTmp = _T("") ;
										cSetBackup.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
										strBackupStr+=_T("~") ;
										strBackupStr+=strBackupStrTmp ;
									}
								}
							}
							cSetBackup.Close() ;
						}


						//对绞
						strSql.Format(_T("select distinct(left(core_sn,2)) as name from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='2'"), iCableTypeId) ;
						if (cSetBackup.IsOpen())
						{
							cSetBackup.Close() ;
						}
						if(cSetBackup.Open(g_PtrCon, strSql))
						{
							if (!cSetBackup.IsBOF())
							{
								cSetBackup.MoveFirst() ;
							}
							while (!cSetBackup.IsEOF())
							{
								CString strName ;
								CString strBackupStrTmp = _T("") ;
								cSetBackup.GetFieldValue(_T("name"), strName) ;

								CADORecordset cSetBackupTmp ;
								strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='2' and left(core_sn,2)='%s'"), iCableTypeId, strName) ;
								if(cSetBackupTmp.IsOpen())
								{
									cSetBackupTmp.Close() ;
								}
								if(cSetBackupTmp.Open(g_PtrCon, strSql) )
								{
									if (cSetBackupTmp.GetRecordCount()>0)
									{
										cSetBackupTmp.MoveFirst() ;
										cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
										if (strBackupStr!=_T(""))
										{
											strBackupStr+=_T(",") ;
										}
										strBackupStr+=strBackupStrTmp ;
										cSetBackupTmp.MoveNext() ;
										if (!cSetBackupTmp.IsEOF())
										{
											strBackupStrTmp = _T("") ;
											cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
											strBackupStr+=_T("~2") ;
										}
									}
									cSetBackupTmp.Close() ;
								}

								cSetBackup.MoveNext() ;
							}
							cSetBackup.Close() ;
						}
						//四芯组
						strSql.Format(_T("select distinct(left(core_sn,len(core_sn)-2)) as name from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='4'"), iCableTypeId) ;
						if (cSetBackup.IsOpen())
						{
							cSetBackup.Close() ;
						}
						if(cSetBackup.Open(g_PtrCon, strSql))
						{
							if (!cSetBackup.IsBOF())
							{
								cSetBackup.MoveFirst() ;
							}
							while (!cSetBackup.IsEOF())
							{
								CString strName ;
								CString strBackupStrTmp = _T("") ;
								cSetBackup.GetFieldValue(_T("name"), strName) ;

								CADORecordset cSetBackupTmp ;
								strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='4' and left(core_sn,len(core_sn)-2)='%s'"), iCableTypeId, strName) ;
								if(cSetBackupTmp.IsOpen())
								{
									cSetBackupTmp.Close() ;
								}
								if(cSetBackupTmp.Open(g_PtrCon, strSql))
								{
									if (cSetBackupTmp.GetRecordCount()>0)
									{
										cSetBackupTmp.MoveFirst() ;
										cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
										if (strBackupStr!=_T(""))
										{
											strBackupStr+=_T(",") ;
										}							
										strBackupStr+=strBackupStrTmp ;
										cSetBackupTmp.MoveNext() ;
										if (!cSetBackupTmp.IsEOF())
										{
											strBackupStrTmp = _T("") ;
											cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
											strBackupStr+=_T("~4") ;
										}
									}
									cSetBackupTmp.Close() ;
								}	

								cSetBackup.MoveNext() ;
							}
							cSetBackup.Close() ;
						}

						strSql.Format(_T("update cablenet_ent set backup_string='%s' where handle_low=%d"), strBackupStr, iBoxHandle) ;
						ExecSqlString(strSql) ;

						// 					cSetAllBox.Edit() ;
						// 					cSetAllBox.SetFieldValue(_T("backup_string"), strBackupStr) ;
						// 					cSetAllBox.Update() ;

						//end 计算备用
						//20100405-1740 here

						cSetAllBox.MoveNext() ;
					}
				}
				cSetAllBox.Close() ;

			}
			//end 填充端子编号,芯线名称

			//begin 分线盘数据
			this->ComputeFxData(iXHL_handle_low) ;
			//end 分线盘数据
			

			//绘制编号为iIndex的配线图

			CString strSaveFile ;
			strSaveFile.Format(_T("%s\\%s.dwg"), this->m_strSavePath, strFileName) ;			
			this->DrawPx(strSaveFile, iXHL_handle_low) ;

			iIndex++ ;

			cSet.MoveNext() ;
		}
		acedRestoreStatusBar() ;		
		ads_printf(_T("\n电缆配线结束：......\n")) ;

		cSet.Close() ;

		//end 一个信号楼ent产生一张图纸，循环产生所有图纸

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeCoreAndDraw")) ;
	}  

	return bRet ;

}
//读取电缆网络图并验证数据
BOOL CCableNetForJY::ReadAndFillDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;

	acedSetStatusBarProgressMeter(_T("readcablenet..."), 1, 100) ;	

				
	if (!this->ReadEntToDB(pConnection, pCurDb))
	{
		return FALSE ;
	}

	acedSetStatusBarProgressMeterPos(10) ;

	if (!this->ReadConCableToDB(pConnection, pCurDb))
	{
		return FALSE ;
	}

	acedSetStatusBarProgressMeterPos(20) ;

	if (!this->FillDB())
	{
		return FALSE ;
	}
	//acedSetStatusBarProgressMeterPos(2) ;

	acedRestoreStatusBar() ;

	
	return bRet ;
}

//绘制配线图，一个如楼端绘制一张图
void CCableNetForJY::DrawPx(CString strDwgFilePath, long lXHLHandle)
{
	try
	{
		AcDbDatabase *pCurDb = new AcDbDatabase() ;

		CString strBlockFile ;
		strBlockFile.Format(_T("%s\\support\\cablenet\\QT\\DZ.DWG"), m_strSysBasePath) ;
		
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;
		AcDbObjectId textStyleId = NULL ;
		textStyleId = mysys.CreateTextStyleByFileName(_T("ZhfDlpx"), _T("txt.shx"), _T("hztxt.shx"), 2, 0.8) ;

		CADORecordset cSet ;
		CString strSql ;
		
		//begin 绘制芯线
		strSql.Format(_T("select * from cable_core_draw")) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF())
		{
			CString strDevName, strDevNameTmp, strCoreName, strCoreSn ;
			double dPt_x = 0 ;
			double dPt_y = 0 ;
			CString strStartNum = _T("") ;
			CString strEndNum = _T("") ;
			int iLeftOrRight = 0 ;
			int iIsNew = 0 ;
			int iIsLast = 0 ;
			int iIsVirtual = 0 ;
			long iBoxHandle = 0 ;
			CString strDevType = _T("") ;//芯线所在的设备类型
			CString strTmp ;
			int iIsAdd = 0 ;

			cSet.GetFieldValue(_T("dev_name"), strDevName) ;
			cSet.GetFieldValue(_T("core_name"), strCoreName) ;
			cSet.GetFieldValue(_T("belong_box_handle_low"), iBoxHandle) ;
			cSet.GetFieldValue(_T("core_sn"), strCoreSn) ;
			cSet.GetFieldValue(_T("pt_x"), dPt_x) ;
			cSet.GetFieldValue(_T("pt_y"), dPt_y) ;
			cSet.GetFieldValue(_T("start_num"), strStartNum) ;
			cSet.GetFieldValue(_T("end_num"), strEndNum) ;
			cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
			cSet.GetFieldValue(_T("is_new"), iIsNew) ;
			cSet.GetFieldValue(_T("is_last"), iIsLast) ;
			cSet.GetFieldValue(_T("is_virtual"), iIsVirtual) ; //虚拟芯不用绘制,但是要绘制末端编号
			cSet.GetFieldValue(_T("is_add"), iIsAdd) ; //加芯线要画成红色

			strDevNameTmp = strDevName ;
			strDevNameTmp.TrimRight(_T("-A")) ;
			strDevNameTmp.TrimRight(_T("-B")) ;

			//取得设备类型
			strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
			CADORecordset cSetFindDevType ;
			if (cSetFindDevType.IsOpen())
			{
				cSetFindDevType.Close() ;
			}
			cSetFindDevType.Open(g_PtrCon, strSql) ;
			if (cSetFindDevType.GetRecordCount()>0)
			{
				cSetFindDevType.MoveFirst() ;			

				cSetFindDevType.GetFieldValue(_T("devtype"), strDevType) ;
			}
			cSetFindDevType.Close() ;

			AcGePoint3d pt_3d_start(dPt_x, dPt_y, 0) ;
			AcGePoint3d pt_3d_end(pt_3d_start) ;
			AcGePoint3d pt_3d_text(pt_3d_start.x, pt_3d_start.y+1, 0) ;
			AcDb::TextHorzMode iHorMode = AcDb::kTextLeft ;
			
			if (iLeftOrRight==1)
			{
				pt_3d_start.x+=m_dCellWidth/2 ;
				pt_3d_end.x-=m_dCellWidth/2 ;
				pt_3d_text.x = pt_3d_start.x-3 ;
				
				iHorMode = AcDb::kTextRight ;

			}
			else 
			{
				pt_3d_start.x-=m_dCellWidth/2 ;
				pt_3d_end.x+=m_dCellWidth/2 ;
				pt_3d_text.x = pt_3d_start.x+3 ;

			}

			if (iIsVirtual==0)
			{
				if (dPt_x!=0) //dPt_x不为0表示不是信号楼内的芯线
				{
					//芯线
					int iColorIndex = (iIsAdd==0?0:1) ;					
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, iColorIndex) ;
				}
				//端子
				AcDbBlockReference *pRef = NULL ;
				if (!(iIsNew&&iIsAdd||iIsAdd&&dPt_x==0))//加芯的起点不要绘制端子
				{
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_start,0, 1) )
					{
						pRef->close() ;
					}
				}

				AcGePoint3d pt_3d_con1 = pt_3d_start,pt_3d_con2 = pt_3d_start ;
				pt_3d_con2.y+=5 ;
				if (iIsAdd) //加芯线和上面的线连接起来
				{
					mysys.AddAcDbLine(pt_3d_con1, pt_3d_con2, 1) ;
				}

				if (iIsLast==1)
				{
					if (iIsAdd) //加芯线和上面的线连接起来
					{
						pt_3d_con1 = pt_3d_con2 = pt_3d_end ;
						pt_3d_con2.y+=5 ;
						mysys.AddAcDbLine(pt_3d_con1, pt_3d_con2, 1) ;
					}
					else
					{
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_end,0, 1) )
						{
							pRef->close() ;
						}
					}

				}



			}
			
			//
			ads_point base_pt_text = {0,0,0} ;
			//AcDbObjectId objID ;
			//端子号
			if ((iIsNew==1 && iIsVirtual==0) || (dPt_x==0 && iIsAdd==0))
			{
				base_pt_text[0] = pt_3d_start.x ;
				base_pt_text[1] = pt_3d_start.y+1 ;
				//strTmp.Format(_T("%s"), strStartNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ; //起点
				//mysys.WriteText(base_pt_text, strStartNum, objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ; //起点
				mysys.AddText(strStartNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;//起点

			}

			if (dPt_x!=0)
			{
				base_pt_text[0] = pt_3d_end.x ;
				base_pt_text[1] = pt_3d_end.y+1 ;
				//strTmp.Format(_T("%d"), iEndNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ;//末点
				int iFoundMark = strEndNum.Find(_T("-")) ;
				if(iFoundMark>-1)
				{
					CString strEndNum1 = strEndNum.Left(iFoundMark) ;
					CString strEndNum2 = strEndNum.Mid(iFoundMark+1, strEndNum.GetLength()-iFoundMark-1) ;
					AcGePoint3d pt_3d_end_for4(pt_3d_end) ;				

					if (iLeftOrRight==0)
					{
						pt_3d_end_for4.x+=6 ;						
					}
					else
					{
						pt_3d_end_for4.x-=6 ;
					}
					mysys.AddAcDbLine(pt_3d_end, pt_3d_end_for4) ;

					//加芯不绘制DZ块
					if (iIsAdd==0)
					{
						AcDbBlockReference *pRef = NULL ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_end_for4,0, 1) )
						{
							pRef->close() ;
						}
					}

					ads_point base_pt_text_for4 = {pt_3d_end_for4.x,pt_3d_end_for4.y+1,0} ;
					//mysys.WriteText(base_pt_text, strEndNum1, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//末点
					mysys.AddText(strEndNum1, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//末点
					//mysys.WriteText(base_pt_text_for4, strEndNum2, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//末点
					mysys.AddText(strEndNum2, base_pt_text_for4, base_pt_text_for4, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//末点


				}
				else
				{
					//mysys.WriteText(base_pt_text, strEndNum, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//末点
					mysys.AddText(strEndNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//末点
				}

			}
			if (iIsVirtual==0)
			{
				base_pt_text[0] = pt_3d_start.x+(pt_3d_end.x-pt_3d_start.x)/2+(pt_3d_start.x>pt_3d_end.x?-2:2) ;			
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode) ; //芯线名
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
				mysys.AddText(strCoreSn, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
				
				base_pt_text[0] = pt_3d_text.x ;
				base_pt_text[1] = pt_3d_text.y ; 
				//信号楼的暂时不画
				// 			if (dPt_x==0)
				// 			{
				// 				if (iLeftOrRight==1)
				// 				{
				// 					base_pt_text[0] = pt_3d_start.x+2 ;
				// 					iHorMode = AcDb::kTextLeft ;
				// 				}
				// 				else 
				// 				{
				// 					base_pt_text[0] = pt_3d_start.x-2 ;
				// 					iHorMode = AcDb::kTextRight ;
				// 				}
				// 				
				// 			}
				if (dPt_x!=0)
				{
					strTmp.Format(_T("%s-%s"), strDevNameTmp, strCoreName) ;

					// 					if (strDevType.Left(2)==_T("JZ"))
					// 					{
					// 						CString strDevNameTmp = strDevName ;
					// 						strDevNameTmp.TrimRight(_T("-A")) ;
					// 						strDevNameTmp.TrimRight(_T("-B")) ;
					// 						strTmp.Format(_T("%s-%s"), strDevNameTmp, strCoreName) ;
					//					}
					// 					else if (strDevType.Left(1)==_T("J") || strDevType.Left(1)==_T("F"))
					// 					{
					// 						strTmp.Format(_T("%s%s"), strDevNameTmp, strCoreName) ;
					// 					}
					if (strCoreName.Left(1) == _T("J") || strCoreName.Left(1) == _T("F") )
					{
						strTmp.Format(_T("%s%s"), strDevNameTmp, strCoreName) ;
					}
				
					//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, iHorMode) ;
					//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
					mysys.AddText(strTmp, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
					
				}
			}

			cSet.MoveNext() ;
		}

		cSet.Close() ;
		//end 绘制芯线
		
		//begin 绘制title和边框
		strSql.Format(_T("select * from cablenet_ent where belong_xhl_handle_low=%d"), lXHLHandle) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF())
		{
			CString strDevName, strBoxName, strCableString, strBackupString ;
			CString strDevType = _T("") ;
			CString strTmp ;
			int iLeftOrRight = 0 ;
			double dTitle_x = 0 ;
			double dTitle_y = 0 ;
			double dBottom_x = 0 ;
			double dBottom_y = 0 ;
			cSet.GetFieldValue(_T("devname"), strDevName) ;
			cSet.GetFieldValue(_T("boxtype"), strTmp) ;

			strBoxName = strTmp ; //.Left(strTmp.Find(_T("_"))) ;
			//strBoxName.MakeUpper() ;
			cSet.GetFieldValue(_T("cablestring"), strCableString) ;
			cSet.GetFieldValue(_T("backup_string"), strBackupString) ;
			cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
			cSet.GetFieldValue(_T("px_title_x"), dTitle_x) ;
			cSet.GetFieldValue(_T("px_title_y"), dTitle_y) ;
			cSet.GetFieldValue(_T("px_bottom_x"), dBottom_x) ;
			cSet.GetFieldValue(_T("px_bottom_y"), dBottom_y) ;
			cSet.GetFieldValue(_T("devtype"), strDevType) ;


			int iPlusX = iLeftOrRight==0?-1:1 ;
			AcDb::TextHorzMode iHorMode = AcDb::kTextLeft ;
			AcDb::TextVertMode iVerMode = AcDb::kTextVertMid ;

			//AcGePoint3d pt_3d_text(0,0,0) ;
			ads_point pt_base_text = {0,0,0} ;
			pt_base_text[1] = dTitle_y+m_dCellHight ;
			pt_base_text[0] = iLeftOrRight==0?dTitle_x-m_dCellWidth:dTitle_x ;
			//AcCmColor clr ;
			//clr.setColorIndex(0) ;
			//AcDbObjectId objID ;

			if (strBoxName==_T("XHL"))
			{
				pt_base_text[0] = pt_base_text[0]+m_dCellWidth/2 ;
				iHorMode = AcDb::kTextCenter ;				
				//mysys.WriteText(pt_base_text, strDevName, 0, iHorMode,2,0.8, clr) ;				
				//mysys.WriteText(pt_base_text, strDevName,objID, 2, 0.8,iHorMode, iVerMode) ;
				//mysys.WriteText(pt_base_text, _T("信号楼"), objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(_T("信号楼"), pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
				pt_base_text[1] += m_dCellHight*2 ;
				strDevName+=_T("#") ;
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

				mysys.AddAcDb2dPolyline(AcGePoint3d(0,0,0), AcGePoint3d(0,dBottom_y,0), 0.5) ; //信号楼封口线

			}
			else
			{
				pt_base_text[0] = pt_base_text[0]+4 ;
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8,iHorMode, iVerMode) ;
				if ((strDevType.Left(1)==_T("J") && strDevType.Left(2)!=_T("JZ")) || strDevType.Left(1)==_T("F"))
				{
					int iMarkLoc = strDevType.Find(_T("-")) ;
					if (iMarkLoc!=-1)
					{
						strDevName+=strDevType.Left(strDevType.Find(_T("_"))) ;
					}
					// 					else
					// 					{
					// 						strDevName+=strDevType ;
					// 					}
				}
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

				iHorMode = AcDb::kTextRight ;
				pt_base_text[0] = pt_base_text[0]-4+m_dCellWidth-4 ;
				//mysys.WriteText(pt_base_text, strBoxName, objID, 2, 0.8,iHorMode, iVerMode) ;
				//mysys.WriteText(pt_base_text, strBoxName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strBoxName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
			}

			AcGePoint3d pt_3d_start(dTitle_x,dTitle_y,0) ;
			AcGePoint3d pt_3d_end(dTitle_x,dTitle_y,0) ;
			AcGePoint3d pt_3d_tmp(0,0,0) ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5) ;
			//mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_tmp.x = pt_3d_end.x ;
			pt_3d_tmp.y = dBottom_y ;
			//mysys.AddAcDbLine(pt_3d_end, pt_3d_tmp) ;
			mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_tmp, 0.5) ;
			pt_3d_start.y = dBottom_y ;
			mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_tmp, 0.5) ;
			pt_3d_start.x = dTitle_x ;
			pt_3d_start.y = dTitle_y+m_dCellHight*2 ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			pt_3d_end.y = pt_3d_start.y ;
			mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5) ;
			pt_3d_start.x = pt_3d_end.x ;
			pt_3d_start.y = dTitle_y ;
			mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_start, 0.5) ;

		
			if (strCableString!=_T(""))
			{
				pt_3d_start.x = dBottom_x - 10 ;
				pt_3d_start.y = dBottom_y + 5;
				pt_3d_end.x = dBottom_x + 10 ;
				pt_3d_end.y = pt_3d_start.y ;
				mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				pt_base_text[0] = dBottom_x + iPlusX  ;
				pt_base_text[1] = dBottom_y + 6 ; 
				iHorMode = iLeftOrRight==0?AcDb::kTextRight:AcDb::kTextLeft ;
				//strCableString.Insert(strCableString.Find(_T("-"))+1, " ") ;
				//strCableString+=" " ;
				//strCableString.Insert(0, "  ") ;
				//mysys.WriteText(pt_base_text, strCableString, 0, iHorMode,2,0.8, clr) ;	
				//mysys.WriteText(pt_base_text, strCableString, objID, 2, 0.8, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
				mysys.AddText(strCableString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;
				pt_base_text[1] = dBottom_y+4 ;
				//AcDbObjectId objId ;
				//mysys.WriteText(pt_base_text, strBackupString, objId, 2, 0.8, iHorMode, AcDb::kTextTop, 0, NULL, 0) ;
				//mysys.WriteText(pt_base_text, strBackupString, objID, 2, 0.8, iHorMode, AcDb::kTextTop, 0,textStyleId, 0) ;
				mysys.AddText(strBackupString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
				pt_base_text[0] = dBottom_x - iPlusX*5 ;
				//mysys.WriteText(pt_base_text, _T("备用"), objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextTop, 0, textStyleId, 0) ;
				mysys.AddText(_T("备用"), pt_base_text, pt_base_text, AcDb::kTextCenter, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;

			}
			//here

			cSet.MoveNext() ;
		}
		cSet.Close() ;
		//end 绘制title和边框

		pCurDb->saveAs(strDwgFilePath) ;

		delete pCurDb ;
		pCurDb = NULL ;

		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawPx")) ;
	} 


}

//查找一个设备所的所有分支（以is_start=1为结束的一条链路）
void CCableNetForJY::FindBranch(long iDesHandle, int &iBranchNum)
{
	try
	{	
		CADORecordset cSet ;
		CString strSql ;
		
		strSql.Format(_T("select * from cablenet_ent where next_ent_handle_low=%d"), iDesHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF())
		{
			int iIsStart = -1 ;
			cSet.GetFieldValue(_T("is_start"), iIsStart) ;

			if (iIsStart==1)
			{
				iBranchNum++ ;
				cSet.MoveNext() ;
				continue ;
			}
			
			long iEntHandle = 0 ;
			cSet.GetFieldValue(_T("handle_low"), iEntHandle) ;
			this->FindBranch(iEntHandle, iBranchNum) ;

			cSet.MoveNext() ;
		}
		cSet.Close() ;
	}

	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FindBranch")) ;
	} 
}

void CCableNetForJY::FindNextEnt(long iHandle, int iForwardToLeftOrRight)
{
	CString strNextHandeName[] = {_T("left_ent_handle"), _T("right_ent_handle")};
	// 	if (iForwardToLeftOrRight==0)
	// 	{
	// 		strNextHandeName = "left_ent_handle" ;
	// 	}
	// 	else if (iForwardToLeftOrRight==1)
	// 	{
	// 		strNextHandeName = "right_ent_handle" ;
	// 	}
	try	
	{				
		CADORecordset cSet ;
		CString strSql ;
		
		//begin 设置cablenet_ent表的left_ent_handle和right_ent_handle
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				long iNextHandle = 0 ;
				CString strCableStr ;

				cSet.GetFieldValue(_T("next_ent_handle_low"), iNextHandle) ;
				if (iNextHandle>0)//说明此实体已经被处理过，其后续实体肯定被遍历过了，退出递归
				{
					cSet.Close() ;
					return ;
				}
				cSet.GetFieldValue(strNextHandeName[iForwardToLeftOrRight], iNextHandle) ;
				if (iNextHandle<=0)//说明此实体右边没有实体，这种情况下应该是信号楼，退出递归
				{
					cSet.Edit();
					cSet.SetFieldValue(_T("left_or_right"),(iForwardToLeftOrRight+1)%2) ;
					cSet.Update();
					cSet.Close() ;
					return ;
				}

				strSql.Format(_T("select * from connect_cable where %s=%d"), strNextHandeName[(iForwardToLeftOrRight+1)%2], iHandle) ;
				CADORecordset cSetTmp ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if(cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						cSetTmp.MoveFirst() ;							
						cSetTmp.GetFieldValue(_T("cablestring"), strCableStr) ;

					}
					cSetTmp.Close() ;
				}

				cSet.Edit() ;
				cSet.SetFieldValue(_T("next_ent_handle_low"), iNextHandle) ;
				cSet.SetFieldValue(_T("cablestring"), strCableStr) ;
				cSet.SetFieldValue(_T("left_or_right"),(iForwardToLeftOrRight+1)%2) ;
				cSet.Update() ;

				FindNextEnt(iNextHandle, iForwardToLeftOrRight) ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillDB")) ;
	} 
}

int CCableNetForJY::ReadFxpDataToDb(CString strXlsFileName)
{
	int iReturn = 0 ;
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	CString strExec, strMsg ;
	//清除分线盘数据
	CDBUtility::ClearTable(_T("fx_data")) ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( _T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		
		//判断文件的合法性；
		BOOL bCheckFile = TRUE ;
		CString  strCellName[3] = { _T("B1"), _T("C1"),_T("D1") } ;
		CString strCellContent[3] = {_T("设备名称"), _T("芯线名称"), _T("分线端子")} ;
		_variant_t range_var ;
		for ( int i = 0 ; i < 3 ; i++ )
		{
			pRange = NULL ;
			pRange = pSheet->GetRange( (_variant_t)strCellName[i] ) ;//, (_variant_t) strCellName[i] );
			range_var = pRange->GetText() ;
			pRange = NULL ;
			CString strVar ;
			strVar = (CString ) range_var.bstrVal ;
			strVar.TrimLeft() ;
			strVar.TrimRight() ;
			if( strVar != strCellContent[i] )
			{
				bCheckFile = FALSE ;
				strMsg.Format(_T("格式不符合：%s不是%s"),strCellName[i], strCellContent[i]) ;
				AfxMessageBox(strMsg) ;
				break ;
			}
		}
		
		if (bCheckFile)
		{
			int iCur_Row = 2 ; //起始行
			CString strCellName ;
			CString strXhl = _T(""); //信号楼端
			CString strDev = _T(""); //设备名称
			CString strCore = _T(""); //芯线名称
			CString strFx = _T(""); // 分线端子

			CString strTmp ;
			BOOL bNext = TRUE ; 

			while (bNext) 
			{			
				//信号楼端
				strCellName.Format(_T("B%d"), iCur_Row) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				range_var = pRange->GetText() ;
				strDev = range_var.bstrVal ;
				strDev.TrimLeft() ;
				strDev.TrimRight() ;
				if (strDev=="") 
				{
					bNext = FALSE ;
					continue ;
				}
				// 				//设备名称
				// 				strCellName.Format(_T("B%d"), iCur_Row) ;
				// 				pRange = NULL ;
				// 				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				// 				range_var = pRange->GetText() ;
				// 				strDev = range_var.bstrVal ;
				// 				strDev.TrimLeft() ;
				// 				strDev.TrimRight() ;

				//芯线名称
				strCellName.Format(_T("C%d"), iCur_Row) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				range_var = pRange->GetText() ;
				strCore = range_var.bstrVal ;
				strCore.TrimLeft() ;
				strCore.TrimRight() ;

				//分线端子
				strCellName.Format(_T("D%d"), iCur_Row) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				range_var = pRange->GetText() ;
				strFx = range_var.bstrVal ;
				strFx.TrimLeft() ;
				strFx.TrimRight() ;
				CString strShelf = _T(""), strLayer = _T(""), strBox = _T(""), strCell = _T("") ;
				CStringArray strArrayTmp ;
				CUserSystem mysys ;
				CBlkUtility::DivideString(strFx, _T("-"), strArrayTmp) ;
				if (strArrayTmp.GetSize()==3)
				{
					strShelf = strArrayTmp.GetAt(0).Mid(1) ;
					strLayer = strArrayTmp.GetAt(1).Left(strArrayTmp.GetAt(1).GetLength()-2) ;
					strBox = strArrayTmp.GetAt(1).Right(1) ;
					strCell = strArrayTmp.GetAt(2) ;

					int iShelf = 0, iLayer = 0, iBox = 0, iCell = 0 ;
					iShelf = _tstoi(strShelf) ;
					iLayer = _tstoi(strLayer) ;
					iBox = _tstoi(strBox) ;
					iCell = _tstoi(strCell) ;

					strExec.Format(_T("insert into fx_data(xhl_node, dev_name, core_name, shelf_num, layer_num, box_num, cell_num, fx_name) values('%s','%s','%s', %d, %d, %d, %d,'%s')"), strXhl, strDev, strCore, iShelf, iLayer, iBox, iCell, strFx) ;
					//ptrCon->Execute((_bstr_t) strExec, NULL, NULL) ;
					ExecSqlString(strExec) ;

				}
				else
				{
					CString strMsg ;
					strMsg.Format(_T("分线数据导入表格第%d行数据未导入成功，请检查该行分线端子是否合乎要求，或联系开发者！"), iCur_Row) ;
					AfxMessageBox(strMsg) ;
				}

				iCur_Row++ ;
				iReturn = iCur_Row-2 ;
			}
		}
		else
		{
			AfxMessageBox( _T("选择的文件不符合文件格式") ) ;			
		}

		//////////////////////////////////////////////////////////////////////////
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	
	return iReturn ;

}

//计算分线盘数据
void CCableNetForJY::ComputeFxData(long iXhlHandle)
{
	try	
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin 取得信号楼端的名称，例如DC-130
		CString strXhl = _T("") ;
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iXhlHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			cSet.MoveFirst() ;
			cSet.GetFieldValue(_T("devname"), strXhl) ;
		}
		cSet.Close() ;
		//end 取得信号楼端的名称，例如DC-130

		//begin 取得所有属于该信号楼的芯线
		strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d order by pt_y desc"), iXhlHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF())
		{
			CString strFxName = _T("0") ; //计算结果，分线端子

			CString strDevName = _T("") ;
			CString strDevNameTmp = _T("") ;
			CString strCoreName = _T("") ;
			int iIsShare = 0 ;
			cSet.GetFieldValue(_T("dev_name"), strDevName) ;
			cSet.GetFieldValue(_T("core_name"), strCoreName) ;
			cSet.GetFieldValue(_T("is_share"), iIsShare) ;
			strDevNameTmp = strDevName ;
			strDevNameTmp.TrimRight(_T("-A")) ;
			strDevNameTmp.TrimRight(_T("-B")) ;
// 			if (iIsShare>0)
// 			{
// 				strDevNameTmp = _T("") ;
// 			}

			
			//直接查找
			CADORecordset cSetFoundFx ;
			int iFoundFx = -1 ;

			// 			strSql.Format(_T("select * from fx_data where xhl_node='%s' and dev_name='%s' and core_name='%s'"), strXhl, strDevNameTmp, strCoreName) ;

			strSql.Format(_T("select * from fx_data where (dev_name='%s' or [dev_name]&[dev_type]='%s') and core_name='%s'"), strDevNameTmp, strDevNameTmp, strCoreName) ;
			if (cSetFoundFx.IsOpen())
			{
				cSetFoundFx.Close() ;
			}
			cSetFoundFx.Open(g_PtrCon, strSql) ;
			if (cSetFoundFx.GetRecordCount()>0)
			{
				cSetFoundFx.MoveFirst() ;

				int iShelfNum = 0 ;
				int iLayerNum = 0 ;
				int iBoxNum = 0 ;
				int iCellNum = 0 ;
				cSetFoundFx.GetFieldValue(_T("shelf_num"), iShelfNum) ;
				cSetFoundFx.GetFieldValue(_T("layer_num"), iLayerNum) ;
				cSetFoundFx.GetFieldValue(_T("box_num"), iBoxNum) ;
				cSetFoundFx.GetFieldValue(_T("cell_num"), iCellNum) ;

				strFxName.Format(_T("F%d-%d0%d-%d"), iShelfNum, iLayerNum, iBoxNum, iCellNum) ;	
				if (strCoreName==_T("QK") || strCoreName==_T("QKH") || strCoreName==_T("DH")|| strCoreName==_T("DHH")|| strCoreName==_T("DS1")|| strCoreName==_T("DS2"))
				{
					strFxName.Format(_T("F%d-D%d-%d"), iShelfNum, iBoxNum, iCellNum) ;
				}
			
			}
			cSetFoundFx.Close() ;
			
			//没有直接找到，找起始端子，再计算分线端子(如果是共享端子最后直接指定fx)
			if (strFxName==_T("0") && iIsShare==0)
			{
				//here 20100412-1705
				strSql.Format(_T("select * from cablenet_ent where devname='%s'"), strDevName) ;
				CADORecordset cSetFound ;
				CString strDevType = _T("") ;
				CString strDevFor = _T("") ;
				if (cSetFound.IsOpen())
				{
					cSetFound.Close() ;
				}
				cSetFound.Open(g_PtrCon, strSql) ;
				if (cSetFound.GetRecordCount()>0)
				{
					cSetFound.MoveFirst() ;

					//20110504:define_dev表中dev_type变成JZA_5_0_JH形式

// 					CString strDevTypeTmp = _T("") ;
					cSetFound.GetFieldValue(_T("devtype"), strDevType) ;
					cSetFound.GetFieldValue(_T("dev_for"), strDevFor) ;
// 					int iFoundMark = strDevTypeTmp.Find(_T("_")) ;
// 					strDevType = strDevTypeTmp ;
// 					if (iFoundMark!=-1)
// 					{
// 						strDevType = strDevTypeTmp.Left(iFoundMark) ;
// 					}

					
				}
				cSetFound.Close() ;

				//找到该类型设备所用全部芯线
				if (!strDevType.IsEmpty())
				{
					strSql.Format(_T("select * from define_dev_use_cablecore where dev_id=(select id from define_dev where dev_type='%s' and dev_for='%s')"), strDevType, strDevFor) ;
					if (cSetFound.IsOpen())
					{
						cSetFound.Close() ;
					}
					cSetFound.Open(g_PtrCon, strSql) ;
					if (cSetFound.GetRecordCount()>0)
					{
						cSetFound.MoveFirst() ;
						CString strFirstFxName = _T("") ; //起始分线端子名称
						int iFxPlus = 0 ;
						
						CString strFirstCoreName = _T("") ; //起始芯线名称
						cSetFound.GetFieldValue(_T("core_name"), strFirstCoreName) ;
						CString strFind = _T("") ;
						strFind.Format(_T("core_name='%s'"), strCoreName) ;
						if(cSetFound.Find(strFind))
						{
							cSetFound.GetFieldValue(_T("fx_plus"), iFxPlus) ;
						}
						
						//查找该设备的分线盘起始端子号
						//strSql.Format(_T("select * from fx_data where xhl_node='%s' and dev_name='%s' and core_name='%s'"), strXhl, strDevNameTmp, strFirstCoreName) ;
						strSql.Format(_T("select * from fx_data where (dev_name='%s' or [dev_name]&[dev_type]='%s') and core_name='%s'"), strDevNameTmp, strDevNameTmp, strFirstCoreName) ;
						CADORecordset cSetFoundFirstFx ;
						if (cSetFoundFirstFx.IsOpen())
						{
							cSetFoundFirstFx.Close() ;
						}
						cSetFoundFirstFx.Open(g_PtrCon, strSql) ;
						if (cSetFoundFirstFx.GetRecordCount()>0)
						{
							cSetFoundFirstFx.MoveFirst() ;

							int iShelfNum = 0 ;
							int iLayerNum = 0 ;
							int iBoxNum = 0 ;
							int iCellNum = 0 ;
							cSetFoundFirstFx.GetFieldValue(_T("shelf_num"), iShelfNum) ;
							cSetFoundFirstFx.GetFieldValue(_T("layer_num"), iLayerNum) ;
							cSetFoundFirstFx.GetFieldValue(_T("box_num"), iBoxNum) ;
							cSetFoundFirstFx.GetFieldValue(_T("cell_num"), iCellNum) ;

							strFirstFxName.Format(_T("F%d-%d0%d-%d"), iShelfNum, iLayerNum, iBoxNum, iCellNum) ;		
							//	cSetFoundFirstFx.GetFieldValue(_T("fx_name"), strFirstFxName) ;
						}
						cSetFoundFirstFx.Close() ;

						if (!strFirstFxName.IsEmpty())
						{
							CStringArray strArrayFxName ;
							CUserSystem mysys ;
							CBlkUtility::DivideString(strFirstFxName, _T("-"), strArrayFxName) ;
							int iPlus1 = iFxPlus/6 ;
							int iPlus2 = iFxPlus%6 ;
							if (strArrayFxName.GetAt(1).Left(1)==_T("D"))
							{
								int iLastNum = _tstoi(strArrayFxName.GetAt(2)) +iPlus2 ;
								strFxName.Format(_T("%s-%s-%d"), strArrayFxName.GetAt(0), strArrayFxName.GetAt(1), iLastNum) ;
							}
							else
							{
								CString strMid = strArrayFxName.GetAt(1) ;
								int iLastNum = _tstoi(strArrayFxName.GetAt(2)) + iPlus2 ;
								CString strMidPre = strMid.Left(strMid.GetLength()-2) ;
								CString strMidBack = strMid.Right(1) ;
								int iMidBack = _tstoi(strMidBack) + iPlus1 ;
								int iMidPlus1 = iMidBack/10 ;
								int iMidPlus2 = iMidBack%10 ;
								int iMidPre = _tstoi(strMidPre) + iMidPlus1 ;
								strFxName.Format(_T("%s-%d0%d-%d"), strArrayFxName.GetAt(0), iMidPre, iMidBack==10?1:iMidBack, iLastNum) ;
							}							
						}
					}
					cSetFound.Close() ;
				}

			}

			//综合端子 FS FSH JS JSH
			if (strCoreName==_T("FS")||strCoreName==_T("FSH")||strCoreName==_T("JS")||strCoreName==_T("JSH"))
			{
				strSql.Format(_T("select * from zh_place where gd_name='%s' and type='%s'"), strDevName, strCoreName.Left(2)) ;
				if (cSetFoundFx.IsOpen())
				{
					cSetFoundFx.Close() ;
				}
				if(cSetFoundFx.Open(g_PtrCon, strSql))
				{
					if (cSetFoundFx.GetRecordCount()>0)
					{
						cSetFoundFx.MoveFirst() ;
						
						int iShelfNum = 0 ;
						int iLayerNum = 0 ;
						int iBoxNum = 0 ;
						cSetFoundFx.GetFieldValue(_T("shelf_num"), iShelfNum) ;
						cSetFoundFx.GetFieldValue(_T("layer_num"), iLayerNum) ;
						cSetFoundFx.GetFieldValue(_T("box_num"), iBoxNum) ;
						int iPlus1 = 0 ;
						int iPlus2 = 0 ;
						if (strCoreName.Left(2)==_T("FS"))
						{
							iPlus1 = 1 ;
						}
						else
						{
							iPlus1 = 2 ;
						}
						if (strCoreName.Right(1)==_T("H"))
						{
							iPlus2 = 2 ;
						}
						strFxName.Format(_T("ZH%d-D%d-%d"), iShelfNum, 10-iLayerNum, ((iBoxNum-1)/2)*4+iPlus1+iPlus2) ; 				
					}
					cSetFoundFx.Close() ;
				}				
			}

			cSet.Edit() ;
			cSet.SetFieldValue(_T("start_num"), strFxName) ;
			cSet.Update() ;

			cSet.MoveNext() ;
		}
		cSet.Close() ;
		//end 取得所有属于该信号楼的芯线
		

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeFxData")) ;
	} 

}


//多个如楼端画到一张图
void CCableNetForJY::DrawPx(int iIndex, int iDwgSum, AcDbDatabase *pCurDb, long iXhlHandle, double dCurTotalYForOneDwg)
{
	try
	{
		CString strBlockFile ;
		strBlockFile.Format(_T("%s\\support\\cablenet\\QT\\DZ.DWG"), m_strSysBasePath) ;
	
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;
		AcDbObjectId textStyleId ;
		textStyleId = mysys.CreateTextStyleByFileName(_T("ZhfDlpx"), _T("txt.shx"), _T("hztxt.shx"), 2, 0.8) ;

		CADORecordset cSet ;
		CString strSql ;
		
		//begin 绘制芯线
		strSql.Format(_T("select * from cable_core_draw")) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF())
		{
			CString strDevName, strDevNameTmp, strCoreName, strCoreSn ;
			double dPt_x = 0 ;
			double dPt_y = 0 ;
			CString strStartNum = _T("") ;
			CString strEndNum = _T("") ;
			int iLeftOrRight = 0 ;
			int iIsNew = 0 ;
			int iIsLast = 0 ;
			int iIsVirtual = 0 ;
			long iBoxHandle = 0 ;
			CString strDevType = _T("") ;//芯线所在的设备类型
			CString strTmp ;

			cSet.GetFieldValue(_T("dev_name"), strDevName) ;
			cSet.GetFieldValue(_T("core_name"), strCoreName) ;
			cSet.GetFieldValue(_T("belong_box_handle_low"),iBoxHandle) ;
			cSet.GetFieldValue(_T("core_sn"), strCoreSn) ;
			cSet.GetFieldValue(_T("pt_x"), dPt_x) ;
			cSet.GetFieldValue(_T("pt_y"), dPt_y) ;
			cSet.GetFieldValue(_T("start_num"), strStartNum) ;
			cSet.GetFieldValue(_T("end_num"), strEndNum) ;
			cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
			cSet.GetFieldValue(_T("is_new"), iIsNew) ;
			cSet.GetFieldValue(_T("is_last"), iIsLast) ;
			cSet.GetFieldValue(_T("is_virtual"), iIsVirtual) ; //虚拟芯不用绘制,但是要绘制末端编号	

			strDevNameTmp = strDevName ;
			strDevNameTmp.TrimRight(_T("-A")) ;
			strDevNameTmp.TrimRight(_T("-B")) ;

			//取得设备类型
			strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
			CADORecordset cSetFindDevType ;
			if (cSetFindDevType.IsOpen())
			{
				cSetFindDevType.Close() ;
			}
			cSetFindDevType.Open(g_PtrCon, strSql) ;
			if (cSetFindDevType.GetRecordCount()>0)
			{
				cSetFindDevType.MoveFirst() ;			

				cSetFindDevType.GetFieldValue(_T("devtype"), strDevType) ;
			}
			cSetFindDevType.Close() ;
			
			dPt_y-=dCurTotalYForOneDwg ;

			AcGePoint3d pt_3d_start(dPt_x, dPt_y, 0) ;
			AcGePoint3d pt_3d_end(pt_3d_start) ;
			AcGePoint3d pt_3d_text(pt_3d_start.x, pt_3d_start.y+1, 0) ;
			AcDb::TextHorzMode iHorMode = AcDb::kTextLeft ;
			
			if (iLeftOrRight==1)
			{
				pt_3d_start.x+=m_dCellWidth/2 ;
				pt_3d_end.x-=m_dCellWidth/2 ;
				pt_3d_text.x = pt_3d_start.x-3 ;
				
				iHorMode = AcDb::kTextRight ;

			}
			else 
			{
				pt_3d_start.x-=m_dCellWidth/2 ;
				pt_3d_end.x+=m_dCellWidth/2 ;
				pt_3d_text.x = pt_3d_start.x+3 ;

			}

			if (iIsVirtual==0)
			{
				if (dPt_x!=0) //dPt_x不为0表示不是信号楼内的芯线
				{
					//芯线
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
				//端子
				AcDbBlockReference *pRef = NULL ;
				if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_start,0, 1) )
				{
					pRef->close() ;
				}
				if (iIsLast==1)
				{
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_end,0, 1) )
					{
						pRef->close() ;
					}
				}
			}

			
			//
			ads_point base_pt_text = {0,0,0} ;
			//AcDbObjectId objID ;
			//端子号
			if ((iIsNew==1 && iIsVirtual==0) || dPt_x==0)
			{
				base_pt_text[0] = pt_3d_start.x ;
				base_pt_text[1] = pt_3d_start.y+1 ;
				//strTmp.Format(_T("%s"), strStartNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ; //起点
				//mysys.WriteText(base_pt_text, strStartNum, objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ; //起点
				mysys.AddText(strStartNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;//起点

			}

			if (dPt_x!=0)
			{
				base_pt_text[0] = pt_3d_end.x ;
				base_pt_text[1] = pt_3d_end.y+1 ;
				//strTmp.Format(_T("%d"), iEndNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ;//末点
				//mysys.WriteText(base_pt_text, strEndNum, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//末点
				mysys.AddText(strEndNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//末点
			}
			if (iIsVirtual==0)
			{
				base_pt_text[0] = pt_3d_start.x+(pt_3d_end.x-pt_3d_start.x)/2+(pt_3d_start.x>pt_3d_end.x?-2:2) ;			
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode) ; //芯线名
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
				mysys.AddText(strCoreSn, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
				
				base_pt_text[0] = pt_3d_text.x ;
				base_pt_text[1] = pt_3d_text.y ; 
	
				if (dPt_x!=0)
				{
					strTmp.Format(_T("%s-%s"), strDevNameTmp, strCoreName) ;
					// 					if (strDevType.Left(2)==_T("JZ"))
					// 					{
					// 						CString strDevNameTmp = strDevName ;
					// 						strDevNameTmp.TrimRight(_T("-A")) ;
					// 						strDevNameTmp.TrimRight(_T("-B")) ;
					// 						strTmp.Format(_T("%s-%s"), strDevNameTmp, strCoreName) ;
					//					}
					// 					else if (strDevType.Left(1)==_T("J") || strDevType.Left(1)==_T("F"))
					// 					{
					// 						strTmp.Format(_T("%s%s"), strDevNameTmp, strCoreName) ;
					// 					}
					if (strCoreName.Left(1) == _T("J") || strCoreName.Left(1) == _T("F") )
					{
						strTmp.Format(_T("%s%s"), strDevNameTmp, strCoreName) ;
					}
					//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, iHorMode) ;
					//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
					mysys.AddText(strTmp, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
					
				}

			}


			cSet.MoveNext() ;
		}

		cSet.Close() ;
		//end 绘制芯线
		
		//begin 绘制title和边框
		strSql.Format(_T("select * from cablenet_ent where belong_xhl_handle_low=%d"), iXhlHandle) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}
		while (!cSet.IsEOF())
		{
			CString strDevName, strBoxName, strCableString, strBackupString ;
			CString strDevType = _T("") ;
			CString strTmp ;
			int iLeftOrRight = 0 ;
			double dTitle_x = 0 ;
			double dTitle_y = 0 ;
			double dBottom_x = 0 ;
			double dBottom_y = 0 ;

			dTitle_y-=dCurTotalYForOneDwg ;
			cSet.GetFieldValue(_T("devname"), strDevName) ;
			cSet.GetFieldValue(_T("boxtype"), strTmp) ;

			strBoxName = strTmp ; //.Left(strTmp.Find(_T("_"))) ;
			//strBoxName.MakeUpper() ;
			cSet.GetFieldValue(_T("cablestring"), strCableString) ;
			cSet.GetFieldValue(_T("backup_string"), strBackupString) ;
			cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
			cSet.GetFieldValue(_T("px_title_x"), dTitle_x) ;
			cSet.GetFieldValue(_T("px_title_y"), dTitle_y) ;
			cSet.GetFieldValue(_T("px_bottom_x"), dBottom_x) ;
			cSet.GetFieldValue(_T("px_bottom_y"), dBottom_y) ;
			cSet.GetFieldValue(_T("devtype"), strDevType) ;

			dTitle_y-=dCurTotalYForOneDwg ;
			dBottom_y-=dCurTotalYForOneDwg ;


			int iPlusX = iLeftOrRight==0?-1:1 ;
			AcDb::TextHorzMode iHorMode = AcDb::kTextLeft ;
			AcDb::TextVertMode iVerMode = AcDb::kTextVertMid ;

			//AcGePoint3d pt_3d_text(0,0,0) ;
			ads_point pt_base_text = {0,0,0} ;
			pt_base_text[1] = dTitle_y+m_dCellHight ;
			pt_base_text[0] = iLeftOrRight==0?dTitle_x-m_dCellWidth:dTitle_x ;
			//AcCmColor clr ;
			//clr.setColorIndex(0) ;
			//AcDbObjectId objID ;

			if (strBoxName==_T("XHL"))
			{
				pt_base_text[0] = pt_base_text[0]+m_dCellWidth/2 ;
				iHorMode = AcDb::kTextCenter ;				
				//mysys.WriteText(pt_base_text, strDevName, 0, iHorMode,2,0.8, clr) ;				
				//mysys.WriteText(pt_base_text, strDevName,objID, 2, 0.8,iHorMode, iVerMode) ;
				if (iIndex==0)
				{
					//mysys.WriteText(pt_base_text, _T("信号楼"), objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
					mysys.AddText(_T("信号楼"), pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
				}
				if (iIndex==iDwgSum-1)
				{
					mysys.AddAcDb2dPolyline(AcGePoint3d(0,0,0), AcGePoint3d(0,dBottom_y,0), 0.5) ; //信号楼封口线
				}

				pt_base_text[0]  = pt_base_text[0] + m_dCellWidth*(iLeftOrRight==0?-1:1) ;
				pt_base_text[1] += m_dCellHight*2 ;
				strDevName+=_T("#") ;
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

			}
			else
			{
				pt_base_text[0] = pt_base_text[0]+4 ;
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8,iHorMode, iVerMode) ;
				if ((strDevType.Left(1)==_T("J") && strDevType.Left(2)!=_T("JZ")) || strDevType.Left(1)==_T("F"))
				{
					int iFindLoc = strDevType.Find(_T("_")) ;
					if(iFindLoc!=-1)
						{
							strDevName+=strDevType.Left(iFindLoc) ;
						}						
				}
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

				iHorMode = AcDb::kTextRight ;
				pt_base_text[0] = pt_base_text[0]-4+m_dCellWidth-4 ;
				//mysys.WriteText(pt_base_text, strBoxName, objID, 2, 0.8,iHorMode, iVerMode) ;
				//mysys.WriteText(pt_base_text, strBoxName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strBoxName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
			}

			AcGePoint3d pt_3d_start(dTitle_x,dTitle_y,0) ;
			AcGePoint3d pt_3d_end(dTitle_x,dTitle_y,0) ;
			AcGePoint3d pt_3d_tmp(0,0,0) ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			if(!(iIndex!=0&&strBoxName==_T("XHL")))
			{
				mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5);//标题框下线
			}
			
			pt_3d_tmp.x = pt_3d_end.x ;
			pt_3d_tmp.y = dBottom_y ;			
			mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_tmp, 0.5) ;//左边框

			pt_3d_start.y = dBottom_y ;
			if (!(iIndex!=iDwgSum-1 && strBoxName==_T("XHL")))
			{
				mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_tmp, 0.5) ;//下边框
			}			

			pt_3d_start.x = dTitle_x ;
			pt_3d_start.y = dTitle_y+m_dCellHight*2 ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			pt_3d_end.y = pt_3d_start.y ;
			if (strBoxName!=_T("XHL") ||(strBoxName==_T("XHL") && iIndex==0) )
			{
				mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5) ;//标题框上线
			}			

			pt_3d_start.x = pt_3d_end.x ;
			pt_3d_start.y = dTitle_y ;
			mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_start, 0.5) ; //标题框左线

			if (strBoxName==_T("XHL") && iIndex!=0)
			{
				pt_3d_end.y+=m_dCellHight*2 ;
				mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_start, 0.5) ; 
			}
		
			if (strCableString!="")
			{
				pt_3d_start.x = dBottom_x - 10 ;
				pt_3d_start.y = dBottom_y + 5;
				pt_3d_end.x = dBottom_x + 10 ;
				pt_3d_end.y = pt_3d_start.y ;
				mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				pt_base_text[0] = dBottom_x + iPlusX  ;
				pt_base_text[1] = dBottom_y + 6 ;
				iHorMode = iLeftOrRight==0?AcDb::kTextRight:AcDb::kTextLeft ;
				//strCableString.Insert(strCableString.Find(_T("-"))+1, " ") ;
				//strCableString+=" " ;
				//strCableString.Insert(0, "  ") ;
				//mysys.WriteText(pt_base_text, strCableString, 0, iHorMode,2,0.8, clr) ;	
				//mysys.WriteText(pt_base_text, strCableString, objID, 2, 0.8, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
				mysys.AddText(strCableString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;
				pt_base_text[1] = dBottom_y+4 ;
				//AcDbObjectId objId ;
				//mysys.WriteText(pt_base_text, strBackupString, objId, 2, 0.8, iHorMode, AcDb::kTextTop, 0, NULL, 0) ;
				//mysys.WriteText(pt_base_text, strBackupString, objID, 2, 0.8, iHorMode, AcDb::kTextTop, 0,textStyleId, 0) ;
				mysys.AddText(strBackupString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
				pt_base_text[0] = dBottom_x - iPlusX*5 ;
				//mysys.WriteText(pt_base_text, _T("备用"), objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextTop, 0, textStyleId, 0) ;
				mysys.AddText(_T("备用"), pt_base_text, pt_base_text, AcDb::kTextCenter, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;

			}
			//here

			cSet.MoveNext() ;
		}
		cSet.Close() ;
		//end 绘制title和边框
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawPx2")) ;
	} 


}

//针对多个入楼端子画在一张图上的情况
void CCableNetForJY::ComputeCoreAndDraw2()
{
	CUserSystem mysys ;

	try
	{				
		CADORecordset cSet ;
		CString strSql ;
		AcDbDatabase *pCurDb = new AcDbDatabase ;

		//begin 一个信号楼ent产生一张图纸，循环产生所有图纸
		//////////////////////////////////////////////////////////////////////////

		strSql.Format(_T("SELECT distinct belong_xhl_handle_low FROM cablenet_ent")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (!cSet.IsBOF())
		{
			cSet.MoveFirst() ;
		}

		int iRsCount = cSet.GetRecordCount() ;

		CString strBarMsg ;
		strBarMsg.Format(_T("电缆配线:%d张"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;

		int iIndex = 0 ;
		double dCurTotalYForOneDwg = 0 ; //绘制到一张图纸时，记录每个块绘制时的Y坐标		

		while (!cSet.IsEOF())
		{
			//先清空表
			CDBUtility::ClearTable(_T("cable_core_draw")) ;

			acedSetStatusBarProgressMeterPos(iIndex) ;			

			long iXHL_handle_low = 0 ;			
			cSet.GetFieldValue(_T("belong_xhl_handle_low"), iXHL_handle_low) ;			

			//begin 遍历以iXHL_handle_low为终点的所有起点，然后以该起点往信号楼方向走一遍，计算芯线数据
			strSql.Format(_T("select * from cablenet_ent where is_start=1 and belong_xhl_handle_low=%d order by pt_y desc"), iXHL_handle_low) ;
			CADORecordset cSet_AllStart ;
			cSet_AllStart.Open(g_PtrCon, strSql) ;
			if (!cSet_AllStart.IsBOF())
			{
				cSet_AllStart.MoveFirst() ;
			}
			//每张配线图的Y坐标从0开始
			double dCurTotalY = 0 ;
			int iAddYMode = 0 ; //此次Y的增加是增加芯线/0，还是增加底边框/1
			BOOL bIsFirstRoute = TRUE ;

			//被遍历的次数
			long iScan_Time = -1 ;
			
			while (!cSet_AllStart.IsEOF())
			{
				if (bIsFirstRoute)
				{
					dCurTotalY-=m_dCellHight*2 ;
					bIsFirstRoute = FALSE ;
				}
				else
				{
					dCurTotalY-=m_dCellHight*3 ;
				}
				double dCurTitleY = dCurTotalY ;

				long iStartHandle_Low = 0 ;
				int iLeftORight = -1 ;		
				cSet_AllStart.GetFieldValue(_T("handle_low"), iStartHandle_Low) ;				
				cSet_AllStart.GetFieldValue(_T("left_or_right"), iLeftORight) ;				

				CLongArray iArrayHandle_OneRoute ;
				this->FindOneRoute(iStartHandle_Low, iArrayHandle_OneRoute) ;
				double dCurX = 0 ; //在每条径路（从起始到信号楼）遍历过程中的x坐标
				double dPlusX = (iLeftORight==1?1:-1) ;//左边从信号楼往左x坐标是减少，右边反之
				int iEntNum = iArrayHandle_OneRoute.GetSize() ;
				//begin for 遍历一条径路中的每一个ent（box、hz）计算其使用电缆芯线
				for (int i=0; i<iEntNum; i++)
				{
					long iHandleTmp = iArrayHandle_OneRoute.GetAt(i) ;

					strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iHandleTmp) ;
					CADORecordset cSetOneEntInRoute ;
					if (cSetOneEntInRoute.IsOpen())
					{
						cSetOneEntInRoute.Close() ;
					}
					cSetOneEntInRoute.Open(g_PtrCon, strSql) ;
					cSetOneEntInRoute.MoveFirst() ;
					
					CString strDevType = _T("") ;
					CString strDevName = _T("") ;
					CString strDevFor = _T("") ;
					long iBranch_num = 0 ;
// 					if (i==0&&iHandleTmp==351)
// 					{
// 						int iiiii = 0 ;
// 					}
					cSetOneEntInRoute.GetFieldValue(_T("scan_time"), iScan_Time) ;
					cSetOneEntInRoute.GetFieldValue(_T("devtype"), strDevType) ;
					cSetOneEntInRoute.GetFieldValue(_T("devname"), strDevName) ;
					cSetOneEntInRoute.GetFieldValue(_T("dev_for"), strDevFor) ;
					cSetOneEntInRoute.GetFieldValue(_T("branch_num"), iBranch_num) ;

					//begin if 没有被遍历过，第一次遍历要设置title，并为此盒对应设备产生新芯线,若不是起始，则还要加上延续芯线
					if (iScan_Time==0)
					{
						//title的坐标
						double dTitleX = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
						double dTitleY = dCurTitleY ;

						cSetOneEntInRoute.Edit() ;
						cSetOneEntInRoute.SetFieldValue(_T("px_title_x"), dTitleX) ;
						cSetOneEntInRoute.SetFieldValue(_T("px_title_y"), dTitleY) ;
						cSetOneEntInRoute.Update() ;
						
						//盒子有对应设备，不是分向盒，则必定有引出的电缆
						if (strDevType!="")
						{
							//20110504:define_dev表中dev_type由类似JZA变为类似JZA_5_0_JH
// 							int iFindMark = strDevType.Find(_T("_")) ;
// 							if (iFindMark!=-1)
// 							{
// 								strDevType = strDevType.Left(iFindMark) ;
// 							}
							CADORecordset cSetGetDev/*, cSetGetCoredetail*/ ;
							//查表获取该类型设备使用的芯线
							strSql.Format(_T("select * from define_dev where dev_type='%s' and dev_for='%s'"), strDevType, strDevFor) ;
							if (cSetGetDev.IsOpen())
							{
								cSetGetDev.Close() ;
							}
							cSetGetDev.Open(g_PtrCon, strSql) ;
							if (cSetGetDev.GetRecordCount()>0)
							{
								cSetGetDev.MoveFirst() ;
								int iDevId = -1 ;
								cSetGetDev.GetFieldValue(_T("id"), iDevId) ;
								CADORecordset cSetGetCoreDetail ;
								strSql.Format(_T("select * from define_dev_use_cablecore where dev_id=%d"), iDevId) ;
								if (cSetGetCoreDetail.IsOpen())
								{
									cSetGetCoreDetail.Close() ;
								}
								cSetGetCoreDetail.Open(g_PtrCon, strSql) ;
								if (cSetGetCoreDetail.GetRecordCount()>0)
								{
									cSetGetCoreDetail.MoveFirst() ;
									while (!cSetGetCoreDetail.IsEOF())
									{
										CString strCoreName = _T("") ; //芯线名称，例：DH DHH
										CString strCoreSn = _T("") ; //芯线编号,例如：D1-1 II2-3
										int iIsInside = 0 ;			//内部不需要引出的芯线，0/非内部，1/内部
										int iIsShare = 0 ;			//是不是共用线，比如DH DHH 0/不共用，1/共用
										CString strStartNum = _T("") ;			//芯线起点使用的盒子的端子
										CString strEndNum = _T("") ;			//芯线末点使用的盒子的端子
										int iIsVirtual = 0 ;	//0/非虚拟，1/虚拟芯，占位用
										double dCurCoreX = dTitleX ;
										double dCurCoreY = dCurTotalY ;
										int iIsNew = 1 ;

										cSetGetCoreDetail.GetFieldValue(_T("is_virtual"), iIsVirtual) ;

										cSetGetCoreDetail.GetFieldValue(_T("is_share"), iIsShare) ;
										if (iIsShare>0 && i>0) //如果是共用线，且不是第一个ent，则要检查是否前面链接的盒子里面已有，若有则不再增加该芯线，进入下一芯线
										{
											long iHandlePre = iArrayHandle_OneRoute.GetAt(i-1) ;
											strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iHandlePre,strCoreName) ;
											CADORecordset cSetFindExistCore ;
											cSetFindExistCore.Open(g_PtrCon, strSql) ;
											if (cSetFindExistCore.GetRecordCount()>0)
											{
												cSetFindExistCore.Close() ;
												cSetGetCoreDetail.MoveNext() ;
												continue ;
											}
											cSetFindExistCore.Close() ;
											//here 20100326
										}

										cSetGetCoreDetail.GetFieldValue(_T("is_inside"), iIsInside) ;
										if (iIsInside==1) //如果是纯内部线，则将采用默认的端点和电缆芯
										{
											cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
											cSetGetCoreDetail.GetFieldValue(_T("end_num_default"), strEndNum) ;
											cSetGetCoreDetail.GetFieldValue(_T("cable_core_sn_default"), strCoreSn) ;											
										}
										else if (iIsInside==2)//如果是半纯内部线，则将采用默认的起点
										{
											cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
										}
										cSetGetCoreDetail.GetFieldValue(_T("core_name"), strCoreName) ;

										BOOL bFoundExist = FALSE ;
										if ((strDevType.Left(3)==_T("JZA")||strDevType.Left(3)==_T("JLA")||strDevType.Left(3)==_T("CZA"))&& i>0)//如果是JZA则基本可以确定是第2个盒子，加上i>0起检验作用
										{
											//对于已经存在的芯线，延续
											strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iArrayHandle_OneRoute.GetAt(i-1), strCoreName) ;
											CADORecordset cSetFoundExist ;
											if (cSetFoundExist.IsOpen())
											{
												cSetFoundExist.Close() ;
											}
											cSetFoundExist.Open(g_PtrCon, strSql) ;
											if (cSetFoundExist.GetRecordCount()>0)
											{
												cSetFoundExist.MoveFirst() ;
												cSetFoundExist.GetFieldValue(_T("pt_y"), dCurCoreY) ;
												iIsNew = 0 ;
												bFoundExist = TRUE ;									
											}
											cSetFoundExist.Close() ;
										}
										if (!bFoundExist)
										{
											dCurTotalY-=m_dCellHight ;
											dCurCoreY = dCurTotalY ;
										}
										
										iAddYMode = 0 ;
										strSql.Format(_T("insert into cable_core_draw(dev_name, core_name, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_virtual) \
													  values('%s','%s',%d, %d, %d, %d, %f,%f, '%s', '%s', '%s', %d, %d)"), strDevName, strCoreName , iHandleTmp, iIsNew, iIsInside, iIsShare, dCurCoreX, dCurCoreY, strCoreSn, strStartNum, strEndNum, iLeftORight, iIsVirtual) ;
										ExecSqlString(strSql) ;
										
										cSetGetCoreDetail.MoveNext() ;
									}
								}
								cSetGetCoreDetail.Close() ;
							}
							cSetGetDev.Close() ;

						}
						
					}
					//end if 没有被遍历过，第一次遍历要设置title，并为此盒对应设备产生新芯线,若不是起始，则还要加上延续芯线

					
					//除起始box外，其他都要延续前一个盒子的芯线
					if(i>0) 
					{
						long iHandlePre = iArrayHandle_OneRoute.GetAt(i-1) ;
						// 						if (iHandlePre==351)
						// 						{
						// 							int iiii = 0 ;
						// 						}
						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_inside<>1"), iHandlePre) ;
						CADORecordset cSetPreBoxCore ;
						cSetPreBoxCore.Open(g_PtrCon, strSql) ;
						if (!cSetPreBoxCore.IsBOF())
						{
							cSetPreBoxCore.MoveFirst() ;
						}
						//int iRsSum = cSetPreBoxCore.GetRecordCount() ;
						while (!cSetPreBoxCore.IsEOF())
						{
							CString strDevName, strCoreName, strCoreSn ;
							long iBelong_Handle = 0 ;						
							int iIsShare = 0 ;
							CString strStartNum = _T("") ;
							CString strEndNum = _T("") ;
							int iLeftOrRightTmp = -1 ;
							double dPt_x = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
							double dPt_y = 0 ;

							cSetPreBoxCore.GetFieldValue(_T("dev_name"), strDevName) ;
							cSetPreBoxCore.GetFieldValue(_T("core_name"), strCoreName) ;
							cSetPreBoxCore.GetFieldValue(_T("belong_box_handle_low"), iBelong_Handle) ;	
							cSetPreBoxCore.GetFieldValue(_T("is_share"), iIsShare) ;
							cSetPreBoxCore.GetFieldValue(_T("pt_y"), dPt_y) ;
							cSetPreBoxCore.GetFieldValue(_T("core_sn"), strCoreSn) ;
							cSetPreBoxCore.GetFieldValue(_T("start_num"), strStartNum) ;
							cSetPreBoxCore.GetFieldValue(_T("end_num"), strEndNum) ;
							cSetPreBoxCore.GetFieldValue(_T("left_or_right"), iLeftOrRightTmp) ;

							//对于已经存在的共享芯线，不再延续
							strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s' and is_share>0"), iArrayHandle_OneRoute.GetAt(i), strCoreName) ;
							CADORecordset cSetHasShared ;
							if (cSetHasShared.IsOpen())
							{
								cSetHasShared.Close() ;
							}
							cSetHasShared.Open(g_PtrCon, strSql) ;
							if (cSetHasShared.GetRecordCount()>0)
							{
								cSetHasShared.Close() ;
								cSetPreBoxCore.MoveNext() ;
								continue ;
							}
							cSetHasShared.Close() ;

							//对于已经存在的芯线，不再延续，跟共享芯线不一样
							//比如:共享芯线是只要有DH,就不再延续了，此处是即使有X1了，但不是同一个设备的X1，则一样要延续
							strSql.Format(_T("select * from cable_core_draw where [dev_name]='%s' and [core_name]='%s' and belong_box_handle_low=%d"), strDevName, strCoreName, iArrayHandle_OneRoute.GetAt(i)) ;
							CADORecordset cSetHasThisCore ;
							if (cSetHasThisCore.IsOpen())
							{
								cSetHasThisCore.Close() ;
							}
							cSetHasThisCore.Open(g_PtrCon, strSql) ;
							if (cSetHasThisCore.GetRecordCount()>0)
							{
								cSetHasThisCore.Close() ;
								cSetPreBoxCore.MoveNext() ;
								continue ;
							}
							cSetHasThisCore.Close() ;

							strSql.Format(_T("insert into cable_core_draw(dev_name,core_name, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right) \
										  values('%s','%s',%d, %d, %d, %d, %f,%f,'%s','%s', '%s', %d)"), strDevName, strCoreName, iHandleTmp, 0, 0, iIsShare, dPt_x, dPt_y, strCoreSn, strStartNum, strEndNum, iLeftOrRightTmp) ;
							ExecSqlString(strSql) ;

							cSetPreBoxCore.MoveNext() ;							
						}
						cSetPreBoxCore.Close() ;
						
					}

					//每遍历一次将scan_time加1
					iScan_Time++ ;
					//strSql.Format(_T("update cablenet_ent set scan_time=%d"), iScan_Time) ;	
					//ExecSqlString(strSql) ;
					cSetOneEntInRoute.Edit() ;					
					bool b3 = cSetOneEntInRoute.SetFieldValue(_T("scan_time"), iScan_Time) ;
					bool b1 = cSetOneEntInRoute.Update() ;			


					//若遍历次数和分支（branch）相同，则需要设置bottom_title的位置，单元格合拢
					if (iBranch_num==0 || iScan_Time==iBranch_num)
					{
						double dBottom_Title_x = (iEntNum-i-1)*dPlusX*m_dCellWidth ; 
						if (iAddYMode==0)
						{
							dCurTotalY-=m_dCellHight*2 ;
						}						
						iAddYMode = 1 ;
						cSetOneEntInRoute.Edit() ;
						cSetOneEntInRoute.SetFieldValue(_T("px_bottom_x"), dBottom_Title_x) ;
						cSetOneEntInRoute.SetFieldValue(_T("px_bottom_y"), dCurTotalY) ;
						cSetOneEntInRoute.Update() ;

					}
					cSetOneEntInRoute.Close() ;
				}
				//end for 遍历一条径路中的每一个ent（box、hz）计算其使用电缆芯线

				cSet_AllStart.MoveNext() ;
			}
			cSet_AllStart.Close() ;
			//end 遍历以iXHL_handle_low为终点的所有起点，然后以该起点往信号楼方向走一遍，计算芯线数据

			
			//begin 填充端子编号,芯线名称
			//////////////////////////////////////////////////////////////////////////
			CADORecordset cSetAllBox ;
			CString strFileName = _T("未知");

			strSql.Format(_T("select distinct belong_box_handle_low from cable_core_draw")) ;
			if(cSetAllBox.IsOpen())
			{
				cSetAllBox.Close() ;
			}
			cSetAllBox.Open(g_PtrCon, strSql) ;
			if (cSetAllBox.GetRecordCount()>0)
			{
				cSetAllBox.MoveFirst() ;
				
				while(!cSetAllBox.IsEOF())
				{
					long iBoxHandle = 0 ;
					cSetAllBox.GetFieldValue(_T("belong_box_handle_low"), iBoxHandle) ;

					//	int iBoxDzSum = 0 ; //该盒子端子总数 注意查询盒子如果是xhl，没有结果
					CString strBoxType = _T("") ;
					//begin 得到该盒子类型
					CADORecordset cSetGetBoxType ;
					strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
					if(cSetGetBoxType.IsOpen())
					{
						cSetGetBoxType.Close() ;
					}
					cSetGetBoxType.Open(g_PtrCon, strSql) ;
					if (cSetGetBoxType.GetRecordCount()>0)
					{
						cSetGetBoxType.MoveFirst() ;
						cSetGetBoxType.GetFieldValue(_T("boxtype"), strBoxType) ;
						if (strBoxType==_T("XHL")) //盒子是信号楼，则跳出继续下次循环
						{
							cSetGetBoxType.GetFieldValue(_T("devname"), strFileName) ;

							cSetGetBoxType.Close() ;
							cSetAllBox.MoveNext() ;
							continue ;
						}
					}
					cSetGetBoxType.Close() ;
					//end 得到该盒子类型

					//begin 得到盒子的box_type_id
					int iBoxTypeId = -1 ;
					CADORecordset cSetGetBoxTypeId ;
					//strBoxType.MakeUpper() ;
					strSql.Format(_T("select * from define_box where box_type='%s'"), strBoxType) ;
					if(cSetGetBoxTypeId.IsOpen())
					{
						cSetGetBoxTypeId.Close() ;
					}
					cSetGetBoxTypeId.Open(g_PtrCon, strSql) ;
					//	int ii = cSetBoxDzSum.GetRecordCount() ;
					if(cSetGetBoxTypeId.GetRecordCount()>0)
					{
						cSetGetBoxTypeId.MoveFirst() ;
						cSetGetBoxTypeId.GetFieldValue(_T("id"), iBoxTypeId) ;
						//	cSetGetBoxTypeId.GetFieldValue(_T("box_dz_num"), iBoxDzSum) ;
					}
					cSetGetBoxTypeId.Close() ;

					//end 得到盒子的box_type_id

					//先将该型号盒子的所有端子的is_used重置0					
					CADORecordset cSetUpdateBoxDetail ;
					strSql.Format(_T("select * from define_box_detail where box_type_id=%d"), iBoxTypeId) ;
					if(cSetUpdateBoxDetail.IsOpen())
					{
						cSetUpdateBoxDetail.Close() ;
					}
					cSetUpdateBoxDetail.Open(g_PtrCon, strSql) ;
					if (!cSetUpdateBoxDetail.IsBOF())
					{
						cSetUpdateBoxDetail.MoveFirst() ;
					}
					while (!cSetUpdateBoxDetail.IsEOF())
					{
						cSetUpdateBoxDetail.Edit() ;
						cSetUpdateBoxDetail.SetFieldValue(_T("is_used"), 0) ;
						cSetUpdateBoxDetail.Update() ;
						cSetUpdateBoxDetail.MoveNext() ;
					}
					cSetUpdateBoxDetail.Close() ;

					//here 20100401 
					
					//begin 处理所有共享芯线的端子
					//////////////////////////////////////////////////////////////////////////

					//begin 先将内部占用的端子标出
					//从前面盒子引入的端子
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in(select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_inside=1"), iBoxHandle) ;
					CADORecordset cSetFindDz ;
					if (cSetFindDz.IsOpen())
					{
						cSetFindDz.Close() ;
					}
					cSetFindDz.Open(g_PtrCon, strSql) ;
					if (!cSetFindDz.IsBOF())
					{
						cSetFindDz.MoveFirst() ;
					}
					while (!cSetFindDz.IsEOF())
					{
						CString strDzNode = _T("") ;
						cSetFindDz.GetFieldValue(_T("end_num"), strDzNode) ;
						strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
						ExecSqlString(strSql) ;

						cSetFindDz.MoveNext() ;
					}
					cSetFindDz.Close() ;
					//本盒子伸出的端子
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_inside>0"), iBoxHandle) ;
					if (cSetFindDz.IsOpen())
					{
						cSetFindDz.Close() ;
					}
					cSetFindDz.Open(g_PtrCon, strSql) ;
					if (!cSetFindDz.IsBOF())
					{
						cSetFindDz.MoveFirst() ;
					}
					while (!cSetFindDz.IsEOF())
					{
						CString strDzNode = _T("") ;
						cSetFindDz.GetFieldValue(_T("start_num"), strDzNode) ;
						strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
						ExecSqlString(strSql) ;
						
						cSetFindDz.MoveNext() ;
					}
					cSetFindDz.Close() ;
					//end 先将内部占用的端子标出

					//begin 计算共享芯线的端子号
					//typedef CMap<CString, LPCSTR, CString ,LPCSTR> CMapStrToInt ;
					//CMapStrToInt mapCoreNameToDzNum ; //for examp:<"DHH", 42>
					CMapStringToString mapCoreNameToDzNum ;
					CADORecordset cSetShareType ; 
					//将共享芯甄选出来，排序按照is_share的大小来，分配端子号时按排序来（从最后端子开始）
					strSql.Format(_T("select * from (select is_share, core_name from cable_core_draw where belong_box_handle_low \
								  in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share>0 and is_inside<>1 \
								  union select is_share, core_name from cable_core_draw where belong_box_handle_low=%d and is_share>0 and is_inside=0) order by is_share"), iBoxHandle, iBoxHandle) ;
					if(cSetShareType.IsOpen())
					{
						cSetShareType.Close() ;
					}
					cSetShareType.Open(g_PtrCon, strSql) ;
					if (cSetShareType.GetRecordCount()>0)
					{
						cSetShareType.MoveFirst() ;
						
						while (!cSetShareType.IsEOF())
						{
							CString strShareCoreName ;
							cSetShareType.GetFieldValue(_T("core_name"), strShareCoreName) ;

							int iFoundDz = -1 ;
							CString strCurDzNum = _T("") ;
							
							CADORecordset cSetDzDetail ;
							strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0 order by id desc"), iBoxTypeId) ;
							if (cSetDzDetail.IsOpen())
							{
								cSetDzDetail.Close() ;
							}
							cSetDzDetail.Open(g_PtrCon, strSql) ;
							if (cSetDzDetail.GetRecordCount()>0)
							{
								cSetDzDetail.MoveFirst() ;
								cSetDzDetail.GetFieldValue(_T("box_node"), strCurDzNum) ;

								cSetDzDetail.Edit() ;
								cSetDzDetail.SetFieldValue(_T("is_used"), 1) ;
								cSetDzDetail.Update() ;
								iFoundDz = 1 ;
							}
							else
							{
								iFoundDz = 0 ;
							}
							cSetDzDetail.Close() ;
							
							if (iFoundDz==1)
							{
								mapCoreNameToDzNum.SetAt(strShareCoreName, strCurDzNum) ;//for examp:<"DHH", 42>		
							}

							cSetShareType.MoveNext() ;
						}
					}
					cSetShareType.Close() ;
					//end 计算共享芯线的端子号

					//begin 先给从前一个盒子引入的共用芯线的末端指定端子号
					CADORecordset cSetPreShare ;
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low \
								  in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share>0 and is_inside<>1"), iBoxHandle) ;
					if(cSetPreShare.IsOpen())
					{
						cSetPreShare.Close() ;
					}
					cSetPreShare.Open(g_PtrCon, strSql) ;
					if (!cSetPreShare.IsBOF())
					{
						cSetPreShare.MoveFirst() ;
					}
					while (!cSetPreShare.IsEOF())
					{
						CString strCoreName ;
						CString strDzNum = _T("") ;
						cSetPreShare.GetFieldValue(_T("core_name"), strCoreName) ;
						mapCoreNameToDzNum.Lookup(strCoreName, strDzNum) ;
						cSetPreShare.Edit() ;
						cSetPreShare.SetFieldValue(_T("end_num"), strDzNum) ;
						cSetPreShare.Update() ;
						
						cSetPreShare.MoveNext() ;
					}
					cSetPreShare.Close() ;
					//end 先给从前一个盒子引入的共用芯线的末端指定端子号
					
					//begin 给属于本盒子的共享芯线起始端子指定端子号
					CADORecordset cSetCurBoxShare ;
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_share>0 and is_inside=0"), iBoxHandle) ;
					if(cSetCurBoxShare.IsOpen())
					{
						cSetCurBoxShare.Close() ;
					}
					cSetCurBoxShare.Open(g_PtrCon, strSql) ;
					if (!cSetCurBoxShare.IsBOF())
					{
						cSetCurBoxShare.MoveFirst() ;
					}
					while (!cSetCurBoxShare.IsEOF())
					{
						CString strCoreName ;
						CString strDzNum = _T("") ;
						cSetCurBoxShare.GetFieldValue(_T("core_name"), strCoreName) ;
						mapCoreNameToDzNum.Lookup(strCoreName, strDzNum) ;
						cSetCurBoxShare.Edit() ;
						cSetCurBoxShare.SetFieldValue(_T("start_num"), strDzNum) ;
						cSetCurBoxShare.Update() ;

						cSetCurBoxShare.MoveNext() ;
					}
					cSetCurBoxShare.Close() ;
					//end 给属于本盒子的共享芯线起始端子指定端子号

					//end 处理所有共享芯线的端子

					//begin 处理剩余的非共享芯线的端子:先处理该盒子新用的芯（写start端）+再处理从前面盒子引入的芯（写end端）
					CADORecordset cSetOtherCore ;
					// 					strSql.Format(_T("select * from \
					// 						(select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 \
					// 						union select * from cable_core_draw where belong_box_handle_low \
					// 						in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside=0) order by pt_y desc"), iBoxHandle,iBoxHandle) ;
					//本盒子新用芯
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0"), iBoxHandle) ;
					if (cSetOtherCore.IsOpen())
					{
						cSetOtherCore.Close() ;
					}
					cSetOtherCore.Open(g_PtrCon, strSql) ;
					if (!cSetOtherCore.IsBOF())
					{
						cSetOtherCore.MoveFirst() ;
					}
					while (!cSetOtherCore.IsEOF())
					{
						int iFoundDz = -1 ;
						CString strCurDzNum = _T("") ;

						CADORecordset cSetDzDetail ;
						strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0"), iBoxTypeId) ;
						if (cSetDzDetail.IsOpen())
						{
							cSetDzDetail.Close() ;
						}
						cSetDzDetail.Open(g_PtrCon, strSql) ;
						if (cSetDzDetail.GetRecordCount()>0)
						{
							cSetDzDetail.MoveFirst() ;
							cSetDzDetail.GetFieldValue(_T("box_node"), strCurDzNum) ;

							cSetDzDetail.Edit() ;
							cSetDzDetail.SetFieldValue(_T("is_used"), 1) ;
							cSetDzDetail.Update() ;
							iFoundDz = 1 ;
						}
						else
						{
							iFoundDz = 0 ;
						}
						cSetDzDetail.Close() ;

						if (iFoundDz==1)
						{
							cSetOtherCore.Edit() ;
							cSetOtherCore.SetFieldValue(_T("start_num"), strCurDzNum) ;
							cSetOtherCore.Update() ;
						}

						cSetOtherCore.MoveNext() ;
					}
					cSetOtherCore.Close() ;

					//从前面盒子引入的芯
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside<>1 order by pt_y desc"), iBoxHandle) ;
					if (cSetOtherCore.IsOpen())
					{
						cSetOtherCore.Close() ;
					}
					cSetOtherCore.Open(g_PtrCon, strSql) ;
					if (!cSetOtherCore.IsBOF())
					{
						cSetOtherCore.MoveFirst() ;
					}					
					while (!cSetOtherCore.IsEOF())
					{
						int iFoundDz = -1 ;
						CString strCurDzNum = _T("") ;
						
						CADORecordset cSetDzDetail ;
						strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0"), iBoxTypeId) ;
						if (cSetDzDetail.IsOpen())
						{
							cSetDzDetail.Close() ;
						}
						cSetDzDetail.Open(g_PtrCon, strSql) ;
						if (cSetDzDetail.GetRecordCount()>0)
						{
							cSetDzDetail.MoveFirst() ;
							cSetDzDetail.GetFieldValue(_T("box_node"), strCurDzNum) ;

							cSetDzDetail.Edit() ;
							cSetDzDetail.SetFieldValue(_T("is_used"), 1) ;
							cSetDzDetail.Update() ;
							iFoundDz = 1 ;
						}
						else
						{
							iFoundDz = 0 ;
						}
						cSetDzDetail.Close() ;
						
						if (iFoundDz==1)
						{
							cSetOtherCore.Edit() ;
							cSetOtherCore.SetFieldValue(_T("end_num"), strCurDzNum) ;
							cSetOtherCore.Update() ;
						}				

						cSetOtherCore.MoveNext() ;
					}
					cSetOtherCore.Close() ;


					//end 处理剩余的非共享芯线的端子,先处理该盒子新用的芯（写start端）+再处理从前面盒子引入的芯（写end端）

					//begin 标出哪些不再延续的芯线，cable_core_draw:is_last，is_last的端子要绘制end端子
					CADORecordset cSetIsLastCore ; 
					strSql.Format(_T("SELECT * FROM cable_core_draw WHERE [dev_name]&[core_name] \
								  in (select [dev_name]&[core_name] from cable_core_draw where belong_box_handle_low=%d) \
								  and [dev_name]&[core_name] not in (select [dev_name]&[core_name] from cable_core_draw where \
								  belong_box_handle_low=(select next_ent_handle_low from cablenet_ent where handle_low=%d))"), iBoxHandle, iBoxHandle) ;
					if (cSetIsLastCore.IsOpen())
					{
						cSetIsLastCore.Close() ;
					}
					cSetIsLastCore.Open(g_PtrCon, strSql) ;
					if (!cSetIsLastCore.IsBOF())
					{
						cSetIsLastCore.MoveFirst() ;
					}
					while (!cSetIsLastCore.IsEOF())
					{
						cSetIsLastCore.Edit() ;
						cSetIsLastCore.SetFieldValue(_T("is_last"), 1) ;
						cSetIsLastCore.Update();
						cSetIsLastCore.MoveNext() ;
					}
					cSetIsLastCore.Close() ;
					//end 标出哪些不再延续的芯线，cable_core_draw:is_last，is_last的端子要绘制end端子

					//begin 计算盒子内芯线的编号 core_sn
					//////////////////////////////////////////////////////////////////////////
					//获取连接电缆的字符串，类似35-12(4)
					CString strCableStr = _T("") ;
					CADORecordset cSetGetCableStr ;
					strSql.Format(_T("select cablestring from cablenet_ent where handle_low=%d"), iBoxHandle) ;
					if (cSetGetCableStr.IsOpen())
					{
						cSetGetCableStr.Close() ;
					}
					cSetGetCableStr.Open(g_PtrCon, strSql) ;
					if (!cSetGetCableStr.IsBOF())
					{
						cSetGetCableStr.MoveFirst() ;
					}
					if (cSetGetCableStr.GetRecordCount()>0)
					{
						cSetGetCableStr.GetFieldValue(_T("cablestring"), strCableStr) ;
					}
					cSetGetCableStr.Close() ;
					CString strCabeStrTmp1 = strCableStr.Mid(strCableStr.Find('-')+1) ;
					CString strCabeStrTmp2 = strCabeStrTmp1.Left(strCabeStrTmp1.Find('(')) ;//电缆芯数
					//取得电缆定义的id
					int iCableTypeId = -1 ;
					CADORecordset cSetGetCableTypeId ;
					strSql.Format(_T("select id from define_cable_type where cable_name='%s'"), strCabeStrTmp2) ;
					if(cSetGetCableTypeId.IsOpen())
					{
						cSetGetCableTypeId.Close() ;
					}
					cSetGetCableTypeId.Open(g_PtrCon, strSql) ;
					if (!cSetGetCableTypeId.IsBOF())
					{
						cSetGetCableTypeId.MoveFirst() ;
					}
					if (cSetGetCableTypeId.GetRecordCount()>0)
					{
						cSetGetCableTypeId.GetFieldValue(_T("id"), iCableTypeId) ;
					}
					cSetGetCableTypeId.Close() ;

					//先将该型号电缆的所有芯的is_used重置0

					CADORecordset cSetUpdate ;
					strSql.Format(_T("select * from define_core_detail where cable_type_id=%d"), iCableTypeId) ;
					if(cSetUpdate.IsOpen())
					{
						cSetUpdate.Close() ;
					}
					cSetUpdate.Open(g_PtrCon, strSql) ;
					if (!cSetUpdate.IsBOF())
					{
						cSetUpdate.MoveFirst() ;
					}
					while (!cSetUpdate.IsEOF())
					{
						cSetUpdate.Edit() ;
						cSetUpdate.SetFieldValue(_T("is_used"), 0) ;
						cSetUpdate.Update() ;
						cSetUpdate.MoveNext() ;
					}
					cSetUpdate.Close() ;
					//ExecSqlString(strSql) ;
				
					//将内部芯已经占用的芯线标出来					
					CADORecordset cSetComputeCoreSn ;
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and is_inside=1"), iBoxHandle) ;
					if(cSetComputeCoreSn.IsOpen())
					{
						cSetComputeCoreSn.Close() ;
					}
					cSetComputeCoreSn.Open(g_PtrCon,strSql) ;
					if (!cSetComputeCoreSn.IsBOF())
					{
						cSetComputeCoreSn.MoveFirst() ;
					}					
					while (!cSetComputeCoreSn.IsEOF())
					{
						CString strCableCoreSn = _T("") ;

						cSetComputeCoreSn.GetFieldValue(_T("core_sn"), strCableCoreSn) ;
						//如果是内部芯线，则在define_core_detail里面找出这个芯线别标示为已用芯
						CADORecordset cSetUpdateCoreDeatail ;
						strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and core_sn='%s'"), iCableTypeId, strCableCoreSn) ;
						if (cSetUpdateCoreDeatail.IsOpen())
						{
							cSetUpdateCoreDeatail.Close();
						}
						cSetUpdateCoreDeatail.Open(g_PtrCon, strSql) ;
						if (!cSetUpdateCoreDeatail.IsBOF())
						{
							cSetUpdateCoreDeatail.MoveFirst() ;
						}
						if (!cSetUpdateCoreDeatail.IsEOF())
						{
							cSetUpdateCoreDeatail.Edit() ;
							cSetUpdateCoreDeatail.SetFieldValue(_T("is_used"), 1) ;
							cSetUpdateCoreDeatail.Update() ;
						}
						cSetUpdateCoreDeatail.Close() ;
						
						cSetComputeCoreSn.MoveNext() ;
					}
					cSetComputeCoreSn.Close() ;

					//给非内部芯分配芯线,包括半内部芯，即is_inside=2
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and is_inside<>1 order by pt_y desc"), iBoxHandle) ;
					if(cSetComputeCoreSn.IsOpen())
					{
						cSetComputeCoreSn.Close() ;
					}
					cSetComputeCoreSn.Open(g_PtrCon,strSql) ;
					if (!cSetComputeCoreSn.IsBOF())
					{
						cSetComputeCoreSn.MoveFirst() ;
					}					
					while (!cSetComputeCoreSn.IsEOF())
					{
						int iFundCoreSn = -1 ;
						CString strCableCoreSn = _T("") ;

						//得到指定型号的电缆的所有未使用的芯
						CADORecordset cSetCoreDetail ;
						strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0"), iCableTypeId) ;
						if (cSetCoreDetail.IsOpen())
						{
							cSetCoreDetail.Close() ;
						}
						cSetCoreDetail.Open(g_PtrCon, strSql) ;
						if (cSetCoreDetail.GetRecordCount()>0)
						{
							cSetCoreDetail.MoveFirst() ;
							cSetCoreDetail.GetFieldValue(_T("core_sn"), strCableCoreSn) ;

							cSetCoreDetail.Edit() ;
							cSetCoreDetail.SetFieldValue(_T("is_used"),1) ;
							cSetCoreDetail.Update() ;
							iFundCoreSn = 1 ;
						}
						else
						{
							iFundCoreSn = 0 ;
						}
						cSetCoreDetail.Close() ;
					
						
						if (iFundCoreSn==1) //找到芯线才更新数据，否则不更新
						{
							cSetComputeCoreSn.Edit() ;
							cSetComputeCoreSn.SetFieldValue(_T("core_sn"), strCableCoreSn) ;
							cSetComputeCoreSn.Update() ;
						}
						
						cSetComputeCoreSn.MoveNext() ;
					}
					cSetComputeCoreSn.Close() ;				
					
					//end 计算盒子内芯线的编号 core_sn

					
					//begin 计算备用,20100406注：假定备用是连续的
					CString strBackupStr = _T("") ;					
					CADORecordset cSetBackup ;
					//单芯
					strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='1'"), iCableTypeId) ;
					if (cSetBackup.IsOpen())
					{
						cSetBackup.Close() ;
					}
					cSetBackup.Open(g_PtrCon, strSql) ;
					if (cSetBackup.GetRecordCount()>0)
					{
						CString strBackupStrTmp = _T("") ;
						cSetBackup.MoveFirst() ;
						cSetBackup.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
						strBackupStr+=strBackupStrTmp ;
						cSetBackup.MoveNext() ;
						if (!cSetBackup.IsEOF())
						{
							strBackupStrTmp = _T("") ;
							cSetBackup.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
							strBackupStr+=_T("~") ;
							strBackupStr+=strBackupStrTmp ;
						}
					}
					cSetBackup.Close() ;
					
					//对绞
					strSql.Format(_T("select distinct(left(core_sn,2)) as name from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='2'"), iCableTypeId) ;
					if (cSetBackup.IsOpen())
					{
						cSetBackup.Close() ;
					}
					cSetBackup.Open(g_PtrCon, strSql) ;					
					if (!cSetBackup.IsBOF())
					{
						cSetBackup.MoveFirst() ;
					}
					while (!cSetBackup.IsEOF())
					{
						CString strName ;
						CString strBackupStrTmp = _T("") ;
						cSetBackup.GetFieldValue(_T("name"), strName) ;

						CADORecordset cSetBackupTmp ;
						strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='2' and left(core_sn,2)='%s'"), iCableTypeId, strName) ;
						if(cSetBackupTmp.IsOpen())
						{
							cSetBackupTmp.Close() ;
						}
						cSetBackupTmp.Open(g_PtrCon, strSql) ;
						if (cSetBackupTmp.GetRecordCount()>0)
						{
							cSetBackupTmp.MoveFirst() ;
							cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
							if (strBackupStr!=_T(""))
							{
								strBackupStr+=_T(",") ;
							}
							strBackupStr+=strBackupStrTmp ;
							cSetBackupTmp.MoveNext() ;
							if (!cSetBackupTmp.IsEOF())
							{
								strBackupStrTmp = _T("") ;
								cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
								strBackupStr+="~2" ;
							}
						}
						cSetBackupTmp.Close() ;

						cSetBackup.MoveNext() ;
					}
					cSetBackup.Close() ;

					//四芯组
					strSql.Format(_T("select distinct(left(core_sn,5)) as name from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='4'"), iCableTypeId) ;
					if (cSetBackup.IsOpen())
					{
						cSetBackup.Close() ;
					}
					cSetBackup.Open(g_PtrCon, strSql) ;					
					if (!cSetBackup.IsBOF())
					{
						cSetBackup.MoveFirst() ;
					}
					while (!cSetBackup.IsEOF())
					{
						CString strName ;
						CString strBackupStrTmp = _T("") ;
						cSetBackup.GetFieldValue(_T("name"), strName) ;

						CADORecordset cSetBackupTmp ;
						strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='4' and left(core_sn,5)='%s'"), iCableTypeId, strName) ;
						if(cSetBackupTmp.IsOpen())
						{
							cSetBackupTmp.Close() ;
						}
						cSetBackupTmp.Open(g_PtrCon, strSql) ;

						if (cSetBackupTmp.GetRecordCount()>0)
						{
							cSetBackupTmp.MoveFirst() ;
							cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
							if (strBackupStr!=_T(""))
							{
								strBackupStr+=_T(",") ;
							}							
							strBackupStr+=strBackupStrTmp ;
							cSetBackupTmp.MoveNext() ;
							if (!cSetBackupTmp.IsEOF())
							{
								strBackupStrTmp = _T("") ;
								cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
								strBackupStr+=_T("~4") ;
							}
						}
						cSetBackupTmp.Close() ;
						
						cSetBackup.MoveNext() ;
					}
					cSetBackup.Close() ;

					strSql.Format(_T("update cablenet_ent set backup_string='%s' where handle_low=%d"), strBackupStr, iBoxHandle) ;
					ExecSqlString(strSql) ;
					
// 					cSetAllBox.Edit() ;
// 					cSetAllBox.SetFieldValue("backup_string", strBackupStr) ;
// 					cSetAllBox.Update() ;
					
					//end 计算备用
					//20100405-1740 here	


					cSetAllBox.MoveNext() ;
				}
			}
			cSetAllBox.Close() ;
			//end 填充端子编号,芯线名称

			//begin 分线盘数据
			this->ComputeFxData(iXHL_handle_low) ;
			//end 分线盘数据
			

			//绘制编号为iIndex的配线图

			this->DrawPx(iIndex, iRsCount, pCurDb, iXHL_handle_low, dCurTotalYForOneDwg) ;
			dCurTotalYForOneDwg-=dCurTotalY ;
			dCurTotalYForOneDwg+= m_dCellHight*2 ;		

			iIndex++ ;

			cSet.MoveNext() ;
		}
		acedRestoreStatusBar() ;		
		ads_printf(_T("\n电缆配线结束：......\n")) ;

		cSet.Close() ;
		//end 一个信号楼ent产生一张图纸，循环产生所有图纸
		
		CString strSaveFile ;
		strSaveFile.Format(_T("%s\\站内电缆配线(单图模式).dwg"), this->m_strSavePath) ;		
		pCurDb->saveAs(strSaveFile) ;		
		delete pCurDb ;
		pCurDb = NULL ;
	

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeCoreAndDraw")) ;
	}  

}

//将信号机图块几点的小竖线长度变为0.01
BOOL CCableNetForJY::BatchChangeJYJ(AcDbDatabase *pCurDb)
{
		BOOL bRet = TRUE ;
// 	CUserSystem mysys ;
// 	mysys.m_pDb = pCurDb ;
	
	AcDbBlockTable * pBlkTbl = NULL ;
	pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{		
			//处理单线 AcDbLine ;
			//const char * pName = pEnt->isA()->name() ;
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine *pLine = AcDbLine::cast(pEnt) ;
				AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
				pLine->getStartPoint(pt_3d_start) ;
				pLine->getEndPoint(pt_3d_end) ;
				if (pt_3d_start.x==0.00&&pt_3d_end.x==0.00)
				{
					double d1 = fabs(pt_3d_start.y) ;
					double d2 = fabs(pt_3d_end.y) ;
					if (d1==0.01 && d2==0.01)
					{
						pt_3d_start.y = pt_3d_start.y/10 ;
						pt_3d_end.y = pt_3d_end.y/10 ;
						pLine->setStartPoint(pt_3d_start) ;
						pLine->setEndPoint(pt_3d_end) ;

					}
				}
				
			}
			pEnt->close() ;
		}
		
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return bRet ;

}

//取得该盒子前一个盒子,仅对ZD6适用，取得双动的第2动
//modify:20111026,对ZYJ6-7也适用
BOOL CCableNetForJY::GetPreviewEnt(long iCurEntHandle, long &iPreviewEntHandle)
{
	BOOL bRet = FALSE ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		
		//标示出起始(没有前一个链接的)的ent
		strSql.Format(_T("SELECT handle_low FROM cablenet_ent WHERE next_ent_handle_low=%d"), iCurEntHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		cSet.Open(g_PtrCon, strSql) ;
		if (cSet.GetRecordCount()>0)
		{
			cSet.MoveFirst() ;			
			cSet.GetFieldValue(_T("handle_low"), iPreviewEntHandle) ;
			bRet = TRUE ;			
		}
		
		cSet.Close() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetPreviewEnt")) ;
	} 

	return bRet ;
}


int CCableNetForJY::GetAllConVirtualPLine()
{
	return 0 ;

}

// 取得链路上的开始节点（计算加芯长度的起始节点，最后一个是锁闭器的不算）
void CCableNetForJY::GetFirstEnt(long iCurHandle, long & iStartHandle)
{
}

// 循环给每个box设置分支编号，信号楼为第一级，编号1，以信号楼为下一个的box按y从大到小编号1-1、1-2、......
void CCableNetForJY::SetBoxBranchIndex(long nCurBoxHandle, const CString & strCurIndex)
{
	try
	{
		CADORecordset cSet ;
		CString strSql ;		
		strSql.Format(_T("SELECT * FROM cablenet_ent WHERE next_ent_handle_low=%d order by pt_y desc"), nCurBoxHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;	
				int i = 1 ;
				while (!cSet.IsEOF())
				{
					long  nBoxHandleTmp = 0 ;
					cSet.GetFieldValue(_T("handle_low"), nBoxHandleTmp) ;
					CString strIndexTmp ;
					strIndexTmp.Format(_T("%s-%d"), strCurIndex, i) ;
					cSet.Edit() ;
					cSet.SetFieldValue(_T("branch_index"), strIndexTmp) ;
					cSet.Update() ;

					this->SetBoxBranchIndex(nBoxHandleTmp, strIndexTmp) ; 

					i++ ;
					cSet.MoveNext() ;
				}				
			}

			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetBoxBranchIndex")) ;
	} 
	
}
