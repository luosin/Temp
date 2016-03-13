// LKDataTable.cpp: implementation of the CLKDataTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "LKDataTable.h"
#include "DBUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLKDataTable::CLKDataTable()
{
	m_bAutoComputeXhjName = FALSE ;
	m_strMileSysName = _T("K") ;
	m_iLimitToFG = 1200 ;
}

CLKDataTable::~CLKDataTable()
{

}

void CLKDataTable::ReplaceOld()
{
	CUserSystem mysys ;

	ads_name ssname, temp_name ;
	AcDbObjectId blkObjID ;
	AcDbHandle blkhandle;
	acutPrintf(_T("\n选择要替换的实体")) ;
	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		long lLength = 0 ;
		CString strName = _T("") ;
		acedSSLength(ssname, &lLength) ;
		for (int i=0; i<lLength; i++)
		{
			acedSSName(ssname, i, temp_name) ;
			AcDbEntity *pEnt = NULL ;
			if (mysys.OpenAcDbEntity(temp_name, pEnt))
			{
				if (_tcscmp(pEnt->isA()->name(), _T("AcDbText"))==0)
				{
					AcDbText *pText = (AcDbText*)pEnt ;
					strName = pText->textString() ;
				}
				if (_tcscmp(pEnt->isA()->name(), _T("AcDbBlockReference"))==0)
				{
					AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;

					//得到块名
					CString strBlockName ;
					mysys.GetBlockName(pRef, strBlockName) ;
					strBlockName.MakeLower() ;
					if (strBlockName.Left(3)==_T("pmt"))
					{
						acdbGetObjectId(blkObjID, temp_name); 
					}
					else
					{
						pEnt->erase() ;
					}
				}
				else
				{
					pEnt->erase() ;
				}

				pEnt->close() ;
				pEnt = NULL ;
			}
		}

		AcDbEntity *pBlkEnt = NULL ;
		if (Acad::eOk==acdbOpenObject(pBlkEnt, blkObjID, AcDb::kForRead))
		{			
			AcDbBlockReference * pRef = (AcDbBlockReference *)pBlkEnt ;
			mysys.SetAttValue(pRef, _T("NAME"), strName) ;

			pBlkEnt->close() ;
		}

		acedSSFree(ssname) ;
	}


}

void CLKDataTable::DrawJYJ()
{
	CUserSystem mysys ;

	mysys.m_pDb = acdbHostApplicationServices()->workingDatabase() ;

	ads_name en ;
	ads_point pt = {0,0,0} ;
	if(RTNORM==acedEntSel(_T("\n选择绝缘节所在直线"), en, pt))
	{
		acutPrintf(_T("\n%.3f-%.3f"), pt[0],pt[1]) ;
		AcDbEntity *pEnt = NULL ;	
		if (mysys.OpenAcDbEntity(en, pEnt))
		{
			if (_tcscmp(pEnt->isA()->name(), _T("AcDbLine"))==0 || _tcscmp(pEnt->isA()->name(), _T("AcDbPolyline"))==0 )
			{
				AcGePoint3d pt_3d_insert(0,0,0) ;
				mysys.ConvertPtAdsTo3D(pt, pt_3d_insert) ;
				AcDbBlockReference *pRef = NULL ;
				CStringArray strArrayAtt ;
				strArrayAtt.Add(_T("")) ;				
				if (mysys.InsertBlock(pRef, _T("JYJ_PT_1"), pt_3d_insert, &strArrayAtt,FALSE, 0))
				{
					pRef->close() ;
				}

			}
			pEnt->close() ;
		}



	}

}

int CLKDataTable::ImportXHJTable(CString strXlsFileName)
{
	int iQJNum = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_xhj_table")) ;
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

		CString strCheckCell[2] = {_T("B3"), _T("L3")} ;
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
			if (strVar!=_T("区间"))
			{
				bCheckOk = FALSE ;
				break ;
			}
		}
		if (bCheckOk)
		{
			CString strColName[2][9] = {
				{_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K")},
				{_T("M"),_T("N"),_T("O"),_T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),_T("U")}
			} ;

			int iCurRow = 5 ;
			BOOL bNext = TRUE ;
			while (bNext)
			{
				BOOL bCurRowIsBlank = TRUE ;
				CString strBarMsg ;
				strBarMsg.Format(_T("读取区间信号机布置表：第%d行"), iCurRow) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, 17) ;	

				for (int i=0; i<2; i++)
				{
					CString strName, strLocShow, strLoc, strSetFreq ;
					long iLocShow = 0 ;
					long iLoc = 0 ;
					int iLen = 0;
					int iDivideNum = 0 ;
					int iTallOrLow = 1 ;
					int iHasAdmit = 0 ;
					long iShowNum = 0 ;
					int iUpOrDown = 0 ;
					int iIsDivide = 0 ;
					int iType = 0 ;

					iUpOrDown = i ;

					BOOL bReadSomething = FALSE ;
					for (int iColIndex=0; iColIndex<9; iColIndex++)
					{
						acedSetStatusBarProgressMeterPos(i*9+iColIndex) ;

						CString strCellName = _T(""), strCellText = _T("") ;
						strCellName.Format(_T("%s%d"), strColName[i][iColIndex], iCurRow) ;
						pRange = NULL ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						strCellText = (CString)(pRange->GetText().bstrVal) ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;

						if (iColIndex==1)
						{
							if (strCellText==_T(""))
							{
								break ;
							}
							else
							{
								bReadSomething = TRUE ;
							}
						}


						switch (iColIndex)
						{
						case 0:
							strName = (strCellText.IsEmpty()?_T("F"):strCellText) ;
							break;
						case 1:
							{
								strLocShow = strCellText ;
							}
							break;
						case 2:
							strLoc = strCellText ;
							break;
						case 3:
							iLen = _tstoi(strCellText) ;
							break;
						case 4:
							iDivideNum = (strCellText==_T("")?0:_tstoi(strCellText)) ;
							break;
						case 5:
							{
								if (strCellText==_T("")||strCellText==_T("1")||strCellText==_T("高柱"))
								{
									iTallOrLow = 1 ;
								}
								else
								{
									iTallOrLow = 0 ;
								}
							}							
							break;
						case 6:
							iHasAdmit= (strCellText==_T("")?0:_tstoi(strCellText)) ;
							break;
						case 7:
							{
								if (strCellText==_T(""))
								{
									iShowNum = 2 ;
								}
								else if (CBlkUtility::IsStrMatch(_T("^[1,2]\\d+-[1,2]$"), strCellText))
								{
									iShowNum = 2 ;
									strSetFreq = strCellText ;
								}
								else if (CBlkUtility::IsStrMatch(_T("^[1-5]$"), strCellText))
								{
									iShowNum = _tstoi(strCellText) ;
								}
							}

							break;
						case 8:
							{
								if (strCellText==_T("车站"))
								{
									iType = 1 ;
									iQJNum++ ;
								}
								else if (strCellText==_T("出站"))
								{
									iType = 2 ;
								}
								else if (strCellText==_T("进站"))
								{
									iType = 3 ;
								}
								else if (strCellText==_T("进站1"))
								{
									iType = 3 ;
									iIsDivide = 1 ;
								}
								else if (strCellText==_T("中继站"))
								{
									iType = 11 ;
									iQJNum++ ;
								}
								else if (strCellText==_T("1"))
								{
									iIsDivide = 1 ;
								}
							}
							break;
						}
					}
					if (bReadSomething)
					{
						//显示里程转换为数值
						if (strLocShow.Find(_T("+"))==-1)
						{
							iLocShow = _tstol(strLocShow) ;
						}
						else if (this->GetNumOfLoc(strLocShow, iLocShow)==FALSE)
						{
							acutPrintf(_T("\n第%d行设置里程%s不符合格式"), iCurRow, strLocShow) ;
						}

						//里程转换为数值
						if (strLoc.Find(_T('+'))==-1)
						{
							if (strLoc.Find(_T('.'))!=-1)
							{
								strLoc.Remove(_T('.')) ;
							}
							iLoc = _tstol(strLoc) ;
						}
						else if (this->GetNumOfLoc(strLoc, iLoc)==FALSE)
						{
							acutPrintf(_T("\n第%d行里程%s不符合格式"), iCurRow, strLoc) ;
						}

						strExec.Format(_T("insert into import_xhj_table(name, loc_show, loc_show_num, loc, loc_num, loc_real, len, divide_num,tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq,type)\
										  values('%s','%s',%d,'%s', %d, %d, %d,%d,%d,%d, %d,%d,%d, '%s', %d)"), strName, strLocShow, iLocShow, strLoc, iLoc, iLoc, iLen, iDivideNum, iTallOrLow, iHasAdmit, iShowNum, iUpOrDown, iIsDivide, strSetFreq, iType) ;
						ExecSqlString(strExec) ;
						bCurRowIsBlank = FALSE ;	
					}
				}

				if (bCurRowIsBlank)
				{
					bNext = FALSE ;
				}
				else
				{
					iCurRow++ ;
				}

				acedRestoreStatusBar() ;
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
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}

	this->UpDateAllRealLoc() ;

	return iQJNum-1 ;
}

//分隔轨道并完善整个xhj表
void CLKDataTable::ComputerFG()
{
	CDBUtility::ClearTable(_T("compute_xhj_table")) ;
	CUserSystem mysys ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from import_xhj_table where type=1 or type=11 order by loc_real asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iSectionSum = cSet.GetRecordCount()-1 ;
			CString strBarMsg ;
			strBarMsg.Format(_T("处理%d个区间分隔点"), iSectionSum) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum) ;	

			int iStationIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iStationIndex) ;

				long iIdStation = 0, iNextIdStation = 0 ;
				long iTypeStation = 0, iNextTypeStation = 0 ;//本车站和下一个车站的类型，车站或者中继站
				long iLocStation = 0, iNextLocStation = 0 ;
				long iRealLocStation = 0, iNextRealLocStation = 0 ;
				CString strStationName = _T("") ;
				CString strNextStationName = _T("") ;

				cSet.GetFieldValue(_T("id"), iIdStation) ;
				cSet.GetFieldValue(_T("type"), iTypeStation) ;
				cSet.GetFieldValue(_T("loc_num"), iLocStation) ;
				cSet.GetFieldValue(_T("loc_real"), iRealLocStation) ;
				cSet.GetFieldValue(_T("name"), strStationName) ;

				cSet.MoveNext() ;
				if (!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("id"), iNextIdStation) ;
					cSet.GetFieldValue(_T("type"), iNextTypeStation) ;
					cSet.GetFieldValue(_T("loc_num"), iNextLocStation) ;
					cSet.GetFieldValue(_T("loc_real"), iNextRealLocStation) ;
					cSet.GetFieldValue(_T("name"), strNextStationName) ;
					//插入车站、下出站、上进站
					//strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) select name, loc_show, loc_num, loc_real,len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type from import_xhj_table where id=%d or (((up_or_down=0 and type=2) or (up_or_down=1 and type=3)) and id>%d and id<%d)"), iIdStation, iIdStation, iNextIdStation) ;
					//ExecSqlString(strSql) ; 

					if (iIdStation==2857)
					{
						int iii = 0 ;
					}

					//begin 如果本车站是中继站
					if (iTypeStation==11 && iStationIndex>0)
					{
						//插入本中继站
						strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real,len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), iIdStation) ;
						ExecSqlString(strSql) ; 

						//找中继站左边的最后一个信号点
						CADORecordset cSetFindLeftLastTG_X ;
						for (int i=0; i<2; i++)
						{				
							strSql.Format(_T("select * from import_xhj_table where loc_real<%d and type=0 and up_or_down=%d and right(name,1)<>'链' order by loc_real asc"), iRealLocStation, i==0?0:1) ;
							if (cSetFindLeftLastTG_X.IsOpen())
							{
								cSetFindLeftLastTG_X.Close() ;
							}
							if (cSetFindLeftLastTG_X.Open(g_PtrCon, strSql))
							{
								if (cSetFindLeftLastTG_X.GetRecordCount()>0)
								{
									cSetFindLeftLastTG_X.MoveLast() ;
									
									long iDivideNumTG_X = 0 ;
									cSetFindLeftLastTG_X.GetFieldValue(_T("divide_num"), iDivideNumTG_X) ;
									long iLocLeftLastTG_X = 0 ;
									cSetFindLeftLastTG_X.GetFieldValue(_T("loc_num"), iLocLeftLastTG_X) ;
									long iRealLocLeftLastTG_X = 0 ;
									cSetFindLeftLastTG_X.GetFieldValue(_T("loc_real"), iRealLocLeftLastTG_X) ;
									long iIdLeftLastTG_X = 0 ;
									cSetFindLeftLastTG_X.GetFieldValue(_T("id"), iIdLeftLastTG_X) ;
									
// 									CString strLocShowLeftLastTG_X = _T("") ;
// 									cSetFindLeftLastTG_X.GetFieldValue(_T("loc_show"), strLocShowLeftLastTG_X) ;
// 									CString strMileSysName = _T("K") ; //里程体系名称，比如DK、K
// 									if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]\\d+\\+\\d{3}$"), strLocShowLeftLastTG_X))
// 									{
// 										CBlkUtility::RegReplace(_T("^(\\w*[k,K])\\d+\\+\\d{3}$"), _T("$1"), strLocShowLeftLastTG_X, strMileSysName) ;
// 									}
									
									//找出中继站右边下行第一个信号点
									long iLocRigthFirstTG_X = 0 ;
									long iRealLocRigthFirstTG_X = 0 ;
									long iIdRightFirstTG_X = 0 ;
									CADORecordset cSetFindRightFirstTG_X ;
									strSql.Format(_T("select * from import_xhj_table where loc_real>%d and type=0 and up_or_down=%d and right(name,1)<>'链' order by loc_real asc"), iRealLocStation, i==0?0:1) ;
									if (cSetFindRightFirstTG_X.IsOpen())
									{
										cSetFindRightFirstTG_X.Close() ;
									}
									if (cSetFindRightFirstTG_X.Open(g_PtrCon, strSql))
									{
										if (cSetFindRightFirstTG_X.GetRecordCount()>0)
										{
											cSetFindRightFirstTG_X.MoveFirst() ;
											cSetFindRightFirstTG_X.GetFieldValue(_T("loc_num"), iLocRigthFirstTG_X) ;
											cSetFindRightFirstTG_X.GetFieldValue(_T("loc_real"), iRealLocRigthFirstTG_X) ;
											cSetFindRightFirstTG_X.GetFieldValue(_T("id"), iIdRightFirstTG_X) ;
										}
										cSetFindRightFirstTG_X.Close() ;
									}									
									
									//begin 查找在iIdLeftLastTG_X和iIdRightFirstTG_X之间有无长短链
									long iIdLongShortChain = 0 ;
									long iLenLongShortChain = 0 ;
									long iShowLocLongShortChain = 0 ;
									long iRealLocLongShortChain = 0 ;
									this->GetLongShortChain(iIdLeftLastTG_X, iIdRightFirstTG_X, iIdLongShortChain, iLenLongShortChain, iShowLocLongShortChain, iRealLocLongShortChain) ;
									//end 查找在iIdLeftLastTG_X和iIdRightFirstTG_X之间有无长短链
									
									long iLenTG_X = iLocRigthFirstTG_X-iLocLeftLastTG_X+iLenLongShortChain ;
									if (iLenTG_X<=m_iLimitToFG && iDivideNumTG_X==0)//不大于1000且不要求分割
									{
										if (iRealLocLongShortChain>iRealLocStation)
										{
											strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i,iIdLongShortChain ) ;
											ExecSqlString(strSql) ;
										}							
									}
									else //要分割的
									{
										long iDivideNumInFact = iLenTG_X/m_iLimitToFG+(iLenTG_X%m_iLimitToFG==0?0:1) ;
										if (iDivideNumTG_X>0)//如果人工指定分隔数，则以人工指定为准
										{
											iDivideNumInFact = iDivideNumTG_X+1 ;
										}									
										
										long iPlusNum = iLenTG_X%iDivideNumInFact ; //余数
										long iCurLoc = iLocLeftLastTG_X ;
										long iRealCurLoc = iRealLocLeftLastTG_X ;
										long iLastDivLen = iLenTG_X/iDivideNumInFact ; 
										BOOL bHaveInsertLongShortChain = FALSE ;
										for (int iDivIndex=0;iDivIndex<(iDivideNumInFact-1);iDivIndex++)
										{
											long iCurDivLen = 0 ;
											iCurDivLen = iLenTG_X/iDivideNumInFact+(iDivIndex>=(iDivideNumInFact-iPlusNum-1)?1:0) ;

											//当前点在中继站左边
											if ((iRealCurLoc+iLastDivLen)<iRealLocStation)
											{												
												iCurLoc = iCurLoc+iLastDivLen ;
												if (iIdLongShortChain>0 && iRealLocLongShortChain<(iRealCurLoc+iLastDivLen))
												{
													iCurLoc = iCurLoc+iLastDivLen-iLenLongShortChain ;
												}
											}
											//中继站在前一点和当前点中间
											else if (iRealLocStation>iRealCurLoc && (iRealCurLoc+iLastDivLen)>iRealLocStation)
											{
												//前点-长短链-中继站-当前点
												if (iRealLocStation>iRealLocLongShortChain && iRealLocLongShortChain>iRealCurLoc)
												{
													iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
												}
												//前点-中继站-长短链-当前点
												else if (iRealLocLongShortChain>iRealLocStation && iRealLocLongShortChain<(iRealCurLoc+iLastDivLen))
												{
													iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain ) ;
													ExecSqlString(strSql) ;
												}
												else
												{
													iCurLoc= iCurLoc+iLastDivLen ;
												}

												CString strFName = _T("") ;
												this->GetFName(iCurLoc, strFName,i==0?0:1) ;
												CString strFLocShow = _T("") ;
												this->GetLocShow(iCurLoc, m_strMileSysName, strFLocShow) ;
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iRealCurLoc+iLastDivLen, iCurDivLen, i==0?0:1) ;
												ExecSqlString(strSql) ;
											}
											//中继站-前点.....
											else if (iRealCurLoc>iRealLocStation)
											{
												//长短链在前点和当前点之间
												if (iRealLocLongShortChain>iRealCurLoc && iRealLocLongShortChain<(iRealCurLoc+iLastDivLen))
												{
													iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain ) ;
													ExecSqlString(strSql) ;
												}
												else
												{
													iCurLoc= iCurLoc+iLastDivLen ;
												}
												CString strFName = _T("") ;
												this->GetFName(iCurLoc, strFName,i==0?0:1) ;
												CString strFLocShow = _T("") ;
												this->GetLocShow(iCurLoc, m_strMileSysName, strFLocShow) ;
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real,len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iRealCurLoc+iLastDivLen, iCurDivLen, i==0?0:1) ;
												ExecSqlString(strSql) ;
											}
											if (iDivIndex==(iDivideNumInFact-2))
											{
												if (iRealLocLongShortChain>(iRealCurLoc+iLastDivLen) && iRealLocLongShortChain>iRealLocStation)
												{
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
													ExecSqlString(strSql) ;
												}
											}

											iRealCurLoc = iRealCurLoc+iLastDivLen ;
											iLastDivLen = iCurDivLen ;
										}								
									}
								}	
								cSetFindLeftLastTG_X.Close() ;
							}
						}
						
					}
					//end 如果本车站是中继站

					CADORecordset cSetFind ;

					//begin 处理通过
					//////////////////////////////////////////////////////////////////////////
					
					//begin 如果有上进站，则对下行有X1LQG，要先看X1LQG要不要分隔
					//查找有无SJZ
					strSql.Format(_T("select * from import_xhj_table where loc_real>%d and loc_real<%d and up_or_down=1 and type=3 order by loc_real asc"), iRealLocStation, iNextRealLocStation) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							cSetFind.MoveFirst() ;
							long iIdSJZ = 0 ;
							long iLoc_SJZ = 0 ;
							long iRealLocSJZ = 0 ;
							cSetFind.GetFieldValue(_T("loc_num"), iLoc_SJZ) ;
							cSetFind.GetFieldValue(_T("loc_real"), iRealLocSJZ) ;
							cSetFind.GetFieldValue(_T("id"), iIdSJZ) ;

							int iiii = 0 ;

							//插入本车站到SJZ之间所有点
							strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real,len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where loc_real>=%d and loc_real<=%d"), iRealLocStation, iRealLocSJZ) ;
							ExecSqlString(strSql) ; 

							for (int i=0; i<2; i++)
							{						
								CADORecordset cSetFindFirstTG_X ;
								strSql.Format(_T("select * from import_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and right(name,1)<>'链' order by loc_real asc"), iRealLocSJZ, iNextRealLocStation, i==0?0:1) ;
								if (cSetFindFirstTG_X.IsOpen())
								{
									cSetFindFirstTG_X.Close() ;
								}
								if (cSetFindFirstTG_X.Open(g_PtrCon, strSql))
								{
									if (cSetFindFirstTG_X.GetRecordCount()>0)
									{
										cSetFindFirstTG_X.MoveFirst() ;
										
										long iLocFirtTG_X = 0 ;
										long iRealLocFirtTG_X = 0 ;
										cSetFindFirstTG_X.GetFieldValue(_T("loc_num"), iLocFirtTG_X) ;
										cSetFindFirstTG_X.GetFieldValue(_T("loc_real"), iRealLocFirtTG_X) ;
										long iIdFirtTG_X = 0 ;
										cSetFindFirstTG_X.GetFieldValue(_T("id"), iIdFirtTG_X) ;
										CString strLocShowFirstTG_X = _T("") ;
// 										cSetFindFirstTG_X.GetFieldValue(_T("loc_show"), strLocShowFirstTG_X) ;
// 										CString strMileSysName = _T("K") ; //里程体系名称，比如DK、K
// 										if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]\\d+\\+\\d{3}$"), strLocShowFirstTG_X))
// 										{
// 											CBlkUtility::RegReplace(_T("^(\\w*[k,K])\\d+\\+\\d{3}$"), _T("$1"), strLocShowFirstTG_X, strMileSysName) ;
// 										}
										
										//begin 查找在SJZ和iLocFirtTG_X之间有无长短链
										long iIdLongShortChain = 0 ;
										long iLenLongShortChain = 0 ;
										long iShowLocLongShortChain = 0 ;
										long iRealLocLongShortChain = 0 ;
										this->GetLongShortChain(iIdSJZ, iIdFirtTG_X, iIdLongShortChain, iLenLongShortChain, iShowLocLongShortChain, iRealLocLongShortChain) ;
										//end 查找在SJZ和iLocFirtTG_X之间有无长短链
										
										long iLenX1LQ = iLocFirtTG_X - iLoc_SJZ +iLenLongShortChain ;
										if (iLenX1LQ>m_iLimitToFG)
										{
											long iDivideNum = iLenX1LQ/m_iLimitToFG+(iLenX1LQ%m_iLimitToFG==0?0:1) ;
											long iPlusNum = iLenX1LQ%iDivideNum ; //余数
											long iCurLoc = iLoc_SJZ ;
											long iRealCurLoc = iRealLocSJZ ;
											long iLastDivLen = iLenX1LQ/iDivideNum ;

											if (i==0)
											{
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, is_reverse, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, 1, ba_offset from import_xhj_table where id=%d"), _T("SF"),iLastDivLen, i==0?0:1,iIdSJZ) ;
												ExecSqlString(strSql) ;
											}
											else if (i==1)
											{
												strSql.Format(_T("update compute_xhj_table set len=%d where loc_num=%d and up_or_down=1 and type=3"), iLastDivLen, iLoc_SJZ) ;
												ExecSqlString(strSql) ;
											}

											BOOL bHaveInsertLongShortChain = FALSE ;
											for (int iDivIndex=0;iDivIndex<(iDivideNum-1);iDivIndex++)
											{
												long iCurDivLen = 0 ;
												iCurDivLen = iLenX1LQ/iDivideNum+(iDivIndex>=(iDivideNum-iPlusNum-1)?1:0) ;
												//若长短链在SJZ和FirstTG_X之间，则要将该长短链插入db
												if (iShowLocLongShortChain>iCurLoc && iShowLocLongShortChain<(iCurLoc+iLastDivLen))
												{
													iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain) ;
													ExecSqlString(strSql) ;
													bHaveInsertLongShortChain = TRUE ;
												}
												else
												{
													iCurLoc= iCurLoc+iLastDivLen ;
												}

												if (iDivIndex==(iDivideNum-2))
												{
													if (iShowLocLongShortChain>iCurLoc && bHaveInsertLongShortChain==FALSE)
													{
														strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real,len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
														ExecSqlString(strSql) ;
													}
												}

												CString strFName = _T("") ;
												this->GetFName(iCurLoc, strFName,i==0?0:1) ;
												CString strFLocShow = _T("") ;
												this->GetLocShow(iCurLoc, m_strMileSysName, strFLocShow) ;
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real,len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iRealCurLoc+iLastDivLen, iCurDivLen,i==0?0:1) ;
												ExecSqlString(strSql) ;	

												iRealCurLoc = iRealCurLoc+iLastDivLen ;
												iLastDivLen = iCurDivLen ;
											}
										}
										else 
										{
											if (i==0)
											{
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, is_reverse, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, %d, is_divide, '', type, 1, ba_offset from import_xhj_table where id=%d"), _T("SF"),iLenX1LQ, i==0?0:1,iIdSJZ) ;
												ExecSqlString(strSql) ;
											}
											else if (i==1)
											{
												strSql.Format(_T("update compute_xhj_table set len=%d where loc_num=%d and up_or_down=1 and type=3"), iLenX1LQ, iLoc_SJZ) ;
												ExecSqlString(strSql) ;
											}

											if (iIdLongShortChain>0)
											{											
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain) ;
												ExecSqlString(strSql) ;
											}

										}
										
									}
									cSetFindFirstTG_X.Close() ;
								}
							}
						}

						cSetFind.Close() ;
					}
					//end 如果有上进站，则对下行有X1LQG，要先看X1LQG要不要分隔

					if(strStationName==_T("扬州线路所"))
					{
						int iii = 0 ;
					}

					//遍历所有通过，看区段长度是否大于1000，进行自动分隔
					for (int i=0; i<2; i++)
					{						
						strSql.Format(_T("select * from import_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and type=0 and right(name,1)<>'链' order by loc_real asc"), iRealLocStation, iNextRealLocStation, i==0?0:1) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							int iTgCount = cSetFind.GetRecordCount() ; //通过信号点的数量

							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}
							while (!cSetFind.IsEOF())
							{
								long iIdTG_X = 0, iNextIdTG_X = 0 ;
								long iLocTG_X = 0, iNextLocTG_X =0 ;
								long iRealLocTG_X = 0, iRealNextLocTG_X =0 ;
								long iLenTG_X = 0 ;
								long iDivideNumTG_X = 0 ;
								CString strNameTG_X = _T("") ;
								CString strSetFreqTG_X = _T("") ;
								
								cSetFind.GetFieldValue(_T("id"), iIdTG_X) ;
								cSetFind.GetFieldValue(_T("loc_num"), iLocTG_X) ;
								cSetFind.GetFieldValue(_T("loc_real"), iRealLocTG_X) ;
								cSetFind.GetFieldValue(_T("divide_num"), iDivideNumTG_X) ;
								cSetFind.GetFieldValue(_T("name"), strNameTG_X) ;
								cSetFind.GetFieldValue(_T("set_freq"), strSetFreqTG_X) ;
								
								if(strNameTG_X==_T("6225"))
								{
									int iii = 0 ;
								}
								

								/*
								if (strNameTG_X == _T("F"))
								{
									this->GetFName(iLocTG_X, strNameTG_X,i==0?0:1) ;
									//strNameTG_X.Format(_T("F000%d%d"), iLocTG_X/1000, (iLocTG_X%1000)/100%2==0?(iLocTG_X%1000)/100+(iLocTG_X%1000%100/10>4?1:-1):(iLocTG_X%1000)/100) ;
								}
								else if (strNameTG_X.Left(1)==_T("F"))
								{
									if (m_bAutoComputeXhjName)
									{
										this->GetFName(iLocTG_X, strNameTG_X,i==0?0:1) ;
									}
								}
								else//将信号机57转换为00057
								{
									CString strNameTmp = strNameTG_X  ;
									this->GetXHJName(strNameTmp, strNameTG_X) ;
								}
								*/
								
// 								CString strLocShowTG_X = _T("") ;
// 								cSetFind.GetFieldValue(_T("loc_show"), strLocShowTG_X) ;
// 								CString strMileSysName = _T("K") ; //里程体系名称，比如DK、K
// 								if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]\\d+\\+\\d{3}$"), strLocShowTG_X))
// 								{
// 									CBlkUtility::RegReplace(_T("^(\\w*[k,K])\\d+\\+\\d{3}$"), _T("$1"), strLocShowTG_X, strMileSysName) ;
// 								}
								
								//指针移动下架信号点
								cSetFind.MoveNext() ;
								if (!cSetFind.IsEOF())
								{
									cSetFind.GetFieldValue(_T("id"), iNextIdTG_X) ;
									cSetFind.GetFieldValue(_T("loc_num"), iNextLocTG_X) ;
									cSetFind.GetFieldValue(_T("loc_real"), iRealNextLocTG_X) ;
									if (iIdTG_X==99)
									{
										int iii = 0 ;
									}
									
									//begin 查找当前通过信号机和下一架之间有无长短链
									long iIdLongShortChain = 0 ;
									long iLenLongShortChain = 0 ;
									long iShowLocLongShortChain = 0 ;
									long iRealLocLongShortChain = 0 ;
									this->GetLongShortChain(iIdTG_X, iNextIdTG_X, iIdLongShortChain, iLenLongShortChain, iShowLocLongShortChain, iRealLocLongShortChain) ;
									//end 查找当前通过信号机和下一架之间有无长短链
									
									iLenTG_X = iNextLocTG_X - iLocTG_X + iLenLongShortChain ;								
									if (iLenTG_X<=m_iLimitToFG && iDivideNumTG_X==0)//不大于1000且不要求分割
									{
										strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X, iIdTG_X) ;
										ExecSqlString(strSql) ;	
										if (iIdLongShortChain>0)
										{
											strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i,iIdLongShortChain ) ;
											ExecSqlString(strSql) ;
										}	
									}
									else //要分割的
									{
										long iDivideNumInFact = iLenTG_X/m_iLimitToFG+(iLenTG_X%m_iLimitToFG==0?0:1) ;
										if (iDivideNumTG_X>0)//如果人工指定分隔数，则以人工指定为准
										{
											iDivideNumInFact = iDivideNumTG_X+1 ;
										}
										
										//插入起始信号点
										strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X/iDivideNumInFact, iIdTG_X) ;
										ExecSqlString(strSql) ;
										
										long iPlusNum = iLenTG_X%iDivideNumInFact ; //余数
										long iCurLoc = iLocTG_X ;
										long iRealCurLoc = iRealLocTG_X ;
										long iLastDivLen = iLenTG_X/iDivideNumInFact ; 
										BOOL bHaveInsertLongShortChain = FALSE ;
										for (int iDivIndex=0;iDivIndex<(iDivideNumInFact-1);iDivIndex++)
										{
											long iCurDivLen = 0 ;
											iCurDivLen = iLenTG_X/iDivideNumInFact+(iDivIndex>=(iDivideNumInFact-iPlusNum-1)?1:0) ;
											
											if (iShowLocLongShortChain>iCurLoc && iShowLocLongShortChain<=(iCurLoc+iLastDivLen))
											{
												iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;//
												strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
												ExecSqlString(strSql) ;
												bHaveInsertLongShortChain = TRUE ;
											}
											else
											{
												iCurLoc= iCurLoc+iLastDivLen ;
											}

											if (iDivIndex==(iDivideNumInFact-2))
											{
												if (iShowLocLongShortChain>iCurLoc && bHaveInsertLongShortChain==FALSE)
												{
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
													ExecSqlString(strSql) ;
												}
											}
											
											CString strFName = _T("") ;
											this->GetFName(iCurLoc, strFName,i==0?0:1) ;
											CString strFLocShow = _T("") ;
											this->GetLocShow(iCurLoc, m_strMileSysName, strFLocShow) ;
											strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iRealCurLoc+iLastDivLen, iCurDivLen, i==0?0:1) ;
											ExecSqlString(strSql) ;	
											
											iRealCurLoc = iRealCurLoc+iLastDivLen ;
											iLastDivLen = iCurDivLen ;
										}								
									}
									
									
								}
								else//最后一个下行通过
								{
									//1.右侧站是中继站
									if (iNextTypeStation==11)
									{
										//找中继站右边的第一个信号点
										CADORecordset cSetFindRightFirtTG_X ;
										strSql.Format(_T("select * from import_xhj_table where loc_real>%d and type=0 and up_or_down=%d and right(name,1)<>'链' order by loc_real asc"), iNextRealLocStation, i==0?0:1) ;
										if (cSetFindRightFirtTG_X.IsOpen())
										{
											cSetFindRightFirtTG_X.Close() ;
										}
										if (cSetFindRightFirtTG_X.Open(g_PtrCon, strSql))
										{
											if (cSetFindRightFirtTG_X.GetRecordCount()>0)
											{
												cSetFindRightFirtTG_X.MoveFirst() ;
												
												long iLocRigthFirstTG_X = 0 ;
												long iRealLocRigthFirstTG_X = 0 ;
												cSetFindRightFirtTG_X.GetFieldValue(_T("loc_num"), iLocRigthFirstTG_X) ;
												cSetFindRightFirtTG_X.GetFieldValue(_T("loc_real"), iRealLocRigthFirstTG_X) ;
												long iIdRightFirstTG_X = 0 ;
												cSetFindRightFirtTG_X.GetFieldValue(_T("id"), iIdRightFirstTG_X) ;
												
												//begin 查找在iIdTG_X和iIdRightFirstTG_X之间有无长短链
												long iIdLongShortChain = 0 ;
												long iLenLongShortChain = 0 ;
												long iShowLocLongShortChain = 0 ;
												long iRealLocLongShortChain = 0 ;
												this->GetLongShortChain(iIdTG_X, iIdRightFirstTG_X, iIdLongShortChain, iLenLongShortChain, iShowLocLongShortChain, iRealLocLongShortChain) ;
												//end 查找在iIdTG_X和iIdRightFirstTG_X之间有无长短链
												
												iLenTG_X = iLocRigthFirstTG_X-iLocTG_X+iLenLongShortChain ;
												if (iLenTG_X<=m_iLimitToFG && iDivideNumTG_X==0)//不大于1000且不要求分割
												{
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X, iIdTG_X) ;
													ExecSqlString(strSql) ;
													if (iIdLongShortChain>0 && iRealLocLongShortChain<iNextRealLocStation)
													{
														strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i,iIdLongShortChain ) ;
														ExecSqlString(strSql) ;
													}
												}
												else //要分割的
												{
													long iDivideNumInFact = iLenTG_X/m_iLimitToFG+(iLenTG_X%m_iLimitToFG==0?0:1) ;
													if (iDivideNumTG_X>0)//如果人工指定分隔数，则以人工指定为准
													{
														iDivideNumInFact = iDivideNumTG_X+1 ;
													}
													
													//插入起始信号点
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X/iDivideNumInFact, iIdTG_X) ;
													ExecSqlString(strSql) ;
													
													long iPlusNum = iLenTG_X%iDivideNumInFact ; //余数
													long iCurLoc = iLocTG_X ;
													long iRealCurLoc = iRealLocTG_X ;
													long iLastDivLen = iLenTG_X/iDivideNumInFact ;	
													BOOL bHaveInsertLongShortChain = FALSE ;
													for (int iDivIndex=0;iDivIndex<(iDivideNumInFact-1);iDivIndex++)
													{
														long iCurDivLen = 0 ;
														iCurDivLen = iLenTG_X/iDivideNumInFact+(iDivIndex>=(iDivideNumInFact-iPlusNum-1)?1:0) ;
														
														//当前点在中继站左边
														if ((iRealCurLoc+iLastDivLen)<iNextRealLocStation)
														{
															iCurLoc = iCurLoc+iLastDivLen ;
															if (iIdLongShortChain>0 && iRealLocLongShortChain<(iRealCurLoc+iLastDivLen))
															{
																iCurLoc = iCurLoc+iLastDivLen-iLenLongShortChain ;
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain ) ;
																ExecSqlString(strSql) ;
															}

															CString strFName = _T("") ;
															this->GetFName(iCurLoc, strFName,i==0?0:1) ;
															CString strFLocShow = _T("") ;
															this->GetLocShow(iCurLoc, m_strMileSysName, strFLocShow) ;
															strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iRealCurLoc+iLastDivLen, iCurDivLen, i==0?0:1) ;
															ExecSqlString(strSql) ;
														}
														//中继站在前一点和当前点中间
														else if (iRealLocStation>iRealCurLoc && (iRealCurLoc+iLastDivLen)>iRealLocStation)
														{
															//前点-长短链-中继站-当前点
															if (iRealLocStation>iRealLocLongShortChain && iRealLocLongShortChain>iRealCurLoc)
															{
																iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain ) ;
																ExecSqlString(strSql) ;
															}
															//前点-中继站-长短链-当前点
															else if (iRealLocLongShortChain>iRealLocStation && iRealLocLongShortChain<(iRealCurLoc+iLastDivLen))
															{
																iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
															}
															else
															{
																iCurLoc= iCurLoc+iLastDivLen ;
															}													
														}
														//中继站-前点.....
														else if (iRealCurLoc>iRealLocStation)
														{
															//长短链在前点和当前点之间
															if (iRealLocLongShortChain>iRealCurLoc && iRealLocLongShortChain<(iRealCurLoc+iLastDivLen))
															{
																iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
															}
															else
															{
																iCurLoc= iCurLoc+iLastDivLen ;
															}
														}
														if (iDivIndex==(iDivideNumInFact-2))
														{
															if (iRealLocLongShortChain>(iRealCurLoc+iLastDivLen)&&iNextRealLocStation>iRealLocLongShortChain)
															{
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
																ExecSqlString(strSql) ;
															}
														}


														/*
														if (iShowLocLongShortChain>=iCurLoc&& iShowLocLongShortChain<=(iCurLoc+iLastDivLen))//长短链在前一个信号点和当前信号点之间
														{
															if (iShowLocLongShortChain>=iNextLocStation)//中继站在前一个信号点和长短链之间(顺序：前信号点-中继站-长短链-当前信号点)
															{
																break ;
																//不要插入任何thing
																//iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;																
															}
															else if (iShowLocLongShortChain<(iNextLocStation+iLenLongShortChain) && (iNextLocStation+iLenLongShortChain)<(iCurLoc+iLastDivLen))//前信号点-长短链-中继站-当前信号点
															{
																//要插入长短链
																iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) select name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain) ;
																ExecSqlString(strSql) ;	
																bHaveInsertLongShortChain = TRUE ;
																
															}
															else if (iNextLocStation>=(iCurLoc+iLastDivLen+iLenLongShortChain))
															{
																iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) select name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain) ;
																ExecSqlString(strSql) ;
																bHaveInsertLongShortChain = TRUE ;
																																
																CString strFName = _T("") ;
																this->GetFName(iCurLoc, strFName,i==0?0:1) ;
																CString strFLocShow = _T("") ;
																this->GetLocShow(iCurLoc, strMileSysName, strFLocShow) ;
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iCurDivLen, i==0?0:1) ;
																ExecSqlString(strSql) ;	
															}
														}
														else if (iNextLocStation>(iCurLoc+iLastDivLen))
														{
															iCurLoc= iCurLoc+iLastDivLen ;
															CString strFName = _T("") ;
															this->GetFName(iCurLoc, strFName,i==0?0:1) ;
															CString strFLocShow = _T("") ;
															this->GetLocShow(iCurLoc, strMileSysName, strFLocShow) ;
															strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, 1, 0, 2, %d, 0, '',0 )"), strFName, strFLocShow, iCurLoc, iCurDivLen, i==0?0:1) ;
															ExecSqlString(strSql) ;															
														}
														else
														{
															iCurLoc= iCurLoc+iLastDivLen ;
														}
														
														
														if (iDivIndex==(iDivideNumInFact-2))
														{
															if (iShowLocLongShortChain>iCurLoc && iNextIdStation>iIdLongShortChain && bHaveInsertLongShortChain==FALSE)
															{
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) select name, loc_show, loc_num, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
																ExecSqlString(strSql) ;
															}
														}	
														*/

														iRealCurLoc = iRealCurLoc+iLastDivLen ;
														iLastDivLen = iCurDivLen ;
													}								
												}
											}
											cSetFindRightFirtTG_X.Close() ;
										}
									}
									//2.右侧站是正常站
									else if (iNextTypeStation==1)
									{
										//找XJZ
										CADORecordset cSetFindXJZ ;
										strSql.Format(_T("select * from import_xhj_table where loc_real>%d and loc_real<%d and type=3 and up_or_down=0 order by loc_real asc"), iRealLocStation, iNextRealLocStation) ;
										if (cSetFindXJZ.IsOpen())
										{
											cSetFindXJZ.Close() ;
										}
										if (cSetFindXJZ.Open(g_PtrCon, strSql))
										{
											if (cSetFindXJZ.GetRecordCount()>0)
											{
												cSetFindXJZ.MoveFirst() ;
												
												long iLocXJZ = 0 ;
												cSetFindXJZ.GetFieldValue(_T("loc_num"), iLocXJZ) ;
												long iRealLocXJZ = 0 ;
												cSetFindXJZ.GetFieldValue(_T("loc_real"), iRealLocXJZ) ;
												long iIdXJZ = 0 ;
												cSetFindXJZ.GetFieldValue(_T("id"), iIdXJZ) ;
												CString strFreqXJZ = _T("") ;
												cSetFindXJZ.GetFieldValue(_T("set_freq"), strFreqXJZ) ;
												
												//begin 查找在iIdTG_X和iIdXJZ之间有无长短链
												long iIdLongShortChain = 0 ;
												long iLenLongShortChain = 0 ;
												long iShowLocLongShortChain = 0 ;
												long iRealLocLongShortChain = 0 ;
												this->GetLongShortChain(iIdTG_X, iIdXJZ, iIdLongShortChain, iLenLongShortChain, iShowLocLongShortChain,iRealLocLongShortChain) ;
												//end 查找在iIdTG_X和iIdXJZ之间有无长短链
												
												iLenTG_X = iLocXJZ-iLocTG_X+iLenLongShortChain ;
												if (iLenTG_X<=m_iLimitToFG && iDivideNumTG_X==0)//不大于1000且不要求分割
												{
													if (strSetFreqTG_X==_T("") && i==0)
													{
														strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, '%s', type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X, strFreqXJZ, iIdTG_X ) ;
													}
													else
													{
														strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X, iIdTG_X) ;
													}
													ExecSqlString(strSql) ;	
													if (iIdLongShortChain>0 && iRealLocLongShortChain<iRealLocXJZ)
													{
														strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i,iIdLongShortChain ) ;
														ExecSqlString(strSql) ;
													}
												}
												else //要分割的
												{
													long iDivideNumInFact = iLenTG_X/m_iLimitToFG+(iLenTG_X%m_iLimitToFG==0?0:1) ;
													if (iDivideNumTG_X>0)//如果人工指定分隔数，则以人工指定为准
													{
														iDivideNumInFact = iDivideNumTG_X+1 ;
													}
													
													//插入起始信号点
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select '%s', loc_show, loc_num, loc_real, %d, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), strNameTG_X, iLenTG_X/iDivideNumInFact, iIdTG_X) ;
													ExecSqlString(strSql) ;
													
													long iPlusNum = iLenTG_X%iDivideNumInFact ; //余数
													long iCurLoc = iLocTG_X ;
													long iRealCurLoc = iRealLocTG_X ;
													long iLastDivLen = iLenTG_X/iDivideNumInFact ; 
													BOOL bHaveInsertLongShortChain = FALSE ;
													for (int iDivIndex=0;iDivIndex<(iDivideNumInFact-1);iDivIndex++)
													{
														long iCurDivLen = 0 ;
														iCurDivLen = iLenTG_X/iDivideNumInFact+(iDivIndex>=(iDivideNumInFact-iPlusNum-1)?1:0) ;
														
														if (iShowLocLongShortChain>iCurLoc && iShowLocLongShortChain<=(iCurLoc+iLastDivLen))
														{
															iCurLoc= iCurLoc+iLastDivLen-iLenLongShortChain ;
															strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i==0?0:1,iIdLongShortChain) ;
															ExecSqlString(strSql) ;
															bHaveInsertLongShortChain = TRUE ;
														}
														else
														{
															iCurLoc= iCurLoc+iLastDivLen ;
														}

														if (iDivIndex==(iDivideNumInFact-2))
														{
															if (iShowLocLongShortChain>iCurLoc && bHaveInsertLongShortChain==FALSE)
															{
																strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), i, iIdLongShortChain) ;
																ExecSqlString(strSql) ;
															}
														}

														CString strFName = _T("") ;
														this->GetFName(iCurLoc, strFName,i==0?0:1) ;
														CString strFLocShow = _T("") ;
														this->GetLocShow(iCurLoc, m_strMileSysName, strFLocShow) ;
														strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type) values('%s','%s',%d, %d, %d, 1, 0, 2, %d, 0, '%s',0 )"), strFName, strFLocShow, iCurLoc, iRealCurLoc+iLastDivLen, iCurDivLen, i==0?0:1, i==0?strFreqXJZ:_T("")) ;
														ExecSqlString(strSql) ;	
														
														iRealCurLoc = iRealCurLoc+iLastDivLen ;														
														iLastDivLen = iCurDivLen ;
													}								
												}

												if (i==1)
												{
													strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, is_reverse, ba_offset) select '%s', loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, '', type, 1, ba_offset from import_xhj_table where id=%d"), _T("XF"), i==0?0:1,iIdXJZ) ;
													ExecSqlString(strSql) ;
												}

											}
											
											
											cSetFindXJZ.Close() ;
										}
										
									}
									//end 2.右侧站是正常站
								}
							}	
							
							if (iTgCount==0 && i==1 && iNextTypeStation==1) //没有通过信号机，上面的插入XN语句不会执行，故在此添加
							{
								strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, is_reverse, ba_offset) select '%s', loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, %d, is_divide, '', type, 1, ba_offset from import_xhj_table where id=(select id from import_xhj_table where loc_real>%d and loc_real<%d and type=3 and up_or_down=0 order by loc_real asc)"), _T("XF"), i==0?0:1,iRealLocStation, iNextRealLocStation) ;
								ExecSqlString(strSql) ;
							}
						
							cSetFind.Close() ;
						}
					}

					//////////////////////////////////////////////////////////////////////////
					//end 处理通过
					//插入下进站、上出站
					strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide,  type, ba_offset from import_xhj_table where (((up_or_down=1 and type=2) or (up_or_down=0 and type=3)) and loc_real>%d and loc_real<%d)"), iRealLocStation, iNextRealLocStation) ;
					ExecSqlString(strSql) ; 

				}
				else //next为eof，到最后一条记录
				{
					strSql.Format(_T("INSERT INTO compute_xhj_table(name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset) select name, loc_show, loc_num, loc_real, len, tall_or_low, has_admit, show_num, up_or_down, is_divide, set_freq, type, ba_offset from import_xhj_table where id=%d"), iIdStation) ;
					ExecSqlString(strSql) ; 
					break ;
				}

				//cSet.MoveNext() ;
				iStationIndex++ ;

			}
			cSet.Close() ;

			acedRestoreStatusBar() ;

		}

		//begin 自动计算信号机名称
		if (m_bAutoComputeXhjName)
		{
			strSql.Format(_T("select * from compute_xhj_table where type=0 and right(name,1)<>'链' order by loc_real asc")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				while (!cSet.IsEOF())
				{
					long iLocNum = 0 ;
					int iUpOrDown = 0 ;
					CString strName = _T("") ;
					cSet.GetFieldValue(_T("name"), strName) ;
					cSet.GetFieldValue(_T("loc_num"), iLocNum) ;
					cSet.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
					
					if (strName.Left(1)!=_T("F"))
					{
						this->GetFName(iLocNum, strName, iUpOrDown) ;
						strName.TrimLeft(_T("F")) ;	
						cSet.Edit() ;
						cSet.SetFieldValue(_T("name"), strName) ;
						cSet.Update() ;
					}
					cSet.MoveNext() ;
				}
				
				cSet.Close() ;
			}

		}
		//end 自动计算信号机名称


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputerFG")) ;

	}


}

