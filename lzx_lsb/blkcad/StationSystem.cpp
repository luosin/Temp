// StationSystem.cpp: implementation of the CStationSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "StationSystem.h"
#include "DButility.h"
#include "YthCircuitData.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStationSystem::CStationSystem()
{

	CBlkUtility::GetBasePath(m_strSysBasePath) ;

}

CStationSystem::~CStationSystem()
{

}

int CStationSystem::ReadFxpBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;
	
	double dLayerHeight = 19.000 ;
	double dBoxHeight = 15.000 ;
	double dBoxWidth = 18.000 ;
	double dZHBoxWidth = 20.000 ;
	
	
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("fxp_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;

				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;

				if (strBlockName==_T("sbbzt_fxp"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strFxNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("FXNUM"), strFxNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bF[1-9]\\d*\\b"), strFxNum))//检查strFxNum是否是F1，F2的形式的字符串
					{
						iRet ++ ;

						if (!strFxNum.Mid(1).IsEmpty())
						{
							iShelfNum = _tstoi(strFxNum.Mid(1)) ;
						}

						CString strBarMsg ;
						strBarMsg.Format(_T("正在读取%s"), strFxNum ) ;
						acedSetStatusBarProgressMeter(strBarMsg, 0, 10) ;	

						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=0; iLayerIndex<11; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							pt_3d_cur_base.y= pt_3d_ref.y+(iLayerIndex*dLayerHeight) ;
							//读取防雷组合类型
							AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x-dZHBoxWidth, pt_3d_cur_base.y+dLayerHeight, 0) ;
							AcGePoint3d pt_3d_bottomright(pt_3d_cur_base) ;
							CArray<AcDbText *, AcDbText *> arrayZHType ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayZHType) ;
							mysys.SortEnt(arrayZHType, 2) ;
							CStringArray strArrayZHType ;
							mysys.GetHaveSortedText(arrayZHType, strArrayZHType) ;
							mysys.DestroyEntInArray(arrayZHType) ;

							if (strArrayZHType.GetSize()==0)
							{
								continue ;
							}
							CString strZHType1 = strArrayZHType.GetAt(0) ;

							if (strZHType1 !=_T("预留"))
							{
								if (strArrayZHType.GetSize()>0) //
								{
									int iZHType = -1 ;
									int iZHTypeInFact = -1 ;

									if (strZHType1==_T("道岔防雷组合"))
									{
										iZHType = 0 ;
									}
									else if (strZHType1==_T("信号机防雷组合"))
									{
										iZHType = 1 ;
									}
									else if (strZHType1==_T("LEU防雷组合"))
									{
										iZHType = 2 ;
									}
									else if (strZHType1==_T("送电端防雷组合"))
									{
										iZHType = 3 ;
									}
									else if (strZHType1==_T("受电端防雷组合"))
									{
										iZHType = 4 ;
									}
									else if (strZHType1.Find(_T("送"))!=-1&&strZHType1.Find(_T("受"))!=-1)
									{
										iZHType = 1000 ;
										//acutPrintf("\nshelf:%d-layer:%d", iShelfNum, iLayerIndex ) ;
									}
// 									else if (strArrayZHType.GetAt(0)==_T("缺口监测组合"))
// 									{
// 										iZHType = 5 ;
// 									}
// 									else if (strArrayZHType.GetAt(0)==_T("其它组合"))
// 									{
// 										iZHType = 6 ;
// 									}

									iZHTypeInFact = iZHType ;
									
									//循环找出每层的01-09格中所填的内容
									for (int iBoxIndex=1; iBoxIndex<10; iBoxIndex++)
									{
										pt_3d_topleft.x = pt_3d_cur_base.x+(iBoxIndex-1)*dBoxWidth ;
										pt_3d_topleft.y = pt_3d_cur_base.y+dBoxHeight ;
										pt_3d_bottomright.x = pt_3d_topleft.x+dBoxWidth ;
										pt_3d_bottomright.y = pt_3d_cur_base.y ;
										CArray<AcDbText *, AcDbText *> arrayCurFindDev ;
										mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayCurFindDev) ;
										mysys.SortEnt(arrayCurFindDev, 2) ;
										CStringArray strArrayCurFindDev ;
										mysys.GetHaveSortedText(arrayCurFindDev, strArrayCurFindDev) ;
										mysys.DestroyEntInArray(arrayCurFindDev) ;
										
										for (int iDevIndex=0; iDevIndex<strArrayCurFindDev.GetSize(); iDevIndex++)
										{
											CString strCurDev = strArrayCurFindDev.GetAt(iDevIndex) ;
											strCurDev.TrimLeft() ;
											strCurDev.TrimRight() ;
											if (iZHType==1000)//如果是送受电混合，则strCurDev应该是类似9DG(送电)或者D1G(受电)
											{
												if (strCurDev.Find(_T("送"))!=-1)
												{											
													iZHTypeInFact = 3 ;
												}
												else if (strCurDev.Find(_T("受"))!=-1)
												{											
													iZHTypeInFact = 4 ;
												}
												CString strTmp ;
												int iLocFind = strCurDev.Find(_T("(")) ;
												if (iLocFind!=-1)
												{
													strTmp = strCurDev.Left(iLocFind) ;
												}												
												strCurDev = strTmp ;
											}
											//数据入库
											try
											{
												strSql.Format(_T("insert into fxp_place(dev_name,shelf_num, layer_num, box_num, index_in_box, zuhe_type) \
																 values('%s',%d,%d,%d,%d,%d)"),\
																 strCurDev, iShelfNum, iLayerIndex, iBoxIndex, iDevIndex, iZHTypeInFact ) ;
												ExecSqlString(strSql) ;
											}
											catch (_com_error& e)
											{
												CString sBuff = CBlkUtility::GetErrorDescription(e) ;
												AfxMessageBox(sBuff) ;	
											}
											catch (...)
											{
												AfxMessageBox(_T("unknown error in ReadFxpBztToDB")) ;

											}
										} //end for iDevIndex
										
									} //end for iBoxIndex
								}
							} //end if strZHType1 !=_T("预留")						
						} //end for iLayerIndex	
						
						acedRestoreStatusBar() ;
					}// end ismatch				
					
				}//end if (strBlockName==_T("sbbzt_fxp"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

void CStationSystem::ComputeFxData()
{
	CUserSystem mysys ;
	CDBUtility::ClearTable(_T("fx_data")) ;

	try
	{
		CString strSql ;

		/*
		//begin 处理零层QK DS DH 暂时不处理
		CStringArray strArraySwitch ;
		int iSwitchNum = this->FindAllSwitch(strArraySwitch, 0) ; //得到所有道岔的名称(编号)
		CADORecordset cSetZeroLayer ;
		*/
		//CString strZeroLayerDev[4] = {_T("XQK"),_T("XDH")/*,_T("SQK"),_T("SDH"),_T("XDS"),_T("XDS-DH"),_T("SDS"),_T("SDS-DH")*/ } ;
	/*	CString strZeroLayerDevCore[4][2] = {
			{_T("QK"),_T("QKH")},
			{_T("DH"),_T("DHH")},
//			{_T("QK"),_T("QKH")},
//			{_T("DH"),_T("DHH")},
// 			{_T("DS1"),_T("DS2")},
// 			{_T("DH"),_T("DHH")},
// 			{_T("DS1"),_T("DS2")},			
// 			{_T("DH"),_T("DHH")}
		} ;
		for(int iZeroDevIndex=0; iZeroDevIndex<2; iZeroDevIndex++)
		{
			strSql.Format(_T("select * from fxp_place where layer_num=0 and dev_name='%s'"), strZeroLayerDev[iZeroDevIndex]) ;
			if (cSetZeroLayer.IsOpen())
			{
				cSetZeroLayer.Close() ;
			}
			cSetZeroLayer.Open(g_PtrCon, strSql) ;
			if (cSetZeroLayer.GetRecordCount()>0)
			{
				cSetZeroLayer.MoveFirst() ;

				int iShelfNum = 0 ;
				int iBoxNum = 0 ;
				cSetZeroLayer.GetFieldValue(_T("shelf_num"), iShelfNum) ;
				cSetZeroLayer.GetFieldValue(_T("box_num"), iBoxNum) ;
				for (int iSwitchIndex=0; iSwitchIndex<strArraySwitch.GetSize(); iSwitchIndex++)
				{
					for (int i=0; i<2; i++)
					{
						int iBoxNumInFact = iBoxNum+iSwitchIndex/9 ;
						int iCellNum = (iSwitchIndex%9)*2+i+1 ;

						strSql.Format(_T("insert into fx_data(dev_name, dev_type, core_name, shelf_num, layer_num, box_num, cell_num, type) \
							values('%s','%s','%s', %d, %d, %d, %d, %d)"), strArraySwitch.GetAt(iSwitchIndex), strZeroLayerDev[iZeroDevIndex], strZeroLayerDevCore[iZeroDevIndex][i], iShelfNum, 0, iBoxNumInFact, iCellNum, 4) ;
						try
						{
							ExecSqlString(strSql) ;
						}
						catch (_com_error& e)
						{
							CString sBuff = CBlkUtility::GetErrorDescription(e) ;
							AfxMessageBox(sBuff) ;	
						}
						catch (...)
						{
							AfxMessageBox("unknown error in zerolayer") ;							
						}

					}
				}

			}
			cSetZeroLayer.Close() ;

		}
		//end 处理零层QK DS DH
		*/

		CADORecordset cSetAll ;

		strSql.Format(_T("select * from fxp_place"));
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		cSetAll.Open(g_PtrCon, strSql) ;
		if (!cSetAll.IsBOF())
		{
			cSetAll.MoveFirst() ;
		}

		int iRsCount = cSetAll.GetRecordCount() ;
		int iIndexCurProcess = 0 ;
		CString strBarMsg ;
		strBarMsg.Format(_T("计算%d条分线数据"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
		while (!cSetAll.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iIndexCurProcess) ;

			int iId = -1 ;
			CString strDevName ;
			int iShelfNum = 0 ;
			int iLayerNum = 0 ;
			int iBoxNum = 0 ;
			int iIndexInBox = 0 ;
			int iZHType = -1 ;

			cSetAll.GetFieldValue(_T("id"), iId) ;
			cSetAll.GetFieldValue(_T("dev_name"), strDevName) ;
			cSetAll.GetFieldValue(_T("shelf_num"), iShelfNum) ;
			cSetAll.GetFieldValue(_T("layer_num"), iLayerNum) ;
			cSetAll.GetFieldValue(_T("box_num"), iBoxNum) ;
			cSetAll.GetFieldValue(_T("index_in_box"), iIndexInBox) ;
			cSetAll.GetFieldValue(_T("zuhe_type"), iZHType) ;
			//acutPrintf(_T("\nid:%d,devname:%s"), iId, strDevName) ;

// 			if (iId==184)
// 			{
// 				int iii = 0 ;
// 			}

			if (iLayerNum==0) //零层
			{

			}
			else if (iZHType==0) //道岔防雷组合
			{
				if (strDevName.Find(_T("-"))!=-1 ) //道岔防雷组合字符串形式必须是13-J1, 1/3-XL
				{
					CStringArray strArrayDevName ;
					CBlkUtility::DivideString(strDevName, _T("-"), strArrayDevName) ;

					CString strSwitchSN, strDevType ;
					assert(strArrayDevName.GetSize()>1) ;
					strSwitchSN = strArrayDevName.GetAt(0) ;
					strDevType = strArrayDevName.GetAt(1) ;
					if (strDevType==_T("XL"))
					{
						CStringArray strArraySwitchForXL ;
						if (strSwitchSN.Find(_T("/"))!=-1)
						{
							CBlkUtility::DivideString(strSwitchSN, _T("/"), strArraySwitchForXL) ;
						}
						else if (strSwitchSN.Find(_T(","))!=-1)
						{
							CBlkUtility::DivideString(strSwitchSN, _T(","), strArraySwitchForXL) ;
						}
						if (strArraySwitchForXL.GetSize()>1)
						{
							for (int iSwitchIndex=0; iSwitchIndex<strArraySwitchForXL.GetSize(); iSwitchIndex++)
							{
								CString strSwitchTmp = strArraySwitchForXL.GetAt(iSwitchIndex) ;
								if (strSwitchTmp==_T("3"))
								{
									int ii = 0 ;
								}
								this->ComputeOneDevFxp(strSwitchTmp, _T("XL"), iShelfNum, iLayerNum, iBoxNum, iSwitchIndex, 0) ;
							}
						}
						else
						{
							//this->ComputeOneDevFxp(strSwitchSN, _T("XL"), iShelfNum, iLayerNum, iBoxNum, 0, 0) ;//modified 20100710-2018

							this->ComputeOneDevFxp(strSwitchSN, _T("XL"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 0) ;
						}

					}
					else 
					{
						this->ComputeOneDevFxp(strSwitchSN, strDevType, iShelfNum, iLayerNum, iBoxNum, 0, 0) ;
					}
				}
				else if (strDevName==_T("XQK")||strDevName==_T("SQK")) //道岔缺口
				{
					this->ComputeOneDevFxp(strDevName, _T("QK"), iShelfNum, iLayerNum, iBoxNum, 0, 100) ;
				}
				else if (strDevName==_T("XDH")||strDevName==_T("SDH")) //电话
				{
					this->ComputeOneDevFxp(strDevName, _T("DH"), iShelfNum, iLayerNum, iBoxNum, 0, 101) ;
				}

			}
			else if (iZHType==1)//信号机防雷组合
			{
				CString strXhjType = this->GetXHJType(strDevName) ;

				if (strXhjType==_T("DS"))
				{
					this->ComputeOneDevFxp(strDevName, _T("DS"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 110) ;
				}
				else if (strXhjType==_T("DH"))
				{
					this->ComputeOneDevFxp(strDevName, _T("DH"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 111) ;
				}
				else if (strXhjType==_T("CZ"))//出站S3,SI,XII,S%%161 ,CBlkUtility::IsStrMatch("\\b[S,X]\\w*([1-9][0-9]*|I|II|%%[0-9]+)\\b"
				{
					this->ComputeOneDevFxp(strDevName, _T("CZ"), iShelfNum, iLayerNum, iBoxNum, 0, 1) ;
				}
				else if(strXhjType==_T("JZA"))//进站X XN S SN SD,CBlkUtility::IsStrMatch("\\b[S,X][A-Z]*\\b"
				{
					int iIdJz = -1 ;
					CADORecordset cSetFindJZ ;
					strSql.Format(_T("select * from fxp_place where dev_name='%s'"), strDevName) ;
					if(cSetFindJZ.IsOpen())
					{
						cSetFindJZ.Close() ;
					}
					cSetFindJZ.Open(g_PtrCon, strSql) ;
					if (cSetFindJZ.GetRecordCount()>0)
					{
						cSetFindJZ.MoveFirst() ;						
						cSetFindJZ.GetFieldValue(_T("id"), iIdJz) ;
					}
					cSetFindJZ.Close() ;

					if (iId==iIdJz)
					{
						//here 20100608-2316
						this->ComputeOneDevFxp(strDevName, _T("JZA"), iShelfNum, iLayerNum, iBoxNum, 0, 1) ;
					}
				}
				else if(strXhjType==_T("JLA"))//进路
				{
					int iIdJL = -1 ;
					CADORecordset cSetFindJL ;
					strSql.Format(_T("select * from fxp_place where dev_name='%s'"), strDevName) ;
					if(cSetFindJL.IsOpen())
					{
						cSetFindJL.Close() ;
					}
					cSetFindJL.Open(g_PtrCon, strSql) ;
					if (cSetFindJL.GetRecordCount()>0)
					{
						cSetFindJL.MoveFirst() ;						
						cSetFindJL.GetFieldValue(_T("id"), iIdJL) ;
					}
					cSetFindJL.Close() ;

					if (iId==iIdJL)
					{
						//here 20100608-2316
						this->ComputeOneDevFxp(strDevName, _T("JLA"), iShelfNum, iLayerNum, iBoxNum, 0, 1) ;
					}
				}
				else if (strXhjType==_T("DC")) //调车 D1,D2 CBlkUtility::IsStrMatch("\\bD[1-9][0-9]*\\b", strDevName)
				{
					this->ComputeOneDevFxp(strDevName, _T("DC"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 1) ;
				}
				else if (CBlkUtility::IsStrMatch(_T("\\bY[A-Z]*[0-9]*\\b"), strDevName)) //预告
				{
					this->ComputeOneDevFxp(strDevName, _T("YG"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 1) ;
				}
				

			}
			else if (iZHType==2) //LEU防雷组合
			{
				this->ComputeOneDevFxp(strDevName, _T("YD"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 2) ;
			}
			else if (iZHType==3) //送电端防雷组合
			{
				this->ComputeOneDevFxp(strDevName, _T("FS"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 3) ;
			}
			else if (iZHType==4) //受电端防雷组合
			{
				this->ComputeOneDevFxp(strDevName, _T("JS"), iShelfNum, iLayerNum, iBoxNum, iIndexInBox, 3) ;
			}

			iIndexCurProcess++ ;
			cSetAll.MoveNext() ;
		}
		cSetAll.Close() ;

		acedRestoreStatusBar() ;

		//begin 填组合端子
		//CADORecordset cSetZNode ;
		CString strDcNode[5] = {_T("05-1"),_T("05-2"),_T("05-3"),_T("05-4"),_T("05-5")} ;
		CString strDcNodeXL[2][3] = {
			{_T("01-5"),_T("01-6"),_T("01-7")},
			{_T("02-5"),_T("02-6"),_T("02-7")},
		};
		CString strCZXHJNode[6] = {_T("03-1"),_T("03-2"),_T("03-3"),_T("03-4"),_T("03-5"),_T("03-6")} ;
		CString strDCXHJNode[9] = {_T("03-1"),_T("03-2"),_T("03-3"),_T("04-1"),_T("04-2"),_T("04-3"),_T("05-1"),_T("05-2"),_T("05-3")} ;
		CString strJNode[6] = {_T("01-7"),_T("01-8"),_T("02-7"),_T("02-8"),_T("03-7"),_T("03-8")} ;

		CMapStringToString mapJzNormCoreToZNode,mapJzCoreToZNode, mapJzNCoreToZNode ;
		CMapStringToString mapJLNormCoreToZNode,mapJLCoreToZNode, mapJLNCoreToZNode ;
		CString strJzNormNode[8][2] = {
			{_T("1U"),_T("03-1")},
			{_T("L"),_T("03-2")},
			{_T("H"),_T("03-3")},
			{_T("2U"),_T("03-4")},
			{_T("YB"),_T("03-5")},
			{_T("LUH"),_T("03-6")},
			{_T("HH"),_T("03-7")},
			{_T("YBH"),_T("03-8")}
		} ;
		CString strJzNode[8][2] = {
			{_T("1U"),_T("02-2")},
			{_T("L"),_T("03-2")},
			{_T("H"),_T("03-3")},
			{_T("2U"),_T("03-4")},
			{_T("YB"),_T("03-5")},
			{_T("LUH"),_T("03-6")},
			{_T("HH"),_T("03-7")},
			{_T("YBH"),_T("03-8")}
		} ;
		CString strJzNNode[8][2] = {
			{_T("1U"),_T("04-2")},
			{_T("L"),_T("03-2")},
			{_T("H"),_T("03-3")},
			{_T("2U"),_T("03-4")},
			{_T("YB"),_T("03-5")},
			{_T("LUH"),_T("03-6")},
			{_T("HH"),_T("03-7")},
			{_T("YBH"),_T("03-8")}
		} ;

		CString strJLNormNode[9][2] = {
			{_T("1U"),_T("03-1")},
			{_T("L"),_T("03-2")},
			{_T("H"),_T("03-3")},
			{_T("2U"),_T("03-4")},
			{_T("B"),_T("03-5")},
			{_T("YB"),_T("03-6")},
			{_T("LUH"),_T("03-7")},
			{_T("HBH"),_T("03-8")},
			{_T("YBH"),_T("03-9")}
		} ;
		CString strJLNode[9][2] = {
			{_T("1U"),_T("02-2")},
			{_T("L"),_T("03-2")},
			{_T("H"),_T("03-3")},
			{_T("2U"),_T("03-4")},
			{_T("B"),_T("03-5")},
			{_T("YB"),_T("03-6")},
			{_T("LUH"),_T("03-7")},
			{_T("HBH"),_T("03-8")},
			{_T("YBH"),_T("03-9")}
		} ;
		CString strJLNNode[9][2] = {
			{_T("1U"),_T("04-2")},
			{_T("L"),_T("03-2")},
			{_T("H"),_T("03-3")},
			{_T("2U"),_T("03-4")},
			{_T("B"),_T("03-5")},
			{_T("YB"),_T("03-6")},
			{_T("LUH"),_T("03-7")},
			{_T("HBH"),_T("03-8")},
			{_T("YBH"),_T("03-9")}
		} ;
		for (int iJZNorm=0; iJZNorm<8; iJZNorm++)
		{
			mapJzNormCoreToZNode.SetAt(strJzNormNode[iJZNorm][0], strJzNormNode[iJZNorm][1]) ;
		}
		for (int iJZ=0; iJZ<8; iJZ++)
		{
			mapJzCoreToZNode.SetAt(strJzNode[iJZ][0], strJzNode[iJZ][1]) ;
		}
		for (int iJZN=0; iJZN<8; iJZN++)
		{
			mapJzNCoreToZNode.SetAt(strJzNNode[iJZN][0], strJzNNode[iJZN][1]) ;
		}
		for (int iJLNorm=0; iJLNorm<9; iJLNorm++)
		{
			mapJLNormCoreToZNode.SetAt(strJLNormNode[iJLNorm][0], strJLNormNode[iJLNorm][1]) ;
		}
		for (int iJL=0; iJL<9; iJL++)
		{
			mapJLCoreToZNode.SetAt(strJLNode[iJL][0], strJLNode[iJL][1]) ;
		}
		for (int iJLN=0; iJLN<9; iJLN++)
		{
			mapJLNCoreToZNode.SetAt(strJLNNode[iJLN][0], strJLNNode[iJLN][1]) ;
		}

		CADORecordset cSetZNode ;
		strSql.Format(_T("select * from fx_data")) ;
		if (cSetZNode.IsOpen())
		{
			cSetZNode.Close() ;
		}
		cSetZNode.Open(g_PtrCon, strSql) ;
		if (!cSetZNode.IsBOF())
		{
			cSetZNode.MoveFirst() ;
		}

		iRsCount = cSetZNode.GetRecordCount() ;
		iIndexCurProcess = 0 ;
		strBarMsg ;
		strBarMsg.Format(_T("计算%d条分线数据的组合/轨道位置"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

		while (!cSetZNode.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iIndexCurProcess) ;

			CString strZNode = _T("") ;

			CString strDevName, strDevType, strCoreName ;
			int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0, iCellNum = 0, iType = -1 ;
			cSetZNode.GetFieldValue(_T("dev_name"), strDevName) ;
			cSetZNode.GetFieldValue(_T("dev_type"), strDevType) ;
			cSetZNode.GetFieldValue(_T("core_name"), strCoreName) ;
			//cSetZNode.GetFieldValue(_T("shelf_num"), iShelfNum) ;
			cSetZNode.GetFieldValue(_T("layer_num"), iLayerNum) ;
			//cSetZNode.GetFieldValue(_T("box_num"), iBoxNum) ;
			cSetZNode.GetFieldValue(_T("cell_num"), iCellNum) ;
			cSetZNode.GetFieldValue(_T("type"), iType) ;

			CADORecordset cSetFindZ ;
			if (iType==0)//道岔
			{
				if (strDevType!=_T("XL"))
				{
					CString strDcDev ;
					strDcDev.Format(_T("%s-%s"), strDevName, strDevType) ;
				
					strSql.Format(_T("select * from z_place where dev_name='%s'"), strDcDev) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (cSetFindZ.GetRecordCount()>0)
					{
						cSetFindZ.MoveFirst() ;
						int iZShelf = 0, iZLayer = 0 ;
						cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf) ;
						cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer) ;
						strZNode.Format(_T("Z%d-%d%s"), iZShelf, iZLayer, strDcNode[iCellNum-1]) ;
					}
					cSetFindZ.Close() ;
				}
				else
				{				
					strSql.Format(_T("select * from z_place where dev_name='%s' and type='XLZ'"), strDevName) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (cSetFindZ.GetRecordCount()>0)
					{
						cSetFindZ.MoveFirst() ;
						int iZShelf = 0, iZLayer = 0, iIndexInBox = 0 ;
						cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf) ;
						cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer) ;
						cSetFindZ.GetFieldValue(_T("index_in_box"), iIndexInBox) ; //strDcNodeXL跟组合位置中index_in_box有关
						strZNode.Format(_T("Z%d-%d%s"), iZShelf, iZLayer, strDcNodeXL[iIndexInBox][(iCellNum-1)%3]) ;
					}
					cSetFindZ.Close() ;

				}
			}
			else if (iType==1)//信号机
			{
				if (strDevType==_T("JZA"))
				{
					int iZShelf = 0, iZLayer = 0 ;
					int iZShelf_USU = 0, iZLayer_USU = 0 ;
					int iIndexInBox = 0 ;
					BOOL bHaveUSUZ = FALSE ;
					strSql.Format(_T("select * from z_place where dev_name='%s'"), strDevName) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (!cSetFindZ.IsBOF())
					{
						cSetFindZ.MoveFirst() ;
					}
					int iRsCount = cSetFindZ.GetRecordCount() ;
					while (!cSetFindZ.IsEOF())
					{						
						CString strJzType = _T("") ;
						cSetFindZ.GetFieldValue(_T("type"), strJzType) ;

						if (strJzType==_T("JZZ"))
						{
							cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf) ;
							cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer) ;
						}
						else if (strJzType==_T("USUZ"))
						{
							cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf_USU) ;
							cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer_USU) ;
							cSetFindZ.GetFieldValue(_T("index_in_box"), iIndexInBox) ;
							bHaveUSUZ = TRUE ;
						}

						cSetFindZ.MoveNext() ;
					}
					cSetFindZ.Close() ;

					CString strNodeTmp = _T("") ;
					if (!bHaveUSUZ)
					{
						mapJzNormCoreToZNode.Lookup(strCoreName, strNodeTmp) ;						
					}
					else
					{
						if (strCoreName==_T("1U"))
						{
							iZShelf = iZShelf_USU ;
							iZLayer = iZLayer_USU ;
						}

						if (iIndexInBox==0)
						{
							mapJzCoreToZNode.Lookup(strCoreName, strNodeTmp) ;	
						}
						else if (iIndexInBox==1)
						{
							mapJzNCoreToZNode.Lookup(strCoreName, strNodeTmp) ;	
						}
					}
					strZNode.Format(_T("Z%d-%d%s"), iZShelf, iZLayer, strNodeTmp) ;
				}
				else if (strDevType==_T("JLA"))
				{
					int iZShelf = 0, iZLayer = 0 ;
					int iZShelf_USU = 0, iZLayer_USU = 0 ;
					int iIndexInBox = 0 ;
					BOOL bHaveUSUZ = FALSE ;
					strSql.Format(_T("select * from z_place where dev_name='%s'"), strDevName) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (!cSetFindZ.IsBOF())
					{
						cSetFindZ.MoveFirst() ;
					}
					int iRsCount = cSetFindZ.GetRecordCount() ;
					while (!cSetFindZ.IsEOF())
					{						
						CString strJzType = _T("") ;
						cSetFindZ.GetFieldValue(_T("type"), strJzType) ;
						
						if (strJzType==_T("JLZ"))
						{
							cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf) ;
							cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer) ;
						}
						else if (strJzType==_T("USUZ"))
						{
							cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf_USU) ;
							cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer_USU) ;
							cSetFindZ.GetFieldValue(_T("index_in_box"), iIndexInBox) ;
							bHaveUSUZ = TRUE ;
						}
						
						cSetFindZ.MoveNext() ;
					}
					cSetFindZ.Close() ;
					
					CString strNodeTmp = _T("") ;
					if (!bHaveUSUZ)
					{
						mapJLNormCoreToZNode.Lookup(strCoreName, strNodeTmp) ;						
					}
					else
					{
						if (strCoreName==_T("1U"))
						{
							iZShelf = iZShelf_USU ;
							iZLayer = iZLayer_USU ;
						}

						if (iIndexInBox==0)
						{
							mapJLCoreToZNode.Lookup(strCoreName, strNodeTmp) ;	
						}
						else if (iIndexInBox==1)
						{
							mapJLNCoreToZNode.Lookup(strCoreName, strNodeTmp) ;	
						}
					}
					strZNode.Format(_T("Z%d-%d%s"), iZShelf, iZLayer, strNodeTmp) ;
				}
				else if (strDevType==_T("CZ"))
				{
					strSql.Format(_T("select * from z_place where dev_name='%s'"), strDevName) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (cSetFindZ.GetRecordCount()>0)
					{
						cSetFindZ.MoveFirst() ;
						int iZShelf = 0, iZLayer = 0 ;
						cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf) ;
						cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer) ;
						strZNode.Format(_T("Z%d-%d%s"), iZShelf, iZLayer, strCZXHJNode[iCellNum-1]) ;
					}
					cSetFindZ.Close() ;
				}
				else if (strDevType==_T("DC"))
				{
					strSql.Format(_T("select * from z_place where dev_name='%s'"), strDevName) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (cSetFindZ.GetRecordCount()>0)
					{
						cSetFindZ.MoveFirst() ;
						
						int iZShelf = 0, iZLayer = 0, iIndexInBox = 0;
						
						cSetFindZ.GetFieldValue(_T("shelf_num"), iZShelf) ;
						cSetFindZ.GetFieldValue(_T("layer_num"), iZLayer) ;
						cSetFindZ.GetFieldValue(_T("index_in_box"), iIndexInBox) ;		
						
						strZNode.Format(_T("Z%d-%d%s"), iZShelf, iZLayer, strDCXHJNode[iIndexInBox*3+(iCellNum-1)%3]) ;
					}
					cSetFindZ.Close() ;
				}
			}
			else if (iType==3)//轨道送受电（只有受电）
			{
				if (strDevType==_T("JS"))
				{
					strSql.Format(_T("select * from g_place where gd_name='%s'"), strDevName) ;
					if (cSetFindZ.IsOpen())
					{
						cSetFindZ.Close() ;
					}
					cSetFindZ.Open(g_PtrCon, strSql) ;
					if (cSetFindZ.GetRecordCount()>0)
					{
						cSetFindZ.MoveFirst() ;
						int iGShelf = 0, iGLayer = 0 ;
						cSetFindZ.GetFieldValue(_T("shelf_num"), iGShelf) ;
						cSetFindZ.GetFieldValue(_T("layer_num"), iGLayer) ;
						strZNode.Format(_T("G%d-%d%s"), iGShelf, iGLayer, strJNode[iCellNum-1]) ;
					}
					cSetFindZ.Close() ;

				}
			}

			cSetZNode.Edit() ;
			cSetZNode.SetFieldValue(_T("z_node"), strZNode) ;
			cSetZNode.Update() ;

			iIndexCurProcess++ ;

			cSetZNode.MoveNext() ;
		}
		cSetZNode.Close() ;

		acedRestoreStatusBar() ;

		//end 填组合端子	
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

void CStationSystem::ComputeOneDevFxp(CString strDevName, CString strDevType, int iShelfNum, int iLayerNum, int iBoxNum, int iIndexInBox, int iType)
{
	try
	{
		CString strSql ;
		
		if ((iType/100)==1)
		{
			CString strCore[2] = {_T("Q"), _T("H")} ;

			if (iType==100||iType==101)
			{
				for (int i=0; i<2; i++)
				{
					strSql.Format(_T("insert into fx_data(dev_name, dev_type, core_name, shelf_num, layer_num, box_num, cell_num, type) \
					values('%s','%s','%s', %d, %d, %d, %d, %d)"), strDevName, strDevType, strCore[i], iShelfNum, iLayerNum, iBoxNum, i+1, iType) ;
					ExecSqlString(strSql) ;
				}
			}
			else if (iType==110)
			{
				CString strDsCore[2] = {_T("1"), _T("2")} ;
				for (int i=0; i<2; i++)
				{
					strSql.Format(_T("insert into fx_data(dev_name, dev_type, core_name, shelf_num, layer_num, box_num, cell_num, type) \
					values('%s','%s','%s', %d, %d, %d, %d, %d)"), strDevName, strDevType, strDsCore[i], iShelfNum, iLayerNum, iBoxNum, i+1, iType) ;
					ExecSqlString(strSql) ;
				}
			}
			else if (iType==111)
			{
				for (int i=0; i<2; i++)
				{
					strSql.Format(_T("insert into fx_data(dev_name, dev_type, core_name, shelf_num, layer_num, box_num, cell_num, type) \
					values('%s','%s','%s', %d, %d, %d, %d, %d)"), strDevName, strDevType, strCore[i], iShelfNum, iLayerNum, iBoxNum, i+5, iType) ;
					ExecSqlString(strSql) ;
				}
			}

			return ;
		}

		CADORecordset cSetAll ;
		
		//20120515：由于后期更改了define_dev表里内容，JZA类型扩充（不再仅是为京沪）,JZA for 京沪，变为JZA_5_0_0,并以dev_for=JH来限制，
		strSql.Format(_T("select * from define_dev_use_cablecore where is_share=0 and dev_id=(select id from define_dev where left(dev_type,len('%s'))='%s' and dev_for='JH')"), strDevType,strDevType);
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		cSetAll.Open(g_PtrCon, strSql) ;
		if (!cSetAll.IsBOF())
		{
			cSetAll.MoveFirst() ;
		}

		int iRsCount = cSetAll.GetRecordCount() ;
		int iStartIndex = iIndexInBox*iRsCount ;

		//int iIndex = 0 ;
		while (!cSetAll.IsEOF())
		{			
			CString strCoreName ;
			cSetAll.GetFieldValue(_T("core_name"), strCoreName) ;

			int iBoxNumInFact = ((iBoxNum + iStartIndex/6)-1)%9+1 ;//(iBoxNum+(iStartIndex==6?0:iStartIndex/6))%9 ;
			int iLayerNumInFact = iLayerNum + ((iBoxNum + iStartIndex/6)-1)/9 ;
			int iIndexInFact = iStartIndex%6+1 ;

			strSql.Format(_T("insert into fx_data(dev_name, dev_type, core_name, shelf_num, layer_num, box_num, cell_num, type) \
				values('%s','%s','%s', %d, %d, %d, %d, %d)"), strDevName, strDevType, strCoreName, iShelfNum, iLayerNumInFact, iBoxNumInFact, iIndexInFact, iType) ;
			try
			{
				ExecSqlString(strSql) ;
			}
			catch (_com_error& e)
			{
				CString sBuff = CBlkUtility::GetErrorDescription(e) ;
				AfxMessageBox(sBuff) ;	
			}
			catch (...)
			{
				AfxMessageBox(_T("unknown error in insert into fx_data")) ;				
			}

			iStartIndex++ ;

			cSetAll.MoveNext() ;
		}
		cSetAll.Close() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeOneDevFxp")) ;

	}


}


