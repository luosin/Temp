// XhlCableInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XhlCableInfoDlg.h"

// CXhlCableInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CXhlCableInfoDlg, CDialog)
CXhlCableInfoDlg::CXhlCableInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXhlCableInfoDlg::IDD, pParent)
	, m_iCableLenIntoXhl(50)
	, m_dSumOld(0)
	, m_pEnt(NULL)
	, m_iCrossGd(0)
	, m_iPerGd(0)
	, m_iPerBwtweenGd(0)
{
}

CXhlCableInfoDlg::~CXhlCableInfoDlg()
{
}

void CXhlCableInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XHLCABLE_INTOXHL, m_iCableLenIntoXhl);
	DDX_Text(pDX, IDC_EDIT_XHLCABLE_CROSSGD, m_iCrossGd);
	DDV_MinMaxInt(pDX, m_iCrossGd, 0, 50);
	DDX_Text(pDX, IDC_EDIT_XHLCABLE_PERGD, m_iPerGd);
	DDX_Text(pDX, IDC_EDIT_XHLCABLE_BETWEENGD, m_iPerBwtweenGd);
}


BEGIN_MESSAGE_MAP(CXhlCableInfoDlg, CDialog)
	ON_MESSAGE(ZHFMSG_ZHFEDIT_TEXTCHANGED, OnTextChanged)
	ON_EN_CHANGE(IDC_EDIT_XHLCABLE_INTOXHL, OnEnChangeEditXhlcableIntoxhl)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_XHLCABLE_CROSSGD, OnEnChangeEditXhlcableCrossgd)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CXhlCableInfoDlg ��Ϣ�������

BOOL CXhlCableInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CUserSystem mysys ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("CableIntoXhl"), m_iCableLenIntoXhl) ;
	int iTmp = 0 ;	
	if (mysys.GetXdata(m_pEnt, _T("INTOXHL_CABLE"), iTmp))
	{
		m_iCableLenIntoXhl = iTmp ; //��¥50��
	}
	iTmp = 0 ;
	if (mysys.GetXdata(m_pEnt, _T("CROSSGD_CABLE"), iTmp))
	{
		m_iCrossGd = iTmp ; 
	}
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("CablePerGd"), m_iPerGd) ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("CablePerBetweenGd"), m_iPerBwtweenGd) ;

	double dSum = m_iCableLenIntoXhl+m_iCrossGd*m_iPerGd+(m_iCrossGd-1)*m_iPerBwtweenGd ;
	m_dSumOld = dSum ;
	CString strTitle ; 
	strTitle.Format(_T("�����³��ȣ�%.0f��"), dSum) ;
	this->SetWindowText(strTitle) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

BOOL CXhlCableInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
// 	if (pMsg->message == WM_KEYDOWN)
// 	{
// 		if (pMsg->wParam==VK_RETURN)
// 		{			
// 			this->AddEdit() ;
// 			return TRUE ;
// 		}
// 	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CXhlCableInfoDlg::OnTextChanged(WPARAM wparam,LPARAM lparam)
{
	HWND hWnd = (HWND)wparam ;
	this->UpdateCableLen() ;

	return 0 ;
}


void CXhlCableInfoDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	this->UpdateData() ;
	CUserSystem mysys ;
	mysys.SetXdata(m_pEnt, _T("INTOXHL_CABLE"), m_iCableLenIntoXhl) ; //��¥50��
	mysys.SetXdata(m_pEnt, _T("CROSSGD_CABLE"), m_iCrossGd) ; 

	double dSum =  m_iCableLenIntoXhl+m_iCrossGd*m_iPerGd+(m_iCrossGd-1)*m_iPerBwtweenGd ;

	if (abs(m_dSumOld-dSum)>1) //�˳��ȱ仯��Ӧ������֮�����ĵ���
	{
		CString strMsg ;
		strMsg.Format(_T("���ִ���ʾ�����ڣ��������ܳ���%.0f��Ϊ%.0f,��Ҫ���ĺ������������ĵ��³��ȣ�������δ���������и�����Ӧ���ȣ�"), m_dSumOld, dSum) ;
		AfxMessageBox(strMsg)  ;
	}

	CDialog::OnOK();
}

void CXhlCableInfoDlg::OnEnChangeEditXhlcableIntoxhl()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;
	this->UpdateCableLen() ;
}

int CXhlCableInfoDlg::UpdateCableLen(void)
{
	double dSum =  m_iCableLenIntoXhl+m_iCrossGd*m_iPerGd+(m_iCrossGd-1)*m_iPerBwtweenGd ;

	CString strTitle ; 
	strTitle.Format(_T("�����³��ȣ�%.0f��"), dSum) ;
	this->SetWindowText(strTitle) ;
	//m_dSumOld = dSum ;
	return 0;
}

void CXhlCableInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: �ڴ������Ϣ����������
}

void CXhlCableInfoDlg::OnEnChangeEditXhlcableCrossgd()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;
	this->UpdateCableLen() ;
}

void CXhlCableInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
