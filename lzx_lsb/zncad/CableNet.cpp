// CableNet.cpp: implementation of the CCableNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"
#include "CableNet.h"
#include "ZnUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCableNet::CCableNet()
: m_iXhjDcSet(0)
, m_bShareWithDevName(FALSE)
, m_bDefaultCablecore(FALSE)
, m_bDrawBackup(TRUE)
, m_iDsNumXhj(0)
, m_iQkNum(0)
, m_iDcDsNum(0)
{
	m_pCurDb = NULL ;	
	m_iDrawMode = 0 ;	
	m_dCellHight = 5 ;
	m_dCellWidth = 40 ;
	m_iReadCablenetPos = 0 ;
	m_iReadCablenetCount = 100 ;
	m_AdoDb.Open(g_strAdoCon) ;
	m_RecordSet.SetConnection(&m_AdoDb) ;	
	CBlkUtility::GetBasePath(m_strSysBasePath) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("XhjDsNum"), m_iDsNumXhj) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("DcQkNum"), m_iQkNum) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("DcDsNum"), m_iDcDsNum) ;

}

CCableNet::~CCableNet()
{
	m_RecordSet.Close() ;
	m_AdoDb.Close() ;
}

BOOL CCableNet::ReadEntToDB(AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	mysys.ZoomView() ;

	CString strSql ;
	
	m_AdoDb.ClearTable(_T("cablenet_ent")) ;

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
				Adesk::UInt32 iHandle_Low = 0 ;
				iHandle_Low = 	mysys.GetHandle(pEnt) ;

				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;	
				
				//�õ�����
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;

				CStringArray strArrayBlkRet ;
				CBlkUtility::DivideString(strBlockName, _T("_"), strArrayBlkRet) ;

				CString strBoxType ;
				CString strDevType ;
				CString strBlockNameDev ;
				CString strDevName ;
				CString strDevFor = _T("JY1") ;
				int iHasDs = 0 ;
				int iIsDianMaHua = 0 ;
				CString strFreq ;

				//ת�޻�ר�ã�������ֵ
				//strZzjIndex��ת�޻��ڱ����������λ�ã���1/2������ʾ�ú��Ӵ���2���еĵ�һ����
				//	ע�⣬���ڽ�����ֱͨ��¥�ģ�����ZYJ7+SH6˫����ϣ�ÿһ��Ӧ����д��1/1��
				CString strZzjType ; 
				CString strZzjIndex ; 

				AcGePoint3d pt_3d_block(0,0,0) ;
				pt_3d_block = pRef->position() ;

				AcGePoint3d pt_3d_search_topleft(0,0,0) ;
				AcGePoint3d pt_3d_search_bottomright(0,0,0) ;

				CString strBlkType = strArrayBlkRet.GetAt(0) ;
				if (strBlkType==_T("BOX")) 
				{
					CString strBlkNamePart1 = strArrayBlkRet.GetAt(1) ;
					CString strBlkNamePart2 = strArrayBlkRet.GetAt(2) ;
					mysys.GetAttValue(pRef, strBlkNamePart1, strBoxType) ;
					if (strBlkNamePart1==_T("XB"))
					{
						if (strBlkNamePart2.Left(2)==_T("CG"))
						{
							mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
						}
						else
						{
							pt_3d_search_topleft.x = pt_3d_block.x-6 ;
							pt_3d_search_topleft.y = pt_3d_block.y+7 ;
							pt_3d_search_bottomright.x = pt_3d_block.x+6 ;
							pt_3d_search_bottomright.y = pt_3d_block.y-7 ;
							//�ҳ����Ӷ�Ӧ���豸��Ŀǰֻ���źŻ���20100120��
							this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strBlockNameDev, strDevName, iHasDs, 0) ;
						}
					}
					else if (strBlkNamePart1==_T("HF"))
					{
						CString strHFType = strArrayBlkRet.GetAt(2) ;
						//HFTYPE=0:ʹ��BOX_HF_0�飬HFTYPE=1��ʹ��BOX_HF_1��
						if (strHFType==_T("0"))
						{
							pt_3d_search_topleft.x = pt_3d_block.x-9 ;
							pt_3d_search_topleft.y = pt_3d_block.y+9 ;
							pt_3d_search_bottomright.x = pt_3d_block.x+9 ;
							pt_3d_search_bottomright.y = pt_3d_block.y-9 ;

							this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strBlockNameDev, strDevName, iHasDs, 1) ;						
						}
						else if (strHFType==_T("1"))
						{
							mysys.GetAttValue(pRef, _T("HFNUM"), strDevName) ;
						}
					}
					else if (strBlkNamePart1==_T("HZ"))
					{
						if (strBlkNamePart2!=_T("0"))
						{
							mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
							if (strBlkNamePart2==_T("ZZJ"))
							{
								mysys.GetAttValue(pRef, _T("INDEX"), strZzjIndex) ;
								mysys.GetAttValue(pRef, _T("TYPE"), strZzjType) ;
							}
						}
						else 
						{
							pt_3d_search_topleft.x = pt_3d_block.x-6 ;
							pt_3d_search_topleft.y = pt_3d_block.y+7 ;
							pt_3d_search_bottomright.x = pt_3d_block.x+6 ;
							pt_3d_search_bottomright.y = pt_3d_block.y-7 ;
							//�ҳ����Ӷ�Ӧ���豸
							this->FindDevForBox(pt_3d_search_topleft, pt_3d_search_bottomright, strBlockNameDev, strDevName, iHasDs, 0) ;
						}
					}	
					else if (strBlkNamePart1==_T("FSD"))//���ܵ�
					{
						mysys.GetAttValue(pRef, _T("GDNAME"), strDevName) ;
						CString strBlkNamePart3 = strArrayBlkRet.GetAt(3) ;
						if (strBlkNamePart3==_T("1")) //���뻯
						{
							mysys.GetAttValue(pRef, _T("FREQ"), strFreq) ;
							if (strFreq.IsEmpty())
							{
								acutPrintf(_T("\n**************************��ע�⣬�ڶ�ȡ���뻯����ʱ��δ����%s��Ƶ��!"), strDevName) ;
							}
						}
						if (strBlockName.Right(4)==_T("S_00")) //�շ�
						{
							mysys.GetAttValue(pRef, _T("HZ"), strBoxType) ;
						}
						//strBoxType = _T("XB1") ;//20150130:���յ��ˣ���Ҫ���ݿ����жϺ������ͣ����뻯���Ӱ��о��
					}
					else if (strBlkNamePart1==_T("LD")) //�շ��״�
					{
						mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
						strBoxType = _T("LD") ;
						strDevType.Format(_T("TF_LD_%s"), strArrayBlkRet.GetAt(2)) ;
					}

				}//end if box
				else if (strBlkType==_T("ZZJ"))
				{					
					if (strArrayBlkRet.GetAt(2)==_T("JH"))
					{
						strDevFor = _T("JH") ;
						CString strTmp ;
						mysys.GetAttValue(pRef, _T("DCNUM-DEV"), strTmp) ;
						strDevType = _T("") ;
						int iLocFind = strTmp.Find(_T('-')) ;
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
						mysys.GetAttValue(pRef, _T("ZZJ_TYPE"), strDevFor) ;
						mysys.GetAttValue(pRef, _T("DC_NUM"), strDevName) ;

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
					}
				}
				else if (strBlkType==_T("YDQ"))
				{
					CString strYDQTypeTmp = strArrayBlkRet.GetAt(1) ; //YD-HZ
					int iLocFind = strYDQTypeTmp.Find(_T("-")) ;
					CString strYDQTypeTmpPre ;
					if (iLocFind!=-1)
					{
						strYDQTypeTmpPre = strYDQTypeTmp.Left(iLocFind) ; //YD
						strBoxType = strYDQTypeTmp.Mid(iLocFind+1) ; //HZ
					}
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
					if (strBlockNameDev.Left(3)==_T("XHJ"))
					{
						if (m_iDsNumXhj==1)
						{
							strDevFor = _T("JY3") ;
						}
						if (strBlockNameDev.Mid(4,2)==_T("DC")) //����
						{
							if (m_iDcDsNum==1)
							{
								strDevFor = _T("JY2") ;
							}
							else if (m_iDcDsNum==2)
							{
								strDevFor = _T("JY3") ;
							}
							else
							{
								strDevFor = _T("JY1") ;
							}
						}
					}
					if (strBlockName.Left(10)==_T("BOX_HZ_ZZJ"))
					{
						if (m_iQkNum==1)
						{
							strDevFor = _T("JY4") ;
						}
					}
				strSql.Format(_T("insert into cablenet_ent(handle_low, devname, devtype, dev_block, dev_for, has_ds, boxtype, box_block, pt_x,pt_y, is_dianmahua, zzj_type, zzj_index, freq) \
					values(%d,'%s','%s','%s','%s', %d, '%s', '%s', %f,%f, %d, '%s', '%s', '%s')"),\
					iHandle_Low,  strDevName, strDevType, strBlockNameDev, strDevFor, iHasDs, strBoxType, strBlockName, pt_3d_block.x, pt_3d_block.y, iIsDianMaHua, strZzjType, strZzjIndex, strFreq) ;
				m_AdoDb.Execute(strSql) ;
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

	return bRet ;
}


