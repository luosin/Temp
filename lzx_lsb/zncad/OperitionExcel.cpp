#pragma once
#include "operitionexcel.h"
//#include "stdafx.h"
//#include "znsxt.h"
//#include "CableInfoDlg.h"
//#include "XhlCableInfoDlg.h"
#include "znutility.h"

//#include "EntPropertyDlg.h"

#include "..\blkuserdll\ViewExtents.h"

// COperitionExcel::COperitionExcel(void)
// {
// }
 COleVariant
// covTrue((short)TRUE),
// covFalse((short)FALSE),
 covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);    

COperitionExcel::COperitionExcel( void )
{
	open_excel_file=_T("");
	m_pExcel=NULL;
	pWorkBooks=NULL;
	pWorkBook=NULL;
	pWorksheets=NULL;
	pSheet=NULL;
	pRange=NULL;
}

COperitionExcel::~COperitionExcel(void)
{
}

BOOL COperitionExcel::OpenExcel( CString str_name )
{
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
			return FALSE ;
		}
		pWorkBooks=NULL;
		pWorkBooks = m_pExcel->GetWorkbooks() ;
		CString open_excel_file=str_name;
		pWorkBook=NULL;
		pWorkBook = pWorkBooks->Open( (_bstr_t)open_excel_file) ;
		pWorksheets=NULL;
		pWorksheets  = pWorkBook->GetSheets() ;
		if(pWorksheets!=NULL)
		return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in OpenExcel"));			
		return FALSE;
	}	
	return FALSE;

}
BOOL COperitionExcel::ClearCell( int iNum_sheet,int iRow,int iCol )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;		
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName ;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//判断是否是合并格，如何是的话，应该先拆除合并再山茶
		int iRowCnt=0;
		int iColCnt=0;
		if(IsCombandCells(iNum_sheet,iRow,iCol,iRowCnt,iColCnt)==TRUE)
		{
			RangePtr pRangeTemp=pRange->GetMergeArea();
			pRangeTemp->UnMerge();
		}
		pRange->ClearContents();
		if(pRange!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in ClearCell"));			
		return FALSE;
	}	
	return FALSE;


}

BOOL COperitionExcel::WriteDatatoExcel( int iNum_sheet,int iRow,int iCol,CString Data )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;		
 		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellContent=Data;
		strCellContent.Replace(_T("%%161"),_T("I")) ;
		strCellContent.Replace(_T("%%162"),_T("II")) ;
		strCellContent.Replace(_T("%%163"),_T("III")) ;
		strCellContent.Replace(_T("%%164"),_T("IV")) ;
		strCellContent.Replace(_T("%%165"),_T("V")) ;
		strCellContent.Replace(_T("%%166"),_T("VI")) ;
		strCellContent.Replace(_T("%%167"),_T("VII")) ;
		strCellContent.Replace(_T("%%168"),_T("VIII")) ;
		strCellContent.Replace(_T("%%169"),_T("IX")) ;
		strCellContent.Replace(_T("%%170"),_T("X")) ;
		CString strCellName ;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
		pRange->PutNumberFormat((_variant_t)_T("@")) ;//20101110 modified
		pRange->FormulaR1C1 = (_variant_t)strCellContent ;	
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		pFont->PutColorIndex((_variant_t)(1));
		if(pRange!=NULL)
		return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in WriteDatatoExcel"));			
		return FALSE;
	}	
	return FALSE;


}

