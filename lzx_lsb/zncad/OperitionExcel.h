#pragma once
#include "../blkuserdll/UserSystem.h"
// #include "CableBoxNode.h"
// #include "../tcl/tcl/sequential_tree.h"
// #include "XhjDATA.h"
// #include "GdDATA.h"
// #include "DcDATA.h"
#include "afxcoll.h"
#include "StdAfx.h"
//#include "DistToXhlPalette.h"
//#include<vector>
//using namespace  tcl;
using namespace std;



class COperitionExcel
{
public:
	COperitionExcel(void);
	~COperitionExcel(void);
protected:
	CString       open_excel_file;
	_ApplicationPtr m_pExcel ;
	WorkbooksPtr pWorkBooks;
	_WorkbookPtr pWorkBook;
	SheetsPtr pWorksheets ; 
	RangePtr pRange  ;
	_WorksheetPtr pSheet  ;
public:
	BOOL OpenExcel(CString str_name);
	BOOL WriteDatatoExcel(int iNum_sheet,int iRow,int iCol,CString Data);
	BOOL WriteDatatoExcel(int iNum_sheet,int iRow,int iCol,vector<CString>& vec_str_Data);
    BOOL WriteDatatoExcel(int iNum_sheet,int iRow,int iCol,int iData);
    BOOL SetCellFontColor(int iNum_sheet,int iRow,int iCol,long indexColor);
	BOOL ReadDatafromExcel(int iNum_sheet,int iRow,int iCol,CString &strData);
	BOOL GetColumnCount(int iNum_sheet,int &iColCount);
 	BOOL GetStartColumn(int iNum_sheet,int &iCol);
 	BOOL GetStartRow(int iNum_sheet,int &iRow);
 	BOOL GetRowCount(int iNum_sheet,int &iRowCount);
 	BOOL ClearCell( int iNum_sheet,int iRow,int iCol);
	BOOL ClearAllCells(int iNum_sheet);
	//BOOL IsCombandCells(int iNum_sheet,int iRow,int iCol);
	BOOL IsCombandCells( int iNum_sheet,int iRow,int iCol,int &iRow_cnt,int &iCol_cnt);
	BOOL SetRowCombandCells(int iNum_sheet,int iRow,int iCol,int iColCnt);
	BOOL SetColumnCombandCells(int iNum_sheet,int iRow,int iCol,int iRowCnt);
	BOOL SaveExcel();
	BOOL RealseExcel();
	BOOL SetColumnCombandCellsAndCenterShow( int iNum_sheet,int iRow,int iCol,int iRowCnt );
	BOOL SaveasXSLFile(const CString &xls_file);
};