//modified in 20110802:原来是类似于200,050的百米部分小于100的减去1，现统一都加1
BOOL CLKDataTable::GetFName(long iLoc, CString &strFName, int iDownOrUp)
{
	int iKNum = 0 ;//千米部分
	int iMNum = 0 ;//百米部分
	iKNum = iLoc/1000 ;
	if (iDownOrUp==0)
	{
		if (iLoc%1000/100%2==1)
		{
			iMNum = iLoc%1000/100 ;		
		}
		else
		{
// 			if(iLoc%1000<100)
// 			{
// 				iMNum = 9 ;
// 				iKNum-- ;
// 			}
// 			else
// 			{
				iMNum = iLoc%1000/100+1 ;
// 			}
		}

	}
	else
	{
		if (iLoc%1000/100%2==0)//百位是偶数
		{
			iMNum = iLoc%1000/100 ;
		}
		else //百位是奇数
		{
// 			if (iLoc%1000>=900)
// 			{
// 				iMNum = 0 ;
// 				iKNum++ ;
// 			}
// 			else
// 			{
				iMNum = iLoc%1000/100 + 1 ; 
				//begin modify in 2012-02-24 谢兰英发现k842+983 转成84210 后修改程序
				if (iMNum==10)
				{
					iKNum++ ;
					iMNum=0 ;
				}
				//end modify in 2012-02-24 谢兰英发现k842+983 转成84210 后修改程序
// 			}
		}
	}

	CString strNameTmp1 , strNameTmp2 ;
	strNameTmp1.Format(_T("%d%d"), iKNum, iMNum) ;
	this->GetXHJName(strNameTmp1, strNameTmp2) ;
	
	strFName.Format(_T("F%s"), strNameTmp2) ;

	return TRUE ;
}

BOOL CLKDataTable::GetLocShow(long iLoc, const CString strMileSys, CString &strLocShow)
{
	strLocShow.Format(_T("%s%d+%d%d%d"), strMileSys, iLoc/1000, iLoc%1000/100, iLoc%1000%100/10, iLoc%1000%100%10 ) ;
	return TRUE ;
}


//暂时只考虑2个信号点之间只有一个长短链的可能
int CLKDataTable::GetLongShortChain(long iIdStart, long iIdEnd, long &iIdLongShortChain, long &iLenLongShortChain, long &iShowLocLongShortChain, long &iRealLocLongShortChain, int iReadTableIndex)
{
	CString strTableName[2] = {_T("import_xhj_table"), _T("compute_xhj_table")} ;
	int iRet = 0 ;
	//begin 查找在iIdStart和iIdEnd之间有无长短链
	CUserSystem mysys ;
	CString strSql ;
	CADORecordset cSetFindLongShortChain ;
	strSql.Format(_T("select * from %s where id>%d and id<%d and up_or_down=0 and right(name,1)='链'"), strTableName[iReadTableIndex],iIdStart, iIdEnd) ;
	if (cSetFindLongShortChain.IsOpen())
	{
		cSetFindLongShortChain.Close() ;
	}
	if (cSetFindLongShortChain.Open(g_PtrCon, strSql))
	{
		iRet = cSetFindLongShortChain.GetRecordCount() ;
		if (iRet>0)
		{
			cSetFindLongShortChain.MoveFirst() ;
			while (!cSetFindLongShortChain.IsEOF())
			{
				long iLenLSChain = 0 ;
				CString strNameLSChainTmp = _T("") ;
				CString strLocShowLSChainTmp = _T("") ;
				cSetFindLongShortChain.GetFieldValue(_T("id"), iIdLongShortChain) ;
				cSetFindLongShortChain.GetFieldValue(_T("len"), iLenLSChain) ;
				cSetFindLongShortChain.GetFieldValue(_T("name"), strNameLSChainTmp) ;
				cSetFindLongShortChain.GetFieldValue(_T("loc_show"), strLocShowLSChainTmp) ;
				cSetFindLongShortChain.GetFieldValue(_T("loc_real"), iRealLocLongShortChain) ;
				if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]\\d+\\+\\d{3}$"), strLocShowLSChainTmp))
				{
					CString strPart1 = _T("") , strPart2 = _T("") ;
					CBlkUtility::RegReplace(_T("^\\w*[k,K](\\d+)\\+(\\d{3})$"), _T("$1"), strLocShowLSChainTmp, strPart1) ;
					CBlkUtility::RegReplace(_T("^\\w*[k,K](\\d+)\\+(\\d{3})$"), _T("$2"), strLocShowLSChainTmp, strPart2) ;
					iShowLocLongShortChain = _tstoi(strPart1)*1000+_tstoi(strPart2) ;
				}
				if (strNameLSChainTmp==_T("长链"))
				{
					iLenLongShortChain+=iLenLSChain ;
				}
				else if (strNameLSChainTmp==_T("短链"))
				{
					iLenLongShortChain-=iLenLSChain ;
				}
				
				cSetFindLongShortChain.MoveNext() ;
			}
		}
		cSetFindLongShortChain.Close() ;
	}
	//end 查找在iIdStart和iIdEnd之间有无长短链

// 	//begin 查找在iIdStart和iIdEnd之间有无长短链
// 	CUserSystem mysys ;
// 	CString strSql ;
// 	CADORecordset cSetFind ;
// 
// 	查询start 和 end 2个信号点的loc
// 		long iStartEndLoc[2] = {0,0} ;
// 		for (int i=0; i<2; i++)
// 		{
// 			strSql.Format(_T("select * from import_xhj_table where id=%d"),i==0?iIdStart:iIdEnd) ;
// 			if (cSetFind.IsOpen())
// 			{
// 				cSetFind.Close() ;
// 			}
// 			if (cSetFind.Open(g_PtrCon, strSql))
// 			{
// 				if (cSetFind.GetRecordCount()>0)
// 				{
// 					cSetFind.MoveFirst() ;
// 					cSetFind.GetFieldValue(_T("loc_num"),iStartEndLoc[i]) ;
// 				}
// 				cSetFind.Close() ;
// 			}
// 		}
// 		//遍历所有长短链
// 		strSql.Format(_T("select * from import_xhj_table where right(name,1)='链'")) ;
// 		if (cSetFind.IsOpen())
// 		{
// 			cSetFind.Close() ;
// 		}
// 		if (cSetFind.Open(g_PtrCon, strSql))
// 		{		 
// 			if (cSetFind.GetRecordCount()>0)
// 			{
// 				cSetFind.MoveFirst() ;
// 				while (!cSetFind.IsEOF())
// 				{
// 					long iIdTmp = 0 ;
// 					long iLenTmp = 0 ;
// 					long iLocTmp = 0 ;
// 					long iShowLocTmp = 0 ;
// 					CString strNameLSChainTmp = _T("") ;			
// 					cSetFind.GetFieldValue(_T("id"), iIdTmp) ;
// 					cSetFind.GetFieldValue(_T("len"), iLenTmp) ;
// 					cSetFind.GetFieldValue(_T("loc_num"), iLocTmp) ;
// 					cSetFind.GetFieldValue(_T("loc_show_num"), iShowLocTmp) ;
// 					cSetFind.GetFieldValue(_T("name"), strNameLSChainTmp) ;
// 					if (strNameLSChainTmp==_T("长链"))
// 					{
// 						iLenTmp = iLenTmp ;
// 					}
// 					else if (strNameLSChainTmp==_T("短链"))
// 					{
// 						iLenTmp = -iLenTmp ;
// 					}
// 					
// 					if (iShowLocTmp>iStartEndLoc[0]&&iLocTmp<iStartEndLoc[1]&&iShowLocTmp<(iStartEndLoc[1]+iLenTmp))
// 					{
// 						iIdLongShortChain = iIdTmp ;
// 						iLenLongShortChain = iLenTmp ;
// 						iShowLocLongShortChain = iShowLocTmp ;
// 						iRet = 1 ;
// 						break ;
// 					}		
// 					cSetFind.MoveNext() ;
// 				}
// 			}
// 			cSetFind.Close() ;
// 		}
// 		//end 查找在iIdStart和iIdEnd之间有无长短链

	return iRet ;

}

BOOL CLKDataTable::GetXHJName(const CString strOldName, CString &strNewName)
{
	long iOldName = 0 ;
	iOldName = _tstol(strOldName) ;
	strNewName.Format(_T("%d%d%d%d%d"),iOldName/10000 ,iOldName%10000/1000,iOldName%10000%1000/100,iOldName%10000%1000%100/10,iOldName%10000%1000%100%10 ) ;

	return TRUE ;
}

void CLKDataTable::ComputeFreq()
{
	try
	{
		CString strSql = _T("") ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from compute_xhj_table where type=1 order by loc_real asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iSectionSum = cSet.GetRecordCount()-1 ;
			CString strBarMsg ;
			strBarMsg.Format(_T("处理频率")) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum) ;	

			int iStationIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iStationIndex) ;


				long iRealLocStation = 0, iRealNextLocStation = 0 ;
				
				CString strStation ;
				cSet.GetFieldValue(_T("name"), strStation) ;
				if (strStation == _T("滕州东"))
				{
					int iii = 0 ;
				}
				cSet.GetFieldValue(_T("loc_real"), iRealLocStation) ;
				
				cSet.MoveNext() ;
				if (!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("loc_real"), iRealNextLocStation) ;
					
					CADORecordset cSetFind ;
					
					for(int i=0; i<2; i++)
					{	
						strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and type<>1 and type<>2 and type<>11 and not (type=3 and up_or_down=%d and is_reverse=%d) and right(name,1)<>'链' order by loc_real %s"), iRealLocStation, iRealNextLocStation, i, i, i, i==0?_T("desc"):_T("asc")) ;
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon,strSql))
						{
							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
							}
							int iFreqIndex = 0 ;
							while (!cSetFind.IsEOF())
							{
								CString strSetFreq = _T("") ;
								CString strNewSetFreq = _T("") ;
								cSetFind.GetFieldValue(_T("set_freq"), strSetFreq) ;
																
								int iFreqIndexTmp = this->GetFreqIndex(strSetFreq, i) ;
								//有预设
								if (iFreqIndexTmp>-1)
								{
									iFreqIndex = iFreqIndexTmp+1 ;
								}
								else
								{
									this->GetFreqByIndex(iFreqIndex%4, strNewSetFreq, i) ;
									iFreqIndex++ ;
									cSetFind.Edit() ;
									cSetFind.SetFieldValue(_T("set_freq"), strNewSetFreq) ;
									cSetFind.Update() ;
								}				
									
								cSetFind.MoveNext() ;
							}
							cSetFind.Close() ;
						}						

					}				


				}
				else //next为eof，到最后一条记录
				{
					break ;
				}
				iStationIndex++ ;
			}
			cSet.Close() ;

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
		AfxMessageBox(_T("unknown error in ComputeFreq")) ;

	}


}

void CLKDataTable::ComputeDivide()
{
	try
	{
		CString strSql = _T("") ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from compute_xhj_table where type=1 or type=11 order by loc_real asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iSectionSum = cSet.GetRecordCount()-1 ;
			CString strBarMsg ;
			strBarMsg.Format(_T("处理%d个区间分界"), iSectionSum) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum) ;	

			int iStationIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iStationIndex) ;


				long iIdStation = 0, iNextIdStation = 0 ;
				long iTypeStation = 0, iNextTypeStation = 0 ;//本车站和下一个车站的类型，车站或者中继站
				long iLocStation = 0, iNextLocStation = 0 ;
				long iRealLocStation = 0, iRealNextLocStation = 0 ;
				
				cSet.GetFieldValue(_T("id"), iIdStation) ;
				cSet.GetFieldValue(_T("type"), iTypeStation) ;
				cSet.GetFieldValue(_T("loc_num"), iLocStation) ;
				cSet.GetFieldValue(_T("loc_real"), iRealLocStation) ;
// 				if (iRealLocStation==1035881)
// 				{
// 					int iii = 0 ;
// 				}
				
				cSet.MoveNext() ;
				if (!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("id"), iNextIdStation) ;
					cSet.GetFieldValue(_T("type"), iNextTypeStation) ;
					cSet.GetFieldValue(_T("loc_num"), iNextLocStation) ;
					cSet.GetFieldValue(_T("loc_real"), iRealNextLocStation) ;
					
					CADORecordset cSetFind ;

					int iCurDivideSum = 0 ;
					strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and is_divide=1 order by loc_real asc"), iRealLocStation, iRealNextLocStation) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						iCurDivideSum = cSetFind.GetRecordCount() ;
						cSetFind.Close() ;
					}
					//表示当前区间还没有指定分界点
					if (iCurDivideSum==0)
					{
// 						if (iIdStation==171)
// 						{
// 							int ia = 0 ;
// 						}
// 						long iIdLongShortChain_BetweenStation = 0 ;
// 						long iLenLongShortChain_BetweenStation = 0 ;
// 						long iShowLocLongShortChain_BetweenStation = 0 ;
// 						long iRealLocLongShortChain = 0 ;
// 						this->GetLongShortChain(iIdStation, iNextIdStation, iIdLongShortChain_BetweenStation, iLenLongShortChain_BetweenStation, iShowLocLongShortChain_BetweenStation,iRealLocLongShortChain,1) ;
						
						//2站间距离(加减长短链后的实际距离)
						//long iLenBetweenStation = iNextLocStation-iLocStation+iLenLongShortChain_BetweenStation ;
						long iLenBetweenStation = iRealNextLocStation-iRealLocStation ;
						//2站中心点loc，是以起点站中心为起点加实际区间半长
						//long iMidLocBetweenStation = iLocStation+iLenBetweenStation/2 ;
						long iMidLocBetweenStation = iRealLocStation+iLenBetweenStation/2 ;
						
						for(int i=0; i<2; i++)
						{	
							long iIdFoundDivide = 0 ;//离中心最近的信号机的Id

							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and type=0 and right(name,1)<>'链' and left(name,1)<>'F' order by loc_real asc"), iRealLocStation, iRealNextLocStation , i) ;
							if (cSetFind.IsOpen())
							{
								cSetFind.Close() ;
							}
							if (cSetFind.Open(g_PtrCon,strSql))
							{
								if (!cSetFind.IsBOF())
								{
									cSetFind.MoveFirst() ;
								}
								int iRsCount = cSetFind.GetRecordCount() ;
								long iMinLen = iLenBetweenStation/2 ;
								while (!cSetFind.IsEOF())
								{
									long iIdCurTGXHJ = 0 ;
									long iLocCurTGXHJ = 0 ;
									long iRealLocCurTGXHJ = 0 ;
									cSetFind.GetFieldValue(_T("id"), iIdCurTGXHJ) ;
									cSetFind.GetFieldValue(_T("loc_num"), iLocCurTGXHJ) ;
									cSetFind.GetFieldValue(_T("loc_real"), iRealLocCurTGXHJ) ;

// 									if (iIdCurTGXHJ==97)
// 									{
// 										int iiii = 0 ;
// 									}

// 									long iIdLongShortChain_ToCurTG = 0 ;
// 									long iLenLongShortChain_ToCurTG = 0 ;
// 									long iShowLocLongShortChain_ToCurTG = 0 ;
// 									long iRealLocLongShortChain = 0 ;
// 									this->GetLongShortChain(iIdStation, iIdCurTGXHJ, iIdLongShortChain_ToCurTG, iLenLongShortChain_ToCurTG, iShowLocLongShortChain_ToCurTG, iRealLocLongShortChain,1) ;

// 									long iCurTGLoc = iLocCurTGXHJ+iLenLongShortChain_ToCurTG ;
									long iCurTGLoc = iRealLocCurTGXHJ ;
									
									if (abs(iMidLocBetweenStation-iCurTGLoc)<iMinLen)
									{
										iMinLen = abs(iMidLocBetweenStation-iCurTGLoc) ;
										iIdFoundDivide = iIdCurTGXHJ ;
									}
									cSetFind.MoveNext() ;
								}
								cSetFind.Close() ;
							}

							strSql.Format(_T("update compute_xhj_table set is_divide=1 where id=%d"), iIdFoundDivide) ;
							ExecSqlString(strSql) ;
						}

					}




				}
				else //next为eof，到最后一条记录
				{
					break ;
				}

				iStationIndex++ ;
			}
			cSet.Close() ;

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
		AfxMessageBox(_T("unknown error in ComputeDivide")) ;

	}

}

