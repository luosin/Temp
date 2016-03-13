// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- zhftool.cpp : Initialization functions
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//#include "resource.h"
#include <afxdllx.h>

//CUserSystem g_userSystem ;
//CString g_strAdoCon = _T("") ; //数据库连接字符串
CString g_strBasePath = _T("") ; //系统路径
//-----------------------------------------------------------------------------
//----- Define the sole extension module object.
AC_IMPLEMENT_EXTENSION_MODULE(zhftoolDLL)
//- Please do not remove the 3 following lines. These are here to make .NET MFC Wizards
//- running properly. The object will not compile but is require by .NET to recognize
//- this project as being an MFC project
#ifdef NEVER
AFX_EXTENSION_MODULE zhftoolExtDLL ={ NULL, NULL } ;
#endif

//----- Now you can use the CAcModuleResourceOverride class in
//----- your application to switch to the correct resource instance.
//----- Please see the ObjectARX Documentation for more details

//-----------------------------------------------------------------------------
//----- DLL Entry Point
extern "C"
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	//----- Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved) ;

	if ( dwReason == DLL_PROCESS_ATTACH ) {
        _hdllInstance =hInstance ;
		zhftoolDLL.AttachInstance (hInstance) ;
		InitAcUiDLL () ;
	} else if ( dwReason == DLL_PROCESS_DETACH ) {
		zhftoolDLL.DetachInstance () ;
	}
	return (TRUE) ;
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
	BOOL bReturn2 = TRUE ;
	
	//for access 2000
	//g_strAdoCon.Format (_T("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s\\db\\xhcad.mdb"), g_strBasePath);
	//for access 97
	//g_strAdoCon.Format (_T("Provider=Microsoft.Jet.OLEDB.3.51;Persist Security Info=False;Data Source=%s\\db\\xhcad-97.mdb"), g_strBasePath);
	//for win7 x86 x64
	//g_strAdoCon.Format (_T("Provider=Microsoft.ACE.OLEDB.12.0;Persist Security Info=False;Data Source=%s\\db\\xhcad.mdb"), g_strBasePath);	
	
// 	if( bFirst)
// 	{
// 		::AfxOleInit() ;
// 		bFirst = FALSE ;
// 	}


		
	bReturn = TRUE ;
	return bReturn && bReturn2 ;
}
