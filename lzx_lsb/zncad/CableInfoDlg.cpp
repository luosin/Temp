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
//----- CableInfoDlg.cpp : Implementation of CCableInfoDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CableInfoDlg.h"
#include "ZnUtility.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CCableInfoDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CCableInfoDlg, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_MESSAGE(ZHFMSG_ZHFEDIT_TEXTCHANGED, OnTextChanged)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CCableInfoDlg::CCableInfoDlg (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CCableInfoDlg::IDD, pParent, hInstance) 
, m_strCableAll(_T(""))
, m_strCableIntoXhlName(_T(""))
, m_strCableTypeName(_T(""))
, m_nBackup(0)
, m_strCableSn(_T(""))
, m_dCoefficient(1.05)
, m_iPerIntoXhl(50)
, m_iPerGd(10)
, m_iPerBetweenGd(10)
, m_iPerBox(4)
, m_iPerBoxF(4)
, m_iPerBetweenBoxF(500)
// , m_iLenForPlus(0)
{
	CZnUtility::GetCableParam(m_dCoefficient, m_iPerIntoXhl, m_iPerGd, m_iPerBetweenGd, m_iPerBox, m_iPerBoxF, m_iPerBetweenBoxF) ;
}

CCableInfoDlg::~CCableInfoDlg()
{
}

//-----------------------------------------------------------------------------
void CCableInfoDlg::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_ALL, m_editCable);	
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_LEN, m_editLen);	
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_DIST1, m_editDist1);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_DIST2, m_editDist2);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_BOXNUM, m_editBoxNum);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_GDNUM, m_editGdNum);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_PLUS, m_editPlus);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_BOXFNUM, m_editBoxFNum);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_INTOXHL, m_editIntoXhl);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_TYPE, m_editType);
	DDX_Control(pDX, IDC_EDIT_CABLEINFO_BACKUP, m_editBackup);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_COEFFICIENT, m_dCoefficient);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_PERINTOXHL, m_iPerIntoXhl);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_PERGD, m_iPerGd);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_PERBETWEENGD, m_iPerBetweenGd);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_PERBOX, m_iPerBox);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_PERBOXF, m_iPerBoxF);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_PERBETWEENBOXF, m_iPerBetweenBoxF);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_SN, m_strCableSn);
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_INTOXHLNAME, m_strCableIntoXhlName);	
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_OLD, m_strCableOld);	
	DDX_Text(pDX, IDC_EDIT_CABLEINFO_LENFORPLUS, m_iLenForPlus);

}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CCableInfoDlg::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

BOOL CCableInfoDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strTipTitle = _T("提示：") ;
	CString strRegInt = _T("^-?[0-9]+[0-9]*$") ;
	CString strTipInt = _T("请输入整数!") ;
	CString strRegDouble = _T("^-?(0|[1-9]+[0-9]*)(\\.\\d{0,3})?$") ; //小数部分不超过3位
	CString strTipDouble = _T("请输入小数位数最多3位的小数!") ;

	for (int i=IDC_EDIT_CABLEINFO_ALL; i<=IDC_EDIT_CABLEINFO_BACKUP; i++)
	{
		CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(i) ;
		if (i==IDC_EDIT_CABLEINFO_ALL)
		{			
		}
		else if (i==IDC_EDIT_CABLEINFO_TYPE)
		{
			pEdit->SetValidate(_T("^\\d+[A-Z]*$"), strTipTitle, _T("输入示例:4、16A")) ;
		}
		else 
		{
			pEdit->SetValidate(strRegInt, strTipTitle, strTipInt) ;
		}
// 		else
// 		{
// 			pEdit->SetValidate(strRegDouble, strTipTitle, strTipDouble) ;
// 			pEdit->SetWindowText(_T("0")) ;
// 		}
	}

	this->SetEditText() ;

	CRect rectClient ;
	GetClientRect(rectClient) ;		
	//rectClient.DeflateRect(5,0,5,0) ;
	rectClient.left =rectClient.Width()/2+5 ;	
	rectClient.right =rectClient.right-15 ;	
	rectClient.bottom-=68 ;
	m_grid.CreateGrid( WS_CHILD|WS_VISIBLE, rectClient, this, IDC_LIST_GRID );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

