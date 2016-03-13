#include "StdAfx.h"
#include "cablecore.h"

CCableCore::CCableCore(void)
: m_strDevName(_T(""))
, m_strCoreName(_T(""))
, m_strCoreNameShow(_T(""))
, m_iIsInside(0)
, m_iIsShare(0)
, m_iIsAdd(0)
, m_strFreq(_T(""))
{
}

CCableCore::~CCableCore(void)
{
}

CCableCore::CCableCore(const CString& strDevName, const CString& strCoreName)
: m_strDevName(strDevName)
, m_strCoreName(strCoreName)
, m_strCoreNameShow(strCoreName)
, m_iIsInside(0)
, m_iIsShare(0)
, m_iIsAdd(0)
, m_strFreq(_T(""))
{
}

CCableCore::CCableCore(const CString& strDevName, const CString& strCoreName, const CString& strCoreNameShow, CString strFreq, int iIsInside, int iIsShare, int iIsAdd)
: m_strDevName(strDevName)
, m_strCoreName(strCoreName)
, m_strCoreNameShow(strCoreNameShow)
, m_strFreq(strFreq)
, m_iIsInside(iIsInside)
, m_iIsShare(iIsShare)
, m_iIsAdd(iIsAdd)
{
}

int CCableCore::IsInside(void)
{
	return m_iIsInside;
}

int CCableCore::IsShare(void)
{
	return m_iIsShare;
}

int CCableCore::IsAdd(void)
{
	return m_iIsAdd;
}
bool CCableCore::IsSameCore(const CCableCore& cableCore)
{
	if (this->m_strDevName==cableCore.m_strDevName&&this->m_strCoreName==cableCore.m_strCoreName)
	{
		return true;
	}
	return false;
}
