// CableNetForBT.cpp: implementation of the CCableNetForBT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CableNetForBT.h"

#include "SBDestination.h"
#include "DBUtility.h"
//#include "../blkdll/BlkUtitily.h"


#ifdef VER_FOR_14
#include "dbhandle.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern BOOL bFirst ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCableNetForBT::CCableNetForBT()
: m_iHandleCurXhl(0)
, m_nForProj(0)
, m_iDownLineIsUp(0)
{
	m_pCurDb = NULL ;

	m_iDrawMode = 0 ;

	m_dCellHight = 5 ;
	m_dCellWidth = 40 ;
	m_iReadCablenetPos = 0 ;
	m_iReadCablenetCount = 100 ;

	CBlkUtility::GetBasePath(m_strSysBasePath) ;

	m_iCurIsLeftOrRight = 0;
	m_dStationLocX = 0.0;
}

CCableNetForBT::~CCableNetForBT()
{

}

BOOL CCableNetForBT::ReadEntToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//����ͼ�� AcDbBlockReference ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				//�õ�ʵ���handle����ÿ��dwg��Ψһ�ı�ʾ��
				AcDbHandle handleTmp;
				Adesk::UInt32 iHandle_Low = 0, iHandle_High = 0 ;
				pEnt->getAcDbHandle(handleTmp) ;

				iHandle_Low = handleTmp  ;							

				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;	

				//�õ�����
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
						//�ҳ����Ӷ�Ӧ���豸��Ŀǰֻ���źŻ���20100120��
						this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strDevType, strDevName, 0) ;						
					}
					else if (strTmp==_T("HF"))
					{
						CString strHFType = strArrayBlkRet.GetAt(2) ;
						//HFTYPE=0:ʹ��BOX_HF_0�飬HFTYPE=1��ʹ��BOX_HF_1��
						if (strHFType==_T("0"))
						{
							pt_3d_search_topleft.x = pt_3d_block.x-9 ;
							pt_3d_search_topleft.y = pt_3d_block.y+9 ;
							pt_3d_search_bottomright.x = pt_3d_block.x+9 ;
							pt_3d_search_bottomright.y = pt_3d_block.y-9 ;

							CString strDevTypeTmp ;
							this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strDevTypeTmp, strDevName, 1) ;						
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
						//�ҳ����Ӷ�Ӧ���豸
						this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strDevType, strDevName, 0) ;						
						strBoxType.Insert(0,_T("HZ")) ;
					}	

				}//end if box
				// 				else if (strArrayBlkRet.GetAt(0)==_T("XHJ"))
				// 				{
				// 					CString strXhjTypeTmp = strArrayBlkRet.GetAt(1) ; //jz-xb
				// 					CString strXhjTypeTmpPre = strXhjTypeTmp.Left(strXhjTypeTmp.Find(_T("-"))) ; //jz
				// 					CString strXhjTypeTmpBack = strXhjTypeTmp.Mid(strXhjTypeTmp.Find(_T("-"))+1) ; //xb
				// 					//�����ʽXHJ_5_JH
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
						int iLocFind = strTmp.Find(_T('-')) ;
						strDevType = strTmp ;
						if (iLocFind!=-1)
						{
							strDevType.Format(_T("%s"),strTmp.Mid(iLocFind+1)) ;
							strTmp.Delete(iLocFind) ;
						}
						strDevName = strTmp ;

						strTmp = strArrayBlkRet.GetAt(1) ;
						//strTmp.MakeUpper() ;				
						mysys.GetAttValue(pRef, strTmp, strBoxType) ;
						strBoxType.Insert(0, _T("HZ")) ;
					}
					else if (strArrayBlkRet.GetAt(2)==_T("JY"))
					{
						strDevFor = _T("") ;
						mysys.GetAttValue(pRef, _T("DC_NUM"), strDevName) ;
						int iLocFind = strDevName.Find(_T("-")) ;
						if (iLocFind!=-1)
						{
							strDevFor = strDevName.Mid(iLocFind+1) ; //ZD6/ZYJ6-7
							strDevName = strDevName.Left(iLocFind) ; //������ 
						}

						CString strActIndex = _T(""), strUseCoreNum = _T(""), strActNum = _T("") ;
						mysys.GetAttValue(pRef, _T("ACT_INDEX"), strActIndex) ;
						if (strActIndex==_T(""))
						{
							strActIndex = _T("1") ;
						}
						mysys.GetAttValue(pRef, _T("CORE_NUM"), strUseCoreNum) ;
						mysys.GetAttValue(pRef, _T("ACT_NUM"), strActNum) ;
						strDevType.Format(_T("ZZJ_%s_%s"), strActNum, strActIndex) ;
						mysys.GetAttValue(pRef, _T("HZ"), strBoxType) ;
						strBoxType.Insert(0, _T("HZ")) ;

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

				//�������
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
	// 		AfxMessageBox(_T("ע�⣺û�з����ź�¥�ڵ㣡")) ;
	// 	}

	return bRet ;

}


BOOL CCableNetForBT::FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strDevType, CString &strDevName, int iFindType)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pRef->position() ;
				//ֻ����涨����
				if((textp_3d.x>pt_3d_topleft.x && textp_3d.x<pt_3d_bottomright.x) && (textp_3d.y>pt_3d_bottomright.y && textp_3d.y<pt_3d_topleft.y))
				{
					//�õ�����

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
									//�����ʽJZB_5_0_0
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

BOOL CCableNetForBT::ReadConCableToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//����AcDbPolyline ;
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
				//ads_printf(_T("\n�߶���ʼ�� %.2f-%.2f %.2f-%.2f"), pt_3d_ptStartEnd[0].x,pt_3d_ptStartEnd[0].y,pt_3d_ptStartEnd[1].x,pt_3d_ptStartEnd[1].y ) ;
				//����ʼ�㰴x��С��������
				if (pt_3d_ptStartEnd[0].x>pt_3d_ptStartEnd[1].x)
				{
					AcGePoint3d pt_3d_tmp(0,0,0) ;
					pt_3d_tmp = pt_3d_ptStartEnd[0] ;
					pt_3d_ptStartEnd[0] = pt_3d_ptStartEnd[1] ;
					pt_3d_ptStartEnd[1] = pt_3d_tmp ;
				}

				//here, 20100120-2255
				//begin polyline��߶˵��ܱ�Ҫ��������
				//���ڿ������ź�¥�ڵ�ʵ�壬�Ƚ�С����������һ����С�ķ�Χ
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
					//�����һ����С��Χû���ҵ�����Ҫ�����ź�¥���ӣ���������Χ
					pt1_3d_topleft.x = pt_3d_ptStartEnd[0].x-6 ;
					pt1_3d_topleft.y = pt_3d_ptStartEnd[0].y+3.5 ;
					pt1_3d_bottomright.x = pt_3d_ptStartEnd[0].x+4 ;
					pt1_3d_bottomright.y = pt_3d_ptStartEnd[0].y-3.5 ;
					bFindLeft = this->FindBoxBetweenCable(pt1_3d_topleft,pt1_3d_bottomright,left_ent_Handle) ;
				}
				if (!bFindLeft)
				{
					strError.Format(_T("%s\nLeft near %f-%f no Box found"), strError, pt_3d_ptStartEnd[0].x, pt_3d_ptStartEnd[0].y );
					ads_printf(strError) ;
					pEnt->close() ;
					continue ;					
				}
				iLeftEntHandle_Low = left_ent_Handle ;				
				//end polyline��߶˵��ܱ�Ҫ��������

				//begin polyline�ұ߶˵��ܱ�Ҫ��������
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
					strError.Format(_T("%s\nRight near %f-%f no Box found"), strError, pt_3d_ptStartEnd[1].x, pt_3d_ptStartEnd[1].y );
					ads_printf(strError) ;
					pEnt->close() ;
					continue ;
				}
				iRightEntHandle_Low = right_ent_Handle ;					
				//end polyline�ұ߶˵��ܱ�Ҫ��������		


				//����������������ĵ����ַ�������100-12(6)				
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


BOOL CCableNetForBT::FindBoxBetweenCable(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright, AcDbHandle &entHandle)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
				//if (_tcscmp(pEnt->isA()->name(), _T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pRef->position() ;
				//ֻ����涨����
				if((textp_3d.x>pt_3d_topleft.x && textp_3d.x<pt_3d_bottomright.x) && (textp_3d.y>pt_3d_bottomright.y && textp_3d.y<pt_3d_topleft.y))
				{
					//�õ�����					
					CString strBlockName ;
					mysys.GetBlockName(pRef, strBlockName) ;					
					//strBlockName.MakeLower() ;

					strBlockName.TrimLeft();
					strBlockName.TrimRight() ;
					if (!strBlockName.IsEmpty())
					{
						CString strBlockType = strBlockName.Left(strBlockName.Find(_T("_"))) ;
						strBlockType.MakeUpper() ;
						//20110512:XHJ��BOX������ȥ���˴�strBlockType==_T("XHJ"),�����ź�¥����strBlockType==_T("DZ")
						//if (strBlockType==_T("BOX")||strBlockType==_T("XHJ")||strBlockType==_T("ZZJ")||strBlockType==_T("FJD")||strBlockType==_T("YDQ"))
						if (strBlockType==_T("BOX")||strBlockType==_T("ZZJ")||strBlockType==_T("FJD")||strBlockType==_T("YDQ")||strBlockType==_T("DZ"))
						{
							pEnt->getAcDbHandle(entHandle) ; 
							bRet = TRUE ;						
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

BOOL CCableNetForBT::FindCableStrInLine(const AcGeLineSeg3d lin, CString &strCableStr, AcGePoint3d &pt_3d_text)
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
		//��������ʵ�� ;
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

void CCableNetForBT::DoAll()
{

}

//�ݹ��һ����·
void CCableNetForBT::FindOneRoute(long iCurEntHandle, CLongArray &iArrayEntHandle)
{
	iArrayEntHandle.Add(iCurEntHandle) ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//��ʾ����ʼ(û��ǰһ�����ӵ�)��ent
		strSql.Format(_T("SELECT next_ent_handle_low FROM cablenet_ent WHERE handle_low=%d and next_ent_handle_low>0"), iCurEntHandle) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql) )
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				long iHandleTmp = 0 ;
				cSet.GetFieldValue(_T("next_ent_handle_low"), iHandleTmp) ;
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

//���������������
BOOL CCableNetForBT::FillDB()
{
	BOOL bRet = TRUE ;
	try	
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin ����cablenet_ent���left_ent_handle��right_ent_handle
		strSql.Format(_T("select * from cablenet_ent")) ;
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
				cSetTmp.Open(g_PtrCon, strSql) ;
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

			cSet.MoveNext() ;
		}
		cSet.Close() ;
		//end ����cablenet_ent���left_ent_handle��right_ent_handle

		//begin ����cablenet_ent���next_ent_handle_low, is_start, left_or_right
		CString strColName[] = {_T("left_ent_handle"), _T("right_ent_handle")} ;
		for (int i=0; i<2; i++)
		{
			strSql.Format(_T("select * from cablenet_ent where %s=0 and boxtype<>'XHL'"), strColName[i]) ;
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

		//here 20100325
		//end ����cablenet_ent���next_ent_handle_low

		acedSetStatusBarProgressMeterPos(40) ;

		//begin ����cablenet_ent���branch
		strSql.Format(_T("select * from cablenet_ent")) ;
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
		//end ����cablenet_ent���branch

		acedSetStatusBarProgressMeterPos(50) ;

		//begin ����cablenet_ent���belong_xhl_handle_low
		strSql.Format(_T("select * from cablenet_ent where is_start=1")) ;
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
		BOOL bSetProgressMod = iRsCount>50?TRUE:FALSE ;
		int iSumTmp = 0 ;
		//BOOL bHaveSetXHLRightOrLeft = FALSE ;
		while (!cSet.IsEOF()) 
		{
			long iHandleLow ;
			cSet.GetFieldValue(_T("handle_low"), iHandleLow) ;	

			CLongArray iArrayEntHandle ;
			//�ݹ������ihandleLow��ʵ���handle��Ϊ��ʼ�����ź�¥Ϊ����������·������ʵ��
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
			//�Դ�ʵ��Ϊ��ʼ����·�ϵ�ʵ�����
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
		//end ����cablenet_ent���belong_xhl_handle_low

		//begin ����ĳЩXB����豸cablenet_ent:devname�����JH�ߣ�û���豸����JZ JL�źŻ�ʹ�õ�,20110504����CZA/CZB��
		strSql.Format(_T("select * from cablenet_ent where left(devtype,3)='JZB' or left(devtype,3)='JLB' or left(devtype,3)='CZB' ")) ;
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
			long iNextEntHandle = 0 ;
			CString strDevName = _T("") ;
			CString strDevType = _T("") ;
			cSet.GetFieldValue(_T("next_ent_handle_low"), iNextEntHandle) ;
			cSet.GetFieldValue(_T("devname"), strDevName) ;
			cSet.GetFieldValue(_T("devtype"), strDevType) ;

			//begin ������һ�����ӵ��豸���ͺ�����
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
			//end ������һ�����ӵ��豸���ͺ�����

			cSet.Edit() ;
			strDevName+=_T("-B") ;
			cSet.SetFieldValue(_T("devname"), strDevName) ;
			cSet.Update() ;

			cSet.MoveNext() ;
		}
		cSet.Close() ;

		//end ����ĳЩXB����豸cablenet_ent:devname�����JH�ߣ�û���豸����JZ�źŻ�ʹ�õ�,20110504����CZA/CZB��


		//begin ��ZD6��ת�޻�ȷ����ʹ��о��
		//modify:20111026,����ZYJ6-7

		strSql.Format(_T("select * from cablenet_ent")) ;
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
			CString strDevFor ;			
			cSet.GetFieldValue(_T("dev_for"), strDevFor) ;
			if (strDevFor==_T("ZD6")||strDevFor==_T("ZYJ6")||strDevFor==_T("ZYJ7"))
			{
				long iCurHandle = 0, iStartHandle = 0;
				CString strDevType ;
				cSet.GetFieldValue(_T("handle_low"), iCurHandle) ;
				iStartHandle = iCurHandle ;
				cSet.GetFieldValue(_T("devtype"), strDevType) ;
				CStringArray strArrayDevType ;
				CBlkUtility::DivideString(strDevType, _T("_"), strArrayDevType) ;
				if (strArrayDevType.GetAt(1)==_T("2") && strArrayDevType.GetAt(2)==_T("1"))
				{
					this->GetPreviewEnt(iCurHandle, iStartHandle) ;
				}
				CLongArray iArrayRouteHandle ;
				this->FindOneRoute(iStartHandle, iArrayRouteHandle) ;

				CString strHandleStr = _T("") ;
				for (int iHandleIndex=0; iHandleIndex<iArrayRouteHandle.GetSize(); iHandleIndex++)
				{					
					CString strHandleTmp ;
					strHandleTmp.Format(_T("%d,"), iArrayRouteHandle.GetAt(iHandleIndex)) ;
					strHandleStr+=strHandleTmp ;					
				}
				strHandleStr.TrimRight(_T(",")) ;

				strSql.Format(_T("select * from cablenet_ent where handle_low in(%s)"), strHandleStr) ;
				CADORecordset cSetTmp ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				cSetTmp.Open(g_PtrCon, strSql) ;
				long iDistanceToXhl = 0 ;
				if (cSetTmp.GetRecordCount()>0)
				{
					cSetTmp.MoveFirst() ;

					while (!cSetTmp.IsEOF())
					{
						CString strCableStr ;
						cSetTmp.GetFieldValue(_T("cablestring"), strCableStr) ;
						long iLenTmp = 0 ;
						iLenTmp = _tstoi(strCableStr.Left(strCableStr.Find(_T("-")))) ;
						iDistanceToXhl+=iLenTmp ;

						cSetTmp.MoveNext() ;
					}

				}
				cSetTmp.Close() ;

				long iCoreNum = 0 ;
				strSql.Format(_T("SELECT top 1 * FROM define_zzj_plus where act_num=%s and act_index=%s and cable_len>=%d and dev_for='%s' "), strArrayDevType.GetAt(1), strArrayDevType.GetAt(2), iDistanceToXhl, strDevFor) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						cSetTmp.GetFieldValue(_T("core_num"), iCoreNum) ;
					}
					cSetTmp.Close() ;
				}

				CString strCoreNumTmp ;
				strCoreNumTmp.Format(_T("_%d"), iCoreNum) ;
				strDevType+=strCoreNumTmp ;

				cSet.Edit() ;
				cSet.SetFieldValue(_T("devtype"), strDevType) ;
				cSet.SetFieldValue(_T("distance_to_xhl"), iDistanceToXhl) ;
				cSet.Update() ;
			}
			else if (strDevFor==_T("SH6"))
			{
				CString strDevType ;			
				cSet.GetFieldValue(_T("devtype"), strDevType) ;
				strDevType+=_T("_1") ;
				cSet.Edit() ;
				cSet.SetFieldValue(_T("devtype"), strDevType) ;				
				cSet.Update() ;
			}


			cSet.MoveNext() ;
		}
		cSet.Close() ;

		//end ��ZD6��ת�޻�ȷ����ʹ��о��


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

