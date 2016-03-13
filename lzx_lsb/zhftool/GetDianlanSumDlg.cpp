// GetDianlanSumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetDianlanSumDlg.h"
#include "SBDestination.h"
// #include "Resource.h"
#include "..\blkuserdll\BlkExcelTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetDianlanSumDlg dialog


CGetDianlanSumDlg::CGetDianlanSumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetDianlanSumDlg::IDD, pParent)
	, m_strReg(_T(""))
	, m_bMoReg(FALSE)
	, m_nTongJiMode(1)
	, m_strRegLen(_T(""))
	, m_strRegType(_T(""))
	, m_iTwoOrFourLine(0)
{
	//{{AFX_DATA_INIT(CGetDianlanSumDlg)
	m_strDianLanDir = _T("");
	//}}AFX_DATA_INIT
// 	strArrayFilePath.SetSize(50) ;
// 
// 	m_strArrayLength1.SetSize(200) ;
// 	m_strArrayType1.SetSize(200) ;
// 	m_strArrayLength2.SetSize(200) ;
// 	m_strArrayType2.SetSize(200) ;
// 	m_strArrayAllStr.SetSize(200) ;
// 
// 	m_strArrayHaveType1.SetSize(200) ;
// 	m_strArrayHaveTypeLen1.SetSize(200) ;
// 	m_strArrayHaveType2.SetSize(200) ;
// 	m_strArrayHaveTypeLen2.SetSize(200) ;
// 
// 	m_strArray1S1S.SetSize(100) ;
// 	m_strArray1S2S.SetSize(50) ;
// 	m_strArray1S3S.SetSize(30) ;
	
}


void CGetDianlanSumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetDianlanSumDlg)
	DDX_Text(pDX, IDC_EDIT_DIANLANDIR, m_strDianLanDir);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_GETDIANLAN_REG, m_strReg);
	DDX_Check(pDX, IDC_CHECK_GETDIANLAN_MOREG, m_bMoReg);
	DDX_Radio(pDX, IDC_RADIO_GETDIANLAN_1, m_nTongJiMode);
	DDX_Text(pDX, IDC_EDIT_GETDIANLAN_REG_LEN, m_strRegLen);
	DDX_Text(pDX, IDC_EDIT_GETDIANLAN_REG_TYPE, m_strRegType);
	DDX_Radio(pDX, IDC_RADIO_GETDIANLAN_2LINE, m_iTwoOrFourLine);
}


BEGIN_MESSAGE_MAP(CGetDianlanSumDlg, CDialog)
	//{{AFX_MSG_MAP(CGetDianlanSumDlg)
	ON_BN_CLICKED(IDC_BUTTON_Do, OnBUTTONDo)
	ON_BN_CLICKED(IDC_BUTTON_GetDianlanDir, OnBUTTONGetDianlanDir)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_GETDIANLAN_MOREG, OnBnClickedCheckGetdianlanMoreg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDianlanSumDlg message handlers

