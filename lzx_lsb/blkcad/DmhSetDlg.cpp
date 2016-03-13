// DmhSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DmhSetDlg.h"
#include "SBDestination.h"
#include "ZnDmh.h"

#define WM_USER_THREAD_FINISHED WM_USER+0x102

// CDmhSetDlg �Ի���

IMPLEMENT_DYNAMIC(CDmhSetDlg, CDialog)
CDmhSetDlg::CDmhSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDmhSetDlg::IDD, pParent)
	, m_strSavePath(_T(""))
	, m_iDwgType(0)
	, m_strFitFor(_T(""))
{
}

CDmhSetDlg::~CDmhSetDlg()
{
}

void CDmhSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DMH_SET_SAVEPATH, m_strSavePath);
	DDX_Control(pDX, IDC_PROGRESS_DMH_SET_ALL, m_ctrlProgressAll);
	DDX_Control(pDX, IDC_PROGRESS_DMH_SET_CUR, m_ctrlProgressCur);
}


BEGIN_MESSAGE_MAP(CDmhSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DMH_SET_SEL, OnBnClickedButtonDmhSetSel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_USER_THREAD_PROGRESSRANGE, OnSetProgressRange)
	ON_MESSAGE(WM_USER_THREAD_PROGRESSPOS, OnSetProgressPos)
	ON_MESSAGE(WM_USER_THREAD_FINISHED, OnThreadFinished)
	
END_MESSAGE_MAP()


// CDmhSetDlg ��Ϣ�������

void CDmhSetDlg::OnBnClickedButtonDmhSetSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData() ;
	CSBDestination sb( m_hWnd , IDS_BFF_TITLE ) ;
	sb.SetInitialSelection(m_strSavePath) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if(sb.SelectFolder())
	{
		this->m_strSavePath = sb.GetSelectedFolder() ;	
		this->UpdateData(FALSE) ;
	}
}