BOOL COperitionExcel::WriteDatatoExcel( int iNum_sheet,int iRow,int iCol,vector<CString>& vec_str_Data )
{
	try
	{
		//long t1=GetTickCount();
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;		
		//pWorksheets  = pWorkBook->GetSheets() ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellContent;
		for(int i=0;i<vec_str_Data.size();i++)
		{
			strCellContent+=vec_str_Data[i];
		}
		strCellContent.Replace(_T("%%161"),_T("I")) ;
		strCellContent.Replace(_T("%%162"),_T("II")) ;
		strCellContent.Replace(_T("%%163"),_T("III")) ;
		strCellContent.Replace(_T("%%164"),_T("IV")) ;
		strCellContent.Replace(_T("%%165"),_T("V")) ;
		strCellContent.Replace(_T("%%166"),_T("VI")) ;
		strCellContent.Replace(_T("%%167"),_T("VII")) ;
		strCellContent.Replace(_T("%%168"),_T("VIII")) ;
		strCellContent.Replace(_T("%%169"),_T("IX")) ;
		strCellContent.Replace(_T("%%170"),_T("X")) ;
		CString strCellName ;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		pFont->PutColorIndex((_variant_t)(1));
		//pRange->PutRowHeight(18.292683);
		if(pRange!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in WriteDatatoExcel"));			
		return FALSE;
	}	
	return FALSE;


}
// BOOL COperitionExcel::ReadDataFromExcel( int iNum_sheet,int iRow,int iCol,vector<CString>& vec_str_Data)
// {
// 	try
// 	{
// 		VARIANT vt_index ;
// 		vt_index.vt = VT_I2 ;
// 		vt_index.iVal = iNum_sheet;		
// 		//pWorksheets  = pWorkBook->GetSheets() ;
// 		pSheet = pWorksheets ->GetItem( vt_index ) ;
// 		CString strCellContent;
// 		CString strCellName ;
// 		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
// 		strCellName.Format(_T("%s%d"),strColName, iRow) ;
// 		pRange = NULL ;
// 		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
// 		//pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
// 		pRange->GetValue2();
// 		if(pRange!=NULL)
// 			return TRUE;
// 		else
// 			return FALSE;
// 	}
// 	catch(_com_error &e)
// 	{
// 		//iFound = 0 ;
// 		CString sBuff = CBlkUtility::GetErrorDescription(e);
// 		AfxMessageBox(sBuff);  
// 		return FALSE;
// 	}
// 	catch(...)
// 	{	
// 		//iFound = 0 ;
// 		AfxMessageBox(_T("未知的错误! in WriteDatatoExcel"));			
// 		return FALSE;
// 	}	
// 	return FALSE;
// 
// 
// }


BOOL COperitionExcel::WriteDatatoExcel( int iNum_sheet,int iRow,int iCol,int iData )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		//pWorksheets  = pWorkBook->GetSheets() ;
		//int iCellContent=iData;
		CString strCellName;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		pRange->PutFormula((_variant_t)iData) ;//20101110 modified
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		pFont->PutColorIndex((_variant_t)(1));
		double d_h=(double) pRange->GetHeight();
		double d_w=(double)pRange->GetWidth();
		pRange->PutRowHeight(18.292683);
		
		//pRange->PutColumnWidth(2*d_w);
		//acutPrintf(_T("行高=%6f，列宽=%6f"),d_h,d_w);
		//long t2=GetTickCount();
		//CString str;
		//str.Format(_T("Write int time:%d ms"),t2-t1);
		//acutPrintf(_T("\n %s"),str);
		if(pRange!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in WriteDatatoExcel"));			
		return FALSE;
	}	
	return FALSE;
}

BOOL COperitionExcel::SaveExcel()
{
		pWorkBook->Save() ;
	    return TRUE;
}
BOOL COperitionExcel::SaveasXSLFile(const CString &xls_file)
{
	//保存excel文件
	try
	{
		VARIANT varOptional ;
		VariantInit(&varOptional);
		varOptional.vt		= VT_ERROR;
		varOptional.scode	= (long)DISP_E_PARAMNOTFOUND;		
		pWorkBook->SaveAs( (_variant_t) xls_file ,COleVariant(varOptional), COleVariant(varOptional), 
		COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), xlExclusive, 
		COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional), COleVariant(varOptional)) ;
		return TRUE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in SaveAsXLSFile"));			
		return FALSE;
	}	
	return FALSE;
	

}
BOOL COperitionExcel::RealseExcel()
{
	pWorkBook->Save() ;
	///关闭打开的各个资源
	pWorkBooks->Close( ) ;
	pWorkBooks = NULL ;
	m_pExcel->Quit() ;		
	//m_pExcel->Release() ;
	m_pExcel = NULL ;
	return TRUE;
}

