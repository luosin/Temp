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
//----- ZhfPalette.h : Declaration of the CZhfPalette
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
#include "GsPreviewCtrl.h"
#include <afxtempl.h>
#include "afxcoll.h"

//-----------------------------------------------------------------------------
class CZhfPalette : public CAdUiPalette {
	DECLARE_DYNAMIC (CZhfPalette)

public:
	CZhfPalette () ;
	//CZhfPalette(const TCHAR* szFilePath) ;
	virtual ~CZhfPalette () ;

protected:
	DECLARE_MESSAGE_MAP()
public:
	//CAcUiNumericEdit* m_pEdit1;
	void AddControls();
	void DeleteControls(void);
	CArray<CGsPreviewCtrl*,CGsPreviewCtrl*> m_pArrayPreviewCtrl;
	CArray<CStatic*,CStatic*> m_pArrayStatic;
	//CGsPreviewCtrl m_pPreCtrl ;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ResizeCtrl(int iX, int iY);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnCtrlIsSelected(WPARAM wparam,LPARAM lparam);
	int m_iOldPosY;
	CStringArray m_strArrayFile;
	CStringArray m_strArrayFileName;
	Acad::ErrorStatus Wblock(AcDbDatabase*& pDbFrom, AcDbDatabase*& pDbTo, /*const AcGePoint3d& pt_3d_base,*/ const AcGeMatrix3d & xform=AcGeMatrix3d::kIdentity);

	int m_iCtrlWidth;
	int m_iCtrlHeight;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// 当前选择
	int m_iCurSel;
	//CString m_strFilePath;
	// 图块显示方式：0，不显示属性，1，显示属性，2，显示属性默认值
	int m_iFilterMode;
	void FilterDb(AcDbDatabase* pDb, int iFilterMode=0);
	int m_iMarginX;
	int m_iMarginY;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// -----------------------------------------------------------------------------
	virtual void OnSetActive(void);
	// 首次显示
	bool m_bFirstActive;
} ;
