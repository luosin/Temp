// EditFreqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditFreqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditFreqDlg dialog


CEditFreqDlg::CEditFreqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFreqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditFreqDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CEditFreqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditFreqDlg)
	DDX_Control(pDX, IDC_LIST_FREQ, m_listCtrFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditFreqDlg, CDialog)
	//{{AFX_MSG_MAP(CEditFreqDlg)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_FREQ, OnEndlabeleditListFreq)
	//}}AFX_MSG_MAP
	ON_NOTIFY(HDN_ENDTRACK, 0, OnEndtrack)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditFreqDlg message handlers

BOOL CEditFreqDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_imlNull.Create(20, 20, ILC_COLORDDB|ILC_MASK, 2, 2); 
	m_listCtrFreq.SetImageList(&m_imlNull, LVSIL_SMALL);
	m_listCtrFreq.InsertColumn(0, _T("Ãû³Æ"), LVCFMT_CENTER, 90) ;
	m_listCtrFreq.InsertColumn(1, _T("ÆµÂÊ"), LVCFMT_CENTER, 90) ;

	CString strData ;
	int iRow = m_strArrayGuiDao.GetSize() ;
	for (int i=0; i<iRow; i++)
	{
		m_listCtrFreq.InsertItem(i, strData) ;
		
		strData = m_strArrayGuiDao.GetAt(i) ;

		m_listCtrFreq.SetItemText(i, 0, strData) ;
		m_listCtrFreq.SetItemText(i, 1, m_strArrayFreq.GetAt(i)) ;
	}


	m_listCtrFreq.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT) ;


	m_listCtrFreq.SetItemControl(COMBOBOX,1);

	m_listCtrFreq.MapControlsForSubItems();
	m_listCtrFreq.ShowControls(0);
	m_listCtrFreq.EnableControls(TRUE);

	CString strFreq[8] = {_T("1700-1"), _T("2300-1"), _T("1700-2"), _T("2300-2"),_T("2000-1"), _T("2600-1"), _T("2000-2"), _T("2600-2")} ;
	for (int iFreq=0; iFreq<8; iFreq++)
	{
		m_listCtrFreq.AddToControlsData(1,strFreq[iFreq]);
		m_listCtrFreq.MapControlsForSubItems();
		m_listCtrFreq.ShowControls(0);
		m_listCtrFreq.EnableControls(TRUE);
	}


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditFreqDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	/*
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	*/
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CEditFreqDlg::OnEndlabeleditListFreq(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CEditFreqDlg::OnEndtrack(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	NMHEADER * pHeade = (NMHEADER *) pNMHDR;
	m_listCtrFreq.AdjustControlsAccordingToHeaderWidth (phdn);
	*pResult = 0;
} 

void CEditFreqDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	int iGuiDaoNum = m_strArrayGuiDao.GetSize() ;
	m_strArrayFreq.RemoveAll() ;
	CString strMsg("") ;
	for (int i=0; i<iGuiDaoNum; i++)
	{
		CString  strFreqTmp ; //strGuiDaoTmp ;
		//strGuiDaoTmp = m_listCtrFreq.GetItemText(i,0) ;
		strFreqTmp = m_listCtrFreq.GetItemText(i,1) ;
		m_strArrayFreq.Add(strFreqTmp) ;

		//strMsg.Format("%d:%s-%s", i, strGuiDaoTmp, strFreqTmp) ;
		//AfxMessageBox(strMsg) ;

	}
	
	CDialog::OnOK();
}
