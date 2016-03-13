// BlkExcelTool.cpp: implementation of the CBlkExcelTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BlkExcelTool.h"
#include "../blkdll/BlkUtitily.h"
#include "UserSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlkExcelTool::CBlkExcelTool()
{

}

CBlkExcelTool::~CBlkExcelTool()
{

}

int CBlkExcelTool::GetExcelRange(CString strXlsFileName, CString strMark, CArray<double, double> &arrayWidth, CArray<CExcelRange*, CExcelRange*> &arrayexcelrange, int iSearchStartRow, int iSearchEndRow, TCHAR chSearchStartCol[], TCHAR chSearchEndCol[])
{
	int iFound = 0 ;
	//CUserSystem mysys ;
	
	arrayexcelrange.RemoveAll() ;
	
	int iSearchStartCol = CBlkUtility::GetNumFromExcelColName(chSearchStartCol) ;	
	int iSearchEndCol = CBlkUtility::GetNumFromExcelColName(chSearchEndCol) ;
	
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	WorksheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	
	try
	{
		
		if (bFirst) 
		{
			::AfxOleInit ( ) ;
			bFirst = FALSE ;
		}		
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks () ;
		
		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		
		pWorksheets  = m_pExcel->GetSheets( ) ;
		//获得sheet1的指针
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		
		for(int iRow=iSearchStartRow; iRow<=iSearchEndRow; iRow++)
		{
			for(int iCol=iSearchStartCol; iCol<=iSearchEndCol; iCol++)
			{
				CString strCellName ;
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
				strCellName.Format(_T("%s%d"),strColName, iRow) ;
				
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				_variant_t range_var = pRange->GetText() ;
				strCellText = (CString)range_var.bstrVal ;
				strCellText.TrimLeft() ;
				strCellText.TrimRight() ;
				if (strCellText==strMark) 
				{
					_variant_t bIsMergeCells = pRange->MergeCells ;	
					if (bIsMergeCells.boolVal==-1)
					{
						RangePtr pRangeTmp ;
						pRangeTmp = pRange->GetMergeArea() ;
						double dWidth = (double)(pRangeTmp->Width)*40*2.41/255 ;
						arrayWidth.Add(dWidth) ;
						
						long iMergeCellColNum = pRangeTmp->GetColumns()->GetCount() ;
						int iMergeCellEndCol = iCol + iMergeCellColNum - 1 ;
						
						//CString strMergeCellEndCol = CBlkUtility::GetExcelFieldName(iMergeCellEndCol) ;
						
						BOOL bBlockEnd = FALSE ;
						int iRow_Block = iRow ;
						while (!bBlockEnd)
						{
							iRow_Block++ ;
							CString strCellName_Block ;
							CString strCellText_Block ;							
							strCellName_Block.Format(_T("%s%d"), strColName, iRow_Block) ;
							
							pRange = NULL ;
							pRange = pSheet->GetRange((_variant_t)strCellName_Block, vtMissing) ;
							_variant_t range_var_block = pRange->GetText() ;
							strCellText_Block = (CString)range_var_block.bstrVal ;
							strCellText_Block.TrimLeft() ;
							strCellText_Block.TrimRight() ;
							
							if (strCellText_Block=="")
							{
								bBlockEnd = TRUE ;
							}
						}
						//char * chStartColName = NULL ;
						//char * chEndColName = NULL ;
						//chStartColName = strColName.GetBuffer(strColName.GetLength()) ;
						//chEndColName = strMergeCellEndCol.GetBuffer(strMergeCellEndCol.GetLength()) ;						
						
						CExcelRange * excelrange = new CExcelRange(iRow, iRow_Block-1, iCol, iMergeCellEndCol) ;
						
						//CString strMsg1 ;
						//strMsg1.Format(_T("%d-%d-%d-%d\n"), excelrange->iStartRow, excelrange->iEndRow, excelrange->iStartCol, excelrange->iEndCol) ;
						//AfxMessageBox(strMsg1) ;						
						
						arrayexcelrange.Add(excelrange) ;						
						
						iFound++ ;
						
					}
				}				
			}			
		}					
								
		///关闭打开的各个资源
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		//m_pExcel->Release() ;
		m_pExcel = NULL ;
	}
	catch(_com_error &e)
	{
		iFound = 0 ;
		//CString sBuff = CBlkUtility::GetErrorDescription(e);
		//AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		iFound = 0 ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	
	
	
	return iFound ;
}

BOOL CBlkExcelTool::DwgToExcel()
{
	CArray<AcDbText *, AcDbText *> arrayText_ ;
	CArray<AcDbLine *, AcDbLine *> arrayLine_ ;
	CArray<double, double> arrayX_ ;
	CArray<double, double> arrayY_ ;
	
	//arrayText.RemoveAll() ;
	//arrayLine.RemoveAll() ;
	//arrayX.RemoveAll() ;
	//arrayY.RemoveAll() ;

	CUserSystem mysys ;
	
	//////////////////////////////////////////////////////////////////////////	
	ads_name sset ;
	int err = ads_ssget(NULL, NULL, NULL, NULL, sset) ;
	if (err!=RTNORM)
	{
		ads_printf(_T("\nError selection set")) ;
		return FALSE;
	}
	long lLength = -1 ;
	ads_sslength(sset, & lLength) ;
	ads_name en ;
	AcDbObjectId eId ;
	AcDbEntity * pEnt = NULL ;
	for(int iEntIndex=0; iEntIndex<lLength; iEntIndex++)
	{
		ads_ssname(sset, iEntIndex, en) ;
		
		Acad::ErrorStatus es = acdbGetObjectId(eId, en) ;
		if (es==Acad::eOk)
		{
			if (Acad::eOk == acdbOpenAcDbEntity(pEnt, eId, AcDb::kForRead))
			{
				const TCHAR * pEntTypeName ;
				pEntTypeName = pEnt->isA()->name() ;
				
				//////////////////////////////////////////////////////////////////////////
				
				if (_tcscmp(pEntTypeName, _T("AcDbText")) == 0)
				{		
					AcDbText * pText = NULL ;
					pText = (AcDbText *)pEnt ;
					AcGePoint3d textp_3d(0,0,0) ;
					
					AcDbText * pTextTmp = new AcDbText ;
					pTextTmp->setTextString(pText->textString()) ;
					textp_3d.x = pText->position().x  ;
					textp_3d.y = pText->position().y  ;
					pTextTmp->setPosition(textp_3d) ;
					arrayText_.Add(pTextTmp) ;
					
					//pText->close() ;
				}
				else if (_tcscmp(pEntTypeName, _T("AcDbMText")) == 0) 
				{
					AcDbMText * pMtext = NULL ;
					pMtext = (AcDbMText *)pEnt ;
					AcGePoint3d textp_3d(0,0,0) ;			
					
					AcDbText * pTextTmp = new AcDbText ;
					pTextTmp->setTextString(pMtext->contents()) ;
					textp_3d.x = pMtext->location().x  ;
					textp_3d.y = pMtext->location().y  ;
					pTextTmp->setPosition(textp_3d) ;
					arrayText_.Add(pTextTmp) ;
					
					//pMtext->close() ;
					
				}
				else if (_tcscmp(pEntTypeName, _T("AcDbLine")) == 0)
				{
					AcDbLine * pLine = NULL ;
					pLine = (AcDbLine *)pEnt ;
					AcGePoint3d startP_3d(0,0,0) ;
					AcGePoint3d endP_3d(0,0,0) ;
					
					startP_3d.x = pLine->startPoint().x  ;
					startP_3d.y = pLine->startPoint().y  ;
					endP_3d.x = pLine->endPoint().x  ;
					endP_3d.y = pLine->endPoint().y  ;
					
					//和x轴y轴平行
					if (startP_3d.x==endP_3d.x || startP_3d.y==endP_3d.y)
					{
						AcDbLine * pLineTmp = new AcDbLine ;
						pLineTmp->setStartPoint(startP_3d) ;
						pLineTmp->setEndPoint(endP_3d) ;
						
						//ads_printf("(%d,%d)-(%d,%d)", startP_3d.x, startP_3d.y, endP_3d.x, endP_3d.y) ;
						
						arrayLine_.Add(pLineTmp) ;		
					}
					else
					{
						//直线斜率
						double dTanArc = (endP_3d.y-startP_3d.y)/(endP_3d.x-startP_3d.x) ;
						//和x轴夹角小于1度的
						if (fabs(dTanArc)<0.017455) 
						{
							AcDbLine * pLineTmp = new AcDbLine ;
							
							double dMid_y = (endP_3d.y+startP_3d.y)/2 ;
							startP_3d.y = dMid_y ;
							endP_3d.y = dMid_y ;	
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;
							
							//ads_printf("(%d,%d)-(%d,%d)", startP_3d.x, startP_3d.y, endP_3d.x, endP_3d.y) ;					
							
							arrayLine_.Add(pLineTmp) ;				
						}
						//和y轴夹角小于1度的
						else if (fabs(dTanArc)>57.289962) 
						{
							AcDbLine * pLineTmp = new AcDbLine ;
							
							double dMid_x = (endP_3d.x+startP_3d.x)/2 ;
							startP_3d.x = dMid_x ;
							endP_3d.x = dMid_x ;
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;
							
							//ads_printf("(%d,%d)-(%d,%d)", startP_3d.x, startP_3d.y, endP_3d.x, endP_3d.y) ;					
							
							arrayLine_.Add(pLineTmp) ;				
						}
					}
					
					//pLine->close() ;			
				}				
				else if (_tcscmp(pEntTypeName, _T("AcDbPolyline")) == 0)
				{
					AcDbPolyline *pPolyline = NULL ;
					pPolyline = (AcDbPolyline *) pEnt ;
					
					
					for(int iPolyLineVert=0; iPolyLineVert<(pPolyline->numVerts()-1); iPolyLineVert++)
					{		
						AcGePoint3d startP_3d(0,0,0) ;
						AcGePoint3d endP_3d(0,0,0) ;
						pPolyline->getPointAt(iPolyLineVert, startP_3d) ;
						pPolyline->getPointAt(iPolyLineVert+1, endP_3d) ;
						startP_3d.x = startP_3d.x ;
						startP_3d.y = startP_3d.y  ;
						endP_3d.x = endP_3d.x  ;
						endP_3d.y = endP_3d.y  ;	
						
						//和x轴y轴平行
						if (startP_3d.x==endP_3d.x || startP_3d.y==endP_3d.y)
						{
							AcDbLine * pLineTmp = new AcDbLine ;
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;
							
							arrayLine_.Add(pLineTmp) ;		
						}
						else
						{					
							//直线斜率
							double dTanArc = (endP_3d.y-startP_3d.y)/(endP_3d.x-startP_3d.x) ;
							
							//和x轴夹角小于1度的
							if (fabs(dTanArc)<0.017455) 
							{
								AcDbLine * pLineTmp = new AcDbLine ;
								
								double dMid_y = (endP_3d.y+startP_3d.y)/2 ;
								startP_3d.y = dMid_y ;
								endP_3d.y = dMid_y ;	
								pLineTmp->setStartPoint(startP_3d) ;
								pLineTmp->setEndPoint(endP_3d) ;
								
								arrayLine_.Add(pLineTmp) ;				
							}
							//和y轴夹角小于1度的
							else if (fabs(dTanArc)>57.289962) 
							{
								AcDbLine * pLineTmp = new AcDbLine ;
								
								double dMid_x = (endP_3d.x+startP_3d.x)/2 ;
								startP_3d.x = dMid_x ;
								endP_3d.x = dMid_x ;
								pLineTmp->setStartPoint(startP_3d) ;
								pLineTmp->setEndPoint(endP_3d) ;
								
								arrayLine_.Add(pLineTmp) ;				
							}
						}
						
					}
					if (Adesk::kTrue==pPolyline->isClosed())
					{
						AcGePoint3d startP_3d(0,0,0) ;
						AcGePoint3d endP_3d(0,0,0) ;
						pPolyline->getPointAt(0, startP_3d) ;
						pPolyline->getPointAt(pPolyline->numVerts()-1, endP_3d) ;
						startP_3d.x = startP_3d.x  ;
						startP_3d.y = startP_3d.y  ;
						endP_3d.x = endP_3d.x  ;
						endP_3d.y = endP_3d.y  ;	
						
						//和x轴y轴平行
						if (startP_3d.x==endP_3d.x || startP_3d.y==endP_3d.y)
						{
							AcDbLine * pLineTmp = new AcDbLine ;
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;
							
							arrayLine_.Add(pLineTmp) ;		
						}
						else
						{					
							//直线斜率
							double dTanArc = (endP_3d.y-startP_3d.y)/(endP_3d.x-startP_3d.x) ;
							
							//和x轴夹角小于1度的
							if (fabs(dTanArc)<0.017455) 
							{
								AcDbLine * pLineTmp = new AcDbLine ;
								
								double dMid_y = (endP_3d.y+startP_3d.y)/2 ;
								startP_3d.y = dMid_y ;
								endP_3d.y = dMid_y ;	
								pLineTmp->setStartPoint(startP_3d) ;
								pLineTmp->setEndPoint(endP_3d) ;
								
								arrayLine_.Add(pLineTmp) ;				
							}
							//和y轴夹角小于1度的
							else if (fabs(dTanArc)>57.289962) 
							{
								AcDbLine * pLineTmp = new AcDbLine ;
								
								double dMid_x = (endP_3d.x+startP_3d.x)/2 ;
								startP_3d.x = dMid_x ;
								endP_3d.x = dMid_x ;
								pLineTmp->setStartPoint(startP_3d) ;
								pLineTmp->setEndPoint(endP_3d) ;
								
								arrayLine_.Add(pLineTmp) ;				
							}
						}						
						
					}
					//pPolyline->close() ;			
				}
				else if (_tcscmp(pEntTypeName, _T("AcDbBlockReference")) == 0)
				{
					AcDbBlockReference * pBlkRef = NULL ;
					pBlkRef = (AcDbBlockReference *)pEnt ;
					
					//得到块的属性
					AcDbObjectIterator * pObjIter = NULL ;
					pObjIter = pBlkRef->attributeIterator() ;
					for(pObjIter->start(); !pObjIter->done(); pObjIter->step()) 
					{
						AcDbObjectId attId ;
						attId = pObjIter->objectId() ;
						AcDbAttribute * pAttrib = NULL ;
						pBlkRef->openAttribute(pAttrib, attId, AcDb::kForRead, Adesk::kFalse) ;
						
						AcDbText * pTextAtt = new AcDbText ;
						
						pTextAtt->setTextString(pAttrib->textString()) ;
						pTextAtt->setPosition(pAttrib->position()) ;
						arrayText_.Add(pTextAtt) ;
						
						pAttrib->close() ;	
					}
					delete pObjIter ;
					pObjIter = NULL ;
					
					
					AcGePoint3d insertP(0,0,0) ;
					insertP = pBlkRef->position() ;
					mysys.GetAllEntIdFromBlock(pBlkRef, insertP, arrayLine_, arrayText_) ;
					
					//pBlkRef->close() ;
				}
				//////////////////////////////////////////////////////////////////////////	
				pEnt->close() ;	
				pEnt = NULL ;
				
			}
		}
	}
	ads_ssfree(sset) ;
	//////////////////////////////////////////////////////////////////////////
	
	//ads_printf("process to getxy...\n") ;

	mysys.GetXY(arrayLine_, arrayX_, arrayY_) ;
	mysys.SortText(arrayText_) ;
	
	int iarrayX_Len = arrayX_.GetSize() ;
	int iarrayY_Len = arrayY_.GetSize() ;
	int iarrayTextLen = arrayText_.GetSize() ;
	int iarrayLineLen = arrayLine_.GetSize() ;
	
	//ads_printf("before into for loop x:%d,y:%d\n", iarrayX_Len, iarrayY_Len) ;
	
	//////////////////////////////////////////////////////////////////////////
	
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	//WorksheetsPtr pWorksheets = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	
	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return FALSE ;
		}
		//使excel程序显示可见
		m_pExcel->PutVisible(0,VARIANT_TRUE) ;
		//得到所有的工作簿
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		//添加一个新的工作簿，返回结果为刚添加的工作簿的指针
		pWorkBook = pWorkBooks->Add() ;
		
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;		
		
		//得到当前工作簿的所有的sheet
		pWorksheets = pWorkBook->GetSheets() ;
		//定位于第一个sheet
		pSheet = pWorksheets->GetItem(vt_index) ;
		
		//////////////////////////////////////////////////////////////////////////
		//begin double for loop
		
		for(int iRow=0; iRow<(iarrayY_Len-1); iRow++)
		{
			//ads_printf("第:%d行\n", iRow) ;
			for(int iCol=0; iCol<(iarrayX_Len-1); iCol++)
			{
				//ads_printf("--第:%d列\n", iCol) ;
				
				int iMergeX = 0 ;
				int iMergeY = 0 ;
				int iRowTmp = iRow ;
				int iColTmp = iCol ;
				BOOL bColFindOk = FALSE ;
				BOOL bRowFindOk = FALSE ;
				
				double dMid_x1 = (arrayX_.GetAt(iCol+1)+arrayX_.GetAt(iCol))/2 ;
				double dMid_y1 = arrayY_.GetAt(iRow) ;
				double dMid_x2 = arrayX_.GetAt(iCol) ;
				double dMid_y2 = (arrayY_.GetAt(iRow+1)+arrayY_.GetAt(iRow))/2 ;
				
				AcGePoint3d pointMid_3d1(0, 0, 0) ;
				AcGePoint3d pointMid_3d2(0, 0, 0) ;
				
				pointMid_3d1.x = dMid_x1 ;
				pointMid_3d1.y = dMid_y1 ;
				pointMid_3d2.x = dMid_x2 ;
				pointMid_3d2.y = dMid_y2 ;
				
				
				BOOL bIsPointMid1InLine = mysys.IsPointInAnyLine(pointMid_3d1, arrayLine_) ;
				
				BOOL bIsPointMid2InLine = mysys.IsPointInAnyLine(pointMid_3d2, arrayLine_) ;
				
				if (!( bIsPointMid1InLine && bIsPointMid2InLine )) 
				{
					continue ;
				}
				
				//顺x轴方向搜寻rang的下一个角
				while (!bColFindOk && iColTmp<(iarrayX_Len-1)) 
				{
					double dColTmp = arrayX_.GetAt(iColTmp+1) ;
					double dRowTmp = (arrayY_.GetAt(iRow+1)+arrayY_.GetAt(iRow))/2 ;
					AcGePoint3d point_3d(dColTmp, dRowTmp, 0) ;
					
					if (mysys.IsPointInAnyLine(point_3d,arrayLine_))
					{
						bColFindOk = TRUE ;
					}
					else
					{
						iColTmp++ ;
					}	
					iMergeX++ ;
					
				}		
				
				//顺x轴方向搜寻rang的下一个角
				iRowTmp = iRow ;
				iColTmp = iCol ;
				while (!bRowFindOk && iRowTmp<(iarrayY_Len-1)) 
				{
					double dRowTmp = arrayY_.GetAt(iRowTmp+1) ;
					double dColTmp = (arrayX_.GetAt(iCol+1)+arrayX_.GetAt(iCol))/2 ;
					AcGePoint3d point_3d(dColTmp, dRowTmp, 0) ;
					
					if (mysys.IsPointInAnyLine(point_3d,arrayLine_))
					{
						bRowFindOk = TRUE ;
					}
					else
					{
						iRowTmp++ ;
					}
					iMergeY++ ;
				}
				
				//
				if (iMergeX==1 && iMergeY==1 && bColFindOk && bRowFindOk)
				{
					//处理该单元格
					
					CString strCellContent(_T("")) ;
					double dStartX, dEndX, dStartY, dEndY ;
					dStartX = arrayX_.GetAt(iCol) ;
					dEndX = arrayX_.GetAt(iCol+1) ;
					dStartY = arrayY_.GetAt(iRow) ;
					dEndY = arrayY_.GetAt(iRow+1) ;
					
					for(int iTxtIndex=0; iTxtIndex<iarrayTextLen; iTxtIndex++)
					{
						AcDbText * pTextTmp = NULL ;
						pTextTmp = arrayText_.GetAt(iTxtIndex) ;
						AcGePoint3d pointtext_3d(0,0,0) ;
						pointtext_3d = pTextTmp->position() ;
						double dTxtX = pointtext_3d.x ;
						double dTxtY = pointtext_3d.y ;
						
						if (dTxtX>dStartX && dTxtX<dEndX && dTxtY>dEndY && dTxtY<dStartY) 
						{
							CString strTxtTmp ;
							strTxtTmp = pTextTmp->textString() ;
							//strCellContent+=strTxtTmp ;
							strCellContent.Format(_T("%s%s\n"), strCellContent, strTxtTmp) ;							
						}
						
						pTextTmp->close() ;
					}
					//strCellContent.Left(strCellContent.GetLength()-1) ;
					
					CString strCellName ;
					strCellName.Format(_T("%s%d"), CBlkUtility::GetExcelFieldName(iCol+1), iRow+1) ;
					
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					
					strCellContent.TrimRight(_T("\n")) ;

					pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
					
					pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
					
					MSExcel::FontPtr pFont = pRange->GetFont();
					pFont->PutSize((_variant_t)((long)10)) ;
					//pFont->PutBold(COleVariant((short)TRUE));//Apply Bold to Headers
					
					
					pRange->PutHorizontalAlignment((_variant_t)((long)MSExcel::xlHAlignCenter)) ;
					pRange->PutVerticalAlignment((_variant_t)((long)MSExcel::xlVAlignCenter)) ;
					pRange->PutWrapText((_variant_t)((bool)TRUE)) ;
					
					pRange->BorderAround(COleVariant((short)1), MSExcel::xlThin , MSExcel::xlColorIndexAutomatic ,vtMissing) ;  
					
					//ads_printf("----单元格:%d-%d\n", iRow, iCol ) ;
					
				}
				else if (bRowFindOk && bColFindOk)
				{
					//处理合并格
					
					CString strCellContent(_T("")) ;
					double dStartX, dEndX, dStartY, dEndY ;
					dStartX = arrayX_.GetAt(iCol) ;
					dEndX = arrayX_.GetAt(iCol+iMergeX) ;
					dStartY = arrayY_.GetAt(iRow) ;
					dEndY = arrayY_.GetAt(iRow+iMergeY) ;
					
					for(int iTxtIndex=0; iTxtIndex<iarrayTextLen; iTxtIndex++)
					{
						AcDbText * pTextTmp = NULL ;
						pTextTmp = arrayText_.GetAt(iTxtIndex) ;
						AcGePoint3d pointtext_3d(0,0,0) ;
						pointtext_3d = pTextTmp->position() ;
						double dTxtX = pointtext_3d.x ;
						double dTxtY = pointtext_3d.y ;
						
						if (dTxtX>dStartX && dTxtX<dEndX && dTxtY>dEndY && dTxtY<dStartY) 
						{
							CString strTxtTmp ;
							strTxtTmp = pTextTmp->textString() ;							
							//strCellContent+=strTxtTmp ;
							strCellContent.Format(_T("%s%s\n"), strCellContent, strTxtTmp) ;							
						}						
						pTextTmp->close() ;
					}			
					
					//strCellContent.Left(strCellContent.GetLength()-1) ;
					
					CString strCelName1, strCelName2;
					//CString strTmp("合并格") ;
					strCelName1.Format(_T("%s%d"), CBlkUtility::GetExcelFieldName(iCol+1), iRow+1)  ;
					strCelName2.Format(_T("%s%d"), CBlkUtility::GetExcelFieldName(iCol+iMergeX), iRow+iMergeY) ;	
					
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCelName1, (_variant_t)strCelName2) ;
					pRange->Merge(vtMissing) ;	
					strCellContent.TrimRight(_T("\n")) ;	
					
					pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
					
					pRange->FormulaR1C1 = (_variant_t)strCellContent ;
					
					MSExcel::FontPtr pFont = pRange->GetFont();
					pFont->PutSize((_variant_t)((long)10)) ;
					//	pFont->PutBold(COleVariant((short)TRUE));//Apply Bold to Headers
					
					pRange->PutHorizontalAlignment((_variant_t)((long)MSExcel::xlHAlignCenter)) ;
					pRange->PutVerticalAlignment((_variant_t)((long)MSExcel::xlVAlignCenter)) ;
					pRange->PutWrapText((_variant_t)((bool)TRUE)) ;
					
					pRange->BorderAround(COleVariant((short)1), MSExcel::xlThin , MSExcel::xlColorIndexAutomatic ,vtMissing) ;  
					
					//ads_printf("----合并格:%d(%d)-%d(%d)\n", iRow,iMergeY, iCol, iMergeX) ;
				}
				
				//////////////////////////////////////////////////////////////////////////
				//设置列宽
				if (iRow==0 && iCol<(iarrayX_Len-1))
				{
					double dWidthInDwg = arrayX_.GetAt(iCol+1)-arrayX_.GetAt(iCol) ;
					CString strColCellName ;			 
					strColCellName.Format(_T("%s1"), CBlkUtility::GetExcelFieldName(iCol+1)) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
					double dwidthInExcel = dWidthInDwg/2.41 ;
					pRange->PutColumnWidth((_variant_t)dwidthInExcel) ;						
					
				}
				//////////////////////////////////////////////////////////////////////////
				
			}
			
			//////////////////////////////////////////////////////////////////////////
			//设置行高
			if (iRow<(iarrayY_Len-1)) 
			{
				double dHeightInDwg = arrayY_.GetAt(iRow)-arrayY_.GetAt(iRow+1) ;
				CString strRowCellName ;
				strRowCellName.Format(_T("A%d"), iRow+1) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strRowCellName, vtMissing) ;
				double dHeightInExcel = dHeightInDwg/0.328 ;
				
				pRange->PutRowHeight((_variant_t)dHeightInExcel) ;
				
			}			
			//////////////////////////////////////////////////////////////////////////		
			
		}
		//end double for loop
		//////////////////////////////////////////////////////////////////////////

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
	
	for(int i=0; i<iarrayTextLen; i++)
	{
		delete arrayText_[i] ;
		arrayText_[i] = NULL ;		
	}
	for(int iLineNum=0; iLineNum<iarrayLineLen; iLineNum++)
	{		
		delete arrayLine_[iLineNum] ;
		arrayLine_[iLineNum] = NULL ; 
	}

	return TRUE ;

}
// 将统计的电缆信息写入excel
void CBlkExcelTool::WriteSumDianToXls(const CString& strSaveFileName, const CStringArray& m_strArrayAllStr, const CStringArray& m_strArrayHaveType1, const CStringArray& m_strArrayHaveType2, const CStringArray& m_strArrayHaveTypeLen1, const CStringArray& m_strArrayHaveTypeLen2, const CStringArray& m_strArray1S1S, const CStringArray& m_strArray1S2S, const CStringArray& m_strArray1S3S, const CMapStringToString& m_mapBoxTypeAndNum )
{
	_ApplicationPtr pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	//WorksheetsPtr pWorksheets = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = pExcel.CreateInstance ( _T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return ;
		}
		//使excel程序显示可见
		//pExcel->PutVisible(0,VARIANT_TRUE) ;
		//得到所有的工作簿
		pWorkBooks = pExcel->GetWorkbooks() ;
		//添加一个新的工作簿，返回结果为刚添加的工作簿的指针
		pWorkBook = pWorkBooks->Add() ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;		

		//得到当前工作簿的所有的sheet
		pWorksheets = pWorkBook->GetSheets() ;
		//定位于第一个sheet
		pSheet = pWorksheets->GetItem(vt_index) ;

		//////////////////////////////////////////////////////////////////////////
		//begin double for loop
		CString strCols[11] = {_T("A"), _T("C"), _T("D"), _T("F"), _T("G"), _T("I"), _T("J"), _T("L"), _T("M"), _T("N"),_T("O")} ;
		CString strColNames[11] = {_T("所有统计文本"), _T("电缆类型"), _T("电缆长度（m）"), _T("主干电缆"), _T("电缆长度（m）"),_T("盒子类型"), _T("盒子个数"),_T("1送1受(个)"),_T("1送2受(个)"), _T("1送3受(个)"), _T("轨道电路支线电缆6(2)(米)")} ;

		CString strColCellName ;

		for (int i=0; i<11; i++)
		{
			strColCellName.Format(_T("%s1"), strCols[i]) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
			double dwidthInExcel = 16 ;
			pRange->PutColumnWidth((_variant_t)dwidthInExcel) ;		
			pRange->FormulaR1C1 = (_variant_t)strColNames[i] ;  
		}

		for(int iStrIndex=0; iStrIndex<m_strArrayAllStr.GetSize(); iStrIndex++)
		{
			strColCellName.Format(_T("A%d"), iStrIndex+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
			//double dwidthInExcel = 25 ;
			//pRange->PutColumnWidth((_variant_t)dwidthInExcel) ;				
			pRange->FormulaR1C1 = (_variant_t)m_strArrayAllStr.GetAt(iStrIndex) ;         		
		}

		for(int iHave1=0; iHave1<m_strArrayHaveType1.GetSize(); iHave1++)
		{
			strColCellName.Format(_T("C%d"), iHave1+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
			//double dwidthInExcel = 15 ;
			//pRange->PutColumnWidth((_variant_t)dwidthInExcel) ;				
			pRange->FormulaR1C1 = (_variant_t)m_strArrayHaveType1.GetAt(iHave1) ;

			strColCellName.Format(_T("D%d"), iHave1+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;					
			pRange->FormulaR1C1 = (_variant_t)m_strArrayHaveTypeLen1.GetAt(iHave1) ;		
		}


		for(int iHave2=0; iHave2<m_strArrayHaveType2.GetSize(); iHave2++)
		{
			strColCellName.Format(_T("F%d"), iHave2+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
			//double dwidthInExcel = 15 ;
			//pRange->PutColumnWidth((_variant_t)dwidthInExcel) ;				
			pRange->FormulaR1C1 = (_variant_t)m_strArrayHaveType2.GetAt(iHave2) ;

			strColCellName.Format(_T("G%d"), iHave2+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;			
			pRange->FormulaR1C1 = (_variant_t)m_strArrayHaveTypeLen2.GetAt(iHave2) ;
		}

		//盒子
		POSITION pos = m_mapBoxTypeAndNum.GetStartPosition() ;
		int iHZIndex = 0 ;
		while(pos!=NULL)
		{
			CString strKey, strValue ;
			m_mapBoxTypeAndNum.GetNextAssoc(pos, strKey, strValue) ;

			strColCellName.Format(_T("I%d"), iHZIndex+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
			pRange->FormulaR1C1 = (_variant_t)strKey ;

			strColCellName.Format(_T("J%d"), iHZIndex+2) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;			
			pRange->FormulaR1C1 = (_variant_t)strValue ;

			iHZIndex++ ;
		}

		//轨道电路
		int i1S1SCount = m_strArray1S1S.GetSize() ;
		for(int i1S1S=0; i1S1S<i1S1SCount; i1S1S++)
		{
			if (i1S1S==0)
			{
				strColCellName.Format(_T("L%d"), i1S1S+2) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
				CString strSum ;
				strSum.Format(_T("%d"), i1S1SCount) ;
				pRange->FormulaR1C1 = (_variant_t)strSum ;
			}

			strColCellName.Format(_T("L%d"), i1S1S+3) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;			
			pRange->FormulaR1C1 = (_variant_t)m_strArray1S1S.GetAt(i1S1S) ;
		}
		int i1S2SCount = m_strArray1S2S.GetSize() ;
		for(int i1S2S=0; i1S2S<i1S2SCount; i1S2S++)
		{
			if (i1S2S==0)
			{
				strColCellName.Format(_T("M%d"), i1S2S+2) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
				CString strSum ;
				strSum.Format(_T("%d"), i1S2SCount) ;
				pRange->FormulaR1C1 = (_variant_t)strSum ;
			}

			strColCellName.Format(_T("M%d"), i1S2S+3) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;			
			pRange->FormulaR1C1 = (_variant_t)m_strArray1S2S.GetAt(i1S2S) ;
		}
		int i1S3SCount = m_strArray1S3S.GetSize() ;
		for(int i1S3S=0; i1S3S<i1S3SCount; i1S3S++)
		{
			if (i1S3S==0)
			{
				strColCellName.Format(_T("N%d"), i1S3S+2) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
				CString strSum ;
				strSum.Format(_T("%d"), i1S3SCount) ;
				pRange->FormulaR1C1 = (_variant_t)strSum ;
			}

			strColCellName.Format(_T("N%d"), i1S3S+3) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;			
			pRange->FormulaR1C1 = (_variant_t)m_strArray1S3S.GetAt(i1S3S) ;
		}

		//轨道电路支线电缆
		int iBranchLen = 0 ;
		iBranchLen = i1S1SCount*20+i1S2SCount*30+i1S3SCount*40 ;
		strColCellName.Format(_T("O2")) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strColCellName, vtMissing) ;
		CString strSum ;
		strSum.Format(_T("=L2*20+M2*30+N2*40")) ;
		pRange->Formula = (_variant_t)strSum ;

		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;		

		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;
		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;
		//end double for loop
		//////////////////////////////////////////////////////////////////////////
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
}

// 将电缆径路中的电缆信息写入excel供审核
void CBlkExcelTool::WriteCableInfoToXls(const CString& strSaveFileName, const CStringArray& strArrayCableSn, const CStringArray& strArrayCableAll, double dCoeff, vector<int > &vecParam,  vector<vector<int > > &VecCableData )
{
	_ApplicationPtr pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = pExcel.CreateInstance ( _T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return ;
		}
		//使excel程序显示可见
		//pExcel->PutVisible(0,VARIANT_TRUE) ;
		//得到所有的工作簿
		pWorkBooks = pExcel->GetWorkbooks() ;
		//添加一个新的工作簿，返回结果为刚添加的工作簿的指针
		pWorkBook = pWorkBooks->Add() ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;		

		//得到当前工作簿的所有的sheet
		pWorksheets = pWorkBook->GetSheets() ;
		//定位于第一个sheet
		pSheet = pWorksheets->GetItem(vt_index) ;

		////////////////////////////////////////////////////////////////////////////
		//参数
		CString strCellName, strCellContent ;
		CString strParamNames[7] = {_T("弯曲系数（β)"),_T("进楼增缆(a)"),_T("过轨增缆(g)"),_T("轨道间距(g2)"),_T("每盒增缆(b)"),_T("每接续增缆（c）"),_T("接续间距(c2)")} ;
		for (int i=0; i<7; i++)
		{
			for (TCHAR ch=_T('A'); ch<_T('C'); ch++)
			{
				strCellName.Format(_T("%c%d"), ch, i+1) ;
				if (ch==_T('A'))
				{
					strCellContent = strParamNames[i] ;
				}
				else if (ch==_T('B'))
				{
					if (i==0)
					{
						strCellContent.Format(_T("%.2f"), dCoeff) ;
					}
					else
					{
						strCellContent.Format(_T("%d"), vecParam[i-1]) ;
					}
				}
				pRange = NULL ;				
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				pRange->FormulaR1C1 = (_variant_t) strCellContent ;

			}
		}
		//表头
		CString strHeaders[9] = {_T("电缆编号"), _T("汇总信息"), _T("距信号楼1"), _T("距信号楼2"), _T("过轨数"), _T("箱盒数"),  _T("修正"), _T("接续盒数"), _T("进楼")} ;
		int iColWidths[9] = {15, 12, 10, 10, 8, 8, 8, 8, 8} ;
		for (TCHAR ch=_T('A'); ch<_T('J'); ch++)
		{
			int i = (int)(ch-_T('A')) ;
			strCellName.Format(_T("%c10"), ch) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
			pRange->FormulaR1C1 = (_variant_t) strHeaders[i] ;	
			pRange->PutColumnWidth((_variant_t)iColWidths[i]) ;
		}
		CString strHeaders2[5] = {_T("电缆长度"), _T("间距"), _T("过轨"), _T("箱盒"), _T("接续")} ;
		int iColWidths2[5] = {10, 8, 8, 8, 8} ;
		for (TCHAR ch=_T('L'); ch<_T('Q'); ch++)
		{
			int i = (int)(ch-_T('L')) ;
			strCellName.Format(_T("%c10"), ch) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
			pRange->FormulaR1C1 = (_variant_t) strHeaders2[i] ;
			pRange->PutColumnWidth((_variant_t)iColWidths2[i]) ;
		}

		//电缆数据
		int iNumRow = VecCableData[0].size() ;
		for (int i=0; i<iNumRow; i++) 
		{
			//基础数据(要审核的)
			for (TCHAR ch=_T('A'); ch<_T('J'); ch++)
			{
				int j = (int)(ch-_T('A')) ;
				strCellName.Format(_T("%c%d"), ch, i+11) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				if (j==0)
				{
					strCellContent = strArrayCableSn.GetAt(i) ;
				}
				else if (j==1)
				{
					strCellContent = strArrayCableAll.GetAt(i) ;
					//strCellContent.Format(_T("=CONCATENATE(CEILING(L%d, 5), "-", R%d, "(", S%d, ")" )"), i+11, i+11, i+11) ;
					//strCellContent.Format(_T("=CONCATENATE(CEILING(R[0]C[%d], 5), \"-\", R[0]C[%d], \"\(\", R[0]C[%d], \"\)\" )"), (int)(_T('L')-ch), (int)(_T('R')-ch), (int)(_T('S')-ch) ) ;
				}
				else 
				{
					strCellContent.Format(_T("%d"), VecCableData[j-2][i]) ;
				}
				pRange->FormulaR1C1 = (_variant_t) strCellContent;				
			}
			//中间计算数据(供参考)
			for (TCHAR ch=_T('L'); ch<_T('Q'); ch++)
			{			
				strCellName.Format(_T("%c%d"), ch, i+11) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				if (ch==_T('L'))
				{
					//strCellContent.Format(_T("=(SUM(M%d:P%d)+G%d+I%d)*B1"), i+11, i+11 ) ;
					//strCellContent.Format(_T("=CEILING\(SUM\(R[0]C[1]:R[0]C[5]\)*R[%d]C[%d], 5\)"), 1-i-11,  (int)(_T('B')-ch) ) ;
					strCellContent.Format(_T("=CEILING\(\(SUM\(R[0]C[%d]:R[0]C[%d]\)+R[0]C[%d]+R[0]C[%d]\)*R[%d]C[%d], 5\)"),  (int)(_T('M')-ch),  (int)(_T('P')-ch), (int)(_T('G')-ch),(int)(_T('I')-ch), 1-i-11,  (int)(_T('B')-ch) ) ;
				}
				else if (ch==_T('M'))
				{
					//strCellContent.Format(_T("=ABS(C%d-D%d)"), i+11, i+11 ) ;
					strCellContent.Format(_T("=ABS(R[0]C[%d]-R[0]C[%d])"), (int)(_T('C')-ch) , (int)(_T('D')-ch)) ;
				}
				else if (ch==_T('N'))
				{
					//strCellContent.Format(_T("=E%d*B3+IF(E%d>1, E%d-1,0)*B4"), i+11, i+11,  i+11 ) ;
					strCellContent.Format(_T("=R[0]C[%d]*R[%d]C[%d]+IF(R[0]C[%d]>1, R[0]C[%d]-1,0)*R[%d]C[%d]"), (int)(_T('E')-ch), 3-i-11, (int)(_T('B')-ch), (int)(_T('E')-ch), (int)(_T('E')-ch), 4-i-11, (int)(_T('B')-ch) ) ;
				}
				else if (ch==_T('O'))
				{
					//strCellContent.Format(_T("=F%d*B5"), i+11 ) ;
					strCellContent.Format(_T("=R[0]C[%d]*R[%d]C[%d]"),  (int)(_T('F')-ch), 5-i-11, (int)(_T('B')-ch) ) ;
				}
				else if (ch==_T('P'))
				{
					//strCellContent.Format(_T("=G%d*B6"), i+11 ) ;
					strCellContent.Format(_T("=R[0]C[%d]*R[%d]C[%d]"),  (int)(_T('H')-ch), 6-i-11, (int)(_T('B')-ch) ) ;
				}

				pRange->FormulaR1C1 = (_variant_t) strCellContent;				
			}
		}

		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;		

		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;
		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;
		//end double for loop
		//////////////////////////////////////////////////////////////////////////
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
}

// 将距离信号楼距离信息导出到excel
void CBlkExcelTool::WriteDistToXhlToXls(const CString& strSaveFileName, const vector<vector<vector<CString> > >& vecSection)
{	
	_ApplicationPtr pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = pExcel.CreateInstance ( _T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			//return ;
		}
		//使excel程序显示可见
		//pExcel->PutVisible(0,VARIANT_TRUE) ;
		//得到所有的工作簿
		pWorkBooks = pExcel->GetWorkbooks() ;
		//添加一个新的工作簿，返回结果为刚添加的工作簿的指针
		pWorkBook = pWorkBooks->Add() ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;		

		//得到当前工作簿的所有的sheet
		pWorksheets = pWorkBook->GetSheets() ;
		//定位于第一个sheet
		pSheet = pWorksheets->GetItem(vt_index) ;

		////////////////////////////////////////////////////////////////////////////
		CString strCellName, strCellContent ;
		//表头
		CString strHeaders[] = {_T("类型"), _T("名称"), _T("距离"), _T("区段"), _T("上/下栏")} ;
		int iColWidths[] = {10, 10, 10, 10, 10} ;
		for (TCHAR ch=_T('B'); ch<_T('G'); ch++)
		{
			int i = (int)(ch-_T('B')) ;
			strCellName.Format(_T("%c1"), ch) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
			pRange->FormulaR1C1 = (_variant_t) strHeaders[i] ;	
			pRange->PutColumnWidth((_variant_t)iColWidths[i]) ;
		}
		int iRow = 0 ;
		int iNumSec = vecSection.size() ;
		acutPrintf(_T("\nWriteDistToXhlToXls,NumSec:%d"), iNumSec) ;
		for (int iSecIndex=0; iSecIndex<iNumSec; iSecIndex++)
		{
			int iNumRow = vecSection[iSecIndex].size() ;
			acutPrintf(_T("\nWriteDistToXhlToXls,Sec[%d].NumRow:%d"), iSecIndex, iNumRow) ;
			for (int j=0; j<iNumRow; j++)
			{		
				for (TCHAR ch=_T('B'); ch<_T('G'); ch++)
				{
					int i = (int)(ch-_T('B')) ;
					strCellName.Format(_T("%c%d"), ch, iRow+2) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					
					if (i==4) //上下栏
					{
						strCellContent = vecSection[iSecIndex][j][2] ;	
						int iLoc = strCellContent.Find(_T("__")) ;
						if (iLoc!=-1)
						{
							strCellContent = strCellContent.Mid(iLoc+2) ;							
						}
					}
					else
					{
						strCellContent = vecSection[iSecIndex][j][i] ;	
						if (i==2)
						{
							CString strCellContentUpOrDown  ;
							int iLoc = strCellContent.Find(_T("__")) ;
							if (iLoc!=-1)
							{
								strCellContent = strCellContent.Left(iLoc) ;
							}
						}
					}

					pRange->FormulaR1C1 = (_variant_t) strCellContent ;	
					pRange->PutColumnWidth((_variant_t)iColWidths[i]) ;
				}
				iRow++ ;
			}

		}
		acutPrintf(_T("\n")) ;

		//保存excel文件
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;		

		pWorkBook->SaveAs( (_variant_t) strSaveFileName, COleVariant(varOptional), COleVariant(varOptional), 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
			COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;
		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;
		//end double for loop
		//////////////////////////////////////////////////////////////////////////
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
	
}


// 从excel导入距离信号楼距离
int CBlkExcelTool::ReadDistToXhlFromXls(const CString& strFileName, vector<vector<vector<CString> > >& vecSection)
{
	int iRet = 0 ;
	
	_ApplicationPtr pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	SheetsPtr pWorksheets = NULL ; 
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	try
	{
		if (bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = pExcel.CreateInstance ( _T("Excel.Application") ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;			
		}
		pWorkBooks = pExcel->GetWorkbooks () ;

		pWorkBook = pWorkBooks->Open( (_bstr_t)strFileName ) ;
		_variant_t vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		pWorksheets  = pExcel->GetSheets() ; 
		//获得sheet1的指针
		pSheet = pWorksheets->GetItem( vt_index ) ;

		//////////////////////////////////////////////////////////////////////////
		//检查一下表头是否符合
		BOOL bHeaderValid = TRUE ;
		CString strCellName, strCellContent ;
		//表头
		CString strHeaders[] = {_T("类型"), _T("名称"), _T("距离"), _T("区段"), _T("上/下栏")} ;
		for (TCHAR ch=_T('B'); ch<_T('G'); ch++)
		{
			int i = (int)(ch-_T('B')) ;
			strCellName.Format(_T("%c1"), ch) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
			_variant_t range_var = pRange->GetText() ;	
			strCellContent = (CString ) range_var.bstrVal ;
			if (strCellContent!=strHeaders[i])
			{
				bHeaderValid = FALSE ;
				break;
			}
		}
		if (bHeaderValid)
		{
			int iCur_Row = 2 ; //起始行

			CString strTmp ;
			BOOL bNext = TRUE ; 

			vector<vector<CString> > vecToXhl ;

			CString strPreSec = _T("1") ;
			while (bNext) 
			{
				vector<CString > vec ;
				CString strCurSec = _T("1") ;
				for (TCHAR ch=_T('B'); ch<_T('F'); ch++)
				{
					int i = (int)(ch-_T('B')) ;
					strCellName.Format(_T("%c%d"), ch, iCur_Row) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					_variant_t range_var = pRange->GetText() ;	
					strCellContent = (CString ) range_var.bstrVal ;
					strCellContent.Trim() ;
					if (i==0&&strCellContent.IsEmpty())
					{
						bNext = FALSE ;
						break;;
					}

					if (i==3&&!strCellContent.IsEmpty())
					{
						strCurSec = strCellContent ;
					}

					if (i==2)
					{
						strCellName.Format(_T("F%d"), iCur_Row) ;
						pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
						_variant_t range_var = pRange->GetText() ;	
						CString strCellContentTmp = (CString ) range_var.bstrVal ;
						if (!strCellContentTmp.IsEmpty())
						{
							strCellContent.AppendFormat(_T("__%s"), strCellContentTmp) ;
						}
					}
					vec.push_back(strCellContent) ;
				}
				
				if (bNext==FALSE)
				{
					break;
				}
				if (strCurSec.Compare(strPreSec)!=0)
				{
					vecSection.push_back(vecToXhl) ;
					vecToXhl.clear() ;
					vecToXhl.push_back(vec) ;		
					strPreSec = strCurSec ;
				}
				else
				{
					vecToXhl.push_back(vec) ;
				}

				iCur_Row++ ;
				iRet = iCur_Row-1 ;
			}
			//最后一个区段(don't forget)
			vecSection.push_back(vecToXhl) ;
		}
		else
		{
			AfxMessageBox(_T("表头不符，请检查！")) ;
		}
	
		//////////////////////////////////////////////////////////////////////////
		pWorkBook->Close( ) ;
		pWorkBook = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;
		//end double for loop
		//////////////////////////////////////////////////////////////////////////
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		pExcel->Quit() ;
		pExcel = NULL ;		
	}
	
	return iRet ;
}
