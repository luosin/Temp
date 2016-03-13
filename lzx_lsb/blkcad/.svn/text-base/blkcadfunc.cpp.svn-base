#include "StdAfx.h"
//#include "StdArx.h"

//#include <atlbase.h>
// #include "../blkuserdll/DiskInfo.h"

//_ConnectionPtr g_PtrCon ; 


void Find(CString strStart, CString strEnd, CStringArray &strArray, CArray<CStringArray * , CStringArray *> &arrayJinLu)
{
	CStringArray strArray1 ;
	strArray1.Copy(strArray) ;

	strArray1.Add(strStart) ;
	if (strStart!=strEnd)
	{
		try
		{
			CADORecordset myset ;
			CString strSql ;
			strSql.Format(_T("select * from guidao_back where name='%s'"), strStart) ;
			myset.Open(g_PtrCon, strSql) ;
			if (!myset.IsBOF())
			{
				myset.MoveFirst() ;
			}
			while (!myset.IsEOF())
			{
				CString strNext1, strNext2, strNext3 ;
				myset.GetFieldValue(_T("next1"), strNext1) ;
				myset.GetFieldValue(_T("next2"), strNext2) ;
				myset.GetFieldValue(_T("next3"), strNext3) ;

				if (strNext1!=_T(""))
				{
					Find(strNext1, strEnd, strArray1, arrayJinLu) ;
				}
				if (strNext2!=_T("")) 
				{
					Find(strNext2, strEnd, strArray1, arrayJinLu) ;
				}
				if (strNext3!=_T("")) 
				{
					Find(strNext3, strEnd, strArray1, arrayJinLu) ;
				}

				myset.MoveNext() ;
			}
			myset.Close() ;			
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


	}
	else//到达末端
	{
		if (strArray1.GetSize()>2)
		{
			CStringArray * strArrayTmp = new CStringArray ;
			strArrayTmp->Copy(strArray1) ;
			arrayJinLu.Add(strArrayTmp) ;
		}
		/*
		CString strMsg("") ;
		for(int i=0; i<strArray1.GetSize(); i++)
		{
		strMsg.Format("%s-%s\n",strMsg,strArray1.GetAt(i)) ;
		}
		AfxMessageBox(strMsg) ;
		*/		
	}
}

// BOOL ExecSqlString(CString strSql)
// {
// 	BOOL bRet = FALSE ;
// 	//CADORecordset cSet ;
// 	try
// 	{
// 		g_PtrCon->Execute((_bstr_t)strSql, NULL, NULL) ;
// 		bRet = TRUE ;
// 	}
// 	catch (_com_error& e)
// 	{
// 		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
// 		AfxMessageBox(sBuff) ;
// 	}
// 	catch (...)
// 	{
// 		AfxMessageBox(_T("unknown error")) ;
// 	}
// 	return bRet ;
// }

BOOL GetGuiDaoBlockPosition(CString strTag, CString strValue, AcGePoint3d &pt_3d_block, AcGePoint3d &pt_3d_attribute)
{
	BOOL bRet = FALSE ;

	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;

	AcDbBlockTable * pBlkTbl = NULL ;
	g_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;

	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{		
			//处理图块 AcDbBlockReference ;
			if (_tcscmp(pEnt->isA()->name(),_T("AcDbBlockReference")) == 0)
			{
				AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;				

				//得到块名
// 				AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
// 				AcDbObjectId blkTblObjId ;
// 				blkTblObjId = pRef->blockTableRecord() ;
// 				acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead) ;
// 				TCHAR * chBlockName ;
// 				pBlkTblRecTmp->getName(chBlockName) ;
// 				pBlkTblRecTmp->close() ;

				CString strBlockName ;
				g_cSystem.GetBlockName(pRef, strBlockName) ;
				
				strBlockName.MakeLower() ;


				//确认是要读取的轨道类型
				if (strBlockName.Mid(0,15)==_T("maxubiao_guidao"))
				{					
					CString strValueTmp ;
					g_cSystem.GetAttValue(pRef, strTag, strValueTmp) ;
					if (strValueTmp==strValue) 
					{
						pt_3d_block = pRef->position() ;

						AcDbAttribute * pAtt = NULL ;
						if(g_cSystem.FindAtt(pRef, _T("GUIDAO"), pAtt))
						{
							pt_3d_attribute = pAtt->position() ;
							pAtt->close() ;
						}

						bRet = TRUE ;
					}				

				}

			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return bRet ;
}

BOOL InitSystem()
{
	BOOL bReturn = FALSE ;

	/*
	//取得硬盘序列号；
	CDiskInfo diskinfo ;
	IDINFO infotmp ;
	diskinfo.GetPhysicalDriveInfoInNT(0,&infotmp) ;	

	TCHAR * pDriveSN = infotmp.sSerialNumber ;
	CString strResult = diskinfo.GetRegNum(pDriveSN) ;
	*/

	//////////////////////////////////////////////////////////////////////////	
	//deleted
	/*
	CString strDriveSN ;
	strDriveSN.Format("%s", pDriveSN) ;
	strDriveSN.TrimLeft() ;
	strDriveSN.TrimRight() ;

	AfxMessageBox(strDriveSN) ;


	TCHAR * preserved = strrev(pDriveSN) ;
	//TCHAR ch = *(preserved+3) ;

	CString strRevSN , strTmp1, strTmp2, strResult ;
	strRevSN.Format("%s", preserved) ;	
	strRevSN.TrimLeft() ;
	strRevSN.TrimRight() ;
	for(int i=0; i< strRevSN.GetLength(); i++)
	{
		if (i%2==0)
		{
			strTmp1 += strRevSN.GetAt(i) + 2 ;
		}
		else
		{
			strTmp2 += strRevSN.GetAt(i) + 5 ;
		}		
	}
	strResult.Format("%s%s", strTmp1, strTmp2) ;
	*/
	//////////////////////////////////////////////////////////////////////////
	
	
	//ads_printf("\n%s",strResult) ;


	if (!CBlkUtility::GetBasePath(g_strBasePath))
	{
		AfxMessageBox(_T("BasePath没有设定")) ;
		return FALSE ;
	}
	g_cSystem.m_strBasePath = g_strBasePath ;	

	//
	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,_T("SOFTWARE\\ZHFCAD") ))
	{
		// 		TCHAR  chSN[100] ;
		// 		chSN[0] = '\0' ;
		// 		DWORD lSize = sizeof(chSN) ;
		// 		if (ERROR_SUCCESS == cReg.QueryValue(chSN, "sn", &lSize)) 
		// 		{
		// 			if (strcmp(strResult, chSN) == 0)
		// 			{
		// 				bReturn = TRUE ;
		// 			}
		// 		}
	}
	else
	{
		if(ERROR_SUCCESS==cReg.Create(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\ZHFCAD")))
		{

		}
	}

	cReg.Close() ;
/*
	if (bReturn == FALSE)
	{
		

		ads_printf("\n序列号:%s", pDriveSN) ;
		char  ch[100] ;
		ch[0] = '\0' ;
		DWORD lSize = sizeof(ch) ;
		
		ads_getstring(1,"\n注册码:",ch) ;
		if (strcmp(ch, strResult)==0 )
		{
			HKEY hKey ;
			//struct HKEY__*RootKey;		//注册表主键名称
			TCHAR *SubKey;				//欲打开注册表项的地址
			LPBYTE SetContent_S;		//字符串类型
			
			//RootKey = HKEY_LOCAL_MACHINE ;
			SubKey = "SOFTWARE\\hhcad" ;	
			TCHAR *ReValueName = "sn" ;
			SetContent_S = LPBYTE(ch) ;	
			

			long lOpenKeyRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,"SOFTWARE\\hhcad",0, KEY_WRITE, &hKey );
			
			if (lOpenKeyRet==ERROR_SUCCESS )
			{
				if (RegSetValueEx(hKey, ReValueName, NULL,REG_SZ, SetContent_S,CString(SetContent_S).GetLength())!=ERROR_SUCCESS) 
				{
					ads_printf("注册失败-1") ;
				}
				else
				{
					bReturn = TRUE ;
					ads_printf("\n注册成功!") ;

				}
			}
			else
			{
				if (RegCreateKey(HKEY_LOCAL_MACHINE,"SOFTWARE\\hhcad", &hKey)==ERROR_SUCCESS)
				{
					if (RegSetValueEx(hKey,	"sn",0, REG_SZ,	SetContent_S, CString(SetContent_S).GetLength()))  
					{
						ads_printf("往注册表写值未成功！") ;
					}
					else
					{
						bReturn = TRUE ;
						ads_printf("\n注册成功!") ;
					}
						
				}
				else
				{
					ads_printf("创建注册键未成功！") ;
				}
			}	
			RegCloseKey(hKey) ;
			
		}
		else
		{
			ads_printf("注册码不正确！") ;
		}
	}
	*/
	BOOL bReturn2 = FALSE ;
	
	CString strCon ;
	strCon.Format (_T("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s\\db\\xhcad.mdb"), g_strBasePath);
	//strCon.Format (_T("Provider=Microsoft.Jet.OLEDB.3.51;Persist Security Info=False;Data Source=%s\\db\\xhcad-97.mdb"), g_strBasePath);
	//for win7 x86 x64
	//strCon.Format (_T("Provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=%s\\db\\xhcad.mdb"), g_strBasePath);	
	
	if( bFirst)
	{
		::AfxOleInit() ;
		bFirst = FALSE ;
	}
	if( g_PtrCon != NULL ) 
	{
		g_PtrCon = NULL ;
	}
	g_PtrCon .CreateInstance ( __uuidof( Connection ) ); 
	HRESULT hr;
	try
	{
		if (g_PtrCon != NULL)
		{
			hr = g_PtrCon->Open((_bstr_t)strCon, _bstr_t(L""), _bstr_t(L""), adConnectUnspecified);
			
			if(FAILED(hr))
			{
				_com_issue_error(hr);
				g_PtrCon = NULL ;
				AfxMessageBox( _T("数据库连接失败,确认是否正确安装!" )) ;
			}
			else
			{
				//已经连接了数据；
				bReturn2 = TRUE ;
				//g_cIni.Read( g_strIniFileName );
				//g_cSystem.strBasePath = g_strBasePath ;
				
			}
		}
		else
		{
			AfxMessageBox( _T("没有连接实例，失败") ) ;
		}
	}
	catch(_com_error& e)
	{
		g_PtrCon = NULL ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		g_PtrCon = NULL ;
		AfxMessageBox(_T("未知的错误!"));
	}
	
		
	bReturn = TRUE ;
	return bReturn && bReturn2 ;
}


// getFile
//
// Returns: RTNORM if sucessfully
//          RTERROR if failure
//          The selected file in pResult
//
int getFile(
			// input:
			const TCHAR* pCmdAsk,  // prompt for command line mode
			const TCHAR* pDiaAsk,  // prompt for dialog box mode
			const TCHAR* pExt,     // file extension
			// output:
			TCHAR* pResult)        // selected file returned to the caller.
			// must be an allocated buffer!
{
	TCHAR tmpResult[134];
	struct resbuf *pRb = NULL;
	int res = RTERROR;
	TCHAR drive[_MAX_DRIVE], dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME], ext[_MAX_EXT];

	// command line argument?
	//
	if ( NULL != (pRb = acedGetArgs()) && RTSTR == pRb->restype) {
		// yes!
		//
		//_tcscpy_s( tmpResult, pRb->resval.rstring );
		CBlkUtility::tcscpy( tmpResult , (_tcslen(pRb->resval.rstring)+1)*sizeof(TCHAR), pRb->resval.rstring ) ;
		// continue later: look if acedFindFile finds the file!
		//
		res = RTNORM;
	}
	else {
		// no command line argument
		//
		struct resbuf fileDia;

		// is "filedia" set to 1?
		//
		acedGetVar( _T("filedia"), &fileDia );
		if (1 == fileDia.resval.rint) {
			// yes, display dialog box
			//
			struct resbuf fileRes;

			res = acedGetFileD( pDiaAsk, NULL, pExt, 0, &fileRes );
			if (RTNORM != res) return res;
			if (RTSTR == fileRes.restype) {
				// user selected valid file

				CBlkUtility::tcscpy( pResult, (_tcslen(fileRes.resval.rstring)+1)*sizeof(TCHAR), fileRes.resval.rstring );
				free( fileRes.resval.rstring );
				return RTNORM;
			}
		}
		// Prompt at command line required. Either
		// filedia = 0, or "type it" button in dialog was pressed.
		//
		res = acedGetString( 0, pCmdAsk, tmpResult );
		if (RTNORM != res) return res;
	}
	// look if there's already the file extension attached
	//
	_tsplitpath( tmpResult, drive, dir, fname, ext );
	// if not, append file extension
	//
	if (_tcsicmp( ext, pExt ) != 0)
		_tmakepath( tmpResult, drive, dir, fname, pExt );

	// look if file exists (only if not in dialog mode)
	//
	res = acedFindFile( tmpResult, pResult );
	if (RTNORM != res)
		acutPrintf( _T("\nUnable to open %s.\n"), tmpResult );
	return res;
}
