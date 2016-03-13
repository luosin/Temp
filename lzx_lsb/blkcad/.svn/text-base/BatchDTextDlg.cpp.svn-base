// BatchDTextDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "BatchDTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchDTextDlg dialog

CString g_strSuperTxtFront = _T("") ;
CString g_strSuperTxtBack = _T("");
int g_iSuperTxtStart = 0 ;
int g_iSuperTxtEnd = 1 ;
int g_iSuperTxtIncrement = 1 ;
int g_iSuperTxtRepeat = 1 ;
int g_iTextDirect = 0 ;


CBatchDTextDlg::CBatchDTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchDTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBatchDTextDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_strFront = g_strSuperTxtFront;
	m_strBack = g_strSuperTxtBack;
	m_iStart = g_iSuperTxtStart;
	m_iEnd = g_iSuperTxtEnd;
	m_iIncrement = g_iSuperTxtIncrement;
	m_iRepeat = g_iSuperTxtRepeat;
	m_iTextDirect = g_iTextDirect ;
	//}}AFX_DATA_INIT
}


void CBatchDTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchDTextDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX, IDC_EDIT_BATCHTEXT_FRONT, m_strFront);
	DDX_Text(pDX, IDC_EDIT_BATCHTEXT_BACK, m_strBack);
	DDX_Text(pDX, IDC_EDIT_BATCHTEXT_START, m_iStart);
	DDX_Text(pDX, IDC_EDIT_BATCHTEXT_END, m_iEnd);
	DDX_Text(pDX, IDC_EDIT_BATCHTEXT_INCREMENT, m_iIncrement);
	DDX_Text(pDX, IDC_EDIT_BATCHTEXT_REPEAT, m_iRepeat);
	DDV_MinMaxInt(pDX, m_iRepeat, 1, 1000);
	DDX_Radio(pDX, IDC_BATCHTEXT_RADIO1, m_iTextDirect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchDTextDlg, CDialog)
	//{{AFX_MSG_MAP(CBatchDTextDlg)
	ON_BN_CLICKED(IDC_BUTTON_Ok, OnBUTTONOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchDTextDlg message handlers

void CBatchDTextDlg::OnBUTTONOk() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE) ;
	////
	switch(m_iTextDirect)
	{
	case 0:
		this->WriteBatchTextVer() ;
		break;
	case 1:
		this->WriteBatchTextHor() ;
		break;
	default:
		break;
	}

	////
	g_strSuperTxtFront = this->m_strFront ;
	g_strSuperTxtBack = this->m_strBack ;
	g_iSuperTxtStart = this->m_iStart ;
	g_iSuperTxtEnd = this->m_iEnd ;
	g_iSuperTxtIncrement = this->m_iIncrement ;
	g_iSuperTxtRepeat = this->m_iRepeat ;
	g_iTextDirect = this->m_iTextDirect ;
	
	CDialog::OnOK() ;
}

