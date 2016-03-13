#include "StdAfx.h"
#include "dbutility.h"

_ConnectionPtr g_PtrCon ; 

BOOL ExecSqlString(CString strSql)
{
	BOOL bRet = FALSE ;
	//CADORecordset cSet ;
	try
	{
		g_PtrCon->Execute((_bstr_t)strSql, NULL, NULL) ;
		bRet = TRUE ;
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error")) ;
	}
	return bRet ;
}

CDBUtility::CDBUtility(void)
{
}

CDBUtility::~CDBUtility(void)
{
}

void CDBUtility::ClearTable(CString strTbName)
{
	CString strSql ;
	strSql.Format(_T("delete * from %s"), strTbName) ;
	ExecSqlString(strSql) ;
	//重置id
	strSql.Format(_T("alter table [%s] alter column id counter(1,1)"),strTbName) ;
	ExecSqlString(strSql) ;
}

// 取得数据库表的最大Id
int CDBUtility::GetTableMaxId(const CString & strTableName)
{
	int nId = 0 ;
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select max(id) as maxid from %s"), strTableName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.GetFieldValue(_T("maxid"), nId) ;
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in GetTableMaxId!"));			
	}
	return nId;
}


// 取得数据库表中，某一个字段的值
int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, CString & strValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(strFildName, strValue) ;
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}
int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, int & nValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(strFildName, nValue) ;
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}

int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, long & nValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(strFildName, nValue) ;
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}

int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, double & dValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				cSetFind.GetFieldValue(strFildName, dValue) ;
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}

// 取得数据库表中，某一个字段的值
int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, CStringArray & strArrayValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				while (!cSetFind.IsEOF())
				{
					CString strValue ;
					cSetFind.GetFieldValue(strFildName, strValue) ;
					strArrayValue.Add(strValue) ;

					cSetFind.MoveNext() ;
				}
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}

int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, CArray<int, int> & nArrayValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				while (!cSetFind.IsEOF())
				{
					int iValue ;
					cSetFind.GetFieldValue(strFildName, iValue) ;
					nArrayValue.Add(iValue) ;

					cSetFind.MoveNext() ;
				}
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}

int CDBUtility::GetRecordValue(const CString & strSql, const CString & strFildName, CArray<long, long> & nArrayValue)
{
	int nRet = 0 ;
	try
	{
		CADORecordset cSetFind ;
		if (cSetFind.IsOpen())
		{
			cSetFind.Close() ;
		}
		if (cSetFind.Open(g_PtrCon, strSql))
		{
			nRet = cSetFind.GetRecordCount() ;
			if (nRet>0)
			{
				cSetFind.MoveFirst() ;
				while (!cSetFind.IsEOF())
				{
					long iValue ;
					cSetFind.GetFieldValue(strFildName, iValue) ;
					nArrayValue.Add(iValue) ;

					cSetFind.MoveNext() ;
				}
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}


int CDBUtility::SetRecordValue(const CString & strSql, const CString & strFiledName, const CString & strValue)
{
	try
	{
		CADORecordset cSet ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			cSet.MoveFirst() ;
			while (!cSet.IsEOF())
			{
				cSet.Edit() ;
				cSet.SetFieldValue(strFiledName, strValue) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetRecordValue")) ;
	}
	return 0;
}
int CDBUtility::SetRecordValue(const CString & strSql, const CString & strFiledName, int  & iValue)
{
	try
	{
		CADORecordset cSet ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			cSet.MoveFirst() ;
			while (!cSet.IsEOF())
			{
				cSet.Edit() ;
				cSet.SetFieldValue(strFiledName, iValue) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetRecordValue")) ;
	}
	return 0;
}
int CDBUtility::SetRecordValue(const CString & strSql, const CString & strFiledName, long & lValue)
{
	try
	{
		CADORecordset cSet ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			cSet.MoveFirst() ;
			while (!cSet.IsEOF())
			{
				cSet.Edit() ;
				cSet.SetFieldValue(strFiledName, lValue) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetRecordValue")) ;
	}
	return 0;
}
int CDBUtility::SetRecordValue(const CString & strSql, const CString & strFiledName, double & dValue)
{
	try
	{
		CADORecordset cSet ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			cSet.MoveFirst() ;
			while (!cSet.IsEOF())
			{
				cSet.Edit() ;
				cSet.SetFieldValue(strFiledName, dValue) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetRecordValue")) ;
	}
	return 0;
}
