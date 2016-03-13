#include "stdafx.h"
#include "bttools.h"

CBtTools::CBtTools(void)
{
}

CBtTools::~CBtTools(void)
{
}


// 重新计算轨道电路补偿电容
int CBtTools::ReComputeCapability(const CString & strDir)
{
	CUserSystem mysys ;
	CStringArray strArrayFile ;
	CBlkUtility::FindFile(strDir, _T("dwg"), strArrayFile) ;
	for (int i=0; i<strArrayFile.GetSize(); i++)
	{
		AcDbDatabase *pDb = new AcDbDatabase() ;
		mysys.m_pDb = pDb ;
		if (pDb->readDwgFile(strArrayFile.GetAt(i))==Acad::eOk)
		{
			CString strBlkNames[2] = {_T("FREQU_1"), _T("FREQU_2")} ;
			for (int iBlkIndex=0; iBlkIndex<2; iBlkIndex++)
			{
				CLongArray nArrayBlkHandle ;
				int iFind = mysys.FindBlk(strBlkNames[iBlkIndex], nArrayBlkHandle) ;
				mysys.SortEnt(nArrayBlkHandle, 1) ;
				for (int j=0; j<nArrayBlkHandle.GetCount(); j++)
				{
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(nArrayBlkHandle.GetAt(j), pEnt, AcDb::kForWrite))
					{
						AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
						CString strFreq, strGuiDaoLen, strCapability ;
						mysys.GetAttValue(pRef, _T("FREQUENCY"), strFreq) ;
						mysys.GetAttValue(pRef, _T("LENGTH"), strGuiDaoLen) ;
						mysys.GetAttValue(pRef, _T("capability"), strCapability) ;
						CString strGuiDao ;
						int iStart = strCapability.Find(_T("(")) ;
						int iEnd = strCapability.Find(_T(")")) ;
						if (iStart==-1||iEnd==-1)
						{
							AcGePoint3d pt_3d_ref = pRef->position() ;
							acutPrintf(_T("\n电容字符串没找到轨道,%s,%.3f-%.3f"), strArrayFile.GetAt(i), pt_3d_ref.x, pt_3d_ref.y) ;
							pEnt->close() ;
							continue;
						}
						strGuiDao = strCapability.Mid(iStart+1, iEnd-iStart) ;

						strFreq = strFreq.Left(4) ;
						double dGuiDaoLen = _tstof(strGuiDaoLen.TrimRight(_T("m"))) ;

						double dLen = 24.0 ;
						if (j==0||j==(iFind-1))
						{
							dLen = 12.0 ;
						}
						if(GetCapInfo(strFreq, dGuiDaoLen, dLen, strCapability)==TRUE)
						{
							strCapability.AppendFormat(_T("(%s)"), strGuiDao) ;
							mysys.SetAttValue(pRef, _T("capability"), strCapability) ;
							mysys.SetAttColor(pRef, _T("capability"), 3) ;
						}

						pEnt->close() ;
					}
				}
			}
			pDb->saveAs(strArrayFile.GetAt(i)) ;
		}
		delete pDb ;
		pDb = NULL ;
	}
	return 0;
}

BOOL CBtTools::GetCapInfo(const CString & strFreq, double dGuiDaoLen, double dLenPlus, CString & strCapability)
{
	BOOL bRet = FALSE ;
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from define_capability where cap_freq='%s' and cap_min<=%f and cap_max>=%f"), strFreq, dGuiDaoLen, dGuiDaoLen) ;
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iCount = cSet.GetRecordCount() ;
			if (iCount>0)
			{
				cSet.MoveFirst() ;
				int iCapType = 0, iCapNum = 0 ;
				cSet.GetFieldValue(_T("cap_type"), iCapType) ;
				cSet.GetFieldValue(_T("cap_num"), iCapNum) ;
				double dLen = (dGuiDaoLen-dLenPlus)/iCapNum ;
				strCapability.Format(_T("c=%duf*%d l=%.1f"), iCapType, iCapNum, dLen) ;
				bRet = TRUE ;
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
		AfxMessageBox(_T("unknown error in GetCapInfo")) ;		
	}

	return bRet;
}
