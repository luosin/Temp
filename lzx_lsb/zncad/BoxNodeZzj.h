#pragma once
#include "boxnodebase.h"

class CBoxNodeZzj :	public CBoxNodeBase
{
public:
	CBoxNodeZzj(void);
	//strDragType:某1机的型号，比如ZD6-EJ的A机牵引是ZD6-E ；iDragIndex，第几机牵引；m_strDcQyType：尖轨或芯轨的牵引方式（比如ZYJ7+SH6）；
	CBoxNodeZzj(const CString& strBoxType, const CString& strDevName, const CString& strDevType, const CString& strDragType=_T("ZD6-D"), int iDragIndex=1, const CString& m_strDcQyType=_T("ZD6-D"), const CString& m_strActIndex=_T("1/1"), long nIdPlus=0, long nHandleBox=0, long nHandleDev=0, const CString& strBoxBlock=_T(""), const CString& strDevBlock=_T(""),  const CString& strCableString=_T(""), int iDistToXhl=0) ;
	~CBoxNodeZzj(void);
	CString m_strDragType;
	int m_iDragIndex ;
	CString m_strDcQyType;
	CString m_strActIndex ;
	long m_nIdPlus;
	//1：尖轨，2：芯轨
	int m_iJxg;
};