//��������
BOOL CCableNetForBT::ComputeCoreAndDraw()
{
	BOOL bRet = TRUE ;
	//CUserSystem mysys ;
	//mysys.m_pDb = this->m_pCurDb ;
	try
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin һ���ź�¥ent����һ��ͼֽ��ѭ����������ͼֽ
		//////////////////////////////////////////////////////////////////////////

		//strSql.Format(_T("SELECT distinct belong_xhl_handle_low FROM cablenet_ent")) ;
		strSql.Format(_T("SELECT * FROM cablenet_ent where boxtype='XHL'")) ;
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
		strBarMsg.Format(_T("��������:%d��"), iRsCount) ;
		acedSetStatusBarProgressMeter(_T("��������"), 0, iRsCount-1) ;

		int iIndex = 0 ;
		double dCurTotalYForOneDwg = 0 ; //���Ƶ�һ��ͼֽʱ����¼ÿ�������ʱ��Y����	

		while (!cSet.IsEOF())
		{
			//����ձ�
			CDBUtility::ClearTable(_T("cable_core_draw")) ;

			//ads_printf(_T("\n%d"), iIndex) ;

			acedSetStatusBarProgressMeterPos(iIndex) ;			

			long iXHL_handle_low = 0 ;	
			CString strXhlName = _T("") ;
			cSet.GetFieldValue(_T("belong_xhl_handle_low"), iXHL_handle_low) ;	
			cSet.GetFieldValue(_T("devname"), strXhlName) ;	
			acutPrintf(_T("\n%d:Begin %s "), iIndex, strXhlName) ;


			//begin ������iXHL_handle_lowΪ�յ��������㣬Ȼ���Ը�������ź�¥������һ�飬����о������
			strSql.Format(_T("select * from cablenet_ent where is_start=1 and belong_xhl_handle_low=%d order by pt_y desc"), iXHL_handle_low) ;
			CADORecordset cSet_AllStart ;
			cSet_AllStart.Open(g_PtrCon, strSql) ;
			if (!cSet_AllStart.IsBOF())
			{
				cSet_AllStart.MoveFirst() ;
			}
			//ÿ������ͼ��Y�����0��ʼ
			double dCurTotalY = 0 ;
			int iAddYMode = 0 ; //�˴�Y������������о��/0���������ӵױ߿�/1
			BOOL bIsFirstRoute = TRUE ;

			//�������Ĵ���
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
				double dCurX = 0 ; //��ÿ����·������ʼ���ź�¥�����������е�x����
				double dPlusX = (iLeftORight==1?1:-1) ;//��ߴ��ź�¥����x�����Ǽ��٣��ұ߷�֮
				int iEntNum = iArrayHandle_OneRoute.GetSize() ;
				//ǰһ��devname
				CString strPreDevName = _T("") ;
				//begin for ����һ����·�е�ÿһ��ent��box��hz��������ʹ�õ���о��
				//
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

					if (strDevType.Left(7)==_T("ZZJ_2_1")) //modified 20110613:˫���еĵ�1������devnameҪ��2���������ϳ�һ������47/49����ʽ
					{
						strDevName.Insert(0, _T("/")) ;
						strDevName.Insert(0, strPreDevName) ;

						strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and left(core_name,2)='MH'"), iArrayHandle_OneRoute.GetAt(i-1)) ;
						CADORecordset cSetFoundExist ;
						if (cSetFoundExist.IsOpen())
						{
							cSetFoundExist.Close() ;
						}
						cSetFoundExist.Open(g_PtrCon, strSql) ;
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


					//begin if û�б�����������һ�α���Ҫ����title����Ϊ�˺ж�Ӧ�豸������о��,��������ʼ����Ҫ��������о��
					if (iScan_Time==0)
					{
						//title������
						double dTitleX = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
						double dTitleY = dCurTitleY ;

						cSetOneEntInRoute.Edit() ;
						cSetOneEntInRoute.SetFieldValue(_T("px_title_x"), dTitleX) ;
						cSetOneEntInRoute.SetFieldValue(_T("px_title_y"), dTitleY) ;
						cSetOneEntInRoute.Update() ;

						//�����ж�Ӧ�豸�����Ƿ���У���ض��������ĵ���
						if (strDevType!="")
						{
							//20110504:define_dev����dev_type������JZA��Ϊ����JZA_5_0_JH

							// 							int iFindMark = strDevType.Find(_T("_")) ;
							// 							if (iFindMark!=-1)
							// 							{
							// 								strDevType = strDevType.Left(iFindMark) ;
							// 							}
							CADORecordset cSetGetDev/*, cSetGetCoredetail*/ ;
							//����ȡ�������豸ʹ�õ�о��

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
							cSetGetDev.Open(g_PtrCon, strSql) ;
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
									cSetGetCoreDetail.Open(g_PtrCon, strSql) ;
									if (cSetGetCoreDetail.GetRecordCount()>0)
									{
										cSetGetCoreDetail.MoveFirst() ;
										while (!cSetGetCoreDetail.IsEOF())
										{
											CString strCoreName = _T("") ; //о�����ƣ�����DH DHH
											CString strCoreSn = _T("") ; //о�߱��,���磺D1-1 II2-3
											int iIsInside = 0 ;			//�ڲ�����Ҫ������о�ߣ�0/���ڲ���1/�ڲ�
											int iIsShare = 0 ;			//�ǲ��ǹ����ߣ�����DH DHH 0/�����ã�1/����
											CString strStartNum = _T("") ;			//о�����ʹ�õĺ��ӵĶ���
											CString strEndNum = _T("") ;			//о��ĩ��ʹ�õĺ��ӵĶ���
											int iIsVirtual = 0 ;	//0/�����⣬1/����о��ռλ��
											double dCurCoreX = dTitleX ;
											double dCurCoreY = dCurTotalY ;
											int iIsNew = 1 ;
											int iIsAdd = 0 ;

											cSetGetCoreDetail.GetFieldValue(_T("is_virtual"), iIsVirtual) ;
											cSetGetCoreDetail.GetFieldValue(_T("is_add"), iIsAdd) ;

											cSetGetCoreDetail.GetFieldValue(_T("is_share"), iIsShare) ;
											if (iIsShare>0 && i>0) //����ǹ����ߣ��Ҳ��ǵ�һ��ent����Ҫ����Ƿ�ǰ�����ӵĺ����������У������������Ӹ�о�ߣ�������һо��
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
											if (iIsInside==1) //����Ǵ��ڲ��ߣ��򽫲���Ĭ�ϵĶ˵�͵���о
											{
												cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
												cSetGetCoreDetail.GetFieldValue(_T("end_num_default"), strEndNum) ;
												cSetGetCoreDetail.GetFieldValue(_T("cable_core_sn_default"), strCoreSn) ;											
											}
											else if (iIsInside==2)//����ǰ봿�ڲ��ߣ��򽫲���Ĭ�ϵ����
											{
												cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
											}
											cSetGetCoreDetail.GetFieldValue(_T("core_name"), strCoreName) ;

											BOOL bFoundExist = FALSE ;
											if ((strDevType.Left(3)==_T("JZA")||strDevType.Left(3)==_T("JLA")||strDevType.Left(3)==_T("CZA"))&& i>0)//�����JZA���������ȷ���ǵ�2�����ӣ�����i>0���������
											{
												//�����Ѿ����ڵ�о�ߣ�����
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
											//modified by zhf:20110613
											else if (strDevType.Left(7)==_T("ZZJ_2_1") && strCoreName.Left(2)==_T("X4")) //����˫�������1��X4о��
											{
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
											strSql.Format(_T("insert into cable_core_draw(dev_name, core_name, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_virtual, is_add) \
															 values('%s','%s',%d, %d, %d, %d, %f,%f, '%s', '%s', '%s', %d, %d, %d)"), strDevNameTmp[iDevIndex], strCoreName , iHandleTmp, iIsNew, iIsInside, iIsShare, dCurCoreX, dCurCoreY, strCoreSn, strStartNum, strEndNum, iLeftORight, iIsVirtual, iIsAdd) ;
											ExecSqlString(strSql) ;

											cSetGetCoreDetail.MoveNext() ;
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
					//end if û�б�����������һ�α���Ҫ����title����Ϊ�˺ж�Ӧ�豸������о��,��������ʼ����Ҫ��������о��


					//����ʼbox�⣬������Ҫ����ǰһ�����ӵ�о��
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

							//�����Ѿ����ڵĹ���о�ߣ���������
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

							//�����Ѿ����ڵ�о�ߣ�����������������о�߲�һ��
							//����:����о����ֻҪ��DH,�Ͳ��������ˣ��˴��Ǽ�ʹ��X1�ˣ�������ͬһ���豸��X1����һ��Ҫ����
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
						cSetPreBoxCore.Close() ;

					}

					//ÿ����һ�ν�scan_time��1
					iScan_Time++ ;
					//strSql.Format(_T("update cablenet_ent set scan_time=%d"), iScan_Time) ;	
					//ExecSqlString(strSql) ;
					cSetOneEntInRoute.Edit() ;					
					bool b3 = cSetOneEntInRoute.SetFieldValue(_T("scan_time"), iScan_Time) ;
					bool b1 = cSetOneEntInRoute.Update() ;			


					//�����������ͷ�֧��branch����ͬ������Ҫ����bottom_title��λ�ã���Ԫ���£
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

					strPreDevName = strDevName ; //������devname�Ա���һ��devʹ��(��˫������ĵ�1��ʱʹ��)
				}
				//end for ����һ����·�е�ÿһ��ent��box��hz��������ʹ�õ���о��

				cSet_AllStart.MoveNext() ;
			}
			cSet_AllStart.Close() ;
			//end ������iXHL_handle_lowΪ�յ��������㣬Ȼ���Ը�������ź�¥������һ�飬����о������


			//begin �����ӱ��,о������
			//////////////////////////////////////////////////////////////////////////
			CADORecordset cSetAllBox ;
			CString strFileName ;
			strFileName.Format(_T("δ֪-%s"),strXhlName);

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

					//	int iBoxDzSum = 0 ; //�ú��Ӷ������� ע���ѯ���������xhl��û�н��
					CString strBoxType = _T("") ;
					//begin �õ��ú�������
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
						if (strBoxType==_T("XHL")) //�������ź�¥�������������´�ѭ��
						{
							cSetGetBoxType.GetFieldValue(_T("devname"), strFileName) ;

							cSetGetBoxType.Close() ;
							cSetAllBox.MoveNext() ;
							continue ;
						}
					}
					cSetGetBoxType.Close() ;
					//end �õ��ú�������

					//begin �õ����ӵ�box_type_id
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

					//end �õ����ӵ�box_type_id

					//�Ƚ����ͺź��ӵ����ж��ӵ�is_used����0					
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

					//begin �������й���о�ߵĶ���
					//////////////////////////////////////////////////////////////////////////

					//begin �Ƚ��ڲ�ռ�õĶ��ӱ��
					//��ǰ���������Ķ���
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
					//����������Ķ���
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
					//end �Ƚ��ڲ�ռ�õĶ��ӱ��

					//begin ���㹲��о�ߵĶ��Ӻ�
					//typedef CMap<CString, LPCSTR, CString ,LPCSTR> CMapStrToInt ;
					//CMapStrToInt mapCoreNameToDzNum ; //for examp:<"DHH", 42>
					CMapStringToString mapCoreNameToDzNum ;
					CADORecordset cSetShareType ; 
					//������о��ѡ������������is_share�Ĵ�С����������Ӻ�ʱ�����������������ӿ�ʼ��
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
					//end ���㹲��о�ߵĶ��Ӻ�

					//begin �ȸ���ǰһ����������Ĺ���о�ߵ�ĩ��ָ�����Ӻ�
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
					//begin �˶δ���Ϊ���1�������Ͻ�4�����ϵ�о��Ȩ��֮��
					int iPreShareCount = cSetPreShare.GetRecordCount() ;
					int iPreShareIndex = 0 ;
					int iPreSharePlus = mapCoreNameToDzNum.GetCount() ;
					//end �˶δ���Ϊ���1�������Ͻ�4�����ϵ�о��Ȩ��֮��
					while (!cSetPreShare.IsEOF())
					{
						CString strCoreName ;
						CString strDzNum = _T("") ;
						cSetPreShare.GetFieldValue(_T("core_name"), strCoreName) ;
						mapCoreNameToDzNum.Lookup(strCoreName, strDzNum) ;
						//begin �˶δ���Ϊ���1�������Ͻ�4�����ϵ�о��Ȩ��֮��
						if ((iPreShareCount/iPreSharePlus) >=5 ) //5��ǣ���ſ���
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
						//end �˶δ���Ϊ���1�������Ͻ�4�����ϵ�о��Ȩ��֮��
						cSetPreShare.Edit() ;
						cSetPreShare.SetFieldValue(_T("end_num"), strDzNum) ;
						cSetPreShare.Update() ;

						iPreShareIndex++ ;
						cSetPreShare.MoveNext() ;
					}
					cSetPreShare.Close() ;
					//end �ȸ���ǰһ����������Ĺ���о�ߵ�ĩ��ָ�����Ӻ�

					//begin �����ڱ����ӵĹ���о����ʼ����ָ�����Ӻ�
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
					//end �����ڱ����ӵĹ���о����ʼ����ָ�����Ӻ�

					//end �������й���о�ߵĶ���

					//begin ����ʣ��ķǹ���о�ߵĶ���:�ȴ���ú������õ�о��дstart�ˣ�+�ٴ����ǰ����������о��дend�ˣ�
					CADORecordset cSetOtherCore ;
					// 					strSql.Format(_T("select * from \
					// 						(select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 \
					// 						union select * from cable_core_draw where belong_box_handle_low \
					// 						in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside=0) order by pt_y desc"), iBoxHandle,iBoxHandle) ;
					//����������о
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 and is_add=0"), iBoxHandle) ;
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

					//��ǰ����������о
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside<>1 and is_add=0 order by pt_y desc"), iBoxHandle) ;
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


					//end ����ʣ��ķǹ���о�ߵĶ���,�ȴ���ú������õ�о��дstart�ˣ�+�ٴ����ǰ����������о��дend�ˣ�

					//begin �����Щ����������о�ߣ�cable_core_draw:is_last��is_last�Ķ���Ҫ����end����
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
					//end �����Щ����������о�ߣ�cable_core_draw:is_last��is_last�Ķ���Ҫ����end����

					//begin ���������о�ߵı�� core_sn
					//////////////////////////////////////////////////////////////////////////
					//��ȡ���ӵ��µ��ַ���������35-12(4)
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
					//����2����ڡ�403a-4(0)*1*1.0-185m�����ַ�������Ч�ģ������ڡ�T5-4a-4(0)*1.0-145m��������Ч���ʲ���������ʽ��ͨ��
					/*
					CString strCabeStrTmp1 = strCableStr.Mid(strCableStr.Find('-')+1) ;
					CString strCabeStrTmp2 = strCabeStrTmp1.Left(strCabeStrTmp1.Find('(')) ;//����о��
					*/
					CString strCabeStrTmp2 ;//����о��
					CBlkUtility::RegReplace(_T("^.+-(\\d+[AB]?)\\(\\d+\\)\\*.+-(\\d+|X+)m$"),_T("$1"),strCableStr, strCabeStrTmp2) ;

					//ȡ�õ��¶����id
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

					//�Ƚ����ͺŵ��µ�����о��is_used����0

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

					//���ڲ�о�Ѿ�ռ�õ�о�߱����					
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
						//������ڲ�о�ߣ�����define_core_detail�����ҳ����о�߱��ʾΪ����о
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

					//�����ڲ�о����о��,�������ڲ�о����is_inside=2
					
					//begin �绰Ҫ�öԽʻ���4о�飬�����ȸ��绰����о��
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and is_inside<>1 and left(core_name,3)='TEL' order by pt_y desc"), iBoxHandle) ;
					if(cSetComputeCoreSn.IsOpen())
					{
						cSetComputeCoreSn.Close() ;
					}
					cSetComputeCoreSn.Open(g_PtrCon,strSql) ;
					if (!cSetComputeCoreSn.IsBOF())
					{
						cSetComputeCoreSn.MoveFirst() ;
					}
					int iUseCoreId = 0 ; //�������绰оҪ�öԽʻ��ǽʣ����øñ����Ա��¼TEL�õ�define_core_detail��id���Ա�TEL_R��id+1��о��
					while (!cSetComputeCoreSn.IsEOF())
					{
						int iFundCoreSn = -1 ;
						CString strCableCoreSn = _T("") ;

						CString strCoreNameTmp = _T("") ;
						cSetComputeCoreSn.GetFieldValue(_T("core_name"), strCoreNameTmp) ;

						//�õ�ָ���ͺŵĵ��µ�����δʹ�õ�о
						CADORecordset cSetCoreDetail ;						
						//begin ���� �绰оҪ�öԽʻ��ǽ�
						if (strCoreNameTmp==_T("TEL"))
						{
							strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and (core_sn like 'D%%-1' or core_sn like '%%16%%-1' or core_sn like '%%16%%-3') order by id desc"), iCableTypeId) ;
						}
						else if (strCoreNameTmp==_T("TEL_R"))
						{
							strSql.Format(_T("select * from define_core_detail where id=%d"), iUseCoreId+1) ;
						}
						//end ���� �绰оҪ�öԽʻ��ǽ�
						if (cSetCoreDetail.IsOpen())
						{
							cSetCoreDetail.Close() ;
						}
						if(cSetCoreDetail.Open(g_PtrCon, strSql))
						{
							while(!cSetCoreDetail.IsEOF())
							{
								cSetCoreDetail.GetFieldValue(_T("core_sn"), strCableCoreSn) ;
								cSetCoreDetail.GetFieldValue(_T("id"), iUseCoreId) ;

								cSetCoreDetail.Edit() ;
								cSetCoreDetail.SetFieldValue(_T("is_used"),1) ;
								cSetCoreDetail.Update() ;
								iFundCoreSn = 1 ;
								break ;

							}
							cSetCoreDetail.Close() ;
						}

						if (iFundCoreSn==1) //�ҵ�о�߲Ÿ������ݣ����򲻸���
						{
							cSetComputeCoreSn.Edit() ;
							cSetComputeCoreSn.SetFieldValue(_T("core_sn"), strCableCoreSn) ;
							cSetComputeCoreSn.Update() ;
						}

						cSetComputeCoreSn.MoveNext() ;
					}
					cSetComputeCoreSn.Close() ;	
					//end �绰Ҫ�öԽʻ���4о�飬�����ȸ��绰����о��

					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and is_inside<>1 and left(core_name,3)<>'TEL' order by pt_y desc"), iBoxHandle) ;
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

						CString strCoreNameTmp = _T("") ;
						cSetComputeCoreSn.GetFieldValue(_T("core_name"), strCoreNameTmp) ;

						//�õ�ָ���ͺŵĵ��µ�����δʹ�õ�о
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
							cSetCoreDetail.GetFieldValue(_T("id"), iUseCoreId) ;

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


						if (iFundCoreSn==1) //�ҵ�о�߲Ÿ������ݣ����򲻸���
						{
							cSetComputeCoreSn.Edit() ;
							cSetComputeCoreSn.SetFieldValue(_T("core_sn"), strCableCoreSn) ;
							cSetComputeCoreSn.Update() ;
						}

						cSetComputeCoreSn.MoveNext() ;
					}
					cSetComputeCoreSn.Close() ;				

					//end ���������о�ߵı�� core_sn


					//begin ���㱸��,20100406ע���ٶ�������������
					CString strBackupStr = _T("") ;					
					CADORecordset cSetBackup ;
					//��о
					strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='1'"), iCableTypeId) ;
					if (cSetBackup.IsOpen())
					{
						cSetBackup.Close() ;
					}
					if (cSetBackup.Open(g_PtrCon, strSql) )					
					{
						int iDxRsCount = cSetBackup.GetRecordCount() ;
						if (iDxRsCount>0)
						{
							CString strBackupStrTmp = _T("") ;
							cSetBackup.MoveFirst() ;
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

					//�Խ�				
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
							cSetBackupTmp.MoveNext() ; //���ֻ��2����¼������movenext��Ϊeof����ʾD1-1~2�����Ϊeof��D1-2
							if (!cSetBackupTmp.IsEOF())
							{
								strBackupStrTmp = _T("") ;
								cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
								strBackupStr+=_T("~2") ;
							}
						}
						cSetBackupTmp.Close() ;

						cSetBackup.MoveNext() ;
					}
					cSetBackup.Close() ;


					//��о��
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
							if (!cSetBackupTmp.IsBOF())
							{
								cSetBackupTmp.MoveFirst() ;
							}
							int iPreFourIndex = 0 ;							
							while(!cSetBackupTmp.IsEOF())
							{
								cSetBackupTmp.GetFieldValue(_T("core_sn"), strBackupStrTmp) ;
								int iCurFourIndex = _tstoi(strBackupStrTmp.Right(1)) ;
								if (iPreFourIndex==0)
								{
									if (strBackupStr!=_T(""))
									{
										strBackupStr+=_T(",") ;
									}							
									strBackupStr+=strBackupStrTmp ;									
								}
								else 
								{
									if (iPreFourIndex==iCurFourIndex-1)//������ţ�����iPreFourIndex=2(%%161-2),iCurFourIndex=3(%%161-3)
									{
										CString strTailTmp ;
										int iLenTmp = strBackupStr.GetLength() ;										
										strTailTmp.Format(_T("%s~%d"), strBackupStr.Left(strBackupStr.Find(_T("~"), iLenTmp-2)==-1? iLenTmp:iLenTmp-2), iCurFourIndex) ;
										strBackupStr = strTailTmp ;										

									}
									else //��������ţ�����iPreFourIndex=2(%%161-2),iCurFourIndex=4(%%161-4)
									{
										if (strBackupStr!=_T(""))
										{
											strBackupStr+=_T(",") ;
										}							
										strBackupStr+=strBackupStrTmp ;										
									}
								}
								iPreFourIndex = iCurFourIndex ;

								cSetBackupTmp.MoveNext() ;
							}

						}
						cSetBackupTmp.Close() ;

						cSetBackup.MoveNext() ;
					}
					cSetBackup.Close() ;

					strSql.Format(_T("update cablenet_ent set backup_string='%s' where handle_low=%d"), strBackupStr, iBoxHandle) ;
					ExecSqlString(strSql) ;

					// 					cSetAllBox.Edit() ;
					// 					cSetAllBox.SetFieldValue(_T("backup_string"), strBackupStr) ;
					// 					cSetAllBox.Update() ;

					//end ���㱸��
					//20100405-1740 here	


					cSetAllBox.MoveNext() ;
				}
			}
			cSetAllBox.Close() ;
			//end �����ӱ��,о������

			//begin ����������
			this->ComputeFxData(iXHL_handle_low) ;
			//end ����������


			//���Ʊ��ΪiIndex������ͼ

			CString strSaveFile ;
			strSaveFile.Format(_T("%s\\%s.dwg"), this->m_strSavePath, strFileName) ;			
			this->DrawPx(strSaveFile, iXHL_handle_low) ;

			acutPrintf(_T("\n%d:End %s "), iIndex, strXhlName) ;

			iIndex++ ;

			cSet.MoveNext() ;
		}
		acedRestoreStatusBar() ;		
		//ads_printf(_T("\n�������߽�����......\n")) ;

		cSet.Close() ;

		//end һ���ź�¥ent����һ��ͼֽ��ѭ����������ͼֽ

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
//��ȡ��������ͼ����֤����
BOOL CCableNetForBT::ReadAndFillDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;
	acedSetStatusBarProgressMeter(_T("readcablenet..."), 1, 100) ;	

	acutPrintf(_T("\nBegin ReadEntToDB")) ;
	if (!this->ReadEntToDB(pConnection, pCurDb))
	{
		return FALSE ;
	}
	acutPrintf(_T("\nEnd ReadEntToDB")) ;

	acedSetStatusBarProgressMeterPos(10) ;

	acutPrintf(_T("\nBegin ReadConCableToDB")) ;
	if (!this->ReadConCableToDB(pConnection, pCurDb))
	{
		return FALSE ;
	}
	acutPrintf(_T("\nEnd ReadConCableToDB")) ;

	acedSetStatusBarProgressMeterPos(20) ;

	acutPrintf(_T("\nBegin FillDB")) ;
	if (!this->FillDB())
	{
		return FALSE ;
	}
	acutPrintf(_T("\nEnd FillDB")) ;

	//acedSetStatusBarProgressMeterPos(2) ;

	acedRestoreStatusBar() ;

	return bRet ;
}

