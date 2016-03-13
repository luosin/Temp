// CABLENETFORBT.h: interface for the CCableNetForBT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CABLENETFORBT_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_)
#define AFX_CABLENETFORBT_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCableNetForBT  
{
public:
	int GetAllConVirtualPLine();
	BOOL GetPreviewEnt(long iCurEntHandle, long &iPreviewEntHandle);
	BOOL BatchChangeJYJ(AcDbDatabase *pCurDb);
	CCableNetForBT();
	virtual ~CCableNetForBT();

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
	BOOL FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strDevType, CString & strDevName, int iFindType=0);
	BOOL ReadEntToDB(LPUNKNOWN pConnection, AcDbDatabase * pCurDb);

	int ReplaceWaySideEquipment(void);
	BOOL HasVerifyLine(const AcGePoint3d pt_3d_1,const AcGePoint3d pt_3d_2);
	
	// 处理发送图形(一个多义线,下面一个实体箭头)
	int ReplaceFS(void);
	// //检验是否存在以pt_3d_1-3为顶点的solid实体
	BOOL HasVerifySolid(const AcGePoint3d pt_3d_1, const AcGePoint3d pt_3d_2, CString &strName, int &iFindTxtLeftOrRight);
	// 处理接收图形（一个圆圈，一个十字，下端出圆圈）
	int ReplaceJS(void);
	// 确定圆内是否有十字线且旁边一定区域有文字
	BOOL HasVerifyJS(const AcGePoint3d & pt_3d_center, CString & strName, int &iFindTxtLeftOrRight);
	// 将块的属性文字偏移一定数值，默认iDirect=10，向右
	void TransformAtt(AcDbBlockReference* pRef, double dPlus, int iDirect=10);
	// 处理盒子图形
	int ReplaceHz(void);
	// 绘制信号楼端
	BOOL DrawXhlDz(void);
	// 读入巴铁电缆径路图到数据库
	int ReadBtDljlToDb(void);
	// 遍历所有的盒子,Adesk::UInt32 iPreLineHandle 是传入的直线，即前一个box连入此box的线
	int LoopToFindBox(AcDbBlockReference * pRef, Adesk::UInt32 iPreLineHandle);
	// 取得实体范围的对角pt
	Acad::ErrorStatus GetExtent(const AcDbEntity * pEnt, AcGePoint3d & pt_3d_min, AcGePoint3d & pt_3d_max);
	// 循环直线端点，直到找到下一个box
	BOOL FindNextBox(AcDbLine * &pLine, const AcGePoint3d pt_3d_from, AcDbBlockReference *& pRefPre, CStringArray &strArrayFindText, Adesk::UInt32 &iHandlePreLine);
	
	// 当前信号楼端子的handle
	unsigned long m_iHandleCurXhl;
	// 当前是信号楼左侧0的电缆还是右侧1
	int m_iCurIsLeftOrRight;
	// 取得轨旁设备数据
	int GetGpDev(const AcDbBlockReference * pRefGpBox, CString & strDevName, CString & strDevType, CString & strBoxType);
	// 处理信号机、转辙机、计轴等终端图形文本
	int ReplaceOther(void);
	double m_dStationLocX;
	// 识别信号机图形
	int ReplaceXhj(void);
	// 取得列车信号机类型
	BOOL GetXhjType(const CString strXhjName, CString & strXhjType);
	// for bt
	BOOL FillDB2(void);
	// 在直线端点查找是否有盒子（分线盒或者gpbox）
	BOOL FindBoxAtEndSide(const AcGePoint3d & pt_3d_endside, unsigned long & iFindBoxHandle);
	// 查找端点在pSrcLine上的直线
	int FindConnectLine(const AcDbLine* pSrcLine, CLongArray & iArrayHandleFindLine, AcGePoint3dArray &ptArrayFrom);
	int DelLoopNumXdata(void);
	// 导入用以绘制码序表示意图的轨道等信息表格
	int ImportGdTable(const CString strXlsFileName);
	// 按巴铁需求替换相关字符
	int ReplaceCoreString();
	// 绘制巴铁码序表平面图
	void DrawBtMxbPmt(void);
	// 根据输入的参数创建报警器的块
	BOOL CreateBjqBlock(int iLxNum, int iUpOrDown);
	int ChangeTo2D(void);
	// 发送接收终端盒内的端子1、2替换为D1、D2,11、12替换为27、28
	BOOL ReplaceTie(AcDbDatabase *& pCurDb, const AcGePoint3d & pt_3d_title, int iType);
	BOOL FindZzjType(AcDbDatabase *& pCurDb, const AcGePoint3d & pt_3d_title, CString & strZzjType);
	BOOL DealWithZzjMainCable(AcDbDatabase *& pCurDb, const CString & strLeftOrRight);
	// 查找字符串
	BOOL FindText(AcDbDatabase *& pCurDb,const CString& strDestText, AcGePoint3d & pt_3d_find, const double dXmin, const double dXmax, const double dYmin, const double dYmax, BOOL bReg=FALSE);

	// 针对巴铁工程不同期，0，前期（盒子编号类421），1，后期（盒子编号类T6-4）
	int m_nForProj;
	// 下行线是否绘制在上边，0：是，1：不是
	int m_iDownLineIsUp;
};

#endif // !defined(AFX_CABLENETFORBT_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_)
