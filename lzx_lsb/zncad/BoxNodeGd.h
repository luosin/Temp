#pragma once
#include "boxnodebase.h"

class CBoxNodeGd :	public CBoxNodeBase
{
public:
	CBoxNodeGd(void);
	CBoxNodeGd(const CString& strBoxType, const CString& strDevName, const CString& strDevType, long nHandleBox=0, long nHandleDev=0, const CString& strBoxBlock=_T(""), const CString& strDevBlock=_T(""),  const CString& strCableString=_T(""), int iDistToXhl=0) ;

	~CBoxNodeGd(void);
};
