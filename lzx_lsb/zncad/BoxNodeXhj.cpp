#include "boxnodexhj.h"

CBoxNodeXhj::CBoxNodeXhj(void)
: m_iBoxSum(0)
, m_iBoxIndex(0)
{
}

CBoxNodeXhj::CBoxNodeXhj(const CString& strBoxType, const CString& strDevName, const CString& strDevType, int iBoxSum,  int iBoxIndex, long nHandleBox, long nHandleDev, const CString& strBoxBlock, const CString& strDevBlock,  const CString& strCableString, int iDistToXhl)
:CBoxNodeBase(strBoxType, strDevName, strDevType, nHandleBox, nHandleDev, strBoxBlock, strDevBlock,  strCableString, iDistToXhl)
, m_iBoxSum(iBoxSum)
, m_iBoxIndex(iBoxIndex)
{
}

CBoxNodeXhj::~CBoxNodeXhj(void)
{
}
