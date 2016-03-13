// CableNetForJY.h: interface for the CCableNetForJY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CABLENETFORJY_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_)
#define AFX_CABLENETFORJY_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCableNetForJY  
{
public:
	int GetAllConVirtualPLine();
	BOOL GetPreviewEnt(long iCurEntHandle, long &iPreviewEntHandle);
	BOOL BatchChangeJYJ(AcDbDatabase *pCurDb);
	CCableNetForJY();
	virtual ~CCableNetForJY();

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
	BOOL FindCableStrInLine(const AcGeLineSeg3d lin, CString &strCableStr, AcGePoint3d &pt_3d_text);
	BOOL FindBoxBetweenCable(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,  AcDbHandle &entHandle);
	// 	BOOL m_bSet_XHL_X;
	// 	double m_dXHL_X;
	BOOL ReadConCableToDB(LPUNKNOWN pConnection, AcDbDatabase *pCurDb);
	BOOL FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strDevType, CString & strDevName, CString & strDevFor, int iFindType=0);
	BOOL ReadEntToDB(LPUNKNOWN pConnection, AcDbDatabase * pCurDb);


	// ȡ����·�ϵĿ�ʼ�ڵ�
	void GetFirstEnt(long iCurHandle, long & iStartHandle);
	// ѭ����ÿ��box���÷�֧��ţ��ź�¥Ϊ��һ�������1�����ź�¥Ϊ��һ����box��y�Ӵ�С���1-1��1-2��......
	void SetBoxBranchIndex(long nCurBoxHandle, const CString & strCurIndex);
	// �����źŻ����ã�0��Ĭ���޵�˿��1����2о��˿
	int m_iXhjDcSet;
};

#endif // !defined(AFX_CABLENETFORJY_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_)