BOOL COperitionExcel::GetColumnCount( int iNum_sheet,int &iColCount )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		//pWorksheets  = pWorkBook->GetSheets() ;
		//int iCellContent=iData;
		//CString strCellName;
		//CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		//strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange=pSheet->GetUsedRange();
		RangePtr pRangeTem=pRange->GetColumns();
		iColCount=pRangeTem->GetCount();
		if(pRangeTem!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in GetColumnCount"));			
		return FALSE;
	}	
	return FALSE;

}

BOOL COperitionExcel::GetRowCount( int iNum_sheet,int &iRowCount )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		pRange = NULL ;
		pRange=pSheet->GetUsedRange();
		RangePtr pRangeTem=pRange->GetRows();
		iRowCount=pRangeTem->GetCount();
		if(pRangeTem!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in GeRowCount"));			
		return FALSE;
	}	
	return FALSE;

}

BOOL COperitionExcel::GetStartColumn( int iNum_sheet,int &iCol )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		//pWorksheets  = pWorkBook->GetSheets() ;
		//int iCellContent=iData;
		//CString strCellName;
		//CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		//strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange=pSheet->GetUsedRange();
		RangePtr pRangeTem=pRange->GetColumns();
		iCol=pRangeTem->GetColumn();
		if(pRangeTem!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in GetStartColumn"));			
		return FALSE;
	}	
	return FALSE;

}

BOOL COperitionExcel::GetStartRow( int iNum_sheet,int &iRow )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		//pWorksheets  = pWorkBook->GetSheets() ;
		//int iCellContent=iData;
		//CString strCellName;
		//CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		//strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange=pSheet->GetUsedRange();
		RangePtr pRangeTem=pRange->GetRows();
		iRow=pRangeTem->GetRow();
		if(pRangeTem!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in GetStartColumn"));			
		return FALSE;
	}	
	return FALSE;

}

BOOL COperitionExcel::ClearAllCells( int iNum_sheet )
{
	//BOOL bRet=FALSE;
	VARIANT vt_index ;
	vt_index.vt = VT_I2 ;
	vt_index.iVal = iNum_sheet;	
	pSheet = pWorksheets ->GetItem( vt_index ) ;
	//pWorksheets  = pWorkBook->GetSheets() ;
	//int iCellContent=iData;
	//CString strCellName;
	//CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
	//strCellName.Format(_T("%s%d"),strColName, iRow) ;
	//pRange = NULL ;
	//pRange=pSheet->GetUsedRange();
	//this->OpenExcel(_T("C:\\辅助开发\\平面图联锁表生成1.xls"));
	int iColCnt=0;
	this->GetColumnCount(1,iColCnt);
	//acutPrintf(_T("\nExcel一共有%d列"),count);
	int iRowCnt=0;
	this->GetRowCount(1,iRowCnt);
	acutPrintf(_T("\n Excel一共有%d行，%d列"),iRowCnt,iColCnt);
	acutPrintf(_T("\n准备清空原有Excel数据"));
	int iStartRow=0;
	int iStartCol=0;
	this->GetStartRow(1,iStartRow);
	this->GetStartColumn(1,iStartCol);
	acutPrintf(_T("\n 起始行 %d,起始列%d"),iStartRow,iStartCol);
	for(int i=iStartRow+2;i<iRowCnt+1;i++)
	{
		for(int j=iStartCol;j<iColCnt+1;j++)
		{
			this->ClearCell(1,i,j);
		}
	}
    return TRUE;
}