//��������ͼ ��ͼģʽ
void CCableNetForBT::DrawPx(CString strDwgFilePath, long lXHLHandle)
{
	try
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;

		CString strBlockFile ;
		strBlockFile.Format(_T("%s\\support\\cablenet\\QT\\DZ.DWG"), m_strSysBasePath) ;

		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;
		AcDbObjectId textStyleId ;
		textStyleId = mysys.CreateTextStyleByFileName(_T("CHINESE"), _T("Times New Roman"), _T(""), 2, 0.8) ;

		CADORecordset cSet ;
		CString strSql ;

		//begin ����о��
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
			CString strDevType = _T("") ;//о�����ڵ��豸����
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
			cSet.GetFieldValue(_T("is_virtual"), iIsVirtual) ; //����о���û���,����Ҫ����ĩ�˱��
			cSet.GetFieldValue(_T("is_add"), iIsAdd) ; //��о��Ҫ���ɺ�ɫ

			//20111205:for bt�� convert %%161~170  to I~X
			if (strCoreSn.Left(2)==_T("%%"))
			{
				CString str1, str2 ;
				CString strReg = _T("^(%%\\d{3})(-\\d)$") ;
				CBlkUtility::RegReplace(strReg, _T("$1"), strCoreSn, str1) ;
				CBlkUtility::RegReplace(strReg, _T("$2"), strCoreSn, str2) ;				
				CString str3 ;
				CBlkUtility::ConvertRomanToChar(str1, str3) ;
				strCoreSn.Format(_T("%s%s"), str3, str2) ;
			}

			strDevNameTmp = strDevName ;
			//20111206:for bt
// 			strDevNameTmp.TrimRight(_T("-A")) ;
// 			strDevNameTmp.TrimRight(_T("-B")) ;

			//ȡ���豸����
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
				if (dPt_x!=0) //dPt_x��Ϊ0��ʾ�����ź�¥�ڵ�о��
				{
					//о��
					int iColorIndex = (iIsAdd==0?0:1) ;					
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end, iColorIndex) ;
				}
				//����
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
			//���Ӻ�
			if ((iIsNew==1 && iIsVirtual==0) || dPt_x==0)
			{
				base_pt_text[0] = pt_3d_start.x ;
				base_pt_text[1] = pt_3d_start.y+1 ;
				//strTmp.Format(_T("%s"), strStartNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ; //���
				//mysys.WriteText(base_pt_text, strStartNum, objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ; //���
				mysys.AddText(strStartNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;//���

			}

			if (dPt_x!=0)
			{
				base_pt_text[0] = pt_3d_end.x ;
				base_pt_text[1] = pt_3d_end.y+1 ;
				//strTmp.Format(_T("%d"), iEndNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ;//ĩ��
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
					AcDbBlockReference *pRef = NULL ;
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_end_for4,0, 1) )
					{
						pRef->close() ;
					}

					ads_point base_pt_text_for4 = {pt_3d_end_for4.x,pt_3d_end_for4.y+1,0} ;
					//mysys.WriteText(base_pt_text, strEndNum1, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//ĩ��
					mysys.AddText(strEndNum1, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��
					//mysys.WriteText(base_pt_text_for4, strEndNum2, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//ĩ��
					mysys.AddText(strEndNum2, base_pt_text_for4, base_pt_text_for4, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��


				}
				else
				{
					//mysys.WriteText(base_pt_text, strEndNum, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//ĩ��
					mysys.AddText(strEndNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��
				}

			}
			if (iIsVirtual==0)
			{
				base_pt_text[0] = pt_3d_start.x+(pt_3d_end.x-pt_3d_start.x)/2+(pt_3d_start.x>pt_3d_end.x?-2:2) ;			
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode) ; //о����
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode, AcDb::kTextBase, 0,textStyleId, 0) ;
				mysys.AddText(strCoreSn, base_pt_text, base_pt_text,iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;

				base_pt_text[0] = pt_3d_text.x ;
				base_pt_text[1] = pt_3d_text.y ; 
				//�ź�¥����ʱ����
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
					//����о����;
					strTmp.Format(_T("%s_%s"), strDevNameTmp, strCoreName) ;

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
					mysys.AddText(strTmp, base_pt_text, base_pt_text,iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;

				}

			}



			cSet.MoveNext() ;
		}

		cSet.Close() ;
		//end ����о��

		//begin ����title�ͱ߿�
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

			//20111205:for bt convert %%161~170  to I~X
			if (strBackupString.Find(_T("%%"))!=-1)
			{
				CStringArray strArrayBack ;
				CBlkUtility::DivideString(strBackupString, _T(","), strArrayBack) ;
				int iBLen = strArrayBack.GetCount() ;
				for (int iB=0; iB<iBLen; iB++)
				{
					CString strBack ;
					strBack = strArrayBack.GetAt(iB) ;
					if (strBack.Left(2)==_T("%%"))
					{
						CString str1, str2 ;
						CString strReg = _T("^(%%\\d{3})(\\S+)$") ;
						CBlkUtility::RegReplace(strReg, _T("$1"), strBack, str1) ;
						CBlkUtility::RegReplace(strReg, _T("$2"), strBack, str2) ;				
						CString str3 ;
						CBlkUtility::ConvertRomanToChar(str1, str3) ;
						strBack.Format(_T("%s%s"), str3, str2) ;
						strArrayBack.SetAt(iB, strBack) ;
					}
				}
				strBackupString = _T("") ;
				CBlkUtility::ConvertArrayToString(strArrayBack, _T(","), strBackupString) ;
			}


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
				//mysys.WriteText(pt_base_text, _T("Signalling Equipment Room(LPC)"), objID, 2.5, 0.75, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(_T("Signalling Equipment Room(LPC)"), pt_base_text, pt_base_text,iHorMode, iVerMode, 2.5, 0.75, 0, 0, textStyleId) ;
				pt_base_text[1] += m_dCellHight*2 ;
				strDevName+=_T("#") ;
				strDevName.TrimLeft(_T("L_")) ;
				strDevName.TrimLeft(_T("R_")) ;
				//mysys.WriteText(pt_base_text, strDevName, objID, 2.5, 0.9, iHorMode, iVerMode, 0,textStyleId, 0) ;
				mysys.AddText(strDevName, pt_base_text, pt_base_text,iHorMode, iVerMode, 2.5, 0.75, 0, 0, textStyleId) ;

				mysys.AddAcDbPolyLine(AcGePoint3d(0,0,0), AcGePoint3d(0,dBottom_y,0), 0.5) ; //�ź�¥�����

			}
			else
			{
// 				pt_base_text[0] = pt_base_text[0]+4 ;
// 				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8,iHorMode, iVerMode) ;
// 				if ((strDevType.Left(1)==_T("J") && strDevType.Left(2)!=_T("JZ")) || strDevType.Left(1)==_T("F"))
// 				{
// 					int iMarkLoc = strDevType.Find(_T("-")) ;
// 					if (iMarkLoc!=-1)
// 					{
// 						strDevName+=strDevType.Left(strDevType.Find(_T("_"))) ;
// 					}
// 					// 					else
// 					// 					{
// 					// 						strDevName+=strDevType ;
// 					// 					}
// 				}
// 				mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;
// 
// 				iHorMode = AcDb::kTextRight ;
// 				pt_base_text[0] = pt_base_text[0]-4+m_dCellWidth-4 ;
// 				//mysys.WriteText(pt_base_text, strBoxName, objID, 2, 0.8,iHorMode, iVerMode) ;
// 				mysys.WriteText(pt_base_text, strBoxName, objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;

				//20111205:change for bt
				CString strBoxText = strDevName ;
				pt_base_text[0] = pt_base_text[0]+m_dCellWidth/2 ;
				//mysys.WriteText(pt_base_text, strDevName, objID, 2, 0.8,iHorMode, iVerMode) ;
				if (strDevType==_T(""))//����
				{
					strBoxText.Format(_T("Junction Box_%s"),strDevName) ;
				}
				else if (strDevType.Left(9)==_T("bt_zzj_dd")||strDevType.Left(7)==_T("bt_ax_0")) //�綯ת�޻��ͼ��ᣨ������111A��AX(DMU��
				{
					strBoxText.Format(_T("Terminal Box_%s"),strDevName) ;
				}
				else if (strDevType==_T("bt_zzj_d"))
				{
					CString strTmp = strDevName ;
					strTmp.TrimRight(_T(" D")) ;
					strBoxText.Format(_T("Terminal Box_%s"),strTmp) ;
				}
				else if (strDevType==_T("bt_zzj_ind"))
				{
					CString strTmp = strDevName ;
					strTmp.TrimRight(_T(" IND")) ;
					strBoxText.Format(_T("Terminal Box_%s_IND"),strTmp) ;
				}
				else if (strDevType==_T("bt_dljl_dev_js"))
				{
					strBoxText.Format(_T("Terminal Box_%s_R"),strDevName) ;
				}
				else if (strDevType==_T("bt_dljl_dev_fs"))
				{
					strBoxText.Format(_T("Terminal Box_%s_F"),strDevName) ;
				}
				//mysys.WriteText(pt_base_text, strBoxText, objID, 2.5, 0.9, AcDb::kTextCenter, iVerMode, 0,textStyleId, 0) ;	
				mysys.AddText(strBoxText, pt_base_text, pt_base_text,AcDb::kTextCenter, iVerMode, 2.5, 0.9, 0, 0, textStyleId) ;
				
			}

			AcGePoint3d pt_3d_start(dTitle_x,dTitle_y,0) ;
			AcGePoint3d pt_3d_end(dTitle_x,dTitle_y,0) ;
			AcGePoint3d pt_3d_tmp(0,0,0) ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.5) ;
			//mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
			pt_3d_tmp.x = pt_3d_end.x ;
			pt_3d_tmp.y = dBottom_y ;
			//mysys.AddAcDbLine(pt_3d_end, pt_3d_tmp) ;
			mysys.AddAcDbPolyLine(pt_3d_end, pt_3d_tmp, 0.5) ;
			pt_3d_start.y = dBottom_y ;
			mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_tmp, 0.5) ;
			pt_3d_start.x = dTitle_x ;
			pt_3d_start.y = dTitle_y+m_dCellHight*2 ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			pt_3d_end.y = pt_3d_start.y ;
			mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.5) ;
			pt_3d_start.x = pt_3d_end.x ;
			pt_3d_start.y = dTitle_y ;
			mysys.AddAcDbPolyLine(pt_3d_end, pt_3d_start, 0.5) ;


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
				CString strSpareText = _T("N/S") ;
				if(!strBackupString.IsEmpty())
				{
					//mysys.WriteText(pt_base_text, strBackupString, objID, 2, 0.8, iHorMode, AcDb::kTextTop, 0,textStyleId, 0) ;
					mysys.AddText(strBackupString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
					strSpareText = _T("SPARE") ;
				}
				pt_base_text[0] = dBottom_x - iPlusX*5 ;
				//mysys.WriteText(pt_base_text, strSpareText, objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextTop, 0, textStyleId, 0) ;
				mysys.AddText(strSpareText, pt_base_text, pt_base_text, AcDb::kTextCenter, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
			}
			//here

			cSet.MoveNext() ;
		}
		cSet.Close() ;
		//end ����title�ͱ߿�
		strSql.Format(_T("select * from")) ;

		//begin ����ͼ��

		//end ����ͼ��

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

//����һ���豸�������з�֧����is_start=1Ϊ������һ����·��
void CCableNetForBT::FindBranch(long iDesHandle, int &iBranchNum)
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
		if(cSet.Open(g_PtrCon, strSql) )
		{
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

void CCableNetForBT::FindNextEnt(long iHandle, int iForwardToLeftOrRight)
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

		//begin ����cablenet_ent���left_ent_handle��right_ent_handle
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iHandle) ;
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
			long iNextHandle = 0 ;
			CString strCableStr ;

			cSet.GetFieldValue(_T("next_ent_handle_low"), iNextHandle) ;
			if (iNextHandle>0)//˵����ʵ���Ѿ���������������ʵ��϶����������ˣ��˳��ݹ�
			{
				cSet.Close() ;
				return ;
			}
			cSet.GetFieldValue(strNextHandeName[iForwardToLeftOrRight], iNextHandle) ;
			if (iNextHandle<=0)//˵����ʵ���ұ�û��ʵ�壬���������Ӧ�����ź�¥���˳��ݹ�
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
			cSetTmp.Open(g_PtrCon, strSql) ;
			if (cSetTmp.GetRecordCount()>0)
			{
				cSetTmp.MoveFirst() ;							
				cSetTmp.GetFieldValue(_T("cablestring"), strCableStr) ;

			}
			cSetTmp.Close() ;


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

int CCableNetForBT::ReadFxpDataToDb(CString strXlsFileName)
{
	int iReturn = 0 ;
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	CString strExec, strMsg ;
	//�������������
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
			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		//�ж��ļ��ĺϷ��ԣ�
		BOOL bCheckFile = TRUE ;
		CString  strCellName[3] = { _T("B1"), _T("C1"),_T("D1") } ;
		CString strCellContent[3] = {_T("�豸����"), _T("о������"), _T("���߶���")} ;
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
				strMsg.Format(_T("��ʽ�����ϣ�%s����%s"),strCellName[i], strCellContent[i]) ;
				AfxMessageBox(strMsg) ;
				break ;
			}
		}

		if (bCheckFile)
		{
			int iCur_Row = 2 ; //��ʼ��
			CString strCellName ;
			CString strXhl = _T(""); //�ź�¥��
			CString strDev = _T(""); //�豸����
			CString strCore = _T(""); //о������
			CString strFx = _T(""); // ���߶���

			CString strTmp ;
			BOOL bNext = TRUE ; 

			while (bNext) 
			{			
				//�ź�¥��
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
				// 				//�豸����
				// 				strCellName.Format(_T("B%d"), iCur_Row) ;
				// 				pRange = NULL ;
				// 				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				// 				range_var = pRange->GetText() ;
				// 				strDev = range_var.bstrVal ;
				// 				strDev.TrimLeft() ;
				// 				strDev.TrimRight() ;

				//о������
				strCellName.Format(_T("C%d"), iCur_Row) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				range_var = pRange->GetText() ;
				strCore = range_var.bstrVal ;
				strCore.TrimLeft() ;
				strCore.TrimRight() ;

				//���߶���
				strCellName.Format(_T("D%d"), iCur_Row) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				range_var = pRange->GetText() ;
				strFx = range_var.bstrVal ;
				strFx.TrimLeft() ;
				strFx.TrimRight() ;

				strExec.Format(_T("insert into fx_data(xhl_node, dev_name, core_name, fx_name) values('%s','%s','%s','%s')"), strXhl, strDev, strCore, strFx) ;
				//ptrCon->Execute((_bstr_t) strExec, NULL, NULL) ;
				ExecSqlString(strExec) ;

				iCur_Row++ ;
				iReturn = iCur_Row-2 ;
			}
		}
		else
		{
			AfxMessageBox( _T("ѡ����ļ��������ļ���ʽ") ) ;			
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
		AfxMessageBox(_T("δ֪�Ĵ���!"));

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}

	return iReturn ;

}

//�������������
void CCableNetForBT::ComputeFxData(long iXhlHandle)
{
	try	
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin ȡ���ź�¥�˵����ƣ�����DC-130
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
		//end ȡ���ź�¥�˵����ƣ�����DC-130

		//begin ȡ���������ڸ��ź�¥��о��
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
			CString strFxName = _T("0") ; //�����������߶���

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


			//ֱ�Ӳ���
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

			//û��ֱ���ҵ�������ʼ���ӣ��ټ�����߶���(����ǹ���������ֱ��ָ��fx)
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

					//20110504:define_dev����dev_type���JZA_5_0_JH��ʽ

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

				//�ҵ��������豸����ȫ��о��
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
						CString strFirstFxName = _T("") ; //��ʼ���߶�������
						int iFxPlus = 0 ;

						CString strFirstCoreName = _T("") ; //��ʼо������
						cSetFound.GetFieldValue(_T("core_name"), strFirstCoreName) ;
						CString strFind = _T("") ;
						strFind.Format(_T("core_name='%s'"), strCoreName) ;
						if(cSetFound.Find(strFind))
						{
							cSetFound.GetFieldValue(_T("fx_plus"), iFxPlus) ;
						}

						//���Ҹ��豸�ķ�������ʼ���Ӻ�
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

			//�ۺ϶��� FS FSH JS JSH
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
		//end ȡ���������ڸ��ź�¥��о��


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

//��ͼģʽ
void CCableNetForBT::DrawPx(int iIndex, int iDwgSum, AcDbDatabase *pCurDb, long iXhlHandle, double dCurTotalYForOneDwg)
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

		//begin ����о��
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
			CString strDevType = _T("") ;//о�����ڵ��豸����
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
			cSet.GetFieldValue(_T("is_virtual"), iIsVirtual) ; //����о���û���,����Ҫ����ĩ�˱��	

			strDevNameTmp = strDevName ;
			strDevNameTmp.TrimRight(_T("-A")) ;
			strDevNameTmp.TrimRight(_T("-B")) ;

			//ȡ���豸����
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
				if (dPt_x!=0) //dPt_x��Ϊ0��ʾ�����ź�¥�ڵ�о��
				{
					//о��
					mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
				}
				//����
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
			//���Ӻ�
			if ((iIsNew==1 && iIsVirtual==0) || dPt_x==0)
			{
				base_pt_text[0] = pt_3d_start.x ;
				base_pt_text[1] = pt_3d_start.y+1 ;
				//strTmp.Format(_T("%s"), strStartNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ; //���
				//mysys.WriteText(base_pt_text, strStartNum, objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ; //���
				mysys.AddText(strStartNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.8, 0, 0) ; //���
			}

			if (dPt_x!=0)
			{
				base_pt_text[0] = pt_3d_end.x ;
				base_pt_text[1] = pt_3d_end.y+1 ;
				//strTmp.Format(_T("%d"), iEndNum) ;
				//mysys.WriteText(base_pt_text, strTmp, objID, 2, 0.7, AcDb::kTextCenter) ;//ĩ��
				//mysys.WriteText(base_pt_text, strEndNum, objID, 2, 0.7, AcDb::kTextCenter, AcDb::kTextBase, 0,textStyleId, 0) ;//ĩ��
				mysys.AddText(strEndNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0,0,textStyleId) ;//ĩ��
			}
			if (iIsVirtual==0)
			{
				base_pt_text[0] = pt_3d_start.x+(pt_3d_end.x-pt_3d_start.x)/2+(pt_3d_start.x>pt_3d_end.x?-2:2) ;			
				//mysys.WriteText(base_pt_text, strCoreSn, objID, 2, 0.7, iHorMode) ; //о����
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
		//end ����о��

		//begin ����title�ͱ߿�
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
					//mysys.WriteText(pt_base_text, _T("�ź�¥"), objID, 2, 0.8, iHorMode, iVerMode, 0,textStyleId, 0) ;	
					mysys.AddText(_T("�ź�¥"), pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
				}
				if (iIndex==iDwgSum-1)
				{
					mysys.AddAcDbPolyLine(AcGePoint3d(0,0,0), AcGePoint3d(0,dBottom_y,0), 0.5) ; //�ź�¥�����
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
				mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.5);//���������
			}

			pt_3d_tmp.x = pt_3d_end.x ;
			pt_3d_tmp.y = dBottom_y ;			
			mysys.AddAcDbPolyLine(pt_3d_end, pt_3d_tmp, 0.5) ;//��߿�

			pt_3d_start.y = dBottom_y ;
			if (!(iIndex!=iDwgSum-1 && strBoxName==_T("XHL")))
			{
				mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_tmp, 0.5) ;//�±߿�
			}			

			pt_3d_start.x = dTitle_x ;
			pt_3d_start.y = dTitle_y+m_dCellHight*2 ;
			pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
			pt_3d_end.y = pt_3d_start.y ;
			if (strBoxName!=_T("XHL") ||(strBoxName==_T("XHL") && iIndex==0) )
			{
				mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.5) ;//���������
			}			

			pt_3d_start.x = pt_3d_end.x ;
			pt_3d_start.y = dTitle_y ;
			mysys.AddAcDbPolyLine(pt_3d_end, pt_3d_start, 0.5) ; //���������

			if (strBoxName==_T("XHL") && iIndex!=0)
			{
				pt_3d_end.y+=m_dCellHight*2 ;
				mysys.AddAcDbPolyLine(pt_3d_end, pt_3d_start, 0.5) ; 
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
				//mysys.WriteText(pt_base_text, _T("����"), objID, 2, 0.8, AcDb::kTextCenter, AcDb::kTextTop, 0, textStyleId, 0) ;
				mysys.AddText(_T("����"), pt_base_text, pt_base_text, AcDb::kTextCenter, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;

			}
			//here

			cSet.MoveNext() ;
		}
		cSet.Close() ;
		//end ����title�ͱ߿�

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