BOOL CCableInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
// 	if(WM_CHAR == pMsg->message)
// 	{
// 		if(m_editCoefficient.GetSafeHwnd() == pMsg->hwnd)
// 		{
// 			char ch = (TCHAR) pMsg->wParam;
// 
// 			if(ch == '.'  ||(ch >='0' && ch <='9'))
// 				return CDialog::PreTranslateMessage(pMsg);
// 			else
// 				return true;
// 		}
// 	}		

	return CAcUiDialog::PreTranslateMessage(pMsg);
}
LRESULT CCableInfoDlg::OnTextChanged(WPARAM wparam,LPARAM lparam)
{
	HWND hWnd = (HWND)wparam ;

	CString strText ;	
	if (hWnd==m_editDist1.m_hWnd||hWnd==m_editDist2.m_hWnd||hWnd==m_editGdNum.m_hWnd||hWnd==m_editBoxNum.m_hWnd||hWnd==m_editPlus.m_hWnd)
	{
		int iNumBoxF = ComputeNumBoxF() ;
		m_editBoxFNum.SetInt(iNumBoxF) ;		

		int iDist1 = 0, iDist2 = 0, iNumGd = 0, iNumBox = 0, iPlus = 0,  iIntoXhl = 0 ;
		m_editDist1.GetInt(iDist1) ;
		m_editDist2.GetInt(iDist2) ;
		m_editGdNum.GetInt(iNumGd) ;
		m_editBoxNum.GetInt(iNumBox) ;
		m_editPlus.GetInt(iPlus) ;
		//m_editBoxFNum.GetInt(iNumBoxF) ;
		m_editIntoXhl.GetInt(iIntoXhl) ;
		int iLen = (abs(iDist1-iDist2)+iNumGd*m_iPerGd+(iNumGd>1?(iNumGd-1)*m_iPerBetweenGd:0)+iNumBox*m_iPerBox+iPlus+iNumBoxF*m_iPerBoxF+iIntoXhl) * m_dCoefficient ;
		m_editLen.SetInt(iLen) ;
	}
	else if (hWnd==m_editType.m_hWnd||hWnd==m_editBackup.m_hWnd)
	{
		CString strType ;
		int i1 = 0 ;
		m_editType.GetWindowText(strType) ;
		m_editBackup.GetInt(i1) ;		
		int iLen1 = 0 ;
		m_editLen.GetInt(iLen1) ;
		int iLen = (iLen1/10)*10+((iLen1%10)<5?5:10) ;
		strText.Format(_T("%d-%s(%d)"), iLen, strType, i1) ;
		m_editCable.SetWindowText(strText) ;					
	}
	else if (hWnd==m_editLen.m_hWnd)
	{
		int iLen1= 0 ;
		m_editLen.GetInt(iLen1) ;
		int iLen = (iLen1/10)*10+((iLen1%10)<5?5:10) ;
		CString strType ;		
		m_editType.GetWindowText(strType) ;
		int iBackup = 0 ;
		m_editBackup.GetInt(iBackup) ;		
		strText.Format(_T("%d-%s(%d)"), iLen, strType, iBackup) ;
		m_editCable.SetWindowText(strText) ;				
	}

	return 0 ;
}

