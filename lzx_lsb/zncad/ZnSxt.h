#pragma once
#include "../blkuserdll/UserSystem.h"
#include "CableBoxNode.h"
#include "BoxNodeEnt.h"
#include "../tcl/tcl/sequential_tree.h"
#include "XhjDATA.h"
#include "GdDATA.h"
#include "DcDATA.h"
//#include "afxcoll.h"
#include "afxcoll.h"
#include "OperitionExcel.h"
/*
struct XHJATTDATA
{
	XHJATTDATA()
	{
		strName = _T("") ;
		strLoc = _T("") ; 
		bIsLieChe = FALSE ;
		strBoxType = _T("XB1") ; 
		iBoxNum = 1 ; 
		iCoreWireToBoxI = 0 ; 
		iCoreWireToBoxII  = 0 ;
		iBoxInWhere  = 1 ; //default:down
		bHasDdUnit  = TRUE;
		bIsAluminum = TRUE ; 
		bHasPhone  = TRUE ; 
		iDSNum = 2 ; 
		dSpeedLimit = 0 ;
	}
	CString strName ;//信号机名，在块属性中
	CString strLoc ; //公里标，缺省为空
	BOOL bIsLieChe ;//是否列车，缺省为否
	CString strBoxType ; //采用箱盒类型，XB1/XB2/HZ12/HZ24（缺省值为XB1）
	int iBoxNum ; //箱盒类型的数量（缺省值为1）
	int iCoreWireToBoxI ; // 进入箱盒I实际使用芯线数（无缺省值）；注：当为多个箱盒时，需要分别输入（箱盒I、箱盒II…..），不包含灯丝和电话芯线数
	int iCoreWireToBoxII ;
	int iBoxInWhere ; //箱盒位于信号机上0/下1/左2/右3（缺省值为信号机下）
	BOOL bHasDdUnit ;//是否采用点灯单元（缺省值为是）
	BOOL bIsAluminum; //是否为铝合金机构（缺省值为是）
	BOOL bHasPhone ; //是否有电话（缺省值为是）
	int iDSNum ; //灯丝报警芯线数2、3（缺省值为2，有灯丝报警时有此选项）
	double dSpeedLimit ; //限速值（无缺省值，当有限速时有此选项）
};
*/
using namespace  tcl;
using namespace std;

class CZnSxt
{
public:
	CZnSxt(void);
	virtual ~CZnSxt(void);
	int Test(void);
	int  num;
	int  iJLnum;
	//超限时走的轨道
	CLongArray nArrayHandLineCX;
	//超限时对应需要占用的轨道区段
	CStringArray strArrayGuiDaoCX;
	CStringArray str_GuDaoName;
	CStringArray sXGuDaoName;
	CStringArray sSGuDaoName;
	CLongArray nAarryHandleGuDaoS;
	CLongArray nAarryHandleGuDaoX;
	CLongArray nAarryHandleXJC;
	//下行接近进站区段
	CLongArray nAarryHandleXJJC;
	CLongArray nAarryHandleSJC;
	CLongArray nAarryHandleSJJC;
	COperitionExcel oper_excel;
	//查询和pEntSrc相交的多段线，若是定位相交的线，则返回多段线的段数（base 0）,若是反位相交的，返回多段线的顶点序号（base 0）
	//iMode:默认-1表示反位相交情况，iMode=100，表示信号机绝缘节相交情况，iIndexOfVertex需要返回相交的Vertex序号（base 0）;其他表示定位相交，iIndexOfVertex需要返回相交的段数（base 0）
	int GetIntersectPLine(AcDbEntity *pEntSrc, UINT &nHandleIntersectEnt, UINT &nIndexOfVertex, AcGePoint3d &pt_3d_intersect, int &iDwIntersectMoveToVertex, int iMode=-1);
	//查询和pEntSrc(信号机或绝缘节)相交的多段线，返回值：1,1个交点在顶点,顶点所在线在当前信号机或者绝缘节的左侧；5,1个交点在顶点,顶点所在线在当前信号机或者绝缘节的右侧
	int GetIntersectPLine(AcDbEntity *pEntSrc, CLongArray &nArrayHandleIntersect, CLongArray &nArrayIndexOfVertex, AcGePoint3dArray &pt3dArrayIntersect) ;
	// 前期工作：确定道岔中心点，可能需要移动岔后连线的及其上的实体（循环）
	int PreDoGetDcCenter();
	// 取得和DC块定/反位方向连接的线
	BOOL GetDcConLineDFW(const AcDbBlockReference *pRef, CLongArray &nArrayHandleIntersectLine, CLongArray &nArrayIndexOfVertex, AcGePoint3dArray &pt3dArrayIntersect, int &iDwIntersectMoveToVertex);
	// 查找一条FW连线移动引起的所有移动
	void SearchToMoveEnt(UINT nHandleLineFW,  UINT nIndexOfVertex, const AcGePoint2d &pt_2d_rotate, const double dRotateAngle);
	// 读入所有实体到数据库
	int ReadAllEntToDB();
	// 取得信号机、绝缘节所在的线的信息
	BOOL GetXhjJyjConLine(const AcDbBlockReference *pRef, UINT &nHandleIntersectLine, UINT &nIndexOfVertex, AcGePoint3d &pt_3d_intersect);
	// 取得信号机、绝缘节所在的线的信息(可能为两段)
	int GetXhjJyjConLine(const AcDbBlockReference *pRef, CLongArray &nArrayHandleIntersect, CLongArray &nArrayIndexOfVertex, AcGePoint3dArray &pt3dArrayIntersect);
	int DrawGuiDao(void);
	// 绘制双线图
	int DrawSxt(double dScaleX, double dScaleY, const AcGePoint3d & pt_3d_orign, double dLenDcPlus=8.000, double dDistBetweenSX=2.500);
	// 读取各绝缘节、道岔前后关系到relation表
	void FillRelationDB(void);
	// 查找由一个绝缘节起始的一条轨道包含的区段
	void FindOneGuiDao(long nHandleFrom, const int iIndexGuiDao, int iForword=0);
	// 顺序设置极性
	void FillPolar(int iPolarSn, int iDrawOption);
	// 调整图纸
	void AdjustDwg(void);
	// 调整道岔、信号机块的属性，使其符合美观, itype=0，道岔
	void AdjustBlock(AcDbBlockReference*& pRef, int iType=0);

