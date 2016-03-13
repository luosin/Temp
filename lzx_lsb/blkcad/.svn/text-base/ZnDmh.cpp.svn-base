#include "stdafx.h"
#include "zndmh.h"

#include "DBUtility.h"

#include "ConNode.h"

extern BOOL bFirst ;

CZnDmh::CZnDmh(void)
: m_pDb(NULL)
, m_strPreZ(_T("Z"))
, m_strPreZY(_T("ZY"))
, m_strPreZZH(_T("ZZH"))
, m_iTwoOrFourLine(2)
, m_strStation(_T(""))
, m_strDwgSn(_T(""))
, m_bIsFLInZH(FALSE)
, m_iFxpFrom(0)
, m_strFxpStart(_T(""))
, m_strFxpCurDz(_T(""))
, m_strNPlusInZY(_T(""))
, m_bUseHLP(FALSE)
, m_strProjectName(_T(""))
{
	CBlkUtility::GetBasePath(m_strSysBasePath) ;
}

CZnDmh::~CZnDmh(void)
{
	if (m_pDb!=NULL)
	{
		delete m_pDb ;
		m_pDb = NULL ;
	}
}

// 处理计算输入excel表格
int CZnDmh::ImportDmh(const CString & strXlsFileName)
{
	acedSetStatusBarProgressMeter(_T("导入电码化输入表"), 1, 4) ;

	acedSetStatusBarProgressMeterPos(1) ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strSql, strMsg ;
	CString strSheetNames[9] = {
		_T("电码化类型"),
		_T("YDJ-进路输入"),
		_T("YDJ-股道输入"),
		_T("2XZ-YDJ-联锁信息"),
		_T("2XZ-YDJ-架柜排列表"),
		_T("2XZ-YDJ-架柜内部"),
		_T("复示组合1"),
		_T("FBJ组合"),
		_T("复示组合2")
	} ;
	
	//清除old数据
	CDBUtility::ClearTable(_T("dmh_import")) ;
	CUserSystem mysys ;

	acedSetStatusBarProgressMeterPos(2) ;

	try
	{
		if(bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox(_T("初始化Excel失败,可能是没有安装Excel")) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;
		pWorkBooks->Open((_bstr_t)strXlsFileName) ;
		pWorksheets  = m_pExcel->GetSheets() ;

		//2个表格，一个进路输入，一个股道输入		
		for(int i=0; i<2;i++)
		{
			acedSetStatusBarProgressMeterPos(3+i) ;		

			pSheet = pWorksheets ->GetItem(_variant_t(strSheetNames[i+1])) ;
			CString chStartCol = _T("B") ;
			CString chCurCol = chStartCol ;
			int iFirstRow = 1 ;
			
			BOOL bContinue = TRUE ;
			int iLineIndex = 0 ;
			do 
			{
				CString strCellName = _T(""), strCellTextLine = _T("") ;
				strCellName.Format(_T("%s%d"),chCurCol, iFirstRow) ;
				pRange = pSheet->GetRange((_variant_t)strCellName) ;
				strCellTextLine = (CString)(pRange->GetText().bstrVal) ; //下行线1、下行线2、上行线1、同一股道......
				strCellTextLine.Trim() ;
				if (!strCellTextLine.IsEmpty())
				{
					_variant_t bIsMergeCells = pRange->MergeCells ;
					long iMergeCols = 1 ; //若不是合并格，则仅读取1列
					if (bIsMergeCells.boolVal==-1) //是合并格
					{
						RangePtr pRangeTmp ;
						pRangeTmp = pRange->GetMergeArea() ;
						iMergeCols = pRangeTmp->GetColumns()->GetCount() ;
					}
					for (CString ch=chCurCol; IsColInRange(ch, chCurCol, iMergeCols); IncreaseCol(ch))
					{
						CString strCellTexts[21] ;
						strCellTexts[0] = strCellTextLine ;
						CString strJinluName = _T("") ; //每列进路的命名，比如XJM、SIFM
						//前1~5行，描述性字段
						for (int iRowIndex=2; iRowIndex<=21;iRowIndex++)
						{							
							strCellName.Format(_T("%s%d"), ch, iRowIndex) ;
							pRange = pSheet->GetRange((_variant_t)strCellName) ;
							strCellTexts[iRowIndex-1] = (CString)(pRange->GetText().bstrVal) ;
							strCellTexts[iRowIndex-1].Trim() ;
							//各位置值
							if (iRowIndex>5)
							{
								//第6行，写股道名称（没有分割的股道）
								if (iRowIndex==6) //计算一次进路名称
								{
									int iFindLoc1 = strCellTexts[4].Find(_T("→")) ;
									int iFindLoc2 = strCellTexts[2].Find(_T("(")) ;

									strJinluName.Format(_T("%s%s"), iFindLoc1==-1?strCellTexts[4]:strCellTexts[4].Left(iFindLoc1),iFindLoc2==-1?strCellTexts[2]:strCellTexts[2].Left(iFindLoc2)) ;
								}
								if (strCellTexts[iRowIndex-1].IsEmpty())
								{
									continue;
								}

								strSql.Format(_T("insert into dmh_import(dmh_import_line, dmh_import_lineindex, dmh_import_type, dmh_import_jfm, dmh_import_freq, dmh_import_xhj, dmh_import_guidao, dmh_import_cellcol, dmh_import_cellrow,dmh_import_jinluname, dmh_import_table)\
												 values('%s', %d, '%s', '%s', '%s', '%s', '%s','%s',%d, '%s', %d)"), strCellTexts[0], iLineIndex, strCellTexts[1], strCellTexts[2], strCellTexts[3], strCellTexts[4],  strCellTexts[iRowIndex-1], ch, iRowIndex, strJinluName, i) ;
								ExecSqlString(strSql) ;
							}
						}						
						
					}
					IncreaseCol(chCurCol, iMergeCols) ;
				}
				else
				{
					bContinue = FALSE ;
				}
				iLineIndex++ ;
			} while (bContinue);
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

	acedRestoreStatusBar() ;
	return 0;
}

// 预处理输入表格
int CZnDmh::PreDoDmhImport(const CString & strXlsFileName)
{
	_ApplicationPtr m_pExcel = NULL ;
	//WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strSql, strMsg ;
	CString strSheetNames[9] = {
		_T("电码化类型"),
		_T("YDJ-进路输入"),
		_T("YDJ-股道输入"),
		_T("2XZ-YDJ-联锁信息"),
		_T("2XZ-YDJ-架柜排列表"),
		_T("2XZ-YDJ-架柜内部"),
		_T("复示组合1"),
		_T("FBJ组合"),
		_T("复示组合2")
	} ;

// 	//清除old数据
// 	CDBUtility::ClearTable(_T("dmh_import")) ;
// 	CUserSystem mysys ;

	CStringArray strArrayGuiDaoR, strArrayGuiDaoF, strArrayJMFMFC ; 

	try
	{
		if(bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox(_T("初始化Excel失败,可能是没有安装Excel")) ;
			return -1 ;
		}

		//将数据读入array
		CADORecordset cSet ;
		//填充 strArrayJMFMFC
		strSql.Format(_T("select distinct dmh_import_jfm, dmh_import_cellcol, dmh_import_table from dmh_import order by dmh_import_table,dmh_import_cellcol asc")) ;
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
			while(!cSet.IsEOF())
			{
				CString strJFM ;
				cSet.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;
				strJFM.Trim() ;
				if (!strJFM.IsEmpty())
				{
					strArrayJMFMFC.Add(strJFM) ;
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}

		//填充 strArrayGuiDaoR, strArrayGuiDaoF
		strSql.Format(_T("select * from dmh_import order by id asc")) ;
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
			
			while(!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				cSet.GetFieldValue(_T("dmh_import_guidao"), strGuiDaoTmp) ;
				if (strGuiDaoTmp.Right(1)==_T("+"))
				{
					strArrayGuiDaoR.Add(strGuiDaoTmp) ;
				}
				else
				{
					strArrayGuiDaoF.Add(strGuiDaoTmp) ;
				}

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}		

		pWorkBook = m_pExcel->GetWorkbooks()->Open((_bstr_t)strXlsFileName) ;
		pWorksheets = pWorkBook->GetSheets() ;
		//pWorksheets  = m_pExcel->GetSheets() ;

		//填充联锁信息表
		pSheet = pWorksheets ->GetItem(_variant_t(strSheetNames[3])) ;

		CString strCellName, strCellName2, strCellText ;

		int iLsXxStartRow = 6 ; //读写起始行
		TCHAR chLsXxRStartCol = _T('A'),chLsXxFStartCol = _T('I') ; //读写起始列
		int iGuiDaoRCount = strArrayGuiDaoR.GetCount() ;
		for (int iGuiDaoR=0; iGuiDaoR<iGuiDaoRCount; iGuiDaoR++)
		{
			CString strGuiDaoRTmp ;
			strGuiDaoRTmp = strArrayGuiDaoR.GetAt(iGuiDaoR) ;			
			strCellName.Format(_T("%c%d"), chLsXxRStartCol, iLsXxStartRow+iGuiDaoR) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
			pRange->PutFormulaR1C1((_variant_t) strGuiDaoRTmp) ;
		}
		int iGuiDaoFCount = strArrayGuiDaoF.GetCount() ;
		for (int iGuiDaoF=0; iGuiDaoF<iGuiDaoFCount; iGuiDaoF++)
		{
			CString strGuiDaoFTmp ;
			strGuiDaoFTmp = strArrayGuiDaoF.GetAt(iGuiDaoF) ;			
			strCellName.Format(_T("%c%d"), chLsXxFStartCol, iLsXxStartRow+iGuiDaoF) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
			pRange->PutFormulaR1C1((_variant_t) strGuiDaoFTmp) ;
		}

		//填充架柜排列表
		pSheet = pWorksheets ->GetItem(_variant_t(strSheetNames[4])) ;
		int iJgplRowsPerTb = 15 ; //每个表的行
		int iJgplColsPerJMFMFC = 6 ;//每行写JM FM FC的单元格有几列
		int iJgplColsPerR = 5 ;//每行写受电端轨道的单元格有几列
		int iJgplColsPerF = 3 ;//每行写送电端轨道的单元格有几列
		int iJgplStartRow = 7 ; //读写起始行
		TCHAR chJgplStartCol = _T('A') ; //读写起始列
		//填充JM/FM/FC等信息
		int iJMFMFCCount = strArrayJMFMFC.GetCount() ;
		//需要多少行ZH来填充JMFMFC和接收、发送轨道
		int iJMFMFCNeedZHRow = iJMFMFCCount/iJgplColsPerJMFMFC+(iJMFMFCCount%iJgplColsPerJMFMFC==0?0:1) ;
		int iGuiDaoRNeedZHRow = iGuiDaoRCount/iJgplColsPerR+(iGuiDaoRCount%iJgplColsPerR==0?0:1) ;
		int iGuiDaoFNeedZHRow = iGuiDaoFCount/iJgplColsPerF+(iGuiDaoFCount%iJgplColsPerF==0?0:1) ;		
		int iJgplTotalNeedZHRow = iJMFMFCNeedZHRow + iGuiDaoRNeedZHRow + iGuiDaoFNeedZHRow ;
		//需要几个ZH柜
		int iZHCount = iJgplTotalNeedZHRow/9+(iJgplTotalNeedZHRow%9==0?0:1) ;
		for (int iZHIndex=0; iZHIndex<iZHCount; iZHIndex++)
		{			
			//ZHn-10
			strCellName.Format(_T("%c%d"), chJgplStartCol, iJgplStartRow+iZHIndex*iJgplRowsPerTb) ;
			strCellName2.Format(_T("%c%d"), chJgplStartCol, iJgplStartRow+3+iZHIndex*iJgplRowsPerTb) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, (_variant_t)strCellName2) ;
			pRange->Merge(vtMissing) ;
			strCellText.Format(_T("ZH%d-10"), iZHIndex+1) ;
			pRange->PutFormulaR1C1((_variant_t)strCellText) ;
			//MGFL-T
			strCellName.Format(_T("%c%d"), chJgplStartCol+1, iJgplStartRow+iZHIndex*iJgplRowsPerTb) ;
			strCellName2.Format(_T("%c%d"), chJgplStartCol+1, iJgplStartRow+3+iZHIndex*iJgplRowsPerTb) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, (_variant_t)strCellName2) ;
			pRange->Merge(vtMissing) ;			
			pRange->PutFormulaR1C1((_variant_t)_T("MGFL-T")) ;
			//防雷1~36
			for (int iFlIndex=0; iFlIndex<36; iFlIndex++)
			{
				strCellName.Format(_T("%c%d"), chJgplStartCol+2+iFlIndex/2, iJgplStartRow+(iFlIndex%2==0?0:2)+iZHIndex*iJgplRowsPerTb) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				strCellText.Format(_T("%d"), iFlIndex+1) ;
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;
				//格式
				MSExcel::FontPtr pFont = pRange->GetFont();
				pFont->PutSize((_variant_t)((long)8)) ;
				pRange->PutHorizontalAlignment((_variant_t)((long)MSExcel::xlHAlignCenter)) ;
				pRange->PutVerticalAlignment((_variant_t)((long)MSExcel::xlVAlignCenter)) ;
			}
			//ZHn-9~ZHn-1
			for (int iZHRowIndex=9; iZHRowIndex>0; iZHRowIndex--)
			{
				strCellName.Format(_T("%c%d"), chJgplStartCol, iJgplStartRow+3+9-iZHRowIndex+1+iZHIndex*iJgplRowsPerTb) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				strCellText.Format(_T("ZH%d-%d"), iZHIndex+1, iZHRowIndex) ;
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;
			}
		}
		//填充JM FM FC 等
		for (int iJMFMFCRowIndex=0; iJMFMFCRowIndex<iJMFMFCNeedZHRow; iJMFMFCRowIndex++)
		{
			int iCurRow = iJgplStartRow+4+iJMFMFCRowIndex+iJMFMFCRowIndex/9*iJgplRowsPerTb ;
			//ZPW.TFGZ
			strCellName.Format(_T("%c%d"), chJgplStartCol+1, iCurRow) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
			pRange->PutFormulaR1C1((_variant_t)_T("ZPW.TFGZ")) ;
			//JM(1)FM(1)....
			int iMaxCol = ((iJMFMFCRowIndex==iJMFMFCNeedZHRow-1)?(iJMFMFCCount-iJMFMFCRowIndex*iJgplColsPerJMFMFC):iJgplColsPerJMFMFC) ;
			for (int iJMFMFCColIndex=0; iJMFMFCColIndex<iMaxCol; iJMFMFCColIndex++)
			{
				strCellText = strArrayJMFMFC.GetAt(iJMFMFCRowIndex*iJgplColsPerJMFMFC+iJMFMFCColIndex) ;
				strCellName.Format(_T("%c%d"), chJgplStartCol+1+iJMFMFCColIndex+1, iCurRow) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;
			}
		}
		int iFlInputIndex = 0 ; //在当前柜子防雷填充的区段的序号，换柜子就置0
		int iPreZH = 0 ; //记录前次ZH柜的序号，和当前序号对比，若变化了则iFlInputIndex置0
		//填充受电端区段
		for (int iGuiDaoRIndex=0; iGuiDaoRIndex<iGuiDaoRNeedZHRow; iGuiDaoRIndex++)
		{
			int iCurZH = (iJMFMFCNeedZHRow+iGuiDaoRIndex)/9 ; //当前的ZH柜序号
			if (iPreZH!=iCurZH)
			{
				iFlInputIndex = 0 ;
			}
			//当前行号
			int iCurRow = iJgplStartRow+4+(iJMFMFCNeedZHRow+iGuiDaoRIndex)/9*iJgplRowsPerTb+(iJMFMFCNeedZHRow+iGuiDaoRIndex)%9 ;
			//MGL-R
			strCellName.Format(_T("%c%d"), chJgplStartCol+1, iCurRow) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
			pRange->PutFormulaR1C1((_variant_t)_T("MGL-R")) ;
			//受电端轨道....
			int iMaxCol = ((iGuiDaoRIndex==iGuiDaoRNeedZHRow-1)?(iGuiDaoRCount-iGuiDaoRIndex*iJgplColsPerR):iJgplColsPerR) ;
			for (int iGuiDaoRColIndex=0; iGuiDaoRColIndex<iMaxCol; iGuiDaoRColIndex++)
			{
				strCellText = strArrayGuiDaoR.GetAt(iGuiDaoRIndex*iJgplColsPerR+iGuiDaoRColIndex) ;
				strCellName.Format(_T("%c%d"), chJgplStartCol+1+iGuiDaoRColIndex+1, iCurRow) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;

				//填充防雷（ZHn-10）
				strCellName.Format(_T("%c%d"), chJgplStartCol+2+iFlInputIndex/2, iJgplStartRow+(iFlInputIndex%2==0?1:3)+iCurZH*iJgplRowsPerTb) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;
				iFlInputIndex++ ;
			}
            //设置next一个单元格为×
			for (int iMarkColIndex=0; iMarkColIndex<(iJgplColsPerJMFMFC-iJgplColsPerR); iMarkColIndex++)
			{
				strCellName.Format(_T("%c%d"), chJgplStartCol+2+iJgplColsPerR+iMarkColIndex, iCurRow) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
				BordersPtr pBorders = pRange->GetBorders() ;
				BorderPtr pBorder =	pBorders->GetItem(MSExcel::xlDiagonalDown) ;
				pBorder->PutLineStyle(MSExcel::xlContinuous) ;
				pBorder = pBorders->GetItem(MSExcel::xlDiagonalUp) ;
				pBorder->PutLineStyle(MSExcel::xlContinuous) ;	
			}	
			iPreZH = iCurZH ;
		}
        //填充送电端区段
		for (int iGuiDaoFIndex=0; iGuiDaoFIndex<iGuiDaoFNeedZHRow; iGuiDaoFIndex++)
		{
			int iCurZH = (iJMFMFCNeedZHRow+iGuiDaoRNeedZHRow+iGuiDaoFIndex)/9 ; //当前的ZH柜序号
			if (iPreZH!=iCurZH)
			{
				iFlInputIndex = 0 ;
			}
			//当前行号
			int iCurRow = iJgplStartRow+4+(iJMFMFCNeedZHRow+iGuiDaoRNeedZHRow+iGuiDaoFIndex)/9*iJgplRowsPerTb+(iJMFMFCNeedZHRow+iGuiDaoRNeedZHRow+iGuiDaoFIndex)%9 ;
			//MGL-F
			strCellName.Format(_T("%c%d"), chJgplStartCol+1, iCurRow) ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
			pRange->PutFormulaR1C1((_variant_t)_T("MGL-F")) ;
			//受电端轨道....
			int iMaxCol = ((iGuiDaoFIndex==iGuiDaoFNeedZHRow-1)?(iGuiDaoFCount-iGuiDaoFIndex*iJgplColsPerF):iJgplColsPerF) ;
			for (int iGuiDaoFColIndex=0; iGuiDaoFColIndex<iMaxCol; iGuiDaoFColIndex++)
			{
				strCellText = strArrayGuiDaoF.GetAt(iGuiDaoFIndex*iJgplColsPerF+iGuiDaoFColIndex) ;
				strCellName.Format(_T("%c%d"), chJgplStartCol+1+iGuiDaoFColIndex+1, iCurRow) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;

				//填充防雷（ZHn-10）
				strCellName.Format(_T("%c%d"), chJgplStartCol+2+iFlInputIndex/2, iJgplStartRow+(iFlInputIndex%2==0?1:3)+iCurZH*iJgplRowsPerTb) ;				
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
				pRange->PutFormulaR1C1((_variant_t)strCellText) ;
				iFlInputIndex++ ;
			}
			//设置next三个单元格为×
			for (int iMarkColIndex=0; iMarkColIndex<(iJgplColsPerJMFMFC-iJgplColsPerF); iMarkColIndex++)
			{
				strCellName.Format(_T("%c%d"), chJgplStartCol+2+iJgplColsPerF+iMarkColIndex, iCurRow) ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;	
				BordersPtr pBorders = pRange->GetBorders() ;
				BorderPtr pBorder =	pBorders->GetItem(MSExcel::xlDiagonalDown) ;
				pBorder->PutLineStyle(MSExcel::xlContinuous) ;
				pBorder = pBorders->GetItem(MSExcel::xlDiagonalUp) ;
				pBorder->PutLineStyle(MSExcel::xlContinuous) ;
			}

			iPreZH = iCurZH ;
		}

		pWorkBook->Save() ;

		pWorkBook->Close() ;
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
	return 0;
}

// 将输入表格读回数据库（程序生成初稿，经人工更改，最后再导入数据库）
int CZnDmh::ReImportDmh(const CString & strXlsFileName)
{
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strSql, strMsg ;
	CString strSheetNames[9] = {
		_T("电码化类型"),
			_T("YDJ-进路输入"),
			_T("YDJ-股道输入"),
			_T("2XZ-YDJ-联锁信息"),
			_T("2XZ-YDJ-架柜排列表"),
			_T("2XZ-YDJ-架柜内部"),
			_T("复示组合1"),
			_T("FBJ组合"),
			_T("复示组合2")
	} ;

	//清除old数据
	CString strDBTbls[] = {_T("dmh_zzh"), _T("dmh_lsxxr"), _T("dmh_lsxxf")} ;
	for (int iTb=0; iTb<3; iTb++)
	{
		CDBUtility::ClearTable(strDBTbls[iTb]) ;
	}

// 	CUserSystem mysys ;

	try
	{
		if(bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox(_T("初始化Excel失败,可能是没有安装Excel")) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBooks->Open((_bstr_t)strXlsFileName) ;

		pWorksheets  = m_pExcel->GetSheets() ;

		//begin 读入2XZ-YDJ-架柜排列表
		pSheet = pWorksheets ->GetItem(_variant_t(strSheetNames[4])) ;

		TCHAR chStartCol = _T('A') ; //起始列
		TCHAR chCurCol = chStartCol ;
		int iStartRow = 7 ; //起始行
		int iCurRow = iStartRow ;
		int iReadNullCell = 0 ; //连读5行都没有读到类似ZHn-10的文字，就不再继续读了			
		BOOL bContinue = TRUE ;			
		do 
		{
			CString strCellName = _T(""), strCellText = _T("") ;
			strCellName.Format(_T("%c%d"),chStartCol, iCurRow) ;
			pRange = pSheet->GetRange((_variant_t)strCellName) ;
			strCellText = (CString)(pRange->GetText().bstrVal) ;
			strCellText.Trim() ;

			acutPrintf(_T("\n%s:%s"),strCellName, strCellText) ;

			if (CBlkUtility::IsStrMatch(_T("^ZH\\d+-10$"), strCellText))
			{
				iReadNullCell = 0 ; 

				CString strLayer = strCellText ; //ZH1-10
				int iShelf = 0, iLayer = 0, iBox = 0 ;
				CString strShelf ;
				CBlkUtility::RegReplace(_T("^ZH(\\d+)-10$"), _T("$1"), strLayer, strShelf) ;
				iShelf = _tstoi(strShelf) ;

				//读取ZHn-10层的FL
				for (int iFl=0; iFl<2; iFl++)
				{
					iCurRow = iCurRow+1+iFl ;
					for (chCurCol=chStartCol+2; chCurCol<chStartCol+2+18; chCurCol++)
					{
						strCellName.Format(_T("%c%d"),chCurCol, iCurRow) ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						strCellText = (CString)(pRange->GetText().bstrVal) ;
						strCellText.Trim() ;

						if (!strCellText.IsEmpty())
						{								
							iBox = int(chCurCol-chStartCol-2)*2+iFl+1 ;
							strSql.Format(_T("insert into dmh_zzh(dmh_zzh_shelf, dmh_zzh_layer, dmh_zzh_box, dmh_zzh_content)\
											 values(%d, %d, %d, '%s')"), iShelf, 10, iBox, strCellText) ;
							ExecSqlString(strSql) ;
						}
					}
				}

				//读取ZHn-9~1层的轨道
				for (int iLayerIndex=1; iLayerIndex<=9; iLayerIndex++)
				{
					iCurRow++ ;
					iLayer = 10-iLayerIndex ;
					for (chCurCol=chStartCol+1; chCurCol<chStartCol+11; chCurCol++)
					{
						strCellName.Format(_T("%c%d"),chCurCol, iCurRow) ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						strCellText = (CString)(pRange->GetText().bstrVal) ;
						strCellText.Trim() ;

						if (!strCellText.IsEmpty())
						{								
							iBox = int(chCurCol-chStartCol-1) ;
							strSql.Format(_T("insert into dmh_zzh(dmh_zzh_shelf, dmh_zzh_layer, dmh_zzh_box, dmh_zzh_content)\
											 values(%d, %d, %d, '%s')"), iShelf, iLayer, iBox, strCellText) ;
							ExecSqlString(strSql) ;
						}
					}
				}

				iCurRow+=3 ;
			}
			else
			{	
				iCurRow++ ;
				iReadNullCell++ ;
			}

			if (iReadNullCell==3)
			{
				bContinue = FALSE ;
			}				

		} while (bContinue);
		//end 读入2XZ-YDJ-架柜排列表

		//begin 读入2XZ-YDJ-联锁信息

		pSheet = pWorksheets ->GetItem(_variant_t(strSheetNames[3])) ;

		//受电端
		iStartRow = 6 ;
		iCurRow = iStartRow ;
		chStartCol = _T('A') ;
		chCurCol = chStartCol ;
		iReadNullCell = 0 ;

		bContinue = TRUE ;
		do 
		{
			CString strCellName = _T(""), strCellText = _T("") ;
			strCellName.Format(_T("%c%d"),chStartCol, iCurRow) ;
			pRange = pSheet->GetRange((_variant_t)strCellName) ;
			strCellText = (CString)(pRange->GetText().bstrVal) ;
			strCellText.Trim() ;

			acutPrintf(_T("\n%s:%s"),strCellName, strCellText) ;

			if (!strCellText.IsEmpty())
			{
				iReadNullCell = 0 ;

				//读取一行

				CStringArray strArrayCellText ;
				for (chCurCol=chStartCol; chCurCol<chStartCol+8; chCurCol++)
				{
					strCellName.Format(_T("%c%d"),chCurCol, iCurRow) ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.Trim() ;
					strArrayCellText.Add(strCellText) ;
				}								

				strSql.Format(_T("insert into dmh_lsxxr(dmh_lsxxr_guidao, dmh_lsxxr_circuittype, dmh_lsxxr_gjfpoint, dmh_lsxxr_gjfside,dmh_lsxxr_gside1, dmh_lsxxr_gside2, dmh_lsxxr_fxploc1, dmh_lsxxr_fxploc2)\
								 values('%s','%s','%s','%s','%s','%s','%s','%s')"), strArrayCellText.GetAt(0),strArrayCellText.GetAt(1),strArrayCellText.GetAt(2),strArrayCellText.GetAt(3),strArrayCellText.GetAt(4),strArrayCellText.GetAt(5),strArrayCellText.GetAt(6),strArrayCellText.GetAt(7)) ;
				ExecSqlString(strSql) ;
			}
			else
			{
				iReadNullCell++ ;
			}

			iCurRow++ ;

			if (iReadNullCell==2)
			{
				bContinue = FALSE ;
			}	

		} while (bContinue);

		//送电端
		iStartRow = 6 ;
		iCurRow = iStartRow ;
		chStartCol = _T('J') ;
		chCurCol = chStartCol ;
		iReadNullCell = 0 ;

		bContinue = TRUE ;
		do 
		{
			CString strCellName = _T(""), strCellText = _T("") ;
			strCellName.Format(_T("%c%d"),chStartCol, iCurRow) ;
			pRange = pSheet->GetRange((_variant_t)strCellName) ;
			strCellText = (CString)(pRange->GetText().bstrVal) ;
			strCellText.Trim() ;

			acutPrintf(_T("\n%s:%s"),strCellName, strCellText) ;

			if (!strCellText.IsEmpty())
			{
				iReadNullCell = 0 ;

				//读取一行

				CStringArray strArrayCellText ;
				for (chCurCol=chStartCol; chCurCol<chStartCol+5; chCurCol++)
				{
					strCellName.Format(_T("%c%d"),chCurCol, iCurRow) ;
					pRange = pSheet->GetRange((_variant_t)strCellName) ;
					strCellText = (CString)(pRange->GetText().bstrVal) ;
					strCellText.Trim() ;
					strArrayCellText.Add(strCellText) ;
				}								

				strSql.Format(_T("insert into dmh_lsxxf(dmh_lsxxf_guidao, dmh_lsxxf_power1, dmh_lsxxf_power2, dmh_lsxxf_fxploc1,dmh_lsxxf_fxploc2)\
								 values('%s','%s','%s','%s','%s')"), strArrayCellText.GetAt(0),strArrayCellText.GetAt(1),strArrayCellText.GetAt(2),strArrayCellText.GetAt(3),strArrayCellText.GetAt(4)) ;
				ExecSqlString(strSql) ;
			}
			else
			{
				iReadNullCell++ ;
			}

			iCurRow++ ;

			if (iReadNullCell==2)
			{
				bContinue = FALSE ;
			}	

		} while (bContinue);

		//end 读入2XZ-YDJ-联锁信息

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
		AfxMessageBox(_T("未知的错误 in ReadZzhFromXlsToDb !"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	return 0;
}

//计算电路图
int CZnDmh::ComputeCircuit(void)
{
	return 0;
}

// 读取strGuiDao在ZZH表中的信息
int CZnDmh::GetZzhInfo(const CString & strGuiDao, int & iShelf_FL, int & iLayer_FL, int & iBox_FL, int & iShelf, int & iLayer, int & iBox)
{
	int iRet = -1 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//防雷信息
		strSql.Format(_T("select * from dmh_in_zzhcell where dmh_in_zzhcell_cellcontent='%s' and dmh_in_zzhcell_layerid in(select id from dmh_in_zzhlayer where dmh_in_zzhlayer_layerblock='dmh_box_zzh_mgfl-t')"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iRet = cSet.GetRecordCount() ;

			if (iRet==1)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_in_zzhcell_shelfnum"), iShelf_FL) ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_layernum"), iLayer_FL) ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_boxnum"), iBox_FL) ;				
			}
			cSet.Close() ;
		}
		//ZH_LOC
		strSql.Format(_T("select * from dmh_in_zzhcell where dmh_in_zzhcell_cellcontent='%s' and dmh_in_zzhcell_layerid not in (select id from dmh_in_zzhlayer where dmh_in_zzhlayer_layerblock in ('dmh_box_zzh_mgfl-t','dmh_box_zzh_tfgz'))"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iRet = cSet.GetRecordCount() ;

			if (iRet==1)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_in_zzhcell_shelfnum"), iShelf) ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_layernum"), iLayer) ;
				cSet.GetFieldValue(_T("dmh_in_zzhcell_boxnum"), iBox) ;
				
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
		AfxMessageBox(_T("unknown error in GetZzhInfo")) ;
	} 

	return iRet;
}

// 读取strGuiDao在联锁信息表中相关信息-受电端
int CZnDmh::GetLsInfoR(const CString & strGuiDao, CString & strCircuitType, CString & strGJFPoint, CString & strGJFSide, CString & strGSide1, CString & strGSide2, CString & strFLoc1, CString & strFLoc2)
{
	int iRet = -1 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		strSql.Format(_T("select * from dmh_lsxxr where dmh_lsxxr_guidao='%s'"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iRet = cSet.GetRecordCount() ;
			if (iRet>0)
			{
				cSet.MoveFirst() ;
				cSet.GetFieldValue(_T("dmh_lsxxr_circuittype"), strCircuitType) ;
				cSet.GetFieldValue(_T("dmh_lsxxr_gjfpoint"), strGJFPoint) ;
				cSet.GetFieldValue(_T("dmh_lsxxr_gjfside"), strGJFSide) ;
				cSet.GetFieldValue(_T("dmh_lsxxr_gside1"), strGSide1) ;
				cSet.GetFieldValue(_T("dmh_lsxxr_gside2"), strGSide2) ;
				cSet.GetFieldValue(_T("dmh_lsxxr_fxploc1"), strFLoc1) ;
				cSet.GetFieldValue(_T("dmh_lsxxr_fxploc2"), strFLoc2) ;
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
		AfxMessageBox(_T("unknown error in GetLsInfoR")) ;
	} 

	return iRet;
}

// 读取联锁信息表中相关信息-送电端
int CZnDmh::GetLsInfoF(const CString & strGuiDao, CString & strPower1, CString & strPower2, CString & strFLoc1, CString & strFLoc2)
{
	int iRet = -1 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		strSql.Format(_T("select * from dmh_lsxxf where dmh_lsxxf_guidao='%s'"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iRet = cSet.GetRecordCount() ;
			if (iRet>0)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_lsxxf_power1"), strPower1) ;
				cSet.GetFieldValue(_T("dmh_lsxxf_power2"), strPower2) ;
				cSet.GetFieldValue(_T("dmh_lsxxf_fxploc1"), strFLoc1) ;
				cSet.GetFieldValue(_T("dmh_lsxxf_fxploc2"), strFLoc2) ;
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
		AfxMessageBox(_T("unknown error in GetLsInfoF")) ;
	} 

	return iRet;
}

// 读取组合内部侧面端子
int CZnDmh::GetZhNbNode(const CString &strZhNb, const CString &strLocType, const CString & strNodeName,	int iLoc, CString & strNode)
{
	int iRet = -1 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		strSql.Format(_T("select * from dmh_def_nodes where dmh_nodes_zhnb='%s' and dmh_nodes_loctype='%s' and dmh_nodes_nodename='%s' and dmh_nodes_loc=%d"), strZhNb, strLocType, strNodeName, iLoc) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iRet = cSet.GetRecordCount() ;
			if (iRet>0)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_nodes_node"), strNode) ;
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
		AfxMessageBox(_T("unknown error in GetZhNbNode")) ;
	} 

	return iRet;
}

BOOL CZnDmh::InsertDwgToDb(const CString strDwgFilePath, AcDbDatabase *& pDb, const AcGeMatrix3d & xform, const CMapStringToString & mapTypeToStr, const CMapStringToString & mapToAddXdata, const CString strRepType, int iIdAdd)
{
	BOOL bRet = TRUE ;

	AcDbObjectId id;
	AcDbObjectIdArray list;
	AcDbDatabase extDatabase(Adesk::kFalse);
	CUserSystem mysys ;
	mysys.m_pDb = &extDatabase ;

	if (Acad::eOk != extDatabase.readDwgFile(strDwgFilePath))
	{
		acutPrintf(_T("\n读取模板文件 %s 失败!"), strDwgFilePath);
		return FALSE ;
	}
	AcDbBlockTable* pBT;
	if (Acad::eOk != extDatabase.getSymbolTable(pBT, AcDb::kForRead))
	{
		acutPrintf(_T("\n从模板文件 %s 读取块表失败!"), strDwgFilePath);
		return FALSE ;
	}
	AcDbBlockTableRecord* pBTR;
	Acad::ErrorStatus es = pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForRead);
	pBT->close();
	if (Acad::eOk != es) 
	{
		acutPrintf(_T("\ngetting model space from: %s 失败!"), strDwgFilePath);
		return FALSE ;
	}

	AcDbBlockTableRecordIterator* pIT;
	if (Acad::eOk != pBTR->newIterator(pIT)) 
	{
		acutPrintf(_T("\nError iterating model space of DWG"));
		pBTR->close();
		return FALSE ;
	}

	for (; !pIT->done(); pIT->step()) 
	{
		if (Acad::eOk == pIT->getEntityId(id))
		{
			list.append(id);

			// There is a bug in ARX that causes extension dictionaries
			// to appear to be soft owners of their contents.  This causes
			// the contents to be skipped during wblock.  To fix this we
			// must explicitly tell the extension dictionary to be a hard
			// owner of it's entries.
			//
			AcDbEntity *pEnt;
			if (Acad::eOk == pIT->getEntity(pEnt, AcDb::kForWrite)) 
			{  
				//begin 增加实体ID数值，防止重复
				CString strAppNames[7] = {
					_T("ID"),_T("CON_1_ID"),_T("CON_2_ID"),_T("PARENT_ID"),_T("CON_1_CABLE_ID"),
					_T("CON_2_CABLE_ID"), _T("CON_GD_ID")
				} ;				
				for (int i=0; i<7; i++)
				{
					long lId = 0 ;
					if (mysys.GetXdata(pEnt,strAppNames[i], lId))
					{
						long lNewId = lId+iIdAdd ;
						mysys.SetXdata(pEnt,strAppNames[i], lNewId) ;
					}
				}
				//end 增加实体ID数值，防止重复

				//begin 替换相关文本
				if (_tcscmp(pEnt->isA()->name(), _T("AcDbText"))==0)
				{
					AcDbText *pText = (AcDbText*)pEnt ;
					CString strType ;
					if (mysys.GetXdata(pEnt, strRepType, strType))
					{
						CString strValue ;	
						//替换相关文本
						if(mapTypeToStr.Lookup(strType, strValue))
						{
							if (strValue.IsEmpty())
							{
								strValue = _T("XXX") ;
								pText->setColorIndex(2) ;
							}
							pText->setTextString(strValue) ;
						}
					}
				}
				//end 替换相关文本

				//being 增加xdata 不仅文本，块的xdata也可以增加
				CString strType ;
				CString strAddApps[2] = {_T("TYPE"), _T("MY_SN")} ; //有些端子比如GCJ_0000块中GCJ1_22端子05-18，只设定了MY_SN,没有TYPE
				for (int iAddAppIndex=0; iAddAppIndex<2; iAddAppIndex++)
				{
					if (mysys.GetXdata(pEnt, strAddApps[iAddAppIndex], strType))
					{
						CString strValue ;		
						//增加xdata
						//strType,例:NODE_A18
						//strValue,例：MY_SN,NGL_NODE_A18_0;PAR_SN,ZJC_GCJ1_22
						if (mapToAddXdata.Lookup(strType, strValue))
						{
							CStringArray strArrayXdata ;
							CBlkUtility::DivideString(strValue, _T(";"), strArrayXdata) ;
							for (int iXdataIndex=0; iXdataIndex<strArrayXdata.GetCount(); iXdataIndex++)
							{
								CString strAppXdata = strArrayXdata.GetAt(iXdataIndex) ;
								strAppXdata.Trim() ;
								CString strApp, strXdata ;
								int iMarkLoc = strAppXdata.Find(_T(",")) ;
								strApp = strAppXdata.Left(iMarkLoc) ;
								strXdata = strAppXdata.Mid(iMarkLoc+1, strAppXdata.GetLength()-iMarkLoc) ;

								mysys.SetXdata(pEnt, strApp, strXdata) ;
							}
						}
					}

				}
				//end 增加xdata 不仅文本，块的xdata也可以增加

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
	if (Acad::eOk != extDatabase.wblock(pTempDb, list, pt_3d))
	{
		acutPrintf(_T("\nwblock failed!"));
		return FALSE ;
	}		
	if (Acad::eOk != pDb->insert(xform, pTempDb))
	{
		acutPrintf(_T("insert failed!"));
		return FALSE ;
	}	

	delete pTempDb;

	return bRet ;
}

BOOL CZnDmh::InsertDwgToDb1(const CString strDwgFilePath, AcDbDatabase *& pDb, const AcGeMatrix3d & xform, const CMapStringToString & mapToReplaceXdata, const CMapStringToString & mapTypeToStr, const CMapStringToString & mapToAddXdata, const CString strRepType, int iIdAdd)
{
	BOOL bRet = TRUE ;

	AcDbObjectId id;
	AcDbObjectIdArray list;
	AcDbDatabase extDatabase(Adesk::kFalse);
	CUserSystem mysys ;
	mysys.m_pDb = &extDatabase ;

	if (Acad::eOk != extDatabase.readDwgFile(strDwgFilePath))
	{
		acutPrintf(_T("\n读取模板文件 %s 失败!"), strDwgFilePath);
		return FALSE ;
	}
	AcDbBlockTable* pBT;
	if (Acad::eOk != extDatabase.getSymbolTable(pBT, AcDb::kForRead))
	{
		acutPrintf(_T("\n从模板文件 %s 读取块表失败!"), strDwgFilePath);
		return FALSE ;
	}
	AcDbBlockTableRecord* pBTR;
	Acad::ErrorStatus es = pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForRead);
	pBT->close();
	if (Acad::eOk != es) 
	{
		acutPrintf(_T("\ngetting model space from: %s 失败!"), strDwgFilePath);
		return FALSE ;
	}

	AcDbBlockTableRecordIterator* pIT;
	if (Acad::eOk != pBTR->newIterator(pIT)) 
	{
		acutPrintf(_T("\nError iterating model space of DWG"));
		pBTR->close();
		return FALSE ;
	}

	for (; !pIT->done(); pIT->step()) 
	{
		if (Acad::eOk == pIT->getEntityId(id))
		{
			list.append(id);

			// There is a bug in ARX that causes extension dictionaries
			// to appear to be soft owners of their contents.  This causes
			// the contents to be skipped during wblock.  To fix this we
			// must explicitly tell the extension dictionary to be a hard
			// owner of it's entries.
			//
			AcDbEntity *pEnt;
			if (Acad::eOk == pIT->getEntity(pEnt, AcDb::kForWrite)) 
			{ 
				//测试用，当时读到n+1d_zj，下面代码过不去，经查，发现是my_sn定义FB_[JDQINDEX_FBJ_FBJ_31，应为FB_[JDQINDEX_FBJ]_FBJ_31
// 				 CString strEntTypeName ;
// 				 strEntTypeName.Format(_T("%s"), pEnt->isA()->name()) ;
// 				 UINT iEntHandle = mysys.GetHandle(pEnt) ;
// 				 acutPrintf(_T("\n%s-%d"), strEntTypeName, iEntHandle) ;
// 				 if (strEntTypeName==_T("AcDbText"))
// 				 {
// 				 	AcDbText *pText1 = AcDbText::cast(pEnt) ;
// 				 	AcGePoint3d pt_3d_text = pText1->position() ;
// 				 	acutPrintf(_T("-(%.3f,%.3f)"), pt_3d_text.x, pt_3d_text.y) ;
// 				 }



				//begin 增加实体ID数值，防止重复
				CString strAppNames[7] = {
					_T("ID"),_T("CON_1_ID"),_T("CON_2_ID"),_T("PARENT_ID"),_T("CON_1_CABLE_ID"),
						_T("CON_2_CABLE_ID"),_T("CON_GD_ID")
				} ;				
				for (int i=0; i<7; i++)
				{
					long lId = 0 ;
					if (mysys.GetXdata(pEnt,strAppNames[i], lId))
					{
						long lNewId = lId+iIdAdd ;
						mysys.SetXdata(pEnt,strAppNames[i], lNewId) ;
					}
				}
				//end 增加实体ID数值，防止重复

				CString strType ;

				CString strToRepApps[5] = {_T("TYPE"), _T("PAR_SN"), _T("MY_SN"), _T("FIRST_SN"), _T("SECOND_SN")} ;
				for (int iRep=0; iRep<5; iRep++)
				{
					strType = _T("") ;
					if(mysys.GetXdata(pEnt, strToRepApps[iRep], strType)) //例：strType=_T("CD_[GJFINDEX]GJF1_[JDQJDGROUP]1")
					{
						CStringArray strArrayFind ;
						int iFindSum = CBlkUtility::FindString(strType, _T("["), _T("]"), strArrayFind) ;
						if (iFindSum>0)
						{
							for (int iFindIndex=0; iFindIndex<iFindSum; iFindIndex++)
							{
								CString strToReplace ;
								strToReplace = strArrayFind.GetAt(iFindIndex) ;
								CString strRepValue ;
								if (mapToReplaceXdata.Lookup(strToReplace, strRepValue))
								{
									strToReplace.Insert(0, _T("[")) ;
									strToReplace.Append(_T("]")) ;
									strType.Replace(strToReplace, strRepValue) ;									
								}
							}
							mysys.SetXdata(pEnt, strToRepApps[iRep], strType) ;
						}
					}
				}

				//begin 替换相关文本
				if (_tcscmp(pEnt->isA()->name(), _T("AcDbText"))==0)
				{
					AcDbText *pText = (AcDbText*)pEnt ;
					strType = _T("") ;
					if (mysys.GetXdata(pEnt, strRepType, strType))
					{
						CString strValue ;	
						//替换相关文本
						if(mapTypeToStr.Lookup(strType, strValue))
						{
							if (strValue.IsEmpty())
							{
								strValue = _T("XXX") ;
								pText->setColorIndex(2) ;
							}
							pText->setTextString(strValue) ;
						}
					}
				}
				//end 替换相关文本

				//being 增加xdata 不仅文本，块的xdata也可以增加
				strType = _T("") ;
				CString strAddApps[2] = {_T("TYPE"), _T("MY_SN")} ; //有些端子比如GCJ_0000块中GCJ1_22端子05-18，只设定了MY_SN,没有TYPE
				for (int iAddAppIndex=0; iAddAppIndex<2; iAddAppIndex++)
				{
					if (mysys.GetXdata(pEnt, strAddApps[iAddAppIndex], strType))
					{
						CString strValue ;		
						//增加xdata
						//strType,例:NODE_A18
						//strValue,例：MY_SN,NGL_NODE_A18_0;PAR_SN,ZJC_GCJ1_22
						if (mapToAddXdata.Lookup(strType, strValue))
						{
							CStringArray strArrayXdata ;
							CBlkUtility::DivideString(strValue, _T(";"), strArrayXdata) ;
							for (int iXdataIndex=0; iXdataIndex<strArrayXdata.GetCount(); iXdataIndex++)
							{
								CString strAppXdata = strArrayXdata.GetAt(iXdataIndex) ;
								strAppXdata.Trim() ;
								CString strApp, strXdata ;
								int iMarkLoc = strAppXdata.Find(_T(",")) ;
								strApp = strAppXdata.Left(iMarkLoc) ;
								strXdata = strAppXdata.Mid(iMarkLoc+1, strAppXdata.GetLength()-iMarkLoc) ;

								mysys.SetXdata(pEnt, strApp, strXdata) ;
							}
						}
					}

				}
				//end 增加xdata 不仅文本，块的xdata也可以增加

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
	if (Acad::eOk != extDatabase.wblock(pTempDb, list, pt_3d))
	{
		acutPrintf(_T("\nwblock failed!"));
		return FALSE ;
	}		
	if (Acad::eOk != pDb->insert(xform, pTempDb))
	{
		acutPrintf(_T("insert failed!"));
		return FALSE ;
	}	

	delete pTempDb;

	return bRet ;
}

// 填充输入表
int CZnDmh::FillImport(const CString & strXlsFile)
{
	return 0;
}

// 绘制综合柜架柜图
void CZnDmh::DrawZZH(const CString &strSavePath)
{
	CStringArray strArrayGuiDaoR, strArrayGuiDaoF, strArrayGuiDao, strArrayJinLuName ; 
	AcDbDatabase *pDb = new AcDbDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;
	CString strSaveFile ;
	strSaveFile.Format(_T("%s\\综合柜布置图.dwg"), strSavePath) ;
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	double dMainTbWidth = 188.000 ; //综合柜主框宽度
	double dMainTbRowHeight = 20.000 ; //每层高度
	double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 38.000 ; //左侧序号、组合类型等列所占宽度（从框架左边框到组合单元的距离）
	double dTFGZCellWidth = 25.000 ; //TFGZ、MGL-R组合单元格宽度
	double dMGLFCellWidth = 40.000 ; //MGL-F组合单元格宽度
	double dSRTZCellWidth = 12.500 ; //SRTZ组合单元格宽度
	double dNFLCellWidth = 8.000 ; //防雷单元格宽度
	double dNFLCellHeight = 5.000 ; //防雷单元格高度
	double dBetweenMainTb = 5.000 ; //ZZH1和ZZH2之间的距离
	double dFrameBaseX = -30.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -40.000 ; //左Frame的插入基点y

	try
	{
		//将数据读入array
		CADORecordset cSet ;
		CString strSql ;
		//填充 strArrayJinLuName
		strSql.Format(_T("select distinct dmh_import_table,dmh_import_cellcol,dmh_import_jinluname from dmh_import order by dmh_import_table asc,dmh_import_cellcol asc")) ;
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
			while(!cSet.IsEOF())
			{
				CString strJinLuName ;
				cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
				strJinLuName.Trim() ;
				if (!strJinLuName.IsEmpty())
				{
					strArrayJinLuName.Add(strJinLuName) ;
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}

		//填充 strArrayGuiDaoR, strArrayGuiDaoF, strArrayGuiDao
		strSql.Format(_T("select * from dmh_import order by id asc")) ;
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

			while(!cSet.IsEOF())
			{
				CString strGuiDaoTmp ;
				cSet.GetFieldValue(_T("dmh_import_guidao"), strGuiDaoTmp) ;
				if (strGuiDaoTmp.Right(1)==_T("+"))
				{
					strArrayGuiDaoR.Add(strGuiDaoTmp) ;
				}
				else
				{
					strArrayGuiDaoF.Add(strGuiDaoTmp) ;
				}
				//for 四线制
				strArrayGuiDao.Add(strGuiDaoTmp) ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}


		int iGuiDaoRCount = strArrayGuiDaoR.GetCount() ;
		int iGuiDaoFCount = strArrayGuiDaoF.GetCount() ;
		//for 四线制
		int iGuiDaoCount = iGuiDaoRCount+iGuiDaoFCount ;

		//填充架柜排列表
		//int iJgplRowsPerTb = 15 ; //每个表的行
		int iJgplColsPerTFGZ = 6 ;//TFGZ组合有几个单元
		int iJgplColsPerR = 5 ;//每行写受电端轨道的单元格有几列
		int iJgplColsPerF = 3 ;//每行写送电端轨道的单元格有几列
		//int iJgplStartRow = 7 ; //读写起始行
		//TCHAR chJgplStartCol = _T('A') ; //读写起始列
		//填充进路name等信息
		int iJinLuNameCount = strArrayJinLuName.GetCount() ;
		//需要多少行ZH来填充进路name和接收、发送轨道
		int iTFGZNeedZHRow = iJinLuNameCount/iJgplColsPerTFGZ+(iJinLuNameCount%iJgplColsPerTFGZ==0?0:1) ; //TFGZ占用行数
		int iMGLRNeedZHRow = iGuiDaoRCount/iJgplColsPerR+(iGuiDaoRCount%iJgplColsPerR==0?0:1) ; //MGL-R占用行数
		int iMGLFNeedZHRow = iGuiDaoFCount/iJgplColsPerF+(iGuiDaoFCount%iJgplColsPerF==0?0:1) ; //MGL-F占用行数	
		int iJgplTotalNeedZHRow = iTFGZNeedZHRow + iMGLRNeedZHRow + iMGLFNeedZHRow ; //总占用ZH柜行数		
		int iZHCount = iJgplTotalNeedZHRow/9+(iJgplTotalNeedZHRow%9==0?0:1) ;//需要几个ZH柜
		if (m_bIsFLInZH==FALSE) //防雷不在综合柜（在防雷分线柜）
		{
			iZHCount = iJgplTotalNeedZHRow/10+(iJgplTotalNeedZHRow%10==0?0:1) ;//需要几个ZH柜
		}

		//for 四线制
		int iGuiDaoNumPerSRTZRow = 24 ;//4*(3*2)
		int iGuiDaoNumPerMGFLRow = 36 ;//4*(3*2)
		int iSRTZNeedZHRow = iGuiDaoCount/iGuiDaoNumPerSRTZRow + (iGuiDaoCount%iGuiDaoNumPerSRTZRow==0?0:1) ; //SRTZ占用行数
		int iMGFLNeedZHRow = iGuiDaoCount/iGuiDaoNumPerMGFLRow + (iGuiDaoCount%iGuiDaoNumPerMGFLRow==0?0:1) ; //MGFL-T占用行数
		if (m_bIsFLInZH==FALSE) //防雷不在综合柜（在防雷分线柜）
		{
			iMGFLNeedZHRow = 0 ;
		}
		if (m_iTwoOrFourLine==4)
		{
			iJgplTotalNeedZHRow = iMGFLNeedZHRow+iTFGZNeedZHRow + iSRTZNeedZHRow ; //总占用ZH柜行数
			iZHCount = iJgplTotalNeedZHRow/10+(iJgplTotalNeedZHRow%10==0?0:1) ;//需要几个ZH柜
		}

		AcDbBlockReference *pRef = NULL ;
		CString strBlockName = _T("") ;
		CString strBlockName_Cell = _T("dmh_box_zzh_cell") ;
		AcGePoint3d pt_3d_insert(0,0,0) ;
		CStringArray strArrayVal ;

		for (int iZHIndex=0; iZHIndex<iZHCount; iZHIndex++)
		{
			//插入dmh_box_zzh_main（组合柜、综合柜共用一个主框架）
			strBlockName = _T("dmh_box_zzh_main") ;
			pt_3d_insert.x = iZHIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = 0 ;
			strArrayVal.RemoveAll() ;
			CString strZZHIndex ;
			strZZHIndex.Format(_T("%s%d"),m_strPreZZH, iZHIndex+1) ;
			strArrayVal.Add(strZZHIndex) ;
			strArrayVal.Add(_T("站内综合柜")) ;			
			if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			
			//在第一个综合架下面插入注释:"+"号表示受电端
			if (iZHIndex==0)
			{
				strBlockName = _T("dmh_circuit_note_0002_0") ;
				pt_3d_insert.y -= 9  ;		
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}
			}
		}

		int iFlInputIndex = 0 ; //在当前柜子防雷填充的区段的序号，换柜子就置0
		int iPreZH = 0 ; //记录前次ZH柜的序号，和当前序号对比，若变化了则iFlInputIndex置0
		
        //二线制时绘制MGL-R、MGL-F
		if (m_iTwoOrFourLine==2)
		{
			if (m_bIsFLInZH==TRUE)
			{
				for (int iZHIndex=0; iZHIndex<iZHCount; iZHIndex++)
				{				
					//在ZZHn-10插入MGFL-T
					strBlockName = _T("dmh_box_zzh_mgfl-t") ;
					pt_3d_insert.x = iZHIndex*(dMainTbWidth+dBetweenMainTb) ;
					pt_3d_insert.y = dMainTbRowHeight*9 ;
					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}

			int iDivider = m_bIsFLInZH?9:10 ; //

			//插入TFGZ块
			strBlockName = _T("dmh_box_zzh_tfgz") ;	
			for (int iTFGZIndex=0; iTFGZIndex<iTFGZNeedZHRow; iTFGZIndex++)
			{
				pt_3d_insert.x = iTFGZIndex/iDivider*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (iDivider-iTFGZIndex%iDivider-1)*dMainTbRowHeight ;					
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}
			}
			//填充进路类型名称(XJM、SIFM......）
			for (int iJinLuNameIndex=0; iJinLuNameIndex<iJinLuNameCount; iJinLuNameIndex++)
			{
				pt_3d_insert.x = (iJinLuNameIndex/iJgplColsPerTFGZ/iDivider)*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+(iJinLuNameIndex%iJgplColsPerTFGZ)*dTFGZCellWidth+dTFGZCellWidth/2 ;
				pt_3d_insert.y = (iDivider-(iJinLuNameIndex/iJgplColsPerTFGZ%iDivider)-1)*dMainTbRowHeight+dMainTbRowHeight/2 ;
				strArrayVal.RemoveAll() ;
				strArrayVal.Add(strArrayJinLuName.GetAt(iJinLuNameIndex)) ;
				if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
				{
					pRef->close() ;
					pRef = NULL ;
				}
			}
			//插入MGL-R块
			strBlockName = _T("dmh_box_zzh_mgl-r") ;
			for (int iMGLRIndex=0; iMGLRIndex<iMGLRNeedZHRow; iMGLRIndex++)
			{
				pt_3d_insert.x = (iMGLRIndex+iTFGZNeedZHRow)/iDivider*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (iDivider-(iMGLRIndex+iTFGZNeedZHRow)%iDivider-1)*dMainTbRowHeight ;					
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}
			}
			//插入MGL-F块
			strBlockName = _T("dmh_box_zzh_mgl-f") ;
			for (int iMGLFIndex=0; iMGLFIndex<iMGLFNeedZHRow; iMGLFIndex++)
			{
				pt_3d_insert.x = (iMGLFIndex+iTFGZNeedZHRow+iMGLRNeedZHRow)/iDivider*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (iDivider-(iMGLFIndex+iTFGZNeedZHRow+iMGLRNeedZHRow)%iDivider-1)*dMainTbRowHeight ;					
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}
			}

			//填充受电端区段
			CString strGuiDao = _T("") ;
			for (int iMGLRIndex=0; iMGLRIndex<iMGLRNeedZHRow; iMGLRIndex++)
			{
				int iCurZH = (iTFGZNeedZHRow+iMGLRIndex)/iDivider ; //当前的ZH柜序号
				if (iPreZH!=iCurZH)
				{
					iFlInputIndex = 0 ;
				}
				//受电端轨道....
				int iMaxCol = ((iMGLRIndex==iMGLRNeedZHRow-1)?(iGuiDaoRCount-iMGLRIndex*iJgplColsPerR):iJgplColsPerR) ; //当前行的占用单元格数
				for (int iGuiDaoRColIndex=0; iGuiDaoRColIndex<iMaxCol; iGuiDaoRColIndex++)
				{
					strGuiDao = strArrayGuiDaoR.GetAt(iMGLRIndex*iJgplColsPerR+iGuiDaoRColIndex) ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strGuiDao) ;
					pt_3d_insert.x = iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+iGuiDaoRColIndex*dTFGZCellWidth+dTFGZCellWidth/2 ;
					pt_3d_insert.y = (iDivider-(iMGLRIndex+iTFGZNeedZHRow)%iDivider-1)*dMainTbRowHeight+dMainTbRowHeight/2 ;	
					if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
					
					if (m_bIsFLInZH==TRUE)
					{
						//填充防雷（ZHn-10）
						pt_3d_insert.x = iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+(iFlInputIndex/2+0.5)*dNFLCellWidth ;
						pt_3d_insert.y = dMainTbRowHeight*iDivider + (iFlInputIndex%2==0?dNFLCellHeight*5/2:dNFLCellHeight/2) ;
						if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
						{
							mysys.SetAttHeight(pRef, 2, 0.5, _T("CELL_CONTENT")) ;
							pRef->close() ;
							pRef = NULL ;
						}
					}

					iFlInputIndex++ ;
				}

				iPreZH = iCurZH ;
			}		

			//填充送电端区段
			for (int iMGLFIndex=0; iMGLFIndex<iMGLFNeedZHRow; iMGLFIndex++)
			{
				int iCurZH = (iTFGZNeedZHRow+iMGLRNeedZHRow+iMGLFIndex)/iDivider ; //当前的ZH柜序号
				if (iPreZH!=iCurZH)
				{
					iFlInputIndex = 0 ;
				}
				//受电端轨道....
				int iMaxCol = ((iMGLFIndex==iMGLFNeedZHRow-1)?(iGuiDaoFCount-iMGLFIndex*iJgplColsPerF):iJgplColsPerF) ;//当前行的占用单元格数
				for (int iGuiDaoFColIndex=0; iGuiDaoFColIndex<iMaxCol; iGuiDaoFColIndex++)
				{
					strGuiDao = strArrayGuiDaoF.GetAt(iMGLFIndex*iJgplColsPerF+iGuiDaoFColIndex) ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strGuiDao) ;
					pt_3d_insert.x = iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+iGuiDaoFColIndex*dMGLFCellWidth+dMGLFCellWidth/2 ;
					pt_3d_insert.y = (iDivider-(iMGLFIndex+iTFGZNeedZHRow+iMGLRNeedZHRow)%iDivider-1)*dMainTbRowHeight+dMainTbRowHeight/2 ;	
					if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
					
					if (m_bIsFLInZH)
					{
						//填充防雷（ZHn-10）
						pt_3d_insert.x = iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+(iFlInputIndex/2+0.5)*dNFLCellWidth ;
						pt_3d_insert.y = dMainTbRowHeight*9 + (iFlInputIndex%2==0?dNFLCellHeight*5/2:dNFLCellHeight/2) ;
						if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
						{
							mysys.SetAttHeight(pRef, 2, 0.5, _T("CELL_CONTENT")) ;
							pRef->close() ;
							pRef = NULL ;
						}
					}

					iFlInputIndex++ ;
				}
				iPreZH = iCurZH ;
			}
		}
		else if (m_iTwoOrFourLine==4) //四线制
		{
			if (m_bIsFLInZH)
			{
				//插入MGFL块
				strBlockName = _T("dmh_box_zzh_mgfl-t") ;
				for (int iMGFLIndex=0; iMGFLIndex<iMGFLNeedZHRow; iMGFLIndex++)
				{
					pt_3d_insert.x = iMGFLIndex/10*(dMainTbWidth+dBetweenMainTb) ;
					pt_3d_insert.y = (10-iMGFLIndex%10-1)*dMainTbRowHeight ;					
					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					//填充防雷
					int iCurZH = iMGFLNeedZHRow/10 ; //当前的ZH柜序号

					//轨道....
					int iMaxINDEX = ((iMGFLIndex==iMGFLNeedZHRow-1)?(iGuiDaoCount-iMGFLIndex*iGuiDaoNumPerMGFLRow):iGuiDaoNumPerMGFLRow) ; //当前行的占用单元格数
					for (int iGuiDaoIndex=0; iGuiDaoIndex<iMaxINDEX; iGuiDaoIndex++)
					{
						CString strGuiDao ;
						strGuiDao = strArrayGuiDao.GetAt(iMGFLIndex*iGuiDaoNumPerMGFLRow+iGuiDaoIndex) ;
						strArrayVal.RemoveAll() ;
						strArrayVal.Add(strGuiDao) ;

						pt_3d_insert.x = iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+(iGuiDaoIndex/2+0.5)*dNFLCellWidth ;
						pt_3d_insert.y = (10-iMGFLIndex%10-1)*dMainTbRowHeight + (iGuiDaoIndex%2==0?dNFLCellHeight*5/2:dNFLCellHeight/2) ;

						if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
						{
							mysys.SetAttHeight(pRef, 2, 0.5, _T("CELL_CONTENT")) ;
							pRef->close() ;
							pRef = NULL ;
						}
					}
				}
			}

			//插入SRTZ块
			strBlockName = _T("dmh_box_zzh_srtz") ;
			for (int iSRTZIndex=0; iSRTZIndex<iSRTZNeedZHRow; iSRTZIndex++)
			{
				pt_3d_insert.x = (iSRTZIndex+iMGFLNeedZHRow)/10*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (10-(iSRTZIndex+iMGFLNeedZHRow)%10-1)*dMainTbRowHeight ;					
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				int iCurZH = (iMGFLNeedZHRow+iSRTZIndex)/10 ; //当前的ZH柜序号

				//轨道....
				int iMaxRINDEX = ((iSRTZIndex==iSRTZNeedZHRow-1)?(iGuiDaoCount-iSRTZIndex*iGuiDaoNumPerSRTZRow):iGuiDaoNumPerSRTZRow) ; //当前行的占用单元格数
				for (int iGuiDaoIndex=0; iGuiDaoIndex<iMaxRINDEX; iGuiDaoIndex++)
				{					
					CString strGuiDao ;
					strGuiDao = strArrayGuiDao.GetAt(iSRTZIndex*iGuiDaoNumPerSRTZRow+iGuiDaoIndex) ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strGuiDao) ;
					pt_3d_insert.x = iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+(iGuiDaoIndex/2)*dSRTZCellWidth+dSRTZCellWidth/2 ;
					pt_3d_insert.y = (10-(iSRTZIndex+iMGFLNeedZHRow)%10-1)*dMainTbRowHeight+(1.5+(iGuiDaoIndex+1)%2)*dRowTopBottomHeight ;	
					if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
					{
						mysys.SetAttHeight(pRef, 2.2, 0.65, _T("CELL_CONTENT")) ;

						pRef->close() ;
						pRef = NULL ;
					}
				}
			}

			//插入TFGZ块
			strBlockName = _T("dmh_box_zzh_tfgz") ;	
			for (int iTFGZIndex=0; iTFGZIndex<iTFGZNeedZHRow; iTFGZIndex++)
			{
				pt_3d_insert.x = (iTFGZIndex+iMGFLNeedZHRow+iSRTZNeedZHRow)/10*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (10-(iTFGZIndex+iMGFLNeedZHRow+iSRTZNeedZHRow)%10-1)*dMainTbRowHeight ;					
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				int iCurZH = (iMGFLNeedZHRow+iSRTZNeedZHRow+iTFGZIndex)/10 ; //当前的ZH柜序号

				//轨道....
				int iMaxJinLuIndex = ((iTFGZIndex==iTFGZNeedZHRow-1)?(iJinLuNameCount-iTFGZIndex*iJgplColsPerTFGZ):iJgplColsPerTFGZ) ; //当前行的占用单元格数


				//填充进路类型名称(XJM、SIFM......）
				for (int iJinLuNameIndex=0; iJinLuNameIndex<iMaxJinLuIndex; iJinLuNameIndex++)
				{
					pt_3d_insert.x =  iCurZH*(dMainTbWidth+dBetweenMainTb)+dLeftWidth+iJinLuNameIndex*dTFGZCellWidth+dTFGZCellWidth/2 ;
					pt_3d_insert.y = (10-(iTFGZIndex+iMGFLNeedZHRow+iSRTZNeedZHRow)%10-1)*dMainTbRowHeight+dMainTbRowHeight/2 ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strArrayJinLuName.GetAt(iJinLuNameIndex+iTFGZIndex*iJgplColsPerTFGZ)) ;
					if (mysys.InsertBlock(pRef, strBlockName_Cell, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
		}	

		//插入Frame
		strBlockName = _T("frame_left") ;
		pt_3d_insert.x = dFrameBaseX ;
		pt_3d_insert.y = dFrameBaseY ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		//内边框
		AcGePoint3dArray ptArrayFrame ;
		ptArrayFrame.append(AcGePoint3d(-5, dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZHCount, dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZHCount, dFrameBaseY+297-5, 0)) ;
		ptArrayFrame.append(AcGePoint3d(-5, dFrameBaseY+297-5, 0)) ;
		mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
		//外边框
		ptArrayFrame.setAt(0,AcGePoint3d(dFrameBaseX, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(1,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZHCount+5, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(2,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZHCount+5, dFrameBaseY+297, 0)) ;
		ptArrayFrame.setAt(3,AcGePoint3d(dFrameBaseX, dFrameBaseY+297, 0)) ;
		mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	
		//签字栏
		strBlockName = _T("SIGN_3") ;
		pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZHCount ;
		pt_3d_insert.y = dFrameBaseY+5 ;
		strArrayVal.RemoveAll() ;
		strArrayVal.Add(m_strStation) ;
		strArrayVal.Add(m_strDwgSn) ;		
		strArrayVal.Add(CBlkUtility::GetDateStr()) ;
		strArrayVal.Add(_T("站内电码化综合柜布置图")) ;
		strArrayVal.Add(m_strProjectName) ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		mysys.ModifyFontStyle(_T("standard"), _T("hztxt.shx")) ;
		pDb->saveAs(strSaveFile) ;
		delete pDb ;
		pDb = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in DrawZZH!"));			
	}
}

// 绘制组合柜
void CZnDmh::DrawZ(const CString &strSavePath)
{
	AcDbDatabase *pDb = new AcDbDatabase() ;
	g_cSystem.m_pDb = pDb ;
	CString strSaveFile ;
	strSaveFile.Format(_T("%s\\组合柜布置图.dwg"), strSavePath) ;
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	double dMainTbWidth = 188.000 ; //组合柜主框宽度
	double dMainTbRowHeight = 20.000 ; //每层高度
	double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 38.000 ; //左侧序号、组合类型等列所占宽度（从框架左边框到组合单元的距离）
	double dCellWidth = 15.000 ; //组合单元格宽度
	double dCellWidthHLP = 9.375 ;//150/16,若单元格变化此值随着变
	double dBetweenMainTb = 5.000 ; //Z1和Z2之间的距离
	double dFrameBaseX = -30.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -40.000 ; //左Frame的插入基点y

	try
	{
		//将数据读入array
		CADORecordset cSet ;
		CString strSql ;

		int iZIndex = 0 ; //组合架序号
		int iZLocRow = 10 ; //组合架内层号，10~1
		//int iZCellIndex = 1 ; //每个组合内继电器位置序号，从1~10

		AcDbBlockReference *pRef = NULL ;
		CString strBlockName = _T("") ;
		AcGePoint3d pt_3d_insert(0,0,0) ;
		CStringArray strArrayVal ;

		//先插入一个组合架
		strBlockName = _T("dmh_box_zzh_main") ;
		pt_3d_insert.x = pt_3d_insert.y = 0 ;		
		strArrayVal.RemoveAll() ;
		CString strZIndex ;
		strZIndex.Format(_T("%s%d"),m_strPreZ, iZIndex+1) ;
		strArrayVal.Add(strZIndex) ;
		strArrayVal.Add(_T("站内组合柜")) ;			
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		//在第一个组合架下面插入注释:"+"号表示受电端
		strBlockName = _T("dmh_circuit_note_0002_0") ;
		pt_3d_insert.y -= 9  ;		
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		//begin 读取第一个进路表(第一个表是进路，第二个是股道输入）
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_type, dmh_import_xhj, dmh_import_cellcol from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)			
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}

				CStringArray strArrayGuiDao ; //记录本进路的轨道区段
				CString strHasFinished = _T("") ; //记录已经处理过的进路的列，在反发（2CD/2CD1）中，小于5个轨道的拼接，所以可能提前处理，后面就不用再处理了
				//CString strPreLineName = _T("") ; //记录前次的线名，每条线在第一次遇到正接时记录股道信息
				int iGuDaoFenGeNum = 0 ; //股道有几个分割，>1表示有分割
				CString strGuDao = _T("") ; //取得股道名称（不带分割号）

				while(!cSet.IsEOF())
				{
					strArrayGuiDao.RemoveAll() ;


					CString strLineName, strJinLuType, strStartEndXhj, strCellCol ;
					cSet.GetFieldValue(_T("dmh_import_line"), strLineName) ; //下行线1
					cSet.GetFieldValue(_T("dmh_import_type"), strJinLuType) ;//正接、反发、正发、反接
					cSet.GetFieldValue(_T("dmh_import_xhj"), strStartEndXhj) ; //X->XI
					cSet.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ; // B、C、D......

					//处理过的进路不再处理
					CString strToFind ;
					strToFind.Format(_T("%s,"), strCellCol) ;
					if (strHasFinished.Find(strToFind)!=-1)
					{
						cSet.MoveNext() ;
						continue;
					}

					strHasFinished.AppendFormat(_T("%s,"), strCellCol) ;

					//按各进路类型依次选择、排列组合
					int iRsCount = 0 ;
					if (strJinLuType==_T("正接")) //每遇到正接，初始数据
					{
						iGuDaoFenGeNum = 0 ;
						strGuDao = _T("") ;
					}
					//begin 取得每条进路的轨道区段
					CADORecordset cSetOneJinLu ;
					strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLineName, strJinLuType) ;
					if (cSetOneJinLu.IsOpen())
					{
						cSetOneJinLu.Close() ;
					}
					if (cSetOneJinLu.Open(g_PtrCon, strSql))
					{
						iRsCount = cSetOneJinLu.GetRecordCount() ;
						if (iRsCount>0)
						{
							cSetOneJinLu.MoveFirst() ;			
							while(!cSetOneJinLu.IsEOF())
							{
								CString strGuiDao ;
								int nCellRow = 0 ;
								cSetOneJinLu.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
								cSetOneJinLu.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
								if (nCellRow!=6)
								{
									strArrayGuiDao.Add(strGuiDao) ;
								}

								if(strJinLuType==_T("正接")) //每次只在正接时统计股道信息，以此为准
								{
									if (strGuiDao.Left(1) == _T("*"))
									{
										iGuDaoFenGeNum++ ;
										//取得股道名称
										if (strGuDao.IsEmpty())
										{
											int iFindLocG = strGuiDao.Find(_T("G")) ;
											if (iFindLocG!=-1)
											{
												strGuDao = strGuiDao.Mid(1, iFindLocG) ;
											}
										}
									}
								}
								cSetOneJinLu.MoveNext() ;
							}
						}
						cSetOneJinLu.Close() ;
					}
					//end 取得每条进路的轨道区段

					iRsCount = strArrayGuiDao.GetCount() ; //股道无分割时，股道不在GCJ1~GCJ10

					if (strJinLuType==_T("正接")|| strJinLuType==_T("正发"))
					{
						//begin 轨道数大于5，则用1CDA-1、1CDA-2
						if (iRsCount>5) 
						{
							//插入1CDA-1 or 1CDB
							CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
							strBlockName = iGuDaoFenGeNum>1?_T("dmh_box_z_1cdb"):_T("dmh_box_z_1cda_1") ;
							pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
							strArrayVal.RemoveAll() ;
							strArrayVal.Add(iGuDaoFenGeNum>1?_T("1CDB"):_T("1CDA-1")) ;
							strArrayVal.Add(strStartEndXhj) ;							
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}
							//插入轨道
							strBlockName = _T("dmh_box_z_cell") ;
							for (int iGuiDaoIndex=0; iGuiDaoIndex<5; iGuiDaoIndex++)
							{
								//GCJ位置
								pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iGuiDaoIndex+dCellWidth/2 ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
								strArrayVal.RemoveAll() ;						
								strArrayVal.Add(strArrayGuiDao.GetAt(iGuiDaoIndex)) ;							
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//GJF位置
								pt_3d_insert.x+=dCellWidth*5 ;
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
							}
							iZLocRow-- ;

							//插入1CDA_2
							CheckToAddZ(iZLocRow, iZIndex) ;						
							strBlockName = _T("dmh_box_z_1cda_2") ;
							pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
							strArrayVal.RemoveAll() ;
							strArrayVal.Add(iGuDaoFenGeNum>1?_T("1CDA"):_T("1CDA-2")) ;
							strArrayVal.Add(strStartEndXhj) ;
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}
							//插入轨道
							strBlockName = _T("dmh_box_z_cell") ;
							for (int iGuiDaoIndex=0; iGuiDaoIndex<iRsCount-5; iGuiDaoIndex++)
							{
								//GCJ位置
								pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iGuiDaoIndex+dCellWidth/2 ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
								strArrayVal.RemoveAll() ;						
								strArrayVal.Add(strArrayGuiDao.GetAt(iGuiDaoIndex+5)) ;							
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//GJF位置
								pt_3d_insert.x+=dCellWidth*5 ;
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
							}
							iZLocRow-- ;
						}//end 轨道数大于5，则用1CDA-1、1CDA-2
						else //begin 轨道数小于5，则用1CDA(无分割) 1CDB(有分割)
						{
							CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
							//插入1CDA
							strBlockName = iGuDaoFenGeNum>1?_T("dmh_box_z_1cdb"):_T("dmh_box_z_1cda_1") ;
							pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
							strArrayVal.RemoveAll() ;
							strArrayVal.Add(iGuDaoFenGeNum>1?_T("1CDB"):_T("1CDA")) ;
							strArrayVal.Add(strStartEndXhj) ;							
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}
							//插入轨道
							strBlockName = _T("dmh_box_z_cell") ;
							for (int iGuiDaoIndex=0; iGuiDaoIndex<iRsCount; iGuiDaoIndex++)
							{
								//GCJ位置
								pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iGuiDaoIndex+dCellWidth/2 ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
								strArrayVal.RemoveAll() ;						
								strArrayVal.Add(strArrayGuiDao.GetAt(iGuiDaoIndex)) ;							
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//GJF位置
								pt_3d_insert.x+=dCellWidth*5 ;
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
							}
							iZLocRow-- ;
						}
						//end 轨道数小于5，则用1CDA

						if (strJinLuType==_T("正接")) //正发没有ZJC
						{
							//插入ZJC
							CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
							strBlockName = _T("dmh_box_z_zjc") ;
							pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
							//有无反发、正发、反接
							BOOL bHasFanFa = FALSE ;
							BOOL bHasZhengFa = FALSE ;
							BOOL bHasFanJie = FALSE ;
							this->HasJinLuType(strLineName, bHasFanFa, bHasZhengFa, bHasFanJie) ;					
							strArrayVal.RemoveAll() ;						
							strArrayVal.Add(strStartEndXhj) ;
							if (bHasFanFa)//有无反发，有则4、7位插继电器，否则不插
							{
								strArrayVal.Add(_T("JMJF")) ;
								strArrayVal.Add(_T("FMJ")) ;
							}
							else
							{
								strArrayVal.Add(_T("")) ;
								strArrayVal.Add(_T("")) ;
							}
							if (iGuDaoFenGeNum>1)//股道带分割否，不带则8、9位放置GJF1、GCJ1，带分割则第8位放ZGJ,第9位不插
							{
								strArrayVal.Add(_T("ZGJ")) ;
								strArrayVal.Add(_T("")) ;
							}
							else
							{
								strArrayVal.Add(_T("GJF1")) ;
								strArrayVal.Add(_T("GCJ1")) ;
							}
							if (bHasZhengFa&&!bHasFanJie)//有正发，无反接，则使用第10位FMJ,否则不插
							{
								strArrayVal.Add(_T("FMJ")) ;
							}
							else
							{
								strArrayVal.Add(_T("")) ;
							}
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}
							//插入cell内容
							strBlockName = _T("dmh_box_z_cell") ;
							CString strStartXhj = _T(""), strEndXhj = _T(""), strEndXhj_F = _T("") ;

							int iFindLoc = strStartEndXhj.Find(_T("→")) ;
							int iLength = strStartEndXhj.GetLength() ;
							if (iFindLoc!=-1)
							{
								strStartXhj = strStartEndXhj.Left(iFindLoc) ;
								strEndXhj = strStartEndXhj.Mid(iFindLoc+2) ;
								strEndXhj_F.Format(_T("%s%s"), strEndXhj.Left(1)==_T("S")?_T("X"):_T("S"), strEndXhj.Mid(1)) ;
							}
							else
							{
								AfxMessageBox(_T("起止信号机格式没按类似X→XI！")) ;
							}
							for (int iZJCIndex=0; iZJCIndex<10; iZJCIndex++)
							{
								pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iZJCIndex+dCellWidth/2 ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;

								strArrayVal.RemoveAll() ;

								if (iZJCIndex<3) //X
								{
									strArrayVal.Add(strStartXhj) ;
								}
								else if (iZJCIndex==4||iZJCIndex==5) //XI
								{
									strArrayVal.Add(strEndXhj) ;
								}
								else if (iZJCIndex==3||iZJCIndex==6) //X SI
								{
									if (bHasFanFa)//有无反发，有则4、7位插继电器，否则不插
									{
										strArrayVal.Add(iZJCIndex==3?strStartXhj:strEndXhj_F) ;
									}
									else
									{
										strArrayVal.Add(_T("")) ;
									}
								}
								else if (iZJCIndex==7||iZJCIndex==8) //IG
								{
									if (iGuDaoFenGeNum>1)//股道带分割否，不带则8、9位放置GJF1、GCJ1，带分割则第8位放ZGJ,第9位不插
									{
										strArrayVal.Add(iZJCIndex==7?strGuDao:_T("")) ;
									}
									else
									{
										strArrayVal.Add(strGuDao) ;
									}
								}
								else if (iZJCIndex==9) //第10个继电器
								{
									if (bHasZhengFa&&!bHasFanJie)//有正发，无反接，则使用第10位FMJ,否则不插
									{
										strArrayVal.Add(strEndXhj) ;
									}
									else
									{
										strArrayVal.Add(_T("")) ;
									}
								}
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}						
							}
							iZLocRow-- ;
						}
					}
					else if(strJinLuType==_T("反发")||strJinLuType==_T("反接"))
					{
						CStringArray strArrayGuiDao2 ; //记录第二个轨道数小于5的进路的轨道
						CString strGuDao2 = _T("") ; //记录第二个轨道数小于5的进路的股道（反接）
						CString strStartEndXhj2 = _T("") ; //记录第二个轨道数小于5的进路的起止信号机
						CString strLineName2  = _T(""), strJinLuType2 = _T("") ; //找到的第二个小于5个区段的进路的线名和进路类型
						BOOL bFindFanFaFanJie = FALSE ; //当第一个进路轨道数小于5时，第二个小于5的反发或反接进路是否能找到

						if (iRsCount>5) //独占一个2CD组合
						{
							//插入2CD
							CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
							strBlockName = iGuDaoFenGeNum>1?_T("dmh_box_z_2cd_1"):_T("dmh_box_z_2cd1_1") ; //带分割的调用2CD,不带分割的调用2CD1
							pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
							strArrayVal.RemoveAll() ;						
							strArrayVal.Add(strStartEndXhj) ;							
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}
							//插入轨道
							strBlockName = _T("dmh_box_z_cell") ;
							for (int iGuiDaoIndex=0; iGuiDaoIndex<iRsCount; iGuiDaoIndex++)
							{							
								pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iGuiDaoIndex+dCellWidth/2 ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
								strArrayVal.RemoveAll() ;						
								strArrayVal.Add(strArrayGuiDao.GetAt(iGuiDaoIndex)) ;							
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
							}
							iZLocRow-- ;
						}
						else if (iRsCount>0) //查询后续的有无小于等于5个轨道的反发或者反接进路，共用一个2CD组合
						{
							//begin 查询后续小于5个轨道的反发或者反接
							CString strHasFinishedTmp = strHasFinished ;
							strHasFinishedTmp.Trim(_T(",")) ;
							strHasFinishedTmp.Replace(_T(","), _T("','")) ;
							strHasFinishedTmp.Insert(0, _T("'")) ;
							strHasFinishedTmp.Append(_T("'")) ;
							//增加判断：要查找的线路是否和当前线路同属分割or不分割,"and dmh_import_line not in (select dmh_import_line from dmh_import where dmh_import_type="正接" and dmh_import_cellrow=6)"
							strSql.Format(_T("select distinct dmh_import_line, dmh_import_type, dmh_import_xhj, dmh_import_cellcol from dmh_import where dmh_import_table=0 and dmh_import_type in('反发','反接') and dmh_import_cellcol>'%s' and dmh_import_cellcol not in(%s) and dmh_import_line %s in(select dmh_import_line from dmh_import where dmh_import_type='正接' and dmh_import_cellrow=6) order by dmh_import_cellcol asc"), strCellCol, strHasFinishedTmp, iGuDaoFenGeNum>1?_T("not"):_T("")) ;

							if (cSetOneJinLu.IsOpen())
							{
								cSetOneJinLu.Close() ;
							}
							if (cSetOneJinLu.Open(g_PtrCon, strSql))
							{
								if (cSetOneJinLu.GetRecordCount()>0)
								{
									cSetOneJinLu.MoveFirst() ;

									while (!bFindFanFaFanJie&&!cSetOneJinLu.IsEOF())
									{
										CString strCellColTmp ;
										cSetOneJinLu.GetFieldValue(_T("dmh_import_line"), strLineName2) ;
										cSetOneJinLu.GetFieldValue(_T("dmh_import_type"), strJinLuType2) ;
										cSetOneJinLu.GetFieldValue(_T("dmh_import_xhj"), strStartEndXhj2) ;
										cSetOneJinLu.GetFieldValue(_T("dmh_import_cellcol"), strCellColTmp) ;
										strSql.Format(_T("select * from dmh_import where dmh_import_table=0 and dmh_import_cellrow>6 and dmh_import_cellrow<17 and dmh_import_cellcol='%s' order by dmh_import_cellrow"), strCellColTmp) ;
										CADORecordset cSetFindJinLu ;
										if (cSetFindJinLu.IsOpen())
										{
											cSetFindJinLu.Close() ;
										}
										if (cSetFindJinLu.Open(g_PtrCon, strSql))
										{
											int iRsCount2 = cSetFindJinLu.GetRecordCount() ;
											//找到少于5个轨道的进路
											if (iRsCount2>0&&iRsCount2<=5)
											{
												if (iGuDaoFenGeNum==1&&strJinLuType2==_T("反接")) //股道无分割时，轨道名在第6行
												{												
													CStringArray strArrayGuiDaoTmp ;
													strSql.Format(_T("select * from dmh_import where dmh_import_table=0 and dmh_import_cellrow=6 and dmh_import_cellcol='%s' order by dmh_import_cellrow"), strCellColTmp) ;
													if(CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuiDaoTmp)==1)
													{
														CString strGuDaoTmp = strArrayGuiDaoTmp.GetAt(0);
														if(strGuDaoTmp.Left(1) == _T("*"))
														{													
															//取得股道名称			
															int iFindLocG = strGuDaoTmp.Find(_T("G")) ;
															if (iFindLocG!=-1)
															{
																strGuDao2 = strGuDaoTmp.Mid(1, iFindLocG) ;
															}													
														}
													}
												}

												cSetFindJinLu.MoveFirst() ;
												while(!cSetFindJinLu.IsEOF())
												{
													CString strGuiDaoTmp ;
													cSetFindJinLu.GetFieldValue(_T("dmh_import_guidao"), strGuiDaoTmp) ;
													strArrayGuiDao2.Add(strGuiDaoTmp) ;
													if (strJinLuType2==_T("反接")&&strGuDao2.IsEmpty())
													{											
														if(strGuiDaoTmp.Left(1) == _T("*"))
														{													
															//取得股道名称			
															int iFindLocG = strGuiDaoTmp.Find(_T("G")) ;
															if (iFindLocG!=-1)
															{
																strGuDao2 = strGuiDaoTmp.Mid(1, iFindLocG) ;
															}													
														}													
													}

													cSetFindJinLu.MoveNext() ;
												}

												bFindFanFaFanJie = TRUE ;
												strHasFinished.AppendFormat(_T("%s,"), strCellColTmp) ;
											}							

											cSetFindJinLu.Close() ;
										}

										cSetOneJinLu.MoveNext() ;
									}
								}							
								cSetOneJinLu.Close() ;
							}
							//end 查询后续小于5个轨道的反发或者反接

							//begin 有小于5个轨道的反接反发的进路
							if (bFindFanFaFanJie)
							{
								//插入2CD-1
								CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
								strBlockName = iGuDaoFenGeNum>1?_T("dmh_box_z_2cd_2"):_T("dmh_box_z_2cd1_2") ; //带分割的调用2CD,不带分割的调用2CD1 ;
								pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
								strArrayVal.RemoveAll() ;						
								strArrayVal.Add(strStartEndXhj) ;
								strArrayVal.Add(strStartEndXhj2) ;
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//插入轨道
								strBlockName = _T("dmh_box_z_cell") ;
								for (int iGuiDaoIndex=0; iGuiDaoIndex<iRsCount; iGuiDaoIndex++)
								{								
									pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iGuiDaoIndex+dCellWidth/2 ;
									pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
									strArrayVal.RemoveAll() ;						
									strArrayVal.Add(strArrayGuiDao.GetAt(iGuiDaoIndex)) ;							
									if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
									{
										pRef->close() ;
										pRef = NULL ;
									}
								}
								for (int iGuiDaoIndex=0; iGuiDaoIndex<strArrayGuiDao2.GetCount(); iGuiDaoIndex++)
								{								
									pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*(iGuiDaoIndex+5)+dCellWidth/2 ;
									pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
									strArrayVal.RemoveAll() ;						
									strArrayVal.Add(strArrayGuiDao2.GetAt(iGuiDaoIndex)) ;							
									if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
									{
										pRef->close() ;
										pRef = NULL ;
									}
								}
								iZLocRow-- ;

							}//end 有小于5个轨道的反接反发的进路						
							else
							{
								//插入2CD
								CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
								strBlockName = iGuDaoFenGeNum>1?_T("dmh_box_z_2cd_1"):_T("dmh_box_z_2cd1_1") ; //带分割的调用2CD,不带分割的调用2CD1 ;
								pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
								strArrayVal.RemoveAll() ;						
								strArrayVal.Add(strStartEndXhj) ;							
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//插入轨道
								strBlockName = _T("dmh_box_z_cell") ;
								for (int iGuiDaoIndex=0; iGuiDaoIndex<iRsCount; iGuiDaoIndex++)
								{							
									pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iGuiDaoIndex+dCellWidth/2 ;
									pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
									strArrayVal.RemoveAll() ;						
									strArrayVal.Add(strArrayGuiDao.GetAt(iGuiDaoIndex)) ;							
									if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
									{
										pRef->close() ;
										pRef = NULL ;
									}
								}
								iZLocRow-- ;
							}
						}

						//begin jinlu1 or jinlu2 是反接 ，则还需要增加FJC
						CString strJinLuTypes[2] = {strJinLuType, bFindFanFaFanJie==TRUE? strJinLuType2:_T("")} ;
						for (int iJinLuIndex=0; iJinLuIndex<2; iJinLuIndex++)
						{
							if (strJinLuTypes[iJinLuIndex]==_T("反接")) 
							{
								CString strStartEndXhjTmp = (iJinLuIndex==0?strStartEndXhj:strStartEndXhj2) ;
								CString strGuDaoTmp = (iJinLuIndex==0?strGuDao:strGuDao2) ;

								//插入FJC
								CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
								strBlockName = _T("dmh_box_z_fjc") ;
								pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
								strArrayVal.RemoveAll() ;								
								strArrayVal.Add(strStartEndXhjTmp) ;
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//插入cell内容
								strBlockName = _T("dmh_box_z_cell") ;
								CString strStartXhjTmp = _T(""), strEndXhjTmp = _T(""), strEndXhjTmp_F = _T("") ;

								int iFindLoc = strStartEndXhjTmp.Find(_T("→")) ;
								int iLength = strStartEndXhjTmp.GetLength() ;
								if (iFindLoc!=-1)
								{
									strStartXhjTmp = strStartEndXhjTmp.Left(iFindLoc) ;
									strEndXhjTmp = strStartEndXhjTmp.Mid(iFindLoc+2) ;
									strEndXhjTmp_F.Format(_T("%s%s"), strEndXhjTmp.Left(1)==_T("S")?_T("X"):_T("S"), strEndXhjTmp.Mid(1)) ;
								}
								else
								{
									AfxMessageBox(_T("起止信号机格式没按类似X→XI！")) ;
								}
								for (int iFJCIndex=0; iFJCIndex<9; iFJCIndex++)
								{
									pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iFJCIndex+dCellWidth/2 ;
									pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;	

									strArrayVal.RemoveAll() ;

									if (iFJCIndex<4) //start信号机
									{
										strArrayVal.Add(strStartXhjTmp) ;
									}
									else if (iFJCIndex<6) //end信号机
									{
										strArrayVal.Add(strEndXhjTmp) ;
									}
									else if (iFJCIndex<8) //end_f
									{
										strArrayVal.Add(strEndXhjTmp_F) ;
									}
									else 
									{
										strArrayVal.Add(strGuDaoTmp) ;
									}

									if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
									{
										pRef->close() ;
										pRef = NULL ;
									}						
								}
								iZLocRow-- ;
							}
						}
						//end jinlu1 or jinlu2 是反接 ，则还需要增加FJC

					} //end 反发、反接

					cSet.MoveNext() ;
				}

				//begin ZBJH
				CStringArray strArrayXJZ, strArraySJZ ; 
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					CString strJinLuType ;				
					cSet.GetFieldValue(_T("dmh_import_type"), strJinLuType) ;//正接、反发、正发、反接			

					if (strJinLuType==_T("正接"))
					{
						CString strStartEndXhj ;					
						cSet.GetFieldValue(_T("dmh_import_xhj"), strStartEndXhj) ; //X->XI
						//取JZ
						CString strJZ ;
						int iLocFind = strStartEndXhj.Find(_T("→")) ;
						if (iLocFind!=-1)
						{
							strJZ = strStartEndXhj.Left(iLocFind) ;
						}
						strSql.Format(_T("select * from dmh_import where instr(dmh_import_line, '半自动接近') and dmh_import_xhj='%s' and dmh_import_table=1 "), strJZ) ;
						CStringArray strArrayRec ;
						if(CDBUtility::GetRecordValue(strSql, _T("id"), strArrayRec)==0)//非0说明以strJZ为进站信号机的该线为半自动接近
						{
							if (strJZ.Left(1)==_T("X"))
							{
								strArrayXJZ.Add(strJZ) ;
							}
							else if (strJZ.Left(1)==_T("S"))
							{
								strArraySJZ.Add(strJZ) ;
							}						
						}					
					}

					cSet.MoveNext() ;
				}
				int iXJZCount = strArrayXJZ.GetCount() ;
				int iSJZCount = strArraySJZ.GetCount() ;
				int iMinCount = min(iXJZCount, iSJZCount) ;
				int iMaxCount = max(iXJZCount, iSJZCount) ;
				int iNeedRow_ZBJH = iMaxCount ;
				for (int i=0; i<iNeedRow_ZBJH; i++)
				{
					//插入ZBJH
					CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
					strBlockName = _T("dmh_box_z_zbjh") ;
					pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
					pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;									
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
					{
						pRef->close() ;
						pRef = NULL ;
					}
					//插入cell
					strBlockName = _T("dmh_box_z_cell") ;
					for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
					{							
						pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
						pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
						strArrayVal.RemoveAll() ;
						CString strCell ;
						if (iCellIndex<5) //x
						{
							if (iXJZCount==iMinCount)
							{
								if (i<iXJZCount)
								{
									strCell = strArrayXJZ.GetAt(i) ;
								}
								else
								{
									strCell = _T("") ;
								}
							}
							else
							{
								strCell = strArrayXJZ.GetAt(i) ;
							}
						}
						else //s
						{
							if (iSJZCount==iMinCount)
							{
								if (i<iSJZCount)
								{
									strCell = strArraySJZ.GetAt(i) ;
								}
								else
								{
									strCell = _T("") ;
								}
							}
							else
							{
								strCell = strArraySJZ.GetAt(i) ;
							}
						}
						if (!strCell.IsEmpty())
						{
							strArrayVal.Add(strCell) ;							
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}
						}
					}
					iZLocRow-- ;

				}
				//end ZBJH

				cSet.Close() ;
			}
		}
		//end 读取第一个进路表(第一个表是进路，第二个是股道输入）

		//begin XHFS
		CStringArray strArrayJdqName, strArrayXhj ;
		strSql.Format(_T("select distinct dmh_import_line,dmh_import_lineindex, dmh_import_xhj from dmh_import where dmh_import_table=1 order by dmh_import_lineindex")) ;
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
				CString strLine, strXhj ;
				cSet.GetFieldValue(_T("dmh_import_line"), strLine) ;
				cSet.GetFieldValue(_T("dmh_import_xhj"), strXhj) ;
				CString strJdqName ;
				if (strLine.Find(_T("同一股道"))!=-1)
				{
					strJdqName = _T("LXJF") ;					
				}
				else if (strLine.Find(_T("半自动接近"))!=-1)
				{
					strJdqName = _T("YXJF") ;
				}
				else if (strLine.Find(_T("调车接近"))!=-1)
				{
					strJdqName = _T("DXJF") ;
				}
				strArrayJdqName.Add(strJdqName) ;
				strArrayXhj.Add(strXhj) ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		int iXhjCount_XHFS = strArrayXhj.GetCount() ;
		int iNeedRow_XHFS = iXhjCount_XHFS/10+(iXhjCount_XHFS%10==0?0:1) ;
		for (int i=0; i<iNeedRow_XHFS; i++)
		{
			//插入XHFS
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_box_z_xhfs") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;	
			strArrayVal.RemoveAll() ;
			for (int iJdqIndex=0; iJdqIndex<10; iJdqIndex++)
			{
				CString strVal = _T("XJF") ;
				if ((i*10+iJdqIndex)<iXhjCount_XHFS)
				{
					strVal = strArrayJdqName.GetAt(i*10+iJdqIndex) ;
				}
				strArrayVal.Add(strVal) ;
			}
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_box_z_cell") ;
			for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;
				if ((i*10+iCellIndex)<iXhjCount_XHFS)
				{
					strCell = strArrayXhj.GetAt(i*10+iCellIndex) ;
					strArrayVal.Add(strCell) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}				
			}
			iZLocRow-- ;
		}

		//end XHFS

		//begin GDFS
		CStringArray strArrayGuiDao ;
		strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_cellrow<16 order by dmh_import_cellrow")) ;
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

			while(!cSet.IsEOF())
			{
				CString strGuiDao ;
				cSet.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
				strGuiDao.TrimLeft(_T("*")) ;
				strGuiDao.TrimRight(_T("+")) ;
				if(CBlkUtility::FindStrInArray(strArrayGuiDao, strGuiDao)==-1)
				{
					strSql.Format(_T("select * from dmh_import where dmh_import_guidao in ('*%s','%s+', '*%s+') and dmh_import_table=0 order by dmh_import_cellrow"), strGuiDao, strGuiDao, strGuiDao) ;
					CStringArray strArrayRec ;
					if(CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayRec)==0)
					{
						strArrayGuiDao.Add(strGuiDao) ;
					}
				}
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		int iGuiDaoCount = strArrayGuiDao.GetCount() ;
		int iNeedRow_GDFS = iGuiDaoCount/8+(iGuiDaoCount%8==0?0:1) ;
		for (int i=0; i<iNeedRow_GDFS; i++)
		{
			//插入GDFS
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_box_z_gdfs") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_box_z_cell") ;
			for (int iCellIndex=0; iCellIndex<8; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;
				if ((i*8+iCellIndex)<iGuiDaoCount)
				{
					strCell = strArrayGuiDao.GetAt(i*8+iCellIndex) ;
					strArrayVal.Add(strCell) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
			iZLocRow-- ;
		}
		//end GDFS

		//begin FBJ
		CStringArray strArrayFBJCell ;
		strArrayFBJCell.Add(_T("+1")) ; //+1FBJ放置在最前面
		//正线进路表
		strSql.Format(_T("select distinct dmh_import_jfm, dmh_import_cellcol,dmh_import_jinluname from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				CString strHasFinished = _T("") ; //记录已经处理过的进路的列
				while (!cSet.IsEOF())
				{
					CString strCellCol ;
					cSet.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ;
					//处理过的进路不再处理
					CString strToFind ;
					strToFind.Format(_T("%s,"), strCellCol) ;
					if (strHasFinished.Find(strToFind)!=-1)
					{
						cSet.MoveNext() ;
						continue;
					}

					strHasFinished.AppendFormat(_T("%s,"), strCellCol) ;

					CString strJinLuName, strJFM ;
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					cSet.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;

					//begin 查询和当前进路共用发送器的进路
					BOOL bFoundShare = FALSE ;
					CString strJinLuName2, strJFM2, strCellCol2 ;
					if (strJFM.Right(1)==_T(")"))
					{
						int iFindMarkLoc = strJFM.ReverseFind(_T('(')) ; //例：JM(1)，左括号的位置
						if (iFindMarkLoc!=-1)
						{
							int iMarkLen = strJFM.GetLength()-iFindMarkLoc ; //例：JM(1)，最后（1）的长度
							CString strMark = strJFM.Mid(iFindMarkLoc) ; //例：JM(1)中（1）
							CADORecordset cSetFindShare ;
							strSql.Format(_T("select * from (select distinct dmh_import_jfm, dmh_import_cellcol,dmh_import_jinluname from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc) where dmh_import_cellcol>'%s' and right(dmh_import_jfm,%d)='%s'"), strCellCol, iMarkLen, strMark) ;
							if (cSetFindShare.IsOpen())
							{
								cSetFindShare.Close() ;
							}
							if (cSetFindShare.Open(g_PtrCon, strSql))
							{
								int iRsCountFindShare = cSetFindShare.GetRecordCount() ;
								if (iRsCountFindShare==1)
								{
									cSetFindShare.MoveFirst() ;
									cSetFindShare.GetFieldValue(_T("dmh_import_cellcol"), strCellCol2) ;
									cSetFindShare.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName2) ;
									cSetFindShare.GetFieldValue(_T("dmh_import_jfm"), strJFM2) ;

									bFoundShare = TRUE ;
									strHasFinished.AppendFormat(_T("%s,"), strCellCol2) ;
								}
								else if(iRsCountFindShare>1)
								{
									CString strMsg ;
									strMsg.Format(_T("下面查询结果大于1：\n%s"), strSql) ;	
									AfxMessageBox(strMsg) ;
								}
								cSetFindShare.Close() ;
							}
						}
					}
					//end 查询和当前进路共用发送器的进路

					if (!strJinLuName2.IsEmpty())
					{
						strJinLuName.AppendFormat(_T("/%s"), strJinLuName2) ;
					}

					strArrayFBJCell.Add(strJinLuName) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		//侧线股道表
		strSql.Format(_T("select distinct dmh_import_type, dmh_import_freq, dmh_import_xhj,dmh_import_jinluname, dmh_import_lineindex from dmh_import where dmh_import_table=1 order by dmh_import_lineindex asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					CString strJinLuName ;				
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					strArrayFBJCell.Add(strJinLuName) ;
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
		//绘制FBJ组合
		int iFBJCount = strArrayFBJCell.GetCount() ;
		int iNeedRow_FBJ = iFBJCount/10+(iFBJCount%10==0?0:1) ;
		for (int i=0; i<iNeedRow_FBJ; i++)
		{
			//插FBJ
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_box_z_fbj") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_box_z_cell") ;
			for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;
				if ((i*10+iCellIndex)<iFBJCount)
				{
					strCell = strArrayFBJCell.GetAt(i*10+iCellIndex) ;
					strArrayVal.Add(strCell) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
			iZLocRow-- ;
		}

		//end FBJ

		//插入汇流排
		if (m_bUseHLP==TRUE)
		{
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_box_z_hlp") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			strArrayVal.RemoveAll() ;
			strArrayVal.Add(_T("H1")) ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_boxd_z_cell") ;
			CString strHLPNames[14] = {_T("H-F2"),_T("H-F3"),_T("H-F5"),_T("H-F9"),_T("H-F10"),_T("H-F11"),_T("H-F12"),_T("H-F14"),_T("H-F15"),_T("H-F16"),_T("H-F17"),_T("H-F18"),_T("H-1700"),_T("H-2000")} ;
			for (int iCellIndex=0; iCellIndex<14; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidthHLP*iCellIndex+dCellWidthHLP/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight/2 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;	
				strCell = strHLPNames[iCellIndex] ;
				strArrayVal.Add(strCell) ;							
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
					pRef->close() ;
					pRef = NULL ;
				}				
			}
			iZLocRow-- ;
		}

		//插入Frame
		strBlockName = _T("frame_left") ;
		pt_3d_insert.x = dFrameBaseX ;
		pt_3d_insert.y = dFrameBaseY ;
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		//内边框
		AcGePoint3dArray ptArrayFrame ;
		ptArrayFrame.append(AcGePoint3d(-5, dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1), dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1), dFrameBaseY+297-5, 0)) ;
		ptArrayFrame.append(AcGePoint3d(-5, dFrameBaseY+297-5, 0)) ;
		g_cSystem.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
		//外边框
		ptArrayFrame.setAt(0,AcGePoint3d(dFrameBaseX, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(1,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1)+5, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(2,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1)+5, dFrameBaseY+297, 0)) ;
		ptArrayFrame.setAt(3,AcGePoint3d(dFrameBaseX, dFrameBaseY+297, 0)) ;
		g_cSystem.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	
		//签字栏
		strBlockName = _T("SIGN_3") ;
		pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*(iZIndex+1) ;
		pt_3d_insert.y = dFrameBaseY+5 ;
		strArrayVal.RemoveAll() ;
		strArrayVal.Add(m_strStation) ;
		strArrayVal.Add(m_strDwgSn) ;		
		strArrayVal.Add(CBlkUtility::GetDateStr()) ;
		strArrayVal.Add(_T("站内电码化组合柜布置图")) ;
		strArrayVal.Add(m_strProjectName) ;
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		g_cSystem.ModifyFontStyle(_T("standard"), _T("hztxt2.shx")) ;
		pDb->saveAs(strSaveFile) ;
		delete pDb ;
		pDb = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in DrawZ!"));			
	}
}