BOOL CCableNet::FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strBlockName, CString &strDevName, int& iHasDs, int iFindType)
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
				AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pRef->position() ;
				//ֻ����涨����
				if((textp_3d.x>pt_3d_topleft.x && textp_3d.x<pt_3d_bottomright.x) && (textp_3d.y>pt_3d_bottomright.y && textp_3d.y<pt_3d_topleft.y))
				{
					CString strBlockNameTmp ;
					mysys.GetBlockName(pRef, strBlockNameTmp) ;					
					strBlockNameTmp.TrimLeft();
					strBlockNameTmp.TrimRight() ;
					if (!strBlockNameTmp.IsEmpty())
					{
						CStringArray strArrayRet ;
						CBlkUtility::DivideString(strBlockNameTmp, _T("_"), strArrayRet) ;
						switch (iFindType)
						{
						case 0:
							{
								if (strBlockNameTmp==_T("AZ"))
								{
									mysys.GetAttValue(pRef, _T("NAME"), strDevName) ;
									strBlockName = strBlockNameTmp ;
									bRet = TRUE ;	
								}
								if (strBlockNameTmp.Left(3)==_T("XHJ"))
								{
									mysys.GetAttValue(pRef, _T("XHJNAME"), strDevName) ;
									int iNumDs = 0 ;
									if (mysys.GetXdata(pRef, _T("DSNUM"), iNumDs))
									{
										iHasDs = iNumDs ;
									}
									strBlockName = strBlockNameTmp ;
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

BOOL CCableNet::ReadConCableToDB(AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;
	CString strError(_T("Error in ReadConCableToDB:")) ;

	CString strSql ;

	m_AdoDb.ClearTable(_T("connect_cable")) ;

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
				long iLeftEntHandle_Low = 0 ;
				long iRightEntHandle_Low = 0 ;
				
				CString strCableStr ;
				AcGePoint3d pt_3d_text(0,0,0) ;

				AcDbPolyline * pPolyLine = (AcDbPolyline *)pEnt ;	
				int iVertNum = pPolyLine->numVerts() ;
				AcGePoint3d pt_3d_ptStartEnd[2] ;
				//pt= {(0,0,0),(0,0,0)} ;
				pPolyLine->getPointAt(0,pt_3d_ptStartEnd[0]) ;
				pPolyLine->getPointAt(iVertNum-1, pt_3d_ptStartEnd[1]) ;
				//acutPrintf(_T("\n�߶���ʼ�� %.2f-%.2f %.2f-%.2f"), pt_3d_ptStartEnd[0].x,pt_3d_ptStartEnd[0].y,pt_3d_ptStartEnd[1].x,pt_3d_ptStartEnd[1].y ) ;
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

				BOOL bFindLeft = FALSE ;
				bFindLeft = this->FindBoxBetweenCable(pt_3d_ptStartEnd[0], iLeftEntHandle_Low) ;
				if (!bFindLeft)
				{
					strError.Format(_T("%s\nLeft near (%.3f-%.3f) no Box found"), strError, pt_3d_ptStartEnd[0].x, pt_3d_ptStartEnd[0].y );
					acutPrintf(strError) ;
					Acad::ErrorStatus es = pEnt->upgradeOpen() ;
					if(Acad::eOk == es)
					{
						pEnt->setColorIndex(3) ;
						pEnt->downgradeOpen() ;
					}					
					pEnt->close() ;
					continue ;					
				}
				//end polyline��߶˵��ܱ�Ҫ��������
				
				//begin polyline�ұ߶˵��ܱ�Ҫ��������
				BOOL bFindRight = FALSE ;
				bFindRight = this->FindBoxBetweenCable(pt_3d_ptStartEnd[1], iRightEntHandle_Low) ;
				if (!bFindRight)
				{
					strError.Format(_T("%s\nRight near (%.3f-%.3f) no Box found"), strError, pt_3d_ptStartEnd[1].x, pt_3d_ptStartEnd[1].y );
					acutPrintf(strError) ;
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
				if (strCableStr.IsEmpty())
				{
					Acad::ErrorStatus es = pEnt->upgradeOpen() ;
					if(Acad::eOk == es)
					{
						pEnt->setColorIndex(1) ;
						pEnt->downgradeOpen() ;
					}
					strError.Format(_T("%s\nPolyline (%.3f-%.3f)(%.3f-%.3f) no cablestring found,It's color has been set red"), strError, pt_3d_ptStartEnd[0].x, pt_3d_ptStartEnd[0].y, pt_3d_ptStartEnd[1].x, pt_3d_ptStartEnd[1].y);
					acutPrintf(strError) ;					
				}
				
				try
				{
					strSql.Format(_T("insert into connect_cable(cablestring,pt_x,pt_y,left_ent_handle,right_ent_handle) \
									 values('%s',%f,%f,%d,%d)"),\
									 strCableStr,pt_3d_text.x,pt_3d_text.y,iLeftEntHandle_Low, iRightEntHandle_Low) ;
					m_AdoDb.Execute(strSql) ;
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

	return bRet ;
}

BOOL CCableNet::FindBoxBetweenCable(AcGePoint3d pt_3d_base, long &nHandle)
{
	BOOL bRet = FALSE ;

// 							//20110512:XHJ��BOX������ȥ���˴�strBlockType==_T("XHJ"),�����ź�¥����strBlockType==_T("DZ")
// 							//if (strBlockType==_T("BOX")||strBlockType==_T("XHJ")||strBlockType==_T("ZZJ")||strBlockType==_T("FJD")||strBlockType==_T("YDQ"))
// 							if (strBlockType==_T("BOX")||strBlockType==_T("ZZJ")||strBlockType==_T("FJD")||strBlockType==_T("YDQ")||strBlockType==_T("DZ"))
// 							{
// 								pEnt->getAcDbHandle(entHandle) ; 
// 								bRet = TRUE ;						
// 							}

	CUserSystem mysys ;
	mysys.m_pDb = m_pCurDb ;

	mysys.ZoomView(pt_3d_base, 50, 50) ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0, _T("INSERT"),0) ;
	long iLen = 0 ;
	CLongArray iArrayHandle ;
	double dOffset = 0.5 ;
	ads_point pt_min = {pt_3d_base.x-dOffset, pt_3d_base.y-dOffset, 0}, pt_max = {pt_3d_base.x+dOffset, pt_3d_base.y+dOffset, 0};
	iLen = mysys.GetSSEntHandle(_T("C"), pt_min, pt_max, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;
	if (iLen>0)
	{
		nHandle = iArrayHandle.GetAt(0) ;
		bRet = TRUE ;
		if (iLen>1)
		{
			CString strHandles ;
			CBlkUtility::ConvertArrayToString(iArrayHandle, _T(","), strHandles) ;
			acutPrintf(_T("\nע�⣺�ڶ���߶˵�%.3f,%.3f�����ֲ�ֹһ����, %s"), pt_3d_base.x, pt_3d_base.y, strHandles) ;
		}
	}

	return bRet ;
}

BOOL CCableNet::FindCableStrInLine(const AcGeLineSeg3d lin, CString &strCableStr, AcGePoint3d &pt_3d_text)
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
					if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*-([4-9]|[1-6]\\d)[A-Z]*\\(([1-9]|[1-2]\\d)\\)$"),strText))
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

void CCableNet::DoAll()
{
}

//�ݹ��һ����·
void CCableNet::FindOneRoute(long iCurEntHandle, CLongArray &iArrayEntHandle)
{
	iArrayEntHandle.Add(iCurEntHandle) ;

	try
	{
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		
		//��ʾ����ʼ(û��ǰһ�����ӵ�)��ent
		strSql.Format(_T("SELECT * FROM cablenet_ent WHERE handle_low=%d and next_ent_handle_low>0"), iCurEntHandle) ;

		if(cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
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

//���������������
BOOL CCableNet::FillDB()
{
	BOOL bRet = TRUE ;
	CString strErrMsg ;
	try	
	{				
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;

		//begin ����cablenet_ent���left_ent_handle��right_ent_handle
		strSql.Format(_T("select * from cablenet_ent")) ;
		if(cSet.Open(strSql))
		{
			while (!cSet.IsEOF())
			{
				long iHandle = 0 ;
				cSet.GetFieldValue(_T("handle_low"), iHandle) ;

				CString strColName[] = {_T("right_ent_handle"), _T("left_ent_handle")} ;
				for (int i=0; i<2; i++)
				{
					strSql.Format(_T("select * from connect_cable where %s=%d"), strColName[i], iHandle) ;
					CADORecordset cSetTmp (&m_AdoDb) ;
					if(cSetTmp.Open(strSql))
					{
						if (cSetTmp.GetRecordCount()>0)
						{
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
		//end ����cablenet_ent���left_ent_handle��right_ent_handle

		//begin ����cablenet_ent���next_ent_handle_low, is_start, left_or_right
		CString strColName[] = {_T("left_ent_handle"), _T("right_ent_handle")} ;
		for (int i=0; i<2; i++)
		{
			strSql.Format(_T("select * from cablenet_ent where %s=0 and boxtype<>'XHL'"), strColName[i]) ;
			if(cSet.Open(strSql))
			{
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
		//end ����cablenet_ent���next_ent_handle_low
		
		acedSetStatusBarProgressMeterPos(40) ;

		//begin ����cablenet_ent���branch
		strSql.Format(_T("select * from cablenet_ent")) ;
		if(cSet.Open(strSql))
		{
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

		acedSetStatusBarProgressMeterPos(50) ;

		//begin ����cablenet_ent���belong_xhl_handle_low
		strSql.Format(_T("select * from cablenet_ent where is_start=1")) ;
		if(cSet.Open(strSql))
		{
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
					m_AdoDb.Execute(strSql) ;	
					if (i==(iEntSum-1))
					{
						int iLeftOrRight = 0 ;
						cSet.GetFieldValue(_T("left_or_right"), iLeftOrRight) ;
						strSql.Format(_T("update cablenet_ent set left_or_right=%d where handle_low=%d"), iLeftOrRight, iHandleTmp) ;
						m_AdoDb.Execute(strSql) ;
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
		}
		//end ����cablenet_ent���belong_xhl_handle_low

		//begin ĳЩ�ӷ����źŻ���2��XB������
		strSql.Format(_T("select * from cablenet_ent where left(dev_block, 6) in ('XHJ_LJ', 'XHJ_LF') ")) ;
		if(cSet.Open(strSql))
		{
			while (!cSet.IsEOF())
			{
				long iNextEntHandle = 0 ;
				CString strDevName ;
				CString strDevType ;
				CString strBoxType ;
				CString strDevFor ;
				cSet.GetFieldValue(_T("next_ent_handle_low"), iNextEntHandle) ;
				cSet.GetFieldValue(_T("devname"), strDevName) ;
				cSet.GetFieldValue(_T("devtype"), strDevType) ;
				cSet.GetFieldValue(_T("boxtype"), strBoxType) ;
				cSet.GetFieldValue(_T("dev_for"), strDevFor) ;

				//begin ������һ�����ӵ��豸���ͺ�����
				strSql.Format(_T("select * from cablenet_ent where handle_low=%d and left(boxtype, 2)='XB' and dev_block='' "), iNextEntHandle) ;
				CADORecordset cSetNextEnt (&m_AdoDb) ;				
				if (cSetNextEnt.Open(strSql))
				{
					if (cSetNextEnt.GetRecordCount()==1)
					{
						CString strNextBoxType ;
						cSetNextEnt.GetFieldValue(_T("boxtype"), strNextBoxType) ;

						CString strNextDevName ;
						strNextDevName.Format(_T("%s-A"), strDevName) ;
						strDevName.Append(_T("-B")) ;
						
						CString strNextDevType  = strDevType ;
						strNextDevType.Insert(3, _T('A')) ;
						strNextDevType.AppendFormat(_T("_%s-%s"), strBoxType, strNextBoxType) ;
						strDevType.Insert(3, _T('B')) ;
						strDevType.AppendFormat(_T("_%s-%s"), strBoxType, strNextBoxType) ;                    

						cSetNextEnt.Edit() ;
						cSetNextEnt.SetFieldValue(_T("devname"), strNextDevName) ;
						cSetNextEnt.SetFieldValue(_T("devtype"), strNextDevType) ;
						cSetNextEnt.SetFieldValue(_T("dev_for"), strDevFor) ;
						cSetNextEnt.Update() ;

						cSet.Edit() ;
						cSet.SetFieldValue(_T("devname"), strDevName) ;
						cSet.SetFieldValue(_T("devtype"), strDevType) ;
						cSet.Update() ;
					}
					cSetNextEnt.Close() ;
				}
				//end ������һ�����ӵ��豸���ͺ�����

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end ĳЩ�ӷ����źŻ���2��XB������

		//begin �Ե���ת�޻����¹����ܽᣬ�޸�ͼ�顢���Ͷ��塢о�߶��塢��о���� 20150324
		strSql.Format(_T("select * from cablenet_ent where is_start=1 and box_block='BOX_HZ_ZZJ_0' ")) ; //ֻ����ת�޻��������豸��ͬ�µ����
		CLongArray nArrayHandleStartZzj ;
		m_RecordSet.GetRecordValue(strSql, _T("handle_low"), nArrayHandleStartZzj) ;
		for (int i=0; i<nArrayHandleStartZzj.GetCount(); i++)
		{
			long nHandle = nArrayHandleStartZzj.GetAt(i) ;
			CLongArray nArrayHandleZzj ;
			CStringArray strArrayZzjType, strArrayZzjIndex ;
			CString strPreDcName ;
			LoopZzj(nHandle, strPreDcName, nArrayHandleZzj, strArrayZzjType, strArrayZzjIndex) ;
		}
		//end �Ե���ת�޻����¹����ܽᣬ�޸�ͼ�顢���Ͷ��塢о�߶��塢��о���� 20150324


		//begin ��ZD6 ZY4����ת�޻�ȷ����ʹ��о��
		//modify:20111026,����ZYJ6-7
		//modify:20121212:ZY4+ZD6˫����ϣ�л��������д�˲��ִ���
		/*
		strSql.Format(_T("select * from cablenet_ent where is_start=1")) ;
		if(cSet.Open(strSql))
		{
			while (!cSet.IsEOF())
			{
				long iStartHandle = 0;
				CString strDevFor ;
				cSet.GetFieldValue(_T("dev_for"), strDevFor) ;
				cSet.GetFieldValue(_T("handle_low"), iStartHandle) ;

				if (strDevFor==_T("SH5")||strDevFor==_T("SH6")) //��һ���ڵ���������������������һ���ڵ㿪ʼ���о
				{
					cSet.GetFieldValue(_T("next_ent_handle_low"), iStartHandle) ;
					//�������SH5/6�������㲻�����о���㣬������ΪZZJ_0_0_5
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
				//��·���һ�����ź�¥��ȥ��
				for (int iHandleIndex=0; iHandleIndex<iArrayRouteHandle.GetSize()-1; iHandleIndex++)
				{					
					CString strHandleTmp ;
					strHandleTmp.Format(_T("%d,"), iArrayRouteHandle.GetAt(iHandleIndex)) ;
					strHandleStr+=strHandleTmp ;					
				}
				strHandleStr.TrimRight(_T(",")) ;
				
				strSql.Format(_T("select * from cablenet_ent where handle_low in(%s) "), strHandleStr) ;
				CADORecordset cSetTmp(&m_AdoDb) ;
				long iDistanceToXhl = 0 ;
				if(cSetTmp.Open(strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						//����
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
								strErrMsg.Format(_T("ע�⣺%sȱ�ٵ�����Ϣ��"), strDevNameTmp) ;
								AfxMessageBox(strErrMsg) ;
							}

							cSetTmp.MoveNext() ;
						}

						//Ϊ��iStartHandleΪ��ͷ��һ����·�ϵ�����ת�޻��ڵ�ȷ������
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
								m_RecordSet.GetRecordValue(strSql, _T("core_num"), iCoreNum) ;

								CString strCoreNumTmp ;
								strCoreNumTmp.Format(_T("_%d"), iCoreNum) ;
								strDevType.Append(strCoreNumTmp) ;

								//������ڵ���ZY4/SH5�������ƶ�һλ������һ�ڵ��Ƿ���SH5/ZY4,����ýڵ����Ͷ�2оB5/B6
								int nId = 0 ;
								cSetTmp.GetFieldValue(_T("id"), nId) ;
								if (strDevFor2==_T("SH5"))
								{
									strSql.Format(_T("select * from cablenet_ent where handle_low=(select next_ent_handle_low from cablenet_ent where id =%d)"), nId) ;
									CString strDevForNext ;
									m_RecordSet.GetRecordValue(strSql, _T("dev_for"), strDevForNext) ;
									if (strDevForNext == _T("ZY4"))
									{
										strDevType.Append(_T("_1")) ;												
									}
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
		*/
		//end ��ZD6 ZY4����ת�޻�ȷ����ʹ��о��

		strSql.Format(_T("SELECT * FROM cablenet_ent where boxtype='XHL'")) ;
		if(cSet.Open(strSql))
		{
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
			cSet.Close() ;
		}

		//begin ����շ�JSQ,������devtype
		strSql.Format(_T("SELECT * FROM cablenet_ent where box_block='BOX_HZ_JSQ_0' and is_start=1 ")) ;
		CLongArray nArrayHandleJsqStart ;
		m_RecordSet.GetRecordValue(strSql, _T("handle_low"), nArrayHandleJsqStart) ;
		CString strJsqNamePlus[4] = {_T("Z1"), _T("B1"),_T("Z2"),_T("B2")} ;
		CString strJsqNamePlus2[2] = {_T("Z0"), _T("B0")} ;
		for (int iJsqStartIndex=0; iJsqStartIndex<nArrayHandleJsqStart.GetCount(); iJsqStartIndex++)
		{
			CLongArray nArrayHanldeLoop ;
			this->LoopConnectBox(nArrayHandleJsqStart.GetAt(iJsqStartIndex), nArrayHanldeLoop, _T("BOX_HZ_JSQ_0")) ;
			int iCount = nArrayHanldeLoop.GetCount() ;
			
			CString strCableString ; //������ʼ���ӵ���о��Ϊ12(4)�������·ֻ����2����������ΪZ0/B0
			strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nArrayHandleJsqStart.GetAt(iJsqStartIndex)) ;
			m_RecordSet.GetRecordValue(strSql, _T("cablestring"), strCableString) ;
			if (strCableString.Right(5)==_T("12(4)") && iCount==2)
			{
				for (int ii=0; ii<iCount; ii++)
				{
					long nHandleCur = nArrayHanldeLoop.GetAt(ii) ;
					CString strDevType ;
					strDevType.Format(_T("TF_JSQ_%s"), strJsqNamePlus2[iCount-ii-1]) ;
					strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nHandleCur) ;
					m_RecordSet.SetRecordValue(strSql, _T("devtype"), strDevType) ;			
				}
			}
			else
			{
				for (int ii=0; ii<iCount; ii++)
				{
					long nHandleCur = nArrayHanldeLoop.GetAt(ii) ;
					CString strDevType ;
					strDevType.Format(_T("TF_JSQ_%s"), strJsqNamePlus[iCount-ii-1]) ;
					strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nHandleCur) ;
					m_RecordSet.SetRecordValue(strSql, _T("devtype"), strDevType) ;

                    if (ii%2==0) //���B2����B1��devname
					{
						CString strDevNameJSQ ;
						strSql.Format(_T("select devname from cablenet_ent where handle_low=(select next_ent_handle_low from cablenet_ent where handle_low=%d)"), nHandleCur) ;
						m_RecordSet.GetRecordValue(strSql, _T("devname"), strDevNameJSQ) ;
						strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nHandleCur) ;
						m_RecordSet.SetRecordValue(strSql, _T("devname"), strDevNameJSQ) ;
					}				
				}
			}
		}
		//end ����շ�JSQ,������devtype
		
		//begin TF_SD��о����³��Ⱦ�����о��
		strSql.Format(_T("SELECT * FROM cablenet_ent where devtype='TF_SD' ")) ;
		if(cSet.Open(strSql))
		{
			while (!cSet.IsEOF())
			{
				long nHandleCurSD = 0 ;
				cSet.GetFieldValue(_T("handle_low"), nHandleCurSD) ;
				CLongArray nArrayHandleConBox ;
				this->LoopConnectBox(nHandleCurSD, nArrayHandleConBox, _T("")) ;
				if (nArrayHandleConBox.GetCount()>0)
				{
					nArrayHandleConBox.RemoveAt(nArrayHandleConBox.GetCount()-1) ;
				}
				CString strRet ;
				CBlkUtility::ConvertArrayToString(nArrayHandleConBox, _T(","), strRet ) ;
				strSql.Format(_T("select sum( left(cablestring,instr(cablestring, '-')-1)) as sumlen from cablenet_ent where handle_low in(%s)"), strRet) ;
				int iLen = 0 ;
				double dLen = 0 ;
				m_RecordSet.GetRecordValue(strSql, _T("sumlen"), dLen) ;
				iLen = (int)dLen ;
				if (iLen>200)
				{
					strSql.Format(_T("select top 1 * from define_zzj_plus where cable_len>%d and dev_for='TF_SD' order by cable_len asc"), iLen) ;
					int iNumQ = 0, iNumH = 0 ;
					m_RecordSet.GetRecordValue(strSql, _T("act_num"), iNumQ) ;
					m_RecordSet.GetRecordValue(strSql, _T("act_index"), iNumH) ;
					if (iNumH>0&&iNumQ>0)
					{
						CString strDevType ;
						strDevType.Format(_T("TF_SD_%d%d"), iNumQ, iNumH) ;
						cSet.Edit() ;
						cSet.SetFieldValue(_T("devtype"), strDevType) ;
						cSet.Update() ;
					}
				}
                
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end TF_SD��о����³��Ⱦ�����о��

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

//��������,һ����¥��һ��ͼ
BOOL CCableNet::ComputeCoreAndDraw()
{
	BOOL bRet = TRUE ;
	CString strErrMsg ;

	try
	{				
		CADORecordset cSet(&m_AdoDb) ;
		CString strSql ;

		//begin һ���ź�¥ent����һ��ͼֽ��ѭ����������ͼֽ
		//////////////////////////////////////////////////////////////////////////

		strSql.Format(_T("SELECT * FROM cablenet_ent where boxtype='XHL'")) ;
		if (cSet.Open(strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;

			CString strBarMsg ;
			strBarMsg.Format(_T("��������:%d��"), iRsCount) ;
			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;

			int iIndex = 0 ;
			double dCurTotalYForOneDwg = 0 ; //���Ƶ�һ��ͼֽʱ����¼ÿ�������ʱ��Y����		

			while (!cSet.IsEOF())
			{
				//����ձ�
				m_AdoDb.ClearTable(_T("cable_core_draw")) ;

				acedSetStatusBarProgressMeterPos(iIndex) ;			

				long iXHL_handle_low = 0 ;			
				cSet.GetFieldValue(_T("handle_low"), iXHL_handle_low) ;	
				CString strFileName = _T("δ֪");
				cSet.GetFieldValue(_T("devname"), strFileName) ;
				
				double dCurTotalY = 0 ;
				this->ComputeCoreOfOneXhl(iXHL_handle_low, dCurTotalY) ;

				//begin ����������
				this->ComputeFxData(iXHL_handle_low) ;
				//end ����������			

				//���Ʊ��ΪiIndex������ͼ
				if (m_strSavePath.IsEmpty())
				{
					AcApDocument* pDocCur = acDocManager->curDocument() ;
					CString strFileCur =  pDocCur->fileName() ;
					CBlkUtility::GetFilePath(strFileCur, m_strSavePath) ;
				}
				CString strSaveFile ;
				strSaveFile.Format(_T("%s\\%s.dwg"), this->m_strSavePath, strFileName) ;			
				this->DrawPx(strSaveFile, iXHL_handle_low) ;

				iIndex++ ;

				cSet.MoveNext() ;
			}
			acedRestoreStatusBar() ;		
			acutPrintf(_T("\n�������߽�����......\n")) ;

			cSet.Close() ;
		}
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
BOOL CCableNet::ReadAndFillDB(AcDbDatabase *pCurDb)
{
	BOOL bRet = TRUE ;

	acedSetStatusBarProgressMeter(_T("readcablenet..."), 1, 100) ;	
				
	if (!this->ReadEntToDB(pCurDb))
	{
		return FALSE ;
	}
	this->ComputeDevAndBoxType() ; 
	acedSetStatusBarProgressMeterPos(10) ;

	if (!this->ReadConCableToDB(pCurDb))
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

//��������ͼ��һ����¥�˻���һ��ͼ
void CCableNet::DrawPx(CString strDwgFilePath, long lXHLHandle)
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

		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		
		//begin ����о��
		strSql.Format(_T("select * from cable_core_draw order by pt_y desc")) ;
		if( cSet.Open(strSql))
		{
			while (!cSet.IsEOF())
			{
				CString strDevName, strDevNameTmp, strCoreName, strCoreNameShow, strCoreSn ;
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
				int iIsShare = 0 ;

				cSet.GetFieldValue(_T("dev_name"), strDevName) ;
				cSet.GetFieldValue(_T("core_name"), strCoreName) ;
				cSet.GetFieldValue(_T("core_name_show"), strCoreNameShow) ;
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
				cSet.GetFieldValue(_T("is_share"), iIsShare) ;

				if (iBoxHandle==49)
				{
					int iiii = 0  ;
				}

				strDevNameTmp = strDevName ;
				//begin �����շ�JSQ������ȥAB
				strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
				CString strBelongBoxBlock ;
				m_RecordSet.GetRecordValue(strSql, _T("box_block"), strBelongBoxBlock) ;
				//end �����շ�JSQ������ȥAB
				if (strBelongBoxBlock!=_T("BOX_HZ_JSQ_0"))
				{
					strDevNameTmp.TrimRight(_T("-A")) ;
					strDevNameTmp.TrimRight(_T("-B")) ;
				}

				//ȡ���豸����
				strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
				m_RecordSet.GetRecordValue(strSql, _T("devtype"), strDevType) ;
				
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
					if (!(iIsNew&&iIsAdd||iIsAdd&&dPt_x==0))//��о����㲻Ҫ���ƶ���
					{
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_start,0, 1) )
						{
							pRef->close() ;
						}
					}

					AcGePoint3d pt_3d_con1 = pt_3d_start,pt_3d_con2 = pt_3d_start ;
					pt_3d_con2.y+=5 ;
					if (iIsAdd) //��о�ߺ����������������
					{
						mysys.AddAcDbLine(pt_3d_con1, pt_3d_con2, 1) ;
					}

					if (iIsLast==1)
					{
						if (iIsAdd) //��о�ߺ����������������
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
				
				ads_point base_pt_text = {0,0,0} ;
				//AcDbObjectId objID ;
				//���Ӻ�
				if ((iIsNew==1 && iIsVirtual==0) || (dPt_x==0 && iIsAdd==0))
				{
					base_pt_text[0] = pt_3d_start.x ;
					base_pt_text[1] = pt_3d_start.y+1 ;
					mysys.AddText(strStartNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;//���
				}

				if (dPt_x!=0)
				{
					base_pt_text[0] = pt_3d_end.x ;
					base_pt_text[1] = pt_3d_end.y+1 ;
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

						//��о������DZ��
						if (iIsAdd==0)
						{
							AcDbBlockReference *pRef = NULL ;
							if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_end_for4,0, 1) )
							{
								pRef->close() ;
							}
						}

						ads_point base_pt_text_for4 = {pt_3d_end_for4.x,pt_3d_end_for4.y+1,0} ;
						mysys.AddText(strEndNum1, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��
						mysys.AddText(strEndNum2, base_pt_text_for4, base_pt_text_for4, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��
					}
					else
					{
						mysys.AddText(strEndNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��
					}
				}
				if (iIsVirtual==0)
				{
					base_pt_text[0] = pt_3d_start.x+(pt_3d_end.x-pt_3d_start.x)/2+(pt_3d_start.x>pt_3d_end.x?-2:2) ;			
					mysys.AddText(strCoreSn, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;

					base_pt_text[0] = pt_3d_text.x ;
					base_pt_text[1] = pt_3d_text.y ; 
					//�ź�¥����ʱ����

					if (dPt_x!=0)
					{
						if (iIsShare>0&&m_bShareWithDevName==FALSE)
						{
							strTmp = strCoreNameShow ;
						}
						else
						{
							strTmp.Format(_T("%s-%s"), strDevNameTmp, strCoreNameShow) ;
							if (strCoreNameShow.Left(1) == _T("J") || strCoreNameShow.Left(1) == _T("F") )
							{
								strTmp.Format(_T("%s%s"), strDevNameTmp, strCoreNameShow) ;
							}
						}
						//����о������
						mysys.AddText(strTmp, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
					}
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end ����о��
		
		//begin ����title�ͱ߿�
		strSql.Format(_T("select * from cablenet_ent where belong_xhl_handle_low=%d"), lXHLHandle) ;
		if(cSet.Open(strSql) )
		{
			while (!cSet.IsEOF())
			{
				CString strDevName, strBoxName, strCableString, strBackupString, strFreq ;
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
				cSet.GetFieldValue(_T("freq"), strFreq) ;

				int iPlusX = iLeftOrRight==0?-1:1 ;
				AcDb::TextHorzMode iHorMode = AcDb::kTextLeft ;
				AcDb::TextVertMode iVerMode = AcDb::kTextVertMid ;

				//AcGePoint3d pt_3d_text(0,0,0) ;
				ads_point pt_base_text = {0,0,0} ;
				pt_base_text[1] = dTitle_y+m_dCellHight ;
				pt_base_text[0] = iLeftOrRight==0?dTitle_x-m_dCellWidth:dTitle_x ;

				if (strBoxName==_T("XHL"))
				{
					pt_base_text[0] = pt_base_text[0]+m_dCellWidth/2 ;
					iHorMode = AcDb::kTextCenter ;				
					mysys.AddText(_T("�ź�¥"), pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
					pt_base_text[1] += m_dCellHight*2 ;
					strDevName+=_T("#") ;
					mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
					mysys.AddAcDb2dPolyline(AcGePoint3d(0,0,0), AcGePoint3d(0,dBottom_y,0), 0.5) ; //�ź�¥�����
				}
				else
				{
					pt_base_text[0] = pt_base_text[0]+4 ;
					if ((strDevType.Left(1)==_T("J") && strDevType.Left(2)!=_T("JZ")) || strDevType.Left(1)==_T("F"))
					{
						int iMarkLoc = strDevType.Find(_T("-")) ;
						if (iMarkLoc!=-1)
						{
							strDevName+=strDevType.Left(strDevType.Find(_T("_"))) ;
						}
					}
					mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

					iHorMode = AcDb::kTextRight ;
					pt_base_text[0] = pt_base_text[0]-4+m_dCellWidth-4 ;
					mysys.AddText(strBoxName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

					if (!strFreq.IsEmpty())
					{
						iHorMode = AcDb::kTextMid;
						pt_base_text[0] = pt_base_text[0]-15 ;
						int iColorIndex = 3 ; //ż��Ƶ����ɫ
						int iTmp = _ttoi(strFreq.Left(2)) ;
						if (iTmp%2==1)
						{
							iColorIndex = 1 ; //����Ƶ�ʺ�ɫ
						}						
						mysys.AddText(strFreq, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, iColorIndex, textStyleId) ;
					}
				}

				AcGePoint3d pt_3d_start(dTitle_x,dTitle_y,0) ;
				AcGePoint3d pt_3d_end(dTitle_x,dTitle_y,0) ;
				AcGePoint3d pt_3d_tmp(0,0,0) ;
				pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
				mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5) ;
				pt_3d_tmp.x = pt_3d_end.x ;
				pt_3d_tmp.y = dBottom_y ;
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
					mysys.AddText(strCableString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;
					if (m_bDrawBackup==TRUE)
					{
						pt_base_text[1] = dBottom_y+4 ;
						mysys.AddText(strBackupString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
						pt_base_text[0] = dBottom_x - iPlusX*5 ;
						mysys.AddText(_T("����"), pt_base_text, pt_base_text, AcDb::kTextCenter, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
					}
				}
				//here

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end ����title�ͱ߿�

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
void CCableNet::FindBranch(long iDesHandle, int &iBranchNum)
{
	try
	{	
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		
		strSql.Format(_T("select * from cablenet_ent where next_ent_handle_low=%d"), iDesHandle) ;
		if(cSet.Open(strSql))
		{
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

void CCableNet::FindNextEnt(long iHandle, int iForwardToLeftOrRight)
{
	CString strNextHandeName[] = {_T("left_ent_handle"), _T("right_ent_handle")};
	try	
	{				
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		
		//begin ����cablenet_ent���left_ent_handle��right_ent_handle
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iHandle) ;
		if(cSet.Open(strSql))
		{
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
				m_RecordSet.GetRecordValue(strSql, _T("cablestring"), strCableStr) ;

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

int CCableNet::ReadFxpDataToDb(CString strXlsFileName)
{
	int iReturn = 0 ;
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	CString strExec, strMsg ;
	//�������������
	m_AdoDb.ClearTable(_T("fx_data")) ;

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
					m_AdoDb.Execute(strExec) ;

				}
				else
				{
					CString strMsg ;
					strMsg.Format(_T("�������ݵ������%d������δ����ɹ���������з��߶����Ƿ�Ϻ�Ҫ�󣬻���ϵ�����ߣ�"), iCur_Row) ;
					AfxMessageBox(strMsg) ;
				}

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
void CCableNet::ComputeFxData(long iXhlHandle)
{
	try	
	{				
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;

		//begin ȡ���ź�¥�˵����ƣ�����DC-130
		CString strXhl = _T("") ;
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iXhlHandle) ;
		m_RecordSet.GetRecordValue(strSql, _T("devname"), strXhl) ;
		//end ȡ���ź�¥�˵����ƣ�����DC-130

		//begin ȡ���������ڸ��ź�¥��о��
		strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d order by pt_y desc"), iXhlHandle) ;
		if(cSet.Open(strSql))
		{
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

				//ֱ�Ӳ���
				CADORecordset cSetFoundFx (&m_AdoDb) ;
				int iFoundFx = -1 ;
				strSql.Format(_T("select * from fx_data where (dev_name='%s' or [dev_name]&[dev_type]='%s') and core_name='%s'"), strDevNameTmp, strDevNameTmp, strCoreName) ;
				if (cSetFoundFx.Open(strSql))
				{
					if (cSetFoundFx.GetRecordCount()>0)
					{
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
				}

				//û��ֱ���ҵ�������ʼ���ӣ��ټ�����߶���(����ǹ���������ֱ��ָ��fx)
				if (strFxName==_T("0") && iIsShare==0)
				{
					//here 20100412-1705
					strSql.Format(_T("select * from cablenet_ent where devname='%s'"), strDevName) ;
					CADORecordset cSetFound (&m_AdoDb) ;
					CString strDevType = _T("") ;
					CString strDevFor = _T("") ;
					if (cSetFound.Open(strSql))
					{
						if (cSetFound.GetRecordCount()>0)
						{
							//20110504:define_dev����dev_type���JZA_5_0_JH��ʽ					
							cSetFound.GetFieldValue(_T("devtype"), strDevType) ;
							cSetFound.GetFieldValue(_T("dev_for"), strDevFor) ;
						}
						cSetFound.Close() ;
					}

					//�ҵ��������豸����ȫ��о��
					if (!strDevType.IsEmpty())
					{
						strSql.Format(_T("select * from define_dev_use_cablecore where dev_id=(select id from define_dev where dev_type='%s' and dev_for='%s') order by id"), strDevType, strDevFor) ;
						if (cSetFound.Open(strSql))
						{
							if (cSetFound.GetRecordCount()>0)
							{
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
								CADORecordset cSetFoundFirstFx (&m_AdoDb) ;						
								if (cSetFoundFirstFx.Open(strSql))
								{
									if (cSetFoundFirstFx.GetRecordCount()>0)
									{
										int iShelfNum = 0 ;
										int iLayerNum = 0 ;
										int iBoxNum = 0 ;
										int iCellNum = 0 ;
										cSetFoundFirstFx.GetFieldValue(_T("shelf_num"), iShelfNum) ;
										cSetFoundFirstFx.GetFieldValue(_T("layer_num"), iLayerNum) ;
										cSetFoundFirstFx.GetFieldValue(_T("box_num"), iBoxNum) ;
										cSetFoundFirstFx.GetFieldValue(_T("cell_num"), iCellNum) ;
	
										strFirstFxName.Format(_T("F%d-%d0%d-%d"), iShelfNum, iLayerNum, iBoxNum, iCellNum) ;		
									}
									cSetFoundFirstFx.Close() ;
								}
		
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
				}

				//�ۺ϶��� FS FSH JS JSH
				if (strCoreName==_T("FS")||strCoreName==_T("FSH")||strCoreName==_T("JS")||strCoreName==_T("JSH"))
				{
					strSql.Format(_T("select * from zh_place where gd_name='%s' and type='%s'"), strDevName, strCoreName.Left(2)) ;
					if(cSetFoundFx.Open(strSql))
					{
						if (cSetFoundFx.GetRecordCount()>0)
						{
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
		}
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

//�����¥�˻���һ��ͼ
void CCableNet::DrawPx(int iIndex, int iDwgSum, AcDbDatabase *pCurDb, long iXhlHandle, double dCurTotalYForOneDwg)
{
	try
	{
		CString strBlockFile ;
		strBlockFile.Format(_T("%s\\support\\cablenet\\QT\\DZ.DWG"), m_strSysBasePath) ;
	
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;
		AcDbObjectId textStyleId ;
		textStyleId = mysys.CreateTextStyleByFileName(_T("ZhfDlpx"), _T("txt.shx"), _T("hztxt.shx"), 2, 0.8) ;

		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		
		//begin ����о��
		strSql.Format(_T("select * from cable_core_draw")) ;
		if (cSet.Open(strSql))
		{
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
				m_RecordSet.GetRecordValue(strSql, _T("devtype"), strDevType) ;

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
				
				ads_point base_pt_text = {0,0,0} ;				
				//���Ӻ�
				if ((iIsNew==1 && iIsVirtual==0) || dPt_x==0)
				{
					base_pt_text[0] = pt_3d_start.x ;
					base_pt_text[1] = pt_3d_start.y+1 ;
					mysys.AddText(strStartNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;//���
				}
	
				if (dPt_x!=0)
				{
					base_pt_text[0] = pt_3d_end.x ;
					base_pt_text[1] = pt_3d_end.y+1 ;
					mysys.AddText(strEndNum, base_pt_text, base_pt_text, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;//ĩ��
				}
				if (iIsVirtual==0)
				{
					base_pt_text[0] = pt_3d_start.x+(pt_3d_end.x-pt_3d_start.x)/2+(pt_3d_start.x>pt_3d_end.x?-2:2) ;			
					mysys.AddText(strCoreSn, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
					
					base_pt_text[0] = pt_3d_text.x ;
					base_pt_text[1] = pt_3d_text.y ; 
		
					if (dPt_x!=0)
					{
						strTmp.Format(_T("%s-%s"), strDevNameTmp, strCoreName) ;
						if (strCoreName.Left(1) == _T("J") || strCoreName.Left(1) == _T("F") )
						{
							strTmp.Format(_T("%s%s"), strDevNameTmp, strCoreName) ;
						}
						mysys.AddText(strTmp, base_pt_text, base_pt_text, iHorMode, AcDb::kTextBase, 2, 0.7, 0, 0, textStyleId) ;
					}
				}	
				cSet.MoveNext() ;
			}	
			cSet.Close() ;
		}
		//end ����о��
		
		//begin ����title�ͱ߿�
		strSql.Format(_T("select * from cablenet_ent where belong_xhl_handle_low=%d"), iXhlHandle) ;
		if(cSet.Open(strSql))
		{
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

				ads_point pt_base_text = {0,0,0} ;
				pt_base_text[1] = dTitle_y+m_dCellHight ;
				pt_base_text[0] = iLeftOrRight==0?dTitle_x-m_dCellWidth:dTitle_x ;

				if (strBoxName==_T("XHL"))
				{
					pt_base_text[0] = pt_base_text[0]+m_dCellWidth/2 ;
					iHorMode = AcDb::kTextCenter ;				
					if (iIndex==0)
					{
						mysys.AddText(_T("�ź�¥"), pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
					}
					if (iIndex==iDwgSum-1)
					{
						mysys.AddAcDb2dPolyline(AcGePoint3d(0,0,0), AcGePoint3d(0,dBottom_y,0), 0.5) ; //�ź�¥�����
					}

					pt_base_text[0]  = pt_base_text[0] + m_dCellWidth*(iLeftOrRight==0?-1:1) ;
					pt_base_text[1] += m_dCellHight*2 ;
					strDevName+=_T("#") ;
					mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

				}
				else
				{
					pt_base_text[0] = pt_base_text[0]+4 ;
					if ((strDevType.Left(1)==_T("J") && strDevType.Left(2)!=_T("JZ")) || strDevType.Left(1)==_T("F"))
					{
						int iFindLoc = strDevType.Find(_T("_")) ;
						if(iFindLoc!=-1)
						{
							strDevName+=strDevType.Left(iFindLoc) ;
						}						
					}
					mysys.AddText(strDevName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;

					iHorMode = AcDb::kTextRight ;
					pt_base_text[0] = pt_base_text[0]-4+m_dCellWidth-4 ;
					mysys.AddText(strBoxName, pt_base_text, pt_base_text, iHorMode, iVerMode, 2, 0.8, 0, 0, textStyleId) ;
				}

				AcGePoint3d pt_3d_start(dTitle_x,dTitle_y,0) ;
				AcGePoint3d pt_3d_end(dTitle_x,dTitle_y,0) ;
				AcGePoint3d pt_3d_tmp(0,0,0) ;
				pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
				if(!(iIndex!=0&&strBoxName==_T("XHL")))
				{
					mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5);//���������
				}

				pt_3d_tmp.x = pt_3d_end.x ;
				pt_3d_tmp.y = dBottom_y ;			
				mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_tmp, 0.5) ;//��߿�

				pt_3d_start.y = dBottom_y ;
				if (!(iIndex!=iDwgSum-1 && strBoxName==_T("XHL")))
				{
					mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_tmp, 0.5) ;//�±߿�
				}			

				pt_3d_start.x = dTitle_x ;
				pt_3d_start.y = dTitle_y+m_dCellHight*2 ;
				pt_3d_end.x = dTitle_x+iPlusX*m_dCellWidth ;
				pt_3d_end.y = pt_3d_start.y ;
				if (strBoxName!=_T("XHL") ||(strBoxName==_T("XHL") && iIndex==0) )
				{
					mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.5) ;//���������
				}			

				pt_3d_start.x = pt_3d_end.x ;
				pt_3d_start.y = dTitle_y ;
				mysys.AddAcDb2dPolyline(pt_3d_end, pt_3d_start, 0.5) ; //���������

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
					mysys.AddText(strCableString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextBase, 2, 0.8, 0, 0, textStyleId) ;
					pt_base_text[1] = dBottom_y+4 ;
					mysys.AddText(strBackupString, pt_base_text, pt_base_text, iHorMode, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
					pt_base_text[0] = dBottom_x - iPlusX*5 ;
					mysys.AddText(_T("����"), pt_base_text, pt_base_text, AcDb::kTextCenter, AcDb::kTextTop, 2, 0.8, 0, 0, textStyleId) ;
				}
				//here

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
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
void CCableNet::ComputeCoreAndDraw2()
{
	try
	{				
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		AcDbDatabase *pCurDb = new AcDbDatabase ;

		//begin һ���ź�¥ent����һ��ͼֽ��ѭ����������ͼֽ
		//////////////////////////////////////////////////////////////////////////
		strSql.Format(_T("SELECT * FROM cablenet_ent where boxtype='XHL'")) ;
		if (cSet.Open(strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
	
			CString strBarMsg ;
			strBarMsg.Format(_T("��������:%d��"), iRsCount) ;
			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;
	
			int iIndex = 0 ;
			double dCurTotalYForOneDwg = 0 ; //���Ƶ�һ��ͼֽʱ����¼ÿ�������ʱ��Y����		
	
			while (!cSet.IsEOF())
			{
				//����ձ�
				m_AdoDb.ClearTable(_T("cable_core_draw")) ;
	
				acedSetStatusBarProgressMeterPos(iIndex) ;			
	
				long iXHL_handle_low = 0 ;			
				cSet.GetFieldValue(_T("handle_low"), iXHL_handle_low) ;	

				double dCurTotalY = 0 ;
				this->ComputeCoreOfOneXhl(iXHL_handle_low, dCurTotalY) ;
	
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
			acutPrintf(_T("\n�������߽�����......\n")) ;
	
			cSet.Close() ;
		}
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

//ȡ�øú���ǰһ������,����ZD6���ã�ȡ��˫���ĵ�2��
//modify:20111026,��ZYJ6-7Ҳ����
BOOL CCableNet::GetPreviewEnt(long iCurEntHandle, long &iPreviewEntHandle)
{
	BOOL bRet = FALSE ;
	try
	{
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;
		
		//��ʾ����ʼ(û��ǰһ�����ӵ�)��ent
		strSql.Format(_T("SELECT handle_low FROM cablenet_ent WHERE next_ent_handle_low=%d"), iCurEntHandle) ;
		if (cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.GetFieldValue(_T("handle_low"), iPreviewEntHandle) ;
				bRet = TRUE ;			
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
		AfxMessageBox(_T("unknown error in GetPreviewEnt")) ;
	} 

	return bRet ;
}

// ѭ����ÿ��box���÷�֧��ţ��ź�¥Ϊ��һ�������1�����ź�¥Ϊ��һ����box��y�Ӵ�С���1-1��1-2��......
void CCableNet::SetBoxBranchIndex(long nCurBoxHandle, const CString & strCurIndex)
{
	try
	{
		CADORecordset cSet (&m_AdoDb) ;
		CString strSql ;		
		strSql.Format(_T("SELECT * FROM cablenet_ent WHERE next_ent_handle_low=%d order by pt_y desc"), nCurBoxHandle) ;
		if(cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
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

// ����һ����¥�˵����е���о��
void CCableNet::ComputeCoreOfOneXhl(long iXHL_handle_low, double& dCurTotalY)
{
	try
	{				
		CString strSql ;

		//begin ������iXHL_handle_lowΪ�յ��������㣬Ȼ���Ը�������ź�¥������һ�飬����о������
		strSql.Format(_T("select * from cablenet_ent where is_start=1 and belong_xhl_handle_low=%d order by branch_index"), iXHL_handle_low) ; //20130420:����branch_index�У������ж���ʼbox���Ƶ�˳��y����
		CADORecordset cSet_AllStart(&m_AdoDb) ;
		if (cSet_AllStart.Open(strSql))
		{
			//ÿ������ͼ��Y�����0��ʼ
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

				CLongArray iArrayHandle_OneRoute ; //��iStartHandle_LowΪ����һ����·�ϵ����нڵ㣨���ӣ�
				this->FindOneRoute(iStartHandle_Low, iArrayHandle_OneRoute) ;
				double dCurX = 0 ; //��ÿ����·������ʼ���ź�¥�����������е�x����
				double dPlusX = (iLeftORight==1?1:-1) ;//��ߴ��ź�¥����x�����Ǽ��٣��ұ߷�֮
				int iEntNum = iArrayHandle_OneRoute.GetSize() ;

				//ǰһ��devname
				CString strPreDevName = _T("") ;
				//begin for ����һ����·�е�ÿһ��ent��box��hz��������ʹ�õ���о��
				CString strPassedHandle = _T("") ; //�Ѿ��߹�����·�ڵ��handle
				for (int i=0; i<iEntNum; i++)
				{					
					long iHandleCurEnt = iArrayHandle_OneRoute.GetAt(i) ;
					strPassedHandle.AppendFormat(_T("%d,"), iHandleCurEnt) ;

					strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iHandleCurEnt) ;
					CADORecordset cSetOneEntInRoute(&m_AdoDb) ;
					if(cSetOneEntInRoute.Open(strSql))
					{
						if (cSetOneEntInRoute.GetRecordCount()==0)
						{
							cSetOneEntInRoute.Close() ;
							continue;
						}
						CString strDevType ;
						CString strDevName ;
						CString strDevFor ;
						CString strBoxType ;
						CString strFreq ;

						long iBranch_num = 0 ;
						cSetOneEntInRoute.GetFieldValue(_T("scan_time"), iScan_Time) ;
						cSetOneEntInRoute.GetFieldValue(_T("devtype"), strDevType) ;
						cSetOneEntInRoute.GetFieldValue(_T("devname"), strDevName) ;
						cSetOneEntInRoute.GetFieldValue(_T("dev_for"), strDevFor) ;
						cSetOneEntInRoute.GetFieldValue(_T("boxtype"), strBoxType) ;
						cSetOneEntInRoute.GetFieldValue(_T("branch_num"), iBranch_num) ;
						CString strBoxBlock ;
						cSetOneEntInRoute.GetFieldValue(_T("box_block"), strBoxBlock) ;
						cSetOneEntInRoute.GetFieldValue(_T("freq"), strFreq) ;

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
							if (strDevType!=_T(""))
							{
								CADORecordset cSetGetDev(&m_AdoDb) ;
								//����ȡ�������豸ʹ�õ�о��
								strSql.Format(_T("select * from define_dev where dev_type='%s' and dev_for='%s'"), strDevType, strDevFor) ;
								if (m_RecordSet.GetRecordCount(strSql)==0)
								{
									CString strSqlTmp ;
									strSqlTmp.Format(_T("select * from define_dict where source='%s' and dict_for='FOR_CORE'"), strDevType) ;
									CString strDevTypeTmp ;
									if(m_RecordSet.GetRecordValue(strSqlTmp, _T("destination"), strDevTypeTmp)==1)
									{
										strSql.Format(_T("select * from define_dev where dev_type='%s' and dev_for='%s'"), strDevTypeTmp, strDevFor) ;
									}
								}

								CString strDevNameTmp[2] = {_T(""), _T("")} ;
								strDevNameTmp[0] = strDevName ;
								//begin �ǵ绯���κ���XB��
								if ((strDevType==_T("FSD_GD_0")&&strBoxType==_T("XB2"))||(strBoxBlock==_T("BOX_FSD_S-S_00"))) 
								{
									int iLocFind = strDevName.Find(_T(" ")) ;
									if (iLocFind!=-1)
									{
										strDevNameTmp[0] = strDevName.Left(iLocFind) ;
										strDevNameTmp[1] = strDevName.Mid(iLocFind+1) ;
										strSql.Format(_T("select * from define_dev where dev_type='%s' and dev_for='%s' union all select * from define_dev where dev_type='%s' and dev_for='%s'"), strDevType, strDevFor, strDevType, strDevFor) ;
									}
								}
								//end �ǵ绯���κ���XB��

								//begin ת�޻��Ȳ�ѯ�ֵ����޴�strDevType��ӳ�䣬���м�����ӳ������ΪstrDevType��������Ҫ���Ǽ�о
								CMapStringToString mapZzjPlusCore ;
								long nZzjPlusId = 0 ; //��о��Ӧ��¼���������������ж��ڣ����ڷ�����࣬������õ�
								if (strBoxBlock==_T("BOX_HZ_ZZJ_0"))
								{
									CString strSqlTmp ;
									strSqlTmp.Format(_T("select  * from define_dict where dict_for='ZZJ_CORE' and source='%s'"), strDevType) ;
									CString strDestination ;
									if (m_RecordSet.GetRecordValue(strSqlTmp, _T("destination"), strDestination) >0)
									{
										strSql.Format(_T("select * from define_dev where dev_type='%s' and dev_for='%s'"), strDestination, strDevFor) ;
									}
									cSetOneEntInRoute.GetFieldValue(_T("zzj_plus_id"), nZzjPlusId) ;
									if (nZzjPlusId>0)
									{
										strSqlTmp.Format(_T("select * from define_zzj_plus where id=%d"), nZzjPlusId) ;
										CString strCoreList ;
										if (m_RecordSet.GetRecordValue(strSqlTmp, _T("core_list"), strCoreList) >0)
										{
											CStringArray strArrayCoreList ;
											CBlkUtility::DivideString(strCoreList, _T("_"), strArrayCoreList) ;
											for (int iIndexCoreList=0; iIndexCoreList<strArrayCoreList.GetCount(); iIndexCoreList++)
											{
												CString strCoreTmp = strArrayCoreList.GetAt(iIndexCoreList) ;
												int iFindLoc = strCoreTmp.Find(_T('-')) ;
												if (iFindLoc!=-1)
												{
													mapZzjPlusCore.SetAt(strCoreTmp.Left(iFindLoc), strCoreTmp.Right(strCoreTmp.GetLength()-iFindLoc-1)) ;
												}												
											}
										}
									}
								}
								//end ת�޻��Ȳ�ѯ�ֵ����޴�strDevType��ӳ�䣬���м�����ӳ������ΪstrDevType��������Ҫ���Ǽ�о

								//���������XB�䣬CcSetGetDev��ѯһ���ӦΨһ��¼
								if(cSetGetDev.Open(strSql))
								{
									int iDevIndex = 0 ;
									while (!cSetGetDev.IsEOF())
									{
										int iDevId = -1 ;
										cSetGetDev.GetFieldValue(_T("id"), iDevId) ;

										//����о����Ϣ
										CADORecordset cSetGetCoreDetail(&m_AdoDb) ;
										strSql.Format(_T("select * from define_dev_use_cablecore where dev_id=%d order by id"), iDevId) ;
										if(cSetGetCoreDetail.Open(strSql))
										{
											while (!cSetGetCoreDetail.IsEOF())
											{
												CString strCoreName  ; //о�����ƣ�����DH DHH
												CString strCoreNameShow  ; //о����ʾ���ƣ�����core_name ΪX32����ʾΪX3
												CString strCoreSn  ; //о�߱��,���磺D1-1 II2-3
												int iIsInside = 0 ;			//�ڲ�����Ҫ������о�ߣ�0/���ڲ���1/�ڲ�
												int iIsShare = 0 ;			//�ǲ��ǹ����ߣ�����DH DHH 0/�����ã�1/����
												CString strStartNum  ;			//о�����ʹ�õĺ��ӵĶ���
												CString strEndNum  ;			//о��ĩ��ʹ�õĺ��ӵĶ���
												int iIsVirtual = 0 ;	//0/�����⣬1/����о��ռλ��
												double dCurCoreX = dTitleX ;
												double dCurCoreY = dCurTotalY ;
												int iIsNew = 1 ;
												int iIsAdd = 0 ;

												cSetGetCoreDetail.GetFieldValue(_T("is_virtual"), iIsVirtual) ;
												cSetGetCoreDetail.GetFieldValue(_T("is_add"), iIsAdd) ;
												cSetGetCoreDetail.GetFieldValue(_T("core_name"), strCoreName) ;
												cSetGetCoreDetail.GetFieldValue(_T("core_name_show"), strCoreNameShow) ;
												cSetGetCoreDetail.GetFieldValue(_T("is_share"), iIsShare) ;
												if (iIsShare>0 && i>0) //����ǹ����ߣ��Ҳ��ǵ�һ��ent����Ҫ����Ƿ�ǰ�����ӵĺ����������У������������Ӹ�о�ߣ�������һо��
												{
													long iHandlePre = iArrayHandle_OneRoute.GetAt(i-1) ;
													strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iHandlePre,strCoreName) ;
													if (m_RecordSet.GetRecordCount(strSql)>0)
													{
														cSetGetCoreDetail.MoveNext() ;
														continue ;
													}
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

												if (m_bDefaultCablecore)
												{
													CString strStartNumTmp, strEndNumTmp ;
													cSetGetCoreDetail.GetFieldValue(_T("start_num_default"), strStartNumTmp) ;
													cSetGetCoreDetail.GetFieldValue(_T("end_num_default"), strEndNumTmp) ;
													if (strStartNumTmp!=_T("0")&&!strStartNumTmp.IsEmpty())
													{
														strStartNum = strStartNumTmp ;
													}
													if (strEndNumTmp!=_T("0")&&!strEndNumTmp.IsEmpty())
													{
														strEndNum = strEndNumTmp ;
													}
												}

												BOOL bFoundExist = FALSE ;
												if ((strDevType.Left(3)==_T("JZA")||strDevType.Left(3)==_T("JLA")||strDevType.Left(3)==_T("CZA"))&& i>0)//�����JZA���������ȷ���ǵ�2�����ӣ�����i>0���������
												{
													//�����Ѿ����ڵ�о�ߣ�����
													strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iArrayHandle_OneRoute.GetAt(i-1), strCoreName) ;													
													if(m_RecordSet.GetRecordValue(strSql, _T("pt_y"), dCurCoreY)>0)
													{
														iIsNew = 0 ;
														bFoundExist = TRUE ;		
													}	
												}
												else if (strDevType.Left(7)==_T("ZZJ_2_1") && strCoreName.Left(2)==_T("X4")) //����˫�������1��X4о��
												{
													strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s'"), iArrayHandle_OneRoute.GetAt(i-1), strCoreName) ;
													if(m_RecordSet.GetRecordValue(strSql, _T("pt_y"), dCurCoreY)>0)
													{
														iIsNew = 0 ;
														bFoundExist = TRUE ;		
													}
												}
												if (!bFoundExist)
												{
													dCurTotalY-=m_dCellHight ;
													dCurCoreY = dCurTotalY ;
												}

												iAddYMode = 0 ;
// 												strDevNameTmp[iDevIndex].TrimRight(_T("-B")) ;
// 												strDevNameTmp[iDevIndex].TrimRight(_T("-A")) ;
												strSql.Format(_T("insert into cable_core_draw(dev_name, core_name, core_name_show, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_virtual, is_add, freq) \
																 values('%s','%s', '%s', %d, %d, %d, %d, %f,%f, '%s', '%s', '%s', %d, %d, %d, '%s')"), strDevNameTmp[iDevIndex], strCoreName , strCoreNameShow, iHandleCurEnt, iIsNew, iIsInside, iIsShare, dCurCoreX, dCurCoreY, strCoreSn, strStartNum, strEndNum, iLeftORight, iIsVirtual, iIsAdd, strFreq) ;
												m_AdoDb.Execute(strSql) ;

												//begin ��о
												CString strCoreNum ;
												if (mapZzjPlusCore.Lookup(strCoreName, strCoreNum) )
												{
													int iCoreNum = _tstoi(strCoreNum) ;
													if (iCoreNum>1)
													{
														for (int iPlusIndex=0; iPlusIndex<(iCoreNum-1); iPlusIndex++)
														{
															CString strCoreNamePlus = strCoreName ;
															strCoreNamePlus.AppendFormat(_T("+%d"), iPlusIndex+1) ;
															iIsNew = 1 ;
															//iIsInside = 0 ;
															iIsShare = 0 ;
															strCoreSn = _T("") ;
															strStartNum = _T("") ;
															strEndNum = _T("") ;
															iIsVirtual  = 0 ;
															iIsAdd = 1 ;
															dCurTotalY-=m_dCellHight ;
															dCurCoreY = dCurTotalY ;
															strSql.Format(_T("insert into cable_core_draw(dev_name, core_name, core_name_show, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_virtual, is_add) \
																			 values('%s','%s', '%s', %d, %d, %d, %d, %f,%f, '%s', '%s', '%s', %d, %d, %d)"), strDevNameTmp[iDevIndex], strCoreNamePlus , strCoreNamePlus,  iHandleCurEnt, iIsNew, iIsInside, iIsShare, dCurCoreX, dCurCoreY, strCoreSn, strStartNum, strEndNum, iLeftORight, iIsVirtual, iIsAdd) ;
															m_AdoDb.Execute(strSql) ;
														}
													}
												}
												//end ��о

												cSetGetCoreDetail.MoveNext() ;
											} 

											cSetGetCoreDetail.Close() ;
										}

										cSetGetDev.MoveNext() ;
										iDevIndex++ ;
									}

									cSetGetDev.Close() ;
								}
							}
						}
						//end if û�б�����������һ�α���Ҫ����title����Ϊ�˺ж�Ӧ�豸������о��,��������ʼ����Ҫ��������о��

						//����ʼbox�⣬������Ҫ����ǰһ�����ӵ�о��
						if(i>0) 
						{
							long iHandlePre = iArrayHandle_OneRoute.GetAt(i-1) ;

							strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_inside<>1"), iHandlePre) ;
							CADORecordset cSetPreBoxCore (&m_AdoDb) ;
							if(cSetPreBoxCore.Open(strSql))
							{
								if (cSetPreBoxCore.GetRecordCount()>0)
								{
									while (!cSetPreBoxCore.IsEOF())
									{
										CString strDevNamePre, strCoreNamePre, strCoreNameShowPre, strCoreSnPre, strFreqPre ;
										long iBelong_HandlePre = 0 ;						
										int iIsSharePre = 0 ;
										CString strStartNum = _T("") ;
										CString strEndNum = _T("") ;
										int iLeftOrRightTmp = -1 ;
										double dPt_x = (iEntNum-i-1)*dPlusX*m_dCellWidth ;
										double dPt_y = 0 ;
										int iIsAddPre = 0 ;

										cSetPreBoxCore.GetFieldValue(_T("dev_name"), strDevNamePre) ;
										cSetPreBoxCore.GetFieldValue(_T("core_name"), strCoreNamePre) ;
										cSetPreBoxCore.GetFieldValue(_T("core_name_show"), strCoreNameShowPre) ;
										cSetPreBoxCore.GetFieldValue(_T("belong_box_handle_low"), iBelong_HandlePre) ;	
										cSetPreBoxCore.GetFieldValue(_T("is_share"), iIsSharePre) ;
										cSetPreBoxCore.GetFieldValue(_T("pt_y"), dPt_y) ;
										cSetPreBoxCore.GetFieldValue(_T("core_sn"), strCoreSnPre) ;
										cSetPreBoxCore.GetFieldValue(_T("start_num"), strStartNum) ;
										cSetPreBoxCore.GetFieldValue(_T("end_num"), strEndNum) ;
										cSetPreBoxCore.GetFieldValue(_T("left_or_right"), iLeftOrRightTmp) ;
										cSetPreBoxCore.GetFieldValue(_T("is_add"), iIsAddPre) ;
										cSetPreBoxCore.GetFieldValue(_T("freq"), strFreqPre) ;

										//�����Ѿ����ڵĹ���о�ߣ���������
										strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and core_name='%s' and is_share>0"), iArrayHandle_OneRoute.GetAt(i), strCoreNamePre) ;
										if (m_RecordSet.GetRecordCount(strSql)>0)
										{											
											cSetPreBoxCore.MoveNext() ;
											continue ;
										}									

										//�����Ѿ����ڵ�о�ߣ�����������������о�߲�һ��
										//����:����о����ֻҪ��DH,�Ͳ��������ˣ��˴��Ǽ�ʹ��X1�ˣ�������ͬһ���豸��X1����һ��Ҫ����
										strSql.Format(_T("select * from cable_core_draw where [dev_name]='%s' and [core_name]='%s' and belong_box_handle_low=%d"), strDevNamePre, strCoreNamePre, iArrayHandle_OneRoute.GetAt(i)) ;
										if (m_RecordSet.GetRecordCount(strSql)>0)
										{
											cSetPreBoxCore.MoveNext() ;
											continue ;
										}

										strSql.Format(_T("insert into cable_core_draw(dev_name,core_name, core_name_show, belong_box_handle_low, is_new, is_inside, is_share, pt_x, pt_y, core_sn, start_num, end_num, left_or_right, is_add, freq) \
														 values('%s','%s', '%s', %d, %d, %d, %d, %f,%f,'%s','%s', '%s', %d, %d, '%s')"), strDevNamePre, strCoreNamePre, strCoreNameShowPre, iHandleCurEnt, 0, 0, iIsSharePre, dPt_x, dPt_y, strCoreSnPre, strStartNum, strEndNum, iLeftOrRightTmp, iIsAddPre, strFreqPre) ;
										m_AdoDb.Execute(strSql) ;

										cSetPreBoxCore.MoveNext() ;							
									}
								}
								cSetPreBoxCore.Close() ;
							}
						}

						//ÿ����һ�ν�scan_time��1
						iScan_Time++ ;

						cSetOneEntInRoute.Edit() ;					
						cSetOneEntInRoute.SetFieldValue(_T("scan_time"), iScan_Time) ;
						cSetOneEntInRoute.Update() ;

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

						strPreDevName = strDevName ; //������devname�Ա���һ��devʹ��(��˫������ĵ�1��ʱʹ��,20121220�����ã���ǰһ���ڵ㲻һ���ǵ�2����������������)
					}
				}
				//end for ����һ����·�е�ÿһ��ent��box��hz��������ʹ�õ���о��

				cSet_AllStart.MoveNext() ;
			}
			cSet_AllStart.Close() ;
		}
		//end ������iXHL_handle_lowΪ�յ��������㣬Ȼ���Ը�������ź�¥������һ�飬����о������

		//begin �����ӱ��,о������
		//////////////////////////////////////////////////////////////////////////
		CADORecordset cSetAllBox (&m_AdoDb) ;
		strSql.Format(_T("select distinct belong_box_handle_low from cable_core_draw")) ;
		if(cSetAllBox.Open(strSql))
		{
			if (cSetAllBox.GetRecordCount()>0)
			{
				while(!cSetAllBox.IsEOF())
				{
					long iBoxHandle = 0 ;
					cSetAllBox.GetFieldValue(_T("belong_box_handle_low"), iBoxHandle) ;

					//	int iBoxDzSum = 0 ; //�ú��Ӷ������� ע���ѯ���������xhl��û�н��
					CString strBoxType = _T("") ;
					CString strDevType = _T("") ;//20130412:л����ʯ̫��devtypeΪF_25_0��J_25_0ʱ�����ڷǱ��豸����о�߶��Ӵ�11��ʼ��
					//begin �õ��ú�������
					CADORecordset cSetGetBoxType (&m_AdoDb) ;
					strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
					if(cSetGetBoxType.Open(strSql) )
					{
						if (cSetGetBoxType.GetRecordCount()>0)
						{
							cSetGetBoxType.GetFieldValue(_T("boxtype"), strBoxType) ;
							cSetGetBoxType.GetFieldValue(_T("devtype"), strDevType) ;
							if (strBoxType==_T("XHL")) //�������ź�¥�������������´�ѭ��
							{
								cSetGetBoxType.Close() ;
								cSetAllBox.MoveNext() ;
								continue ;
							}
						}
						cSetGetBoxType.Close() ;
					}
					//end �õ��ú�������

					//begin �õ����ӵ�box_type_id
					int iBoxTypeId = -1 ;
					CADORecordset cSetGetBoxTypeId (&m_AdoDb) ;
					strSql.Format(_T("select * from define_box where box_type='%s'"), strBoxType) ;
					m_RecordSet.GetRecordValue(strSql, _T("id"), iBoxTypeId) ;
					//end �õ����ӵ�box_type_id

					//�Ƚ����ͺź��ӵ����ж��ӵ�is_used����0
					//20130412:л����ʯ̫��devtypeΪF_25_0��J_25_0ʱ�����ڷǱ��豸����о�߶��Ӵ�11��ʼ��
					// �ھ���վxh-5�����У�HF-7�ĵ�42������û����0�ɹ�����֪�ιʣ�����update�����ֲ��������£��ѽ������������0���ɹ�����Ҫ��record�رպ�Ÿ������һ����
					strSql.Format(_T("select * from define_box_detail where box_type_id=%d"), iBoxTypeId) ;
					m_RecordSet.SetRecordValue(strSql, _T("is_used"), 0) ;

					if (strDevType==_T("F_25_0")) //�͵��ռ��1��3���ӣ�����������11��ʼ��
					{
						strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and ((val(box_node)<11 and val(box_node)>3) or box_node='2')"), iBoxTypeId) ;
						m_AdoDb.Execute(strSql) ;
					}
					else if (strDevType==_T("J_25_0"))//�ܵ��ռ��1��2���ӣ�����������11��ʼ��
					{
						strSql.Format(_T("update define_box_detail set is_used=1 where box_type_id=%d and (val(box_node)<11 and val(box_node)>2) "), iBoxTypeId) ;
						m_AdoDb.Execute(strSql) ;
					}
					
					//begin �����·�ĺ��ӣ����ܵ���Ӳ���1~4���������뻯ʹ�ã�������������
					if (strDevType==_T("FSD_GD_F")||strDevType==_T("FSD_GD_S"))
					{
						strSql.Format(_T("select * from define_box_detail where box_type_id=%d and val(box_node)<5"), iBoxTypeId) ;
						m_RecordSet.SetRecordValue(strSql, _T("is_used"), 1) ;
					}
					//end �����·�ĺ��ӣ����ܵ���Ӳ���1~4���������뻯ʹ�ã�������������

					//here 20100401 

					//begin �������й���о�ߵĶ���
					//////////////////////////////////////////////////////////////////////////
					//begin �Ƚ��ڲ�ռ�õĶ��ӱ��
					//��ǰ���������Ķ���
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in(select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_inside=1"), iBoxHandle) ;
					CADORecordset cSetFindDz (&m_AdoDb) ;
					if(cSetFindDz.Open(strSql))
					{
						while (!cSetFindDz.IsEOF())
						{
							CString strDzNode = _T("") ;
							cSetFindDz.GetFieldValue(_T("end_num"), strDzNode) ;

							strSql.Format(_T("select * from define_box_detail where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
							m_RecordSet.SetRecordValue(strSql, _T("is_used"), 1) ;

							cSetFindDz.MoveNext() ;
						}
						cSetFindDz.Close() ;
					}
					//����������Ķ��� 
					//20130420:����������о�ߣ���ʹ����о�����ڲ�����ڲ��ģ�л����ʯ̫�ṩ������о�߻���������������ǰ�ţ�
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_inside>0 and is_share=0"), iBoxHandle) ;
					if(cSetFindDz.Open(strSql))
					{
						while (!cSetFindDz.IsEOF())
						{
							CString strDzNode = _T("") ;
							cSetFindDz.GetFieldValue(_T("start_num"), strDzNode) ;

							strSql.Format(_T("select * from define_box_detail where box_type_id=%d and box_node='%s'"), iBoxTypeId, strDzNode) ;
							m_RecordSet.SetRecordValue(strSql, _T("is_used"), 1) ;

							cSetFindDz.MoveNext() ;
						}
						cSetFindDz.Close() ;
					}
					//end �Ƚ��ڲ�ռ�õĶ��ӱ��

					//begin ���㹲��о�ߵĶ��Ӻ�
					CMapStringToString mapCoreNameToDzNum ;
					CADORecordset cSetShareType (&m_AdoDb) ; 
					//������о��ѡ������������is_share�Ĵ�С����������Ӻ�ʱ�����������������ӿ�ʼ��
					//unionǰ��/�����select ȥ����where.... and is_inside<>1��л����ʯ̫������о�߲����ڲ����ȫ�����Ӵ������ǰ�ţ�
					strSql.Format(_T("select * from (select is_share, core_name from cable_core_draw where belong_box_handle_low \
									 in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share>0 \
									 union select is_share, core_name from cable_core_draw where belong_box_handle_low=%d and is_share>0 ) order by is_share"), iBoxHandle, iBoxHandle) ;
					if(cSetShareType.Open(strSql))
					{
						if (cSetShareType.GetRecordCount()>0)
						{
							while (!cSetShareType.IsEOF())
							{
								CString strShareCoreName ;
								cSetShareType.GetFieldValue(_T("core_name"), strShareCoreName) ;

								int iFoundDz = -1 ;
								CString strCurDzNum = _T("") ;

								CADORecordset cSetDzDetail (&m_AdoDb) ;
								strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0 order by id desc"), iBoxTypeId) ;
								if(cSetDzDetail.Open(strSql))
								{
									if (cSetDzDetail.GetRecordCount()>0)
									{
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
									mapCoreNameToDzNum.SetAt(strShareCoreName, strCurDzNum) ;//for examp:<"DHH", 42>		
								}

								cSetShareType.MoveNext() ;
							}
						}
						cSetShareType.Close() ;
					}
					//end ���㹲��о�ߵĶ��Ӻ�

					//begin �ȸ���ǰһ����������Ĺ���о�ߵ�ĩ��ָ�����Ӻ�
					CADORecordset cSetPreShare (&m_AdoDb) ;
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low \
									 in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share>0 and is_inside<>1"), iBoxHandle) ;
					if(cSetPreShare.Open(strSql))
					{
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
					}
					//end �ȸ���ǰһ����������Ĺ���о�ߵ�ĩ��ָ�����Ӻ�

					//begin �����ڱ����ӵĹ���о����ʼ����ָ�����Ӻ�
					//select ȥ�� and is_inside=0 ������о�߲����Ƿ��ڲ�
					CADORecordset cSetCurBoxShare (&m_AdoDb) ;
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_share>0 "), iBoxHandle) ;
					if(cSetCurBoxShare.Open(strSql))
					{
						while (!cSetCurBoxShare.IsEOF())
						{
							CString strCoreName ;
							CString strDzNum = _T("") ;
							cSetCurBoxShare.GetFieldValue(_T("core_name"), strCoreName) ;
							CString strDzNumTmp ;
							cSetCurBoxShare.GetFieldValue(_T("start_num"), strDzNumTmp) ;
							if (m_bDefaultCablecore&&strDzNumTmp!=_T("0")&&!strDzNumTmp.IsEmpty())
							{
							}
							else
							{
								mapCoreNameToDzNum.Lookup(strCoreName, strDzNum) ;
								cSetCurBoxShare.Edit() ;
								cSetCurBoxShare.SetFieldValue(_T("start_num"), strDzNum) ;
								cSetCurBoxShare.Update() ;
							}

							cSetCurBoxShare.MoveNext() ;
						}
						cSetCurBoxShare.Close() ;
					}
					//end �����ڱ����ӵĹ���о����ʼ����ָ�����Ӻ�

					//end �������й���о�ߵĶ���

					//begin ����ʣ��ķǹ���о�ߵĶ���:�ȴ���ú������õ�о��дstart�ˣ�+�ٴ����ǰ����������о��дend�ˣ�
					CADORecordset cSetOtherCore (&m_AdoDb) ;
					//����������о
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_new=1 and is_share=0 and is_inside=0 and is_add=0"), iBoxHandle) ;
					if(cSetOtherCore.Open(strSql))
					{
						while (!cSetOtherCore.IsEOF())
						{
							int iFoundDz = -1 ;
							CString strCurDzNum = _T("") ;

							CADORecordset cSetDzDetail (&m_AdoDb) ;
							strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0"), iBoxTypeId) ;
							if(cSetDzDetail.Open(strSql))
							{
								if (cSetDzDetail.GetRecordCount()>0)
								{
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
								cSetOtherCore.SetFieldValue(_T("start_num"), strCurDzNum) ;
								cSetOtherCore.Update() ;
							}

							cSetOtherCore.MoveNext() ;
						}
						cSetOtherCore.Close() ;
					}

					//��ǰ����������о
					strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low in (select handle_low from cablenet_ent where next_ent_handle_low=%d) and is_share=0 and is_inside<>1 and is_add=0 order by pt_y desc"), iBoxHandle) ;
					if(cSetOtherCore.Open(strSql))
					{		
						while (!cSetOtherCore.IsEOF())
						{
							int iFoundDz = -1 ;
							CString strCurDzNum = _T("") ;

							CADORecordset cSetDzDetail (&m_AdoDb) ;
							strSql.Format(_T("select * from define_box_detail where box_type_id=%d and is_used=0"), iBoxTypeId) ;
							if(cSetDzDetail.Open(strSql))
							{
								if (cSetDzDetail.GetRecordCount()>0)
								{
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
					//end ����ʣ��ķǹ���о�ߵĶ���,�ȴ���ú������õ�о��дstart�ˣ�+�ٴ����ǰ����������о��дend�ˣ�

					//begin �����Щ����������о�ߣ�cable_core_draw:is_last��is_last�Ķ���Ҫ����end����
					strSql.Format(_T("SELECT * FROM cable_core_draw WHERE [dev_name]&[core_name] \
									 in (select [dev_name]&[core_name] from cable_core_draw where belong_box_handle_low=%d) \
									 and [dev_name]&[core_name] not in (select [dev_name]&[core_name] from cable_core_draw where \
									 belong_box_handle_low=(select next_ent_handle_low from cablenet_ent where handle_low=%d))"), iBoxHandle, iBoxHandle) ;	
					m_RecordSet.SetRecordValue(strSql, _T("is_last"), 1) ;
					//end �����Щ����������о�ߣ�cable_core_draw:is_last��is_last�Ķ���Ҫ����end����

					//begin ���������о�ߵı�� core_sn
					//////////////////////////////////////////////////////////////////////////
					this->ComputeCoreOfOneBox(iBoxHandle) ;

					cSetAllBox.MoveNext() ;
				}
			}
			cSetAllBox.Close() ;

		}
		//end �����ӱ��,о������
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeCoreOfOneXhl")) ;
	}
}

// �ڶ�������ͼ�����������ݿ���Ӧ���ֶΣ���ǰ�Ǽ����ڶ�������м���ģ�
void CCableNet::ComputeDevAndBoxType(void)
{
	CString strMsg ;
	try
	{				
		CString strSql ;
		strSql.Format(_T("select * from cablenet_ent ")) ; 
		CADORecordset cSet(&m_AdoDb) ;
		if (cSet.Open(strSql))
		{
			while (!cSet.IsEOF())
			{				
				CString strBoxBlock ;
				cSet.GetFieldValue(_T("box_block"), strBoxBlock) ;
				CStringArray strArrayBoxBlock ;
				CBlkUtility::DivideString(strBoxBlock, _T("_"), strArrayBoxBlock) ;
				int iCountBoxArray = strArrayBoxBlock.GetCount() ;

				CString strDevBlock ;
				cSet.GetFieldValue(_T("dev_block"), strDevBlock) ;
				CStringArray strArrayDevBlock ;
				CBlkUtility::DivideString(strDevBlock, _T("_"), strArrayDevBlock) ;
				int iCountDevArray = strArrayDevBlock.GetCount() ;

				CString strDevName ;
				cSet.GetFieldValue(_T("devname"), strDevName) ;

				if (strArrayBoxBlock.GetCount()>2)
				{
					CString strBoxTypeTmp =  strArrayBoxBlock.GetAt(1) ;
					CString strBoxType ;
					CString strDevType ;
					if (strBoxTypeTmp==_T("HZ")||strBoxTypeTmp==_T("HF"))
					{
						//����boxtype
						cSet.GetFieldValue(_T("boxtype"), strBoxType) ;
						if (strBoxType.Left(1)!=_T("H"))
						{
							strBoxType.Insert(0, strBoxTypeTmp) ;
							cSet.Edit() ;
							cSet.SetFieldValue(_T("boxtype"), strBoxType) ;
							cSet.Update() ;
						}

						//����devtype
						if (iCountDevArray>7) //��ʱֻ���źŻ�
						{
							if (strArrayDevBlock.GetAt(0)==_T("XHJ"))
							{
								CString strNumBiaoShiQi =  strArrayDevBlock.GetAt(4) ;
								int iHasDs = 0 ;
								cSet.GetFieldValue(_T("has_ds"), iHasDs) ;
								if (strArrayDevBlock.GetAt(1)==_T("DC")&&iHasDs>0)
								{
									strNumBiaoShiQi = _T("1") ; //�ڴ˱�ʾ������DS
								}
								
								strDevType.Format(_T("XHJ_%s_%s"), strArrayDevBlock.GetAt(2), strNumBiaoShiQi) ;
								cSet.Edit() ;
								cSet.SetFieldValue(_T("devtype"), strDevType) ;
								cSet.Update() ;
							}
						}
						else if (iCountDevArray==1)
						{
							if (strDevBlock==_T("AZ"))
							{
								strDevType.Format(_T("TF_%s"), strDevBlock) ;
								cSet.Edit() ;
								cSet.SetFieldValue(_T("devtype"), strDevType) ;
								cSet.Update() ;
							}
						}
						if (iCountBoxArray==4) //�շ�ϵ��
						{
							CString strBoxBlockPart2 = strArrayBoxBlock.GetAt(2) ;
							if (strBoxBlockPart2==_T("CZ-XH")||strBoxBlockPart2==_T("CZ-DY")||strBoxBlockPart2==_T("JSQ")||strBoxBlockPart2==_T("TB")||strBoxBlockPart2==_T("XBQ")||strBoxBlockPart2==_T("XJQ"))
							{
								strDevType.Format(_T("TF_%s"), strBoxBlockPart2) ;
								cSet.Edit() ;
								cSet.SetFieldValue(_T("devtype"), strDevType) ;
								cSet.Update() ;
							}
						}
					}
					else if (strBoxTypeTmp==_T("XB"))
					{
						CString strBoxBlockPart2 = strArrayBoxBlock.GetAt(2) ;
						if (strBoxBlockPart2==_T("CG-XH")||strBoxBlockPart2==_T("CG-DY"))
						{
							strDevType.Format(_T("TF_%s"), strBoxBlockPart2) ;
							cSet.Edit() ;
							cSet.SetFieldValue(_T("devtype"), strDevType) ;
							cSet.Update() ;
						}
						if (iCountDevArray>0&&strArrayDevBlock.GetAt(0)==_T("XHJ"))
						{
							CString strNumBiaoShiQi =  strArrayDevBlock.GetAt(4) ;
							int iHasDs = 0 ;
							cSet.GetFieldValue(_T("has_ds"), iHasDs) ;
							if (strArrayDevBlock.GetAt(1)==_T("DC")&&iHasDs>0)
							{
								strNumBiaoShiQi = _T("1") ; //�ڴ˱�ʾ������DS
							}
							if (!strDevName.IsEmpty()&&strDevName.Left(0)==_T("T")) //�շ��źŻ����伴ʹ�κʹ���һ�����ڲ����Ͳ�һ��
							{
								strDevType.Format(_T("XHJ_%s-TF_%s"), strArrayDevBlock.GetAt(2), strNumBiaoShiQi) ;
							}
							else
							{
								strDevType.Format(_T("XHJ_%s_%s"), strArrayDevBlock.GetAt(2), strNumBiaoShiQi) ;
							}
							cSet.Edit() ;
							cSet.SetFieldValue(_T("devtype"), strDevType) ;
							cSet.Update() ;
						}
					}
					else if (strBoxTypeTmp==_T("FSD"))
					{
						CString strDevType, strDevName ;
						cSet.GetFieldValue(_T("devname"), strDevName) ;
						CString strBoxBlockVal2 = strArrayBoxBlock.GetAt(2) ;
						CString strBoxBlockVal3 = strArrayBoxBlock.GetAt(3) ;

						if (strBoxBlockVal3==_T("00")) //�շ�
						{
							if (strBoxBlockVal2==_T("F"))
							{
								strBoxType = _T("XB1") ;
								strDevType = _T("TF_FD") ;
							}
							else if (strBoxBlockVal2==_T("F-F"))
							{
								strBoxType = _T("XB2") ;
								strDevType = _T("TF_FD") ;
							}
							else if (strBoxBlockVal2==_T("S")||strBoxBlockVal2==_T("S-S"))
							{
								strDevType = _T("TF_SD") ;
							}

							cSet.Edit() ;
							cSet.SetFieldValue(_T("devtype"), strDevType) ;
							if (strBoxType.Left(2)==_T("XB"))
							{
								cSet.SetFieldValue(_T("boxtype"), strBoxType) ;
							}
							cSet.Update() ;
						}
						else 
						{
							if (strBoxBlockVal2.Find(_T("-"))==-1)
							{
								//boxtype
								strBoxType = _T("XB1") ;

								//devtype
								if (strBoxBlockVal3==_T("0"))
								{
									//strDevType = _T("FSD_GD_0") ;
									strDevType.Format(_T("FSD_GD_%s"), strBoxBlockVal2) ;
								}
								else
								{
									//strDevType = _T("FSD_DM_0") ;
									strDevType.Format(_T("FSD_DM_%s"), strBoxBlockVal2) ;
								}

								//devname
								if (strBoxBlockVal2==_T("S"))
								{
									if (strDevName.Right(1)!=_T("J"))
									{
										strDevName.Append(_T("J")) ;
									}									
								}
							}
							else
							{
								strBoxType = _T("XB2") ;
								strDevType = _T("FSD_GD_0") ;

								if (strBoxBlockVal2.Find(_T('0'))!=-1)
								{
									if (strBoxBlockVal2.Find(_T('S'))!=-1)
									{
										if (strDevName.Right(1)!=_T("J"))
										{
											strDevName.Append(_T("J")) ;
										}	
									}
								}
								else
								{
									CStringArray strArrayTmp ;
									CBlkUtility::DivideString(strDevName, _T(" "), strArrayTmp) ;
									if (strArrayTmp.GetCount()==2)
									{
										CString strDev1 = strArrayTmp.GetAt(0) ;
										CString strDev2 = strArrayTmp.GetAt(1) ;
										if (strBoxBlockVal2.Left(1)==_T("S"))
										{
											if (strDev1.Right(1)!=_T("J"))
											{
												strDev1.Append(_T("J")) ;
											}												
										}
										if (strBoxBlockVal2.Right(1)==_T("S"))
										{
											if (strDev2.Right(1)!=_T("J"))
											{
												strDev2.Append(_T("J")) ;
											}	
										}
										strDevName.Format(_T("%s %s"), strDev1, strDev2) ;
									}
									else
									{
										strMsg.Format(_T("��ע��:�����·���ܵ�ͼ��%s(GDNAME=%s)"), strBoxBlock, strDevName) ;
									}
								}
							}
							cSet.Edit() ;
							cSet.SetFieldValue(_T("devname"), strDevName) ;
							cSet.SetFieldValue(_T("devtype"), strDevType) ;
							cSet.SetFieldValue(_T("boxtype"), strBoxType) ;
							cSet.Update() ;
						}
					}
// 					else if (strBoxTypeTmp==_T("LD"))
// 					{
// 						strDevType = _T("TF_LD") ;
// 						cSet.Edit() ;
// 						cSet.SetFieldValue(_T("devtype"), strDevType) ;
// 						cSet.Update() ;
// 					}
				}

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
		AfxMessageBox(_T("unknown error in ComputeDevAndBoxType")) ;
	}
}

// ����ʼ���ź�¥��������������Ŀ飬����handle
//��strBoxBlockToFind��Ϊ�գ���ֻ��������ΪstrBoxBlockToFind�Ŀ鼴��ֹ
void CCableNet::LoopConnectBox(long nHandle, CLongArray& nArrayHandle, const CString &strBoxBlockToFind)
{
	try
	{				
		CString strSql ;
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nHandle) ; 
		CADORecordset cSet(&m_AdoDb) ;
		if (cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				CString strBoxBlock ;
				cSet.GetFieldValue(_T("box_block"), strBoxBlock) ;
				if ((!strBoxBlockToFind.IsEmpty()&&strBoxBlockToFind.Compare(strBoxBlock)==0)||strBoxBlockToFind.IsEmpty())
				{
					nArrayHandle.Add(nHandle) ;
					long nHandleNext = 0 ;
					cSet.GetFieldValue(_T("next_ent_handle_low"), nHandleNext) ;
					if (nHandleNext!=0)
					{
						LoopConnectBox(nHandleNext, nArrayHandle, strBoxBlockToFind) ;
					}
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
		AfxMessageBox(_T("unknown error in LoopConnectBox")) ;
	}
}

// �ݹ������Ѱ��һ������ת�޻����
void CCableNet::LoopZzj(long nHandle, CString& strPreDcName, CLongArray& nArrayHandleZzj, CStringArray& strArrayZzjType, CStringArray& strArrayZzjIndex)
{
	CString strMsg ;
	try
	{				
		CString strSql ;
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nHandle) ; 
		CADORecordset cSet(&m_AdoDb) ;
		if (cSet.Open(strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				CString strZzjType, strZzjIndex, strDevName, strBoxBlock;
				cSet.GetFieldValue(_T("zzj_type"), strZzjType) ;
				cSet.GetFieldValue(_T("zzj_index"), strZzjIndex) ;
				cSet.GetFieldValue(_T("devname"), strDevName) ;
				cSet.GetFieldValue(_T("box_block"), strBoxBlock) ;
				if (strBoxBlock!=_T("BOX_HZ_ZZJ_0")) //������ת�޻���ֹͣ�˴�loop
				{
					//����һ��nArrayHandleZzj����洢��һ�����
					SetZzjDevType(nArrayHandleZzj, strArrayZzjType, strArrayZzjIndex) ;
				}
				else
				{
					if (!strPreDcName.IsEmpty()&&strPreDcName!=strDevName)
					{
						////����һ��nArrayHandleZzj����洢��һ�����
						SetZzjDevType(nArrayHandleZzj, strArrayZzjType, strArrayZzjIndex) ;

						nArrayHandleZzj.RemoveAll() ;
						strArrayZzjType.RemoveAll() ;
						strArrayZzjIndex.RemoveAll() ;
						strPreDcName = _T("") ;
						LoopZzj(nHandle, strPreDcName, nArrayHandleZzj, strArrayZzjType, strArrayZzjIndex) ;
					}
					else
					{
						nArrayHandleZzj.Add(nHandle) ;
						strArrayZzjType.Add(strZzjType) ;
						strArrayZzjIndex.Add(strZzjIndex) ;
						long nHandleNext = 0 ;
						cSet.GetFieldValue(_T("next_ent_handle_low"), nHandleNext) ;
						if (strZzjIndex.Left(1)==_T("1")) // 1/1����1/2......
						{
							strPreDcName = strDevName ;
						}
						LoopZzj(nHandleNext, strPreDcName, nArrayHandleZzj, strArrayZzjType, strArrayZzjIndex) ;
					}
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
		AfxMessageBox(_T("unknown error in LoopZzj")) ;
	}
}

// ����ת�޻����������
void CCableNet::SetZzjDevType(const CLongArray& nArrayHandle, CStringArray& strArrayZzjType, CStringArray& strArrayZzjIndex)
{
	int iCount = nArrayHandle.GetCount() ;
	if (iCount==0)
	{
		return ;
	}

	//�������ź�¥����Զ���루����ϵ�һ����Զ���ź�¥���豸��
	int iDistance = DistanceToXhl(nArrayHandle.GetAt(0)) ;

	CString strZzjCombination ; //ת�޻���ϣ���ZD6-D_ZD6-D
	CStringArray strArrayActAndDrag ; //�ڼ�����ǣ������1��A(1A)
	TCHAR chDrag = _T('A') ;

	for (int i=(iCount-1); i>=0; i--)
	{
		CString strZzjType = strArrayZzjType.GetAt(i) ;
		CString strZzjIndex = strArrayZzjIndex.GetAt(i) ;
		CString strActAndDrag ;
		if (i==(iCount-1))
		{
			strZzjCombination = strZzjType ;			
		}
		else
		{
			CString strPreZzjIndex = strArrayZzjIndex.GetAt(i+1) ;
			CString strConStr = _T("+") ;
			if (strPreZzjIndex!=strZzjIndex)
			{
				strConStr = _T("_") ;
				chDrag = _T('A') ;
			}
			else
			{
				chDrag++ ;
			}
			strZzjCombination.AppendFormat(_T("%s%s"), strConStr, strZzjType) ;
		}
		strActAndDrag.Format(_T("%s%c"), strZzjIndex.Left(1), chDrag)  ;
		strArrayActAndDrag.Add(strActAndDrag) ;
	}

	for (int i=(iCount-1); i>=0; i--)
	{
		CString strZzjDevType ;
		 strZzjDevType.Format(_T("%s--%s"), strZzjCombination, strArrayActAndDrag.GetAt(iCount-1-i)) ;
		 CString strSql ;
		 //�ȴӼ�о����ֱ�Ӳ���dev_for=strZzjDevType�������ź�¥�������iDistance�ļ�¼
		 strSql.Format(_T("select top 1 * from define_zzj_plus where dev_for='%s' and cable_len>%d"), strZzjDevType, iDistance) ;
		 long nId = 0 ;
		 m_RecordSet.GetRecordValue(strSql, _T("id"), nId) ;
		 if (nId==0) //δ�ҵ� ����ȥ�ֵ���� strZzjDevTypeƥ���
		 {
			 strSql.Format(_T("select  * from define_dict where dict_for='ZZJ_PLUS' and source='%s'"), strZzjDevType) ;
			 CString strDestination ;
			 m_RecordSet.GetRecordValue(strSql, _T("destination"), strDestination) ;
			 if (!strDestination.IsEmpty())
			 {
				 strSql.Format(_T("select top 1 * from define_zzj_plus where dev_for='%s' and cable_len>%d"), strDestination, iDistance) ;
				 m_RecordSet.GetRecordValue(strSql, _T("id"), nId) ;
			 }
		 }
		 strSql.Format(_T("update cablenet_ent set devtype='%s', zzj_plus_id=%d where handle_low=%d"), strZzjDevType, nId, nArrayHandle.GetAt(i)) ;
		 m_AdoDb.Execute(strSql) ;

		if (nId==0)
		{
			acutPrintf(_T("\nhandle=%d�� devtype=%s �ļ�¼δ�ҵ���о��¼"), nArrayHandle.GetAt(i), strZzjDevType) ;
		}
	}
}

// ���ź�¥�ľ���
int CCableNet::DistanceToXhl(long nHandle)
{
	int iDistance = 0 ;

	CLongArray nArrayHandle ;
	LoopConnectBox(nHandle, nArrayHandle, _T("")) ;
	CString strHandles ;
	CBlkUtility::ConvertArrayToString(nArrayHandle, _T(","),  strHandles) ;
	CString strSql ;
	strSql.Format(_T("select sum(left(cablestring,instr(cablestring,'-')-1)) as sumlen from cablenet_ent where handle_low in(%s) and cablestring<>'' "), strHandles) ;
	double dDist = 0 ;
	m_RecordSet.GetRecordValue(strSql, _T("sumlen"), dDist) ;
	iDistance = (int)dDist ;	

	return iDistance ;
}

// �����һ����������ĵ��¸�о�߷���
int CCableNet::ComputeCoreOfOneBox(long iBoxHandle)
{
	try
	{
		CString strSql ;
		CADORecordset cSet(&m_AdoDb) ;

		long nHandleBelongXhl = 0 ;
		CString strNameIntoXhl ;
		//begin ���������о�ߵı�� core_sn
		//////////////////////////////////////////////////////////////////////////
		//��ȡ���ӵ��µ��ַ���������35-12(4)
		CString strCableStr = _T("") ;
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), iBoxHandle) ;
		m_RecordSet.GetRecordValue(strSql, _T("cablestring"), strCableStr) ;

		m_RecordSet.GetRecordValue(strSql, _T("belong_xhl_handle_low"), nHandleBelongXhl) ;
		strSql.Format(_T("select * from cablenet_ent where handle_low=%d"), nHandleBelongXhl) ;
		m_RecordSet.GetRecordValue(strSql, _T("devname"), strNameIntoXhl) ;
		
		int iLocFind1 = strCableStr.Find(_T('-')) ;
		CString strCabeStrTmp1 ;
		if (iLocFind1!=-1)
		{
			strCabeStrTmp1 = strCableStr.Mid(iLocFind1+1) ;
		}
		int iLocFind2 = strCabeStrTmp1.Find(_T('(')) ;
		CString strCabeStrTmp2 ;
		if (iLocFind2!=-1)
		{
			strCabeStrTmp2 = strCabeStrTmp1.Left(iLocFind2) ;//����о��
		}
		strCabeStrTmp2.TrimRight(_T("L")) ; //��44L,ȥ��L
		//ȡ�õ��¶����id
		int iCableTypeId = -1 ;
		strSql.Format(_T("select id from define_cable_type where cable_name='%s'"), strCabeStrTmp2) ;
		m_RecordSet.GetRecordValue(strSql, _T("id"), iCableTypeId) ;

		//�Ƚ����ͺŵ��µ�����о��is_used����0							
		strSql.Format(_T("select * from define_core_detail where cable_type_id=%d"), iCableTypeId) ;
		m_RecordSet.SetRecordValue(strSql, _T("is_used,is_used_pair"), 0) ;

		//���ڲ�о�Ѿ�ռ�õ�о�߱����					
		CADORecordset cSetComputeCoreSn (&m_AdoDb) ;
		strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and is_inside=1"), iBoxHandle) ;
		if(cSetComputeCoreSn.Open(strSql))
		{	
			while (!cSetComputeCoreSn.IsEOF())
			{
				CString strCableCoreSn = _T("") ;

				cSetComputeCoreSn.GetFieldValue(_T("core_sn"), strCableCoreSn) ;
				//������ڲ�о�ߣ�����define_core_detail�����ҳ����о�߱��ʾΪ����о								
				strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and core_sn='%s'"), iCableTypeId, strCableCoreSn) ;
				m_RecordSet.SetRecordValue(strSql,_T("is_used"), 1) ;
				this->SetCorePairUsed(iCableTypeId, strCableCoreSn) ;

				cSetComputeCoreSn.MoveNext() ;
			}
			cSetComputeCoreSn.Close() ;
		}
		//�����ڲ�о����о��,�������ڲ�о����is_inside=2 
		//20130408������is_inside=1�Ĵ��ڲ���������Ĭ��core_snΪ�գ���Ҳ��Ҫ�������о�ߣ�����SH5����Ϊ���漰��о��������Ĭ��о��
		CString strNameIntoXhlTmp = strNameIntoXhl.Left(2) ;
		int iTypeDljl = -1 ;  //-1��ͨ�ã���ϣ���0������1���źŻ���2�������·��3�����뻯
		iTypeDljl = CZnUtility::GetJltType(strNameIntoXhlTmp) ;
// 		if (strNameIntoXhlTmp==_T("DC"))
// 		{
// 			iTypeDljl = 0 ;
// 		}
// 		else if (strNameIntoXhlTmp==_T("XH"))
// 		{
// 			iTypeDljl = 1 ;
// 		}
// 		else if (strNameIntoXhlTmp==_T("GD"))
// 		{
// 			iTypeDljl = 2 ;
// 		}
// 		else if (strNameIntoXhlTmp==_T("DM"))
// 		{
// 			iTypeDljl = 3 ;
// 		}
		this->ComputeCoreSnDetail(iBoxHandle, iCableTypeId, iTypeDljl) ;
		//end ���������о�ߵı�� core_sn
		//begin ���㱸��,20100406ע���ٶ�������������
		CString strBackupStr = _T("") ;					
		CADORecordset cSetBackup (&m_AdoDb) ;
		//��о
		strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='1'"), iCableTypeId) ;
		if(cSetBackup.Open(strSql))
		{
			int iDxRsCount = cSetBackup.GetRecordCount() ;
			if (iDxRsCount>0)
			{								
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

		//�Խʡ�4о�顢����4о��
		CString strCoreGroupType[3] = {_T("2"), _T("4"), _T("4P")} ; 
		for (int iCoreGroupTypeIndex=0; iCoreGroupTypeIndex<3; iCoreGroupTypeIndex++)
		{
			strSql.Format(_T("select distinct(left(core_sn,len(core_sn)-2)) as name, index_in_group from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='%s' order by index_in_group"), iCableTypeId, strCoreGroupType[iCoreGroupTypeIndex]) ;
			if(cSetBackup.Open(strSql))
			{
				while (!cSetBackup.IsEOF())
				{
					CString strName ;
					CString strBackupStrTmp = _T("") ;
					cSetBackup.GetFieldValue(_T("name"), strName) ;

					CADORecordset cSetBackupTmp (&m_AdoDb) ;
					strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and is_used=0 and core_group_type='%s' and left(core_sn,len(core_sn)-2)='%s'"),  iCableTypeId, strCoreGroupType[iCoreGroupTypeIndex],strName) ;
					if(cSetBackupTmp.Open(strSql) )
					{
						if (cSetBackupTmp.GetRecordCount()>0)
						{
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
								strBackupStr+=(iCoreGroupTypeIndex==0?_T("~2"):_T("~4")) ;
							}
						}
						cSetBackupTmp.Close() ;
					}

					cSetBackup.MoveNext() ;
				}
				cSetBackup.Close() ;
			}
		}
		strSql.Format(_T("update cablenet_ent set backup_string='%s' where handle_low=%d"), strBackupStr, iBoxHandle) ;
		m_AdoDb.Execute(strSql) ;
		//end ���㱸��

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeCoreOfOneBox")) ;
	}

	return 0;
}

// ��������ͼ������ͼ����һ����������ĵ��·���о�ߣ�i=3��ʾ���뻯��Ĭ����������
int CCableNet::ComputeCoreSnDetail(long nHandleBox, long iCableTypeId, int iType)
{
	try
	{
		CString strSql ;
		CADORecordset cSet (&m_AdoDb) ;

		if (iType==3) //���뻯Ҫ��ͬƵ��ͬ�£��ɶ�ʹ��
		{
			//�����ڲ�о����о��,�������ڲ�о����is_inside=2 
			//20130408������is_inside=1�Ĵ��ڲ���������Ĭ��core_snΪ�գ���Ҳ��Ҫ�������о�ߣ�����SH5����Ϊ���漰��о��������Ĭ��о��
			strSql.Format(_T("select dev_name, freq from cable_core_draw where belong_box_handle_low=%d and left(freq,2) in('17', '23') group by dev_name, freq order by Max(pt_y) desc"), nHandleBox) ;
			int iNumOddFreq = m_RecordSet.GetRecordCount(strSql) ; //����Ƶ�ʵĹ������
			strSql.Format(_T("select dev_name, freq from cable_core_draw where belong_box_handle_low=%d and left(freq,2) in('20', '26') group by dev_name, freq order by Max(pt_y) desc"), nHandleBox) ;
			int iNumEvenFreq = m_RecordSet.GetRecordCount(strSql) ; //ż��Ƶ�ʵĹ������
			if ((iNumOddFreq+iNumEvenFreq)==1||(iNumOddFreq==1&&iNumEvenFreq==1)) //1���1ż || 1��+1ż
			{
				strSql.Format(_T("select dev_name, freq from cable_core_draw where belong_box_handle_low=%d group by dev_name, freq order by Max(pt_y) desc"), nHandleBox) ;
				if (cSet.Open(strSql))
				{
					while (!cSet.IsEOF())
					{
						CString strDevName ;
						cSet.GetFieldValue(_T("dev_name"), strDevName) ;

						strSql.Format(_T("select top 2 * from define_core_detail where cable_type_id=%d and core_group_type in('4', '4P') and is_used_pair=0 and backup=0 order by id"), iCableTypeId) ;
						CStringArray strArrayCore ;
						int iCout = m_RecordSet.GetRecordValue(strSql, _T("core_sn"), strArrayCore) ;
						if (iCout==2)
						{
							m_RecordSet.SetRecordValue(strSql, _T("is_used,is_used_pair"), 1) ;
							for (int i=0; i<iCout; i++)
							{
								strSql.Format(_T("select * from cable_core_draw where dev_name='%s' and core_name in (%s) and belong_box_handle_low=%d"), strDevName, i==0?_T("'Q', 'F'"):_T("'QH', 'FH'"), nHandleBox) ;
								m_RecordSet.SetRecordValue(strSql, _T("core_sn"), strArrayCore.GetAt(i)) ;
							}
						}
						else
						{
							acutPrintf(_T("\n���%dΪ���%s��ѯcabletypeid=%d�ĵ���ʱ��δ�ҵ�2������Ҫ��Ŀ���о�ߣ�"), nHandleBox, strDevName, iCableTypeId) ;
						}

						cSet.MoveNext() ;

					}
					cSet.Close() ;
				}	
			}
			else//n��nż
			{
				strSql.Format(_T("select dev_name, freq from cable_core_draw where belong_box_handle_low=%d group by dev_name, freq order by Max(pt_y) desc"), nHandleBox) ;
				if (cSet.Open(strSql))
				{
					CLongArray nArrayIndexInGroup[2] ; //��¼��żռ���о��index_in_group,ֻҪ��ռ��ͬƵ�Ͳ�Ҫʹ�ô�4о����
					while (!cSet.IsEOF())
					{
						CString strDevName ;
						cSet.GetFieldValue(_T("dev_name"), strDevName) ;
						CString strFreq ;
						cSet.GetFieldValue(_T("freq"), strFreq) ;

						int iTmp = _ttoi(strFreq.Left(2)) ;
						CString strIndex ;
						CBlkUtility::ConvertArrayToString(nArrayIndexInGroup[iTmp%2], _T(","), strIndex) ;
						CString strTmp ;
						if (!strIndex.IsEmpty())
						{
							strTmp.Format(_T("and index_in_group not in(%s)"), strIndex) ;
						}					
						strSql.Format(_T("select top 2 * from define_core_detail where cable_type_id=%d and core_group_type in('4P') and is_used_pair=0 and backup=0 %s order by id"), iCableTypeId, strTmp) ;
						CStringArray strArrayCore ;
						int iCout = m_RecordSet.GetRecordValue(strSql, _T("core_sn"), strArrayCore) ;
						if (iCout==2)
						{
							long nIndex = 0 ;
							m_RecordSet.GetRecordValue(strSql, _T("index_in_group"), nIndex) ;
							m_RecordSet.SetRecordValue(strSql, _T("is_used,is_used_pair"), 1) ;
							if (CBlkUtility::FindLongInArray(nArrayIndexInGroup[iTmp%2], nIndex)==-1)
							{
								nArrayIndexInGroup[iTmp%2].Add(nIndex) ;
							}
							for (int i=0; i<iCout; i++)
							{
								strSql.Format(_T("select * from cable_core_draw where dev_name='%s' and core_name in (%s) and belong_box_handle_low=%d"), strDevName, i==0?_T("'Q', 'F'"):_T("'QH', 'FH'"), nHandleBox) ;
								m_RecordSet.SetRecordValue(strSql, _T("core_sn"), strArrayCore.GetAt(i)) ;
							}
						}
						else
						{
							acutPrintf(_T("\n���%dΪ���%s��ѯcabletypeid=%d�ĵ���ʱ��δ�ҵ�2������Ҫ��(4о��Ķ�о)�Ŀ���о�ߣ�"), nHandleBox, strDevName, iCableTypeId) ;
						}

						cSet.MoveNext() ;

					}
					cSet.Close() ;
				}	
			}
		}
		else //�ǵ��뻯�ģ��绰Ҫ�ɶ�ʹ��
		{
			strSql.Format(_T("select * from cable_core_draw where belong_box_handle_low=%d and is_virtual=0 and (is_inside<>1 or (is_inside=1 and core_sn='')) order by pt_y desc"), nHandleBox) ;
			if(cSet.Open(strSql))
			{		
				while (!cSet.IsEOF())
				{
					CString strDevName ;
					CString strCoreName ;
					cSet.GetFieldValue(_T("dev_name"), strDevName) ;
					cSet.GetFieldValue(_T("core_name"), strCoreName) ;
					if (strCoreName==_T("DH")) //
					{
						strSql.Format(_T("select top 2 * from define_core_detail where cable_type_id=%d and core_group_type in('2', '4', '4P') and is_used_pair=0 and backup=0 order by id"), iCableTypeId) ;
						CStringArray strArrayCore ;
						int iCout = m_RecordSet.GetRecordValue(strSql, _T("core_sn"), strArrayCore) ;
						if (iCout==2)
						{
							m_RecordSet.SetRecordValue(strSql, _T("is_used,is_used_pair"), 1) ;
							for (int i=0; i<iCout; i++)
							{
								strSql.Format(_T("select * from cable_core_draw where core_name in (%s) and belong_box_handle_low=%d"), i==0?_T("'DH'"):_T("'DHH'"), nHandleBox) ;
								m_RecordSet.SetRecordValue(strSql, _T("core_sn"), strArrayCore.GetAt(i)) ;
							}
						}
						else
						{
							acutPrintf(_T("\n���%dΪ�豸%s��ѯcabletypeid=%d�ĵ���ʱ��δ�ҵ�2������Ҫ��(�ɶ�)�Ŀ���о�ߣ�"), nHandleBox, strDevName, iCableTypeId) ;
						}
					}
					else if (strCoreName==_T("DHH"))
					{
						cSet.MoveNext() ;
						continue;
					}
					else
					{
						strSql.Format(_T("select top 1 * from define_core_detail where cable_type_id=%d  and is_used=0 and backup=0 order by id"), iCableTypeId) ;
						CString strCoreSn ;
						int iCout = m_RecordSet.GetRecordValue(strSql, _T("core_sn"), strCoreSn) ;
						if (iCout==1)
						{
							m_RecordSet.SetRecordValue(strSql, _T("is_used,is_used_pair"), 1) ;
							this->SetCorePairUsed(iCableTypeId, strCoreSn) ;

							strSql.Format(_T("select * from cable_core_draw where dev_name='%s' and core_name='%s' and belong_box_handle_low=%d"), strDevName, strCoreName, nHandleBox) ;
							m_RecordSet.SetRecordValue(strSql, _T("core_sn"), strCoreSn) ;
						}
						else
						{
							acutPrintf(_T("\n���%dΪ�豸%s��ѯcabletypeid=%d�ĵ���ʱ��δ�ҵ�1������Ҫ��Ŀ���о�ߣ�"), nHandleBox, strDevName, iCableTypeId) ;
						}
					}

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
		AfxMessageBox(_T("unknown error in ComputeCoreSnDetail")) ;
	}
	return 0;
}

// ����ָ��type_id��о����ΪstrCoreSn����ɶ�о��is_used_pair
int CCableNet::SetCorePairUsed(int iCableTypeId, const CString& strCoreSn)
{
	CString strSql ;
	if (strCoreSn.GetLength()>2)
	{
		CString strTmp = strCoreSn.Right(1) ;
		int iTmp =0  ;
		if (strTmp==_T("1"))
		{
			iTmp = 2 ;
		}
		else if (strTmp==_T("2"))
		{
			iTmp = 1 ;
		}
		else if (strTmp==_T("3"))
		{
			iTmp = 4 ;
		}
		else if (strTmp==_T("4"))
		{
			iTmp = 3 ;
		}

		strSql.Format(_T("select * from define_core_detail where cable_type_id=%d and core_sn in ('%s', '%s%d')"), iCableTypeId, strCoreSn, strCoreSn.Left(strCoreSn.GetLength()-1), iTmp) ;
		m_RecordSet.SetRecordValue(strSql, _T("is_used_pair"), 1) ;
	}

	return 0;
}
