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
//----- StdAfx.h : include file for standard system include files,
//-----      or project specific include files that are used frequently,
//-----      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#ifdef _FULLDEBUG_
#pragma message ("Warning! You should not run this application in a Standard AutoCAD release.")
#pragma message ("         Due to the memory managment constraints.")
#endif

#pragma warning(disable: 4786)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------
#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//----- Exclude rarely-used stuff from Windows headers
#endif

//----- Modify the following defines if you have to target a platform prior to the ones specified below.
//----- Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER					//----- Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501			//----- Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT			//----- Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		//----- Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS			//----- Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0500	//----- Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE				//----- Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0600		//----- Change this to the appropriate value to target IE 5.0 or later.
#endif

//-----------------------------------------------------------------------------
//----- 'DEBUG workaround' below prevents the MFC or ATL #include-s from pulling 
//----- in "Afx.h" that would force the debug CRT through #pragma-s.
#if defined(_DEBUG) && !defined(_FULLDEBUG_)
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#pragma message ("     Compiling MFC / STL / ATL header files in release mode.")
#endif

//-----------------------------------------------------------------------------
#include <afxwin.h>				//----- MFC core and standard components
#include <afxext.h>				//----- MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//----- MFC OLE classes
#include <afxodlgs.h>			//----- MFC OLE dialog classes
#include <afxdisp.h>			//----- MFC Automation classes
#endif //----- _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//----- MFC ODBC database classes
#endif //----- _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//----- MFC DAO database classes
#endif //----- _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//----- MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//----- MFC support for Windows Common Controls
#endif //----- _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"


#include "GsPreviewCtrl.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif

//
#define ZHFMSG_PREVIEWCTRL_LBDOWN	WM_USER+1


//-----------------------------------------------------------------------------
//----- Include ObjectDBX/ObjectARX headers
//----- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_			//- Support for the BRep API
//#define _HLR_SUPPORT_				//- Support for the Hidden Line Removal API
//#define _IE_SDK_SUPPORT_			//- Support for the Autodesk Image Engine API
//#define _AMODELER_SUPPORT_		//- Support for the AModeler API
//#define _ASE_SUPPORT_				//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_			//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_		//- Include legacy headers in this project
#include "arxHeaders.h"
//-----------------------------------------------------------------------------
#include "DocData.h"			//----- Your document specific data
#include "rxdebug.h"
//----- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;

#include <atlbase.h>
#include <complex>

#include "../blkdll/BlkUtitily.h"
#include "../blkuserdll/UserSystem.h"
#include "ZhfPaletteSet.h"

#define PI 3.14159265358979323846
typedef CArray<long, long> CLongArray;

extern bool g_bIsToolBarLock ;
extern bool g_bIsTukuFirst ;
extern bool g_bAddJyjWhenInsertXhj  ;
// extern CUserSystem g_userSystem ;
extern CString g_strBasePath ;
extern CMDIFrameWnd* g_pAcadFrame ;
extern  CGsPreviewCtrl* g_pCtrlGsPreview ; //��ǰѡ�е�ͼ��ͼ��ؼ�
extern CArray<CZhfPaletteSet*, CZhfPaletteSet*&> g_pArrayZhfPaletteSet ;


extern BOOL g_bCaseSensitive ;
extern int g_iSel ;


//////////////////////////////////////////////////////////////////////////
//find and replace
extern int g_iColorSel ; 
extern CString g_strFindTxt ;
extern CString g_strReplaceTxt ;
extern BOOL g_bCaseSensitive ;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//inputtext.cpp
extern double g_inputtext_dXOffSet ;
extern double g_inputtext_dYOffSet ;
extern double g_inputtext_dHeight ;
extern double g_inputtext_dWidth ;
extern double g_inputtext_dRotation ;
extern int g_inputtext_iColorIndex ;
extern int g_inputtext_iJustifyIndex ;
extern CString g_inputtext_strStyle ;
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//stextset.cpp
extern CString g_strSTxtSetFront ;
extern CString g_strSTxtSetBack ;
extern int g_iSTxtSetStart ;
extern int g_iSTxtSetIncrement  ;
extern int g_iSTxtSetRepeat  ;
extern int g_iSTxtSetSelectMode ;
extern int g_iSTxtSetSortBy ;

BOOL InitSystem() ;
//////////////////////////////////////////////////////////////////////////
#ifndef IMPORTOFFICE
//for office2003

#import "C:\\Program Files\\Common Files\\Microsoft Shared\\OFFICE11\\mso.dll" rename_namespace("Office")
using namespace Office;

#import "c:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.olb" rename_namespace("VBE6")
using namespace VBE6;

// Excel�����
#import "d:\\Program Files\\Microsoft Office\\OFFICE11\\EXCEL.EXE" rename("RGB","RGBEx"),rename("DialogBox","DialogBoxEx"),named_guids,rename_namespace("MSExcel")
using namespace MSExcel;

#define IMPORTOFFICE
#endif