BOOL COperitionExcel::SetRowCombandCells( int iNum_sheet,int iRow,int iCol,int iColCnt)
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName;
		CString strHebingGe;
		CString strColName= CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		strHebingGe+=strCellName;
		if(iColCnt!=0)
		{
			CString strColName2 = CBlkUtility::GetExcelFieldName(iCol+iColCnt-1);
			strCellName.Format(_T(":%s%d"),strColName2, iRow) ;
			strHebingGe+=strCellName;
		}
		
		//strHebingGe=_T("F103:G104");		
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strHebingGe, vtMissing) ;
		//pRange->Select();
		//acutPrintf("\n 含有%d行 %d列",pRange->GetRows(),pRange->GetColumns());
		//pRange->MergeCells=TRUE;
		if(iColCnt!=0)
		pRange->Merge(vtMissing);
	    //pRange->PutMergeCells(vtMissing);
		//CString strCellContent=_T("测试合并");
		//pRange->PutMergeCells((_variant_t)strHebingGe);
		//pRange->PutFormula((_variant_t)strCellContent) ;//20101110 modified
		int iRow_cnt=0;
		int iCol_cnt=0;
		if(this->IsCombandCells(1,iRow,iCol,iRow_cnt,iCol_cnt)==TRUE)
		{
			//acutPrintf(_T("\n是合并格子"));
			return TRUE;
		}
		else
			return FALSE;
		//return pRange->GetMergeCells().boolVal;
		//return TRUE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in GetStartColumn"));			
		return FALSE;
	}	
	return FALSE;

}

BOOL COperitionExcel::SetColumnCombandCellsAndCenterShow( int iNum_sheet,int iRow,int iCol,int iRowCnt )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName1;
		CString strCellName2;
		CString strHebingGe;
		CString strColName= CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName1.Format(_T("%s%d"),strColName, iRow) ;
		   strHebingGe+=strCellName1;
		if(iRowCnt!=0)
		{
			strCellName2.Format(_T(":%s%d"),strColName, iRow+iRowCnt-1) ;
		   strHebingGe+=strCellName2;
		}
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strHebingGe, vtMissing) ;
		//pRange->MergeCells=TRUE;
// 		orizontalAlignment = xlCenter
// 			.VerticalAlignment = xlCenter
// 			.WrapText = True
// 			.Orientation = 0
// 			.AddIndent = False
// 			.IndentLevel = 0
// 			.ShrinkToFit = False
// 			.ReadingOrder = xlLTR
// 			.MergeCells = True
		//设置合并格居中显示并且自动换行
		pRange->PutHorizontalAlignment(xlCenter);
		pRange->PutVerticalAlignment(xlCenter);
		pRange->PutWrapText(TRUE);
		pRange->PutOrientation(0);
		pRange->PutAddIndent(FALSE);
		pRange->PutIndentLevel(0);
		pRange->ShrinkToFit=FALSE;
		pRange->PutReadingOrder(xlLTR);
		if(iRowCnt!=0)
		pRange->Merge(vtMissing);
		//return pRange->MergeCells;
		int iRow_cnt=0;
		int iCol_cnt=0;
		if(this->IsCombandCells(1,iRow,iCol,iRow_cnt,iCol_cnt)==TRUE)
		{
			//acutPrintf(_T("\n是合并格子"));
			return TRUE;
		}
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in SetColumnCombandCells"));			
		return FALSE;
	}	
	return FALSE;
}
BOOL COperitionExcel::SetColumnCombandCells( int iNum_sheet,int iRow,int iCol,int iRowCnt )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName1;
		CString strCellName2;
		CString strHebingGe;
		CString strColName= CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName1.Format(_T("%s%d"),strColName, iRow) ;
		strHebingGe+=strCellName1;
		if(iRowCnt!=0)
		{
			strCellName2.Format(_T(":%s%d"),strColName, iRow+iRowCnt-1) ;
			strHebingGe+=strCellName2;
		}
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strHebingGe, vtMissing) ;
		//pRange->MergeCells=TRUE;
		// 		orizontalAlignment = xlCenter
		// 			.VerticalAlignment = xlCenter
		// 			.WrapText = True
		// 			.Orientation = 0
		// 			.AddIndent = False
		// 			.IndentLevel = 0
		// 			.ShrinkToFit = False
		// 			.ReadingOrder = xlLTR
		// 			.MergeCells = True
		//设置合并格居中显示并且自动换行
