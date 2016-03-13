// MyAdo.h: interface for the CMyAdo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADODATABASE_H__5F8F7EF0_C7C6_11D6_BF51_0000E8D3B57C__INCLUDED_)
#define AFX_ADODATABASE_H__5F8F7EF0_C7C6_11D6_BF51_0000E8D3B57C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ZHFDLL_EXPORT
#define ZHFDLL __declspec(dllexport)
#else
#define ZHFDLL __declspec(dllimport)
#endif

#include "icrsint.h"
#include <atlstr.h>
#include <afxtempl.h>

#import "c:\\program files\\common files\\system\\ado\\msado21.tlb" rename_namespace("EasyAdoNS") rename ( "EOF", "EndOfFile" )
using namespace EasyAdoNS;

struct CADOFieldInfo
{
	TCHAR m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};

typedef CArray<long, long> CLongArray;
typedef CArray<double, double> CDoubleArray;
typedef CArray<int, int> CIntArray;

CString IntToStr(int nVal);

CString LongToStr(long lVal);

class ZHFDLL CADODatabase
{
public:
	bool Execute(LPCTSTR lpstrExec);
	CADODatabase()
	{
		::CoInitialize(NULL);
			
		m_pConnection = NULL;
		m_strConnection = _T("");
		m_pConnection.CreateInstance(__uuidof(Connection));
	}
	
	~CADODatabase()
	{
		Close();
		m_pConnection.Release();
		m_pConnection = NULL;
		::CoUninitialize();
	}
	
	bool Open(LPCTSTR lpstrConnection = _T(""));
	_ConnectionPtr GetActiveConnection() {return m_pConnection;};
	DWORD GetRecordCount(_RecordsetPtr m_pRs);
	long BeginTransaction() 
		{return m_pConnection->BeginTrans();};
	long CommitTransaction() 
		{return m_pConnection->CommitTrans();};
	long RollbackTransaction() 
		{return m_pConnection->RollbackTrans();};
	bool IsOpen();
	void Close();
	void SetConnectionString(LPCTSTR lpstrConnection)
		{m_strConnection = lpstrConnection;};
	CString GetConnectionString()
		{return m_strConnection;};
	CString GetLastError() 
		{return m_strLastError;};
protected:
	void dump_com_error(_com_error &e);

public:
	_ConnectionPtr m_pConnection;
	CString m_strConnection;
	CString m_strLastError;
	bool ClearTable(LPCTSTR lpstrTbName);
};

class ZHFDLL CADORecordset
{
public:
	enum cadoOpenEnum
	{
		openUnknown = 0,
		openQuery = 1,
		openTable = 2,
		openStoredProc = 3
	};

	enum cadoEditEnum
	{
		dbEditNone = 0,
		dbEditNew = 1,
		dbEdit = 2
	};
	
	enum cadoPositionEnum
	{
	
		positionUnknown = -1,
		positionBOF = -2,
		positionEOF = -3
	};
	