BOOL CBatchDTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//this->UpdateData(FALSE) ;

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_BATCHTEXT_START) ;
	pEdit->SetFocus() ;
	this->UpdateData(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBatchDTextDlg::WriteBatchTextVer()
{
	BOOL bReturn = FALSE ;
	//行数 ;
	int iRowNum = 0 ;
	//行高 ;
	double dRowHeight = 4 ;
	CString strTxt ;
	CString strErroMsg = _T("输入错误:");
	BOOL bFlag = FALSE ;
	
	
	if (m_iStart==m_iEnd)
	{
		if (m_iIncrement==0)
		{
			iRowNum = m_iRepeat ;
			bFlag = TRUE ;
		}
		else
		{
			strErroMsg.Format(_T("%s起始等于结束 而增幅不为0"),strErroMsg) ;
		}
		
	}
	else if (m_iStart>m_iEnd)
	{
		if (m_iIncrement<0)
		{
			iRowNum = ((m_iEnd - m_iStart + m_iIncrement)/m_iIncrement)*m_iRepeat ;
			bFlag = TRUE ;			
		}
		else	
		{
			strErroMsg.Format(_T("%s起始大于结束 而增幅大于0"), strErroMsg) ;
		}
	}
	else if (m_iStart<m_iEnd)
	{
		if (m_iIncrement>0)
		{
			iRowNum = ((m_iEnd - m_iStart + m_iIncrement)/m_iIncrement)*m_iRepeat ;
			bFlag = TRUE ;			
		}
		else	
		{
			strErroMsg.Format(_T("%s起始小于结束 而增幅小于0"), strErroMsg) ;
		}
	}
	
	if (bFlag == TRUE)
	{
		//iDirect==1表示从上往下排，iDirect==0表示从下往上排 ;
		int iDirect = startp_2d[1]>endp_2d[1]?1:0 ;
		if (iDirect==1)
		{
			dRowHeight = (startp_2d[1]-endp_2d[1])/iRowNum ;			
		}
		else
		{
			dRowHeight = (endp_2d[1]-startp_2d[1])/iRowNum ;
			
		}
		
		for(int i=0; i<iRowNum; i++)
		{
			ads_point base_pt= {0, 0, 0} ;
			base_pt[0] = startp_2d[0]+1 ;
			if (iDirect==1)
			{
				base_pt[1] = startp_2d[1]-(dRowHeight-dRowHeight*0.22)-i*dRowHeight ;
			}
			else
			{
				base_pt[1] = startp_2d[1]+dRowHeight*0.22+i*dRowHeight ;
			}
			int iTmp = 0 ;
			double dTextHeight = dRowHeight*0.56 ;
			iTmp = m_iStart+(i/m_iRepeat)*m_iIncrement ;
			strTxt.Format(_T("%s%d%s"),m_strFront, iTmp, m_strBack) ;
			//AfxMessageBox(strTxt) ;
			CUserSystem mysys ;
			mysys.m_pDb = acdbCurDwg();//this->pDb ;
			//mysys.WriteText(base_pt, strTxt, 0, AcDb::kTextLeft, dTextHeight, 0.7) ;
			mysys.AddText(strTxt, base_pt, base_pt, AcDb::kTextLeft, AcDb::kTextBase, dTextHeight, 0.7, 0, 0) ;
		}
		bReturn = TRUE ;
		
	}
	else
	{
		AfxMessageBox(strErroMsg) ;
	}
	//iRowNum = ((m_iEnd - m_iStart + m_iIncrement)/m_iIncrement)*m_iRepeat ;
	
	return bReturn ;

}

BOOL CBatchDTextDlg::WriteBatchTextHor()
{
	BOOL bReturn = FALSE ;
	//列数 ;
	int iColNum = 0 ;
	//行高 ;
	double dRowHeight = 4 ;
	double dColWidth = 2 ;
	CString strTxt ;
	CString strErroMsg = _T("输入错误:");
	BOOL bFlag = FALSE ;
	
	
	if (m_iStart==m_iEnd)
	{
		if (m_iIncrement==0)
		{
			iColNum = m_iRepeat ;
			bFlag = TRUE ;
		}
		else
		{
			strErroMsg.Format(_T("%s起始等于结束 而增幅不为0"),strErroMsg) ;
		}
		
	}
	else if (m_iStart>m_iEnd)
	{
		if (m_iIncrement<0)
		{
			iColNum = ((m_iEnd - m_iStart + m_iIncrement)/m_iIncrement)*m_iRepeat ;
			bFlag = TRUE ;			
		}
		else	
		{
			strErroMsg.Format(_T("%s起始大于结束 而增幅大于0"), strErroMsg) ;
		}
	}
	else if (m_iStart<m_iEnd)
	{
		if (m_iIncrement>0)
		{
			iColNum = ((m_iEnd - m_iStart + m_iIncrement)/m_iIncrement)*m_iRepeat ;
			bFlag = TRUE ;			
		}
		else	
		{
			strErroMsg.Format(_T("%s起始小于结束 而增幅小于0"), strErroMsg) ;
		}
	}
	
	if (bFlag == TRUE)
	{
		//iDirect==1表示从上往下排，iDirect==0表示从下往上排 ;
		int iDirect = startp_2d[0]>endp_2d[0]?0:1 ;
		if (iDirect==0)
		{
			dColWidth = (startp_2d[0]-endp_2d[0])/iColNum ;			
		}
		else
		{
			dColWidth = (endp_2d[0]-startp_2d[0])/iColNum ;
			
		}

		dRowHeight = startp_2d[1] - endp_2d[1] ;
		
		for(int i=0; i<iColNum; i++)
		{
			ads_point base_pt= {0, 0, 0} ;
			base_pt[1] = startp_2d[1] - dRowHeight*0.78 ;
			if (iDirect==1)
			{
				base_pt[0] = startp_2d[0]+1+i*dColWidth ;
			}
			else
			{
				base_pt[0] = startp_2d[0]+1+(iColNum-1-i)*dColWidth ;
			}
			int iTmp = 0 ;
			double dTextHeight = dRowHeight*0.56 ;
			iTmp = m_iStart+(i/m_iRepeat)*m_iIncrement ;
			strTxt.Format(_T("%s%d%s"),m_strFront, iTmp, m_strBack) ;
			//AfxMessageBox(strTxt) ;
			CUserSystem mysys ;
			mysys.m_pDb = acdbCurDwg(); //this->pDb ;
			//mysys.WriteText(base_pt, strTxt, 0, AcDb::kTextLeft, dTextHeight, 0.7) ;
			mysys.AddText(strTxt, base_pt, base_pt, AcDb::kTextLeft, AcDb::kTextBase, dTextHeight, 0.7, 0, 0) ;
		}
		bReturn = TRUE ;
		
	}
	else
	{
		AfxMessageBox(strErroMsg) ;
	}
	//iRowNum = ((m_iEnd - m_iStart + m_iIncrement)/m_iIncrement)*m_iRepeat ;
	
	return bReturn ;
}