//更新importXHJTable中的loc_real
void CLKDataTable::UpDateAllRealLoc()
{
	try
	{
		CString strSql = _T("") ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from import_xhj_table where right(name,1)='链' order by id asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iLongShortChainSum = cSet.GetRecordCount()-1 ;
			CString strBarMsg ;
			strBarMsg.Format(_T("处理%d个长短链"), iLongShortChainSum) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iLongShortChainSum) ;	

			int iLongShortChainIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iLongShortChainIndex) ;


				long iId = 0 ;
				long iLen = 0 ;
				CString strName = _T("") ;
								
				cSet.GetFieldValue(_T("id"), iId) ;
				cSet.GetFieldValue(_T("name"), strName) ;
				cSet.GetFieldValue(_T("len"), iLen) ;

				if (iId==134)
				{
					int iii = 0 ;
				}
				
				if (strName==_T("短链"))
				{
					iLen = 0-iLen ;
				}

				CADORecordset cSetUpdate ;
				strSql.Format(_T("select * from import_xhj_table where id>=%d order by id asc"), iId) ;
				if (cSetUpdate.IsOpen())
				{
					cSetUpdate.Close() ;
				}
				if (cSetUpdate.Open(g_PtrCon, strSql))
				{
					if (!cSetUpdate.IsBOF())
					{
						cSetUpdate.MoveFirst() ;
					}
					while (!cSetUpdate.IsEOF())
					{
						long iRealLoc = 0 ;
						cSetUpdate.GetFieldValue(_T("loc_real"), iRealLoc) ;
						long iNewRealLoc = 0 ;
						iNewRealLoc = iRealLoc+iLen ;
						cSetUpdate.Edit() ;
						cSetUpdate.SetFieldValue(_T("loc_real"), iNewRealLoc) ;
						cSetUpdate.Update() ;
						cSetUpdate.MoveNext() ;
					}
					cSetUpdate.Close() ;
				}


				//strSql.Format(_T("UPDATE import_xhj_table SET loc_real=loc_real%s where id>=%d"), strTmp, iId) ;
				//ExecSqlString(strSql) ;
				
				iLongShortChainIndex++ ;

				cSet.MoveNext() ;

			}
			cSet.Close() ;

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
		AfxMessageBox(_T("unknown error in updateallrealloc")) ;

	}

}

BOOL CLKDataTable::ConvertSpecialChar(const CString strSrc, CString &strRet)
{
	BOOL bRet = FALSE ;
	CString strTmp[10] = {_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX"),_T("X")} ;
	for (int i=0; i<10; i++)
	{
		if (strSrc==strTmp[i])
		{
			if(i==9)
			{
				strRet.Format(_T("10")) ;
			}
			else
			{
				strRet.Format(_T("%d"), i+1) ;
			}
			bRet = TRUE ;
			break ;
		}
		
	}
	
	return bRet ;

}

void CLKDataTable::ComputeXHData()
{
	this->ComputerFG() ;
	this->ComputeDivide() ;
	this->ComputeGdName() ;
	this->ComputeFreq() ;

}

//得到频率当前index
int CLKDataTable::GetFreqIndex(const CString strFreq, int iDownOrUp)
{
	if (iDownOrUp<0||iDownOrUp>1||strFreq==_T(""))
	{
		return -1 ;
	}

	int iRet = -1 ;

	CString strFreqAll[2][4] = {
		{_T("2300-1"),_T("1700-1"),_T("2300-2"),_T("1700-2")},
		{_T("2000-1"),_T("2600-1"),_T("2000-2"),_T("2600-2")}
	} ;
	for (int i=0; i<4; i++)
	{
		if (strFreq==strFreqAll[iDownOrUp][i])
		{
			iRet = i ;
			break ;
		}
	}

	return iRet ;
}

BOOL CLKDataTable::GetFreqByIndex(int iIndex, CString &strFreq, int iDownOrUp)
{
	BOOL bRet = FALSE ;
	CString strFreqAll[2][4] = {
		{_T("2300-1"),_T("1700-1"),_T("2300-2"),_T("1700-2")},
		{_T("2000-1"),_T("2600-1"),_T("2000-2"),_T("2600-2")}
	} ;

	if (iIndex>=0&&iIndex<4&&iDownOrUp>=0&&iDownOrUp<2)
	{
		strFreq = strFreqAll[iDownOrUp][iIndex] ;
		bRet = TRUE ;
	}
	
	return bRet ;

}

//导出信号数据表
void CLKDataTable::ExportXHData(CString strSavePath)
{	
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	CString strTempletFile ;
	strTempletFile.Format(_T("%s\\support\\lkdata\\线路数据表.xls"), m_strSysBasePath) ;

	CUserSystem mysys ;

	//CString strMileSys = _T("DK") ;

	_ApplicationPtr m_pExcel = NULL ;
	//WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	_WorksheetPtr pSheet2 = NULL ;

	CString strMsg ;
	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( _T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return ;
		}

		//pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBook = m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strTempletFile ) ;
		
		CString strCaseName[4] = {_T("上行线正向"),_T("下行线正向"),_T("上行线反向"),_T("下行线反向")} ;
		CString strOrder[4] = {_T("desc"),_T("asc"),_T("asc"),_T("desc")} ;
		
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;

		int iSheetIndex = 0 ;
		for (iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
		{
 		//	continue ;

			vt_index.iVal = iSheetIndex+1 ;
			
			pWorksheets  = m_pExcel->GetSheets( ) ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;			
			
			CString strWriteText = _T("") ;
			
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)_T("A1")) ;
			strWriteText.Format(_T("%s%s信号数据表"), this->m_strXHTitle, strCaseName[iSheetIndex]) ;
			pRange->PutFormulaR1C1((_variant_t)strWriteText) ;	
			
			CADORecordset cSetFind ;
			CString strSql ;
			strSql.Format(_T("select * from compute_xhj_table where type=1 or type=11 order by loc_real %s"), strOrder[iSheetIndex] ) ;
			//strSql.Format(_T("select * from compute_xhj_table where id=1471 order by loc_real %s"), strOrder[iSheetIndex] ) ;

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
				int iStationSum = cSetFind.GetRecordCount()-1 ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%d个站:%s"), iStationSum, strCaseName[iSheetIndex]) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iStationSum) ;	

				int iStationIndex = 0 ;
				int iCurRow = 4 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iStationIndex) ;

					CString strStationName = _T("") ;
					cSetFind.GetFieldValue(_T("name"), strStationName) ;
					long iStationRealLoc = 0 ;
					cSetFind.GetFieldValue(_T("loc_real"), iStationRealLoc) ;
					
					CADORecordset cSetFind2 ; 
					//分界点坐标
					long iLeftDivideRealLoc = iStationRealLoc ;
					long iRightDivideRealLoc = iStationRealLoc ;
					for (int iDivideIndex=0; iDivideIndex<2; iDivideIndex++)
					{
						if (iDivideIndex==0)
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down =%d and is_divide=1 order by loc_real desc"), iStationRealLoc, iSheetIndex==0?1:0) ;
						}
						else
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down =%d and is_divide=1 order by loc_real asc"), iStationRealLoc, iSheetIndex==0?1:0) ;
						}
						if (cSetFind2.IsOpen())
						{
							cSetFind2.Close() ;
						}
						if (cSetFind2.Open(g_PtrCon, strSql))
						{
							if (cSetFind2.GetRecordCount()>0)
							{
								cSetFind2.MoveFirst() ;
								if (iDivideIndex==0)
								{
									cSetFind2.GetFieldValue(_T("loc_real"), iLeftDivideRealLoc) ;
								}
								else
								{
									cSetFind2.GetFieldValue(_T("loc_real"), iRightDivideRealLoc) ;
								}
							}
							cSetFind2.Close() ;
						}
					}
					
					strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and (right(name,1)<>'链' or type=4) and type not in(1,11,2) order by loc_real %s"), iLeftDivideRealLoc, iRightDivideRealLoc, iSheetIndex==0?1:0, strOrder[iSheetIndex]) ;
					if (cSetFind2.IsOpen())
					{
						cSetFind2.Close() ;
					}
					if (cSetFind2.Open(g_PtrCon,strSql))
					{
						if (!cSetFind2.IsBOF())
						{
							cSetFind2.MoveFirst() ;
						}
						int iXhjCount = cSetFind2.GetRecordCount() ;
						int iXhjIndex = 0 ;
						while (!cSetFind2.IsEOF())
						{
							CString strXHJName = _T("") ;
							CString strLocNum = _T("") ;
							CString strFreq = _T("") ;
							CString strGdName = _T("") ;
							//CString strLocShow = _T("") ;
							long iLen = 0 ;
							int iIsReverse = -1 ;
							int iType = -1 ;
							long iLocNum = 0 ;

							cSetFind2.GetFieldValue(_T("name"), strXHJName) ;
							cSetFind2.GetFieldValue(_T("loc_num"), iLocNum) ;
							cSetFind2.GetFieldValue(_T("gd_name"), strGdName) ;
							//cSetFind2.GetFieldValue(_T("loc_show"), strLocShow) ;
							cSetFind2.GetFieldValue(_T("is_reverse"), iIsReverse) ;
							cSetFind2.GetFieldValue(_T("type"), iType) ;

							//电气绝缘节（区间）要偏移信号牌1米
							if (iType==0)
							{
								iLocNum+=(iSheetIndex==0?-1:1) ;
							}
							this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;

// 							//////////////////////////////////////////////////////////////////////////
// 							//为适应区间已出图纸，分割点名称按加减长短链后的里程命名,实际不对
// 							long iRealLoc_F = 0 ;
// 							cSetFind2.GetFieldValue(_T("loc_real"), iRealLoc_F) ;
// 							//////////////////////////////////////////////////////////////////////////

							if (iSheetIndex==1)
							{
								cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
								cSetFind2.GetFieldValue(_T("len"), iLen) ;
							}
							
							if (strGdName==_T(""))
							{
// 								if (iType==3&&iIsReverse==0)//如果遇到进站，加入站内的信号点
// 								{
// 
// 								}
								cSetFind2.MoveNext() ;
								iXhjIndex++ ;
								continue ;
							}
							cSetFind2.MoveNext() ;
							iXhjIndex++ ;
							if (cSetFind2.IsEOF())
							{
								break ;
							}
							if (iSheetIndex==0)
							{
								cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
								cSetFind2.GetFieldValue(_T("len"), iLen) ;
							}

							//begin 下一架信号机info
							//CString strNextXhjName = _T("") ;
							int iNextXhjType = -1 ;
							int iIsNextXhjReverse = -1 ;
							//cSetFind2.GetFieldValue(_T("name"), strNextXhjName) ;
							cSetFind2.GetFieldValue(_T("type"), iNextXhjType) ;
							cSetFind2.GetFieldValue(_T("is_reverse"), iIsNextXhjReverse) ;
							//end 下一架信号机info

							if (iNextXhjType==3 && iIsNextXhjReverse==0)
							{
								iLen-=1 ;
							}
							
							CString strXHJType = _T("") ;
							CString strJYJType = _T("电气绝缘节") ;
							if (strXHJName.Left(1)==_T("F"))
							{
								strXHJType = _T("没有信号机") ;
// 								//////////////////////////////////////////////////////////////////////////
// 								//为适应区间已出图纸，分割点名称按加减长短链后的里程命名,实际不对
// 								this->GetFName(iRealLoc_F, strXHJName, iSheetIndex==0?1:0) ;
// 								//////////////////////////////////////////////////////////////////////////

							}
							else if (iType==3)
							{
								if (iIsReverse==0)
								{
									strXHJType = _T("进站信号机") ;

								}
								else if (iIsReverse==1)
								{
									strXHJType = _T("出站口") ;
									iLen+=1 ;
								}								
								strJYJType = _T("机械绝缘节") ;
							}
							else if (iType==4)
							{
								strXHJType = _T("没有信号机") ;
								strJYJType = _T("机械绝缘节") ;
								//出站信号机								
								CADORecordset cSetFindCz ;
								strSql.Format(_T("select * from import_xhj_info where loc_num=%d and up_or_down=%d and type=2 "), iLocNum, iSheetIndex==0?1:0 ) ;
								if (cSetFindCz.IsOpen())
								{
									cSetFindCz.Close() ;
								}
								if (cSetFindCz.Open(g_PtrCon, strSql))
								{
									if (cSetFindCz.GetRecordCount()>0)
									{
										cSetFindCz.MoveFirst() ;
										cSetFindCz.GetFieldValue(_T("name"), strXHJName) ;
										//strXHJType = _T("出站信号机") ;
										if (strXHJName.Left(1)==(iSheetIndex==0?_T("S"):_T("X")))
										{
											strXHJType = _T("出站信号机") ;
										}
									}

									cSetFindCz.Close() ;
								}					

							}
							else
							{
								strXHJType= _T("通过信号机") ;
							}
							
							CStringArray strArrayTxt ;
							CString strCellContent = _T("") ;
							strCellContent.Format(_T("%d"), iCurRow-3) ;
							CString strStationNameInfact = strStationName ;
							if (strStationName.Find(_T("线路所"))==-1)
							{
								strStationNameInfact.Insert(555, _T("站")) ;
							}

							CString strSectionAtt = _T("") ;
							if (iXhjIndex==1 || iXhjIndex==iXhjCount-1)
							{
								strSectionAtt = _T("TCC边界") ;
							}
							
							strArrayTxt.Add(strCellContent) ;
							strArrayTxt.Add(strStationNameInfact) ;
							strArrayTxt.Add(strXHJName) ;
							strArrayTxt.Add(strLocNum) ;
							strArrayTxt.Add(strXHJType) ;
							strArrayTxt.Add(strJYJType) ;
							strArrayTxt.Add(strGdName) ;
							strArrayTxt.Add(strFreq) ;
							strCellContent.Format(_T("%d"),iLen) ;
							strArrayTxt.Add(strCellContent) ;
							strArrayTxt.Add(strSectionAtt) ;
							
							CString strColName[10] = {_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J")} ;
							
							for (int iColIndex=0; iColIndex<10; iColIndex++ )
							{
								CString strCellName = _T("") ;
								strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
								pRange = NULL ;
								pRange = pSheet->GetRange((_variant_t)strCellName) ;
								pRange->PutNumberFormat((_variant_t)_T("@")) ;
								pRange->PutFormulaR1C1((_variant_t)strArrayTxt.GetAt(iColIndex)) ;
								
							}							
							
							iCurRow++ ;
							//cSetFind2.MoveNext() ;
						}
						
						cSetFind2.Close() ;
					}
					
					iStationIndex++ ;
					cSetFind.MoveNext() ;
				}
			cSetFind.Close() ;

			acedRestoreStatusBar() ;

			}
		}

		//iSheetIndex=2 上行反向 iSheetIndex=3 下行反向
		for (iSheetIndex=2; iSheetIndex<4; iSheetIndex++)
		{		
			vt_index.iVal = iSheetIndex+1 ;
			
			pWorksheets  = m_pExcel->GetSheets( ) ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;			
			
			CString strWriteText = _T("") ;
			
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)_T("A1")) ;
			strWriteText.Format(_T("%s%s信号数据表"), this->m_strXHTitle, strCaseName[iSheetIndex]) ;
			pRange->PutFormulaR1C1((_variant_t)strWriteText) ;	
			
			CADORecordset cSetFind ;
			CString strSql ;
 			strSql.Format(_T("select * from compute_xhj_table where type=1 or type=11 order by loc_real %s"), strOrder[iSheetIndex] ) ;
			//strSql.Format(_T("select * from compute_xhj_table where id=1471 order by loc_real %s"), strOrder[iSheetIndex] ) ;

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
				int iStationSum = cSetFind.GetRecordCount()-1 ;
				CString strBarMsg ;
				strBarMsg.Format(_T("%d个站:%s"), iStationSum, strCaseName[iSheetIndex]) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iStationSum) ;	

				int iStationIndex = 0 ;
				int iCurRow = 4 ;
				long iPreLocReal = 0 ; //起始信号点坐标
				BOOL bIsFirstXhj = 1 ; //第一个信号点
				
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iStationIndex) ;

					CString strStationName = _T("") ;
					cSetFind.GetFieldValue(_T("name"), strStationName) ;
					long iStationRealLoc = 0 ;
					cSetFind.GetFieldValue(_T("loc_real"), iStationRealLoc) ;
					
					if (strStationName==_T("枣庄西"))
					{
						int iii = 0 ;
					}
					
					CADORecordset cSetFind2 ; 
					//分界点坐标
					long iLeftDivideRealLoc = iStationRealLoc ;
					long iRightDivideRealLoc = iStationRealLoc ;
					for (int iDivideIndex=0; iDivideIndex<2; iDivideIndex++)
					{
						if (iDivideIndex==0)
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down =%d and is_divide=1 order by loc_real desc"), iStationRealLoc, iSheetIndex==2?1:0) ;
						}
						else
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down =%d and is_divide=1 order by loc_real asc"), iStationRealLoc, iSheetIndex==2?1:0) ;
						}
						if (cSetFind2.IsOpen())
						{
							cSetFind2.Close() ;
						}
						if (cSetFind2.Open(g_PtrCon, strSql))
						{
							if (cSetFind2.GetRecordCount()>0)
							{
								cSetFind2.MoveFirst() ;
								if (iDivideIndex==0)
								{
									cSetFind2.GetFieldValue(_T("loc_real"), iLeftDivideRealLoc) ;
								}
								else
								{
									cSetFind2.GetFieldValue(_T("loc_real"), iRightDivideRealLoc) ;
								}
							}
							cSetFind2.Close() ;
						}
					}
					//该车站所有信号点
					strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and (right(name,1)<>'链' or type=4) and type not in(1,11,2) order by loc_real %s"), iLeftDivideRealLoc, iRightDivideRealLoc, iSheetIndex==2?1:0, strOrder[iSheetIndex]) ;
					if (cSetFind2.IsOpen())
					{
						cSetFind2.Close() ;
					}
					if (cSetFind2.Open(g_PtrCon,strSql))
					{
						if (!cSetFind2.IsBOF())
						{
							cSetFind2.MoveFirst() ;
						}
						int iRsXhjCount = cSetFind2.GetRecordCount() ;
						int iXhjIndex = 0 ;
						while (!cSetFind2.IsEOF())
						{
							CString strXHJName = _T("") ;
							CString strFreq = _T("") ;
							CString strGdName = _T("") ;
							CString strLocNum = _T("") ;
							long iLen = 0 ;
							int iIsReverse = -1 ;
							int iType = -1 ;
							long iLocNum = 0 ;
							int iBaOffset = 0 ;
							cSetFind2.GetFieldValue(_T("name"), strXHJName) ;
							cSetFind2.GetFieldValue(_T("loc_num"), iLocNum) ;
							cSetFind2.GetFieldValue(_T("is_reverse"), iIsReverse) ;
							cSetFind2.GetFieldValue(_T("type"), iType) ;
							cSetFind2.GetFieldValue(_T("ba_offset"), iBaOffset) ;

							//////////////////////////////////////////////////////////////////////////
							//为适应区间已出图纸，分割点名称按加减长短链后的里程命名,实际不对
							long iRealLoc_F = 0 ;
							cSetFind2.GetFieldValue(_T("loc_real"), iRealLoc_F) ;
							//////////////////////////////////////////////////////////////////////////

							long iLocReal = iRealLoc_F ;  //为计算长度而设
							//电气绝缘节（区间）要偏移信号牌
							if (iType==0)
							{
								iBaOffset = (iBaOffset==0?m_nBaOffset:iBaOffset) ;
								iLocNum+=((iSheetIndex==2?-1:1)*(iBaOffset+1)) ;
								iLocReal+=((iSheetIndex==2?-1:1)*(iBaOffset+1)) ;
							}
							this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;

							//第一个信号点不计算长度，只保存其real坐标
							if (bIsFirstXhj)
							{
								iLen = 0 ;
								iPreLocReal = iLocReal ;
								bIsFirstXhj = FALSE ;
							}
							else
							{
								iLen = abs(iLocReal-iPreLocReal) ; 
								iPreLocReal = iLocReal ;
							}

							if (iSheetIndex==2)
							{
								cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
								//cSetFind2.GetFieldValue(_T("len"), iLen) ;
							}		

							cSetFind2.MoveNext() ;						
							iXhjIndex++ ;
							if (cSetFind2.IsEOF())
							{
								if (iXhjIndex!=1)//==1，在进站处分界，只有一个进站信号机
								{
									// 填写前一个的长度
									CString strCellName = _T("") ;
									strCellName.Format(_T("I%d"), iCurRow-1) ;								
									pRange = NULL ;
									pRange = pSheet->GetRange((_variant_t)strCellName) ;
									pRange->PutNumberFormat((_variant_t)_T("@")) ;
									CString	strLen = _T("") ;
									strLen.Format(_T("%d"),iLen) ;
									pRange->PutFormulaR1C1((_variant_t)iLen) ;
									//
								}
								break ;
							}
							//begin 如果不是最后一个信号机，则进一步检查是不是反向进站，并检查有无站内数据
							CString strNextXhjName = _T("") ;
							cSetFind2.GetFieldValue(_T("name"), strNextXhjName) ;
							if (iIsReverse==1 && (strNextXhjName==_T("S")||strNextXhjName==_T("X")))
							{
								bIsFirstXhj = TRUE ;
								
								// 填写前一个的长度
								CString strCellName = _T("") ;
								strCellName.Format(_T("I%d"), iCurRow-1) ;								
								pRange = NULL ;
								pRange = pSheet->GetRange((_variant_t)strCellName) ;
								pRange->PutNumberFormat((_variant_t)_T("@")) ;
								CString	strLen = _T("") ;
								strLen.Format(_T("%d"),iLen) ;
								pRange->PutFormulaR1C1((_variant_t)iLen) ;
								//
								
								continue ;
							}
							//end 如果不是最后一个信号机，则进一步检查是不是反向进站，并检查有无站内数据

							if (iSheetIndex==3)
							{
								cSetFind2.GetFieldValue(_T("set_freq"), strFreq) ;
								//cSetFind2.GetFieldValue(_T("len"), iLen) ;
							}
							cSetFind2.GetFieldValue(_T("gd_name"), strGdName) ;

							
							CString strXHJType = _T("") ;
							CString strJYJType = _T("电气绝缘节") ;
							if (strXHJName.Left(1)==_T("F"))
							{
								strXHJType = _T("没有信号机") ;
// 								//////////////////////////////////////////////////////////////////////////
// 								//为适应区间已出图纸，分割点名称按加减长短链后的里程命名,实际不对
// 								this->GetFName(iRealLoc_F, strXHJName, iSheetIndex==2?1:0) ;
// 								//////////////////////////////////////////////////////////////////////////
							}
							else if (iType==3)
							{
								if (iIsReverse==1)
								{
									strXHJType = _T("进站信号机") ;
								}
								else if (iIsReverse==0)
								{
									strXHJType = _T("出站口") ;
								}								
								strJYJType = _T("机械绝缘节") ;
							}
							else if (iType==4)
							{
								strXHJType = _T("没有信号机") ;
								strJYJType = _T("机械绝缘节") ;
								//出站信号机								
								CADORecordset cSetFindCz ;
								strSql.Format(_T("select * from import_xhj_info where loc_num=%d and up_or_down=%d and type=2 "), iLocNum, iSheetIndex==2?1:0) ;
								if (cSetFindCz.IsOpen())
								{
									cSetFindCz.Close() ;
								}
								if (cSetFindCz.Open(g_PtrCon, strSql))
								{
									if (cSetFindCz.GetRecordCount()>0)
									{
										cSetFindCz.MoveFirst() ;
										cSetFindCz.GetFieldValue(_T("name"), strXHJName) ;										
										if (strXHJName.Left(1)==(iSheetIndex==2?_T("X"):_T("S")))
										{
											strXHJType = _T("出站信号机") ;
										}
										
									}									
									cSetFindCz.Close() ;
								}
							}
							else
							{
								strXHJType= _T("通过信号机") ;
							}
							
							CStringArray strArrayTxt ;
							CString strCellContent = _T("") ;
							strCellContent.Format(_T("%d"), iCurRow-3) ;

							CString strStationNameInfact = strStationName ;
							if (strStationName.Find(_T("线路所"))==-1)
							{
								strStationNameInfact.Insert(555, _T("站")) ;
							}

							CString strSectionAtt = _T("") ;
							if (iXhjIndex==1 || iXhjIndex==iRsXhjCount-1)
							{
								strSectionAtt = _T("TCC边界") ;
							}

							strArrayTxt.Add(strCellContent) ;
							strArrayTxt.Add(strStationNameInfact) ;
							strArrayTxt.Add(strXHJName) ;
							strArrayTxt.Add(strLocNum) ;
							strArrayTxt.Add(strXHJType) ;
							strArrayTxt.Add(strJYJType) ;
							strArrayTxt.Add(strGdName) ;
							strArrayTxt.Add(strFreq) ;
							strCellContent.Format(_T("%d"),iLen) ;
							strArrayTxt.Add(strCellContent) ;
							strArrayTxt.Add(strSectionAtt) ;

							
							CString strColName[10] = {_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J")} ;
							
							for (int iColIndex=0; iColIndex<10; iColIndex++ )
							{
								if (iColIndex==8 && iLen==0)
								{
									continue ;
								}
								CString strCellName = _T("") ;
								strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
								if (iColIndex==8)
								{
									strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow-1) ;
								}
								pRange = NULL ;
								pRange = pSheet->GetRange((_variant_t)strCellName) ;
								pRange->PutNumberFormat((_variant_t)_T("@")) ;
								pRange->PutFormulaR1C1((_variant_t)strArrayTxt.GetAt(iColIndex)) ;								
							}				
							
							iCurRow++ ;
							//cSetFind2.MoveNext() ;
						}
						
						cSetFind2.Close() ;
					}
					
					iStationIndex++ ;
					cSetFind.MoveNext() ;
				}
			cSetFind.Close() ;

			acedRestoreStatusBar() ;

			}
		}

		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;

		CString strSaveFileName ;
		strSaveFileName.Format(_T("%s\\%s线路数据表.xls"), strSavePath, m_strXHTitle) ;
	
		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;

		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));

		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}
}


void CLKDataTable::ComputeGdName()
{
	try
	{
		CString strSql = _T("") ;
		CADORecordset cSet ;
		for(int i=0; i<2; i++)
		{
			strSql.Format(_T("select * from compute_xhj_table where up_or_down=%d and type<>1 and type<>2 and type<>11 and right(name,1)<>'链' and left(name,1)<>'F' order by loc_real %s"), i, i==0?_T("asc"):_T("desc")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}

				int iSectionSum = cSet.GetRecordCount()-1 ;
				CString strBarMsg ;
				strBarMsg.Format(_T("计算%s轨道名称"), i==0?_T("下行"):_T("上行")) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum) ;	

				int iStationIndex = 0 ;
				while (!cSet.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iStationIndex) ;


					CString strCurXHJName = _T("") ;
					long iRealLocCurXHJ = 0 ;
					long iRealLocNextXHJ = 0 ;
					int iCurXHJType = -1 ;
					int iCurIsReverse = -1 ;
					cSet.GetFieldValue(_T("name"), strCurXHJName) ;
					cSet.GetFieldValue(_T("loc_real"), iRealLocCurXHJ) ;
					cSet.GetFieldValue(_T("type"), iCurXHJType) ;
					cSet.GetFieldValue(_T("is_reverse"), iCurIsReverse) ;

					if (iCurXHJType==3 && iCurIsReverse==0) //X  S
					{
						cSet.MoveNext() ;
						iStationIndex++ ;
						continue ;
					}
					
					cSet.MoveNext() ;
					if (!cSet.IsEOF())
					{
						cSet.GetFieldValue(_T("loc_real"), iRealLocNextXHJ) ;

						CADORecordset cSetFind ;
						if (i==0)
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<%d and up_or_down=%d and type<>11 and right(name,1)<>'链' order by loc_real %s"), iRealLocCurXHJ, iRealLocNextXHJ, i, i==0?_T("asc"):_T("desc")) ;
						}
						else
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<=%d and up_or_down=%d and type<>11 and right(name,1)<>'链' order by loc_real %s"), iRealLocNextXHJ, iRealLocCurXHJ, i, i==0?_T("asc"):_T("desc")) ;
						}
						if (cSetFind.IsOpen())
						{
							cSetFind.Close() ;
						}
						if (cSetFind.Open(g_PtrCon, strSql))
						{
							int iFirstIsReverse = 0 ;

							if (!cSetFind.IsBOF())
							{
								cSetFind.MoveFirst() ;
								cSetFind.GetFieldValue(_T("is_reverse"), iFirstIsReverse) ;
							}
							int iRsCount = cSetFind.GetRecordCount() ;
							
							if (iFirstIsReverse==1)//反向进站 1LQ
							{
								strCurXHJName = (i==0?_T("X1LQ"):_T("S1LQ")) ;
							}

							for (int iXHJIndex=0; iXHJIndex<iRsCount; iXHJIndex++)
							{
								CString strABC = _T("") ;
								CString strGdName = _T("") ;
								if (iRsCount==1 )//|| (iFirstIsReverse==1&&iXHJIndex==0) //1LQG<->1LQBG
								{
									strABC = _T("") ;
								}
								else
								{
									strABC.Format(_T("%c"), 'A'+iRsCount-iXHJIndex-1) ;
								}

								strGdName.Format(_T("%s%sG"), strCurXHJName, strABC) ;

								cSetFind.Edit() ;
								cSetFind.SetFieldValue(_T("gd_name"), strGdName) ;
								cSetFind.Update() ;

								cSetFind.MoveNext() ;
							}

							cSetFind.Close() ;
						}

					}
					else
					{

					}

					iStationIndex++ ;
				}

				cSet.Close() ;


				acedRestoreStatusBar() ;

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
		AfxMessageBox(_T("unknown error in ComputeGdName")) ;

	}

}


