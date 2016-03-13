// StationSystem.h: interface for the CStationSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATIONSYSTEM_H__9C6A1F77_41D8_4B73_A7EA_223E9FB80484__INCLUDED_)
#define AFX_STATIONSYSTEM_H__9C6A1F77_41D8_4B73_A7EA_223E9FB80484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YthCircuitData.h"


class CStationSystem  
{
public:
	CString m_strSysBasePath;
	BOOL ComputeOneTypeCircuit2(const CString strBlockName, const AcGePoint3d pt_3d_ref, AcDbDatabase *pCurDb,AcDbDatabase *&pSaveDb, int &iPxCircuitIndex);
	int Compute_18_s_qc_Circuit();
	int Compute_18_s_indoor_Circuit();
	BOOL ComputeOneTypeCircuit(const CString strBlockName, const AcGePoint3d pt_3d_ref,AcDbDatabase *pCurDb, AcDbDatabase *&pSaveDb, int &iPxCircuitIndex);
	int Compute_18_d_qc_Circuit();
	void DoAll(const CString strSavePath);
	BOOL ConvertSpecialChar(const CString strSrc, CString &strRet, int iMode=0);
	void ReplaceSpecialChar();
	void ComputeCircuitForPx(const CString strDwgFile);
	int Compute_18_d_indoor_Circuit();
	int ComputeJlCircuit();
	CString GetXHJType(const CString strXHJName);
	int ComputerSwitchToDb();
	int Compute_18_outdoor_Circuit(int iMode=0);
	int ReadLkQcDataToDb(CString strXlsFileName);
	int ReadZHBztToDB2(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	int ReadYBztToDB2(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	int ComputeDcCircuit();
	int ComputeCzCircuit();
	int ReadQcDataToDb(CString strXlsFileName);
	int ComputeJzCircuit();
	CString m_strDwgNum;
	CString m_strStationName;
	BOOL InsertDwgToDb(const CString strDwgFilePath, AcDbDatabase *&pDb, const AcGeMatrix3d &xform, const CMapStringToString &mapTypeToStr,CString strRepType=_T("TYPE"),int iIdAdd=1000);
	int Compute25HzCircuit();
	CString m_strSavePath;
	void DrawCircuit(CYthCircuitData *circuitdata);
	int ComputeYthCircuit();
	void DrawFxpPx(AcDbDatabase *pCurDb);
	int ReadGBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	int ReadZGBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	int ReadZHBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	int ReadYBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	int FindAllSwitch(CStringArray &strArraySwitch, int iDownOrUp=0);
	int ReadZBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	void ComputeOneDevFxp(CString strDevName, CString strDevType,  int iShelfNum, int iLayerNum, int iBoxNum, int iIndexInBox, int iType);
	void ComputeFxData();
	int ReadFxpBztToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	CStationSystem();
	virtual ~CStationSystem();

	// 原ReplaceSpecialChar函数是将I、V等替换为%%161、%%165，现反之
	int ReplaceSpecialChar2(void);
	// 读入站内组合柜、移频柜、综合柜、轨道柜、分线盘
	static void ReadBzt(void);
};

#endif // !defined(AFX_STATIONSYSTEM_H__9C6A1F77_41D8_4B73_A7EA_223E9FB80484__INCLUDED_)
