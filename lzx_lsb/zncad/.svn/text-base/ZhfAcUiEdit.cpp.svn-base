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
//----- ZhfAcUiEdit.cpp : Implementation of CZhfAcUiEdit
//-----------------------------------------------------------------------------
#include "StdAfx.h"
//#include "resource.h"
#include "ZhfAcUiEdit.h"
#include "../blkdll/BlkUtitily.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CZhfAcUiEdit, CAcUiEdit)

BEGIN_MESSAGE_MAP(CZhfAcUiEdit, CAcUiEdit)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CZhfAcUiEdit::CZhfAcUiEdit ()  
: m_strRegex(_T(""))
, m_strOld(_T(""))
, m_bFreezeText(FALSE)
{
}

//-----------------------------------------------------------------------------
CZhfAcUiEdit::~CZhfAcUiEdit () {
}

void CZhfAcUiEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//  	CString strmsg ;
// 	strmsg.Format(_T("%d"), nChar) ;
// 	AfxMessageBox(strmsg) ;
// 	if (nChar>32&&nChar<127&&!m_strRegex.IsEmpty())
// 	{
// 		CString strText ;
// 		this->GetWindowText(strText) ;
// 		strText.AppendFormat(_T("%c"), (TCHAR)nChar) ;
// 		BOOL bIsMatch = CBlkUtility::IsStrMatch(m_strRegex, strText) ;
// 		if (!bIsMatch)
// 		{
// 			ShowBalloonTip(m_strTooltipTitle, m_strTooltipText, TTI_INFO) ;
// 			return ;
// 		}
// 	}

	CAcUiEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CZhfAcUiEdit::SetValidate(const CString& strRegex, const CString& strTooltipTitle, const CString& strTooltipText)
{
	m_strRegex = strRegex ;
	m_strTooltipTitle = strTooltipTitle.AllocSysString() ;
	m_strTooltipText = strTooltipText.AllocSysString() ;
}

void CZhfAcUiEdit::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CAcUiEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (this->IsWindowVisible())
	{
		CString strText ;
		this->GetWindowText(strText) ;

		if (m_strOld==strText) 
		{
			return ;
		}
		//else
		{ 
			if (!strText.IsEmpty()&&!CBlkUtility::IsStrMatch(m_strRegex, strText) )
			{
				ShowBalloonTip(m_strTooltipTitle, m_strTooltipText, TTI_INFO) ;
				this->SetWindowText(m_strOld) ;
				this->SetSel(-1) ;
				return ;
			}
			m_strOld = strText ;
			::SendMessage(this->GetParent()->m_hWnd, ZHFMSG_ZHFEDIT_TEXTCHANGED,(WPARAM)this->m_hWnd, 0 ) ;
		}
	}
}

void CZhfAcUiEdit::GetInt(int& i)
{
	CString strText ;
	this->GetWindowText(strText) ;
	if (!strText.IsEmpty()&&CBlkUtility::IsStrMatch(_T("^-?[1-9]+[0-9]*$"), strText) )
	{
		i = _ttoi(strText) ;
	}
}

void CZhfAcUiEdit::GetDouble(double& d)
{
	CString strText ;
	this->GetWindowText(strText) ;
	if (!strText.IsEmpty()&&CBlkUtility::IsStrMatch(_T("^-?(0|[1-9]+[0-9]*)(\\.\\d*)?$"), strText) )
	{
		d = _tstof(strText) ;
	}
}

void CZhfAcUiEdit::SetInt(const int& i)
{
	if (!m_bFreezeText)
	{
		CString strText ;
		strText.Format(_T("%d"), i) ;
		this->SetWindowText(strText) ;
		m_strOld = strText ;
	}
}

void CZhfAcUiEdit::SetDouble(const double& d, int iDeci, BOOL bTrim0)
{
	if (!m_bFreezeText)
	{
		CString strText ;
		if (iDeci==0)
		{
			strText.Format(_T("%.0f"), d) ;
		}
		else if (iDeci==1)
		{
			strText.Format(_T("%.1f"), d) ;
		}
		else if (iDeci==2)
		{
			strText.Format(_T("%.2f"), d) ;
		}
		else if (iDeci==3)
		{
			strText.Format(_T("%.3f"), d) ;
		}
		if (bTrim0)
		{
			if (iDeci>0)
			{
				strText.TrimRight(_T("0")) ;
				strText.TrimRight(_T('.')) ;
			}
		}
		this->SetWindowText(strText) ;
		m_strOld = strText ;
	}
}

void CZhfAcUiEdit::SetFreeze(BOOL bFreeze)
{
	m_bFreezeText = bFreeze ;
}

BOOL CZhfAcUiEdit::GetFreeze()
{
	return m_bFreezeText ;
}

void CZhfAcUiEdit::SetWindowText(LPCTSTR lpszString)
{
	if (!m_bFreezeText)
	{
		CAcUiEdit::SetWindowText(lpszString) ;
	}
}

void CZhfAcUiEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DWORD dw1 = this->GetStyle() ;
	DWORD dw2 = dw1&ES_READONLY ;
	DWORD dw3 = (DWORD)ES_READONLY ;
	SetReadOnly(dw2==dw3?FALSE:TRUE) ;
    SetFreeze(!m_bFreezeText) ;
	CAcUiEdit::OnLButtonDblClk(nFlags, point);
}