	CADODatabase m_AdoDb;
	CADORecordset m_RecordSet;
	// 将平面图转换为由一段一段区段组成的平面图
	void ConvertPMT(void);
	AcDbDatabase *m_pDb;
	CUserSystem m_mysys;
	void SetAcDbDB(AcDbDatabase * pDb);
	// 找出所有的轨道（根据绝缘节划分的，不是根据送受电的轨道电路，不带DG1之类的）
	void FindAllGuiDao(void);
	// 将所有实体读入数据库（程序转换平面图中）
	void ReadAllEntToDB2(void);
	void ReadAllEntToDB2_DT(void);
	// 循环查询轨道线连接的信号机及道岔，已作为计算轨道名称的依据
	void GetGuiDaoInfo(UINT nLineHandle, CLongArray& nArrayHandleXHJ, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType);
	// 取得轨道电路中最大最小的道岔号
	void GetMaxMinDC(const CLongArray& nArrayHandleDC, int& iMaxDC, int& iMinDC);
	long GetMaxGuiDaoId(void);
	//显示平面图信息（比如轨道电路名称，送受电端）,iType=0,显示所有
	void ShowInfo(int iType=0);
	// 增加直弯股切割绝缘节
	int AddCutJYJ(void);
	// 清除属于道岔（nHandleBelong）的切割绝缘节
	int DeleteCutJYJ(long nHandleBelong);
	// 给指定道岔添加切割绝缘节
	int AddCutJYJ(long nHandleDC);
	// 在sxt_line表中增加切割绝缘节对应的节点
	int AddVertexOfCutJYJ(void);
	// 从一条特定的线开始，递归推出后续连接的所有极性（一条多段线上可能因为切割绝缘节有多个极性）,bForward=true：向右推进，反之向左, iStartVertex，从哪个顶点开始推导，iTestMode=0，仅提示极性冲突段，1，显示极性线条，2，显示动画
	void FillPolar2(long nHandleLine, int iPolar, int iStartVertex, bool bForward=true, int iTestMode=0);
	// 取得上下行分界的y坐标，以上为下行，以下为上行
	BOOL GetBorder(double &dBorderY, double &dBorderX);
	// 计算实体是上行还是下行
	void SetEntUpOrDown(void);
	// 找出交叉渡线并更新数据库，算法是：一个绝缘节2端连接线都连接一个道岔反位就是渡线绝缘节(后又增加端头线)
	void FindCrossLineAndDtLine(void);
	// 绝缘节距信号楼距离（和信号机并置的取信号机的值，其他取设定的值的已在读入实体到数据库时处理）,更新信号机、道岔距信号楼距离（左侧为负数）
	void UpdateToXhlDist(void) ;
	int ClearShowInfo(const CString& strToClearEntApp);
	// 删除当前图纸指定的扩展数据
	int DeleteXdata(const CStringArray& strArrayAppName);
	// 绘制跳线
	void DrawJumpLine(void);
	//取得 CloneFrom=nCloneFromHandle,UPDOWN=iUpOrDwon 的多段线
	long GetPLine(long nCloneFromHandle, int iUpOrDwon);
	// 绝缘节所在线的vec
	AcGeVector2d GetAngleOfJyjOnLine(long nHandleJyj);
	// 双线图中上下2个绝缘节
	BOOL GetPtOfSxtJyj(long nHandleJyj, AcGePoint3d& pt_3d_up, AcGePoint3d& pt_3d_down);
	// 取得指定绝缘节两侧的轨道
	int GetGdBesideJyj(long nHandleJyj, long& nHandleLeftLine, long& nHandleRightLine);
	// 取得两线间的信号机在sxt中的copy
	int GetXhjInSxt(long nHandleLeftLine, long nHandleRightLine, long& nHandleXhj);

