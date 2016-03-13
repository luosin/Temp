// LKDataTable.h: interface for the CLKDataTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LKDATATABLE_H__EBEAF84D_BC20_47CE_8672_DED866A01224__INCLUDED_)
#define AFX_LKDATATABLE_H__EBEAF84D_BC20_47CE_8672_DED866A01224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLKDataTable  
{
public:
	int m_iLimitToFG;
	void ComputeJLInfo2();
	void DrawPdt_LKDATA();
	void UpDateAllRealLoc_LKDATA(CString strDbTable);
	int ImportYdq_LKDATA(CString strXlsFileName);
	int ImportXHJ_LKDATA(CString strXlsFileName);
	void DrawPdt2();
	int ImportDFXTable(const CString strXlsFileName);
	BOOL GetNumOfLoc(const CString strLoc, double &dLoc);
	int ImportPdTable(const CString strXlsFileName);
	CString m_strMileSysName;
	BOOL m_bAutoComputeXhjName;
	int GetNameFromLoc(CString strXlsFileName, const CString strFromCol, int iFromStartRow, const CString strToCol, int iToStartRow, int iSheet=1, int iUpOrDown=0);
	void DrawPdt();
	void ExportXHTable(CString strSavePath);
	int ImportStationInfo(const CString strXlsFileName);
	int GetStationInfo(const CString strStationName, CString  &strBigRegionSn, CString &strSmallRegionSn, CString &strStationSn);
	void FormatGroupSn(int iGroupSn, CString &strGroupSn);
	CString m_strSmallRegionSn;
	CString m_strBigRegionSn;
	CString m_strStationSn;
	void ComputeYdqData();
	CString m_strCurStation;
	void InsertZnXhData();
	UINT m_nBaOffset;
	int ImportJzCzXhjInfo(const CString strXlsFileName);
	void GetSpeedStr(long iStartLoc, long iSumLen, CString &strRet, int iUpOrDown, int iPlus);
	int GetOtherDc(const CString strDc, CString &strOtherDc);
	int GetXhjInfo(const CString strXhjName, int &iUpOrDown, long &iXhjLoc, int &iType, int &iIsReverse);
	
	//轨道信息 形式111-113DG
	int GetGdInfo(const CString strGdName, const CString strDcList, int &iDcCase, long &iGdLen, CString &strFreq, long &iDcLoc1, long &iDcLoc2, CString &strDcName1, CString &strDcName2, int iToward=-1);
	//轨道信息 形式IIG, 3G1,111/113WG
	int GetGdInfo(const CString strGdName, long &iLen1, long &iLen2, CString &strFreq1, CString &strFreq2, int &iUpOrDown);
	//轨道信息 形式111DG
	int GetGdInfo(const CString strGdName, const CString strDcList, int &iDcCase,long &iGdLen,CString & strFreq);
	
	//道岔信息
	int GetDcInfo(const CString strDcName, long &iDcLoc);
	void ComputeDc();
	void DrawSyt();
	void ExportYdqData(CString strSavePath);
	void ExportJlData(CString strSavePath);
	void ComputeJLInfo();
	int ImportDcInfo(const CString strXlsFileName);
	BOOL GetNumOfLoc(const CString strLoc, long &iLoc);
	int ImportSpeedInfo(const CString strXlsFileName);
	int ImportGdSectionInfo(const CString strXlsFileName);
	int ImportLsbTable(const CString strXlsFileName);
	CString m_strXHTitle;
	void ComputeGdName();
	CString m_strSysBasePath;
	void ExportXHData(CString strSavePath);
	BOOL GetFreqByIndex(int iIndex, CString &strFreq, int iDownOrUp);
	int GetFreqIndex(const CString strFreq, int iDownOrUp);
	void ComputeXHData();
	BOOL ConvertSpecialChar(const CString strSrc, CString &strRet);
	void UpDateAllRealLoc();
	void ComputeDivide();
	void ComputeFreq();
	BOOL GetXHJName(const CString strOldName, CString &strNewName);
	int GetLongShortChain(long iIdStart, long iIdEnd, long &iIdLongShortChain, long &iLenLongShortChain, long &iShowLocLongShortChain, long &iRealLocLongShortChain, int iReadTableIndex=0);
	BOOL GetLocShow(long iLoc, const CString strMileSys, CString &strLocShow);
	BOOL GetFName(long iLoc, CString &strFName, int iDownOrUp=0);
	void ComputerFG();
	int ImportXHJTable(CString strXlsFileName);
	void DrawJYJ();
	void ReplaceOld();
	CLKDataTable();
	virtual ~CLKDataTable();

	// 不按比例，按相对位置绘制，由列控数据表调入数据
	int DrawPdt_LKDATA_N(void);
};

#endif // !defined(AFX_LKDATATABLE_H__EBEAF84D_BC20_47CE_8672_DED866A01224__INCLUDED_)