int CStationSystem::ReadZBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;

	double dLayerHeight = 14.5 ;
	double dLayerHeight1 = 5 ;
	double dBoxWidth = 18 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("z_place")) ;
	CDBUtility::ClearTable(_T("z_type")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;
				
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;

				strBlockName.MakeLower() ;
				if (strBlockName==_T("sbbzt_z1"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strZNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("ZNUM"), strZNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bZ[1-9]\\d*\\b"), strZNum))//检查strZNum是否是Z1，Z3的形式的字符串
					{
						iRet ++ ;

						if (!strZNum.Mid(1).IsEmpty())
						{
							iShelfNum = _tstoi(strZNum.Mid(1)) ;
						}	
						
						CString strBarMsg ;
						strBarMsg.Format(_T("读取组合排列表%s"), strZNum) ;
						acedSetStatusBarProgressMeter(strBarMsg, 1, 10) ;	
						
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=1; iLayerIndex<11; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							pt_3d_cur_base.y= pt_3d_ref.y+((iLayerIndex-1)*dLayerHeight) ;
							//读取组合类型
							AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y+dLayerHeight1, 0) ;
							AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dBoxWidth, pt_3d_cur_base.y, 0) ;
							
							CArray<AcDbText *, AcDbText *> arrayZType ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayZType) ;
							mysys.SortEnt(arrayZType, 2) ;
							CStringArray strArrayZType ;
							mysys.GetHaveSortedText(arrayZType, strArrayZType) ;
							mysys.DestroyEntInArray(arrayZType) ;
							
							if (strArrayZType.GetSize()==0)
							{
								continue ;
							}
							CString strZType1 = strArrayZType.GetAt(0) ;
							strZType1.TrimLeft() ;
							strZType1.TrimRight() ;
							
							if (strZType1 !=_T(""))
							{							
								//将该组合读入数据库
								BOOL bRetExec = FALSE ;
								try
								{
									strSql.Format(_T("insert into z_type(shelf_num, layer_num,type) \
									values(%d,%d,'%s')"),\
									iShelfNum, iLayerIndex, strZType1 ) ;
									bRetExec = ExecSqlString(strSql) ;
										
								}
								catch (_com_error& e)
								{
									CString sBuff = CBlkUtility::GetErrorDescription(e) ;
									AfxMessageBox(sBuff) ;	
								}
								catch (...)
								{
									AfxMessageBox(_T("unknown error in read 组合 z_type")) ;									
								}
								if (!bRetExec)//组合类型没插入成功则跳入下次循环，不插入此位置的设备
								{
									continue ;
								}

								CADORecordset cSetMaxId ;
								int iMaxId = -1 ;
								strSql.Format(_T("select max(id) as maxid from z_type")) ;
								if (cSetMaxId.IsOpen())
								{
									cSetMaxId.Close() ;
								}
								cSetMaxId.Open(g_PtrCon, strSql) ;
								if (cSetMaxId.GetRecordCount()>0)
								{
									cSetMaxId.MoveFirst() ;
									cSetMaxId.GetFieldValue(_T("maxid"), iMaxId) ;
								}
								cSetMaxId.Close() ;

								if (iMaxId==-1)
								{
									continue ;
								}

								pt_3d_bottomright.y+=dLayerHeight1 ;
								pt_3d_topleft.y = pt_3d_cur_base.y+dLayerHeight ;
								CArray<AcDbText *, AcDbText *> arrayDev ;
								mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayDev) ;
								mysys.SortEnt(arrayDev, 2) ;
								CStringArray strArrayDev ;
								mysys.GetHaveSortedText(arrayDev, strArrayDev) ;
								mysys.DestroyEntInArray(arrayDev) ;

								for (int iDevIndex=0; iDevIndex<strArrayDev.GetSize(); iDevIndex++)
								{
									int iIndexInBox = iDevIndex ;

									CString strDev = strArrayDev.GetAt(iDevIndex) ;
									strDev.TrimLeft() ;
									strDev.TrimRight() ;
									
									CStringArray strArrayDev2 ;
									if (strDev.Find(_T("/"))!=-1)
									{
										CBlkUtility::DivideString(strDev, _T("/"), strArrayDev2) ;
									}
									else if (strDev.Find(_T(","))!=-1)
									{
										CBlkUtility::DivideString(strDev, _T(","), strArrayDev2) ;
									}
									else 
									{
										strArrayDev2.Add(strDev) ;
									}
									for (int iDevIndex2=0;iDevIndex2<strArrayDev2.GetSize(); iDevIndex2++)
									{
										CString strDev2 = strArrayDev2.GetAt(iDevIndex2) ;
										strDev2.TrimLeft() ;
										strDev2.TrimRight() ;
										if (strArrayDev2.GetSize()>1)
										{
											iIndexInBox = iDevIndex2 ;
										}

										int iPlus1 = 0 ;
										CString strPlus1 = _T("") ;
										CBlkUtility::RegReplace(_T("^\\S*[^\\d]([1-9]\\d*)$"), _T("$1"), strDev2, strPlus1) ;
										iPlus1 = _tstoi(strPlus1) ;
										if (CBlkUtility::IsStrMatch(_T("^\\S*%%([1-9]\\d*)$"), strDev2))
										{
											iPlus1 = iPlus1-160 ;
										}
										//将组合位置上的设备名称写入数据库（此处存入的数据未经字符串解析，保留诸如“1，3”，"1/3"）
										try
										{
											strSql.Format(_T("insert into z_place(dev_name, shelf_num, layer_num, index_in_box, type, z_type_id,plus1) \
												values('%s',%d, %d, %d, '%s', %d, %d)"),\
												strDev2, iShelfNum, iLayerIndex, iIndexInBox, strZType1, iMaxId, iPlus1) ;
											ExecSqlString(strSql) ;
										}
										catch (_com_error& e)
										{
											CString sBuff = CBlkUtility::GetErrorDescription(e) ;
											AfxMessageBox(sBuff) ;	
										}
										catch (...)
										{
											AfxMessageBox(_T("unknown error in ReadZBztToDB")) ;

										}
									}
								}												
							} //end if strZHType1 !=_T("")						
						} //end for iLayerIndex	
						
						acedRestoreStatusBar() ;
					}// end ismatch					
					
				}//end if (strBlockName==_T("sbbzt_z1"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

//暂时舍弃该函数
int CStationSystem::FindAllSwitch(CStringArray &strArraySwitch, int iDownOrUp)
{
	int iRet = 0 ;

	try
	{
		CString	strSql ;

		CADORecordset cSetAllSwitch ;
		if (iDownOrUp==0)
		{
			strSql.Format(_T("select * from fxp_place where dev_name like '%%-J1' and id<(select id from fxp_place where dev_name='XQK' and layer_num>0) ")) ;
		}
		else if (iDownOrUp==1)
		{
			strSql.Format(_T("select * from fxp_place where dev_name like '%%-J1' and id>(select id from fxp_place where dev_name='XQK' and layer_num>0) ")) ;
		}	
		
		if(cSetAllSwitch.IsOpen())
		{
			cSetAllSwitch.Close() ;
		}
		cSetAllSwitch.Open(g_PtrCon, strSql) ;
		int iRsCount = cSetAllSwitch.GetRecordCount() ;
		acutPrintf(_T("\ncount %d"), iRsCount) ;
		if (iRsCount>0)
		{
			cSetAllSwitch.MoveFirst() ;
			while (!cSetAllSwitch.IsEOF())
			{
				CString strDev ;
				cSetAllSwitch.GetFieldValue(_T("dev_name"), strDev) ;
				CString strSwitch ;
				int iLocFind = strDev.Find(_T("-")) ;
				if (iLocFind!=-1)
				{
					strSwitch = strDev.Left(iLocFind) ;
				}				
				strArraySwitch.Add(strSwitch) ;
				iRet++ ;

				cSetAllSwitch.MoveNext() ;
			}

		}
		cSetAllSwitch.Close() ;
		
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FindAllSwitch")) ;

	}
	

	return iRet ;

}

int CStationSystem::ReadYBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;

	double dLayerHeight = 80 ;
	double dBoxWidth = 15 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("y_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;
				
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;

				strBlockName.MakeLower() ;
				if (strBlockName==_T("sbbzt_y1"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strYNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("YNUM"), strYNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bY[1-9]\\d*\\b"), strYNum))//检查strYNum是否是Y1，Y3的形式的字符串
					{
						iRet ++ ;

						if (!strYNum.Mid(1).IsEmpty())
						{
							iShelfNum = _tstoi(strYNum.Mid(1)) ;
						}	
						
						CString strBarMsg ;
						strBarMsg.Format(_T("读取移频柜%s"), strYNum) ;
						acedSetStatusBarProgressMeter(strBarMsg, 1, 10) ;	
						
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=1; iLayerIndex<11; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							pt_3d_cur_base.x = pt_3d_ref.x+((iLayerIndex-1)/2*dBoxWidth*2) ;
							pt_3d_cur_base.y = pt_3d_ref.y-((iLayerIndex-1)%2*dLayerHeight) ;
							//读取
							AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
							AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dBoxWidth, pt_3d_cur_base.y-dLayerHeight, 0) ;

							//here 20100616
							CLongArray nArrayBlkHandle ;
							mysys.FindBlkInArea(pt_3d_topleft, pt_3d_bottomright, nArrayBlkHandle) ;
							if (nArrayBlkHandle.GetSize()>0)
							{
								AcDbObjectId objID ;
								AcDbObject * pEntTmp = NULL ;
								AcDbHandle handl1(nArrayBlkHandle.GetAt(0),0) ;
								Acad::ErrorStatus es1 =  pCurDb->getAcDbObjectId(objID,1, handl1) ;
								if(Acad::eOk ==	acdbOpenAcDbObject(pEntTmp, objID, AcDb::kForRead) )
								{
									AcDbBlockReference *pBlkRef2 = NULL ;
									pBlkRef2 = (AcDbBlockReference*)pEntTmp ;
									
									//得到块名
// 									AcDbBlockTableRecord * pBlkTblRecTmp2 = NULL ;
// 									AcDbObjectId blkTblObjId ;
// 									blkTblObjId = pBlkRef2->blockTableRecord() ;
// 									acdbOpenObject(pBlkTblRecTmp2, blkTblObjId, AcDb::kForRead) ;
// 									TCHAR * chBlockName = NULL ;
// 									pBlkTblRecTmp2->getName(chBlockName) ;
// 									pBlkTblRecTmp2->close() ;
									
									CString strBlockName ;
									mysys.GetBlockName(pRef, strBlockName) ;

									if (strBlockName==_T("sbbzt_y2"))
									{
										CString strGDName , strFreq ;
										mysys.GetAttValue(pBlkRef2, _T("GDNAME"), strGDName) ;
										strGDName.TrimLeft() ;
										strGDName.TrimRight() ;
										mysys.GetAttValue(pBlkRef2, _T("FREQUENCY"), strFreq) ;
										strFreq.TrimLeft() ;
										strFreq.TrimRight() ;
										
										try
										{
										strSql.Format(_T("insert into y_place(gd_name, frequency, shelf_num, layer_num) \
											values('%s','%s', %d,%d)"),\
											strGDName, strFreq, iShelfNum, iLayerIndex ) ;
										ExecSqlString(strSql) ;
										
										}
										catch (_com_error& e)
										{
											CString sBuff = CBlkUtility::GetErrorDescription(e) ;
											AfxMessageBox(sBuff) ;	
										}
										catch (...)
										{
											AfxMessageBox(_T("unknown error in insert gd freq")) ;									
										}										
									}
									pEntTmp->close() ;
								}
							}
					
						} //end for iLayerIndex

						acedRestoreStatusBar() ;
					}// end ismatch					
					
				}//end if (strBlockName==_T("sbbzt_z1"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

//读取表中的内容是块的形式
int CStationSystem::ReadZHBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;

	double dLayerHeight = 19 ;
	double dLayerHeightTop = 4 ;
	double dLayerHeightDown = 5 ;
	double dBoxWidth = 18 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("zh_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;
				
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;
				
				strBlockName.MakeLower() ;
				if (strBlockName==_T("sbbzt_zh1"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strZHNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("ZHNUM"), strZHNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bZH[1-9]\\d*\\b"), strZHNum))//检查strZHNum是否是ZH1，ZH3的形式的字符串
					{
						iRet ++ ;

						if (!strZHNum.Mid(2).IsEmpty())
						{
							iShelfNum = _tstoi(strZHNum.Mid(2)) ;
						}	

						CString strBarMsg ;
						strBarMsg.Format(_T("读取综合柜%s"), strZHNum) ;
						acedSetStatusBarProgressMeter(strBarMsg, 1, 10) ;	
						
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=1; iLayerIndex<11; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							pt_3d_cur_base.y = pt_3d_ref.y+(iLayerIndex-1)*dLayerHeight ;

							for (int iBoxIndex=1; iBoxIndex<9; iBoxIndex++)
							{
								if (iBoxIndex%2==0)
								{
									continue ;
								}
								//读取
								AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x+(iBoxIndex-1)*dBoxWidth, pt_3d_cur_base.y+dLayerHeight-dLayerHeightTop, 0) ;
								AcGePoint3d pt_3d_bottomright(pt_3d_topleft.x+dBoxWidth*2, pt_3d_cur_base.y+dLayerHeightDown, 0) ;
								
								//here 20100616
								
								CLongArray nArrayBlkHandle ;
								mysys.FindBlkInArea(pt_3d_topleft, pt_3d_bottomright, nArrayBlkHandle) ;
								if (nArrayBlkHandle.GetSize()>0)
								{
									AcDbObjectId objID ;
									AcDbObject * pEntTmp = NULL ;
									AcDbHandle handl1(nArrayBlkHandle.GetAt(0),0) ;
									Acad::ErrorStatus es1 =  pCurDb->getAcDbObjectId(objID,1, handl1) ;
									if(Acad::eOk ==	acdbOpenAcDbObject(pEntTmp, objID, AcDb::kForRead) )
									{
										AcDbBlockReference *pBlkRef2 = NULL ;
										pBlkRef2 = (AcDbBlockReference*)pEntTmp ;
										
										//得到块名
// 										AcDbBlockTableRecord * pBlkTblRecTmp2 = NULL ;
// 										AcDbObjectId blkTblObjId ;
// 										blkTblObjId = pBlkRef2->blockTableRecord() ;
// 										acdbOpenObject(pBlkTblRecTmp2, blkTblObjId, AcDb::kForRead) ;
// 										TCHAR * chBlockName = NULL ;
// 										pBlkTblRecTmp2->getName(chBlockName) ;
// 										pBlkTblRecTmp2->close() ;
										
										CString strBlockName ;
										mysys.GetBlockName(pRef, strBlockName) ;
										
										if (strBlockName==_T("sbbzt_zh2"))
										{
											CString strGDName ;
											mysys.GetAttValue(pBlkRef2, _T("GDNAME"), strGDName) ;
											strGDName.TrimLeft() ;
											strGDName.TrimRight() ;
											
											try
											{
											strSql.Format(_T("insert into zh_place(gd_name, shelf_num, layer_num, box_num) \
												values('%s', %d, %d, %d)"),\
												strGDName, iShelfNum, iLayerIndex, iBoxIndex ) ;
											ExecSqlString(strSql) ;
											
											}
											catch (_com_error& e)
											{
												CString sBuff = CBlkUtility::GetErrorDescription(e) ;
												AfxMessageBox(sBuff) ;	
											}
											catch (...)
											{
												AfxMessageBox(_T("unknown error in insert gd to zh")) ;									
											}				
											
										}
										pEntTmp->close() ;
									}
								}
							}					
						} //end for iLayerIndex	
						
						acedRestoreStatusBar() ;
					}// end ismatch					
					
				}//end if (strBlockName==_T("sbbzt_zh1"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

int CStationSystem::ReadZGBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;

	double dLayerHeight = 4.5 ;
	double dBoxWidth = 18 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("z_gd_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名		
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;
				
				strBlockName.MakeLower() ;
				if (strBlockName==_T("sbbzt_z2"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strZLoc ;
					mysys.GetAttValue(pRef, _T("ZLOC"), strZLoc) ;
					if(CBlkUtility::IsStrMatch(_T("\\bZ[1-9]\\d*-[1-9]\\d*\\b"), strZLoc))//检查strZLoc是否是Z1-10 的形式的字符串
					{
						iRet ++ ;

						CString strBarMsg ;
						strBarMsg.Format(_T("读入轨道区段排列表%s"), strZLoc) ;

						acedSetStatusBarProgressMeter(strBarMsg, 1, 10) ;	
					
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iJIndex=1; iJIndex<11; iJIndex++)
						{
							acedSetStatusBarProgressMeterPos(iJIndex) ;

							pt_3d_cur_base.x = pt_3d_ref.x+(iJIndex-1)*dBoxWidth ;
							AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y+dLayerHeight, 0) ;
							AcGePoint3d pt_3d_bottomright(pt_3d_topleft.x+dBoxWidth, pt_3d_cur_base.y, 0) ;

							CArray<AcDbText *, AcDbText *> arrayGD ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayGD) ;
							mysys.SortEnt(arrayGD, 2) ;
							CStringArray strArrayGD ;
							mysys.GetHaveSortedText(arrayGD, strArrayGD) ;
							mysys.DestroyEntInArray(arrayGD) ;

							if (strArrayGD.GetSize()==0)
							{
								continue ;
							}
							CString strGd = strArrayGD.GetAt(0) ;
							strGd.TrimLeft() ;
							strGd.TrimRight() ;
							if (strGd!=_T(""))
							{
								try
								{
									int iShelfNum = 0, iLayerNum = 0 ;
									int iLocFind = strZLoc.Find(_T("-")) ;
									if (iLocFind!=-1)
									{
										iShelfNum = _tstoi(strZLoc.Mid(1, iLocFind-1)) ;
										iLayerNum = _tstoi(strZLoc.Mid(iLocFind+1)) ;

										strSql.Format(_T("insert into z_gd_place(gd_name, z_loc, j_loc, shelf_num, layer_num) \
														 values('%s', '%s', %d, %d,%d)"),\
														 strGd, strZLoc, iJIndex, iShelfNum, iLayerNum ) ;
										ExecSqlString(strSql) ;
									}								
								}
								catch (_com_error& e)
								{
									CString sBuff = CBlkUtility::GetErrorDescription(e) ;
									AfxMessageBox(sBuff) ;	
								}
								catch (...)
								{
									AfxMessageBox(_T("unknown error in insert gd to z_gd_place")) ;									
								}

							}
					
						} //end for iJIndex	
						
						acedRestoreStatusBar() ;

					}// end ismatch					
					
				}//end if (strBlockName==_T("sbbzt_z2"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

int CStationSystem::ReadGBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;
	
	double dLayerHeight = 29.000 ;
	double dBoxHeight = 5.000 ;
	double dBoxWidth1 = 9.000 ;
	double dBoxWidth2 = 44.000 ;
	
	
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("g_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;
				
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;
				
				if (strBlockName==_T("sbbzt_g1") || strBlockName==_T("sbbzt_g2"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strGNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("GNUM"), strGNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bG[1-9]\\d*\\b"), strGNum))//检查strGNum是否是G1，G2的形式的字符串
					{
						iRet ++ ;

						if (!strGNum.Mid(1).IsEmpty())
						{
							iShelfNum = _tstoi(strGNum.Mid(1)) ;
						}	
						
						CString strBarMsg ;
						strBarMsg.Format(_T("读取轨道柜%s"), strGNum) ;

						acedSetStatusBarProgressMeter(strBarMsg, 1, 9) ;	
						
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=1; iLayerIndex<10; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							if (iLayerIndex==5||(strBlockName==_T("sbbzt_g1")&&iLayerIndex==9))
							{
								continue ;
							}
							pt_3d_cur_base.y= pt_3d_ref.y+((iLayerIndex-1)*dLayerHeight) ;
							//读取
							AcGePoint3d pt_3d_topleft(0, 0, 0) ;
							AcGePoint3d pt_3d_bottomright(0, 0, 0) ;
							
							//循环找出每层的A-C格中所填的内容
							CString strBoxName[3] = {_T("A"),_T("B"),_T("C")} ;
							for (int iBoxIndex=0; iBoxIndex<3; iBoxIndex++)
							{
								pt_3d_topleft.x = pt_3d_cur_base.x+dBoxWidth1*4+iBoxIndex*dBoxWidth2 ;
								pt_3d_topleft.y = pt_3d_cur_base.y+dLayerHeight-dBoxHeight*2 ;
								pt_3d_bottomright.x = pt_3d_topleft.x+dBoxWidth2 ;
								pt_3d_bottomright.y = pt_3d_topleft.y-dBoxHeight ;
								CArray<AcDbText *, AcDbText *> arrayCurFindDev ;
								mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayCurFindDev) ;
								mysys.SortEnt(arrayCurFindDev, 2) ;
								CStringArray strArrayCurFindDev ;
								mysys.GetHaveSortedText(arrayCurFindDev, strArrayCurFindDev) ;
								mysys.DestroyEntInArray(arrayCurFindDev) ;
								
								if (strArrayCurFindDev.GetSize()>0)
								{
									CString strCurDev = strArrayCurFindDev.GetAt(0) ;
									strCurDev.TrimLeft() ;
									strCurDev.TrimRight() ;
									
									//数据入库
									try
									{
									strSql.Format(_T("insert into g_place(gd_name,shelf_num, layer_num, box_num) \
										values('%s',%d,%d,'%s')"),\
										strCurDev, iShelfNum, iLayerIndex, strBoxName[iBoxIndex] ) ;
									ExecSqlString(strSql) ;
									}
									catch (_com_error& e)
									{
										CString sBuff = CBlkUtility::GetErrorDescription(e) ;
										AfxMessageBox(sBuff) ;	
									}
									catch (...)
									{
										AfxMessageBox(_T("unknown error in ReadGBztToDB")) ;

									}
									
								}						
								
							} //end for iBoxIndex					
						} //end for iLayerIndex	
						
						acedRestoreStatusBar() ;

					}// end ismatch
					
					
				}//end if (strBlockName==_T("sbbzt_g1"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;

}

void CStationSystem::DrawFxpPx(AcDbDatabase *pCurDb)
{
	double dTableHeight = 297 ;
	double dTableWidth = 420 ;
	double dBetweenTableH = 37 ;
	double dBetweenTableV = 45 ;

	double dBoxHeight = 108 ;
	double dBoxWidth = 30 ;
	double dBetweenBoxH = 5 ;
	double dBetweenBoxV = 17 ;

	double dCellWidth = 25 ;
	double dCellHeight = 7 ;
	double dCellHeigthTitle = 8 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;

	try	
	{				
		CADORecordset cSetShelf ;
		CString strSql ;

		strSql.Format(_T("select distinct(shelf_num) from fx_data order by shelf_num asc")) ;
		if (cSetShelf.IsOpen())
		{
			cSetShelf.Close() ;
		}
		cSetShelf.Open(g_PtrCon, strSql) ;
		if (!cSetShelf.IsBOF())
		{
			cSetShelf.MoveFirst() ;
		}
		int iShelfSum = 0 ;

		int iRsCount = cSetShelf.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("绘制%d个分线柜的配线"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	

		while (!cSetShelf.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iShelfSum) ;

			int iShelfNum = 0 ;
			AcGePoint3d pt_3d_tb_base(0, 0-iShelfSum*(dTableHeight+dBetweenTableV), 0) ;
			cSetShelf.GetFieldValue(_T("shelf_num"), iShelfNum) ; //F1、F2...
			for (int iTbIndex=0; iTbIndex<5; iTbIndex++)//插入5个table
			{
				AcGePoint3d pt_3d_tb_insert(pt_3d_tb_base.x+(dTableWidth+dBetweenTableH)*iTbIndex, pt_3d_tb_base.y, 0) ;

				//begin 绘制属于该table的端子
				CADORecordset cSetPx ;
				strSql.Format(_T("select * from fx_data where shelf_num=%d and (layer_num=%d or layer_num=%d)"), iShelfNum, iTbIndex*2+1, iTbIndex*2+2) ;
				if (cSetPx.IsOpen())
				{
					cSetPx.Close() ;
				}
				cSetPx.Open(g_PtrCon, strSql) ;
				if (!cSetPx.IsBOF())
				{
					cSetPx.MoveFirst() ;
				}
				while (!cSetPx.IsEOF())
				{
					CString strDevName, strDevType, strCoreName, strZNode ;
					int iLayerNum = 0, iBoxNum = 0,  iCellNum = 0, iType = 0 ;

					cSetPx.GetFieldValue(_T("dev_name"), strDevName) ;
					cSetPx.GetFieldValue(_T("dev_type"), strDevType) ;
					cSetPx.GetFieldValue(_T("core_name"), strCoreName) ;
					cSetPx.GetFieldValue(_T("layer_num"), iLayerNum) ;
					cSetPx.GetFieldValue(_T("box_num"), iBoxNum) ;
					cSetPx.GetFieldValue(_T("cell_num"), iCellNum) ;
					cSetPx.GetFieldValue(_T("type"), iType) ;
					cSetPx.GetFieldValue(_T("z_node"), strZNode) ;

					CString strTxtToWrite ;
					if (iType==0)//道岔
					{
						strTxtToWrite.Format(_T("%s-%s-%s"), strDevName, strDevType, strCoreName) ;
					}
					else if (iType==1||iType==2||iType==111||iType==100||iType==101)//信号机 、应答器、DS-DH、QK、QKDH
					{
						strTxtToWrite.Format(_T("%s-%s"), strDevName, strCoreName) ;
					}
					else if (iType==3||iType==110)//轨道、DS
					{
						strTxtToWrite.Format(_T("%s%s"), strDevName, strCoreName) ;
					}
					ads_point pt_base = {0, 0, 0} ;
					
					//室外电缆					
					pt_base[0] = pt_3d_tb_insert.x+(iBoxNum-1)*(dBoxWidth+dBetweenBoxH)+dCellWidth/2 ;
					pt_base[1] = pt_3d_tb_insert.y-(iLayerNum%2)*(dBoxHeight+dBetweenBoxV)-(iCellNum-1)*dCellHeight*2-dCellHeight/2 ;
					//mysys.WriteText(pt_base, strTxtToWrite, 0, AcDb::kTextMid, 2.5, 0.8, clr) ;
					mysys.AddText(strTxtToWrite,pt_base,pt_base, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.8, 0, 1) ;
					//组合端子
					//pt_base[0] = pt_3d_tb_insert.x+(iBoxNum-1)*(dBoxWidth+dBetweenBoxH)+dCellWidth/2 ;
					pt_base[1] -= dCellHeight  ;					
					//mysys.WriteText(pt_base, strZNode, 0, AcDb::kTextMid, 2.5, 0.8, clr) ;
					mysys.AddText(strZNode,pt_base,pt_base, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.8, 0, 1) ;

					cSetPx.MoveNext() ;
				}
				cSetPx.Close() ;
				//end 绘制属于该table的端子
				

				//begin 绘制box名称 即在F1-1002 处放置的设备名称，比如道岔12
				for (int iBoxIndex=1; iBoxIndex<19; iBoxIndex++)
				{

					int iBoxIndexInFact = iBoxIndex%9==0?9:iBoxIndex%9 ;
					CString strBoxName = _T("") ;
					int iLayerNum = iTbIndex*2+(iBoxIndex>9?1:2) ;

					CADORecordset cSetBoxName ;
					strSql.Format(_T("select * from fxp_place where shelf_num=%d and layer_num=%d and box_num=%d"), iShelfNum, iLayerNum, iBoxIndexInFact) ;
					if (cSetBoxName.IsOpen())
					{
						cSetBoxName.Close() ;
					}
					cSetBoxName.Open(g_PtrCon, strSql) ;
					if (cSetBoxName.GetRecordCount()>0)
					{
						cSetBoxName.MoveFirst() ;
						
						while (!cSetBoxName.IsEOF())
						{
							CString strTmp ;
							cSetBoxName.GetFieldValue(_T("dev_name"), strTmp) ;
							if (strBoxName==_T(""))
							{
								strBoxName+=strTmp ;
							}
							else
							{
								strBoxName+=_T(",") ;
								strBoxName+=strTmp ;
							}
							cSetBoxName.MoveNext() ;
						}
					}
					cSetBoxName.Close() ;
					
					if (strBoxName != _T(""))
					{
						ads_point pt_base_name = {0, 0, 0} ;
						pt_base_name[0] = pt_3d_tb_insert.x+5+(iBoxIndexInFact-1)*(dBoxWidth+dBetweenBoxH)+(dCellWidth-5)/2 ;
						pt_base_name[1] = pt_3d_tb_insert.y+2.5*dCellHeigthTitle-(iBoxIndex>9?1:0)*(dBetweenBoxV+dBoxHeight) ;
						//mysys.WriteText(pt_base_name, strBoxName, 0, AcDb::kTextMid, 2, 0.7) ;
						mysys.AddText(strBoxName,pt_base_name,pt_base_name, AcDb::kTextMid, AcDb::kTextBase, 2, 0.7, 0, 0) ;
					}
					
				}

				//end 绘制box名称


				CStringArray strArrayAtt ;
				AcDbBlockReference * pRef = NULL ;
				CString strBlockName ;
				CString strF1, strF2, strDwgName, strDate ;

				strBlockName.Format(_T("pxt_fxp")) ;
				strF1.Format(_T("F%d-%d"), iShelfNum, iTbIndex*2+1 ) ;
				strF2.Format(_T("F%d-%d"), iShelfNum, iTbIndex*2+2 ) ;
				
				strDwgName.Format(_T("分线盘配线图-F%d(%s)"), iShelfNum, CBlkUtility::DigitToCapsString(iTbIndex+1)) ;
				
				strDate.Format(_T("%s"), CBlkUtility::GetDateStr()) ;
				strArrayAtt.Add(strF1) ;
				strArrayAtt.Add(strF2) ;
				strArrayAtt.Add(m_strStationName) ;
				strArrayAtt.Add(strDwgName) ;
				strArrayAtt.Add(m_strDwgNum) ;
				strArrayAtt.Add(strDate) ;

				if( mysys.InsertBlock(pRef, strBlockName, pt_3d_tb_insert, &strArrayAtt, FALSE, 0))
				{
					pRef->close() ;
				}

			}


			iShelfSum++ ;
			cSetShelf.MoveNext() ;
		}
		cSetShelf.Close() ;

		acedRestoreStatusBar() ;
	}		
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawFxpPx")) ;
	} 


}

