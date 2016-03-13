// MaXuBiao.h: interface for the CMaXuBiaoForHD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MaXuBiaoForJH_H__53077AA9_C189_485A_AF91_F10D20886185__INCLUDED_)
#define AFX_MaXuBiao_H__53077AA9_C189_485A_AF91_F10D20886185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaXuBiaoForHD 
{
public:
	CMaXuBiaoForHD();
	virtual ~CMaXuBiaoForHD();

public:
	CString m_strDrawNum;
	CString m_strProjName;
	CString m_strBackStation;
	CString m_strFrontStation;
	void FindDivide(const CString strStation, long &iLeftDivideRealLoc, long &iRightDivideRealLoc, int iUpOrDown=0);
	void GetFrontAndBackStation(const CString strCurStation, CString &strFrontStation, CString &strBackStation);
	void DrawBianMaTableForJH(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &strArrayGuiDao, CStringArray &strArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect=0, BOOL bIsFanXiangChuZhan=0) ;
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
	CString m_strFileName;
	CString m_strDwgSaveDir;
	//ads_point m_Test_pt ;
	
	void GetLinZhanStateSum(int &iRet, int iFirstOrLast = 0);
	int m_iLinZhanStateSum;
	int m_iZpGuiDaoSum_Rigth;
	int m_iZpGuiDaoSum_Left;
	CStringArray  m_strArrayGuiDao_ZP_Right;	
	CStringArray  m_strArrayGuiDao_ZP_Left;
	int m_iGuDao_Type;
	CString m_strGuDao;
	int m_iCurJinLuDirect;
	int FindZP(CStringArray &strArrayGuiDao_ZP, int iStart_id, int iEnd_id, int iDirect=0);
	BOOL GetJinZhanOrChuZhan(CString &strJinZhanXHJ, int &iJinZhanXHJ_Id, CString &strChuZhanXHJ, int &iChuZhanXHJ_Id,CString &strJinZhanXHJ_F, int &iJinZhanXHJ_F_Id, CString &strChuZhanXHJ_F, int &iChuZhanXHJ_F_Id, CString &strGuDao, int &iGuDaoType, int iDirect=0);
	CString m_strChuZhanXHJ_F;
	CString m_strChuZhanXHJ;
	CString m_strJinZhanXHJ_F;
	CString m_strJinZhanXHJ;
	int m_iChuZhanXHJ_F_id;
	int m_iChuZhanXHJ_id;
	int m_iJinZhanXHJ_F_id;
	int m_iJinZhanXHJ_id;
	CString m_strCaseType;
	int m_iDwgIndex;
	void FindJinLu(CString strStart, CString strEnd, CString strPassGuiDao, CStringArray &strArray);
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
	void GetJinLuGuiDao(CStringArray &strArrayGuiDao);
	void JinLuBianMa_S_C_S();
	void DrawBianMaTable(CArray<CStringArray *, CStringArray *> &arrayBianMa, const CStringArray &strArrayGuiDao, CStringArray &strArrayDescribe,const CArray<BOOL,BOOL> &arrayBianMaChange, int iWriteDirect=0, BOOL bIsFanXiangChuZhan=0) ;
	double dTableCellHeight;
	double dTableCellWidth;
	BOOL GetJinZhanOrChuZhan(CString &strJinZhan, int &iJinZhanId, CString &strChuZhan, int &iChuZhanId, CString &strGuDao, int &iGuDaoType, CString &strTowardsJinZhanXHJ, int iUpDown=1, int iDirect=0);
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
	BOOL GetGuiDaoBianMa2(CString strGuiDao,CStringArray &strArrayBianMa, CString &strRetBianMa, BOOL &bBianMaChange,int iCodeType=0, int iUpDown=1, int iDirect=0);
	//BOOL GetGuiDaoBianMa(CString strGuiDao,CStringArray &strArrayBianMa, CString &strRetBianMa, int iCodeType=0, int iUpDown=1, int iDirect=0);
	void JinLuBianMa_X_Z_X();
	BOOL IsDirectJinLu();
	void ReadOneGuiDaoToDb(CString strGuiDao);
	BOOL JinLuBianMa(CStringArray * strArrayJinLu);
	CArray<CStringArray * , CStringArray *>  arrayJinLu;
	void FindJinLu(CString strStart, CString strEnd, CStringArray &strArray);
	void ReadDwgToDb(LPUNKNOWN m_pConnection , AcDbDatabase * pCurDb);

	// 关联前后轨道
	static void ConGuiDao(void);
	// 删除轨道关联
	static void DelGuiDaoCon(void);
	// 检查轨道关联
	static void CheckGuiDaoCon(void);
	// 取消显示轨道关联
	static void UnCheckGuiDaoCon(void);
	// 设置轨道频率
	void SetFreq(void);
	// 设置起始轨道频率（根据起始频率和频率布置规则自动计算其他频率）
	void SetStartFreq(void);
};

#endif // !defined(AFX_MaXuBiao_H__53077AA9_C189_485A_AF91_F10D20886185__INCLUDED_)
