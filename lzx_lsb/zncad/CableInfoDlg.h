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
//----- CableInfoDlg.h : Declaration of the CCableInfoDlg
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//#include "resource.h"
#include "ZhfAcUiEdit.h"
#include "afxwin.h"
#include "CableCoreListUGCtrl.h"

//-----------------------------------------------------------------------------
class CCableInfoDlg : public CAcUiDialog {
	DECLARE_DYNAMIC (CCableInfoDlg)

public:
	CCableInfoDlg (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;
	~CCableInfoDlg() ;

	enum { IDD = IDD_CABLEINFO} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	afx_msg LRESULT OnTextChanged(WPARAM wparam,LPARAM lparam);

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
public:
	CZhfAcUiEdit m_editCable;
	CZhfAcUiEdit m_editLen;	
	CZhfAcUiEdit m_editDist1;
	CZhfAcUiEdit m_editDist2;
	CZhfAcUiEdit m_editBoxNum;
	CZhfAcUiEdit m_editGdNum;
	CZhfAcUiEdit m_editPlus;
	CZhfAcUiEdit m_editType;
	CZhfAcUiEdit m_editBackup;		
	CZhfAcUiEdit m_editBoxFNum;
	CZhfAcUiEdit m_editIntoXhl ;	

	double m_dCoefficient;
	int m_iPerIntoXhl;
	int m_iPerGd;
	int m_iPerBetweenGd;
	int m_iPerBox;
	int m_iPerBoxF;
	int m_iPerBetweenBoxF;

    //电缆编号
	CString m_strCableSn;
	//电缆入楼名称，比如GD-2
	CString m_strCableIntoXhlName;
	CString m_strCableOld;
	int m_iLenForPlus; 	// 转辙机加芯距离

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int SetEditText();
	//CArray<double, double&>* m_pArrayEditDouble;
	CUIntArray m_nArrayData ;
	//CArray<CString, CString&>* m_pArrayEditString;
	//完整电缆字符串
	CString m_strCableAll;
	// 电缆型号名称比如12B
	CString m_strCableTypeName;
	// 备用芯数
	UINT m_nBackup;

	CCableCoreListUGCtrl m_grid;
	// 计算接续盒的数量
	int ComputeNumBoxF(void);
} ;