//站内一体化轨道电路、移频轨道电路驱采
int CStationSystem::ComputeYthCircuit()
{
	int iRet = 0 ;

	double dDwgHeight = 297 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 5 ;
	double dCellWidth = 17 ;
	int iRowNum = 49 ;

	CString strCircuitTemplete  ;
	strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\GD_ZNYP_DRIVE.dwg"), m_strSysBasePath ) ;
	CString strQCNode_Type[5] = {_T("79"),_T("131"),_T("105"),_T("157"),_T("183")} ;

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		AcDbDatabase *pCurDb2 = new AcDbDatabase ;
		CUserSystem mysys2 ;
		mysys2.m_pDb = pCurDb2 ;		
		
		CADORecordset cSetAllGd ;
		CString strSql ;

		strSql.Format(_T("select * from z_gd_place where gd_name in (select gd_name from zh_place where box_num mod 2 =1) and j_loc mod 2 =1 order by shelf_num asc, layer_num asc, j_loc asc")) ;
		if (cSetAllGd.IsOpen())
		{
			cSetAllGd.Close() ;
		}
		cSetAllGd.Open(g_PtrCon, strSql) ;
		if (!cSetAllGd.IsBOF())
		{
			cSetAllGd.MoveFirst() ;
		}

		//////////////////////////////////////////////////////////////////////////
		int iRsCount = cSetAllGd.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("正在计算%d个移频轨道"), iRsCount) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		//////////////////////////////////////////////////////////////////////////
		
		while (!cSetAllGd.IsEOF())
		{
			//////////////////////////////////////////////////////////////////////////
			acedSetStatusBarProgressMeterPos(iRet) ;
			//////////////////////////////////////////////////////////////////////////

			CYthCircuitData circuitdata ;

			CMapStringToString mapCircuitTypeToStr ;
			CString strTccOutJKLoc = _T("") ;
			CString strTccInJKLoc_FJ13 = _T("") ;
			CString strTccInJKLoc_GJ12 = _T("") ;
			CString strLsInJKLoc_GJ21 = _T("") ;
			CString strLsInJKLoc_GJ22 = _T("") ;

			ads_point pt_base_draw = {0, 0, 0} ;
			ads_point pt_base_draw_tmp = {0, 0, 0} ;

			pt_base_draw[1] = 0-(iRet/iRowNum)*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight-dCellHeight/2;

			CString strGdName, strZNum, strYNum, strFreq, strZhNum, strSideGdName, strSideGdYNum, strSideGdFreq ;
			
			int iJNum = 0 ; //继电器index
			int iYShelfNum = 0, iYLayerNum = 0 ;//y
			int iSideYShelfNum = 0, iSideYLayerNum = 0 ; //并机y
			int iZhShelfNum = 0, iZhLayerNum = 0, iZhBoxNum = 0 ; //zh

			cSetAllGd.GetFieldValue(_T("gd_name"), strGdName) ;
			mapCircuitTypeToStr.SetAt(_T("27"), strGdName) ;

			cSetAllGd.GetFieldValue(_T("z_loc"), strZNum) ;
			mapCircuitTypeToStr.SetAt(_T("339"), strZNum) ;
			cSetAllGd.GetFieldValue(_T("j_loc"), iJNum) ;

			CADORecordset cSetFind ;

			strSql.Format(_T("select * from zh_place where gd_name='%s' and box_num mod 2 =1"), strGdName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;
					
					cSetFind.GetFieldValue(_T("shelf_num"), iZhShelfNum) ;
					cSetFind.GetFieldValue(_T("layer_num"), iZhLayerNum) ;
					cSetFind.GetFieldValue(_T("box_num"), iZhBoxNum) ;					
				}				
				cSetFind.Close() ;
			}

			strZhNum.Format(_T("ZH%d-%d"), iZhShelfNum, iZhLayerNum) ;

			strSql.Format(_T("select * from y_place where gd_name='%s'"), strGdName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;
		
					cSetFind.GetFieldValue(_T("frequency"), strFreq) ;
					cSetFind.GetFieldValue(_T("shelf_num"), iYShelfNum) ;
					cSetFind.GetFieldValue(_T("layer_num"), iYLayerNum) ;					
				}
				
				cSetFind.Close() ;
			}
			
			strYNum.Format(_T("Y%d-%d"), iYShelfNum, iYLayerNum) ;
			
			iSideYLayerNum = (iYLayerNum%2)==1?(iYLayerNum+1):(iYLayerNum-1) ;
			strSideGdYNum.Format(_T("Y%d-%d"), iYShelfNum, iSideYLayerNum) ;
			
			strSql.Format(_T("select * from y_place where shelf_num=%d and layer_num=%d"), iYShelfNum, iSideYLayerNum) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					cSetFind.MoveFirst() ;

					cSetFind.GetFieldValue(_T("gd_name"), strSideGdName) ;
					cSetFind.GetFieldValue(_T("frequency"), strSideGdFreq) ;					
				}
				
				cSetFind.Close() ;
			}
			
			pt_base_draw_tmp[0] = dCellWidth/2 ;
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strGdName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strGdName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			CString strZGZLoc = _T("") ;
			int iJNumTmp = (iJNum+1)/2 ;
			strZGZLoc.Format(_T("%s(%d)"), strZNum, iJNumTmp) ; //类似Z1-2(3)
			pt_base_draw_tmp[0] = dCellWidth+dCellWidth/2 ;
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strZGZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strZGZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			
			//begin 至TCC接口柜 out
			strSql.Format(_T("select * from traincontrol_outin_place where dev_name='%s_FJ' and type=0"), strGdName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				
				while (!cSetFind.IsEOF())
				{
					int iQcLayer = 0, iQcBoxNum = 0, iQcCellNum = 0 ;
					cSetFind.GetFieldValue(_T("layer_num"), iQcLayer) ;
					cSetFind.GetFieldValue(_T("box_num"), iQcBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iQcCellNum) ;
					
					strTccOutJKLoc.Format(_T("JK-%d0%d-%d"), iQcLayer, iQcBoxNum, iQcCellNum) ;		
					
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
			//电路图填充data
			mapCircuitTypeToStr.SetAt(_T("209"), strTccOutJKLoc) ;

			for (int iFJ1Index=0; iFJ1Index<5; iFJ1Index++)
			{
				pt_base_draw_tmp[0] = dCellWidth*(2+iFJ1Index)+dCellWidth/2 ;
				if (iFJ1Index==(iJNumTmp-1) && strTccOutJKLoc!=_T(""))
				{
					//mysys.WriteText(pt_base_draw_tmp, strTccOutJKLoc, 0, AcDb::kTextMid, 2.5, 0.65) ;
					mysys.AddText(strTccOutJKLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
				}
				else
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				
			}
			//end 至TCC接口柜 out

			//begin 至TCC接口柜 in
			strSql.Format(_T("select * from traincontrol_outin_place where dev_name like '%s_%%' and type=1"), strGdName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				
				while (!cSetFind.IsEOF())
				{
					int iQcLayer = 0, iQcBoxNum = 0, iQcCellNum = 0 ;
					CString strDevName = _T("") ;
					cSetFind.GetFieldValue(_T("layer_num"), iQcLayer) ;
					cSetFind.GetFieldValue(_T("box_num"), iQcBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iQcCellNum) ;
					cSetFind.GetFieldValue(_T("dev_name"), strDevName) ;					
					
					//电路图填充data
					//mapCircuitTypeToStr.SetAt(_T("417"), strTccInJKLoc) ;
					if (strDevName.Right(2)==_T("FJ"))
					{
						strTccInJKLoc_FJ13.Format(_T("JK-%d0%d-%d"), iQcLayer, iQcBoxNum, iQcCellNum) ;						
					}
					else if (strDevName.Right(2)==_T("GJ"))
					{						
						strTccInJKLoc_GJ12.Format(_T("JK-%d0%d-%d"), iQcLayer, iQcBoxNum, iQcCellNum) ;						
					}	
																
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
			mapCircuitTypeToStr.SetAt(_T("261"), strTccInJKLoc_FJ13) ;
			mapCircuitTypeToStr.SetAt(_T("235"), strTccInJKLoc_GJ12) ;
			for (int iFJ13Index=0; iFJ13Index<5; iFJ13Index++)
			{
				pt_base_draw_tmp[0] = dCellWidth*(7+iFJ13Index)+dCellWidth/2 ;
				
				if (iFJ13Index==(iJNumTmp-1) && strTccInJKLoc_FJ13!=_T(""))
				{
					//mysys.WriteText(pt_base_draw_tmp, strTccInJKLoc_FJ13, 0, AcDb::kTextMid, 2.5, 0.65) ;
					mysys.AddText(strTccInJKLoc_FJ13, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
				}
				else
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				
			}
			for (int iGJ12Index=0; iGJ12Index<5; iGJ12Index++)
			{
				pt_base_draw_tmp[0] = dCellWidth*(12+iGJ12Index)+dCellWidth/2 ;
				
				if (iGJ12Index==(iJNumTmp-1) && strTccInJKLoc_GJ12!=_T(""))
				{
					//mysys.WriteText(pt_base_draw_tmp, strTccInJKLoc_GJ12, 0, AcDb::kTextMid, 2.5, 0.65) ;
					mysys.AddText(strTccInJKLoc_GJ12, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;

				}
				else
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				
			}
			//end 至TCC接口柜 in

			//begin 至联锁接口柜 in
			CString strGdNameTmp = strGdName ; //若为IIG2则要去掉2
			int iGdPartIndex = 0 ;//若有分隔，此表示分隔的第几部分，比如IIG2中的2表示第二部分
			int iGdPartSum = 0 ; //有几个分隔部分
			if (CBlkUtility::IsStrMatch(_T("^.+G[1-9]$"), strGdName))
			{
				CBlkUtility::RegReplace(_T("^(.+G)([1-9])$"), _T("$1"), strGdName, strGdNameTmp) ;
				CString strGdPartIndex = _T("") ;
				CBlkUtility::RegReplace(_T("^(.+G)([1-9])$"), _T("$2"), strGdName, strGdPartIndex) ;
				iGdPartIndex = _tstoi(strGdPartIndex) ;

				//查询有几个分隔
				//////////////////////////////////////////////////////////////////////////	
				int iGdNameTmpLen = strGdNameTmp.GetLength() ;
				CADORecordset cSetFindTmp ;
				strSql.Format(_T("select * from y_place where left(gd_name,%d)='%s'"), iGdNameTmpLen, strGdNameTmp) ;
				if (cSetFindTmp.IsOpen())
				{
					cSetFindTmp.Close() ;
				}
				if (cSetFindTmp.Open(g_PtrCon, strSql))
				{
					iGdPartSum = cSetFindTmp.GetRecordCount() ;	
					cSetFindTmp.Close() ;
				}
				//////////////////////////////////////////////////////////////////////////
			}

			
			strSql.Format(_T("select * from interlock_outin_place where dev_name='%s' and type=1"), strGdNameTmp) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				
				while (!cSetFind.IsEOF())
				{
					int iQcLayer = 0, iQcBoxNum = 0, iQcCellNum = 0 ;
					CString strDevName = _T("") ;
					cSetFind.GetFieldValue(_T("layer_num"), iQcLayer) ;
					cSetFind.GetFieldValue(_T("box_num"), iQcBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iQcCellNum) ;
					cSetFind.GetFieldValue(_T("dev_name"), strDevName) ;
					if (iGdPartIndex==0)
					{
						strLsInJKLoc_GJ21 = _T("IOZ") ;
						strLsInJKLoc_GJ22.Format(_T("JK-%d0%d-%d"), iQcLayer, iQcBoxNum, iQcCellNum) ;
					}
					else if(iGdPartIndex>0)
					{
						if (iGdPartIndex==1)
						{
							strLsInJKLoc_GJ21 = _T("IOZ") ;
						}
						else
						{
							//////////////////////////////////////////////////////////////////////////						
							CADORecordset cSetFindTmp ;
							strSql.Format(_T("select * from node_collect where dwg_type='YPGD-QC' and name='GJ' and loc='22' and index=%d"), iJNumTmp-1) ;
							if (cSetFindTmp.IsOpen())
							{
								cSetFindTmp.Close() ;
							}
							if (cSetFindTmp.Open(g_PtrCon, strSql))
							{
								if (!cSetFindTmp.IsBOF())
								{
									cSetFindTmp.MoveFirst() ;
								}
								while (!cSetFindTmp.IsEOF())
								{					
									CString strNode = _T("") ;
									cSetFindTmp.GetFieldValue(_T("node"), strNode) ;
									strLsInJKLoc_GJ21.Format(_T("%s%s"), strZNum, strNode) ;								
									cSetFindTmp.MoveNext() ;
								}
								cSetFindTmp.Close() ;
							}
							//////////////////////////////////////////////////////////////////////////
						}
						if (iGdPartIndex==iGdPartSum)
						{
							strLsInJKLoc_GJ22.Format(_T("JK-%d0%d-%d"), iQcLayer, iQcBoxNum, iQcCellNum) ;
						}
						else
						{
							//////////////////////////////////////////////////////////////////////////						
							CADORecordset cSetFindTmp ;
							strSql.Format(_T("select * from node_collect where dwg_type='YPGD-QC' and name='GJ' and loc='21' and index=%d"), iJNumTmp+1) ;
							if (cSetFindTmp.IsOpen())
							{
								cSetFindTmp.Close() ;
							}
							if (cSetFindTmp.Open(g_PtrCon, strSql))
							{
								if (!cSetFindTmp.IsBOF())
								{
									cSetFindTmp.MoveFirst() ;
								}
								while (!cSetFindTmp.IsEOF())
								{					
									CString strNode = _T("") ;
									cSetFindTmp.GetFieldValue(_T("node"), strNode) ;
									strLsInJKLoc_GJ22.Format(_T("%s%s"), strZNum, strNode) ;								
									cSetFindTmp.MoveNext() ;
								}
								cSetFindTmp.Close() ;
							}
							//////////////////////////////////////////////////////////////////////////
						}		

					}
					
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
			//电路图填充data
			mapCircuitTypeToStr.SetAt(_T("287"), strLsInJKLoc_GJ21) ;
			mapCircuitTypeToStr.SetAt(_T("313"), strLsInJKLoc_GJ22) ;

			for (int iLSGJIndex=0; iLSGJIndex<5; iLSGJIndex++)
			{				
				pt_base_draw_tmp[0] = dCellWidth*(17+iLSGJIndex*2)+dCellWidth/2 ;		
				
				if (iLSGJIndex==(iJNumTmp-1) && strLsInJKLoc_GJ22!=_T(""))
				{
					//mysys.WriteText(pt_base_draw_tmp, strLsInJKLoc_GJ21, 0, AcDb::kTextMid, 2.5, 0.65) ;
					mysys.AddText(strLsInJKLoc_GJ21,pt_base_draw_tmp,pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0) ;
					pt_base_draw_tmp[0]+=dCellWidth ;
					//mysys.WriteText(pt_base_draw_tmp, strLsInJKLoc_GJ22, 0, AcDb::kTextMid, 2.5, 0.65) ;
					mysys.AddText(strLsInJKLoc_GJ22,pt_base_draw_tmp,pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0) ;

				}
				else
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
					pt_3d_line_start.x+=dCellWidth ;
					pt_3d_line_end.x+=dCellWidth ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				
			}

			//end 至联锁接口柜 in

			//////////////////////////////////////////////////////////////////////////
			//配线电路图上的端子
			strSql.Format(_T("select * from node_collect where dwg_type='YPGD-QC' and index=%d"), iJNumTmp) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				int iNodeIndex = 0 ;
				while (!cSetFind.IsEOF())
				{					
					CString strNode = _T("") ;
					cSetFind.GetFieldValue(_T("node"), strNode) ;					
					//电路图填充data
					mapCircuitTypeToStr.SetAt(strQCNode_Type[iNodeIndex], strNode) ;	
					iNodeIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;
			}
			//////////////////////////////////////////////////////////////////////////

			//插入配线用电路图
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iRet*dDwgHeight, 0); 
			xform.setToTranslation(vec); 					
			this->InsertDwgToDb(strCircuitTemplete, pCurDb2, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iRet*100) ;

			int iFsJsStart2 = 10 ;//下辛店专用3
			int iFsJsStart1 = 1 ; //下辛店专用-1
			circuitdata.m_strFsJsStartNode2.Format(_T("ZH%d-D%d-%d"), iZhShelfNum, iFsJsStart2+(9-iZhLayerNum), (iZhBoxNum/2)*4+1) ;
			circuitdata.m_strFsJsStartNode1.Format(_T("ZH%d-D%d-%d"), iZhShelfNum, iFsJsStart1+(9-iZhLayerNum), (iZhBoxNum/2)*4+1);
			circuitdata.m_strFJNodePre.Format(_T("0%d"), iJNum/2+1);
			circuitdata.m_strGJNodePre.Format(_T("0%d"), iJNum/2+1);
			circuitdata.m_strBackGName.Format(_T(""));
			circuitdata.m_strFrontGName.Format(_T(""));
			circuitdata.m_iSideRNode = (iSideYLayerNum-1)*2+1;
			circuitdata.m_iSideRName = (iSideYLayerNum-1)*3+3;
			circuitdata.m_iSideFsJsNum = iSideYLayerNum;
			circuitdata.m_strSideYNum = strSideGdYNum;
			circuitdata.m_strSideGName = strSideGdName;
			circuitdata.m_iCurRNode = (iYLayerNum-1)*2+1;
			circuitdata.m_iCurRName = (iYLayerNum-1)*3+1;
			circuitdata.m_strCurZhNum = strZhNum;
			circuitdata.m_strCurZNum = strZNum;
			circuitdata.m_iCurFsJsNum = iYLayerNum;
			circuitdata.m_strCurFreq = strFreq;
			circuitdata.m_strCurYNum = strYNum;
			circuitdata.m_strCurGName = strGdName;
			circuitdata.m_strStationName = this->m_strStationName ;
			circuitdata.m_strCircuitType.Format(_T("站内移频轨道电路图(%s)"), strGdName) ;
			circuitdata.m_strSN = this->m_strDwgNum ;
			circuitdata.m_strDate = CBlkUtility::GetDateStr() ;			
			
			this->DrawCircuit(&circuitdata) ;	

			iRet++ ;
			cSetAllGd.MoveNext() ;
		}
		cSetAllGd.Close() ;

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = iRsCount/iRowNum+(iRsCount%iRowNum==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("站内移频轨道电路驱动和采集电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_yth_qc.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
		}
		//end 绘制图框（包括电路示意图，表格等）
		
		
		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_GD_YTHQC")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\站内移频轨道电路图驱动采集.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
		delete pCurDb ;
		pCurDb = NULL ;
		
		mysys2.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
		mysys2.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		strSaveFilePath.Format(_T("%s\\配线专用-站内移频轨道电路驱动采集图.dwg"), this->m_strSavePath) ;
		pCurDb2->saveAs(strSaveFilePath) ;
		delete pCurDb2 ;
		pCurDb2 = NULL ;	

		acedRestoreStatusBar() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeYthCircuit")) ;
	} 


	return iRet ;
}

void CStationSystem::DrawCircuit(CYthCircuitData *circuitdata)
{
	CUserSystem mysys ;
	
	CString strTmpleteFile ;
	strTmpleteFile.Format(_T("%s\\support\\dlt\\dlt_yth.dwg"), m_strSysBasePath) ;
	AcDbDatabase *pCurDb = new AcDbDatabase(false) ;
	mysys.m_pDb = pCurDb ;

	if (Acad::eOk==pCurDb->readDwgFile(strTmpleteFile))
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
			if(Acad::eOk == pBlkTblRecIter->getEntity( pEnt ,AcDb::kForWrite ) )
			{
				AcDbText * pText = NULL ;
				if (_tcscmp ( pEnt->isA()->name() , _T("AcDbText") ) == 0 )
				{
					pText = (AcDbText*)pEnt ;
					int iType = -1 ;
					int iIndex = -1 ;
					if (mysys.GetXdata(pEnt, _T("TYPE"), iType))
					{
						CString strTmp ;
						switch (iType)
						{
						case 2:
							{
								pText->setTextString(circuitdata->m_strCurGName) ;
							}
							break;
						case 3:
							{
								pText->setTextString(circuitdata->m_strCurFreq) ;
							}
							break ;
						case 4:
							{
								strTmp.Format(_T("%d"), circuitdata->m_iCurFsJsNum) ;
								pText->setTextString(strTmp) ;
							}
							break ;
						case 5:
							{
								pText->setTextString(circuitdata->m_strCurZNum) ;
							}
							break ;
						case 6:
							{
								pText->setTextString(circuitdata->m_strCurYNum) ;
							}
							break ;
						case 7:
							{
								pText->setTextString(circuitdata->m_strCurZhNum) ;
							}
							break ;
						case 8:
							{
								if (mysys.GetXdata(pEnt, _T("INDEX"), iIndex))
								{
									CStringArray strArrayTmp ;
									CBlkUtility::DivideString(circuitdata->m_strFsJsStartNode1, _T("-"), strArrayTmp) ;
									if (strArrayTmp.GetSize()==3)
									{
										strTmp.Format(_T("%s-%s-%d"), strArrayTmp.GetAt(0), strArrayTmp.GetAt(1), _tstoi(strArrayTmp.GetAt(2))+iIndex-1) ;
										pText->setTextString(strTmp) ;
									}
								}
							}
							break ;
						case 9:
							{
								if (mysys.GetXdata(pEnt, _T("INDEX"), iIndex))
								{
									CStringArray strArrayTmp ;
									CBlkUtility::DivideString(circuitdata->m_strFsJsStartNode2, _T("-"), strArrayTmp) ;
									if (strArrayTmp.GetSize()==3)
									{
										strTmp.Format(_T("%s-%s-%d"), strArrayTmp.GetAt(0), strArrayTmp.GetAt(1), _tstoi(strArrayTmp.GetAt(2))+iIndex-1) ;
										pText->setTextString(strTmp) ;
									}
								}
							}
							break ;
						case 10:
							{
								pText->setTextString(circuitdata->m_strSideGName) ;
							}
							break ;
						case 11:
							{
								pText->setTextString(circuitdata->m_strSideYNum) ;
							}
							break ;
						case 12:
							{
								strTmp.Format(_T("%d"), circuitdata->m_iSideFsJsNum) ;
								pText->setTextString(strTmp) ;
							}
							break ;
						case 17:
							{
								if (mysys.GetXdata(pEnt, _T("INDEX"), iIndex))
								{
									strTmp.Format(_T("RD%d"), circuitdata->m_iCurRName+iIndex-1) ;
									pText->setTextString(strTmp) ;								
								}
							}
							break ;
						case 18:
							{
								if (mysys.GetXdata(pEnt, _T("INDEX"), iIndex))
								{
									strTmp.Format(_T("D%d"), circuitdata->m_iCurRNode+iIndex-1) ;
									pText->setTextString(strTmp) ;								
								}
							}
							break ;
						case 19:
							{
								strTmp.Format(_T("RD%d"), circuitdata->m_iSideRName) ;
								pText->setTextString(strTmp) ;	
							}
							break ;
						case 20:
							{
								if (mysys.GetXdata(pEnt, _T("INDEX"), iIndex))
								{
									strTmp.Format(_T("D%d"), circuitdata->m_iSideRNode+iIndex-1) ;
									pText->setTextString(strTmp) ;								
								}
							}
							break ;
						case 21:
						case 22:
							{
								if (mysys.GetXdata(pEnt, _T("INDEX"), iIndex))
								{
									CString strTxtTmp = pText->textString() ;
									CStringArray strArrayTmp ;
									CBlkUtility::DivideString(strTxtTmp, _T("-"), strArrayTmp) ;
									if (strArrayTmp.GetSize()==2)
									{
										strTmp.Format(_T("%s-%s"), circuitdata->m_strFJNodePre, strArrayTmp.GetAt(1)) ;
										pText->setTextString(strTmp) ;
									}
								}
							}
							break ;
						case 23:
							{								
								pText->setTextString(circuitdata->m_strStationName) ;	
							}
							break ;
						case 24:
							{								
								pText->setTextString(circuitdata->m_strCircuitType) ;	
							}
							break ;
						case 25:
							{								
								pText->setTextString(circuitdata->m_strSN) ;	
							}
							break ;
						case 26:
							{								
								pText->setTextString(circuitdata->m_strDate) ;	
							}
							break ;
						default:
							break ;
						}
					}
				}

				pEnt->close() ;
			}
		}

		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;

		CString strSaveFileName ;
		CString strCurGName = circuitdata->m_strCurGName ;
		strCurGName.Replace(_T("/"), _T("-")) ;
		
		strSaveFileName.Format(_T("%s\\站内移频轨道电路-%s.dwg"), m_strSavePath, strCurGName) ;
		
		mysys.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
		pCurDb->saveAs(strSaveFileName) ;
	}

	delete pCurDb ;
	pCurDb = NULL ;
}

//计算25Hz轨道电路
int CStationSystem::Compute25HzCircuit()
{
	int iRet = 0 ;

	double dDwgHeight = 297 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 10 ;
	double dCellWidth1 = 18 ;
	double dCellWidth2 = 15 ;
	int iRowNum = 24 ;

	double d1F1JWidth = 147 ;
	double d1F2JWidth = 206 ;
	double d1F3JWidth = 230 ;
	double d1FnJHeight = 82.5689 ;
	CString strCircuitTempleteDwg[3] = {
		_T("GD_1F1J.dwg"),
		_T("GD_1F2J.dwg"),
		_T("GD_1F3J.dwg") 
	} ;
	CString strCircuitTemplete[3] = {_T(""), _T(""),_T("")} ;
	for (int iTempletIndex=0; iTempletIndex<3; iTempletIndex++)
	{
		strCircuitTemplete[iTempletIndex].Format(_T("%s\\support\\xls_grid_dwg\\%s"), m_strSysBasePath, strCircuitTempleteDwg[iTempletIndex]) ;
	}

	CString strGdName_Type[3] = {_T("27"),_T("29"),_T("30")} ;
	CString strGLoc_Type[3] = {_T("443"),_T("445"),_T("446")} ;
	CString strGFx_Type[3][2] = {{_T("183"),_T("209")},{_T("185"),_T("211")},{_T("187"),_T("188")}} ;
	CString strRDGJ1_Type[3] = {_T("287"),_T("289"),_T("302")} ;
	CString strRDGJ2_Type[3] = {_T("313"),_T("315"),_T("305")} ;
	CString strRDGJ3_Type[3] = {_T("184"),_T("186"),_T("303")} ;
	CString strRDGJ4_Type[3] = {_T("210"),_T("212"),_T("304")} ;
	CString strRDGJ11_Type[3] = {_T("339"),_T("341"),_T("300")} ;
	CString strRDGJ12_Type[3] = {_T("365"),_T("367"),_T("301")} ;
	CString strG5_3_Type[3] = {_T("235"),_T("237"),_T("264")} ;
	CString strG5_4_Type[3] = {_T("261"),_T("263"),_T("265")} ;
	
	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		AcDbDatabase *pCurDb2 = new AcDbDatabase ;
		CUserSystem mysys2 ;
		mysys2.m_pDb = pCurDb2 ;

		CADORecordset cSetAllGd ;
		CString strSql ;

		strSql.Format(_T("select * from g_place order by shelf_num asc, layer_num desc, box_num asc")) ;
		if (cSetAllGd.IsOpen())
		{
			cSetAllGd.Close() ;
		}
		cSetAllGd.Open(g_PtrCon, strSql) ;
		if (!cSetAllGd.IsBOF())
		{
			cSetAllGd.MoveFirst() ;
		}

		//////////////////////////////////////////////////////////////////////////
		int iRsCount = cSetAllGd.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("正在计算25Hz轨道")) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		//////////////////////////////////////////////////////////////////////////

		int iDrawIndex = 0 ; //当前绘制到第几行
		int iDrawGroupIndex = 0 ; //当前绘制到第几组
		
		while (!cSetAllGd.IsEOF())
		{
			//////////////////////////////////////////////////////////////////////////
			acedSetStatusBarProgressMeterPos(iRet) ;
			//////////////////////////////////////////////////////////////////////////

			CString strGdName ;
			cSetAllGd.GetFieldValue(_T("gd_name"), strGdName) ;
			if (CBlkUtility::IsStrMatch(_T("\\b.+G\\b"), strGdName)) //如果是strGdName最后一位字符是G则处理，如果是111G1或者111G2类型的跳过 "\\b.+G[1,2]{1}\\b"
			{
				CADORecordset cSetGGroup ;//处理某个轨道组，比如111G、111G1、111G2算一组
				strSql.Format(_T("select * from g_place where gd_name like '%s%%' order by gd_name asc"), strGdName) ;
				if (cSetGGroup.IsOpen())
				{
					cSetGGroup.Close() ;
				}
				if(cSetGGroup.Open(g_PtrCon, strSql))
				{
					if (!cSetGGroup.IsBOF())
					{
						cSetGGroup.MoveFirst() ;
					}
					
					CMapStringToString mapCircuitTypeToStr ; //保存配线用电路图的相关数据

					int iGroupCount = cSetGGroup.GetRecordCount() ; //有n个结果就是1送n受
					int iGroupIndex = 0 ; //在该组中的序号
					CString strZGdLoc = _T("") ;//组合位置
					int iJLoc = 1 ; //在组合中所用继电器的位置
					
					ads_point pt_base_draw = {0, 0, 0} ;
					ads_point pt_base_draw_tmp = {0,0,0} ;

					while (!cSetGGroup.IsEOF())
					{
						int iGShelfNum = 0, iGLayerNum = 0 ;
						CString strGBoxNum = _T("") ; 
						CString strFxp_J = _T("") ; //至分线盘 J
						CString strFxp_JH = _T("") ; //至分线盘 JH
						CString strRDGJ_3 = _T("") ; //至分线盘 RDGJ-3
						CString strRDGJ_4 = _T("") ; //至分线盘 RDGJ-4
						CString strGcp_G_3 = _T("") ; //至轨测盘 RDGJ-3
						CString strGcp_G_4 = _T("") ; //至轨测盘 RDGJ-4
						CString strRDGJ_1 = _T("") ; //至组合侧面 RDGJ-1
						CString strRDGJ_2 = _T("") ; //至组合侧面 RDGJ-2
						CString strRDGJ_11 = _T("") ; //至组合侧面 RDGJ-11
						CString strRDGJ_12 = _T("") ; //至组合侧面 RDGJ-12
						CString strDGJ_1 = _T("") ; //至组合侧面 DGJ-1
						CString strJkg_Loc = _T("") ; //至接口柜
						CString strDGJ_12 = _T("") ; //至接口柜 DGJ-12
						
						pt_base_draw[1] = 0-(iDrawIndex/iRowNum)*(dDwgHeight+dBetweenDwg)-(iDrawIndex%iRowNum)*dCellHeight-dCellHeight/2;
						
						cSetGGroup.GetFieldValue(_T("gd_name"), strGdName) ;
						cSetGGroup.GetFieldValue(_T("shelf_num"), iGShelfNum) ;
						cSetGGroup.GetFieldValue(_T("layer_num"), iGLayerNum) ;
						cSetGGroup.GetFieldValue(_T("box_num"), strGBoxNum) ;

						//电路图填充data
						mapCircuitTypeToStr.SetAt(strGdName_Type[iGroupIndex], strGdName) ;
						//绘制轨道名称
						pt_base_draw[0] = dCellWidth1/2 ;
						//mysys.WriteText(pt_base_draw, strGdName, 0, AcDb::kTextMid, 2.5, 0.7) ;
						mysys.AddText(strGdName, pt_base_draw, pt_base_draw, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
						//绘制送受类型
						pt_base_draw[0] = dCellWidth1+dCellWidth2/2 ;
						CString strDescribe ; 
						strDescribe.Format(_T("一送%s受"), CBlkUtility::DigitToCapsString(iGroupCount)) ;
						//mysys.WriteText(pt_base_draw, strDescribe, 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strDescribe, pt_base_draw, pt_base_draw, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
						//电路图填充data
						CString strGLocTmp ;
						strGLocTmp.Format(_T("G%d-%d"), iGShelfNum, iGLayerNum) ;
						mapCircuitTypeToStr.SetAt(strGLoc_Type[iGroupIndex], strGLocTmp) ;
						//绘制在轨道柜的位置
						CString strGLoc ; //在轨道柜的位置，比如G2-6B
						strGLoc.Format(_T("G%d-%d%s"), iGShelfNum, iGLayerNum, strGBoxNum) ;
						pt_base_draw[0] = dCellWidth1+3*dCellWidth2+dCellWidth2/2 ;
						//mysys.WriteText(pt_base_draw, strGLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
						mysys.AddText(strGLoc, pt_base_draw, pt_base_draw, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;

						CADORecordset cSetFind ;
						if (iGroupIndex==0) //只有在第一条记录才有组合位置,JK
						{
							strSql.Format(_T("select * from z_gd_place where gd_name='%s'"), strGdName) ;
							if (cSetFind.IsOpen())
							{
								cSetFind.Close() ;
							}
							if (cSetFind.Open(g_PtrCon, strSql))
							{
								if (!cSetFind.IsBOF())
								{
									cSetFind.MoveFirst() ;
								}
								while (!cSetFind.IsEOF())
								{
									cSetFind.GetFieldValue(_T("z_loc"), strZGdLoc ) ;
									cSetFind.GetFieldValue(_T("j_loc"), iJLoc) ;
									//电路图填充data
									mapCircuitTypeToStr.SetAt(_T("469"), strZGdLoc) ;
									//绘制组合位置
									CString strZTmp ;
									strZTmp.Format(_T("%s(%d)"), strZGdLoc, iJLoc) ;
									pt_base_draw[0] = dCellWidth1+4*dCellWidth2+dCellWidth2/2 ;
									//mysys.WriteText(pt_base_draw, strZTmp, 0, AcDb::kTextMid, 2.5, 0.75) ;
									mysys.AddText(strZTmp, pt_base_draw, pt_base_draw, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

									//begin DGJ1、12
									CADORecordset cSetFindTmp ;
									strSql.Format(_T("select * from node_collect where name='DGJ' and dwg_type='25HzGD' and index=%d"), iJLoc) ;
									if (cSetFindTmp.IsOpen())
									{
										cSetFindTmp.Close() ;
									}
									if (cSetFindTmp.Open(g_PtrCon, strSql))
									{
										if (!cSetFind.IsBOF())
										{
											cSetFindTmp.MoveFirst() ;
										}
										
										while (!cSetFindTmp.IsEOF())
										{
											CString strDGJNodeLoc = _T(""), strDGJNode = _T("") ;
											cSetFindTmp.GetFieldValue(_T("loc"), strDGJNodeLoc) ;
											if (strDGJNodeLoc==_T("1"))
											{									
												cSetFindTmp.GetFieldValue(_T("node"), strDGJ_1) ;
												//电路图填充data
												mapCircuitTypeToStr.SetAt(_T("391"), strDGJ_1) ;
												//
												pt_base_draw_tmp[0] = dCellWidth1+13*dCellWidth2+dCellWidth2/2 ;
												pt_base_draw_tmp[1] = pt_base_draw[1] ;
												//mysys.WriteText(pt_base_draw_tmp, strDGJ_1, 0, AcDb::kTextMid, 2.5, 0.7) ;
												mysys.AddText(strDGJ_1, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
											}
											else if (strDGJNodeLoc==_T("12"))
											{
												cSetFindTmp.GetFieldValue(_T("node"), strDGJ_12) ;
												//电路图填充data
												mapCircuitTypeToStr.SetAt(_T("418"), strDGJ_12) ;
												//
												pt_base_draw_tmp[0] = dCellWidth1+14*dCellWidth2+dCellWidth2/2 ;
												pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;
												//mysys.WriteText(pt_base_draw_tmp, strDGJ_12, 0, AcDb::kTextMid, 2.5, 0.65) ;
												mysys.AddText(strDGJ_12, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
											}											

											cSetFindTmp.MoveNext() ;
										}
										cSetFindTmp.Close() ;
									}
									//end DGJ1、12

									//begin 至接口柜 JK
									strSql.Format(_T("select * from interlock_outin_place where dev_name='%s' and type=1"), strGdName) ;
									if (cSetFindTmp.IsOpen())
									{
										cSetFindTmp.Close() ;
									}
									if (cSetFindTmp.Open(g_PtrCon, strSql))
									{
										if (!cSetFindTmp.IsBOF())
										{
											cSetFindTmp.MoveFirst() ;
										}
									
										while (!cSetFindTmp.IsEOF())
										{
											int iQcLayer = 0, iQcBoxNum = 0, iQcCellNum = 0 ;
											cSetFindTmp.GetFieldValue(_T("layer_num"), iQcLayer) ;
											cSetFindTmp.GetFieldValue(_T("box_num"), iQcBoxNum) ;
											cSetFindTmp.GetFieldValue(_T("cell_num"), iQcCellNum) ;
											
											strJkg_Loc.Format(_T("JK-%d0%d-%d"), iQcLayer, iQcBoxNum, iQcCellNum) ;	
											
											//电路图填充data
											mapCircuitTypeToStr.SetAt(_T("417"), strJkg_Loc) ;
										
											pt_base_draw_tmp[0] = dCellWidth1+14*dCellWidth2+dCellWidth2/2 ;
											pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
											//mysys.WriteText(pt_base_draw_tmp, strJkg_Loc, 0, AcDb::kTextMid, 2.5, 0.65) ;
											mysys.AddText(strJkg_Loc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
											
											cSetFindTmp.MoveNext() ;
										}
										cSetFindTmp.Close() ;
									}
									//end 至接口柜 JK


									cSetFind.MoveNext() ;
								}
								cSetFind.Close() ;
							}
						}

						//至分线盘J、JH的位置
						strSql.Format(_T("select * from fx_data where dev_name='%s' and dev_type='J'"), strGdName) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}

							pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;

							while (!cSetFind.IsEOF())
							{
								CString strFxDevCoreName = _T("") ;
								int iFxShelf = 0, iFxLayer = 0, iFxBox = 0, iFxCellNum = 0 ;
								cSetFind.GetFieldValue(_T("shelf_num"), iFxShelf) ;
								cSetFind.GetFieldValue(_T("layer_num"), iFxLayer) ;
								cSetFind.GetFieldValue(_T("box_num"), iFxBox) ;
								cSetFind.GetFieldValue(_T("cell_num"), iFxCellNum) ;
								cSetFind.GetFieldValue(_T("core_name"), strFxDevCoreName) ;
								if (strFxDevCoreName==_T("J"))
								{
									strFxp_J.Format(_T("F%d-%d0%d-%d"), iFxShelf, iFxLayer, iFxBox, iFxCellNum) ;
									pt_base_draw_tmp[0] = dCellWidth1+5*dCellWidth2+dCellWidth2/2 ;
									//mysys.WriteText(pt_base_draw_tmp, strFxp_J, 0, AcDb::kTextMid, 2.5, 0.65) ;
									mysys.AddText(strFxp_J, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
								}
								else if (strFxDevCoreName==_T("JH"))
								{
									strFxp_JH.Format(_T("F%d-%d0%d-%d"), iFxShelf, iFxLayer, iFxBox, iFxCellNum) ;
									pt_base_draw_tmp[0] = dCellWidth1+6*dCellWidth2+dCellWidth2/2 ;
									//mysys.WriteText(pt_base_draw_tmp, strFxp_JH, 0, AcDb::kTextMid, 2.5, 0.65) ;
									mysys.AddText(strFxp_JH, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
								}
								
								cSetFind.MoveNext() ;
							}

							cSetFind.Close() ;
						}
						//电路图填充data
						mapCircuitTypeToStr.SetAt(strGFx_Type[iGroupIndex][0], strFxp_J) ;
						mapCircuitTypeToStr.SetAt(strGFx_Type[iGroupIndex][1], strFxp_JH) ;

						// RDGJ-1、2、3、4、11、12
						int iRDGJ_Index = 0 ;
						if (strGBoxNum==_T("A"))
						{
							iRDGJ_Index = 1 ;
						}
						else if (strGBoxNum==_T("B"))
						{
							iRDGJ_Index = 2 ;
						}
						else if (strGBoxNum==_T("C"))
						{
							iRDGJ_Index = 3 ;
						}
						strSql.Format(_T("select * from node_collect where name='RDGJ' and dwg_type='25HzGD' and index=%d"), iRDGJ_Index) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}

							while (!cSetFind.IsEOF())
							{
								CString strRDGJ_Loc = _T("");
								cSetFind.GetFieldValue(_T("loc"), strRDGJ_Loc) ;

								if(strRDGJ_Loc==_T("1"))
								{
									cSetFind.GetFieldValue(_T("node"), strRDGJ_1) ;	
									pt_base_draw_tmp[0] = dCellWidth1+9*dCellWidth2+dCellWidth2/2 ;
									pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
									//mysys.WriteText(pt_base_draw_tmp, strRDGJ_1, 0, AcDb::kTextMid, 2.5, 0.65) ;
									mysys.AddText(strRDGJ_1, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
								}									
								else if(strRDGJ_Loc==_T("2"))
								{
									cSetFind.GetFieldValue(_T("node"), strRDGJ_2) ;
									pt_base_draw_tmp[0] = dCellWidth1+10*dCellWidth2+dCellWidth2/2 ;
									pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
									//mysys.WriteText(pt_base_draw_tmp, strRDGJ_2, 0, AcDb::kTextMid, 2.5, 0.65) ;
									mysys.AddText(strRDGJ_2, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
								}									
								else if(strRDGJ_Loc==_T("3"))
								{
									cSetFind.GetFieldValue(_T("node"), strRDGJ_3) ;
									for (int iRDGD3Index=0; iRDGD3Index<2; iRDGD3Index++)
									{
										pt_base_draw_tmp[0] = dCellWidth1+(5+iRDGD3Index*2)*dCellWidth2+dCellWidth2/2 ;
										pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;
										//mysys.WriteText(pt_base_draw_tmp, strRDGJ_3, 0, AcDb::kTextMid, 2.5, 0.65) ;
										mysys.AddText(strRDGJ_3, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
									}
								}									
								else if(strRDGJ_Loc==_T("4"))
								{
									cSetFind.GetFieldValue(_T("node"), strRDGJ_4) ;
									for (int iRDGD4Index=0; iRDGD4Index<2; iRDGD4Index++)
									{
										pt_base_draw_tmp[0] = dCellWidth1+(6+iRDGD4Index*2)*dCellWidth2+dCellWidth2/2 ;
										pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;
										//mysys.WriteText(pt_base_draw_tmp, strRDGJ_4, 0, AcDb::kTextMid, 2.5, 0.65) ;
										mysys.AddText(strRDGJ_4, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
									}
								}									
								else if(strRDGJ_Loc==_T("11"))
								{
									cSetFind.GetFieldValue(_T("node"), strRDGJ_11) ;
									pt_base_draw_tmp[0] = dCellWidth1+11*dCellWidth2+dCellWidth2/2 ;
									pt_base_draw_tmp[1] = pt_base_draw[1] ;
									//mysys.WriteText(pt_base_draw_tmp, strRDGJ_11, 0, AcDb::kTextMid, 2.5, 0.7) ;
									mysys.AddText(strRDGJ_11, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;

								}									
								else if(strRDGJ_Loc==_T("12"))
								{
									cSetFind.GetFieldValue(_T("node"), strRDGJ_12) ;
									pt_base_draw_tmp[0] = dCellWidth1+12*dCellWidth2+dCellWidth2/2 ;
									pt_base_draw_tmp[1] = pt_base_draw[1] ;
									//mysys.WriteText(pt_base_draw_tmp, strRDGJ_12, 0, AcDb::kTextMid, 2.5, 0.7) ;
									mysys.AddText(strRDGJ_12, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;

								}
	
								cSetFind.MoveNext() ;
							}

							cSetFind.Close() ;
						}

						//电路图填充data
						mapCircuitTypeToStr.SetAt(strRDGJ1_Type[iGroupIndex], strRDGJ_1) ;
						mapCircuitTypeToStr.SetAt(strRDGJ2_Type[iGroupIndex], strRDGJ_2) ;
						mapCircuitTypeToStr.SetAt(strRDGJ3_Type[iGroupIndex], strRDGJ_3) ;
						mapCircuitTypeToStr.SetAt(strRDGJ4_Type[iGroupIndex], strRDGJ_4) ;
						mapCircuitTypeToStr.SetAt(strRDGJ11_Type[iGroupIndex], strRDGJ_11) ;
						mapCircuitTypeToStr.SetAt(strRDGJ12_Type[iGroupIndex], strRDGJ_12) ;						

						//轨测盘配线 都在轨测盘第5层
						long iInGcpPmIndex = -1 ;
						strSql.Format(_T("select * from g_place where shelf_num=%d order by layer_num desc, box_num asc"), iGShelfNum) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}
							CString strFind ;
							strFind.Format(_T("gd_name='%s'"), strGdName) ;
							if(cSetFind.Find(strFind))
							{
								iInGcpPmIndex = cSetFind.GetAbsolutePosition() ;
							}

							cSetFind.Close() ;
						}
						
						strSql.Format(_T("select * from node_collect where name='GD_TEST_PLATE' and dwg_type='25HzGD' and index=%d"), iInGcpPmIndex) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}

							while (!cSetFind.IsEOF())
							{
								CString strInGcpPmLoc = _T("") ;
								CString strGcpPmNode = _T("") ;
								cSetFind.GetFieldValue(_T("loc"), strInGcpPmLoc) ;
								cSetFind.GetFieldValue(_T("node"), strGcpPmNode) ;
								if (strInGcpPmLoc==_T("3"))
								{
									strGcp_G_3.Format(_T("G%d-5%s"), iGShelfNum, strGcpPmNode) ;
									pt_base_draw_tmp[0] = dCellWidth1+7*dCellWidth2+dCellWidth2/2 ;
									pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
									//mysys.WriteText(pt_base_draw_tmp, strGcp_G_3, 0, AcDb::kTextMid, 2.5, 0.65) ;
									mysys.AddText(strGcp_G_3, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;
								}
								else if (strInGcpPmLoc==_T("4"))
								{
									strGcp_G_4.Format(_T("G%d-5%s"), iGShelfNum, strGcpPmNode) ;
									pt_base_draw_tmp[0] = dCellWidth1+8*dCellWidth2+dCellWidth2/2 ;
									pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
									//mysys.WriteText(pt_base_draw_tmp, strGcp_G_4, 0, AcDb::kTextMid, 2.5, 0.65) ;
									mysys.AddText(strGcp_G_4, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.65, 0, 0) ;

								}
								
								cSetFind.MoveNext() ;
							}
							
							cSetFind.Close() ;
						}
						//电路图填充data
						mapCircuitTypeToStr.SetAt(strG5_3_Type[iGroupIndex], strGcp_G_3) ;
						mapCircuitTypeToStr.SetAt(strG5_4_Type[iGroupIndex], strGcp_G_4) ;						

						iGroupIndex++ ;
						iDrawIndex++ ;
						cSetGGroup.MoveNext() ;
					}
					cSetGGroup.Close() ;


					//插入配线用电路图
					AcGeMatrix3d xform;  
					AcGeVector3d vec(0, -iDrawGroupIndex*d1FnJHeight, 0); 
					xform.setToTranslation(vec); 					
					this->InsertDwgToDb(strCircuitTemplete[iGroupCount-1], pCurDb2, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iDrawGroupIndex*100) ;

					iDrawGroupIndex++ ;
				}
				
			}

			iRet++ ;
			cSetAllGd.MoveNext() ;
		}
		cSetAllGd.Close() ;

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = iRsCount/iRowNum+(iRsCount%iRowNum==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("25Hz相敏轨道电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_25hz.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
		}
		//end 绘制图框（包括电路示意图，表格等）


		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_GD_25HZ")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\25Hz轨道电路图.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
		delete pCurDb ;
		pCurDb = NULL ;

		mysys2.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
		mysys2.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		strSaveFilePath.Format(_T("%s\\配线专用-25Hz轨道电路图.dwg"), this->m_strSavePath) ;
		pCurDb2->saveAs(strSaveFilePath) ;
		delete pCurDb2 ;
		pCurDb2 = NULL ;		

		acedRestoreStatusBar() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in Compute25HzCircuit")) ;
	} 


	return iRet ;

}

BOOL CStationSystem::InsertDwgToDb(const CString strDwgFilePath, AcDbDatabase *&pDb, const AcGeMatrix3d &xform, const CMapStringToString &mapTypeToStr, CString strRepType, int iIdAdd)
{
	BOOL bRet = TRUE ;

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	AcDbObjectId id;
    AcDbObjectIdArray list;
    AcDbDatabase extDatabase( Adesk::kFalse );
 
    if (Acad::eOk != extDatabase.readDwgFile(strDwgFilePath))
    {
        acutPrintf(_T("\n读取模板文件 %s 失败!"), strDwgFilePath);
        return FALSE ;
    }
    AcDbBlockTable* pBT;
    if (Acad::eOk != extDatabase.getSymbolTable( pBT, AcDb::kForRead ))
    {
		acutPrintf(_T("\n从模板文件 %s 读取块表失败!"), strDwgFilePath);
        return FALSE ;
    }
    AcDbBlockTableRecord* pBTR;
    Acad::ErrorStatus es = pBT->getAt( ACDB_MODEL_SPACE, pBTR, AcDb::kForRead );
    pBT->close();
    if (Acad::eOk != es) 
	{
		acutPrintf(_T("\ngetting model space from: %s 失败!"), strDwgFilePath);
        return FALSE ;
    }
	
    AcDbBlockTableRecordIterator* pIT;
    if (Acad::eOk != pBTR->newIterator( pIT )) 
	{
        acutPrintf(_T("\nError iterating model space of DWG"));
        pBTR->close();

        return FALSE ;
    }
	
    for ( ; !pIT->done(); pIT->step()) 
	{
        if (Acad::eOk == pIT->getEntityId( id ))
		{
            list.append( id );
			
            // There is a bug in ARX that causes extension dictionaries
            // to appear to be soft owners of their contents.  This causes
			// the contents to be skipped during wblock.  To fix this we
			// must explicitly tell the extension dictionary to be a hard
			// owner of it's entries.
            //
            AcDbEntity *pEnt;
            if ( Acad::eOk == pIT->getEntity(pEnt, AcDb::kForWrite)) 
			{  
				//begin 增加实体ID数值，防止重复
				long lId = 0 ;
				if (mysys.GetXdata(pEnt,_T("ID"), lId))
				{
					long lNewId = lId+iIdAdd ;
					mysys.SetXdata(pEnt, _T("ID"), lNewId) ;
				}
				lId = 0 ;
				if (mysys.GetXdata(pEnt,_T("CON_1_ID"), lId))
				{
					long lNewId = lId+iIdAdd ;
					mysys.SetXdata(pEnt, _T("CON_1_ID"), lNewId) ;
				}
				lId = 0 ;
				if (mysys.GetXdata(pEnt,_T("CON_2_ID"), lId))
				{
					long lNewId = lId+iIdAdd ;
					mysys.SetXdata(pEnt, _T("CON_2_ID"), lNewId) ;
				}
				lId = 0 ;
				if (mysys.GetXdata(pEnt,_T("PARENT_ID"), lId))
				{
					long lNewId = lId+iIdAdd ;
					mysys.SetXdata(pEnt, _T("PARENT_ID"), lNewId) ;
				}
				//end 增加实体ID数值，防止重复

				//begin 替换相关文本
				if (_tcscmp(pEnt->isA()->name(), _T("AcDbText"))==0)
				{
					AcDbText *pText = (AcDbText*)pEnt ;
					int iType = -1 ;
					if (mysys.GetXdata(pEnt, strRepType, iType))
					{
						CString strType, strValue ;
						strType.Format(_T("%d"), iType) ;
						if(mapTypeToStr.Lookup(strType, strValue))
						{
							pText->setTextString(strValue) ;
						}
					}

				}
				//end 替换相关文本
				
				AcDbObjectId obj;
                if ((obj = pEnt->extensionDictionary())!= AcDbObjectId::kNull)
                {
                    AcDbDictionary *pDict = NULL;
                    acdbOpenObject(pDict, obj, AcDb::kForWrite);
                    if (pDict)
					{
                        pDict->setTreatElementsAsHard(Adesk::kTrue);
                        pDict->close();
                    }
                }
                pEnt->close();
            }
        }
    }
	
    delete pIT;
    pBTR->close();
	
    if (list.isEmpty()) 
	{
        acutPrintf(_T("\nNo entities found in model space of DWG"));
        return FALSE ;
    }
	
    AcDbDatabase *pTempDb;
	
	AcGePoint3d pt_3d(0,0,0) ;
    if (Acad::eOk != extDatabase.wblock( pTempDb, list, pt_3d))
    {
        acutPrintf(_T("\nwblock failed!"));
        return FALSE ;
    }		
	if (Acad::eOk != pDb->insert( xform, pTempDb ))
	{
		acutPrintf(_T("insert failed!") );
		return FALSE ;
	}	
	
    delete pTempDb;

	return bRet ;

}

//计算并绘制进站信号机电路图和配线用电路图
int CStationSystem::ComputeJzCircuit()
{
	int iRet = 0 ;

	double dDwgHeight = 297 ;
	double dDwgWidth = 420 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 10 ;
	double dCellWidth1 = 16 ;
	double dCellWidth2 = 18 ;
	int iRowNum = 8 ;
	
	CString strOut_J[7] = {_T("LXJ"),_T("ZXJ"),_T("TXJ"),_T("YXJ"),_T("LUXJ"),_T("KDJ"),_T("XSJ")} ;
	CString strIn_J[11] = {_T("1DJ"),_T("2DJ"),_T("LXJ"),_T("LXJ13"),_T("ZXJ"),_T("ZXJ13"),_T("TXJ"),_T("LUXJ"),_T("KDJ"),_T("SNKJ"),_T("SNJJ")} ;

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from z_place where type='JZZ' order by shelf_num asc, layer_num desc")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		cSetAll.Open(g_PtrCon, strSql) ;
		if (!cSetAll.IsBOF())
		{
			cSetAll.MoveFirst() ;
		}

		int iRsCount = cSetAll.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("正在计算进站信号机电路")) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
		while (!cSetAll.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iRet) ;

			CADORecordset cSetFind ;

			int iUSUZIndexInBox = -1 ; //在USUZ中的序号0/1			
			CString strXHJName = _T("") ; //信号机名称
			CString strJZZLoc = _T("") ; //JZZ位置
			CString strUSUZLoc = _T("") ; //USUZ位置
			
			ads_point pt_base_draw = {0, 0, 0} ;
			ads_point pt_base_draw_tmp = {0, 0, 0} ;
			ads_point pt_base_draw_tmp2 = {0, 0, 0} ;

			pt_base_draw[1] = 0-(iRet/iRowNum)*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight-dCellHeight/2;

			//信号机名称
			cSetAll.GetFieldValue(_T("dev_name"), strXHJName) ;

			//点灯电路
			pt_base_draw_tmp[0] = dCellWidth1/2 ;
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//驱动采集图
			pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+dCellWidth2/2 ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;


			//JZZ组合位置
			pt_base_draw_tmp[0] = dCellWidth1+dCellWidth1/2 ;
			pt_base_draw_tmp2[0] = dDwgWidth+dBetweenDwg+dCellWidth2+dCellWidth2/2 ;
			
			int iJZZShelf = 0, iJZZLayer = 0 ;
			cSetAll.GetFieldValue(_T("shelf_num"), iJZZShelf) ;
			cSetAll.GetFieldValue(_T("layer_num"), iJZZLayer) ;
			strJZZLoc.Format(_T("Z%d-%d"), iJZZShelf, iJZZLayer) ;
			//					
			pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
			//mysys.WriteText(pt_base_draw_tmp, strJZZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strJZZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//
			pt_base_draw_tmp2[1] = pt_base_draw_tmp[1] ;
			//mysys.WriteText(pt_base_draw_tmp2, strJZZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strJZZLoc, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			
			//USUZ组合位置
			CString strUSUZLocTmp = _T("") ;
			strSql.Format(_T("select * from z_place where dev_name='%s' and type='USUZ'"), strXHJName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}				
				while (!cSetFind.IsEOF())
				{
					int iUSUZShelf = 0, iUSUZLayer = 0 ;
					cSetFind.GetFieldValue(_T("shelf_num"), iUSUZShelf) ;
					cSetFind.GetFieldValue(_T("layer_num"), iUSUZLayer) ;					
					strUSUZLocTmp.Format(_T("Z%d-%d"), iUSUZShelf, iUSUZLayer) ;
					//index in box	
					cSetFind.GetFieldValue(_T("index_in_box"), iUSUZIndexInBox) ;					
					strUSUZLoc.Format(_T("%s(%d)"), strUSUZLocTmp, iUSUZIndexInBox+1) ;
					
					cSetFind.MoveNext() ;
				}
				
				cSetFind.Close() ;
			}

			pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;

			if (strUSUZLoc.IsEmpty())
			{
				AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
				AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
				mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
			}
			else
			{
				//mysys.WriteText(pt_base_draw_tmp, strUSUZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strUSUZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			}

			//
			pt_base_draw_tmp2[1] = pt_base_draw_tmp[1] ;
			//mysys.WriteText(pt_base_draw_tmp2, strUSUZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strUSUZLoc, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

			//JZZ至分线盘端子 
			strSql.Format(_T("select * from fx_data where  dev_name='%s'and dev_type='JZA' "), strXHJName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}

				int iFxIndex = 0 ;
				while (!cSetFind.IsEOF())
				{
					CString strCoreName = _T("") ;
					int iFxShelfNum = 0, iFxLayerNum = 0, iFxBoxNum = 0, iFxCellNum = 0 ;
					cSetFind.GetFieldValue(_T("core_name"), strCoreName) ;
					cSetFind.GetFieldValue(_T("shelf_num"), iFxShelfNum) ;
					cSetFind.GetFieldValue(_T("layer_num"), iFxLayerNum) ;
					cSetFind.GetFieldValue(_T("box_num"), iFxBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iFxCellNum) ;
					CString strFxLoc = _T("") ;
					strFxLoc.Format(_T("F%d-%d0%d-%d"), iFxShelfNum, iFxLayerNum, iFxBoxNum, iFxCellNum) ;
					if (strCoreName==_T("1U"))
					{
						//无USUZ
						if (iUSUZIndexInBox==-1) 
						{
							pt_base_draw_tmp[0] = 2*dCellWidth1+dCellWidth1/2 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
							//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;	
							mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
							
							pt_base_draw_tmp[0] = 10*dCellWidth1+dCellWidth1/2 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("B"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("B"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							AcGePoint3d pt_3d_cen(pt_base_draw_tmp[0], pt_base_draw_tmp[1],0) ;
							mysys.AddAcDbCircle(pt_3d_cen, 2) ;
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

							pt_base_draw_tmp[0] = 11*dCellWidth1+dCellWidth1/2 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("A"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("A"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							pt_3d_cen.x = pt_base_draw_tmp[0] ;
							mysys.AddAcDbCircle(pt_3d_cen, 2) ;
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
						}
						else
						{
							pt_base_draw_tmp[0] = 2*dCellWidth1+6 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("至"), 0, AcDb::kTextMid, 2.5, 0.8) ;	
							mysys.AddText(_T("至"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.8, 0, 0) ;
							pt_base_draw_tmp[0] = 2*dCellWidth1+11 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("A"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("A"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							AcGePoint3d pt_3d_cen(pt_base_draw_tmp[0], pt_base_draw_tmp[1],0) ;
							mysys.AddAcDbCircle(pt_3d_cen, 2) ;
							AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

							pt_base_draw_tmp[0] = 10*dCellWidth1+dCellWidth1/2 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("至 1U"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("至 1U"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							pt_3d_line_start.y = pt_base_draw_tmp[1]+dCellHeight/2 ;
							pt_3d_line_end.y = pt_base_draw_tmp[1]+dCellHeight/2 ;						
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

							pt_base_draw_tmp[0] = 11*dCellWidth1+dCellWidth1/2 ;
							//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;	
							mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
						}
					
						cSetFind.MoveNext() ;
						continue ;
					}
					
					pt_base_draw_tmp[0] = (3+iFxIndex)*dCellWidth1+dCellWidth1/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
					//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

					iFxIndex++ ;
					cSetFind.MoveNext() ;
				}

				cSetFind.Close() ;
			}
			
			//驱动采集out LXJ-1 ZXJ-1 TXJ-1 YXJ-1 LUXJ-1 KDJ-1 SNKJ-1
			for (int iOutJ=0; iOutJ<7; iOutJ++)
			{
				CString strJkLoc = _T("") ;
				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=0"), strXHJName, strOut_J[iOutJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;		
						
						cSetFind.MoveNext() ;
					}					
					cSetFind.Close() ;
				}

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(2+iOutJ)*dCellWidth2 + dCellWidth2/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]+(iOutJ==6?-1:1)*dCellHeight/4 ;
				if (strJkLoc==_T(""))
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				}
			}
			//驱动采集in DJ-12 YXJ-12....
			for (int iInJ=0; iInJ<11; iInJ++)
			{
				CString strJkLoc = _T("") ;
				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=1"), strXHJName, strIn_J[iInJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{						
						
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;					
						
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(9+iInJ)*dCellWidth2 + dCellWidth2/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]+(iInJ>8?-1:1)*dCellHeight/4 ;
				if (strJkLoc==_T(""))
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				}
			}	

			iRet++ ;
			cSetAll.MoveNext() ;
		}
		cSetAll.Close() ;

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = iRsCount/iRowNum+(iRsCount%iRowNum==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("进站信号机点灯电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_jz_dd.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
			
			vec.x = dDwgWidth+dBetweenDwg ;
			xform.setToTranslation(vec); 
			strDwgType.Format(_T("进站信号机驱动采集电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_jz_qc.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
		}
		//end 绘制图框（包括电路示意图，表格等）

		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_JZ_DD")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\进站信号机电路图.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
		delete pCurDb ;
		pCurDb = NULL ;

		acedRestoreStatusBar() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeJzCircuit")) ;
	} 

	return iRet ;
}

//读取联锁驱动采集数据
int CStationSystem::ReadQcDataToDb(CString strXlsFileName)
{
	int iReturn = 0 ;
	
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	CString strExec, strMsg ;
	//清除old数据
	CDBUtility::ClearTable("interlock_outin_place") ;
	CUserSystem mysys ;

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

		int iCurShelfNum = 1, iCurLayerNum  = 10, iCurBoxNum = 0, iCurCellNum = 0 ;
		int iStartLayerNum = 10 ;

		CString strReg = _T("^D\\d+\\(JK(\\d*)-(\\d+)\\)$") ;

		//读取sheet1、sheet2
		for (int iSheetIndex=1; iSheetIndex<3; iSheetIndex++)
		{
			VARIANT vt_index ;
			vt_index.vt = VT_I2 ;
			vt_index.iVal = iSheetIndex ;
			
			pWorksheets  = m_pExcel->GetSheets( ) ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;
			
			//判断文件的合法性；
			BOOL bCheckFile = TRUE ;
			CString  strTitleCellName[2] = { _T("B1"), _T("A2") } ;
			CString strTitleCellContent[2] = {_T("^D\\d+\\(JK(\\d*)-(\\d+)\\)$"), _T("1")} ;
			_variant_t range_var ;
			for ( int i = 0 ; i < 2 ; i++ )
			{
				pRange = NULL ;
				pRange = pSheet->GetRange( (_variant_t)strTitleCellName[i] ) ;
				range_var = pRange->GetText() ;
				pRange = NULL ;
				CString strVar ;
				strVar = (CString ) range_var.bstrVal ;
				strVar.TrimLeft() ;
				strVar.TrimRight() ;
				if( CBlkUtility::IsStrMatch(strTitleCellContent[i], strVar)==FALSE)
				{
					bCheckFile = FALSE ;
					strMsg.Format(_T("sheet%d格式不符合：%s不是%s或形式"),iSheetIndex, strTitleCellName[i], strTitleCellContent[i]) ;
					AfxMessageBox(strMsg) ;
					break ;
				}
			}

			if (bCheckFile)
			{
				int iCur_Col = 2 ; //起始列
				
				CString strTmp ;
				BOOL bNext = TRUE ; 
				
				while (bNext) 
				{			
					CString strCellName = _T(""), strCellText = _T("") ;

					strCellName.Format(_T("%s1"), CBlkUtility::GetExcelFieldName(iCur_Col)) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					range_var = pRange->GetText() ;
					strCellText = range_var.bstrVal ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					if (strCellText==_T("")) 
					{
						bNext = FALSE ;
						continue ;
					}

					if (CBlkUtility::IsStrMatch(strReg, strCellText))
					{
						CString strShelfNum = _T(""), strLayerNum = _T(""), strBoxNum = _T(""), strCellNum = _T("") ;
						CString strTmp = _T("") ;
						
						CBlkUtility::RegReplace(strReg, _T("$1"), strCellText, strShelfNum) ;
						CBlkUtility::RegReplace(strReg, _T("$2"), strCellText, strTmp) ;
						
						iCurShelfNum = _tstoi(strShelfNum) ;
						iCurBoxNum = _tstoi(strTmp.Right(1)) ;
						iCurLayerNum = _tstoi(strTmp.Left(strTmp.GetLength()-2)) ; 
					}

					CString strBarMsg ;
					strBarMsg.Format(_T("读取sheet%d %s"), iSheetIndex, strCellText) ;
					acedSetStatusBarProgressMeter(strBarMsg, 2, 33) ;	
					
					//循环读入该列第2行到33行的数据
					for (int iRow=2; iRow<34; iRow++)
					{	
						acedSetStatusBarProgressMeterPos(iRow) ;

						strCellName.Format(_T("%s%d"), CBlkUtility::GetExcelFieldName(iCur_Col), iRow) ;
						pRange = NULL ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						range_var = pRange->GetText() ;
						strCellText = range_var.bstrVal ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						if (!strCellText.IsEmpty())
						{	
							//iCurLayerNum  = iStartLayerNum-(iCur_Col-2)/8 ;
							//iCurBoxNum = (iCur_Col-2)%8+1 ;
							iCurCellNum = iRow-1 ;
							
							strExec.Format(_T("insert into interlock_outin_place(dev_name, shelf_num, layer_num, box_num, cell_num,type)\
								values('%s',%d,%d,%d,%d, %d)"), strCellText, iCurShelfNum, iCurLayerNum, iCurBoxNum, iCurCellNum, iSheetIndex-1) ;
							ExecSqlString(strExec) ;							
						}						
					}

					acedRestoreStatusBar() ;
					
					iCur_Col++ ;					
				}
			}
			else
			{
				AfxMessageBox( _T("选择的文件不符合文件格式") ) ;			
			}

			iStartLayerNum = iCurLayerNum-1 ;

		} //end for	

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

int CStationSystem::ComputeCzCircuit()
{
	int iRet = 0 ;

	double dDwgHeight = 297 ;
	double dDwgWidth = 420 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 5 ;
	double dCellWidth1_Dd = 10 ;
	double dCellWidth2_Dd = 15 ;
	double dCellWidth_Qc = 18 ;
	int iRowNum = 40 ;

	double dPxCircuitHeight = 297 ;
	CString strCircuitTempleteDwg[2] ={ 
		_T("XHJ_CZ_LIGHT.dwg"),
		_T("XHJ_CZ_DRIVE.dwg")
	} ;
	CString strCircuitTemplete[2] ={ 
		_T(""),
		_T("")
	} ;
	for (int iTempletIndex=0; iTempletIndex<2; iTempletIndex++)
	{
		strCircuitTemplete[iTempletIndex].Format(_T("%s\\support\\xls_grid_dwg\\%s"), m_strSysBasePath, strCircuitTempleteDwg[iTempletIndex]) ;
	}

	
 	CString strFxH_BH[6] = {_T("79"),_T("105"),_T("131"),_T("157"),_T("183"),_T("209")} ;
	CString strOut_J[4] = {_T("LXJ"),_T("DXJ"),_T("YXJ"),_T("KDJ")} ;
	CString strOut_Type[4] = {_T("79"),_T("105"),_T("131"),_T("157")} ;
	CString strIn_J[6] = {_T("1DJ"),_T("2DJ"),_T("LXJ"),_T("LXJ13"),_T("DXJ"),_T("KDJ")} ;
	CString strIn_Type[6] = {_T("183"),_T("209"),_T("235"),_T("261"),_T("313")} ;

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		AcDbDatabase *pCurDb2 = new AcDbDatabase ;
		CUserSystem mysys2 ;
		mysys2.m_pDb = pCurDb2 ;

		AcDbDatabase *pCurDb3 = new AcDbDatabase ;
		CUserSystem mysys3 ;
		mysys3.m_pDb = pCurDb3 ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from z_place where type='CZZ' order by shelf_num asc, layer_num desc")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		cSetAll.Open(g_PtrCon, strSql) ;
		if (!cSetAll.IsBOF())
		{
			cSetAll.MoveFirst() ;
		}

		int iRsCount = cSetAll.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("正在计算出站信号机电路")) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
		while (!cSetAll.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iRet) ;

			CADORecordset cSetFind ;

			CMapStringToString mapCircuitTypeToStr, mapCircuitTypeToStr2 ;

			CString strXHJName = _T("") ; //信号机名称
			CString strCZZLoc = _T("") ; //CZZ位置
			
			ads_point pt_base_draw = {0, 0, 0} ;
			ads_point pt_base_draw_tmp = {0, 0, 0} ;
			ads_point pt_base_draw_tmp2 = {0, 0, 0} ;

			pt_base_draw[1] = 0-(iRet/iRowNum)*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight-dCellHeight/2;

			//信号机名称
			cSetAll.GetFieldValue(_T("dev_name"), strXHJName) ;

			//电路图填充data
			mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
			mapCircuitTypeToStr2.SetAt(_T("27"), strXHJName) ;
			//点灯电路
			pt_base_draw_tmp[0] = dCellWidth1_Dd/2 ;
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//驱动采集图
			pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+dCellWidth_Qc/2 ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

			//CZZ组合位置
			pt_base_draw_tmp[0] = dCellWidth1_Dd+dCellWidth2_Dd/2 ;
			pt_base_draw_tmp2[0] = dDwgWidth+dBetweenDwg+dCellWidth_Qc+dCellWidth_Qc/2 ;

			int iJZZShelf = 0, iJZZLayer = 0 ;
			cSetAll.GetFieldValue(_T("shelf_num"), iJZZShelf) ;
			cSetAll.GetFieldValue(_T("layer_num"), iJZZLayer) ;
			strCZZLoc.Format(_T("Z%d-%d"), iJZZShelf, iJZZLayer) ;
			//电路图填充data
			mapCircuitTypeToStr.SetAt(_T("53"), strCZZLoc) ;
			mapCircuitTypeToStr2.SetAt(_T("53"), strCZZLoc) ;
			//					
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strCZZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strCZZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//
			pt_base_draw_tmp2[1] = pt_base_draw_tmp[1] ;
			//mysys.WriteText(pt_base_draw_tmp2, strCZZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;	
			mysys.AddText(strCZZLoc, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			
			//CZZ至分线盘端子 
			strSql.Format(_T("select * from fx_data where  dev_name='%s'and dev_type='CZ'"), strXHJName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}

				int iFxIndex = 0 ;
				while (!cSetFind.IsEOF())
				{
					CString strCoreName = _T("") ;
					int iFxShelfNum = 0, iFxLayerNum = 0, iFxBoxNum = 0, iFxCellNum = 0 ;
					cSetFind.GetFieldValue(_T("core_name"), strCoreName) ;
					cSetFind.GetFieldValue(_T("shelf_num"), iFxShelfNum) ;
					cSetFind.GetFieldValue(_T("layer_num"), iFxLayerNum) ;
					cSetFind.GetFieldValue(_T("box_num"), iFxBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iFxCellNum) ;
					CString strFxLoc = _T("") ;
					strFxLoc.Format(_T("F%d-%d0%d-%d"), iFxShelfNum, iFxLayerNum, iFxBoxNum, iFxCellNum) ;
						
					pt_base_draw_tmp[0] = dCellWidth1_Dd+(1+iFxIndex)*dCellWidth2_Dd+dCellWidth2_Dd/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1] ;
					//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
					mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
					ASSERT(iFxIndex<6) ;
					//
					mapCircuitTypeToStr.SetAt(strFxH_BH[iFxIndex], strFxLoc) ;				

					iFxIndex++ ;
					cSetFind.MoveNext() ;
				}

				cSetFind.Close() ;
			}
			
			//驱动采集out 
			for (int iOutJ=0; iOutJ<4; iOutJ++)
			{
				CString strJkLoc = _T("") ;

				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s_%s' and type=0"), strXHJName, strOut_J[iOutJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;
						
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}
				//
				mapCircuitTypeToStr2.SetAt(strOut_Type[iOutJ], strJkLoc) ;
				
				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(2+iOutJ)*dCellWidth_Qc + dCellWidth_Qc/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1] ;
				if (strJkLoc==_T(""))
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
				}
			}

			//驱动采集in 
			for (int iInJ=0; iInJ<6; iInJ++)
			{
				CString strJkLoc = _T("") ;

				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s_%s' and type=1"), strXHJName, strIn_J[iInJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{								
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;
						
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}
				//
				mapCircuitTypeToStr2.SetAt(strIn_Type[iInJ], strJkLoc) ;

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(6+iInJ)*dCellWidth_Qc + dCellWidth_Qc/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1] ;
				if (strJkLoc==_T(""))
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
				}
			}		
			
			//插入配线用电路图
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iRet*dPxCircuitHeight, 0); 
			xform.setToTranslation(vec); 					
			this->InsertDwgToDb(strCircuitTemplete[0], pCurDb2, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iRet*100) ;
			
			this->InsertDwgToDb(strCircuitTemplete[1], pCurDb3, xform, mapCircuitTypeToStr2, _T("XLS_GRID_TYPE"), iRet*100) ;

			iRet++ ;
			cSetAll.MoveNext() ;
		}
		cSetAll.Close() ;

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = iRsCount/iRowNum+(iRsCount%iRowNum==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("出站信号机点灯电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_cz_dd.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
			
			vec.x = dDwgWidth+dBetweenDwg ;
			xform.setToTranslation(vec); 
			strDwgType.Format(_T("出站信号机驱动采集电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;			
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_cz_qc.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
		}
		//end 绘制图框（包括电路示意图，表格等）

		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_CZ_DD")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\出站信号机电路图.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
		delete pCurDb ;
		pCurDb = NULL ;

// 		mysys2.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
// 		mysys2.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
// 		strSaveFilePath.Format(_T("%s\\配线专用-出站信号机点灯电路图.dwg"), this->m_strSavePath) ;
// 		pCurDb2->saveAs(strSaveFilePath) ;
		delete pCurDb2 ;
		pCurDb2 = NULL ;
		
// 		mysys3.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
// 		mysys3.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
// 		strSaveFilePath.Format(_T("%s\\配线专用-出站信号机驱动采集电路图.dwg"), this->m_strSavePath) ;
// 		pCurDb3->saveAs(strSaveFilePath) ;
		delete pCurDb3 ;
		pCurDb3 = NULL ;

		acedRestoreStatusBar() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeCzCircuit")) ;
	} 

	return iRet ;
}

int CStationSystem::ComputeDcCircuit()
{
	int iRet = 0 ;

	double dDwgHeight = 297 ;
	double dDwgWidth = 420 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 5 ;
	double dCellWidth1_Dd = 10 ;
	double dCellWidth2_Dd = 15 ;
	double dCellWidth_Qc = 23 ;
	int iRowNum = 44 ;

	double dPxCircuitHeight = 297 ;
	CString strCircuitTempleteDwg[2] ={ 
		_T("XHJ_DC_LIGHT.dwg"),
		_T("XHJ_DC_DRIVE.dwg")
	} ;
	CString strCircuitTemplete[2] ={ 
		_T(""),
		_T("")
	} ;
	for (int iTempletIndex=0; iTempletIndex<2; iTempletIndex++)
	{
		strCircuitTemplete[iTempletIndex].Format(_T("%s\\support\\xls_grid_dwg\\%s"), m_strSysBasePath, strCircuitTempleteDwg[iTempletIndex]) ;
	}

	
 	CString strFxA_ABH[3] = {_T("79"),_T("105"),_T("131")} ;
// 	CString strDD_Node_Name[5] = {_T("A"),_T("B"),_T("ABH"),_T("RD1"),_T("RD2")} ;
	CString strDD_Node_Type[5] = {_T("157"),_T("183"),_T("209"),_T("261"),_T("287")} ;
	CString strQC_Node_Type[3] = {_T("151"),_T("152"),_T("153")} ;
	CString strOut_J[1] = {_T("DXJ")} ;
	CString strOut_Type[1] = {_T("79")} ;
	CString strIn_J[2] = {_T("DXJ"),_T("DJ")} ;
	CString strIn_Type[2] = {_T("105"),_T("131")} ;

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		AcDbDatabase *pCurDb2 = new AcDbDatabase ;
		CUserSystem mysys2 ;
		mysys2.m_pDb = pCurDb2 ;

		AcDbDatabase *pCurDb3 = new AcDbDatabase ;
		CUserSystem mysys3 ;
		mysys3.m_pDb = pCurDb3 ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from z_place where type='DXZ' order by plus1")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		cSetAll.Open(g_PtrCon, strSql) ;
		if (!cSetAll.IsBOF())
		{
			cSetAll.MoveFirst() ;
		}

		int iRsCount = cSetAll.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("正在计算调车信号机电路")) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
		while (!cSetAll.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iRet) ;

			CADORecordset cSetFind ;

			CMapStringToString mapCircuitTypeToStr, mapCircuitTypeToStr2 ;

			CString strXHJName = _T("") ; //信号机名称
			CString strDXZLoc = _T("") ; //DXZ位置
			int iDXZIndexInBox = -1 ; //在DXZ中的序号0/1/2	
			
			ads_point pt_base_draw = {0, 0, 0} ;
			ads_point pt_base_draw_tmp = {0, 0, 0} ;
			ads_point pt_base_draw_tmp2 = {0, 0, 0} ;

			pt_base_draw[1] = 0-(iRet/iRowNum)*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight-dCellHeight/2;

			//信号机名称
			cSetAll.GetFieldValue(_T("dev_name"), strXHJName) ;

			//电路图填充data
			mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
			mapCircuitTypeToStr2.SetAt(_T("27"), strXHJName) ;
			//点灯电路
			pt_base_draw_tmp[0] = dCellWidth1_Dd/2 ;
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//驱动采集图
			pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+dCellWidth_Qc/2 ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;


			//DXZ组合位置
			pt_base_draw_tmp[0] = dCellWidth1_Dd+dCellWidth2_Dd/2 ;
			pt_base_draw_tmp2[0] = dDwgWidth+dBetweenDwg+dCellWidth_Qc+dCellWidth_Qc/2 ;
			
			int iJZZShelf = 0, iJZZLayer = 0 ;
			cSetAll.GetFieldValue(_T("shelf_num"), iJZZShelf) ;
			cSetAll.GetFieldValue(_T("layer_num"), iJZZLayer) ;
			CString strDXZLocTmp = _T("") ;
			strDXZLocTmp.Format(_T("Z%d-%d"), iJZZShelf, iJZZLayer) ;
			//电路图填充data
			mapCircuitTypeToStr.SetAt(_T("235"), strDXZLocTmp) ;
			mapCircuitTypeToStr2.SetAt(_T("157"), strDXZLocTmp) ;
			//
			//index in box				
			cSetAll.GetFieldValue(_T("index_in_box"), iDXZIndexInBox) ;				
			
			strDXZLoc.Format(_T("%s(%d)"), strDXZLocTmp, iDXZIndexInBox+1) ;		
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strDXZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strDXZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//
			pt_base_draw_tmp2[1] = pt_base_draw_tmp[1] ;
			//mysys.WriteText(pt_base_draw_tmp2, strDXZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strDXZLoc, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

						
			//DXZ至分线盘端子 
			strSql.Format(_T("select * from fx_data where  dev_name='%s'and dev_type='DC'"), strXHJName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}

				int iFxIndex = 0 ;
				while (!cSetFind.IsEOF())
				{
					CString strCoreName = _T("") ;
					int iFxShelfNum = 0, iFxLayerNum = 0, iFxBoxNum = 0, iFxCellNum = 0 ;
					cSetFind.GetFieldValue(_T("core_name"), strCoreName) ;
					cSetFind.GetFieldValue(_T("shelf_num"), iFxShelfNum) ;
					cSetFind.GetFieldValue(_T("layer_num"), iFxLayerNum) ;
					cSetFind.GetFieldValue(_T("box_num"), iFxBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iFxCellNum) ;
					CString strFxLoc = _T("") ;
					strFxLoc.Format(_T("F%d-%d0%d-%d"), iFxShelfNum, iFxLayerNum, iFxBoxNum, iFxCellNum) ;
						
					pt_base_draw_tmp[0] = dCellWidth1_Dd+(1+iFxIndex)*dCellWidth2_Dd+dCellWidth2_Dd/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1] ;
					//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
					mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
					ASSERT(iFxIndex<3) ;
					//
					mapCircuitTypeToStr.SetAt(strFxA_ABH[iFxIndex], strFxLoc) ;				

					iFxIndex++ ;
					cSetFind.MoveNext() ;
				}

				cSetFind.Close() ;
			}

			
			//驱动采集out 
			for (int iOutJ=0; iOutJ<1; iOutJ++)
			{
				CString strJkLoc = _T("") ;
				
				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=0"), strXHJName, strOut_J[iOutJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;
						
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}
				//
				mapCircuitTypeToStr2.SetAt(strOut_Type[iOutJ], strJkLoc) ;

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(2+iOutJ)*dCellWidth_Qc + dCellWidth_Qc/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1] ;								
				if (strJkLoc==_T(""))
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
				}
				
			}
			//驱动采集in 
			for (int iInJ=0; iInJ<2; iInJ++)
			{
				CString strJkLoc = _T("") ;

				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=1"), strXHJName, strIn_J[iInJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;
						
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}
				//
				mapCircuitTypeToStr2.SetAt(strIn_Type[iInJ], strJkLoc) ;

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(3+iInJ)*dCellWidth_Qc + dCellWidth_Qc/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1] ;
				if (strJkLoc==_T(""))
				{
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.7) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.7, 0, 0) ;
				}

			}

			//配线用 点灯电路
			strSql.Format(_T("select * from node_collect where dwg_type='DC-DD' and index=%d"), iDXZIndexInBox+1) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}	
				int iNodeIndex = 0 ;
				while (!cSetFind.IsEOF())
				{
					CString strNode = _T("") ;
					cSetFind.GetFieldValue(_T("node"), strNode) ;
					mapCircuitTypeToStr.SetAt(strDD_Node_Type[iNodeIndex], strNode) ;
				
					iNodeIndex++ ;
					cSetFind.MoveNext() ;
				}
				
				cSetFind.Close() ;
			}
			//配线用 驱采电路
			strSql.Format(_T("select * from node_collect where dwg_type='DC-QC' and index=%d"), iDXZIndexInBox+1) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}	
				int iNodeIndex = 0 ;
				while (!cSetFind.IsEOF())
				{
					CString strNode = _T("") ;
					cSetFind.GetFieldValue(_T("node"), strNode) ;
					mapCircuitTypeToStr2.SetAt(strQC_Node_Type[iNodeIndex], strNode) ;
					
					iNodeIndex++ ;
					cSetFind.MoveNext() ;
				}
				
				cSetFind.Close() ;
			}
					
			
			//插入配线用电路图
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iRet*dPxCircuitHeight, 0); 
			xform.setToTranslation(vec); 					
			this->InsertDwgToDb(strCircuitTemplete[0], pCurDb2, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iRet*100) ;
			this->InsertDwgToDb(strCircuitTemplete[1], pCurDb3, xform, mapCircuitTypeToStr2, _T("XLS_GRID_TYPE"), iRet*100) ;

			iRet++ ;
			cSetAll.MoveNext() ;
		}
		cSetAll.Close() ;

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = iRsCount/iRowNum+(iRsCount%iRowNum==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("调车信号机点灯电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_dc_dd.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
			
			vec.x = dDwgWidth+dBetweenDwg ;
			xform.setToTranslation(vec); 
			strDwgType.Format(_T("调车信号机驱动采集电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;		
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_dc_qc.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
		}
		//end 绘制图框（包括电路示意图，表格等）


		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_DC_DD")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\调车信号机电路图.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
		delete pCurDb ;
		pCurDb = NULL ;

// 		mysys2.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
// 		mysys2.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
// 		strSaveFilePath.Format(_T("%s\\配线专用-调车信号机点灯电路图.dwg"), this->m_strSavePath) ;
// 		pCurDb2->saveAs(strSaveFilePath) ;
		delete pCurDb2 ;
		pCurDb2 = NULL ;
		
// 		mysys3.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
// 		mysys3.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
// 		strSaveFilePath.Format(_T("%s\\配线专用-调车信号机驱动采集电路图.dwg"), this->m_strSavePath) ;
// 		pCurDb3->saveAs(strSaveFilePath) ;
		delete pCurDb3 ;
		pCurDb3 = NULL ;

		acedRestoreStatusBar() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeDcCircuit")) ;
	} 

	return iRet ;
}

//以文本方式读取单元格的内容
int CStationSystem::ReadYBztToDB2(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;

	double dLayerHeight = 80 ;
	double dBoxWidth = 15 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("y_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;
				
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;
				
				strBlockName.MakeLower() ;
				if (strBlockName==_T("sbbzt_y1"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strYNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("YNUM"), strYNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bY[1-9]\\d*\\b"), strYNum))//检查strYNum是否是Y1，Y3的形式的字符串
					{
						iRet ++ ;

						if (!strYNum.Mid(1).IsEmpty())
						{
							iShelfNum = _tstoi(strYNum.Mid(1)) ;
						}	
						
						CString strBarMsg ;
						strBarMsg.Format(_T("读取移频柜%s"), strYNum) ;
						acedSetStatusBarProgressMeter(strBarMsg, 1, 10) ;	
						
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=1; iLayerIndex<11; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							pt_3d_cur_base.x = pt_3d_ref.x+((iLayerIndex-1)/2*dBoxWidth*2) ;
							pt_3d_cur_base.y = pt_3d_ref.y-((iLayerIndex-1)%2*dLayerHeight) ;
							//读取
							AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
							AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dBoxWidth, pt_3d_cur_base.y-dLayerHeight, 0) ;

							//here 20100616

							CArray<AcDbText *, AcDbText *> arrayY ;
							mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayY) ;
							mysys.SortEnt(arrayY, 2) ;
							CStringArray strArrayY ;
							mysys.GetHaveSortedText(arrayY, strArrayY) ;
							mysys.DestroyEntInArray(arrayY) ;

							int iFindTextSum = strArrayY.GetSize() ;
							if (iFindTextSum>0)
							{
								CString strGdName = _T(""), strGdFreq = _T("") ;

								for (int iFindIndex=0; iFindIndex<iFindTextSum; iFindIndex++)
								{
									CString strTextTmp = _T("") ;
									strTextTmp = strArrayY.GetAt(iFindIndex) ;
									strTextTmp.TrimLeft() ;
									strTextTmp.TrimRight() ;
									if (CBlkUtility::IsStrMatch(_T("^.+G[1-5]?$"), strTextTmp))
									{									
										strGdName = strTextTmp ;
									}
									else //if (CBlkUtility::IsStrMatch(_T("^[1,2]\\d{3}-[1,2]$"), strTextTmp)) //modified in 20100815 取消判断直接将文本复制过去
									{
										strGdFreq = strTextTmp ;
									}
								}

								try
								{
								strSql.Format(_T("insert into y_place(gd_name, frequency, shelf_num, layer_num) \
									values('%s','%s', %d,%d)"),\
									strGdName, strGdFreq, iShelfNum, iLayerIndex ) ;
								ExecSqlString(strSql) ;
								
								}
								catch (_com_error& e)
								{
									CString sBuff = CBlkUtility::GetErrorDescription(e) ;
									AfxMessageBox(sBuff) ;	
								}
								catch (...)
								{
									AfxMessageBox(_T("unknown error in insert gd freq")) ;									
								}
							}
					
						} //end for iLayerIndex	
						
						acedRestoreStatusBar() ;
					}// end ismatch					
					
				}//end if (strBlockName==_T("sbbzt_z1"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;

}

