// BlkExcelTool.h: interface for the CBlkExcelTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLKEXCELTOOL_H__29D15C99_2C64_4321_80F9_370614F72C52__INCLUDED_)
#define AFX_BLKEXCELTOOL_H__29D15C99_2C64_4321_80F9_370614F72C52__INCLUDED_

#ifdef ZHFDLL_EXPORT
#define ZHFDLL __declspec(dllexport)
#else
#define ZHFDLL __declspec(dllimport)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ExcelRange.h"
#include <vector>
using namespace std ;

class ZHFDLL CBlkExcelTool  
{
public:
	CBlkExcelTool();
	virtual ~CBlkExcelTool();

	int GetExcelRange(CString strXlsFileName, CString strMark, CArray<double, double> &arrayWidth, CArray<CExcelRange*, CExcelRange*> &arrayexcelrange, int iSearchStartRow=1, int iSearchEndRow=3000, TCHAR chSearchStartCol[]=_T("a"), TCHAR chSearchEndCol[]=_T("w")) ;

	//将Dwg图纸中选中的表格转到excel里面去
	static BOOL DwgToExcel();

	// 将统计的电缆信息写入excel
	static void WriteSumDianToXls(const CString& strSaveFileName, const CStringArray& m_strArrayAllStr, const CStringArray& m_strArrayHaveType1, const CStringArray& m_strArrayHaveType2, const CStringArray& m_strArrayHaveTypeLen1, const CStringArray& m_strArrayHaveTypeLen2, const CStringArray& m_strArray1S1S, const CStringArray& m_strArray1S2S, const CStringArray& m_strArray1S3S, const CMapStringToString& m_mapBoxTypeAndNum );
	// 将电缆径路中的电缆信息写入excel供审核
	static void WriteCableInfoToXls(const CString& strSaveFileName, const CStringArray& strArrayCableSn, const CStringArray& strArrayCableAll, double dCoeff, vector<int > &vecParam,  vector<vector<int > > &VecCableData );
	// 将距离信号楼距离信息导出到excel
	static void WriteDistToXhlToXls(const CString& strSaveFileName, const vector<vector<vector<CString> > >& vecSection);
	// 从excel导入距离信号楼距离
	static int ReadDistToXhlFromXls(const CString& strFileName, vector<vector<vector<CString> > >& vecSection);
};

#endif // !defined(AFX_BLKEXCELTOOL_H__29D15C99_2C64_4321_80F9_370614F72C52__INCLUDED_)
