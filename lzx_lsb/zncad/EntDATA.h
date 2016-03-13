#pragma once

class CEntDATA
{
public:
	CEntDATA(void);
	virtual ~CEntDATA(void);
	CString m_strName;
	//CEntDATA(const TCHAR* szName=NULL);
	CString GetClassType();
	// 对应图形实体的handle
	long m_nHandle;
	// 距信号楼距离，仅对道岔和信号机
	int m_iDistToXhl;
	// 在图形上的x坐标
	double m_dX;
	// 在图形上的y坐标
	double m_dY;
	//0：道岔，1，信号机，2：轨道电路
	int m_iEntType;
};