	// 绘制指定道岔的转辙机
	void DrawZzj(long nHandleDc);
	// 清除指定道岔的转辙机
	void ClearZzj(long nHandleDc);
	// 绘制所有道岔的转辙机
	void DrawZzj(void);
	// 取得芯轨绘制中心点
	int GetXgInsertPt(long nHandleDc, int iDcDirectV, AcGePoint3d &pt_3d_ret) ;
	
	// 绘制指定送受电、厄流图形
	void DrawBoxFsE(long nHandleJyj);
	// 绘制所有送受电、厄流
	void DrawBoxFsE(void);
	// 清除指定的送受电、厄流图形
	void ClearBoxFsE(long nHandleJyj);

	// 绘制信号机箱盒
	void DrawBoxXhj(long nHandleXhj);
	// 绘制所有信号机盒子
	void DrawBoxXhj(void);
	// 清除指定的信号机的箱盒图形
// 	void ClearBoxXhj(long nHandleXhj);

	// 绘制ZZJ箱盒
	void DrawBoxZzj(AcDbBlockReference* pRefJxg, const CDcDATA* pDataDc, int iQyIndex, int iDcDirectH);
	// 绘制所有ZZJ盒子
	//void DrawBoxZzj(void);
	// 清除指定的ZZJ的箱盒图形
	//void ClearBoxZzj(long nHandleDc);

	// 取得clone信号机
	int GetCloneXhj(long nHandleXhj, long& nHandleCloneXhj);

	// 计算并绘制电缆信息 iJigType：0，箱盒之间的电缆；1，信号楼主电缆；2，起始连接盒子，末端连在信号楼主电缆
	void ComputeCableInfo(long nHandleBoxStart, long nHandleBoxEnd, long nHandleCable, int iJigType=0);
	// 从信号楼出来的主电缆，各分向盒出来的电缆连上此电缆等同于连到信号楼
	void ComputeCableInfo(long nHandleCable);

	// 通过电缆多线段的handle取得其连接字符串,iType=2时是电缆name,=3时是电缆编号
	int GetCableString(long nHandleCableLine, long& nHandleCableString,  int iType=0);
	// 获取盒子距信号楼距离
	int GetBoxDistToXhl(long nHandleBox);

	// 电缆穿过的轨道
	int GetCrossGd(long nHandleCable, CLongArray& nArrayHandleGd);
	// 设置电缆字符串
	int SetCableString(long nHandleCableLine, long nHandleCableString, CString& strCableString,  int iStringType=0);
	// 设置电缆名称,只有如楼端有
	int SetCableName(long nHandleCableLine, long nHandleCableName, CString& strCableName);
	// 读取径路图到数据库
	void ReadJltToDb(void);
	int GetCableName(long nHandleCable, CString& strCableName);
	
	// 循环查询一条线所连接的盒子(从信号楼往末端查询,需要保证在绘径路图电缆时始终从末端往信号楼方向)
	void FindBox(long nHandlePline, long nHandleNextBox, sequential_tree<CBoxNodeEnt>* pSeqTree);