//��Զ����¥���ӻ���һ��ͼ�ϵ����
void CCableNetForBT::ComputeCoreAndDraw2()
{
	CUserSystem mysys ;

	try
	{				
		CADORecordset cSet ;
		CString strSql ;
		AcDbDatabase *pCurDb = new AcDbDatabase ;

		//begin һ���ź�¥ent����һ��ͼֽ��ѭ����������ͼֽ
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
		strBarMsg.Format(_T("��������:%d��"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;

		int iIndex = 0 ;
		double dCurTotalYForOneDwg = 0 ; //���Ƶ�һ��ͼֽʱ����¼ÿ�������ʱ��Y����

// 		int iDwgSum = cSet.GetRecordCount() ; //���ٸ��ź�¥����

		while (!cSet.IsEOF())
		{
			//����ձ�
			CDBUtility::ClearTable(_T("cable_core_draw")) ;

			acedSetStatusBarProgressMeterPos(iIndex) ;			

			long iXHL_handle_low = 0 ;			
			cSet.GetFieldValue(_T("belong_xhl_handle_low"), iXHL_handle_low) ;			

			//begin ������iXHL_handle_lowΪ�յ��������㣬Ȼ���Ը�������ź�¥������һ�飬����о������
			strSql.Format(_T("select * from cablenet_ent where is_start=1 and belong_xhl_handle_low=%d order by pt_y desc"), iXHL_handle_low) ;
			CADORecordset cSet_AllStart ;
			cSet_AllStart.Open(g_PtrCon, strSql) ;
			if (!cSet_AllStart.IsBOF())
			{
				cSet_AllStart.MoveFirst() ;
			}
			//ÿ������ͼ��Y�����0��ʼ
			double dCurTotalY = 0 ;
			int iAddYMode = 0 ; //�˴�Y������������о��/0���������ӵױ߿�/1
			BOOL bIsFirstRoute = TRUE ;

			//�������Ĵ���
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
				double dCurX = 0 ; //��ÿ����·������ʼ���ź�¥�����������е�x����
				double dPlusX = (iLeftORight==1?1:-1) ;//��ߴ��ź�¥����x�����Ǽ��٣��ұ߷�֮
				int iEntNum = iArrayHandle_OneRoute.GetSize() ;
				//begin for ����һ����·�е�ÿһ��ent��box��hz��������ʹ�õ���о��
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

					//begin if û�б�����������һ�α���Ҫ����title����Ϊ�˺ж�Ӧ�豸������о��,��������ʼ����Ҫ��������о��
					if (iScan_Time==0)
					{
						//title������
						double dTitleX = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
						double dTitleY = dCurTitleY ;

						cSetOneEntInRoute.Edit() ;
						cSetOneEntInRoute.SetFieldValue(_T("px_title_x"), dTitleX) ;
						cSetOneEntInRoute.SetFieldValue(_T("px_title_y"), dTitleY) ;
						cSetOneEntInRoute.Update() ;

						//�����ж�Ӧ�豸�����Ƿ���У���ض��������ĵ���
						if (strDevType!="")
						{
							//20110504:define_dev����dev_type������JZA��Ϊ����JZA_5_0_JH
							// 							int iFindMark = strDevType.Find(_T("_")) ;
							// 							if (iFindMark!=-1)
							// 							{
							// 								strDevType = strDevType.Left(iFindMark) ;
							// 							}
							CADORecordset cSetGetDev/*, cSetGetCoredetail*/ ;
							//����ȡ�������豸ʹ�õ�о��
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
										CString strCoreName = _T("") ; //о�����ƣ�����DH DHH
										CString strCoreSn = _T("") ; //о�߱��,���磺D1-1 II2-3
										int iIsInside = 0 ;			//�ڲ�����Ҫ������о�ߣ�0/���ڲ���1/�ڲ�
										int iIsShare = 0 ;			//�ǲ��ǹ����ߣ�����DH DHH 0/�����ã�1/����
										CString strStartNum = _T("") ;			//о�����ʹ�õĺ��ӵĶ���
										CString strEndNum = _T("") ;			//о��ĩ��ʹ�õĺ��ӵĶ���
										int iIsVirtual = 0 ;	//0/�����⣬1/����о��ռλ��
										double dCurCoreX = dTitleX ;
										double dCurCoreY = dCurTotalY ;
										int iIsNew = 1 ;

										cSetGetCoreDetail.GetFieldValue(_T("is_virtual"), iIsVirtual) ;

										cSetGetCoreDetail.GetFieldValue(_T("is_share"), iIsShare) ;
										if (iIsShare>0 && i>0) //����ǹ����ߣ��Ҳ��ǵ�һ��ent����Ҫ����Ƿ�ǰ�����ӵĺ����������У������������Ӹ�о�ߣ�������һо��
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
										if (iIsInside==1) //����Ǵ��ڲ��ߣ��򽫲���Ĭ�ϵĶ˵�͵���о
										{
											cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
											cSetGetCoreDetail.GetFieldValue(_T("end_num_default"), strEndNum) ;
											cSetGetCoreDetail.GetFieldValue(_T("cable_core_sn_default"), strCoreSn) ;											
										}
										else if (iIsInside==2)//����ǰ봿�ڲ��ߣ��򽫲���Ĭ�ϵ����
										{
											cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNum) ;
										}
										cSetGetCoreDetail.GetFieldValue(_T("core_name"), strCoreName) ;

										BOOL bFoundExist = FALSE ;
										if ((strDevType.Left(3)==_T("JZA")||strDevType.Left(3)==_T("JLA")||strDevType.Left(3)==_T("CZA"))&& i>0)//�����JZA���������ȷ���ǵ�2�����ӣ�����i>0���������
										{
											//�����Ѿ����ڵ�о�ߣ�����
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
					//end if û�б�����������һ�α���Ҫ����title����Ϊ�˺ж�Ӧ�豸������о��,��������ʼ����Ҫ��������о��


					//����ʼbox�⣬������Ҫ����ǰһ�����ӵ�о��
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

							//�����Ѿ����ڵĹ���о�ߣ���������
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

							//�����Ѿ����ڵ�о�ߣ�����������������о�߲�һ��
							//����:����о����ֻҪ��DH,�Ͳ��������ˣ��˴��Ǽ�ʹ��X1�ˣ�������ͬһ���豸��X1����һ��Ҫ����
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

					//ÿ����һ�ν�scan_time��1
					iScan_Time++ ;
					//strSql.Format(_T("update cablenet_ent set scan_time=%d"), iScan_Time) ;	
					//ExecSqlString(strSql) ;
					cSetOneEntInRoute.Edit() ;					
					bool b3 = cSetOneEntInRoute.SetFieldValue(_T("scan_time"), iScan_Time) ;
					bool b1 = cSetOneEntInRoute.Update() ;			


					//�����������ͷ�֧��branch����ͬ������Ҫ����bottom_title��λ�ã���Ԫ���£
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
				//end for ����һ����·�е�ÿһ��ent��box��hz��������ʹ�õ���о��

				cSet_AllStart.MoveNext() ;
			}
			cSet_AllStart.Close() ;
			//end ������iXHL_handle_lowΪ�յ��������㣬Ȼ���Ը�������ź�¥������һ�飬����о������


			//begin �����ӱ��,о������
			//////////////////////////////////////////////////////////////////////////
			CADORecordset cSetAllBox ;
			CString strFileName = _T("δ֪");

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

					//	int iBoxDzSum = 0 ; //�ú��Ӷ������� ע���ѯ���������xhl��û�н��
					CString strBoxType = _T("") ;
					//begin �õ��ú�������
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
						if (strBoxType==_T("XHL")) //�������ź�¥�������������´�ѭ��
						{
							cSetGetBoxType.GetFieldValue(_T("devname"), strFileName) ;

							cSetGetBoxType.Close() ;
							cSetAllBox.MoveNext() ;
							continue ;
						}
					}
					cSetGetBoxType.Close() ;
					//end �õ��ú�������

					//begin �õ����ӵ�box_type_id
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

					//end �õ����ӵ�box_type_id

					//�Ƚ����ͺź��ӵ����ж��ӵ�is_used����0					
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

					//begin �������й���о�ߵĶ���
					//////////////////////////////////////////////////////////////////////////

					//begin �Ƚ��ڲ�ռ�õĶ��ӱ��
					//��ǰ���������Ķ���
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
					//����������Ķ���
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
					//end �Ƚ��ڲ�ռ�õĶ��ӱ��

					//begin ���㹲��о�ߵĶ��Ӻ�
					//typedef CMap<CString, LPCSTR, CString ,LPCSTR> CMapStrToInt ;
					//CMapStrToInt mapCoreNameToDzNum ; //for examp:<"DHH", 42>
					CMapStringToString mapCoreNameToDzNum ;
					CADORecordset cSetShareType ; 
					//������о��ѡ������������is_share�Ĵ�С����������Ӻ�ʱ�����������������ӿ�ʼ��
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
					//end ���㹲��о�ߵĶ��Ӻ�

					//begin �ȸ���ǰһ����������Ĺ���о�ߵ�ĩ��ָ�����Ӻ�
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
					//end �ȸ���ǰһ����������Ĺ���о�ߵ�ĩ��ָ�����Ӻ�

					//begin �����ڱ����ӵĹ���о����ʼ����ָ�����Ӻ�
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
					//end �����ڱ����ӵĹ���о����ʼ����ָ�����Ӻ�

					//end �������й���о�ߵĶ���

					//begin ����ʣ��ķǹ���о�ߵĶ���:�ȴ���ú������õ�о��дstart�ˣ�+�ٴ����ǰ����������о��дend�ˣ�
					CADORecordset cSetOtherCore ;
					// 					strSql.Format(_T("select * from \
					// 						(select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 \
					// 						union select * from cable_core_draw where belong_box_handle_low \
					// 						in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside=0) order by pt_y desc"), iBoxHandle,iBoxHandle) ;
					//����������о
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

					//��ǰ����������о
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


					//end ����ʣ��ķǹ���о�ߵĶ���,�ȴ���ú������õ�о��дstart�ˣ�+�ٴ����ǰ����������о��дend�ˣ�

					//begin �����Щ����������о�ߣ�cable_core_draw:is_last��is_last�Ķ���Ҫ����end����
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
					//end �����Щ����������о�ߣ�cable_core_draw:is_last��is_last�Ķ���Ҫ����end����

					//begin ���������о�ߵı�� core_sn
					//////////////////////////////////////////////////////////////////////////
					//��ȡ���ӵ��µ��ַ���������35-12(4)
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
					//����2����ڡ�403a-4(0)*1*1.0-185m�����ַ�������Ч�ģ������ڡ�T5-4a-4(0)*1.0-145m�����ߡ�T5-4a-4(0)*1.0-XXXm��������Ч���ʲ���������ʽ��ͨ��
					/*
					CString strCabeStrTmp1 = strCableStr.Mid(strCableStr.Find('-')+1) ;
					CString strCabeStrTmp2 = strCabeStrTmp1.Left(strCabeStrTmp1.Find('(')) ;//����о��
					*/
					CString strCabeStrTmp2 ;//����о��
					CBlkUtility::RegReplace(_T("^.+-(\\d+[AB]?)\\(\\d+\\)\\*.+-(\\d+|X+)m$"),_T("$1"),strCableStr, strCabeStrTmp2) ;

					//ȡ�õ��¶����id
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

					//�Ƚ����ͺŵ��µ�����о��is_used����0

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

					//���ڲ�о�Ѿ�ռ�õ�о�߱����					
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
						//������ڲ�о�ߣ�����define_core_detail�����ҳ����о�߱��ʾΪ����о
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

					//�����ڲ�о����о��,�������ڲ�о����is_inside=2
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

						//�õ�ָ���ͺŵĵ��µ�����δʹ�õ�о
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


						if (iFundCoreSn==1) //�ҵ�о�߲Ÿ������ݣ����򲻸���
						{
							cSetComputeCoreSn.Edit() ;
							cSetComputeCoreSn.SetFieldValue(_T("core_sn"), strCableCoreSn) ;
							cSetComputeCoreSn.Update() ;
						}

						cSetComputeCoreSn.MoveNext() ;
					}
					cSetComputeCoreSn.Close() ;				

					//end ���������о�ߵı�� core_sn


					//begin ���㱸��,20100406ע���ٶ�������������
					CString strBackupStr = _T("") ;					
					CADORecordset cSetBackup ;
					//��о
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

					//�Խ�
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

					//��о��
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

					//end ���㱸��
					//20100405-1740 here	


					cSetAllBox.MoveNext() ;
				}
			}
			cSetAllBox.Close() ;
			//end �����ӱ��,о������

			//begin ����������
			this->ComputeFxData(iXHL_handle_low) ;
			//end ����������


			//���Ʊ��ΪiIndex������ͼ

			this->DrawPx(iIndex, iRsCount, pCurDb, iXHL_handle_low, dCurTotalYForOneDwg) ;
			dCurTotalYForOneDwg-=dCurTotalY ;
			dCurTotalYForOneDwg+= m_dCellHight*2 ;		

			iIndex++ ;

			cSet.MoveNext() ;
		}
		acedRestoreStatusBar() ;		
		ads_printf(_T("\n�������߽�����......\n")) ;

		cSet.Close() ;
		//end һ���ź�¥ent����һ��ͼֽ��ѭ����������ͼֽ

		CString strSaveFile ;
		strSaveFile.Format(_T("%s\\վ�ڵ�������(��ͼģʽ).dwg"), this->m_strSavePath) ;		
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

//���źŻ�ͼ�鼸���С���߳��ȱ�Ϊ0.01
BOOL CCableNetForBT::BatchChangeJYJ(AcDbDatabase *pCurDb)
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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{		
			//������ AcDbLine ;
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

//ȡ�øú���ǰһ������,����ZD6���ã�ȡ��˫���ĵ�2��
//modify:20111026,��ZYJ6-7Ҳ����
BOOL CCableNetForBT::GetPreviewEnt(long iCurEntHandle, long &iPreviewEntHandle)
{
	BOOL bRet = FALSE ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//��ʾ����ʼ(û��ǰһ�����ӵ�)��ent
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


int CCableNetForBT::GetAllConVirtualPLine()
{
	return 0 ;

}

//��������豸�����о�����4��������ֱ�ߣ�����һ����zhfbtlayer��Ĺ����豸ͼ�� ��
//���о���������acDbLine,��4����3
int CCableNetForBT::ReplaceWaySideEquipment(void)
{
	int iGpDevSum = 0 ;
	AcDbBlockReference *pRef = NULL ;

	struct resbuf *pRb ;	
	pRb = acutBuildList(RTDXF0, _T("LINE"),0) ;
	long iLen=0;
	CLongArray iArrayHandle ;	
	iLen = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;
	if (iLen>0)
	{
		for (int i=0; i<iLen; i++)
		{
			AcDbEntity *pEnt = NULL ;
			if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
			{
				//if (pEnt->isKindOf(AcDbLine::desc()))
				//{
				AcDbLine *pLine = AcDbLine::cast(pEnt) ;

				AcGePoint3d pt_3d_start = pLine->startPoint() ;
				AcGePoint3d pt_3d_end = pLine->endPoint() ;					
				acutPrintf(_T("\n%d"),i) ;

				//ֻ����ˮƽ���ҳ���Ϊ4����
				if (pt_3d_start.y == pt_3d_end.y)
				{					
					double dLen = fabs(pt_3d_end.x-pt_3d_start.x) ;
					if (4==dLen)
					{
						AcGePoint3d pt_3d_1(pt_3d_start.x,pt_3d_start.y-3,0), pt_3d_2(pt_3d_end.x,pt_3d_end.y-3,0) ;

						if(this->HasVerifyLine(pt_3d_start, pt_3d_1) && this->HasVerifyLine(pt_3d_end, pt_3d_2) && this->HasVerifyLine(pt_3d_1,pt_3d_2))
						{
							AcGePoint3d pt_insert = pt_3d_1+(pt_3d_2-pt_3d_1)/2;
							g_cSystem.Set_Layer(_T(""),_T("zhf_bt_cablenet")) ;
							if(g_cSystem.InsertBlock(pRef, _T("bt_dljl_box_gp"), pt_insert))
							{
								iGpDevSum++ ;
								pRef->close() ;
								pRef = NULL ;
							}
							g_cSystem.Set_Layer(_T(""),_T("0")) ;
                            
							pLine->upgradeOpen() ;
							pLine->erase() ;
						}

					}

				}
				//}
				pEnt->close() ;
			}
		}

		acutPrintf(_T("\n������ֱ��%d�������гɹ�ת�������豸%d"),iLen, iGpDevSum ) ;
		
	}


	return 0;
}

BOOL CCableNetForBT::HasVerifyLine(const AcGePoint3d pt_3d_1, const AcGePoint3d pt_3d_2)
{
	BOOL bRet = FALSE ;

	ads_point pt1={0,0,0}, pt2={0,0,0} ;
	g_cSystem.ConvertPt3DToAds(pt_3d_1, pt1) ;
	g_cSystem.ConvertPt3DToAds(pt_3d_2, pt2) ;

	g_cSystem.ZoomView(pt_3d_1+(pt_3d_2-pt_3d_1)/2, 10, 10) ;

	struct resbuf *pRb ;
	//�������յ�Ϊpt1/pt2��ֱ��
	pRb = acutBuildList(-4,_T("<and"),RTDXF0,_T("LINE"),-4,_T("<or"),-4,_T("<and"),10,pt1,11,pt2,-4,_T("and>"),-4,_T("<and"),10,pt2,11,pt1,-4,_T("and>"),-4,_T("or>"),-4,_T("and>"),0) ;
	ads_name sum_name ;	
	long iNum=0;
	CLongArray iArrayHandle ;
	iNum = g_cSystem.GetSSEntHandle(_T("X"), NULL, NULL, pRb,iArrayHandle) ;
	acutRelRb(pRb) ;	

	if (iNum>0)
	{
		bRet = TRUE ;
		for (int i=0; i<iNum; i++)
		{		
			AcDbEntity *pEnt = NULL ;
			if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForWrite))
			{
				pEnt->erase(TRUE) ;
				pEnt->close() ;
			}
		}
	}	
	
	return bRet ;
}

// ������ͼ��(һ��������,����һ��ʵ���ͷ)
int CCableNetForBT::ReplaceFS(void)
{
	int iGpDevSum = 0 ;
	
	acedCommand(RTSTR,_T("zoom"),RTSTR,_T("e"),0) ;

	AcDbBlockReference *pRef = NULL ;

	struct resbuf *pRb ;	
	pRb = acutBuildList(RTDXF0, _T("LINE"), 0) ;
	CLongArray iArrayHandle ;
	long iLen=0;
	iLen = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;
	
	if (iLen>0)
	{
		for (int i=0; i<iLen; i++)
		{
			acutPrintf(_T("\n%d"),i) ;
			
			AcDbEntity *pEnt = NULL ;
			if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
			{
				
				AcDbLine *pLine = AcDbLine::cast(pEnt) ;
				
				//ֻ������2�������
				//if (2==iVertNum)
				//{
					AcGePoint3d pt_3d_start(0,0,0),pt_3d_end(0,0,0) ;
					pLine->getStartPoint(pt_3d_start) ;
					pLine->getEndPoint(pt_3d_end) ;					
					

					//ֻ����ֱ���ҳ���Ϊ4����
					if (pt_3d_start.x == pt_3d_end.x)
					{		
						double dd1 = pt_3d_end.y-pt_3d_start.y ;
						double dd2 = double(dd1) ;
						double dd3 = (double)dd1 ;

						double dLen = fabs((double)(pt_3d_end.y-pt_3d_start.y)) ;
						if (fabs(dLen-1.42)<0.001)
						{
							double dVertButtomY = min(pt_3d_start.y, pt_3d_end.y) ;
							//ʵ���ͷ���ڵ�windows
							AcGePoint3d pt_3d_1(pt_3d_start.x-0.6,dVertButtomY+0.1,0), pt_3d_2(pt_3d_start.x+0.6,dVertButtomY-2.6,0) ;

							//ads_point pt1 = {pt_3d_start.x-20,dVertButtomY+20,0}, pt2 = {pt_3d_start.x+20,dVertButtomY-20,0} ;							
							//acedCommand(RTSTR,_T("zoom"),RTSTR,_T("w"),RTPOINT,pt1, RTPOINT, pt2,0) ;

							int iFindTxtLeftOrRight = 0 ;
							CString strName = _T("") ;

							if(this->HasVerifySolid(pt_3d_1, pt_3d_2, strName, iFindTxtLeftOrRight))
							{
								AcGePoint3d pt_insert(pt_3d_start.x,dVertButtomY-2.5,0);
								g_cSystem.Set_Layer(_T(""),_T("zhf_bt_cablenet")) ;
								CStringArray strArrayAtt ;
								strArrayAtt.Add(strName) ;
								if(g_cSystem.InsertBlock(pRef, _T("bt_dljl_dev_fs"), pt_insert, &strArrayAtt,FALSE))
								{
									//�ַ�����
									if (iFindTxtLeftOrRight==10)
									{
										this->TransformAtt(pRef, 5) ;									
									}

									iGpDevSum++ ;
									pRef->close() ;
									pRef = NULL ;
// 									pLine->upgradeOpen() ;
// 									pLine->erase() ;
								}
								g_cSystem.Set_Layer(_T(""),_T("0")) ;
							}

						}

					}

				//}

				pEnt->close() ;
			}
		}

		acutPrintf(_T("\n��������%d�������гɹ�ת��%d"),iLen, iGpDevSum ) ;
		
	}

	return 0;
}

// //�����Ƿ������pt_3d_1-3Ϊ�����solidʵ��
BOOL CCableNetForBT::HasVerifySolid(const AcGePoint3d pt_3d_1, const AcGePoint3d pt_3d_2, CString &strName, int &iFindTxtLeftOrRight)
{
	BOOL bRet = FALSE ;

	iFindTxtLeftOrRight = 0 ;

	ads_point pt1={0,0,0}, pt2={0,0,0} ;
	g_cSystem.ConvertPt3DToAds(pt_3d_1, pt1) ;
	g_cSystem.ConvertPt3DToAds(pt_3d_2, pt2) ;

	g_cSystem.ZoomView(pt_3d_1+(pt_3d_2-pt_3d_1)/2, 50, 50) ;
	struct resbuf *pRb ;	
	pRb = acutBuildList(RTDXF0,_T("SOLID"),0) ;
	long iNum=0;
	CLongArray iArrayHandle ;
	iNum = g_cSystem.GetSSEntHandle(_T("W"), &pt1, &pt2, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	if (iNum>0)
	{
		//��ʵ�����һ����Χ����, ���Ͻ�pt21�����½ǣ�pt22
		ads_point pt21 = {pt_3d_1.x-5,pt_3d_1.y,0}, pt22={pt_3d_1.x,pt_3d_2.y,0} ;
		struct resbuf *pRb2 ;
		pRb2 = acutBuildList(RTDXF0,_T("TEXT"),0) ;
		long iNum2 = 0 ;
		CLongArray iArrayHandle2 ;
		iNum2 = g_cSystem.GetSSEntHandle(_T("C"), &pt21, &pt22, pRb2, iArrayHandle2) ;
		acutRelRb(pRb2) ;				
		if (iNum2>0)
		{
			iFindTxtLeftOrRight+=1 ;
			AcDbEntity *pEnt2 = NULL ;				
			if(g_cSystem.OpenAcDbEntity(iArrayHandle2.GetAt(0), pEnt2,AcDb::kForRead))
			{
				AcDbText *pText2 = AcDbText::cast(pEnt2) ;
				strName	= pText2->textString() ;
				pEnt2->close() ;
			}
		}			

		//��ʵ���Ҳ�һ����Χ����, ���Ͻ�pt31�����½ǣ�pt32
		ads_point pt31 = {pt_3d_2.x,pt_3d_1.y,0}, pt32={pt_3d_2.x+5,pt_3d_2.y,0} ;
		struct resbuf *pRb3 ;
		pRb3 = acutBuildList(RTDXF0,_T("TEXT"),0) ;
		long iNum3 = 0 ;
		CLongArray iArrayHandle3 ;
		iNum3 = g_cSystem.GetSSEntHandle(_T("C"), &pt31, &pt32, pRb3, iArrayHandle3) ;
		acutRelRb(pRb3) ;		
		if (iNum3>0)
		{
			iFindTxtLeftOrRight+=10 ;
			AcDbEntity *pEnt3 = NULL ;				
			if(g_cSystem.OpenAcDbEntity(iArrayHandle3.GetAt(0), pEnt3,AcDb::kForRead))
			{
				AcDbText *pText3 = AcDbText::cast(pEnt3) ;
				strName	= pText3->textString() ;
				pEnt3->close() ;
			}
		}

	}	

	if (iFindTxtLeftOrRight!=0&&iFindTxtLeftOrRight!=11)
	{
		bRet = TRUE ;
	}

	return bRet ;

}

// �������ͼ�Σ�һ��ԲȦ��һ��ʮ�֣��¶˳�ԲȦ��
int CCableNetForBT::ReplaceJS(void)
{
	AcDbBlockReference *pRef = NULL ;

	struct resbuf *pRb ;
	pRb = acutBuildList(-4,_T("<and"),RTDXF0,_T("CIRCLE"), 40,1.225, -4, _T("and>"), 0) ;	
	long iLen=0;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	int iGpDevSum = 0 ;

	for (int i=0; i<iLen; i++)
	{
		acutPrintf(_T("\n%d"),i) ;

		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
		{
			AcDbCircle *pCircle = AcDbCircle::cast(pEnt) ;
			AcGePoint3d pt_3d_center = pCircle->center() ;

			int iFindTxtLeftOrRight = 0 ;
			CString strName = _T("") ;

			if(this->HasVerifyJS(pt_3d_center,strName, iFindTxtLeftOrRight))
			{					
				g_cSystem.Set_Layer(_T(""),_T("zhf_bt_cablenet")) ;
				CStringArray strArrayAtt ;
				strArrayAtt.Add(strName) ;
				if(g_cSystem.InsertBlock(pRef, _T("bt_dljl_dev_js"), pt_3d_center, &strArrayAtt,FALSE))
				{
					//�ַ�����
					if (iFindTxtLeftOrRight==10)
					{
						this->TransformAtt(pRef, 5) ;
					}

					iGpDevSum++ ;
					pRef->close() ;
					pRef = NULL ;
				}
				g_cSystem.Set_Layer(_T(""),_T("0")) ;
			}

			pEnt->close() ;
		}
	}

	acutPrintf(_T("\n������Բ%d�����гɹ�ת��%d"),iLen, iGpDevSum ) ;	

	return 0;
}

// ȷ��Բ���Ƿ���ʮ�������Ա�һ������������
BOOL CCableNetForBT::HasVerifyJS(const AcGePoint3d & pt_3d_center, CString & strName, int &iFindTxtLeftOrRight)
{
	BOOL bRet = FALSE ;

	iFindTxtLeftOrRight = 0 ;

	ads_point pt_center = {0,0,0}, pt_left={0,0,0}, pt_right={0,0,0}, pt_top={0,0,0}, pt_bottom={0,0,0} ;
	g_cSystem.ConvertPt3DToAds(pt_3d_center, pt_center) ;
	pt_left[0] = pt_center[0]-1.225 ;
	pt_left[1] = pt_right[1] = pt_center[1] ;
	pt_right[0] = pt_center[0]+1.225 ;	
	pt_top[0] = pt_bottom[0] = pt_center[0] ;
	pt_top[1] = pt_center[1]+1.225 ;
	pt_bottom[1] = pt_top[1]-3.92 ;

	g_cSystem.ZoomView(pt_3d_center, 50,50) ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0,_T("LINE"),0) ;
	long iNum=0;
	CLongArray iArrayHandle ;
	iNum = g_cSystem.GetSSEntHandle(_T("W"), &pt_left, &pt_right, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	if (iNum>0)
	{
		struct resbuf *pRb1 ;
		pRb1 = acutBuildList(RTDXF0,_T("LINE"),0) ;
		long iNum_1=0;
		CLongArray iArrayHandle1 ;
		iNum_1 = g_cSystem.GetSSEntHandle(_T("W"), &pt_top, &pt_bottom, pRb1, iArrayHandle1) ;
		acutRelRb(pRb1) ;
		if (iNum_1>0)
		{
			//��ʵ�����һ����Χ����, ���Ͻ�pt21�����½ǣ�pt22
			ads_point pt21 = {pt_3d_center.x-5,pt_top[1],0}, pt22={pt_3d_center.x,pt_bottom[1],0} ;
			struct resbuf *pRb2 ;
			pRb2 = acutBuildList(RTDXF0,_T("TEXT"),0) ;
			long iNum2 = 0 ;
			CLongArray iArrayHandle2 ;
			iNum2 = g_cSystem.GetSSEntHandle(_T("C"), &pt21, &pt22, pRb2, iArrayHandle2) ;
			acutRelRb(pRb2) ;

			if (iNum2>0)
			{
				iFindTxtLeftOrRight+=1 ;
				AcDbEntity *pEnt2 = NULL ;					
				if(g_cSystem.OpenAcDbEntity(iArrayHandle2.GetAt(0), pEnt2,AcDb::kForRead))
				{
					AcDbText *pText2 = AcDbText::cast(pEnt2) ;
					strName	= pText2->textString() ;
					pEnt2->close() ;
				}
			}			

			//��ʵ���Ҳ�һ����Χ����, ���Ͻ�pt31�����½ǣ�pt32
			ads_point pt31 = {pt_3d_center.x,pt_top[1],0}, pt32={pt_3d_center.x+5,pt_bottom[1],0} ;
			struct resbuf *pRb3 ;
			pRb3 = acutBuildList(RTDXF0,_T("TEXT"),0) ;
			long iNum3 = 0 ;
			CLongArray iArrayHandle3 ;
			iNum3 = g_cSystem.GetSSEntHandle(_T("C"), &pt31, &pt32, pRb3, iArrayHandle3) ;
			acutRelRb(pRb3) ;

			if (iNum3>0)
			{
				iFindTxtLeftOrRight+=10 ;
				AcDbEntity *pEnt3 = NULL ;				
				if(g_cSystem.OpenAcDbEntity(iArrayHandle3.GetAt(0), pEnt3,AcDb::kForRead))
				{
					AcDbText *pText3 = AcDbText::cast(pEnt3) ;
					strName	= pText3->textString() ;
					pEnt3->close() ;
				}
			}

		}

	}

	if (iFindTxtLeftOrRight!=0&&iFindTxtLeftOrRight!=11)
	{
		bRet = TRUE ;
	}

	return bRet ;
}

// �������������ƫ��һ����ֵ��Ĭ��iDirect=10������
void CCableNetForBT::TransformAtt(AcDbBlockReference* pRef, double dPlus, int iDirect)
{
	AcDbObjectIterator * pObjIter ;
	pObjIter = pRef->attributeIterator() ;
	for(pObjIter->start(); !pObjIter->done(); pObjIter->step()) 
	{
		AcDbObjectId attId ;
		attId = pObjIter->objectId() ;
		AcDbAttribute *pAttrib = NULL ;
		Acad::ErrorStatus es = pRef->openAttribute(pAttrib, attId, AcDb::kForWrite) ;
		if (Acad::eOk==es)
		{								
			AcDbExtents ext1 ;
			pAttrib->getGeomExtents(ext1) ;

			AcGeMatrix3d  xform ;
			AcGePoint3d ptmin = ext1.minPoint() ;
			AcGePoint3d ptmax = ext1.maxPoint() ;
			double dOffsetX =fabs(ptmax.x-ptmin.x)+dPlus ;
			if (1==iDirect)
			{
				dOffsetX = -fabs(ptmax.x-ptmin.x)+dPlus ;
			}
			AcGeVector3d vec(dOffsetX,0,0) ;
			xform.setToTranslation(vec) ;								
			pAttrib->transformBy(xform) ;

			pAttrib->close() ;
			pAttrib = NULL ;
		}

	}

	delete pObjIter ;
	pObjIter = NULL ;

}

// �������ͼ��
int CCableNetForBT::ReplaceHz(void)
{
// 	acedCommand(RTSTR,_T("zoom"),RTSTR,_T("e"),0) ;

	AcDbBlockReference *pRef = NULL ;

	struct resbuf *pRb ;
	pRb = acutBuildList(-4,_T("<and"),RTDXF0,_T("CIRCLE"), 40,3.000, -4, _T("and>"), 0) ;	
	long iLen=0;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	int iGpDevSum = 0 ;

	for (int i=0; i<iLen; i++)
	{
		acutPrintf(_T("\n%d"),i) ;

		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
		{
			AcDbCircle *pCircle = AcDbCircle::cast(pEnt) ;
			AcGePoint3d pt_3d_center = pCircle->center() ;

			g_cSystem.ZoomView(pt_3d_center, 50, 50) ;

			ads_point pt_topLeft = {pt_3d_center.x-3,pt_3d_center.y+3,0}, pt_bottomright = {pt_3d_center.x+3,pt_3d_center.y-3,0} ;
			struct resbuf * pRb2 ;
			pRb2 = acutBuildList(RTDXF0,_T("TEXT"),0) ;
			long iLen2 = 0 ;
			CLongArray iArrayHandle2 ;
			iLen2 = g_cSystem.GetSSEntHandle(_T("W"),&pt_topLeft, &pt_bottomright,pRb2,iArrayHandle2) ;
			acutRelRb(pRb2) ;
			if (1==iLen2)
			{

				AcDbEntity *pEnt2 = NULL ;
				if (g_cSystem.OpenAcDbEntity(iArrayHandle2.GetAt(0), pEnt2, AcDb::kForRead))
				{
					AcDbText *pText = AcDbText::cast(pEnt2) ;
					CString strText = pText->textString() ;

					g_cSystem.Set_Layer(_T(""),_T("zhf_bt_cablenet")) ;
					CStringArray strArrayAtt ;
					strArrayAtt.Add(strText) ;
					if(g_cSystem.InsertBlock(pRef, _T("bt_dljl_box_hz"), pt_3d_center, &strArrayAtt,FALSE))
					{
						iGpDevSum++ ;
						pRef->close() ;
						pRef = NULL ;
					}
					g_cSystem.Set_Layer(_T(""),_T("0")) ;

					pEnt2->close() ;
					pEnt2 = NULL ;
				}
			}
			else
			{
				acutPrintf(_T("\n������ӣ�ƥ��Բ���ж���1���ı�����Բ��Ϊ %.3f,%.3f������ "),pt_3d_center.x, pt_3d_center.y) ;
			}			

			pEnt->close() ;
		}
	}

	acutPrintf(_T("\n������Բ%d�����гɹ�ת������%d��"),iLen, iGpDevSum ) ;

	return 0;
}

// �����ź�¥��
BOOL CCableNetForBT::DrawXhlDz(void)
{
	AcDbBlockReference *pRef = NULL ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0,_T("LINE"), 0) ;	
	long iLen = 0 ;
	CLongArray iArrayHandle ;
	acutPrintf(_T("\nѡ���ź�¥��ֱ��")) ;
	iLen = g_cSystem.GetSSEntHandle(NULL, NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	if (iLen==0)
	{
		return FALSE ;
	}

	int iGpDevSum = 0 ;

	g_cSystem.ZoomView() ;

	for (int i=0; i<iLen; i++)
	{
		acutPrintf(_T("\n%d"),i) ;

		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
		{
			AcDbLine *pLine = AcDbLine::cast(pEnt) ;

			AcGePoint3d pt_3d_xhl_start(0,0,0), pt_3d_xhl_end(0,0,0) ;
			pLine->getStartPoint(pt_3d_xhl_start) ;
			pLine->getEndPoint(pt_3d_xhl_end) ;

			//��һ���ź�¥����X����
			if (m_dStationLocX==0.0)
			{
				m_dStationLocX = pt_3d_xhl_start.x ;
			}

			//acutPrintf(_T("\n%.3f-%.3f,%.3f-%.3f"), pt_3d_start.x, pt_3d_start.y,pt_3d_end.x,pt_3d_end.y) ;

			//pLine->setColorIndex(1) ;

			ads_point pt_topLeft = {0,0,0}, pt_bottomright = {0,0,0} ;
			g_cSystem.ConvertPt3DToAds(pt_3d_xhl_start, pt_topLeft) ;
			g_cSystem.ConvertPt3DToAds(pt_3d_xhl_end, pt_bottomright) ;

			struct resbuf * pRb2 ;
			pRb2 = acutBuildList(RTDXF0,_T("LINE"),0) ;
			long iLen2 = 0 ;
			CLongArray iArrayHandle2 ;
			iLen2 = g_cSystem.GetSSEntHandle(_T("C"),&pt_topLeft, &pt_bottomright,pRb2, iArrayHandle2) ;
			acutRelRb(pRb2) ;

			for(int i2=0; i2<iLen2; i2++)
			{						
				AcDbEntity *pEnt2 = NULL ;
				if (g_cSystem.OpenAcDbEntity(iArrayHandle2.GetAt(i2), pEnt2, AcDb::kForRead))
				{
					AcDbLine *pLine2 = AcDbLine::cast(pEnt2) ;
					AcGePoint3d pt_3d_start2(0,0,0), pt_3d_end2(0,0,0), pt_3d_tmp2(0,0,0) ;
					pLine2->getStartPoint(pt_3d_start2) ;
					pLine2->getEndPoint(pt_3d_end2) ;
					if (pt_3d_start2.x>pt_3d_end2.x) //��x����С���Ǹ���Ϊ��ʼ��
					{
						pt_3d_tmp2 = pt_3d_start2 ;
						pt_3d_start2 = pt_3d_end2 ;
						pt_3d_end2 = pt_3d_tmp2 ;
					}

					struct resbuf *pRb3 ;
					pRb3 = acutBuildList(RTDXF0, _T("TEXT"), 0) ;
					ads_point pt_topLeft3 = {pt_3d_start2.x-1,pt_3d_start2.y+5,0}, pt_bottomright3 = {pt_3d_end2.x+1, pt_3d_end2.y-1,0} ;
					long iLen3 = 0 ;
					CLongArray iArrayHandle3 ;
					iLen3 = g_cSystem.GetSSEntHandle(_T("W"), &pt_topLeft3, &pt_bottomright3, pRb3, iArrayHandle3) ;
					acutRelRb(pRb3) ;

					CString strText = _T("") ;

					if (1==iLen3)
					{
						if(g_cSystem.GetTextString(iArrayHandle3.GetAt(0), strText))
						{
							CString strName = _T("") ;
							strName = strText.Left(strText.Find(_T("-"))) ;
							AcGePoint3d pt_3d_insert(0,0,0) ;

							if (fabs(pt_3d_start2.x-pt_3d_xhl_start.x)<0.01) //����������ź�¥ֱ���ϣ�˵�����Ҳ�
							{	
								strName.Insert(0,_T("R_")) ;
								pt_3d_insert = pt_3d_start2 ;
								pt_3d_insert.x-=1 ;
// 								//��һ���ź�¥����X����
// 								if (m_dStationLocX==0.0)
// 								{
// 									m_dStationLocX = pt_3d_start.x ;
// 								}
							}
							else if(fabs(pt_3d_end2.x-pt_3d_xhl_start.x)<0.01) //���
							{
								strName.Insert(0,_T("L_")) ;
								pt_3d_insert = pt_3d_end2 ;
								pt_3d_insert.x+=1 ;
							}

							g_cSystem.Set_Layer(_T(""),_T("zhf_bt_cablenet")) ;
							CStringArray strArrayAtt ;
							strArrayAtt.Add(strName) ;
							if(g_cSystem.InsertBlock(pRef, _T("bt_dljl_box_xhl"), pt_3d_insert, &strArrayAtt,FALSE))
							{
								iGpDevSum++ ;
								pRef->close() ;
								pRef = NULL ;
							}
							g_cSystem.Set_Layer(_T(""),_T("0")) ;

						}
					}
					else
					{
						//acutPrintf(_T("\n�� %.3f,%.3f %.3f,%.3f ���ڷ��ֵ��ı���������1��"), pt_topLeft3.x,pt_topLeft3.y, pt_bottomright3.x ,pt_bottomright3.y) ;
					}
					pEnt2->close() ;
					pEnt2 = NULL ;
				}
			}
			

			pEnt->close() ;
		}
	}	

	//ɾ����2���ߣ����ű���box
	for (int i2=0; i2<iLen; i2++)
	{		
		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i2),pEnt,AcDb::kForWrite))
		{
			pEnt->erase(TRUE) ;
			pEnt->close() ;
		}
	}

	acutPrintf(_T("\n������Բ%d�����гɹ�ת������%d��"),iLen, iGpDevSum ) ;

	return TRUE ;
}

