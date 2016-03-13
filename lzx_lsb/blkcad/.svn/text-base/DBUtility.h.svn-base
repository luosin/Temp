#pragma once

/*
template<typename T>
int GetRecordValue(const CString & strSql, const CString & strFiledName, T & Value)
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
				cSetFind.GetFieldValue(strFiledName, Value) ;
			}
			cSetFind.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetRecordValue")) ;
	}
	return nRet;
}

template<typename T>
int SetRecordValue(const CString & strSql, const CString & strFiledName, T & Value)
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
				cSet.SetFieldValue(strFiledName, Value) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetRecordValue")) ;
	}
	return 0;
}
*/

class CDBUtility
{
public:
	CDBUtility(void);
	virtual ~CDBUtility(void);

	//清空table
	static void ClearTable(CString strTbName);
	// 取得数据库表的最大Id
	static int GetTableMaxId(const CString & strTableName);

	// 取得数据库表中，某一个字段的值
	static int GetRecordValue(const CString & strSql, const CString & strFildName, CString & strValue);
	static int GetRecordValue(const CString & strSql, const CString & strFildName, int & nValue);
	static int GetRecordValue(const CString & strSql, const CString & strFildName, long & nValue);
	static int GetRecordValue(const CString & strSql, const CString & strFildName, double & dValue);

	static int GetRecordValue(const CString & strSql, const CString & strFildName, CStringArray & strArrayValue);
	static int GetRecordValue(const CString & strSql, const CString & strFildName, CArray<int, int> & nArrayValue);
	static int GetRecordValue(const CString & strSql, const CString & strFildName, CLongArray & nArrayValue);

	static int SetRecordValue(const CString & strSql, const CString & strFiledName, const CString & strValue);
	static int SetRecordValue(const CString & strSql, const CString & strFiledName, int & iValue);
	static int SetRecordValue(const CString & strSql, const CString & strFiledName, long & lValue);
	static int SetRecordValue(const CString & strSql, const CString & strFiledName, double & dValue);

};