	// 循环查询一条线所连接的盒子(从信号楼往末端查询,需要保证在绘径路图电缆时始终从末端往信号楼方向), 20150728改：默认（从根节点往子节点搜）iDirect=0，表示本段线搜寻方向为从末端向始端
	void FindBox(long nHandlePline, long nHandleNextBox, long nHandleBelongXhl, CString& strBranchIndex);

	// 查找盒子的其他连接电缆
	int FindCable(long nHandleBox, long nHandlePrePline, CLongArray& nArrayHandleFind);
	// 绘制电缆网络图
	void DrawCableNet(void);
	double GetBoxWidth(const CString& strBoxType);
	double GetBoxHeight(const CString& strBoxType);
// 	// 默认尖轨数
// 	int m_iJgNum;
// 	// 默认芯轨数量
// 	int m_iXgNum;
	// 编辑实体（信号机、道岔、轨道电路）数据
	void ModEntData(void);
	// 查找pt_3d_base所在位置的实体
	 int FindEnt(const AcGePoint3d& pt_3d_pick, const void* strDxfCode, const void* strAppName, CLongArray& nArrayHandle);
	 // 计算极性时遇到冲突，退出标志
	 bool m_bFillPolarErro;
	 // 计算指定盒子引入电缆总芯数
	 int GetCableCoreNum(sequential_tree<CBoxNodeEnt>& cableBoxTree, CString& strCableTypeName, int& iBackup, int iDist);

	 BOOL GetBoxInfo(long nHandleBox,CBoxNodeEnt& boxNode);

	 BOOL GetBoxInfo_XHJ(AcDbEntity* pEnt, long nHandleBoxBelong, int &iBoxSumCurXhj, int &iBoxIndexCurXhj, CString & strDevName, CString & strBlkNameDev, CString & strDevType);
	 // 取得一个设备的芯线
	 int GetDevCore(CBoxNodeEnt& boxNode/*const CString& strDevType, const CString& strDevName, const CString& strFreq, vector<CCableCore>* pVec*/, int iDist = 0);
	 // 查找满足条件的电缆型号, iDevType=-1：通用（混合），0：道岔，1，信号机，2：轨道电路；iCableType=0,普通综合扭绞（数字）电缆，1，内屏蔽数字电缆
	 BOOL GetUseCableType(int iUseCoreNum, int iNumNeed4PGroup, int& iBackupNum, CString& strCableTypeName, int iDevType=0, int iCableType=0);
	 // 当前绘制/编辑的电缆径路类型，-1：通用（混合），0：道岔，1，信号机，2：轨道电路，3：电码化
	 int m_iCurDljlType;
	 // 检查并顺正、合并轨道线（两头无任何连接的标示出来）
	 void CheckGdLine(void);
	 // 当前图纸所有道岔
	 int GetAllDc(CLongArray& nArrayHandleDc);
	 // 在相应的线段上绘制电缆信息字符串, iType=0是电缆芯线长度、类型、备用，=1是编号（从SetCableString分离出来，为复用）
	 int DrawCableString(long nHandleCableLine, const CString& strCableString,  int iStringType=0 );
	 // 找出属于指定handle的实体，并递归属于他的所有实体, iType=0不递归，只找一层
	 void FindEntBelong(long nHandle, CLongArray& nArrayHandleBelong, int iType=0);
	 // 取得信号机/道岔/轨道属性数据
	 void GetEntData(AcDbEntity* pEnt, CEntDATA& entData);
	 void GetEntData(long nHandleDc, CEntDATA& entData);
	 // 设置信号机/道岔/轨道属性数据
	 void SetEntData(AcDbEntity* pEnt, CEntDATA* pEntData);
	 void SetEntData(long nHandleDc, CEntDATA* pEntData);

