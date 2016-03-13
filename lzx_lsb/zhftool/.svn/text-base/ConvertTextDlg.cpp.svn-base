// ConvertTextDlg.cpp : 实现文件
//

#include "stdafx.h"
//#include "blkcad.h"
#include "ConvertTextDlg.h"


// CConvertTextDlg 对话框

IMPLEMENT_DYNAMIC(CConvertTextDlg, CDialog)
CConvertTextDlg::CConvertTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertTextDlg::IDD, pParent)
	, m_iTransRangeIndex(0)
{
}

CConvertTextDlg::~CConvertTextDlg()
{
}

void CConvertTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ColorIndex, m_ctrColorIndex);
	DDX_Radio(pDX, IDC_RADIO_TRANSTEXT_GLOB, m_iTransRangeIndex);
}


BEGIN_MESSAGE_MAP(CConvertTextDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CONV_ATT, OnBnClickedButtonConvAtt)
END_MESSAGE_MAP()


// CConvertTextDlg 消息处理程序

void CConvertTextDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	this->UpdateData(TRUE) ;	
	CUserSystem mysys ;		 
	mysys.m_pDb = acdbHostApplicationServices()->workingDatabase() ;

	int iCurColorSel = this->m_ctrColorIndex.GetCurSel() ;
// 	if (iCurColorSel>7)
// 	{
// 		mysys.m_iColorIndex = 256 ;
// 	}
// 	else
// 	{
// 		mysys.m_iColorIndex = iCurColorSel ;
// 	}

	//全图转换
	if (m_iTransRangeIndex==0)
	{
		mysys.ConvertMTexttoTextAll() ;
		CDialog::OnOK() ;

	}
	//选择转换
	else if (m_iTransRangeIndex==1) 
	{
		CDialog::OnOK() ;

		ads_name sum_name  , temp_name  ;
		struct resbuf eb ;
		TCHAR chSbuf[10] ;
		const CString strDxf = _T("MTEXT") ;
		CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;	
		eb.restype = 0 ;
		eb.resval .rstring = chSbuf ;
		eb.rbnext = NULL ;
		ads_printf ( _T("\n选择实体: ") ) ;

		if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
		{
			long lLength = 0 ;
			ads_sslength(sum_name, &lLength) ;
			for(int i = 0; i<lLength; i++)
			{
				ads_ssname(sum_name, i, temp_name) ;
				AcDbEntity * pEnt = NULL ;
				if (mysys.OpenAcDbEntity(temp_name, pEnt))
				{
					AcDbMText * pMText = NULL ;

		
						pMText = (AcDbMText *)pEnt ;

						//炸开后的实体集合 ；
						AcDbVoidPtrArray entSet ;
						pMText->explode(entSet) ;				

						// 文本行数 ；
						int iEntLength = entSet.length() ;

						for(int i2=0; i2<iEntLength; i2++)
						{
							AcDbEntity *pExpEnt = (AcDbEntity*)entSet[i2] ;
							mysys.AddEntityToDb(pExpEnt) ;
							pExpEnt->close() ;

						}
						pMText->erase(Adesk::kTrue) ;
					pEnt->close() ;
				}
				

			}
			ads_ssfree(sum_name) ;
		}


	}
}

void CConvertTextDlg::OnBnClickedButtonConvAtt()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE) ;	
	CUserSystem mysys ;		 
	mysys.m_pDb = acdbHostApplicationServices()->workingDatabase() ;

	int iCurColorSel = this->m_ctrColorIndex.GetCurSel() ;
// 	if (iCurColorSel>7)
// 	{
// 		mysys.m_iColorIndex = 256 ;
// 	}
// 	else
// 	{
// 		mysys.m_iColorIndex = iCurColorSel ;
// 	}


	//全图转换
	if (m_iTransRangeIndex==0)
	{
		mysys.ConvertAttrtoTextAll() ;
		CDialog::OnOK() ;
	}
	else if (m_iTransRangeIndex==1)
	{
		CDialog::OnOK() ;
		ads_name sum_name  , temp_name  ;
		struct resbuf eb ;
		TCHAR chSbuf[10] ;
		const CString strDxf = _T("ATTDEF") ;
		CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
		eb.restype = 0 ;
		eb.resval .rstring = chSbuf ;
		eb.rbnext = NULL ;
		ads_printf ( _T("\n选择实体: ") ) ;

		if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
		{
			long lLength = 0 ;
			ads_sslength(sum_name, &lLength) ;
			for(int i = 0; i<lLength; i++)
			{
				ads_ssname(sum_name, i, temp_name) ;
				AcDbEntity * pEnt = NULL ;
				if (mysys.OpenAcDbEntity(temp_name, pEnt))
				{
					AcDbAttributeDefinition * pAttr = NULL ;

					if (_tcscmp(pEnt->isA()->name(),_T("AcDbAttributeDefinition")) == 0)
					{
						pAttr = (AcDbAttributeDefinition *)pEnt ;
						CString strAttr ;

						int attrColorIndex = pAttr->colorIndex() ;
						//文本内容 ;
						TCHAR * p = pAttr->tag() ;
						strAttr = p ;

						AcGePoint3d AttrP_3d(0, 0, 0) ;						
						AcGePoint3d AlignPoint_3d(0, 0, 0) ;
						AlignPoint_3d = pAttr->alignmentPoint() ;

						AttrP_3d = pAttr->position() ;

						AcDbObjectId textStyle = NULL ;
						textStyle = pAttr->textStyle() ;

						//文本高度 宽度 ;
						double dTextWidth = pAttr->widthFactor() ;
						double dTextHeight = pAttr->height();
						//文本旋转方向 ;
						double dAngle_PI = pAttr->rotation() ;

						//文本对齐方式 ;
						//int iAlignMode ; 
						AcDb::TextVertMode iVerMode = pAttr->verticalMode() ;
						AcDb::TextHorzMode iHorMode = pAttr->horizontalMode() ;

						if (pAttr->erase(Adesk::kTrue) == Acad::eOk) 
						{
							//mysys.WriteText(base_pt, AlignPoint_3d, strAttr, dAngle_PI, 3, iVerMode, iHorMode, textStyle,dTextHeight, dTextWidth, -1) ;
							mysys.AddText(strAttr,AttrP_3d,AlignPoint_3d, iHorMode, iVerMode, dTextHeight, dTextWidth, dAngle_PI, attrColorIndex, textStyle) ;
						}
						else
						{
							AfxMessageBox(_T("删除属性文本时出错！")) ;
						}
					}
				}
				pEnt->close() ;

			}
			ads_ssfree(sum_name) ;
		}

	}
}

BOOL CConvertTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_ctrColorIndex.SetCurSel(8) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