// ����������¾�·ͼ�����ݿ�
int CCableNetForBT::ReadBtDljlToDb(void)
{
	CDBUtility::ClearTable(_T("cablenet_ent")) ;
	CDBUtility::ClearTable(_T("connect_cable")) ;

	AcDbBlockReference *pRef = NULL ;

	g_cSystem.ZoomView() ;

	struct resbuf *pRb ;
	pRb = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"), 2,_T("bt_dljl_box_xhl"), -4, _T("and>"), 0) ;
	long iLen=0;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	int iGpDevSum = 0 ;

	acedSetStatusBarProgressMeter(_T("\n������¾�·ͼ"),0, iLen-1) ;

	for (int i=0; i<iLen; i++)
	{
		acedSetStatusBarProgressMeterPos(i) ;
		acutPrintf(_T("\n%d"),i) ;

		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
		{
			AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
			m_iHandleCurXhl = g_cSystem.GetHandle(pRef) ;
			CString strXhlName ;
			g_cSystem.GetAttValue(pRef, _T("NAME"), strXhlName) ;
			acutPrintf(_T("\n%s"), strXhlName) ;
			if (strXhlName.Left(1)==_T("R"))
			{
				m_iCurIsLeftOrRight = 1 ;
			}
			else if (strXhlName.Left(1)==_T("L"))
			{
				m_iCurIsLeftOrRight = 0 ;
			}			

			try
			{
				CString strSql ;
				strSql.Format(_T("insert into cablenet_ent(handle_low,devname,devtype,dev_for, boxtype,left_or_right, pt_x,pt_y, next_ent_handle_low,is_start, belong_xhl_handle_low,is_dianmahua) \
								 values(%d,'%s','%s','%s','%s',%d,%f,%f, %d, %d, %d, %d )"),\
								 m_iHandleCurXhl, strXhlName, _T(""), _T("BT"), _T("XHL"),m_iCurIsLeftOrRight, pRef->position().x, pRef->position().y,0, 0, m_iHandleCurXhl, 0) ;							
				ExecSqlString(strSql) ;
				//acutPrintf(_T("\n%s"),strSql) ;
			}
			catch (_com_error& e)
			{
				CString sBuff = CBlkUtility::GetErrorDescription(e) ;
				AfxMessageBox(sBuff) ;	
			}
			catch (...)
			{
				AfxMessageBox(_T("unknown error in ReadBtDljlToDb")) ;
			}
			
			this->LoopToFindBox(pRef, 0) ;

			pEnt->close() ;
		}
	}
	acedRestoreStatusBar() ;

	//acutPrintf(_T("\n������Բ%d�����гɹ�ת������%d��"),iLen, iGpDevSum ) ;
    
	this->DelLoopNumXdata() ;

	return 0;

}

// �������еĺ���,AcDbHandle hPreLine �Ǵ����ֱ�ߣ���ǰһ��box�����box����
int CCableNetForBT::LoopToFindBox(AcDbBlockReference * pRef,Adesk::UInt32 iPreLineHandle)
{
	AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
	this->GetExtent(pRef, pt_3d_min, pt_3d_max) ;
	ads_point pt_min = {0,0,0}, pt_max = {0,0,0} ;
	g_cSystem.ConvertPt3DToAds(pt_3d_min,pt_min) ;
	g_cSystem.ConvertPt3DToAds(pt_3d_max,pt_max) ;

	Adesk::UInt32 iHandleRef = g_cSystem.GetHandle(pRef) ;

	g_cSystem.ZoomView(pt_3d_min+(pt_3d_max-pt_3d_min)/2,fabs(pt_3d_max.y-pt_3d_min.y)+5, fabs(pt_3d_max.x-pt_3d_min.x)+5) ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0, _T("LINE"),0) ;
	long iLen = 0 ;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("C"), &pt_min, &pt_max, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	//�������ӵ����ӵ�ֱ�ߣ�Ҫ�ų�ǰһ�������ֱ�ߣ����Ѿ��߹���·�ߣ�
	for (int i=0; i<iLen; i++)
	{
		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt, AcDb::kForRead))
		{				
			Adesk::UInt32 iHandleCurLine = g_cSystem.GetHandle(pEnt) ;

			if (iPreLineHandle!=iHandleCurLine)//�ų�ǰһ������������
			{
				AcDbLine *pCurLine = AcDbLine::cast(pEnt) ;
				AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0), pt_3d_from(0,0,0) ;
				pt_3d_start = pCurLine->startPoint() ;
				pt_3d_end = pCurLine->endPoint() ;
				if (pt_3d_start.x>=pt_3d_min.x&&pt_3d_start.x<=pt_3d_max.x&&pt_3d_start.y>=pt_3d_min.y&&pt_3d_start.y<=pt_3d_max.y) //�����pRef ext��Χ��
				{
					pt_3d_from = pt_3d_start ;
				}
				else
				{
					pt_3d_from = pt_3d_end ;
				}
				
				CStringArray strArrayFindText ;
				Adesk::UInt32 iHandleFindPreLine = 0 ; 
				//ÿ�ҵ�һ��box��дһ�����ݿ�

				this->FindNextBox(pCurLine, pt_3d_from, pRef, strArrayFindText, iHandleFindPreLine) ;
				/*
				if(this->FindNextBox(pCurLine, pt_3d_from, pRefFind, strArrayFindText, iHandleFindPreLine))
				{
					CString strBlkName=_T(""), strDevName=_T(""), strDevType=_T(""), strDevFor=_T("BT"), strBoxType=_T(""),strCableString=_T("") ;
					int iIsSatart = 0 ;
					Adesk::UInt32 iHandleFindBox = 0 ;
					//����						
					g_cSystem.GetBlockName(pRefFind, strBlkName) ;
					//��handle
					iHandleFindBox = g_cSystem.GetHandle(pRefFind);									
					//���ӵ����ַ���
					if (strArrayFindText.GetSize()==1)
					{
						strCableString = strArrayFindText.GetAt(0) ;
					}
					else
					{
						CString strMsg ;
						AcGePoint3d pt_3d_blkfind = pRefFind->position() ;
						strMsg.Format(_T("ע�⣺���� %s,%.3f-%.3f �� %d �������ַ���Ӧ��Ψһ"),strBlkName,pt_3d_blkfind.x,pt_3d_blkfind.y, strArrayFindText.GetSize()) ;
						AfxMessageBox(strMsg) ;
					}

					CString strSql ;
					//һ�����ź�¥�˳����ķ�֧�������������豸box��������˴α�������
					if (strBlkName==_T("bt_dljl_box_gp"))
					{
						iIsSatart = 1 ;
						this->GetGpDev(pRefFind, strDevName, strDevType, strBoxType) ;
					}
					else if(strBlkName==_T("bt_dljl_box_hz")) //���ߺ�
					{
						g_cSystem.GetAttValue(pRefFind, _T("NAME"), strDevName) ;
						strDevName.TrimLeft() ;
						strDevName.TrimRight() ;
						if (strDevName.Left(2)==_T("J4") || strDevName.Left(1)==_T("4"))
						{
							strBoxType = _T("HF4") ;
						}
						else if (strDevName.Left(1)==_T("7"))
						{
							strBoxType = _T("HF7") ;
						}
					}
					//�������
					try
					{

						strSql.Format(_T("insert into cablenet_ent(handle_low,devname,devtype,dev_for, boxtype,left_or_right, pt_x,pt_y, next_ent_handle_low,cablestring, is_start, belong_xhl_handle_low,is_dianmahua) \
										 values(%d,'%s','%s','%s','%s',%d,%f,%f, %d, '%s', %d, %d, %d )"),\
										 iHandleFindBox, strDevName, strDevType, strDevFor, strBoxType,m_iCurIsLeftOrRight, pRefFind->position().x, pRefFind->position().y,iHandleRef,strCableString, iIsSatart, m_iHandleCurXhl, 0) ;							
						ExecSqlString(strSql) ;
						//acutPrintf(_T("\n%s"),strSql) ;

						Adesk::UInt32 iLeftEntHandle_Low=0,iRightEntHandle_Low=0 ;
						if (pRef->position().x>pRefFind->position().x)
						{
							iLeftEntHandle_Low = iHandleFindBox ;
							iRightEntHandle_Low = iHandleRef ;
						}
						else
						{
							iLeftEntHandle_Low = iHandleRef ;
							iRightEntHandle_Low = iHandleFindBox ;
						}

						strSql.Format(_T("insert into connect_cable(cablestring,left_ent_handle,right_ent_handle) \
										 values('%s',%d,%d)"),\
										 strCableString, iLeftEntHandle_Low, iRightEntHandle_Low) ;
						ExecSqlString(strSql) ;
						//acutPrintf(_T("\n%s"),strSql) ;
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
					if(strBlkName==_T("bt_dljl_box_hz")) //���ߺ�
					{
						this->LoopToFindBox(pRefFind, iHandleFindPreLine) ;
					}


					pRefFind->close() ;
					pRefFind = NULL ;
				}
				*/

			}

			pEnt->close() ;
		}
	}

	return 0;
}

