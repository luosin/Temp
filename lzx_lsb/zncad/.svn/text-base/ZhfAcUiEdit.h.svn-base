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
//----- ZhfAcUiEdit.h : Declaration of the CZhfAcUiEdit
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"

//-----------------------------------------------------------------------------
class CZhfAcUiEdit : public CAcUiEdit {
	DECLARE_DYNAMIC (CZhfAcUiEdit)

public:
	CZhfAcUiEdit () ;
	virtual ~CZhfAcUiEdit () ;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	AFX_INLINE BOOL CZhfAcUiEdit::ShowBalloonTip(LPCWSTR lpszTitle, LPCWSTR lpszText, INT ttiIcon)
	{
		ASSERT(::IsWindow(m_hWnd));
		EDITBALLOONTIP bt;
		::ZeroMemory(&bt, sizeof(EDITBALLOONTIP));

		bt.cbStruct = sizeof(EDITBALLOONTIP);
		bt.pszTitle = lpszTitle;
		bt.pszText = lpszText;
		bt.ttiIcon = ttiIcon;

		return Edit_ShowBalloonTip(m_hWnd, &bt); // EM_SHOWBALLOONTIP
	}

	AFX_INLINE BOOL CZhfAcUiEdit::ShowBalloonTip(PEDITBALLOONTIP pEditBalloonTip)
	{
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(pEditBalloonTip != NULL);
		if (pEditBalloonTip == NULL)
		{
			return FALSE;
		}
		return Edit_ShowBalloonTip(m_hWnd, pEditBalloonTip); // EM_SHOWBALLOONTIP
	}

	AFX_INLINE BOOL CZhfAcUiEdit::HideBalloonTip()
	{
		ASSERT(::IsWindow(m_hWnd));
		return Edit_HideBalloonTip(m_hWnd); // EM_HIDEBALLOONTIP
	}
private:
	CString m_strRegex;
	LPCWSTR m_strTooltipTitle;
	LPCWSTR m_strTooltipText;
	CString m_strOld;
	// 将窗口文字冻结
	BOOL m_bFreezeText;

public:
	void SetValidate(const CString& strRegex, const CString& strTooltipTitle, const CString& strTooltipText) ;
	afx_msg void OnEnChange();	
	void GetInt(int& i) ;
	void GetDouble(double& d);
	void SetInt(const int& i) ;
	void SetDouble(const double& d, int iDeci=3, BOOL bTrim0=TRUE);
	void SetFreeze(BOOL bFreeze=TRUE) ;
	BOOL GetFreeze() ;
	void SetWindowText(LPCTSTR lpszString);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
} ;
