#pragma once

class CZnUtility
{
public:
	CZnUtility(void);
	~CZnUtility(void);
	//查找在pt_3d_base处的盒子，返回值：0，未找到，1，找到一个多段线（主电缆），>1，盒子
	static int FindConBox(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset=0.1);
	static BOOL IsPointOnRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dHeight, double dWidth, double dOffset=0.1) ;
	static BOOL IsIntersectWithRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_blk, double dHeight, double dWidth, double dOffset=0.1) ;
	static BOOL IsIntersectWithCircle(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dRadius, double dOffset=0.1);
	static BOOL IsIntersectWithLine(const AcGePoint3d& pt_3d_base, const AcDbPolyline* pPolyLine, double dOffset=0.1);

	// 取得多段线顶点数组
	static int GetVertex(ads_name en, AcGePoint2dArray& pt2dArrayVertex);
	// 取得多段线顶点数组
	static int GetVertex(const AcDbPolyline* pPolyLine, AcGePoint2dArray& pt2dArrayVertex);
	// 实体所属类型,转辙机:0,信号机:1,轨道电路:2
	void GetEntBelongType(long nHandleBelong, int& iType);

	// 返回指定的实体（0信号机、1道岔、2轨道电路）
	static int GetEnt(CLongArray& nArrayHandle, int iType);
	static int GetJltType(const CString& strNameIntoXhl);
	// 取得电缆参数
	static int GetCableParam(double& dCoeff, int& iIntoXhl, int& iPerGd, int& iPerBetweenGd, int& iPerBox, int& iPerBoxF, int& iPerBetweenBoxF);

	// 返回对象的类类型
// 	static CString GetClassType(void* pObject);
};