void CGetDianlanSumDlg::OnBUTTONDo() 
{
	// TODO: Add your control notification handler code here
	this->SetWindowText(_T("站内电缆统计-正在处理!")) ;
	
	this->UpdateData(true) ;
	CBlkUtility::SetValueReg(_T("GetDianLanSum"), _T("FilePath"), m_strDianLanDir) ;
	CBlkUtility::SetValueReg(_T("GetDianLanSum"), _T("RegStr"), m_strReg) ;
	CBlkUtility::SetValueReg(_T("GetDianLanSum"), _T("RegStrLen"), m_strRegLen) ;
	CBlkUtility::SetValueReg(_T("GetDianLanSum"), _T("RegStrType"), m_strRegType) ;

	CUserSystem mySys ;
	this->m_strArrayFilePath.RemoveAll() ;
	CBlkUtility::FindFile(this->m_strDianLanDir, _T("dwg"), this->m_strArrayFilePath) ;

	//读取所有dwg文件，取得端子信息存入strArrayLength,strArrayType ;
	//type1,length1:支线
	//type2,length2:干线	
	int iDwgSum = m_strArrayFilePath.GetSize() ;
	if (iDwgSum!=0)
	{
		if (m_nTongJiMode==0)
		{
			m_strArrayLength1.RemoveAll() ;
			m_strArrayType1.RemoveAll() ;
			m_strArrayLength2.RemoveAll() ;
			m_strArrayType2.RemoveAll() ;
			m_strArrayAllStr.RemoveAll() ;

			m_mapBoxTypeAndNum.RemoveAll() ;

			m_strArray1S1S.RemoveAll() ;
			m_strArray1S2S.RemoveAll() ;
			m_strArray1S3S.RemoveAll() ;

			for(int i=0; i<iDwgSum; i++)
			{
				AcDbDatabase * pTmpDb = new AcDbDatabase(false) ;
				pTmpDb->readDwgFile(m_strArrayFilePath.GetAt(i)) ;
				this->GetStrFromDwg(pTmpDb, this->m_strArrayLength1, this->m_strArrayType1,this->m_strArrayLength2, this->m_strArrayType2) ;
				this->SumBoxFromDwg(pTmpDb, m_mapBoxTypeAndNum) ;
				delete pTmpDb ;
				pTmpDb = NULL ;

			}
			this->GetHaveTypeLen() ;
			CString strSaveFileName ;
			int iLen = m_strDianLanDir.GetLength() ;
			int iFindLoc = m_strDianLanDir.ReverseFind(_T('\\')) ;
			strSaveFileName.Format(_T("%s\\%s_电缆统计.xls"), this->m_strDianLanDir, this->m_strDianLanDir.Right(iLen-iFindLoc-1)) ;
			this->WriteToExcel(strSaveFileName) ;

			acutPrintf(_T("\n%s 下共%d个有效dwg文件，共有%d个符合要求的记录"),m_strDianLanDir, iDwgSum, m_strArrayLength1.GetSize()+m_strArrayLength2.GetSize()) ;
		}
		else if (m_nTongJiMode==1)
		{
			acutPrintf(_T("\n%s 目录，共有%d个dwg文件"),m_strDianLanDir, iDwgSum) ;
			for(int i=0; i<iDwgSum; i++)
			{
				m_strArrayLength1.RemoveAll() ;
				m_strArrayType1.RemoveAll() ;
				m_strArrayLength2.RemoveAll() ;
				m_strArrayType2.RemoveAll() ;
				m_strArrayAllStr.RemoveAll() ;

				m_mapBoxTypeAndNum.RemoveAll() ;

				m_strArray1S1S.RemoveAll() ;
				m_strArray1S2S.RemoveAll() ;
				m_strArray1S3S.RemoveAll() ;

				CString strFilePath ;
				strFilePath = m_strArrayFilePath.GetAt(i) ;

				AcDbDatabase * pTmpDb = new AcDbDatabase(false) ;
				pTmpDb->readDwgFile(strFilePath) ;
				this->GetStrFromDwg(pTmpDb, this->m_strArrayLength1, this->m_strArrayType1,this->m_strArrayLength2, this->m_strArrayType2) ;
				this->SumBoxFromDwg(pTmpDb, m_mapBoxTypeAndNum) ;
				this->SumGuiDaoDianLu(pTmpDb, m_strArray1S1S, m_strArray1S2S, m_strArray1S3S) ;
				delete pTmpDb ;
				pTmpDb = NULL ;

				this->GetHaveTypeLen() ;
				CString strSaveFileName ;				
				int iFindLoc = strFilePath.ReverseFind( _T('.') ) ;
				strSaveFileName.Format(_T("%s_电缆统计.xls"), strFilePath.Left(iFindLoc) ) ;
				this->WriteToExcel(strSaveFileName) ;
				acutPrintf(_T("\n(%d):%s 文件，共有%d个符合要求的记录"),i+1, strFilePath, m_strArrayLength1.GetSize()+m_strArrayLength2.GetSize()) ;
			}
		}
	}
	//end 读取端子
	
	CDialog::OnOK() ;
	
}