// ȡ��ʵ�巶Χ�ĶԽ�pt
Acad::ErrorStatus CCableNetForBT::GetExtent(const AcDbEntity * pEnt, AcGePoint3d & pt_3d_min, AcGePoint3d & pt_3d_max)
{
	Acad::ErrorStatus es ;
	AcDbExtents ext1 ;
	es = pEnt->getGeomExtents(ext1) ;	
	if(Acad::eOk==es)
	{
		pt_3d_min = ext1.minPoint() ;
		pt_3d_max = ext1.maxPoint() ;
	}

	return es;
}

// ѭ��ֱ�߶˵㣬ֱ���ҵ���һ��box
//�߶��ν� ����˵㲶׽
//iHandlePreLine���ҵ�box���box��������ֱ�ߵ�handle����box��ǰһ��box����ֱ�߿��ܺü��Σ�
BOOL CCableNetForBT::FindNextBox(AcDbLine * &pLine, const AcGePoint3d pt_3d_from, AcDbBlockReference *& pRefPre, CStringArray &strArrayFindText, Adesk::UInt32 &iHandlePreLine)
{
	BOOL bRet = FALSE ;

	//ǰһ��box��handle
	Adesk::UInt32 iHandlePreBox = g_cSystem.GetHandle(pRefPre) ;
	//ǰһ����ֱ�ߵ�handle
	iHandlePreLine = g_cSystem.GetHandle(pLine) ;

	pLine->upgradeOpen() ;
	g_cSystem.SetXdata(pLine,_T("BT_DLJL_LINE_LOOPNUM"),1) ;
	pLine->downgradeOpen() ;

    g_cSystem.ZoomView(pLine->startPoint()+(pLine->endPoint()-pLine->startPoint())/2,fabs(pLine->endPoint().y-pLine->startPoint().y)+10, fabs(pLine->endPoint().x-pLine->startPoint().x)+10) ;

	g_cSystem.GetStringOnLine(pLine, 5, strArrayFindText, _T("_WP"),2,1) ; 

	AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0),pt_3d_to(0,0,0) ;
	pt_3d_start = pLine->startPoint() ;
	pt_3d_end = pLine->endPoint() ;
	if (pt_3d_start==pt_3d_from)
	{
		pt_3d_to = pt_3d_end ;
	}
	else
	{
		pt_3d_to = pt_3d_start ;
	}

	//find connect box
	/*
	read into db and looptofindbox
	*/
	Adesk::UInt32 iHandleFindBox = 0 ;
	if (this->FindBoxAtEndSide(pt_3d_to, iHandleFindBox))
	{
		CString strBlkName=_T(""), strDevName=_T(""), strDevType=_T(""), strDevFor=_T("BT"), strBoxType=_T(""),strCableString=_T("") ;
		int iIsSatart = 0 ;	

		AcDbEntity *pEntFindBox = NULL ;
		if (g_cSystem.OpenAcDbEntity(iHandleFindBox, pEntFindBox, AcDb::kForRead))
		{
			AcDbBlockReference *pRefFind = AcDbBlockReference::cast(pEntFindBox) ;

			//����						
			g_cSystem.GetBlockName(pRefFind, strBlkName) ;
											
			//���ӵ����ַ���
			if (strArrayFindText.GetSize()==1)
			{
				strCableString = strArrayFindText.GetAt(0) ;
			}
			else
			{
				CString strMsg ;
				AcGePoint3d pt_3d_blkfind = pRefFind->position() ;
				CString strFindTextTmp ;
				for (int i=0; i<strArrayFindText.GetCount(); i++)
				{
					strFindTextTmp.AppendFormat(_T(" %s"), strArrayFindText.GetAt(i)) ;
				}
				strMsg.Format(_T("ע�⣺���� %s,%.3f-%.3f �� %d �������ַ�(%s)��Ӧ��Ψһ"),strBlkName,pt_3d_blkfind.x,pt_3d_blkfind.y, strArrayFindText.GetSize(), strFindTextTmp) ;
				AfxMessageBox(strMsg) ;
			}
			//ÿ���ҵ����Ӻ���һ���׶εĽ�����ǰһ�����Ӻ͵�ǰ����֮��������ַ�����ʱ�Ѿ���¼����һ���׶�֮ǰ��strArrayFindText�������
			strArrayFindText.RemoveAll() ;
			CString strSql ;
			//һ�����ź�¥�˳����ķ�֧�������������豸box��������˴α�������
			if (strBlkName==_T("bt_dljl_box_gp"))
			{
				iIsSatart = 1 ;
				this->GetGpDev(pRefFind, strDevName, strDevType, strBoxType) ;
			}
			else if(strBlkName==_T("bt_dljl_box_hz")) //���ߺ�
			{
				g_cSystem.GetAttValue(pRefFind, _T("NAME"), strDevName) ;
				strDevName.TrimLeft() ;
				strDevName.TrimRight() ;
				if (m_nForProj==0) //����ǰ�ڹ��̣�ͼֽ��Ϊ�ń»���
				{
					if (strDevName.Left(2)==_T("J4") || strDevName.Left(1)==_T("4"))
					{
						strBoxType = _T("HF4") ;
					}
					else if (strDevName.Left(1)==_T("7"))
					{
						strBoxType = _T("HF7") ;
					}

				}
				else if (m_nForProj==1) //�������ڹ��̣�ͼֽ��ΪС���ṩ
				{
					if (strDevName.Right(1)==_T("4"))
					{
						strBoxType = _T("HF4") ;
					}
					else if (strDevName.Right(1)==_T("7"))
					{
						strBoxType = _T("HF7") ;
					}
				}
			}

			//�������
			try
			{
				strSql.Format(_T("insert into cablenet_ent(handle_low,devname,devtype,dev_for, boxtype,left_or_right, pt_x,pt_y, next_ent_handle_low,cablestring, is_start, belong_xhl_handle_low,is_dianmahua) \
								 values(%d,'%s','%s','%s','%s',%d,%f,%f, %d, '%s', %d, %d, %d )"),\
								 iHandleFindBox, strDevName, strDevType, strDevFor, strBoxType,m_iCurIsLeftOrRight, pRefFind->position().x, pRefFind->position().y,iHandlePreBox,strCableString, iIsSatart, m_iHandleCurXhl, 0) ;							
				ExecSqlString(strSql) ;
				//acutPrintf(_T("\n%s"),strSql) ;

				Adesk::UInt32 iLeftEntHandle_Low=0,iRightEntHandle_Low=0 ;
				if (pRefPre->position().x>pRefFind->position().x)
				{
					iLeftEntHandle_Low = iHandleFindBox ;
					iRightEntHandle_Low = iHandlePreBox ;
				}
				else
				{
					iLeftEntHandle_Low = iHandlePreBox ;
					iRightEntHandle_Low = iHandleFindBox ;
				}

				strSql.Format(_T("insert into connect_cable(cablestring,left_ent_handle,right_ent_handle) \
								 values('%s',%d,%d)"),\
								 strCableString, iLeftEntHandle_Low, iRightEntHandle_Low) ;
				ExecSqlString(strSql) ;
				//acutPrintf(_T("\n%s"),strSql) ;
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
			if(strBlkName==_T("bt_dljl_box_hz")) //���ߺ�
			{
				this->LoopToFindBox(pRefFind, iHandlePreLine) ;
			}

			pEntFindBox->close() ;
			bRet = TRUE ;
		}

	}
	//find connect line
	/*
	findnextbox
	*/
	CLongArray iArrayHandleFindLine ;
	AcGePoint3dArray ptArrayFromPt ;
	int iFindLineSum =this->FindConnectLine(pLine, iArrayHandleFindLine, ptArrayFromPt) ;
	for (int iLineIndex=0; iLineIndex<iFindLineSum; iLineIndex++)
	{
		Adesk::UInt32 iHandleCurLine = iArrayHandleFindLine.GetAt(iLineIndex) ;
		AcGePoint3d pt_3d_from = ptArrayFromPt.at(iLineIndex) ;

		AcDbEntity *pCurEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iHandleCurLine,pCurEnt, AcDb::kForRead))
		{
			AcDbLine *pCurLine = AcDbLine::cast(pCurEnt) ;
			bRet = FindNextBox(pCurLine,pt_3d_from, pRefPre,strArrayFindText, iHandlePreLine) ;

			pCurEnt->close() ;
		}	

	}


	/*
	
	ads_point pt_topleft = {0,0,0}, pt_bottomright = {0,0,0} ;
	pt_topleft[0] = pt_3d_to.x-0.0011 ;
	pt_topleft[1] = pt_3d_to.y+0.001 ;
	pt_bottomright[0] = pt_3d_to.x+0.001 ;
	pt_bottomright[1] = pt_3d_to.y-0.001 ;

	g_cSystem.ZoomView(pt_3d_to,10, 10) ;
	
	struct resbuf *pRb ;
	pRb = acutBuildList(-4,_T("<OR"),RTDXF0,_T("LINE"),RTDXF0,_T("INSERT"), -4,_T("OR>"), 0) ;
	long iLen = 0 ;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("C"),&pt_topleft, &pt_bottomright, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;
	
	for(int i=0; i<iLen; i++)
	{
		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt, AcDb::kForRead))
		{
			Adesk::UInt32 iHandelCurEnt = g_cSystem.GetHandle(pEnt) ;
			if (iHandelCurEnt==iHandlePreLine) //pt_to��get����line����insert��������pt_toΪ�˵�ֱ��pLine����Ҫ�ų�
			{
				pEnt->close() ;
				continue;
			}
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRefTmp = AcDbBlockReference::cast(pEnt) ;
				CString strBlkName ;
				g_cSystem.GetBlockName(pRefTmp, strBlkName) ;
				if (strBlkName.Left(11)==_T("bt_dljl_box"))
				{
					pRef = pRefTmp ;
					bRet = TRUE ;
					pEnt->close() ;
					break;
				}
			}
			else if (pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine *pLineNext = AcDbLine::cast(pEnt) ;
				bRet = FindNextBox(pLineNext,pt_3d_to, pRef,strArrayFindText, iHandlePreLine) ;
				pEnt->close() ;
				break;
			}

			pEnt->close() ;
		}
	}
	*/
	
	//else
// 	{
// 		bRet = FALSE ;
// 		CString strMsg ;
// 		strMsg.Format(_T("�ڶ˵� %.3f-%.3f ������box����ֱ�߸���Ϊ%d"),pt_to[0], pt_to[1], iLen) ;
// 		AfxMessageBox(strMsg) ;
// 	}

	return bRet ;
}



// ȡ�ù����豸����
int CCableNetForBT::GetGpDev(const AcDbBlockReference * pRefGpBox, CString & strDevName, CString & strDevType, CString & strBoxType)
{
	int iFind = 0 ;
	//strBoxType = _T("HZ12") ;

	AcGePoint3d pt_3d_gpbox = pRefGpBox->position() ;

	ads_point pt_lefttop = {0,0,0}, pt_bottomright = {0,0,0} ;
	if (m_iCurIsLeftOrRight==0)
	{
		pt_lefttop[0] = pt_3d_gpbox.x-5.5 ;		
		pt_bottomright[0] = pt_3d_gpbox.x-2.5 ;
	}
	else
	{
		pt_lefttop[0] = pt_3d_gpbox.x+2.5 ;		
		pt_bottomright[0] = pt_3d_gpbox.x+5.5 ;
	}
	pt_lefttop[1] = pt_3d_gpbox.y+2.8 ;
	pt_bottomright[1] = pt_3d_gpbox.y+0.2 ;

	//g_cSystem.AddAcDbLine(pt_lefttop, pt_bottomright, 1) ;

	g_cSystem.ZoomView(pt_3d_gpbox,50,50) ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0,_T("INSERT"),0) ;
	long iLen = 0 ;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("C"),&pt_lefttop, &pt_bottomright, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	for (int i=0; i<iLen; i++)
	{
		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i), pEnt, AcDb::kForRead))
		{
			AcDbBlockReference *pFindRef = AcDbBlockReference::cast(pEnt) ;
			CString strBlkName ;
			g_cSystem.GetBlockName(pFindRef, strBlkName) ;
			g_cSystem.GetAttValue(pFindRef,_T("NAME"),strDevName) ;
			strDevName.Trim() ;

			if (strBlkName==_T("bt_dljl_dev_other"))
			{	
				g_cSystem.GetAttValue(pFindRef,_T("TYPE"),strDevType) ;

				strDevType.Trim() ;
				if (strDevType.IsEmpty())
				{
					strBoxType = _T("HZ12") ;
					if (strDevName.Left(2)==_T("AX"))
					{
						strDevType = _T("bt_ax_0") ;							
					}
					else if (strDevName.Right(3)==_T("BOX"))
					{
						strDevType = _T("bt_zzj_box") ;
					}
					else if (strDevName.Right(3)==_T("IND"))
					{
						strDevType = _T("bt_zzj_ind") ;
					}
					else if (strDevName.Right(1)==_T("D"))
					{
						strDevType = _T("bt_zzj_d") ;
					}
				}
				else 
				{
					if (strDevType==_T("bt_xhj_g1y3r1w2"))
					{
						strBoxType = _T("HZ24") ;
					}
					else
					{
						strBoxType = _T("HZ12") ;
					}
				}

				iFind++ ;
			}
			else if (strBlkName.Left(11)==_T("bt_dljl_dev"))
			{
				strDevType = strBlkName ;
				strBoxType = _T("HZ12") ;
		
				iFind++ ;
			}

			pEnt->close() ;
		}
	}

	if (iFind>1)
	{
		CString strMsg ;
		strMsg.Format(_T("ע���飺���Ժ��� %.3f-%.3f,�������ֶ���һ���豸��"), pt_3d_gpbox.x, pt_3d_gpbox.y) ;
		AfxMessageBox(strMsg) ; 
	}

	return 0;
}


// �����źŻ���ת�޻���������ն�ͼ���ı�
int CCableNetForBT::ReplaceOther(void)
{
	acedCommand(RTSTR,_T("zoom"),RTSTR,_T("e"),0) ;    
	
// 	m_dStationLocX = 529739.00 ;
	
	struct resbuf *pRb ;
	pRb = acutBuildList(-4,_T("<and"),RTDXF0,_T("INSERT"), 2,_T("bt_dljl_box_gp"), -4, _T("and>"), 0) ;	
	long iLen=0;
	CLongArray iArrayHandle ;
	iLen = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;

	int iGpDevSum = 0 ;		

	for (int i=0; i<iLen; i++)
	{
		acutPrintf(_T("\n%d"),i) ;

		AcDbEntity *pEnt = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
		{				
			AcDbBlockReference *pRefGpBox = AcDbBlockReference::cast(pEnt) ;
			AcGePoint3d pt_3d_gpbox = pRefGpBox->position() ;
			int iLeftOrRight = -1 ;
			iLeftOrRight = pt_3d_gpbox.x<m_dStationLocX?-1:1 ;

			ads_point pt_topLeft = {0,0,0}, pt_bottomright = {0,0,0} ;
			//�����
			if (iLeftOrRight==-1)
			{
				pt_topLeft[0] = pt_3d_gpbox.x-5.5 ;		
				pt_bottomright[0] = pt_3d_gpbox.x-2.2 ;
			}
			else //�Ҳ�
			{
				pt_topLeft[0] = pt_3d_gpbox.x+2.2 ;		
				pt_bottomright[0] = pt_3d_gpbox.x+5.5 ;
			}
			pt_topLeft[1] = pt_3d_gpbox.y+2.8 ;
			pt_bottomright[1] = pt_3d_gpbox.y+0.2 ;

			//�鿴���Ա�����bt_dljl_�飬���򲻴���
			struct resbuf * pRb3 ;
			pRb3 = acutBuildList(RTDXF0,_T("INSERT"),0) ;				
			long iLen3 = 0 ;
			CLongArray iArrayHandle3 ;
			iLen3 = g_cSystem.GetSSEntHandle(_T("C"),&pt_topLeft, &pt_bottomright,pRb3, iArrayHandle3) ;
			acutRelRb(pRb3) ;
			BOOL bHaveBlk = FALSE ;

			for (int i3=0; i3<iLen3; i3++)
			{
				AcDbEntity *pEnt = NULL ;
				if (g_cSystem.OpenAcDbEntity(iArrayHandle3.GetAt(i3), pEnt, AcDb::kForRead))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
					CString strBlkName ;
					g_cSystem.GetBlockName(pRef, strBlkName) ;
					if (strBlkName!=_T("bt_dljl_box_gp"))
					{
						bHaveBlk = TRUE ;
					}

					pEnt->close() ;
				}
			}				

			if (bHaveBlk==FALSE)
			{
				struct resbuf * pRb2 ;
				pRb2 = acutBuildList(RTDXF0,_T("TEXT"),0) ;
				long iLen2 = 0 ;
				CLongArray iArrayHandle2 ;
				iLen2 = g_cSystem.GetSSEntHandle(_T("C"),&pt_topLeft, &pt_bottomright,pRb2, iArrayHandle2) ;
				acutRelRb(pRb2) ;						

				acutPrintf(_T("\n text len %d"), iLen2) ;
				if (1==iLen2)
				{

					AcDbEntity *pEnt2 = NULL ;
					if (g_cSystem.OpenAcDbEntity(iArrayHandle2.GetAt(0), pEnt2, AcDb::kForRead))
					{							
						AcDbText *pFindText = AcDbText::cast(pEnt2) ;
						CString strText ;
						strText = pFindText->textString() ;
						acutPrintf(_T("\n%d:%s"),i,strText) ;

						strText.Trim() ;
						CString strInsertBlkName = _T("bt_dljl_dev_other") ;
						CString strType = _T("") ;
						if (CBlkUtility::IsStrMatch(_T("^3\\d{2}$"), strText))  //�������� ���� 301
						{								
							strType = _T("bt_xhj_g0y0r0w3") ;
						}
						else if (CBlkUtility::IsStrMatch(_T("^1\\d{2}[AB]?$"), strText))	//�綯ת�޻� ���� 111/111A/111B
						{
							strType = _T("bt_zzj_dd") ;

						}
						else if (CBlkUtility::IsStrMatch(_T("^[UD]\\d+$"), strText)||CBlkUtility::IsStrMatch(_T("^(AS|OH)?S\\d+$"), strText)) //�г� ���� U5 D17/S4 S5 ASS11 OHS2
						{
							this->GetXhjType(strText, strType) ;									
						}


						g_cSystem.Set_Layer(_T(""),_T("zhf_bt_cablenet")) ;
						CStringArray strArrayAtt ;
						strArrayAtt.Add(strText) ;
						strArrayAtt.Add(strType) ;
						AcDbBlockReference *pRef = NULL ;
						AcGePoint3d pt_3d_insert(0,0,0) ;
						pt_3d_insert.x = pt_3d_gpbox.x+iLeftOrRight*4 ;
						pt_3d_insert.y = pt_3d_gpbox.y ;
						if(g_cSystem.InsertBlock(pRef, strInsertBlkName, pt_3d_insert, &strArrayAtt,FALSE))
						{
							if (iLeftOrRight==1)
							{
								this->TransformAtt(pRef,0) ;
							}
							iGpDevSum++ ;
							pRef->close() ;
							pRef = NULL ;
						}
						g_cSystem.Set_Layer(_T(""),_T("0")) ;							

						pEnt2->close() ;
						pEnt2 = NULL ;
					}
				}
				else
				{
					acutPrintf(_T("\n����gpbox�Աߵ�xhj��zzj�������:ƥ�����ж���1���ı���%.3f,%.3f������ "),pt_3d_gpbox.x, pt_3d_gpbox.y) ;
				}					
			}	
			else
			{
				acutPrintf(_T("\ngpbox�Ա���������1���豸�飬%.3f,%.3f�������ٴ��� "),pt_3d_gpbox.x, pt_3d_gpbox.y) ;
			}			

			pEnt->close() ;
		}
	}

	acutPrintf(_T("\n������%d�����гɹ�ת��%d��"),iLen, iGpDevSum ) ;

	return 0;
}

