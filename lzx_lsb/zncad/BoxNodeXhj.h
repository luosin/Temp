#pragma once
#include "boxnodebase.h"

class CBoxNodeXhj :	public CBoxNodeBase
{
public:
	CBoxNodeXhj(void);
	CBoxNodeXhj(const CString& strBoxType, const CString& strDevName, const CString& strDevType, int iBoxSum=1,  int iBoxIndex=1, long nHandleBox=0, long nHandleDev=0, const CString& strBoxBlock=_T(""), const CString& strDevBlock=_T(""),  const CString& strCableString=_T(""), int iDistToXhl=0) ;
	~CBoxNodeXhj(void);
	int m_iBoxSum ;
	int m_iBoxIndex ;
};
