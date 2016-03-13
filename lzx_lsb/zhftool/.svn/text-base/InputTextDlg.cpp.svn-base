// InputTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


double g_inputtext_dXOffSet = 0 ;
double g_inputtext_dYOffSet = 0;
double g_inputtext_dHeight = 2.0 ;
double g_inputtext_dWidth = 0.7 ;
double g_inputtext_dRotation = 0 ;
int g_inputtext_iColorIndex = 8 ;
int g_inputtext_iJustifyIndex = 0 ;
CString g_inputtext_strStyle = _T("默认\(STANDARD\)") ;


/////////////////////////////////////////////////////////////////////////////
// CInputTextDlg dialog

CInputTextDlg::CInputTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputTextDlg)
	m_dHeight = g_inputtext_dHeight ;
	m_dRotation = g_inputtext_dRotation ;
	m_strText = _T("");
	m_dWidth = g_inputtext_dWidth ;
	m_dXOffSet = g_inputtext_dXOffSet ;
	m_dYOffSet = g_inputtext_dYOffSet ;
	m_bSet = FALSE;
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
}


void CInputTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputTextDlg)
	DDX_Control(pDX, IDC_COMBO_INPUTTEXT_COLOR, m_ColorIndex);
	DDX_Control(pDX, IDC_COMBOIDC_INPUTTEXT_JUSTIFY, m_JustifyIndex);
	DDX_Control(pDX, IDC_COMBO_INPUTTEXT_STYLE, m_StyleIndex);
	DDX_Text(pDX, IDC_EDIT_INPUTTEXT_HEIGHT, m_dHeight);
	DDX_Text(pDX, IDC_EDIT_INPUTTEXT_ROTATION, m_dRotation);
	DDX_Text(pDX, IDC_EDIT_INPUTTEXT_TEXT, m_strText);
	DDX_Text(pDX, IDC_EDIT_INPUTTEXT_WIDTH, m_dWidth);
	DDX_Text(pDX, IDC_EDIT_INPUTTEXT_X_OFFSET, m_dXOffSet);
	DDX_Text(pDX, IDC_EDIT_INPUTTEXT_Y_OFFSET, m_dYOffSet);
	DDX_Check(pDX, IDC_CHECK_INPUTTEXT_SET, m_bSet);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputTextDlg, CDialog)
	//{{AFX_MSG_MAP(CInputTextDlg)
	ON_BN_CLICKED(IDC_CHECK_INPUTTEXT_SET, OnCheckInputtextSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputTextDlg message handlers

BOOL CInputTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_ColorIndex.SetCurSel(g_inputtext_iColorIndex) ;
	this->m_JustifyIndex.SetCurSel(g_inputtext_iJustifyIndex) ;

	this->m_StyleIndex.AddString(_T("默认\(STANDARD\)")) ;
	for(int i=0; i<m_strArrayTextStyle.GetSize(); i++)
	{
		this->m_StyleIndex.AddString(m_strArrayTextStyle.GetAt(i)) ;
	}
	int iStyleIndex = m_StyleIndex.FindString(-1, g_inputtext_strStyle) ;
	if (iStyleIndex==CB_ERR) 
	{
		iStyleIndex = 0 ;
	}
	this->m_StyleIndex.SetCurSel(iStyleIndex) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInputTextDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.cx = 500 ;
	//cs.cy = 500 ;
	
	return CDialog::PreCreateWindow(cs);
}

void CInputTextDlg::OnCheckInputtextSet() 
{
	// TODO: Add your control notification handler code here
	CEdit * pEdit_XOffSet = (CEdit *)GetDlgItem(IDC_EDIT_INPUTTEXT_X_OFFSET) ;
	CEdit * pEdit_YOffSet = (CEdit *)GetDlgItem(IDC_EDIT_INPUTTEXT_Y_OFFSET) ;
	CEdit * pEdit_Height = (CEdit *)GetDlgItem(IDC_EDIT_INPUTTEXT_HEIGHT) ;
	CEdit * pEdit_WidthFactor = (CEdit *)GetDlgItem(IDC_EDIT_INPUTTEXT_WIDTH) ;
	CEdit * pEdit_Rotation = (CEdit *)GetDlgItem(IDC_EDIT_INPUTTEXT_ROTATION) ;
	CComboBox * pComboBox_Color = (CComboBox *)GetDlgItem(IDC_COMBO_INPUTTEXT_COLOR) ;
	CComboBox * pComboBox_Justify = (CComboBox *)GetDlgItem(IDC_COMBOIDC_INPUTTEXT_JUSTIFY) ;
	CComboBox * pComboBox_Style = (CComboBox *) GetDlgItem(IDC_COMBO_INPUTTEXT_STYLE) ;
	
	pEdit_XOffSet->EnableWindow(!pEdit_XOffSet->IsWindowEnabled()) ;
	pEdit_YOffSet->EnableWindow(!pEdit_YOffSet->IsWindowEnabled()) ;	
	pEdit_Height->EnableWindow(!pEdit_Height->IsWindowEnabled()) ;
	pEdit_WidthFactor->EnableWindow(!pEdit_WidthFactor->IsWindowEnabled()) ;
	pEdit_Rotation->EnableWindow(!pEdit_Rotation->IsWindowEnabled()) ;	
	pComboBox_Color->EnableWindow(!pComboBox_Color->IsWindowEnabled()) ;
	pComboBox_Justify->EnableWindow(!pComboBox_Justify->IsWindowEnabled()) ;
	pComboBox_Style->EnableWindow(!pComboBox_Style->IsWindowEnabled()) ;
	
	
}

void CInputTextDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	this->UpdateData(TRUE) ;
	if(m_dHeight<=0)
	{
		this->m_strMsg = _T("Height必须大于0") ;
		this->UpdateData(FALSE) ;
	}
	else if (m_dWidth<=0)
	{
		this->m_strMsg = _T("Width Factor 必须大于0") ;
		this->UpdateData(FALSE) ;
	}
	else 
	{
		g_inputtext_dXOffSet = this->m_dXOffSet ;
		g_inputtext_dYOffSet = this->m_dYOffSet ;
		g_inputtext_dHeight = this->m_dHeight ;
		g_inputtext_dWidth = this->m_dWidth ;
		g_inputtext_dRotation = this->m_dRotation ;
		g_inputtext_iColorIndex = this->m_ColorIndex.GetCurSel() ;
		g_inputtext_iJustifyIndex = this->m_JustifyIndex.GetCurSel() ;
		//CString strTextOfStyle ;
		
		this->m_StyleIndex.GetLBText(this->m_StyleIndex.GetCurSel(), m_strTextStyle) ;
		if (m_strTextStyle==_T("默认\(STANDARD\)")) 
		{
			m_strTextStyle=_T("STANDARD") ;
		}
		g_inputtext_strStyle = m_strTextStyle ;	
		
		CDialog::OnOK();
		
	}


}