int CLKDataTable::ImportLsbTable(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_compute_lsb")) ;
	// 	CUserSystem mysys ;

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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}
		
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		
		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		CString strColName[8] = {_T("D"),_T("E"),_T("F"),_T("G"),_T("I"),_T("J"),_T("L"),_T("N")} ;
		
		int iCurRow = 4 ;
		BOOL bNext = TRUE ;
		while (bNext)
		{
			if (iCurRow==26)
			{
				int ii = 0 ;
			}
			CString strToOrFrom = _T(""), strButton = _T(""), strStartXhjName = _T(""), strStartXhjShow = _T(""), strDcList = _T(""), strGdList = _T("");
			int iJlIndex = 0 ;
			int iJlMode = 0 ;			
			
			BOOL bCurRowIsBlank = TRUE ;
			CString strBarMsg ;
			strBarMsg.Format(_T("读取联锁表：第%d行"), iCurRow) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, 7) ;	

			
			BOOL bReadSomething = FALSE ;
			for (int iColIndex=0; iColIndex<8; iColIndex++)
			{

				acedSetStatusBarProgressMeterPos(iColIndex) ;

				CString strCellName = _T(""), strCellText = _T("") ;
				strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				strCellText = (CString)(pRange->GetText().bstrVal) ;
				strCellText.TrimLeft() ;
				strCellText.TrimRight() ;
				
				if (iColIndex==0)
				{
					if (strCellText==_T(""))
					{
						break ;
					}
					else
					{
						bReadSomething = TRUE ;
					}
				}				
				
				switch (iColIndex)
				{
				case 0:
					iJlIndex = _tstoi(strCellText) ;
					break;
				case 1:
					{
						strToOrFrom = strCellText ;
					}
					break;
				case 2:
					iJlMode = _tstoi(strCellText) ;
					break;
				case 3:
					strButton = strCellText ;
					break;
				case 4:
					strStartXhjName = strCellText ;
					break;
				case 5:
					strStartXhjShow = strCellText ;							
					break;
				case 6:
					strDcList = strCellText ;
					{
// 						int iLoc = strCellText.Find(_T("{")) ;
// 						if (iLoc>-1)
// 						{
// 							strDcList = strCellText.Left(iLoc-1) ;
// 						}
// 						else
// 						{
							strDcList = strCellText ;
// 						}
					}
					break;
				case 7:
					{
						strGdList = strCellText ;					
					}
					break;
				}
			}
			if (bReadSomething)
			{		
				strExec.Format(_T("insert into import_compute_lsb(jl_index,to_or_from, jl_mode, jl_button, startxhj_name, startxhj_show, dc_list, gd_list)\
								  values(%d,'%s',%d,'%s', '%s','%s','%s','%s' )"), iJlIndex, strToOrFrom, iJlMode, strButton, strStartXhjName, strStartXhjShow, strDcList, strGdList) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
			}	
			
			if (bCurRowIsBlank)
			{
				bNext = FALSE ;
			}
			else
			{
				iCurRow++ ;
			}

			acedRestoreStatusBar() ;

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
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	
	this->ComputeDc() ;

	return iRet ;
}

int CLKDataTable::ImportGdSectionInfo(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_gdsection_info")) ;

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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}
		
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		
		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		CString strColName[6] = {_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G")} ;
		
		int iCurRow = 3 ;
		BOOL bNext = TRUE ;
		while (bNext)
		{
// 			if (iCurRow==26)
// 			{
// 				int ii = 0 ;
// 			}
			CString strGdName= _T(""), strFreq = _T(""), strReverseDc = _T(""), strBelongStation = _T("") ;
			int iSectionLen = 0 ;
			int iUpOrDown = -1 ;
			
			BOOL bCurRowIsBlank = TRUE ;
			CString strBarMsg ;
			strBarMsg.Format(_T("读取轨道区段表：第%d行"), iCurRow) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, 3) ;	

			
			BOOL bReadSomething = FALSE ;
			for (int iColIndex=0; iColIndex<6; iColIndex++)
			{

				acedSetStatusBarProgressMeterPos(iColIndex) ;

				CString strCellName = _T(""), strCellText = _T("") ;
				strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				strCellText = (CString)(pRange->GetText().bstrVal) ;
				strCellText.TrimLeft() ;
				strCellText.TrimRight() ;
				
				if (iColIndex==0)
				{
					if (strCellText==_T(""))
					{
						break ;
					}
					else
					{
						bReadSomething = TRUE ;
					}
				}				
				
				switch (iColIndex)
				{
				case 0:
					strGdName = strCellText ;
					break;
				case 1:					
					strFreq = strCellText ;					
					break;
				case 2:
					strReverseDc = strCellText ;	
					break;
				case 3:
					iSectionLen = _tstoi(strCellText) ;
					break;	
				case 4:
					if ((strCellText==_T("下行")||strCellText==_T("0")||strCellText==_T("X"))&&strReverseDc==_T(""))
					{
						iUpOrDown = 0 ;
					}
					else if ((strCellText==_T("上行")||strCellText==_T("1")||strCellText==_T("S"))&&strReverseDc==_T(""))
					{
						iUpOrDown = 1 ;
					}
					else if ((strCellText==_T("下行P")||strCellText==_T("10")||strCellText==_T("XP"))&&strReverseDc==_T(""))
					{
						iUpOrDown = 10 ;
					}
					else if ((strCellText==_T("上行P")||strCellText==_T("11")||strCellText==_T("SP"))&&strReverseDc==_T(""))
					{
						iUpOrDown = 11 ;
					}
					else
					{
						iUpOrDown = -1 ;
					}		
					break;	
				case 5:
					strBelongStation = strCellText ;
					break;
				}
			}
			if (bReadSomething)
			{		
				strExec.Format(_T("insert into import_gdsection_info(gd_name, freq, reversedc_name, section_len, up_or_down, belong_station)\
								  values('%s', '%s','%s',%d, %d, '%s' )"), strGdName, strFreq, strReverseDc, iSectionLen, iUpOrDown, strBelongStation) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
			}	
			
			if (bCurRowIsBlank)
			{
				bNext = FALSE ;
			}
			else
			{
				iCurRow++ ;
			}

			acedRestoreStatusBar() ;

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
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	//将反向道岔有2个dc的比如113，111，变为111，113
	try
	{
		CString strSql ;
		CADORecordset cSetFind ;
		strSql.Format(_T("select * from import_gdsection_info where InStr(reversedc_name,',')>0")) ;
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
				CString strGdName = _T("") ;
				CString strReverse = _T("") ;
				cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
				int iFindMark = strGdName.Find(_T("-")) ;
				int iFindMark2 = strGdName.Find(_T("DG")) ;
				strReverse.Format(_T("%s,%s"), strGdName.Left(iFindMark), strGdName.Mid(iFindMark+1, iFindMark2-iFindMark-1) ) ;
				cSetFind.Edit() ;
				cSetFind.SetFieldValue(_T("reversedc_name"), strReverse) ;
				cSetFind.Update() ;

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
		AfxMessageBox(_T("unknown error in ImportGdSectionInfo")) ;

	}

	return iRet ;
}

int CLKDataTable::ImportSpeedInfo(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_speed")) ;

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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks() ;		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		pWorksheets  = m_pExcel->GetSheets() ;

		CString strBarMsg ;
		strBarMsg.Format(_T("读取线路速度表")) ;

		acedSetStatusBarProgressMeter(strBarMsg, 0, 3) ;	

		for (int iSheetIndex=0; iSheetIndex<4; iSheetIndex++)
		{

			acedSetStatusBarProgressMeterPos(iSheetIndex) ;

			VARIANT vt_index ;
			vt_index.vt = VT_I2 ;
			vt_index.iVal = iSheetIndex+1 ;			
			pSheet = pWorksheets ->GetItem( vt_index ) ;
			CString strSheetName  ;
			_bstr_t sheet_name =pSheet->GetName() ;
			strSheetName.Format(_T("%s"),(TCHAR*)sheet_name) ;
			int iUpOrDown = 0 ;
			int iIsReverse = 0 ;
			if (strSheetName.Find(_T("反向"))!=-1 )
			{
				iIsReverse = 1 ;
			}
			if (strSheetName.Find(_T("上行"))!=-1)
			{
				iUpOrDown = 1 ;
			}

			CString strColName[3] = {_T("B"),_T("C"),_T("D")} ;
			
			int iCurRow = 3 ;
			BOOL bNext = TRUE ;
			while (bNext)
			{
				CString strEndLoc = _T("");
				long iSpeed = 0, iLen = 0, iEndLocNum = 0;
				
				BOOL bCurRowIsBlank = TRUE ;
				
				BOOL bReadSomething = FALSE ;
				for (int iColIndex=0; iColIndex<3; iColIndex++)
				{

					CString strCellName = _T(""), strCellText = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					
					if (iColIndex==0)
					{
						if (strCellText==_T(""))
						{
							break ;
						}
						else
						{
							bReadSomething = TRUE ;
						}
					}				
					
					switch (iColIndex)
					{
					case 0:
						iSpeed = _tstoi(strCellText) ;
						break;
					case 1:					
						iLen = _tstoi(strCellText) ;
						break;
					case 2:
						strEndLoc = strCellText ;
						break;
			
					}
				}
				if (bReadSomething)
				{
					if (this->GetNumOfLoc(strEndLoc, iEndLocNum)==FALSE)
					{
						acutPrintf(_T("\n第%d个sheet%d行里程%s不符合格式"), iSheetIndex, iCurRow, strEndLoc) ;
					}


					strExec.Format(_T("insert into import_speed(speed, len, end_loc, end_loc_num, up_or_down, is_reverse)\
									  values(%d,%d,'%s',%d ,%d,%d)"), iSpeed, iLen, strEndLoc, iEndLocNum, iUpOrDown, iIsReverse ) ;
					ExecSqlString(strExec) ;
					bCurRowIsBlank = FALSE ;
					iRet++ ;
				}	
				
				if (bCurRowIsBlank)
				{
					bNext = FALSE ;
				}
				else
				{
					iCurRow++ ;
				}

			}	

		}

		acedRestoreStatusBar() ;
		

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
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return iRet ;
}

BOOL CLKDataTable::GetNumOfLoc(const CString strLoc, long &iLoc)
{
	BOOL bRet = TRUE ;

	CUserSystem mysys ;

	//显示里程转换为数值
	//if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]\\d+\\+\\d{3}$"), strLoc))
	if (CBlkUtility::IsStrMatch(_T("^\\w*\\D+\\d+\\+\\d{3}$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^\\w*\\D+(\\d+)\\+(\\d{3})$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^\\w*\\D+(\\d+)\\+(\\d{3})$"), _T("$2"), strLoc, strPart2) ;
		iLoc = _tstol(strPart1)*1000+_tstol(strPart2) ;
	}
	//else if (CBlkUtility::IsStrMatch(_T("^\\w*[k,K]-\\d+\\+-\\d{3}$"), strLoc))
	else if (CBlkUtility::IsStrMatch(_T("^\\w*\\D+-\\d+\\+-\\d{3}$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^\\w*\\D+-(\\d+)\\+-(\\d{3})$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^\\w*\\D+-(\\d+)\\+-(\\d{3})$"), _T("$2"), strLoc, strPart2) ;
		iLoc = 0-(_tstol(strPart1)*1000+_tstol(strPart2)) ;
	}
	else if (CBlkUtility::IsStrMatch(_T("^(\\d+)\\.*(\\d*)$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^(\\d+)\\.*(\\d*)$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^(\\d+)\\.*(\\d*)$"), _T("$2"), strLoc, strPart2) ;
		iLoc = _tstol(strPart1)*1000+_tstol(strPart2) ;
	}
	else if (CBlkUtility::IsStrMatch(_T("^-(\\d+)\\.*(\\d*)$"), strLoc))
	{
		CString strPart1 = _T("") , strPart2 = _T("") ;
		CBlkUtility::RegReplace(_T("^-(\\d+)\\.*(\\d*)$"), _T("$1"), strLoc, strPart1) ;
		CBlkUtility::RegReplace(_T("^-(\\d+)\\.*(\\d*)$"), _T("$2"), strLoc, strPart2) ;
		iLoc = 0-(_tstol(strPart1)*1000+_tstol(strPart2)) ;
	}
	else
	{
		iLoc = 0 ;
		bRet = FALSE ;
	}

	return bRet ;
}

int CLKDataTable::ImportDcInfo(const CString strXlsFileName)
{
	int iRet = 0 ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_dc_info")) ;
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

		//CString strColName[7] = {_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H")} ;
		CString strColName[7] = {_T("C"),_T("F"),_T("D"),_T("G"),_T("H"),_T("I"),_T("B")} ;
		
		int iCurRow = 3 ;
		BOOL bNext = TRUE ;
		while (bNext)
		{
			CString strDcName = _T(""), strLoc = _T(""), strBelongStation = _T("") ;
			long iLocNum = 0 ;
			int iDcSize = 18 ;
			int iDevNum = 5 ;
			int iUpOrDown = 0 ;
			int iSpeed = 0 ;			
			
			BOOL bCurRowIsBlank = TRUE ;
			CString strBarMsg ;
			strBarMsg.Format(_T("读取道岔信息表：第%d行"), iCurRow) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, 6) ;	

			
			BOOL bReadSomething = FALSE ;
			for (int iColIndex=0; iColIndex<7; iColIndex++)
			{

				acedSetStatusBarProgressMeterPos(iColIndex) ;

				CString strCellName = _T(""), strCellText = _T("") ;
				strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				strCellText = (CString)(pRange->GetText().bstrVal) ;
				strCellText.TrimLeft() ;
				strCellText.TrimRight() ;
				
				if (iColIndex==0)
				{
					if (strCellText==_T(""))
					{
						break ;
					}
					else
					{
						bReadSomething = TRUE ;
					}
				}				
				
				switch (iColIndex)
				{
				case 0:
					strDcName = strCellText ;
					break;
				case 1:
					//iUpOrDown = strCellText==_T("上行")?1:0 ;
					if (strCellText==_T("下行")||strCellText==_T("0")||strCellText==_T("X"))
					{
						iUpOrDown = 0 ;
					}
					else if (strCellText==_T("上行")||strCellText==_T("1")||strCellText==_T("S"))
					{
						iUpOrDown = 1 ;
					}
					else if (strCellText==_T("下行P")||strCellText==_T("10")||strCellText==_T("XP"))
					{
						iUpOrDown = 10 ;
					}
					else if (strCellText==_T("上行P")||strCellText==_T("11")||strCellText==_T("SP"))
					{
						iUpOrDown = 11 ;
					}
					else
					{
						iUpOrDown = -1 ;
					}		
					break;
				case 2:
					strLoc = strCellText ;
					break;
				case 3:
					iSpeed = _tstoi(strCellText) ;
					break;
				case 4:
					iDcSize = _tstoi(strCellText) ;
					break;
				case 5:
					iDevNum = _tstoi(strCellText) ;
					break;
				case 6:
					strBelongStation = strCellText ;
					break;
				}
			}
			if (bReadSomething)
			{	
				if (strLoc.Find(_T("+"))==-1)
				{
					iLocNum = _tstol(strLoc) ;
				}
				else if (this->GetNumOfLoc(strLoc, iLocNum)==FALSE)
				{
					acutPrintf(_T("\n第%d行里程%s不符合格式"), iCurRow, strLoc) ;
				}

				strExec.Format(_T("insert into import_dc_info(name, up_or_down, loc, loc_num, speed, dev_size, dev_num, belong_station)\
								  values('%s',%d,'%s',%d,%d,%d,%d, '%s' )"), strDcName, iUpOrDown, strLoc, iLocNum,  iSpeed,iDcSize, iDevNum, strBelongStation) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
			}	
			
			if (bCurRowIsBlank)
			{
				bNext = FALSE ;
			}
			else
			{
				iCurRow++ ;
			}

			acedRestoreStatusBar() ;

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
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return iRet ;

}

