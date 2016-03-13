#include "StdAfx.h"
#include "entdata.h"

CEntDATA::CEntDATA(void)
: m_strName(_T(""))
, m_nHandle(0)
, m_iDistToXhl(0)
, m_dX(0)
, m_dY(0)
, m_iEntType(0)
{
}

CEntDATA::~CEntDATA(void)
{
}

CString CEntDATA::GetClassType()
{
	const char *szClassName = typeid(*this).name() ;
	CString strClassName ;
	CBlkUtility::ConvMultiToWide(szClassName, strClassName) ;
	strClassName.TrimLeft(_T("class ")) ;
	return strClassName ;
}