void CGetDianlanSumDlg::OnBUTTONGetDianlanDir() 
{
	// TODO: Add your control notification handler code here
	this->m_strArrayFilePath.RemoveAll() ;	
	
	CSBDestination sb( m_hWnd , IDS_BFF_TITLE ) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	sb.SetInitialSelection(m_strDianLanDir) ;
	if(sb.SelectFolder())
	{
		this->m_strDianLanDir = sb.GetSelectedFolder() ;
		CEdit *pEdit = (CEdit *)this->GetDlgItem(IDC_EDIT_DIANLANDIR) ;
		pEdit->SetWindowText(this->m_strDianLanDir) ;
	}		
	
}

//type1,length1:支线
//type2,length2:干线
int CGetDianlanSumDlg::GetStrFromDwg(AcDbDatabase *pDb, CStringArray &strArrayLength1, CStringArray &strArrayType1,CStringArray &strArrayLength2, CStringArray &strArrayType2)
{
	int iRt = 0 ;

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;
	CString strTrackName ;
	CString strSingalName ;
	
	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	
	AcDbEntity * pEnt = NULL ;
	BOOL bNotFoundSingal = TRUE ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			AcDbText * pText = NULL ;
			if (pEnt->isKindOf(AcDbText::desc()))
			//if (strcmp(pEnt->isA()->name(), "AcDbText") == 0)
			{
				pText = (AcDbText *)pEnt ;
				CString strText ;
				strText = pText->textString() ;	//707a-4(0)*1*1.0-110m	
				strText.TrimLeft() ;
				strText.TrimRight() ;

				//CString strReg = _T("^.+-(\\d+\\w*)\\(\\d+\\)\\*(\\d+)\\*(.+)-(\\d+)m$") ;				
				//CString strReg = _T("^[^-]+(-[^-\\s]+)*-(\\d+\\w*)\\(\\d+\\)\\*(\\d+)\\*(.+)-(\\d+)m$") ;
				//CString strReg = _T("^(.+)-(\\d+[AB]?)\\(\\d+\\)\\*(.+)-(\\d+)m$") ;
							
// 				CStringArray strArrayResult1 ;
// 				CBlkUtility::DivideString(strText, _T("-"), strArrayResult1) ;
// 				//if (strArrayResult1.GetSize()==3&&strText.Right(1)==_T("m"))
				if (CBlkUtility::IsStrMatch(m_strReg, strText))
				{
					m_strArrayAllStr.Add(strText) ;					
					
					/*
					CString strType1, strType2, strPlus, strLen ;

					
					CBlkUtility::RegReplace(strReg, _T("$2"), strText, strType1) ;
					CBlkUtility::RegReplace(strReg, _T("$4"), strText, strType2) ;
					CBlkUtility::RegReplace(strReg, _T("$3"), strText, strPlus) ;
					CBlkUtility::RegReplace(strReg, _T("$5"), strText, strLen) ;			


// 					CString str2 = strArrayResult1.GetAt(1) ; //4(0)*1*1.0
// 					CStringArray strArrayResult2 ;
// 					CBlkUtility::DivideString(str2, _T("*"), strArrayResult2) ;
// 
// 					strType1 = str2.Left(str2.Find(_T("\("))) ;//电缆芯数类型，比如4芯
// 					strType2 = strArrayResult2.GetAt(2); //1.0
// 					strPlus = strArrayResult2.GetAt(1); //1
// 					strLen = strArrayResult1.GetAt(2) ; //120m
// 					strLen.TrimRight(_T("m")) ;

					int iLen = _tstoi(strLen)*_tstoi(strPlus) ;//120*1

					CString strLenFact ;
					strLenFact.Format(_T("%d"), iLen) ;
					
					CString strType ;
					strType.Format(_T("%s(%s)"), strType1, strType2) ;
					if (strType1.Right(1)==_T("G"))
					{
						strArrayLength1.Add(strLenFact) ;
						strArrayType1.Add(strType) ;
					}
					else
					{
						strArrayLength2.Add(strLenFact) ;
						strArrayType2.Add(strType) ;
					}
					*/

					/*
					//for bt 
					CString str1, str2, strPlus, strLen ;


					CBlkUtility::RegReplace(m_strReg, _T("$1"), strText, str1) ;
					CBlkUtility::RegReplace(m_strReg, _T("$2"), strText, str2) ;
					CBlkUtility::RegReplace(m_strReg, _T("$3"), strText, strPlus) ;
					CBlkUtility::RegReplace(m_strReg, _T("$4"), strText, strLen) ;			


					int iLen = _tstoi(strLen)*_tstoi(strPlus) ;//120*1

					CString strLenFact ;
					strLenFact.Format(_T("%d"), iLen) ;

					CString strType ;
					strType.Format(_T("%s(%s)"), str2, strPlus) ;					
					if (CBlkUtility::IsStrMatch(_T("^.+[abcdefg]$"), str1)==FALSE)
					{
						strType.Append(_T("G")) ;
						strArrayLength1.Add(strLenFact) ;
						strArrayType1.Add(strType) ;

					}
					else
					{
						strArrayLength2.Add(strLenFact) ;
						strArrayType2.Add(strType) ;
					}
					*/
					
					//begin for jy
					CString strType, strLen ;
					CBlkUtility::RegReplace(m_strReg, m_strRegLen, strText, strLen) ;
					CBlkUtility::RegReplace(m_strReg, m_strRegType, strText, strType) ;
					
					if (strType.Right(1) != _T("G"))
					{						
						strArrayLength1.Add(strLen) ;
						strArrayType1.Add(strType) ;
					}
					else
					{
						strArrayLength2.Add(strLen) ;
						strArrayType2.Add(strType) ;
					}
					//end for jy
				}

			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ; 
	pBlkTblRecIter = NULL ;			

	return iRt ;
}

void CGetDianlanSumDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CGetDianlanSumDlg::OnBnClickedCheckGetdianlanMoreg()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;
	CEdit *pEdit1 =(CEdit *)this->GetDlgItem(IDC_EDIT_GETDIANLAN_REG) ;
	pEdit1->SetReadOnly(!m_bMoReg) ;
	CEdit *pEdit2 =(CEdit *)this->GetDlgItem(IDC_EDIT_GETDIANLAN_REG_LEN) ;
	pEdit2->SetReadOnly(!m_bMoReg) ;
	CEdit *pEdit3 =(CEdit *)this->GetDlgItem(IDC_EDIT_GETDIANLAN_REG_TYPE) ;
	pEdit3->SetReadOnly(!m_bMoReg) ;

}