// ʶ���źŻ�ͼ��
int CCableNetForBT::ReplaceXhj(void)
{
	acedCommand(RTSTR,_T("zoom"),RTSTR,_T("e"),0) ;

	struct resbuf *pRb1 ;
	pRb1 = acutBuildList(RTDXF0,_T("TEXT"), 0) ;
	long iLen1=0;
	CLongArray iArrayHandle1 ;
	iLen1 = g_cSystem.GetSSEntHandle(_T("_X"), NULL, NULL, pRb1, iArrayHandle1) ;
	acutRelRb(pRb1) ;

	int iGpDevSum = 0 ;		

	for (int i=0; i<iLen1; i++)
	{
		AcDbEntity *pEnt1 = NULL ;
		if (g_cSystem.OpenAcDbEntity(iArrayHandle1.GetAt(i),pEnt1,AcDb::kForRead))
		{				
			AcDbText *pText = AcDbText::cast(pEnt1) ;
			AcGePoint3d pt_3d_text = pText->position() ;
			CString strText = pText->textString() ;
			acutPrintf(_T("\n%d(%d):%s"),i, strText.GetLength(),strText) ;
			ads_point pt_topleft = {0,0,0}, pt_bottomright={0,0,0} ;

			if (m_nForProj==0&&CBlkUtility::IsStrMatch(_T("^[UD]\\d+$"), strText) || m_nForProj==1&&CBlkUtility::IsStrMatch(_T("^(ASS|OHS)?\\d+$"), strText))
			{
				if (m_nForProj==0)
				{
					if (strText.Left(1)==_T("D"))
					{
						pt_topleft[0] = pt_3d_text.x-5 ;
						pt_topleft[1] = pt_3d_text.y+10 ;
						pt_bottomright[0] = pt_3d_text.x+35 ;
						pt_bottomright[1] = pt_3d_text.y-10 ;
					}
					else
					{
						pt_topleft[0] = pt_3d_text.x-35 ;
						pt_topleft[1] = pt_3d_text.y+10 ;
						pt_bottomright[0] = pt_3d_text.x+5 ;
						pt_bottomright[1] = pt_3d_text.y-10 ;
					}
				}
				else if (m_nForProj==1)
				{
					int iSignalNum = _tstoi(strText.Right(1)) ;
					if ((m_iDownLineIsUp==0&&iSignalNum%2==0 )||( m_iDownLineIsUp==1&&iSignalNum%2!=0)) //modified��20140520�����л������ϱ�ʱ�������źŻ�������Ϊż��
					{
						pt_topleft[0] = pt_3d_text.x-5 ;
						pt_topleft[1] = pt_3d_text.y+15 ; //20140424:old +10 ;
						pt_bottomright[0] = pt_3d_text.x+35 ;
						pt_bottomright[1] = pt_3d_text.y-10 ;
					}
					else
					{
						pt_topleft[0] = pt_3d_text.x-35 ;
						pt_topleft[1] = pt_3d_text.y+10 ;
						pt_bottomright[0] = pt_3d_text.x+5 ;
						pt_bottomright[1] = pt_3d_text.y-15 ;
					}
				}


				//g_cSystem.AddAcDbLine(pt_topleft, pt_bottomright,1) ;

				struct resbuf *pRb2 ;
				pRb2 = acutBuildList(RTDXF0, _T("CIRCLE"), 0) ;
				long iLen2 = 0 ;
				CLongArray iArrayHandle2 ;
				iLen2 = g_cSystem.GetSSEntHandle(_T("_W"), &pt_topleft, &pt_bottomright, pRb2, iArrayHandle2) ;
				acutRelRb(pRb2) ;

				int iWhite = 0, iGreen = 0, iYellow = 0, iRed = 0 ;

				for (int i2=0; i2<iLen2; i2++)
				{							
					AcDbEntity *pEnt2 = NULL ;
					if (g_cSystem.OpenAcDbEntity(iArrayHandle2.GetAt(i2),pEnt2, AcDb::kForRead))
					{
						AcDbCircle *pCircle = AcDbCircle::cast(pEnt2) ;
						double dRadius = pCircle->radius() ;
						if (fabs(dRadius-0.5)<0.01)//white
						{
							iWhite++ ;
						}
						else if (fabs(dRadius-2)<0.01)//RGY
						{
							AcGePoint3d pt_3d_center = pCircle->center() ;
							ads_point pt_topleft3 = {0,0,0}, pt_bottomright3 = {0,0,0} ;
							pt_topleft3[0] = pt_3d_center.x-dRadius-0.1 ;
							pt_topleft3[1] = pt_3d_center.y+dRadius+0.1 ;
							pt_bottomright3[0] = pt_3d_center.x+dRadius+0.1 ;
							pt_bottomright3[1] = pt_3d_center.y-dRadius-0.1 ;

							//g_cSystem.AddAcDbLine(pt_topleft3, pt_bottomright3,3) ;

							struct resbuf *pRb3 ;
							pRb3 = acutBuildList(RTDXF0, _T("LINE"),0) ;
							long iLen3 = 0 ;
							CLongArray iArrayHandle3 ;
							//��Բ�ھ��Σ���������ΪԲ�ģ��߳�Ϊ�뾶���ཻ��ֱ��							
							//iLen3 = g_cSystem.GetSSEntHandle(_T("C"), &pt_topleft3, &pt_bottomright3, pRb3, iArrayHandle3) ;
							//���ɴ�ѡ
							iLen3 = g_cSystem.GetSSEntHandle(_T("_W"), &pt_topleft3, &pt_bottomright3, pRb3, iArrayHandle3) ;
							acutRelRb(pRb3) ;

							int iHorLine = 0, iVertLine = 0, iAngleLine = 0 ;

							for (int i3=0; i3<iLen3; i3++)
							{											
								AcDbEntity *pEnt3 = NULL ;
								if (g_cSystem.OpenAcDbEntity(iArrayHandle3.GetAt(i3), pEnt3, AcDb::kForRead))
								{
									AcDbLine *pLine = AcDbLine::cast(pEnt3) ;
									AcGePoint3d pt_3d_start = pLine->startPoint() ;
									AcGePoint3d pt_3d_end = pLine->endPoint() ;
									double dLen = pt_3d_start.distanceTo(pt_3d_end) ;
									//Բ��ֱ�ߺ�Բֱ�����Ϊ0.1��
									//if (fabs(dLen-dRadius*2)<0.1)
									if (dLen>=dRadius*1.5 && dLen<=dRadius*2) //20140424:�߶γ��ڰ뾶*1.5��ֱ��֮��
									{
										if (fabs(pt_3d_start.x-pt_3d_end.x)<0.01)
										{
											iVertLine++ ;
										}
										else if (fabs(pt_3d_start.y-pt_3d_end.y)<0.01)
										{
											iHorLine++ ;
										}
										else
										{
											iAngleLine++ ;
										}
									}

									pEnt3->close() ;
								}
							}
							if (iHorLine==1)
							{
								iGreen++ ;
							}
							else if (iVertLine==2)
							{
								iRed++ ;
							}
							else if (iAngleLine==1)
							{
								iYellow++ ;
							}
						}

						pEnt2->close() ;
					}
				}						

				if ((iGreen+iYellow+iRed+iWhite)>0)
				{
					CString strType  ;
					strType.Format(_T("bt_xhj_g%dy%dr%dw%d"), iGreen, iYellow, iRed, iWhite) ;
					g_cSystem.SetXdata(pEnt1, _T("XHJTYPE"), strType) ;			
					iGpDevSum++ ;	
				}			

			}

			pEnt1->close() ;
		}
	}

	acutPrintf(_T("\n������%d�����гɹ�ת��%d��"),iLen1, iGpDevSum ) ;	

	return 0;
}

// ȡ���г��źŻ�����
BOOL CCableNetForBT::GetXhjType(const CString strXhjName, CString & strXhjType)
{
	BOOL bRet = FALSE ;

	AcDbBlockTable *pTb = NULL ;
	g_cSystem.m_pDb->getBlockTable(pTb,AcDb::kForRead) ;
	AcDbBlockTableRecord *pTbRec = NULL ;
	pTb->getAt(ACDB_MODEL_SPACE, pTbRec, AcDb::kForRead) ;
	pTb->close() ;
	AcDbBlockTableRecordIterator *pTbRecIter = NULL ;
	pTbRec->newIterator(pTbRecIter) ;
	pTbRec->close() ;

	for (pTbRecIter->start(); !pTbRecIter->done(); pTbRecIter->step())
	{
		AcDbEntity *pEnt = NULL ;
		if(Acad::eOk==pTbRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText *pText = AcDbText::cast(pEnt) ;
				CString strTextName = pText->textString() ;
				if (m_nForProj==0&&strTextName==strXhjName || m_nForProj==1&&(strTextName==strXhjName.Mid(1)||strTextName==strXhjName))
				{
					bRet = g_cSystem.GetXdata(pEnt, _T("XHJTYPE"), strXhjType) ;
				}
			}
			pEnt->close() ;
		}
		if (bRet)
		{
			break;
		}
	}
	delete pTbRecIter ;
	pTbRecIter = NULL ;


	return bRet ;
}

// for bt
BOOL CCableNetForBT::FillDB2(void)
{
	BOOL bRet = TRUE ;
	try	
	{				
		CADORecordset cSet ;
		CString strSql ;

		//begin ����cablenet_ent���branch
		strSql.Format(_T("select * from cablenet_ent")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql) )
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
		//end ����cablenet_ent���branch

		//begin ����cablenet_ent���belong_xhl_handle_low
		strSql.Format(_T("select * from cablenet_ent where is_start=1")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql) )
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iRsCount = cSet.GetRecordCount() ;
			//BOOL bSetProgressMod = iRsCount>50?TRUE:FALSE ;
			int iSumTmp = 0 ;
			//BOOL bHaveSetXHLRightOrLeft = FALSE ;
			while (!cSet.IsEOF()) 
			{
				long iHandleLow ;
				cSet.GetFieldValue(_T("handle_low"), iHandleLow) ;	

				CLongArray iArrayEntHandle ;
			//�ݹ������ihandleLow��ʵ���handle��Ϊ��ʼ�����ź�¥Ϊ����������·������ʵ��
			this->FindOneRoute(iHandleLow, iArrayEntHandle) ;
			int iEntSum = iArrayEntHandle.GetSize() ;
// 			int iXhlEntHandle = iArrayEntHandle.GetAt(iEntSum-1) ;
// 			for (int i=0; i<iEntSum; i++)
// 			{
// 				long iHandleTmp = iArrayEntHandle.GetAt(i) ;
// 				strSql.Format(_T("update cablenet_ent set belong_xhl_handle_low=%d where handle_low=%d"), iXhlEntHandle, iHandleTmp) ;
// 				ExecSqlString(strSql) ;	
// 				if (i==(iEntSum-1))
// 				{
// 					int iLeftOrRight = 0 ;
// 					cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
// 					strSql.Format(_T("update cablenet_ent set left_or_right=%d where handle_low=%d"), iLeftOrRight, iHandleTmp) ;
// 					ExecSqlString(strSql) ;
// 					//bHaveSetXHLRightOrLeft = TRUE ;
// 				}
// 			}
			//�Դ�ʵ��Ϊ��ʼ����·�ϵ�ʵ�����
			cSet.Edit() ;
			cSet.SetFieldValue(_T("route_num"), iEntSum) ;
			cSet.Update() ;

			iSumTmp ++ ;

			cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end ����cablenet_ent���belong_xhl_handle_low

		//begin ����ʹ��о�߸��ĵ綯ת�޻�bt_zzj_ddΪbt_zzj_dd1/bt_zzj_dd2
		strSql.Format(_T("select * from cablenet_ent where devtype='bt_zzj_dd'")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql) )
		{		
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF()) 
			{
				CString strCableStr ;			
				cSet.GetFieldValue(_T("cablestring"), strCableStr) ;

				CString strNewType ;
				CString strToFind = _T("4(1)") ;
				if (strCableStr.Find(strToFind)!=-1)
				{
					strNewType = _T("bt_zzj_dd1") ;
				}
				else
				{
					strNewType = _T("bt_zzj_dd2") ;
				}

				cSet.Edit() ;
				cSet.SetFieldValue(_T("devtype"), strNewType) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end ����ʹ��о�߸��ĵ綯ת�޻�bt_zzj_ddΪbt_zzj_dd1/bt_zzj_dd2


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

// ��ֱ�߶˵�����Ƿ��к��ӣ����ߺл���gpbox��
BOOL CCableNetForBT::FindBoxAtEndSide(const AcGePoint3d & pt_3d_endside, unsigned long & iFindBoxHandle)
{
	BOOL bRet = FALSE ;

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))				
			{
				AcDbBlockReference * pRef = NULL ;
				pRef = (AcDbBlockReference *)pEnt ;

				CString strBlkName ;
				g_cSystem.GetBlockName(pRef, strBlkName) ;
				if (strBlkName==_T("bt_dljl_box_hz")||strBlkName==_T("bt_dljl_box_gp"))
				{
					AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
					this->GetExtent(pRef, pt_3d_min, pt_3d_max) ;
					if (pt_3d_endside.x>=pt_3d_min.x && pt_3d_endside.x<=pt_3d_max.x && pt_3d_endside.y>=pt_3d_min.y && pt_3d_endside.y<=pt_3d_max.y)
					{
						iFindBoxHandle = g_cSystem.GetHandle(pRef) ;
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

	return bRet;
}

// ���Ҷ˵���pSrcLine�ϵ�ֱ��
int CCableNetForBT::FindConnectLine(const AcDbLine* pSrcLine, CLongArray & iArrayHandleFindLine, AcGePoint3dArray &ptArrayFrom)
{
	int iRet = 0 ;

	Adesk::UInt32 iHandleSrcLine = g_cSystem.GetHandle(pSrcLine) ;

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbLine::desc()))				
			{
				AcDbLine * pCurLine = NULL ;
				pCurLine = (AcDbLine *)pEnt ;
				Adesk::UInt32 iHandleCurLine = g_cSystem.GetHandle(pCurLine) ;
				if (iHandleCurLine!=iHandleSrcLine)
				{
					AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
					pt_3d_start = pCurLine->startPoint() ;
					pt_3d_end = pCurLine->endPoint() ;
					int iStartOrEndPtInLine = 0 ;
					AcGePoint3d pt_3d_from(0,0,0) ;
					if(g_cSystem.IsPointInLine(pt_3d_start, pSrcLine))
					{
						iStartOrEndPtInLine+=1 ;
						pt_3d_from = pt_3d_start ;
					}
					if(g_cSystem.IsPointInLine(pt_3d_end, pSrcLine))
					{
						iStartOrEndPtInLine+=10 ;
						pt_3d_from = pt_3d_end ;
					}
					//���iStartOrEndPtInLine==0,�㲻סֱ����; iStartOrEndPtInLine==11,2���㶼��ֱ����
					if (iStartOrEndPtInLine==1||iStartOrEndPtInLine==10)
					{
						int iLoopNum = 0 ;
						g_cSystem.GetXdata(pEnt,_T("BT_DLJL_LINE_LOOPNUM"),iLoopNum) ;
						if (iLoopNum==0)
						{
							iArrayHandleFindLine.Add(iHandleCurLine) ;
							ptArrayFrom.append(pt_3d_from) ;
							iRet++ ;
							g_cSystem.SetXdata(pEnt,_T("BT_DLJL_LINE_LOOPNUM"),1) ;
						}

					}					
				}

			}
			pEnt->close() ;

		}

	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet;
	
}

int CCableNetForBT::DelLoopNumXdata(void)
{
	int iRet = 0 ;	

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			if (pEnt->isKindOf(AcDbLine::desc()))				
			{	
				Adesk::UInt32 iHandle = g_cSystem.GetHandle(pEnt) ;
				int iNum = 0 ;
				if (g_cSystem.GetXdata(pEnt,_T("BT_DLJL_LINE_LOOPNUM"),iNum))
				{
					g_cSystem.DeleteXdata(pEnt,_T("BT_DLJL_LINE_LOOPNUM")) ;
					iRet++ ;
				}				
			}
			pEnt->close() ;

		}

	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet;
	
}

// �������Ի��������ʾ��ͼ�Ĺ������Ϣ���
int CCableNetForBT::ImportGdTable(const CString strXlsFileName)
{
	
	int iGdNum = 0 ;
	

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//���old����
	CDBUtility::ClearTable(_T("bt_mxb_import")) ;
	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("��ʼ��Excelʧ��,������û�а�װExcel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		CString strCheckCell[2] = {_T("C3"), _T("J3")} ;
		BOOL bCheckOk = TRUE ;
		_variant_t range_var ;
		for (int iCheckIndex=0; iCheckIndex<2; iCheckIndex++)
		{
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCheckCell[iCheckIndex]) ;
			range_var = pRange->GetText() ;
			pRange = NULL ;
			CString strVar ;
			strVar = (CString)range_var.bstrVal ;
			strVar.TrimLeft() ;
			strVar.TrimRight() ;
			if (strVar!=_T("�������"))
			{
				bCheckOk = FALSE ;
				break ;
			}
		}
		if (bCheckOk)
		{
			CString strColName[2][4] = {{_T("B"),_T("C"),_T("D"),_T("E")},	{_T("I"),_T("J"),_T("K"),_T("L")}} ;

			for (int i=0; i<2; i++)
			{
				CString strXhjName = _T(""), strGdName = _T(""), strBjLoc = _T(""), strDkLoc = _T("") ;
				long iBjLoc = 0 ;
				long iDkLoc = 0 ;
				int iUpOrDown = 0 ;

				iUpOrDown = i ;			
				
				int iCurRow = 4 ;
				BOOL bNext = TRUE ;
				while (bNext)
				{
					for (int iColIndex=0; iColIndex<4; iColIndex++)
					{
						CString strCellName = _T(""), strCellText = _T("") ;
						strCellName.Format(_T("%s%d"), strColName[i][iColIndex], iCurRow) ;						
						pRange = NULL ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						strCellText = (CString)(pRange->GetText().bstrVal) ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						acutPrintf(_T("\n%s-%s"), strCellName, strCellText) ;

						if (iColIndex==1)
						{
							if (strCellText==_T(""))
							{
								bNext = FALSE ;
								break ;
							}
						}

						switch (iColIndex)
						{
						case 0:
							{
								strXhjName = strCellText ;
							}						
							break;
						case 1:
							{
								strGdName = strCellText ;
							}
							break;
						case 2:
							{
								strBjLoc = strCellText ;
							}
							break;
						case 3:
							{
								strDkLoc= strCellText ;
							}						
							break;

						}
					}
					if (bNext)
					{

						strExec.Format(_T("insert into bt_mxb_import(bt_mxb_import_guidao, bt_mxb_import_xhj, bt_mxb_import_bj, bt_mxb_import_lx, bt_mxb_import_upordown)\
										  values('%s','%s','%s','%s', %d)"), strGdName, strXhjName, strBjLoc, strDkLoc,  iUpOrDown) ;
						ExecSqlString(strExec) ;					
					}
					iCurRow++ ;
				}

			}


		}

		pWorkBooks->Close() ;
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
		AfxMessageBox(_T("δ֪�Ĵ���!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}


	return 0 ;
}

// �����������滻����ַ�
int CCableNetForBT::ReplaceCoreString()
{
	int iNum = 0 ;

	CUserSystem mysys ;
	CStringArray strArrayFile ;
	CBlkUtility::FindFile(m_strSavePath, _T("dwg"), strArrayFile) ;
	int iFileNum = strArrayFile.GetSize() ;
	acedSetStatusBarProgressMeter(_T("\n�滻�ַ�"),0, iFileNum-1) ;

	CString strReplace[2][16] = {{_T("BOX_CCO"), _T("BOX_/CCO"), _T("BOX_CCI"), _T("BOX_/CCI"), _T("BOX_B50_1"), _T("BOX_B50_2"), _T("BOX_N50_1"), _T("BOX_N50_2"), _T("D_CCO"), _T("D_/CCO"), _T("D_CCI"), _T("D_/CCI"), _T("IND_BX110"), _T("IND_NX110"),  _T(" D_TEL"),_T(" D_TEL_R") },
	{_T("BOX CCO"), _T("BOX /CCO"), _T("BOX CCI"), _T("BOX /CCI"), _T("BOX(B50)"),  _T("BOX(B50)"),  _T("BOX(N50)"),  _T("BOX(N50)"),  _T("CCO"),   _T("/CCO"),   _T("CCI"),   _T("/CCI"),   _T("IND(BX110)"), _T("IND(NX110)") , _T("_TEL"),_T("_TEL_R") }} ;
	for (int i=0; i<iFileNum; i++)
	{
		acedSetStatusBarProgressMeterPos(i) ;
		CString strCurFilePath ;
		strCurFilePath = strArrayFile.GetAt(i) ;

		AcDbDatabase *pCurDb = new AcDbDatabase(false) ;

		if(Acad::eOk == pCurDb->readDwgFile(strCurFilePath))
		{
			mysys.m_pDb = pCurDb ;

			AcDbBlockTable * pBlkTbl = NULL ;
			pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);

			AcDbBlockTableRecord * pBlkTblRec = NULL ;
			pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
			pBlkTbl->close() ;
			AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

			pBlkTblRec->newIterator( pBlkTblRecIter );
			pBlkTblRec->close();

			AcDbEntity * pEnt = NULL ;
			BOOL bDoneZzjMainCable = FALSE ; //�Ƿ��Ѿ������ת�޻����ߵ���9��12��14о��������Ϊ������
			CString strLeftOrRight = _T("left") ;
			if (strCurFilePath.Find(_T("R_"))!=-1)
			{
				strLeftOrRight = _T("right") ;					
			}
			for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
			{
				//��������ʵ�� ;
				if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
				{
					if (pEnt->isKindOf(AcDbText::desc()))				
					{
						AcDbText * pText = AcDbText::cast(pEnt) ;

						CString strText = pText->textString() ;
						AcGePoint3d pt_3d_text(0,0,0) ;
						pt_3d_text = pText->alignmentPoint() ;

						//ת�޻������߸��ߵ���9о��12о��14оΪ�Ǳ���£������о������Ϊ��?��
						if (!bDoneZzjMainCable)
						{
							if (CBlkUtility::IsStrMatch(_T("^Terminal Box_(\\d+[AB]?)$"), strText) || CBlkUtility::IsStrMatch(_T("^Terminal Box_(\\d+[AB]?)_IND$"), strText) || CBlkUtility::IsStrMatch(_T("^Terminal Box_ML-(\\d+[AB]?)$"), strText) || CBlkUtility::IsStrMatch(_T("^Terminal Box_ML-(\\d+[AB]?)_IND$"), strText))
							{
								this->DealWithZzjMainCable(pCurDb, strLeftOrRight ) ;
								bDoneZzjMainCable = TRUE ;
							}
						}


						// ���ͽ����ն˺��ڵĶ���1��2�滻ΪD1��D2,11��12�滻Ϊ27��28							
						if (CBlkUtility::IsStrMatch(_T("^Terminal Box_.+_[RF]$"), strText))
						{
							this->ReplaceTie(pCurDb, pt_3d_text,0) ;
							pEnt->close() ;
							continue;
						}
						// ����绰������11��12�滻Ϊ23��24
						else if (CBlkUtility::IsStrMatch(_T("^Terminal Box_AX\\([DU]M[UD]\\)$"), strText))
						{
							this->ReplaceTie(pCurDb, pt_3d_text,1) ;
							pEnt->close() ;
							continue;
						}
						// ML-BOX �ն˺ж���δ֪������Ϊ��?��//modifed in 20140519,����Ҫ��ѣ���Ϊ����
// 						else if (CBlkUtility::IsStrMatch(_T("^ML-.+ BOX$"), strText))
// 						{								
// 							this->ReplaceTie(pCurDb, pt_3d_text,2) ;
// 							pEnt->close() ;
// 							continue;
// 						}
						// ����ת�޻������������ն˺���ת�޻��������������� о��ΪRWK/NWK����380VAC��
						else if (CBlkUtility::IsStrMatch(_T("^Terminal Box_(\\d+[AB]?)$"), strText))
						{
							CString strZzjType = _T("") ;								
							AcGePoint3d pt_3d_insert(pt_3d_text.x-20,pt_3d_text.y+5,0) ;
							if(this->FindZzjType(pCurDb, pt_3d_text, strZzjType))
							{
								if (strLeftOrRight==_T("right"))
								{										
									pt_3d_insert.x = pt_3d_text.x+20 ;
								}
								CString strZzjName = _T("") ;
								CBlkUtility::RegReplace(_T("^Terminal Box_(\\d+[AB]?)$"), _T("$1"), strText, strZzjName) ;
								CStringArray strArrayAtt ;
								CString strAtt = _T("") ;
								strAtt.Format(_T("Point Machine_%s"), strZzjName) ;
								strArrayAtt.Add(strAtt) ;
								if (strZzjType==_T("vac"))
								{
									for (int iCore=0; iCore<3; iCore++)
									{
										strAtt.Format(_T("%s_L%d/380VAC"),strZzjName, iCore+1) ;
										strArrayAtt.Add(strAtt) ;
									}
								}
								CString strBlockFile ;
								strBlockFile.Format(_T("bt_dljl_block_zzj_%s_%s"), strLeftOrRight, strZzjType) ;
								AcDbBlockReference *pRef = NULL ;
								if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
							}

							pEnt->close() ;
							continue;
						}
						// ����ת�޻������������ն˺���ת�޻��������������� ind
						else if (CBlkUtility::IsStrMatch(_T("^Terminal Box_(\\d+[AB]?)_IND$"), strText))
						{	
							AcGePoint3d pt_3d_insert(pt_3d_text.x-20,pt_3d_text.y+5,0) ;

							if (strLeftOrRight==_T("right"))
							{										
								pt_3d_insert.x = pt_3d_text.x+20 ;
							}
							CString strZzjName = _T("") ;
							CBlkUtility::RegReplace(_T("^Terminal Box_(\\d+[AB]?)_IND$"), _T("$1"), strText, strZzjName) ;
							CStringArray strArrayAtt ;
							CString strAtt = _T("") ;
							strAtt.Format(_T("Trap Machine_%s_IND"), strZzjName) ;
							strArrayAtt.Add(strAtt) ;

							CString strBlockFile ;
							strBlockFile.Format(_T("bt_dljl_block_zzj_%s_ind"), strLeftOrRight) ;
							AcDbBlockReference *pRef = NULL ;
							if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}						

							pEnt->close() ;
							continue;
						}
						// ����ת�޻������������ն˺���ת�޻��������������� ml
						else if (CBlkUtility::IsStrMatch(_T("^Terminal Box_ML-(\\d+[AB]?)$"), strText))
						{								
							AcGePoint3d pt_3d_insert(pt_3d_text.x-20,pt_3d_text.y+5,0) ;

							if (strLeftOrRight==_T("right"))
							{										
								pt_3d_insert.x = pt_3d_text.x+20 ;
							}
							CString strZzjName = _T("") ;
							CBlkUtility::RegReplace(_T("^Terminal Box_ML-(\\d+[AB]?)$"), _T("$1"), strText, strZzjName) ;
							CStringArray strArrayAtt ;
							CString strAtt = _T("") ;
							strAtt.Format(_T("Point Detector_ML-%s"), strZzjName) ;
							strArrayAtt.Add(strAtt) ;

							CString strBlockFile ;
							strBlockFile.Format(_T("bt_dljl_block_zzj_%s_ml"), strLeftOrRight) ;
							AcDbBlockReference *pRef = NULL ;
							if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}						

							pEnt->close() ;
							continue;
						}
						// ����ת�޻������������ն˺���ת�޻��������������� mlind
						else if (CBlkUtility::IsStrMatch(_T("^Terminal Box_ML-(\\d+[AB]?)_IND$"), strText))
						{								
							AcGePoint3d pt_3d_insert(pt_3d_text.x-20,pt_3d_text.y+5,0) ;

							if (strLeftOrRight==_T("right"))
							{										
								pt_3d_insert.x = pt_3d_text.x+20 ;
							}
							CString strZzjName = _T("") ;
							CBlkUtility::RegReplace(_T("^Terminal Box_ML-(\\d+[AB]?)_IND$"), _T("$1"), strText, strZzjName) ;
							CStringArray strArrayAtt ;
							CString strAtt = _T("") ;
							strAtt.Format(_T("Trap Indicator_ML-%s_IND"), strZzjName) ;
							strArrayAtt.Add(strAtt) ;

							CString strBlockFile ;
							strBlockFile.Format(_T("bt_dljl_block_zzj_%s_mlind"), strLeftOrRight) ;
							AcDbBlockReference *pRef = NULL ;
							if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}						

							pEnt->close() ;
							continue;
						}


						for(int i=0; i<16; i++)
						{
							if(strText.Replace(strReplace[0][i],strReplace[1][i])>0)
							{
								pText->setTextString(strText) ;
								pText->setColorIndex(3) ;
								iNum++ ;
								break ;
							}
						}

					}

					pEnt->close() ;

				}

			}

			delete pBlkTblRecIter ;
			pBlkTblRecIter = NULL ;

			mysys.ModifyFontStyle(_T("Standard"), _T("Times New Roman"), FALSE) ;

			pCurDb->saveAs(strCurFilePath) ;
		}
		delete pCurDb ;

	}
	acedRestoreStatusBar() ;

	
	return iNum;
}