//读取表中的内容为文本形式
int CStationSystem::ReadZHBztToDB2(LPUNKNOWN pConnection, AcDbDatabase *pCurDb)
{
	int iRet = 0 ;

	double dLayerHeight = 19 ;
	double dLayerHeightTop = 4 ;
	double dLayerHeightDown = 5 ;
	double dBoxWidth = 18 ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;
	
	CString strSql ;
	_ConnectionPtr ptrCon ;
	ptrCon = (_ConnectionPtr)pConnection ;
	
	CDBUtility::ClearTable(_T("zh_place")) ;
	
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
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
				
				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName = NULL ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;
				
				CString strBlockName ;
				mysys.GetBlockName(pRef, strBlockName) ;

				strBlockName.MakeLower() ;
				if (strBlockName==_T("sbbzt_zh1"))
				{
					AcGePoint3d pt_3d_ref = pRef->position() ;
					//柜架编号
					CString strZHNum ;
					int iShelfNum = 1 ;
					mysys.GetAttValue(pRef, _T("ZHNUM"), strZHNum) ;
					if(CBlkUtility::IsStrMatch(_T("\\bZH[1-9]\\d*\\b"), strZHNum))//检查strZHNum是否是ZH1，ZH3的形式的字符串
					{
						iRet ++ ;

						if (!strZHNum.Mid(2).IsEmpty())
						{
							iShelfNum = _tstoi(strZHNum.Mid(2)) ;
						}	

						CString strBarMsg ;
						strBarMsg.Format(_T("读取综合柜%s"), strZHNum) ;
						acedSetStatusBarProgressMeter(strBarMsg, 1, 10) ;	
						
						//当前行起始（基准）点
						AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
						for (int iLayerIndex=1; iLayerIndex<11; iLayerIndex++)
						{
							acedSetStatusBarProgressMeterPos(iLayerIndex) ;

							pt_3d_cur_base.y = pt_3d_ref.y+(iLayerIndex-1)*dLayerHeight ;

							for (int iBoxIndex=1; iBoxIndex<9; iBoxIndex++)
							{
								//读取
								AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x+(iBoxIndex-1)*dBoxWidth, pt_3d_cur_base.y+dLayerHeight-dLayerHeightTop, 0) ;
								AcGePoint3d pt_3d_bottomright(pt_3d_topleft.x+dBoxWidth, pt_3d_cur_base.y+dLayerHeightDown, 0) ;
								
								//here 20100616
								
								CArray<AcDbText *, AcDbText *> arrayZH ;
								mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayZH) ;
								mysys.SortEnt(arrayZH, 2) ;
								CStringArray strArrayZH ;
								mysys.GetHaveSortedText(arrayZH, strArrayZH) ;
								mysys.DestroyEntInArray(arrayZH) ;
								
								int iFindTextSum = strArrayZH.GetSize() ;
								if (iFindTextSum>0)
								{
									CString strGdName = _T(""), strType = _T("") ;
									
									for (int iFindIndex=0; iFindIndex<iFindTextSum; iFindIndex++)
									{
										CString strTextTmp = _T("") ;
										strTextTmp = strArrayZH.GetAt(iFindIndex) ;
										strTextTmp.TrimLeft() ;
										strTextTmp.TrimRight() ;
										if (CBlkUtility::IsStrMatch(_T("^.+G[1-5]?$"), strTextTmp))
										{									
											strGdName = strTextTmp ;
										}
										else if (strTextTmp==_T("FS")||strTextTmp==_T("JS"))
										{
											strType = strTextTmp ;
										}
									}
									
									try
									{
									strSql.Format(_T("insert into zh_place(gd_name, shelf_num, layer_num, box_num, type) \
										values('%s', %d, %d, %d, '%s')"),\
										strGdName, iShelfNum, iLayerIndex, iBoxIndex, strType ) ;
									ExecSqlString(strSql) ;
									
									}
									catch (_com_error& e)
									{
										CString sBuff = CBlkUtility::GetErrorDescription(e) ;
										AfxMessageBox(sBuff) ;	
									}
									catch (...)
									{
										AfxMessageBox(_T("unknown error in insert gd to zh")) ;									
									}
								}

							}					
						} //end for iLayerIndex	
						
						acedRestoreStatusBar() ;
					}// end ismatch					
					
				}//end if (strBlockName==_T("sbbzt_zh1"))
			}
			pEnt->close() ;
		}
	}
	
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

