#pragma once
#include "afxcoll.h"

class CMaXuBiaoForBT
{
public:
	CMaXuBiaoForBT(void);
	~CMaXuBiaoForBT(void);
	// 导入用以绘制码序表示意图的轨道等信息表格
	int ImportGdTable(const CString strXlsFileName);
	// 绘制巴铁码序表平面图
	void DrawBtMxbPmt(void);
	// 根据输入的参数创建报警器的块
	BOOL CreateBjqBlock(int iLxNum, int iUpOrDown);
	AcDbDatabase *m_pCurDb;
	CString m_strSysBasePath;
	// 在数据库中填充bt_mxb_import_belongxhj字段
	int SetBelongXhj(void);
	CStringList m_strListMaXu;
	CStringList m_strListMaXuRecForLoop;
	CStringList m_strListMaXuRecForMain;
	CStringList m_strListMaXuRecForThrough;
	// 区间编码
	void JinLuBianMa_QJ(int iUpOrDown=0);
	BOOL GetGuiDaoBianMa_QJ(int iGuiDaoId, CString & strRetBianMa, int iCodeType=0, int iUpOrDown=0);
	void DrawBianMaTable(CArray<CStringArray *, CStringArray *> & arrayBianMa, const CStringArray &strArrayOCPGuiDao, const CStringArray &strArrayDescribe, int iUpOrDown=0);
	CString m_strStation;
	// 对于区间码序表，右侧的车站名称
	CString m_strStation2;
	// 计算当某一轨道占用时所有轨道发码情况
	int GetGuiDaoBianMaOCP(int iGuiDaoIdOCP, const CString strBelongXhjOCP, const CString strLxZnBj, int &iGuiDaoIdLeftStart, int &iGuiDaoIdLeftEnd, int iType=0, int iUpOrDown=0);

	int m_iOCPGuiDaoId;
	void JinLuBianMa_ZN(int iUpOrDown);
	BOOL GetGuiDaoBianMa_ZN(int iGuiDaoId, CString & strRetBianMa, int iCodeType=0, int iType=0, int iUpOrDown=0);
	CStringList m_strListMaXuRecForMain_Zn;
	CString m_strSavePath;
};
