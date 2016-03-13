// STextSet.cpp : implementation file
//

#include "stdafx.h"
#include "STextSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTextSet dialog

CString g_strSTxtSetFront =_T("") ;
CString g_strSTxtSetBack =_T("") ;
int g_iSTxtSetStart = 0 ;
int g_iSTxtSetIncrement = 1 ;
int g_iSTxtSetRepeat = 1 ;
int g_iSTxtSetSelectMode = 1 ;
int g_iSTxtSetSortBy = 1 ;


CSTextSet::CSTextSet(CWnd* pParent /*=NULL*/)
	: CDialog(CSTextSet::IDD, pParent)	
{
	//{{AFX_DATA_INIT(CSTextSet)
	m_iStartNum = g_iSTxtSetStart ;
	m_strFrontTxt = g_strSTxtSetFront ;
	m_strBackTxt = g_strSTxtSetBack ;
	m_iIncreNum = g_iSTxtSetIncrement ;
	m_iRepeat = g_iSTxtSetRepeat ;
	m_iSelectMode = g_iSTxtSetSelectMode ;
	m_iSortBy = g_iSTxtSetSortBy ;
	//}}AFX_DATA_INIT
}


void CSTextSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTextSet)
	DDX_Text(pDX, IDC_EDIT_StartNum, m_iStartNum);
	DDX_Text(pDX, IDC_EDIT_FrontTxt, m_strFrontTxt);
	DDX_Text(pDX, IDC_EDIT_BackTxt, m_strBackTxt);
	DDX_Text(pDX, IDC_EDIT_IncreNum, m_iIncreNum);
	DDX_Text(pDX, IDC_EDIT_Repeat, m_iRepeat);
	DDV_MinMaxUInt(pDX, m_iRepeat, 1, 10000);
	DDX_Radio(pDX, IDC_RADIO_STEXT_WINDOW, m_iSelectMode);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_STEXT_X, m_iSortBy);
}


BEGIN_MESSAGE_MAP(CSTextSet, CDialog)
	//{{AFX_MSG_MAP(CSTextSet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_STEXT_WINDOW, OnBnClickedRadioStextWindow)
	ON_BN_CLICKED(IDC_RADIO_STEXT_PICK, OnBnClickedRadioStextPick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTextSet message handlers

void CSTextSet::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData(TRUE) ;

	g_iSTxtSetStart = m_iStartNum ;
	g_strSTxtSetFront = m_strFrontTxt  ;
	g_strSTxtSetBack = m_strBackTxt ;
	g_iSTxtSetIncrement = m_iIncreNum ;
	g_iSTxtSetRepeat = m_iRepeat ;
	g_iSTxtSetSelectMode = m_iSelectMode ;
	g_iSTxtSetSortBy = m_iSortBy ;
	
	CDialog::OnOK();
}

BOOL CSTextSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for (int i=0;i<2; i++)
	{
		CButton *pButton = (CButton *)this->GetDlgItem(IDC_RADIO_STEXT_X+i) ;		
		pButton->EnableWindow(m_iSelectMode==0?TRUE:FALSE) ;
	}

	this->UpdateData(false) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSTextSet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
		{			
			this->NextDlgCtrl() ;
			return TRUE ;
			
		}
	}
	
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSTextSet::OnBnClickedRadioStextWindow()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iSelectMode = 0 ;

	for (int i=0;i<2; i++)
	{
		CButton *pButton = (CButton *)this->GetDlgItem(IDC_RADIO_STEXT_X+i) ;		
		pButton->EnableWindow(m_iSelectMode==0?TRUE:FALSE) ;
	}
}

void CSTextSet::OnBnClickedRadioStextPick()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iSelectMode = 1 ;
	for (int i=0;i<2; i++)
	{
		CButton *pButton = (CButton *)this->GetDlgItem(IDC_RADIO_STEXT_X+i) ;		
		pButton->EnableWindow(m_iSelectMode==0?TRUE:FALSE) ;
	}
}