//读入列控驱采数据
int CStationSystem::ReadLkQcDataToDb(CString strXlsFileName)
{
	int iReturn = 0 ;
	
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	CString strExec, strMsg ;
	//清除old数据
	CDBUtility::ClearTable("traincontrol_outin_place") ;
	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ("Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		//读取sheet1、sheet2
		CString strTitleCellContent[2] = {_T("驱动板"), _T("采集板")} ;
		for (int iSheetIndex=1; iSheetIndex<3; iSheetIndex++)
		{
			VARIANT vt_index ;
			vt_index.vt = VT_I2 ;
			vt_index.iVal = iSheetIndex ;
			
			pWorksheets  = m_pExcel->GetSheets( ) ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;
			
			//判断文件的合法性；
			BOOL bCheckFile = TRUE ;
			CString  strTitleCellName[1] = { _T("C3") } ;
			_variant_t range_var ;
			for ( int i = 0 ; i < 1 ; i++ )
			{
				pRange = NULL ;
				pRange = pSheet->GetRange( (_variant_t)strTitleCellName[i] ) ;
				range_var = pRange->GetText() ;
				pRange = NULL ;
				CString strVar ;
				strVar = (CString ) range_var.bstrVal ;
				strVar.TrimLeft() ;
				strVar.TrimRight() ;
				if( strVar.Left(6) != strTitleCellContent[iSheetIndex-1] )
				{
					bCheckFile = FALSE ;
					strMsg.Format(_T("sheet%d格式不符合：%s不是%s"),iSheetIndex, strTitleCellName[i], strTitleCellContent[iSheetIndex-1]) ;
					AfxMessageBox(strMsg) ;
					break ;
				}
			}

			if (bCheckFile)
			{
				int iCur_Col = 3 ; //起始列
				
				CString strTmp ;
				BOOL bNext = TRUE ; 
				
				while (bNext) 
				{			
					CString strCellName = _T(""), strCellText = _T("") ;

					strCellName.Format(_T("%s3"), CBlkUtility::GetExcelFieldName(iCur_Col)) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					range_var = pRange->GetText() ;
					strCellText = range_var.bstrVal ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					CString strReg = _T("") ;
					strReg.Format(_T("^%s[1-9]\\d*\\(JK(\\d*)-(\\d+)\\)$"), strTitleCellContent[iSheetIndex-1]) ;
					
					if (!CBlkUtility::IsStrMatch(strReg, strCellText)) 
					{
						bNext = FALSE ;
						continue ;
					}
					CString  strShelfNum = _T("") ;
					CBlkUtility::RegReplace(strReg, _T("$1"), strCellText, strShelfNum) ;
					CBlkUtility::RegReplace(strReg, _T("$2"), strCellText, strTmp) ;
					int iShelfNum = 1, iLayerNum = 0, iBoxNum = 0 ;
					iShelfNum = _tstoi(strShelfNum) ;
					iBoxNum = _tstoi(strTmp.Right(1)) ;
					iLayerNum = _tstoi(strTmp.Left(strTmp.GetLength()-2)) ;

					CString strBarMsg ;
					strBarMsg.Format(_T("读取sheet%d %s"), iSheetIndex, strCellText) ;
					acedSetStatusBarProgressMeter(strBarMsg, 5, 36) ;	
					
					//循环读入该列第5行到36行的数据
					for (int iRow=5; iRow<37; iRow++)
					{	
						acedSetStatusBarProgressMeterPos(iRow) ;
						strCellName.Format(_T("%s%d"), CBlkUtility::GetExcelFieldName(iCur_Col+1), iRow) ;
						pRange = NULL ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						range_var = pRange->GetText() ;
						strCellText = range_var.bstrVal ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						if (!strCellText.IsEmpty())
						{
							int iCurCellNum = 0 ;
							iCurCellNum = iRow-4 ;
							
							strExec.Format(_T("insert into traincontrol_outin_place(dev_name, shelf_num, layer_num, box_num, cell_num,type)\
								values('%s',%d,%d,%d,%d, %d)"), strCellText, iShelfNum, iLayerNum, iBoxNum, iCurCellNum, iSheetIndex-1) ;
							ExecSqlString(strExec) ;
							
						}
						
					}

					acedRestoreStatusBar() ;	
					
					iCur_Col+=3 ;					
				}
			}
			else
			{
				AfxMessageBox( _T("选择的文件不符合文件格式" )) ;			
			}			


		} //end for
		

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

int CStationSystem::Compute_18_outdoor_Circuit(int iMode)
{
	int iRet = 0 ;
	
	double dDwgHeight = 650 ;
	double dDwgWidth = 680 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 15 ;
	double dCellHeight2 = 10 ;
	double dCellWidth = 18 ;
	double dTbWidth = dCellWidth*7 ;
	double dBetweenTb = 10 ;
	int iRowNum = 15 ;
	int iRowNum2 = 20 ;
	int iTbNum = 2 ;
	
	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;//电路图
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from switch where dev_size=18 and dev_num=5 and other_id=0")) ;
		if (iMode==1)
		{
			strSql.Format(_T("select * from switch where dev_size=18 and dev_num=5 and other_id>0")) ;
		}
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if(cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}
			//////////////////////////////////////////////////////////////////////////
			int iRsCount = cSetAll.GetRecordCount() ;
			CString strBarMsg ;
			strBarMsg.Format(_T("正在计算18_d_3J+2X 室外电路")) ;
			if (iMode==1)
			{
				strBarMsg.Format(_T("正在计算18_s_3J+2X 室外电路")) ;
			}
			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
			while (!cSetAll.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iRet) ;

				CMapStringToString mapCircuitTypeToStr ;
				CADORecordset cSetFind ;

				CString strName = _T("") ;

				ads_point pt_base_draw = {0, 0, 0} ;
				ads_point pt_base_draw_tmp = {0, 0, 0} ;
				ads_point pt_base_draw2 = {0, 0, 0} ;
				ads_point pt_base_draw_tmp2 = {0, 0, 0} ;
				pt_base_draw[0] = (iRet/iRowNum)%iTbNum*(dTbWidth+dBetweenTb) ;
				pt_base_draw[1] = 0-(iRet/(iRowNum*iTbNum))*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight ;

				pt_base_draw2[0] = -100+(iRet/iRowNum2)%iTbNum*(dTbWidth+dBetweenTb) ;
				pt_base_draw2[1] = -350-(iRet/(iRowNum2*iTbNum))*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum2)*dCellHeight2 ;

				//道岔名称
				cSetAll.GetFieldValue(_T("name"), strName) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth/4 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strName, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				pt_base_draw_tmp2[0] = pt_base_draw2[0]+dCellWidth/4 ;
				pt_base_draw_tmp2[1] = pt_base_draw2[1]-dCellHeight2/2 ;
				//mysys.WriteText(pt_base_draw_tmp2, strName, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strName, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				acutPrintf(_T("\n************%d**************"), iRet) ;

				if (iRet==27)
				{
					int iiii = 0 ;
				}

				//TDF组合位置
				CString strTDF_Dev[5] ={ _T("J1"), _T("J2"), _T("J3"), _T("X1"), _T("X2")} ;
				CString strTDF_ZLoc[5] ={ _T(""), _T(""), _T(""), _T(""), _T("")} ;
				
				for(int iTDFIndex0=0; iTDFIndex0<5; iTDFIndex0++)
				{					
					strSql.Format(_T("select * from z_place where dev_name='%s-%s' and type='TDF' "), strName, strTDF_Dev[iTDFIndex0]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							int iTDFShelf = 0, iTDFLayer = 0 ;							
							cSetFind.GetFieldValue(_T("shelf_num"), iTDFShelf) ;
							cSetFind.GetFieldValue(_T("layer_num"), iTDFLayer) ;
							strTDF_ZLoc[iTDFIndex0].Format(_T("Z%d-%d"), iTDFShelf, iTDFLayer) ;	
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
				}
				for (int iTDFIndex02=0; iTDFIndex02<3; iTDFIndex02++)
				{
					pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth+dCellWidth/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]-(dCellHeight/3)*iTDFIndex02-dCellHeight/6 ;
					//mysys.WriteText(pt_base_draw_tmp, strTDF_ZLoc[iTDFIndex02], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strTDF_ZLoc[iTDFIndex02], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					
				}
				for (int iTDFIndex03=3; iTDFIndex03<5; iTDFIndex03++)
				{
					pt_base_draw_tmp2[0] = pt_base_draw2[0]+dCellWidth+dCellWidth/2 ;
					pt_base_draw_tmp2[1] = pt_base_draw2[1]-(dCellHeight2/2)*(iTDFIndex03-3)-dCellHeight2/4 ;
					//mysys.WriteText(pt_base_draw_tmp2, strTDF_ZLoc[iTDFIndex03], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strTDF_ZLoc[iTDFIndex03], pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				}
				
				//fx位置
				CString strTDF_FXLoc[5][5] ={
					{ _T(""), _T(""), _T(""), _T(""), _T("")},
					{ _T(""), _T(""), _T(""), _T(""), _T("")},
					{ _T(""), _T(""), _T(""), _T(""), _T("")},
					{ _T(""), _T(""), _T(""), _T(""), _T("")},
					{ _T(""), _T(""), _T(""), _T(""), _T("")},
				} ;

				for(int iTDFIndex=0; iTDFIndex<5; iTDFIndex++)
				{					
					strSql.Format(_T("select * from fx_data where dev_name='%s' and dev_type='%s' "), strName, strTDF_Dev[iTDFIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}
						int iCoreIndex = 0 ;
						while (!cSetFind.IsEOF())
						{
							int iTDFShelf = 0, iTDFLayer = 0, iTDFBox = 0, iTDFCell = 0 ;							
							cSetFind.GetFieldValue(_T("shelf_num"), iTDFShelf) ;
							cSetFind.GetFieldValue(_T("layer_num"), iTDFLayer) ;
							cSetFind.GetFieldValue(_T("box_num"), iTDFBox) ;
							cSetFind.GetFieldValue(_T("cell_num"), iTDFCell) ;
							strTDF_FXLoc[iTDFIndex][iCoreIndex].Format(_T("F%d-%d0%d-%d"), iTDFShelf, iTDFLayer, iTDFBox, iTDFCell) ;	

							iCoreIndex++ ;
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
				}
				for (int iTDFIndex2=0; iTDFIndex2<3; iTDFIndex2++)
				{
					for (int iCoreIndex=0; iCoreIndex<5; iCoreIndex++)
					{
						pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*(2+iCoreIndex)+dCellWidth/2 ;
						pt_base_draw_tmp[1] = pt_base_draw[1]-(dCellHeight/3)*iTDFIndex2-dCellHeight/6 ;
						//mysys.WriteText(pt_base_draw_tmp, strTDF_FXLoc[iTDFIndex2][iCoreIndex], 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strTDF_FXLoc[iTDFIndex2][iCoreIndex], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					}
				}
				for (int iTDFIndex3=3; iTDFIndex3<5; iTDFIndex3++)
				{
					for (int iCoreIndex=0; iCoreIndex<5; iCoreIndex++)
					{
						pt_base_draw_tmp2[0] = pt_base_draw2[0]+dCellWidth*(2+iCoreIndex)+dCellWidth/2 ;
						pt_base_draw_tmp2[1] = pt_base_draw2[1]-(dCellHeight2/2)*(iTDFIndex3-3)-dCellHeight2/4 ;
						//mysys.WriteText(pt_base_draw_tmp2, strTDF_FXLoc[iTDFIndex3][iCoreIndex], 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strTDF_FXLoc[iTDFIndex3][iCoreIndex], pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					}					
				}

				iRet++ ;
				cSetAll.MoveNext() ;
			}
			cSetAll.Close() ;

			//begin 绘制图框（包括电路示意图，表格等）
			int iDwgSum = iRsCount/(iRowNum*iTbNum)+(iRsCount%(iRowNum*iTbNum)==0?0:1) ;
			CString strDate ;
			strDate = CBlkUtility::GetDateStr() ;
			for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
			{
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
				xform.setToTranslation(vec); 
				CMapStringToString mapTypeToStr ;
				mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
		
				mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
				mapTypeToStr.SetAt(_T("4"), strDate) ;
				CString strDltTemplete ;
				strDltTemplete.Format(_T("%s\\support\\dlt\\dlt_dc_18_d_3J+2X_outdoor.dwg"), m_strSysBasePath ) ;
				if (iMode==1)
				{
					strDltTemplete.Format(_T("%s\\support\\dlt\\dlt_dc_18_s_3J+2X_outdoor.dwg"), m_strSysBasePath) ;
				}
				this->InsertDwgToDb(strDltTemplete, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
				
			}
			//end 绘制图框（包括电路示意图，表格等）
			CString strDrawType = _T("DLT_DC_18_D_3J+2X_OUTDOOR") ;
			if (iMode==1)
			{
				strDrawType = _T("DLT_DC_18_S_3J+2X_OUTDOOR") ;
			}
			mysys.WriteToRec(_T("xh_station"), _T("draw_type"), strDrawType) ;
			mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
			CString strSaveFilePath ;
			strSaveFilePath.Format(_T("%s\\18D_J(3J)+X(2J)-S700K-室外.dwg"), this->m_strSavePath) ;
			if (iMode==1)
			{
				strSaveFilePath.Format(_T("%s\\18S_J(3J)+X(2J)-S700K-室外.dwg"), this->m_strSavePath) ;
			}
			pCurDb->saveAs(strSaveFilePath) ;
			delete pCurDb ;
			pCurDb = NULL ;			
			
			acedRestoreStatusBar() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in Compute_18_outdoor_Circuit")) ;
	} 	
	
	return iRet ;
}

int CStationSystem::ComputerSwitchToDb()
{
	int iRet = 0 ;

	CDBUtility::ClearTable(_T("switch")) ;

	try
	{
		CADORecordset cSetAll, cSetFind ;
		
		CString strSql ;

		//先将道岔名称导入到表switch中
		strSql.Format(_T("select * from z_place where type='TDZ'")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if (cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}
			while (!cSetAll.IsEOF())
			{
				CString strCurName = _T("") ;
				int iShelfNUm = 0, iLayerNum = 0, iZTypeId = 0 ;

				cSetAll.GetFieldValue(_T("dev_name"), strCurName) ;
				cSetAll.GetFieldValue(_T("shelf_num"), iShelfNUm) ;
				cSetAll.GetFieldValue(_T("layer_num"), iLayerNum) ;
				cSetAll.GetFieldValue(_T("z_type_id"), iZTypeId) ;
	
				strSql.Format(_T("insert into switch(name,shelf_num, layer_num, z_type_id) values('%s',%d, %d, %d)"), strCurName, iShelfNUm, iLayerNum, iZTypeId) ;
				ExecSqlString(strSql) ;			

				cSetAll.MoveNext() ;
			}

			cSetAll.Close() ;
		}

		//开始分析填充表switch
		strSql.Format(_T("select * from switch order by id asc")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if (cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}
			while (!cSetAll.IsEOF())
			{
				CString strCurName = _T("") ;
				int iCurId = 0 ;
				int iCurTypeId = 0 ;
				int iOtherId = 0 ;

				cSetAll.GetFieldValue(_T("id"), iCurId) ;
				cSetAll.GetFieldValue(_T("name"), strCurName) ;
				cSetAll.GetFieldValue(_T("z_type_id"), iCurTypeId) ;

				int iSize = 0, iDevNum = 0 ;
				strSql.Format(_T("select * from import_dc_info where name='%s' and belong_station='%s'"), strCurName, this->m_strStationName) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						cSetFind.GetFieldValue(_T("dev_size"), iSize) ;
						cSetFind.GetFieldValue(_T("dev_num"), iDevNum) ;
						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}
				
				cSetAll.Edit() ;
				cSetAll.SetFieldValue(_T("dev_size"), iSize) ;
				cSetAll.SetFieldValue(_T("dev_num"), iDevNum) ;
				cSetAll.Update() ;

				//other_id已经有了
				cSetAll.GetFieldValue(_T("other_id"), iOtherId) ;
				if (iOtherId>0)
				{
					cSetAll.MoveNext() ;
					continue ;
				}
				
				strSql.Format(_T("select * from switch where name<>'%s' and z_type_id=%d"), strCurName, iCurTypeId) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						cSetFind.GetFieldValue(_T("id"), iOtherId) ;						
						
						cSetFind.MoveNext() ;
					}

					cSetFind.Close() ;
				}

				if (iOtherId>0)
				{
					cSetAll.Edit() ;
					cSetAll.SetFieldValue(_T("other_id"), iOtherId) ;
					cSetAll.Update() ;
				}
						
				
				cSetAll.MoveNext() ;
			}
			
			cSetAll.Close() ;
		}


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputerSwitchToDb")) ;
	} 

	return iRet ;
}

CString CStationSystem::GetXHJType(const CString strXHJName)
{
	CString strRet = _T("") ;

	if (strXHJName==_T("XDS")||strXHJName==_T("SDS"))
	{
		strRet =  _T("DS") ;
	}
	else if (strXHJName==_T("XDS-DH")||strXHJName==_T("SDS-DH"))
	{
		strRet = _T("DH") ;
	}
	else
	{
		try
		{
			CADORecordset cSetAll ;			
			CString strSql ;	
			CString strType = _T("") ;
			
			strSql.Format(_T("select * from z_place where dev_name='%s' and (type='JZZ' or type='JLZ' or type='CZZ' or type='DXZ')"), strXHJName) ;
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if (cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}
				while (!cSetAll.IsEOF())
				{					
					cSetAll.GetFieldValue(_T("type"), strType) ;
					cSetAll.MoveNext() ;
				}				
				cSetAll.Close() ;
			}
			if (strType==_T("JZZ"))
			{
				strRet = _T("JZA") ;
			}
			else if (strType==_T("JLZ"))
			{
				strRet = _T("JLA") ;
			}
			else if (strType==_T("CZZ"))
			{
				strRet = _T("CZ") ;
			}
			else if (strType==_T("DXZ"))
			{
				strRet = _T("DC") ;
			}


		}
		catch (_com_error& e)
		{
			CString sBuff = CBlkUtility::GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in GetXHJType")) ;
		} 


	}

	return strRet ;

}

//进路信号机 从ComputeJZCircuit 复制过来，所以变量命名规则是针对进站的
int CStationSystem::ComputeJlCircuit()
{
	int iRet = 0 ;

	double dDwgHeight = 297 ;
	double dDwgWidth = 420 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 10 ;
	double dCellWidth1 = 15 ;
	double dCellWidth2 = 18 ;
	int iRowNum = 8 ;
	
	CString strOut_J[8] = {_T("LXJ"),_T("ZXJ"),_T("TXJ"),_T("YXJ"),_T("LUXJ"),_T("DXJ"),_T("KDJ"),_T("XSJ")} ;
	CString strIn_J[12] = {_T("1DJ"),_T("2DJ"),_T("LXJ"),_T("LXJ13"),_T("ZXJ"),_T("ZXJ13"),_T("TXJ"),_T("LUXJ"),_T("DXJ"),_T("KDJ"),_T("SNKJ"),_T("SNJJ")} ;

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from z_place where type='JLZ' order by plus1")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		cSetAll.Open(g_PtrCon, strSql) ;
		if (!cSetAll.IsBOF())
		{
			cSetAll.MoveFirst() ;
		}

		int iRsCount = cSetAll.GetRecordCount() ;
		CString strBarMsg ;
		strBarMsg.Format(_T("正在计算进路信号机电路")) ;
		acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
		while (!cSetAll.IsEOF())
		{
			acedSetStatusBarProgressMeterPos(iRet) ;

			CADORecordset cSetFind ;

			CMapStringToString mapCircuitTypeToStr, mapCircuitTypeToStr2 ;

			int iUSUZIndexInBox = -1 ; //在USUZ中的序号0/1			
			CString strXHJName = _T("") ; //信号机名称
			CString strJZZLoc = _T("") ; //JZZ位置
			CString strUSUZLoc = _T("") ; //USUZ位置

			
			ads_point pt_base_draw = {0, 0, 0} ;
			ads_point pt_base_draw_tmp = {0, 0, 0} ;
			ads_point pt_base_draw_tmp2 = {0, 0, 0} ;

			pt_base_draw[1] = 0-(iRet/iRowNum)*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight-dCellHeight/2;

			//信号机名称
			cSetAll.GetFieldValue(_T("dev_name"), strXHJName) ;

			//点灯电路
			pt_base_draw_tmp[0] = dCellWidth1/2 ;
			pt_base_draw_tmp[1] = pt_base_draw[1] ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//驱动采集图
			pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+dCellWidth2/2 ;
			//mysys.WriteText(pt_base_draw_tmp, strXHJName, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strXHJName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;


			//JZZ组合位置
			pt_base_draw_tmp[0] = dCellWidth1+dCellWidth1/2 ;
			pt_base_draw_tmp2[0] = dDwgWidth+dBetweenDwg+dCellWidth2+dCellWidth2/2 ;
			
			int iJZZShelf = 0, iJZZLayer = 0 ;
			cSetAll.GetFieldValue(_T("shelf_num"), iJZZShelf) ;
			cSetAll.GetFieldValue(_T("layer_num"), iJZZLayer) ;
			strJZZLoc.Format(_T("Z%d-%d"), iJZZShelf, iJZZLayer) ;
			//					
			pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
			//mysys.WriteText(pt_base_draw_tmp, strJZZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strJZZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			//
			pt_base_draw_tmp2[1] = pt_base_draw_tmp[1] ;
			//mysys.WriteText(pt_base_draw_tmp2, strJZZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
			mysys.AddText(strJZZLoc, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			
			//USUZ组合位置
			CString strUSUZLocTmp = _T("") ;
			strSql.Format(_T("select * from z_place where dev_name='%s' and type='USUZ'"), strXHJName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}
				
				while (!cSetFind.IsEOF())
				{
					int iUSUZShelf = 0, iUSUZLayer = 0 ;
					cSetFind.GetFieldValue(_T("shelf_num"), iUSUZShelf) ;
					cSetFind.GetFieldValue(_T("layer_num"), iUSUZLayer) ;					
					strUSUZLocTmp.Format(_T("Z%d-%d"), iUSUZShelf, iUSUZLayer) ;
					//index in box	
					cSetFind.GetFieldValue(_T("index_in_box"), iUSUZIndexInBox) ;	
					
					strUSUZLoc.Format(_T("%s(%d)"), strUSUZLocTmp, iUSUZIndexInBox+1) ;					
									
					cSetFind.MoveNext() ;
				}
				
				cSetFind.Close() ;
			}
			pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;
			if (strUSUZLoc.IsEmpty())
			{
				AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
				AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
				mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

				pt_3d_line_start.x = pt_base_draw_tmp2[0]-2.5 ;
				pt_3d_line_end.x = pt_base_draw_tmp2[0]+2.5 ;
				mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
			}
			else
			{
				//mysys.WriteText(pt_base_draw_tmp, strUSUZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strUSUZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				pt_base_draw_tmp2[1] = pt_base_draw_tmp[1] ;
				//mysys.WriteText(pt_base_draw_tmp2, strUSUZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strUSUZLoc, pt_base_draw_tmp2, pt_base_draw_tmp2, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
			}
			
			//JZZ至分线盘端子 
			strSql.Format(_T("select * from fx_data where  dev_name='%s'and dev_type='JLA' "), strXHJName) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (!cSetFind.IsBOF())
				{
					cSetFind.MoveFirst() ;
				}

				int iFxIndex = 0 ;
				while (!cSetFind.IsEOF())
				{
					CString strCoreName = _T("") ;
					int iFxShelfNum = 0, iFxLayerNum = 0, iFxBoxNum = 0, iFxCellNum = 0 ;
					cSetFind.GetFieldValue(_T("core_name"), strCoreName) ;
					cSetFind.GetFieldValue(_T("shelf_num"), iFxShelfNum) ;
					cSetFind.GetFieldValue(_T("layer_num"), iFxLayerNum) ;
					cSetFind.GetFieldValue(_T("box_num"), iFxBoxNum) ;
					cSetFind.GetFieldValue(_T("cell_num"), iFxCellNum) ;
					CString strFxLoc = _T("") ;
					strFxLoc.Format(_T("F%d-%d0%d-%d"), iFxShelfNum, iFxLayerNum, iFxBoxNum, iFxCellNum) ;
					if (strCoreName==_T("1U"))
					{
						//无USUZ
						if (iUSUZIndexInBox==-1) 
						{
							pt_base_draw_tmp[0] = 2*dCellWidth1+dCellWidth1/2 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
							//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;	
							mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
							
							pt_base_draw_tmp[0] = 11*dCellWidth1+dCellWidth1/2 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("B"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("B"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							AcGePoint3d pt_3d_cen(pt_base_draw_tmp[0], pt_base_draw_tmp[1],0) ;
							mysys.AddAcDbCircle(pt_3d_cen, 2) ;
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
							
							pt_base_draw_tmp[0] = 12*dCellWidth1+dCellWidth1/2 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("A"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("A"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							pt_3d_cen.x = pt_base_draw_tmp[0] ;
							mysys.AddAcDbCircle(pt_3d_cen, 2) ;
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
						}
						else
						{
							pt_base_draw_tmp[0] = 2*dCellWidth1+6 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("至"), 0, AcDb::kTextMid, 2.5, 0.8) ;	
							mysys.AddText(_T("至"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.8, 0, 0) ;
							pt_base_draw_tmp[0] = 2*dCellWidth1+11 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("A"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("A"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							AcGePoint3d pt_3d_cen(pt_base_draw_tmp[0], pt_base_draw_tmp[1],0) ;
							mysys.AddAcDbCircle(pt_3d_cen, 2) ;
							AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
							
							pt_base_draw_tmp[0] = 11*dCellWidth1+dCellWidth1/2 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4 ;
							//mysys.WriteText(pt_base_draw_tmp, _T("至 1U"), 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText(_T("至 1U"), pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							pt_3d_line_start.y = pt_base_draw_tmp[1]+dCellHeight/2 ;
							pt_3d_line_end.y = pt_base_draw_tmp[1]+dCellHeight/2 ;						
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
							
							pt_base_draw_tmp[0] = 12*dCellWidth1+dCellWidth1/2 ;
							//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;	
							mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							pt_3d_line_start.x = pt_base_draw_tmp[0]-2.5 ;
							pt_3d_line_end.x = pt_base_draw_tmp[0]+2.5 ;
							mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
						}					

						cSetFind.MoveNext() ;
						continue ;
					}
					
					pt_base_draw_tmp[0] = (3+iFxIndex)*dCellWidth1+dCellWidth1/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]+dCellHeight/4 ;
					//mysys.WriteText(pt_base_draw_tmp, strFxLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strFxLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
					AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1]-dCellHeight/2, 0) ;
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

					iFxIndex++ ;
					cSetFind.MoveNext() ;
				}

				cSetFind.Close() ;
			}

			//驱动采集out LXJ-1 ZXJ-1 TXJ-1 YXJ-1 KDJ-1 SNKJ-1
			for (int iOutJ=0; iOutJ<8; iOutJ++)
			{
				CString strJkLoc = _T("") ;
				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=0"), strXHJName, strOut_J[iOutJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{	
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;						
				
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(2+iOutJ)*dCellWidth2 + dCellWidth2/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]+(iOutJ==7?-1:1)*dCellHeight/4 ;
				
				AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
				AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
				if (strJkLoc==_T(""))
				{
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				}

				pt_base_draw_tmp[1] = pt_base_draw[1]+(iOutJ==7?1:-1)*dCellHeight/4 ;
				pt_3d_line_start.y = pt_3d_line_end.y = pt_base_draw_tmp[1] ;
				mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
			}


			//驱动采集in DJ-12 YXJ-12....
			for (int iInJ=0; iInJ<12; iInJ++)
			{
				CString strJkLoc = _T("") ;			
				strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=1"), strXHJName, strIn_J[iInJ]) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iJkLayerNum = 0, iJkBoxNum = 0, iJkCellNum = 0 ;
						cSetFind.GetFieldValue(_T("layer_num"), iJkLayerNum) ;
						cSetFind.GetFieldValue(_T("box_num"), iJkBoxNum) ;
						cSetFind.GetFieldValue(_T("cell_num"), iJkCellNum) ;
						
						strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayerNum, iJkBoxNum, iJkCellNum ) ;
							
						cSetFind.MoveNext() ;
					}
					
					cSetFind.Close() ;
				}

				pt_base_draw_tmp[0] = dDwgWidth+dBetweenDwg+(10+iInJ)*dCellWidth2 + dCellWidth2/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]+(iInJ>9?-1:1)*dCellHeight/4 ;

				AcGePoint3d pt_3d_line_start(pt_base_draw_tmp[0]-2.5, pt_base_draw_tmp[1], 0) ;
				AcGePoint3d pt_3d_line_end(pt_base_draw_tmp[0]+2.5, pt_base_draw_tmp[1], 0) ;
				if (strJkLoc==_T(""))
				{
					mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;
				}
				else
				{
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				}
				pt_base_draw_tmp[1] = pt_base_draw[1]+(iInJ>9?1:-1)*dCellHeight/4 ;
				pt_3d_line_start.y = pt_3d_line_end.y = pt_base_draw_tmp[1] ;
				mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

			}		
			

			iRet++ ;
			cSetAll.MoveNext() ;
		}
		cSetAll.Close() ;

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = iRsCount/iRowNum+(iRsCount%iRowNum==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("进路信号机点灯电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_jl_dd.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
			
			vec.x = dDwgWidth+dBetweenDwg ;
			xform.setToTranslation(vec); 
			strDwgType.Format(_T("进路信号机驱动采集电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_xhj_jl_qc.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
		}
		//end 绘制图框（包括电路示意图，表格等）


		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_JL_DD")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\进路信号机电路图.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
		delete pCurDb ;
		pCurDb = NULL ;

		acedRestoreStatusBar() ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeJlCircuit")) ;
	} 

	return iRet ;
}


int CStationSystem::Compute_18_d_indoor_Circuit()
{
	int iRet = 0 ;
	
	double dDwgHeight = 297 ;
	double dDwgWidth = 1070 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 25 ;
	double dCellWidth = 18 ;
	double dTbWidth = dCellWidth*6 ;
	double dBetweenTb = 10 ;
	int iRowNum = 9 ;
	int iTbNum = 4 ;	

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;//电路图
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from switch where dev_size=18 and dev_num=5 and other_id=0")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if(cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}

			int iRsCount = cSetAll.GetRecordCount() ;
			CString strBarMsg ;
			strBarMsg.Format(_T("正在计算18_d_3J+2X 室内电路")) ;
			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
			while (!cSetAll.IsEOF())
			{
				acedSetStatusBarProgressMeterPos(iRet) ;
				
				CMapStringToString mapCircuitTypeToStr ;
				CADORecordset cSetFind ;

				CString strName = _T("") ;

				ads_point pt_base_draw = {0, 0, 0} ;
				ads_point pt_base_draw_tmp = {0, 0, 0} ;
				pt_base_draw[0] = (iRet/iRowNum)%iTbNum*(dTbWidth+dBetweenTb) ;
				pt_base_draw[1] = 0-(iRet/(iRowNum*iTbNum))*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight ;

				//道岔名称
				cSetAll.GetFieldValue(_T("name"), strName) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth/4 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strName, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//TDZ组合位置
				CString strTDZLoc = _T("") ;
				int iTDZShelf = 0, iTDZLayer = 0 ;
				cSetAll.GetFieldValue(_T("shelf_num"), iTDZShelf) ;
				cSetAll.GetFieldValue(_T("layer_num"), iTDZLayer) ;
				strTDZLoc.Format(_T("Z%d-%d"), iTDZShelf, iTDZLayer) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth+dCellWidth/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strTDZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strTDZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//BHZ组合位置
				CString strBHZLoc = _T("") ;
				strSql.Format(_T("select * from z_place where dev_name='%s' and type='BHZ'"), strName) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iBHZShelf = 0, iBHZLayer = 0;
						cSetFind.GetFieldValue(_T("shelf_num"), iBHZShelf) ;
						cSetFind.GetFieldValue(_T("layer_num"), iBHZLayer) ;
						strBHZLoc.Format(_T("Z%d-%d"), iBHZShelf, iBHZLayer) ;

						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*3+dCellWidth/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strBHZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strBHZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//GJ-Z/ GJ-Q组合位置
				int iGdShelf = 0, iGdLayer = 0 ; //轨道所在组合位置
				CString strGJLoc = _T("") ; //轨道组合位置字符串
				CString strGJZNode = _T(""), strGJQNode = _T("") ;//类似02-13
				CString strGJZAll = _T(""), strGJQAll = _T("") ; //类似Z2-502-13
				CString strGdName = _T("") ; //道岔所在轨道的名称
				CString strSwitchCircuitType = _T("") ; //一体化轨道电路和25Hz轨道电路,(ZGZ/GFZ)
				int iJLoc = 0 ; //所用继电器位置
				int iSwitchGdType = 0 ; //道岔所在轨道类型，0表示该轨道命名由1个道岔名，比如3DG；1表示由2个，比如18-32DG
				int iSwitchInGdIndex = 0 ;//在类似18-32DG中18的iSwitchInGdIndex为0，32的iSwitchInGdIndex为1
				strSql.Format(_T("select * from z_gd_place where gd_name like '%%%s%%DG'"), strName) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					BOOL bFound = FALSE ;
					while (!cSetFind.IsEOF())
					{					

						CString strGdNameTmp = _T("") ;
						cSetFind.GetFieldValue(_T("gd_name"), strGdNameTmp) ;
						if (strGdNameTmp.Right(2)==_T("DG"))
						{
							if (strGdNameTmp.Left(strGdNameTmp.GetLength()-2)==strName)
							{
								iSwitchGdType = 0 ;
								strGdName = strGdNameTmp ;
								bFound = TRUE ;								
							}
							else if (strGdNameTmp.Find(_T("-"))!=-1)
							{
								CString strGdNameTmp2 = strGdNameTmp.Left(strGdNameTmp.GetLength()-2) ;
								CStringArray strArraySwitch ;
								CBlkUtility::DivideString(strGdNameTmp2, _T("-"), strArraySwitch) ;								
								for (int iSwitchIndex=0; iSwitchIndex<strArraySwitch.GetSize(); iSwitchIndex++)
								{									
									if (strArraySwitch.GetAt(iSwitchIndex)==strName)
									{
										iSwitchGdType = 1 ;
										strGdName = strGdNameTmp ;
										iSwitchInGdIndex = iSwitchIndex ;
										bFound = TRUE ;										
									}
								}
							}
						}

						if (bFound)
						{
							cSetFind.GetFieldValue(_T("shelf_num"), iGdShelf) ;
							cSetFind.GetFieldValue(_T("layer_num"), iGdLayer) ;
							cSetFind.GetFieldValue(_T("j_loc"), iJLoc) ;
							strGJLoc.Format(_T("Z%d-%d"), iGdShelf, iGdLayer) ;
							break ;
						}	
						
						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}
				//查询组合类型是ZGZ / GFZ
				if (!strGJLoc.IsEmpty())
				{
					strSql.Format(_T("select * from z_type where shelf_num=%d and layer_num=%d"), iGdShelf, iGdLayer) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							cSetFind.GetFieldValue(_T("type"), strSwitchCircuitType) ;
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
				}
				//查找GJ-Z GJ-Q 端子
				if (!strSwitchCircuitType.IsEmpty())
				{
					if (strSwitchCircuitType==_T("ZGZ"))
					{
						iJLoc = (iJLoc-1)/2+1 ;
					}
					int iLocFirstNum[3] = {3,4,5} ;

					strSql.Format(_T("select * from node_collect where dwg_type='%s' and index=%d and left(loc,1)='%d'"), strSwitchCircuitType, iJLoc, iLocFirstNum[iSwitchInGdIndex] ) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							CString strNodeTmp = _T("") ;
							CString strLoc = _T("") ;
							cSetFind.GetFieldValue(_T("node"), strNodeTmp) ;
							cSetFind.GetFieldValue(_T("loc"), strLoc) ;
							if (strLoc.Right(1)==_T("1"))
							{
								strGJZNode = strNodeTmp ;
							}
							else if (strLoc.Right(1)==_T("2"))
							{
								strGJQNode = strNodeTmp ;
							}
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
				}
				strGJZAll.Format(_T("%s%s"),strGJLoc, strGJZNode) ;
				strGJQAll.Format(_T("%s%s"),strGJLoc, strGJQNode) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*4+dCellWidth/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strGJZAll, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strGJZAll, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*5+dCellWidth/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strGJQAll, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strGJQAll, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				
				//TDF组合位置
				CString strTDF_Dev[5] ={ _T("J1"), _T("J2"), _T("J3"), _T("X1"), _T("X2")} ;
				CString strTDF_ZLoc[5] ={ _T(""), _T(""), _T(""), _T(""), _T("")} ;

				for(int iTDFIndex=0; iTDFIndex<5; iTDFIndex++)
				{					
					strSql.Format(_T("select * from z_place where dev_name='%s-%s' and type='TDF' "), strName, strTDF_Dev[iTDFIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							int iTDFShelf = 0, iTDFLayer = 0 ;							
							cSetFind.GetFieldValue(_T("shelf_num"), iTDFShelf) ;
							cSetFind.GetFieldValue(_T("layer_num"), iTDFLayer) ;
							strTDF_ZLoc[iTDFIndex].Format(_T("Z%d-%d"), iTDFShelf, iTDFLayer) ;	
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
				}
				for (int iTDFIndex2=0; iTDFIndex2<5; iTDFIndex2++)
				{
					pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*2+dCellWidth/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]-(dCellHeight/5)*iTDFIndex2-dCellHeight/10 ;
					//mysys.WriteText(pt_base_draw_tmp, strTDF_ZLoc[iTDFIndex2], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strTDF_ZLoc[iTDFIndex2], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				}

				iRet++ ;
				cSetAll.MoveNext() ;
			}
			cSetAll.Close() ;

			//begin 绘制图框（包括电路示意图，表格等）
			int iDwgSum = iRsCount/(iRowNum*iTbNum)+(iRsCount%(iRowNum*iTbNum)==0?0:1) ;
			CString strDate ;
			strDate = CBlkUtility::GetDateStr() ;
			for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
			{
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
				xform.setToTranslation(vec); 
				CMapStringToString mapTypeToStr ;
				mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
				CString strDwgType ; 
				strDwgType.Format(_T("单动道岔(3J+2X)室内控制电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
				mapTypeToStr.SetAt(_T("2"), strDwgType) ;
				mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
				mapTypeToStr.SetAt(_T("4"), strDate) ;
				CString strDltFile ;
				strDltFile.Format(_T("%s\\support\\dlt\\dlt_dc_18_d_3J+2X_indoor.dwg"), m_strSysBasePath) ;
				this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
				
			}	

			//end 绘制图框（包括电路示意图，表格等）
			
			mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_DC_18_D_3J+2X_INDOOR")) ;
			mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
			CString strSaveFilePath ;
			strSaveFilePath.Format(_T("%s\\18D_J(3J)+X(2J)-S700K-室内.dwg"), this->m_strSavePath) ;
			pCurDb->saveAs(strSaveFilePath) ;
			delete pCurDb ;
			pCurDb = NULL ;	
			
			acedRestoreStatusBar() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in Compute_18_d_indoor_Circuit")) ;
	} 	
	
	return iRet ;
}

void CStationSystem::ComputeCircuitForPx(const CString strDwgFile)
{

    AcDbDatabase *pCurDb = new AcDbDatabase(false) ;
	if (Acad::eOk==pCurDb->readDwgFile(strDwgFile))
	{		
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		//过渡电路图相关变量
		AcDbDatabase *pSaveDb = new AcDbDatabase ;
		CUserSystem mysys2 ;
		mysys2.m_pDb = pSaveDb;
		int iPxCircuitIndex = 0 ; // 过渡电路图的index
		CString strSaveFile = _T("") ;		
		int iLastMark = strDwgFile.ReverseFind(_T('\\')) ;
		strSaveFile.Format(_T("%s\\配线专用-%s"), strDwgFile.Left(iLastMark), strDwgFile.Mid(iLastMark+1)) ;
		
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
				//if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
				{
					AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;
					
					//得到块名
// 					AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 					AcDbObjectId blkTblObjId ;
// 					blkTblObjId = pRef->blockTableRecord() ;
// 					acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 					TCHAR * chBlockName = NULL ;
// 					pBlkTblRecTmp->getName(chBlockName) ;
// 					pBlkTblRecTmp->close() ;
					
					CString strBlockName ;
					mysys.GetBlockName(pRef, strBlockName) ;

					AcGePoint3d pt_3d_ref = pRef->position() ; //块基点

					//计算一个块内填充的设备的电路
					
					this->ComputeOneTypeCircuit(strBlockName, pt_3d_ref, pCurDb, pSaveDb, iPxCircuitIndex) ;
					this->ComputeOneTypeCircuit2(strBlockName, pt_3d_ref, pCurDb, pSaveDb, iPxCircuitIndex) ;

				}
				pEnt->close() ;
			}
		}
		
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;

		//保存过渡配线电路图
		mysys2.WriteToRec(_T("second_draw"), _T("draw_type"), _T("CIRCUIT")) ;
		mysys2.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;	
		if (iPxCircuitIndex>0)
		{
			pSaveDb->saveAs(strSaveFile) ;
		}
		delete pSaveDb ;
		pSaveDb = NULL ;
		
	}
	
	delete pCurDb ;
	pCurDb = NULL ;

}

void CStationSystem::ReplaceSpecialChar()
{
	try
	{
		CADORecordset cSetAll ;
		CString strSql ;

		CUserSystem mysys ;
		int iProcSum = 0 ;

		//驱动采集数据
		CString strTbName[2] = {_T("interlock_outin_place"), _T("traincontrol_outin_place")} ;
		for (int i=0; i<2; i++)
		{
			iProcSum = 0 ;
			strSql.Format(_T("select * from %s"), strTbName[i]) ;
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if(cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}
				while (!cSetAll.IsEOF())
				{
					CString strDevName = _T("") ;

					CString strDevNameNew = _T("") ;
					cSetAll.GetFieldValue(_T("dev_name"), strDevName) ;

					if (CBlkUtility::IsStrMatch(_T("^((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))(G[1-3]?)$"), strDevName))
					{
						CString strPart1 = _T("") ;
						CString strPart1New = _T("") ;
						CString strPart2 = _T("") ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?)$"), _T("$1"), strDevName, strPart1) ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?)$"), _T("$2"), strDevName, strPart2) ;
						this->ConvertSpecialChar(strPart1, strPart1New) ;
						strDevNameNew.Format(_T("%s%s"), strPart1New, strPart2) ;
					}
					else if (CBlkUtility::IsStrMatch(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)([^IV]+)$"), strDevName))
					{
						if (!CBlkUtility::IsStrMatch(_T("^([S,X])XSJ$"), strDevName))
						{
							CString strPart1 = _T("") ;						
							CString strPart2 = _T("") ;
							CString strPart2New = _T("") ;
							CString strPart3 = _T("") ;
							CBlkUtility::RegReplace(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)([^IV]+)$"), _T("$1"),strDevName, strPart1) ;
							CBlkUtility::RegReplace(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)([^IV]+)$"), _T("$2"),strDevName, strPart2) ;
							CBlkUtility::RegReplace(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)([^IV]+)$"), _T("$3"),strDevName, strPart3) ;
							this->ConvertSpecialChar(strPart2, strPart2New) ;
							strDevNameNew.Format(_T("%s%s%s"), strPart1, strPart2New, strPart3) ;
						}
					}
					else if (CBlkUtility::IsStrMatch(_T("^(I{1,3}|IV|VI{0,3}|IX|X)G[1-3]?_[G,F]J$"), strDevName))
					{
						CString strPart1 = _T("") ;
						CString strPart1New = _T("") ;
						CString strPart2 = _T("") ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?_[G,F]J)$"), _T("$1"), strDevName, strPart1) ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?_[G,F]J)$"), _T("$2"), strDevName, strPart2) ;
						this->ConvertSpecialChar(strPart1, strPart1New) ;
						strDevNameNew.Format(_T("%s%s"), strPart1New, strPart2) ;
					}

					if (strDevNameNew!=strDevName&&strDevNameNew!=_T(""))
					{
						cSetAll.Edit() ;
						cSetAll.SetFieldValue(_T("dev_name"), strDevNameNew) ;
						cSetAll.Update() ;
						acutPrintf(_T("\n%s-%s"), strDevName, strDevNameNew) ;
						iProcSum++ ;
					}


					cSetAll.MoveNext() ;
				}
				cSetAll.Close() ;
				
				acutPrintf(_T("\n%s处理个数:%d"), strTbName[i], iProcSum) ;
			}

		}

		//移频柜、综合柜、轨道柜、轨道组合布置
		CString strTbName2[4] = {_T("z_gd_place"), _T("y_place"),_T("zh_place"),_T("g_place")} ;
		for(int i2=0; i2<4; i2++)
		{
			iProcSum = 0 ;
			strSql.Format(_T("select * from %s"), strTbName2[i2]) ;
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if(cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}
				
				while (!cSetAll.IsEOF())
				{
					CString strDevName = _T("") ;				
					CString strDevNameNew = _T("") ;
					cSetAll.GetFieldValue(_T("gd_name"), strDevName) ;
					
					if (CBlkUtility::IsStrMatch(_T("^(I{1,3}|IV|VI{0,3}|IX|X)G[1-3]?$"), strDevName))
					{
						CString strPart1 = _T("") ;
						CString strPart1New = _T("") ;
						CString strPart2 = _T("") ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?)$"), _T("$1"), strDevName, strPart1) ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?)$"), _T("$2"), strDevName, strPart2) ;
						this->ConvertSpecialChar(strPart1, strPart1New) ;
						strDevNameNew.Format(_T("%s%s"), strPart1New, strPart2) ;
					}
					if (strDevNameNew!=strDevName&&strDevNameNew!=_T(""))
					{
						cSetAll.Edit() ;
						cSetAll.SetFieldValue(_T("gd_name"), strDevNameNew) ;
						cSetAll.Update() ;
						acutPrintf(_T("\n%s-%s"), strDevName, strDevNameNew) ;
						iProcSum++ ;
					}
					
					cSetAll.MoveNext() ;
				}
				cSetAll.Close() ;
				
				acutPrintf(_T("\n%s处理个数:%d"), strTbName2[i2], iProcSum) ;
			}
		}

		//组合柜、
		CString strTbName3[2] = {_T("z_place"), _T("fxp_place")} ;
		for(int i3=0; i3<2; i3++)
		{
			iProcSum = 0 ;
			if (i3==0)
			{
				strSql.Format(_T("select * from %s where type in('JZZ','JLZ','CZZ','DXZ','USUZ')"), strTbName3[i3]) ;
			}
			else
			{
				strSql.Format(_T("select * from %s where zuhe_type in(1,2,3,4)"), strTbName3[i3]) ;
			}
			if (cSetAll.IsOpen())
			{
				cSetAll.Close() ;
			}
			if(cSetAll.Open(g_PtrCon, strSql))
			{
				if (!cSetAll.IsBOF())
				{
					cSetAll.MoveFirst() ;
				}
				
				while (!cSetAll.IsEOF())
				{
					CString strDevName = _T("") ;				
					CString strDevNameNew = _T("") ;
					cSetAll.GetFieldValue(_T("dev_name"), strDevName) ;
					
					if (CBlkUtility::IsStrMatch(_T("^(I{1,3}|IV|VI{0,3}|IX|X)G[1-3]?$"), strDevName))
					{
						CString strPart1 = _T("") ;
						CString strPart1New = _T("") ;
						CString strPart2 = _T("") ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?)$"), _T("$1"), strDevName, strPart1) ;
						CBlkUtility::RegReplace(_T("^(I{1,3}|IV|VI{0,3}|IX|X)(G[1-3]?)$"), _T("$2"), strDevName, strPart2) ;
						this->ConvertSpecialChar(strPart1, strPart1New) ;
						strDevNameNew.Format(_T("%s%s"), strPart1New, strPart2) ;
					}
					else if (CBlkUtility::IsStrMatch(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)$"), strDevName))
					{
						CString strPart1 = _T("") ;						
						CString strPart2 = _T("") ;
						//CString strPart3 = _T("") ;
						CString strPart2New = _T("") ;
 						CBlkUtility::RegReplace(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)$"), _T("$1"),strDevName, strPart1) ;
						CBlkUtility::RegReplace(_T("^([S,X])(I{1,3}|IV|VI{0,3}|IX|X)$"), _T("$2"),strDevName, strPart2) ;
						//CBlkUtility::RegReplace(_T("^([S,X])(.*)(I{1,3}|IV|VI{0,3}|IX|X)$"), _T("$3"),strDevName, strPart3) ;
						this->ConvertSpecialChar(strPart2, strPart2New) ;
						strDevNameNew.Format(_T("%s%s"), strPart1, strPart2New) ;
					}
					else if (CBlkUtility::IsStrMatch(_T("^(BX|BS)(I{1,3}|IV|VI{0,3}|IX|X)$"), strDevName))
					{
						CString strPart1 = _T("") ;						
						CString strPart2 = _T("") ;
						//CString strPart3 = _T("") ;
						CString strPart2New = _T("") ;
						CBlkUtility::RegReplace(_T("^(BX|BS)(I{1,3}|IV|VI{0,3}|IX|X)$"), _T("$1"),strDevName, strPart1) ;
						CBlkUtility::RegReplace(_T("^(BX|BS)(I{1,3}|IV|VI{0,3}|IX|X)$"), _T("$2"),strDevName, strPart2) ;
						//CBlkUtility::RegReplace(_T("^(BX|BS)(.*)(I{1,3}|IV|VI{0,3}|IX|X)$"), _T("$3"),strDevName, strPart3) ;
						this->ConvertSpecialChar(strPart2, strPart2New) ;
						strDevNameNew.Format(_T("%s%s"), strPart1, strPart2New) ;
					}
					if (strDevNameNew!=strDevName&&strDevNameNew!=_T(""))
					{
						cSetAll.Edit() ;
						cSetAll.SetFieldValue(_T("dev_name"), strDevNameNew) ;
						cSetAll.Update() ;
						acutPrintf(_T("\n%s-%s"), strDevName, strDevNameNew) ;
						iProcSum++ ;
					}
					
					cSetAll.MoveNext() ;
				}
				cSetAll.Close() ;
				
				acutPrintf(_T("\n%s处理个数:%d"), strTbName3[i3], iProcSum) ;
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
		AfxMessageBox(_T("unknown error in ConvertSpecialChar")) ;
	} 

}

