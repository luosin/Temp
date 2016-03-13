#include "StdAfx.h"
#include "dcdata.h"

const CString CDcDATA::strDcItem[19] = 
{
	_T("名称"),
	_T("公里标"),
	_T("尖轨牵引点数"),
	_T("尖轨牵引方式"),
	_T("尖轨是否有密贴检查器"),
	_T("尖轨牵引点位于道岔"),
	_T("芯轨牵引点数"),
	_T("芯轨牵引方式"),
	_T("芯轨是否有密贴检查器"),
	_T("芯轨牵引点位于道岔"),
	_T("箱盒类型"),
	_T("是否单动"),
	_T("是否复式交分"),
	_T("多动关联道岔"),
	_T("第几动"),
	_T("是否有电话"),
	_T("是否有道岔缺口报警"),
	_T("限速值"),
	_T("绝缘节直弯股切割")
} ;

CDcDATA::CDcDATA(void)
: m_nLoc(0)
, m_nJG_QyNum(1)
, m_strJG_QyType(_T("ZD6-D"))
, m_bJG_HasMT(FALSE )
, m_strJG_QyPos(_T("下"))
, m_nXG_QyNum(0 )
, m_strXG_QyType(_T("无"))
, m_bXG_HasMT(FALSE)
, m_strXG_QyPos(_T("下"))
, m_strBoxType(_T("HZ24"))
, m_bIsSingleAct(TRUE)
, m_bIsComplex(FALSE)
, m_strRelationDc(_T(""))
, m_nActIndex(0 )
, m_bHasPhone(TRUE )
, m_bHasDCQKBJ(TRUE)
, m_nSpeedLimit (0 )
, m_iCutJYJPos(0 )
{
// 	DWORD iJgNum = 1, iXgNum = 0 ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("JGNUM"), iJgNum) ;
// 	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("XGNUM"), iXgNum) ;
// 	m_nJG_QyNum = iJgNum ;
// 	m_nXG_QyNum = iXgNum ;
}

CDcDATA::~CDcDATA(void)
{
}

// 根据牵引方式得出每一机牵引
int CDcDATA::GetDrag(CStringArray& strArrayDrag, int iJgOrXg)
{
	int iRet = 0 ;
	CString& strQyType = (iJgOrXg==0?m_strJG_QyType:m_strXG_QyType) ;
	int& iQyNum = (iJgOrXg==0?m_nJG_QyNum:m_nXG_QyNum) ;
	if (strQyType==_T("无"))
	{
		iRet = 0 ;
	}
	else if (strQyType==_T("ZD6-D"))
	{
		iRet = 1 ;
		strArrayDrag.Add(_T("ZD6-D")) ;
	}
	else if (strQyType==_T("ZD6-EJ"))
	{
		iRet = 2 ;
		strArrayDrag.Add(_T("ZD6-E")) ;
		strArrayDrag.Add(_T("ZD6-J")) ;
	}
	else if (strQyType==_T("ZD6-EJJ"))
	{
		iRet = 3 ;
		strArrayDrag.Add(_T("ZD6-E")) ;
		strArrayDrag.Add(_T("ZD6-J")) ;
		strArrayDrag.Add(_T("ZD6-J")) ;
	}
	else if (strQyType==_T("ZYJ7+SH6"))
	{
		iRet = 2 ;
		strArrayDrag.Add(_T("ZYJ7")) ;
		strArrayDrag.Add(_T("SH6")) ;
	}
	else if (strQyType==_T("ZYJ7+SH6+SH6"))
	{
		iRet = 3 ;
		strArrayDrag.Add(_T("ZYJ7")) ;
		strArrayDrag.Add(_T("SH6")) ;
		strArrayDrag.Add(_T("SH6")) ;
	}
	else	if (strQyType==_T("ZYJ7")||strQyType==_T("S700K"))
	{
		iRet = iQyNum ;
		for (int i=0; i<iRet; i++)
		{
			strArrayDrag.Add(strQyType) ;
		}
	}

	return iRet;
}

int CDcDATA::GetDrag(const CString& strQyType)
{
	int iRet = 0 ;

	if (strQyType==_T("无"))
	{
		iRet = 0 ;
	}
	else if (strQyType==_T("ZD6-D"))
	{
		iRet = 1 ;
	}
	else if (strQyType==_T("ZD6-EJ"))
	{
		iRet = 2 ;
	}
	else if (strQyType==_T("ZD6-EJJ"))
	{
		iRet = 3 ;
	}
	else if (strQyType==_T("ZYJ7+SH6"))
	{
		iRet = 2 ;
	}
	else if (strQyType==_T("ZYJ7+SH6+SH6"))
	{
		iRet = 3 ;
	}
	else 	if (strQyType==_T("ZYJ7")||strQyType==_T("S700K"))
	{
		iRet = -1 ;
	}

	return iRet;
}

// 共几动
int CDcDATA::GetActionNum(void)
{
	CStringArray strArrayConDc ;
	CBlkUtility::DivideString(m_strRelationDc, _T(","), strArrayConDc) ;	
	return strArrayConDc.GetCount()+1 ;
}