	 // 清除BELONG_ENT属性为nHandleBelong的实体, iType=0不递归，只找一层
	 int ClearEnt(long nHandleBelong, int iType=0);
	 //更新关联道岔属性,iMode=1时，是删除关联（将strConDc的关联道岔删除strDc）
	 int UpdataConDc(const CString& strDc, const CString& strConDc, int iMode=0);
	 // 查找指定名称的道岔
	 int FindDc(const CString& strName , CLongArray& nArrayHandle);
	 // 道岔几动，共几动（此处的几动是对于网络图而言，非实际）
	 int GetDcActionInfo(const CString& strQyType, int iActionNum, int iActionIndex, CString& strActionInfo);
	 // 当前绘制应该使用颜色
	 int m_iColorIndexCur;
	 // 根据当前径路类型，设置其所在层
	 int SetLayer(const AcDbObjectId& objId, int iType);
	 int SetLayer(long nHandle, int iType);
	 // 显示轨道名称（辅助信息）
	 int ShowGdName(AcDbEntity* pEnt);
	 // 显示轨道送受电（辅助信息）
	 int ShowGdFS(AcDbEntity* pEnt);
	 // 显示隐藏道岔关联的切割绝缘节
	 int ShowCutJYJ(long nHandle, bool bShow);
	 // 显示隐藏所有切割绝缘节
	 int ShowCutJYJ(bool bShow);
	 int Test2(void);
	 // 填充电缆芯线定义数据表
	 int FillDefineCoreDetail(void);
	 // 更新送受电盒子
	 int UpdateFsBox(AcDbEntity* pEnt);
	 // 更新电码化电盒子
	 int UpdateDmhBox(AcDbEntity* pEnt) ;
	 // 绘制指定绝缘节两端的电码化箱盒
	 void DrawBoxDMH(long nHandleJyj);
	 void DrawBoxDMH();
	 // 清除指定的绝缘节两侧电码化箱盒
	 int ClearBoxDMH(long nHandleJyj);
	 //取得所有信号楼电缆径路和电缆径路
	 int GetCable(CLongArray (&nArrayHandle)[2]);
	 int ModEntData(int iType);

	 // 在遍历一条径路时如楼端名称
	 CString m_strCurNameIntoXhl;
	 CStringArray m_strArrayCableSn;
	 vector< vector<int > > m_vecCableData ;
	 // 当前正在进行的工作，1：导出需要审核的电缆信息
	 int m_iCurJob;
	 int GetCableParam(double& dCoeff,  vector<int >  &vecCableParam);
	 CStringArray m_strArrayCableAll;
	 // 查找辅助显示的轨道名称
	 int FindShowGdName(AcDbEntity* pEntGd, CLongArray& nArrayHandle);
	 // 以绝缘节或信号机基点为中心构造一个小圆来求和其相交的多段线（左右轨道）
	 int GetIntersectPLine2(AcDbEntity *pEntSrc, CLongArray &nArrayHandleIntersect, CLongArray &nArrayIndexOfVertex, AcGePoint3dArray &pt3dArrayIntersect);
	 // 取得属于指定道岔的多段线（在双线中）
	 int GetSxBelongDc(long nHandleDc, long &nHandlePline);
	 	 int m_i4Or2Line;
	 // 当前盒子对应的ZZJ类型，直接对应define_dev
	 int GetZzjDevType(long nHandleCurBox, CString& strZzjDevType);
	 // 更新双线图中轨道名称(仅更新已有，若没有则不绘制)
	 int UpdateSxtGdName(AcDbEntity* pEnt, const CString strGdName);
	 // 检查重合绝缘节、信号机、漏绝缘节
	 int CheckJyjXhj(void);
	 // 将信号机或绝缘节移至线上
	 void MoveXhjJyj(AcDbBlockReference *pRef) ;

	 // 建立一个电缆径路数（节点是箱盒）
	 void BuildCableBoxTree(long nHandleBox, long nHandleCable, sequential_tree<CBoxNodeEnt>& cableTree);
	 // 重新计算转辙机电缆
	 int ReComputeZzjCable(long nHandleCableIntoXhl);
	 void LoopZzj(sequential_tree<CBoxNodeEnt>* pTree, CString& strPreDcName, vector<sequential_tree<CBoxNodeEnt>* >& vec);
	 void SetZzjDevType(vector<sequential_tree<CBoxNodeEnt>* >& vec);
	 int DistanceToXhl(sequential_tree<CBoxNodeEnt>* pTree) ;

	 // 0:2灯丝；1:3灯丝
	 int m_iDsNumXhj;
	 // 0:2芯QK;1:4芯QK
	 int m_iQkNum;
	 // 调车灯丝
	 int m_iDcDsNum;

	 // 距离栏行高
	 double m_dRowHeightToXhl;

