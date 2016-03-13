#include "StdAfx.h"
#include "gddata.h"

const CString CGdDATA::strGdItem[13] = 
{
	_T("名称"), 
		_T("电路类型"), 
		_T("25Hz轨道电路设备"), 
		_T("是否电化"),
		_T("是否电码化"),
		_T("是否4线制"), 
		_T("是否合用变压器箱"), 
		_T("是否1送多受"), 
		_T("是否主区段"), 
		_T("左端送/受电"), 
		_T("右端送/受电"), 
		_T("左端载频"),
		_T("右端载频") 
} ;

CGdDATA::CGdDATA(void)
{
	m_strCircuitType = _T("25Hz") ;
	m_str25HzDev = _T("二元二位") ;
	m_bIsDH = TRUE ;
	m_bIsDMH = TRUE ;
	m_bIs4Line = FALSE ;
	m_bIsShareXB = FALSE ;
	m_bIs1FnJ = FALSE ;
	m_bIsMainSec = TRUE ;
	m_strLeftFOrS = _T("F") ;
	m_strRightFOrS = _T("S") ;
	m_strFreqLeft = _T("") ;
	m_strFreqRight = _T("") ;
}

CGdDATA::~CGdDATA(void)
{
}