BOOL CGetDianlanSumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CBlkUtility::QueryValueReg(_T("GetDianLanSum"), _T("FilePath"), m_strDianLanDir) ;
	CBlkUtility::QueryValueReg(_T("GetDianLanSum"), _T("RegStr"), m_strReg) ;
	CBlkUtility::QueryValueReg(_T("GetDianLanSum"), _T("RegStrLen"), m_strRegLen) ;
	CBlkUtility::QueryValueReg(_T("GetDianLanSum"), _T("RegStrType"), m_strRegType) ;

	if (m_strReg.IsEmpty())
	{
		m_strReg = _T("^(\\d+)-(\\d+[A-Z]*)\\(\\d+\\)$") ;
	}
	if (m_strRegLen.IsEmpty())
	{
		m_strRegLen = _T("$1") ;
	}
	if (m_strRegType.IsEmpty())
	{
		m_strRegType = _T("$2") ;
	}

	this->UpdateData(FALSE) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CGetDianlanSumDlg::WriteToExcel(const CString &strSaveFileName)
{
	CBlkExcelTool::WriteSumDianToXls(strSaveFileName, m_strArrayAllStr, m_strArrayHaveType1, m_strArrayHaveType2, m_strArrayHaveTypeLen1, m_strArrayHaveTypeLen2, m_strArray1S1S, m_strArray1S2S, m_strArray1S3S, m_mapBoxTypeAndNum ) ;
}

