#pragma once
#include "Resource.h"
#include "xhentugctrl.h"


// CEntPropertyDlg �Ի���

class CEntPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CEntPropertyDlg)

public:
	CEntPropertyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntPropertyDlg();

// �Ի�������
	enum { IDD = IDD_MOD_ENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CXhEntUGCtrl m_grid;
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
};
