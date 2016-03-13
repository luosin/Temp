#pragma once

class CZnUtility
{
public:
	CZnUtility(void);
	~CZnUtility(void);
	//������pt_3d_base���ĺ��ӣ�����ֵ��0��δ�ҵ���1���ҵ�һ������ߣ������£���>1������
	static int FindConBox(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset=0.1);
	static BOOL IsPointOnRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dHeight, double dWidth, double dOffset=0.1) ;
	static BOOL IsIntersectWithRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_blk, double dHeight, double dWidth, double dOffset=0.1) ;
	static BOOL IsIntersectWithCircle(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dRadius, double dOffset=0.1);
	static BOOL IsIntersectWithLine(const AcGePoint3d& pt_3d_base, const AcDbPolyline* pPolyLine, double dOffset=0.1);

	// ȡ�ö���߶�������
	static int GetVertex(ads_name en, AcGePoint2dArray& pt2dArrayVertex);
	// ȡ�ö���߶�������
	static int GetVertex(const AcDbPolyline* pPolyLine, AcGePoint2dArray& pt2dArrayVertex);
	// ʵ����������,ת�޻�:0,�źŻ�:1,�����·:2
	void GetEntBelongType(long nHandleBelong, int& iType);

	// ����ָ����ʵ�壨0�źŻ���1����2�����·��
	static int GetEnt(CLongArray& nArrayHandle, int iType);
	static int GetJltType(const CString& strNameIntoXhl);
	// ȡ�õ��²���
	static int GetCableParam(double& dCoeff, int& iIntoXhl, int& iPerGd, int& iPerBetweenGd, int& iPerBox, int& iPerBoxF, int& iPerBetweenBoxF);

	// ���ض����������
// 	static CString GetClassType(void* pObject);
};
