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
//----- DistToXhlPalette.h : Declaration of the CDistToXhlPalette
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "adui.h"
// #include <afxtempl.h>
// #include "afxcoll.h"
#include "DistToXhlUGCtrl.h"

//-----------------------------------------------------------------------------
class CDistToXhlPalette : public CAdUiPalette {
	DECLARE_DYNAMIC (CDistToXhlPalette)

public:
	CDistToXhlPalette () ;
	virtual ~CDistToXhlPalette () ;

protected:
	afx_msg void OnButtonClickDwgToGrid() ;
	afx_msg void OnButtonClickApplyToDwg();
	afx_msg void OnButtonClickToXls();
	afx_msg void OnButtonClickFromXls();

	DECLARE_MESSAGE_MAP()
public:
	void AddControls();
	void DeleteControls(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 当前选择
	int m_iCurSel;

	// -----------------------------------------------------------------------------
	virtual void OnSetActive(void);
	// 首次显示
	bool m_bFirstActive;

	CDistToXhlUGCtrl m_grid;
	vector<CButton* > m_vecButton ;

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	CButton* NewButton(int nID, CRect rect, int nStyle);
} ;