//计算进路信息表，结果回填import_compute_lsb表
void CLKDataTable::ComputeJLInfo()
{
	CUserSystem mysys ;
	CDBUtility::ClearTable(_T("xhj_gd")) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;	

		//先找出各进站信号机和第一个轨道，便于在发车进路时判断终端信号机
		strSql.Format(_T("select distinct startxhj_name,IIF(InStr(gd_list,',')>0,left(gd_list,InStr(gd_list,',')-1),gd_list) as first_gd from import_compute_lsb where InStr(to_or_from,'至')>0")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				CString strStartXhjName = _T("") ;
				CString strGd = _T("") ;
				cSet.GetFieldValue(_T("startxhj_name"), strStartXhjName) ;
				cSet.GetFieldValue(_T("first_gd"), strGd) ;

				strSql.Format(_T("insert into xhj_gd (xhj_name, gd_name) values('%s','%s')"), strStartXhjName, strGd) ;
				ExecSqlString(strSql) ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

		//begin 遍历所有进路，计算补充每条进路的信息
		strSql.Format(_T("select * from import_compute_lsb")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iJlCount = cSet.GetRecordCount() ;

			CString strBarMsg ;
			strBarMsg.Format(_T("处理%d条进路"), iJlCount) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iJlCount-1) ;	

			int iJlIndex = 0 ;
			while (!cSet.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iJlIndex) ;

				CString strToOrFrom = _T("") ;
				CString strButton = _T("") ;
				CString strStartXhj = _T("") ;
				CString strDcList = _T("") ;
				CString strGdList = _T("") ;
				cSet.GetFieldValue(_T("to_or_from"), strToOrFrom) ;
				cSet.GetFieldValue(_T("jl_button"), strButton) ;
				cSet.GetFieldValue(_T("startxhj_name"), strStartXhj) ;
				cSet.GetFieldValue(_T("dc_list"), strDcList) ;
				cSet.GetFieldValue(_T("gd_list"), strGdList) ;

				int iJlNum = 0 ;
				cSet.GetFieldValue(_T("jl_index"), iJlNum) ;
				if (iJlNum==33)
				{
					int iii = 0 ;
				}

				//至或者由的 股道名称
				CString strToOrFromGd = strToOrFrom ;
				CString strGdNum = _T("XX") ;
				int iLocFind = strToOrFrom.Find(_T("股")) ;
				if (iLocFind>=2)
				{
					strGdNum= strToOrFrom.Mid(2, iLocFind-2) ;
				}				 
				strToOrFromGd.Format(_T("%sG"), strGdNum) ;

				//begin 进路类型  终端信号机				
				
				//道岔都是定位
				BOOL bDcIsAllDw = TRUE ;
				if (strDcList.Find(_T("("))!=-1)
				{	
					CString strDcListTmp = strDcList ;
					CString strDcListTmpRs = strDcListTmp ;
					//复式交分道岔写法如108(106)/204
					CBlkUtility::RegReplace(_T("\\d+\\(\\d+\\)"),_T("AAA"), strDcListTmp, strDcListTmpRs ) ;
					if (strDcListTmpRs.Find(_T("("))!=-1)
					{
						bDcIsAllDw = FALSE ;
					}		
				}

				CString strJlType = _T("") ; //进路类型
				CString strStartXhj_Show = _T("-") ;//始端信号机显示
				CString strEndXhj = _T("") ;

				//起始信号机信息
				int iStartXhj_UpOrDown = -1 ;
				long iStartXhj_LocNum = 0 ;
				int iStartXhj_Type = 0 ;
				int iStartXhj_IsReverse = 0 ;
				if(this->GetXhjInfo(strStartXhj, iStartXhj_UpOrDown, iStartXhj_LocNum, iStartXhj_Type, iStartXhj_IsReverse)==0)
				{
					cSet.MoveNext() ;
					continue ;
				}

				//股道信息
				long iToOrFromGd_Len1 = 0 ;
				long iToOrFromGd_Len2 = 0 ;
				CString strToOrFromGd_Freq1 = _T("") ;
				CString strToOrFromGd_Freq2 = _T("") ;
				int iToOrFromGd_UpOrDown = -1 ;
				if(this->GetGdInfo(strToOrFromGd,  iToOrFromGd_Len1, iToOrFromGd_Len2, strToOrFromGd_Freq1, strToOrFromGd_Freq2, iToOrFromGd_UpOrDown)==0)
				{
					cSet.MoveNext() ;
					continue ;
				}

				//接车
				if (strToOrFrom.Left(2)==_T("至"))
				{
					if ( iStartXhj_UpOrDown==iToOrFromGd_UpOrDown && iStartXhj_IsReverse==0 && (iStartXhj_UpOrDown+iToOrFromGd_UpOrDown)>-1 && bDcIsAllDw )
					{
						strJlType = _T("正线接车") ;//1
					}
					else if ( iStartXhj_UpOrDown==iToOrFromGd_UpOrDown && iStartXhj_IsReverse==1 && (iStartXhj_UpOrDown+iToOrFromGd_UpOrDown)>-1 && bDcIsAllDw )
					{
						strJlType = _T("反向正线接车") ;//2
					}
					else if (iToOrFromGd_UpOrDown==-1)
					{
						strJlType = _T("侧向接车-1") ;//3
						if (iStartXhj_IsReverse)
						{
							strJlType = _T("反向侧向接车-1") ;//4
						}
					}
					else if (iToOrFromGd_UpOrDown==0&&strStartXhj.Left(1)==_T("X") || iToOrFromGd_UpOrDown==1&&strStartXhj.Left(1)==_T("S"))
					{
						strJlType = _T("侧向接车-2") ;//5
						if (iStartXhj_IsReverse)
						{
							strJlType = _T("反向侧向接车-2") ;//6
						}
					}
					else if (iToOrFromGd_UpOrDown==0&&strStartXhj.Left(1)==_T("S") || iToOrFromGd_UpOrDown==1&&strStartXhj.Left(1)==_T("X"))
					{
						strJlType = _T("侧向接车-3") ;//7
						if (iStartXhj_IsReverse)
						{
							strJlType = _T("反向侧向接车-3") ;//8
						}
					}

					if (strStartXhj.Left(1)==_T("X"))
					{
						strEndXhj.Format(_T("X%s"), strGdNum) ;
					}
					else if (strStartXhj.Left(1)==_T("S"))
					{
						strEndXhj.Format(_T("S%s"), strGdNum) ;
					}
				}
				//发车
				else if (strToOrFrom.Left(2)==_T("由"))
				{
					strJlType = _T("发车") ;

					strEndXhj = strButton.Mid(strButton.ReverseFind(_T(' '))+1) ;
					strEndXhj.TrimRight(_T("LA")) ;				

				}
				//end 进路类型 终端信号机

				//begin startxhj_show
				if (strJlType.Find(_T("侧向"))!=-1)
				{
					strStartXhj_Show = _T("UUS") ;
				}
				//end startxhj_show


				CADORecordset cSetFind ;

				//begin 应答器
				//begin 起始应答器编号
				CString strYdqSn = _T("-") ;
				long iStartYdqLoc = 0 ;
				
				strSql.Format(_T("select * from export_ydq where belong_station='%s' and dev_type='有源' and belong_xhj='%s' and right(useage,2) in ('JZ','CZ') order by id asc"), this->m_strCurStation, strStartXhj) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						CString strYdqSnTmp = _T("") ;
						cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
						cSetFind.GetFieldValue(_T("loc_num"), iStartYdqLoc) ;
						//strYdqSn = strYdqSnTmp.Left(strYdqSnTmp.ReverseFind(_T('-'))) ;
						strYdqSn = strYdqSnTmp ;
					}
					cSetFind.Close() ;
				}
				//end 起始应答器编号

				//接车才处理经过的应答器
				CString strPassYdqFinal = _T("-") ;

				if (strToOrFrom.Left(2)==_T("至") && strJlType.Find(_T("正线接车"))==-1)
				{
					CString strCzName1 = _T(""), strCzName2 = _T("") ;
					if (strStartXhj.Left(1)==_T("X"))
					{
						strCzName1.Format(_T("S%s"), strGdNum) ;
						strCzName2.Format(_T("X%s"), strGdNum) ;
					}
					else if (strStartXhj.Left(1)==_T("S"))
					{
						strCzName1.Format(_T("X%s"), strGdNum) ;
						strCzName2.Format(_T("S%s"), strGdNum) ;
					}

					//进站
					long iJzLoc = 0 ;
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and belong_xhj='%s' and right(useage,2)='JZ' order by id asc"), this->m_strCurStation, strStartXhj) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							cSetFind.GetFieldValue(_T("loc_num"), iJzLoc) ;
						}

						cSetFind.Close() ;
					}
					//出站1
					long iCzLoc1 = 0 ;
					CString strCzSn1 = _T("") ;
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and belong_xhj='%s' and right(useage,2)='CZ' order by id asc"), this->m_strCurStation, strCzName1) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							CString strYdqSnTmp = _T("") ;
							cSetFind.GetFieldValue(_T("loc_num"), iCzLoc1) ;
							cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
							strCzSn1 = strYdqSnTmp.Right(5).Left(3) ;
						}
						
						cSetFind.Close() ;
					}
					//定位
					CLongArray iArrayDwLoc ;
					CStringArray strArrayDwSn ;
					
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and left(belong_xhj,%d)='%sG' and right(useage,2)='DW' order by id asc"), this->m_strCurStation, strGdNum.GetLength()+1, strGdNum) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							cSetFind.MoveFirst() ;
							while (!cSetFind.IsEOF())
							{
								long iDwLoc = 0 ;
								CString strDwSn = _T("") ;
								CString strYdqSnTmp = _T("") ;
								cSetFind.GetFieldValue(_T("loc_num"), iDwLoc) ;
								cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
								strDwSn = strYdqSnTmp.Right(3) ;

								iArrayDwLoc.Add(iDwLoc) ;
								strArrayDwSn.Add(strDwSn) ;

								cSetFind.MoveNext() ;
							}

						}						
						cSetFind.Close() ;
					}
					//出站2
					long iCzLoc2 = 0 ;
					CString strCzSn2 = _T("") ;
					strSql.Format(_T("select * from export_ydq where belong_station='%s' and belong_xhj='%s' and right(useage,2)='CZ' order by id asc"), this->m_strCurStation, strCzName2) ;
					if (cSetFind.IsOpen())
					{
						cSetFind.Close() ;
					}
					if (cSetFind.Open(g_PtrCon, strSql))
					{
						if (cSetFind.GetRecordCount()>0)
						{
							CString strYdqSnTmp = _T("") ;
							cSetFind.GetFieldValue(_T("loc_num"), iCzLoc2) ;
							cSetFind.GetFieldValue(_T("sn"), strYdqSnTmp) ;
							strCzSn2 = strYdqSnTmp.Right(5).Left(3) ;
						}						
						cSetFind.Close() ;
					}

					strPassYdqFinal.Format(_T("%s/%d"), strCzSn1, abs(iCzLoc1-iJzLoc)) ;
					
					int iDwSum = iArrayDwLoc.GetSize() ;
					CString strPassYdqDw = _T("") ;
					long iPreLoc = iCzLoc1 ;
					for (int iDwIndex=0; iDwIndex<iDwSum; iDwIndex++)
					{
						CString strPassYdqDwTmp = _T("") ;
						strPassYdqDwTmp.Format(_T(",\n%s/%d"), strArrayDwSn.GetAt(iDwIndex), abs(int(iArrayDwLoc.GetAt(iDwIndex)-iPreLoc))) ;
						strPassYdqDw.Insert(555, strPassYdqDwTmp) ;
						iPreLoc = iArrayDwLoc.GetAt(iDwIndex) ;
					}

					strPassYdqFinal.Insert(555, strPassYdqDw) ;
					if (iDwSum>0)
					{
						CString strPassYdqCz2 = _T("") ;
						strPassYdqCz2.Format(_T(",\n%s/%d"), strCzSn2, abs(int(iCzLoc2-iArrayDwLoc.GetAt(iDwSum-1)))) ;
						strPassYdqFinal.Insert(555, strPassYdqCz2) ;
					}

					//strPassYdqFinal.Format(_T("%s/%d,\n%s/%d,\n%s/%d"), strCzSn1, abs(iCzLoc1-iJzLoc), strDwSn, abs(iDwLoc-iCzLoc1), strCzSn2, abs(iCzLoc2-iDwLoc)) ;
				}
				//end 应答器


				//begin dc_list_new
				CString	 strDcListNew = _T("") ;
				CStringArray strArrayDcList ;
				CBlkUtility::DivideString(strDcList, _T(","), strArrayDcList) ;
				for(int iDcIndex=0; iDcIndex<strArrayDcList.GetSize(); iDcIndex++)
				{
					CString strDc = strArrayDcList.GetAt(iDcIndex) ;
					strDc.TrimLeft() ;
					strDc.TrimRight() ;					
					if (strDc.Find(_T("{"))==-1&&strDc.Find(_T("}"))==-1&&strDc.Find(_T("["))==-1&&strDc.Find(_T("]"))==-1&&strDc!=_T(""))
					{
						if (iDcIndex==0)
						{
							strDcListNew = strDc ;
						}
						else
						{
							strDcListNew.Insert(555,_T(",")) ;
							strDcListNew.Insert(555, strDc) ;
						}
					}
				}				
				//end dc_list_new

				//begin gd_list
				CString strGdListNew = _T("") ;
				CStringArray strArrayGdList ;
				CBlkUtility::DivideString(strGdList, _T(","), strArrayGdList) ;
				for(int iStrIndex=0; iStrIndex<strArrayGdList.GetSize(); iStrIndex++)
				{
					CString strGd = strArrayGdList.GetAt(iStrIndex) ;
					strGd.TrimLeft() ;
					strGd.TrimRight() ;					
					if (strGd.Find(_T("<"))==-1&&strGd!=_T(""))
					{
						if (iStrIndex==0)
						{
							strGdListNew = strGd ;
						}
						else
						{
							strGdListNew.Insert(555,_T(",")) ;
							strGdListNew.Insert(555, strGd) ;
						}
					}
				}
				//重新将去掉 <> 空白字符 后的轨道区段list分隔到数组内 
				strArrayGdList.RemoveAll() ;
				CBlkUtility::DivideString(strGdListNew, _T(","), strArrayGdList) ;

				long iLenSum = 0 ; //所有区段累计长度
				CString strGdListFinal = _T("") ; //轨道区段最终输出
				int iGdIndex = 0 ;
				for (iGdIndex=0; iGdIndex<strArrayGdList.GetSize(); iGdIndex++)
				{
					CString strCurGdSection = _T("") ;

					CString strGd = strArrayGdList.GetAt(iGdIndex) ;
					long iGdLen = 0 ;
					long iLen1 = 0, iLen2 = 0 ;
					CString strFreqCase1 = _T("0") ;
					CString strFreqCase2 = _T("0") ;
					CString strXhjCase = _T("没有信号机") ;

					int iDcCase = 0 ;
					int iRsCount = 0 ;//this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreqCase) ;

					
					int iFindMark = strGd.Find(_T("-")) ;
					
					CString strReverseStr = _T("") ; //反位道岔字符串,查询import_gdsection_info时用
					if (strGd.Right(2)==_T("DG"))
					{
						if (iFindMark==-1)//112DG形式
						{
							iRsCount = this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreqCase1) ;
						}
						else //111-113DG形式
						{
							long iDcLoc1 = 0, iDcLoc2 = 0 ;
							CString strDcName1, strDcName2 ;
							iRsCount = this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreqCase1, iDcLoc1, iDcLoc2, strDcName1, strDcName2) ;
						} //else //111-113DG形式
					} //if (strGd.Right(2)==_T("DG"))
					else
					{
						int iUpOrDown = 0 ;						
						iRsCount = this->GetGdInfo(strGd, iLen1, iLen2, strFreqCase1, strFreqCase2, iUpOrDown) ;
						iGdLen = iLen1+iLen2 ;
					}

					iLenSum +=iGdLen ;		

					if (iGdIndex==strArrayGdList.GetSize()-1)
					{
						if (strJlType==_T("发车"))
						{
							strXhjCase = _T("出站口") ;
						}						 
					}

					//分割成G1 G2的情况
					if (iRsCount>1)
					{
						if (strStartXhj.Left(1)==_T("X"))//从左往右先遇到G1，反之先遇到G2
						{
							for (int i=0; i<2; i++)
							{
								CString strGdTmp ;
								strGdTmp.Format(_T("%s%d"), strGd, i+1) ;		
								if (i==1)
								{
									strXhjCase = _T("出站信号机") ;
								}
								CString strFreqCase =(i==0?strFreqCase1:strFreqCase2) ;
								long iGdLenTmp = (i==0?iLen1:iLen2) ;
								
								if (strGdListFinal==_T(""))
								{
									strGdListFinal.Format(_T("%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
								}
								else
								{
									CString strTmp = _T("") ;
									strTmp.Format(_T(", \n%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
									strGdListFinal.Insert(10000, strTmp) ;
								}							
							}
						}
						else
						{
							for (int i=0; i<2; i++)
							{
								CString strGdTmp ;
								strGdTmp.Format(_T("%s%d"), strGd, 2-i) ;		
								if (i==1)
								{
									strXhjCase = _T("出站信号机") ;
								}
								CString strFreqCase =(i==0?strFreqCase2:strFreqCase1) ;
								long iGdLenTmp = (i==0?iLen2:iLen1) ;
								
								if (strGdListFinal==_T(""))
								{
									strGdListFinal.Format(_T("%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
								}
								else
								{
									CString strTmp = _T("") ;
									strTmp.Format(_T(", \n%d\\%s\\%s\\%s"), iGdLenTmp, strFreqCase, strXhjCase, strGdTmp) ;
									strGdListFinal.Insert(10000, strTmp) ;
								}							
							}
						}

					}
					else
					{
						//if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*G$"), strGd))
						if (CBlkUtility::IsStrMatch(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G$"), strGd))
						{
							strXhjCase = _T("出站信号机") ;
						}						 
						if (strGdListFinal==_T(""))
						{
							strGdListFinal.Format(_T("%d\\%s\\%s\\%s"), iGdLen, strFreqCase1, strXhjCase, strGd) ;
						}
						else
						{
							CString strTmp = _T("") ;
							strTmp.Format(_T(", \n%d\\%s\\%s\\%s"), iGdLen, strFreqCase1, strXhjCase, strGd) ;
							strGdListFinal.Insert(10000, strTmp) ;
						}
					}

				} //for (int iGdIndex=0; iGdIndex<strArrayGdList.GetSize(); iGdIndex++)

// 				//如果有轨道频率是0，则其它所有频率全部置0
// 				if (CBlkUtility::IsStrMatch(_T("\\d+\\\\0\\\\.+\\\\.+"), strGdListFinal))
// 				{
// 					CString strGdListFinalTmp = _T("") ;
// 					CString strReg = _T("(\\d+)\\\\(\\d{4}-[1-2]|0)\\\\(.+\\\\.+)") ;
// 					CBlkUtility::RegReplace(strReg, _T("$1\\\\0\\\\$3"), strGdListFinal, strGdListFinalTmp) ;
// 					strGdListFinal = strGdListFinalTmp ;
// 				}


				//begin 为出北京南专有改动
// 				int iFindLoc = strGdListFinal.Find(_T("\\0\\")) ;
// 				if (iFindLoc!=-1)
// 				{
// 					CString strGdListFinalTmp = _T("") ;
// 					CString strReg = _T("\\d{4}-[1-2]") ;
// 					CBlkUtility::RegReplace(strReg, _T("0"), strGdListFinal, strGdListFinalTmp) ;
// 					strGdListFinal = strGdListFinalTmp ;
// 				}
				if (strJlType.Find(_T("侧向接车"))!=-1)
				{
					CString strGdListFinalTmp = _T("") ;
					CString strReg = _T("(\\d+)\\\\\\d{4}-[1-2]\\\\(.+\\\\.*[^IVX\\d]G)") ;
					CString strSub = _T("$1\\\\0\\\\$2") ;
					CBlkUtility::RegReplace(strReg, strSub, strGdListFinal, strGdListFinalTmp) ;
					strGdListFinal = strGdListFinalTmp ;
				}
				
				//end 为出北京南专有改动
				
				//end gd_list

				//begin speed

				CString strSpeedFinal = _T("") ; //速度描述字符串
				if (iJlNum==73)
				{
					int aa = 0 ;
				}

                int iOldInXOrS = iStartXhj_UpOrDown>1?-1:iStartXhj_UpOrDown ;
                long iCurSpeedStartLoc = iStartXhj_LocNum ;//当前速度的起始里程
                long iCurLoc = iStartXhj_LocNum ; //暂时已虹桥为例 X信号机 里程
                long iLenSumCurSpeed = 0 ; //当前段速度累计长度
                int iPlus = 1 ; //进路从左往右是里程增加iplus为1，反向为减iplus为-1
                //long iLenSumAllSpeed = 0 ;//所有速度总的累计长度
				CString strExpectDc = _T("") ; //前一个轨道跟本轨道哪个道岔链接（双动）
				
				if (strStartXhj.Left(1)==_T("X"))
				{
					iPlus = 1 ;
				}
				else if (strStartXhj.Left(1)==_T("S"))
				{
					iPlus = -1 ;
				}			
				
// 				if (iJlNum == 132)
// 				{
// 					int aaa = 0 ;
// 				}

                int iGdSum = strArrayGdList.GetSize() ;
                for (iGdIndex=0; iGdIndex<iGdSum; iGdIndex++)
                {
                    CString strGd = strArrayGdList.GetAt(iGdIndex) ;
                    long iGdLen = 0 ;
                    
                    //////////////////////////////////////////////////////////////////////////
                    if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*DG$"), strGd))//111DG形式
                    {
                        CString strDcName = strGd.Left(strGd.GetLength()-2) ;
                        
                        long iDcLoc = 0 ;//道岔的公里标                         
                        int iCurInXOrS = this->GetDcInfo(strDcName, iDcLoc) ;
                        int iDcCase = 0 ;
                        CString strFreq = _T("") ;
                        this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreq) ;
                        
                        if (iOldInXOrS==0&&iCurInXOrS==0) //x->x
                        {
                            if (iDcCase==0) //道岔在定位
                            {
                                iLenSumCurSpeed+=iGdLen ;
                            }
                            else if (iDcCase==1)//反位
                            {
                                int iLenStraight = (iDcLoc-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
                                iLenSumCurSpeed+=iLenStraight ;
                                //处理前期积累的速度长度,一个阶段结束
                                //此处用函数处理
                                //////////////////////////////////////////////////////////////////////////
//                                 CString strLenSumCurSpeed = _T("") ;
//                                 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
//                                 strSpeedFinal.Insert(555, _T(",")) ;
//                                 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;
								this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
                                //////////////////////////////////////////////////////////////////////////
                                
                                int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
                                iCurSpeedStartLoc = iDcLoc ;
                                iLenSumCurSpeed = iLenCurve ;
								this->GetOtherDc(strDcName, strExpectDc) ;
                            }
                            
                        }
                        else if (iOldInXOrS==0&&iCurInXOrS==1) //x->s
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
                            //此处用函数处理
                            //////////////////////////////////////////////////////////////////////////
//                             CString strLenSumCurSpeed = _T("") ;
//                             strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
//                             strSpeedFinal.Insert(555, _T(",")) ;
//                             strSpeedFinal.Insert(555, strLenSumCurSpeed) ;    
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        else if (iOldInXOrS==0&&iCurInXOrS==-1)//x->c
                        {
                            iLenSumCurSpeed+=iGdLen ;
                        }
                        else if (iOldInXOrS==1&&iCurInXOrS==1)//s->s
                        {
                            if (iDcCase==0) //道岔在定位
                            {
                                iLenSumCurSpeed+=iGdLen ;
                            }
                            else if (iDcCase==1)//反位
                            {
                                int iLenStraight = (iDcLoc-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
                                iLenSumCurSpeed+=iLenStraight ;
                                //处理前期积累的速度长度,一个阶段结束
                                //此处用函数处理
                                //////////////////////////////////////////////////////////////////////////
//                                 CString strLenSumCurSpeed = _T("") ;
//                                 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
//                                 strSpeedFinal.Insert(555, _T(",")) ;
//                                 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;                       
								this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
                                //////////////////////////////////////////////////////////////////////////
                                
                                int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
                                iCurSpeedStartLoc = iDcLoc ;
                                iLenSumCurSpeed = iLenCurve ;

								this->GetOtherDc(strDcName, strExpectDc) ;
                            }
                        }   
                        else if (iOldInXOrS==1&&iCurInXOrS==0)//s->x
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
                            //此处用函数处理
                            //////////////////////////////////////////////////////////////////////////
//                             CString strLenSumCurSpeed = _T("") ;
//                             strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
//                             strSpeedFinal.Insert(555, _T(",")) ;
//                             strSpeedFinal.Insert(555, strLenSumCurSpeed) ;    
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        else if (iOldInXOrS==1&&iCurInXOrS==-1)//s->c
                        {
                            iLenSumCurSpeed+=iGdLen ;
                        }
                        else if (iOldInXOrS==-1&&iCurInXOrS==-1)//c->c
                        {
                            iLenSumCurSpeed+=iGdLen ;
							if (iDcCase==1)//反位
							{
								this->GetOtherDc(strDcName, strExpectDc) ;
							}
                        }   
                        else if (iOldInXOrS==-1&&iCurInXOrS==0)//c->x
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
                            //此处用函数处理
                            //////////////////////////////////////////////////////////////////////////
//                             CString strLenSumCurSpeed = _T("") ;
//                             strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
//                             strSpeedFinal.Insert(555, _T(",")) ;
//                             strSpeedFinal.Insert(555, strLenSumCurSpeed) ;      
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        else if (iOldInXOrS==-1&&iCurInXOrS==1)//c->s
                        {
                            //道岔肯定是反位
                            int iLenCurve = (iDcLoc-iCurLoc)*iPlus ;
                            iLenSumCurSpeed+=iLenCurve ;
                            //处理前期积累的速度长度,一个阶段结束
                            //此处用函数处理
                            //////////////////////////////////////////////////////////////////////////
//                             CString strLenSumCurSpeed = _T("") ;
//                             strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
//                             strSpeedFinal.Insert(555, _T(",")) ;
//                             strSpeedFinal.Insert(555, strLenSumCurSpeed) ;   
							this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
                            //////////////////////////////////////////////////////////////////////////
                            
                            int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度
                            iCurSpeedStartLoc = iDcLoc ;
                            iLenSumCurSpeed = iLenStraight ;
                        }
                        
                        iOldInXOrS = iCurInXOrS ; 
                     } 
					 else if (CBlkUtility::IsStrMatch(_T("^[1-9]\\d*-[1-9]\\d*DG$"), strGd))//111-113DG形式
					 {
						 CString strDcName1, strDcName2 ;
						 CBlkUtility::RegReplace(_T("^([1-9]\\d*)-([1-9]\\d*)DG$"), _T("$1"), strGd, strDcName1) ;
						 CBlkUtility::RegReplace(_T("^([1-9]\\d*)-([1-9]\\d*)DG$"), _T("$2"), strGd, strDcName2) ;
						 
						 long iDcLoc1 = 0, iDcLoc2 =0 ;//道岔的公里标                            
						 int iCurInXOrS = this->GetDcInfo(strDcName1, iDcLoc1) ;
						 this->GetDcInfo(strDcName2, iDcLoc2) ;          
						 
						 int iDcCase = 0 ;
						 CString strFreq = _T("") ;
						 //this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreq) ;
						 this->GetGdInfo(strGd, strDcListNew, iDcCase, iGdLen, strFreq, iDcLoc1, iDcLoc2, strDcName1, strDcName2, iPlus==1?0:1) ;
						 
						 if (iOldInXOrS==0&&iCurInXOrS==0) //x->x
						 {
							 if (iDcCase==10) //道岔全在定位
							 {
								 iLenSumCurSpeed+=iGdLen ;
							 }
							 else if (iDcCase==12)//1反位2定位
							 {
								 int iLenStraight = (iDcLoc1-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc1 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName1, strExpectDc) ;
							 }
							 else if (iDcCase==11)//1定位2反位
							 {
								 int iLenStraight = (iDcLoc2-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc2 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
							 
						 }
						 else if (iOldInXOrS==0&&iCurInXOrS==1) //x->s
						 {
							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;   
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 //此处用函数处理
									 //////////////////////////////////////////////////////////////////////////
// 									 CString strLenSumCurSpeed = _T("") ;
// 									 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 									 strSpeedFinal.Insert(555, _T(",")) ;
// 									 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;  
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ;  
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								}
							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;  
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;

								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                         

							 }

						 }
						 else if (iOldInXOrS==0&&iCurInXOrS==-1)//x->c
						 {
							 iLenSumCurSpeed+=iGdLen ;
						 }
						 else if (iOldInXOrS==1&&iCurInXOrS==1)//s->s
						 {
							 if (iDcCase==10) //道岔全在定位
							 {
								 iLenSumCurSpeed+=iGdLen ;
							 }
							 else if (iDcCase==12)//1反位2定位
							 {
								 int iLenStraight = (iDcLoc1-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc1 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName1, strExpectDc) ;
							 }
							 else if (iDcCase==11)//1定位2反位
							 {
								 int iLenStraight = (iDcLoc2-iCurLoc)*iPlus ;//道岔反位时，从前一个绝缘节到道岔的距离
								 iLenSumCurSpeed+=iLenStraight ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 int iLenCurve = iGdLen-iLenStraight ; //道岔反位时，走弯的长度
								 iCurSpeedStartLoc = iDcLoc2 ;
								 iLenSumCurSpeed = iLenCurve ;

								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
							 
						 }   
						 else if (iOldInXOrS==1&&iCurInXOrS==0)//s->x
						 {
							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;    
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 //此处用函数处理
									 //////////////////////////////////////////////////////////////////////////
// 									 CString strLenSumCurSpeed = _T("") ;
// 									 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 									 strSpeedFinal.Insert(555, _T(",")) ;
// 									 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;  
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ; 
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								}

							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ; 
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;
								 
								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                      
							 }
						 }
						 else if (iOldInXOrS==1&&iCurInXOrS==-1)//s->c
						 {
							 iLenSumCurSpeed+=iGdLen ;                               
						 }
						 else if (iOldInXOrS==-1&&iCurInXOrS==-1)//c->c
						 {
							 iLenSumCurSpeed+=iGdLen ; 
							 if (iDcCase==12)//1反位2定位
							 {
								 this->GetOtherDc(strDcName1, strExpectDc) ;
							 }
							 else if (iDcCase==11)//1定位2反位
							 {
								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
							 else if (iDcCase==13)//1、2均反位
							 {
								 this->GetOtherDc(strDcName2, strExpectDc) ;
							 }
						 }   
						 else if (iOldInXOrS==-1&&iCurInXOrS==0)//c->x
						 {
							 //如果没有提供侧线道岔坐标，则给个默认的前后道岔值							
							 if (strExpectDc!=strDcName1&&strExpectDc!=strDcName2)
							 {								 
								 acutPrintf(_T("第%d条进路c->x时strExprctDc为空，注意检查!\n"), iJlNum) ;
								 
// 								 if (strToOrFrom.Left(2)==_T("至"))
// 								 {
// 									strExpectDc = strDcName1 ;		
// 								 }
// 								 else if (strToOrFrom.Left(2)==_T("由"))
// 								 {
// 									 strExpectDc = strDcName2 ;
// 								 }

								strExpectDc = strDcName1 ;

							 }
							 //////////////////////////////////////////////////////////////////////////

							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {	
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;    
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 //此处用函数处理
									 //////////////////////////////////////////////////////////////////////////
// 									 CString strLenSumCurSpeed = _T("") ;
// 									 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 									 strSpeedFinal.Insert(555, _T(",")) ;
// 									 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;   
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 0, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ;  
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								 }
							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;  
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;
								 
								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                         
								 
							 }

						 }
						 else if (iOldInXOrS==-1&&iCurInXOrS==1)//c->s
						 {
							 //如果没有提供侧线道岔坐标，则给个默认的前后道岔值
							 if (strExpectDc!=strDcName1&&strExpectDc!=strDcName2)
							 {
								 acutPrintf(_T("第%d条进路c->s时strExprctDc为空，注意检查!\n"), iJlNum) ;
// 								 if (strToOrFrom.Left(2)==_T("至"))
// 								 {
// 									 strExpectDc = strDcName1 ;		
// 								 }
// 								 else if (strToOrFrom.Left(2)==_T("由"))
// 								 {
// 									 strExpectDc = strDcName2 ;
// 								 }
								 strExpectDc = strDcName1 ;
							 }
							 //////////////////////////////////////////////////////////////////////////

							 //上一个道岔和dc1链接
							 if (strExpectDc==strDcName1)
							 {	
								 //道岔1肯定是反位
								 int iLenCurve = (iDcLoc1-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ; 
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 
								 iCurSpeedStartLoc = iDcLoc1 ;
								 
								 if (iDcCase==12) //反、定
								 {                                   
									 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
									 iLenSumCurSpeed = iLenStraight ;                                                                
								 }
								 else if (iDcCase==13) //反、反
								 {
									 int iLenStraight = (iDcLoc2-iDcLoc1)*iPlus ; //道岔反位时,直向长度                                  
									 iLenSumCurSpeed = iLenStraight ;
									 //处理前期积累的速度长度,一个阶段结束
									 //此处用函数处理
									 //////////////////////////////////////////////////////////////////////////
// 									 CString strLenSumCurSpeed = _T("") ;
// 									 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 									 strSpeedFinal.Insert(555, _T(",")) ;
// 									 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;  
									 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, 1, iPlus) ;
									 //////////////////////////////////////////////////////////////////////////
									 
									 iCurSpeedStartLoc = iDcLoc2 ;
									 iLenSumCurSpeed = iGdLen-iLenCurve-iLenStraight ;  
									 
									 this->GetOtherDc(strDcName2, strExpectDc) ;
								 }
							 }
							 else if (strExpectDc==strDcName2) //上一个道岔和dc2链接
							 {
								 //道岔2肯定是反位
								 int iLenCurve = (iDcLoc2-iCurLoc)*iPlus ;
								 iLenSumCurSpeed+=iLenCurve ;
								 //处理前期积累的速度长度,一个阶段结束
								 //此处用函数处理
								 //////////////////////////////////////////////////////////////////////////
// 								 CString strLenSumCurSpeed = _T("") ;
// 								 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 								 strSpeedFinal.Insert(555, _T(",")) ;
// 								 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;  
								 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, -1, iPlus) ;
								 //////////////////////////////////////////////////////////////////////////
								 iCurSpeedStartLoc = iDcLoc2 ;
								 
								 int iLenStraight = iGdLen-iLenCurve ; //道岔反位时,直向长度                                 
								 iLenSumCurSpeed = iLenStraight ;                                                       
							 }
						 }
						 
						 iOldInXOrS = iCurInXOrS ; 
                     }
					 else if (CBlkUtility::IsStrMatch(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-2]?$"), strGd))//3G IIG IG1形式
					 {
						 CString strFreq1 = _T("") ;
						 CString strFreq2 = _T("") ;
						 int iUpOrDown = -1 ;                
						 long iLen1 = 0, iLen2 = 0 ;
						 this->GetGdInfo(strGd,  iLen1, iLen2, strFreq1, strFreq2, iUpOrDown) ;

						 iGdLen = iLen1+iLen2 ;
						 
						 iLenSumCurSpeed+=iGdLen ;
						 
// 						 //处理前期积累的速度长度,一个阶段结束
// 						 //此处用函数处理
// 						 //////////////////////////////////////////////////////////////////////////
// 						 CString strLenSumCurSpeed = _T("") ;
// 						 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 						 strSpeedFinal.Insert(555, _T(",")) ;
// 						 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;                              
// 						 //////////////////////////////////////////////////////////////////////////                                                      
						 iOldInXOrS = iUpOrDown ; 
					 }
					 else
					 {
						 CString strFreq1 = _T("") ;
						 CString strFreq2 = _T("") ;
						 int iUpOrDown = -1 ;                    
						 long iLen1 = 0, iLen2 = 0 ;
						 this->GetGdInfo(strGd, iLen1, iLen2, strFreq1, strFreq2, iUpOrDown) ;
						 iGdLen = iLen1+iLen2 ;
						 
						 iLenSumCurSpeed+=iGdLen ;
						 
						 iOldInXOrS = iUpOrDown ;
					 }
					 
					 iCurLoc+=(iGdLen*iPlus) ;

					 if (iGdIndex==iGdSum-1)//最后一个轨道
					 {
						 //处理前期积累的速度长度,一个阶段结束
						 //此处用函数处理
						 //////////////////////////////////////////////////////////////////////////
// 						 CString strLenSumCurSpeed = _T("") ;
// 						 strLenSumCurSpeed.Format(_T("%d"), iLenSumCurSpeed) ;
// 						 strSpeedFinal.Insert(555, _T(",")) ;
// 						 strSpeedFinal.Insert(555, strLenSumCurSpeed) ;                              
						 ////////////////////////////////////////////////////////////////////////// 
						 this->GetSpeedStr(iCurSpeedStartLoc, iLenSumCurSpeed, strSpeedFinal, iOldInXOrS, iPlus) ;

					 }
				}

				//begin 合并速度

				//end 合并速度


				//end speed


				cSet.Edit() ;
				cSet.SetFieldValue(_T("jl_type"), strJlType) ;
				cSet.SetFieldValue(_T("startxhj_show_new"), strStartXhj_Show) ;
				cSet.SetFieldValue(_T("endxhj_name"), strEndXhj) ;
				cSet.SetFieldValue(_T("dc_list_new"), strDcListNew) ;
				cSet.SetFieldValue(_T("gd_section"), strGdListFinal) ;
				cSet.SetFieldValue(_T("gd_len_sum"), iLenSum) ;
				cSet.SetFieldValue(_T("line_speed"), strSpeedFinal) ;
				cSet.SetFieldValue(_T("ydq_sn"), strYdqSn) ;
				cSet.SetFieldValue(_T("pass_ydq"), strPassYdqFinal) ;
				

				cSet.Update() ;

				cSet.MoveNext() ;
				iJlIndex++ ;
			}


			acedRestoreStatusBar() ;

			cSet.Close() ;
			
		}
		//end 遍历所有进路，计算补充每条进路的信息



	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;	
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in ComputeJLInfo")) ;
		
	}

}

void CLKDataTable::ExportJlData(CString strSavePath)
{	
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	CString strTempletFile ;
	strTempletFile.Format(_T("%s\\support\\lkdata\\列车进路信息表.xls"), m_strSysBasePath) ;

	CUserSystem mysys ;

	_ApplicationPtr m_pExcel = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	_WorksheetPtr pSheet2 = NULL ;

	CString strMsg ;
	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( _T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return ;
		}
	
		pWorkBook = m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strTempletFile ) ;		
	
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;


		vt_index.iVal = 1 ;
		
		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		
		
		CString strWriteText = _T("") ;
		
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)_T("A1")) ;
		strWriteText.Format(_T("%s进路信息数据表"), this->m_strCurStation) ;
		pRange->PutFormulaR1C1((_variant_t)strWriteText) ;	
		
		CADORecordset cSetFind ;
		CString strSql ;
		strSql.Format(_T("select * from import_compute_lsb")) ;
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
			int iJlSum = cSetFind.GetRecordCount() ;
			CString strBarMsg ;
			strBarMsg.Format(_T("导出%d条进路"), iJlSum) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iJlSum) ;	

			int iJlIndex = 0 ;
			int iCurRow = 4 ;

			CString strFiledName[13] = {
				_T("id"),_T("ydq_sn"),_T("jl_index"),_T("to_or_from"),_T("jl_type"),_T("startxhj_name"),
				_T("startxhj_show_new"),_T("endxhj_name"),_T("pass_ydq"),_T("dc_list_new"),_T("line_speed"),_T("gd_section"),
				_T("gd_len_sum")
				} ;
			CString strColName[13] = {
				_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),
				_T("G"),_T("H"),_T("I"),_T("J"),_T("K"),_T("L"),
				_T("O")
				} ;

			while (!cSetFind.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iJlIndex) ;

				for (int i=0; i<13; i++)
				{
					CString strContent = _T("") ;
					cSetFind.GetFieldValue(strFiledName[i], strContent) ;

					CString strCellName = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[i], iCurRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					//pRange->PutNumberFormat((_variant_t)_T("@")) ;
					pRange->PutFormulaR1C1((_variant_t)strContent) ;	
					
				}
				
				iCurRow++ ;
				iJlIndex++ ;
				cSetFind.MoveNext() ;
			}
			CString strCellName1 = _T("A4"),strCellName2 = _T("") ;			
			strCellName2.Format(_T("O%d"), iCurRow) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName1, (_variant_t)strCellName2) ;
			pRange->PutHorizontalAlignment((_variant_t)(MSExcel::xlLeft*1.0)) ;
			pRange->PutVerticalAlignment((_variant_t)(MSExcel::xlCenter*1.0)) ;
			pRange = NULL ;
// 			strCellName2.Format(_T("L%d"), iCurRow) ;
// 			pRange = pSheet->GetRange((_variant_t)_T("L1"), (_variant_t)strCellName2) ;
// 			pRange->GetColumns()->AutoFit() ;

			cSetFind.Close() ;

			acedRestoreStatusBar() ;

		}	

		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;

		CString strSaveFileName ;
		strSaveFileName.Format(_T("%s\\进路信息数据表-%s.xls"), strSavePath, m_strCurStation) ;
	
		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;

		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));

		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}


}

void CLKDataTable::ExportYdqData(CString strSavePath)
{	
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	CString strTempletFile ;
	strTempletFile.Format(_T("%s\\support\\lkdata\\应答器位置表.xls"), m_strSysBasePath) ;

	CUserSystem mysys ;

	_ApplicationPtr m_pExcel = NULL ;
	//WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	_WorksheetPtr pSheet2 = NULL ;

	CString strMsg ;
	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( _T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return ;
		}
		
		pWorkBook = m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strTempletFile ) ;
		
		CString strCaseName[2] = {_T("上行线"),_T("下行线")} ;
		CString strOrder[2] = {_T("desc"),_T("asc")} ;
		
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;


		CString strColName[8] = {_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H")} ;

		CADORecordset cSetFind ;
		CString strSql ;

		for (int iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
		{
			vt_index.iVal = iSheetIndex+1 ;
			
			pWorksheets  = m_pExcel->GetSheets( ) ;
			pSheet = pWorksheets ->GetItem( vt_index ) ;			
			
			CString strWriteText = _T("") ;
			
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)_T("A1")) ;
			strWriteText.Format(_T("%s%s应答器位置表"), this->m_strXHTitle, strCaseName[iSheetIndex]) ;
			pRange->PutFormulaR1C1((_variant_t)strWriteText) ;	
			
			strSql.Format(_T("select * from export_ydq where up_or_down=%d order by plus2 %s,plus1 asc "), iSheetIndex==0?1:0, iSheetIndex==0?_T("desc"):_T("asc")) ;
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
				int iStationSum = cSetFind.GetRecordCount()-1 ;
				CString strBarMsg ;
				strBarMsg.Format(_T("列控数据之应答器位置表:%s"), strCaseName[iSheetIndex]) ;

				acedSetStatusBarProgressMeter(strBarMsg, 0, iStationSum) ;	

				int iStationIndex = 0 ;
				int iCurRow = 3 ;
				int iYdqIndex = 0 ;
				while (!cSetFind.IsEOF())
				{

					acedSetStatusBarProgressMeterPos(iStationIndex) ;


					CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
					
					iYdqIndex++ ;
					strContent[0].Format(_T("%d"), iYdqIndex) ;	
					cSetFind.GetFieldValue(_T("name"), strContent[1]) ;
					cSetFind.GetFieldValue(_T("sn"), strContent[2]) ;
					cSetFind.GetFieldValue(_T("loc"), strContent[3]) ;
					cSetFind.GetFieldValue(_T("dev_type"), strContent[4]) ;
					cSetFind.GetFieldValue(_T("useage"), strContent[5]) ;
					cSetFind.GetFieldValue(_T("comment"), strContent[6]) ;					
					cSetFind.GetFieldValue(_T("belong_station"), strContent[7]) ;
					strContent[7].Insert(555, _T("站")) ;
			
					for (int iColIndex=0; iColIndex<8; iColIndex++ )
					{
						CString strCellName = _T("") ;
						strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
						pRange = NULL ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						//pRange->PutNumberFormat((_variant_t)_T("@")) ;
						pRange->PutFormulaR1C1((_variant_t)strContent[iColIndex]) ;										
					}
					iCurRow++ ;		
					
					iStationIndex++ ;
					cSetFind.MoveNext() ;
				}
			cSetFind.Close() ;

			acedRestoreStatusBar() ;

			}
		}


		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;

		CString strSaveFileName ;
		strSaveFileName.Format(_T("%s\\%s应答器位置表.xls"), strSavePath, m_strXHTitle) ;
	
		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;

		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));

		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}

}

