#pragma once
#include "StdAfx.h"
#include "CableCore.h"

class CBoxNodeBase
{
public:
	CBoxNodeBase(void);
	virtual ~CBoxNodeBase(void);

	CBoxNodeBase(int iEntType);
	CBoxNodeBase(const CString& strBoxType, const CString& strDevName, const CString& strDevType, int iEntType=-1) ;

	CString m_strBoxType;
	CString m_strDevName;
	CString m_strDevType;
	//-2：信号楼，-1：分向盒，0：道岔，1，信号机，2：轨道电路，3：电码化
	int m_iEntType;

	std::vector<CCableCore> m_vecCableCore ;		

	CString GetClassType();
	// 查询一条芯线是否存在，只要devname和corename相同即认为同一芯
	bool FindCableCore(const CCableCore& cableCore);
};