	 //取得信号机、道岔到信号楼的距离
	 int GetDistToXhl(const AcGePoint3d& pt_3d_toxhlheader, vector<vector<vector<CString> > >& vecSection) ;
	 //取得地铁各设备的公里标
	 int GetDistToXhl_DT(const AcGePoint3d& pt_3d_toxhlheader1, const AcGePoint3d& pt_3d_toxhlheader2, vector<vector<vector<CString> > >& vecSection) ;
	 // 取得平面图距离信号楼标题栏块的基点
	 	 int GetToXhlHeaderPos(AcGePoint3d& pt_3d_toxhlheader1, AcGePoint3d& pt_3d_toxhlheader2) ;
	 // 用当前面板的距离更新图上的距离（仅平面图）
	 int ApplyDistToXhlToDwg(vector<vector<vector<CString> > > vecSection);
	 // 距离栏文本高度（文本竖直放，宽）
	 double m_dHeightOfDistText;
	 double m_dHeightOfDistText_DT;
	 double m_dMaxLenPmt;
	 // 绘制双线图（地铁）
	 int DrawSxt_DT(double dScaleX, double dScaleY, const AcGePoint3d & pt_3d_orign, double dLenDcPlus=8.000, double dDistBetweenSX=2.500);
    //luo :
	 // 当前图纸所有道岔
//	 int GetAllDc(CLongArray& nArrayHandleDc);
	 int GetAll_LJ_XHJ(CLongArray& nArrayHandleDc);
	 int GetSomeBlocks(CString name_left2,CLongArray& nArrayHandleDc);
// 	 //依据信号楼坐标来区分信号机上下咽喉
	 void GetEntName(long nHandle,CString &Entname);
	 void GetHandleByName(CString blkname,long &nHandle);
	 //void FindNextLine( long nHandleLine,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 // void FindNextLine( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 //查找轨道区段后继	
	 void FindNextLine( long nHandleLineStart,long nHandleLineEnd,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void LsbTest();
	 void LSBExcel();
	 void XlsToLSB(AcGePoint3d start_pt_3d,float f_length_xls);
	 // void GetPathGuiDao( long nHandleLineStart,long nHandleLineEnd,vector<CString> &sVectorPath, CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 //void GetPathGuiDao( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void LsbTest2();
	 void TestExcel();
	 void PrintGuoDaocha(vector<CString>PathDaocha);
	 // void FindNextLine2( long nHandleLineStart,long nHandleLineEnd,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 //void FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,CLongArray &nArrayHandleXHJ,bool bForword/*=true*/ );
	 void FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void FindNextLine3( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray& nArrayZhiXHJ,CLongArray& nArrayXiangXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 void GetPathGuiDao2( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 BOOL GetAnotherDuXianDaoCha(long nHandleDc_One,long &nHandleDc_Another);
	 void  FindNextDiaoCheXHJ(long nHandleDC_XHJ,CLongArray& nArrayHandleNext_DC_XHJ);
	 long GetDuXianMinDaoCha( long nHandleDc_One,long nHandleDc_Another );
	 long GetDuXianMaxDaoCha( long nHandleDc_One,long nHandleDc_Another );
	 BOOL GetJiaoChaDuXianAnotherDaoChas( long nHandleDc_One,long nHandleDc_Another,long &nHandleDc_Min,long &nHandleDc_Max );
	 BOOL GetDaidongDaochas(long nHandleDC,long &nHandleDc_DdMin,long &nHandleDc_DdMax);
	 BOOL GetDaidongDaochas( long nHandleDC,CString &str_daidongdaochas);
	 void GetGuiDaoInfoOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleXHJ, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType);
	 void GetGuiDaoInfoOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleXHJ, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType);
	 void FindNextLine4( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &PassDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 // void GetPathGuiDao4( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void GetPathGuiDao4( long nHandleLineStart,long nHandleLineEnd,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &DaochaNum,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 //FindNextLine5专门用于查找某轨道区段的后继
	 //void FindNextLine5( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 BOOL GetDaidongDaochas1( long nHandleDC,long &nHandleDc_DdMin,long &nHandleDc_DdMax );
	 BOOL GetDaidongDaochas1( long nHandleDC, CString &str_daidongdaochas);
	 void GetPathGuiDao3( long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 BOOL GetBlockToward( long nHandleXHJ ,int &iToward);
	 // void GetGuiDaoDuanXHJ(bool conline_left,UINT nLineHandle, CLongArray& nArrayHandleXHJ);
	 void GetGuiDaoDuanXHJ(bool bforward,UINT nLineHandle, long& ZhiXHJ,long& XiangXHJ);
	 void GetGuiDaoDuanXHJ(UINT nLineHandle, bool bToward,CLongArray& nArrayXHJ);
	 // void GetGuiDaoDuanXHJ(UINT nLineHandle, CLongArray& nArrayXHJ);
	// void FindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 void GetGuiDaoXHJ(UINT nLineHandle, CLongArray& nArrayXHJ);
	// void GetBlockPosition_X( long nHandle,double &point_x );
	 BOOL GetBlockPosition_X( long nHandle,double &point_x );
	 // void WriteDataToExcel(int iNum_sheet,int iNum_Column,int iNum_Rows,vector<CString> vec_str);
	 // int WriteDataToExcel();
	 int WriteDataToExcel(int iNum_Sheet,int iNum_Row,int iNum_Column,vector<CString>vec_str);
	 int WriteDataToExcel(int iNum_Sheet,int iNum_Row,int iNum_Column,CString str_Data);
	 int WriteDataToExcel(int iNum_Sheet,int iNum_Row,int iNum_Column,int iData);
	 // int GetNextDCXHJByDCXHJ(const long nHandleDC,CLongArray nArrayHandleNextDC);
	// int GetNextDCXHJByDCXHJ( const long nHandleDC,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& strArrayDC );
	 int GetNextDCXHJByDCXHJ( const long nHandleDC,CStringArray& Gudao,CStringArray& JieJinQuDuan,CLongArray& nArrayHandleNextLine,CStringArray& strArrayDC );
	// int GetNextDCXHJByDCXHJ( const long nHandleDC,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& strArrayDC );
	 //根据调车信号机所在的轨道线获取后继调车终端，为递归调用函数
	 int FindNextDCByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CStringArray& JieJinQuDuan,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDCZhong,bool bfoward );
	 //在GetPathGuiDao3()的基础上，增加找出在进路上的所有信号机的功能
	 void GetPathGuiDaoAndXHJ( long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void TiquGuoDaocha( vector<CString>PathDaocha, vector<CString>& vec_Daocha);
	 void GetLieChePathGuiDaoAndXHJ( long nHandleStartXHJ,long nHandleEndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void GetLieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 void GetLieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	// void GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,int &iJL_Cnt,bool bForword/*=true*/ );
	 void GetDiaoChePathGuiDaoAndXHJ( CString& StartXHJ,CString& EndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void TiquEffectiveData( vector<CString>& vec_SourceData, CStringArray& strArray_Data);
	 void DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 //增加到下一条区段时，因存在超限需占用的轨道区段
	 void DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray&strCXGuiDaoName, bool bForword/*=true*/ );
	 void DiaoCheFindNextLineAndDaocha( long nHandleLineStart,long nHandleLineEnd,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 // void DiaoCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 void LieCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray &strCXGuiDaoName,bool bForword/*=true*/ );
	 void FindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 void GetDiaoChePathGuiDaoAndDiDuiXHJ( CString& StartXHJ,CString& EndXHJ,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,bool bForword/*=true*/ );
	 //int FindNextDuiXiangDCByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CStringArray& JieJinQuDuan,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDiDuiDC,bool bfoward );
	 int FindNextDuiXiangDCByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDiDuiDC,bool bfoward );
	 void GetEntBlkNameByHandle( const long nHandle,CString &str_ent_name);
	 //void GetEntBlkNameByHandle( CString& str_ent_blk_name,long nHandle );
	 int FindNextLieCheXHJByLine( long nHandleLine,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& nArrayLieChe,bool bfoward );
	 int GetXHJ_ZuoCe_GdLine( long nHandleDC,long &nHandleLine,bool &foward);
	 int GetXHJ_YouCe_GdLine( long nHandleDC,long &nHandleLine,bool &foward);
	 int GetXHJ_YouCe_GdLine( long nHandleDC,long &nHandleLine,int &iToward);
	 void GetHandleByEntBlkName( CString str_ent_blk_name,long &nHandle );
	 void GetGuiDaoDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType);
	 void GetGuiDaoChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC, CLongArray& nArrayLineDoneCur, CLongArray& nArrayLineCurType);
	 BOOL FindNextLineByJYJ(long nHandleLineStart,long &nHandleLineNext,bool bForward);
	 BOOL IsOntheHorizonline(long nHandleLineStart,long nHandleLineEnd,bool bForward);
	 void GetOnLineDaoChaOrderByXAsc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	 void GetOnLineChaOrderByXDesc(UINT nLineHandle, CLongArray& nArrayHandleDC);
	 int GetJYJ_ZuoCe_GdLine( long nHandleJYJ,long &nHandleLine);
	 int GetJYJ_YouCe_GdLine( long nHandleJYJ,long &nHandleLine);
	 BOOL GetBlockPosition( long nHandleBlk ,AcGePoint3d& Ptpost);
	 BOOL GetClosedPointToLine(AcGePoint3d& GivePt,AcGePoint3d & DestPt,long nHandleLine,int& upordown);
	 BOOL FindCXGuiDao(const long nHandleCXJYJ,long & nHandleLineConLeft,long& nHandleLineCXLeft,long& nHandleLineConRight,long& nHandleLineCXRight,CString& str_CX_GdName_left,CString &str_CX_GdName_Right);
	 int FindALLChaoXianGuiDao(CLongArray& nArrayHandleLine,CStringArray& strArrayGuiDao);
	 //void GetDiaoChePathGuiDaoAndXHJ2( CString& StartXHJ,CString& EndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX, int &iJL_Cnt,bool bForword/*=true*/ );
	 void GetDiaoChePathGuiDaoAndXHJ2( CString& StartXHJ,CString& EndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,CStringArray &strArrayJieJinquduan,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX, int &iJL_Cnt,bool bSXiaxing,bool bForword/*=true*/ );
	 void DiaoCheFindNextLineAndXHJ2( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray&strCXGuiDaoName,bool bForword/*=true*/ );
	 void IsAddConditionDiaoChePath(long nHandleLineStart,long nHandleLineEnd,CLongArray &nArrayHandleStop,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString>& vec_strGuiDaoCX, vector<CString>& vec_str_tiaojian,int &iJL_Cnt,int &iTJ_Cnt,bool bForword/*=true*/ );
	 void IsAddConditionDiaoChePath(long nHandleLineStart,long nHandleLineEnd,CLongArray &nArrayHandleStop,long LastDaocha,vector<CString>&sDaocha,CStringArray &GuDao,CLongArray&nArrayHandleLine, vector<CString>& vec_str_tiaojian,int &iJL_Cnt,int &iTJ_Cnt,bool bForword/*=true*/ );
	 int FindNextDuiXiangDCByLine2( long nHandleLine,long LastDaocha,CStringArray& Gudao,CLongArray& nArrayHandleNextLine,CStringArray& nArrayDiDuiDC,CStringArray &strArrayDaocha,bool bfoward );
	 void TiquFirstDaocha( CString strDaochas, CString &Daocha);
	 void TiquFirstDaocha2( CString strDaochas, CString &Daocha);
	// void DiaoCheFindNextLineAndXHJAndDaocha( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CString &strTiaojianDaocha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,bool bForword/*=true*/ );
	 BOOL FindNextZhiGuidao(long nHandleStartLine,long &nHandleNextLine,bool bForward);
	 //void GetJieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void GetJieChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void GetFaChePathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void TranslateCStringtoVector( CString strDaocha, vector<CString> &vec_strData);
	// void FindFirstDiffString(vector<CString>&vec_str1,vector<CString>&vec_str2,CString &str_diff);
	 void FindFirstDiffString( vector<CString>&vec_str1,vector<CString>&vec_str2,CString &str_diff1,CString&str_diff2 );
	 void GetTongGuoPathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 int GetAll_PMT_XBD( CLongArray& nArrayHandleXBD );
	 void Get_Block_Online( long nHandle_blk,long &nHandleLine );
	 void GetYanXuPathGuiDaoAndXHJ( CString& Start_Gd_name,long nHandleStartXHJ,long nHandleEndXHJ,long& nHandleLineFirst,long nHandleLineStart,long nHandleLineEnd,long LastDaocha,vector<CString>&sVectorPath,vector<CString>&sDaocha, vector<CString>& sXHJ,CStringArray &GuDao,CLongArray&nArrayHandleLine,vector<CString> &vec_strGuiDaoCX,int &iJL_Cnt,bool bForword/*=true*/ );
	 void TranslateVectorToCString(vector<CString> &vec_strData,CString &str_Data);
	 void DeleteSameData( vector<CString>& vec_SourceData, vector<CString>& str_Vec_Dest_Data);
	 void FaCheFindNextLineAndXHJ( long nHandleLineStart,long nHandleLineEnd,CStringArray &GuoDaoCha,CStringArray &Gudao,long LastDaocha,CLongArray& nArrayHandleThisDaocha,CStringArray& GuoXHJ,CLongArray& nArrayHandleLine,CStringArray &strCXGuiDaoName,bool bForword/*=true*/ );
	 void CombandXHJData( vector<CString>& vec_SourceData, vector<CString>& str_Vec_Dest_Data);
	 BOOL SelectFilePath(CString &strFilePath);
	 };
	
