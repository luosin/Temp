// XhlCableInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XhlCableInfoDlg.h"

// CXhlCableInfoDlg 对话框

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


// CXhlCableInfoDlg 消息处理程序

BOOL CXhlCableInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CUserSystem mysys ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("CableIntoXhl"), m_iCableLenIntoXhl) ;
	int iTmp = 0 ;	
	if (mysys.GetXdata(m_pEnt, _T("INTOXHL_CABLE"), iTmp))
	{
		m_iCableLenIntoXhl = iTmp ; //进楼50米
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
	strTitle.Format(_T("主电缆长度（%.0f）"), dSum) ;
	this->SetWindowText(strTitle) ;

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

BOOL CXhlCableInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData() ;
	CUserSystem mysys ;
	mysys.SetXdata(m_pEnt, _T("INTOXHL_CABLE"), m_iCableLenIntoXhl) ; //进楼50米
	mysys.SetXdata(m_pEnt, _T("CROSSGD_CABLE"), m_iCrossGd) ; 

	double dSum =  m_iCableLenIntoXhl+m_iCrossGd*m_iPerGd+(m_iCrossGd-1)*m_iPerBwtweenGd ;

	if (abs(m_dSumOld-dSum)>1) //此长度变化，应更新与之相连的电缆
	{
		CString strMsg ;
		strMsg.Format(_T("出现此提示是由于：主电缆总长由%.0f变为%.0f,需要更改和主电缆相连的电缆长度，程序暂未处理，请自行更改相应长度！"), m_dSumOld, dSum) ;
		AfxMessageBox(strMsg)  ;
	}

	CDialog::OnOK();
}

void CXhlCableInfoDlg::OnEnChangeEditXhlcableIntoxhl()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	this->UpdateData() ;
	this->UpdateCableLen() ;
}

int CXhlCableInfoDlg::UpdateCableLen(void)
{
	double dSum =  m_iCableLenIntoXhl+m_iCrossGd*m_iPerGd+(m_iCrossGd-1)*m_iPerBwtweenGd ;

	CString strTitle ; 
	strTitle.Format(_T("主电缆长度（%.0f）"), dSum) ;
	this->SetWindowText(strTitle) ;
	//m_dSumOld = dSum ;
	return 0;
}

void CXhlCableInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: 在此添加消息处理程序代码
}

void CXhlCableInfoDlg::OnEnChangeEditXhlcableCrossgd()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	this->UpdateData() ;
	this->UpdateCableLen() ;
}

void CXhlCableInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
