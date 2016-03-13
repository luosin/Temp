// MaXuBiao.h: interface for the CMaXuBiaoForJH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MaXuBiaoForJH_H__53077AA9_C189_485A_AF91_F10D20886185__INCLUDED_)
#define AFX_MaXuBiao_H__53077AA9_C189_485A_AF91_F10D20886185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaXuBiaoForJH 
{
public:
	CMaXuBiaoForJH();
	virtual ~CMaXuBiaoForJH();

public:
	CString m_strBackStation;
	CString m_strFrontStation;
	void FindDivide(const CString strStation, long &iLeftDivideRealLoc, long &iRightDivideRealLoc, int iUpOrDown=0);
	void GetFrontAndBackStation(const CString strCurStation, CString &strFrontStation, CString &strBackStation);
	void DrawBianMaTableForJH(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &cstrArrayGuiDao, CStringArray &cstrArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect=0, BOOL bIsFanXiangChuZhan=0) ;
	void ReleaseJLArray();
	void MaXuBiaoForJH();
	BOOL ConvertSpecialChar(const CString strSrc, CString &strRet);
	CString m_strSysBasePath;
	void ExportLKQCTable(CString strSaveFileName);
	void ReadDwgToDb2(LPUNKNOWN m_pConnection, AcDbDatabase * pCurDb);
	double	m_dWidthGuiDao;
	double	m_dWidthBianMa;
	double	m_dHeightGuiDao;
	double	m_dHeightBianMa;
	
	CStringArray m_strArrayCurJinLuFre;
	int m_iVersionFor;
	double m_dBetweenTableAndBottom;
	double m_dSiYiTuHeight;
	void GetJinLuGuiDaoFreq(const CStringArray &strArrayGuiDao, CStringArray &strArrayFreq);
	void SetOneGuiDaoFreq(CString strGuiDao, CString strFreq);
	AcDbDatabase * m_pDb;
	void SetGuiDaoFreq(CString &strGuiDao, const CString &strEndGuiDao, int &iIndex, int iUpOrDown=0);
	CString m_strStationName;
	double m_dFrameLeft_x;
	CString m_cstrFileName;
	CString cstrDwgSaveDir;
	//ads_point m_Test_pt ;
	
	void GetLinZhanStateSum(int &iRet, int iFirstOrLast = 0);
	int m_iLinZhanStateSum;
	int m_iZpGuiDaoSum_Rigth;
	int m_iZpGuiDaoSum_Left;
	CStringArray  m_cstrArrayGuiDao_ZP_Right;	
	CStringArray  m_cstrArrayGuiDao_ZP_Left;
	int m_iGuDao_Type;
	CString m_cstrGuDao;
	int m_iCurJinLuDirect;
	int FindZP(CStringArray &cstrArrayGuiDao_ZP, int iStart_id, int iEnd_id, int iDirect=0);
	BOOL GetJinZhanOrChuZhan(CString &cstrJinZhanXHJ, int &iJinZhanXHJ_Id, CString &cstrChuZhanXHJ, int &iChuZhanXHJ_Id,CString &cstrJinZhanXHJ_F, int &iJinZhanXHJ_F_Id, CString &cstrChuZhanXHJ_F, int &iChuZhanXHJ_F_Id, CString &cstrGuDao, int &iGuDaoType, int iDirect=0);
	CString m_cstrChuZhanXHJ_F;
	CString m_cstrChuZhanXHJ;
	CString m_cstrJinZhanXHJ_F;
	CString m_cstrJinZhanXHJ;
	int m_iChuZhanXHJ_F_id;
	int m_iChuZhanXHJ_id;
	int m_iJinZhanXHJ_F_id;
	int m_iJinZhanXHJ_id;
	CString m_cstrCaseType;
	int m_iDwgIndex;
	void FindJinLu(CString cstrStart, CString cstrEnd, CString cstrPassGuiDao, CStringArray &cstrArray);
	void JinLuBianMa_ZJZ_S_F();
	void JinLuBianMa_ZJZ_S();
	void JinLuBianMa_ZJZ_X_F();
	void JinLuBianMa_ZJZ_X();
	void JinLuBianMa_S_ZC_X_F();
	void JinLuBianMa_S_C_S_F();
	void JinLuBianMa_S_Z_S_F();
	void JinLuBianMa_X_ZC_S_F();
	void JinLuBianMa_X_C_X_F();
	void JinLuBianMa_X_Z_X_F();
	void JinLuBianMa_S_ZC_X();
	void JinLuBianMa_X_ZC_S();
	void GetJinLuGuiDao(CStringArray &cstrArrayGuiDao);
	void JinLuBianMa_S_C_S();
	void DrawBianMaTable(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &cstrArrayGuiDao, CStringArray &cstrArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect=0, BOOL bIsFanXiangChuZhan=0) ;
	double dTableCellHeight;
	double dTableCellWidth;
	BOOL GetJinZhanOrChuZhan(CString &cstrJinZhan, int &iJinZhanId, CString &cstrChuZhan, int &iChuZhanId, CString &cstrGuDao, int &iGuDaoType, CString &cstrTowardsJinZhanXHJ, int iUpDown=1, int iDirect=0);
	AcDbObjectId textStyleId;
	AcDbObjectId textStyleId_GuiDao;
	double dTableCur_Y;
	double dTableCur_X;
	double dTableStart_Y;
	double dTableStart_X;
	double dBetweenTable_Y;
	double dDescribeCellWidth ;
	void JinLuBianMa_S_Z_S();
	void JinLuBianMa_X_C_X();
	void DoALL();
	BOOL GetGuiDaoBianMa2(CString cstrGuiDao,CStringArray &cstrArrayBianMa, CString &cstrRetBianMa, BOOL &bBianMaChange,int iCodeType=0, int iUpDown=1, int iDirect=0);
	//BOOL GetGuiDaoBianMa(CString cstrGuiDao,CStringArray &cstrArrayBianMa, CString &cstrRetBianMa, int iCodeType=0, int iUpDown=1, int iDirect=0);
	void JinLuBianMa_X_Z_X();
	BOOL IsDirectJinLu();
	void ReadOneGuiDaoToDb(CString cstrGuiDao);
	BOOL JinLuBianMa(CStringArray * cstrArrayJinLu);
	CArray<CStringArray * , CStringArray *>  arrayJinLu;
	void FindJinLu(CString cstrStart, CString cstrEnd, CStringArray &cstrArray);
	void ReadDwgToDb(LPUNKNOWN m_pConnection , AcDbDatabase * pCurDb);

};

#endif // !defined(AFX_MaXuBiao_H__53077AA9_C189_485A_AF91_F10D20886185__INCLUDED_)
