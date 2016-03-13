// CableNetForKZ.h: interface for the CCableNetForKZ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CABLENETFORKZ_H__433ABF4C_C0F1_4AF7_AFCC_9F116604C984__INCLUDED_)
#define AFX_CABLENETFORKZ_H__433ABF4C_C0F1_4AF7_AFCC_9F116604C984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCableNetForKZ  
{
public:
	CString m_strSysBasePath;
	void ComputeCoreAndDraw2();
	void DrawPx(int iIndex,  int iDwgSum, AcDbDatabase *pCurDb, long iXhlHandle, double dCurTotalYForOneDwg);
	int m_iDrawMode;
	void ComputeFxData(long iXhlHandle);
	int ReadFxpDataToDb(CString strXlsFileName);
	void FindNextEnt(long iHandle, int iForwardToLeftOrRight);
	int m_iReadCablenetCount;
	int m_iReadCablenetPos;
	CString m_strSavePath;
	void FindBranch(long iDesHandle, int &iBranchNum);
	void DrawPx(CString strDwgFilePath, long lXHLHandle);
	BOOL ReadAndFillDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	BOOL ComputeCoreAndDraw();
	BOOL FillDB();
	double m_dCellHight;
	double m_dCellWidth;
	void FindOneRoute(long iCurEntHandle, CLongArray & iArrayEntHandle);
	AcDbDatabase* m_pCurDb;
	void DoAll();
// 	BOOL IsStrMatch(const CString strReg, const CString strToCheck);
	BOOL FindCableStrInLine(const AcGeLineSeg3d lin, CString &strCableStr, AcGePoint3d &pt_3d_text);
	BOOL FindBoxBetweenCable(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,  AcDbHandle &entHandle);
// 	BOOL m_bSet_XHL_X;
// 	double m_dXHL_X;
	BOOL ReadConCableToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	BOOL FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strDevType, CString & strDevName);
	BOOL ReadEntToDB(LPUNKNOWN pConnection, AcDbDatabase * pCurDb);

	CCableNetForKZ();
	virtual ~CCableNetForKZ();

};

#endif // !defined(AFX_CABLENETFORKZ_H__433ABF4C_C0F1_4AF7_AFCC_9F116604C984__INCLUDED_)