//统计每个类型的电缆长度
void CGetDianlanSumDlg::GetHaveTypeLen(void)
{	
	m_strArrayHaveType1.RemoveAll() ;
	m_strArrayHaveTypeLen1.RemoveAll() ;
	for(int i1=0; i1<m_strArrayType1.GetSize(); i1++)
	{
		int iHaveTypeLoc = -1 ;
		for(int iHaveTypeIndex=0; iHaveTypeIndex<m_strArrayHaveType1.GetSize(); iHaveTypeIndex++)
		{
			if (m_strArrayType1.GetAt(i1) == m_strArrayHaveType1.GetAt(iHaveTypeIndex))
			{
				iHaveTypeLoc = iHaveTypeIndex ;
			}
		}

		if (iHaveTypeLoc==-1)
		{
			m_strArrayHaveType1.Add(m_strArrayType1.GetAt(i1)) ;
			m_strArrayHaveTypeLen1.Add(m_strArrayLength1.GetAt(i1)) ;
		}
		else
		{
			CString strLenTmp ; 
			strLenTmp = m_strArrayHaveTypeLen1.GetAt(iHaveTypeLoc) ;
			int iOld=_tstoi(strLenTmp) ;
			strLenTmp = m_strArrayLength1.GetAt(i1) ;
			int iAdd = _tstoi(strLenTmp) ;
			int iNew = iOld + iAdd ;
			strLenTmp.Format(_T("%d"), iNew) ;
			m_strArrayHaveTypeLen1.SetAt(iHaveTypeLoc, strLenTmp) ;
		}
	}

	m_strArrayHaveType2.RemoveAll() ;
	m_strArrayHaveTypeLen2.RemoveAll() ;
	for(int i2=0; i2<m_strArrayType2.GetSize(); i2++)
	{
		int iHaveTypeLoc = -1 ;
		for(int iHaveTypeIndex=0; iHaveTypeIndex<m_strArrayHaveType2.GetSize(); iHaveTypeIndex++)
		{
			if (m_strArrayType2.GetAt(i2) == m_strArrayHaveType2.GetAt(iHaveTypeIndex))
			{
				iHaveTypeLoc = iHaveTypeIndex ;
			}
		}

		if (iHaveTypeLoc==-1)
		{
			m_strArrayHaveType2.Add(m_strArrayType2.GetAt(i2)) ;
			m_strArrayHaveTypeLen2.Add(m_strArrayLength2.GetAt(i2)) ;
		}
		else
		{
			CString strLenTmp ; 
			strLenTmp = m_strArrayHaveTypeLen2.GetAt(iHaveTypeLoc) ;
			int iOld=_tstoi(strLenTmp) ;
			strLenTmp = m_strArrayLength2.GetAt(i2) ;
			int iAdd = _tstoi(strLenTmp) ;
			int iNew = iOld + iAdd ;
			strLenTmp.Format(_T("%d"), iNew) ;
			m_strArrayHaveTypeLen2.SetAt(iHaveTypeLoc, strLenTmp) ;
		}
	}
}