int CCableInfoDlg::SetEditText()
{	
	int iCount = m_nArrayData.GetCount() ;
	if (iCount<7)
	{
		acutPrintf(_T("\n电缆信息对话框设置文本失败，因为存储数据数组小于预定个数6")) ;
		return 0 ;
	}
	int iDist1 = m_nArrayData.GetAt(0) ;
	int iDist2 = m_nArrayData.GetAt(1) ;
	int iDist = abs(iDist1-iDist2);
	int i2 = m_nArrayData.GetAt(2) ;
	int i3 = m_nArrayData.GetAt(3) ;
	int i4 = m_nArrayData.GetAt(4) ;
	int i5 = m_nArrayData.GetAt(5) ;
	int i6 = m_nArrayData.GetAt(6) ;
	int iLen1 = (iDist+i2*m_iPerGd+(i2>1?(i2-1)*m_iPerBetweenGd:0)+i3*m_iPerBox+i4+i5*m_iPerBoxF+i6) * m_dCoefficient ;
	int iLen = (iLen1/10)*10+((iLen1%10)<5?5:10) ;

	for (int i=IDC_EDIT_CABLEINFO_ALL; i<=IDC_EDIT_CABLEINFO_INTOXHL; i++)
	{
		CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(i) ;
		if (i==IDC_EDIT_CABLEINFO_ALL)
		{
			CString strCableAllTmp ;
			strCableAllTmp.Format(_T("%d-%s(%d)"), iLen, m_strCableTypeName, m_nBackup) ;
			pEdit->SetWindowText(strCableAllTmp) ;
		}
		else if (i==IDC_EDIT_CABLEINFO_LEN)
		{
			pEdit->SetInt(iLen1) ;
		}
		else
		{
			pEdit->SetInt(m_nArrayData.GetAt(i-IDC_EDIT_CABLEINFO_DIST1)) ;
		}
	}
	m_editType.SetWindowText(m_strCableTypeName) ;
	m_editBackup.SetInt(m_nBackup) ;

	return 0;
}

void CCableInfoDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_editCable.GetWindowText(m_strCableAll) ;
	m_nArrayData.RemoveAll() ;
	for (int i=IDC_EDIT_CABLEINFO_DIST1; i<=IDC_EDIT_CABLEINFO_INTOXHL; i++)
	{
		CZhfAcUiEdit* pEdit = (CZhfAcUiEdit*)this->GetDlgItem(i) ;
		int iTmp = 0 ;
		pEdit->GetInt(iTmp) ;
		m_nArrayData.Add(iTmp) ;
	}

	CAcUiDialog::OnOK();
}

// 计算接续盒的数量
int CCableInfoDlg::ComputeNumBoxF(void)
{
	int iDist1 = 0, iDist2 = 0, iNumCrossGd = 0, iNumBox = 0, iNumPlus = 0, iIntoXhl = 0 ;
	m_editDist1.GetInt(iDist1) ;
	m_editDist2.GetInt(iDist2) ;
	m_editGdNum.GetInt(iNumCrossGd) ;
	m_editBoxNum.GetInt(iNumBox) ;
	m_editPlus.GetInt(iNumPlus) ;
	m_editIntoXhl.GetInt(iIntoXhl) ;

	int iNumBoxF = 0 ;

	int iDist = abs(iDist1-iDist2) ;	
	int iLen1 =(iDist+iNumCrossGd*m_iPerGd+(iNumCrossGd>1?(iNumCrossGd-1)*m_iPerBetweenGd:0)+iNumBox*m_iPerBox+iNumPlus+iIntoXhl) * m_dCoefficient ;
	int iNumBoxF1 = iLen1/m_iPerBetweenBoxF ;
	iNumBoxF = iNumBoxF1 ;
	int iLen2 = (iDist+iNumCrossGd*m_iPerGd+(iNumCrossGd>1?(iNumCrossGd-1)*m_iPerBetweenGd:0)+iNumBox*m_iPerBox+iNumPlus+iNumBoxF1*m_iPerBoxF+iIntoXhl) * m_dCoefficient ;
	int iNumBoxF2 = iLen2/m_iPerBetweenBoxF ;
	if (iNumBoxF2>iNumBoxF1)
	{
		iNumBoxF = iNumBoxF2 ;
	}
// 	else //再加上一个接续试试
// 	{
// 		int iLen3 = (iDist+iNumCrossGd*m_iPerGd+(iNumCrossGd>1?(iNumCrossGd-1)*m_iPerBetweenGd:0)+iNumBox*m_iPerBox+iNumPlus+(iNumBoxF2+1)*m_iPerBoxF+iIntoXhl) * m_dCoefficient ;
// 		int iNumBoxF3 = iLen3/m_iPerBetweenBoxF ;
// 		if (iNumBoxF3>iNumBoxF2)
// 		{
// 			iNumBoxF = iNumBoxF3 ;
// 		}
// 	}
	return iNumBoxF ;
}