//绘制示意图
void CLKDataTable::DrawSyt()
{
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	//CString strMileSys = _T("DK") ;

	try
	{
		CString strSql ;
		CADORecordset cSetStation ;
		strSql.Format(_T("select * from compute_xhj_table where type in(1,11) order by loc_real asc")) ;
		if (cSetStation.IsOpen())
		{
			cSetStation.Close() ;
		}
		if (cSetStation.Open(g_PtrCon, strSql))
		{
			if (!cSetStation.IsBOF())
			{
				cSetStation.MoveFirst() ;
			}
			int iSectionSum = cSetStation.GetRecordCount()-1 ;
			int iSectionIndex = 0 ;
			double dCurY = 0 ;
			double dBetweenSection = 20 ;
			double dBetweenUpDown = 20 ;
			double dBetweenGd = 50 ;
			double dFrameHeight = 60 ;

			CString strBarMsg ;
			strBarMsg.Format(_T("绘制%d个区间"), iSectionSum) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iSectionSum-1) ;	

			while (!cSetStation.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iSectionIndex) ;

				long iPreStationRealLoc = 0 ;
				cSetStation.GetFieldValue(_T("loc_real"), iPreStationRealLoc) ;
				int iPreStationType = 0 ;
				cSetStation.GetFieldValue(_T("type"), iPreStationType) ;
				cSetStation.MoveNext() ;
				if (cSetStation.IsEOF())
				{
					break ;
				}
				long iBackStationRealLoc = 0 ;
				cSetStation.GetFieldValue(_T("loc_real"), iBackStationRealLoc) ;
				int iBackStationType = 0 ;
				cSetStation.GetFieldValue(_T("type"), iBackStationType) ;
				
				CADORecordset cSetFind ;
				

				int iRsMaxCount = 0 ;
				//begin 下行
				dCurY = iSectionIndex*(dFrameHeight+dBetweenSection) ; 				
				strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and right(name,1)<>'链' and (up_or_down=0 and type<>2 or type in(1,11)) order by loc_real asc"), iPreStationRealLoc, iBackStationRealLoc) ;
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
					int iRsCount = cSetFind.GetRecordCount() ;
					iRsMaxCount = iRsCount>iRsMaxCount?iRsCount:iRsMaxCount ;
					int iXhIndex = 0 ;
					double dCurX = 0 ;
					while (!cSetFind.IsEOF())
					{
						dCurX = iXhIndex*dBetweenGd ;
						
						CString strName = _T("") ;
						CString strLocNum = _T("") ;
						CString strFreq = _T("") ;
						CString strGdName = _T("") ;
						CString strYdqType = _T("") ;
						long iLocNum = 0 ;
						long iLen = 0 ;
						long iXhRealLoc = 0 ;
						int iIsDivide = 0 ;
						int iType = 0 ;
						int iUpOrDown = 0 ;
						int iIsReverse = 0 ;
						cSetFind.GetFieldValue(_T("name"), strName) ;
						cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
						cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;
						cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
						cSetFind.GetFieldValue(_T("len"), iLen) ;
						cSetFind.GetFieldValue(_T("loc_real"), iXhRealLoc) ;
						cSetFind.GetFieldValue(_T("is_divide"), iIsDivide) ;
						cSetFind.GetFieldValue(_T("type"), iType) ;
						cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
						cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
						cSetFind.GetFieldValue(_T("ydq_type"), strYdqType) ;
						this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;
						
						CString strLen ;
						strLen.Format(_T("%d"), iLen) ;
						
						ads_point pt_insert = {0,0,0};
						pt_insert[0] = dCurX ;
						pt_insert[1] = dCurY ;
						AcGePoint3d pt_3d_insert(0,0,0) ;

						AcDbBlockReference *pRef = NULL ;
						CStringArray strArrayAtt ;
						CString strBlockName = _T("") ;
						
						if (iType==1||iType==11)
						{
							mysys.Set_Layer(_T(""), _T("station")) ;
							pt_insert[1] = dCurY+(iUpOrDown==0?10:-10) ;
							//AcCmColor clr ;
							//clr.setColorIndex(4) ;
							//mysys.WriteText(pt_insert, strName, 0, AcDb::kTextCenter, 2, 0.7, clr) ;
							mysys.AddText(strName, pt_insert, pt_insert, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 4) ;
							pt_insert[1] -= 6 ;
							//mysys.WriteText(pt_insert, strLocNum, 0, AcDb::kTextCenter, 2, 0.7, clr) ;
							mysys.AddText(strLocNum, pt_insert, pt_insert, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 4) ;
							pt_insert[1]+=3 ;
							AcGePoint3d pt_3d_start(dCurX-3, pt_insert[1], 0) ;
							AcGePoint3d pt_3d_end(dCurX+3, pt_insert[1], 0) ;
							mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
						}
						else if (iType==0||iType==2||iType==3)
						{
							if (strName.Left(1)==_T("F"))
							{
								strBlockName = _T("syt_tg_x_f") ;
							}
							else if (iType==2)
							{
								strBlockName = _T("syt_cz_x") ;
								if (iUpOrDown==1)
								{
									strBlockName = _T("syt_cz_s") ;
									strName = _T("SI") ;
								}
							}
							else if (iType==3)
							{
								strBlockName = _T("syt_jz_x_z") ;
								if (iIsReverse==1)
								{
									strBlockName = _T("syt_jz_s_f") ;
								}
							}
							else
							{
								strBlockName = _T("syt_tg_x") ;
							}

							strArrayAtt.Add(strName) ;
							strArrayAtt.Add(strLocNum) ;
							mysys.Set_Layer(_T(""), _T("xhj")) ;
							mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert ) ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}

							//绘制freq len gdname
							if (iType==0||iIsReverse==1)
							{
								pt_3d_insert.x = dCurX+dBetweenGd/4 ;
								if (iType==3&&iIsReverse==1)
								{
									pt_3d_insert.x = dCurX+dBetweenGd/2+2 ;
								}
								strArrayAtt.RemoveAll();
								strArrayAtt.Add(strLen) ;
								strArrayAtt.Add(strFreq) ;
								//strGdName.Insert(555,_T("G")) ;
								strArrayAtt.Add(strGdName) ;
								strBlockName = _T("syt_freq_x") ;
								mysys.Set_Layer(_T(""), _T("freq")) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
								{
									pRef->close() ;
								}								
							}							
							//是否是分界
							if (iIsDivide==1)
							{
								pt_3d_insert.x = dCurX ;
								pt_3d_insert.y+=25 ;
								if (mysys.InsertBlock(pRef, _T("PMT_DIVIDE_0"), pt_3d_insert))
								{
									pRef->close() ;
								}	
							}
						}
						
						//此区段有无长短链
						cSetFind.MoveNext() ;
						long iNextXhRealLoc = 0 ; 
						if (!cSetFind.IsEOF())
						{
							cSetFind.GetFieldValue(_T("loc_real"), iNextXhRealLoc) ;

							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and right(name,1)='链' and up_or_down=0 order by loc_real asc"), iXhRealLoc, iNextXhRealLoc) ;
							CADORecordset cSetFindChain ;
							if (cSetFindChain.IsOpen())
							{
								cSetFindChain.Close() ;
							}
							if (cSetFindChain.Open(g_PtrCon, strSql))
							{
								if (cSetFindChain.GetRecordCount()>0)
								{
									cSetFindChain.MoveFirst() ;
									int iChainIndex=0 ;
									while (!cSetFindChain.IsEOF())
									{
										CString strName, strLocShow, strLoc ;
										long iChainLoc = 0, iChainLen = 0 ;
										cSetFindChain.GetFieldValue(_T("name"), strName) ;
										cSetFindChain.GetFieldValue(_T("loc_show"), strLocShow) ;
										cSetFindChain.GetFieldValue(_T("loc_num"), iChainLoc) ;
										cSetFindChain.GetFieldValue(_T("len"), iChainLen) ;
										this->GetLocShow(iChainLoc, m_strMileSysName, strLoc) ;
										strLocShow.Insert(555, _T("=")) ;
										strLocShow.Insert(555, strLoc) ;
										
										pt_3d_insert.x = dCurX+dBetweenGd/2+5*(iChainIndex+1) ;
										if (iType==3&&iIsReverse==1)
										{
											pt_3d_insert.x+=6 ;
										}
										pt_3d_insert.y = dCurY ;
										strArrayAtt.RemoveAll() ;
										strArrayAtt.Add(strLocShow) ;
										CString strChainLen ;
										strChainLen.Format(_T("%s=%d"), strName, iChainLen) ;
										strArrayAtt.Add(strChainLen) ;
										strBlockName = _T("syt_chain_x") ;
										mysys.Set_Layer(_T(""), _T("chain")) ;
										if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
										{
											pRef->close() ;
										}
										iChainIndex++ ;
										cSetFindChain.MoveNext() ;
									}
								}
								cSetFindChain.Close() ;
							}

						}	
						
						//绘制应答器
						mysys.Set_Layer(_T(""), _T("ydq")) ;

						pt_3d_insert.y = dCurY ;
						if (strYdqType==_T("JZ")||strYdqType==_T("FJZ"))
						{
							int iPlus = 1 ;
							if (strYdqType==_T("FJZ"))
							{
								iPlus = -1 ;
							}
							long iYdqLoc = iLocNum-30*iPlus-10*(iPlus==1?1:0) ;
							CString strYdqLoc = _T("") ;
							this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
							CString strYdqName =_T("") ;
							strYdqName.Format(_T("B%s-%s"), strName, strYdqType) ;
							strArrayAtt.RemoveAll() ;
							strArrayAtt.Add(strYdqName) ;
							strArrayAtt.Add(strYdqLoc) ;
							strBlockName = iPlus==1?_T("syt_ydq_jz_x"):_T("syt_ydq_fjz_s") ;
							pt_3d_insert.x = dCurX-(iPlus==1?10:4)*iPlus ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}

							iYdqLoc = iLocNum-250*iPlus ;
							this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;	
							CString strFName = _T("") ;
							this->GetFName(iYdqLoc, strFName, 0) ;							
							strYdqName.Format(_T("B%s-DW"), strFName.Mid(1)) ;
							strArrayAtt.RemoveAll() ;
							strArrayAtt.Add(strYdqName) ;
							strArrayAtt.Add(strYdqLoc) ;
							strBlockName = _T("syt_ydq_dw_x") ;
							pt_3d_insert.x = dCurX-19*iPlus ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}
						}
						else if (strYdqType==_T("Q")||strYdqType==_T("DW"))
						{
							long iYdqLoc = iLocNum-199-(strYdqType==_T("Q")?5:0)  ;
							CString strYdqLoc = _T("") ;
							this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
							CString strFName = _T("") ;
							this->GetFName(iYdqLoc, strFName, 0) ;
							CString strYdqName =_T("") ;
							strYdqName.Format(_T("B%s-%s"), strFName.Mid(1), strYdqType) ;
							strArrayAtt.RemoveAll() ;
							strArrayAtt.Add(strYdqName) ;
							strArrayAtt.Add(strYdqLoc) ;
							strBlockName = _T("syt_ydq_q_x") ;
							if (strYdqType==_T("DW"))
							{
								strBlockName = _T("syt_ydq_dw_x") ;
							}
							pt_3d_insert.x = dCurX-8 ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}
						}
						else if (strYdqType==_T("ZJ"))
						{
							for (int iZJIndex=0; iZJIndex<2; iZJIndex++)
							{
								long iYdqLoc = iLocNum-200*(iZJIndex+1)+1-10*(iZJIndex+1) ;
								CString strYdqLoc = _T("") ;
								this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
								CString strFName = _T("") ;
								this->GetFName(iYdqLoc, strFName, 0) ;
								CString strYdqName =_T("") ;
								strYdqName.Format(_T("B%s-%s"), strFName.Mid(1), strYdqType) ;
								strArrayAtt.RemoveAll() ;
								strArrayAtt.Add(strYdqName) ;
								strArrayAtt.Add(strYdqLoc) ;
								strBlockName = _T("syt_ydq_jz_x") ;
								pt_3d_insert.x = dCurX-10*(iZJIndex+1) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
								{
									pRef->close() ;
								}

							}

						}
						
						iXhIndex++ ;
						//cSetFind.MoveNext() ;
					}
					//轨道直线
					mysys.Set_Layer(_T(""), _T("gd_line")) ;
					AcGePoint3d pt_3d_start(0, dCurY, 0) ;
					AcGePoint3d pt_3d_end(dCurX, dCurY, 0) ;
					mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.4) ;

					cSetFind.Close() ;
				}
				//end 下行
				
				//begin 上行
				dCurY = iSectionIndex*(dFrameHeight+dBetweenSection)-dBetweenUpDown ; 	
				if (iBackStationRealLoc==59587)
				{
					int aaa = 0 ;
				}
				strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and right(name,1)<>'链' and (up_or_down=1 and type<>2 or type in(1,11)) order by loc_real desc"), iPreStationRealLoc, iBackStationRealLoc) ;
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
					int iRsCount = cSetFind.GetRecordCount() ;
					iRsMaxCount = iRsCount>iRsMaxCount?iRsCount:iRsMaxCount ;
					int iXhIndex = 0 ;
					double dCurX = 0 ;
					BOOL bPassFirstTg = FALSE ;
					while (!cSetFind.IsEOF())
					{
						dCurX = (iRsCount-iXhIndex-1)*dBetweenGd ;
						
						CString strName = _T("") ;
						CString strLocNum = _T("") ;
						CString strFreq = _T("") ;
						CString strGdName = _T("") ;
						CString strYdqType = _T("") ;
						long iLen = 0 ;
						long iLocNum = 0 ;
						long iXhRealLoc = 0 ;
						int iIsDivide = 0 ;
						int iType = 0 ;
						int iUpOrDown = 0 ;
						int iIsReverse = 0 ;
						cSetFind.GetFieldValue(_T("name"), strName) ;
						cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
						cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;	
						cSetFind.GetFieldValue(_T("len"), iLen) ;
						cSetFind.GetFieldValue(_T("loc_real"), iXhRealLoc) ;
						cSetFind.GetFieldValue(_T("is_divide"), iIsDivide) ;
						cSetFind.GetFieldValue(_T("type"), iType) ;
						cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
						cSetFind.GetFieldValue(_T("is_reverse"), iIsReverse) ;
						cSetFind.GetFieldValue(_T("ydq_type"), strYdqType) ;
						this->GetLocShow(iLocNum, m_strMileSysName, strLocNum) ;
						

						if (!bPassFirstTg)
						{
							if (iType==0)
							{
								if (iBackStationType==11)
								{						
									CADORecordset cSetFind2 ;
									strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down=1 and type=0 order by loc_real asc"), iBackStationRealLoc) ;
									if (cSetFind2.IsOpen())
									{
										cSetFind2.Close() ;
									}
									if (cSetFind2.Open(g_PtrCon, strSql))
									{
										if (cSetFind2.GetRecordCount()>0)
										{
											cSetFind2.MoveFirst() ;
											cSetFind2.GetFieldValue(_T("gd_name"), strGdName) ;
										}
										cSetFind2.Close() ;
									}
								}
								else
								{
									cSetFind.MovePrevious() ;
									cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
									cSetFind.MoveNext() ;
								}
								bPassFirstTg = TRUE ;
							}

						}
						else
						{
							cSetFind.MovePrevious() ;
							cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
							cSetFind.MoveNext() ;
						}				

						CString strLen ;
						strLen.Format(_T("%d"), iLen) ;
						
						ads_point pt_insert = {0,0,0};
						pt_insert[0] = dCurX ;
						pt_insert[1] = dCurY ;

						AcGePoint3d pt_3d_insert(0,0,0) ;
						AcDbBlockReference *pRef = NULL ;
						CStringArray strArrayAtt ;
						CString strBlockName = _T("") ;
						
						if (iType==1||iType==11)
						{
							mysys.Set_Layer(_T(""), _T("station")) ;
							pt_insert[1] = dCurY+(iUpOrDown==0?10:-10) ;
							//AcCmColor clr ;
							//clr.setColorIndex(4) ;
							//mysys.WriteText(pt_insert, strName, 0, AcDb::kTextCenter, 2, 0.7, clr) ;
							mysys.AddText(strName, pt_insert, pt_insert, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 4) ;
							pt_insert[1] -= 6 ;
							//mysys.WriteText(pt_insert, strLocNum, 0, AcDb::kTextCenter, 2, 0.7, clr) ;
							mysys.AddText(strLocNum, pt_insert, pt_insert, AcDb::kTextCenter, AcDb::kTextBase, 2, 0.7, 0, 4) ;
							pt_insert[1]+=3 ;
							AcGePoint3d pt_3d_start(dCurX-3, pt_insert[1], 0) ;
							AcGePoint3d pt_3d_end(dCurX+3, pt_insert[1], 0) ;
							mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;
						}
						else if (strName.Right(2)==_T("链")) //汉字要占2个字节
						{
							cSetFind.MoveNext() ;
							continue ;
						}
						else if (iType==0||iType==2||iType==3)
						{
							if (strName.Left(1)==_T("F"))
							{
								strBlockName = _T("syt_tg_s_f") ;
							}
							else if (iType==2)
							{
								strBlockName = _T("syt_cz_s") ;
								if (iUpOrDown==0)
								{
									strBlockName = _T("syt_cz_x") ;
									strName = _T("XII") ;
								}
							}
							else if (iType==3)
							{
								strBlockName = _T("syt_jz_s_z") ;
								if (iIsReverse==1)
								{
									strBlockName = _T("syt_jz_x_f") ;
								}
							}
							else
							{
								strBlockName = _T("syt_tg_s") ;
							}
							strArrayAtt.Add(strName) ;
							strArrayAtt.Add(strLocNum) ;
							mysys.Set_Layer(_T(""), _T("xhj")) ;
							mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert ) ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}

							//绘制freq len gdname
							if (iType==0||(iType==3&&iIsReverse==0))
							{								
								pt_3d_insert.x = dCurX+dBetweenGd/2;
// 								if (iType==3&&iIsReverse==1)
// 								{
// 									pt_3d_insert.x = dCurX-dBetweenGd/2-2 ;
// 								}
								strArrayAtt.RemoveAll();
								strArrayAtt.Add(strLen) ;
								strArrayAtt.Add(strFreq) ;
								//strGdName.Insert(555,_T("G")) ;
								strArrayAtt.Add(strGdName) ;
								strBlockName = _T("syt_freq_s") ;
								mysys.Set_Layer(_T(""), _T("freq")) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
								{
									pRef->close() ;
								}								
							}
							//是否是分界
							if (iIsDivide==1)
							{
								pt_3d_insert.x = dCurX ;
								pt_3d_insert.y-=25 ;
								if (mysys.InsertBlock(pRef, _T("PMT_DIVIDE_0"), pt_3d_insert))
								{
									pRef->setRotation(PI) ;
									pRef->close() ;
								}	
							}
						}
						
						//此区段有无长短链						
						cSetFind.MovePrevious() ;
						long iNextXhRealLoc = 0 ; 
						if (!cSetFind.IsBOF())
						{
							cSetFind.GetFieldValue(_T("loc_real"), iNextXhRealLoc) ;
							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and right(name,1)='链' and up_or_down=1 order by loc_real asc"), iXhRealLoc, iNextXhRealLoc) ;
							CADORecordset cSetFindChain ;
							if (cSetFindChain.IsOpen())
							{
								cSetFindChain.Close() ;
							}
							if (cSetFindChain.Open(g_PtrCon, strSql))
							{
								if (cSetFindChain.GetRecordCount()>0)
								{
									cSetFindChain.MoveFirst() ;
									int iChainIndex=0 ;
									while (!cSetFindChain.IsEOF())
									{
										CString strName, strLocShow, strLoc ;
										long iChainLoc = 0, iChainLen = 0 ;
										cSetFindChain.GetFieldValue(_T("name"), strName) ;
										cSetFindChain.GetFieldValue(_T("loc_show"), strLocShow) ;
										cSetFindChain.GetFieldValue(_T("loc_num"), iChainLoc) ;
										cSetFindChain.GetFieldValue(_T("len"), iChainLen) ;
										this->GetLocShow(iChainLoc, m_strMileSysName, strLoc) ;
										strLocShow.Insert(555, _T("=")) ;
										strLocShow.Insert(555, strLoc) ;
										
										pt_3d_insert.x = dCurX+dBetweenGd/2+5*(iChainIndex+1) ;
										if (iType==3&&iIsReverse==0)
										{
											pt_3d_insert.x+=6 ;
										}
										pt_3d_insert.y = dCurY ;
										strArrayAtt.RemoveAll() ;
										strArrayAtt.Add(strLocShow) ;
										CString strChainLen ;
										strChainLen.Format(_T("%s=%d"), strName, iChainLen) ;
										strArrayAtt.Add(strChainLen) ;
										strBlockName = _T("syt_chain_s") ;
										mysys.Set_Layer(_T(""), _T("chain")) ;
										if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
										{
											pRef->close() ;
										}
										iChainIndex++ ;
										cSetFindChain.MoveNext() ;
									}
								}
								cSetFindChain.Close() ;
							}

						}
						cSetFind.MoveNext() ;
						
						mysys.Set_Layer(_T(""), _T("ydq")) ;					
						//绘制应答器
						pt_3d_insert.y = dCurY ;
						if (strYdqType==_T("JZ")||strYdqType==_T("FJZ"))
						{
							int iPlus = 1 ;
							if (strYdqType==_T("FJZ"))
							{
								iPlus = -1 ;
							}

							long iYdqLoc = iLocNum+30*iPlus+10*(iPlus==1?1:0) ;
							CString strYdqLoc = _T("") ;
							this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
							CString strYdqName =_T("") ;							
							strYdqName.Format(_T("B%s-%s"), strName, strYdqType) ;
							strArrayAtt.RemoveAll() ;
							strArrayAtt.Add(strYdqName) ;
							strArrayAtt.Add(strYdqLoc) ;
							strBlockName = iPlus==1?_T("syt_ydq_jz_s"):_T("syt_ydq_fjz_x") ;
							pt_3d_insert.x = dCurX+(iPlus==1?10:4)*iPlus ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}
							
							iYdqLoc = iLocNum+250*iPlus ;
							this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
							CString strFName = _T("") ;
							this->GetFName(iYdqLoc, strFName, 1) ;							
							strYdqName.Format(_T("B%s-DW"), strFName.Mid(1)) ;
							strArrayAtt.RemoveAll() ;
							strArrayAtt.Add(strYdqName) ;
							strArrayAtt.Add(strYdqLoc) ;
							strBlockName = _T("syt_ydq_dw_s") ;
							pt_3d_insert.x = dCurX+19*iPlus ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}
							
						}
						else if (strYdqType==_T("Q")||strYdqType==_T("DW"))
						{
							long iYdqLoc = iLocNum+199+(strYdqType==_T("Q")?5:0) ;
							CString strYdqLoc = _T("") ;
							this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
							CString strFName = _T("") ;
							this->GetFName(iYdqLoc, strFName, 1) ;
							CString strYdqName =_T("") ;
							strYdqName.Format(_T("B%s-%s"), strFName.Mid(1), strYdqType) ;
							strArrayAtt.RemoveAll() ;
							strArrayAtt.Add(strYdqName) ;
							strArrayAtt.Add(strYdqLoc) ;
							strBlockName = _T("syt_ydq_q_s") ;
							if (strYdqType==_T("DW"))
							{
								strBlockName = _T("syt_ydq_dw_s") ;
							}
							pt_3d_insert.x = dCurX+8 ;
							if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
							{
								pRef->close() ;
							}
						}
						else if (strYdqType==_T("ZJ"))
						{
							for (int iZJIndex=0; iZJIndex<2; iZJIndex++)
							{								
								long iYdqLoc = iLocNum+200*(iZJIndex+1)-1+10*(iZJIndex+1) ;
								CString strYdqLoc = _T("") ;
								this->GetLocShow(iYdqLoc, m_strMileSysName, strYdqLoc) ;
								CString strFName = _T("") ;
								this->GetFName(iYdqLoc, strFName, 1) ;
								CString strYdqName =_T("") ;
								strYdqName.Format(_T("B%s-%s"), strFName.Mid(1), strYdqType) ;
								strArrayAtt.RemoveAll() ;
								strArrayAtt.Add(strYdqName) ;
								strArrayAtt.Add(strYdqLoc) ;
								strBlockName = _T("syt_ydq_jz_s") ;
								pt_3d_insert.x = dCurX+10*(iZJIndex+1) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayAtt,FALSE, 0))
								{
									pRef->close() ;
								}
								
							}
							
						}

						
						iXhIndex++ ;
						cSetFind.MoveNext() ;
					}

					//轨道直线
					mysys.Set_Layer(_T(""), _T("gd_line")) ;
					AcGePoint3d pt_3d_start(0, dCurY, 0) ;
					AcGePoint3d pt_3d_end((iRsCount-1)*dBetweenGd, dCurY, 0) ;
					mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.4) ;

					cSetFind.Close() ;
				}
				//end 上行

				//绘制本section的边框
				mysys.Set_Layer(_T(""), _T("frame")) ;
				double dX = (iRsMaxCount-1)*dBetweenGd+10;
				double dYTop = iSectionIndex*(dFrameHeight+dBetweenSection)+30;
				double dYBottom = iSectionIndex*(dFrameHeight+dBetweenSection)-dBetweenUpDown-30;
				ads_point pt_frame[4] = {
					{-10,dYTop,0},
					{dX,dYTop,0},
					{dX,dYBottom,0},
					{-10, dYBottom,0}
				} ; 
				AcGePoint3dArray ptArray ;
				AcGePoint3d pt_3d_frame(0,0,0) ;
				for (int i=0; i<4; i++)
				{
					pt_3d_frame.x = pt_frame[i][0] ;
					pt_3d_frame.y = pt_frame[i][1] ;
					ptArray.append(pt_3d_frame) ;
				}
				mysys.AddAcDb2dPolyline(ptArray,0.4,0) ;				

				iSectionIndex++ ;
				mysys.Set_Layer(_T(""), _T("0")) ;

			}
			cSetStation.Close() ;

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
		AfxMessageBox(_T("unknown error in DrawSyt")) ;

	}

}

//读取进出站信号机
int CLKDataTable::ImportJzCzXhjInfo(const CString strXlsFileName)
{
	int iRet = 0 ;

	CUserSystem mysys ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("import_xhj_info")) ;

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
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}
		
		pWorkBooks = m_pExcel->GetWorkbooks() ;		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		pWorksheets  = m_pExcel->GetSheets() ;
		
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;			
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		
		CString strColName[6] = {_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G")} ;
		
		int iCurRow = 3 ;
		BOOL bNext = TRUE ;
		CString strBarMsg = _T("") ;
		while (bNext)
		{
			CString strXHJName = _T("") ;
			CString strLoc = _T("");
			CString strBelongStation = _T("");
			int iUpOrDown = -1 ;
			long iLocNum = 0 ;
			int iXHJType = 0 ;
			int iIsReverse = 0 ;
			int iPlus1 = 0 ;
			
			BOOL bCurRowIsBlank = TRUE ;
			
			BOOL bReadSomething = FALSE ;
			strBarMsg.Format(_T("读取进出站信号机表：第%d行"), iCurRow) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, 4) ;	

			
			for (int iColIndex=0; iColIndex<6; iColIndex++)
			{

				acedSetStatusBarProgressMeterPos(iColIndex) ;

				
				CString strCellName = _T(""), strCellText = _T("") ;
				strCellName.Format(_T("%s%d"), strColName[iColIndex], iCurRow) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				strCellText = (CString)(pRange->GetText().bstrVal) ;
				strCellText.TrimLeft() ;
				strCellText.TrimRight() ;
				
				if (iColIndex==0)
				{
					if (strCellText==_T(""))
					{
						break ;
					}
					else
					{
						bReadSomething = TRUE ;
					}
				}				
				
				switch (iColIndex)
				{
				case 0:
					strXHJName = strCellText ;
					break;
				case 1:	
					if (strCellText==_T("下行")||strCellText==_T("0")||strCellText==_T("XZ"))
					{
						iUpOrDown = 0 ;
					}
					else if (strCellText==_T("上行")||strCellText==_T("1")||strCellText==_T("SZ"))
					{
						iUpOrDown = 1 ;
					}
					else if (strCellText==_T("下行侧向")||strCellText==_T("2")||strCellText==_T("XC"))
					{
						iUpOrDown = 2 ;
					}
					else if (strCellText==_T("上行侧向")||strCellText==_T("3")||strCellText==_T("SC"))
					{
						iUpOrDown = 3 ;
					}
// 					else //备用，例如现在写XDZ，认为是X行侧向,将来可能会认为是正向（三四线）
// 					{
// 						if(strCellText.Left(1)==_T("X")) 
// 						{
// 							iUpOrDown = 2 ;
// 						}
// 						else if (strCellText.Left(1)==_T("S"))
// 						{
// 							iUpOrDown = 3 ;
// 						}
// 					}
					else if (strCellText==_T("下行P")||strCellText==_T("10")||strCellText==_T("XZP"))
					{
						iUpOrDown = 10 ;
					}
					else if (strCellText==_T("上行P")||strCellText==_T("11")||strCellText==_T("SZP"))
					{
						iUpOrDown = 11 ;
					}

					break;
				case 2:
					strLoc = strCellText ;
					break;
				case 3:
					if (strCellText ==_T("进站")||strCellText==_T("3")||strCellText==_T("JZ"))
					{
						iXHJType = 3 ;
					}
					else if (strCellText ==_T("出站")||strCellText==_T("2")||strCellText==_T("CZ"))
					{
						iXHJType = 2 ;
					}
					else if (strCellText ==_T("股道")||strCellText==_T("GD"))
					{
						iXHJType = 100 ;
					}
					break;
				case 4:
					if (strCellText==_T("反向")||strCellText==_T("1"))
					{
						iIsReverse = 1 ;
					}
					else 
					{
						iIsReverse = 0 ;
					}
					break;
				case 5:
					strBelongStation = strCellText ;
					break;
					
				}
			}
			if (bReadSomething)
			{
				if (strLoc.Find(_T("+"))==-1)
				{
					iLocNum = _tstol(strLoc) ;
				}
				else if (this->GetNumOfLoc(strLoc, iLocNum)==FALSE)
				{
					acutPrintf(_T("\n第%d行里程%s不符合格式"), iCurRow, strLoc) ;
				}

				if (CBlkUtility::IsStrMatch(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-3]?$"), strXHJName))
				{
					CString strPart1 ;
					CString strPart1New = _T("") ;
					CBlkUtility::RegReplace(_T("^(((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))G[1-3]?$"), _T("$1"), strXHJName, strPart1) ;
					BOOL bConvertOk = this->ConvertSpecialChar(strPart1, strPart1New) ;
					iPlus1 = _tstoi(bConvertOk==TRUE?strPart1New:strPart1) ;						
				}
				else if (CBlkUtility::IsStrMatch(_T("^[S,X](((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))$"), strXHJName))
				{
					CString strPart1 ;
					CString strPart1New = _T("") ;
					CBlkUtility::RegReplace(_T("^[S,X](((XC|XL|L?X{0,3})(IX|IV|V?I{0,3}))|([1-9]\\d*))$"), _T("$1"), strXHJName, strPart1) ;
					BOOL bConvertOk = this->ConvertSpecialChar(strPart1, strPart1New) ;
					iPlus1 = _tstoi(bConvertOk==TRUE?strPart1New:strPart1) ;
				}						

				strExec.Format(_T("insert into import_xhj_info(name, up_or_down, loc, loc_num, type, is_reverse, belong_station, plus1)\
								  values('%s',%d,'%s',%d , %d, %d,'%s', %d)"), strXHJName, iUpOrDown, strLoc, iLocNum, iXHJType, iIsReverse, strBelongStation, iPlus1) ;
				ExecSqlString(strExec) ;
				bCurRowIsBlank = FALSE ;
				iRet++ ;
			}	
			
			if (bCurRowIsBlank)
			{
				bNext = FALSE ;
			}
			else
			{
				iCurRow++ ;
			}

			acedRestoreStatusBar() ;

			
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
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	
	
	return iRet ;
}

//将站内信号点插入compute_xhj_table
void CLKDataTable::InsertZnXhData()
{
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select distinct belong_station from import_gdsection_info")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while (!cSet.IsEOF())
			{
				CString strBelongStation = _T("") ;
				cSet.GetFieldValue(_T("belong_station"), strBelongStation) ;

				if (strBelongStation==_T("德州东"))
				{
					int ii = 0 ;
				}

				CADORecordset cSetFind ;
				//begin 下行
				//begin 查询进站信号机的公里标
				long iJzLocReal = 0 ;
				long iJzLocNum = 0 ;
				strSql.Format(_T("select * from compute_xhj_table where loc_real<(select loc_real from compute_xhj_table where name='%s') and up_or_down=0 and type=3 and is_reverse=0 order by loc_real desc"), strBelongStation) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						cSetFind.GetFieldValue(_T("loc_real"), iJzLocReal) ;
						cSetFind.GetFieldValue(_T("loc_num"), iJzLocNum) ;
					}
					cSetFind.Close() ;
				}
				//end 查询进站信号机的公里标
				//begin 查询反向进站信号机的公里标
				long iJzLocReal_F = 0 ;
				long iJzLocNum_F = 0 ;
				strSql.Format(_T("select * from compute_xhj_table where loc_real>(select loc_real from compute_xhj_table where name='%s') and up_or_down=0 and type=3 and is_reverse=1 order by loc_real asc"), strBelongStation) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						cSetFind.GetFieldValue(_T("loc_real"), iJzLocReal_F) ;
						cSetFind.GetFieldValue(_T("loc_num"), iJzLocNum_F) ;
					}
					cSetFind.Close() ;
				}
				//end 查询反向进站信号机的公里标

				//begin 遍历所有站内正线轨道，将其写入compute_xhj_table			
				strSql.Format(_T("select * from import_gdsection_info where belong_station='%s' and up_or_down=0 order by id asc"), strBelongStation) ;
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
					long iOldRealLoc = iJzLocReal ;
					long iOldLocNum = iJzLocNum ;
					int iIndex = 0 ;
					int iRsCount = cSetFind.GetRecordCount() ;
					while (!cSetFind.IsEOF())
					{
						CString strGdName = _T(""), strFreq = _T("") ;
						long iGdLen = 0 ;						
						cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
						cSetFind.GetFieldValue(_T("freq"), strFreq) ;
						cSetFind.GetFieldValue(_T("section_len"), iGdLen) ;

						long iCurRealLoc = iOldRealLoc+iGdLen ;	
						long iCurLocNum = iOldLocNum+iGdLen ;

						//////////////////////////////////////////////////////////////////////////
						//此区段有无长短链
						CADORecordset cSetFindChain ;
						long iChainRealLoc = 0  ;
						long iChainLen = 0 ;
						strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and right(name,1)='链' order by loc_real asc"), iOldRealLoc, iCurRealLoc) ;
						if (cSetFindChain.IsOpen())
						{
							cSetFindChain.Close() ;
						}
						if (cSetFindChain.Open(g_PtrCon, strSql))
						{
							if (cSetFindChain.GetRecordCount()>0)
							{
								CString strChainName = _T("") ;
								cSetFindChain.GetFieldValue(_T("loc_real"), iChainRealLoc) ;
								cSetFindChain.GetFieldValue(_T("len"), iChainLen) ;
								cSetFindChain.GetFieldValue(_T("name"), strChainName) ;
								if (strChainName==_T("短链"))
								{
									iChainLen = 0-iChainLen ;
								}
							}
							cSetFindChain.Close() ;
						}
						//////////////////////////////////////////////////////////////////////////
						
						if (iChainRealLoc>iOldRealLoc && iChainRealLoc<iCurRealLoc)
						{
							iCurLocNum = iCurLocNum - iChainLen ;
						}	
						
						if (iIndex==0)
						{
							strSql.Format(_T("update compute_xhj_table set len=%d, set_freq='%s', gd_name='%s' where loc_real=%d and up_or_down=0 and type=3 and is_reverse=0"),iGdLen, strFreq, strGdName, iJzLocReal) ;
							ExecSqlString(strSql) ;							
						}
						else
						{
							//是否是出站信号机	
							CString strXHJName = _T("") ;
							CADORecordset cSetFindCz ;
							strSql.Format(_T("select * from import_xhj_info where loc_num=%d and up_or_down=%d and type=2 "), iOldLocNum, 0 ) ;
							if (cSetFindCz.IsOpen())
							{
								cSetFindCz.Close() ;
							}
							if (cSetFindCz.Open(g_PtrCon, strSql))
							{
								if (cSetFindCz.GetRecordCount()>0)
								{
									cSetFindCz.MoveFirst() ;
									cSetFindCz.GetFieldValue(_T("name"), strXHJName) ;			
								}
								
								cSetFindCz.Close() ;
							}	
							strSql.Format(_T("insert into compute_xhj_table(name, loc_num,loc_real, len, up_or_down, set_freq, type, gd_name) values('%s', %d, %d, %d, %d, '%s',%d,'%s')"), strXHJName, iOldLocNum, iOldRealLoc, iGdLen, 0, strFreq, 4, strGdName) ;
							ExecSqlString(strSql) ;
						}
						
// 						if (iIndex==iRsCount-1)
// 						{
// 							strSql.Format(_T("update compute_xhj_table set gd_name='%s' where loc_real=%d"), strGdName, iJzLocReal) ;
// 							ExecSqlString(strSql) ;				
// 						}


						iOldRealLoc = iCurRealLoc ;
						iOldLocNum = iCurLocNum ;
						iIndex++ ;
						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}

				//end 遍历所有站内正线轨道，将其写入compute_xhj_table

				//end 下行

				//begin 上行
				//begin 查询反向进站信号机的公里标
				iJzLocReal_F = 0 ;
				iJzLocNum_F = 0 ;
				strSql.Format(_T("select * from compute_xhj_table where loc_real<(select loc_real from compute_xhj_table where name='%s') and up_or_down=1 and type=3 and is_reverse=1 order by loc_real desc"), strBelongStation) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						cSetFind.GetFieldValue(_T("loc_real"), iJzLocReal_F) ;
						cSetFind.GetFieldValue(_T("loc_num"), iJzLocNum_F) ;
					}
					cSetFind.Close() ;
				}
				//end 查询反向进站信号机的公里标
				//begin 查询进站信号机的公里标
				iJzLocReal = 0 ;
				iJzLocNum = 0 ;
				strSql.Format(_T("select * from compute_xhj_table where loc_real>(select loc_real from compute_xhj_table where name='%s') and up_or_down=1 and type=3 and is_reverse=0 order by loc_real asc"), strBelongStation) ;
				if (cSetFind.IsOpen())
				{
					cSetFind.Close() ;
				}
				if (cSetFind.Open(g_PtrCon, strSql))
				{
					if (cSetFind.GetRecordCount()>0)
					{
						cSetFind.MoveFirst() ;
						cSetFind.GetFieldValue(_T("loc_real"), iJzLocReal) ;
						cSetFind.GetFieldValue(_T("loc_num"), iJzLocNum) ;
					}
					cSetFind.Close() ;
				}
				//end 查询进站信号机的公里标

				//begin 遍历所有站内正线轨道，将其写入compute_xhj_table			
				strSql.Format(_T("select * from import_gdsection_info where belong_station='%s' and up_or_down=1 order by id asc"), strBelongStation) ;
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
					long iOldRealLoc = iJzLocReal_F ;
					long iOldLocNum = iJzLocNum_F ;
					CString strOldGdName = _T("") ;
					int iIndex = 0 ;
					int iRsCount = cSetFind.GetRecordCount() ;
					while (!cSetFind.IsEOF())
					{
						CString strGdName = _T(""), strFreq = _T("") ;
						long iGdLen = 0 ;						
						cSetFind.GetFieldValue(_T("gd_name"), strGdName) ;
						cSetFind.GetFieldValue(_T("freq"), strFreq) ;
						cSetFind.GetFieldValue(_T("section_len"), iGdLen) ;

						long iCurRealLoc = iOldRealLoc+iGdLen ;	
						long iCurLocNum = iOldLocNum+iGdLen ;

						//////////////////////////////////////////////////////////////////////////
						//此区段有无长短链
						CADORecordset cSetFindChain ;
						long iChainRealLoc = 0  ;
						long iChainLen = 0 ;
						strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and right(name,1)='链' order by loc_real asc"), iOldRealLoc, iCurRealLoc) ;
						if (cSetFindChain.IsOpen())
						{
							cSetFindChain.Close() ;
						}
						if (cSetFindChain.Open(g_PtrCon, strSql))
						{
							if (cSetFindChain.GetRecordCount()>0)
							{
								CString strChainName = _T("") ;
								cSetFindChain.GetFieldValue(_T("loc_real"), iChainRealLoc) ;
								cSetFindChain.GetFieldValue(_T("len"), iChainLen) ;
								cSetFindChain.GetFieldValue(_T("name"), strChainName) ;
								if (strChainName==_T("短链"))
								{
									iChainLen = 0-iChainLen ;
								}
							}
							cSetFindChain.Close() ;
						}
						//////////////////////////////////////////////////////////////////////////
						
						if (iChainRealLoc>iOldRealLoc && iChainRealLoc<iCurRealLoc)
						{
							iCurLocNum = iCurLocNum - iChainLen ;
						}	
						
						if (iIndex==0)
						{
							strSql.Format(_T("update compute_xhj_table set len=%d, set_freq='%s' where loc_real=%d and up_or_down=1 and type=3 and is_reverse=1"),iGdLen, strFreq, iJzLocReal_F) ;
							ExecSqlString(strSql) ;							
						}
						else
						{
							//是否是出站信号机	
							CString strXHJName = _T("") ;
							CADORecordset cSetFindCz ;
							strSql.Format(_T("select * from import_xhj_info where loc_num=%d and up_or_down=%d and type=2 "), iOldLocNum, 1 ) ;
							if (cSetFindCz.IsOpen())
							{
								cSetFindCz.Close() ;
							}
							if (cSetFindCz.Open(g_PtrCon, strSql))
							{
								if (cSetFindCz.GetRecordCount()>0)
								{
									cSetFindCz.MoveFirst() ;
									cSetFindCz.GetFieldValue(_T("name"), strXHJName) ;			
								}
								
								cSetFindCz.Close() ;
							}
							strSql.Format(_T("insert into compute_xhj_table(name, loc_num,loc_real, len, up_or_down, set_freq, type, gd_name) values('%s', %d, %d, %d, %d, '%s',%d,'%s')"), strXHJName, iOldLocNum, iOldRealLoc, iGdLen, 1, strFreq, 4, strOldGdName) ;
							ExecSqlString(strSql) ;			
						}

						if (iIndex==iRsCount-1)
						{
							strSql.Format(_T("update compute_xhj_table set gd_name='%s' where loc_real=%d and up_or_down=1 and type=3 and is_reverse=0"), strGdName, iJzLocReal) ;
							ExecSqlString(strSql) ;				
						}

						iOldRealLoc = iCurRealLoc ;
						iOldLocNum = iCurLocNum ;
						strOldGdName = strGdName ;
						iIndex++ ;
						cSetFind.MoveNext() ;
					}
					cSetFind.Close() ;
				}
				//end 遍历所有站内正线轨道，将其写入compute_xhj_table

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
		AfxMessageBox(_T("unknown error in InsertZnXhData")) ;

	}

}

