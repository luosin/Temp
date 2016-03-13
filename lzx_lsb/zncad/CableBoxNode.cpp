#include "StdAfx.h"
#include "cableboxnode.h"

CCableBoxNode::CCableBoxNode(void)
: m_strBoxType(_T(""))
, m_strDevName(_T(""))
, m_strDevType(_T(""))
, m_strFreq(_T(""))
{
	
}

CCableBoxNode::~CCableBoxNode(void)
{
}

CCableBoxNode::CCableBoxNode(const CString& strBoxType, const CString& strDevName, const CString& strDevType, const CString& strFreq)
: m_strBoxType(strBoxType)
, m_strDevName(strDevName)
,m_strDevType(strDevType)
,m_strFreq(strFreq)
{
}
// 查询一条芯线是否存在，只要devname和corename相同即认为同一芯
bool CCableBoxNode::FindCableCore(const CCableCore& cableCore)
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