	enum cadoSearchEnum
	{	
		searchForward = 1,
		searchBackward = -1
	};

	
	bool SetFieldValue(int nIndex, CString strValue);
	bool SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	bool SetFieldValue(int nIndex, int nValue);
	bool SetFieldValue(LPCTSTR lpFieldName, int nValue);
	bool SetFieldValue(int nIndex, long lValue);
	bool SetFieldValue(LPCTSTR lpFieldName, long lValue);
	bool SetFieldValue(int nIndex, double dblValue);
	bool SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	bool SetFieldValue(int nIndex, COleDateTime time);
	bool SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);

	void CancelUpdate();
	bool Update();
	void Edit();
	bool AddNew();

	bool Find(LPCTSTR lpFind, int nSearchDirection = CADORecordset::searchForward);
	bool FindFirst(LPCTSTR lpFind);
	bool FindNext();
	int i_count;

	CADORecordset()
	{
		m_pRecordset = NULL;
		m_pCmd = NULL;
		i_count=0;
		m_strQuery = _T("");
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pCmd.CreateInstance(__uuidof(Command));
		m_nEditStatus = CADORecordset::dbEditNone;
		m_nSearchDirection = CADORecordset::searchForward;
	}

	CADORecordset(CADODatabase* pAdoDatabase);

	~CADORecordset()
	{
		Close();
		m_pRecordset.Release();
		m_pCmd.Release();
		m_pRecordset = NULL;
		m_pCmd = NULL;
		m_strQuery = _T("");
		m_nEditStatus = dbEditNone;
	}

	CString GetQuery() 
		{return m_strQuery;};
	void SetQuery(LPCSTR strQuery) 
		{m_strQuery = strQuery;};
	bool RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	bool IsOpen();
	void Close();
	bool Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	bool Open(LPCTSTR lpstrExec = _T(""), int nOption = CADORecordset::openUnknown);
	bool GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
	bool GetFieldValue(int nIndex, double& dbValue);
	bool GetFieldValue(LPCTSTR lpFieldName, long& lValue);
	bool GetFieldValue(int nIndex, long& lValue);
	bool GetFieldValue(LPCTSTR lpFieldName, int& nValue);
	bool GetFieldValue(int nIndex, int& nValue);
	bool GetFieldValue(LPCTSTR lpFieldName, CString& strValue);
	bool GetFieldValue(int nIndex, CString& strValue);
	bool GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time);
	bool GetFieldValue(int nIndex, COleDateTime& time);
	bool IsFieldNull(LPCTSTR lpFieldName);
	bool IsFieldNull(int nIndex);
	bool IsFieldEmpty(LPCTSTR lpFieldName);
	bool IsFieldEmpty(int nIndex);	
	bool IsEof()
		{return m_pRecordset->EndOfFile == VARIANT_TRUE;};
	bool IsEOF()
		{return m_pRecordset->EndOfFile == VARIANT_TRUE;};
	bool IsBof()
		{return m_pRecordset->BOF == VARIANT_TRUE;};
	bool IsBOF()
		{return m_pRecordset->BOF == VARIANT_TRUE;};
	void MoveFirst() 
		{m_pRecordset->MoveFirst();};
	void MoveNext() 
		{m_pRecordset->MoveNext();};
	void MovePrevious() 
		{m_pRecordset->MovePrevious();};
	void MoveLast() 
		{m_pRecordset->MoveLast();};
	long GetAbsolutePage()
		{return m_pRecordset->GetAbsolutePage();};
	void SetAbsolutePage(int nPage)
		{m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);};
	long GetPageCount()
		{return m_pRecordset->GetPageCount();};
	long GetPageSize()
		{return m_pRecordset->GetPageSize();};
	void SetPageSize(int nSize)
		{m_pRecordset->PutPageSize(nSize);};
	long GetAbsolutePosition()
		{return m_pRecordset->GetAbsolutePosition();};
	void SetAbsolutePosition(int nPosition)
		{m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);};
	bool GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	bool GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	bool GetChunk(LPCTSTR lpFieldName, CString& strValue);
	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	CString GetLastError() {return m_strLastError;};
	void GetBookmark()
		{m_varBookmark = m_pRecordset->Bookmark;};
	bool SetBookmark();
	bool Delete();

	// 取得table的最大id
	int GetTableMaxId(LPCTSTR lpstrTableName);
	//取得结果集数量
	int GetRecordCount(LPCTSTR lpstrSql) ;
	// 取得数据库表中，某一个字段的值
	int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, CString & strValue);
	int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, int & nValue);
	 int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, long & nValue);
	 int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, double & dValue);

	 int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, CStringArray& strArrayValue);
	 int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, CIntArray& nArrayValue);
	 int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, CLongArray& nArrayValue);
	 int GetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFildName, CDoubleArray& nArrayValue);

	 //连续对一lpstrSql进行SetRecordValue操作要注意，假如本连续操作的前次操作对该lpstrSql结果集已经产生了影响，那么后续操作将是错误的，此时可以将所要更新的列名使用filed1,filed2......的形式传给第二个参数
	 int SetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFiledName, LPCTSTR lpstrValue);
	 int SetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFiledName, int iValue);
	 int SetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFiledName, long  lValue);
	 int SetRecordValue(LPCTSTR lpstrSql, LPCTSTR lpstrFiledName, double dValue);

protected:
	_ConnectionPtr m_pConnection;
	int m_nSearchDirection;
	CString m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	CString m_strLastError;
	void dump_com_error(_com_error &e);
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCmd;
	CString m_strQuery;
public:
	void SetConnection(CADODatabase * pAdoDatabase);
};

#endif // !defined(AFX_ADODATABASE_H__5F8F7EF0_C7C6_11D6_BF51_0000E8D3B57C__INCLUDED_)