void CLKDataTable::ComputeYdqData()
{
	//CString strMileSys = _T("DK") ;
	
	CUserSystem mysys ;
	
	//清除old数据
	CDBUtility::ClearTable(_T("export_ydq")) ;

	//begin test 
// 	this->m_strBigRegionSn = _T("028") ;
// 	this->m_strSmallRegionSn = _T("1") ;	
	//end test
	
	CString strCaseName[2] = {_T("上行线"),_T("下行线")} ;
	CString strOrder[2] = {_T("desc"),_T("asc")} ;
	
	CADORecordset cSetFind ;
	CString strSql ;

	for (int iSheetIndex=0; iSheetIndex<2; iSheetIndex++)
	{	
		strSql.Format(_T("select * from compute_xhj_table where type=1 or type=11 order by loc_real %s"), strOrder[iSheetIndex] ) ;
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
			int iStationSum = cSetFind.GetRecordCount()-1 ;
			CString strBarMsg ;
			strBarMsg.Format(_T("列控数据之应答器位置表：%d个站:%s"), iStationSum, strCaseName[iSheetIndex]) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iStationSum) ;	

			int iStationIndex = 0 ;
			int iCurRow = 3 ;
			int iYdqIndex = 0 ;
			
			while (!cSetFind.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iStationIndex) ;

				CString strStationName = _T("") ;
				cSetFind.GetFieldValue(_T("name"), strStationName) ;
				long iStationRealLoc = 0 ;
				cSetFind.GetFieldValue(_T("loc_real"), iStationRealLoc) ;
				int iStationType = 0 ;
				cSetFind.GetFieldValue(_T("type"), iStationType) ;

				this->m_strCurStation = strStationName ;

// 				if (strStationName==_T("常州北"))
// 				{
// 					int ii = 0 ;
// 				}

				//通过站名来查询大区编号、小区编号、车站编号
				this->GetStationInfo(this->m_strCurStation, m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn) ;
				
				int iYdqGroupSn = iSheetIndex==0?2:1 ;//应答器组编号
			
				CADORecordset cSetFind2 ; 
				//分界点坐标
				long iLeftDivideRealLoc = iStationRealLoc ;
				long iRightDivideRealLoc = iStationRealLoc ;
				for (int iDivideIndex=0; iDivideIndex<2; iDivideIndex++)
				{
					if (iDivideIndex==0)
					{
						strSql.Format(_T("select * from compute_xhj_table where loc_real<%d and up_or_down =%d and is_divide=1 order by loc_real desc"), iStationRealLoc, iSheetIndex==0?1:0) ;
					}
					else
					{
						strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and up_or_down =%d and is_divide=1 order by loc_real asc"), iStationRealLoc, iSheetIndex==0?1:0) ;
					}
					if (cSetFind2.IsOpen())
					{
						cSetFind2.Close() ;
					}
					if (cSetFind2.Open(g_PtrCon, strSql))
					{
						if (cSetFind2.GetRecordCount()>0)
						{
							cSetFind2.MoveFirst() ;
							if (iDivideIndex==0)
							{
								cSetFind2.GetFieldValue(_T("loc_real"), iLeftDivideRealLoc) ;
							}
							else
							{
								cSetFind2.GetFieldValue(_T("loc_real"), iRightDivideRealLoc) ;
							}
						}
						cSetFind2.Close() ;
					}
				}
				
				//如果是中继站，则找出离中继站最近的信号机
				CString strZjzNearestXhj = _T("") ;
				long iNearestLen = 10000 ;
				if (iStationType==11)
				{
					for (int iLOR=0; iLOR<2; iLOR++)
					{
						if (iLOR==0)
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<%d and type=0 and left(name,1)<>'F' and up_or_down=%d order by loc_real desc"), iLeftDivideRealLoc, iStationRealLoc, iSheetIndex==0?1:0) ;
						}
						else
						{
							strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<=%d and type=0 and left(name,1)<>'F' and up_or_down=%d order by loc_real asc"), iStationRealLoc, iRightDivideRealLoc, iSheetIndex==0?1:0) ;
						}
						if (cSetFind2.IsOpen())
						{
							cSetFind2.Close() ;
						}
						if (cSetFind2.Open(g_PtrCon, strSql))
						{
							if (cSetFind2.GetRecordCount()>0)
							{
								long iRealLocTmp = 0 ;
								cSetFind2.MoveFirst() ;
								cSetFind2.GetFieldValue(_T("loc_real"), iRealLocTmp) ;
								if (abs(iRealLocTmp-iStationRealLoc)<iNearestLen)
								{
									iNearestLen = abs(iRealLocTmp-iStationRealLoc) ;
									cSetFind2.GetFieldValue(_T("name"), strZjzNearestXhj) ;
								}								 
								
							}
							cSetFind2.Close() ;
						}
					}					
				}
				
				//遍历每个信号点
				strSql.Format(_T("select * from compute_xhj_table where loc_real>=%d and loc_real<=%d and up_or_down=%d and right(name,1)<>'链' and left(name,1)<>'F' and type not in(1,11,2) order by loc_real %s"), iLeftDivideRealLoc, iRightDivideRealLoc, iSheetIndex==0?1:0, strOrder[iSheetIndex]) ;
				if (cSetFind2.IsOpen())
				{
					cSetFind2.Close() ;
				}
				if (cSetFind2.Open(g_PtrCon,strSql))
				{
					if (!cSetFind2.IsBOF())
					{
						cSetFind2.MoveFirst() ;
					}
					int iRsCount = cSetFind2.GetRecordCount() ;
					int iXhjIndex2 = 0 ; //如果在进站分界，本站是中继站，则第一个信号点是SN，舍弃
					while (!cSetFind2.IsEOF())
					{
						CString strXHJName = _T("") ;
						long iXhjID = 0 ;
						int iIsReverse = -1 ;
						int iIsDivide = 0 ;
						int iType = -1 ;
						long iLocNum = 0 ;
						long iXhjRealLoc1 = 0 ;
						long iXhjRealLoc2 = 0 ;
						cSetFind2.GetFieldValue(_T("name"), strXHJName) ;
						cSetFind2.GetFieldValue(_T("id"), iXhjID) ;
						cSetFind2.GetFieldValue(_T("is_reverse"), iIsReverse) ;
						cSetFind2.GetFieldValue(_T("is_divide"), iIsDivide) ;
						cSetFind2.GetFieldValue(_T("type"), iType) ;
						cSetFind2.GetFieldValue(_T("loc_num"), iLocNum) ;
						cSetFind2.GetFieldValue(_T("loc_real"), iXhjRealLoc1) ;	
					
						cSetFind2.MoveNext() ;
						if (cSetFind2.IsEOF())
						{	
							//记录不止1条时的最后一条记录时
							if (iRsCount>1)
							{
								iYdqGroupSn = iSheetIndex==0?72:71 ;//站内侧线应答器组编号下行从71开始，上行从72开始

								long iLocNumFcz = 0 ; //为了计算轨道中心dw距离Fcz的偏移

								//begin 加入站内侧线部分应答器
								CADORecordset cSetFindZN ;
								
								//begin 侧线股道上的 FCZ DW CZ
								strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=2 and left(name,1)='%s' order by plus1 asc"), strStationName, iSheetIndex==0?3:2, iSheetIndex==0?_T("X"):_T("S")) ;
								if (cSetFindZN.IsOpen())
								{
									cSetFindZN.Close() ;
								}
								if (cSetFindZN.Open(g_PtrCon, strSql))
								{
									if (!cSetFindZN.IsBOF())
									{
										cSetFindZN.MoveFirst() ;
									}
									while (!cSetFindZN.IsEOF())
									{
										CString strFCZName = _T("") ;
										cSetFindZN.GetFieldValue(_T("name"), strFCZName) ;
										
										CString strGdSN = strFCZName.Mid(1) ;
										
										//////////////////////////////////////////////////////////////////////////
										//begin 一条侧线
										
										CADORecordset cSetFindZN_C ;											
										//begin 反向出站
										strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=2 and name='%s%s'"), strStationName, iSheetIndex==0?3:2, iSheetIndex==0?_T("X"):_T("S"), strGdSN) ;
										if (cSetFindZN_C.IsOpen())
										{
											cSetFindZN_C.Close() ;
										}
										if (cSetFindZN_C.Open(g_PtrCon, strSql))
										{
											if (cSetFindZN_C.GetRecordCount()>0)
											{
												cSetFindZN_C.MoveFirst() ;
												
												long iLocNumZN = 0 ;
												cSetFindZN_C.GetFieldValue(_T("loc_num"), iLocNumZN) ;
												iLocNumFcz = iLocNumZN ;
												CString strXHJNameZN = _T("") ;
												cSetFindZN_C.GetFieldValue(_T("name"), strXHJNameZN) ;
												
												for (int i=0; i<2; i++)
												{
													CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
													
													iYdqIndex++ ;
													strContent[0].Format(_T("%d"), iYdqIndex) ;
													long iYdqLocZN = iLocNumZN+20*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;									
													this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;													
													strContent[1].Format(_T("B%s-%d"), strXHJNameZN, i+1) ;
													CString strGroupSn = _T("") ;
													this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
													strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;
													strContent[4] = (i==0?_T("有源"):_T("无源")) ;
													strContent[5] = _T("FCZ") ;
													strContent[6] = _T("") ;
													strContent[7] = strStationName ;
													
													CString strComment = _T("") ;
													strComment.Format(_T("侧线，%d"), abs(20*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1))) ;
													
													//将应答器插入export_ydq表中
													strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, dev_type, useage, belong_xhj, belong_station, up_or_down, comment) values('%s','%s','%s', %d, '%s', '%s', '%s', '%s', %d, '%s')"), strContent[1], strContent[2], strContent[3], iYdqLocZN, strContent[4], strContent[5], strXHJNameZN, strContent[7], iSheetIndex==0?1:0, strComment) ;
													ExecSqlString(strSql) ;
													
													iCurRow++ ;
												}
												iYdqGroupSn+=2 ;
												
											}
											cSetFindZN_C.Close() ;
										}
										//end 反向出站
										
										//begin 定位
										strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and left(name,%d)='%sG' and type=100 order by name %s "), strStationName, strGdSN.GetLength()+1, strGdSN, iSheetIndex==0?_T("desc"):_T("asc") ) ;
										if (cSetFindZN_C.IsOpen())
										{
											cSetFindZN_C.Close() ;
										}
										if (cSetFindZN_C.Open(g_PtrCon, strSql))
										{
											if (cSetFindZN_C.GetRecordCount()>0)
											{
												cSetFindZN_C.MoveFirst() ;

												while (!cSetFindZN_C.IsEOF())
												{
													long iLocNumZN = 0 ;
													cSetFindZN_C.GetFieldValue(_T("loc_num"), iLocNumZN) ;
													CString strXHJNameZN = _T("") ;
													cSetFindZN_C.GetFieldValue(_T("name"), strXHJNameZN) ;
													
													CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
													
													iYdqIndex++ ;
													strContent[0].Format(_T("%d"), iYdqIndex) ;
													long iYdqLocZN = iLocNumZN  ; //+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;									
													this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;																		
													strContent[1].Format(_T("B%s"), strXHJNameZN) ;
													
													CString strGroupSn = _T("") ;
													this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
													strContent[2].Format(_T("%s-%s-%s-%s"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn) ;
													
													strContent[4] = _T("无源") ;
													strContent[5] = _T("DW") ;
													strContent[6] = _T("") ;
													strContent[7] = strStationName ;
													
													CString strComment = _T("") ;
													strComment.Format(_T("侧线，%d"), abs(iLocNumFcz-iLocNumZN)) ;
													
													//将应答器插入export_ydq表中
													strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, dev_type, useage, belong_xhj, belong_station, up_or_down,comment) values('%s','%s','%s', %d, '%s', '%s', '%s', '%s', %d, '%s')"), strContent[1], strContent[2], strContent[3], iYdqLocZN, strContent[4], strContent[5], strXHJNameZN, strContent[7], iSheetIndex==0?1:0, strComment) ;
													ExecSqlString(strSql) ;
													
													iCurRow++ ;	
													iYdqGroupSn+=2 ;

													cSetFindZN_C.MoveNext() ;

												}		
											}
											cSetFindZN_C.Close() ;
										}
										//end 定位
										
										//begin 正向出站
										strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=2 and name='%s%s'"), strStationName, iSheetIndex==0?3:2, iSheetIndex==0?_T("S"):_T("X"), strGdSN) ;
										if (cSetFindZN_C.IsOpen())
										{
											cSetFindZN_C.Close() ;
										}
										if (cSetFindZN_C.Open(g_PtrCon, strSql))
										{
											if (cSetFindZN_C.GetRecordCount()>0)
											{
												cSetFindZN_C.MoveFirst() ;
												
												long iLocNumZN = 0 ;
												cSetFindZN_C.GetFieldValue(_T("loc_num"), iLocNumZN) ;
												CString strXHJNameZN = _T("") ;
												cSetFindZN_C.GetFieldValue(_T("name"), strXHJNameZN) ;
												
												for (int i=0; i<2; i++)
												{
													CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
													
													iYdqIndex++ ;
													strContent[0].Format(_T("%d"), iYdqIndex) ;
													long iYdqLocZN = iLocNumZN+20*(iSheetIndex==0?1:-1)+5*(1-i)*(iSheetIndex==0?1:-1) ;									
													this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;																		
													strContent[1].Format(_T("B%s-%d"), strXHJNameZN, i+1) ;

													CString strGroupSn = _T("") ;
													this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
													strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

													strContent[4] = (i==1?_T("有源"):_T("无源")) ;
													strContent[5] = _T("CZ") ;
													strContent[6] = _T("") ;
													strContent[7] = strStationName ;
													
													CString strComment = _T("") ;
													strComment.Format(_T("侧线，%d"), abs(20*(iSheetIndex==0?1:-1)+5*(1-i)*(iSheetIndex==0?1:-1))) ;

													//将应答器插入export_ydq表中
													strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, dev_type, useage, belong_xhj, belong_station, up_or_down, comment) values('%s','%s','%s', %d, '%s', '%s', '%s', '%s', %d,'%s')"), strContent[1], strContent[2], strContent[3], iYdqLocZN, strContent[4], strContent[5], strXHJNameZN, strContent[7], iSheetIndex==0?1:0, strComment) ;
													ExecSqlString(strSql) ;
													
													iCurRow++ ;													
												}
												iYdqGroupSn+=2 ;
												
											}
											cSetFindZN_C.Close() ;
										}
										//end 正向出站
										
										//end 一条侧线
										//////////////////////////////////////////////////////////////////////////
										
										cSetFindZN.MoveNext() ;
										}
										cSetFindZN.Close() ;
									}
									//end 侧线股道上的 FCZ DW CZ
									
									strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=3 "), strStationName, iSheetIndex==0?3:2) ;
									if (cSetFindZN.IsOpen())
									{
										cSetFindZN.Close() ;
									}
									if (cSetFindZN.Open(g_PtrCon, strSql))
									{
										if (!cSetFindZN.IsBOF())
										{
											cSetFindZN.MoveFirst() ;
										}
										while (!cSetFindZN.IsEOF())
										{
											int iIsReverseZN = 0 ;
											cSetFindZN.GetFieldValue(_T("is_reverse"), iIsReverseZN) ;
											long iLocNumZN = 0 ;
											cSetFindZN.GetFieldValue(_T("loc_num"), iLocNumZN) ;
											CString strXHJNameZN = _T("") ;
											cSetFindZN.GetFieldValue(_T("name"), strXHJNameZN) ;
											
											//begin 侧线进站 反向进站
											
											for (int i=0; i<3; i++)
											{
												CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
												
												iYdqIndex++ ;
												strContent[0].Format(_T("%d"), iYdqIndex) ;
												long iYdqLocZN = 0 ;
												if(iIsReverseZN==0)
												{
													iYdqLocZN = iLocNumZN+30*(iSheetIndex==0?1:-1)+5*(2-i)*(iSheetIndex==0?1:-1) ;
													strContent[4] = (i==2?_T("有源"):_T("无源")) ;
												}
												else 
												{
													iYdqLocZN = iLocNumZN+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;
													strContent[4] = (i==0?_T("有源"):_T("无源")) ;
												}													
												this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;																		
												strContent[1].Format(_T("B%s-%d"), strXHJNameZN, i+1) ;
												
												CString strGroupSn = _T("") ;
												this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
												strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

												strContent[5] = (iIsReverseZN==0?_T("JZ"):_T("FJZ")) ;
												strContent[6] = _T("") ;
												strContent[7] = strStationName ;
												
												//将应答器插入export_ydq表中
												strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, dev_type, useage, belong_xhj, belong_station, up_or_down) values('%s','%s','%s', %d, '%s', '%s', '%s', '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLocZN, strContent[4], strContent[5], strXHJNameZN, strContent[7], iSheetIndex==0?1:0) ;
												ExecSqlString(strSql) ;
												
												iCurRow++ ;
											}
											iYdqGroupSn+=2 ;
											
											//end 侧线进站 反向进站
											
											cSetFindZN.MoveNext() ;
										}
										cSetFindZN.Close() ;
									}									
									//end 加入站内侧线部分应答器
									
									break ;
								}
								else if (iType==3) //最后一个信号点刚好是进站，也是分界，而且是最后一站
								{
									cSetFind2.MovePrevious() ;
								}
							}
							else
							{
								cSetFind2.GetFieldValue(_T("loc_real"), iXhjRealLoc2) ;								
								cSetFind2.MovePrevious() ;
							}
							
							CString strYdqGroupType = _T("") ;
							
							//区间信号机
							if (iType==0)
							{
								//离中继站最近的信号机
								if (iStationType==11&&strZjzNearestXhj==strXHJName)
								{
									strYdqGroupType = _T("ZJ") ;
									
									for (int iZjYdqIndex=0; iZjYdqIndex<2; iZjYdqIndex++)
									{
										for (int i=0; i<3; i++)
										{
											CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
											
											iYdqIndex++ ;
											strContent[0].Format(_T("%d"), iYdqIndex) ;
											long iYdqLoc = iLocNum+200*(iSheetIndex==0?1:-1)*(2-iZjYdqIndex)+(iSheetIndex==0?-1:1)+5*(2-i)*(iSheetIndex==0?1:-1) ;									
											this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;												
											CString strFName = _T("") ;
											this->GetFName(iYdqLoc, strFName, iSheetIndex==0?1:0) ;							
											strContent[1].Format(_T("B%s-%d"), strFName.Mid(1), i+1) ;
											
											CString strGroupSn = _T("") ;
											this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
											strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

											strContent[4] = (i==2?_T("有源"):_T("无源")) ;
											strContent[5] = (iZjYdqIndex==0?_T("ZJ1"):_T("ZJ2")) ;
											strContent[6] = _T("") ;
											strContent[7] = strStationName ;

											long iYdqLocReal = iXhjRealLoc1+200*(iSheetIndex==0?1:-1)*(2-iZjYdqIndex)+(iSheetIndex==0?-1:1)+5*(2-i)*(iSheetIndex==0?1:-1) ;
											
											//将应答器插入export_ydq表中
											strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
											ExecSqlString(strSql) ;
											
											iCurRow++ ;
										}
										iYdqGroupSn+=2 ;										
									}
									
								}
								else
								{
									strYdqGroupType = _T("Q") ;
									for (int i=0; i<2; i++)
									{
										CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
										
										iYdqIndex++ ;
										strContent[0].Format(_T("%d"), iYdqIndex) ;
										long iYdqLoc = iLocNum+199*(iSheetIndex==0?1:-1)+5*(1-i)*(iSheetIndex==0?1:-1) ;									
										this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;
										CString strFName = _T("") ;
										this->GetFName(iYdqLoc, strFName, iSheetIndex==0?1:0) ;							
										strContent[1].Format(_T("B%s-%d"), strFName.Mid(1), i+1) ;
										
										CString strGroupSn = _T("") ;
										this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
										strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

										strContent[4] = _T("无源") ;
										strContent[5] = _T("Q") ;
										strContent[6] = _T("") ;
										strContent[7] = strStationName ;
										
										long iYdqLocReal = iXhjRealLoc1+199*(iSheetIndex==0?1:-1)+5*(1-i)*(iSheetIndex==0?1:-1) ;

										//将应答器插入export_ydq表中
										strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
										ExecSqlString(strSql) ;
										
										iCurRow++ ;
									}
									iYdqGroupSn+=2 ;
								}
								
								//begin 查看信号机防护区段需不需要插入DW应答器
								if (iXhjRealLoc1>0&&iXhjRealLoc2>0)
								{
									long iSectionLen = labs(iXhjRealLoc2-iXhjRealLoc1) ;
									
									CADORecordset cSetFindDw ;
									if (iSheetIndex==0)
									{
										strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and left(name,1)='F' order by loc_real %s"), iXhjRealLoc2, iXhjRealLoc1, iSheetIndex==0?1:0, strOrder[iSheetIndex]) ;
									}
									else
									{
										strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and left(name,1)='F' order by loc_real %s"), iXhjRealLoc1, iXhjRealLoc2, iSheetIndex==0?1:0, strOrder[iSheetIndex]) ;
									}
									if (cSetFindDw.IsOpen())
									{
										cSetFindDw.Close() ;
									}
									if (cSetFindDw.Open(g_PtrCon, strSql))
									{
										int iFCount = cSetFindDw.GetRecordCount() ;
										int iFIndex = 0 ;
										if (iFCount>0)
										{
											cSetFindDw.MoveFirst() ;
											while (!cSetFindDw.IsEOF())
											{										
												if (iFIndex==(iFCount/2))
												{
													long iFLocNum = 0, iFLocNumReal = 0 ;
													cSetFindDw.GetFieldValue(_T("id"), iXhjID) ;
													cSetFindDw.GetFieldValue(_T("name"), strXHJName) ;
													cSetFindDw.GetFieldValue(_T("loc_num"), iFLocNum) ;
													cSetFindDw.GetFieldValue(_T("loc_real"), iFLocNumReal) ;
													CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
													
													iYdqIndex++ ;
													strContent[0].Format(_T("%d"), iYdqIndex) ;
													long iYdqLoc = iFLocNum+199*(iSheetIndex==0?1:-1) ;									
													this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;
													CString strFName = _T("") ;
													this->GetFName(iYdqLoc, strFName, iSheetIndex==0?1:0) ;									
													strContent[1].Format(_T("B%s"), strFName.Mid(1)) ;
													
													CString strGroupSn = _T("") ;
													this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
													strContent[2].Format(_T("%s-%s-%s-%s"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn) ;

													strContent[4] = _T("无源") ;
													strContent[5] = _T("DW") ;
													strContent[6] = _T("") ;
													strContent[7] = strStationName ;

													long iYdqLocReal = iFLocNumReal+199*(iSheetIndex==0?1:-1) ;	
													
													//将应答器插入export_ydq表中
													strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
													ExecSqlString(strSql) ;
													
													cSetFindDw.Edit() ;
													cSetFindDw.SetFieldValue(_T("ydq_type"), _T("DW")) ;
													cSetFindDw.Update() ;
													
													iCurRow++ ;
													iYdqGroupSn+=2 ;
													break ;
												}
												iFIndex++ ;
												cSetFindDw.MoveNext() ;
											}
										}
										cSetFindDw.Close() ;
									}

								}								
								//end 查看信号机防护区段需不需要插入DW应答器								
								
							}
							else if (iType==3&&iIsReverse==0)
							{
								strYdqGroupType = _T("JZ") ;
								
								CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
								
								iYdqIndex++ ;
								strContent[0].Format(_T("%d"), iYdqIndex) ;
								long iYdqLoc = iLocNum+250*(iSheetIndex==0?1:-1) ;									
								this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;
								CString strFName = _T("") ;
								this->GetFName(iYdqLoc, strFName, iSheetIndex==0?1:0) ;									
								strContent[1].Format(_T("B%s"), strFName.Mid(1)) ;
								
								CString strGroupSn = _T("") ;
								this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
								strContent[2].Format(_T("%s-%s-%s-%s"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn) ;

								strContent[4] = _T("无源") ;
								strContent[5] = _T("DW") ;
								strContent[6] = _T("") ;
								strContent[7] = strStationName ;

								long iYdqLocReal = iXhjRealLoc1+250*(iSheetIndex==0?1:-1) ;	
								
								//将应答器插入export_ydq表中
								strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
								ExecSqlString(strSql) ;
								
								iCurRow++ ;
								iYdqGroupSn+=2 ;
								
								for (int i=0; i<3; i++)
								{
									CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
									
									iYdqIndex++ ;
									strContent[0].Format(_T("%d"), iYdqIndex) ;
									long iYdqLoc = iLocNum+30*(iSheetIndex==0?1:-1)+5*(2-i)*(iSheetIndex==0?1:-1) ;									
									this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;																		
									strContent[1].Format(_T("B%s-%d"), strXHJName, i+1) ;
									
									CString strGroupSn = _T("") ;
									this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
									strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

									strContent[4] = (i==2?_T("有源"):_T("无源")) ;
									strContent[5] = _T("JZ") ;
									strContent[6] = _T("") ;
									strContent[7] = strStationName ;

									long iYdqLocReal = iXhjRealLoc1+30*(iSheetIndex==0?1:-1)+5*(2-i)*(iSheetIndex==0?1:-1) ;	
									
									//将应答器插入export_ydq表中
									strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d,'%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
									ExecSqlString(strSql) ;
									
									iCurRow++ ;
								}
								iYdqGroupSn+=2 ;
							}
							else if (iType==3&&iIsReverse==1)
							{
								strYdqGroupType = _T("FJZ") ;
								
								//在进站分界时，反向进站归进站信号机所归属的站所有，下一个站不再为其配FJZ/FDW	
								if (!(iXhjIndex2==0&&iRsCount>1&&iType==3&&iIsReverse==1&&iIsDivide==1))								
								{									
									for (int i=0; i<3; i++)
									{
										CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
										
										iYdqIndex++ ;
										strContent[0].Format(_T("%d"), iYdqIndex) ;
										long iYdqLoc = iLocNum+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;									
										this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;																		
										strContent[1].Format(_T("B%s-%d"), strXHJName, i+1) ;
										
										CString strGroupSn = _T("") ;
										this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
										strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;
										
										strContent[4] = (i==0?_T("有源"):_T("无源")) ;
										strContent[5] = _T("FJZ") ;
										strContent[6] = _T("") ;
										strContent[7] = strStationName ;
										
										long iYdqLocReal = iXhjRealLoc1+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;	
										
										//将应答器插入export_ydq表中
										strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal,strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
										ExecSqlString(strSql) ;
										
										iCurRow++ ;
									}
									iYdqGroupSn+=2 ;
									
									CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
									
									iYdqIndex++ ;
									strContent[0].Format(_T("%d"), iYdqIndex) ;
									long iYdqLoc = iLocNum+250*(iSheetIndex==0?-1:1) ;									
									this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;
									CString strFName = _T("") ;
									this->GetFName(iYdqLoc, strFName, iSheetIndex==0?1:0) ;									
									strContent[1].Format(_T("B%s"), strFName.Mid(1)) ;
									
									CString strGroupSn = _T("") ;
									this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
									strContent[2].Format(_T("%s-%s-%s-%s"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn) ;
									
									strContent[4] = _T("无源") ;
									strContent[5] = _T("FDW") ;
									strContent[6] = _T("") ;
									strContent[7] = strStationName ;
									
									long iYdqLocReal = iXhjRealLoc1+250*(iSheetIndex==0?-1:1) ;
									
									//将应答器插入export_ydq表中
									strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d,'%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID,strContent[7], iSheetIndex==0?1:0) ;
									ExecSqlString(strSql) ;
									
									iCurRow++ ;
									iYdqGroupSn+=2 ;
									
								}
								
								//begin 查看信号机防护区段需不需要插入DW应答器
								if (iXhjRealLoc1>0&&iXhjRealLoc2>0)
								{
									long iSectionLen = labs(iXhjRealLoc2-iXhjRealLoc1) ;
									
									CADORecordset cSetFindDw ;
									if (iSheetIndex==0)
									{
										strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and left(name,1)='F' order by loc_real %s"), iXhjRealLoc2, iXhjRealLoc1, iSheetIndex==0?1:0, strOrder[iSheetIndex]) ;
									}
									else
									{
										strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and up_or_down=%d and left(name,1)='F' order by loc_real %s"), iXhjRealLoc1, iXhjRealLoc2, iSheetIndex==0?1:0, strOrder[iSheetIndex]) ;
									}
									if (cSetFindDw.IsOpen())
									{
										cSetFindDw.Close() ;
									}
									if (cSetFindDw.Open(g_PtrCon, strSql))
									{
										int iFCount = cSetFindDw.GetRecordCount() ;
										int iFIndex = 0 ;
										if (iFCount>0)
										{
											cSetFindDw.MoveFirst() ;
											while (!cSetFindDw.IsEOF())
											{										
												if (iFIndex==(iFCount/2))
												{
													long iFLocNum = 0, iFLocNumReal = 0 ;
													cSetFindDw.GetFieldValue(_T("id"), iXhjID) ;
													cSetFindDw.GetFieldValue(_T("name"), strXHJName) ;
													cSetFindDw.GetFieldValue(_T("loc_num"), iFLocNum) ;
													cSetFindDw.GetFieldValue(_T("loc_real"), iFLocNumReal) ;
													CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
													
													iYdqIndex++ ;
													strContent[0].Format(_T("%d"), iYdqIndex) ;
													long iYdqLoc = iFLocNum+199*(iSheetIndex==0?1:-1) ;									
													this->GetLocShow(iYdqLoc, m_strMileSysName, strContent[3]) ;
													CString strFName = _T("") ;
													this->GetFName(iYdqLoc, strFName, iSheetIndex==0?1:0) ;									
													strContent[1].Format(_T("B%s"), strFName.Mid(1)) ;
													
													CString strGroupSn = _T("") ;
													this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
													strContent[2].Format(_T("%s-%s-%s-%s"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn) ;

													strContent[4] = _T("无源") ;
													strContent[5] = _T("DW") ;
													strContent[6] = _T("") ;
													strContent[7] = strStationName ;

													long iYdqLocReal = iFLocNumReal+199*(iSheetIndex==0?1:-1) ;	
													
													//将应答器插入export_ydq表中
													strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLoc, iYdqLocReal, strContent[4], strContent[5], strXHJName, iXhjID, strContent[7], iSheetIndex==0?1:0) ;
													ExecSqlString(strSql) ;
													
													cSetFindDw.Edit() ;
													cSetFindDw.SetFieldValue(_T("ydq_type"), _T("DW")) ;
													cSetFindDw.Update() ;
													
													iCurRow++ ;
													iYdqGroupSn+=2 ;
													break ;
												}
												iFIndex++ ;
												cSetFindDw.MoveNext() ;
											}
										}
										cSetFindDw.Close() ;
									}

								}								
								//end 查看信号机防护区段需不需要插入DW应答器								
								
							}
							
							cSetFind2.Edit() ;
							cSetFind2.SetFieldValue(_T("ydq_type"), strYdqGroupType) ;
							cSetFind2.Update() ;
							
							//begin 加入站内应答器
							if (iType==3 && iIsReverse==0)//如果是进站，则加入站内的
							{
								CADORecordset cSetFindZN ;

								long iCzLocNum_F = 0 ,iCzLocNumReal_F = 0 ; //为推算股道中心的定位应答器的loc_real
								
								//begin 反向出站
								//strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=2 and left(name,1)='%s'"), strStationName, iSheetIndex==0?1:0, iSheetIndex==0?_T("X"):_T("S")) ;
								strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and type=4 and up_or_down=%d and left(name,1)='%s'"), iLeftDivideRealLoc, iRightDivideRealLoc, iSheetIndex==0?1:0, iSheetIndex==0?_T("X"):_T("S") ) ;
								if (cSetFindZN.IsOpen())
								{
									cSetFindZN.Close() ;
								}
								if (cSetFindZN.Open(g_PtrCon, strSql))
								{
									if (cSetFindZN.GetRecordCount()>0)
									{
										cSetFindZN.MoveFirst() ;
										
										long iLocNumZN = 0, iLocNumZNReal = 0 ;
										cSetFindZN.GetFieldValue(_T("id"), iXhjID) ;										
										cSetFindZN.GetFieldValue(_T("loc_num"), iLocNumZN) ;
										cSetFindZN.GetFieldValue(_T("loc_real"), iLocNumZNReal) ;

										iCzLocNum_F = iLocNumZN ;
										iCzLocNumReal_F = iLocNumZNReal ;

										CString strXHJNameZN = _T("") ;
										cSetFindZN.GetFieldValue(_T("name"), strXHJNameZN) ;
										
										for (int i=0; i<2; i++)
										{
											CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
											
											iYdqIndex++ ;
											strContent[0].Format(_T("%d"), iYdqIndex) ;
											long iYdqLocZN = iLocNumZN+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;									
											this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;																		
											strContent[1].Format(_T("B%s-%d"), strXHJNameZN, i+1) ;
											
											CString strGroupSn = _T("") ;
											this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
											strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

											strContent[4] = (i==0?_T("无源"):_T("无源")) ; //strContent[4] = (i==0?_T("有源"):_T("无源")) ;
											strContent[5] = _T("FCZ") ;
											strContent[6] = _T("") ;
											strContent[7] = strStationName ;

											long iYdqLocReal = iLocNumZNReal+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;	
											
											//将应答器插入export_ydq表中
											strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLocZN, iYdqLocReal, strContent[4], strContent[5], strXHJNameZN, iXhjID, strContent[7], iSheetIndex==0?1:0) ;
											ExecSqlString(strSql) ;
											
											iCurRow++ ;
										}
										iYdqGroupSn+=2 ;
										
										
									}
									cSetFindZN.Close() ;
								}
								//end 反向出站
								
								//begin 定位
								strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=100 order by name %s "), strStationName, iSheetIndex==0?1:0, iSheetIndex==0?_T("desc"):_T("asc")) ;
								//strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and type=4 and up_or_down=%d and left(name,1)='%s'"), iLeftDivideRealLoc, iRightDivideRealLoc, iSheetIndex==0?1:0, iSheetIndex==0?_T("X"):_T("S") ) ;
								if (cSetFindZN.IsOpen())
								{
									cSetFindZN.Close() ;
								}
								if (cSetFindZN.Open(g_PtrCon, strSql))
								{
									if (cSetFindZN.GetRecordCount()>0)
									{
										cSetFindZN.MoveFirst() ;

										while (!cSetFindZN.IsEOF())
										{
											long iLocNumZN = 0 ;
											cSetFindZN.GetFieldValue(_T("loc_num"), iLocNumZN) ;
											CString strXHJNameZN = _T("") ;
											cSetFindZN.GetFieldValue(_T("name"), strXHJNameZN) ;
											
											CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
											
											iYdqIndex++ ;
											strContent[0].Format(_T("%d"), iYdqIndex) ;
											long iYdqLocZN = iLocNumZN  ; //+30*(iSheetIndex==0?-1:1)+5*i*(iSheetIndex==0?-1:1) ;									
											this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;																		
											strContent[1].Format(_T("B%s"), strXHJNameZN) ;
											
											CString strGroupSn = _T("") ;
											this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
											strContent[2].Format(_T("%s-%s-%s-%s"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn) ;
											
											strContent[4] = _T("无源") ;
											strContent[5] = _T("DW") ;
											strContent[6] = _T("") ;
											strContent[7] = strStationName ;
											
											long iYdqLocZNReal = iCzLocNumReal_F+(iLocNumZN-iCzLocNum_F) ;
											
											//将应答器插入export_ydq表中
											strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLocZN, iYdqLocZNReal, strContent[4], strContent[5], strXHJNameZN, strContent[7], iSheetIndex==0?1:0) ;
											ExecSqlString(strSql) ;
											
											iCurRow++ ;	
											iYdqGroupSn+=2 ;

											cSetFindZN.MoveNext() ;
										}
										

										
									}
									cSetFindZN.Close() ;
								}
								//end 定位
								
								//begin 正向出站
								//strSql.Format(_T("select * from import_xhj_info where belong_station='%s' and up_or_down=%d and type=2 and left(name,1)='%s'"), strStationName, iSheetIndex==0?1:0, iSheetIndex==0?_T("S"):_T("X")) ;
								strSql.Format(_T("select * from compute_xhj_table where loc_real>%d and loc_real<%d and type=4 and up_or_down=%d and left(name,1)='%s'"), iLeftDivideRealLoc, iRightDivideRealLoc, iSheetIndex==0?1:0, iSheetIndex==0?_T("S"):_T("X") ) ;
								if (cSetFindZN.IsOpen())
								{
									cSetFindZN.Close() ;
								}
								if (cSetFindZN.Open(g_PtrCon, strSql))
								{
									if (cSetFindZN.GetRecordCount()>0)
									{
										cSetFindZN.MoveFirst() ;
										
										long iLocNumZN = 0 ,iLocNumZNReal = 0 ;
										cSetFindZN.GetFieldValue(_T("id"), iXhjID) ;
										cSetFindZN.GetFieldValue(_T("loc_num"), iLocNumZN) ;
										cSetFindZN.GetFieldValue(_T("loc_real"), iLocNumZNReal) ;
										CString strXHJNameZN = _T("") ;
										cSetFindZN.GetFieldValue(_T("name"), strXHJNameZN) ;
										
										for (int i=0; i<2; i++)
										{
											CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
											
											iYdqIndex++ ;
											strContent[0].Format(_T("%d"), iYdqIndex) ;
											long iYdqLocZN = iLocNumZN+30*(iSheetIndex==0?1:-1)+5*(1-i)*(iSheetIndex==0?1:-1) ;									
											this->GetLocShow(iYdqLocZN, m_strMileSysName, strContent[3]) ;																		
											strContent[1].Format(_T("B%s-%d"), strXHJNameZN, i+1) ;
											
											CString strGroupSn = _T("") ;
											this->FormatGroupSn(iYdqGroupSn, strGroupSn) ;
											strContent[2].Format(_T("%s-%s-%s-%s-%d"), m_strBigRegionSn, m_strSmallRegionSn, m_strStationSn, strGroupSn, i+1) ;

											strContent[4] = (i==1?_T("无源"):_T("无源")) ; //原来是 strContent[4] = (i==1?_T("有源"):_T("无源")) ;
											strContent[5] = _T("CZ") ;
											strContent[6] = _T("") ;
											strContent[7] = strStationName ;

											long iYdqLocZNReal = iLocNumZNReal+30*(iSheetIndex==0?1:-1)+5*(1-i)*(iSheetIndex==0?1:-1) ;	
											
											//将应答器插入export_ydq表中
											strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strContent[1], strContent[2], strContent[3], iYdqLocZN, iYdqLocZNReal, strContent[4], strContent[5],  strXHJNameZN, iXhjID, strContent[7], iSheetIndex==0?1:0) ;
											ExecSqlString(strSql) ;
											
											iCurRow++ ;
										}
										iYdqGroupSn+=2 ;
										
									}
									cSetFindZN.Close() ;
								}
								//end 正向出站			
								
							}
							//end 加入站内应答器
							
							iXhjIndex2++ ;

							cSetFind2.MoveNext() ;
							
						}
						
						cSetFind2.Close() ;
					}
					
					iStationIndex++ ;
					cSetFind.MoveNext() ;
				}
				cSetFind.Close() ;

				acedRestoreStatusBar() ;

			}

			//being 进站外方1/2/3JG定位应答器由单个变2个，组名变FQ
			strSql.Format(_T("select * from compute_xhj_table where type=1 order by loc_real %s"), strOrder[iSheetIndex] ) ;
			if (cSetFind.IsOpen())
			{
				cSetFind.Close() ;
			}
			if (cSetFind.Open(g_PtrCon, strSql))
			{
				if (cSetFind.GetRecordCount()>0)
				{
					if (!cSetFind.IsBOF())
					{
						cSetFind.MoveFirst() ;
					}
					while (!cSetFind.IsEOF())
					{
						CString	strStationName = _T("") ;
						cSetFind.GetFieldValue(_T("name"), strStationName) ;
						
						if (strStationName == _T("南京南")&&iSheetIndex==1)
						{
							int iii = 0 ;
						}
						
						CADORecordset cSetFind2 ;
						strSql.Format(_T("select top 4 * from export_ydq where up_or_down=%d and loc_real%s(select TOP 1 loc_real from export_ydq where belong_station='%s' and useage='JZ' and up_or_down=%d and loc_real>0  order by loc_real %s) and useage='DW' and belong_station='%s' order by loc_real %s"),  iSheetIndex==0?1:0, iSheetIndex==0?_T(">"):_T("<"), strStationName, iSheetIndex==0?1:0, strOrder[iSheetIndex], strStationName, strOrder[(iSheetIndex+1)%2] ) ;
						CString strSqlTmp = strSql ;
						if (cSetFind2.IsOpen())
						{
							cSetFind2.Close() ;
						}
						if (cSetFind2.Open(g_PtrCon, strSql))
						{
							if (cSetFind2.GetRecordCount()>0)
							{
								if (!cSetFind2.IsBOF())
								{
									cSetFind2.MoveFirst() ;
								}
								
								int iDwIndex = 0 ;
								while (!cSetFind2.IsEOF())
								{
									if (iDwIndex==0)
									{
										iDwIndex++ ;
										cSetFind2.MoveNext() ;
										continue ;
									}
									
									CString strYdqName1 = _T(""), strSn1 = _T(""), strLoc1 = _T(""), strBelongXhj = _T(""), strBelongStation = _T("") ;
									long iLocNum1 = 0, iLocNumReal1 = 0 ;
									long iXhjID = 0 ;
									cSetFind2.GetFieldValue(_T("id"), iXhjID) ;
									cSetFind2.GetFieldValue(_T("name"), strYdqName1) ;
									cSetFind2.GetFieldValue(_T("sn"), strSn1) ;
									cSetFind2.GetFieldValue(_T("loc"), strLoc1) ;
									cSetFind2.GetFieldValue(_T("belong_xhj"), strBelongXhj) ;
									cSetFind2.GetFieldValue(_T("loc_num"), iLocNum1) ;
									cSetFind2.GetFieldValue(_T("loc_real"), iLocNumReal1) ;
									cSetFind2.GetFieldValue(_T("belong_station"), strBelongStation) ;
									// 							if (strBelongXhj==_T("00557"))
									// 							{
									// 								int iii = 0 ;
									// 							}
									
									CString strYdqName2 = _T(""), strSn2 = _T(""), strLoc2 = _T("") ;
									long iLocNum2 = 0, iLocNumReal2 = 0 ;
									
									strYdqName2 = strYdqName1 ;
									strSn2 = strSn1 ;
									strLoc2 = strLoc1 ;
									iLocNum2 = iLocNum1 ;
									iLocNum1 = iLocNum1 + 5*(iSheetIndex==0?1:-1) ;
									iLocNumReal2 = iLocNumReal1 ;
									iLocNumReal1 = iLocNumReal1 + 5*(iSheetIndex==0?1:-1) ;
									this->GetLocShow(iLocNum1, m_strMileSysName, strLoc1) ;
									CString strFName = _T("") ;
									this->GetFName(iLocNum1, strFName, iSheetIndex==0?1:0) ;									
									strYdqName1.Format(_T("B%s"), strFName.Mid(1)) ;
									strYdqName2 = strYdqName1 ;
									
									if (strYdqName1==_T("B10239"))
									{
										int iiii = 0 ;
									}
									
									strYdqName1.Insert(555, _T("-1")) ;
									strYdqName2.Insert(555, _T("-2")) ;
									strSn1.Insert(555, _T("-1")) ;
									strSn2.Insert(555, _T("-2")) ;
									
									cSetFind2.Edit() ;
									cSetFind2.SetFieldValue(_T("name"), strYdqName1) ;
									cSetFind2.SetFieldValue(_T("sn"), strSn1) ;
									cSetFind2.SetFieldValue(_T("loc"), strLoc1) ;							
									cSetFind2.SetFieldValue(_T("loc_num"), iLocNum1) ;
									cSetFind2.SetFieldValue(_T("loc_real"), iLocNumReal1) ;
									cSetFind2.SetFieldValue(_T("useage"), _T("FQ")) ;
									cSetFind2.Update() ;
									
									strSql.Format(_T("insert into export_ydq (name, sn, loc, loc_num, loc_real, dev_type, useage, belong_xhj, belong_xhj_id, belong_station, up_or_down) values('%s','%s','%s', %d, %d, '%s', '%s', '%s', %d, '%s', %d)"), strYdqName2, strSn2, strLoc2, iLocNum2, iLocNumReal2, _T("无源"), _T("FQ"), strBelongXhj, iXhjID,strBelongStation, iSheetIndex==0?1:0) ;
									ExecSqlString(strSql) ;
									
									cSetFind2.MoveNext() ;
								}
								
							}

							
							cSetFind2.Close() ;
						}
						
						
						cSetFind.MoveNext() ;
					}
					
				}
				
				cSetFind.Close() ;
				
			}
			//end 进站外方1/2/3JG定位应答器由单个变2个，组名变FQ
		}
		
		//begin 增加用来排序的列plus
		strSql.Format(_T("select * from export_ydq order by id")) ;
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
				CString strSn = _T("") ;
				//long iPlus = 0 ;
				cSetFind.GetFieldValue(_T("sn"), strSn) ;

				CStringArray strArrayBianHao ;
				CBlkUtility::DivideString(strSn, _T("-"), strArrayBianHao) ;
				int iCount = strArrayBianHao.GetSize() ;
				//strSn.Replace(_T("-"), _T("")) ;
				if (iCount<5)
				{
					strSn.Insert(555, _T("00")) ;
				}
