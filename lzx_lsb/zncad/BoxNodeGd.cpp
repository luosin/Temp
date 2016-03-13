#include "boxnodegd.h"

CBoxNodeGd::CBoxNodeGd(void)
{
}

CBoxNodeGd::CBoxNodeGd(const CString& strBoxType, const CString& strDevName, const CString& strDevType, long nHandleBox, long nHandleDev, const CString& strBoxBlock, const CString& strDevBlock,  const CString& strCableString, int iDistToXhl)
:CBoxNodeBase(strBoxType, strDevName, strDevType, nHandleBox, nHandleDev, strBoxBlock, strDevBlock,  strCableString, iDistToXhl)
{
}

CBoxNodeGd::~CBoxNodeGd(void)
{
}