// ���ư��������ƽ��ͼ
void CCableNetForBT::DrawBtMxbPmt(void)
{
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	m_pCurDb = pCurDb ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	AcDbObjectId objIdTextStyle ;

	objIdTextStyle = mysys.CreateTextStyle(_T("CHINESE"),_T("Times New Roman"), _T(""), 2, 0.7) ;
	pCurDb->setTextstyle(objIdTextStyle) ;

	double dBetweenUpOrDown = 12.000 ;
	double dBetweenGd = 18.000 ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		CString strOrder[2] = {_T("asc"),_T("desc")} ;

		for (int i=0; i<2; i++)
		{
			//
			strSql.Format(_T("SELECT * FROM bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), i, strOrder[i]) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if(cSet.Open(g_PtrCon, strSql))
			{
				int iGdSum = cSet.GetRecordCount() ;
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}

				int iPlus = i==0?1:-1 ;//������X��Ӧ��ͬ���������������ӣ����м��٣�
				int iCurGdIndex = 0 ;
				double dStartX = i==0?0:dBetweenGd*iGdSum ;
				double dGdY = iPlus*dBetweenUpOrDown/2 ;
				double dEndX = i==1?0:dBetweenGd*iGdSum ;

				//����2�����ֱ��
				AcGePoint3d pt_3d_start(dStartX, dGdY,0), pt_3d_end(dEndX, dGdY,0) ;
				mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.4) ;
				
				CString strBlockFile ;				

				BOOL bPassAss = FALSE ;
				while (!cSet.IsEOF())
				{
					double dCurStartX = dStartX+iPlus*iCurGdIndex*dBetweenGd ;  //��ǰ������ʼ��
					
					CString strGdName, strXhjName, strBjLoc, strDkLoc ;
					cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strGdName) ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjName) ;
					cSet.GetFieldValue(_T("bt_mxb_import_bj"), strBjLoc) ;
					cSet.GetFieldValue(_T("bt_mxb_import_lx"), strDkLoc) ;
					CString strJyjName = _T("bt_mxb_jxjyj") ;
					if (!bPassAss)
					{
						if (strXhjName.Left(3)==_T("ASS"))
						{
							bPassAss = TRUE ;
						}
					}
					else if (strXhjName.Left(3)!=_T("OHS"))
					{
						strJyjName = _T("bt_mxb_dqjyj") ;
					}
					AcDbBlockReference *pRef = NULL ;
					CStringArray strArrayAtt ;
					AcGePoint3d pt_3d_insert(dCurStartX, dGdY, 0) ;

					//��Ե��
					strBlockFile.Format(_T("%s\\support\\bt\\%s.dwg"), m_strSysBasePath, strJyjName) ;
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
					{
						pRef->close() ;
						pRef = NULL ;
					}
					//���һ���������ĩ�˾�Ե��
					if (iGdSum==(iCurGdIndex+1))
					{
						pt_3d_insert.x = dCurStartX+dBetweenGd*iPlus ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
						{
							pRef->close() ;
							pRef = NULL ;
						}	
					}

                    //�������
					pt_3d_insert.x = dCurStartX+iPlus*6.3 ;
					strBlockFile.Format(_T("bt_mxb_gd_%s_0"), i==0?_T("down"):_T("up") ) ;
					strArrayAtt.Add(strGdName) ;
					strArrayAtt.Add(_T("")) ;
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					//�źŻ�
					if (strXhjName!=_T(""))
					{
						pt_3d_insert.x = dCurStartX+(-1)*iPlus ;
						if (strXhjName.Left(3)==_T("ASS"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_ass_%s_0"), i==0?_T("down"):_T("up") ) ;
						}
						else if (strXhjName.Left(3)==_T("OHS"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_ohs_%s_0"), i==0?_T("down"):_T("up")) ;
						}
						else if (strXhjName.Left(2)==_T("AD"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a_%s_0"), i==0?_T("down"):_T("up")) ;
						}
						else if (strXhjName.Left(2)==_T("AD"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a_down_0")) ;
						}
						else if (strXhjName.Left(2)==_T("AU"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a_up_0")) ;
						}
						else if (strXhjName.Left(3)==_T("LXD"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a-lx_down_0")) ;
						}
						else if (strXhjName.Left(3)==_T("LXU"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a-lx_up_0")) ;
						}
						strArrayAtt.RemoveAll() ;
						strArrayAtt.Add(strXhjName) ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}
					}

					//����
					if (strBjLoc!=_T(""))
					{
						strArrayAtt.RemoveAll() ;
						CBlkUtility::DivideString(strBjLoc, _T(","), strArrayAtt) ;
						int iLxNum = strArrayAtt.GetCount() ;						
						strBlockFile.Format(_T("bt_mxb_bjq_%s_%d"), i==0?_T("down"):_T("up"), iLxNum) ;
						AcDbObjectId objIdBjq ;
						if (!mysys.HaveBlockName(strBlockFile, objIdBjq))
						{
							this->CreateBjqBlock(iLxNum, i) ;
						}
						pt_3d_insert.x = dCurStartX ;
						pt_3d_insert.y = i==0? dGdY+12:dGdY-12 ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}


					}

					//����
					if (strDkLoc!=_T(""))
					{
						pt_3d_insert.x = dCurStartX+14*iPlus ;
						pt_3d_insert.y = 0 ;
						strArrayAtt.RemoveAll() ;
						strArrayAtt.Add(strDkLoc) ;
						strBlockFile = _T("bt_mxb_lx_0") ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}

					}





					iCurGdIndex++ ;

					cSet.MoveNext() ;
				}


				cSet.Close() ;

			}


		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawBtMxbPmt")) ;
	} 
}

// ��������Ĳ��������������Ŀ�
BOOL CCableNetForBT::CreateBjqBlock(int iLxNum, int iUpOrDown)
{
	// ��õ�ǰͼ�����ݿ�Ŀ��
	AcDbBlockTable *pBlkTbl;
	m_pCurDb->getBlockTable(pBlkTbl, AcDb::kForWrite);
	// �����µĿ���¼
	AcDbBlockTableRecord *pBlkTblRec;
	pBlkTblRec = new AcDbBlockTableRecord();
	CString strBlkName ;
	strBlkName.Format(_T("bt_mxb_bjq_%s_%d"), iUpOrDown==0?_T("down"):_T("up"), iLxNum) ;
	pBlkTblRec->setName(strBlkName);
	// ������¼��ӵ������
	AcDbObjectId blkDefId;
	pBlkTbl->add(blkDefId, pBlkTblRec);
	pBlkTbl->close();

	// �����¼�����ʵ��
	AcDbObjectId entId;
	AcGePoint3d pt_3d_ent(-13, 0, 0) ;
	for (int i=0; i<iLxNum; i++)
	{
		if (iUpOrDown==0)
		{
			pt_3d_ent.x = -13 ;
			pt_3d_ent.y = (iLxNum-i-1)*2+0.25 ;
		}
		else
		{
			pt_3d_ent.x = 0.7 ;
			pt_3d_ent.y = 0-(i+1)*2+0.25-1-2*2.5 ;
		}
		CString strAtt ;
		strAtt.Format(_T("LX%d"),i+1) ;
		AcDbAttributeDefinition *pAttDef = new AcDbAttributeDefinition(pt_3d_ent, strAtt, strAtt, strAtt);
		pAttDef->setHeight(1.5) ;
		pAttDef->setWidthFactor(0.7) ;
		pBlkTblRec->appendAcDbEntity(entId, pAttDef);
		pAttDef->close();
	}
	CString strText[2] = {_T("ANNUNCIATOR"), _T("APPROACH")} ;
	for (int i2=0; i2<2; i2++)
	{
		if (iUpOrDown==0)
		{
			pt_3d_ent.x = -13 ;
			pt_3d_ent.y = 2*iLxNum+i2*2.5+0.45 ;
		}
		else
		{
			pt_3d_ent.x = 0.7 ;
			pt_3d_ent.y = 0-1-(i2+1)*2.5+0.45 ;
		}
		AcDbText *pText = new AcDbText(pt_3d_ent, strText[i2]) ;
		pText->setHeight(1.6) ;
		pText->setWidthFactor(0.7) ;
		pBlkTblRec->appendAcDbEntity(entId, pText);
		pText->close() ;
	}

	AcGePoint3d pt_3d_solid_0(0,2*iLxNum+2*2.5+0.5,0), pt_3d_solid_1(-3,2*iLxNum+2*2.5,0), pt_3d_solid_2(-3,2*iLxNum+2*2.5+1,0) ;
	if (iUpOrDown==1)
	{
		pt_3d_solid_1.x = pt_3d_solid_2.x = 3 ;
		pt_3d_solid_0.y = -0.5 ;
		pt_3d_solid_1.y = -1 ;
		pt_3d_solid_2.y = 0 ;
	}
    AcDbSolid *pSolid = new AcDbSolid(pt_3d_solid_0, pt_3d_solid_1, pt_3d_solid_2) ;
    
	AcGePoint3d pt_3d_start(-13, pt_3d_solid_0.y, 0), pt_3d_end(0, pt_3d_solid_0.y, 0);
	if (iUpOrDown==1)
	{
		pt_3d_start.x = 13 ;
		pt_3d_start.y = pt_3d_end.y = -0.5 ;
	}
	AcDbLine *pLine1 = new AcDbLine(pt_3d_start, pt_3d_end); // ����һ��ֱ��
	pt_3d_start.set(0, 0, 0);
	pt_3d_end.set(0, pt_3d_solid_2.y, 0);
	if (iUpOrDown==1)
	{
		pt_3d_end.set(0, 0-1-2*2.5-iLxNum*2, 0);
	}
	AcDbLine *pLine2 = new AcDbLine(pt_3d_start, pt_3d_end); // ����һ��ֱ��
	
	pBlkTblRec->appendAcDbEntity(entId, pSolid);
	pBlkTblRec->appendAcDbEntity(entId, pLine1);
	pBlkTblRec->appendAcDbEntity(entId, pLine2);

	// �ر�ʵ��Ϳ���¼
	pSolid->close();
	pLine1->close();
	pLine2->close();	
	pBlkTblRec->close();
	
	return 0;
}

//����ʵ�������zֵ���Ķ�Ϊ��0���˴���������Ϊ0
int CCableNetForBT::ChangeTo2D(void)
{
	int iRet = 0 ;

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			//����ͼ�� AcDbBlockReference ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
				AcGePoint3d pt_3d_base(0,0,0) ;
				pt_3d_base = pRef->position() ;
				pt_3d_base.z = 0 ;
				pRef->setPosition(pt_3d_base) ;
				iRet++ ;
			}
			else if (pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText * pText = AcDbText::cast(pEnt) ;
				AcGePoint3d pt_3d_base(0,0,0) ;
				pt_3d_base = pText->position() ;
				pt_3d_base.z = 0 ;
				pText->setPosition(pt_3d_base) ;
				iRet++ ;
			}
			else if (pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine * pLine = AcDbLine::cast(pEnt) ;
				AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
				pt_3d_start = pLine->startPoint() ;
				pt_3d_end = pLine->endPoint() ;
				pt_3d_start.z = pt_3d_end.z = 0 ;
				pLine->setStartPoint(pt_3d_start) ;
				pLine->setEndPoint(pt_3d_end) ;	
				iRet++ ;
			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet;
}

//1. ���ͽ����ն˺��ڵĶ���1��2�滻ΪD1��D2,11��12�滻Ϊ27��28
//2. ����绰������11��12�滻Ϊ23��24
BOOL CCableNetForBT::ReplaceTie(AcDbDatabase *& pCurDb, const AcGePoint3d & pt_3d_title, int iType)
{
	CString strReplace0[2][4] = {{_T("1"),_T("2"),_T("11"),_T("12")},{_T("D1"),_T("D2"),_T("27"),_T("28")}} ;
	CString strReplace1[2][2] = {{_T("11"),_T("12")},{_T("23"),_T("24")}} ;
	//CString strReplace2[2][2] = {{_T("1"),_T("")},{_T("23"),_T("24")}} ;
	double dXYOffset[3][4] = {{-20,20,-35,-5},{-20,20,-40,-5},{-20,20,-65,-5}} ;

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			if (pEnt->isKindOf(AcDbText::desc()))				
			{
				AcDbText * pText = AcDbText::cast(pEnt) ;

				AcGePoint3d pt_3d_text(0,0,0) ;
				pt_3d_text = pText->position() ;
				if (pt_3d_text.x>pt_3d_title.x+dXYOffset[iType][0] && pt_3d_text.x<pt_3d_title.x+dXYOffset[iType][1] && pt_3d_text.y>pt_3d_title.y+dXYOffset[iType][2] && pt_3d_text.y<pt_3d_title.y+dXYOffset[iType][3])
				{
					CString strText = pText->textString() ;

					switch (iType)
					{
					case 0:
						{
							for(int i=0; i<4; i++)
							{
								if (strText==strReplace0[0][i])
								{
									pText->setTextString(strReplace0[1][i]) ;
									pText->setColorIndex(3) ;
									break;
								}
							}
						}
						break;
					case 1:
						{
							for(int i=0; i<2; i++)
							{
								if (strText==strReplace1[0][i])
								{
									pText->setTextString(strReplace1[1][i]) ;
									pText->setColorIndex(3) ;
									break;
								}
							}
						}
						break;
					case 2:
						{
							if (CBlkUtility::IsStrMatch(_T("^\\d+$"), strText))
							{
								pText->setTextString(_T("?")) ;
								pText->setColorIndex(3) ;
								break;
							}						
						}
						break;
					}
				}

			}

			pEnt->close() ;

		}

	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return 0;
}

BOOL CCableNetForBT::FindZzjType(AcDbDatabase *& pCurDb, const AcGePoint3d & pt_3d_title, CString & strZzjType)
{
	BOOL bRet = FALSE ;

	double dXYOffset[4] = {-20,20,-20,-5} ;

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbText::desc()))				
			{
				AcDbText * pText = AcDbText::cast(pEnt) ;
				CString strText = pText->textString() ;

				AcGePoint3d pt_3d_text(0,0,0) ;
				pt_3d_text = pText->position() ;
				if (pt_3d_text.x>pt_3d_title.x+dXYOffset[0] && pt_3d_text.x<pt_3d_title.x+dXYOffset[1] && pt_3d_text.y>pt_3d_title.y+dXYOffset[2] && pt_3d_text.y<pt_3d_title.y+dXYOffset[3])
				{
					if(CBlkUtility::IsStrMatch(_T("^\\d+[AB]?_[RN]WK\\([BN]\\)$"), strText))
					{
						strZzjType = _T("wk") ;
						bRet = TRUE ;
						pEnt->close() ;
						break;
					}
					else if (CBlkUtility::IsStrMatch(_T("^\\d+[AB]?_.+380VAC$"), strText))
					{
						strZzjType = _T("vac") ;
						bRet = TRUE ;
						pEnt->close() ;
						break;
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

BOOL CCableNetForBT::DealWithZzjMainCable(AcDbDatabase *& pCurDb, const CString & strLeftOrRight)
{
	double dNull = 12345.12345 ;
	AcGePoint3d pt_3d_find1(0,0,0),pt_3d_find2(0,0,0) ;

	if (this->FindText(pCurDb, _T("Signalling Equipment Room(LPC)"), pt_3d_find1, dNull, dNull, dNull,dNull))
	{
		if (this->FindText(pCurDb, _T("^.+-(9|12|14)\\(\\d+\\).+$"), pt_3d_find2, pt_3d_find1.x-60, pt_3d_find1.x+60, -10000,10000, TRUE))
		{
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
				//��������ʵ�� ;
				if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
				{
					if (pEnt->isKindOf(AcDbText::desc()))				
					{
						AcDbText * pText = AcDbText::cast(pEnt) ;
						CString strText = pText->textString() ;

						AcGePoint3d pt_3d_text(0,0,0) ;
						pt_3d_text = pText->position() ;

						if ((strLeftOrRight==_T("left") && pt_3d_text.x>pt_3d_find1.x-20 && pt_3d_text.x<pt_3d_find1.x-10)|| strLeftOrRight==_T("right") && pt_3d_text.x>pt_3d_find1.x+10 && pt_3d_text.x<pt_3d_find1.x+20)
						{
							if (strText!=_T("SPARE"))
							{
								pText->setTextString(_T("?")) ;
							}

						}						
					}
					pEnt->close() ;
				}
			}

			delete pBlkTblRecIter ;
			pBlkTblRecIter = NULL ;

		}
	}
	return 0;
}

// �����ַ���
BOOL CCableNetForBT::FindText(AcDbDatabase *& pCurDb,const CString& strDestText, AcGePoint3d & pt_3d_find, const double dXmin, const double dXmax, const double dYmin, const double dYmax, BOOL bReg)
{
	BOOL bRet = FALSE ;

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
		//��������ʵ�� ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbText::desc()))				
			{
				AcDbText * pText = AcDbText::cast(pEnt) ;
				CString strText = pText->textString() ;

				AcGePoint3d pt_3d_text(0,0,0) ;
				pt_3d_text = pText->position() ;
				if ((bReg==FALSE && strText==strDestText) || (bReg==TRUE && CBlkUtility::IsStrMatch(strDestText, strText)) )
				{
					if (fabs(dXmin-12345.12345)>0.01)
					{
						if (pt_3d_text.x>dXmin && pt_3d_text.x<dXmax && pt_3d_text.y>dYmin && pt_3d_text.y<dYmax)
						{
							pt_3d_find = pText->alignmentPoint() ;
							pEnt->close() ;
							bRet = TRUE ;	
							break;
						}
					}
					else
					{
						pt_3d_find = pText->alignmentPoint() ;
						pEnt->close() ;
						bRet = TRUE ;	
						break;

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