// 				CString strNewSn = _T("") ;
				for (int i=0; i<iCount; i++)
				{
					CString strBianHao = strArrayBianHao.GetAt(i) ;
					if (strBianHao.GetLength()<2)
					{
						strBianHao.Insert(0, _T("0")) ;
						strArrayBianHao.SetAt(i, strBianHao) ;
					}
					// strNewSn.Insert(555, strBianHao) ;
				}

				CString strPlus1 = _T(""), strPlus2 = _T("") ;
				strPlus1.Format(_T("%s%s"), strArrayBianHao.GetAt(3), strArrayBianHao.GetAt(4)) ;
				strPlus2.Format(_T("%s%s"), strArrayBianHao.GetAt(1), strArrayBianHao.GetAt(2)) ;
				//strPlus1.TrimLeft(_T("0")) ;				

				cSetFind.Edit() ;
				cSetFind.SetFieldValue(_T("plus1"), strPlus1) ;
				cSetFind.SetFieldValue(_T("plus2"), strPlus2) ;
				cSetFind.Update() ;

				cSetFind.MoveNext() ;
			}
			cSetFind.Close() ;
		}
		
		
		//end 增加用来排序的列plus


}

//将进行过自动分隔、添加分界等等处理后的信号机表格再导出来
void CLKDataTable::ExportXHTable(CString strSavePath)
{
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	CString strTempletFile ;
	strTempletFile.Format(_T("%s\\support\\lkdata\\信号机布置表.xls"), m_strSysBasePath) ;

	CUserSystem mysys ;

	//CString strMileSys = _T("DK") ;

	_ApplicationPtr m_pExcel = NULL ;
	//WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	_WorksheetPtr pSheet2 = NULL ;

	CString strMsg ;
	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application")) ;
		if(FAILED(hr))
		{
			AfxMessageBox(_T("初始化Excel失败,可能是没有安装Excel")) ;
			return ;
		}

		//pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBook = m_pExcel->GetWorkbooks ()->Open( (_bstr_t)strTempletFile ) ;

		CString strColName[2][8] = {
			{_T("C"),_T("D"),_T("E"),_T("F"),_T("H"),_T("I"),_T("J"),_T("K")},
			{_T("M"),_T("N"),_T("O"),_T("P"),_T("R"),_T("S"),_T("T"),_T("U")}
		} ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;		
		vt_index.iVal = 1 ;		
		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		
		CString strWriteText = _T("") ;
		
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)_T("B1")) ;
		strWriteText.Format(_T("%s信号机布置表"), this->m_strXHTitle) ;
		pRange->PutFormulaR1C1((_variant_t)strWriteText) ;	
		
		CADORecordset cSetFind ;
		CString strSql ;
		strSql.Format(_T("select * from compute_xhj_table where is_reverse<>1 and type<>4 and not(right(name,1)='链' and up_or_down=1) order by loc_real asc")) ;
		
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
			int iSum = cSetFind.GetRecordCount() ;
			CString strBarMsg ;
			strBarMsg.Format(_T("%d条记录"), iSum) ;

			acedSetStatusBarProgressMeter(strBarMsg, 0, iSum-1) ;	

			int iIndex = 0 ;
			int iCurRowDown = 5 ;
			int iCurRowUp = 5 ;
			while (!cSetFind.IsEOF())
			{

				acedSetStatusBarProgressMeterPos(iIndex) ;


				int iCurUseRow = iCurRowDown ;

				CString strName = _T(""), strLocShow = _T(""), strFreq = _T("") ;
				long iLocNum = 0, iLocReal = 0, iLen = 0, iTallOrLow = 1, iHasAdmit = 0, iShowNum = 2, iUpOrDown = 0, iIsDivede = 0, iType = 0 ;
				
				cSetFind.GetFieldValue(_T("name"), strName) ;
				cSetFind.GetFieldValue(_T("loc_show"), strLocShow) ;
				cSetFind.GetFieldValue(_T("loc_num"), iLocNum) ;
				cSetFind.GetFieldValue(_T("loc_real"), iLocReal) ;
				cSetFind.GetFieldValue(_T("len"), iLen) ;
				cSetFind.GetFieldValue(_T("tall_or_low"), iTallOrLow) ;
				cSetFind.GetFieldValue(_T("has_admit"), iHasAdmit) ;
				cSetFind.GetFieldValue(_T("show_num"), iShowNum) ;
				cSetFind.GetFieldValue(_T("up_or_down"), iUpOrDown) ;
				cSetFind.GetFieldValue(_T("is_divide"), iIsDivede) ;
				cSetFind.GetFieldValue(_T("set_freq"), strFreq) ;
				cSetFind.GetFieldValue(_T("type"), iType) ;

				if (iLocReal==108511)
				{
					int iii = 0 ;
				}
				
				CString strContent[8] = {_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T(""),_T("")} ;
				CString strTmp  = _T("") ;
				strName.TrimLeft(_T("0")) ;
				strContent[0] = strName.Left(1)==_T("F")?_T("F"):strName ; //名称				
				strContent[1] = strLocShow ; //设置里程
				this->GetLocShow(iLocNum, m_strMileSysName, strTmp) ;
				//strTmp.Format(_T("%d"), iLocNum) ;
				strContent[2] = strTmp ;//里程
				strTmp.Format(_T("%d"), iLen) ;
				strContent[3] = strTmp;//长度
				strTmp = _T("") ;
				if (iTallOrLow==0)
				{
					strTmp = _T("矮柱") ;
				}
				strContent[4] = strTmp ;
				strTmp = _T("") ;
				if (iHasAdmit==1)
				{
					strTmp = _T("1") ;
				}
				strContent[5] = strTmp ;
				strTmp = _T("") ;
				if (iShowNum!=2)
				{
					strTmp.Format(_T("%d"), iShowNum) ;
				}
				if (iType==3/*&&strFreq!=_T("")*/)
				{
					strTmp = _T("") ;
					if (iUpOrDown==1)
					{
						strTmp = strFreq ;
					}
					else
					{
						CADORecordset cSetFindFreq ; 
						strSql.Format(_T("select top 1 * from compute_xhj_table where loc_real<%d and up_or_down=0 and type=0 and right(name,1)<>'链' order by loc_real desc"), iLocReal) ;
						if (cSetFindFreq.IsOpen())
						{
							cSetFindFreq.Close() ;
						}
						if (cSetFindFreq.Open(g_PtrCon, strSql))
						{
							if (cSetFindFreq.GetRecordCount()>0)
							{
								cSetFindFreq.MoveFirst() ;
								cSetFindFreq.GetFieldValue(_T("set_freq"), strTmp) ;
							}
							cSetFindFreq.Close() ;
						}

					}
					
				}
				strContent[6] = strTmp ;
				switch (iType)
				{
				case 1:
					strTmp = _T("车站") ;
					break;
				case 2:
					strTmp = _T("出站") ;
					break;
				case 3:
					strTmp = _T("进站") ;
					break;
				case 11:
					strTmp = _T("中继站") ;
					break;
				default:
					strTmp = _T("") ;
					break;
				}
				if (iIsDivede==1)
				{
					strTmp.Insert(555,_T("1")) ;
				}
				strContent[7] = strTmp;


				iCurUseRow = iUpOrDown==0?iCurRowDown:iCurRowUp ;
				if ((iType==3&&iUpOrDown==0)||strName.Right(2)==_T("链")||iType==11)
				{
					iCurUseRow = max(iCurRowDown, iCurRowUp) ;
				}
				
				for (int iColIndex=0; iColIndex<8; iColIndex++)
				{
					CString strCellName = _T("") ;
					strCellName.Format(_T("%s%d"), strColName[iUpOrDown][iColIndex], iCurUseRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					pRange->PutNumberFormat((_variant_t)_T("@")) ;
					pRange->PutFormulaR1C1((_variant_t)strContent[iColIndex]) ;
				}
				if (iType==1)
				{
					iCurUseRow++ ;
					iCurRowDown = iCurRowUp = iCurUseRow ;
				}
				else if (iType==2)
				{
					if (iUpOrDown==0)
					{
						iCurRowDown++ ;
					}
					else
					{
						iCurRowUp = iCurRowDown ;
					}
				}
				else if (iType==3)
				{
					if (iUpOrDown==0)
					{
						iCurRowDown = iCurUseRow ;
						iCurRowUp = iCurRowDown ;
						iCurRowDown++ ;
					}
					else
					{
						iCurRowUp ++ ;
					}					
				}
				else if (iType==11)
				{
					iCurUseRow++ ;
					iCurRowDown = iCurRowUp = iCurUseRow ;
				}
				else if (iType==0)
				{
					if (strName.Right(2)==_T("链"))
					{
						iCurUseRow++ ;
						iCurRowDown = iCurRowUp = iCurUseRow ;
					}
					else if (iUpOrDown==0)
					{
						iCurRowDown++ ;
					}
					else if (iUpOrDown==1)
					{
						iCurRowUp++ ;
					}
				}


				
				iIndex++ ;
				cSetFind.MoveNext() ;
			}
			cSetFind.Close() ;

			acedRestoreStatusBar() ;

		}
		

		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;

		CString strSaveFileName ;
		strSaveFileName.Format(_T("%s\\%s信号机布置表.xls"), strSavePath, m_strXHTitle) ;

		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;

		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));

		pWorkBook->Close() ;
		pWorkBook = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;
	}


}

