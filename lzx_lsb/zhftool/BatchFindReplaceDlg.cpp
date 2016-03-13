// BatchFindReplaceDlg.cpp : implementation file
//

#include "StdAfx.h"
//#include "resource.h"
#include "BatchFindReplaceDlg.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchFindReplaceDlg dialog


CBatchFindReplaceDlg::CBatchFindReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchFindReplaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBatchFindReplaceDlg)
	m_bUseAtt = FALSE;
	m_strAtt = _T("");
	m_strFold = _T("");
	m_strNewBlk = _T("");
	m_strOldBlk = _T("");
	m_iSelOption = 0;
	//}}AFX_DATA_INIT
}


void CBatchFindReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchFindReplaceDlg)
	DDX_Check(pDX, IDC_CHECK_FRB_ATT, m_bUseAtt);
	DDX_Text(pDX, IDC_EDIT_FRB_ATT, m_strAtt);
	DDX_Text(pDX, IDC_EDIT_FRB_FOLD, m_strFold);
	//DDX_Text(pDX, IDC_EDIT_FRB_NEWBLK, m_strNewBlk);
	//DDX_Text(pDX, IDC_EDIT_FRB_OLDBLK, m_strOldBlk);
	DDX_Radio(pDX, IDC_RADIO_FRB_SEL, m_iSelOption);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FRB_OLDBLK, m_comboOldBlk);
	DDX_Control(pDX, IDC_COMBO_FRB_NEWBLK, m_comboNewBlk);
}


BEGIN_MESSAGE_MAP(CBatchFindReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CBatchFindReplaceDlg)	
	ON_COMMAND_RANGE(IDC_RADIO_FRB_SEL, IDC_RADIO_FRB_FOLD, OnRadioFrbSel)
	ON_BN_CLICKED(IDC_CHECK_FRB_ATT, OnCheckFrbAtt)
	ON_BN_CLICKED(IDC_BUTTON_FRB_SELNEWDWG, OnButtonFrbSelnewdwg)
	ON_BN_CLICKED(IDC_BUTTON_FRB_SELFOLD, OnButtonFrbSelfold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchFindReplaceDlg message handlers

void CBatchFindReplaceDlg::OnRadioFrbSel(UINT nID) 
{
	CButton *pButton = (CButton *)this->GetDlgItem(IDC_BUTTON_FRB_SELFOLD) ;
	pButton->EnableWindow(nID==IDC_RADIO_FRB_FOLD?TRUE:FALSE) ;
	
}

BOOL CBatchFindReplaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBlkUtility::QueryValueReg(_T("ReplaceBlock"), _T("OldBlockName"), m_strOldBlk) ;
	CBlkUtility::QueryValueReg(_T("ReplaceBlock"), _T("NewBlockName"), m_strNewBlk) ;
	CBlkUtility::QueryValueReg(_T("ReplaceBlock"), _T("NewBlockFold"), m_strFold) ;
	CBlkUtility::QueryValueReg(_T("ReplaceBlock"), _T("Att"), m_strAtt) ;
	this->UpdateData(FALSE) ;
	CUserSystem mysys ;
	mysys.m_pDb = acdbHostApplicationServices()->workingDatabase() ;
	CStringArray strArrayBlkName ;
	mysys.GetAllBlockTable(strArrayBlkName, 1) ;
	for (int i=0; i<strArrayBlkName.GetCount(); i++)
	{
		CString strBlkName = strArrayBlkName.GetAt(i) ;
		m_comboOldBlk.AddString(strBlkName) ;
		if (m_strOldBlk==strBlkName)
		{
			m_comboOldBlk.SetCurSel(i) ;
		}
		m_comboNewBlk.AddString(strBlkName) ;
		if (m_strNewBlk==strBlkName)
		{
			m_comboNewBlk.SetCurSel(i) ;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBatchFindReplaceDlg::OnCheckFrbAtt() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE) ;

	CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_FRB_ATT) ;
	pEdit->EnableWindow(m_bUseAtt) ;
	if (m_bUseAtt)
	{
		pEdit->SetFocus() ;
	}
	
}

