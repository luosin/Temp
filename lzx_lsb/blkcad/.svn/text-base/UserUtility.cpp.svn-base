// UserUtility.cpp: implementation of the CUserUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserUtility::CUserUtility()
{
}

CUserUtility::~CUserUtility()
{
}

// 批量设置扩展数据（从数据库读取）
int CUserUtility::SetXdataSS(void)
{
	CUserSystem mysys ;	

	AcDbObjectId layer_id ;
	layer_id = mysys.Set_Layer(_T(""), _T("define")) ;

	ads_name ssname, temp_name ;
	ads_point pt ;

	acutPrintf(_T("\n选择实体")) ;
	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		CStringArray strArrayAppName, strArrayValue ;
		CLongArray nArrayType ;
		try
		{
			CADORecordset cSet ;
			CString strSql ;
			strSql.Format(_T("select * from xdata order by id asc")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}
				while(!cSet.IsEOF())
				{
					CString strAppName, strValue ;
					long iType = 0 ;
					cSet.GetFieldValue(_T("xdata_appname"), strAppName) ;
					cSet.GetFieldValue(_T("xdata_type"), iType) ;
					cSet.GetFieldValue(_T("xdata_value"), strValue) ;
					nArrayType.Add(iType) ;
					strArrayAppName.Add(strAppName) ;
					strArrayValue.Add(strValue) ;

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
			AfxMessageBox(_T("unknown error in CUserUtility::SetXdataSS(void)")) ;
		}

		long nLength = 0 ;
		acedSSLength(ssname, &nLength) ;

		if (nLength!=nArrayType.GetCount())
		{
			acedSSFree(ssname) ;
			AfxMessageBox(_T("选择实体和数据内数据数量不一致，请重新选择！")) ;
			return -1 ;
		}

		for (int i=0; i<nLength; i++)
		{
			acedSSName(ssname, i, temp_name) ;
			AcDbEntity *pEnt = NULL ;
			if (mysys.OpenAcDbEntity(temp_name, pEnt))
			{
				switch (nArrayType.GetAt(i))
				{
				case 0:
					mysys.SetXdata(pEnt, strArrayAppName.GetAt(i), _tstoi(strArrayValue.GetAt(i))) ;
					break;
				case 1:
					mysys.SetXdata(pEnt, strArrayAppName.GetAt(i), strArrayValue.GetAt(i)) ;
					break ;
				}

				pEnt->setColorIndex(3) ;
				pEnt->close() ;
				pEnt = NULL ;
			}
		}
		acedSSFree(ssname) ;	
	}
	else
	{
		AfxMessageBox(_T("没有选中任何实体")) ;
	}
	return 0;
}