//该函数iMode参数为后加，=0，=1的处理风格差异较大，临时使用
BOOL CStationSystem::ConvertSpecialChar(const CString strSrc, CString &strRet,  int iMode)
{
	BOOL bRet = FALSE ;
	CString strTmp[10] = {_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX"),_T("X")} ;
	CString strTmp2[10] = {_T("%%161"),_T("%%162"),_T("%%163"),_T("%%164"),_T("%%165"),_T("%%166"),_T("%%167"),_T("%%168"),_T("%%169"),_T("%%170")} ;
	if (iMode==0)
	{
		for (int i=0; i<10; i++)
		{
			if (strSrc==strTmp[i])
			{
				strRet.Format(_T("%%%%%d"), 161+i) ;

				bRet = TRUE ;
				break ;
			}

		}
	}
	else
	{
		CString strSrcTmp = strSrc ;
		for (int i=0; i<10; i++)
		{
			if (strSrcTmp.Replace(strTmp2[i],strTmp[i])!=0)
			{
				strRet = strSrcTmp ;
				bRet = TRUE ;
				break ;
			}

		}

	}


	return bRet ;

}

void CStationSystem::DoAll(const CString strSavePath)
{

}

int CStationSystem::Compute_18_d_qc_Circuit()
{
	int iRet = 0 ;
	
	double dDwgHeight = 297 ;
	double dDwgWidth = 823 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 15 ;
	double dCellWidth = 19 ;
	double dTbWidth = dCellWidth*12 ;
	double dBetweenTb = 10 ;
	int iRowNum = 14 ;
	int iTbNum = 3 ;	

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;//电路图
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from switch where dev_size=18 and dev_num=5 and other_id=0")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if(cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}

			int iRsCount = cSetAll.GetRecordCount() ;
			CString strBarMsg ;
			strBarMsg.Format(_T("正在计算18_d_3J+2X 驱采电路")) ;
			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
			while (!cSetAll.IsEOF())
			{			
				acedSetStatusBarProgressMeterPos(iRet) ;

				CMapStringToString mapCircuitTypeToStr ;
				CADORecordset cSetFind ;

				CString strName = _T("") ;

				ads_point pt_base_draw = {0, 0, 0} ;
				ads_point pt_base_draw_tmp = {0, 0, 0} ;
				pt_base_draw[0] = (iRet/iRowNum)%iTbNum*(dTbWidth+dBetweenTb) ;
				pt_base_draw[1] = 0-(iRet/(iRowNum*iTbNum))*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight ;

				//道岔名称
				cSetAll.GetFieldValue(_T("name"), strName) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/2 ;
				//mysys.WriteText(pt_base_draw_tmp, strName, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//TDZ组合位置
				CString strTDZLoc = _T("") ;
				int iTDZShelf = 0, iTDZLayer = 0 ;
				cSetAll.GetFieldValue(_T("shelf_num"), iTDZShelf) ;
				cSetAll.GetFieldValue(_T("layer_num"), iTDZLayer) ;
				strTDZLoc.Format(_T("Z%d-%d"), iTDZShelf, iTDZLayer) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth+dCellWidth/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/6 ;
				//mysys.WriteText(pt_base_draw_tmp, strTDZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strTDZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//BHZ组合位置
				CString strBHZLoc = _T("") ;
				strSql.Format(_T("select * from z_place where dev_name='%s' and type='BHZ'"), strName) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						int iBHZShelf = 0, iBHZLayer = 0;
						cSetFind.GetFieldValue(_T("shelf_num"), iBHZShelf) ;
						cSetFind.GetFieldValue(_T("layer_num"), iBHZLayer) ;
						strBHZLoc.Format(_T("Z%d-%d"), iBHZShelf, iBHZLayer) ;

						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/3-dCellHeight/6 ;
				//mysys.WriteText(pt_base_draw_tmp, strBHZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strBHZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
				
				CString strOutZHName[7] = {_T("DCJ"),_T("FCJ"),_T("YCJ"),_T("SJ"),_T("JGAJ"),_T("XGAJ"),_T("FWJ")} ;
				CString strOutZHNode[7] = {_T("01-1"),_T("01-2"),_T("01-3"),_T("01-4"),_T("03-1"),_T("03-2"),_T("03-7")} ;
				for (int iOutIndex=0; iOutIndex<7; iOutIndex++)
				{
					CString strJkLoc = _T("") ;

					strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=0"), strName, strOutZHName[iOutIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							int iJkLayer = 0,iJkBox = 0,iJkCell = 0 ;							
							cSetFind.GetFieldValue(_T("layer_num"), iJkLayer) ;
							cSetFind.GetFieldValue(_T("box_num"), iJkBox) ;
							cSetFind.GetFieldValue(_T("cell_num"), iJkCell) ;
							strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayer, iJkBox, iJkCell) ;	
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}

					pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*(2+iOutIndex)+dCellWidth/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*2/3-dCellHeight/6 ;
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					if (iOutIndex<4)
					{
						pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/6 ;
					}
					else
					{
						pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/3-dCellHeight/6 ;
					}
					//mysys.WriteText(pt_base_draw_tmp, strOutZHNode[iOutIndex], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strOutZHNode[iOutIndex], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				}

				CString strInZHName[3] = {_T("DBJ"),_T("FBJ"),_T("YCJ")} ;
				CString strInZHNode[3] = {_T("02-1"),_T("02-2"),_T("04-3")} ;
				for (int iInIndex=0; iInIndex<3; iInIndex++)
				{
					CString strJkLoc = _T("") ;
					
					strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=1"), strName, strInZHName[iInIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							int iJkLayer = 0,iJkBox = 0,iJkCell = 0 ;							
							cSetFind.GetFieldValue(_T("layer_num"), iJkLayer) ;
							cSetFind.GetFieldValue(_T("box_num"), iJkBox) ;
							cSetFind.GetFieldValue(_T("cell_num"), iJkCell) ;
							strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayer, iJkBox, iJkCell) ;	
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
					
					pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*(9+iInIndex)+dCellWidth/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*2/3-dCellHeight/6 ;
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/3 ;
					//mysys.WriteText(pt_base_draw_tmp, strInZHNode[iInIndex], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strInZHNode[iInIndex], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				}	

				iRet++ ;
				cSetAll.MoveNext() ;
			}
			cSetAll.Close() ;

			//begin 绘制图框（包括电路示意图，表格等）
			int iDwgSum = iRsCount/(iRowNum*iTbNum)+(iRsCount%(iRowNum*iTbNum)==0?0:1) ;
			CString strDate ;
			strDate = CBlkUtility::GetDateStr() ;
			for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
			{
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
				xform.setToTranslation(vec); 
				CMapStringToString mapTypeToStr ;
				mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
// 				CString strDwgType ; 
// 				strDwgType.Format(_T("单动道岔(3J+2X)室内控制电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
// 				mapTypeToStr.SetAt(_T("2"), strDwgType) ;
				mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
				mapTypeToStr.SetAt(_T("4"), strDate) ;
				CString strDltFile ;
				strDltFile.Format(_T("%s\\support\\dlt\\dlt_dc_18_d_3J+2X_qc.dwg"), m_strSysBasePath) ;
				this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
				
			}	

			//end 绘制图框（包括电路示意图，表格等）
			
			mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_DC_18_D_3J+2X_QC")) ;
			mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
			CString strSaveFilePath ;
			strSaveFilePath.Format(_T("%s\\18D_J(3J)+X(2J)-S700K-驱采.dwg"), this->m_strSavePath) ;
			pCurDb->saveAs(strSaveFilePath) ;
			delete pCurDb ;
			pCurDb = NULL ;	
			
			acedRestoreStatusBar() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in Compute_18_d_qc_Circuit")) ;
	} 	
	
	return iRet ;
}