// 统计箱盒数量
int CGetDianlanSumDlg::SumBoxFromDwg(AcDbDatabase *pDb, CMapStringToString & mapBoxTypeAndNum)
{
	int iRt = 0 ;

	int iXB1 = 0, iXB2 = 0 ;
	int iHZ0 = 0, iHZ6 = 0, iHZ12 = 0, iHZ24 = 0 ;
	int iHF4 = 0 ,iHF7 = 0 ;

	CString strXB1, strXB2, strHZ0, strHZ6, strHZ12, strHZ24, strHF4, strHF7 ; 
	if(mapBoxTypeAndNum.Lookup(_T("XB1"), strXB1))
	{
		iXB1 = _tstoi(strXB1) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("XB2"), strXB2) )
	{
		iXB2 = _tstoi(strXB2) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("HZ0"), strHZ0) )
	{
		iHZ0 = _tstoi(strHZ0) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("HZ6"), strHZ6) )
	{
		iHZ6 = _tstoi(strHZ6) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("HZ12"), strHZ12) )
	{
		iHZ12 = _tstoi(strHZ12) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("HZ24"), strHZ24) )
	{
		iHZ24 = _tstoi(strHZ24) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("HF4"), strHF4) )
	{
		iHF4 = _tstoi(strHF4) ;
	}
	if(	mapBoxTypeAndNum.Lookup(_T("HF7"), strHF7) )
	{
		iHF7 = _tstoi(strHF7) ;
	}

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();
	
	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			AcDbBlockReference * pRef = NULL ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				pRef = AcDbBlockReference::cast(pEnt) ;

				CString strBlkName ;
				mysys.GetBlockName(pRef, strBlkName) ;
				if (strBlkName==_T("BOX_XB_0"))
				{
					CString strXB ;
					mysys.GetAttValue(pRef, _T("XB"), strXB) ;
					if (strXB==_T("XB1"))
					{
						iXB1 ++ ;
					}
					else if (strXB==_T("XB2"))
					{
						iXB2++ ;
					}
				}
				else if (strBlkName==_T("BOX_HZ_0")||strBlkName==_T("YDQ_YD-HZ_0"))
				{
					int iHZ = -1 ;
					mysys.GetAttValue(pRef, _T("HZ"), iHZ) ;
					if (iHZ==0)
					{
						iHZ0++ ;
					}
					else if (iHZ==6)
					{
						iHZ6++ ;
					}
					else if (iHZ==12)
					{
						iHZ12++ ;
					}
					else if (iHZ==24)
					{
						iHZ24++ ;
					}					
				}
				else if (strBlkName==_T("BOX_HF_1")||strBlkName==_T("BOX_HF_0"))
				{
					int iHF = -1 ;
					mysys.GetAttValue(pRef, _T("HF"), iHF) ;
					if (iHF==4)
					{
						iHF4++ ;
					}
					else if (iHF==7)
					{
						iHF7++ ;
					}					
				}
				else if (strBlkName==_T("BOX_HZ_ZZJ_0"))
				{
					CString strHZ ;
					mysys.GetAttValue(pRef, _T("HZ"), strHZ) ;
					if (strHZ==_T("HZ0"))
					{
						iHZ0++ ;
					}
					else if (strHZ==_T("HZ6"))
					{
						iHZ6++ ;
					}
					else if (strHZ==_T("HZ12"))
					{
						iHZ12++ ;
					}
					else if (strHZ==_T("HZ24"))
					{
						iHZ24++ ;
					}
				}
				else if (strBlkName.Left(7)==_T("BOX_FSD"))
				{
					if (m_iTwoOrFourLine==4)
					{
						if (strBlkName.Right(1)==_T("0"))
						{
							if (strBlkName.Find(_T("-"))!=-1)
							{
								iXB2++ ;
							}
							else
							{
								iXB1++;
							}
						}
					}
					else
					{
						if (strBlkName.Find(_T("-"))!=-1)
						{
							iXB2++ ;
						}
						else
						{
							iXB1++;
						}
					}

				}

			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ; 
	pBlkTblRecIter = NULL ;	

	strXB1.Format(_T("%d"), iXB1) ;
	strXB2.Format(_T("%d"), iXB2);
	strHZ0.Format(_T("%d"), iHZ0);
	strHZ6.Format(_T("%d"), iHZ6) ;
	strHZ12.Format(_T("%d"), iHZ12) ;
	strHZ24.Format(_T("%d"), iHZ24) ;
	strHF4.Format(_T("%d"), iHF4) ;
	strHF7.Format(_T("%d"), iHF7) ;

	mapBoxTypeAndNum.SetAt(_T("XB1"), strXB1) ;
	mapBoxTypeAndNum.SetAt(_T("XB2"), strXB2) ;
	mapBoxTypeAndNum.SetAt(_T("HZ0"), strHZ0) ;
	mapBoxTypeAndNum.SetAt(_T("HZ6"), strHZ6) ;
	mapBoxTypeAndNum.SetAt(_T("HZ12"), strHZ12) ;
	mapBoxTypeAndNum.SetAt(_T("HZ24"), strHZ24) ;
	mapBoxTypeAndNum.SetAt(_T("HF4"), strHF4) ;
	mapBoxTypeAndNum.SetAt(_T("HF7"), strHF7) ;

	return iRt ;
}