void CBatchFindReplaceDlg::OnButtonFrbSelnewdwg() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true,NULL,NULL,NULL,_T("(图形文件(*.dwg)|*.*|"),NULL);
	filedlg.m_ofn.lpstrTitle = _T("选择图块文件") ;
	if (filedlg.DoModal()==IDOK)
	{
		CString strFilePath = filedlg.GetPathName() ;
		this->m_strNewBlk = strFilePath ;
		int iIndex = m_comboNewBlk.AddString(strFilePath) ;
		m_comboNewBlk.SetCurSel(iIndex) ;

		this->UpdateData(FALSE) ;
	}
	
}

void CBatchFindReplaceDlg::OnButtonFrbSelfold() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData() ;
	
	CSBDestination sb(m_hWnd, IDS_BFF_TITLE) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if (sb.SelectFolder())
	{
		this->m_strFold = sb.GetSelectedFolder() ;
		this->UpdateData(FALSE) ;
	}
	
}

void CBatchFindReplaceDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	this->UpdateData(TRUE) ;

	m_comboOldBlk.GetLBText(m_comboOldBlk.GetCurSel(), m_strOldBlk) ;
	m_comboNewBlk.GetLBText(m_comboNewBlk.GetCurSel(), m_strNewBlk) ;

	CBlkUtility::SetValueReg(_T("ReplaceBlock"), _T("OldBlockName"), m_strOldBlk) ;
	CBlkUtility::SetValueReg(_T("ReplaceBlock"), _T("NewBlockName"), m_strNewBlk) ;
	CBlkUtility::SetValueReg(_T("ReplaceBlock"), _T("NewBlockFold"), m_strFold) ;
	CBlkUtility::SetValueReg(_T("ReplaceBlock"), _T("Att"), m_strAtt) ;

	if (m_strOldBlk==_T("")||m_strNewBlk==_T(""))
	{
		AfxMessageBox(_T("块名称不能为空！")) ;
		return ;
	}
	else if (m_iSelOption==2 && m_strFold==_T(""))
	{
		AfxMessageBox(_T("请选择文件夹！")) ;
		return ;
	}
	else if (m_bUseAtt && CBlkUtility::IsStrMatch(_T("^[^,;]+,[^,;]+(;[^,;]+,[^,;]+)*$"), m_strAtt)==FALSE)
	{
		AfxMessageBox(_T("属性对应的格式要满足类似:旧块属性a,新块属性b;旧块属性c,新块属性d......")) ;
		return ;
	}

	if (m_bUseAtt)
	{
		this->GetMapStrToStrAtt() ;
	}	


	CDialog::OnOK();
}

int CBatchFindReplaceDlg::GetMapStrToStrAtt()
{
	CUserSystem mysys ;
	int iRet = 0 ;
		
	CStringArray strArrayAttMap ;
	CBlkUtility::DivideString(this->m_strAtt, _T(";"), strArrayAttMap) ;
	for (int i=0; i<strArrayAttMap.GetSize(); i++)
	{
		CString strOldAtt, strNewAtt, strAttMapTmp ;
		strAttMapTmp = strArrayAttMap.GetAt(i) ;
		int iLocFind = strAttMapTmp.Find(_T(",")) ;
		if (iLocFind==-1)
		{
			continue;
		}
		strOldAtt = strAttMapTmp.Left(iLocFind) ;
		strNewAtt = strAttMapTmp.Mid(iLocFind+1) ;
		if (!strOldAtt.IsEmpty()&&!strNewAtt.IsEmpty())
		{
			this->m_mapStrToStrAtt.SetAt(strOldAtt, strNewAtt) ;
			iRet++ ;
		}		
	}
	return iRet ;

}
