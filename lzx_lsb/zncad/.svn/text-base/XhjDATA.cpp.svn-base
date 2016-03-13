#include "StdAfx.h"
#include "xhjdata.h"

const CString CXhjDATA::strXhjItem[12] = {_T("名称"), _T("公里标"), _T("列车"), _T("箱盒类型"), _T("进入箱盒I芯数"), _T("进入箱盒II芯数"), _T("箱盒位于信号机"), _T("采用点灯单元"), _T("铝合金结构"), _T("有电话"), _T("灯丝报警芯数"), _T("限速值") } ;

CXhjDATA::CXhjDATA(void)
: m_strBlkName(_T(""))
, m_nLoc(0) 
, m_bIsTrain(FALSE)
, m_strBoxType(_T("XB1") ) 
, m_nCoreNumI(0) 
, m_nCoreNumII(0) 
, m_strBoxPos(_T("下")) 
, m_bHasDDUnit(TRUE)  
, m_bIsAlu(TRUE)
, m_bHasPhone(TRUE) 
, m_nDSNum(2)
, m_nSpeedLimit(0) 
, m_strBoxTypeOption(_T(""))
{
}

CXhjDATA::CXhjDATA(const CString& strBlkName)
: m_strBlkName(strBlkName)
, m_nLoc(0) 
, m_bIsTrain(FALSE)
, m_strBoxType(_T("XB1") ) 
, m_nCoreNumI(0) 
, m_nCoreNumII(0) 
, m_strBoxPos(_T("下")) 
, m_bHasDDUnit(TRUE)  
, m_bIsAlu(TRUE)
, m_bHasPhone(TRUE) 
, m_nDSNum(2)
, m_nSpeedLimit(0) 
, m_strBoxTypeOption(_T(""))
{
	Init() ;
}

CXhjDATA::~CXhjDATA(void)
{
}

BOOL CXhjDATA::Init(void)
{
	if (!m_strBlkName.IsEmpty())
	{
		CADODatabase adoDb;
		CADORecordset rs;
		adoDb.Open(g_strAdoCon) ;
		rs.SetConnection(&adoDb) ;

		CString strSql ;
		strSql.Format(_T("select * from define_default_xhj where block_name='%s'"), m_strBlkName) ;
		if (rs.Open(strSql))
		{
			while(!rs.IsEOF())
			{
				long nTmp = 0 ;
				CString strTmp ;
				rs.GetFieldValue(_T("is_train"), nTmp) ;
				m_bIsTrain = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("box_type"), m_strBoxTypeOption) ;
				int iLoc = m_strBoxTypeOption.Find(_T(";")) ;
				m_strBoxType = (iLoc==-1?m_strBoxTypeOption:m_strBoxTypeOption.Left(iLoc)) ;
				rs.GetFieldValue(_T("core_num1"), m_nCoreNumI) ;
				rs.GetFieldValue(_T("core_num2"), m_nCoreNumII) ;
				rs.GetFieldValue(_T("has_ddunit"), nTmp) ;
				m_bHasDDUnit = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("is_alu"), nTmp) ;
				m_bIsAlu = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("has_phone"), nTmp) ;
				m_bHasPhone = (nTmp==0?FALSE:TRUE) ;
				rs.GetFieldValue(_T("ds_num"), m_nDSNum) ;
				rs.GetFieldValue(_T("speed_limit"), m_nSpeedLimit) ;

				rs.MoveNext() ;
			}
			rs.Close() ;
		}


		rs.Close() ;
		adoDb.Close() ;
	}

	return 0;
}