// 检查需不需要增加组合柜
void CZnDmh::CheckToAddZ(int & iZLocRow, int & iZIndex)
{
	if (iZLocRow==0) //需要换组合架了
	{
		iZLocRow = 10 ;
		iZIndex++ ;
		//插入一个组合架
		AcGePoint3d pt_3d_insert(iZIndex*(188+5),0,0) ;
		CStringArray strArrayVal ;
		AcDbBlockReference *pRef = NULL ;
		CString strZIndex ;
		strZIndex.Format(_T("%s%d"), m_strPreZ, iZIndex+1) ;
		strArrayVal.Add(strZIndex) ;
		strArrayVal.Add(_T("站内组合柜")) ;			
		if (g_cSystem.InsertBlock(pRef, _T("dmh_box_zzh_main") , pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}
	}
}

// 查询某条线有无 反发、正发、反接
void CZnDmh::HasJinLuType(const CString & strLineName, BOOL & bHasFanFa, BOOL & bHasZhengFa, BOOL & bHasFanJie)
{
	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select distinct dmh_import_type from dmh_import where dmh_import_line='%s'"), strLineName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;			
				while(!cSet.IsEOF())
				{
					CString strJinLuTypeTmp ;
					cSet.GetFieldValue(_T("dmh_import_type"), strJinLuTypeTmp) ;
					if (strJinLuTypeTmp==_T("反发"))
					{
						bHasFanFa = TRUE ;
					}
					else if (strJinLuTypeTmp==_T("正发"))
					{
						bHasZhengFa = TRUE ;
					}
					else if (strJinLuTypeTmp==_T("反接"))
					{
						bHasFanJie = TRUE ;
					}
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in HasJinLuType !"));			
	}
}

// 绘制站内移频柜图
void CZnDmh::DrawZY(const CString &strSavePath)
{
	CString strErroMsg ;

	AcDbDatabase *pDb = new AcDbDatabase() ;
	g_cSystem.m_pDb = pDb ;
	CString strSaveFile ;
	strSaveFile.Format(_T("%s\\移频柜布置图.dwg"), strSavePath) ;
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	double dMainTbWidth = 160.000 ; //移频柜主框宽度
	//double dMainTbRowHeight = 20.000 ; //每层高度
	//double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 24.000 ; //左侧序号、类型等列所占宽度（从框架左边框到单元格的距离）
	double dCellWidth = 27.200 ; //单元格宽度
	double dCellContentY_S = 28.000 ; //单元格填充图块的y坐标（在频率所在矩形的正中）上行
	double dCellContentY_X = 120.000 ; //单元格填充图块的y坐标（在频率所在矩形的正中）下行
	double dBetweenMainTb = 5.000 ; //ZY1和ZY2之间的距离
	double dFrameBaseX = -50.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -60.000 ; //左Frame的插入基点y
	double dFrameBaseX_Inside = -25.000 ; //左Frame的内框基点x
	double dMainTbToFrame = 20.000 ; //移频架到边框的距离

	BOOL bIsSetNPlusLoc = FALSE ;// 是否指定+1的位置
	BOOL bHasSetNPlusLoc = FALSE ;// 是否已写了+1的位置
	int iNPlusLoc[2] = {0,0} ;


	if (!m_strNPlusInZY.IsEmpty())
	{
		bIsSetNPlusLoc = TRUE ;
		CStringArray strArrayRet ;
		CBlkUtility::DivideString(m_strNPlusInZY, _T("-"), strArrayRet) ;
		if (strArrayRet.GetCount()==2)
		{
			CString strPre, strBack ; //分别对应ZY1-9的ZY1、9
			strPre =  strArrayRet.GetAt(0) ;
			strBack =  strArrayRet.GetAt(1) ;
			int iPreLen = strPre.GetLength() ;
			int iPreZYLen = m_strPreZY.GetLength() ; //m_strPreZY的len
			int iBackLen = strBack.GetLength() ; 
			if (iPreLen==iPreZYLen) //ZY1-9写成ZY-9的情况
			{
				iNPlusLoc[0] = 1 ;
			}
			else
			{
				iNPlusLoc[0] = _tstoi(strPre.Right(iPreLen-iPreZYLen)) ;
			}
			iNPlusLoc[1] = _tstoi(strBack) ;			
		}
	}

	try
	{
		//将数据读入array
		CADORecordset cSet ;
		CString strSql ;

		int iCellIndex_X = 0 ; //下行发送器总序号（下行排在上边，占用1FS、3FS......）
		int iCellIndex_S = 0 ; //上行发送器总序号（下行排在上边，占用2FS、4FS......）

		AcDbBlockReference *pRef = NULL ;
		CString strBlockName = _T("") ;
		AcGePoint3d pt_3d_insert(0,0,0) ;
		CStringArray strArrayVal ;

		//begin 读取第一个进路表(第一个表是进路，第二个是股道输入）
		//begin 遍历每条线路
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where dmh_import_table=0 order by dmh_import_lineindex asc")) ;
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

			while(!cSet.IsEOF())
			{
				CString strLineName ;
				cSet.GetFieldValue(_T("dmh_import_line"), strLineName) ; //例：下行线1

				int iDownOrUp = 0 ; //上下行，1/0
				iDownOrUp = (strLineName.Find(_T("下行"))==-1?1:0);
				int & iCellIndexCurent = (iDownOrUp==1?iCellIndex_S:iCellIndex_X) ; //引用单元格序号
				double dCellContentYCurent = (iDownOrUp==1?dCellContentY_S:dCellContentY_X) ;

				//begin 遍历一条线上的进路
				CADORecordset cSetOneLine ;
				strSql.Format(_T("select distinct dmh_import_jinluname, dmh_import_jfm, dmh_import_freq,dmh_import_cellcol from dmh_import where dmh_import_table=0 and dmh_import_line='%s' order by dmh_import_cellcol"), strLineName) ;
				if (cSetOneLine.IsOpen())
				{
					cSetOneLine.Close() ;
				}
				if (cSetOneLine.Open(g_PtrCon, strSql))
				{
					if (cSetOneLine.GetRecordCount()>0)
					{
						cSetOneLine.MoveFirst() ;

						CString strHasFinished = _T("") ; //记录已经处理过的进路的列
						while (!cSetOneLine.IsEOF())
						{
							CString strCellCol ;
							cSetOneLine.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ;
							//处理过的进路不再处理
							CString strToFind ;
							strToFind.Format(_T("%s,"), strCellCol) ;
							if (strHasFinished.Find(strToFind)!=-1)
							{
								cSetOneLine.MoveNext() ;
								continue;
							}

							strHasFinished.AppendFormat(_T("%s,"), strCellCol) ;

							CString strJinLuName, strJFM, strFreq ;
							cSetOneLine.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
							cSetOneLine.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;
							cSetOneLine.GetFieldValue(_T("dmh_import_freq"), strFreq) ;
                            
							//begin 查询和当前进路共用发送器的进路
							BOOL bFoundShare = FALSE ;
							CString strJinLuName2, strJFM2, strFreq2, strCellCol2 ;
							if (strJFM.Right(1)==_T(")"))
							{
								int iFindMarkLoc = strJFM.ReverseFind(_T('(')) ; //例：JM(1)，左括号的位置
								if (iFindMarkLoc!=-1)
								{
									int iMarkLen = strJFM.GetLength()-iFindMarkLoc ; //例：JM(1)，最后（1）的长度
									CString strMark = strJFM.Mid(iFindMarkLoc) ; //例：JM(1)中（1）
									CADORecordset cSetFindShare ;
									strSql.Format(_T("select * from (select distinct dmh_import_jinluname, dmh_import_jfm, dmh_import_freq,dmh_import_cellcol from dmh_import where dmh_import_table=0 and dmh_import_line='%s' order by dmh_import_cellcol) where dmh_import_cellcol>'%s' and right(dmh_import_jfm,%d)='%s'"), strLineName, strCellCol, iMarkLen, strMark) ;
									if (cSetFindShare.IsOpen())
									{
										cSetFindShare.Close() ;
									}
									if (cSetFindShare.Open(g_PtrCon, strSql))
									{
										int iRsCountFindShare = cSetFindShare.GetRecordCount() ;
										if (iRsCountFindShare==1)
										{
											cSetFindShare.MoveFirst() ;
											cSetFindShare.GetFieldValue(_T("dmh_import_cellcol"), strCellCol2) ;
											cSetFindShare.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName2) ;
											cSetFindShare.GetFieldValue(_T("dmh_import_jfm"), strJFM2) ;
											cSetFindShare.GetFieldValue(_T("dmh_import_freq"), strFreq2) ; 
											bFoundShare = TRUE ;
											strHasFinished.AppendFormat(_T("%s,"), strCellCol2) ;
										}
										else if(iRsCountFindShare>1)
										{
											strErroMsg.Format(_T("下面查询结果大于1：\n%s"), strSql) ;	
											AfxMessageBox(strErroMsg) ;
										}
										cSetFindShare.Close() ;
									}
								}
							}
							//end 查询和当前进路共用发送器的进路

							//begin 检查是否该位置被设置为放置N+1
							if (bIsSetNPlusLoc==TRUE)
							{
								if (iDownOrUp==((iNPlusLoc[1]+1)%2))
								{
									if (iNPlusLoc[0]==(int)(iCellIndexCurent/5+1)&&((int)(iNPlusLoc[1]-1)/2)==(int)(iCellIndexCurent%5))
									{
										pt_3d_insert.x = iCellIndexCurent/5*(dMainTbWidth+dBetweenMainTb)+iCellIndexCurent%5*dCellWidth+dLeftWidth+dCellWidth/2 ;
										pt_3d_insert.y = dCellContentYCurent ;
										strArrayVal.RemoveAll() ;									
										strArrayVal.Add(_T("站内设备（+1）")) ;
										strArrayVal.Add(_T("1700-1")) ;
										strArrayVal.Add(_T("2000-1")) ;
										strBlockName = _T("dmh_box_zy_cell_1") ;
										if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
										{
											pRef->close() ;
											pRef = NULL ;
										}
										iCellIndexCurent++ ;
										bHasSetNPlusLoc = TRUE ;
									}
								}
							}
							//end 检查是否该位置被设置为放置N+1

							//关于频率的输入还需要进一步确认
							CString strJinLuNameInCell ;
							strBlockName = _T("dmh_box_zy_cell") ;
							strArrayVal.RemoveAll() ;
							if (bFoundShare)
							{
								strJinLuNameInCell.Format(_T("%s/%s"), strJinLuName, strJinLuName2) ;
								strArrayVal.Add(strJinLuNameInCell) ;
								if (!strFreq.IsEmpty()&&!strFreq2.IsEmpty()&&strFreq!=strFreq2)
								{
									strBlockName = _T("dmh_box_zy_cell_1") ;
									strArrayVal.Add(strFreq) ;
									strArrayVal.Add(strFreq2) ;
								}
								else
								{
									strArrayVal.Add(strFreq) ;
								}
							}
							else
							{
								strArrayVal.Add(strJinLuName) ;
								strArrayVal.Add(strFreq) ;
							}
							pt_3d_insert.x = iCellIndexCurent/5*(dMainTbWidth+dBetweenMainTb)+iCellIndexCurent%5*dCellWidth+dLeftWidth+dCellWidth/2 ;
							pt_3d_insert.y = dCellContentYCurent ;
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								pRef->close() ;
								pRef = NULL ;
							}

							iCellIndexCurent++ ; //上行或者下行的cell总序号增1

							cSetOneLine.MoveNext() ;
						}
					}

					cSetOneLine.Close() ;
				}				
				//end 遍历一条线上的进路

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 遍历每条线路
		//end 读取第一个进路表(第一个表是进路，第二个是股道输入）
		
		//begin 读取第二个进路表(第一个表是进路，第二个是股道输入）		
		strSql.Format(_T("select distinct dmh_import_type, dmh_import_freq, dmh_import_xhj,dmh_import_jinluname, dmh_import_lineindex from dmh_import where dmh_import_table=1 order by dmh_import_lineindex asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				
				while (!cSet.IsEOF())
				{
					CString strDownOrUp, strFreq, strXhj, strJinLuName ;
					cSet.GetFieldValue(_T("dmh_import_type"), strDownOrUp) ;
					cSet.GetFieldValue(_T("dmh_import_xhj"), strXhj) ;
					cSet.GetFieldValue(_T("dmh_import_freq"), strFreq) ;
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					
					int iDownOrUp = 0 ; //上下行，1/0
					iDownOrUp = (strDownOrUp.Find(_T("下行"))==-1?1:0) ;
					int & iCellIndexCurent = (iDownOrUp==1?iCellIndex_S:iCellIndex_X) ; //引用单元格序号
					double dCellContentYCurent = (iDownOrUp==1?dCellContentY_S:dCellContentY_X) ;

					//begin 检查是否该位置被设置为放置N+1
					if (bIsSetNPlusLoc==TRUE)
					{
						if (iDownOrUp==((iNPlusLoc[1]+1)%2))
						{
							if (iNPlusLoc[0]==(int)(iCellIndexCurent/5+1)&&((int)(iNPlusLoc[1]-1)/2)==(int)(iCellIndexCurent%5))
							{
								pt_3d_insert.x = iCellIndexCurent/5*(dMainTbWidth+dBetweenMainTb)+iCellIndexCurent%5*dCellWidth+dLeftWidth+dCellWidth/2 ;
								pt_3d_insert.y = dCellContentYCurent ;
								strArrayVal.RemoveAll() ;									
								strArrayVal.Add(_T("站内设备（+1）")) ;
								strArrayVal.Add(_T("1700-1")) ;
								strArrayVal.Add(_T("2000-1")) ;
								strBlockName = _T("dmh_box_zy_cell_1") ;
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								iCellIndexCurent++ ;
								bHasSetNPlusLoc = TRUE ;
							}
						}
					}
					//end 检查是否该位置被设置为放置N+1

					strBlockName = _T("dmh_box_zy_cell") ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strJinLuName) ;
					strArrayVal.Add(strFreq) ;
					pt_3d_insert.x = iCellIndexCurent/5*(dMainTbWidth+dBetweenMainTb)+iCellIndexCurent%5*dCellWidth+dLeftWidth+dCellWidth/2 ;
					pt_3d_insert.y = dCellContentYCurent ;
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					iCellIndexCurent++ ; //上行或者下行的cell总序号增1

					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
		//end 读取第二个进路表(第一个表是进路，第二个是股道输入）

		//若没有指定N+1位置，增加一个+1(或者，指定了，但到目前为止还没写)
		if (bIsSetNPlusLoc==FALSE||(bIsSetNPlusLoc==TRUE&&bHasSetNPlusLoc==FALSE))
		{
			strBlockName = _T("dmh_box_zy_cell_1") ;
			strArrayVal.RemoveAll() ;
			strArrayVal.Add(_T("站内设备（+1）")) ;
			strArrayVal.Add(_T("1700-1")) ;
			strArrayVal.Add(_T("2000-1")) ;
			pt_3d_insert.x = iCellIndex_X/5*(dMainTbWidth+dBetweenMainTb)+iCellIndex_X%5*dCellWidth+dLeftWidth+dCellWidth/2 ;
			pt_3d_insert.y = dCellContentY_X ;
			if (bIsSetNPlusLoc==TRUE&&bHasSetNPlusLoc==FALSE)
			{
				pt_3d_insert.x =(iNPlusLoc[0]-1)*(dMainTbWidth+dBetweenMainTb)+(iNPlusLoc[1]-1)/2*dCellWidth+dLeftWidth+dCellWidth/2 ;
				pt_3d_insert.y = (iNPlusLoc[1]%2==0?dCellContentY_S:dCellContentY_X) ;
			}
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}

			iCellIndex_X++ ; //上行或者下行的cell总序号增1
		}

		//begin 绘制移频架 dmh_box_zy_main
		int iMaxCellIndex = max(iCellIndex_X, iCellIndex_S) ;
		//移频架总数
		int iZYCount = iMaxCellIndex/5+(iMaxCellIndex%5==0?0:1) ;		
		strBlockName = _T("dmh_box_zy_main") ;
		for (int iZYIndex=0; iZYIndex<iZYCount; iZYIndex++)
		{
			pt_3d_insert.x = iZYIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = 0 ;		
			strArrayVal.RemoveAll() ;
			CString strZYIndex ;
			strZYIndex.Format(_T("%s%d"), m_strPreZY, iZYIndex+1) ;
			strArrayVal.Add(strZYIndex) ;				
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
		}
		//end 绘制移频架 dmh_box_zy_main

		//插入Frame
		strBlockName = _T("frame_left") ;
		pt_3d_insert.x = dFrameBaseX ;
		pt_3d_insert.y = dFrameBaseY ;
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		//内边框
		AcGePoint3dArray ptArrayFrame ;
		ptArrayFrame.append(AcGePoint3d(dFrameBaseX_Inside, dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZYCount+dMainTbToFrame, dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZYCount+dMainTbToFrame, dFrameBaseY+297-5, 0)) ;
		ptArrayFrame.append(AcGePoint3d(dFrameBaseX_Inside, dFrameBaseY+297-5, 0)) ;
		g_cSystem.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
		//外边框
		ptArrayFrame.setAt(0,AcGePoint3d(dFrameBaseX, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(1,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZYCount+dMainTbToFrame+5, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(2,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*iZYCount+dMainTbToFrame+5, dFrameBaseY+297, 0)) ;
		ptArrayFrame.setAt(3,AcGePoint3d(dFrameBaseX, dFrameBaseY+297, 0)) ;
		g_cSystem.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	
		//签字栏
		strBlockName = _T("SIGN_3") ;
		pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZYCount+dMainTbToFrame ;
		pt_3d_insert.y = dFrameBaseY+5 ;
		strArrayVal.RemoveAll() ;
		strArrayVal.Add(m_strStation) ;
		strArrayVal.Add(m_strDwgSn) ;		
		strArrayVal.Add(CBlkUtility::GetDateStr()) ;
		strArrayVal.Add(_T("站内电码化移频柜布置图")) ;
		strArrayVal.Add(m_strProjectName) ;
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		g_cSystem.ModifyFontStyle(_T("standard"), _T("hztxt2.shx")) ;
		pDb->saveAs(strSaveFile) ;
		delete pDb ;
		pDb = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in DrawZY!"));			
	}
}

// 读入组合架到数据库
void CZnDmh::ReadZ(const CString & strFileName)
{
	CString strMsg ;

	double dMainTbWidth = 188.000 ; //组合柜主框宽度
	double dMainTbRowHeight = 20.000 ; //每层高度
	double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 38.000 ; //左侧序号、组合类型等列所占宽度（从框架左边框到组合单元的距离）
	double dCellWidth = 15.000 ; //组合单元格宽度
	double dCellWidthHLP = 9.375 ;//150/16,若单元格变化此值随着变
	double dBetweenMainTb = 5.000 ; //Z1和Z2之间的距离
	double dFrameBaseX = -30.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -40.000 ; //左Frame的插入基点y

	m_pDb = new AcDbDatabase(false) ;
	//CUserSystem mysys ;
	g_cSystem.m_pDb = m_pDb ;
	CDBUtility::ClearTable(_T("dmh_in_zlayer")) ;
	CDBUtility::ClearTable(_T("dmh_in_zcell")) ;
	if(Acad::eOk==m_pDb->readDwgFile(strFileName))
	{
		AcDbBlockTable * pBlkTbl = NULL ;
		m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

		pBlkTblRec->newIterator(pBlkTblRecIter);
		pBlkTblRec->close();

		AcDbEntity * pEnt = NULL ;
		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
		{
			if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;					
					CString strBlkName ;
					g_cSystem.GetBlockName(pRef, strBlkName) ;
					if (strBlkName==_T("dmh_box_zzh_main"))
					{
						CString strSql ;

						CString strZIndex ;
						g_cSystem.GetAttValue(pRef, _T("ZZH_INDEX"), strZIndex) ;
						int nZIndex = 0 ;
						//nZIndex = _tstoi(strZIndex.Right(1)) ; // 此处默认组合架序号应该都是个位数，有问题否？(20130909:有问题了，果然有ZY10),另如果只有一个组合架，要写成Z1，不能写Z
						nZIndex = _tstoi(strZIndex.Right(strZIndex.GetLength()-m_strPreZ.GetLength())) ; 
						if (nZIndex!=0)
						{
							AcGePoint3d pt_3d_z(0,0,0) ;
							pt_3d_z = pRef->position() ;
							//1~10层组合块插入点应该严格捕捉
							for (int iLayerIndex=0; iLayerIndex<10; iLayerIndex++)
							{
								AcGePoint3d pt_3d_layer(0,0,0) ;
								pt_3d_layer.x = pt_3d_z.x ;
								pt_3d_layer.y = pt_3d_z.y+iLayerIndex*20 ;
								//查找插入点在pt_3d_layer处的块
								CString strLayerBlkName, strShowType, strStartEnd1, strStartEnd2 ;
								CStringArray strArrayJdqName ;
								int iFindLayerNum = FindZLayer(pt_3d_layer, strLayerBlkName, strShowType, strStartEnd1, strStartEnd2, strArrayJdqName) ;
								if (iFindLayerNum==1)
								{
									//存组合layer信息
									strSql.Format(_T("insert into dmh_in_zlayer(dmh_in_zlayer_shelfnum, dmh_in_zlayer_layernum, dmh_in_zlayer_layerblock, dmh_in_zlayer_startendxhj1, dmh_in_zlayer_startendxhj2) values(%d, %d, '%s', '%s', '%s')"), nZIndex, iLayerIndex+1, strLayerBlkName, strStartEnd1, strStartEnd2) ;
									ExecSqlString(strSql) ;
									
									int nLayerId = 0 ;
									nLayerId = CDBUtility::GetTableMaxId(_T("dmh_in_zlayer")) ;

									if (strLayerBlkName==_T("dmh_box_z_hlp"))
									{
										//查找本层1~16box
										for (int iCellIndex=0; iCellIndex<16; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dCellWidthHLP;
											pt_3d_cell_bottomright.y = pt_3d_layer.y ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidthHLP ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dMainTbRowHeight ;
											//
											CStringArray strArrayCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell_D(pt_3d_cell_topleft, pt_3d_cell_bottomright, strArrayCellContent) ;

											if (iFindCellNum==1)
											{
												int iCellContentCount = strArrayCellContent.GetCount() ; //==1表示是dmh_boxd_z_cell1,==2表示是dmh_boxd_z_cell2
												if (iCellContentCount==1)
												{
													strSql.Format(_T("insert into dmh_in_zcell(dmh_in_zcell_layerid, dmh_in_zcell_shelfnum, dmh_in_zcell_layernum, dmh_in_zcell_boxnum, dmh_in_zcell_cellcontent) values(%d, %d, %d, %d, '%s')"), nLayerId, nZIndex, iLayerIndex+1, iCellIndex+1, strArrayCellContent.GetAt(0)) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在Z%d-%d的第%d位置处找到不止一个z_cell块，请检查是否有重合！"), nZIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
									else
									{
										//查找本层1~10继电器情况
										for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dMainTbRowHeight ;
											//
											CString strCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell(_T("dmh_box_z_cell"), pt_3d_cell_topleft, pt_3d_cell_bottomright, strCellContent) ;

											if (iFindCellNum==1)
											{
												if (!strCellContent.IsEmpty())
												{
													strSql.Format(_T("insert into dmh_in_zcell(dmh_in_zcell_layerid, dmh_in_zcell_shelfnum, dmh_in_zcell_layernum, dmh_in_zcell_boxnum, dmh_in_zcell_jdqname, dmh_in_zcell_cellcontent) values(%d, %d, %d, %d, '%s', '%s')"), nLayerId, nZIndex, iLayerIndex+1, iCellIndex+1, strArrayJdqName.GetAt(iCellIndex), strCellContent) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在Z%d-%d的第%d位置处找到不止一个z_cell块，请检查是否有重合！"), nZIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}


								}
								else if (iFindLayerNum>1)
								{
									strMsg.Format(_T("在(%.3f,%.3f)处找到不止一个组合块，请检查是否有重合！"), pt_3d_layer.x, pt_3d_layer.y) ;
									AfxMessageBox(strMsg) ;
									continue;
								}
								else
								{
									strMsg.Format(_T("\n在(%.3f,%.3f)处找到0个组合块！"), pt_3d_layer.x, pt_3d_layer.y) ;
									acutPrintf(strMsg) ;
									continue;
								}
							}
						}
					}
				}
				pEnt->close() ;
			}
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;
	}

	delete m_pDb ;
	m_pDb = NULL ;
}

// 查找在指定点的组合块（layer）
int CZnDmh::FindZLayer(const AcGePoint3d & pt_3d_base, CString & strBlockName, CString & strShowType, CString & strStartEndXhj1, CString & strStartEndXhj2, CStringArray & strArrayJdqName)
{
	int nRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;	
				AcGePoint3d pt_3d_layer(0,0,0) ;
				pt_3d_layer = pRef->position() ;
				if (pt_3d_layer==pt_3d_base)
				{	
					CString strBlockNameTmp ;
					g_cSystem.GetBlockName(pRef, strBlockNameTmp) ;
					if (strBlockNameTmp!=_T("dmh_box_zzh_main"))
					{
						strBlockName = strBlockNameTmp ;
						g_cSystem.GetAttValue(pRef, _T("START_END1"), strStartEndXhj1) ;
						g_cSystem.GetAttValue(pRef, _T("START_END2"), strStartEndXhj2) ;
						this->GetJdqNames(strBlockName, strArrayJdqName) ;
						if (strBlockName==_T("dmh_box_z_zjc"))
						{						
							int iJdqIndexs[5] = {4, 7, 8, 9, 10} ;
							for (int i=0; i<5; i++)
							{
								CString strJdqName ;
								CString strAppName ;
								strAppName.Format(_T("JDQ-NAME-%d"), iJdqIndexs[i]) ;
								if(g_cSystem.GetAttValue(pRef, strAppName, strJdqName))
								{
									strArrayJdqName.SetAt(iJdqIndexs[i]-1, strJdqName) ;
								}														
							}						
						}
						else if (strBlockName==_T("dmh_box_z_xhfs"))
						{
							for (int i=0; i<10; i++)
							{
								CString strJdqName ;
								CString strAppName ;
								strAppName.Format(_T("JDQ-NAME-%d"), i+1) ;
								if(g_cSystem.GetAttValue(pRef, strAppName, strJdqName))
								{
									strArrayJdqName.SetAt(i, strJdqName) ;
								}															
							}
						}
						nRet++ ;
					}

				}
			}
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return nRet;
}

// 查找组合中1~10继电器位置放置的元素
int CZnDmh::FindCell(const CString & strToFindBlk,const AcGePoint3d& pt_3d_topleft, const AcGePoint3d& pt_3d_bottomright, CString & strCellContent)
{
	int nRet = 0 ;

	CLongArray nArrayBlkHandle ;
	int iFindBlkNum = g_cSystem.FindBlkInArea(pt_3d_topleft, pt_3d_bottomright, nArrayBlkHandle) ;
	if (iFindBlkNum>0)
	{	
		AcDbEntity *pEnt = NULL ;
		for (int i=0; i<iFindBlkNum; i++)
		{				
			if(g_cSystem.OpenAcDbEntity(nArrayBlkHandle.GetAt(i), pEnt, AcDb::kForRead))
			{
				AcDbBlockReference *pRef = NULL ;
				pRef = AcDbBlockReference::cast(pEnt) ;
				CString strBlkName ;
				g_cSystem.GetBlockName(pRef, strBlkName) ;
				if (strBlkName==strToFindBlk)
				{
					CString strCellContentTmp ;
					g_cSystem.GetAttValue(pRef, _T("CELL_CONTENT"), strCellContentTmp) ;
					if (!strCellContentTmp.IsEmpty())
					{
						strCellContent = strCellContentTmp ;
					}
					nRet++ ;
				}

				pEnt->close() ;
			}
		}
	}

	return nRet ;
}

int CZnDmh::GetJdqNames(const CString & strBlkName, CStringArray & strArrayJdqNames)
{
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_def_z where dmh_def_z_blkname='%s' order by dmh_def_z_index"), strBlkName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					CString strJdqName = _T("") ;
					cSet.GetFieldValue(_T("dmh_def_z_jdqname"), strJdqName) ;
					strArrayJdqNames.Add(strJdqName) ;
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in GetJdqNames!"));			
	}
	return 0;
}

// 读入综合柜图
void CZnDmh::ReadZZH(const CString & strFileName)
{
	CString strMsg ;

	double dMainTbWidth = 188.000 ; //综合柜主框宽度
	double dMainTbRowHeight = 20.000 ; //每层高度
	double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 38.000 ; //左侧序号、组合类型等列所占宽度（从框架左边框到组合单元的距离）
	double dTFGZCellWidth = 25.000 ; //TFGZ、MGL-R组合单元格宽度
	double dMGLFCellWidth = 40.000 ; //MGL-F组合单元格宽度
	double dSRTZCellWidth = 12.500 ; //SRTZ组合单元格宽度
	double dNFLCellWidth = 8.000 ; //防雷单元格宽度
	double dNFLCellHeight = 5.000 ; //防雷单元格高度
	double dBetweenMainTb = 5.000 ; //ZZH1和ZZH2之间的距离
	double dFrameBaseX = -30.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -40.000 ; //左Frame的插入基点y

	CString strBlockName_Cell = _T("dmh_box_zzh_cell");


	m_pDb = new AcDbDatabase(false) ;
	//CUserSystem mysys ;
	g_cSystem.m_pDb = m_pDb ;
	CDBUtility::ClearTable(_T("dmh_in_zzhlayer")) ;
	CDBUtility::ClearTable(_T("dmh_in_zzhcell")) ;

	Acad::ErrorStatus es = m_pDb->readDwgFile(strFileName) ;
	if(Acad::eOk== es)
	{
		AcDbBlockTable * pBlkTbl = NULL ;
		m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

		pBlkTblRec->newIterator(pBlkTblRecIter);
		pBlkTblRec->close();

		AcDbEntity * pEnt = NULL ;
		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
		{
			if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;					
					CString strBlkName ;
					g_cSystem.GetBlockName(pRef, strBlkName) ;
					if (strBlkName==_T("dmh_box_zzh_main"))
					{
						CString strSql ;

						CString strZZHIndex ;
						g_cSystem.GetAttValue(pRef, _T("ZZH_INDEX"), strZZHIndex) ;
						int nZZHIndex = 0 ;
						//nZZHIndex = _tstoi(strZZHIndex.Right(1)) ; // 此处默认综合架序号应该都是个位数，有问题否？(20130909:有问题了，果然有ZY10),另如果只有一个综合架，要写成ZZH1，不能写ZZH
						nZZHIndex = _tstoi(strZZHIndex.Right(strZZHIndex.GetLength()-m_strPreZZH.GetLength())) ; 
						if (nZZHIndex!=0)
						{
							AcGePoint3d pt_3d_zzh(0,0,0) ;
							pt_3d_zzh = pRef->position() ;
							//1~10层组合块插入点应该严格捕捉
							for (int iLayerIndex=0; iLayerIndex<10; iLayerIndex++)
							{
								AcGePoint3d pt_3d_layer(0,0,0) ;
								pt_3d_layer.x = pt_3d_zzh.x ;
								pt_3d_layer.y = pt_3d_zzh.y+iLayerIndex*dMainTbRowHeight ;
								//查找插入点在pt_3d_layer处的块
								CString strLayerBlkName ;								
								int iFindLayerNum = FindZZHLayer(pt_3d_layer, strLayerBlkName) ;
								if (iFindLayerNum==1)
								{
									//存组合layer信息
									strSql.Format(_T("insert into dmh_in_zzhlayer(dmh_in_zzhlayer_shelfnum, dmh_in_zzhlayer_layernum, dmh_in_zzhlayer_layerblock) values(%d, %d, '%s')"), nZZHIndex, iLayerIndex+1, strLayerBlkName) ;
									ExecSqlString(strSql) ;

									int nLayerId = 0 ;
									nLayerId = CDBUtility::GetTableMaxId(_T("dmh_in_zzhlayer")) ;

									//查找本层各单元格情况
									if (strLayerBlkName==_T("dmh_box_zzh_mgfl-t")) 
									{
										for (int iCellIndex=0; iCellIndex<36; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex/2*dNFLCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y+((iCellIndex+1)%2)*dNFLCellHeight*2 ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dNFLCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dNFLCellHeight ;
											//
											CString strCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell(strBlockName_Cell, pt_3d_cell_topleft, pt_3d_cell_bottomright, strCellContent) ;

											if (iFindCellNum==1)
											{
												if (!strCellContent.IsEmpty())
												{
													strSql.Format(_T("insert into dmh_in_zzhcell(dmh_in_zzhcell_layerid, dmh_in_zzhcell_shelfnum, dmh_in_zzhcell_layernum, dmh_in_zzhcell_boxnum, dmh_in_zzhcell_cellcontent) values(%d, %d, %d, %d, '%s')"), nLayerId, nZZHIndex, iLayerIndex+1, iCellIndex+1, strCellContent) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在ZZH%d-%d的第%d位置处找到不止一个dmh_box_zzh_cell块，请检查是否有重合！"), nZZHIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
									else if (strLayerBlkName==_T("dmh_box_zzh_tfgz"))
									{
										for (int iCellIndex=0; iCellIndex<6; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dTFGZCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y+dRowTopBottomHeight ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dTFGZCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_layer.y+dMainTbRowHeight-dRowTopBottomHeight ;
											//
											CString strCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell(strBlockName_Cell, pt_3d_cell_topleft, pt_3d_cell_bottomright, strCellContent) ;

											if (iFindCellNum==1)
											{
												if (!strCellContent.IsEmpty())
												{
													strSql.Format(_T("insert into dmh_in_zzhcell(dmh_in_zzhcell_layerid, dmh_in_zzhcell_shelfnum, dmh_in_zzhcell_layernum, dmh_in_zzhcell_boxnum, dmh_in_zzhcell_cellcontent) values(%d, %d, %d, %d, '%s')"),nLayerId, nZZHIndex, iLayerIndex+1, iCellIndex+1, strCellContent) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在ZZH%d-%d的第%d位置处找到不止一个dmh_box_zzh_cell块，请检查是否有重合！"), nZZHIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}

									else if (strLayerBlkName==_T("dmh_box_zzh_mgl-r"))
									{
										for (int iCellIndex=0; iCellIndex<5; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dTFGZCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y+dRowTopBottomHeight ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dTFGZCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_layer.y+dMainTbRowHeight-dRowTopBottomHeight ;
											//
											CString strCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell(strBlockName_Cell, pt_3d_cell_topleft, pt_3d_cell_bottomright, strCellContent) ;

											if (iFindCellNum==1)
											{
												if (!strCellContent.IsEmpty())
												{
													strSql.Format(_T("insert into dmh_in_zzhcell(dmh_in_zzhcell_layerid, dmh_in_zzhcell_shelfnum, dmh_in_zzhcell_layernum, dmh_in_zzhcell_boxnum, dmh_in_zzhcell_cellcontent) values(%d, %d, %d, %d, '%s')"),nLayerId, nZZHIndex, iLayerIndex+1, iCellIndex+1, strCellContent) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在ZZH%d-%d的第%d位置处找到不止一个dmh_box_zzh_cell块，请检查是否有重合！"), nZZHIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
									else if (strLayerBlkName==_T("dmh_box_zzh_mgl-f"))
									{
										for (int iCellIndex=0; iCellIndex<3; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dMGLFCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y+dRowTopBottomHeight ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dMGLFCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_layer.y+dMainTbRowHeight-dRowTopBottomHeight ;
											//
											CString strCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell(strBlockName_Cell, pt_3d_cell_topleft, pt_3d_cell_bottomright, strCellContent) ;

											if (iFindCellNum==1)
											{
												if (!strCellContent.IsEmpty())
												{
													strSql.Format(_T("insert into dmh_in_zzhcell(dmh_in_zzhcell_layerid, dmh_in_zzhcell_shelfnum, dmh_in_zzhcell_layernum, dmh_in_zzhcell_boxnum, dmh_in_zzhcell_cellcontent) values(%d, %d, %d, %d, '%s')"),nLayerId, nZZHIndex, iLayerIndex+1, iCellIndex+1, strCellContent) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在ZZH%d-%d的第%d位置处找到不止一个dmh_box_zzh_cell块，请检查是否有重合！"), nZZHIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}									
									else if (strLayerBlkName==_T("dmh_box_zzh_srtz"))
									{
										for (int iCellIndex=0; iCellIndex<24; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+(iCellIndex/2)*dSRTZCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y+dRowTopBottomHeight*(1+(iCellIndex+1)%2) ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dSRTZCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dRowTopBottomHeight ;
											//
											CString strCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell(strBlockName_Cell, pt_3d_cell_topleft, pt_3d_cell_bottomright, strCellContent) ;

											if (iFindCellNum==1)
											{
												if (!strCellContent.IsEmpty())
												{
													strSql.Format(_T("insert into dmh_in_zzhcell(dmh_in_zzhcell_layerid, dmh_in_zzhcell_shelfnum, dmh_in_zzhcell_layernum, dmh_in_zzhcell_boxnum, dmh_in_zzhcell_cellcontent) values(%d, %d, %d, %d, '%s')"),nLayerId, nZZHIndex, iLayerIndex+1, iCellIndex+1, strCellContent) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在ZZH%d-%d的第%d位置处找到不止一个dmh_box_zzh_cell块，请检查是否有重合！"), nZZHIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
								}
								else if (iFindLayerNum>1)
								{
									strMsg.Format(_T("在(%.3f,%.3f)处找到不止一个组合块，请检查是否有重合！"), pt_3d_layer.x, pt_3d_layer.y) ;
									AfxMessageBox(strMsg) ;
									continue;
								}
								else
								{
									strMsg.Format(_T("\n在(%.3f,%.3f)处找到0个组合块！"), pt_3d_layer.x, pt_3d_layer.y) ;
									acutPrintf(strMsg) ;
									continue;
								}
							}
						}
					}
				}
				pEnt->close() ;
			}
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;
	}

	delete m_pDb ;
	m_pDb = NULL ;
}

int CZnDmh::FindZZHLayer(const AcGePoint3d & pt_3d_base, CString & strBlkName)
{
	int nRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;	
				AcGePoint3d pt_3d_layer(0,0,0) ;
				pt_3d_layer = pRef->position() ;
				if (pt_3d_layer==pt_3d_base)
				{					
					g_cSystem.GetBlockName(pRef, strBlkName) ;
					if (strBlkName!=_T("dmh_box_zzh_main"))
					{		
						nRet++ ;
					}
				}
			}
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return nRet;
}

// 读入站内移频柜
void CZnDmh::ReadZY(const CString & strFileName)
{
	CString strMsg ;

	double dMainTbWidth = 160.000 ; //移频柜主框宽度
	double dLeftWidth = 24.000 ; //左侧序号、类型等列所占宽度（从框架左边框到单元格的距离）
	double dCellWidth = 27.000 ; //单元格宽度
	double dCellTopLeftY_X = 132.000 ; //第3层，1FS单元格
	double dCellBottomRightY_X = 112.000 ; //第3层，1FS单元格
	double dCellTopLeftY_S = 40.000 ; //第1层，2FS单元格
	double dCellBottomRightY_S = 20.000 ; //第1层，2FS单元格
	double dBetweenMainTb = 5.000 ; //ZY1和ZY2之间的距离
	double dFrameBaseX = -50.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -60.000 ; //左Frame的插入基点y
	double dFrameBaseX_Inside = -25.000 ; //左Frame的内框基点x
	double dMainTbToFrame = 20.000 ; //移频架到边框的距离

	m_pDb = new AcDbDatabase(false) ;
	g_cSystem.m_pDb = m_pDb ;
	CDBUtility::ClearTable(_T("dmh_in_zycell")) ;

	Acad::ErrorStatus es = m_pDb->readDwgFile(strFileName) ;
	if(Acad::eOk== es)
	{
		AcDbBlockTable * pBlkTbl = NULL ;
		m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

		pBlkTblRec->newIterator(pBlkTblRecIter);
		pBlkTblRec->close();

		AcDbEntity * pEnt = NULL ;
		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
		{
			if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;					
					CString strBlkName ;
					g_cSystem.GetBlockName(pRef, strBlkName) ;
					if (strBlkName==_T("dmh_box_zy_main"))
					{
						CString strSql ;

						CString strZYIndex ;
						g_cSystem.GetAttValue(pRef, _T("ZY_INDEX"), strZYIndex) ;
						int nZYIndex = 0 ;
						//nZYIndex = _tstoi(strZYIndex.Right(1)) ; // 此处默认移频架序号应该都是个位数，有问题否？(20130909:有问题了，果然有ZY10),另如果只有一个移频架，要写成ZY1，不能写ZY
						nZYIndex = _tstoi(strZYIndex.Right(strZYIndex.GetLength()-m_strPreZY.GetLength())) ;
						if (nZYIndex!=0)
						{
							AcGePoint3d pt_3d_zy(0,0,0) ;
							pt_3d_zy = pRef->position() ;
							//1~10FS
							for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
							{
								AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
								pt_3d_cell_topleft.x = pt_3d_zy.x + dLeftWidth + iCellIndex/2*dCellWidth;
								pt_3d_cell_bottomright.y = pt_3d_zy.y+(iCellIndex%2==1?dCellBottomRightY_S:dCellBottomRightY_X) ;
								pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidth ;
								pt_3d_cell_topleft.y = pt_3d_zy.y+(iCellIndex%2==1?dCellTopLeftY_S:dCellTopLeftY_X) ;
								
								CString strJinLuName, strFreq1, strFreq2 ;								
								int iFindCellNum = FindZYCell(pt_3d_cell_topleft, pt_3d_cell_bottomright, strJinLuName, strFreq1, strFreq2) ;
								if (iFindCellNum==1)
								{
									//存FS cell信息
									strSql.Format(_T("insert into dmh_in_zycell(dmh_in_zycell_shelfnum, dmh_in_zycell_boxnum, dmh_in_zycell_jinluname, dmh_in_zycell_freq1, dmh_in_zycell_freq2) values(%d, %d, '%s', '%s', '%s')"), nZYIndex, iCellIndex+1, strJinLuName, strFreq1, strFreq2) ;
									ExecSqlString(strSql) ;

								}
								else if (iFindCellNum>1)
								{
									strMsg.Format(_T("在(%.3f,%.3f)(%.3f,%.3f)处找到不止一个单元块，请检查是否有重合！"), pt_3d_cell_topleft.x, pt_3d_cell_topleft.y, pt_3d_cell_bottomright.x, pt_3d_cell_bottomright.y) ;
									AfxMessageBox(strMsg) ;
									continue;
								}
								else
								{
									strMsg.Format(_T("\n在(%.3f,%.3f)(%.3f,%.3f)处找到0个单元块！"),pt_3d_cell_topleft.x, pt_3d_cell_topleft.y, pt_3d_cell_bottomright.x, pt_3d_cell_bottomright.y) ;
									acutPrintf(strMsg) ;
									continue;
								}
							}
						}
					}
				}
				pEnt->close() ;
			}
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;
	}

	delete m_pDb ;
	m_pDb = NULL ;
}

// 查找ZY移频柜单元块
int CZnDmh::FindZYCell(const AcGePoint3d & pt_3d_topleft, const AcGePoint3d & pt_3d_bottomright, CString & strJinLuName, CString & strFreq1, CString & strFreq2)
{
	int nRet = 0 ;

	CLongArray nArrayBlkHandle ;
	int iFindBlkNum = g_cSystem.FindBlkInArea(pt_3d_topleft, pt_3d_bottomright, nArrayBlkHandle) ;
	if (iFindBlkNum>0)
	{	
		AcDbEntity *pEnt = NULL ;
		for (int i=0; i<iFindBlkNum; i++)
		{				
			if(g_cSystem.OpenAcDbEntity(nArrayBlkHandle.GetAt(i), pEnt, AcDb::kForRead))
			{
				AcDbBlockReference *pRef = NULL ;
				pRef = AcDbBlockReference::cast(pEnt) ;
				CString strBlkName ;
				g_cSystem.GetBlockName(pRef, strBlkName) ;
				if (strBlkName.Find(_T("dmh_box_zy_cell"))!=-1)
				{					
					g_cSystem.GetAttValue(pRef, _T("JINLUNAME"), strJinLuName) ;
					g_cSystem.GetAttValue(pRef, _T("FREQ1"), strFreq1) ;
					g_cSystem.GetAttValue(pRef, _T("FREQ2"), strFreq2) ;

					nRet++ ;
				}

				pEnt->close() ;
			}
		}
	}

	return nRet ;
}

// 绘制电路图
void CZnDmh::DrawCircuit(const CString &strSavePath)
{
	CString strMsg ;
	CUserSystem mysys ;

	double dWidth_NGL_Code = 65.000 ;
	double dWidth_NGL_NoCode = 56.000 ; 
	double dWidthZJC_0000 = 40.000 ;
	double dWidthGCJ_0000 = 65.000 ;
	double dWidthZJC_FMJ = 53.000 ;
	double dWidthFS = 253.000 ; 

	double dNGL_Y = 0.000 ;
	double dGCJ_Y = -84.000 ; //GCJ块插入点Y坐标

	double dOffsetX_NGL_GCJ = 17.000 ; //NGL和GCJ插入点x坐标不是对齐的，需要错开一点

	double dWidth_TFGZ = 50.000 ;
	double dWidthFBJ = 59.000 ;
	double dOffsetY_FS = 26.000; // FS块插入时Y偏离dGCJ_Y的距离
	double dOffsetY_FSPLUS = 47.000 ;//+1FS插入时Y偏离dGCJ_Y的距离
	double dWidthGJFALL_0000 = 180.000 ;
	double dWidthGJFALL_0001 = 130.000 ;
	double dWidthGJFGJ_0000 = 165.000 ;
	double dWidthGJFGJ_0001 = 80.000 ;
	double dWidthZJC_0002 = 80.000 ;

	double dWidthZBJH = 33.000 ;
	double dWidthGJFGCJ = 28.000 ;
	double dWidthGCJGJF_JFMJ = 28.000 ;

	double dWidth_ZLOC = 25.000 ; //组合位置图框的宽度
	double dHeight_ZLOC = 5.000 ;

	double dHeight_NGL = 75.000 ;
	double dHeight_ZJC = 71.000 ;
	double dHeight_GCJGJF_GJF = 28.000 ;
	double dHeight_GCJGJF_Frame = 62.000 ;

	double dBetween_NGL_ZJC = 9.000 ;

	//侧线股道等
	double dOffsetY_NGLGJF1 = 50.000 ; //侧线的NGL、GJF1相对正线时Y坐标偏移
	
	try
	{
		CString strSql ;
		CADORecordset cSet ;
		m_ctrlProgressAll->SetRange(1, 3) ;
		
		//先处理第一个table
		
		m_ctrlProgressAll->SetPos(1) ;		
		
		//::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)0, 0) ;	
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex, dmh_import_type, dmh_import_xhj, dmh_import_cellcol, dmh_import_jinluname from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int nRsCount = cSet.GetRecordCount() ;
			//acedSetStatusBarProgressMeter(_T("生成电码化电路"), 1, nRsCount) ;
			m_ctrlProgressCur->SetRange(1, nRsCount) ;
			
			if (nRsCount>0)
			{
				cSet.MoveFirst() ;

				int iGuDaoFenGe = 1 ; //股道是否分割，每条线都有正接，只在正接时判断是否分割，遇到正接就重新设置该值,1表示无分割，其他表示有分割
				CStringArray strArrayGuDao ;
				CString strJZXHJ,strFJZXHJ,strCZXHJ, strFCZXHJ ; //每条线假定必有正接，故只在一条线的正接进路中取一次进站、出站 、反出站、反进站//20140626: 有只有正接或正发等的可能
				CString strGuDao ; //股道名称
				int iMaxGCJ_ZJ = 0, iMaxGCJ_ZF = 0 ; //保存正接和正发的GCJ个数

				int iDwgIndex = 0 ;

				int iLineIndexPre = 0 ; //如果当前进路不是ZJ,但是不和上条进路是一条线也须单独计算进出站信号机等，不能沿用正接时计算的进出站信号机
				while (!cSet.IsEOF())
				{
					//acedSetStatusBarProgressMeterPos(iDwgIndex+1) ;
					m_ctrlProgressCur->SetPos(iDwgIndex+1) ;
					//::PostMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSPOS, (WPARAM)(iDwgIndex+1), 0) ;

					//一条记录就是一张图纸
					CString strSaveFileName ; //存储电码化图纸的文件名					
					AcDbDatabase *pSaveDb = new AcDbDatabase ; //存储数据库
					mysys.m_pDb = pSaveDb;	
					//记录扩展数据值
					CMapStringToString mapToReplace, mapStrToStrXdata, mapToAddXdata ;
					CString strCircuitTemplete ; //当前选择的底图模板
					AcGeMatrix3d xform;  
					AcGeVector3d vec(0,0,0) ;

					CString strLine, strJinLuType, strStartEndXhj, strCellCol, strJinLuName ;
					int iLineIndex = 0 ;
					cSet.GetFieldValue(_T("dmh_import_line"), strLine) ; //例：下行线1
					cSet.GetFieldValue(_T("dmh_import_lineindex"), iLineIndex) ; //例：下行线1为0
					cSet.GetFieldValue(_T("dmh_import_type"), strJinLuType) ; //例：正接
					cSet.GetFieldValue(_T("dmh_import_xhj"), strStartEndXhj) ; //例：X->XI
					cSet.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ; //例：B
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ; //进路名，例：XJM

					strMsg.Format(_T("正线电码化-%s(%d/%d)"), strJinLuName, iDwgIndex+1, nRsCount) ;
					::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)(&strMsg), 0) ;

					int iMaxGCJ = 0 ; //当前进路GCJ的最大编号（个数）

					int iUpOrDown = 0 ; //0：下行，1：上行
					int iRunDirect = 0 ; //运行方向，决定图纸布局，0，从左往右，1，从右往左
					if (strLine.Find(_T("下行"))!=-1)
					{
						iUpOrDown = 0 ;
						if (strJinLuType.Find(_T("正"))!=-1)
						{
							iRunDirect = 0 ;
						}
						else
						{
							iRunDirect = 1 ;
						}
					}
					else
					{
						iUpOrDown = 1 ;
						if (strJinLuType.Find(_T("正"))!=-1)
						{
							iRunDirect = 1 ;
						}
						else
						{
							iRunDirect = 0 ;
						}
					}

					if (strJinLuType==_T("正接"))
					{ 
						//取得进出站信号机等信息,只在每条线的正接进路中执行此操作//20140626:如果当前进路不是ZJ,但是不和上条进路是一条线也须单独计算进出站信号机等，不能沿用正接时计算的进出站信号机
						strJZXHJ = strFCZXHJ = strCZXHJ = strFJZXHJ = _T("") ;
						this->GetLineInfo(strLine, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;

						//股道
						iGuDaoFenGe = 1 ;
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and left(dmh_import_guidao,1)='*' order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iGuDaoFenGe = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;
						if (iGuDaoFenGe==1)
						{
							strGuDao = strArrayGuDao.GetAt(0) ;
						}
						else if (iGuDaoFenGe>1)
						{
							CString strGuDaoTmp = strArrayGuDao.GetAt(0) ;
							strGuDao = strGuDaoTmp.Left(strGuDaoTmp.GetLength()-1) ;
						}						

						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;
						iMaxGCJ_ZJ = iMaxGCJ ;						

						strSaveFileName.Format(_T("%s\\%s%s正向接车进路电码化电路图.dwg"), strSavePath, m_strStation, strStartEndXhj) ;

						//acutPrintf(_T("\n%s"), strSaveFileName) ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标
								double dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								double dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X								

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;									
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ								
									this->FillXdataNGL(nGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_0%d%d%d_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_1%d00_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,0,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((iGuDaoFenGe==1&&nGuiDaoIndex==0)||(iGuDaoFenGe>1&&(nGuiDaoIndex==(iGuDaoFenGe-1))))
										{
											dBaseX-=dWidthZJC_0000 ; 
										}
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((nGuiDaoIndex==0&&iGuDaoFenGe==1)||(iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1))
										{
											dBaseX+=dWidthZJC_0000 ; 
										}
									}
									//AcGeVector3d vec(nGuiDaoIndex*(iIsSendCode==0?56:65)*(iIsUpOrDown==0?-1:1),0, 0); 

									xformNGL.setToTranslation(vecNGL);	
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									//begin 股道无分割插入GCJ_0000（GCJ1）和ZJC_0000块
									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{
										//GCJ_0000
										this->FillXdataGCJ_0000(0, strGuiDao, strStartEndXhj, mapStrToStrXdata, mapToAddXdata) ;
										vecNGL.x =vecNGL.x+dOffsetX_NGL_GCJ ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//ZJC_JMJ
										this->FillXdataJMJ_0000(0, iGuDaoFenGe, strStartEndXhj, mapStrToStrXdata, mapToAddXdata) ;
										vecNGL.x = vecNGL.x+(iRunDirect==0?-dWidthZJC_0000:dWidthGCJ_0000) ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_jmj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 股道无分割插入ZJC块（GCJ1）

									//begin 股道有分割时，插入ZJC_JMJ块（无GCJ1）
									if (iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1)
									{
										this->FillXdataJMJ_0000(0, iGuDaoFenGe, strStartEndXhj, mapStrToStrXdata, mapToAddXdata) ;
										//double dZJC_X = dBaseX+20*(iRunDirect==0?1:-1) ;
										vecNGL.x = vecNGL.x+(iRunDirect==0?(dOffsetX_NGL_GCJ-dWidthZJC_0000):dOffsetX_NGL_GCJ+dWidthGCJ_0000) ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_jmj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata,mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 股道有分割时，插入ZJC块（无GCJ1）

									//begin 绘制GCJ
									if (!(iGuDaoFenGe==1&&nGuiDaoIndex==0) && iIsSendCode==1) //第一个是GCJ1，股道,不再1GCJ~10GCJ
									{
										this->FillXdataGCJ(nGCJIndex, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 0) ;
										double dGCJ_X = dBaseX+dOffsetX_NGL_GCJ+(iRunDirect==0?0:-dWidth_NGL_Code) ;
										if (iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1)
										{
											dGCJ_X = dGCJ_X+dWidthZJC_0000*(iRunDirect==0?1:-1) ; //股道分割时，股道和非股道区段之间空出一定距离，放置ZJC
										}
										vecNGL.x = dGCJ_X ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_0%d%d%d_%d.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:1, nGCJIndex<10?0:1,nGCJIndex<10?nGCJIndex:0, iRunDirect) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_000%d_%d.dwg"), m_strSysBasePath, nGCJIndex%2==0?2:1, iRunDirect) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										if (nGCJIndex==6) //记录1CDA-2图框起始位置
										{
											dCDA2_StartX = dBaseX+(iRunDirect==0?(65+17):-47) ;
										}
										if (nGCJIndex>=6)
										{
											dCDA2_EndX = dBaseX+(iRunDirect==0?17:17) ;
										}
									}
									// end 绘制GCJ

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数		

								//begin 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）
								double dGCJZLoc1_X = 0 ;
								double dGCJZLoc1_Y = -170.000 ;
								CString strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								CString strZhShowName = _T("1CDA") ;
								if (iGuDaoFenGe==1) //股道无分割
								{
									if (dCDA2_StartX!=0) //说明GCJ个数大于5
									{
										strZhShowName = _T("1CDA-1") ;
									}
								}
								else //股道有分割时此处必是1CDB
								{
									strZLayerBlkName = _T("dmh_box_z_1cdb") ;
									strZhShowName = _T("1CDB") ;
								}
								this->FillXdataGCJZLoc(0, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）

								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -155,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -84,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -84,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -155,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;
									strZhShowName = _T("1CDA-2") ;
									if (iGuDaoFenGe>1)
									{
										strZhShowName = _T("1CDA") ;
									}
									this->FillXdataGCJZLoc(0, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

									double dGCJZLoc2_X = (iRunDirect==0?dCDA2_EndX:dCDA2_StartX) ;
									double dGCJZLoc2_Y =  -155 ;		
									vec.x = dGCJZLoc2_X ;
									vec.y = dGCJZLoc2_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）								

								//begin 绘制ZPW.TFGZ
								double dTFGZLoc_X = dWidthGCJ_0000+dOffsetX_NGL_GCJ ; //下行时TFGZ块的插入坐标
								double dTFGZLoc_Y = dGCJ_Y ;
								if (iRunDirect==1)
								{
									dTFGZLoc_X = -dWidth_TFGZ+dOffsetX_NGL_GCJ ;
								}
								this->FillXdataTFGZ(strJinLuName, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 0) ;
								vec.x = dTFGZLoc_X ;
								vec.y = dTFGZLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_tfgz_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZPW.TFGZ

								//begin 绘制正接 ZJC_FMJ
								double dZJC_FMJ_X = dTFGZLoc_X+dWidth_TFGZ ;
								double dZJC_FMJ_Y = dTFGZLoc_Y ;
								if (iRunDirect==1)
								{
									dZJC_FMJ_X = dTFGZLoc_X-dWidthZJC_FMJ  ;
								}
								this->FillXdataZFJC_FMJ(iLineIndex, strJZXHJ, strFCZXHJ, strCZXHJ,strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZJC_FMJ_X ;
								vec.y = dZJC_FMJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fmj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZJC_FMJ(zjc_0001)	

								//begin 绘制说明note
								double dNoteLoc_X = dZJC_FMJ_X ;
								double dNoteLoc_Y = dZJC_FMJ_Y+60 ;
								vec.x = dNoteLoc_X ;
								vec.y = dNoteLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_note_0000_0.dwg"), m_strSysBasePath) ;
								mapStrToStrXdata.RemoveAll() ;
								mapToAddXdata.RemoveAll() ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制说明note

								//begin 绘制FBJ块
								double dFBJLoc_X = dZJC_FMJ_X+dWidthZJC_FMJ ;
								double dFBJLoc_Y = dTFGZLoc_Y ;
								if (iRunDirect==1)
								{
									dFBJLoc_X = dZJC_FMJ_X-dWidthFBJ ;
								}
								this->FillXdataFBJ(0, strJinLuName,mapToReplace,mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFBJLoc_X ;
								vec.y = dFBJLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fbj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FBJ块

								//begin 绘制FS块
								double dFSLoc_X = dFBJLoc_X+dWidthFBJ ;
								double dFSLoc_Y = dGCJ_Y-dOffsetY_FS ;
								if (iRunDirect==1)
								{
									dFSLoc_X = dFBJLoc_X ;
								}
								this->FillXdataFS(iUpOrDown, 0, strJinLuName,mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFSLoc_X ;
								vec.y = dFSLoc_Y ;
								xform.setTranslation(vec) ;
								//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc_3_%d.dwg"), m_strSysBasePath, iIsUpOrDown) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fs_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FS块

								//begin 绘制零散电路
								double dLingSanLoc_FBJ_X = dFSLoc_X+(iRunDirect==0?110:-180) ; //运行方向不同，块布置位置不同
								double dLingSanLoc_FBJ_Y = dFSLoc_Y-85 ;
								this->FillXdataLingSan_FBJ(0, strJinLuName, mapStrToStrXdata) ;
								vec.x = dLingSanLoc_FBJ_X ;
								vec.y = dLingSanLoc_FBJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lingsan_0001_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制零散电路

								//begin 绘制bianma块
								double dBianMaLoc_X = dFSLoc_X+16 ;
								double dBianMaLoc_Y = dFSLoc_Y+25 ;
								if (iRunDirect==1)
								{
									dBianMaLoc_X = dFSLoc_X-245 ;
								}
								this->FillXdataBianMa(0, strJinLuName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapToReplace, mapStrToStrXdata) ;
								vec.x = dBianMaLoc_X ;
								vec.y = dBianMaLoc_Y ;
								xform.setTranslation(vec) ;								
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_bianma_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制bianma块

								//begin 绘制FSPLUS块
								double dFSPLUSLoc_X = dFBJLoc_X+dWidthFBJ ;
								double dFSPLUSLoc_Y = dGCJ_Y+dOffsetY_FSPLUS ;
								if (iRunDirect==1)
								{
									dFSPLUSLoc_X = dFBJLoc_X ;
								}
								this->FillXdataFSPLUS(mapStrToStrXdata) ;
								vec.x = dFSPLUSLoc_X ;
								vec.y = dFSPLUSLoc_Y ;
								xform.setTranslation(vec) ;
								//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc_3_%d.dwg"), m_strSysBasePath, iIsUpOrDown) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fsplus_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FSPLUS块

								//begin 绘制JMJ/FMJ励磁 块
								double dGJFLoc_X = dGCJZLoc1_X-5 ;
								double dGJFLoc_Y = dGCJZLoc1_Y+dHeight_ZLOC ;
								if (iRunDirect==1)
								{
									dGJFLoc_X = dGCJZLoc1_X+dWidth_ZLOC+5+(dCDA2_StartX!=0?dWidthGJFALL_0000+5:dWidthGJFALL_0001) ;
								}
								this->FillXdataLiCi_JFMJ(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;

								vec.x = dGJFLoc_X ;
								vec.y = dGJFLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lici_0%d%d0_0.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:iGuDaoFenGe, dCDA2_StartX!=0?0:1) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制JMJ/FMJ励磁 块

								//begin 绘制fusi-gjf块
								double dGJFGJLoc_X = dGJFLoc_X-(dCDA2_StartX!=0?dWidthGJFALL_0000:dWidthGJFALL_0001)-5 ;
								double dGJFGJLoc_Y = dGJFLoc_Y ;
								if (iRunDirect==1)
								{
									dGJFGJLoc_X = dGJFLoc_X+(dCDA2_StartX!=0?dWidthGJFGJ_0000:dWidthGJFGJ_0001)+5 ;
								}
								this->FillXdataFuSi_GJF(strStartEndXhj, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
								vec.x = dGJFGJLoc_X ;
								vec.y = dGJFGJLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fusi-gjf_00%d0_0.dwg"), m_strSysBasePath, dCDA2_StartX!=0?0:1) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制fusi-gjf块

								//begin 绘制fusi-xjf块
								double dFuSiXJFLoc_X = dGJFGJLoc_X-(dCDA2_StartX!=0?dWidthGJFGJ_0000:dWidthGJFGJ_0001)-5 ;
								double dFuSiXJFLoc_Y = dGJFLoc_Y ;
								if (iRunDirect==1)
								{
									dFuSiXJFLoc_X = dGJFGJLoc_X+dWidthZJC_0002+5 ;
								}
								this->FillXdataFuSi_XJF(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFuSiXJFLoc_X ;
								vec.y = dFuSiXJFLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fusi-xjf_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制fusi-xjf块

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*(iMaxGCJ+(iGuDaoFenGe==1?1:0)) + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dGCJZLoc1_X+dWidth_ZLOC+5+dGJFGCJTotalWidth ; //都从右往左依次绘制1GJF~10GJF
								if (iRunDirect==1)
								{
									dGJFGCJStartX = -5 ; 
								}
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标
								dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ	

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 股道无分割插入GCJGJF_0000、GCJGJF-JFMJ_0000块									
									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{
										//GCJGJF-GJF_0000										
										this->FillXdataGCJGJF_GJF(0, 0, 0, iMaxGCJ, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, iJdqIndexs[1], strZhNames[1], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGCJZLoc1_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gjf_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ(0, 0, iMaxGCJ, strGuiDao, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.y = dGCJZLoc1_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gcj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//正接ZJC-JMJ:GCJGJF-JFMJ_0000
										this->FillXdataGCJGJF_JFMJ_0000(strStartEndXhj, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX-dWidthGJFGCJ ;
										vecGJFGCJ.y = dGCJZLoc1_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-jfmj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX = dBaseX-dWidthGJFGCJ-dWidthGCJGJF_JFMJ ;
									}
									//end 股道无分割插入GCJGJF_0000、GCJGJF-JFMJ_0000块

									//begin 股道有分割时，插入GCJGJF-JFMJ_0000块（无GCJ1）
									if (iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe)
									{
										this->FillXdataGCJGJF_JFMJ_0000(strStartEndXhj, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x = dBaseX ;//dGJFGCJStartX-dWidthGJFGCJ*iGuDaoFenGe ;
										vecGJFGCJ.y = dGCJZLoc1_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-jfmj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGCJGJF_JFMJ ;
									}
									//end 股道有分割时，插入ZJC_0003块（无GCJ1）

									//begin 绘制GCJGJF

									if (!(iGuDaoFenGe==1&&nGuiDaoIndex==0) && iIsSendCode==1) //第一个是GCJ1，股道,不再1GCJ~10GCJ
									{									
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF(0, nGCJIndex, nGCJIndex, iMaxGCJ,strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, iJdqIndexs[1], strZhNames[1], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGCJZLoc1_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ(0, nGCJIndex, iMaxGCJ, strGuiDao, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.y = dGCJZLoc1_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										int iGcjTempleteSn = 2 ;
										if (nGCJIndex%5==1)
										{
											iGcjTempleteSn = 1 ;
										}
										else if (nGCJIndex%5==0)
										{
											iGcjTempleteSn = 5 ;
										}
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gcj_000%d_0.dwg"), m_strSysBasePath, iGcjTempleteSn) ;	
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;

										if (nGCJIndex==6) //记录1CDA-2图框起始位置
										{
											dCDA2_StartX = dBaseX+dWidthGJFGCJ;
										}
										if (nGCJIndex>=6)
										{
											dCDA2_EndX = dBaseX ;
										}
									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZBJH或者GDFS(半自动口)

								strSql.Format(_T("select * from dmh_import where instr(dmh_import_line, '半自动接近')>0 and dmh_import_xhj='%s' and dmh_import_table=1 "), strJZXHJ) ;
								CStringArray strArrayRec ;
								if(CDBUtility::GetRecordValue(strSql, _T("id"), strArrayRec)==0)
								{
									this->FillXdataZBJH_JGJ_1LQ(0, strJZXHJ, iMaxGCJ, mapStrToStrXdata, mapToAddXdata) ;
									vec.x = dBaseX ;
									vec.y = dGCJZLoc1_Y-8 ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zbjh-jgjlqj_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								else//非0说明以strJZ为进站信号机的该线为半自动接近
								{
									this->FillXdataGDFS_GJF1(strJZXHJ, iMaxGCJ, mapStrToStrXdata, mapToAddXdata) ;
									vec.x = dBaseX ;
									vec.y = dGCJZLoc1_Y-8 ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gdfs-gjf1_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

								}

								//绘制CD-2的框
								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, dGCJZLoc1_Y,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, dGCJZLoc1_Y-dHeight_GCJGJF_Frame,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, dGCJZLoc1_Y-dHeight_GCJGJF_Frame,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, dGCJZLoc1_Y,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;
									strZhShowName = _T("1CDA-2") ;
									if (iGuDaoFenGe>1)
									{
										strZhShowName = _T("1CDA") ;
									}
									this->FillXdataGCJZLoc(0, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

									vec.x = dCDA2_StartX-dWidth_ZLOC ;
									vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）	

								//end 绘制GJF-GCJ
								//*********************************************************************************************************************

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = 20 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = 19 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //出站信号机
									{
										strXhjName = strCZXHJ ;
									}
									else if (nGuiDaoIndex==iGuDaoFenGe) //反向出站信号机
									{
										strXhjName = strFCZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}
									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}					


									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制进站
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = 19 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strJZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}								

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,19,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = 25 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//end 绘制示意图								
								
								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, -262+5+287, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, -262+297, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;		
								strBlockName = _T("SIGN_3") ;
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s进站正向接车进路电码化电路图"), strJZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏

							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段						

					}
					else if (strJinLuType==_T("反发"))
					{
						//取得进出站信号机等信息,只在每条线的正接进路中执行此操作//20140626:如果当前进路不是ZJ,但是不和上条进路是一条线也须单独计算进出站信号机等，不能沿用正接时计算的进出站信号机
						if (iLineIndex!=iLineIndexPre)
						{
							strJZXHJ = strFCZXHJ = strCZXHJ = strFJZXHJ = _T("") ;
							this->GetLineInfo(strLine, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;
						}

						strSaveFileName.Format(_T("%s\\%s%s反向发车进路电码化电路图.dwg"),strSavePath, m_strStation, strStartEndXhj) ;
						acutPrintf(_T("\n%s"), strSaveFileName) ;


						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(1, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1
									this->FillXdataNGL(nGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_0%d%d%d_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_1%d00_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,0,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
									}

									xformNGL.setToTranslation(vecNGL);												
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									//绘制GCJ
									if (iIsSendCode==1) 
									{
										this->FillXdataGCJ(nGCJIndex, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 1) ;
										double dGCJ_X = dBaseX+dOffsetX_NGL_GCJ+(iRunDirect==0?0:-dWidth_NGL_Code) ;
										vecNGL.x = dGCJ_X ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_0%d%d%d_%d.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:1, nGCJIndex<10?0:1,nGCJIndex<10?nGCJIndex:0, iRunDirect) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_000%d_%d.dwg"), m_strSysBasePath, nGCJIndex%2==0?2:1, iRunDirect) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									// end 绘制GCJ

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数

								//begin 绘制2CD标签
								double dGCJZLoc1_X = dWidthGCJ_0000+dOffsetX_NGL_GCJ ;
								double dGCJZLoc1_Y = -170.000 ;
								if (iRunDirect==1)
								{
									dGCJZLoc1_X = 0 ;
								}
								CString strZLayerBlkName = (iGuDaoFenGe==1?_T("dmh_box_z_2cd1_1"):_T("dmh_box_z_2cd_1")); //没用上？还有_2的块
								CString strZhShowName = (iGuDaoFenGe==1?_T("2CD1"):_T("2CD")) ;

								this->FillXdataGCJZLoc(1, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制2CD标签	

								//begin 绘制ZPW.TFGZ
								double dTFGZLoc_X = dWidthGCJ_0000+dOffsetX_NGL_GCJ ; //往右行时TFGZ块的插入坐标
								double dTFGZLoc_Y = dGCJ_Y ;
								if (iRunDirect==1)
								{
									dTFGZLoc_X = -dWidth_TFGZ+dOffsetX_NGL_GCJ ;
								}
								this->FillXdataTFGZ(strJinLuName, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 1) ;
								vec.x = dTFGZLoc_X ;
								vec.y = dTFGZLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_tfgz_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZPW.TFGZ

								//begin 绘制反发 ZJC_FMJ
								double dZJC_FMJ_X = dTFGZLoc_X+dWidth_TFGZ ;
								double dZJC_FMJ_Y = dTFGZLoc_Y ;
								if (iRunDirect==1)
								{
									dZJC_FMJ_X = dTFGZLoc_X-dWidthZJC_FMJ  ; //dWidthZJC_FMJ=dWidthFJC_FMJ
								}

								this->FillXdataZFJC_FMJ(iLineIndex, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZJC_FMJ_X ;
								vec.y = dZJC_FMJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fmj_1000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FJC_FMJ(zjc_0001)	

								//begin 绘制说明note
								double dNoteLoc_X = dZJC_FMJ_X ;
								double dNoteLoc_Y = dZJC_FMJ_Y+60 ;
								vec.x = dNoteLoc_X ;
								vec.y = dNoteLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_note_0000_0.dwg"), m_strSysBasePath) ;
								mapStrToStrXdata.RemoveAll() ;
								mapToAddXdata.RemoveAll() ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制说明note

								//begin 绘制JMJ/FMJ励磁 块
								double dGJFLoc_X = dGCJZLoc1_X-5 ;
								double dGJFLoc_Y = dGCJZLoc1_Y+dHeight_ZLOC ;
								if (iRunDirect==1)
								{
									dGJFLoc_X = dGCJZLoc1_X+dWidth_ZLOC+5+(iMaxGCJ_ZJ>5?dWidthGJFALL_0000+5:dWidthGJFALL_0001) ;
								}
								this->FillXdataLiCi_JFMJ(1, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
								vec.x = dGJFLoc_X ;
								vec.y = dGJFLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lici_10%d0_0.dwg"), m_strSysBasePath, iMaxGCJ_ZJ>5?0:1) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制JMJ/FMJ励磁 块

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*iMaxGCJ + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dGCJZLoc1_X+dWidth_ZLOC+5+dGJFGCJTotalWidth ; //都从右往左依次绘制1GJF~10GJF
								if (iRunDirect==1)
								{
									dGJFGCJStartX = -5 ; 
								}
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标

								double dGJF_CD1_StartX = 0 ; //反发中，GJF的zloc取得是正接时的1CDA-1/1CDA-2的zloc
								double dGJF_CD1_EndX = 0 ;
								double dGJF_CD2_StartX = 0 ;
								double dGJF_CD2_EndX = 0 ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1

									if (iIsSendCode==0) //不发码的此处不处理
									{
										cSetGuiDao.MoveNext() ;
										continue;
									}

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(1, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									int iGJFIndex =  0 ;//iMaxGCJ-nGCJIndex+1 ;
									if (!strJdqNames[1].IsEmpty())
									{
										iGJFIndex = _tstoi(strJdqNames[1].Left(strJdqNames[1].GetLength()-4)) ; //从组合柜读取的GJF1的名称，例：4GJF1
									}			

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 绘制GCJGJF
									if (nGuiDaoIndex==0) //先插入一个ZJC-FMJ块
									{
										//begin 绘制ZJC_FMJ(gcjgjf-jfmj_1000)
										this->FillXdataGCJGJF_JFMJ_1000(iLineIndex,strJZXHJ, strFCZXHJ, strCZXHJ,strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
										vec.x = dBaseX ;
										vec.y = dGCJZLoc1_Y ;
										xform.setTranslation(vec) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-jfmj_1000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//end 绘制ZJC_FMJ(gcjgjf-jfmj_1000)	
										dBaseX-=dWidthGJFGCJ ;

										if (iGJFIndex>5)
										{
											dGJF_CD2_StartX = dBaseX;
										}
										else
										{
											dGJF_CD1_StartX = dBaseX;
										}
									}

									if (iIsSendCode==1) 
									{
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF(1, nGCJIndex, iGJFIndex, iMaxGCJ, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, iJdqIndexs[1], strZhNames[1], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGCJZLoc1_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ(1, nGCJIndex, iMaxGCJ, strGuiDao, iJdqIndexs[0], strZhNames[0], mapToReplace,  mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.y = dGCJZLoc1_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										int iGcjTempleteSn = 2 ;
										if (nGCJIndex%5==1)
										{
											iGcjTempleteSn = 1 ;
										}
										else if (nGCJIndex%5==0)
										{
											iGcjTempleteSn = 5 ;
										}
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gcj_000%d_0.dwg"), m_strSysBasePath, iGcjTempleteSn) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;

										if (iGJFIndex==5)
										{
											dGJF_CD2_EndX = dBaseX+dWidthGJFGCJ;
											dGJF_CD1_StartX = dBaseX+dWidthGJFGCJ;
										}
										if (iGJFIndex<5)
										{
											dGJF_CD1_EndX = dBaseX;
										}
									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZJC-GJF1(和股道的链接)
								this->FillXdataZJC_GJF1(1, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, iMaxGCJ,mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dBaseX ;
								vec.y = dGCJZLoc1_Y-8 ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc-gjf1_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

								//1GJF1~5GJF1所在的组合
								AcGePoint3dArray pt_3d_array_1cda1frame ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_StartX, dGCJZLoc1_Y,0)) ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_StartX, dGCJZLoc1_Y-28,0)) ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_EndX, dGCJZLoc1_Y-28,0)) ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_EndX, dGCJZLoc1_Y,0)) ;
								mysys.AddAcDb2dPolyline(pt_3d_array_1cda1frame, 0.5, 0) ;

								strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								strZhShowName = _T("1CDA") ;
								if (iMaxGCJ_ZJ>5)
								{
									strZhShowName = _T("1CDA-1") ;
								}
								if (iGuDaoFenGe>1)
								{
									strZLayerBlkName = _T("dmh_box_z_1cdb") ;
									strZhShowName = _T("1CDB") ;
								}
								this->FillXdataGCJZLoc(0, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

								vec.x = dGJF_CD1_StartX-dWidth_ZLOC ;
								vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

								//6GJF1~10GJF1所在的组合（若有）
								if (dGJF_CD2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_StartX, dGCJZLoc1_Y,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_StartX, dGCJZLoc1_Y-28,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_EndX, dGCJZLoc1_Y-28,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_EndX, dGCJZLoc1_Y,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;
									strZhShowName = _T("1CDA-2") ;
									if (iGuDaoFenGe>1)
									{
										strZhShowName = _T("1CDA") ;
									}
									this->FillXdataGCJZLoc(0, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

									vec.x = dGJF_CD2_StartX-dWidth_ZLOC ;
									vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 绘制GJF-GCJ
								//*********************************************************************************************************************

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = 20 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = 19 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //进站信号机
									{
										strXhjName = strJZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0001_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}

									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									if (nGuiDaoIndex==0&&iRunDirect==1) //进站信号机较长，故箭头要右移
									{
										pt_3d_insert_syt.x = dBaseX+13 ;
									}
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制FCZ
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = 19 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strFCZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,19,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = 25 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//end 绘制示意图

								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, -262+5+287, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, -262+297, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	


								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s出站反向发车进路电码化电路图"), strFCZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏

							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段
					}
					else if (strJinLuType==_T("正发"))
					{
						//取得进出站信号机等信息,只在每条线的正接进路中执行此操作//20140626:如果当前进路不是ZJ,但是不和上条进路是一条线也须单独计算进出站信号机等，不能沿用正接时计算的进出站信号机
						if (iLineIndex!=iLineIndexPre)
						{
							strJZXHJ = strFCZXHJ = strCZXHJ = strFJZXHJ = _T("") ;
							this->GetLineInfo(strLine, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;
						}
						
						strSaveFileName.Format(_T("%s\\%s%s正向发车进路电码化电路图.dwg"), strSavePath, m_strStation, strStartEndXhj) ;
						acutPrintf(_T("\n%s"), strSaveFileName) ;
						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;
						iMaxGCJ_ZF = iMaxGCJ ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标
								double dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								double dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1
									this->FillXdataNGL(nGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_0%d%d%d_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_1%d00_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,0,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
									}

									xformNGL.setToTranslation(vecNGL);												
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									//绘制GCJ
									if (iIsSendCode==1) 
									{
										this->FillXdataGCJ(nGCJIndex, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 2) ;
										double dGCJ_X = dBaseX+dOffsetX_NGL_GCJ+(iRunDirect==0?0:-dWidth_NGL_Code) ;
										vecNGL.x = dGCJ_X ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_0%d%d%d_%d.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:1, nGCJIndex<10?0:1,nGCJIndex<10?nGCJIndex:0, iRunDirect) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_000%d_%d.dwg"), m_strSysBasePath, nGCJIndex%2==0?2:1, iRunDirect) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										if (nGCJIndex==6) //记录1CDA-2图框起始位置
										{
											dCDA2_StartX = dBaseX+(iRunDirect==0?(65+17):-47) ;
										}
										if (nGCJIndex>=6)
										{
											dCDA2_EndX = dBaseX+(iRunDirect==0?17:17) ;
										}
									}
									// end 绘制GCJ

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数

								//begin 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）
								double dGCJZLoc1_X = dWidthGCJ_0000+dOffsetX_NGL_GCJ ;
								double dGCJZLoc1_Y = -170.000 ;
								if (iRunDirect==1)
								{
									dGCJZLoc1_X = 0 ;
								}
								CString strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								CString strZhShowName = _T("1CDA") ;
								if (iGuDaoFenGe==1) //股道无分割
								{
									if (dCDA2_StartX!=0) //说明GCJ个数大于5
									{
										strZhShowName = _T("1CDA-1") ;
									}
								}
								else //股道有分割时此处必是1CDB
								{
									strZLayerBlkName = _T("dmh_box_z_1cdb") ;
									strZhShowName = _T("1CDB") ;
								}
								this->FillXdataGCJZLoc(2, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）

								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -155,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -84,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -84,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -155,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;
									strZhShowName = _T("1CDA-2") ;
									if (iGuDaoFenGe>1)
									{
										strZhShowName = _T("1CDA") ;
									}
									this->FillXdataGCJZLoc(2, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

									double dGCJZLoc2_X = (iRunDirect==0?dCDA2_EndX:dCDA2_StartX) ;
									double dGCJZLoc2_Y =  -155 ;		
									vec.x = dGCJZLoc2_X ;
									vec.y = dGCJZLoc2_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）								

								//begin 绘制ZPW.TFGZ
								double dTFGZLoc_X = dWidthGCJ_0000+dOffsetX_NGL_GCJ ; //往右行时TFGZ块的插入坐标
								double dTFGZLoc_Y = dGCJ_Y ;
								if (iRunDirect==1)
								{
									dTFGZLoc_X = -dWidth_TFGZ+dOffsetX_NGL_GCJ ;
								}
								this->FillXdataTFGZ(strJinLuName, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 2) ;
								vec.x = dTFGZLoc_X ;
								vec.y = dTFGZLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_tfgz_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZPW.TFGZ

								//begin 绘制正发 FJC_FMJ
								double dFJC_FMJ_X = dTFGZLoc_X+dWidth_TFGZ ;
								double dFJC_FMJ_Y = dTFGZLoc_Y ;
								if (iRunDirect==1)
								{
									dFJC_FMJ_X = dTFGZLoc_X-dWidthZJC_FMJ  ; //dWidthZJC_FMJ=dWidthFJC_FMJ
								}
								this->FillXdataFJC_FMJ(iLineIndex, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFJC_FMJ_X ;
								vec.y = dFJC_FMJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fmj_2000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FJC_FMJ(zjc_0001)	

								//begin 绘制说明note
								double dNoteLoc_X = dFJC_FMJ_X ;
								double dNoteLoc_Y = dFJC_FMJ_Y+60 ;
								vec.x = dNoteLoc_X ;
								vec.y = dNoteLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_note_0000_0.dwg"), m_strSysBasePath) ;
								mapStrToStrXdata.RemoveAll() ;
								mapToAddXdata.RemoveAll() ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制说明note

								//begin 绘制FBJ块
								double dFBJLoc_X = dFJC_FMJ_X+dWidthZJC_FMJ ;
								double dFBJLoc_Y = dTFGZLoc_Y ;
								if (iRunDirect==1)
								{
									dFBJLoc_X = dFJC_FMJ_X-dWidthFBJ ;
								}
								this->FillXdataFBJ(2, strJinLuName,mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFBJLoc_X ;
								vec.y = dFBJLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fbj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FBJ块

								//begin 绘制FS块
								double dFSLoc_X = dFBJLoc_X+dWidthFBJ ;
								double dFSLoc_Y = dGCJ_Y-dOffsetY_FS ;
								if (iRunDirect==1)
								{
									dFSLoc_X = dFBJLoc_X ;
								}
								this->FillXdataFS(iUpOrDown, 2, strJinLuName,mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFSLoc_X ;
								vec.y = dFSLoc_Y ;
								xform.setTranslation(vec) ;
								//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc_3_%d.dwg"), m_strSysBasePath, iIsUpOrDown) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fs_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FS块

								//begin 绘制零散电路
								double dLingSanLoc_FBJ_X = dFSLoc_X+(iRunDirect==0?110:-180) ; //运行方向不同，块布置位置不同
								double dLingSanLoc_FBJ_Y = dFSLoc_Y-85 ;
								this->FillXdataLingSan_FBJ(2, strJinLuName, mapStrToStrXdata) ;
								vec.x = dLingSanLoc_FBJ_X ;
								vec.y = dLingSanLoc_FBJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lingsan_0001_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制零散电路

								//begin 绘制bianma块
								double dBianMaLoc_X = dFSLoc_X+16 ;
								double dBianMaLoc_Y = dFSLoc_Y+25 ;
								if (iRunDirect==1)
								{
									dBianMaLoc_X = dFSLoc_X-245 ;
								}
								this->FillXdataBianMa(2, strJinLuName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapToReplace, mapStrToStrXdata) ;
								vec.x = dBianMaLoc_X ;
								vec.y = dBianMaLoc_Y ;
								xform.setTranslation(vec) ;								
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_bianma_0001_%d.dwg"), m_strSysBasePath, iUpOrDown) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制bianma块

								//begin 绘制FSPLUS块
								double dFSPLUSLoc_X = dFBJLoc_X+dWidthFBJ ;
								double dFSPLUSLoc_Y = dGCJ_Y+dOffsetY_FSPLUS ;
								if (iRunDirect==1)
								{
									dFSPLUSLoc_X = dFBJLoc_X ;
								}
								this->FillXdataFSPLUS(mapStrToStrXdata) ;
								vec.x = dFSPLUSLoc_X ;
								vec.y = dFSPLUSLoc_Y ;
								xform.setTranslation(vec) ;
								//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc_3_%d.dwg"), m_strSysBasePath, iIsUpOrDown) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fsplus_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FSPLUS块

								//begin 绘制JMJ/FMJ励磁 块
								double dGJFLoc_X = dGCJZLoc1_X-5 ;
								double dGJFLoc_Y = dGCJZLoc1_Y+dHeight_ZLOC ;
								if (iRunDirect==1)
								{
									dGJFLoc_X = dGCJZLoc1_X+dWidth_ZLOC+5+(dCDA2_StartX!=0?dWidthGJFALL_0000+5:dWidthGJFALL_0001) ;
								}
								this->FillXdataLiCi_JFMJ(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
								vec.x = dGJFLoc_X ;
								vec.y = dGJFLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lici_20%d0_0.dwg"), m_strSysBasePath, dCDA2_StartX!=0?0:1) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制JMJ/FMJ励磁 块

								//begin 绘制fusi-gjf块
								double dGJFGJLoc_X = dGJFLoc_X-(dCDA2_StartX!=0?dWidthGJFALL_0000:dWidthGJFALL_0001)-5 ;
								double dGJFGJLoc_Y = dGJFLoc_Y ;
								if (iRunDirect==1)
								{
									dGJFGJLoc_X = dGJFLoc_X+(dCDA2_StartX!=0?dWidthGJFGJ_0000:dWidthGJFGJ_0001)+5 ;
								}
								this->FillXdataFuSi_GJF(strStartEndXhj, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
								vec.x = dGJFGJLoc_X ;
								vec.y = dGJFGJLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fusi-gjf_00%d0_0.dwg"), m_strSysBasePath, dCDA2_StartX!=0?0:1) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制fusi-gjf块

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*iMaxGCJ + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dGCJZLoc1_X+dWidth_ZLOC+5+dGJFGCJTotalWidth ; //都从右往左依次绘制1GJF~10GJF
								if (iRunDirect==1)
								{
									dGJFGCJStartX = -5 ; 
								}
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标
								dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 绘制GCJGJF
									if (nGuiDaoIndex==0) //先插入一个FJC-FMJ块
									{
										//begin 绘制FJC_FMJ(gcjgjf-jfmj_2000)
										this->FillXdataGCJGJF_JFMJ_2000(iLineIndex, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
										vec.x = dBaseX ;
										vec.y = dGCJZLoc1_Y ;
										xform.setTranslation(vec) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-jfmj_2000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//end 绘制FJC_FMJ(gcjgjf-jfmj_2000)	
										dBaseX-=dWidthGJFGCJ ;
									}

									if (iIsSendCode==1) 
									{
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF(2, nGCJIndex, nGCJIndex, iMaxGCJ,strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, iJdqIndexs[1], strZhNames[1], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGCJZLoc1_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ(2, nGCJIndex, iMaxGCJ, strGuiDao, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.y = dGCJZLoc1_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										int iGcjTempleteSn = 2 ;
										if (nGCJIndex%5==1)
										{
											iGcjTempleteSn = 1 ;
										}
										else if (nGCJIndex%5==0)
										{
											iGcjTempleteSn = 5 ;
										}
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gcj_000%d_0.dwg"), m_strSysBasePath, iGcjTempleteSn) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;

										if (nGCJIndex==6) //记录1CDA-2图框起始位置
										{
											dCDA2_StartX = dBaseX+dWidthGJFGCJ;
										}
										if (nGCJIndex>=6)
										{
											dCDA2_EndX = dBaseX ;
										}
									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZJC-GJF1(和股道的链接)
								this->FillXdataZJC_GJF1(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, iMaxGCJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dBaseX ;
								vec.y = dGCJZLoc1_Y-8 ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc-gjf1_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

								//绘制CD-2的框
								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, dGCJZLoc1_Y,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, dGCJZLoc1_Y-dHeight_GCJGJF_Frame,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, dGCJZLoc1_Y-dHeight_GCJGJF_Frame,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, dGCJZLoc1_Y,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;
									strZhShowName = _T("1CDA-2") ;
									if (iGuDaoFenGe>1)
									{
										strZhShowName = _T("1CDA") ;
									}
									this->FillXdataGCJZLoc(2, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

									vec.x = dCDA2_StartX-dWidth_ZLOC ;
									vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）	

								//end 绘制GJF-GCJ
								//*********************************************************************************************************************

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = 20 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = 19 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //反进站信号机
									{
										strXhjName = strFJZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0011_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}

									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									if (nGuiDaoIndex==0&&iRunDirect==0) //反进站信号机较长，故箭头要左移
									{
										pt_3d_insert_syt.x = dBaseX-13 ;
									}
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制CZ
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = 19 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strCZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,19,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = 25 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制示意图

								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, -262+5+287, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, -262+297, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s出站正向发车进路电码化电路图"), strCZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏

							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段						

					}
					else if (strJinLuType==_T("反接"))
					{
						//取得进出站信号机等信息,只在每条线的正接进路中执行此操作//20140626:如果当前进路不是ZJ,但是不和上条进路是一条线也须单独计算进出站信号机等，不能沿用正接时计算的进出站信号机
						if (iLineIndex!=iLineIndexPre)
						{
							strJZXHJ = strFCZXHJ = strCZXHJ = strFJZXHJ = _T("") ;
							this->GetLineInfo(strLine, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;
						}

						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;

						strSaveFileName.Format(_T("%s\\%s%s反向接车进路电码化电路图.dwg"), strSavePath, m_strStation, strStartEndXhj) ;
						acutPrintf(_T("\n%s"), strSaveFileName) ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标								
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(3, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ								
									this->FillXdataNGL(nGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_0%d%d%d_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_1%d00_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,0,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((iGuDaoFenGe==1&&nGuiDaoIndex==0)||(iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1))
										{
											dBaseX-=dWidthZJC_0000 ; 
										}
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((nGuiDaoIndex==0&&iGuDaoFenGe==1)||(iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1))
										{
											dBaseX+=dWidthZJC_0000 ; 
										}
									}
									//AcGeVector3d vec(nGuiDaoIndex*(iIsSendCode==0?56:65)*(iIsUpOrDown==0?-1:1),0, 0); 

									xformNGL.setToTranslation(vecNGL);												
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									//begin 股道无分割插入GCJ_0000（GCJ1）和ZJC_JMJ块
									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{
										//GCJ_0000
										this->FillXdataGCJ_0000(3, strGuiDao, strStartEndXhj, mapStrToStrXdata, mapToAddXdata) ;
										vecNGL.x =vecNGL.x+dOffsetX_NGL_GCJ ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_1000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//JMJ
										this->FillXdataJMJ_0000(3,iGuDaoFenGe, strStartEndXhj, mapStrToStrXdata, mapToAddXdata) ;
										vecNGL.x = vecNGL.x+(iRunDirect==0?-dWidthZJC_0000:dWidthGCJ_0000) ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_jmj_3000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 股道无分割插入ZJC块（GCJ1）

									//begin 股道有分割时，插入ZJC_JMJ块（无GCJ1）
									if (iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1)
									{
										this->FillXdataJMJ_0000(3,iGuDaoFenGe, strStartEndXhj, mapStrToStrXdata, mapToAddXdata) ;										
										vecNGL.x = vecNGL.x+(iRunDirect==0?(dOffsetX_NGL_GCJ-dWidthZJC_0000):dOffsetX_NGL_GCJ+dWidthGCJ_0000) ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_jmj_3000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata,mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

									}
									//end 股道有分割时，插入ZJC块（无GCJ1）

									//begin 绘制GCJ

									if (!(iGuDaoFenGe==1&&nGuiDaoIndex==0) && iIsSendCode==1) //第一个是GCJ1，股道,不再1GCJ~10GCJ
									{
										this->FillXdataGCJ(nGCJIndex, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 3) ;
										double dGCJ_X = dBaseX+dOffsetX_NGL_GCJ+(iRunDirect==0?0:-dWidth_NGL_Code) ;
										if (iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1)
										{
											dGCJ_X = dGCJ_X+dWidthZJC_0000*(iRunDirect==0?1:-1) ; //股道分割时，股道和非股道区段之间空出一定距离，放置ZJC
										}
										vecNGL.x = dGCJ_X ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_0%d%d%d_%d.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:1, nGCJIndex<10?0:1,nGCJIndex<10?nGCJIndex:0, iRunDirect) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_000%d_%d.dwg"), m_strSysBasePath, nGCJIndex%2==0?2:1, iRunDirect) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

									}
									// end 绘制GCJ

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数

								//begin 绘制2CD标签
								double dGCJZLoc1_X = 0 ;
								double dGCJZLoc1_Y = -170.000 ;
								CString strZLayerBlkName = (iGuDaoFenGe==1?_T("dmh_box_z_2cd1_1"):_T("dmh_box_z_2cd_1")); //没用上？还有_2的块
								CString strZhShowName = (iGuDaoFenGe==1?_T("2CD1"):_T("2CD")) ;

								this->FillXdataGCJZLoc(3, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制2CD标签

								//begin 绘制ZPW.TFGZ
								double dTFGZLoc_X = dWidthGCJ_0000+dOffsetX_NGL_GCJ ; //下行时TFGZ块的插入坐标
								double dTFGZLoc_Y = dGCJ_Y ;
								if (iRunDirect==1)
								{
									dTFGZLoc_X = -dWidth_TFGZ+dOffsetX_NGL_GCJ ;
								}
								this->FillXdataTFGZ(strJinLuName, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, 0) ;
								vec.x = dTFGZLoc_X ;
								vec.y = dTFGZLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_tfgz_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZPW.TFGZ

								//begin 绘制反接 FJC_FMJ
								double dZJC_FMJ_X = dTFGZLoc_X+dWidth_TFGZ ;
								double dZJC_FMJ_Y = dTFGZLoc_Y ;
								if (iRunDirect==1)
								{
									dZJC_FMJ_X = dTFGZLoc_X-dWidthZJC_FMJ  ;
								}
								this->FillXdataFJC_FMJ(iLineIndex, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZJC_FMJ_X ;
								vec.y = dZJC_FMJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fmj_3000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FJC_FMJ

								//begin 绘制说明note
								double dNoteLoc_X = dZJC_FMJ_X ;
								double dNoteLoc_Y = dZJC_FMJ_Y+60 ;
								vec.x = dNoteLoc_X ;
								vec.y = dNoteLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_note_0000_0.dwg"), m_strSysBasePath) ;
								mapStrToStrXdata.RemoveAll() ;
								mapToAddXdata.RemoveAll() ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制说明note

								//begin 绘制JMJ/FMJ励磁 块
								double dGJFLoc_X = dGCJZLoc1_X-5 ;
								double dGJFLoc_Y = dGCJZLoc1_Y+dHeight_ZLOC ;
								if (iRunDirect==1)
								{
									dGJFLoc_X = dGCJZLoc1_X+dWidth_ZLOC+5+(iMaxGCJ>5?dWidthGJFALL_0000+5:dWidthGJFALL_0001) ;
								}
								this->FillXdataLiCi_JFMJ(3, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
								vec.x = dGJFLoc_X ;
								vec.y = dGJFLoc_Y ;
								xform.setTranslation(vec) ;
								//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zjc_3_%d.dwg"), m_strSysBasePath, iIsUpOrDown) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lici_3%d%d0_0.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:iGuDaoFenGe, iMaxGCJ_ZF>5?0:1) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制JMJ/FMJ励磁 块

								//begin 绘制fusi-xjf块
								double dFuSiXJFLoc_X = dGJFLoc_X-(iMaxGCJ>5?dWidthGJFALL_0000:dWidthGJFALL_0001)-5 ;
								double dFuSiXJFLoc_Y = dGJFLoc_Y ;
								if (iRunDirect==1)
								{
									dFuSiXJFLoc_X = dGJFLoc_X+(iMaxGCJ>5?dWidthGJFGJ_0000:dWidthGJFGJ_0001)+5 ;
								}
								this->FillXdataFuSi_XJF(3, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFuSiXJFLoc_X ;
								vec.y = dFuSiXJFLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fusi-xjf_3000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;								
								//end 绘制fusi-xjf块

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*(iMaxGCJ+(iGuDaoFenGe==1?1:0)) + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dGCJZLoc1_X+dWidth_ZLOC+5+dGJFGCJTotalWidth ; //都从右往左依次绘制1GJF~10GJF
								if (iRunDirect==1)
								{
									dGJFGCJStartX = -5 ; 
								}
								cSetGuiDao.MoveFirst() ;

								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标
								double dGJF_CD1_StartX = 0 ; //反发中，GJF的zloc取得是正接时的1CDA-1/1CDA-2的zloc
								double dGJF_CD1_EndX = 0 ;
								double dGJF_CD2_StartX = 0 ;
								double dGJF_CD2_EndX = 0 ;
								double dGJF_CD3_StartX = 0 ;//股道有分割时，分割股道GJF1的组合位置是相应的正接ZLOC
								double dGJF_CD3_EndX = 0 ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ	

									if (iIsSendCode==0) //不发码的此处不处理
									{
										cSetGuiDao.MoveNext() ;
										continue;
									}

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(3, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									int iGJFIndex =  0 ;//iMaxGCJ-nGCJIndex+1 ;
									if (!strJdqNames[1].IsEmpty())
									{
										iGJFIndex = _tstoi(strJdqNames[1].Left(strJdqNames[1].GetLength()-4)) ; //从组合柜读取的GJF1的名称，例：4GJF1
									}

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 股道无分割插入GCJGJF_0000、gcjgjf-jfmj_3000块									
									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF(3, 0, 0, iMaxGCJ, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, iJdqIndexs[1], strZhNames[1], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGCJZLoc1_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gjf_3000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ(3, 0, iMaxGCJ, strGuiDao, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;
										vecGJFGCJ.y = dGCJZLoc1_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;					
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gcj_3000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//gcjgjf-jfmj_3000
										this->FillXdataGCJGJF_JFMJ_3000(iMaxGCJ, strStartEndXhj, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX-dWidthGJFGCJ ;
										vecGJFGCJ.y = dGCJZLoc1_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-jfmj_3000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX = dBaseX-dWidthGJFGCJ-dWidthGCJGJF_JFMJ ;

										if (iGJFIndex>5)
										{
											dGJF_CD2_StartX = dBaseX;
										}
										else
										{
											dGJF_CD1_StartX = dBaseX;
										}
									}
									//end 股道无分割插入GCJGJF_0000、ZJC_0003块

									//begin 股道有分割时，插入gcjgjf-jfmj_3000块（无GCJ1）
									if (iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe)
									{
										this->FillXdataGCJGJF_JFMJ_3000(iMaxGCJ, strStartEndXhj, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x = dBaseX ;//dGJFGCJStartX-dWidthGJFGCJ*iGuDaoFenGe ;
										vecGJFGCJ.y = dGCJZLoc1_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-jfmj_3000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dGJF_CD3_StartX = dGJFGCJStartX ;
										dGJF_CD3_EndX = dBaseX ;

										dBaseX-=dWidthGCJGJF_JFMJ ;

										if (iGJFIndex>5)
										{
											dGJF_CD2_StartX = dBaseX;
										}
										else
										{
											dGJF_CD1_StartX = dBaseX;
										}
									}
									//end 股道有分割时，插入ZJC_0003块（无GCJ1）

									//begin 绘制GCJGJF							
									if (!(iGuDaoFenGe==1&&nGuiDaoIndex==0) && iIsSendCode==1) //第一个是GCJ1，股道,不再1GCJ~10GCJ
									{	
// 										int iGJFIndex =  0 ;//iMaxGCJ-nGCJIndex+1 ;
// 										iGJFIndex = _tstoi(strJdqNames[1].Left(strJdqNames[1].GetLength()-4)) ; //从组合柜读取的GJF1的名称，例：4GJF1
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF(3, nGCJIndex, iGJFIndex, iMaxGCJ, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, iJdqIndexs[1], strZhNames[1], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGCJZLoc1_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ(3, nGCJIndex, iMaxGCJ, strGuiDao, iJdqIndexs[0], strZhNames[0], mapToReplace, mapStrToStrXdata, mapToAddXdata, iGuDaoFenGe, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;
										vecGJFGCJ.y = dGCJZLoc1_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										int iGcjTempleteSn = 2 ;
										if (nGCJIndex%5==1)
										{
											iGcjTempleteSn = 1 ;
										}
										else if (nGCJIndex%5==0)
										{
											iGcjTempleteSn = 5 ;
										}
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcjgjf-gcj_000%d_0.dwg"), m_strSysBasePath, iGcjTempleteSn) ;	
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;

										if (iGJFIndex==5)
										{
											dGJF_CD2_EndX = dBaseX+dWidthGJFGCJ;
											dGJF_CD1_StartX = dBaseX+dWidthGJFGCJ;
										}
										if (iGJFIndex<5)
										{
											dGJF_CD1_EndX = dBaseX;
										}
									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZBJH或者GDFS(半自动口)
								this->FillXdataZBJH_JGJ_1LQ(3, strJZXHJ,iMaxGCJ,mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dBaseX ;
								vec.y = dGCJZLoc1_Y-8 ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zbjh-jgjlqj_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;


								//1GJF1~5GJF1所在的组合
								AcGePoint3dArray pt_3d_array_1cda1frame ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_StartX, dGCJZLoc1_Y,0)) ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_StartX, dGCJZLoc1_Y-28,0)) ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_EndX, dGCJZLoc1_Y-28,0)) ;
								pt_3d_array_1cda1frame.append(AcGePoint3d(dGJF_CD1_EndX, dGCJZLoc1_Y,0)) ;
								mysys.AddAcDb2dPolyline(pt_3d_array_1cda1frame, 0.5, 0) ;

								strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								strZhShowName = _T("1CDA") ;
								if (dGJF_CD2_StartX!=0)
								{
									strZhShowName = _T("1CDA-1") ;
								}
								if (iGuDaoFenGe>1)
								{
									strZLayerBlkName = _T("dmh_box_z_1cdb") ;
									strZhShowName = _T("1CDB") ;
								}
								this->FillXdataGCJZLoc(2, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

								vec.x = dGJF_CD1_StartX-dWidth_ZLOC ;
								vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

								//6GJF1~10GJF1所在的组合（若有）
								if (dGJF_CD2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_StartX, dGCJZLoc1_Y,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_StartX, dGCJZLoc1_Y-28,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_EndX, dGCJZLoc1_Y-28,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dGJF_CD2_EndX, dGCJZLoc1_Y,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;
									strZhShowName = _T("1CDA-2") ;
									if (iGuDaoFenGe>1)
									{
										strZhShowName = _T("1CDA") ;
									}
									this->FillXdataGCJZLoc(2, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;

									vec.x = dGJF_CD2_StartX-dWidth_ZLOC ;
									vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}

								//股道分割GJF1所在的组合（若有）
								if (dGJF_CD3_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_3frame ;
									pt_3d_array_3frame.append(AcGePoint3d(dGJF_CD3_StartX, dGCJZLoc1_Y,0)) ;
									pt_3d_array_3frame.append(AcGePoint3d(dGJF_CD3_StartX, dGCJZLoc1_Y-28,0)) ;
									pt_3d_array_3frame.append(AcGePoint3d(dGJF_CD3_EndX, dGCJZLoc1_Y-28,0)) ;
									pt_3d_array_3frame.append(AcGePoint3d(dGJF_CD3_EndX, dGCJZLoc1_Y,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_3frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cdb") ;
									strZhShowName = _T("1CDB") ;

									this->FillXdataGCJZLoc(0, strZLayerBlkName, strZhShowName, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata, 1) ;

									vec.x = dGJF_CD3_StartX-dWidth_ZLOC ;
									vec.y = dGCJZLoc1_Y-dHeight_ZLOC;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}

								//end 绘制GJF-GCJ
								//*********************************************************************************************************************

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = 20 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = 19 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //反出站信号机
									{
										strXhjName = strFCZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									}
									else if (nGuiDaoIndex==iGuDaoFenGe) //出站信号机
									{
										strXhjName = strCZXHJ ;	
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}
									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制反进站
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = 19 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strFJZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0011_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,19,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = 25 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//end 绘制示意图

								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, -262+5+287, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, -262+297, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s进站反向接车进路电码化电路图"), strFJZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏

							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段					

					}

					//删除没用的端子
					this->DelNoUserEnt(pSaveDb) ;

					//将sn转换为ID
					this->TransMySnToId(pSaveDb) ;

					//删除过程sn等xdata
					//this->DelSn(pSaveDb) ;

					mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
					mysys.SetLayerStatus(_T("hide"), true) ;
					mysys.ModifyFontStyle(_T("Standard"), _T("hztxt.shx"), TRUE) ;
					//mysys.ModifyFontStyle(_T("HZ"), _T("Times New Roman"), FALSE) ;

					if (!strSaveFileName.IsEmpty())
					{
						pSaveDb->saveAs(strSaveFileName) ;
					}
					delete pSaveDb ;
					pSaveDb = NULL ;

					cSet.MoveNext() ;

					iDwgIndex++ ;
					iLineIndexPre = iLineIndex ;
				}
			}
			//acedRestoreStatusBar() ;

			cSet.Close() ;
		}		

		//处理第二个table
		
		m_ctrlProgressAll->SetPos(2) ;
		//::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)1, 0) ;		
		double dOffsetY_CeXian = -60.000 ;//侧线各块Y坐标下沉60
		dGCJ_Y+=dOffsetY_CeXian ; 
		strSql.Format(_T("select distinct dmh_import_line,dmh_import_lineindex from dmh_import where dmh_import_table=1 order by dmh_import_lineindex")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int nRsCount = cSet.GetRecordCount() ;
			if (nRsCount>0)
			{
				m_ctrlProgressCur->SetRange(1, nRsCount) ;

				cSet.MoveFirst() ;

				int iDwgIndex = 0 ;
				while (!cSet.IsEOF())
				{
					m_ctrlProgressCur->SetPos(iDwgIndex+1) ;

					//一条记录就是一张图纸
					CString strSaveFileName ; //存储电码化图纸的文件名					
					AcDbDatabase *pSaveDb = new AcDbDatabase ; //存储数据库
					mysys.m_pDb = pSaveDb;	
					//记录扩展数据值
					CMapStringToString mapToReplace, mapStrToStrXdata, mapToAddXdata ;
					CString strCircuitTemplete ; //当前选择的底图模板
					AcGeMatrix3d xform;  
					AcGeVector3d vec(0,0,0) ;

					CString strLine ; //线路名称，例：同一股道1、 调车接近轨
					int iLineIndex = 0 ;//线别序号
					cSet.GetFieldValue(_T("dmh_import_line"), strLine) ;
					cSet.GetFieldValue(_T("dmh_import_lineindex"), iLineIndex) ; //数据库Access初定义dmh_import_lineindex为整形，此处读不出，改成长整形ok
					//cSet.GetFieldValue(_T("dmh_import_lineindex"), iLineIndex) ;

					int iJinLuType = 4 ; //进路类型，4：侧线股道；5：调车接近轨；6：半自动接近轨
					if (strLine.Find(_T("调车接近"))!=-1)
					{
						iJinLuType = 5 ;
					}
					else if (strLine.Find(_T("半自动接近"))!=-1)
					{
						iJinLuType = 6 ;
					}

					strMsg.Format(_T("侧线电码化-%s(%d/%d)"), strLine, iDwgIndex+1, nRsCount) ;
					::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)(&strMsg), 0) ;

					CString strGuDao = _T("") ;
					//type	    jfm	 freq  xhj	cellcol
					//上行频率	FC		   S5	 E
					//下行频率	FC		   X5	 F
					CADORecordset cSetJinLu ;	
					strSql.Format(_T("select distinct dmh_import_type,dmh_import_jfm,dmh_import_freq, dmh_import_xhj,dmh_import_cellcol, dmh_import_jinluname from dmh_import where dmh_import_table=1 and  dmh_import_lineindex=%d order by dmh_import_cellcol"), iLineIndex) ;
					if (cSetJinLu.IsOpen())
					{
						cSetJinLu.Close() ;
					}
					if (cSetJinLu.Open(g_PtrCon, strSql))
					{
						int iJinLuNum = cSetJinLu.GetRecordCount() ;

						cSetJinLu.MoveFirst() ;

						while (!cSetJinLu.IsEOF())
						{
							CString strType, strJfm, strFreq, strXhj, strGuiDao, strJinLuName ;
							cSetJinLu.GetFieldValue(_T("dmh_import_type"), strType) ; //例：
							cSetJinLu.GetFieldValue(_T("dmh_import_jfm"), strJfm) ;
							cSetJinLu.GetFieldValue(_T("dmh_import_freq"), strFreq) ;
							cSetJinLu.GetFieldValue(_T("dmh_import_xhj"), strXhj) ;
							cSetJinLu.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;					

							if (strGuDao.IsEmpty())
							{
								strSql.Format(_T("select * from dmh_import where dmh_import_xhj='%s' and dmh_import_lineindex=%d"), strXhj, iLineIndex) ;
								CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strGuiDao) ;
								strGuDao = strGuiDao ;
								strGuDao.TrimRight(_T('+')) ;
							}

							int iUpOrDown = 0 ; //0：下行，1：上行
							int iRunDirect = 0 ; //运行方向，决定图纸布局，0，从左往右，1，从右往左
							if (strType.Find(_T("下行"))!=-1)
							{
								iUpOrDown = 0 ;
								iRunDirect = 1 ;								
							}
							else
							{
								iUpOrDown = 1 ;
								iRunDirect = 0 ;
							}

							//begin 绘制FS块
							double dFSLoc_X = 5+dWidthFS ;
							double dFSLoc_Y = dGCJ_Y-dOffsetY_FS ;
							if (iRunDirect==0)
							{
								dFSLoc_X = -5-dWidthFS ;
							}						
							this->FillXdataFS(iUpOrDown, iJinLuType, strJinLuName, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
							vec.x = dFSLoc_X ;
							vec.y = dFSLoc_Y ;
							xform.setTranslation(vec) ;								
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fs_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
							this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制FS块

							//begin 绘制零散电路
							double dLingSanLoc_FBJ__X = dFSLoc_X+(iRunDirect==0?110:-180) ; //运行方向不同，块布置位置不同
							double dLingSanLoc_FBJ_Y = dFSLoc_Y-65 ;
							this->FillXdataLingSan_FBJ(4, strJinLuName, mapStrToStrXdata) ;
							vec.x = dLingSanLoc_FBJ__X ;
							vec.y = dLingSanLoc_FBJ_Y ;
							xform.setTranslation(vec) ;
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lingsan_0001_0.dwg"), m_strSysBasePath) ;
							this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制零散电路

							//begin 绘制bianma块
							double dBianMaLoc_X = dFSLoc_X-245 ;
							double dBianMaLoc_Y = dFSLoc_Y+25 ;
							if (iRunDirect==0)
							{
								dBianMaLoc_X = dFSLoc_X+16 ;
							}
							if (iJinLuType==6)
							{
								this->FillXdataBianMa(iJinLuType, strJinLuName, strXhj, _T(""), strXhj, _T(""), mapToReplace, mapStrToStrXdata) ;
							}
							else
							{
								this->FillXdataBianMa(iJinLuType, strJinLuName, _T(""), _T(""), strXhj, _T(""), mapToReplace, mapStrToStrXdata) ;
							}

							vec.x = dBianMaLoc_X ;
							vec.y = dBianMaLoc_Y ;
							xform.setTranslation(vec) ;								
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_bianma_000%d_%d.dwg"), m_strSysBasePath, iJinLuType-2, iJinLuType==6?0:iUpOrDown) ;
							this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制bianma块

							//begin 绘制FBJ块
							double dFBJLoc_X = dFSLoc_X ;
							double dFBJLoc_Y = dGCJ_Y ;
							if (iRunDirect==0)
							{
								dFBJLoc_X = dFSLoc_X-dWidthFBJ ;
							}
							this->FillXdataFBJ(4, strJinLuName, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
							vec.x = dFBJLoc_X ;
							vec.y = dFBJLoc_Y ;
							xform.setTranslation(vec) ;						
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fbj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
							this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制FBJ块

							//begin 绘制FSPLUS块
							double dFSPLUSLoc_X = dFBJLoc_X ;
							double dFSPLUSLoc_Y = dGCJ_Y+dOffsetY_FSPLUS ;
							if (iRunDirect==0)
							{
								dFSPLUSLoc_X = dFBJLoc_X+dWidthFBJ ;
							}
							this->FillXdataFSPLUS(mapStrToStrXdata) ;
							vec.x = dFSPLUSLoc_X ;
							vec.y = dFSPLUSLoc_Y ;
							xform.setTranslation(vec) ;						
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_fsplus_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
							this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制FSPLUS块

							//begin 插入零散电路（XHFS、GDFS组合）
							double dLingSan_Loc_XHFS_X = (iRunDirect==0?dFSPLUSLoc_X-85:dFBJLoc_X) ;
							double dLingSan_Loc_XHFS_Y = dFSPLUSLoc_Y+56 ;
							vec.x = dLingSan_Loc_XHFS_X ;
							vec.y = dLingSan_Loc_XHFS_Y ;
							xform.setTranslation(vec) ;	
							if (iJinLuType==4) //侧线股道
							{
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_lingsan_000%d_0.dwg"), m_strSysBasePath, iRunDirect==1?3:2) ;

								if (iUpOrDown==1) //上行侧放置LINGSAN_XHFS
								{
									CString strCZ_X, strCZ_S ;
									strCZ_S = strXhj ;
									strCZ_X.Format(_T("X%s"), strCZ_S.Right(strCZ_S.GetLength()-1)) ;
									this->FillXdataLingSan_XHFS(strCZ_X, strCZ_S, mapToReplace, mapStrToStrXdata) ;
								}
								else if (iUpOrDown==0) //下行侧放置LINGSAN_GDFS
								{
									this->FillXdataLingSan_GDFS(strGuDao, mapToReplace, mapStrToStrXdata) ;
								}
							}
							this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 插入零散电路（XHFS、GDFS组合）

							//begin 绘制ZPW.TFGZ
							double dTFGZLoc_X = dFBJLoc_X+dWidthFBJ ; 
							double dTFGZLoc_Y = dGCJ_Y ;
							if (iRunDirect==0)
							{
								dTFGZLoc_X = dFBJLoc_X-dWidth_TFGZ ;
							}
							this->FillXdataTFGZ(strJinLuName, mapStrToStrXdata, mapToAddXdata, 1, 4, iRunDirect) ;
							vec.x = dTFGZLoc_X ;
							vec.y = dTFGZLoc_Y ;
							xform.setTranslation(vec) ;
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_tfgz_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
							this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制ZPW.TFGZ

							//begin 绘制说明note
							double dNoteLoc_X = -55 ;
							double dNoteLoc_Y = -24 ;
							vec.x = dNoteLoc_X ;
							vec.y = dNoteLoc_Y ;
							xform.setTranslation(vec) ;
							strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_note_0000_0.dwg"), m_strSysBasePath) ;
							mapStrToStrXdata.RemoveAll() ;
							mapToAddXdata.RemoveAll() ;
							this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
							//end 绘制说明note

							//begin 绘制NGL、GJF1等
							CADORecordset cSetGuiDao ;
							strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_jinluname='%s' order by dmh_import_cellrow"), strJinLuName) ;
							if (cSetGuiDao.IsOpen())
							{
								cSetGuiDao.Close() ;
							}
							if (cSetGuiDao.Open(g_PtrCon, strSql))
							{
								int iGuiDaoCount = cSetGuiDao.GetRecordCount() ;
								if (iGuiDaoCount>0)
								{
									cSetGuiDao.MoveFirst() ;
									int iGuiDaoIndex = 0 ;
									while (!cSetGuiDao.IsEOF())
									{
										CString strGuiDao ;									
										int nCellRow = 0 ;
										cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
										cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

										int iJdqIndex = 0 ; //GJF1继电器在GDFS组合中的序号（1~8）

										CString strIsROrF = _T("F") ;
										if(strGuiDao.Right(1)==_T("+"))
										{
											strIsROrF = _T("R") ;
										}
										//输入excel表中轨道所处的行，范围6~15是发码端，16~20是不发码端
										int iIsSendCode = nCellRow>15?0:1 ; //是否发码端
										//int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ								

										//begin 绘制NGL
										double dNGLLoc_X = dTFGZLoc_X+dWidth_TFGZ+iGuiDaoIndex*dWidth_NGL_Code-dOffsetX_NGL_GCJ ; 
										double dNGLLoc_Y = dOffsetY_CeXian ;
										if (iRunDirect==0)
										{
											dNGLLoc_X = dTFGZLoc_X-(iGuiDaoIndex+1)*dWidth_NGL_Code-dOffsetX_NGL_GCJ ;
										}
										int iGCJIndex = iGuiDaoIndex+1+(iRunDirect==0?200:100) ;
										this->FillXdataNGL(iGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;
										vec.x = dNGLLoc_X ;
										vec.y = dNGLLoc_Y ;
										xform.setTranslation(vec) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_0%d%d%d_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0, iIsSendCode) ;
										if (m_iTwoOrFourLine==4)
										{
											strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_ngl_1%d00_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1) ;
										}									
										if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
										{
											this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										}
										//end 绘制NGL

										if (iIsSendCode==1)
										{
											//begin 绘制GJF1
											double dGJF1Loc_X = dNGLLoc_X+dOffsetX_NGL_GCJ ; 
											double dGJF1Loc_Y = dGCJ_Y ;

											this->FillXdataGJF1_CeXian(strJinLuName, strGuiDao, iGCJIndex, mapStrToStrXdata, mapToAddXdata) ;
											vec.x = dGJF1Loc_X ;
											vec.y = dGJF1Loc_Y ;
											xform.setTranslation(vec) ;
											strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gjf1_0001_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
											this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
											//end 绘制GJF1
										}

										iGuiDaoIndex++ ;
										cSetGuiDao.MoveNext() ;
									}

								}
								cSetGuiDao.Close() ;
							}
							//end 绘制NGL、GJF1等

							cSetJinLu.MoveNext() ;
						}

						//begin 绘制整个电路图的边框和签字栏
						AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
						mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

						//内边框4个点
						AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -262+5, 0) ;
						AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
						AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
						AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, -262+5+287, 0) ;
						//外边框4个点
						AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -262, 0) ;
						AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
						AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
						AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, -262+297, 0) ;

						AcGePoint3d pt_3d_insert(0,0,0) ;

						CString strBlockName ;
						//左边frame
						strBlockName = _T("frame_left") ;
						pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
						pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
						AcDbBlockReference *pRef = NULL ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
						{
							pRef->close() ;
							pRef = NULL ;
						}

						AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
						AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
						CLongArray iArrayHandle ;
						double dframe_sign_3_hor_width = 180.000 ;
						double dframe_sign_3_hor_height = 34.000 ;								

						//判断右下角是否为空，为空则绘制一个横向的frame于此
						pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
						pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
						pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
						pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

						iArrayHandle.RemoveAll() ;								
						int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
						if (iFindEntSum==0)
						{									
							strBlockName = _T("SIGN_3") ;
						}
						else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
						{
							pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
							pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
							pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
							pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
							iArrayHandle.RemoveAll() ;
							iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
							//其他情况只能往右延展绘制一个竖向的frame
							if (iFindEntSum!=0)
							{
								pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
								pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
								pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
								pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
							}

							strBlockName = _T("frame_sign_3_ver_new") ;
						}	

						//内边框
						AcGePoint3dArray ptArrayFrame ;
						ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
						ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
						ptArrayFrame.append(pt_3d_frame_in_topright) ;
						ptArrayFrame.append(pt_3d_frame_in_topleft) ;
						mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
						//外边框
						ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
						ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
						ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
						ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
						mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

						//签字栏
						CStringArray strArrayVal ;
						pt_3d_insert = pt_3d_frame_in_bottomright ;

						strArrayVal.RemoveAll() ;
						strArrayVal.Add(m_strStation) ;
						strArrayVal.Add(m_strDwgSn) ;		
						strArrayVal.Add(CBlkUtility::GetDateStr()) ;
						CString strDwgName ;
						strDwgName.Format(_T("股道电码化电路图(%s)"), strGuDao) ;
						strArrayVal.Add(strDwgName) ;
						strArrayVal.Add(m_strProjectName) ;
						if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}
						//end 绘制整个电路图的边框和签字栏

						cSetJinLu.Close() ;
					}

					strSaveFileName.Format(_T("%s\\%s侧线股道电码化电路图(%s).dwg"), strSavePath, m_strStation, strGuDao) ;

					//删除没用的端子
					this->DelNoUserEnt(pSaveDb) ;

					//将sn转换为ID
					this->TransMySnToId(pSaveDb) ;

					//删除过程sn等xdata
					//this->DelSn(pSaveDb) ;

					mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
					mysys.SetLayerStatus(_T("hide"), true) ;
					mysys.ModifyFontStyle(_T("Standard"), _T("hztxt.shx"), TRUE) ;

					pSaveDb->saveAs(strSaveFileName) ;

					delete pSaveDb ;
					pSaveDb = NULL ;

					cSet.MoveNext() ;
					iDwgIndex++ ;
				}
			}			

			cSet.Close() ;
		}		

		//处理n+1
		m_ctrlProgressAll->SetPos(3) ;
		//::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)1, 0) ;		
		//double dOffsetY_CeXian = -60.000 ;//侧线各块Y坐标下沉60
		//dGCJ_Y+=dOffsetY_CeXian ; 
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid in (select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_fbj')order by dmh_in_zcell_shelfnum asc, dmh_in_zcell_layernum desc, dmh_in_zcell_boxnum asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int nRsCount = cSet.GetRecordCount() ;
			if (nRsCount>0)
			{
				m_ctrlProgressCur->SetRange(1, nRsCount) ;

				CString strSaveFileName ; //存储n+1图纸的文件名	
				strSaveFileName.Format(_T("%s\\%sn+1电路图.dwg"), strSavePath, m_strStation) ;

				AcDbDatabase *pSaveDb = new AcDbDatabase ; //存储数据库
				mysys.m_pDb = pSaveDb;	
				//记录扩展数据值
				CMapStringToString mapToReplace, mapStrToStrXdata, mapToAddXdata ;
				CString strCircuitTemplete ; //当前选择的底图模板
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0,0,0) ;

				int iFBJIndex = 0 ;
				double dBaseX = 0 ; //插入点
				double dBaseY = -257.000 ;
				double dBlockWidth = 0 ; //插入块的宽度

				AcDbBlockReference *pRef = NULL ;
				CStringArray strArrayVal ;

				CString strBlockName = _T("") ;
				AcGePoint3d pt_3d_insert(0,0,0) ;

				while (!cSet.IsEOF())
				{
					m_ctrlProgressCur->SetPos(iFBJIndex+1) ;

					dBlockWidth = 100.000 ;

					CString strJinLuName ;
					cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strJinLuName) ;
					int iFindMarkLoc = strJinLuName.Find(_T("/")) ; //有无共用fs的情况
					CString strJinLuNameTmp  = strJinLuName ;
					if (iFindMarkLoc!=-1)
					{
						strJinLuNameTmp = strJinLuName.Left(iFindMarkLoc) ;
					}

					//查询该进路相关信息
					CString strJZ, strFCZ, strCZ, strFJZ ; //取得进出站信号机以便填充扩展数据
					CString strLine, strJinLuType ; 
					strSql.Format(_T("select * from dmh_import where dmh_import_jinluname='%s'"), strJinLuNameTmp) ;
					CADORecordset cSetFindOneJinLu ;
					if (cSetFindOneJinLu.IsOpen())
					{
						cSetFindOneJinLu.Close() ;
					}
					if (cSetFindOneJinLu.Open(g_PtrCon, strSql))
					{
						int iFindOneJinLuCount = cSetFindOneJinLu.GetRecordCount() ;
						if (iFindOneJinLuCount>0)
						{
							cSetFindOneJinLu.MoveFirst() ;

							cSetFindOneJinLu.GetFieldValue(_T("dmh_import_line"), strLine) ;
							cSetFindOneJinLu.GetFieldValue(_T("dmh_import_type"), strJinLuType) ;
							int iTableIndex = 0 ;
							cSetFindOneJinLu.GetFieldValue(_T("dmh_import_table"), iTableIndex) ;
							if (iTableIndex==0)
							{
								this->GetLineInfo(strLine, strJZ, strFCZ, strCZ, strFJZ) ; //正线
							}
							else
							{
								CString strXhj ;
								cSetFindOneJinLu.GetFieldValue(_T("dmh_import_xhj"), strXhj) ; //侧线、调车等
								if (strLine.Find(_T("半自动接近"))!=-1)
								{
									strJZ = strXhj ;
								}
								else	
								{
									strCZ = strXhj ;
								}							
							}
						}

						cSetFindOneJinLu.Close() ;
					}

					strBlockName = _T("") ;
					int iUpOrDown = 0 ; //0:下行，1：上行
					if (strLine.Find(_T("上行"))!=-1)
					{
						iUpOrDown = 1 ;
					}
					else if (strJinLuType.Find(_T("上行"))!=-1)
					{
						iUpOrDown = 1 ;
					}

					int iJinLuType = 0 ; //进路类型，-1:FS；4：侧线股道；5：调车接近轨；6：半自动接近轨

					if (strJinLuNameTmp.Find(_T("+1"))!=-1)
					{
						iJinLuType = -1 ;
						strBlockName.Format(_T("dmh_n+1_fs_000%d_0"), m_bUseHLP==TRUE?1:0) ;
						dBlockWidth = 140.000 ;
					}
					else if (strJinLuType==_T("正接")||strJinLuType==_T("反发"))
					{
						iJinLuType = 0 ;
						strBlockName.Format(_T("dmh_n+1_zj-ff_0000_%d"), iUpOrDown) ;
						dBlockWidth = 120.000 ;
					}
					else if (strJinLuType==_T("正发")||strJinLuType==_T("反接"))
					{
						iJinLuType = 2 ;
						strBlockName.Format(_T("dmh_n+1_zf-fj_0000_%d"), iUpOrDown) ;
						dBlockWidth = 120.000 ;
					}
					else if (strLine.Find(_T("同一股道"))!=-1)
					{
						iJinLuType = 4 ;
						strBlockName.Format(_T("dmh_n+1_gd_0000_%d"), iUpOrDown) ;
						dBlockWidth = 50.000 ;
					}
					else if (strLine.Find(_T("调车接近"))!=-1)
					{
						iJinLuType = 5 ;
						strBlockName.Format(_T("dmh_n+1_dj_0000_%d"), iUpOrDown) ;
						dBlockWidth = 50.000 ;
					}
					else if (strLine.Find(_T("半自动接近"))!=-1)
					{
						iJinLuType = 6 ;
						strBlockName.Format(_T("dmh_n+1_jg_0000_%d"), iUpOrDown) ;
						dBlockWidth = 80.000 ;
					}

					if (!strBlockName.IsEmpty())
					{
						this->FillXdataNAdd1(iJinLuType, strJinLuName, strJZ, strFCZ, strCZ, strFJZ, mapStrToStrXdata, mapToAddXdata, mapToReplace, iUpOrDown) ;

						vec.x = dBaseX ;
						vec.y = dBaseY ;
						xform.setTranslation(vec) ;
						strCircuitTemplete.Format(_T("%s\\support\\dmh\\%s.dwg"), m_strSysBasePath, strBlockName) ;
						this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform,mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
					}

					dBaseX+=dBlockWidth ;//即使strBlockName为空，也要讲dBaseX加dBlockWidth（默认100）,以便查漏
					iFBJIndex++ ;
					acutPrintf(_T("\n%d*********************"), iFBJIndex) ;

					//删除没用的端子
					this->DelNoUserEnt(pSaveDb) ;

					cSet.MoveNext() ;
				}

				//绘制“接本图”标记、说明文字（note）等

				pt_3d_insert.x = dBaseX ;
				pt_3d_insert.y = -145.4281 ;			
				strBlockName = _T("dmh_n+1_con_0") ;
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				pt_3d_insert.x = dBaseX ;
				pt_3d_insert.y = -52.6004 ;			
				strBlockName = _T("dmh_n+1_con_1") ;
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				pt_3d_insert.x = dBaseX+5 ;
				pt_3d_insert.y = -70 ;			
				strBlockName = _T("dmh_circuit_note_0001_0") ;
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				//begin 绘制整个电路图的边框和签字栏
				AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
				mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

				//内边框4个点
				AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x, -262+5, 0) ;
				AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
				AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
				AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x, -262+5+287, 0) ;
				//外边框4个点
				AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25, -262, 0) ;
				AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
				AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
				AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25, -262+297, 0) ;

				//左边frame
				strBlockName = _T("frame_left") ;
				pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
				pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
				AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
				CLongArray iArrayHandle ;
				double dframe_sign_3_hor_width = 180.000 ;
				double dframe_sign_3_hor_height = 34.000 ;								

				//判断右下角是否为空，为空则绘制一个横向的frame于此
				pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
				pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
				pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
				pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

				iArrayHandle.RemoveAll() ;								
				int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
				if (iFindEntSum==0)
				{									
					strBlockName = _T("SIGN_3") ;
				}
				else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
				{
					pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
					pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
					pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
					pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
					iArrayHandle.RemoveAll() ;
					iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
					//其他情况只能往右延展绘制一个竖向的frame
					if (iFindEntSum!=0)
					{
						pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
						pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
						pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
						pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
					}

					strBlockName = _T("frame_sign_3_ver_new") ;
				}	

				//内边框
				AcGePoint3dArray ptArrayFrame ;
				ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
				ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
				ptArrayFrame.append(pt_3d_frame_in_topright) ;
				ptArrayFrame.append(pt_3d_frame_in_topleft) ;
				mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
				//外边框
				ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
				ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
				ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
				ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
				mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

				//签字栏
				pt_3d_insert = pt_3d_frame_in_bottomright ;

				strArrayVal.RemoveAll() ;
				strArrayVal.Add(m_strStation) ;
				strArrayVal.Add(m_strDwgSn) ;		
				strArrayVal.Add(CBlkUtility::GetDateStr()) ;
				CString strDwgName ;
				strDwgName.Format(_T("N+1电码化电路图")) ;
				strArrayVal.Add(strDwgName) ;
				strArrayVal.Add(m_strProjectName) ;
				if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					pRef->close() ;
					pRef = NULL ;
				}
				//end 绘制整个电路图的边框和签字栏	

				//将sn转换为ID
				//this->TransNAdd1SnToId(pSaveDb) ;

				//删除过程sn等xdata
				//this->DelSn(pSaveDb) ;

				mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
				mysys.SetLayerStatus(_T("hide"), true) ;
				mysys.ModifyFontStyle(_T("Standard"), _T("hztxt.shx"), TRUE) ;

				pSaveDb->saveAs(strSaveFileName) ;

				delete pSaveDb ;
				pSaveDb = NULL ;

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
		AfxMessageBox(_T("unknown error in DrawCircuit")) ;
	} 
}

