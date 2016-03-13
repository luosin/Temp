// ReadLKDataDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadLKDataDlg.h"
#include "SBDestination.h"


// CReadLKDataDlg �Ի���

IMPLEMENT_DYNAMIC(CReadLKDataDlg, CDialog)
CReadLKDataDlg::CReadLKDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadLKDataDlg::IDD, pParent)
	, m_strDir(_T(""))
	, m_strFileCZXHJ(_T(""))
	, m_strFileGDQD(_T(""))
	, m_strFileDCXX(_T(""))
	, m_strFileXLSD(_T(""))
	, m_strFileCZXX(_T(""))
	, m_strFilePDXX(_T(""))
	, m_strFileFXXX(_T(""))
	, m_strFileQJXH(_T(""))
{
}

CReadLKDataDlg::~CReadLKDataDlg()
{
}

void CReadLKDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_FILEPATH, m_strDir);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_CZXHJFILE, m_strFileCZXHJ);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_GDQDFILE, m_strFileGDQD);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_DCXXFILE, m_strFileDCXX);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_XLSDFILE, m_strFileXLSD);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_CZXXFILE, m_strFileCZXX);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_PDXXFILE, m_strFilePDXX);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_FXXXFILE, m_strFileFXXX);
	DDX_Text(pDX, IDC_EDIT_READLKDATA_QJXHFILE, m_strFileQJXH);
}


BEGIN_MESSAGE_MAP(CReadLKDataDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELDIR, OnBnClickedButtonReadlkdataSeldir)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELCZXHJ, OnBnClickedButtonReadlkdataSelczxhj)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELGDQD, OnBnClickedButtonReadlkdataSelgdqd)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELDCXX, OnBnClickedButtonReadlkdataSeldcxx)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELXLSD, OnBnClickedButtonReadlkdataSelxlsd)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELCZXX, OnBnClickedButtonReadlkdataSelczxx)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELPDXX, OnBnClickedButtonReadlkdataSelpdxx)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELFXXX, OnBnClickedButtonReadlkdataSelfxxx)
	ON_BN_CLICKED(IDC_BUTTON_READLKDATA_SELQJXH, OnBnClickedButtonReadlkdataSelqjxh)	
END_MESSAGE_MAP()


// CReadLKDataDlg ��Ϣ�������

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSeldir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;

	CSBDestination sb( NULL , IDS_BFF_TITLE ) ;
	sb.SetInitialSelection(m_strDir) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strDir= sb.GetSelectedFolder() ;

	int iIDs[8] = {IDC_EDIT_READLKDATA_CZXHJFILE, IDC_EDIT_READLKDATA_GDQDFILE, IDC_EDIT_READLKDATA_DCXXFILE, IDC_EDIT_READLKDATA_XLSDFILE, IDC_EDIT_READLKDATA_CZXXFILE, IDC_EDIT_READLKDATA_PDXXFILE, IDC_EDIT_READLKDATA_FXXXFILE, IDC_EDIT_READLKDATA_QJXHFILE } ;
	CString strToFinds[8] = {_T("����վ�źŻ�"), _T("�������"), _T("������Ϣ"), _T("��·�ٶ�"), _T("��վ��Ϣ") , _T("�¶�"), _T("����"), _T("�źŻ�����")} ;


	CStringArray strArrayFile ;
	CBlkUtility::FindFile(m_strDir, _T("xls"), strArrayFile, 1) ;
	for (int i=0; i<strArrayFile.GetCount(); i++)
	{
		CString strFilePath ;
		strFilePath = strArrayFile.GetAt(i) ;

		if (strFilePath.Find(strToFinds[0])!=-1)
		{
			m_strFileCZXHJ = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[1])!=-1)
		{
			m_strFileGDQD = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[2])!=-1)
		{
			m_strFileDCXX = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[3])!=-1)
		{
			m_strFileXLSD = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[4])!=-1)
		{
			m_strFileCZXX = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[5])!=-1)
		{
			m_strFilePDXX = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[6])!=-1)
		{
			m_strFileFXXX = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[7])!=-1)
		{
			m_strFileQJXH = strFilePath ;
		}
	}

	this->UpdateData(FALSE) ;

	CString strFiles[8] = {m_strFileCZXHJ, m_strFileGDQD, m_strFileDCXX, m_strFileXLSD, m_strFileCZXX, m_strFilePDXX, m_strFileFXXX, m_strFileQJXH} ;

	for (int j=0; j<8; j++)
	{
		CEdit *pEdit = (CEdit *)this->GetDlgItem(iIDs[j]) ;
		int iLoc = strFiles[j].Find(strToFinds[j]) ;
		if (iLoc!=-1)
		{
			pEdit->SetSel(iLoc, iLoc+6) ;
		}
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelczxhj()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ�����վ�źŻ��ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileCZXHJ ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileCZXHJ = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelgdqd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ���������ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileGDQD ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileGDQD = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSeldcxx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ�������Ϣ�ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileDCXX ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileDCXX = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelxlsd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ����·�ٶ��ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileXLSD ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileXLSD = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelczxx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ��վ��Ϣ�ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileCZXX ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileCZXX = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelpdxx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ���¶���Ϣ�ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFilePDXX ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFilePDXX = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelfxxx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ�������Ϣ�ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileFXXX ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileFXXX = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadLKDataDlg::OnBnClickedButtonReadlkdataSelqjxh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.xls)|*.xls|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ�������źŻ����ñ��ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileQJXH ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileQJXH = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}