BOOL CStationSystem::ComputeOneTypeCircuit(const CString strBlockName, const AcGePoint3d pt_3d_ref, AcDbDatabase *pCurDb,AcDbDatabase *&pSaveDb, int &iPxCircuitIndex)
{
	BOOL bRet = FALSE ;

	double dPxCircuitHeight = 297 ;
	
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	int iIdAddPlus = 1000 ;

	//在r14下插入的块名会变成大写，而2002不会，所以统一将名称转为小写
	CString strBlockNameTmp = strBlockName ;
	strBlockNameTmp.MakeLower() ;

	//进站点灯
    if (strBlockNameTmp==_T("dlt_xhj_jz_dd_tb"))
    {
		double dCellHeight = 10 ;
		double dCellWidth = 16 ;
		CString strFx1U_YBH[8] = {_T("314"),_T("105"),_T("131"),_T("157"),_T("183"),_T("209"),_T("235"),_T("261")} ;

        //////////////////////////////////////////////////////////////////////////
        //begin process dlt_xhj_jz_dd_tb
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
        for (int i=0; i<8; i++)
        {
            CString strXHJName = _T(""), strJZLoc = _T(""),strUSULoc = _T("") ;
            int iIndexInBox = 0 ;
            
            pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
            //单元格左上角、右下角
            AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
            AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
            
            CArray<AcDbText *, AcDbText *> arrayXHJName ;
            mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
            mysys.SortEnt(arrayXHJName, 2) ;
            CStringArray strArrayXHJName ;
            mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
            mysys.DestroyEntInArray(arrayXHJName) ;
            
            if (strArrayXHJName.GetSize()>0)
            {
                strXHJName = strArrayXHJName.GetAt(0) ;
                strXHJName.TrimLeft() ;
                strXHJName.TrimRight() ;
                if (strXHJName!=_T(""))
                {
                    CMapStringToString mapCircuitTypeToStr ;
                    
                    mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
                    
                    pt_3d_topleft.x+=dCellWidth ;
                    pt_3d_bottomright.x+=dCellWidth ;
                    
                    CArray<AcDbText *, AcDbText *> arrayLoc ;
                    mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
                    mysys.SortEnt(arrayLoc, 2) ;
                    CStringArray strArrayLoc ;
                    mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
                    mysys.DestroyEntInArray(arrayLoc) ;
                    if (strArrayLoc.GetSize()>0)
                    {
                        strJZLoc = strArrayLoc.GetAt(0) ;
                        mapCircuitTypeToStr.SetAt(_T("53"), strJZLoc) ;
                        if (strArrayLoc.GetSize()>1) //有USUZ
                        {
                            strUSULoc = strArrayLoc.GetAt(1) ;
                            int iFind1 = 0, iFind2 = 0 ;
                            iFind1 = strUSULoc.Find(_T("(")) ;
                            iFind2 = strUSULoc.Find(_T(")")) ;
                            CString strUSULocTmp = strUSULoc.Left(iFind1) ;
                            mapCircuitTypeToStr.SetAt(_T("340"), strUSULocTmp) ;
                            CString strIndexInBox = _T("") ;
                            strIndexInBox = strUSULoc.Mid(iFind1+1, iFind2-iFind1-1) ;
                            iIndexInBox = _tstoi(strIndexInBox) ;                                     
                            
                            try 
                            { 
                                CADORecordset cSetFind ;
                                CString strSql ;
                                CString strUSUZ_A_Node = _T(""), strUSUZ_B_Node = _T("") ;
                                strSql.Format(_T("select * from node_collect where name like 'USUZ-%%' and dwg_type='JZ-DD' and index=%d"), iIndexInBox) ;
                                if (cSetFind.IsOpen())
                                {
                                    cSetFind.Close() ;
                                }
                                if (cSetFind.Open(g_PtrCon, strSql))
                                {
                                    if (!cSetFind.IsBOF())
                                    {
                                        cSetFind.MoveFirst() ;
                                    }                   
                                    while (!cSetFind.IsEOF())
                                    {
                                        CString strNodeName = _T("") ;
                                        cSetFind.GetFieldValue(_T("name"), strNodeName) ;
                                        if (strNodeName==_T("USUZ-A"))
                                        {
                                            cSetFind.GetFieldValue(_T("node"), strUSUZ_A_Node) ;
                                        }
                                        else if (strNodeName==_T("USUZ-B"))
                                        {
                                            cSetFind.GetFieldValue(_T("node"), strUSUZ_B_Node) ;
                                        }
                                        
                                        cSetFind.MoveNext() ;
                                    }
                                    
                                    cSetFind.Close() ;
                                }                                           
                                
                                mapCircuitTypeToStr.SetAt(_T("366"), strUSUZ_A_Node) ;
                                mapCircuitTypeToStr.SetAt(_T("392"), strUSUZ_B_Node) ;  
                            }
                            catch (_com_error& e)
                            {
                                CString sBuff = CBlkUtility::GetErrorDescription(e) ;
                                AfxMessageBox(sBuff) ;  
                            }
                            catch (...)
                            {
                                AfxMessageBox(_T("unknown error in pxcircuit-usuz")) ;
                            }                               
                        }// end 处理USUZ    
                        
                    } //end 读组合位置
                    
                    //读出1U-YBH的分线位置
                    for (int iFxIndex=0; iFxIndex<8; iFxIndex++)
                    {
                        pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
                        pt_3d_topleft.y = pt_3d_cur_base.y ;
                        pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
                        pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight/2 ;
                        if (iFxIndex==0)
                        {
                            if (strUSULoc!=_T(""))
                            {
                                pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*11 ;
                                pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*12 ;
                                pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight/2 ;
                                pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
                            }
                        }
                        CArray<AcDbText *, AcDbText *> arrayText ;
                        mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
                        mysys.SortEnt(arrayText, 2) ;
                        CStringArray strArrayText ;
                        mysys.GetHaveSortedText(arrayText, strArrayText) ;
                        mysys.DestroyEntInArray(arrayText) ;
                        int iSize = 0 ;
                        iSize = strArrayText.GetSize() ;
                        if (iSize>0)
                        {
                            mapCircuitTypeToStr.SetAt(strFx1U_YBH[iFxIndex], strArrayText.GetAt(0)) ;
                        }
                    } //end for 
                    
                    //插入配线用电路图
                    CString strCircuitTemplete = _T("") ;
                    if (iIndexInBox==0)
                    {
                        strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_JZ_LIGHT_NOUSUZ.dwg"), m_strSysBasePath) ;
                    }
                    else
                    {
                        strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_JZ_LIGHT.dwg"), m_strSysBasePath) ;
                    }
                    AcGeMatrix3d xform;  
                    AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
                    xform.setToTranslation(vec);                    
                    this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;            
                    
                    iPxCircuitIndex++ ; 
                }
            }
			
        }
		//end process dlt_xhj_jz_dd_tb
		//////////////////////////////////////////////////////////////////////////                             
    }
	//进站驱动采集
	else if (strBlockNameTmp==_T("dlt_xhj_jz_qc_tb"))
	{
		double dCellHeight = 10 ;
		double dCellWidth = 18 ;
		
		CString strJkLXJ_SNJJ[18] = {_T("79"),_T("105"),_T("131"),_T("157"),_T("183"),_T("209"),_T("236"),_T("261"),_T("287"),_T("313"),_T("339"),_T("365"),_T("391"),_T("417"),_T("443"),_T("469"),_T("496"),_T("522")} ;

		//////////////////////////////////////////////////////////////////////////
		//begin process dlt_xhj_jz_qc_tb		
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
		for (int i=0; i<8; i++)
		{
			CString strXHJName = _T(""), strJZLoc = _T(""),strUSULoc = _T("") ;
			int iIndexInBox = 0 ;
			
			pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
			//单元格左上角、右下角
			AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
			AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
			
			CArray<AcDbText *, AcDbText *> arrayXHJName ;
			mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
			mysys.SortEnt(arrayXHJName, 2) ;
			CStringArray strArrayXHJName ;
			mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
			mysys.DestroyEntInArray(arrayXHJName) ;
			
			if (strArrayXHJName.GetSize()>0)
			{
				strXHJName = strArrayXHJName.GetAt(0) ;
				strXHJName.TrimLeft() ;
				strXHJName.TrimRight() ;
				if (strXHJName!=_T(""))
				{
					CMapStringToString mapCircuitTypeToStr ;
					
					mapCircuitTypeToStr.SetAt(_T("28"), strXHJName) ;
					
					pt_3d_topleft.x+=dCellWidth ;
					pt_3d_bottomright.x+=dCellWidth ;
					
					CArray<AcDbText *, AcDbText *> arrayLoc ;
					mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
					mysys.SortEnt(arrayLoc, 2) ;
					CStringArray strArrayLoc ;
					mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
					mysys.DestroyEntInArray(arrayLoc) ;
					if (strArrayLoc.GetSize()>0)
					{
						strJZLoc = strArrayLoc.GetAt(0) ;
						mapCircuitTypeToStr.SetAt(_T("53"), strJZLoc) ;
						if (strArrayLoc.GetSize()>1) //有USUZ
						{
							strUSULoc = strArrayLoc.GetAt(1) ;
							int iFind1 = 0, iFind2 = 0 ;
							iFind1 = strUSULoc.Find(_T("(")) ;
							iFind2 = strUSULoc.Find(_T(")")) ;
							CString strUSULocTmp = strUSULoc.Left(iFind1) ;
							mapCircuitTypeToStr.SetAt(_T("574"), strUSULocTmp) ;
							CString strIndexInBox = _T("") ;
							strIndexInBox = strUSULoc.Mid(iFind1+1, iFind2-iFind1-1) ;
							iIndexInBox = _tstoi(strIndexInBox) ;                                     

							try 
							{ 
								CADORecordset cSetFind ;
								CString strSql ;
								CString strSNKJ_1_Node = _T(""), strSNKJ_12_Node = _T(""), strSNJJ_12_Node = _T("") ;
								strSql.Format(_T("select * from node_collect where name like 'SN%%' and dwg_type='JZ-QC' and index=%d"), iIndexInBox) ;
								if (cSetFind.IsOpen())
								{
									cSetFind.Close() ;
								}
								if (cSetFind.Open(g_PtrCon, strSql))
								{
									if (!cSetFind.IsBOF())
									{
										cSetFind.MoveFirst() ;
									}                   
									while (!cSetFind.IsEOF())
									{
										CString strNodeName = _T("") ;
										CString strLoc = _T("") ;
										cSetFind.GetFieldValue(_T("name"), strNodeName) ;
										cSetFind.GetFieldValue(_T("loc"), strLoc) ;
										if (strNodeName==_T("SNKJ")&&strLoc==_T("1"))
										{
											cSetFind.GetFieldValue(_T("node"), strSNKJ_1_Node) ;
										}
										else if (strNodeName==_T("SNKJ")&&strLoc==_T("12"))
										{
											cSetFind.GetFieldValue(_T("node"), strSNKJ_12_Node) ;
										}
										else if (strNodeName==_T("SNJJ")&&strLoc==_T("12"))
										{
											cSetFind.GetFieldValue(_T("node"), strSNJJ_12_Node) ;
										}
										
										cSetFind.MoveNext() ;
									}
									
									cSetFind.Close() ;
								}
								
								mapCircuitTypeToStr.SetAt(_T("600"), strSNKJ_1_Node) ;
								mapCircuitTypeToStr.SetAt(_T("626"), strSNKJ_12_Node) ;
								mapCircuitTypeToStr.SetAt(_T("652"), strSNJJ_12_Node) ;
								
							}
							catch (_com_error& e)
							{
								CString sBuff = CBlkUtility::GetErrorDescription(e) ;
								AfxMessageBox(sBuff) ;  
							}
							catch (...)
							{
								AfxMessageBox(_T("unknown error in pxcircuit-usuz")) ;
							}                               
						}// end 处理USUZ    
						
					} //end 读组合位置
					
					//读出LXJ-SNJJ的JK位置
					for (int iFxIndex=0; iFxIndex<18; iFxIndex++)
					{
						pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
						pt_3d_topleft.y = pt_3d_cur_base.y ;
						pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
						pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight/2 ;
						if (iFxIndex==6||iFxIndex==16||iFxIndex==17)
						{
							if (strUSULoc!=_T(""))
							{
								pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
								pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
								pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight/2 ;
								pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
							}
							else
							{
								continue ;
							}
						}
						CArray<AcDbText *, AcDbText *> arrayText ;
						mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
						mysys.SortEnt(arrayText, 2) ;
						CStringArray strArrayText ;
						mysys.GetHaveSortedText(arrayText, strArrayText) ;
						mysys.DestroyEntInArray(arrayText) ;
						if (strArrayText.GetSize()>0)
						{                                           
							mapCircuitTypeToStr.SetAt(strJkLXJ_SNJJ[iFxIndex], strArrayText.GetAt(0)) ;
						}
					} //end for 
					
					//插入配线用电路图
					CString strCircuitTemplete ;
					strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_JZ_DRIVE.dwg"), m_strSysBasePath) ;
					
					AcGeMatrix3d xform;  
					AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
					xform.setToTranslation(vec);                    
					this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
					
					iPxCircuitIndex++ ; 
        }
      }
							
    }
    // end process dlt_xhj_jz_qc_tb
    //////////////////////////////////////////////////////////////////////////
  }
  //进路点灯
  else if (strBlockNameTmp==_T("dlt_xhj_jl_dd_tb"))
  {
	  	double dCellHeight = 10 ;
		double dCellWidth = 15 ;
		CString strFx1U_YBH[9] = {_T("314"),_T("105"),_T("131"),_T("157"),_T("132"),_T("183"),_T("209"),_T("235"),_T("261")} ;

        //////////////////////////////////////////////////////////////////////////
        //begin process dlt_xhj_jl_dd_tb
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
        for (int i=0; i<8; i++)
        {            
            CString strXHJName = _T(""), strJZLoc = _T(""),strUSULoc = _T("") ;
            int iIndexInBox = 0 ;
            
            pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
            //单元格左上角、右下角
            AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
            AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
            
            CArray<AcDbText *, AcDbText *> arrayXHJName ;
            mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
            mysys.SortEnt(arrayXHJName, 2) ;
            CStringArray strArrayXHJName ;
            mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
            mysys.DestroyEntInArray(arrayXHJName) ;
            
            if (strArrayXHJName.GetSize()>0)
            {
                strXHJName = strArrayXHJName.GetAt(0) ;
                strXHJName.TrimLeft() ;
                strXHJName.TrimRight() ;
                if (strXHJName!=_T(""))
                {
                    CMapStringToString mapCircuitTypeToStr ;
                    
                    mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
                    
                    pt_3d_topleft.x+=dCellWidth ;
                    pt_3d_bottomright.x+=dCellWidth ;
                    
                    CArray<AcDbText *, AcDbText *> arrayLoc ;
                    mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
                    mysys.SortEnt(arrayLoc, 2) ;
                    CStringArray strArrayLoc ;
                    mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
                    mysys.DestroyEntInArray(arrayLoc) ;
                    if (strArrayLoc.GetSize()>0)
                    {
                        strJZLoc = strArrayLoc.GetAt(0) ;
                        mapCircuitTypeToStr.SetAt(_T("53"), strJZLoc) ;
                        if (strArrayLoc.GetSize()>1) //有USUZ
                        {
                            strUSULoc = strArrayLoc.GetAt(1) ;
                            int iFind1 = 0, iFind2 = 0 ;
                            iFind1 = strUSULoc.Find(_T("(")) ;
                            iFind2 = strUSULoc.Find(_T(")")) ;
                            CString strUSULocTmp = strUSULoc.Left(iFind1) ;
                            mapCircuitTypeToStr.SetAt(_T("340"), strUSULocTmp) ;
                            CString strIndexInBox = _T("") ;
                            strIndexInBox = strUSULoc.Mid(iFind1+1, iFind2-iFind1-1) ;
                            iIndexInBox = _tstoi(strIndexInBox) ;                                     
                            
                            try 
                            { 
                                CADORecordset cSetFind ;
                                CString strSql ;
                                CString strUSUZ_A_Node = _T(""), strUSUZ_B_Node = _T("") ;
                                strSql.Format(_T("select * from node_collect where name like 'USUZ-%%' and dwg_type='JZ-DD' and index=%d"), iIndexInBox) ;
                                if (cSetFind.IsOpen())
                                {
                                    cSetFind.Close() ;
                                }
                                if (cSetFind.Open(g_PtrCon, strSql))
                                {
                                    if (!cSetFind.IsBOF())
                                    {
                                        cSetFind.MoveFirst() ;
                                    }                   
                                    while (!cSetFind.IsEOF())
                                    {
                                        CString strNodeName = _T("") ;
                                        cSetFind.GetFieldValue(_T("name"), strNodeName) ;
                                        if (strNodeName==_T("USUZ-A"))
                                        {
                                            cSetFind.GetFieldValue(_T("node"), strUSUZ_A_Node) ;
                                        }
                                        else if (strNodeName==_T("USUZ-B"))
                                        {
                                            cSetFind.GetFieldValue(_T("node"), strUSUZ_B_Node) ;
                                        }
                                        
                                        cSetFind.MoveNext() ;
                                    }
                                    
                                    cSetFind.Close() ;
                                }                                           
                                
                                mapCircuitTypeToStr.SetAt(_T("366"), strUSUZ_A_Node) ;
                                mapCircuitTypeToStr.SetAt(_T("392"), strUSUZ_B_Node) ;  
                            }
                            catch (_com_error& e)
                            {
                                CString sBuff = CBlkUtility::GetErrorDescription(e) ;
                                AfxMessageBox(sBuff) ;  
                            }
                            catch (...)
                            {
                                AfxMessageBox(_T("unknown error in pxcircuit-usuz")) ;
                            }                               
                        }// end 处理USUZ    
                        
                    } //end 读组合位置
                    
                    //读出1U-YBH的分线位置
                    for (int iFxIndex=0; iFxIndex<9; iFxIndex++)
                    {
                        pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
                        pt_3d_topleft.y = pt_3d_cur_base.y ;
                        pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
                        pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight/2 ;
                        if (iFxIndex==0)
                        {
                            if (strUSULoc!=_T(""))
                            {
                                pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*12 ;
                                pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*13 ;
                                pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight/2 ;
                                pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
                            }
                        }
                        CArray<AcDbText *, AcDbText *> arrayText ;
                        mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
                        mysys.SortEnt(arrayText, 2) ;
                        CStringArray strArrayText ;
                        mysys.GetHaveSortedText(arrayText, strArrayText) ;
                        mysys.DestroyEntInArray(arrayText) ;
                        int iSize = 0 ;
                        iSize = strArrayText.GetSize() ;
                        if (iSize>0)
                        {
                            mapCircuitTypeToStr.SetAt(strFx1U_YBH[iFxIndex], strArrayText.GetAt(0)) ;
                        }
                    } //end for 
                    
                    //插入配线用电路图
                    CString strCircuitTemplete = _T("") ;
                    if (iIndexInBox==0)
                    {
                        strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_JL_LIGHT_NOUSUZ.dwg"), m_strSysBasePath) ;
                    }
                    else
                    {
                        strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_JL_LIGHT.dwg"), m_strSysBasePath) ;
                    }
                    AcGeMatrix3d xform;  
                    AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
                    xform.setToTranslation(vec);                    
                    this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;            
                    
                    iPxCircuitIndex++ ; 
                }
            }
			
        }
		//end process dlt_xhj_jl_dd_tb
		//////////////////////////////////////////////////////////////////////////  
  }
  //进路驱动采集
  else if (strBlockNameTmp==_T("dlt_xhj_jl_qc_tb"))
  {
	  double dCellHeight = 10 ;
	  double dCellWidth = 18 ;
	  CString strJkLXJ_SNJJ[20] = {
		  _T("79"),_T("105"),_T("131"),_T("157"),_T("183"),_T("200"),_T("209"),_T("236"),
			  _T("261"), _T("287"),_T("313"),_T("339"),_T("365"),_T("391"),_T("417"),_T("443"),_T("201"),_T("469"),_T("496"),_T("522")} ;

	  //////////////////////////////////////////////////////////////////////////
	  //begin process dlt_xhj_jl_qc_tb	  
	  AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
	  for (int i=0; i<8; i++)
	  {		  
		  CString strXHJName = _T(""), strJZLoc = _T(""),strUSULoc = _T("") ;
		  int iIndexInBox = 0 ;
		  
		  pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
		  //单元格左上角、右下角
		  AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
		  AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
		  
		  CArray<AcDbText *, AcDbText *> arrayXHJName ;
		  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
		  mysys.SortEnt(arrayXHJName, 2) ;
		  CStringArray strArrayXHJName ;
		  mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
		  mysys.DestroyEntInArray(arrayXHJName) ;
		  
		  if (strArrayXHJName.GetSize()>0)
		  {
			  strXHJName = strArrayXHJName.GetAt(0) ;
			  strXHJName.TrimLeft() ;
			  strXHJName.TrimRight() ;
			  if (strXHJName!=_T(""))
			  {
				  CMapStringToString mapCircuitTypeToStr ;
				  CString strUSULocTmp = _T("") ;
				  CString strSNKJ_1_Node = _T(""), strSNKJ_12_Node = _T(""), strSNJJ_12_Node = _T("") ;
				  
				  mapCircuitTypeToStr.SetAt(_T("28"), strXHJName) ;
				  
				  pt_3d_topleft.x+=dCellWidth ;
				  pt_3d_bottomright.x+=dCellWidth ;
				  
				  CArray<AcDbText *, AcDbText *> arrayLoc ;
				  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
				  mysys.SortEnt(arrayLoc, 2) ;
				  CStringArray strArrayLoc ;
				  mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
				  mysys.DestroyEntInArray(arrayLoc) ;
				  if (strArrayLoc.GetSize()>0)
				  {
					  strJZLoc = strArrayLoc.GetAt(0) ;
					  if (strArrayLoc.GetSize()>1) //有USUZ
					  {
						  strUSULoc = strArrayLoc.GetAt(1) ;
						  int iFind1 = 0, iFind2 = 0 ;
						  iFind1 = strUSULoc.Find(_T("(")) ;
						  iFind2 = strUSULoc.Find(_T(")")) ;
						  strUSULocTmp = strUSULoc.Left(iFind1) ;
						  CString strIndexInBox = _T("") ;
						  strIndexInBox = strUSULoc.Mid(iFind1+1, iFind2-iFind1-1) ;
						  iIndexInBox = _tstoi(strIndexInBox) ;                                     
						  
						  try 
						  { 
							  CADORecordset cSetFind ;
							  CString strSql ;
							  strSql.Format(_T("select * from node_collect where name like 'SN%%' and dwg_type='JZ-QC' and index=%d"), iIndexInBox) ;
							  if (cSetFind.IsOpen())
							  {
								  cSetFind.Close() ;
							  }
							  if (cSetFind.Open(g_PtrCon, strSql))
							  {
								  if (!cSetFind.IsBOF())
								  {
									  cSetFind.MoveFirst() ;
								  }                   
								  while (!cSetFind.IsEOF())
								  {
									  CString strNodeName = _T("") ;
									  CString strLoc = _T("") ;
									  cSetFind.GetFieldValue(_T("name"), strNodeName) ;
									  cSetFind.GetFieldValue(_T("loc"), strLoc) ;
									  if (strNodeName==_T("SNKJ")&&strLoc==_T("1"))
									  {
										  cSetFind.GetFieldValue(_T("node"), strSNKJ_1_Node) ;
									  }
									  else if (strNodeName==_T("SNKJ")&&strLoc==_T("12"))
									  {
										  cSetFind.GetFieldValue(_T("node"), strSNKJ_12_Node) ;
									  }
									  else if (strNodeName==_T("SNJJ")&&strLoc==_T("12"))
									  {
										  cSetFind.GetFieldValue(_T("node"), strSNJJ_12_Node) ;
									  }
									  
									  cSetFind.MoveNext() ;
								  }
								  
								  cSetFind.Close() ;
							  }	  
						  }
						  catch (_com_error& e)
						  {
							  CString sBuff = CBlkUtility::GetErrorDescription(e) ;
							  AfxMessageBox(sBuff) ;  
						  }
						  catch (...)
						  {
							  AfxMessageBox(_T("unknown error in pxcircuit-usuz")) ;
						  }                               
					  }// end 处理USUZ    
					  
				  } //end 读组合位置
				  mapCircuitTypeToStr.SetAt(_T("53"), strJZLoc) ;
				  mapCircuitTypeToStr.SetAt(_T("574"), strUSULocTmp) ;
				  mapCircuitTypeToStr.SetAt(_T("600"), strSNKJ_1_Node) ;
				  mapCircuitTypeToStr.SetAt(_T("626"), strSNKJ_12_Node) ;
				  mapCircuitTypeToStr.SetAt(_T("652"), strSNJJ_12_Node) ;
				  
				  //读出LXJ-SNJJ的JK位置
				  for (int iFxIndex=0; iFxIndex<20; iFxIndex++)
				  {
					  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
					  pt_3d_topleft.y = pt_3d_cur_base.y ;
					  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight/2 ;
					  if (iFxIndex==7||iFxIndex==18||iFxIndex==19)
					  {
						  if (strUSULoc!=_T(""))
						  {
							  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
							  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
							  pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight/2 ;
							  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
						  }
						  else
						  {
							  continue ;
						  }
					  }
					  CArray<AcDbText *, AcDbText *> arrayText ;
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  CStringArray strArrayText ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  if (strArrayText.GetSize()>0)
					  {                                           
						  mapCircuitTypeToStr.SetAt(strJkLXJ_SNJJ[iFxIndex], strArrayText.GetAt(0)) ;
					  }
					  else
					  {
						  mapCircuitTypeToStr.SetAt(strJkLXJ_SNJJ[iFxIndex], _T("")) ;
					  }
				  } //end for 
				  
				  //插入配线用电路图
				  CString strCircuitTemplete ;
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_JL_DRIVE.dwg"), m_strSysBasePath) ;
				  
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec);                    
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				  
				  iPxCircuitIndex++ ; 
			}
		}
	  
    }
    // end process dlt_xhj_jl_qc_tb
    //////////////////////////////////////////////////////////////////////////
  }
  //出站点灯
  else if (strBlockNameTmp==_T("dlt_xhj_cz_dd_tb"))
  {
		double dCellHeight = 5 ;
		double dCellWidth0 = 10 ;
		double dCellWidth = 15 ;
		CString strFxH_BH[6] = {_T("79"),_T("105"),_T("131"),_T("157"),_T("183"),_T("209")} ;

        //////////////////////////////////////////////////////////////////////////
        //begin process dlt_xhj_cz_dd_tb
		AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
        for (int i=0; i<40; i++)
        {            
            CString strXHJName = _T(""), strCZZLoc = _T("") ;
            
            pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
            //单元格左上角、右下角
            AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
            AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth0, pt_3d_cur_base.y-dCellHeight, 0) ;
            
            CArray<AcDbText *, AcDbText *> arrayXHJName ;
            mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
            mysys.SortEnt(arrayXHJName, 2) ;
            CStringArray strArrayXHJName ;
            mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
            mysys.DestroyEntInArray(arrayXHJName) ;
            
            if (strArrayXHJName.GetSize()>0)
            {
                strXHJName = strArrayXHJName.GetAt(0) ;
                strXHJName.TrimLeft() ;
                strXHJName.TrimRight() ;
                if (strXHJName!=_T(""))
                {
                    CMapStringToString mapCircuitTypeToStr ;
                    
                    mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
                    
                    pt_3d_topleft.x+=dCellWidth0 ;
                    pt_3d_bottomright.x+=dCellWidth ;
                    
                    CArray<AcDbText *, AcDbText *> arrayLoc ;
                    mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
                    mysys.SortEnt(arrayLoc, 2) ;
                    CStringArray strArrayLoc ;
                    mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
                    mysys.DestroyEntInArray(arrayLoc) ;
                    if (strArrayLoc.GetSize()>0)
                    {
                        strCZZLoc = strArrayLoc.GetAt(0) ;                        
                    } //end 读组合位置
					mapCircuitTypeToStr.SetAt(_T("53"), strCZZLoc) ;       
                    
                    //读出H-BH的分线位置
                    for (int iFxIndex=0; iFxIndex<6; iFxIndex++)
                    {
                        pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(1+iFxIndex) ;
                        pt_3d_topleft.y = pt_3d_cur_base.y ;
                        pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(2+iFxIndex) ;
                        pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
       
                        CArray<AcDbText *, AcDbText *> arrayText ;
                        mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
                        mysys.SortEnt(arrayText, 2) ;
                        CStringArray strArrayText ;
                        mysys.GetHaveSortedText(arrayText, strArrayText) ;
                        mysys.DestroyEntInArray(arrayText) ;
                        int iSize = 0 ;
                        iSize = strArrayText.GetSize() ;
                        if (iSize>0)
                        {
                            mapCircuitTypeToStr.SetAt(strFxH_BH[iFxIndex], strArrayText.GetAt(0)) ;
                        }
						else
						{
							mapCircuitTypeToStr.SetAt(strFxH_BH[iFxIndex], _T("")) ;
						}
                    } //end for 
                    
                    //插入配线用电路图
                    CString strCircuitTemplete ;
					strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_CZ_LIGHT.dwg"), m_strSysBasePath) ;
                    
                    AcGeMatrix3d xform;  
                    AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
                    xform.setToTranslation(vec);                    
                    this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;            
                    
                    iPxCircuitIndex++ ; 
                }
            }
			
        }
		//end process dlt_xhj_cz_dd_tb
		//////////////////////////////////////////////////////////////////////////                             

  }
  //出站驱采
  else if (strBlockNameTmp==_T("dlt_xhj_cz_qc_tb"))
  {
	  double dCellHeight = 5 ;
	  double dCellWidth = 18 ;
	  CString strJkLXJ_KDJ[10] = {_T("79"),_T("105"),_T("131"),_T("157"),_T("183"),_T("209"),_T("235"),_T("261"),_T("287"),_T("313")} ;
	  
	  //////////////////////////////////////////////////////////////////////////
	  //begin process dlt_xhj_cz_qc_tb
	  
	  AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
	  for (int i=0; i<40; i++)
	  {
		  
		  CString strXHJName = _T(""), strCZZLoc = _T("") ;
		  
		  pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
		  //单元格左上角、右下角
		  AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
		  AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
		  
		  CArray<AcDbText *, AcDbText *> arrayXHJName ;
		  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
		  mysys.SortEnt(arrayXHJName, 2) ;
		  CStringArray strArrayXHJName ;
		  mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
		  mysys.DestroyEntInArray(arrayXHJName) ;
		  
		  if (strArrayXHJName.GetSize()>0)
		  {
			  strXHJName = strArrayXHJName.GetAt(0) ;
			  strXHJName.TrimLeft() ;
			  strXHJName.TrimRight() ;
			  if (strXHJName!=_T(""))
			  {
				  CMapStringToString mapCircuitTypeToStr ;
				  
				  mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
				  
				  pt_3d_topleft.x+=dCellWidth ;
				  pt_3d_bottomright.x+=dCellWidth ;
				  
				  CArray<AcDbText *, AcDbText *> arrayLoc ;
				  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
				  mysys.SortEnt(arrayLoc, 2) ;
				  CStringArray strArrayLoc ;
				  mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
				  mysys.DestroyEntInArray(arrayLoc) ;
				  if (strArrayLoc.GetSize()>0)
				  {
					  strCZZLoc = strArrayLoc.GetAt(0) ;
				  } //end 读组合位置
				  mapCircuitTypeToStr.SetAt(_T("53"), strCZZLoc) ;			 
				  
				  //读出LXJ-KDJ-13的JK位置
				  for (int iFxIndex=0; iFxIndex<10; iFxIndex++)
				  {
					  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
					  pt_3d_topleft.y = pt_3d_cur_base.y ;
					  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
		
					  CArray<AcDbText *, AcDbText *> arrayText ;
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  CStringArray strArrayText ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  if (strArrayText.GetSize()>0)
					  {                                           
						  mapCircuitTypeToStr.SetAt(strJkLXJ_KDJ[iFxIndex], strArrayText.GetAt(0)) ;
					  }
					  else 
					  {
						  mapCircuitTypeToStr.SetAt(strJkLXJ_KDJ[iFxIndex], _T("")) ;
					  }
				  } //end for 
				  
				  //插入配线用电路图
				  CString strCircuitTemplete ; 
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_CZ_DRIVE.dwg"), m_strSysBasePath) ;
				  
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec);                    
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				  
				  iPxCircuitIndex++ ; 
        }
      }
	  
    }
    // end process dlt_xhj_cz_qc_tb
    //////////////////////////////////////////////////////////////////////////
	
  }
  //调车点灯
  else if (strBlockNameTmp==_T("dlt_xhj_dc_dd_tb"))
  {	  
	  double dCellHeight = 5 ;
	  double dCellWidth0 = 10 ;
	  double dCellWidth = 15 ;
	  CString strFxA_ABH[3] = {_T("79"),_T("105"),_T("131")} ;
	  CString strDD_Node_Type[5] = {_T("157"),_T("183"),_T("209"),_T("261"),_T("287")} ;
	  CString strDD_Type[5] = {_T("A"),_T("B"),_T("ABH"),_T("RD1"),_T("RD2")} ;

	  //////////////////////////////////////////////////////////////////////////
	  //begin process dlt_xhj_dc_dd_tb
	  AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
	  for (int i=0; i<44; i++)
	  {		  
		  CString strXHJName = _T(""), strDXZLocTmp = _T(""), strDXZLoc = _T("") ;
		  int iIndexInBox = 0 ;
		  
		  pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
		  //单元格左上角、右下角
		  AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
		  AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
		  
		  CArray<AcDbText *, AcDbText *> arrayXHJName ;
		  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
		  mysys.SortEnt(arrayXHJName, 2) ;
		  CStringArray strArrayXHJName ;
		  mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
		  mysys.DestroyEntInArray(arrayXHJName) ;
		  
		  if (strArrayXHJName.GetSize()>0)
		  {
			  strXHJName = strArrayXHJName.GetAt(0) ;
			  strXHJName.TrimLeft() ;
			  strXHJName.TrimRight() ;
			  if (strXHJName!=_T(""))
			  {
				  CMapStringToString mapCircuitTypeToStr ;
				  
				  mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
				  
				  pt_3d_topleft.x+=dCellWidth ;
				  pt_3d_bottomright.x+=dCellWidth ;
				  
				  CArray<AcDbText *, AcDbText *> arrayLoc ;
				  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
				  mysys.SortEnt(arrayLoc, 2) ;
				  CStringArray strArrayLoc ;
				  mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
				  mysys.DestroyEntInArray(arrayLoc) ;
				  if (strArrayLoc.GetSize()>0)
				  {
					  strDXZLocTmp = strArrayLoc.GetAt(0) ;
					  int iFind1 = 0, iFind2 = 0 ;
					  iFind1 = strDXZLocTmp.Find(_T("(")) ;
					  iFind2 = strDXZLocTmp.Find(_T(")")) ;
					  strDXZLoc = strDXZLocTmp.Left(iFind1) ;

					  mapCircuitTypeToStr.SetAt(_T("235"), strDXZLoc) ;

					  CString strIndexInBox = _T("") ;
					  strIndexInBox = strDXZLocTmp.Mid(iFind1+1, iFind2-iFind1-1) ;
					  iIndexInBox = _tstoi(strIndexInBox) ;

					  //
					  CString strSql = _T("") ;
					  CADORecordset cSetFind ;
					  for (int iNodeIndex=0; iNodeIndex<5; iNodeIndex++)
					  {
						  CString strNode = _T("") ;
						  strSql.Format(_T("select * from node_collect where dwg_type='DC-DD' and index=%d and name='%s'"), iIndexInBox, strDD_Type[iNodeIndex]) ;
						  if (cSetFind.IsOpen())
						  {
							  cSetFind.Close() ;
						  }
						  if (cSetFind.Open(g_PtrCon, strSql))
						  {
							  if (!cSetFind.IsBOF())
							  {
								  cSetFind.MoveFirst() ;
							  }							 
							  while (!cSetFind.IsEOF())
							  {								  
								  cSetFind.GetFieldValue(_T("node"), strNode) ;
								  cSetFind.MoveNext() ;
							  }
							  
							  cSetFind.Close() ;
						  }
						  mapCircuitTypeToStr.SetAt(strDD_Node_Type[iNodeIndex], strNode) ;						  								 

					  }		  
					  
				  } //end 读组合位置
				  
				  //读出A-ABH的分线位置
				  for (int iFxIndex=0; iFxIndex<3; iFxIndex++)
				  {
					  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+(1+iFxIndex)*dCellWidth ;
					  pt_3d_topleft.y = pt_3d_cur_base.y ;
					  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(2+iFxIndex) ;
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ; //modified 20101224-1655
	
					  CArray<AcDbText *, AcDbText *> arrayText ;
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  CStringArray strArrayText ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  int iSize = 0 ;
					  iSize = strArrayText.GetSize() ;
					  if (iSize>0)
					  {
						  mapCircuitTypeToStr.SetAt(strFxA_ABH[iFxIndex], strArrayText.GetAt(0)) ;
					  }
					  else
					  {
						  mapCircuitTypeToStr.SetAt(strFxA_ABH[iFxIndex], _T("")) ;
					  }
				  } //end for 
				  
				  //插入配线用电路图
				  CString strCircuitTemplete ;
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_DC_LIGHT.dwg"), m_strSysBasePath) ;		
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec);                    
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;            
				  
				  iPxCircuitIndex++ ; 
                }
            }
			
        }
		//end process dlt_xhj_dc_dd_tb
		//////////////////////////////////////////////////////////////////////////                             
  }
  //调车驱采
  else if (strBlockNameTmp==_T("dlt_xhj_dc_qc_tb"))
  {
	  double dCellHeight = 5 ;
	  double dCellWidth = 23 ;
	  CString strJkDXJ_DJ12[3] = {_T("79"),_T("105"),_T("131")} ;
	  CString strQC_Node_Type[3] = {_T("151"),_T("152"),_T("153")} ;
	  CString strQC_Type[3] = {_T("DXJ"),_T("DXJ12"),_T("DJ12")} ;
	  
	  //////////////////////////////////////////////////////////////////////////
	  //begin process dlt_xhj_dc_qc_tb
	  
	  AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;
	  for (int i=0; i<44; i++)
	  {		  
		  CString strXHJName = _T(""), strDXZLocTmp = _T(""), strDXZLoc = _T("") ;
		  int iIndexInBox = 0 ;
		  
		  pt_3d_cur_base.y= pt_3d_ref.y-i*dCellHeight ;
		  //单元格左上角、右下角
		  AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
		  AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;
		  
		  CArray<AcDbText *, AcDbText *> arrayXHJName ;
		  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayXHJName) ;
		  mysys.SortEnt(arrayXHJName, 2) ;
		  CStringArray strArrayXHJName ;
		  mysys.GetHaveSortedText(arrayXHJName, strArrayXHJName) ;
		  mysys.DestroyEntInArray(arrayXHJName) ;
		  
		  if (strArrayXHJName.GetSize()>0)
		  {
			  strXHJName = strArrayXHJName.GetAt(0) ;
			  strXHJName.TrimLeft() ;
			  strXHJName.TrimRight() ;
			  if (strXHJName!=_T(""))
			  {
				  CMapStringToString mapCircuitTypeToStr ;
				  
				  mapCircuitTypeToStr.SetAt(_T("27"), strXHJName) ;
				  
				  pt_3d_topleft.x+=dCellWidth ;
				  pt_3d_bottomright.x+=dCellWidth ;
				  
				  CArray<AcDbText *, AcDbText *> arrayLoc ;
				  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayLoc) ;
				  mysys.SortEnt(arrayLoc, 2) ;
				  CStringArray strArrayLoc ;
				  mysys.GetHaveSortedText(arrayLoc, strArrayLoc) ;
				  mysys.DestroyEntInArray(arrayLoc) ;
				  if (strArrayLoc.GetSize()>0)
				  {
					  strDXZLocTmp = strArrayLoc.GetAt(0) ;
					  int iFind1 = 0, iFind2 = 0 ;
					  iFind1 = strDXZLocTmp.Find(_T("(")) ;
					  iFind2 = strDXZLocTmp.Find(_T(")")) ;
					  strDXZLoc = strDXZLocTmp.Left(iFind1) ;
					  
					  mapCircuitTypeToStr.SetAt(_T("157"), strDXZLoc) ;
					  
					  CString strIndexInBox = _T("") ;
					  strIndexInBox = strDXZLocTmp.Mid(iFind1+1, iFind2-iFind1-1) ;
					  iIndexInBox = _tstoi(strIndexInBox) ;
					  
					  //
					  CString strSql = _T("") ;
					  CADORecordset cSetFind ;
					  for (int iNodeIndex=0; iNodeIndex<3; iNodeIndex++)
					  {
						  CString strNode = _T("") ;

						  strSql.Format(_T("select * from node_collect where dwg_type='DC-QC' and index=%d and name='%s'"), iIndexInBox, strQC_Type[iNodeIndex]) ;
						  if (cSetFind.IsOpen())
						  {
							  cSetFind.Close() ;
						  }
						  if (cSetFind.Open(g_PtrCon, strSql))
						  {
							  if (!cSetFind.IsBOF())
							  {
								  cSetFind.MoveFirst() ;
							  }	
							  while (!cSetFind.IsEOF())
							  {							  
								  cSetFind.GetFieldValue(_T("node"), strNode) ;						  
								  cSetFind.MoveNext() ;
							  }
							  
							  cSetFind.Close() ;
						  }
						  mapCircuitTypeToStr.SetAt(strQC_Node_Type[iNodeIndex], strNode) ;
					  }
					  
				  } //end 读组合位置
				  
				  //读出LXJ-SNJJ的JK位置
				  for (int iFxIndex=0; iFxIndex<3; iFxIndex++)
				  {
					  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*(2+iFxIndex) ;
					  pt_3d_topleft.y = pt_3d_cur_base.y ;
					  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth*(3+iFxIndex) ;
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ; //modified 20101224-1657
			
					  CArray<AcDbText *, AcDbText *> arrayText ;
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  CStringArray strArrayText ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  if (strArrayText.GetSize()>0)
					  {                                           
						  mapCircuitTypeToStr.SetAt(strJkDXJ_DJ12[iFxIndex], strArrayText.GetAt(0)) ;
					  }
					  else
					  {
						  mapCircuitTypeToStr.SetAt(strJkDXJ_DJ12[iFxIndex], _T("")) ;
					  }
				  } //end for 
				  
				  //插入配线用电路图
				  CString strCircuitTemplete ;
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\XHJ_DC_DRIVE.dwg"), m_strSysBasePath) ;
				  
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec);                    
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				  
				  iPxCircuitIndex++ ; 
        }
      }
	  
    }
    // end process dlt_xhj_dc_qc_tb
    //////////////////////////////////////////////////////////////////////////
	
  }
  //25Hz轨道
  else if (strBlockNameTmp==_T("dlt_25hz_tb"))
  {
	  double dCellHeight = 10 ;
	  double dCellWidth0 = 18 ;
	  double dCellWidth = 15 ;
	  int iRowSum = 24 ;
	  AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;

	  int iCurRow = 0 ;
	  while (iCurRow<iRowSum)
	  {
		  pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;
		  //单元格左上角、右下角
		  AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x+dCellWidth0, pt_3d_cur_base.y, 0) ;
          AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth0+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;

		  //先看是一送几受
		  CArray<AcDbText *, AcDbText *> arrayText ;
		  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
		  mysys.SortEnt(arrayText, 2) ;
		  CStringArray strArrayText ;
		  mysys.GetHaveSortedText(arrayText, strArrayText) ;
		  mysys.DestroyEntInArray(arrayText) ;

		  if (strArrayText.GetSize()>0)
		  {
			  CString strGDType = strArrayText.GetAt(0) ;
			  strGDType.TrimLeft() ;
			  strGDType.TrimRight() ;
			  if (strGDType==_T("一送一受"))
			  {
				  CMapStringToString mapCircuitTypeToStr ;

				  pt_3d_topleft.x = pt_3d_cur_base.x ;
				  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0 ;
				  
				  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
				  mysys.SortEnt(arrayText, 2) ;
				  strArrayText.RemoveAll() ;
				  mysys.GetHaveSortedText(arrayText, strArrayText) ;
				  mysys.DestroyEntInArray(arrayText) ;

				  //轨道名称
				  CString strGDName = _T("");
				  if (strArrayText.GetSize()>0)
				  {
					  strGDName = strArrayText.GetAt(0) ;
				  }
				  mapCircuitTypeToStr.SetAt(_T("27"), strGDName) ;

				  CString strTypeNum[12] = {_T("443"),_T("469"),_T("183"),_T("209"),_T("235"),_T("261"),_T("287"),_T("313"),_T("339"),_T("365"),_T("391"),_T("417")} ;
				  CString strTypeNum2[3] = {_T("184"),_T("210"),_T("418")} ;

				  for (int i=0; i<12; i++)
				  {
					  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(3+i) ;
					  pt_3d_topleft.y = pt_3d_cur_base.y ;
					  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(4+i);
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
					  if ((i>1&&i<8)||i==11)
					  {
						  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight/2 ;
					  }
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  strArrayText.RemoveAll() ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;

					  CString strText = _T("") ;
					  if (strArrayText.GetSize()>0)
					  {
						  strText = strArrayText.GetAt(0) ;
						  if (i==0)
						  {
							  CString strTmp ;
							  strTmp = strText.Left(strText.GetLength()-1) ;
							  strText = strTmp ;
						  }
						  else if (i==1)
						  {
							  CString strTmp ;
							  int iLocFind = strText.Find(_T("(")) ;
							  if (iLocFind!=-1)
							  {
								  strTmp = strText.Left(iLocFind) ;
							  }
							  strText = strTmp ;
						  }
					  }
					  mapCircuitTypeToStr.SetAt(strTypeNum[i], strText) ; 
				  }
				  for (int i2=0; i2<3; i2++)
				  {
					  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(5+i2) ;
					  pt_3d_topleft.y = pt_3d_cur_base.y-dCellHeight/2 ;
					  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(6+i2);
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight ;
					  if (i2==2)
					  {
						  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*14 ;
						  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*15;
					  }
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  strArrayText.RemoveAll() ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  
					  CString strText = _T("") ;
					  if (strArrayText.GetSize()>0)
					  {
						  strText = strArrayText.GetAt(0) ;
					  }
					  mapCircuitTypeToStr.SetAt(strTypeNum2[i2], strText) ; 
				  }

				  //插入配线用电路图
				  CString strCircuitTemplete ;
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\GD_1F1J.dwg"), m_strSysBasePath) ;
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec); 					
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;

				  iPxCircuitIndex++ ;

				  iCurRow++ ;
			  }
			  else if (strGDType==_T("一送二受")||strGDType==_T("一送两受"))
			  {
				  CMapStringToString mapCircuitTypeToStr ;

				  CString strTypeNum_GD[2] = {_T("27"), _T("29")};
				  pt_3d_topleft.x = pt_3d_cur_base.x ;
				  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0 ;
				  for (int iGdIndex=0; iGdIndex<2; iGdIndex++)
				  {
					  pt_3d_topleft.y = pt_3d_cur_base.y+dCellHeight*iGdIndex ;
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight*(iGdIndex+1) ;
					  
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  strArrayText.RemoveAll() ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  
					  //轨道名称
					  CString strGDName = _T("");
					  if (strArrayText.GetSize()>0)
					  {
						  strGDName = strArrayText.GetAt(0) ;
					  }
					  mapCircuitTypeToStr.SetAt(strTypeNum_GD[iGdIndex], strGDName) ;
				  }
				  
				  
				  CString strTypeNum[2][12] = {
					  {_T("443"),_T("469"),_T("183"),_T("209"),_T("235"),_T("261"),_T("287"),_T("313"),_T("339"),_T("365"),_T("391"),_T("417")},
					  {_T("445"),_T("0"),_T("185"),_T("211"),_T("237"),_T("263"),_T("289"),_T("315"),_T("341"),_T("367"),_T("0"),_T("0")}
				  };
				  CString strTypeNum2[2][3] = {
					  {_T("184"),_T("210"),_T("418")},
					  {_T("186"),_T("212"),_T("0")}
				  };
				  for (int iRow=0; iRow<2; iRow++)
				  {			  
					  for (int i=0; i<12; i++)
					  {
						  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(3+i) ;
						  pt_3d_topleft.y = pt_3d_cur_base.y-iRow*dCellHeight ;
						  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(4+i);
						  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight*(iRow+1) ;
						  if ((i>1&&i<8)||i==11)
						  {
							  pt_3d_bottomright.y = pt_3d_cur_base.y-iRow*dCellHeight-dCellHeight/2 ;
						  }
						  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
						  mysys.SortEnt(arrayText, 2) ;
						  strArrayText.RemoveAll() ;
						  mysys.GetHaveSortedText(arrayText, strArrayText) ;
						  mysys.DestroyEntInArray(arrayText) ;
						  
						  CString strText = _T("") ;
						  if (strArrayText.GetSize()>0)
						  {
							  strText = strArrayText.GetAt(0) ;
							  if (i==0)
							  {
								  CString strTmp ;
								  strTmp = strText.Left(strText.GetLength()-1) ;
								  strText = strTmp ;
							  }
							  else if (i==1)
							  {
								  CString strTmp ;
								  int iLocFind = strText.Find(_T("(")) ;
								  if (iLocFind!=-1)
								  {
									  strTmp = strText.Left(iLocFind) ;
								  }								
								  strText = strTmp ;
							  }
						  }
						  mapCircuitTypeToStr.SetAt(strTypeNum[iRow][i], strText) ; 
					  }
					  for (int i2=0; i2<3; i2++)
					  {
						  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(5+i2) ;
						  pt_3d_topleft.y = pt_3d_cur_base.y-iRow*dCellHeight-dCellHeight/2 ;
						  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(6+i2);
						  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight*(iRow+1) ;
						  if (i2==2)
						  {
							  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*14 ;
							  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*15;
						  }
						  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
						  mysys.SortEnt(arrayText, 2) ;
						  strArrayText.RemoveAll() ;
						  mysys.GetHaveSortedText(arrayText, strArrayText) ;
						  mysys.DestroyEntInArray(arrayText) ;
						  
						  CString strText = _T("") ;
						  if (strArrayText.GetSize()>0)
						  {
							  strText = strArrayText.GetAt(0) ;
						  }
						  mapCircuitTypeToStr.SetAt(strTypeNum2[iRow][i2], strText) ; 
					  }
				  }
				  
				  //插入配线用电路图
				  CString strCircuitTemplete ;
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\GD_1F2J.dwg"), m_strSysBasePath) ;
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec); 					
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				  
				  iPxCircuitIndex++ ;
				  iCurRow+=2 ;
			  }
			  else if (strGDType==_T("一送三受"))
			  {
				  CMapStringToString mapCircuitTypeToStr ;

				  CString strTypeNum_GD[3] = {_T("27"), _T("29"), _T("30")};
				  pt_3d_topleft.x = pt_3d_cur_base.x ;
				  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0 ;
				  for (int iGdIndex=0; iGdIndex<3; iGdIndex++)
				  {
					  pt_3d_topleft.y = pt_3d_cur_base.y+dCellHeight*iGdIndex ;
					  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight*(iGdIndex+1) ;
					  
					  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
					  mysys.SortEnt(arrayText, 2) ;
					  strArrayText.RemoveAll() ;
					  mysys.GetHaveSortedText(arrayText, strArrayText) ;
					  mysys.DestroyEntInArray(arrayText) ;
					  
					  //轨道名称
					  CString strGDName = _T("");
					  if (strArrayText.GetSize()>0)
					  {
						  strGDName = strArrayText.GetAt(0) ;
					  }
					  mapCircuitTypeToStr.SetAt(strTypeNum_GD[iGdIndex], strGDName) ;
				  }
				  
				  
				  CString strTypeNum[3][12] = {
					  {_T("443"),_T("469"),_T("183"),_T("209"),_T("235"),_T("261"),_T("287"),_T("313"),_T("339"),_T("365"),_T("391"),_T("417")},
					  {_T("445"),_T("0"),_T("185"),_T("211"),_T("237"),_T("263"),_T("289"),_T("315"),_T("341"),_T("367"),_T("0"),_T("0")},
					  {_T("444"),_T("0"),_T("187"),_T("188"),_T("264"),_T("265"),_T("302"),_T("305"),_T("300"),_T("301"),_T("0"),_T("0")}
				  };
				  CString strTypeNum2[3][3] = {
					  {_T("184"),_T("210"),_T("418")},
					  {_T("186"),_T("212"),_T("0")},
					  {_T("303"),_T("304"),_T("0")}
				  };
				  for (int iRow=0; iRow<3; iRow++)
				  {			  
					  for (int i=0; i<12; i++)
					  {
						  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(3+i) ;
						  pt_3d_topleft.y = pt_3d_cur_base.y-iRow*dCellHeight ;
						  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(4+i);
						  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight*(iRow+1) ;
						  if ((i>1&&i<8)||i==11)
						  {
							  pt_3d_bottomright.y = pt_3d_cur_base.y-iRow*dCellHeight-dCellHeight/2 ;
						  }
						  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
						  mysys.SortEnt(arrayText, 2) ;
						  strArrayText.RemoveAll() ;
						  mysys.GetHaveSortedText(arrayText, strArrayText) ;
						  mysys.DestroyEntInArray(arrayText) ;
						  
						  CString strText = _T("") ;
						  if (strArrayText.GetSize()>0)
						  {
							  strText = strArrayText.GetAt(0) ;
							  if (i==0)
							  {
								  CString strTmp ;
								  strTmp = strText.Left(strText.GetLength()-1) ;
								  strText = strTmp ;
							  }
							  else if (i==1)
							  {
								  CString strTmp ;
								  int iLocFind = strText.Find(_T("(")) ;
								  if (iLocFind!=-1)
								  {
									  strTmp = strText.Left(iLocFind) ;
								  }
								  strText = strTmp ;
							  }
						  }
						  mapCircuitTypeToStr.SetAt(strTypeNum[iRow][i], strText) ; 
					  }
					  for (int i2=0; i2<3; i2++)
					  {
						  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(5+i2) ;
						  pt_3d_topleft.y = pt_3d_cur_base.y-iRow*dCellHeight-dCellHeight/2 ;
						  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*(6+i2);
						  pt_3d_bottomright.y = pt_3d_cur_base.y-dCellHeight*(iRow+1) ;
						  if (i2==2)
						  {
							  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*14 ;
							  pt_3d_bottomright.x = pt_3d_cur_base.x+dCellWidth0+dCellWidth*15;
						  }
						  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
						  mysys.SortEnt(arrayText, 2) ;
						  strArrayText.RemoveAll() ;
						  mysys.GetHaveSortedText(arrayText, strArrayText) ;
						  mysys.DestroyEntInArray(arrayText) ;
						  
						  CString strText = _T("") ;
						  if (strArrayText.GetSize()>0)
						  {
							  strText = strArrayText.GetAt(0) ;
						  }
						  mapCircuitTypeToStr.SetAt(strTypeNum2[iRow][i2], strText) ; 
					  }
				  }
				  
				  //插入配线用电路图
				  CString strCircuitTemplete ;
				  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\GD_1F3J.dwg"),m_strSysBasePath) ;
				  AcGeMatrix3d xform;  
				  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
				  xform.setToTranslation(vec); 					
				  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
				  
				  iPxCircuitIndex++ ;

				  iCurRow+=3 ;
			  }
			  else
			  {
				  iCurRow++ ;
			  }
		  }
		  else
		  {
			  iCurRow++ ;
		  }
	  }

  }
  //一体化轨道驱采
  else if (strBlockNameTmp==_T("dlt_yth_qc_tb"))
  {
	  double dCellHeight = 5 ;
	  double dCellWidth = 17 ;
	  int iRowSum = 49 ;
	  AcGePoint3d pt_3d_cur_base(pt_3d_ref) ;

	  for (int iCurRow=0; iCurRow<iRowSum; iCurRow++)
	  {
		  pt_3d_cur_base.y = pt_3d_ref.y-iCurRow*dCellHeight ;
		  //单元格左上角、右下角
		  AcGePoint3d pt_3d_topleft(pt_3d_cur_base.x, pt_3d_cur_base.y, 0) ;
          AcGePoint3d pt_3d_bottomright(pt_3d_cur_base.x+dCellWidth, pt_3d_cur_base.y-dCellHeight, 0) ;

		  CArray<AcDbText *, AcDbText *> arrayText ;
		  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
		  mysys.SortEnt(arrayText, 2) ;
		  CStringArray strArrayText ;
		  mysys.GetHaveSortedText(arrayText, strArrayText) ;
		  mysys.DestroyEntInArray(arrayText) ;

		  //轨道名称
		  if (strArrayText.GetSize()>0)
		  {
			  CMapStringToString mapCircuitTypeToStr ;

			  CString strGDName = strArrayText.GetAt(0) ;
			  mapCircuitTypeToStr.SetAt(_T("27"), strGDName) ;

			  pt_3d_topleft.x+=dCellWidth ;
			  pt_3d_bottomright.x+=dCellWidth ;
			  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
			  mysys.SortEnt(arrayText, 2) ;
			  strArrayText.RemoveAll() ;
			  mysys.GetHaveSortedText(arrayText, strArrayText) ;
			  mysys.DestroyEntInArray(arrayText) ;
			  if (strArrayText.GetSize()>0)
			  {
				  CString strZLocTmp = strArrayText.GetAt(0) ;
				  if (CBlkUtility::IsStrMatch(_T("^(Z[1-9]\\d*-[1-9]\\d*)\\(([1-9])\\)$"), strZLocTmp))
				  {
					  CString strZLoc = _T("") ;
					  CString strIndexInBox = _T("") ;
					  int iIndexInBox = 0 ;
					  CBlkUtility::RegReplace(_T("^(Z[1-9]\\d*-[1-9]\\d*)\\(([1-9])\\)$"),_T("$1"), strZLocTmp, strZLoc) ;
					  CBlkUtility::RegReplace(_T("^(Z[1-9]\\d*-[1-9]\\d*)\\(([1-9])\\)$"),_T("$2"), strZLocTmp, strIndexInBox) ;
					  iIndexInBox = _tstoi(strIndexInBox) ;
					  mapCircuitTypeToStr.SetAt(_T("339"), strZLoc) ;

					  CString strTypeNum[5] = {_T("209"),_T("261"),_T("235"),_T("313"),_T("287")} ; 
					  CString strTypeNum2[5] = {_T("79"),_T("131"),_T("105"),_T("157"),_T("183")} ;

					  for (int i=0; i<4; i++)
					  {
						  if (i<3)
						  {
							  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*2+dCellWidth*5*i+dCellWidth*(iIndexInBox-1);
							  pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
							  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
							  mysys.SortEnt(arrayText, 2) ;
							  strArrayText.RemoveAll() ;
							  mysys.GetHaveSortedText(arrayText, strArrayText) ;
							  mysys.DestroyEntInArray(arrayText) ;
							  CString strJkLoc = _T("") ;
							  if (strArrayText.GetSize()>0)
							  {
								  strJkLoc = strArrayText.GetAt(0) ;
							  }
							  mapCircuitTypeToStr.SetAt(strTypeNum[i], strJkLoc) ;
						  }
						  else if(i==3)
						  {
							  pt_3d_topleft.x = pt_3d_cur_base.x+dCellWidth*2+dCellWidth*5*i+dCellWidth*2*(iIndexInBox-1);
							  pt_3d_bottomright.x = pt_3d_topleft.x+dCellWidth ;
							  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
							  mysys.SortEnt(arrayText, 2) ;
							  strArrayText.RemoveAll() ;
							  mysys.GetHaveSortedText(arrayText, strArrayText) ;
							  mysys.DestroyEntInArray(arrayText) ;
							  CString strIOZ = _T("") ;
							  if (strArrayText.GetSize()>0)
							  {
								  strIOZ = strArrayText.GetAt(0) ;
							  }
							  mapCircuitTypeToStr.SetAt(strTypeNum[4], strIOZ) ;

							  pt_3d_topleft.x +=dCellWidth ;
							  pt_3d_bottomright.x += dCellWidth ;
							  mysys.FindTxtInArea(pt_3d_topleft, pt_3d_bottomright, arrayText) ;
							  mysys.SortEnt(arrayText, 2) ;
							  strArrayText.RemoveAll() ;
							  mysys.GetHaveSortedText(arrayText, strArrayText) ;
							  mysys.DestroyEntInArray(arrayText) ;
							  CString strIOZ2 = _T("") ;
							  if (strArrayText.GetSize()>0)
							  {
								  strIOZ2 = strArrayText.GetAt(0) ;
							  }
							  mapCircuitTypeToStr.SetAt(strTypeNum[3], strIOZ2) ;
						  }				  
					  }

					  //计算端子
					  try
					  {
						  CADORecordset cSetFind ;
						  CString strSql ;
						  strSql.Format(_T("select * from node_collect where dwg_type='YPGD-QC' and index=%d"), iIndexInBox) ;

						  if (cSetFind.IsOpen())
						  {
							  cSetFind.Close() ;
						  }
						  if (cSetFind.Open(g_PtrCon, strSql))
						  {
							  if (!cSetFind.IsBOF())
							  {
								  cSetFind.MoveFirst() ;
							  }
							  int iNodeIndex = 0 ;
							  while (!cSetFind.IsEOF())
							  {
								  CString strNode = _T("") ;
								  cSetFind.GetFieldValue(_T("node"), strNode) ;
								  mapCircuitTypeToStr.SetAt(strTypeNum2[iNodeIndex], strNode) ;

								  iNodeIndex++ ;
								  cSetFind.MoveNext() ;
							  }
							  
							  cSetFind.Close() ;
						  }

					  }
					  catch (_com_error& e)
					  {
						  CString sBuff = CBlkUtility::GetErrorDescription(e) ;
						  AfxMessageBox(sBuff) ;  
					  }
					  catch (...)
					  {
						  AfxMessageBox(_T("unknown error in ComputeOneTypeCircuit - ythqc")) ;
                      } 

					  //插入配线用电路图
					  CString strCircuitTemplete ;
					  strCircuitTemplete.Format(_T("%s\\support\\xls_grid_dwg\\GD_ZNYP_DRIVE.dwg"), m_strSysBasePath) ;
					  AcGeMatrix3d xform;  
					  AcGeVector3d vec(0, -iPxCircuitIndex*dPxCircuitHeight, 0); 
					  xform.setToTranslation(vec); 					
					  this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapCircuitTypeToStr, _T("XLS_GRID_TYPE"), iPxCircuitIndex*iIdAddPlus) ;
					  
					  iPxCircuitIndex++ ;
					  
				  }
				  else
				  {
					  continue ;
				  }
			  }
		  }
	  }
  }  
  
  return bRet ;  
  
}

