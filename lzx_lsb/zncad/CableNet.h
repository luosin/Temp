// CableNet.h: interface for the CCableNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CABLENET_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_)
#define AFX_CABLENET_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCableNet  
{
public:
	CADODatabase m_AdoDb;
	CADORecordset m_RecordSet;
	CString m_strSysBasePath;

	CCableNet();
	virtual ~CCableNet();

	BOOL GetPreviewEnt(long iCurEntHandle, long &iPreviewEntHandle);
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
	BOOL ReadAndFillDB(AcDbDatabase *pCurDb);
	BOOL ComputeCoreAndDraw();
	BOOL FillDB();
	double m_dCellHight;
	double m_dCellWidth;
	void FindOneRoute(long iCurEntHandle, CLongArray & iArrayEntHandle);
	AcDbDatabase* m_pCurDb;
	void DoAll();
	BOOL FindCableStrInLine(const AcGeLineSeg3d lin, CString &strCableStr, AcGePoint3d &pt_3d_text);
	BOOL FindBoxBetweenCable(AcGePoint3d pt_3d_base,  long &nHandle);
	// 	BOOL m_bSet_XHL_X;
	// 	double m_dXHL_X;
	BOOL ReadConCableToDB(AcDbDatabase *pCurDb);
	BOOL FindDevForBox(AcGePoint3d pt_3d_topleft, AcGePoint3d pt_3d_bottomright,CString &strBlockName, CString & strDevName, int& iHasDs, int iFindType=0);
	BOOL ReadEntToDB(AcDbDatabase * pCurDb);
	// 循环给每个box设置分支编号，信号楼为第一级，编号1，以信号楼为下一个的box按y从大到小编号1-1、1-2、......
	void SetBoxBranchIndex(long nCurBoxHandle, const CString & strCurIndex);
	// 调车信号机设置，0：默认无灯丝，1：有2芯灯丝
	int m_iXhjDcSet;
	// 计算一个如楼端的所有电缆芯线
	void ComputeCoreOfOneXhl(long iXHL_handle_low, double& dCurTotalY);
	// 在读入网络图后计算填充数据库相应的字段（以前是夹杂在读入过程中计算的）
	void ComputeDevAndBoxType(void);
	// 从起始往信号楼方向遍历其相连的块，返回handle
	//若strBoxBlockToFind不为空，则只遍历到不为strBoxBlockToFind的块即终止
	void LoopConnectBox(long nHandle, CLongArray& nArrayHandle,  const CString &strBoxBlockToFind);
	// 共享芯线绘制时带不带设备名称
	BOOL m_bShareWithDevName;
	// 按define_dev_use_cablecore默认的起始、终端端子设置端子（若默认有非空或非0值）
	BOOL m_bDefaultCablecore;
	// 绘制备用信息否
	BOOL m_bDrawBackup;
	// 递归遍历搜寻出一个个的转辙机组合
	void LoopZzj(long nHandle, CString& strPreDcName, CLongArray& nArrayHandleZzj, CStringArray& strArrayZzjType, CStringArray& strArrayZzjIndex);
	// 设置转辙机的组合类型
	void SetZzjDevType(const CLongArray& nArrayHandle, CStringArray& strArrayZzjType, CStringArray& strArrayZzjIndex);
	// 到信号楼的距离
	int DistanceToXhl(long nHandle);
	// 计算从一个盒子伸出的电缆各芯线分配
	int ComputeCoreOfOneBox(long nHandleBox);
	// 电缆网络图到配线图，给一个盒子伸出的电缆分配芯线（i=3表示电码化，默认是其他）
	int ComputeCoreSnDetail(long nHandleBox, long iCableTypeId, int iType = -1);
	// 设置指定type_id，芯线名为strCoreSn及其成对芯的is_used_pair
	int SetCorePairUsed(int iCableTypeId, const CString& strCoreSn);
	int m_iDsNumXhj;
	int m_iQkNum;
	// 调车灯丝
	int m_iDcDsNum;
};

#endif // !defined(AFX_CABLENETFORJY_H__FEFEAEAC_4728_40C0_ADA6_06687B14B8AA__INCLUDED_)
