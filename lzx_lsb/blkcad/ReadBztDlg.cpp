// ReadBztDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
// #include "blkcad.h"
#include "ReadBztDlg.h"
#include "SBDestination.h"


// CReadBztDlg �Ի���

IMPLEMENT_DYNAMIC(CReadBztDlg, CDialog)
CReadBztDlg::CReadBztDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadBztDlg::IDD, pParent)
	, m_strDir(_T(""))
	, m_strFileZ(_T(""))
	, m_strFileY(_T(""))
	, m_strFileZH(_T(""))
	, m_strFileG(_T(""))
	, m_strFileF(_T(""))
{
}

CReadBztDlg::~CReadBztDlg()
{
}

void CReadBztDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READBZT_PATH, m_strDir);
	DDX_Text(pDX, IDC_EDIT_READBZT_Z, m_strFileZ);
	DDX_Text(pDX, IDC_EDIT_READBZT_Y, m_strFileY);
	DDX_Text(pDX, IDC_EDIT_READBZT_ZH, m_strFileZH);
	DDX_Text(pDX, IDC_EDIT_READBZT_G, m_strFileG);
	DDX_Text(pDX, IDC_EDIT_READBZT_F, m_strFileF);
}


BEGIN_MESSAGE_MAP(CReadBztDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELPATH, OnBnClickedButtonSelpath)
	ON_BN_CLICKED(IDC_BUTTON_SELZ, OnBnClickedButtonSelz)
	ON_BN_CLICKED(IDC_BUTTON_SELY, OnBnClickedButtonSely)
	ON_BN_CLICKED(IDC_BUTTON_SELZH, OnBnClickedButtonSelzh)
	ON_BN_CLICKED(IDC_BUTTON_SELG, OnBnClickedButtonSelg)
	ON_BN_CLICKED(IDC_BUTTON_SELF, OnBnClickedButtonSelf)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CReadBztDlg ��Ϣ�������

void CReadBztDlg::OnBnClickedButtonSelpath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;

	CSBDestination sb( NULL , IDS_BFF_TITLE ) ;
	sb.SetInitialSelection(m_strDir) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(!sb.SelectFolder())
		return ;
	this->m_strDir= sb.GetSelectedFolder() ;

	int iIDs[5] = {IDC_EDIT_READBZT_Z, IDC_EDIT_READBZT_Y, IDC_EDIT_READBZT_ZH, IDC_EDIT_READBZT_G, IDC_EDIT_READBZT_F} ;
	CString strToFinds[5] = {_T("��Ϲ�"), _T("��Ƶ��"), _T("�ۺϹ�"), _T("�����"), _T("���߹�") } ;


	CStringArray strArrayFile ;
	CBlkUtility::FindFile(m_strDir, _T("dwg"), strArrayFile, 1) ;
	for (int i=0; i<strArrayFile.GetCount(); i++)
	{
		CString strFilePath ;
		strFilePath = strArrayFile.GetAt(i) ;

		if (strFilePath.Find(strToFinds[0])!=-1)
		{
			m_strFileZ = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[1])!=-1)
		{
			m_strFileY = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[2])!=-1)
		{
			m_strFileZH = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[3])!=-1)
		{
			m_strFileG = strFilePath ;
		}
		else if (strFilePath.Find(strToFinds[4])!=-1)
		{
			m_strFileF = strFilePath ;
		}
	}

	this->UpdateData(FALSE) ;

	CString strFiles[5] = {m_strFileZ, m_strFileY, m_strFileZH, m_strFileG, m_strFileF} ;

	for (int j=0; j<5; j++)
	{
		CEdit *pEdit = (CEdit *)this->GetDlgItem(iIDs[j]) ;
		int iLoc = strFiles[j].Find(strToFinds[j]) ;
		if (iLoc!=-1)
		{
			pEdit->SetSel(iLoc, iLoc+6) ;
		}
	}


}

void CReadBztDlg::OnBnClickedButtonSelz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.dwg)|*.dwg|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ����Ϲ��ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileZ ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileZ = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadBztDlg::OnBnClickedButtonSely()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.dwg)|*.dwg|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ����Ƶ���ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileY ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileY = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadBztDlg::OnBnClickedButtonSelzh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.dwg)|*.dwg|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ���ۺϹ��ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileZH ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileZH = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadBztDlg::OnBnClickedButtonSelg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.dwg)|*.dwg|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ�������ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileG ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileG = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadBztDlg::OnBnClickedButtonSelf()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE) ;
	CFileDialog filedlg(true, NULL, NULL, NULL, _T("(�ļ�(*.dwg)|*.dwg|"),NULL) ;
	filedlg.m_ofn.lpstrTitle = _T("ѡ����߹��ļ�") ;
	filedlg.m_ofn.lpstrInitialDir = m_strFileF ;
	if (filedlg.DoModal()==IDOK)
	{
		this->m_strFileF = filedlg.GetPathName() ;
		this->UpdateData(FALSE) ;
	}
}

void CReadBztDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
