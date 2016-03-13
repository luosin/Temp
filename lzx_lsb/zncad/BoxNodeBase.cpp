#include "boxnodebase.h"

CBoxNodeBase::CBoxNodeBase(void)
: m_strBoxType(_T(""))
, m_strDevName(_T(""))
, m_strDevType(_T(""))
, m_iEntType(-1)
{
}

CBoxNodeBase::~CBoxNodeBase(void)
{
}

CBoxNodeBase::CBoxNodeBase(int iEntType)
: m_iEntType(iEntType)
, m_strBoxType(_T(""))
, m_strDevName(_T(""))
, m_strDevType(_T(""))
{
}

CBoxNodeBase::CBoxNodeBase(const CString& strBoxType, const CString& strDevName, const CString& strDevType, int iEntType)
: m_strBoxType(strBoxType)
, m_strDevName(strDevName)
, m_strDevType(strDevType)
, m_iEntType(iEntType)
{
}

CString CBoxNodeBase::GetClassType()
{
	const char *szClassName = typeid(*this).name() ;
	CString strClassName ;
	CBlkUtility::ConvMultiToWide(szClassName, strClassName) ;
	strClassName.TrimLeft(_T("class ")) ;
	return strClassName ;
}

// 查询一条芯线是否存在，只要devname和corename相同即认为同一芯
bool CBoxNodeBase::FindCableCore(const CCableCore& cableCore)
{
	vector<CCableCore>::iterator it = m_vecCableCore.begin() ;
	for (; it!=m_vecCableCore.end(); ++it)
	{
		if (it->IsSameCore(cableCore))
		{
			return true ;
		}
	}
	return false;
}