// 		pRange->PutHorizontalAlignment(xlCenter);
// 		pRange->PutVerticalAlignment(xlCenter);
// 		pRange->PutWrapText(TRUE);
// 		pRange->PutOrientation(0);
// 		pRange->PutAddIndent(FALSE);
// 		pRange->PutIndentLevel(0);
// 		pRange->ShrinkToFit=FALSE;
// 		pRange->PutReadingOrder(xlLTR);
		if(iRowCnt!=0)
			pRange->Merge(vtMissing);
		//return pRange->MergeCells;
		int iRow_cnt=0;
		int iCol_cnt=0;
		if(this->IsCombandCells(1,iRow,iCol,iRow_cnt,iCol_cnt)==TRUE)
		{
			//acutPrintf(_T("\n是合并格子"));
			return TRUE;
		}
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in SetColumnCombandCells"));			
		return FALSE;
	}	
	return FALSE;
}

BOOL COperitionExcel::IsCombandCells( int iNum_sheet,int iRow,int iCol,int &iRow_cnt,int &iCol_cnt)
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName;
		CString strHebingGe;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//acutPrintf("\nboolVal=%d",pRange->GetMergeCells().boolVal);
		if(pRange->GetMergeCells().boolVal==-1)
		{
			//获取合并格行数和列数
			RangePtr pRangeTem=pRange->GetMergeArea();
			iRow_cnt=pRangeTem->GetRows();
			iCol_cnt=pRangeTem->GetColumns();
			return TRUE;
		}
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in IsCombandCells"));			
		return FALSE;
	}	
	return FALSE;

}

BOOL COperitionExcel::ReadDatafromExcel( int iNum_sheet,int iRow,int iCol,CString &strData )
{
	try
	{
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;	
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName;
		CString strHebingGe;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		//取得文本 ;
		_variant_t range_var = pRange->GetText() ;	
		strData = (CString ) range_var.bstrVal ;
		strData.TrimLeft() ;
		strData.TrimRight() ;
		strData.Replace(_T("{"),_T("\\{")); 
		strData.Replace(_T("}"),_T("\\}")) ;
		return TRUE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误!in ReadDataFromExcel"));			
		return FALSE;
	}	
	return FALSE;


}

BOOL COperitionExcel::SetCellFontColor( int iNum_sheet,int iRow,int iCol,long indexColor )
{
	try
	{
		//long t1=GetTickCount();
		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = iNum_sheet;		
		//pWorksheets  = pWorkBook->GetSheets() ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;
		CString strCellName ;
		CString strColName = CBlkUtility::GetExcelFieldName(iCol) ;
		strCellName.Format(_T("%s%d"),strColName, iRow) ;
		pRange = NULL ;
		pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
		MSExcel::FontPtr pFont = pRange->GetFont();
		pFont->PutSize((_variant_t)((long)10)) ;
		//pFont->GetColorIndex()
		pFont->PutColorIndex((_variant_t)(indexColor));
		if(pRange!=NULL)
			return TRUE;
		else
			return FALSE;
	}
	catch(_com_error &e)
	{
		//iFound = 0 ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		return FALSE;
	}
	catch(...)
	{	
		//iFound = 0 ;
		AfxMessageBox(_T("未知的错误! in SetCellFontColor"));			
		return FALSE;
	}	
	return FALSE;


}