BOOL CDmhSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CBlkUtility::QueryValueReg(_T("DMH"),  _T("SavePath"), m_strSavePath) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FitFor"), m_strFitFor) ;

	CString strWindowText ;
	this->GetWindowText(strWindowText) ;

	if (m_iDwgType==0) //���ɼܹ�ͼ
	{
		strWindowText.AppendFormat(_T("-���ɼܹ���ͼ(%s)"), m_strFitFor) ;
	}
	else if (m_iDwgType==1)
	{
		strWindowText.AppendFormat(_T("-��ȡ�ܹ���ͼ(%s)"), m_strFitFor) ;
	}
	else if (m_iDwgType==2)
	{
		strWindowText.AppendFormat(_T("-���ɵ�·ͼ(%s)"), m_strFitFor) ;
	}
	else if (m_iDwgType==3)
	{
		strWindowText.AppendFormat(_T("-��������ͼ(%s)"), m_strFitFor) ;
	}
	this->SetWindowText(strWindowText) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CDmhSetDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_strSavePath.IsEmpty())
	{
		AfxMessageBox(_T("���·��Ϊ�գ�������ѡ��")) ;
		return ;
	}
	CBlkUtility::SetValueReg(_T("DMH"),  _T("SavePath"), m_strSavePath) ;

	this->GetDlgItem(IDOK)->EnableWindow(FALSE) ;
	this->GetDlgItem(IDCANCEL)->EnableWindow(FALSE) ;
	this->GetDlgItem(IDC_BUTTON_DMH_SET_SEL)->EnableWindow(FALSE) ;

	CZnDmh dmh ;

	//dmh.m_strProjectName = _T("��ɽ���Զ��������칤�̽��ݣ���������ɽ���ض�") ;

	DWORD i2Or4Line = 0, iFlOn = 0, iFlFrom = 0, iUserHLP = 0 ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("TwoOrFourLine"), i2Or4Line) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FlOnZhOrFxp"), iFlOn) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FlFrom"), iFlFrom) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("FxpStart"), dmh.m_strFxpStart) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("PreZ"), dmh.m_strPreZ) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("PreZY"), dmh.m_strPreZY) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("PreZZH"), dmh.m_strPreZZH) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("NPlusZYLoc"), dmh.m_strNPlusInZY) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("UseHLP"), iUserHLP) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("ProjectName"), dmh.m_strProjectName) ;
	CBlkUtility::QueryValueReg(_T("DmhOptions"), _T("DwgSn"), dmh.m_strDwgSn) ;

	dmh.m_iTwoOrFourLine = (i2Or4Line==0?2:4) ;
	dmh.m_bIsFLInZH = (iFlOn==0?TRUE:FALSE) ;
	dmh.m_iFxpFrom = iFlFrom ;
	dmh.m_strFxpCurDz = dmh.m_strFxpStart ;
	dmh.m_bUseHLP = (iUserHLP==0?FALSE:TRUE) ;

	if (m_iDwgType==0)
	{
//		m_ctrlProgressAll.SetRange(1, 300) ;
// 		CZnDmh dmh ;
// 		dmh.m_iTwoOrFourLine = 4 ;
		dmh.m_strStation = m_strSavePath.Right(m_strSavePath.GetLength()-m_strSavePath.ReverseFind(_T('\\'))-1) ;
		dmh.m_strStation.Append(_T("վ")) ;
		if (m_strFitFor==_T("ͨ��Ժ"))
		{
			// 		for (int i1=0; i1<100; i1++)
			// 		{
			// 			m_ctrlProgressAll.SetPos(i1+1) ;
			// 			//Sleep(10) ;
			// 		}
			dmh.DrawZZH(m_strSavePath) ;
			// 		for (int i2=100; i2<200; i2++)
			// 		{
			// 			m_ctrlProgressAll.SetPos(i2+1) ;
			// 			//Sleep(10) ;
			// 		}	
			dmh.DrawZ(m_strSavePath) ;
			// 		for (int i3=200; i3<300; i3++)
			// 		{
			// 			m_ctrlProgressAll.SetPos(i3+1) ;
			// 			//Sleep(10) ;
			// 		}
			dmh.DrawZY(m_strSavePath) ;
		}
		else if (m_strFitFor==_T("�绯��"))
		{
			dmh.DrawZD(m_strSavePath) ;
			dmh.DrawZY(m_strSavePath) ;
		}
		else
		{
			AfxMessageBox(_T("�����ڡ��������á���ѡ�����õ�λ��")) ;
		}


	}
	else if (m_iDwgType==1)
	{
		m_ctrlProgressAll.SetRange(1, 300) ;
// 		CZnDmh dmh ;
// 		dmh.m_iTwoOrFourLine = 4 ;

		CString strFileName ;	
		if (m_strFitFor==_T("ͨ��Ժ"))
		{
			// 		for (int i1=0; i1<100; i1++)
			// 		{
			// 			m_ctrlProgressAll.SetPos(i1+1) ;
			// 			//Sleep(10) ;
			// 		}
			strFileName.Format(_T("%s\\��Ϲ���ͼ.dwg"), m_strSavePath) ;
			dmh.ReadZ(strFileName) ;

			// 		for (int i2=100; i2<200; i2++)
			// 		{
			// 			m_ctrlProgressAll.SetPos(i2+1) ;
			// 			//Sleep(10) ;
			// 		}	
			strFileName.Format(_T("%s\\��Ƶ����ͼ.dwg"), m_strSavePath) ;
			dmh.ReadZY(strFileName) ;
			// 		for (int i3=200; i3<300; i3++)
			// 		{
			// 			m_ctrlProgressAll.SetPos(i3+1) ;
			// 			//Sleep(10) ;
			// 		}	
			strFileName.Format(_T("%s\\�ۺϹ���ͼ.dwg"), m_strSavePath) ;
			dmh.ReadZZH(strFileName) ;
		}
		else if (m_strFitFor==_T("�绯��"))
		{
			strFileName.Format(_T("%s\\��Ϲ���ͼ.dwg"), m_strSavePath) ;
			dmh.ReadZD(strFileName) ;

			strFileName.Format(_T("%s\\��Ƶ����ͼ.dwg"), m_strSavePath) ;
			dmh.ReadZY(strFileName) ;
		}
		else
		{
			AfxMessageBox(_T("�����ڡ��������á���ѡ�����õ�λ��")) ;
		}
		

	}
	else if (m_iDwgType==2)
	{
// 		THREADPARMS *ptp = new THREADPARMS ;
// 		strncpy(ptp->chPath, (LPCTSTR)m_strSavePath, sizeof(ptp->chPath)) ;
// 		ptp->hWnd = m_hWnd ;
// 		AfxBeginThread(ThreadFuncDrawCircuit, ptp) ;


		dmh.m_hWndToSendMsg = this->m_hWnd ;
		dmh.m_ctrlProgressCur = &m_ctrlProgressCur ;
		dmh.m_ctrlProgressAll = &m_ctrlProgressAll ;
// 		MSG msg ;
// 		if (::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
// 		{
// 			::TranslateMessage(&msg);
// 			::DispatchMessage(&msg);
// 		}
		;
		dmh.m_strStation = m_strSavePath.Right(m_strSavePath.GetLength()-m_strSavePath.ReverseFind(_T('\\'))-1) ;
		dmh.m_strStation.Append(_T("վ")) ;
		if (m_strFitFor==_T("ͨ��Ժ"))
		{
			dmh.DrawCircuit(m_strSavePath) ;
		}
		else if (m_strFitFor==_T("�绯��"))
		{
			dmh.DrawCircuitD(m_strSavePath) ;
		}
		
	}
	else if (m_iDwgType==3)
	{
		dmh.m_strStation = m_strSavePath.Right(m_strSavePath.GetLength()-m_strSavePath.ReverseFind(_T('\\'))-1) ;
		dmh.m_strStation.Append(_T("վ")) ;
		dmh.DoAllPx(m_strSavePath) ;
	}
	//if (m_iDwgType<2)
	//{
		OnOK();
	//}
	
}
LRESULT CDmhSetDlg::OnSetProgressRange(WPARAM wParam, LPARAM lParam)
{
	//m_ctrlProgressAll.SetRange(1, (short)wParam) ;	
	
	CEdit *pEdit = (CEdit*)this->GetDlgItem(IDC_EDIT_DMH_SET_CURITEM) ;	
	CString *strMsg ;
	strMsg = (CString *)wParam ;
	//AfxMessageBox(*strMsg) ;
	pEdit->SetWindowText(*strMsg) ;
	::SendMessage(pEdit->m_hWnd, WM_PAINT, 0, 0) ;
	//this->UpdateData(FALSE) ;
	return 0 ;
}
LRESULT CDmhSetDlg::OnSetProgressPos(WPARAM wParam, LPARAM lParam)
{
	m_ctrlProgressAll.SetPos((int)wParam) ;
	return 0 ;
}
LRESULT CDmhSetDlg::OnThreadFinished(WPARAM wParam, LPARAM lParam)
{
	OnOK() ;
	return 0 ;
}

UINT ThreadFuncDrawCircuit(LPVOID pParam)
{
	THREADPARMS *ptp = (THREADPARMS*)pParam ;
	HWND hWnd = ptp->hWnd ;
	CString strPath ;
	strPath.Format(_T("%s"), ptp->chPath) ;
	delete ptp ;

	CZnDmh dmh ;
	dmh.m_hWndToSendMsg = hWnd ;
	dmh.DrawCircuit(strPath) ;

	::PostMessage(hWnd, WM_USER_THREAD_FINISHED, (WPARAM)0, 0) ;
    
	return 0 ;
}
