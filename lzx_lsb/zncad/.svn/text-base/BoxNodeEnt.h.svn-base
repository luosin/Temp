#pragma once
#include "boxnodebase.h"

class CBoxNodeEnt :	public CBoxNodeBase
{
public:
	CBoxNodeEnt(void);
	~CBoxNodeEnt(void);
	CBoxNodeEnt(const CString& strBoxType, const CString& strDevName, const CString& strDevType, int iEntType=-1, long nHandleBox=0, long nHandleDev=0, const CString& strBoxBlock=_T(""), const CString& strDevBlock=_T(""),  const CString& strCableString=_T(""), int iDistToXhl=0 ) ;

	long m_nHandleBox;
	long m_nHandleDev;
	CString m_strBoxBlock;
	CString m_strDevBlock;
	CString m_strCableString;
	int m_iDistToXhl;

	CString m_strFreq ;

	int m_iBoxSum ;
	int m_iBoxIndex ;

	CString m_strDragType;
	int m_iDragIndex ;
	CString m_strDcQyType;
	CString m_strActIndex ;
	long m_nIdPlus;
	//1：尖轨，2：芯轨
	int m_iJxg;

	// 转辙机加芯长度（距信号楼最远端的）
	int m_iLenForPlus;
	//箱盒和next（往信号楼方向）箱盒间的连线
	long m_nHandleCable ;
};