int CStationSystem::Compute_18_s_indoor_Circuit()
{	
	int iRet = 0 ;
	
	double dDwgHeight = 297 ;
	double dDwgWidth = 1052 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 25 ;
	double dCellWidth = 18 ;
	double dTbWidth = dCellWidth*12 ;
	double dBetweenTb = 10 ;
	int iRowNum = 9 ;
	int iTbNum = 2 ;	

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;//电路图
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		CLongArray nArrayId, nArrayOtherId ;

		strSql.Format(_T("select * from switch where dev_size=18 and dev_num=5 and other_id>id")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if (cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}
			while (!cSetAll.IsEOF())
			{
				int nId = 0, nOtherId = 0 ;
				cSetAll.GetFieldValue(_T("id"), nId) ;
				cSetAll.GetFieldValue(_T("other_id"), nOtherId) ;
				nArrayId.Add(nId) ;
				nArrayOtherId.Add(nOtherId) ;

				cSetAll.MoveNext() ;
			}
			cSetAll.Close() ;
		}

		int nIdSum = nArrayId.GetSize() ;

		for (int iIdIndex=0; iIdIndex<nIdSum; iIdIndex++)
		{
			for (int iIdType=0; iIdType<2; iIdType++)
			{
				int nCurId = 0 ;
				if (iIdType==0)
				{
					nCurId = nArrayId.GetAt(iIdIndex) ;
				}
				else
				{
					nCurId = nArrayOtherId.GetAt(iIdIndex) ;
				}

				strSql.Format(_T("select * from switch where id=%d"), nCurId) ;
				if (cSetAll.IsOpen())
				{
					cSetAll.Close() ;
				}
				if(cSetAll.Open(g_PtrCon, strSql))
				{
					if (!cSetAll.IsBOF())
					{
						cSetAll.MoveFirst() ;
					}

					int iRsCount = cSetAll.GetRecordCount() ;
					CString strBarMsg ;
					strBarMsg.Format(_T("正在计算18_s_3J+2X 室内电路")) ;
					acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
					
					while (!cSetAll.IsEOF())
					{
						acedSetStatusBarProgressMeterPos(iRet) ;

						CMapStringToString mapCircuitTypeToStr ;
						CADORecordset cSetFind ;
						
						CString strName = _T("") ;
						
						ads_point pt_base_draw = {0, 0, 0} ;
						ads_point pt_base_draw_tmp = {0, 0, 0} ;
						pt_base_draw[0] = (iIdIndex/iRowNum)%iTbNum*(dTbWidth+dBetweenTb)+dCellWidth*6*iIdType ;
						pt_base_draw[1] = 0-(iIdIndex/(iRowNum*iTbNum))*(dDwgHeight+dBetweenDwg)-(iIdIndex%iRowNum)*dCellHeight ;
						
						//道岔名称
						cSetAll.GetFieldValue(_T("name"), strName) ;
						pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth/4 ;
						pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/2 ;
						//mysys.WriteText(pt_base_draw_tmp, strName, 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
						
						//TDZ组合位置
						CString strTDZLoc = _T("") ;
						int iTDZShelf = 0, iTDZLayer = 0 ;
						cSetAll.GetFieldValue(_T("shelf_num"), iTDZShelf) ;
						cSetAll.GetFieldValue(_T("layer_num"), iTDZLayer) ;
						strTDZLoc.Format(_T("Z%d-%d"), iTDZShelf, iTDZLayer) ;
						pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth+dCellWidth/2 ;
						//mysys.WriteText(pt_base_draw_tmp, strTDZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strTDZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
						
						//BHZ组合位置
						CString strBHZLoc = _T("") ;
						strSql.Format(_T("select * from z_place where dev_name='%s' and type='BHZ'"), strName) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}
							while (!cSetFind.IsEOF())
							{
								int iBHZShelf = 0, iBHZLayer = 0;
								cSetFind.GetFieldValue(_T("shelf_num"), iBHZShelf) ;
								cSetFind.GetFieldValue(_T("layer_num"), iBHZLayer) ;
								strBHZLoc.Format(_T("Z%d-%d"), iBHZShelf, iBHZLayer) ;
								
								cSetFind.MoveNext() ;
							}
							cSetFind.Close() ;
						}
						pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*3+dCellWidth/2 ;
						//mysys.WriteText(pt_base_draw_tmp, strBHZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strBHZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
						
						//GJ-Z/ GJ-Q组合位置
						int iGdShelf = 0, iGdLayer = 0 ; //轨道所在组合位置
						CString strGJLoc = _T("") ; //轨道组合位置字符串
						CString strGJZNode = _T(""), strGJQNode = _T("") ;//类似02-13
						CString strGJZAll = _T(""), strGJQAll = _T("") ; //类似Z2-502-13
						CString strGdName = _T("") ; //道岔所在轨道的名称
						CString strSwitchCircuitType = _T("") ; //一体化轨道电路和25Hz轨道电路,(ZGZ/GFZ)
						int iJLoc = 0 ; //所用继电器位置
						int iSwitchGdType = 0 ; //道岔所在轨道类型，0表示该轨道命名由1个道岔名，比如3DG；1表示由2个，比如18-32DG
						int iSwitchInGdIndex = 0 ;//在类似18-32DG中18的iSwitchInGdIndex为0，32的iSwitchInGdIndex为1
						strSql.Format(_T("select * from z_gd_place where gd_name like '%%%s%%DG'"), strName) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}
							BOOL bFound = FALSE ;
							while (!cSetFind.IsEOF())
							{			
								CString strGdNameTmp = _T("") ;
								cSetFind.GetFieldValue(_T("gd_name"), strGdNameTmp) ;
								if (strGdNameTmp.Right(2)==_T("DG"))
								{
									if (strGdNameTmp.Left(strGdNameTmp.GetLength()-2)==strName)
									{
										iSwitchGdType = 0 ;
										strGdName = strGdNameTmp ;
										bFound = TRUE ;								
									}
									else if (strGdNameTmp.Find(_T("-"))!=-1)
									{
										CString strGdNameTmp2 = strGdNameTmp.Left(strGdNameTmp.GetLength()-2) ;
										CStringArray strArraySwitch ;
										CBlkUtility::DivideString(strGdNameTmp2, _T("-"), strArraySwitch) ;								
										for (int iSwitchIndex=0; iSwitchIndex<strArraySwitch.GetSize(); iSwitchIndex++)
										{									
											if (strArraySwitch.GetAt(iSwitchIndex)==strName)
											{
												iSwitchGdType = 1 ;
												strGdName = strGdNameTmp ;
												iSwitchInGdIndex = iSwitchIndex ;
												bFound = TRUE ;										
											}
										}
									}
								}
								
								if (bFound)
								{
									cSetFind.GetFieldValue(_T("shelf_num"), iGdShelf) ;
									cSetFind.GetFieldValue(_T("layer_num"), iGdLayer) ;
									cSetFind.GetFieldValue(_T("j_loc"), iJLoc) ;
									strGJLoc.Format(_T("Z%d-%d"), iGdShelf, iGdLayer) ;
									break ;
								}	
								
								cSetFind.MoveNext() ;
							}
							cSetFind.Close() ;
						}
						//查询组合类型是ZGZ / GFZ
						if (!strGJLoc.IsEmpty())
						{
							strSql.Format(_T("select * from z_type where shelf_num=%d and layer_num=%d"), iGdShelf, iGdLayer) ;
							if (cSetFind.IsOpen())
							{
								cSetFind.Close() ;
							}
							if (cSetFind.Open(g_PtrCon, strSql))
							{
								if (!cSetFind.IsBOF())
								{
									cSetFind.MoveFirst() ;
								}						
								while (!cSetFind.IsEOF())
								{
									cSetFind.GetFieldValue(_T("type"), strSwitchCircuitType) ;
									cSetFind.MoveNext() ;
								}
								cSetFind.Close() ;						
							}
						}
						//查找GJ-Z GJ-Q 端子
						if (!strSwitchCircuitType.IsEmpty())
						{
							if (strSwitchCircuitType==_T("ZGZ"))
							{
								iJLoc = (iJLoc-1)/2+1 ;
							}
							int iLocFirstNum[3] = {3,4,5} ;
							
							strSql.Format(_T("select * from node_collect where dwg_type='%s' and index=%d and left(loc,1)='%d'"), strSwitchCircuitType, iJLoc, iLocFirstNum[iSwitchInGdIndex] ) ;
							if (cSetFind.IsOpen())
							{
								cSetFind.Close() ;
							}
							if (cSetFind.Open(g_PtrCon, strSql))
							{
								if (!cSetFind.IsBOF())
								{
									cSetFind.MoveFirst() ;
								}						
								while (!cSetFind.IsEOF())
								{
									CString strNodeTmp = _T("") ;
									CString strLoc = _T("") ;
									cSetFind.GetFieldValue(_T("node"), strNodeTmp) ;
									cSetFind.GetFieldValue(_T("loc"), strLoc) ;
									if (strLoc.Right(1)==_T("1"))
									{
										strGJZNode = strNodeTmp ;
									}
									else if (strLoc.Right(1)==_T("2"))
									{
										strGJQNode = strNodeTmp ;
									}
									cSetFind.MoveNext() ;
								}
								cSetFind.Close() ;						
							}
						}
						strGJZAll.Format(_T("%s%s"),strGJLoc, strGJZNode) ;
						strGJQAll.Format(_T("%s%s"),strGJLoc, strGJQNode) ;
						pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*4+dCellWidth/2 ;
						//mysys.WriteText(pt_base_draw_tmp, strGJZAll, 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strGJZAll, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
						pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*5+dCellWidth/2 ;
						//mysys.WriteText(pt_base_draw_tmp, strGJQAll, 0, AcDb::kTextMid, 2.5, 0.75) ;
						mysys.AddText(strGJQAll, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
						
						//TDF组合位置
						CString strTDF_Dev[5] ={ _T("J1"), _T("J2"), _T("J3"), _T("X1"), _T("X2")} ;
						CString strTDF_ZLoc[5] ={ _T(""), _T(""), _T(""), _T(""), _T("")} ;
						
						for(int iTDFIndex=0; iTDFIndex<5; iTDFIndex++)
						{					
							strSql.Format(_T("select * from z_place where dev_name='%s-%s' and type='TDF' "), strName, strTDF_Dev[iTDFIndex]) ;
							if (cSetFind.IsOpen())
							{
								cSetFind.Close() ;
							}
							if (cSetFind.Open(g_PtrCon, strSql))
							{
								if (!cSetFind.IsBOF())
								{
									cSetFind.MoveFirst() ;
								}						
								while (!cSetFind.IsEOF())
								{
									int iTDFShelf = 0, iTDFLayer = 0 ;							
									cSetFind.GetFieldValue(_T("shelf_num"), iTDFShelf) ;
									cSetFind.GetFieldValue(_T("layer_num"), iTDFLayer) ;
									strTDF_ZLoc[iTDFIndex].Format(_T("Z%d-%d"), iTDFShelf, iTDFLayer) ;	
									cSetFind.MoveNext() ;
								}
								cSetFind.Close() ;						
							}
						}
						for (int iTDFIndex2=0; iTDFIndex2<5; iTDFIndex2++)
						{
							pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*2+dCellWidth/2 ;
							pt_base_draw_tmp[1] = pt_base_draw[1]-(dCellHeight/5)*iTDFIndex2-dCellHeight/10 ;
							//mysys.WriteText(pt_base_draw_tmp, strTDF_ZLoc[iTDFIndex2], 0, AcDb::kTextMid, 2.5, 0.75) ;
							mysys.AddText( strTDF_ZLoc[iTDFIndex2], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
							
						}
						
						iRet++ ;
						cSetAll.MoveNext() ;
					}
					cSetAll.Close() ;
					
					acedRestoreStatusBar() ;			
				}
			}
		}

		//begin 绘制图框（包括电路示意图，表格等）
		int iDwgSum = nIdSum/(iRowNum*iTbNum)+(nIdSum%(iRowNum*iTbNum)==0?0:1) ;
		CString strDate ;
		strDate = CBlkUtility::GetDateStr() ;
		for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
		{
			AcGeMatrix3d xform;  
			AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
			xform.setToTranslation(vec); 
			CMapStringToString mapTypeToStr ;
			mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
			CString strDwgType ; 
			strDwgType.Format(_T("双动道岔(3J+2X)室内控制电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
			mapTypeToStr.SetAt(_T("2"), strDwgType) ;
			mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
			mapTypeToStr.SetAt(_T("4"), strDate) ;
			CString strDltFile ;
			strDltFile.Format(_T("%s\\support\\dlt\\dlt_dc_18_s_3J+2X_indoor.dwg"), m_strSysBasePath) ;
			this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
			
		}	
		
		//end 绘制图框（包括电路示意图，表格等）
		
		mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_DC_18_S_3J+2X_INDOOR")) ;
		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
		CString strSaveFilePath ;
		strSaveFilePath.Format(_T("%s\\18S_J(3J)+X(2J)-S700K-室内.dwg"), this->m_strSavePath) ;
		pCurDb->saveAs(strSaveFilePath) ;
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
		AfxMessageBox(_T("unknown error in Compute_18_s_indoor_Circuit")) ;
	} 	
	
	return iRet ;


}

int CStationSystem::Compute_18_s_qc_Circuit()
{
	int iRet = 0 ;
	
	double dDwgHeight = 297 ;
	double dDwgWidth = 948 ;
	double dBetweenDwg = 10 ;
	double dCellHeight = 20 ;
	double dCellWidth = 19 ;
	double dTbWidth = dCellWidth*14 ;
	double dBetweenTb = 10 ;
	int iRowNum = 11 ;
	int iTbNum = 3 ;	

	try	
	{
		AcDbDatabase *pCurDb = new AcDbDatabase ;//电路图
		CUserSystem mysys ;
		mysys.m_pDb = pCurDb ;

		CADORecordset cSetAll ;
		CString strSql ;

		strSql.Format(_T("select * from switch where dev_size=18 and dev_num=5 and other_id>id")) ;
		if (cSetAll.IsOpen())
		{
			cSetAll.Close() ;
		}
		if(cSetAll.Open(g_PtrCon, strSql))
		{
			if (!cSetAll.IsBOF())
			{
				cSetAll.MoveFirst() ;
			}

			int iRsCount = cSetAll.GetRecordCount() ;
			CString strBarMsg ;
			strBarMsg.Format(_T("正在计算18_s_3J+2X 驱采电路")) ;
			acedSetStatusBarProgressMeter(strBarMsg, 0, iRsCount-1) ;	
		
			while (!cSetAll.IsEOF())
			{
				acedSetStatusBarProgressMeterPos(iRet) ;

				CADORecordset cSetFind ;

				int nOtherId = 0 ;
				cSetAll.GetFieldValue(_T("other_id"), nOtherId) ;
				CString strOtherName = _T("") ;
				strSql.Format(_T("select * from switch where id=%d"), nOtherId) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						cSetFind.GetFieldValue(_T("name"), strOtherName) ;
						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}

				CString strName = _T("") ;

				ads_point pt_base_draw = {0, 0, 0} ;
				ads_point pt_base_draw_tmp = {0, 0, 0} ;
				pt_base_draw[0] = (iRet/iRowNum)%iTbNum*(dTbWidth+dBetweenTb) ;
				pt_base_draw[1] = 0-(iRet/(iRowNum*iTbNum))*(dDwgHeight+dBetweenDwg)-(iRet%iRowNum)*dCellHeight ;

				//道岔名称
				cSetAll.GetFieldValue(_T("name"), strName) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/2 ;
				CString strAllName = _T("") ;
				strAllName.Format(_T("%s/%s"), strName, strOtherName) ;
				//mysys.WriteText(pt_base_draw_tmp, strAllName, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strAllName, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//TDZ组合位置
				CString strTDZLoc = _T("") ;
				int iTDZShelf = 0, iTDZLayer = 0 ;
				cSetAll.GetFieldValue(_T("shelf_num"), iTDZShelf) ;
				cSetAll.GetFieldValue(_T("layer_num"), iTDZLayer) ;
				strTDZLoc.Format(_T("Z%d-%d"), iTDZShelf, iTDZLayer) ;
				pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth+dCellWidth/2 ;
				pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/8 ;
				//mysys.WriteText(pt_base_draw_tmp, strTDZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
				mysys.AddText(strTDZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				//1BHZ/2BHZ组合位置
				CString	strArrayName[2] = {_T(""), _T("")} ;
				strArrayName[0] = strName ;
				strArrayName[1] = strOtherName ;
				for (int iBHZIndex=0; iBHZIndex<2; iBHZIndex++)
				{
					CString str1BHZLoc = _T("") ;
					strSql.Format(_T("select * from z_place where dev_name='%s' and type='BHZ'"), strArrayName[iBHZIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}
						while (!cSetFind.IsEOF())
						{
							int iBHZShelf = 0, iBHZLayer = 0;
							cSetFind.GetFieldValue(_T("shelf_num"), iBHZShelf) ;
							cSetFind.GetFieldValue(_T("layer_num"), iBHZLayer) ;
							str1BHZLoc.Format(_T("Z%d-%d"), iBHZShelf, iBHZLayer) ;
							
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;
					}
					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*(iBHZIndex+1)/4-dCellHeight/8 ;
					//mysys.WriteText(pt_base_draw_tmp, str1BHZLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(str1BHZLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				}

				
				CString strOutZHName[9] = {_T("DCJ"),_T("FCJ"),_T("YCJ"),_T("SJ"),_T("JGAJ"),_T("XGAJ"),_T("FWJ"),_T("JGAJ"),_T("XGAJ")} ;
				CString strOutZHNode[9] = {_T("01-1"),_T("01-2"),_T("01-3"),_T("01-4"),_T("03-1"),_T("03-2"),_T("03-7"),_T("03-1"),_T("03-2")} ;
				for (int iOutIndex=0; iOutIndex<9; iOutIndex++)
				{
					CString strJkLoc = _T("") ;

					CString strNameTmp = strAllName ;
					if (iOutIndex>3&&iOutIndex<6)
					{
						strNameTmp = strName ;
					}
					else if (iOutIndex>6)
					{
						strNameTmp = strOtherName ;
					}

					strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=0"), strNameTmp, strOutZHName[iOutIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							int iJkLayer = 0,iJkBox = 0,iJkCell = 0 ;							
							cSetFind.GetFieldValue(_T("layer_num"), iJkLayer) ;
							cSetFind.GetFieldValue(_T("box_num"), iJkBox) ;
							cSetFind.GetFieldValue(_T("cell_num"), iJkCell) ;
							strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayer, iJkBox, iJkCell) ;	
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}

					pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*(2+iOutIndex)+dCellWidth/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*3/4-dCellHeight/8 ;
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;
					if (iOutIndex<4)
					{
						pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/8 ;
					}
					else if(iOutIndex<7)
					{
						pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight/4-dCellHeight/8 ;
					}
					else
					{
						pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*2/4-dCellHeight/8 ;
					}
					//mysys.WriteText(pt_base_draw_tmp, strOutZHNode[iOutIndex], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strOutZHNode[iOutIndex], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				}

				CString strInZHName[3] = {_T("DBJ"),_T("FBJ"),_T("YCJ")} ;
				CString strInZHNode[3] = {_T("02-1"),_T("02-2"),_T("04-3")} ;
				for (int iInIndex=0; iInIndex<3; iInIndex++)
				{
					CString strJkLoc = _T("") ;
					
					strSql.Format(_T("select * from interlock_outin_place where dev_name='%s%s' and type=1"), strAllName, strInZHName[iInIndex]) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (!cSetFind.IsBOF())
						{
							cSetFind.MoveFirst() ;
						}						
						while (!cSetFind.IsEOF())
						{
							int iJkLayer = 0,iJkBox = 0,iJkCell = 0 ;							
							cSetFind.GetFieldValue(_T("layer_num"), iJkLayer) ;
							cSetFind.GetFieldValue(_T("box_num"), iJkBox) ;
							cSetFind.GetFieldValue(_T("cell_num"), iJkCell) ;
							strJkLoc.Format(_T("JK-%d0%d-%d"), iJkLayer, iJkBox, iJkCell) ;	
							cSetFind.MoveNext() ;
						}
						cSetFind.Close() ;						
					}
					
					pt_base_draw_tmp[0] = pt_base_draw[0]+dCellWidth*(11+iInIndex)+dCellWidth/2 ;
					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*3/4-dCellHeight/8 ;
					//mysys.WriteText(pt_base_draw_tmp, strJkLoc, 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strJkLoc, pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

					pt_base_draw_tmp[1] = pt_base_draw[1]-dCellHeight*3/8 ;
					//mysys.WriteText(pt_base_draw_tmp, strInZHNode[iInIndex], 0, AcDb::kTextMid, 2.5, 0.75) ;
					mysys.AddText(strInZHNode[iInIndex], pt_base_draw_tmp, pt_base_draw_tmp, AcDb::kTextMid, AcDb::kTextBase, 2.5, 0.75, 0, 0) ;

				}	

				iRet++ ;
				cSetAll.MoveNext() ;
			}
			cSetAll.Close() ;

			//begin 绘制图框（包括电路示意图，表格等）
			int iDwgSum = iRsCount/(iRowNum*iTbNum)+(iRsCount%(iRowNum*iTbNum)==0?0:1) ;
			CString strDate ;
			strDate = CBlkUtility::GetDateStr() ;
			for (int iDwgIndex=0; iDwgIndex<iDwgSum; iDwgIndex++)
			{
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0, -iDwgIndex*(dDwgHeight+dBetweenDwg), 0); 
				xform.setToTranslation(vec); 
				CMapStringToString mapTypeToStr ;
				mapTypeToStr.SetAt(_T("1"), m_strStationName) ;
// 				CString strDwgType ; 
// 				strDwgType.Format(_T("单动道岔(3J+2X)室内控制电路图(%s)"), CBlkUtility::DigitToCapsString(iDwgIndex+1)) ;
// 				mapTypeToStr.SetAt(_T("2"), strDwgType) ;
				mapTypeToStr.SetAt(_T("3"), m_strDwgNum) ;
				mapTypeToStr.SetAt(_T("4"), strDate) ;
				CString strDltFile ;
				strDltFile.Format(_T("%s\\support\\dlt\\dlt_dc_18_s_3J+2X_qc.dwg"), m_strSysBasePath) ;
				this->InsertDwgToDb(strDltFile, pCurDb, xform, mapTypeToStr, _T("TYPE"), iDwgIndex*4) ;
				
			}	

			//end 绘制图框（包括电路示意图，表格等）
			
			mysys.WriteToRec(_T("xh_station"), _T("draw_type"), _T("DLT_DC_18_S_3J+2X_QC")) ;
			mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx"), TRUE) ;
			CString strSaveFilePath ;
			strSaveFilePath.Format(_T("%s\\18S_J(3J)+X(2J)-S700K-驱采.dwg"), this->m_strSavePath) ;
			pCurDb->saveAs(strSaveFilePath) ;
			delete pCurDb ;
			pCurDb = NULL ;	
			
			acedRestoreStatusBar() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in Compute_18_s_qc_Circuit")) ;
	} 	
	
	return iRet ;
}