// 统计轨道电路（1送1受、1送2受、1送3受）
int CGetDianlanSumDlg::SumGuiDaoDianLu(AcDbDatabase *pDb, CStringArray &strArray1S1S, CStringArray &strArray1S2S, CStringArray &strArray1S3S)
{
	CString strMsg ;

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	CMapStringToString mapAllGdName ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			AcDbBlockReference * pRef = NULL ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				pRef = AcDbBlockReference::cast(pEnt) ;

				CString strBlkName ;
				mysys.GetBlockName(pRef, strBlkName) ;
				if (m_iTwoOrFourLine==0&&(strBlkName==_T("BOX_FSD_S_0")||strBlkName==_T("BOX_FSD_S_1"))  || m_iTwoOrFourLine==1&&strBlkName==_T("BOX_FSD_S_0") ) //二线制电码化和轨道送受电一起考虑，四线制只考虑电码化或轨道电路一种
				//if (strBlkName.Left(5)==_T("FJD_J"))
				{
					CString strGdName, strGdNameTmp ;
					mysys.GetAttValue(pRef, _T("GDNAME"), strGdName) ;
					//strGdName = strGdNameTmp.Left(strGdNameTmp.ReverseFind(_T('G'))+1) ;
					CString strGdNum ;
					if (mapAllGdName.Lookup(strGdName, strGdNum))
					{
						int iGdNum = _tstoi(strGdNum) ;
						iGdNum++ ;
						strGdNum.Format(_T("%d"),iGdNum) ;
						mapAllGdName.SetAt(strGdName, strGdNum) ;
					}
					else
					{
						mapAllGdName.SetAt(strGdName, _T("1")) ;
					}					
				}
			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ; 
	pBlkTblRecIter = NULL ;	

	POSITION pos = mapAllGdName.GetStartPosition() ;	
	while(pos!=NULL)
	{
		CString strKey, strValue ;
		mapAllGdName.GetNextAssoc(pos, strKey, strValue) ;
		if (!(strKey.Right(2)==_T("1J")||strKey.Right(2)==_T("2J")))
		{
			CString strGd1J = strKey, strGd2J = strKey ;
			strGd1J.Insert(strKey.GetLength()-1, _T("1")) ;
			strGd2J.Insert(strKey.GetLength()-1, _T("2")) ;

			CString strValue1J ,strValue2J ;
            if(mapAllGdName.Lookup(strGd2J, strValue2J))
			{				
				if (mapAllGdName.Lookup(strGd1J, strValue1J)==FALSE)
				{
					strMsg.Format(_T("%s发现有%s个%s,但没有找到%s"), strKey, strValue2J, strGd2J, strGd1J) ;
				}
				strArray1S3S.Add(strKey) ;
			}
			else if (mapAllGdName.Lookup(strGd1J, strValue1J))
			{
				strArray1S2S.Add(strKey) ;
			}
			else
			{
				strArray1S1S.Add(strKey) ;
			}
		}
	}

	return 0;
}
