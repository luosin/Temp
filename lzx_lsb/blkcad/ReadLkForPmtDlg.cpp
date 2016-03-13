// ReadLkForPmtDlg.cpp : 实现文件
//

#include "stdafx.h"
// #include "blkcad.h"
#include "ReadLkForPmtDlg.h"
#include "SBDestination.h"


// CReadLkForPmtDlg 对话框

IMPLEMENT_DYNAMIC(CReadLkForPmtDlg, CDialog)
CReadLkForPmtDlg::CReadLkForPmtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadLkForPmtDlg::IDD, pParent)
	, m_strPath(_T(""))
	, m_strXhFile(_T(""))
	, m_strYdqFile(_T(""))
	, m_nPmtOption(0)
	, m_bSelXH(TRUE)
	, m_bSelYDQ(TRUE)
{
}

CReadLkForPmtDlg::~CReadLkForPmtDlg()
{
}

void CReadLkForPmtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READLKFORPMT_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_READLKFORPMT_XH, m_strXhFile);
	DDX_Text(pDX, IDC_EDIT_READLKFORPMT_YDQ, m_strYdqFile);
	DDX_Radio(pDX, IDC_RADIO_READLKFORPMT_1, m_nPmtOption);
	DDX_Check(pDX, IDC_CHECK_READLKFORPMT_SELXH, m_bSelXH);
	DDX_Check(pDX, IDC_CHECK_READLKFORPMT_SELYDQ, m_bSelYDQ);
}


BEGIN_MESSAGE_MAP(CReadLkForPmtDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_READLKFORPMT_SELPATH, OnBnClickedButtonReadlkforpmtSelpath)
	ON_BN_CLICKED(IDC_BUTTON_READLKFORPMT_SELXH, OnBnClickedButtonReadlkforpmtSelxh)
	ON_BN_CLICKED(IDC_BUTTON_READLKFORPMT_SELYDQ, OnBnClickedButtonReadlkforpmtSelydq)
//	ON_BN_CLICKED(IDC_CHECK_READLKFORPMT_SELXH, OnBnClickedCheckReadlkforpmtSelxh)
END_MESSAGE_MAP()


// CReadLkForPmtDlg 消息处理程序

void CReadLkForPmtDlg::OnBnClickedButtonReadlkforpmtSelpath()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData() ;

	CSBDestination sb( NULL , IDS_BFF_TITLE ) ;
	sb.SetInitialSelection(m_strPath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strPath= sb.GetSelectedFolder() ;

	int iIDs[2] = {IDC_EDIT_READLKFORPMT_XH, IDC_EDIT_READLKFORPMT_YDQ} ;
	CString strToFinds[2] = {_T("线路数据表"), _T("应答器位置表")} ;


	CStringArray strArrayFile ;
	CBlkUtility::FindFile(m_strPath, _T("xls"), strArrayFile, 1) ;
	for (int i=0; i<strArrayFile.GetCount(); i++)
	{
		CString strFilePath ;
		strFilePath = strArrayFile.GetAt(i) ;

		if (strFilePath.Find(strToFinds[0])!=-1)
		{
			m_strXhFile = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[1])!=-1)
		{
			m_strYdqFile = strFilePath ;
		}

	}

	this->UpdateData(FALSE) ;

	CString strFiles[2] = {m_strXhFile, m_strYdqFile} ;

	for (int j=0; j<2; j++)
	{
		CEdit *pEdit = (CEdit *)this->GetDlgItem(iIDs[j]) ;
		int iLoc = strFiles[j].Find(strToFinds[j]) ;
		if (iLoc!=-1)
		{
			pEdit->SetSel(iLoc, iLoc+6) ;
		}
	}

}

void CReadLkForPmtDlg::OnBnClickedButtonReadlkforpmtSelxh()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(文件(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("选择信号数据表") ;
	filedlg.m_ofn.lpstrInitialDir = m_strXhFile ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strXhFile = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLkForPmtDlg::OnBnClickedButtonReadlkforpmtSelydq()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(文件(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("选择应答器位置表") ;
	filedlg.m_ofn.lpstrInitialDir = m_strYdqFile ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strYdqFile = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLkForPmtDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData() ;

	if ((m_bSelXH==TRUE && m_strXhFile.Find(_T("线路数据表"))==-1) || (m_bSelYDQ==TRUE &&m_strYdqFile.Find(_T("应答器位置表"))==-1))
	{
		AfxMessageBox(_T("选择的文件不符合要求！")) ;
		return ;
	}

	CDialog::OnOK();
}

//void CReadLkForPmtDlg::OnBnClickedCheckReadlkforpmtSelxh()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
