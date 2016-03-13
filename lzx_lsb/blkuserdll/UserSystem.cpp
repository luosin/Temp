// UserSystem.cpp: implementation of the CUserSystem class.
//AUTHOR :周海锋
//EMAIL:zhfeng@crscd.com.cn blksun@sina.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserSystem.h"
#include "../blkdll/BlkUtitily.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern BOOL bFirst ;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserSystem::CUserSystem()
{
	//m_iColorIndex = 256 ;
	m_dFontSize = 2.5 ;
	m_dFontScale = 0.7 ;
	m_pDb = acdbHostApplicationServices()->workingDatabase() ;
}

CUserSystem::~CUserSystem()
{
}

/*
void CUserSystem::WriteText(ads_point base_Pt, CString strText, double dAngle_PI, int iAlignMode, double dFontSize)
//iAlignMode为文本的对齐模式；
{	
	//转换坐标：
	AcGePoint3d base_3d (0,0,0 );
	ConvertPtAdsTo3D( base_Pt , base_3d ) ;

	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText* pText = new AcDbText() ;
			pText->setPosition(base_3d);
			//pText->setVerticalMode(AcDb::kTextVertMid);
			switch(iAlignMode)
			{
			case 0:
				{
					pText->setVerticalMode(AcDb::kTextTop ) ;
					pText->setHorizontalMode(AcDb::kTextLeft);
					break;
				}
			case 1:
				{
					pText->setVerticalMode(AcDb::kTextTop) ;
					pText->setHorizontalMode(AcDb::kTextCenter);
					break;
				}
			case 2:
				{
					pText->setVerticalMode(AcDb::kTextTop) ;
					pText->setHorizontalMode(AcDb::kTextRight);
					break;
				}
			case 3:
				{
					pText->setVerticalMode(AcDb::kTextVertMid) ;
					pText->setHorizontalMode(AcDb::kTextLeft) ;
					break;
				}
			case 4:
				{
					pText->setVerticalMode(AcDb::kTextVertMid) ;
					pText->setHorizontalMode(AcDb::kTextCenter) ;
					break;
				}
			case 5:
				{
					pText->setVerticalMode(AcDb::kTextVertMid) ;
					pText->setHorizontalMode(AcDb::kTextRight) ;
					break;
				}
			case 6:
				{
					pText->setVerticalMode(AcDb::kTextBottom) ;
					pText->setHorizontalMode(AcDb::kTextLeft) ;
					break;
				}
			case 7:
				{
					pText->setVerticalMode(AcDb::kTextBottom) ;
					pText->setHorizontalMode(AcDb::kTextCenter) ;
					break;
				}
			case 8:
				{
					pText->setVerticalMode(AcDb::kTextBottom) ;
					pText->setHorizontalMode(AcDb::kTextRight) ;
					break;
				}
			case 9:
				{
					pText->setVerticalMode(AcDb::kTextBase) ;
					pText->setHorizontalMode(AcDb::kTextLeft) ;
					break;
				}
			default:
				{
					pText->setVerticalMode(AcDb::kTextVertMid) ;
					pText->setHorizontalMode(AcDb::kTextCenter) ;
				}
			}
			pText->setAlignmentPoint(base_3d);
			pText->setTextString(strText);
			pText->setHeight ( dFontSize ) ;
			if (dAngle_PI >= PI * 0.5001 && dAngle_PI <= PI*1.5001 )
			{
				pText->setRotation(dAngle_PI + PI);		
			}
			else
			{
				pText->setRotation(dAngle_PI );		
			}
			AcDbObjectId text_Id;
			pText->setWidthFactor( dFontScale ) ;
			AcCmColor color ;
			color.setColorIndex(this->iColorIndex) ;
			pText->setColor ( color ) ;
			pBlkTblRec->appendAcDbEntity(text_Id,pText);
			pBlkTblRec->close();
			pText->close();

		}
		pBlkTbl->close();
	}
}
*/

/*
//水平或者垂直方向有无对齐方式标志 ;
// Vert  Horz  iHaveVertOrHorzFlag
//  0     0        0
//  0     1        1
//  1     0        2
//  1     1        3
//
void CUserSystem::WriteText(ads_point base_Pt, AcGePoint3d AlignmentPoint_3d, CString strText, double dAngle_PI , int iHaveVertOrHorzFlag, AcDb::TextVertMode iVerMode, AcDb::TextHorzMode iHorMode, AcDbObjectId textStyle, double dTextHeight,double dTextWidth, int iColorIndex )
{
	//转换坐标：
	AcGePoint3d base_3d (0,0,0 );
	ConvertPtAdsTo3D( base_Pt , base_3d ) ;

	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText * pText= new AcDbText;		
			
			pText->setPosition(base_3d);

			if ((iHaveVertOrHorzFlag/2)==1) 
			{
				pText->setVerticalMode(iVerMode) ;
			}
			if ((iHaveVertOrHorzFlag%2)==1) 
			{
				pText->setHorizontalMode(iHorMode) ;
			}

			if (textStyle!=NULL)
			{
				pText->setTextStyle(textStyle) ;

			}
			pText->setAlignmentPoint(AlignmentPoint_3d);
			pText->setTextString(strText);
			pText->setWidthFactor(dTextWidth) ;
			pText->setHeight (dTextHeight) ;
			if (dAngle_PI >= PI * 0.5001 && dAngle_PI <= PI*1.5001 )
			{
				pText->setRotation(dAngle_PI + PI);		
			}
			else
			{
				pText->setRotation(dAngle_PI );		
			}
			AcDbObjectId text_Id;
			AcCmColor color ;
			if (iColorIndex==-1)
			{
				iColorIndex = this->iColorIndex ;
			}
			color.setColorIndex(iColorIndex) ;
			pText->setColor ( color ) ;

			pBlkTblRec->appendAcDbEntity(text_Id,pText);
			pBlkTblRec->close();
			pText->close();

		}
		pBlkTbl->close();
	}

}
*/
/*
////todel
void CUserSystem::WriteText(ads_point base_Pt, CString strText, double dAngle_PI ,  AcDb::TextHorzMode iHorMode, double dTextHeight,double dTextWidth )
{
	//转换坐标：
	AcGePoint3d base_3d (0,0,0 );
	ConvertPtAdsTo3D( base_Pt , base_3d ) ;

	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText * pText= new AcDbText ;


			pText->setPosition(base_3d);

			pText->setHorizontalMode(iHorMode) ;
			pText->setAlignmentPoint(base_3d);
			pText->setTextString(strText);
			pText->setWidthFactor(dTextWidth) ;
			pText->setHeight (dTextHeight) ;
			if (dAngle_PI >= PI * 0.5001 && dAngle_PI <= PI*1.5001 )
			{
				pText->setRotation(dAngle_PI + PI);		
			}
			else
			{
				pText->setRotation(dAngle_PI );		
			}
			AcDbObjectId text_Id;
			AcCmColor color ;
			color.setColorIndex(3) ;
			pText->setColor ( color ) ;
			pBlkTblRec->appendAcDbEntity(text_Id,pText);
			pBlkTblRec->close();
			pText->close();

		}
		pBlkTbl->close();
	}

}
*/

/*
void CUserSystem::WriteText(ads_point base_Pt, CString strText, double dAngle_PI ,  AcDb::TextHorzMode iHorMode, double dTextHeight,double dTextWidth,AcCmColor color)
{
	//转换坐标：
	AcGePoint3d base_3d (0,0,0 );
	ConvertPtAdsTo3D( base_Pt , base_3d ) ;

	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText * pText= new AcDbText ;

			pText->setPosition(base_3d);

			pText->setHorizontalMode(iHorMode) ;
			pText->setAlignmentPoint(base_3d);
			pText->setTextString(strText);
			pText->setWidthFactor(dTextWidth) ;
			pText->setHeight (dTextHeight) ;
			if (dAngle_PI >= PI * 0.5001 && dAngle_PI <= PI*1.5001 )
			{
				pText->setRotation(dAngle_PI + PI);		
			}
			else
			{
				pText->setRotation(dAngle_PI );		
			}
			AcDbObjectId text_Id;
			pText->setColor ( color ) ;
			pBlkTblRec->appendAcDbEntity(text_Id,pText);
			pBlkTblRec->close();
			pText->close();

		}
		pBlkTbl->close();
	}

}
*/
/*
//
void CUserSystem::WriteText(ads_point base_Pt, CString strText, AcDbObjectId &objId, double dHeight, double dWidthFactor, AcDb::TextHorzMode iHorMode, AcDb::TextVertMode iVerMode, int iColorIndex, AcDbObjectId textStyle, double dAngle ) 
{
	AcGePoint3d base_3d(0, 0, 0) ;
	ConvertPtAdsTo3D(base_Pt, base_3d) ;

	AcDbBlockTable * pBlkTbl = NULL ;
	if (Acad::eOk==m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		if (Acad::eOk==pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec, AcDb::kForWrite))
		{
			AcDbText * pText = new	AcDbText ;
			pText->setTextString(strText) ;

			if (textStyle!=NULL)
			{
				pText->setTextStyle(textStyle) ;
			}

			pText->setHorizontalMode(iHorMode) ;
			pText->setVerticalMode(iVerMode) ;
			pText->setPosition(base_3d) ;
			pText->setAlignmentPoint(base_3d) ;
			if (dHeight>0)
			{
				pText->setHeight(dHeight) ;
			}
			else
			{
				pText->setHeight(2) ;
			}
			if (dWidthFactor>0)
			{
				pText->setWidthFactor(dWidthFactor) ;
			}
			else
			{
				pText->setWidthFactor(0.8) ;
			}

			AcCmColor color ;
			if (iColorIndex>=0 && iColorIndex<=256)
			{
				color.setColorIndex(iColorIndex) ;
			}
			//color.setColorIndex(0) ;
			pText->setColor(color) ;

			double dAangle = PI*dAngle/180 ;

			if (dAngle >= PI * 0.5001 && dAngle <= PI*1.5001 )
			{
				pText->setRotation(dAngle + PI);		
			}
			else
			{
				pText->setRotation(dAngle );		
			}


			//pText->setRotation(dAngle) ;

			//AcDbObjectId text_id ;
			pBlkTblRec->appendAcDbEntity(objId, pText) ;
			pBlkTblRec->close() ;
			pText->close() ;

		}
		pBlkTbl->close() ;
	}
}
*/
void CUserSystem::AddText(AcDbObjectId &objIdText,const TCHAR *chText,AcGePoint3d & pt_3d_position, AcGePoint3d & pt_3d_align, AcDb::TextHorzMode textHMode, AcDb::TextVertMode textVMode, double dHeight, double dWidthFactor, double dAngle,  Adesk::UInt16 nColorIndex, AcDbObjectId objIdTextStyle)
{
	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //解决后台数据库写入的文本，对齐方式问题，记住，文本写入数据库后，要把当前数据库设置为原始状态

	AcDbBlockTable * pBlkTbl = NULL ;
	if (Acad::eOk==m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		if (Acad::eOk==pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec, AcDb::kForWrite))
		{
			AcDbText * pText = new	AcDbText() ;
			pText->setTextString(chText) ;

			if (objIdTextStyle!=NULL)
			{
 				pText->setTextStyle(objIdTextStyle) ;
			}

			if (dHeight>0)
			{
				pText->setHeight(dHeight) ;
			}
			else
			{
				pText->setHeight(2) ;
			}
			if (dWidthFactor>0)
			{
				pText->setWidthFactor(dWidthFactor) ;
			}
			else
			{
				pText->setWidthFactor(0.8) ;
			}

			AcCmColor color ;
			if (nColorIndex>=0 && nColorIndex<=256)
			{
				color.setColorIndex(nColorIndex) ;
			}
			pText->setColor(color) ;

			//HorzMode=kTextLeft&&VertMode=kTextBase||HorzMode=kTextAlign||HorzMode=kTextFit，只有这三种情况下，插入点是position，其他插入点都是alignment
			//
			pText->setHorizontalMode(textHMode) ;
			pText->setVerticalMode(textVMode) ;

			if (textHMode==AcDb::kTextLeft&&textVMode==AcDb::kTextBase)
			{
				pText->setPosition(pt_3d_position) ;
			}
			else if (textHMode==AcDb::kTextAlign||textHMode==AcDb::kTextFit)
			{
				pText->setPosition(pt_3d_position) ;
				pText->setAlignmentPoint(pt_3d_align) ;
			}
			else
			{
				pText->setAlignmentPoint(pt_3d_align) ;
				//pText->setPosition(pt_3d_position) ; //如果没有此句，在当前AcDbDatabase *pDb = acdbCurDwg()下能很好的工作，但是在AcDbDatabase *pDb = new AcDbDatabase，就坐标全为0了
			}

// 			AcGePoint3d pt_3d_test1 = pText->position() ;
// 			AcGePoint3d pt_3d_test2 = pText->alignmentPoint() ;

// 			pText->adjustAlignment(pDb) ;
			
			//double dAngle = PI*dAngle/180 ;

			if (dAngle >= PI * 0.5001 && dAngle <= PI*1.5001 )
			{
				pText->setRotation(dAngle + PI);		
			}
			else
			{
				pText->setRotation(dAngle );	
			}
			pBlkTblRec->appendAcDbEntity(objIdText, pText) ;
			pBlkTblRec->close() ;
			
			pText->close() ;

		}
		pBlkTbl->close() ;
	}

	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;

}
void CUserSystem::AddText(const TCHAR *chText, AcGePoint3d & pt_3d_position, AcGePoint3d & pt_3d_align, AcDb::TextHorzMode textHMode, AcDb::TextVertMode textVMode, double dHeight, double dWidthFactor, double dAngle,  Adesk::UInt16 nColorIndex, AcDbObjectId objIdTextStyle)
{
	AcDbObjectId objIdText ;
	this->AddText(objIdText, chText, pt_3d_position, pt_3d_align, textHMode, textVMode, dHeight, dWidthFactor,dAngle,nColorIndex, objIdTextStyle) ;
}

void CUserSystem::AddText(AcDbObjectId &objIdText,const TCHAR *chText,AcGePoint2d & pt_2d_position, AcGePoint2d & pt_2d_align, AcDb::TextHorzMode textHMode, AcDb::TextVertMode textVMode, double dHeight, double dWidthFactor, double dAngle,  Adesk::UInt16 nColorIndex, AcDbObjectId objIdTextStyle)
{
	AcGePoint3d pt_3d_position(0,0,0), pt_3d_align(0,0,0) ;
	this->ConvertPoint(pt_2d_position, pt_3d_position) ;
	this->ConvertPoint(pt_2d_align, pt_3d_align) ;
	this->AddText(objIdText, chText, pt_3d_position, pt_3d_align, textHMode, textVMode, dHeight, dWidthFactor,dAngle,nColorIndex, objIdTextStyle) ;
}
void CUserSystem::AddText(const TCHAR *chText, AcGePoint2d & pt_2d_position, AcGePoint2d & pt_2d_align, AcDb::TextHorzMode textHMode, AcDb::TextVertMode textVMode, double dHeight, double dWidthFactor, double dAngle,  Adesk::UInt16 nColorIndex, AcDbObjectId objIdTextStyle)
{
	AcGePoint3d pt_3d_position(0,0,0), pt_3d_align(0,0,0) ;
	this->ConvertPoint(pt_2d_position, pt_3d_position) ;
	this->ConvertPoint(pt_2d_align, pt_3d_align) ;
	this->AddText(chText, pt_3d_position, pt_3d_align, textHMode, textVMode, dHeight, dWidthFactor,dAngle,nColorIndex, objIdTextStyle) ;
}


void CUserSystem::AddText(AcDbObjectId &objIdText, const TCHAR *chText, ads_point & pt_position, ads_point & pt_align, AcDb::TextHorzMode textHMode, AcDb::TextVertMode textVMode, double dHeight, double dWidthFactor, double dAngle,  Adesk::UInt16 nColorIndex, AcDbObjectId objIdTextStyle) 
{
	AcGePoint3d pt_3d_position(0,0,0), pt_3d_align(0,0,0) ;
	this->ConvertPtAdsTo3D(pt_position, pt_3d_position) ;
	this->ConvertPtAdsTo3D(pt_align, pt_3d_align) ;

	this->AddText(objIdText, chText, pt_3d_position, pt_3d_align, textHMode, textVMode, dHeight, dWidthFactor,dAngle,nColorIndex, objIdTextStyle) ;	
}

void CUserSystem::AddText(const TCHAR *chText, ads_point & pt_position, ads_point & pt_align, AcDb::TextHorzMode textHMode, AcDb::TextVertMode textVMode, double dHeight, double dWidthFactor, double dAngle,  Adesk::UInt16 nColorIndex, AcDbObjectId objIdTextStyle)
{
	AcDbObjectId objIdText ;
	this->AddText(objIdText, chText, pt_position, pt_align, textHMode, textVMode, dHeight, dWidthFactor,dAngle,nColorIndex, objIdTextStyle) ;
}

void CUserSystem::AddMText(ads_point base_Pt, CString strText, AcDbObjectId textStyleId, AcDbMText::AttachmentPoint iAttachment, double dTextHeight,double dTextWidth, int icolor ) 
{
	//转换坐标：
	AcGePoint3d base_3d (0,0,0 );
	ConvertPtAdsTo3D( base_Pt , base_3d ) ;


	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbMText * pMText= new AcDbMText ;

			if (textStyleId!=NULL)
			{
				pMText->setTextStyle(textStyleId) ;				
			}
			pMText->setLocation(base_3d) ;	

			pMText->setAttachment(iAttachment) ;

			pMText->setContents(strText) ;		

			pMText->setWidth(dTextWidth) ;
			pMText->setTextHeight(dTextHeight) ;

			AcDbObjectId text_Id;
			AcCmColor color ;
			color.setColorIndex(icolor) ;
			pMText->setColor(color) ;			

			pBlkTblRec->appendAcDbEntity(text_Id,pMText);
			pBlkTblRec->close();
			pMText->close();

		}
		pBlkTbl->close();
	}
}

void CUserSystem::ConvertPtAdsTo3D(ads_point point_Pt, AcGePoint3d &point_3d)
{
	point_3d.x = point_Pt[0] ;
	point_3d.y = point_Pt[1] ;
	point_3d.z = point_Pt[2] ;
}

//转换点类型
void CUserSystem::ConvertPoint(const AcGePoint3d &pt_3d_src, AcGePoint2d &pt_2d_to) 
{
	pt_2d_to.x = pt_3d_src.x ;
	pt_2d_to.y = pt_3d_src.y ;
}

//转换点类型
void CUserSystem::ConvertPoint(const AcGePoint2d &pt_2d_src, AcGePoint3d &pt_3d_to) 
{
	pt_3d_to.x = pt_2d_src.x ;
	pt_3d_to.y = pt_2d_src.y ;
	//pt_3d_to.z = 0 ;
}

void CUserSystem::ConvertAttrtoTextAll()
{
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbAttributeDefinition * pAttr = NULL ;

			//处理属性文本  
			if (pEnt->isKindOf(AcDbAttributeDefinition::desc()))
				//if (strcmp(pEnt->isA()->name(),"AcDbAttributeDefinition") == 0)
			{
				pAttr = (AcDbAttributeDefinition *)pEnt ;
				CString strAttr ;
				//AcCmColor attrColor = pAttr->color() ;
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

				Adesk::UInt16 nColorIndex = pAttr->colorIndex() ;

				//文本对齐方式 ;
				AcDb::TextVertMode iVerMode = pAttr->verticalMode() ;
				AcDb::TextHorzMode iHorMode = pAttr->horizontalMode() ;

				if (pAttr->erase(Adesk::kTrue) == Acad::eOk) 
				{					
					this->AddText(strAttr, AttrP_3d, AlignPoint_3d, iHorMode, iVerMode,dTextHeight, dTextWidth, dAngle_PI, nColorIndex, textStyle) ;
				}
				else
				{
					AfxMessageBox(_T("删除属性文本时出错！")) ;
				}

			}
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

}
/*
void CUserSystem::ConvertAlignToInt(AcDb::TextVertMode iVerMode, AcDb::TextHorzMode iHorMode, int &iResult)
{
	if (iVerMode == AcDb::kTextTop && iHorMode == AcDb::kTextLeft ) 
	{
		iResult = 0 ;
	}
	else if (iVerMode == AcDb::kTextTop && iHorMode == AcDb::kTextCenter ) 
	{
		iResult = 1 ;
	}
	else if (iVerMode == AcDb::kTextTop && iHorMode == AcDb::kTextRight ) 
	{
		iResult = 2 ;
	}
	else if (iVerMode == AcDb::kTextVertMid && iHorMode == AcDb::kTextLeft ) 
	{
		iResult = 3 ;
	}
	else if (iVerMode == AcDb::kTextVertMid && iHorMode == AcDb::kTextCenter ) 
	{
		iResult = 4 ;
	}
	else if (iVerMode == AcDb::kTextVertMid && iHorMode == AcDb::kTextRight ) 
	{
		iResult = 5 ;
	}
	else if (iVerMode == AcDb::kTextBottom && iHorMode == AcDb::kTextLeft ) 
	{
		iResult = 6 ;
	}
	else if (iVerMode == AcDb::kTextBottom && iHorMode == AcDb::kTextCenter ) 
	{
		iResult = 7 ;
	}
	else if (iVerMode == AcDb::kTextBottom && iHorMode == AcDb::kTextRight ) 
	{
		iResult = 8 ;
	}
	else
	{
		iResult = 9 ;

	}


}
*/

//转换整张图纸中的多行文本为单行文本 ;
void CUserSystem::ConvertMTexttoTextAll()
{
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbMText * pMText = NULL ;

			//处理多行文本 AcDbMText ;
			if (pEnt->isKindOf(AcDbMText::desc()))			
				//if (strcmp(pEnt->isA()->name(),"AcDbMText") == 0)
			{
				pMText = (AcDbMText *)pEnt ;
				// 				AcDbText * pText = NULL ;
				// 				
				// 				AcGePoint3d MtextP_3d(0, 0, 0) ;
				// 				ads_point base_pt = {0, 0, 0} ;	
				// 				
				// 				AcCmColor mtextColor = pMText->color() ;
				// 				// 文本内容 ；
				// 				CString strText ;
				// 				//Height ;
				// 				double dMtextHeight = 3 ;
				// 				//width Factor ;
				// 				double dMtextWidth = 0.7 ;
				// 				//TextStyle ;
				// 				AcDbObjectId textStyle = NULL ;
				// 				//旋转方向 ;
				// 				double dAngle_PI = 0 ;

				//炸开后的实体集合 ；
				AcDbVoidPtrArray entSet ;
				pMText->explode(entSet) ;				

				// 文本行数 ；
				int iEntLength = entSet.length() ;

				for(int i2=0; i2<iEntLength; i2++)
				{
					AcDbEntity *pExpEnt = (AcDbEntity*)entSet[i2] ;
					this->AddEntityToDb(pExpEnt) ;
					pExpEnt->close() ;

					// 					pText = (AcDbText *)entSet.at(i) ;
					// 					strText = pText->textString() ;
					// 					MtextP_3d = pText->position() ;
					// 
					// 					dMtextHeight = pText->height() ;
					// 					dMtextWidth = pText->widthFactor() ;
					// 				    textStyle = pMText->textStyle() ;
					// 					//文本旋转方向 ;
					// 					dAngle_PI = pMText->rotation() ;
					// 
					// 					base_pt[X] = MtextP_3d.x ;
					// 					base_pt[Y] = MtextP_3d.y ;
					// 					WriteText(base_pt, MtextP_3d, strText,dAngle_PI,1, AcDb::kTextBottom, AcDb::kTextLeft, textStyle, dMtextHeight,dMtextWidth, -1) ;
				}
				pMText->erase(Adesk::kTrue) ;
			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

}


AcDbObjectId CUserSystem::AddEntityToDb(AcDbEntity *pEnt)
{
	AcDbObjectId entId; 
	AcDbBlockTable * pBlkTbl = NULL ;
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	if( Acad::eOk == m_pDb->getBlockTable( pBlkTbl , AcDb::kForRead ) ) 
	{
		if( Acad::eOk == pBlkTbl->getAt( ACDB_MODEL_SPACE , pBlkTblRec , AcDb::kForWrite ) )
		{
			Acad::ErrorStatus esAppend = pBlkTblRec->appendAcDbEntity(entId, pEnt) ;
			pBlkTblRec->close() ;
		}
		pBlkTbl->close() ;

	}
	//pEnt->close() ;
	return entId ;

}

/*
void CUserSystem::AddAcDb2dPolyline(AcGePoint3d start_3d, AcGePoint3d end_3d, double dWidth, int iLine_Type)
{
AcGePoint3dArray temp_3d_ary ;
temp_3d_ary.append( start_3d ) ;
temp_3d_ary.append( end_3d ) ;

AcDb2dPolyline * pLine_2d = new AcDb2dPolyline ( AcDb::k2dSimplePoly , temp_3d_ary , 0 , Adesk::kFalse , dWidth , dWidth ) ; 
pLine_2d->setDefaultEndWidth( dWidth ) ;
pLine_2d->setDefaultStartWidth( dWidth ) ;
AddEntityToDb( (AcDbEntity * )pLine_2d ) ;
if( iLine_Type != 0 )
{

CString strR14Path ;
if( this->GetR14Path( strR14Path ) )
{
CString strLinePath ;
strLinePath.Format( "%s\\support\\acad.lin" , strR14Path ) ;
if( this->Load_Line_Type( strLinePath , "Dashedx2" ) )
{
pLine_2d->setLinetype( "Dashedx2" ) ;
}
}


}
pLine_2d->close() ;


}
*/
void CUserSystem::AddAcDb2dPolyline(AcGePoint3d start_3d, AcGePoint3d end_3d, double dWidth, int iLine_Type, int iColor)
{
	AcGePoint3dArray temp_3d_ary ;
	temp_3d_ary.append( start_3d ) ;
	temp_3d_ary.append( end_3d ) ;

	AcDb2dPolyline * pLine_2d = new AcDb2dPolyline ( AcDb::k2dSimplePoly , temp_3d_ary , 0 , Adesk::kFalse , dWidth , dWidth ) ; 
	pLine_2d->setDefaultEndWidth( dWidth ) ;
	pLine_2d->setDefaultStartWidth( dWidth ) ;
	AcCmColor color ;
	if (!((iColor>=0 && iColor<=7) || iColor==256))
	{
		iColor = 0 ;
	}
	color.setColorIndex(iColor) ;
	pLine_2d->setColor(color) ;

	AddEntityToDb( (AcDbEntity * )pLine_2d ) ;
	if( iLine_Type != 0 )
	{
		/*
		CString strR14Path ;
		if( this->GetR14Path( strR14Path ) )
		{
		CString strLinePath ;
		strLinePath.Format( "%s\\support\\acad.lin" , strR14Path ) ;
		if( this->Load_Line_Type( strLinePath , "Dashedx2" ) )
		{
		pLine_2d->setLinetype( "Dashedx2" ) ;
		}
		}
		*/

	}
	pLine_2d->close() ;


}



//////////////////////////////////////////////////////////////////////////
//取得POLYLINE结点坐标

void CUserSystem::collectVertices(const AcDb2dPolyline * pline, AcGePoint3dArray & pts, bool asWcsPts)
{
	ASSERT(pline != NULL);
	ASSERT(pts.isEmpty());

	AcDbObjectIterator* vertexIter = pline->vertexIterator();
	ASSERT(vertexIter != NULL);
	if (vertexIter == NULL)
	{
		return;
	}

	AcDb2dVertex * vertex;
	for (; !vertexIter->done(); vertexIter->step()) 
	{
		if (acdbOpenObject(vertex, vertexIter->objectId(), AcDb::kForRead) == Acad::eOk) 
		{
			AcGePoint3d pp(pline->vertexPosition(*vertex));
			if (vertex->vertexType() != AcDb::k2dSplineCtlVertex)
			{
				if (asWcsPts)
				{
					pts.append(pline->vertexPosition(*vertex));   // return WCS
				}
				else
				{
					pts.append(vertex->position());      // return ECS
				}
			}
			vertex->close();
		}
	}
	delete vertexIter;

	ASSERT(pts.isEmpty() == Adesk::kFalse);

	if (pline->isClosed()) 
	{
		AcGePoint3d tmpPt = pts[0];  // used to be a bug in dynamic arrays (not sure if its still there??)
		pts.append(tmpPt);
	}
}

//

//////////////////////////////////////////////////////////////////////////
//
//取得LWPOLYLINE结点坐标

void CUserSystem::collectVertices(const AcDbPolyline * pline, AcGePoint3dArray & pts)
{
	ASSERT(pline != NULL);
	ASSERT(pts.isEmpty());

	AcGePoint3d pt;
	for (UINT vertexNumber = 0; vertexNumber < pline->numVerts(); vertexNumber++) 
	{
		pline->getPointAt(vertexNumber, pt);
		pts.append(pt);
	}

	ASSERT(pts.isEmpty() == Adesk::kFalse);

	if (pline->isClosed())
	{
		AcGePoint3d tmpPt = pts[0];
		pts.append(tmpPt);
	}
}

//

//清除指定范围内的text和polyline ;
//

void CUserSystem::ClearEntity(double dStartX, double dStartY, double dEndX, double dEndY)
{
	AcDbBlockTable * pBlkTbl = NULL ;
	this->m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{

			//处理文本 AcDbText ;
			if (pEnt->isKindOf(AcDbText::desc()))
				//if (strcmp(pEnt->isA()->name(),"AcDbText") == 0)
			{
				AcDbText * pText = NULL ;
				pText = (AcDbText *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pText->position() ;
				//只处理规定区域的文本；
				if((textp_3d.x>dStartX && textp_3d.x<dEndX) && (textp_3d.y>dStartY && textp_3d.y<dEndY))
				{	
					pText->erase(Adesk::kTrue) ;
				}
			}
			if (pEnt->isKindOf(AcDb2dPolyline::desc()))
			{
				AcDb2dPolyline * pPolyline = NULL ;				
				pPolyline = (AcDb2dPolyline *)pEnt ;

				AcGePoint3d firstP_3d(0, 0, 0) ;
				AcGePoint3dArray verticesP_3dArray ;
				this->collectVertices(pPolyline, verticesP_3dArray, TRUE) ;
				firstP_3d = verticesP_3dArray.at(0) ;				

				if ((firstP_3d.x>=dStartX && firstP_3d.x<=dEndX) && (firstP_3d.y>=dStartY && firstP_3d.y<=dEndY)) 
				{
					pPolyline->erase(Adesk::kTrue) ;
				}				
			}
			pEnt->close() ;
		}

	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

/*
AcDbObjectId CUserSystem::createTextStyle(CString fontName, CString bigFontName, CString textStyleName)
{
AcGiTextStyle *TextStyle=new AcGiTextStyle
(fontName,
bigFontName,
0,
1,
0,
0,
Adesk::kFalse,
Adesk::kFalse,
Adesk::kFalse,
Adesk::kFalse,
Adesk::kFalse,
textStyleName); //字体名 
//TextStyle->set
AcDbObjectId textStyleId;
toAcDbTextStyle(*TextStyle, textStyleId) ;
return textStyleId ;

}
*/

AcDbObjectId CUserSystem::AddAcDbLine(AcGePoint3d pt_3d_start, AcGePoint3d pt_3d_end, int iColor)
{

	AcDbLine *pLine = new AcDbLine(pt_3d_start, pt_3d_end);

	if (!((iColor>=0 && iColor<=7) || iColor==256))
	{
		iColor = 0 ;
	}
	pLine->setColorIndex(iColor) ;

	AcDbObjectId objId ;
	objId = AddEntityToDb(pLine) ;

	pLine->close();

	return objId ;	
}
AcDbObjectId CUserSystem::AddAcDbLine(ads_point pt_start, ads_point pt_end, int iColor)
{
	AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
	this->ConvertPtAdsTo3D(pt_start,pt_3d_start) ;
	this->ConvertPtAdsTo3D(pt_end, pt_3d_end) ;
	return AddAcDbLine(pt_3d_start, pt_3d_end, iColor) ;
}

void CUserSystem::AddAcDb2dPolyline(AcGePoint3dArray PlinePointArray, double dWidth, int iLine_Type)
{	
	AcDb2dPolyline * pLine_2d = new AcDb2dPolyline ( AcDb::k2dSimplePoly , PlinePointArray , 0 , Adesk::kTrue , dWidth , dWidth ) ; 
	pLine_2d->setDefaultEndWidth( dWidth ) ;
	pLine_2d->setDefaultStartWidth( dWidth ) ;
	AddEntityToDb( (AcDbEntity * )pLine_2d ) ;
	if( iLine_Type != 0 )
	{
		/*
		CString strR14Path ;
		if( this->GetR14Path( strR14Path ) )
		{
		CString strLinePath ;
		strLinePath.Format( "%s\\support\\acad.lin" , strR14Path ) ;
		if( this->Load_Line_Type( strLinePath , "Dashedx2" ) )
		{
		pLine_2d->setLinetype( "Dashedx2" ) ;
		}
		}
		*/		
	}
	pLine_2d->close() ;

}

AcDbObjectId CUserSystem::AddAcDbPolyLine(const AcGePoint2dArray &pt2dArray,  const AcArray<double>& dWidths, int iColor, BOOL bIsClosed)
{
	UINT nSize = pt2dArray.length() ;
	UINT nSizeWidth = dWidths.length() ;

	AcDbPolyline *pPolyLine = new AcDbPolyline(nSize) ;
	for (int i=0; i<nSize; i++)
	{
		double dWidth = (nSize==nSizeWidth?dWidths.at(i):0) ;
		pPolyLine->addVertexAt(i, pt2dArray.at(i), 0, dWidth, dWidth) ;
	}
	pPolyLine->setColorIndex(iColor) ;
	pPolyLine->setClosed(bIsClosed) ;
	AcDbObjectId objId ;
	objId = AddEntityToDb(pPolyLine) ;

	pPolyLine->close() ;

	return objId ;
}
AcDbObjectId CUserSystem::AddAcDbPolyLine(const AcGePoint2dArray &pt2dArray,  double dWidth, int iColor, BOOL bIsClosed)
{
	UINT nSize = pt2dArray.length() ;
	AcArray<double> dWidths ;
	for (int i=0; i<nSize; i++)
	{
		dWidths.append(dWidth) ;
	}
	return AddAcDbPolyLine(pt2dArray, dWidths, iColor, bIsClosed) ;
}

AcDbObjectId CUserSystem::AddAcDbPolyLine(const AcGePoint3dArray &pt3dArray,  double dWidth, int iColor, BOOL bIsClosed)
{
	AcGePoint2dArray pt2dArray ;
	UINT nSize = pt3dArray.length() ;
	for (int i=0; i<nSize; i++)
	{
		AcGePoint2d pt_2d_tmp(pt3dArray.at(i).x, pt3dArray.at(i).y) ;
		pt2dArray.append(pt_2d_tmp) ;
	}
	return AddAcDbPolyLine(pt2dArray, dWidth, iColor, bIsClosed) ;
}
// AcDbObjectId CUserSystem::AddAcDbPolyLine(AcGePoint3d start_3d, AcGePoint3d end_3d, double dWidth, int iColor, BOOL bIsClosed)
// {
// 	AcGePoint2d start_2d, end_2d;
// 	start_2d.x = start_3d.x ;
// 	start_2d.y = start_3d.y ;
// 	end_2d.x = end_3d.x ;
// 	end_2d.y = end_3d.y ;
// 
// 	AcGePoint2dArray ptArray ;
// 	ptArray.append(start_2d) ;
// 	ptArray.append(end_2d) ;
// 
// 	return AddAcDbPolyLine(ptArray, dWidth, iColor, bIsClosed) ;
// }

//根据起点、圆弧角、半径、弦方向绘制圆弧。dAngle 圆弧弧度, dDirectOfChord,弦的方向角度
AcDbObjectId CUserSystem::AddAcDbPolyLine(AcGePoint3d pt_3d_start, double dAngle, double dRadius, double dDirectOfChord, double dWidth, int iColor, BOOL bIsClosed)
{
	AcDbPolyline *pPolyLine = new AcDbPolyline(2) ;
	double dBulge = tan(1 / 4. * dAngle);  //凸度	
	pPolyLine->addVertexAt(0, pt_3d_start.convert2d(AcGePlane::kXYPlane), dBulge, dWidth, dWidth) ;
	double dLenOfChord =sqrt(pow(dRadius, 2)*2-2*dRadius*dRadius*cos(dAngle)) ; //余弦定理，c^2 = a^2 + b^2 - 2ab*cos(γ), γ是ab夹角
	AcGePoint2d pt_2d_end(pt_3d_start.x+dLenOfChord, pt_3d_start.y) ;
	pPolyLine->addVertexAt(1, pt_2d_end, dBulge, dWidth, dWidth) ;
	this->RotationEnt(pPolyLine, dDirectOfChord, pt_3d_start) ;
	pPolyLine->setColorIndex(iColor) ;
	pPolyLine->setClosed(bIsClosed) ;
	AcDbObjectId objId ;
	objId = AddEntityToDb(pPolyLine) ;

	pPolyLine->close() ;

	return objId ;
}
//根据起点、圆弧角、圆心绘制圆弧。dAngle 圆弧弧度, pt_3d_center 圆心
AcDbObjectId CUserSystem::AddAcDbPolyLine(AcGePoint3d pt_3d_start,  double dAngle, AcGePoint3d pt_3d_center, double dWidth, int iColor, BOOL bIsClosed)
{
	AcGeLineSeg3d lineSeg3d(pt_3d_start, pt_3d_center) ;
	AcGeVector3d vec = lineSeg3d.direction() ;
	double dDirectOfChord = vec.angleOnPlane(AcGePlane::kXYPlane)-(PI-dAngle)/2 ;
	double dRadius = lineSeg3d.length() ;
	return AddAcDbPolyLine(pt_3d_start, dAngle, dRadius, dDirectOfChord, dWidth, iColor, bIsClosed) ;	
}
//根据起点、终点、圆弧角绘制圆弧。dAngle 圆弧弧度,=0时为直线
AcDbObjectId CUserSystem::AddAcDbPolyLine(AcGePoint3d pt_3d_start, AcGePoint3d pt_3d_second, double dWidth, double dAngle, int iColor, BOOL bIsClosed)
{
	AcDbPolyline *pPolyLine = new AcDbPolyline(2) ;
	double dBulge = tan(1 / 4. * dAngle);  //凸度	
	pPolyLine->addVertexAt(0, pt_3d_start.convert2d(AcGePlane::kXYPlane), dBulge, dWidth, dWidth) ;
	pPolyLine->addVertexAt(1, pt_3d_second.convert2d(AcGePlane::kXYPlane), dBulge, dWidth, dWidth) ;
	pPolyLine->setColorIndex(iColor) ;
	pPolyLine->setClosed(bIsClosed) ;
	AcDbObjectId objId ;
	objId = AddEntityToDb(pPolyLine) ;
	pPolyLine->close() ;
	
	return objId ;
}

BOOL CUserSystem::Verify_FileName( const TCHAR * chFileName , BOOL bPropt )
{
	BOOL bReturn = FALSE ;
	TCHAR chTemp[MAX_PATH] ;
	CString strPropt ;
	if( RTNORM == ads_findfile ( chFileName , chTemp ) )
	{
		bReturn = TRUE ;
	}
	else
	{
		bReturn = FALSE ;
		if( bPropt )
		{
			strPropt.Format (_T("找不到系统环境文件：%s;程序无法继续进行") , chFileName ) ; 
			::MessageBox ( ::AfxGetApp()->m_pMainWnd->m_hWnd  , strPropt ,_T("提示信息") , MB_ICONHAND ) ;
		}
	}
	return bReturn ;
}

BOOL CUserSystem::HaveBlockName( CString strBlockName , AcDbObjectId & Block_Id ) 
{
	BOOL bReturn = FALSE ;
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable( pBlkTbl , AcDb::kForRead ) ;
	if( Acad::eOk == pBlkTbl->getAt( strBlockName , Block_Id ) )
	{
		bReturn = TRUE ;
	}
	else
	{
		bReturn = FALSE ;
	}
	pBlkTbl->close() ;
	return bReturn ;
}

//nMode:default=0,提供块名比如pmt_XXX,nMode=1,提供的是块所在dwg路径
BOOL CUserSystem::InsertDwgFile(CString strBlockName , AcDbObjectId  &Block_Id, int nMode)
{
	BOOL bReturn = FALSE ;
	CString strBlockNameInFact = strBlockName ;
	if (nMode==1)
	{
		CString strDwgName ;
		int iLocFind1 = strBlockName.ReverseFind(_T('\\')) ;
		if (iLocFind1==-1)
		{
			return FALSE ;
		}
		strDwgName = strBlockName.Mid(iLocFind1+1) ;
		int iLocFind2 = strDwgName.Find(_T('.')) ;
		if (iLocFind2==-1)
		{
			return FALSE ;
		}
		strBlockNameInFact = strDwgName.Left(iLocFind2) ;
	}

	if( this->HaveBlockName ( strBlockNameInFact , Block_Id ) )
	{
		bReturn = TRUE ;
	}
	else
	{
		CString strDwgFileName = _T("") ;
		if (nMode==1)
		{
			strDwgFileName = strBlockName ;
		}
		else
		{
			CString strFolder ;
			int iLoc = 0 ;
			iLoc = strBlockName.Find(_T("_"), 0) ;
			if (iLoc!=-1)
			{
				strFolder = strBlockName.Mid(0, iLoc) ;
			}	
			CString strBasePath = _T("") ;
			CBlkUtility::GetBasePath(strBasePath) ;
			strDwgFileName.Format(_T("%s\\support\\%s\\%s.dwg"), strBasePath, strFolder, strBlockName) ;
		}

		AcDbDatabase * pTemp_Db = new AcDbDatabase(false);
		if( this->Verify_FileName( strDwgFileName , FALSE ) )
		{	
			Acad::ErrorStatus esRead = pTemp_Db->readDwgFile( strDwgFileName ) ;

			if( Acad::eOk == esRead )
			{
				if( Acad::eOk == m_pDb->insert( Block_Id , strBlockNameInFact  , pTemp_Db ) )
				{
					bReturn = TRUE ;
				}				
			}
		}
		delete pTemp_Db ;
	}
	return bReturn ;
}

// BOOL CUserSystem::InsertBlock(AcDbBlockReference *& pRef , CString strBlockName  , AcGePoint3d pt_3d_insert )
// {
// 	BOOL bReturn = FALSE ;
// 	AcDbObjectId objId ;
// 	if(  this->InsertDwgFile(strBlockName, objId))
// 	{
// 		pRef = new AcDbBlockReference ; 
// 		pRef->setBlockTableRecord(objId) ;
// 		pRef->setPosition( pt_3d_insert ) ;
// 		this->AddEntityToDb( (AcDbEntity * ) pRef ) ;
// 		bReturn = TRUE ;
// 	}
// 	return bReturn ;
// }

//nMode=1:strBlockName是fullpath,nMode=0:strBlockName是块名
BOOL CUserSystem::InsertBlock(AcDbBlockReference *& pRef , CString strBlockName  , AcGePoint3d pt_3d_insert , CStringArray * pStrArrayVal , BOOL bUniformFont, int nMode )
{
	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;

	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //解决后台数据库写入的文本，对齐方式问题，记住，文本写入数据库后，要把当前数据库设置为原始状态

	BOOL bReturn = FALSE ;
	AcDbObjectId objId ;
	if(  this->InsertDwgFile( strBlockName , objId, nMode ) )
	{
		pRef = new AcDbBlockReference ; 
		pRef->setBlockTableRecord( objId ) ;
		pRef->setPosition( pt_3d_insert ) ;
		//AcGeScale3d scale_3d(0.5) ;
		//pRef->setScaleFactors(scale_3d) ;
		this->AddEntityToDb( (AcDbEntity * ) pRef ) ;
		AcDbObject * pObj = NULL ;
		if( Acad::eOk == acdbOpenAcDbObject( pObj , objId , AcDb::kForRead ) )
		{
			AcDbBlockTableRecord * pBlkTblRec ;
			pBlkTblRec = (AcDbBlockTableRecord * ) pObj ;
			if( Adesk::kTrue == pBlkTblRec->hasAttributeDefinitions() ) //有属性定义;
			{
				AcDbBlockTableRecordIterator * pItor = NULL ;
				pBlkTblRec->newIterator( pItor ) ;
				AcDbEntity * pEnt = NULL ;
				int iSize = 0 ;
				iSize = pStrArrayVal->GetSize() ;
				int iCurIndex = 0 ;

				for( pItor->start() ; !pItor->done() ; pItor->step() )
				{
					pItor->getEntity( pEnt , AcDb::kForRead ) ;
					if( pEnt->isKindOf(AcDbAttributeDefinition::desc()))
					{
						AcDbAttributeDefinition * pAttDef = NULL ;
						pAttDef = ( AcDbAttributeDefinition * ) pEnt ;

						if( iCurIndex < iSize )
						{
							AcDbAttribute * pAtt = NULL ;
							pAtt = new AcDbAttribute ;
							pAtt->setTag( pAttDef->tag() ) ;

							AcDb::TextHorzMode attHMode = pAttDef->horizontalMode() ;
							AcDb::TextVertMode attVMode = pAttDef->verticalMode() ;

							pAtt->setHorizontalMode( attHMode );
							pAtt->setVerticalMode ( attVMode ) ;

							AcGePoint3d pt_3d_position( 0,0,0 ), pt_3d_align(0,0,0), Att_Def_3d ( 0,0,0 ) ;
							Att_Def_3d = pAttDef->position() ;
							pt_3d_position[X] = pt_3d_insert[X] + Att_Def_3d[X] ;
							pt_3d_position[Y] = pt_3d_insert[Y] + Att_Def_3d[Y] ;

							Att_Def_3d = pAttDef->alignmentPoint() ;
							pt_3d_align[X] = pt_3d_insert[X] + Att_Def_3d[X] ;
							pt_3d_align[Y] = pt_3d_insert[Y] + Att_Def_3d[Y] ;
							

							//////////////////////////////////////////////////////////////////////////
							if (attHMode==AcDb::kTextLeft&&attVMode==AcDb::kTextBase)
							{
								pAtt->setPosition( pt_3d_position ) ;
							}
							else if (attHMode==AcDb::kTextAlign||attHMode==AcDb::kTextFit)
							{
								pAtt->setPosition(pt_3d_position) ;
								pAtt->setAlignmentPoint(pt_3d_align) ;
							}
							else
							{
								pAtt->setAlignmentPoint(pt_3d_align) ;
							}
							//////////////////////////////////////////////////////////////////////////

							pAtt->setRotation ( pAttDef->rotation() ) ;

							if(  bUniformFont )
							{
								pAtt->setHeight( m_dFontSize ) ;
								pAtt->setWidthFactor ( m_dFontScale ) ;
							}
							else
							{
								pAtt->setHeight( pAttDef->height() ) ;
								pAtt->setWidthFactor ( pAttDef->widthFactor() );
							}

							if( pStrArrayVal->GetAt(iCurIndex ) != _T("0") )  //== 0 就不显示
							{
								pAtt->setTextString( pStrArrayVal->GetAt(iCurIndex ) ) ;
							}
							else
							{
								pAtt->setTextString( _T("") ) ;
							}

							pAtt->setInvisible ( pAttDef->isInvisible() ) ;
							pAtt->setColorIndex(pAttDef->colorIndex()) ;
							pAtt->setTextStyle(pAttDef->textStyle()) ;

							AcDbObjectId objIdAtt ;
							pRef->appendAttribute( objIdAtt , pAtt ) ;
							pAtt->close() ;
							iCurIndex = iCurIndex + 1 ;
						}
					}
					pEnt->close() ;
				}
			}
			pBlkTblRec->close() ;
		}
		bReturn = TRUE ;
	}

	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;

	return bReturn ;
}

//插入块,不指定属性,采用默认值,本函数主要用来替换块时批量插入;nByBlkOrPath=1:strBlockName是fullpath,nByBlkOrPath=0:strBlockName是块名
BOOL CUserSystem::InsertBlock(AcDbBlockReference *&pRef, CString strBlockName, AcGePoint3d Point_3d, BOOL bUniformFont, int nByBlkOrPath, BOOL bWithAtt)
{
	BOOL bReturn = FALSE ;

	AcDbObjectId objId ;
	if (this->InsertDwgFile( strBlockName, objId, nByBlkOrPath) )
	{
		pRef = new AcDbBlockReference ; 
		pRef->setBlockTableRecord( objId ) ;
		pRef->setPosition( Point_3d ) ;
		this->AddEntityToDb( pRef ) ;
		AcDbObject * pObj = NULL ;
		if (bWithAtt)
		{
			if( Acad::eOk == acdbOpenAcDbObject( pObj , objId , AcDb::kForRead ) )
			{
				AcDbBlockTableRecord * pBlkTblRec ;
				pBlkTblRec = (AcDbBlockTableRecord * ) pObj ;
				if( Adesk::kTrue == pBlkTblRec->hasAttributeDefinitions() ) //有属性定义;
				{
					AcDbBlockTableRecordIterator * pItor = NULL ;
					pBlkTblRec->newIterator( pItor ) ;
					AcDbEntity * pEnt = NULL ;

					for( pItor->start() ; !pItor->done() ; pItor->step() )
					{
						pItor->getEntity( pEnt , AcDb::kForRead ) ;					
						if( pEnt->isKindOf(AcDbAttributeDefinition::desc()))
						{
							AcDbAttributeDefinition * pAttDef = NULL ;
							pAttDef = AcDbAttributeDefinition::cast(pEnt) ;

							AcDbAttribute  * pAtt = new AcDbAttribute ;

							pAtt->setInvisible ( pAttDef->isInvisible() ) ;

							pAtt->setHorizontalMode( pAttDef->horizontalMode() );
							pAtt->setVerticalMode ( pAttDef->verticalMode() ) ;
							AcGePoint3d Att_3d( 0,0,0 ) , Att_Def_3d ( 0,0,0 ) ;
							Att_Def_3d = pAttDef->position() ;
							Att_3d[X] = Point_3d[X] + Att_Def_3d[X] ;
							Att_3d[Y] = Point_3d[Y] + Att_Def_3d[Y] ;
							pAtt->setPosition( Att_3d ) ;

							Att_Def_3d = pAttDef->alignmentPoint() ;
							Att_3d[X] = Point_3d[X] + Att_Def_3d[X] ;
							Att_3d[Y] = Point_3d[Y] + Att_Def_3d[Y] ;
							pAtt->setAlignmentPoint( Att_3d );
							pAtt->setRotation ( pAttDef->rotation() ) ;

							if(  bUniformFont )
							{
								pAtt->setHeight( m_dFontSize ) ;
								pAtt->setWidthFactor ( m_dFontScale ) ;
							}
							else
							{
								pAtt->setHeight( pAttDef->height() ) ;
								pAtt->setWidthFactor ( pAttDef->widthFactor() );
							}

							pAtt->setTag(pAttDef->tag()) ;
							pAtt->setTextString(pAttDef->textString()) ;
							pAtt->setColorIndex(pAttDef->colorIndex()) ;
							pAtt->setTextStyle(pAttDef->textStyle()) ;

							AcDbObjectId Att_Id ;
							pRef->appendAttribute( Att_Id , pAtt ) ;
							pAtt->close() ;	
						}
						pEnt->close() ;
					}
				}
				pBlkTblRec->close() ;
			}
		}
		bReturn = TRUE ;
	}

	return bReturn ;
}

//////////////////////////////////////////////////////////////////////////
//将excel表格中单元格的对齐方式转换为cad中mtext的对齐方式  ;
//VerticalAlignment:垂直方向
// case -4160 :  //靠上
// case -4108 :  //居中
// case -4107 :  //靠下
//HorizontalAlignment:水平方向
// case 1:      //默认
// case -4108:  //居中
// case -4131 : //靠左
// case -4152 : //靠右	  
BOOL CUserSystem::ChangeExcelAlignToMText(long lVerAlign, long lHorAlign, AcDbMText::AttachmentPoint &iAttachment) 
{
	//
	BOOL bReturn = FALSE ;
	// case -4160 : 靠上	
	if (lVerAlign==-4160)
	{
		switch(lHorAlign)
		{
		case -4108:
			iAttachment = AcDbMText::kTopCenter ;
			break;
		case -4131:
			iAttachment = AcDbMText::kTopLeft ;
			break;
		case -4152:
			iAttachment = AcDbMText::kTopRight ;
			break;
		default:
			iAttachment = AcDbMText::kTopLeft ;
		}
	}
	// case -4108 : 居中	
	else if (lVerAlign==-4108)
	{
		switch(lHorAlign)
		{
		case -4108:
			iAttachment = AcDbMText::kMiddleCenter ;
			break;
		case -4131:
			iAttachment = AcDbMText::kMiddleLeft ;
			break;
		case -4152:
			iAttachment = AcDbMText::kMiddleRight ;
			break;
		default:
			iAttachment = AcDbMText::kMiddleLeft ;
		}		
	} 
	// case -4107 : 靠下	
	else if (lVerAlign==-4107)
	{
		switch(lHorAlign)
		{
		case -4108:
			iAttachment = AcDbMText::kBottomCenter ;
			break;
		case -4131:
			iAttachment = AcDbMText::kBottomLeft ;
			break;
		case -4152:
			iAttachment = AcDbMText::kBottomRight ;
			break;
		default:
			iAttachment = AcDbMText::kBottomLeft ;
		}		
	}
	else
	{
		//iAttachment = AcDbMText::kBottomLeft ;
		switch(lHorAlign)
		{
		case -4108:
			iAttachment = AcDbMText::kBottomCenter ;
			break;
		case -4131:
			iAttachment = AcDbMText::kBottomLeft ;
			break;
		case -4152:
			iAttachment = AcDbMText::kBottomRight ;
			break;
		default:
			iAttachment = AcDbMText::kBottomLeft ;
		}		

	}
	if (iAttachment!=NULL)
	{
		bReturn = TRUE ;
	}
	return bReturn ;

}

//////////////////////////////////////////////////////////////////////////
//由Mtext的不同的对齐方式，计算mtext的基点位置 ;
//参数 ：base_pt mtext左下脚坐标 ;
//
BOOL CUserSystem::GetMTextBasePt(AcDbMText::AttachmentPoint iAttachment, ads_point base_pt, double dWidth, double dHight, ads_point &real_pt) 
{
	BOOL bReturn = TRUE ;
	switch(iAttachment) 
	{
	case AcDbMText::kTopLeft :
		real_pt[0] = base_pt[0]+0.5 ;
		real_pt[1] = base_pt[1]+dHight-0.5 ;
		break;
	case AcDbMText::kTopCenter :
		real_pt[0] = base_pt[0]+dWidth/2 ;
		real_pt[1] = base_pt[1]+dHight-0.5 ;		
		break;
	case AcDbMText::kTopRight :
		real_pt[0] = base_pt[0]+dWidth-0.5 ;
		real_pt[1] = base_pt[1]+dHight-0.5 ;		
		break;	
	case AcDbMText::kMiddleLeft :
		real_pt[0] = base_pt[0]+0.5 ;
		real_pt[1] = base_pt[1]+dHight/2 ;
		break;
	case AcDbMText::kMiddleCenter :
		real_pt[0] = base_pt[0]+dWidth/2 ;
		real_pt[1] = base_pt[1]+dHight/2 ;		
		break;
	case AcDbMText::kMiddleRight :
		real_pt[0] = base_pt[0]+dWidth-0.5 ;
		real_pt[1] = base_pt[1]+dHight/2 ;		
		break;	
	case AcDbMText::kBottomLeft :
		real_pt[0] = base_pt[0]+0.5 ;
		real_pt[1] = base_pt[1]+0.5 ;
		break;
	case AcDbMText::kBottomCenter :
		real_pt[0] = base_pt[0]+dWidth/2 ;
		real_pt[1] = base_pt[1]+0.5 ;		
		break;
	case AcDbMText::kBottomRight :
		real_pt[0] = base_pt[0]+dWidth-0.5 ;
		real_pt[1] = base_pt[1]+0.5 ;		
		break;
	default:
		real_pt[0] = base_pt[0]+0.5 ;
		real_pt[1] = base_pt[1]+0.5 ;
		bReturn = FALSE ;

	}
	return bReturn ;

}

AcDbObjectId CUserSystem::CreateTextStyle(CString strStyleName,CString strFontName,CString strBigFontFileName,double dSize,double dxScale)
{
	AcDbObjectId fontStyleId ;

	AcDbTextStyleTable * pTextStyleTable ;
	m_pDb->getTextStyleTable(pTextStyleTable, AcDb::kForWrite) ;
	if (Acad::eOk==pTextStyleTable->getAt(strStyleName, fontStyleId,AcDb::kForWrite)) 
	{
		pTextStyleTable->close() ;		
	}
	else
	{
		AcDbTextStyleTableRecord * pTextStyle = new AcDbTextStyleTableRecord ;

		pTextStyle->setName(strStyleName) ;
		pTextStyle->setFont(strFontName, 0, 0, 134, 1) ;
		if (strBigFontFileName!="")
		{
			pTextStyle->setBigFontFileName(strBigFontFileName) ;
		}
		pTextStyle->setTextSize(dSize) ;
		pTextStyle->setXScale(dxScale) ;
		pTextStyleTable->add(fontStyleId, pTextStyle) ;
		pTextStyle->close() ;
		pTextStyleTable->close() ;
	}	

	return fontStyleId ;

}

BOOL CUserSystem::GetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, CString &strVal)
{
	BOOL bResult = FALSE ;
	struct resbuf * pRb;
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		pRb = pRb->rbnext;
		if (pRb->restype < 1010)
		{
			strVal = pRb -> resval.rstring ;
			bResult = TRUE ;
		}
		ads_relrb(pRb);
	}
	return bResult ;
}

BOOL CUserSystem::GetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, CString strFieldName, CString &strVal)
{
	BOOL bResult = FALSE ;
	struct resbuf * pRb , * pTemp ;
	pRb = pEnt->xData(chAppName);
	if (pRb != NULL )
	{
		strFieldName.MakeLower () ;
		CString strTemp ;

		for ( pTemp = pRb ; pTemp ->rbnext != NULL ; pTemp = pTemp->rbnext ) 
		{
			if (pTemp->restype < 1010)
			{
				strTemp = pTemp -> resval.rstring ;
				strTemp.MakeLower () ;
				if( strTemp.Find ( strFieldName , 0 ) != -1 )
				{
					int iLength = 0 , iField_Len = 0 ;
					iLength = strTemp.GetLength() ;
					iField_Len = strFieldName.GetLength () ;

					if( iLength  > iField_Len + 1 )
					{
						strVal = strTemp.Mid ( iField_Len + 1 ) ; 
						bResult = TRUE ;
						break ;
					}
				}
			}
		}

		ads_relrb(pRb);
	}
	return bResult ;
}

// BOOL CUserSystem::GetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, short &iVal)
// {
// 	BOOL bResult = FALSE ;
// 	struct resbuf * pRb;
// 	pRb = pEnt->xData(chAppName);
// 	if(pRb != NULL)
// 	{
// 		pRb = pRb->rbnext;
// 		iVal = pRb -> resval.rint ;
// 		bResult = TRUE ;	
// 		ads_relrb(pRb);
// 	}
// 	return bResult ;
// }

BOOL CUserSystem::GetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, int &iVal)
{
	BOOL bResult = FALSE ;
	long lVal = 0 ;
	bResult = this->GetXdata(pEnt, chAppName, lVal) ;
	iVal = (int)lVal ;

	return bResult ;
}

BOOL CUserSystem::GetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, long &lVal)
{
	BOOL bResult = FALSE ;
	struct resbuf * pRb;
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		pRb = pRb->rbnext;
		lVal = pRb -> resval.rlong  ;
		bResult = TRUE ;	
		ads_relrb(pRb);
	}
	return bResult ;
}

BOOL CUserSystem::GetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, double &dVal)
{
	BOOL bResult = FALSE ;
	struct resbuf * pRb;
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		pRb = pRb->rbnext;
		dVal = pRb -> resval.rreal ;
		bResult = TRUE ;	
		ads_relrb(pRb);
	}
	return bResult ;
}

BOOL CUserSystem::GetXdata(long nHandle,  const TCHAR *chAppName, double &dVal)
{
	BOOL bResult = FALSE ;

	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		bResult = GetXdata(pEnt, chAppName, dVal) ;
		pEnt->close() ;
	}
	return bResult ;
}

BOOL CUserSystem::GetXdata(long nHandle, const TCHAR *chAppName, long &lVal)
{
	BOOL bResult = FALSE ;

	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		bResult = GetXdata(pEnt, chAppName, lVal) ;
		pEnt->close() ;
	}
	return bResult ;
}

BOOL CUserSystem::GetXdata(long nHandle, const TCHAR *chAppName, CString &strVal)
{
	BOOL bResult = FALSE ;

	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		bResult = GetXdata(pEnt, chAppName, strVal) ;
		pEnt->close() ;
	}
	return bResult ;
}

int CUserSystem::ConvExcelCorlorToDwg(int iColorIndex)
{
	int iReturn = 0 ;
	if (iColorIndex==1)
	{
		iReturn = 7 ;
	}
	else
	{
		switch(iColorIndex)
		{
		case 3:
			iReturn = 1 ; 
			break;
		case 4:
			iReturn = 3 ;
			break;
		case 5:
		case 32:
		case 41:
		case 49:
			iReturn = 5 ;
			break;
		case 6:
		case 27:
			iReturn = 2 ;
			break; 
		case 7:
		case 26:
			iReturn = 6 ;
			break;
		case 8:
		case 28:
		case 33:
		case 34:
		case 42:		
			iReturn = 4 ;
			break;
		case 9:
		case 30:
		case 53:
			iReturn = 14 ;
			break;
		case 10:
		case 43:
			iReturn = 76 ;
			break;			
		default:
			iReturn = 0 ;
			break;
		}
	}

	return iReturn ;
}

BOOL CUserSystem::ExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt, double dHeightRate, double dWidthRate)
{
	BOOL bReturn = TRUE ;

	int iStartColNum = CBlkUtility::GetNumFromExcelColName(iStartCol) ;
	int iEndColNum = CBlkUtility::GetNumFromExcelColName(iEndCol) ;

	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	//WorksheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	SheetsPtr pWorksheets = NULL ;

	/*
	AcDbTextStyleTable * pTextStyleTb = NULL ;
	m_pDb->getTextStyleTable(pTextStyleTb, AcDb::kForRead) ;
	AcDbObjectId textStyleId ;
	pTextStyleTb->getAt("standard",textStyleId) ;
	pTextStyleTb->close() ;	
	*/

	AcDbObjectId textStyleId = this->CreateTextStyle(_T("zhffont"), _T("txt"), _T(""), 2.5, 0.73) ;	


	CArray<double, double> dArrayWidths ;
	CArray<double, double> dArrayHeights ;
	try
	{

		if (bFirst) 
		{
			::AfxOleInit ( ) ;
			bFirst = FALSE ;
		}		
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBook = pWorkBooks->Open( (_bstr_t)strExcelFileName ) ;
		_variant_t vt_index ;
		//VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets() ; //m_pExcel->GetSheets( ) ;
		//获得sheet1的指针
		pSheet = pWorksheets->GetItem( vt_index ) ;

		//获得各列的宽度 ;
		for(int ch=iStartColNum; ch<=iEndColNum; ch++)
		{
			CString strCellName ;
			CString strColName = CBlkUtility::GetExcelFieldName(ch) ;
			strCellName.Format(_T("%s1"), strColName) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_width ;
			range_width = pRange->GetColumnWidth() ;
			dArrayWidths.Add((double)range_width) ;	
		}
		//获得各行的高度 ;
		for(int iLineIndex=iStartRow; iLineIndex<=iEndRow; iLineIndex++)
		{
			CString strCellName ;
			strCellName.Format(_T("A%d"), iLineIndex) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_height ;
			range_height = pRange->GetRowHeight() ;
			dArrayHeights.Add((double)range_height) ;
			//CString strTmp ;
			//strTmp.Format(_T("%s-%f"),strCellName, (double)range_height) ;
			//AfxMessageBox(strTmp) ;
		}


		//CString straa ;
		//straa.Format(_T("%d-%d"),dArrayWidths.GetSize(), dArrayHeights.GetSize()) ;
		//AfxMessageBox(straa) ;
		//////////////////////////////////////////////////////////////////////////
		//begin画文字		
		double dHeightSumTmp = 0 ;
		for(int iRowIndex=iStartRow; iRowIndex<=iEndRow; iRowIndex++)
		{

			double dWidthSumTmp = 0 ;

			//逐列处理
			for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
			{
				//int iColIndexTmp = (int)iColIndex-64 ;
				//if (iColIndex == 1) 
				//{
				//	AfxMessageBox(_T("1")) ;
				//}

				CString strCellName ; 
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;


				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列		


					/*
					if (strCellName=="C6")
					{
					CString strTmp ;
					strTmp.Format(_T("iMergeCellsColNum=%d"),iMergeCellsColNum) ;
					AfxMessageBox(strTmp) ;
					}
					*/

					//只在起始行列时处理一次合并格
					if (iRowIndex==iMergeCellStartRow && iColIndex==iMergeCellStartCol)
					{

						//合并格的宽度和高度;
						double dCellsWidth = 0 ;
						double dCellsHeight = 0 ;
						for(int iMergeCellCol=0; iMergeCellCol<iMergeCellsColNum; iMergeCellCol++)
						{
							dCellsWidth = dCellsWidth + dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-iStartColNum) ;
							//CString strtt ;
							//strtt.Format(_T("%s-%f-%d-%f"),strCellName,dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-(iStartCol-64)),iMergeCellStartCol+iMergeCellCol-(iStartCol-64),dArrayWidths.GetAt(0)) ;
							//AfxMessageBox(strtt) ;
						}
						for(int iMergeCellRow=0; iMergeCellRow<iMergeCellsRowNum; iMergeCellRow++)
						{
							dCellsHeight = dCellsHeight + dArrayHeights.GetAt(iMergeCellStartRow+iMergeCellRow-iStartRow) ;
							//int iTmp = iMergeCellStartRow + iMergeCellRow - iStartRow ;
							//double dTmp = dArrayHeights.GetAt(iTmp) ;
						}

						//取得文本 ;
						_variant_t range_var = pRange->GetText() ;	
						strCellText = (CString ) range_var.bstrVal ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;

						strCellText.Replace(_T("{"),_T("\\{")) ;
						strCellText.Replace(_T("}"),_T("\\}")) ;

						//取得文本颜色,大小  ;
						MSExcel::FontPtr font = pRange->Font ;
						_variant_t colorIndex = font->ColorIndex ;
						_variant_t fontSize = font->GetSize() ;
						int iTxtColorIndex =ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
						int iFontSize = (int)fontSize.dblVal ;

						//取得文字对齐方式 ;
						_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
						_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
						//映射到cad中的对齐方式 ;
						AcDbMText::AttachmentPoint iAttachment ;
						if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						}


						//////////////////////////////////////////////////////////////////////////

						//绘制合并格内容 ;

						//合并格左下角坐标 ;
						double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
						double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellsHeight)*dHeightRate ;

						ads_point mergeCell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
						ads_point mergeCell_Real_pt = {0, 0, 0} ;

						//合并格画到dwg中的宽度和高度 ;
						double dTextWidth = dCellsWidth * dWidthRate ;
						double dTextHeight = dCellsHeight * dHeightRate ;						

						if (!(this->GetMTextBasePt(iAttachment, mergeCell_Base_pt, dTextWidth, dTextHeight, mergeCell_Real_pt))) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						} 
						//mergeCell_Real_pt[0]+=1 ;
						//mergeCell_Real_pt[1]+=0.5 ;
						dTextWidth = dTextWidth - 1 ;
						//this->AddMText()
						this->AddMText(mergeCell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.25, dTextWidth, iTxtColorIndex) ;

						//纪录宽度和 ;
						dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

					}
					//不是起始行列则继续下一列
					else
					{
						dWidthSumTmp+=dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						continue ;
					}
				}
				//不是合并格
				else
				{
					//取得文本 ;
					_variant_t range_var = pRange->GetText() ;	
					strCellText = (CString ) range_var.bstrVal ;

					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;


					strCellText.Replace(_T("{"),_T("\\{")) ;
					strCellText.Replace(_T("}"),_T("\\}")) ;

					//取得文本颜色,大小  ;
					MSExcel::FontPtr font = pRange->Font ;
					_variant_t colorIndex = font->ColorIndex ;
					_variant_t fontSize = font->GetSize() ;
					int iTxtColorIndex =this->ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
					int iFontSize = (int)fontSize.dblVal ;

					//取得文字对齐方式 ;
					_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
					_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
					//映射到cad中的对齐方式 ;
					AcDbMText::AttachmentPoint iAttachment ;
					if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
					{
						iAttachment = AcDbMText::kBottomLeft ;
					}
					//CString strtt ;
					//strtt.Format(_T("%d"),(int)iAttachment) ;
					//AfxMessageBox(strtt) ;


					//单元格宽度和高度 ;
					double dCellWidth = dArrayWidths.GetAt(iColIndex-iStartColNum) ;
					double dCellHeight = dArrayHeights.GetAt(iRowIndex-iStartRow) ;

					//////////////////////////////////////////////////////////////////////////

					//绘制单元格内容 ;
					//单元格左下角坐标 ;
					double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
					double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellHeight)*dHeightRate ;

					ads_point Cell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
					ads_point Cell_Real_pt = {0, 0, 0} ;

					//单元格画到dwg中的宽度和高度 ;
					double dTextWidth = dCellWidth * dWidthRate ;
					double dTextHeight = dCellHeight * dHeightRate ;						

					//this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt) ;
					//Cell_Real_pt[0]+=1 ;
					//Cell_Real_pt[1]+=0.5 ;
					//dTextWidth-=2 ;
					if (!(this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt))) 
					{
						iAttachment = AcDbMText::kBottomLeft ;
					} 
					//mergeCell_Real_pt[0]+=1 ;
					//mergeCell_Real_pt[1]+=0.5 ;
					dTextWidth-=1 ;

					this->AddMText(Cell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.23, dTextWidth, iTxtColorIndex) ;

					//纪录宽度和 ;
					dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;					


				}
			}

			//处理完当前行后，得到的高度和 ;
			dHeightSumTmp += dArrayHeights.GetAt(iRowIndex-iStartRow) ;

		}
		//end 画文字
		//////////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////////
		//begin画行线
		double dHeightSumTmp_2 = 0 ;
		for(int iRowIndex2=iStartRow; iRowIndex2<=iEndRow; iRowIndex2++)
		{
			double dWidthSumTmp = 0 ;
			//CArray<AcGePoint3dArray *, AcGePoint3dArray *> carryRowpoint ;
			CArray<double , double > carrayRowStartPoint ;
			CArray<double , double > carrayRowEndPoint ;
			double dStartP = 0 ;
			double dEndP = 0 ;

			//得到总行高
			dHeightSumTmp_2 += dArrayHeights.GetAt(iRowIndex2-iStartRow) ;		


			//逐列处理
			for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
			{

				//int iColIndexTmp = (int)iColIndex-64 ;				

				CString strCellName ; 
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex2) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				//_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				_variant_t bIsMergeCells = pRange->MergeCells ;	

				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

					//CString strtt;
					//strtt.Format(_T("strCellName-%s\niMergeCellsRowNum-%d\niMergeCellsColNum-%d\niMergeCellStartRow-%d\niMergeCellStartCol%d"),strCellName,iMergeCellsRowNum,iMergeCellsColNum,iMergeCellStartRow,iMergeCellStartCol) ;
					//AfxMessageBox(strtt) ;

					//是合并格的最下面的一行,处理方式和非合并格一样
					if (iRowIndex2 == (iMergeCellStartRow+iMergeCellsRowNum-1)) 
					{
						dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						//是起始列
						if (iColIndex ==iStartColNum )
						{
							carrayRowStartPoint.Add(dStartP) ;
						}
						else if (iColIndex == iEndColNum)
						{
							carrayRowEndPoint.Add(dEndP) ;
						}
					}
					else
					{			
						//合并格起始列时处理
						if (iColIndex==iMergeCellStartCol)
						{
							//处理格第一列
							if (iColIndex == iStartColNum)
							{
								carrayRowStartPoint.Add(dStartP) ;
							}
							carrayRowEndPoint.Add(dEndP) ;
							//dStartP = dEndP ;
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;

							//合并格只有一列，起始列就是最后一列
							if (iMergeCellsColNum == 1)
							{
								dStartP = dEndP ;
								carrayRowStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iColIndex == iEndColNum)
								{
									carrayRowEndPoint.Add(dEndP) ;
								}							

							}

						}
						//合并格最后一列处理
						else if(iColIndex == iMergeCellStartCol+iMergeCellsColNum-1)
						{
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
							dStartP = dEndP ;
							carrayRowStartPoint.Add(dStartP) ;
							//是处理格的最后一列
							if (iColIndex == iEndColNum)
							{
								carrayRowEndPoint.Add(dEndP) ;
							}
						}
						//合并格中间的列处理
						else
						{
							dEndP =  dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						}
					}

				}
				//不是合并格
				else
				{
					dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
					//是起始列
					if (iColIndex ==iStartColNum )
					{
						carrayRowStartPoint.Add(dStartP) ;
					}
					else if (iColIndex == iEndColNum)
					{
						carrayRowEndPoint.Add(dEndP) ;
					}

				}
			}

			//////////////////////////////////////////////////////////////////////////
			//
			int iStartpNum = carrayRowStartPoint.GetSize() ;
			int iEndpNum = carrayRowEndPoint.GetSize() ;
			if (iStartpNum == iEndpNum)
			{
				//当前线条的y坐标
				double dRowBaseY = base_pt[1] - (dHeightSumTmp_2)*dHeightRate ;
				for(int iRowLineNum=0; iRowLineNum<carrayRowStartPoint.GetSize(); iRowLineNum++)
				{
					double dRowBaseStartX = base_pt[0] + (carrayRowStartPoint.GetAt(iRowLineNum))*dWidthRate ;
					double dRowBaseEndX = base_pt[0] + (carrayRowEndPoint.GetAt(iRowLineNum))*dWidthRate ;

					AcGePoint3d start_3d(dRowBaseStartX, dRowBaseY, 0) ;
					AcGePoint3d end_3d(dRowBaseEndX, dRowBaseY, 0) ;
					this->AddAcDbLine(start_3d, end_3d) ;
				}
			}
			else
			{
				AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
			}

		}


		//end画行线
		//////////////////////////////////////////////////////////////////////////	

		//////////////////////////////////////////////////////////////////////////
		//begin画列线
		double dWidthSumTmp = 0 ;
		for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
		{
			//int iColIndexTmp = (int)iColIndex-64 ;				

			double dHeightSumTmp = 0 ;
			CArray<double , double > carrayColStartPoint ;
			CArray<double , double > carrayColEndPoint ;
			double dStartP = 0 ;
			double dEndP = 0 ;

			//得到总列宽
			dWidthSumTmp += dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

			//列名称
			CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;

			//逐行处理
			for(int iRowIndex=iStartRow; iRowIndex<=iEndRow;iRowIndex++)
			{
				CString strCellName ; 
				CString strCellText ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				//_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				_variant_t bIsMergeCells = pRange->MergeCells ;	

				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

					//是合并格的最右边的一列,处理方式和非合并格一样
					if (iColIndex == (iMergeCellStartCol+iMergeCellsColNum-1)) 
					{
						dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
						//是起始行
						if (iRowIndex ==iStartRow )
						{
							carrayColStartPoint.Add(dStartP) ;
						}
						else if (iRowIndex == iEndRow)
						{
							carrayColEndPoint.Add(dEndP) ;
						}
					}
					//不是合并格的最右边的一列					
					else
					{					
						//合并格起始行时处理
						if (iRowIndex==iMergeCellStartRow )
						{
							//处理格第一行
							if (iRowIndex == iStartRow)
							{
								carrayColStartPoint.Add(dStartP) ;
							}

							carrayColEndPoint.Add(dEndP) ;
							//dStartP = dEndP ;
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;

							//合并格只有一行，起始行即是最后一行
							if (iMergeCellsRowNum == 1)
							{
								//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
								dStartP = dEndP ;
								carrayColStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iRowIndex == iEndRow)
								{
									carrayColEndPoint.Add(dEndP) ;
								}

							}

						}
						//合并格最后一行处理
						else if(iRowIndex == iMergeCellStartRow+iMergeCellsRowNum-1)
						{
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							dStartP = dEndP ;
							carrayColStartPoint.Add(dStartP) ;
							//是处理格的最后一列
							if (iRowIndex == iEndRow)
							{
								carrayColEndPoint.Add(dEndP) ;
							}
						}
						//合并格中间的行处理
						else
						{
							dEndP =  dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
						}
					}


				}

				//不是合并格
				else
				{
					dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
					//是起始行
					if (iRowIndex ==iStartRow )
					{
						carrayColStartPoint.Add(dStartP) ;
					}
					else if (iRowIndex == iEndRow)
					{
						carrayColEndPoint.Add(dEndP) ;
					}

				}				

			}

			//////////////////////////////////////////////////////////////////////////
			//画线条
			int iStartpNum = carrayColStartPoint.GetSize() ;
			int iEndpNum = carrayColEndPoint.GetSize() ;
			if (iStartpNum == iEndpNum)
			{
				//当前线条的x坐标
				double dRowBaseX = base_pt[0] + (dWidthSumTmp)*dWidthRate ;
				for(int iColLineNum=0; iColLineNum<carrayColStartPoint.GetSize(); iColLineNum++)
				{
					double dColBaseStartY = base_pt[1] - (carrayColStartPoint.GetAt(iColLineNum))*dHeightRate ;
					double dColBaseEndY= base_pt[1] - (carrayColEndPoint.GetAt(iColLineNum))*dHeightRate ;

					AcGePoint3d start_3d(dRowBaseX, dColBaseStartY, 0) ;
					AcGePoint3d end_3d(dRowBaseX, dColBaseEndY, 0) ;
					this->AddAcDbLine(start_3d, end_3d) ;
				}
			}
			else
			{
				AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
			}


		}
		//end画列线
		//////////////////////////////////////////////////////////////////////////

		//画左边线
		double dLeftBottomPointX = base_pt[0] ;
		double dLeftBottomPointY = base_pt[1] - dHeightSumTmp_2 * dHeightRate ;
		AcGePoint3d lefttop_p(0, 0, 0) ;
		AcGePoint3d leftbottom_p(dLeftBottomPointX, dLeftBottomPointY, 0) ;
		this->ConvertPtAdsTo3D(base_pt, lefttop_p) ;
		this->AddAcDbLine(lefttop_p, leftbottom_p) ;

		//画上边线
		double dRightTopPointX = base_pt[0] + dWidthSumTmp * dWidthRate ;
		double dRightTopPointY = base_pt[1] ;
		AcGePoint3d righttop_p(dRightTopPointX, dRightTopPointY, 0) ;
		this->AddAcDbLine(lefttop_p, righttop_p) ;

		///关闭打开的各个资源
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		//m_pExcel->Release() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		bReturn = FALSE ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		bReturn = FALSE ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return bReturn ;
}
BOOL CUserSystem::LSBExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt ,double &d_width,double dHeightRate, double dWidthRate)
{
	BOOL bReturn = TRUE ;

	int iStartColNum = CBlkUtility::GetNumFromExcelColName(iStartCol) ;
	int iEndColNum = CBlkUtility::GetNumFromExcelColName(iEndCol) ;
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	//WorksheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	SheetsPtr pWorksheets = NULL ;
	AcDbObjectId textStyleId = this->CreateTextStyle(_T("zhffont"), _T("txt"), _T(""), 2.5, 0.73) ;	
	CArray<double, double> dArrayWidths ;
	CArray<double, double> dArrayHeights ;
	try
	{

		if (bFirst) 
		{
			::AfxOleInit ( ) ;
			bFirst = FALSE ;
		}		
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks () ;
		pWorkBook = pWorkBooks->Open( (_bstr_t)strExcelFileName ) ;
		_variant_t vt_index ;
		//VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		pWorksheets  = m_pExcel->GetSheets() ; //m_pExcel->GetSheets( ) ;
		//获得sheet1的指针
		pSheet = pWorksheets->GetItem( vt_index ) ;
		double dHeightSumTmp = 0 ;
		double dHeightSumTmp_2 = 0 ;
		//获得各列的宽度 ;
		for(int ch=iStartColNum; ch<=iEndColNum; ch++)
		{
			CString strCellName ;
			CString strColName = CBlkUtility::GetExcelFieldName(ch) ;
			strCellName.Format(_T("%s1"), strColName) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_width ;
			range_width = pRange->GetColumnWidth() ;
			dArrayWidths.Add((double)range_width) ;	
		}
		//获得各行的高度 ;
		if(iStartRow>=3)
		{
			for(int iLineIndex=1;iLineIndex<=2;iLineIndex++)
			{
				CString strCellName ;
				strCellName.Format(_T("A%d"), iLineIndex) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
				_variant_t range_height ;
				range_height = pRange->GetRowHeight() ;
				dArrayHeights.Add((double)range_height);
			}
		}
		for(int iLineIndex=iStartRow; iLineIndex<=iEndRow; iLineIndex++)
		{
			CString strCellName ;
			strCellName.Format(_T("A%d"), iLineIndex) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_height ;
			range_height = pRange->GetRowHeight() ;
			dArrayHeights.Add((double)range_height) ;
		}
		//begin画文字
		dHeightSumTmp=0;
		if(iStartRow>=3)
		{
			//写表头
			for(int iRowIndex=1; iRowIndex<=2; iRowIndex++)
			{

				double dWidthSumTmp = 0 ;
				//逐列处理
				for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
				{
					CString strCellName ; 
					CString strCellText ;
					CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
					strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					_variant_t bIsMergeCells = pRange->MergeCells ;	
					//_variant_t bIsMergeCells = pRange->GetMergeCells();
					//是合并格
					if(bIsMergeCells.boolVal == -1)
					{
						//处理合并格
						RangePtr pRangeTmp ;
						pRangeTmp = pRange->GetMergeArea() ;					 
						long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
						long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数
						long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
						long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列		
						//只在起始行列时处理一次合并格
						if (iRowIndex==iMergeCellStartRow && iColIndex==iMergeCellStartCol)
						{

							//合并格的宽度和高度;
							double dCellsWidth = 0 ;
							double dCellsHeight = 0 ;
							for(int iMergeCellCol=0; iMergeCellCol<iMergeCellsColNum; iMergeCellCol++)
							{
								dCellsWidth = dCellsWidth + dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-iStartColNum) ;
							}
							for(int iMergeCellRow=0; iMergeCellRow<iMergeCellsRowNum; iMergeCellRow++)
							{
								dCellsHeight = dCellsHeight + dArrayHeights.GetAt(iMergeCellStartRow+iMergeCellRow-iStartRow) ;
							}
							//取得文本 ;
							_variant_t range_var = pRange->GetText() ;	
							strCellText = (CString ) range_var.bstrVal ;
							strCellText.TrimLeft() ;
							strCellText.TrimRight() ;

							strCellText.Replace(_T("{"),_T("\\{")) ;
							strCellText.Replace(_T("}"),_T("\\}")) ;

							//取得文本颜色,大小  ;
							MSExcel::FontPtr font = pRange->Font ;
							_variant_t colorIndex = font->ColorIndex ;
							_variant_t fontSize = font->GetSize() ;
							int iTxtColorIndex =ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
							int iFontSize = (int)fontSize.dblVal ;
							//取得文字对齐方式 ;
							_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
							_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
							//映射到cad中的对齐方式 ;
							AcDbMText::AttachmentPoint iAttachment ;
							if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
							{
								iAttachment = AcDbMText::kBottomLeft ;
							}


							//////////////////////////////////////////////////////////////////////////

							//绘制合并格内容 ;

							//合并格左下角坐标 ;
							double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
							double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellsHeight)*dHeightRate ;

							ads_point mergeCell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
							ads_point mergeCell_Real_pt = {0, 0, 0} ;

							//合并格画到dwg中的宽度和高度 ;
							double dTextWidth = dCellsWidth * dWidthRate ;
							double dTextHeight = dCellsHeight * dHeightRate ;						

							if (!(this->GetMTextBasePt(iAttachment, mergeCell_Base_pt, dTextWidth, dTextHeight, mergeCell_Real_pt))) 
							{
								iAttachment = AcDbMText::kBottomLeft ;
							} 
							//mergeCell_Real_pt[0]+=1 ;
							//mergeCell_Real_pt[1]+=0.5 ;
							dTextWidth = dTextWidth - 1 ;
							//this->AddMText()
							this->AddMText(mergeCell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.25, dTextWidth, iTxtColorIndex) ;
							//纪录宽度和 ;
							dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;		
						}
						//不是起始行列则继续下一列
						else
						{
							dWidthSumTmp+=dArrayWidths.GetAt(iColIndex-iStartColNum) ;
							continue ;
						}
					}
					//不是合并格
					else
					{
						//取得文本 ;
						_variant_t range_var = pRange->GetText() ;	
						strCellText = (CString ) range_var.bstrVal ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						strCellText.Replace(_T("{"),_T("\\{")) ;
						strCellText.Replace(_T("}"),_T("\\}")) ;
						//取得文本颜色,大小  ;
						MSExcel::FontPtr font = pRange->Font ;
						_variant_t colorIndex = font->ColorIndex ;
						_variant_t fontSize = font->GetSize() ;
						int iTxtColorIndex =this->ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
						int iFontSize = (int)fontSize.dblVal ;
						//取得文字对齐方式 ;
						_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
						_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
						//映射到cad中的对齐方式 ;
						AcDbMText::AttachmentPoint iAttachment ;
						if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						}
						//单元格宽度和高度 ;
						double dCellWidth = dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						double dCellHeight = dArrayHeights.GetAt(iRowIndex-iStartRow) ;
						//////////////////////////////////////////////////////////////////////////
						//绘制单元格内容 ;
						//单元格左下角坐标 ;
						double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
						double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellHeight)*dHeightRate ;
						ads_point Cell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
						ads_point Cell_Real_pt = {0, 0, 0} ;
						//单元格画到dwg中的宽度和高度 ;
						double dTextWidth = dCellWidth * dWidthRate ;
						double dTextHeight = dCellHeight * dHeightRate ;						
						//this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt) ;
						//Cell_Real_pt[0]+=1 ;
						//Cell_Real_pt[1]+=0.5 ;
						//dTextWidth-=2 ;
						if (!(this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt))) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						} 
						//mergeCell_Real_pt[0]+=1 ;
						//mergeCell_Real_pt[1]+=0.5 ;
						dTextWidth-=1 ;
						this->AddMText(Cell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.23, dTextWidth, iTxtColorIndex) ;
						//纪录宽度和 ;
						dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;					
					}
				}
				//处理完当前行后，得到的高度和 ;
				dHeightSumTmp += dArrayHeights.GetAt(iRowIndex-iStartRow) ;
			}
			//end 画文字
		}
		for(int iRowIndex=iStartRow; iRowIndex<=iEndRow; iRowIndex++)
		{

			double dWidthSumTmp = 0 ;
			//逐列处理
			for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
			{
				CString strCellName ; 
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数
					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列		
					//只在起始行列时处理一次合并格
					if (iRowIndex==iMergeCellStartRow && iColIndex==iMergeCellStartCol)
					{

						//合并格的宽度和高度;
						double dCellsWidth = 0 ;
						double dCellsHeight = 0 ;
						for(int iMergeCellCol=0; iMergeCellCol<iMergeCellsColNum; iMergeCellCol++)
						{
							dCellsWidth = dCellsWidth + dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-iStartColNum) ;
							//CString strtt ;
							//strtt.Format(_T("%s-%f-%d-%f"),strCellName,dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-(iStartCol-64)),iMergeCellStartCol+iMergeCellCol-(iStartCol-64),dArrayWidths.GetAt(0)) ;
							//AfxMessageBox(strtt) ;
						}
						for(int iMergeCellRow=0; iMergeCellRow<iMergeCellsRowNum; iMergeCellRow++)
						{
							if(iStartRow>=3)
								dCellsHeight = dCellsHeight + dArrayHeights.GetAt(iMergeCellStartRow+iMergeCellRow-iStartRow+2) ;
							else
								dCellsHeight = dCellsHeight + dArrayHeights.GetAt(iMergeCellStartRow+iMergeCellRow-iStartRow) ;
							//int iTmp = iMergeCellStartRow + iMergeCellRow - iStartRow ;
							//double dTmp = dArrayHeights.GetAt(iTmp) ;
						}
						//取得文本 ;
						_variant_t range_var = pRange->GetText() ;	
						strCellText = (CString ) range_var.bstrVal ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;

						strCellText.Replace(_T("{"),_T("\\{")) ;
						strCellText.Replace(_T("}"),_T("\\}")) ;

						//取得文本颜色,大小  ;
						MSExcel::FontPtr font = pRange->Font ;
						_variant_t colorIndex = font->ColorIndex ;
						_variant_t fontSize = font->GetSize() ;
						int iTxtColorIndex =ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
						int iFontSize = (int)fontSize.dblVal ;

						//取得文字对齐方式 ;
						_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
						_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
						//映射到cad中的对齐方式 ;
						AcDbMText::AttachmentPoint iAttachment ;
						if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						}


						//////////////////////////////////////////////////////////////////////////

						//绘制合并格内容 ;

						//合并格左下角坐标 ;
						double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
						double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellsHeight)*dHeightRate ;

						ads_point mergeCell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
						ads_point mergeCell_Real_pt = {0, 0, 0} ;

						//合并格画到dwg中的宽度和高度 ;
						double dTextWidth = dCellsWidth * dWidthRate ;
						double dTextHeight = dCellsHeight * dHeightRate ;						

						if (!(this->GetMTextBasePt(iAttachment, mergeCell_Base_pt, dTextWidth, dTextHeight, mergeCell_Real_pt))) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						} 
						//mergeCell_Real_pt[0]+=1 ;
						//mergeCell_Real_pt[1]+=0.5 ;
						dTextWidth = dTextWidth - 1 ;
						//this->AddMText()
						this->AddMText(mergeCell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.25, dTextWidth, iTxtColorIndex) ;

						//纪录宽度和 ;
						dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

					}
					//不是起始行列则继续下一列
					else
					{
						dWidthSumTmp+=dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						continue ;
					}
				}
				//不是合并格
				else
				{
					//取得文本 ;
					_variant_t range_var = pRange->GetText() ;	
					strCellText = (CString ) range_var.bstrVal ;
					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;
					strCellText.Replace(_T("{"),_T("\\{")) ;
					strCellText.Replace(_T("}"),_T("\\}")) ;
					//取得文本颜色,大小  ;
					MSExcel::FontPtr font = pRange->Font ;
					_variant_t colorIndex = font->ColorIndex ;
					_variant_t fontSize = font->GetSize() ;
					int iTxtColorIndex =this->ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
					int iFontSize = (int)fontSize.dblVal ;
					//取得文字对齐方式 ;
					_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
					_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
					//映射到cad中的对齐方式 ;
					AcDbMText::AttachmentPoint iAttachment ;
					if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
					{
						iAttachment = AcDbMText::kBottomLeft ;
					}
					//单元格宽度和高度 ;
					double dCellWidth = dArrayWidths.GetAt(iColIndex-iStartColNum) ;
					double dCellHeight=0;
					if(iStartRow>3)
					  dCellHeight = dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
					else
					{
						dCellHeight = dArrayHeights.GetAt(iRowIndex-iStartRow) ;
					}
					//////////////////////////////////////////////////////////////////////////
					//绘制单元格内容 ;
					//单元格左下角坐标 ;
					double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
					double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellHeight)*dHeightRate ;
					ads_point Cell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
					ads_point Cell_Real_pt = {0, 0, 0} ;
					//单元格画到dwg中的宽度和高度 ;
					double dTextWidth = dCellWidth * dWidthRate ;
					double dTextHeight = dCellHeight * dHeightRate ;						
					//this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt) ;
					//Cell_Real_pt[0]+=1 ;
					//Cell_Real_pt[1]+=0.5 ;
					//dTextWidth-=2 ;
					if (!(this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt))) 
					{
						iAttachment = AcDbMText::kBottomLeft ;
					} 
					//mergeCell_Real_pt[0]+=1 ;
					//mergeCell_Real_pt[1]+=0.5 ;
					dTextWidth-=1 ;

					this->AddMText(Cell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.23, dTextWidth, iTxtColorIndex) ;
					//纪录宽度和 ;
					dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;					
				}
			}
			//处理完当前行后，得到的高度和 ;
			if(iStartRow>3)
			dHeightSumTmp += dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
			else
			dHeightSumTmp += dArrayHeights.GetAt(iRowIndex-iStartRow) ;
		}
		//end 画文字
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//begin画行线
		 dHeightSumTmp_2 = 0 ;
		if(iStartRow>3)
		{
			for(int iRowIndex2=1; iRowIndex2<=2; iRowIndex2++)
			{
				double dWidthSumTmp = 0 ;
				//CArray<AcGePoint3dArray *, AcGePoint3dArray *> carryRowpoint ;
				CArray<double , double > carrayRowStartPoint ;
				CArray<double , double > carrayRowEndPoint ;
				double dStartP = 0 ;
				double dEndP = 0 ;
				//得到总行高
				dHeightSumTmp_2 += dArrayHeights.GetAt(iRowIndex2-1) ;	
				//逐列处理
				for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
				{
					CString strCellName ; 
					CString strCellText ;
					CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
					strCellName.Format(_T("%s%d"),strColName, iRowIndex2) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					//_variant_t bIsMergeCells = pRange->MergeCells ;	
					//_variant_t bIsMergeCells = pRange->GetMergeCells();
					_variant_t bIsMergeCells = pRange->MergeCells ;	
					//是合并格
					if(bIsMergeCells.boolVal == -1)
					{
						//处理合并格
						RangePtr pRangeTmp ;
						pRangeTmp = pRange->GetMergeArea() ;					 
						long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
						long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


						long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
						long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

						//CString strtt;
						//strtt.Format(_T("strCellName-%s\niMergeCellsRowNum-%d\niMergeCellsColNum-%d\niMergeCellStartRow-%d\niMergeCellStartCol%d"),strCellName,iMergeCellsRowNum,iMergeCellsColNum,iMergeCellStartRow,iMergeCellStartCol) ;
						//AfxMessageBox(strtt) ;

						//是合并格的最下面的一行,处理方式和非合并格一样
						if (iRowIndex2 == (iMergeCellStartRow+iMergeCellsRowNum-1)) 
						{
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
							//是起始列
							if (iColIndex ==iStartColNum )
							{
								carrayRowStartPoint.Add(dStartP) ;
							}
							else if (iColIndex == iEndColNum)
							{
								carrayRowEndPoint.Add(dEndP) ;
							}
						}
						else
						{			
							//合并格起始列时处理
							if (iColIndex==iMergeCellStartCol)
							{
								//处理格第一列
								if (iColIndex == iStartColNum)
								{
									carrayRowStartPoint.Add(dStartP) ;
								}
								carrayRowEndPoint.Add(dEndP) ;
								//dStartP = dEndP ;
								dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;

								//合并格只有一列，起始列就是最后一列
								if (iMergeCellsColNum == 1)
								{
									dStartP = dEndP ;
									carrayRowStartPoint.Add(dStartP) ;
									//是处理格的最后一列
									if (iColIndex == iEndColNum)
									{
										carrayRowEndPoint.Add(dEndP) ;
									}							

								}

							}
							//合并格最后一列处理
							else if(iColIndex == iMergeCellStartCol+iMergeCellsColNum-1)
							{
								dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
								dStartP = dEndP ;
								carrayRowStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iColIndex == iEndColNum)
								{
									carrayRowEndPoint.Add(dEndP) ;
								}
							}
							//合并格中间的列处理
							else
							{
								dEndP =  dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
							}
						}

					}
					//不是合并格
					else
					{
						dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						//是起始列
						if (iColIndex ==iStartColNum )
						{
							carrayRowStartPoint.Add(dStartP) ;
						}
						else if (iColIndex == iEndColNum)
						{
							carrayRowEndPoint.Add(dEndP) ;
						}

					}
				}

				//////////////////////////////////////////////////////////////////////////
				//
				int iStartpNum = carrayRowStartPoint.GetSize() ;
				int iEndpNum = carrayRowEndPoint.GetSize() ;
				if (iStartpNum == iEndpNum)
				{
					//当前线条的y坐标
					double dRowBaseY = base_pt[1] - (dHeightSumTmp_2)*dHeightRate ;
					for(int iRowLineNum=0; iRowLineNum<carrayRowStartPoint.GetSize(); iRowLineNum++)
					{
						double dRowBaseStartX = base_pt[0] + (carrayRowStartPoint.GetAt(iRowLineNum))*dWidthRate ;
						double dRowBaseEndX = base_pt[0] + (carrayRowEndPoint.GetAt(iRowLineNum))*dWidthRate ;

						AcGePoint3d start_3d(dRowBaseStartX, dRowBaseY, 0) ;
						AcGePoint3d end_3d(dRowBaseEndX, dRowBaseY, 0) ;
						this->AddAcDbLine(start_3d, end_3d) ;
					}
				}
				else
				{
					AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
				}
			}
		}
		
		for(int iRowIndex2=iStartRow; iRowIndex2<=iEndRow; iRowIndex2++)
		{
			double dWidthSumTmp = 0 ;
			//CArray<AcGePoint3dArray *, AcGePoint3dArray *> carryRowpoint ;
			CArray<double , double > carrayRowStartPoint ;
			CArray<double , double > carrayRowEndPoint ;
			double dStartP = 0 ;
			double dEndP = 0 ;
			//得到总行高
			if(iStartRow>=3)
			{
				dHeightSumTmp_2 += dArrayHeights.GetAt(iRowIndex2-iStartRow+2) ;	
			}
			else
			{
				dHeightSumTmp_2 += dArrayHeights.GetAt(iRowIndex2-iStartRow) ;	
			}
			
			//逐列处理
			for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
			{
				CString strCellName ; 
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex2) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				//_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				_variant_t bIsMergeCells = pRange->MergeCells ;	

				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

					//CString strtt;
					//strtt.Format(_T("strCellName-%s\niMergeCellsRowNum-%d\niMergeCellsColNum-%d\niMergeCellStartRow-%d\niMergeCellStartCol%d"),strCellName,iMergeCellsRowNum,iMergeCellsColNum,iMergeCellStartRow,iMergeCellStartCol) ;
					//AfxMessageBox(strtt) ;

					//是合并格的最下面的一行,处理方式和非合并格一样
					if (iRowIndex2 == (iMergeCellStartRow+iMergeCellsRowNum-1)) 
					{
						dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						//是起始列
						if (iColIndex ==iStartColNum )
						{
							carrayRowStartPoint.Add(dStartP) ;
						}
						else if (iColIndex == iEndColNum)
						{
							carrayRowEndPoint.Add(dEndP) ;
						}
					}
					else
					{			
						//合并格起始列时处理
						if (iColIndex==iMergeCellStartCol)
						{
							//处理格第一列
							if (iColIndex == iStartColNum)
							{
								carrayRowStartPoint.Add(dStartP) ;
							}
							carrayRowEndPoint.Add(dEndP) ;
							//dStartP = dEndP ;
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;

							//合并格只有一列，起始列就是最后一列
							if (iMergeCellsColNum == 1)
							{
								dStartP = dEndP ;
								carrayRowStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iColIndex == iEndColNum)
								{
									carrayRowEndPoint.Add(dEndP) ;
								}							

							}

						}
						//合并格最后一列处理
						else if(iColIndex == iMergeCellStartCol+iMergeCellsColNum-1)
						{
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
							dStartP = dEndP ;
							carrayRowStartPoint.Add(dStartP) ;
							//是处理格的最后一列
							if (iColIndex == iEndColNum)
							{
								carrayRowEndPoint.Add(dEndP) ;
							}
						}
						//合并格中间的列处理
						else
						{
							dEndP =  dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						}
					}

				}
				//不是合并格
				else
				{
					dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
					//是起始列
					if (iColIndex ==iStartColNum )
					{
						carrayRowStartPoint.Add(dStartP) ;
					}
					else if (iColIndex == iEndColNum)
					{
						carrayRowEndPoint.Add(dEndP) ;
					}

				}
			}

			//////////////////////////////////////////////////////////////////////////
			//
			int iStartpNum = carrayRowStartPoint.GetSize() ;
			int iEndpNum = carrayRowEndPoint.GetSize() ;
			if (iStartpNum == iEndpNum)
			{
				//当前线条的y坐标
				double dRowBaseY = base_pt[1] - (dHeightSumTmp_2)*dHeightRate ;
				for(int iRowLineNum=0; iRowLineNum<carrayRowStartPoint.GetSize(); iRowLineNum++)
				{
					double dRowBaseStartX = base_pt[0] + (carrayRowStartPoint.GetAt(iRowLineNum))*dWidthRate ;
					double dRowBaseEndX = base_pt[0] + (carrayRowEndPoint.GetAt(iRowLineNum))*dWidthRate ;

					AcGePoint3d start_3d(dRowBaseStartX, dRowBaseY, 0) ;
					AcGePoint3d end_3d(dRowBaseEndX, dRowBaseY, 0) ;
					this->AddAcDbLine(start_3d, end_3d) ;
				}
			}
			else
			{
				AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
			}

		}


		//end画行线
		//////////////////////////////////////////////////////////////////////////	

		//////////////////////////////////////////////////////////////////////////
		//begin画列线
		double dWidthSumTmp = 0 ;
		for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
		{
			//int iColIndexTmp = (int)iColIndex-64 ;				

			//double dHeightSumTmp = 0 
			dHeightSumTmp=0;
			CArray<double , double > carrayColStartPoint ;
			CArray<double , double > carrayColEndPoint ;
			double dStartP = 0 ;
			double dEndP = 0 ;

			//得到总列宽
			dWidthSumTmp += dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

			//列名称
			CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
            if(iStartRow>3)
			{
				for(int iRowIndex=1; iRowIndex<=2;iRowIndex++)
				{
					CString strCellName ; 
					CString strCellText ;
					strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					//_variant_t bIsMergeCells = pRange->MergeCells ;	
					//_variant_t bIsMergeCells = pRange->GetMergeCells();
					_variant_t bIsMergeCells = pRange->MergeCells ;	
					//是合并格
					if(bIsMergeCells.boolVal == -1)
					{
						//处理合并格
						RangePtr pRangeTmp ;
						pRangeTmp = pRange->GetMergeArea() ;					 
						long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
						long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数
						long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
						long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

						//是合并格的最右边的一列,处理方式和非合并格一样
						if (iColIndex == (iMergeCellStartCol+iMergeCellsColNum-1)) 
						{
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-1) ;
							//是起始行
							if (iRowIndex ==iStartRow )
							{
								carrayColStartPoint.Add(dStartP) ;
							}
							else if (iRowIndex == iEndRow)
							{
								carrayColEndPoint.Add(dEndP) ;
							}
						}
						//不是合并格的最右边的一列					
						else
						{					
							//合并格起始行时处理
							if (iRowIndex==iMergeCellStartRow )
							{
								//处理格第一行
								if (iRowIndex == iStartRow)
								{
									carrayColStartPoint.Add(dStartP) ;
								}

								carrayColEndPoint.Add(dEndP) ;
								//dStartP = dEndP ;
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-1) ;

								//合并格只有一行，起始行即是最后一行
								if (iMergeCellsRowNum == 1)
								{
									//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
									dStartP = dEndP ;
									carrayColStartPoint.Add(dStartP) ;
									//是处理格的最后一列
									if (iRowIndex == iEndRow)
									{
										carrayColEndPoint.Add(dEndP) ;
									}

								}

							}
							//合并格最后一行处理
							else if(iRowIndex == iMergeCellStartRow+iMergeCellsRowNum-1)
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-1) ;
								dStartP = dEndP ;
								carrayColStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iRowIndex == iEndRow)
								{
									carrayColEndPoint.Add(dEndP) ;
								}
							}
							//合并格中间的行处理
							else
							{
								dEndP =  dEndP + dArrayHeights.GetAt(iRowIndex-1) ;
							}
						}


					}

					//不是合并格
					else
					{
						dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-1) ;
						//是起始行
						if (iRowIndex ==iStartRow)
						{
							carrayColStartPoint.Add(dStartP) ;
						}
						else if (iRowIndex ==iEndRow)
						{
							carrayColEndPoint.Add(dEndP) ;
						}

					}				

				}

			}
			//逐行处理
			for(int iRowIndex=iStartRow; iRowIndex<=iEndRow;iRowIndex++)
			{
				CString strCellName ; 
				CString strCellText ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				//_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				_variant_t bIsMergeCells = pRange->MergeCells ;	
				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数
					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

					//是合并格的最右边的一列,处理方式和非合并格一样
					if (iColIndex == (iMergeCellStartCol+iMergeCellsColNum-1)) 
					{
						if(iStartRow>=3)
						{
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
						}
						else
						{
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
						}

						//是起始行
						if (iRowIndex ==iStartRow )
						{
							carrayColStartPoint.Add(dStartP) ;
						}
						else if (iRowIndex == iEndRow)
						{
							carrayColEndPoint.Add(dEndP) ;
						}
					}
					//不是合并格的最右边的一列					
					else
					{					
						//合并格起始行时处理
						if (iRowIndex==iMergeCellStartRow )
						{
							//处理格第一行
							if (iRowIndex == iStartRow)
							{
								carrayColStartPoint.Add(dStartP) ;
							}

							carrayColEndPoint.Add(dEndP) ;
							//dStartP = dEndP ;
							//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							if(iStartRow>=3)
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
							}
							else
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							}

							//合并格只有一行，起始行即是最后一行
							if (iMergeCellsRowNum == 1)
							{
								//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
								dStartP = dEndP ;
								carrayColStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iRowIndex == iEndRow)
								{
									carrayColEndPoint.Add(dEndP) ;
								}

							}

						}
						//合并格最后一行处理
						else if(iRowIndex == iMergeCellStartRow+iMergeCellsRowNum-1)
						{
							//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							if(iStartRow>=3)
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
							}
							else
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							}
							dStartP = dEndP ;
							carrayColStartPoint.Add(dStartP) ;
							//是处理格的最后一列
							if (iRowIndex == iEndRow)
							{
								carrayColEndPoint.Add(dEndP) ;
							}
						}
						//合并格中间的行处理
						else
						{
							//dEndP =  dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							if(iStartRow>=3)
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
							}
							else
							{
								dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							}
						}
					}


				}

				//不是合并格
				else
				{
					//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
					if(iStartRow>=3)
					{
						dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow+2) ;
					}
					else
					{
						dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
					}
					//是起始行
					if (iRowIndex ==iStartRow )
					{
						carrayColStartPoint.Add(dStartP) ;
					}
					else if (iRowIndex == iEndRow)
					{
						carrayColEndPoint.Add(dEndP) ;
					}

				}				

			}

			//////////////////////////////////////////////////////////////////////////
			//画线条
			int iStartpNum = carrayColStartPoint.GetSize() ;
			int iEndpNum = carrayColEndPoint.GetSize() ;
			if (iStartpNum == iEndpNum)
			{
				//当前线条的x坐标
				double dRowBaseX = base_pt[0] + (dWidthSumTmp)*dWidthRate ;
				for(int iColLineNum=0; iColLineNum<carrayColStartPoint.GetSize(); iColLineNum++)
				{
					double dColBaseStartY = base_pt[1] - (carrayColStartPoint.GetAt(iColLineNum))*dHeightRate ;
					double dColBaseEndY= base_pt[1] - (carrayColEndPoint.GetAt(iColLineNum))*dHeightRate ;
					AcGePoint3d start_3d(dRowBaseX, dColBaseStartY, 0) ;
					AcGePoint3d end_3d(dRowBaseX, dColBaseEndY, 0) ;
					this->AddAcDbLine(start_3d, end_3d) ;
				}
			}
			else
			{
				AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
			}


		}
		//返回列宽
		d_width=dWidthSumTmp*dWidthRate;
		//end画列线
		//////////////////////////////////////////////////////////////////////////

		//画左边线
		double dLeftBottomPointX = base_pt[0] ;
		double dLeftBottomPointY = base_pt[1] - dHeightSumTmp_2 * dHeightRate ;
		AcGePoint3d lefttop_p(0, 0, 0) ;
		AcGePoint3d leftbottom_p(dLeftBottomPointX, dLeftBottomPointY, 0) ;
		this->ConvertPtAdsTo3D(base_pt, lefttop_p) ;
		this->AddAcDbLine(lefttop_p, leftbottom_p) ;

		//画上边线
		double dRightTopPointX = base_pt[0] + dWidthSumTmp * dWidthRate ;
		double dRightTopPointY = base_pt[1] ;
		AcGePoint3d righttop_p(dRightTopPointX, dRightTopPointY, 0) ;
		this->AddAcDbLine(lefttop_p, righttop_p) ;

		///关闭打开的各个资源
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		//m_pExcel->Release() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		bReturn = FALSE ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		bReturn = FALSE ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return bReturn ;
}
BOOL CUserSystem::LSBExcelToDwg2(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt ,double &d_width,double &d_row_hight,double dHeightRate, double dWidthRate)
{
	BOOL bReturn = TRUE ;

	int iStartColNum = CBlkUtility::GetNumFromExcelColName(iStartCol) ;
	int iEndColNum = CBlkUtility::GetNumFromExcelColName(iEndCol) ;
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	//WorksheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	SheetsPtr pWorksheets = NULL ;
	AcDbObjectId textStyleId = this->CreateTextStyle(_T("zhffont"), _T("txt"), _T(""), 2.5, 0.73) ;	
	CArray<double, double> dArrayWidths ;
	CArray<double, double> dArrayHeights ;
	try
	{
			if (bFirst) 
		{
			::AfxOleInit ( ) ;
			bFirst = FALSE ;
		}		
		HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return FALSE ;
		}
		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBook = pWorkBooks->Open( (_bstr_t)strExcelFileName ) ;
		_variant_t vt_index ;
		//VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;
		pWorksheets  = m_pExcel->GetSheets() ; //m_pExcel->GetSheets( ) ;
		//获得sheet1的指针
		pSheet = pWorksheets->GetItem( vt_index ) ;

		//获得各列的宽度 ;
		for(int ch=iStartColNum; ch<=iEndColNum; ch++)
		{
			CString strCellName ;
			CString strColName = CBlkUtility::GetExcelFieldName(ch) ;
			strCellName.Format(_T("%s1"), strColName) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_width ;
			range_width = pRange->GetColumnWidth() ;
			dArrayWidths.Add((double)range_width) ;	
		}
		//获取表头的行高
		for(int iLineIndex=1; iLineIndex<=2; iLineIndex++)
		{
			CString strCellName ;
			strCellName.Format(_T("A%d"), iLineIndex) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_height ;
			range_height = pRange->GetRowHeight() ;
			//dArrayHeights.Add((double)range_height) ;
			d_row_hight+=(double)(range_height);
		}
		//在CAD中显示的行高
		d_row_hight=d_row_hight*dHeightRate;
		//获得各行的高度 ;
		for(int iLineIndex=iStartRow; iLineIndex<=iEndRow; iLineIndex++)
		{
			CString strCellName ;
			strCellName.Format(_T("A%d"), iLineIndex) ;
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCellName,vtMissing) ;
			_variant_t range_height ;
			range_height = pRange->GetRowHeight() ;
			dArrayHeights.Add((double)range_height) ;
		}
		for(int i=0;i<dArrayHeights.GetCount();i++)
		{
			acutPrintf(_T("%d行,高%6f"),i,dArrayHeights.GetAt(i));
		}
		//CString straa ;
		//straa.Format(_T("%d-%d"),dArrayWidths.GetSize(), dArrayHeights.GetSize()) ;
		//AfxMessageBox(straa) ;
		//////////////////////////////////////////////////////////////////////////
		//begin画文字		
		double dHeightSumTmp = 0 ;
		for(int iRowIndex=iStartRow; iRowIndex<=iEndRow; iRowIndex++)
		{

			double dWidthSumTmp = 0 ;

			//逐列处理
			for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
			{
				CString strCellName ; 
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数
					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列	
				    //重新获取起始行和起始列的text
					CString strColName = CBlkUtility::GetExcelFieldName(iMergeCellStartCol) ;
					strCellName.Format(_T("%s%d"),strColName, iMergeCellStartRow) ;
					pRange = NULL ;
					pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
					//只在起始行列时处理一次合并格
					 if (iRowIndex==iMergeCellStartRow && iColIndex==iMergeCellStartCol)
					{

						//合并格的宽度和高度;
						double dCellsWidth = 0 ;
						double dCellsHeight = 0 ;
						for(int iMergeCellCol=0; iMergeCellCol<iMergeCellsColNum; iMergeCellCol++)
						{
							dCellsWidth = dCellsWidth + dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-iStartColNum) ;
						}
						for(int iMergeCellRow=0; iMergeCellRow<iMergeCellsRowNum; iMergeCellRow++)
						{
						    //注意加上条件
							if(iMergeCellRow+iMergeCellStartRow<=iEndRow)
							{
								dCellsHeight = dCellsHeight + dArrayHeights.GetAt(iMergeCellStartRow+iMergeCellRow-iStartRow) ;
							}
						}
						//取得文本 ;
						_variant_t range_var = pRange->GetText() ;	
						strCellText = (CString ) range_var.bstrVal ;
						if(iRowIndex==3&&iColIndex==1)
						{
							acutPrintf(_T("\n单元格：%s"),strCellText);
						}
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						strCellText.Replace(_T("{"),_T("\\{")) ;
						strCellText.Replace(_T("}"),_T("\\}")) ;
						//这里还可以做合并内容以及去掉重复等等工作
						//strCellText.Replace(_T("%%16"),_T("")) ;
						strCellText.Replace(_T("%%161"),_T("I")) ;
						strCellText.Replace(_T("%%162"),_T("II")) ;
						strCellText.Replace(_T("%%163"),_T("III")) ;
						strCellText.Replace(_T("%%164"),_T("IV")) ;
						strCellText.Replace(_T("%%165"),_T("V")) ;
						strCellText.Replace(_T("%%166"),_T("VI")) ;
						strCellText.Replace(_T("%%167"),_T("VII")) ;
						strCellText.Replace(_T("%%168"),_T("VIII")) ;
						strCellText.Replace(_T("%%169"),_T("IX")) ;
						strCellText.Replace(_T("%%170"),_T("X")) ;

						//取得文本颜色,大小  ;
						MSExcel::FontPtr font = pRange->Font ;
						_variant_t colorIndex = font->ColorIndex ;
						_variant_t fontSize = font->GetSize() ;
						int iTxtColorIndex =ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
						int iFontSize = (int)fontSize.dblVal ;

						//取得文字对齐方式 ;
						_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
						_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
						//映射到cad中的对齐方式 ;
						AcDbMText::AttachmentPoint iAttachment ;
						if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						}
						//////////////////////////////////////////////////////////////////////////
						//绘制合并格内容 ;
						//合并格左下角坐标 ;
						double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
						double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellsHeight)*dHeightRate ;
						ads_point mergeCell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
						ads_point mergeCell_Real_pt = {0, 0, 0} ;

						//合并格画到dwg中的宽度和高度 ;
						double dTextWidth = dCellsWidth * dWidthRate ;
						double dTextHeight = dCellsHeight * dHeightRate ;						
						if (!(this->GetMTextBasePt(iAttachment, mergeCell_Base_pt, dTextWidth, dTextHeight, mergeCell_Real_pt))) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						} 
						//mergeCell_Real_pt[0]+=1 ;
						//mergeCell_Real_pt[1]+=0.5 ;
						dTextWidth = dTextWidth - 1 ;
						//this->AddMText()
						this->AddMText(mergeCell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.25, dTextWidth, iTxtColorIndex) ;
						//纪录宽度和 ;
						dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

					}
					//另外在第一行轮询的时候遇到部分合并格子，还需要写一次
					else if (iRowIndex==iStartRow&&iStartRow>3&& iColIndex==iMergeCellStartCol)
					{

						//合并格的宽度和高度;
						double dCellsWidth = 0 ;
						double dCellsHeight = 0 ;
						for(int iMergeCellCol=0; iMergeCellCol<iMergeCellsColNum; iMergeCellCol++)
						{
							dCellsWidth = dCellsWidth + dArrayWidths.GetAt(iMergeCellStartCol+iMergeCellCol-iStartColNum) ;
						}
						for(int iMergeCellRow=iStartRow; iMergeCellRow<iMergeCellsRowNum+iMergeCellStartRow; iMergeCellRow++)
						{
							//注意加上条件
							if(iMergeCellRow<=iEndRow)
							{
								dCellsHeight = dCellsHeight + dArrayHeights.GetAt(iMergeCellRow-iStartRow) ;
							}
						}
						//取得文本 ;
						_variant_t range_var = pRange->GetText() ;	
						strCellText = (CString ) range_var.bstrVal ;
						if(iRowIndex==3&&iColIndex==1)
						{
							acutPrintf(_T("\n单元格：%s"),strCellText);
						}
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						strCellText.Replace(_T("{"),_T("\\{")) ;
						strCellText.Replace(_T("}"),_T("\\}")) ;
						//这里还可以做合并内容以及去掉重复等等工作
						strCellText.Replace(_T("%%161"),_T("I")) ;
						strCellText.Replace(_T("%%162"),_T("II")) ;
						strCellText.Replace(_T("%%163"),_T("III")) ;
						strCellText.Replace(_T("%%164"),_T("IV")) ;
						strCellText.Replace(_T("%%165"),_T("V")) ;
						strCellText.Replace(_T("%%166"),_T("VI")) ;
						strCellText.Replace(_T("%%167"),_T("VII")) ;
						strCellText.Replace(_T("%%168"),_T("VIII")) ;
						strCellText.Replace(_T("%%169"),_T("IX")) ;
						strCellText.Replace(_T("%%170"),_T("X")) ;
// 						strCellText.Replace(_T("%%161"),_T("X")) ;
// 						strCellText.Replace(_T("%%161"),_T("XI")) ;;

						//取得文本颜色,大小  ;
						MSExcel::FontPtr font = pRange->Font ;
						_variant_t colorIndex = font->ColorIndex ;
						_variant_t fontSize = font->GetSize() ;
						int iTxtColorIndex =ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
						int iFontSize = (int)fontSize.dblVal ;

						//取得文字对齐方式 ;
						_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
						_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
						//映射到cad中的对齐方式 ;
						AcDbMText::AttachmentPoint iAttachment ;
						if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						}
						//////////////////////////////////////////////////////////////////////////
						//绘制合并格内容 ;
						//合并格左下角坐标 ;
						double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
						double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellsHeight)*dHeightRate ;
						ads_point mergeCell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
						ads_point mergeCell_Real_pt = {0, 0, 0} ;

						//合并格画到dwg中的宽度和高度 ;
						double dTextWidth = dCellsWidth * dWidthRate ;
						double dTextHeight = dCellsHeight * dHeightRate ;						
						if (!(this->GetMTextBasePt(iAttachment, mergeCell_Base_pt, dTextWidth, dTextHeight, mergeCell_Real_pt))) 
						{
							iAttachment = AcDbMText::kBottomLeft ;
						} 
						//mergeCell_Real_pt[0]+=1 ;
						//mergeCell_Real_pt[1]+=0.5 ;
						dTextWidth = dTextWidth - 1 ;
						//this->AddMText()
						this->AddMText(mergeCell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.25, dTextWidth, iTxtColorIndex) ;
						//纪录宽度和 ;
						dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

					}
					//不是起始行列则继续下一列
					else
					{
						dWidthSumTmp+=dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						continue ;
					}
				}
				//不是合并格
				else
				{
					//取得文本 ;
					_variant_t range_var = pRange->GetText() ;	
					strCellText = (CString ) range_var.bstrVal ;

					strCellText.TrimLeft() ;
					strCellText.TrimRight() ;


					strCellText.Replace(_T("{"),_T("\\{")) ;
					strCellText.Replace(_T("}"),_T("\\}")) ;
					//strCellText.Replace(_T("%%16"),_T("")) ;
					strCellText.Replace(_T("%%161"),_T("I")) ;
					strCellText.Replace(_T("%%162"),_T("II")) ;
					strCellText.Replace(_T("%%163"),_T("III")) ;
					strCellText.Replace(_T("%%164"),_T("IV")) ;
					strCellText.Replace(_T("%%165"),_T("V")) ;
					strCellText.Replace(_T("%%166"),_T("VI")) ;
					strCellText.Replace(_T("%%167"),_T("VII")) ;
					strCellText.Replace(_T("%%168"),_T("VIII")) ;
					strCellText.Replace(_T("%%169"),_T("IX")) ;
					strCellText.Replace(_T("%%170"),_T("X")) ;

					//取得文本颜色,大小  ;
					MSExcel::FontPtr font = pRange->Font ;
					_variant_t colorIndex = font->ColorIndex ;
					_variant_t fontSize = font->GetSize() ;
					int iTxtColorIndex =this->ConvExcelCorlorToDwg( (int)colorIndex.iVal )  ;
					int iFontSize = (int)fontSize.dblVal ;

					//取得文字对齐方式 ;
					_variant_t lHorAlign = pRange->GetHorizontalAlignment() ;
					_variant_t lVerAlign = pRange->GetVerticalAlignment() ;
					//映射到cad中的对齐方式 ;
					AcDbMText::AttachmentPoint iAttachment ;
					if (!this->ChangeExcelAlignToMText((long)lVerAlign,(long)lHorAlign,iAttachment)) 
					{
						iAttachment = AcDbMText::kBottomLeft ;
					}
					//CString strtt ;
					//strtt.Format(_T("%d"),(int)iAttachment) ;
					//AfxMessageBox(strtt) ;


					//单元格宽度和高度 ;
					double dCellWidth = dArrayWidths.GetAt(iColIndex-iStartColNum) ;
					double dCellHeight = dArrayHeights.GetAt(iRowIndex-iStartRow) ;

					//////////////////////////////////////////////////////////////////////////

					//绘制单元格内容 ;
					//单元格左下角坐标 ;
					double dCellBaseX = base_pt[0] + dWidthSumTmp*dWidthRate ;
					double dCellBaseY = base_pt[1] -(dHeightSumTmp+dCellHeight)*dHeightRate ;

					ads_point Cell_Base_pt = {dCellBaseX, dCellBaseY, 0} ;
					ads_point Cell_Real_pt = {0, 0, 0} ;

					//单元格画到dwg中的宽度和高度 ;
					double dTextWidth = dCellWidth * dWidthRate ;
					double dTextHeight = dCellHeight * dHeightRate ;						

					//this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt) ;
					//Cell_Real_pt[0]+=1 ;
					//Cell_Real_pt[1]+=0.5 ;
					//dTextWidth-=2 ;
					if (!(this->GetMTextBasePt(iAttachment, Cell_Base_pt, dTextWidth, dTextHeight, Cell_Real_pt))) 
					{
						iAttachment = AcDbMText::kBottomLeft ;
					} 
					//mergeCell_Real_pt[0]+=1 ;
					//mergeCell_Real_pt[1]+=0.5 ;
					dTextWidth-=1 ;

					this->AddMText(Cell_Real_pt, strCellText, textStyleId, iAttachment, iFontSize*0.23, dTextWidth, iTxtColorIndex) ;

					//纪录宽度和 ;
					dWidthSumTmp +=dArrayWidths.GetAt(iColIndex-iStartColNum) ;					


				}
			}

			//处理完当前行后，得到的高度和 ;
			dHeightSumTmp += dArrayHeights.GetAt(iRowIndex-iStartRow) ;
			d_width=dWidthSumTmp*dWidthRate;

		}
		//end 画文字
		//////////////////////////////////////////////////////////////////////////
     


		//////////////////////////////////////////////////////////////////////////
		//begin画行线
		double dHeightSumTmp_2 = 0 ;
		for(int iRowIndex2=iStartRow; iRowIndex2<=iEndRow; iRowIndex2++)
		{
			double dWidthSumTmp = 0 ;
			//CArray<AcGePoint3dArray *, AcGePoint3dArray *> carryRowpoint ;
			CArray<double , double > carrayRowStartPoint ;
			CArray<double , double > carrayRowEndPoint ;
			double dStartP = 0 ;
			double dEndP = 0 ;

			//得到总行高
			dHeightSumTmp_2 += dArrayHeights.GetAt(iRowIndex2-iStartRow) ;		
			//逐列处理
			for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
			{

				//int iColIndexTmp = (int)iColIndex-64 ;				

				CString strCellName ; 
				CString strCellText ;
				CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex2) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				//_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				_variant_t bIsMergeCells = pRange->MergeCells ;	

				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

					//CString strtt;
					//strtt.Format(_T("strCellName-%s\niMergeCellsRowNum-%d\niMergeCellsColNum-%d\niMergeCellStartRow-%d\niMergeCellStartCol%d"),strCellName,iMergeCellsRowNum,iMergeCellsColNum,iMergeCellStartRow,iMergeCellStartCol) ;
					//AfxMessageBox(strtt) ;

					//是合并格的最下面的一行,处理方式和非合并格一样
					if (iRowIndex2 == (iMergeCellStartRow+iMergeCellsRowNum-1)||(iRowIndex2==iEndRow&&iMergeCellStartRow+iMergeCellsRowNum-1>=iEndRow)) 
					{
						dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						//是起始列
						if (iColIndex ==iStartColNum )
						{
							carrayRowStartPoint.Add(dStartP) ;
						}
						else if (iColIndex == iEndColNum)
						{
							carrayRowEndPoint.Add(dEndP) ;
						}
					}
					else
					{			
						//合并格起始列时处理
						if (iColIndex==iMergeCellStartCol)
						{
							//处理格第一列
							if (iColIndex == iStartColNum)
							{
								carrayRowStartPoint.Add(dStartP) ;
							}
							carrayRowEndPoint.Add(dEndP) ;
							//dStartP = dEndP ;
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;

							//合并格只有一列，起始列就是最后一列
							if (iMergeCellsColNum == 1)
							{
								dStartP = dEndP ;
								carrayRowStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iColIndex == iEndColNum)
								{
									carrayRowEndPoint.Add(dEndP) ;
								}							

							}

						}
						//合并格最后一列处理
						else if(iColIndex == iMergeCellStartCol+iMergeCellsColNum-1)
						{
							dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
							dStartP = dEndP ;
							carrayRowStartPoint.Add(dStartP) ;
							//是处理格的最后一列
							if (iColIndex == iEndColNum)
							{
								carrayRowEndPoint.Add(dEndP) ;
							}
						}
						//合并格中间的列处理
						else
						{
							dEndP =  dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
						}
					}

				}
				//不是合并格
				else
				{
					dEndP = dEndP + dArrayWidths.GetAt(iColIndex-iStartColNum) ;
					//是起始列
					if (iColIndex ==iStartColNum )
					{
						carrayRowStartPoint.Add(dStartP) ;
					}
					else if (iColIndex == iEndColNum)
					{
						carrayRowEndPoint.Add(dEndP) ;
					}

				}
			}

			//////////////////////////////////////////////////////////////////////////
			//
			int iStartpNum = carrayRowStartPoint.GetSize() ;
			int iEndpNum = carrayRowEndPoint.GetSize() ;
			if (iStartpNum == iEndpNum)
			{
				//当前线条的y坐标
				double dRowBaseY = base_pt[1] - (dHeightSumTmp_2)*dHeightRate ;
				for(int iRowLineNum=0; iRowLineNum<carrayRowStartPoint.GetSize(); iRowLineNum++)
				{
					double dRowBaseStartX = base_pt[0] + (carrayRowStartPoint.GetAt(iRowLineNum))*dWidthRate ;
					double dRowBaseEndX = base_pt[0] + (carrayRowEndPoint.GetAt(iRowLineNum))*dWidthRate ;

					AcGePoint3d start_3d(dRowBaseStartX, dRowBaseY, 0) ;
					AcGePoint3d end_3d(dRowBaseEndX, dRowBaseY, 0) ;
					this->AddAcDbLine(start_3d, end_3d) ;
				}
			}
			else
			{
				AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
			}

		}


		//end画行线
		//////////////////////////////////////////////////////////////////////////	

		//////////////////////////////////////////////////////////////////////////
		//begin画列线
		double dWidthSumTmp = 0 ;
		for(int iColIndex=iStartColNum; iColIndex<=iEndColNum; iColIndex++)
		{
			//int iColIndexTmp = (int)iColIndex-64 ;				

			double dHeightSumTmp = 0 ;
			CArray<double , double > carrayColStartPoint ;
			CArray<double , double > carrayColEndPoint ;
			double dStartP = 0 ;
			double dEndP = 0 ;

			//得到总列宽
			dWidthSumTmp += dArrayWidths.GetAt(iColIndex-iStartColNum) ;		

			//列名称
			CString strColName = CBlkUtility::GetExcelFieldName(iColIndex) ;

			//逐行处理
			for(int iRowIndex=iStartRow; iRowIndex<=iEndRow;iRowIndex++)
			{
				CString strCellName ; 
				CString strCellText ;
				strCellName.Format(_T("%s%d"),strColName, iRowIndex) ;
				pRange = NULL ;
				pRange = pSheet->GetRange((_variant_t)strCellName, vtMissing) ;
				//_variant_t bIsMergeCells = pRange->MergeCells ;	
				//_variant_t bIsMergeCells = pRange->GetMergeCells();
				_variant_t bIsMergeCells = pRange->MergeCells ;	

				//是合并格
				if(bIsMergeCells.boolVal == -1)
				{
					//处理合并格
					RangePtr pRangeTmp ;
					pRangeTmp = pRange->GetMergeArea() ;					 
					long iMergeCellsRowNum = pRangeTmp->GetRows()->GetCount();//合并格的行数					  
					long iMergeCellsColNum = pRangeTmp->GetColumns()->GetCount() ;//合并格的列数


					long iMergeCellStartRow = pRangeTmp->GetRow() ; //起始行
					long iMergeCellStartCol = pRangeTmp->GetColumn() ; //起始列

					//是合并格的最右边的一列,处理方式和非合并格一样
					if (iColIndex == (iMergeCellStartCol+iMergeCellsColNum-1)) 
					{
						dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
						//是起始行
						if (iRowIndex ==iStartRow )
						{
							carrayColStartPoint.Add(dStartP) ;
						}
						else if (iRowIndex == iEndRow)
						{
							carrayColEndPoint.Add(dEndP) ;
						}
					}
					//不是合并格的最右边的一列					
					else
					{					
						//合并格起始行时处理
						if (iRowIndex==iMergeCellStartRow )
						{
							//处理格第一行
							if (iRowIndex == iStartRow)
							{
								carrayColStartPoint.Add(dStartP) ;
							}

							carrayColEndPoint.Add(dEndP) ;
							//dStartP = dEndP ;
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;

							//合并格只有一行，起始行即是最后一行
							if (iMergeCellsRowNum == 1)
							{
								//dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
								dStartP = dEndP ;
								carrayColStartPoint.Add(dStartP) ;
								//是处理格的最后一列
								if (iRowIndex == iEndRow)
								{
									carrayColEndPoint.Add(dEndP) ;
								}

							}

						}
						//合并格最后一行处理
						else if(iRowIndex == iMergeCellStartRow+iMergeCellsRowNum-1)
						{
							dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
							dStartP = dEndP ;
							carrayColStartPoint.Add(dStartP) ;
							//是处理格的最后一列
							if (iRowIndex == iEndRow)
							{
								carrayColEndPoint.Add(dEndP) ;
							}
						}
						//合并格中间的行处理
						else
						{
							dEndP =  dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
						}
					}


				}

				//不是合并格
				else
				{
					dEndP = dEndP + dArrayHeights.GetAt(iRowIndex-iStartRow) ;
					//是起始行
					if (iRowIndex ==iStartRow )
					{
						carrayColStartPoint.Add(dStartP) ;
					}
					else if (iRowIndex == iEndRow)
					{
						carrayColEndPoint.Add(dEndP) ;
					}

				}				

			}

			//////////////////////////////////////////////////////////////////////////
			//画线条
			int iStartpNum = carrayColStartPoint.GetSize() ;
			int iEndpNum = carrayColEndPoint.GetSize() ;
			if (iStartpNum == iEndpNum)
			{
				//当前线条的x坐标
				double dRowBaseX = base_pt[0] + (dWidthSumTmp)*dWidthRate ;
				for(int iColLineNum=0; iColLineNum<carrayColStartPoint.GetSize(); iColLineNum++)
				{
					double dColBaseStartY = base_pt[1] - (carrayColStartPoint.GetAt(iColLineNum))*dHeightRate ;
					double dColBaseEndY= base_pt[1] - (carrayColEndPoint.GetAt(iColLineNum))*dHeightRate ;

					AcGePoint3d start_3d(dRowBaseX, dColBaseStartY, 0) ;
					AcGePoint3d end_3d(dRowBaseX, dColBaseEndY, 0) ;
					this->AddAcDbLine(start_3d, end_3d) ;
				}
			}
			else
			{
				AfxMessageBox(_T("线段坐标的起点和终点不对称!")) ;
			}


		}
		//end画列线
		//////////////////////////////////////////////////////////////////////////

		//画左边线
		double dLeftBottomPointX = base_pt[0] ;
		double dLeftBottomPointY = base_pt[1] - dHeightSumTmp_2 * dHeightRate ;
		AcGePoint3d lefttop_p(0, 0, 0) ;
		AcGePoint3d leftbottom_p(dLeftBottomPointX, dLeftBottomPointY, 0) ;
		this->ConvertPtAdsTo3D(base_pt, lefttop_p) ;
		this->AddAcDbLine(lefttop_p, leftbottom_p) ;

		//画上边线
		double dRightTopPointX = base_pt[0] + dWidthSumTmp * dWidthRate ;
		double dRightTopPointY = base_pt[1] ;
		AcGePoint3d righttop_p(dRightTopPointX, dRightTopPointY, 0) ;
		this->AddAcDbLine(lefttop_p, righttop_p) ;

		///关闭打开的各个资源
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		//m_pExcel->Release() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		bReturn = FALSE ;
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		bReturn = FALSE ;
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}	

	return bReturn ;
}


// 主要用在替换块时,通过选取文件来插入块 ;
// ////////////////////////////////////////////////////////////////////////
// BOOL CUserSystem::InsertDwgFileByPath(CString strFilePath, AcDbObjectId &Block_Id)
// {
// 	BOOL bReturn = FALSE ;
// 
// 	CString strDwgName, strBlockName ;
// 	CStringArray strArrayTmp ;
// 	CBlkUtility::DivideString(strFilePath, _T("\\"), strArrayTmp) ;
// 	strDwgName = strArrayTmp.GetAt(strArrayTmp.GetSize()-1) ;
// 
// 	int iLoc = strDwgName.Find(_T(".")) ;
// 	strBlockName = strDwgName.Mid(0, iLoc) ;
// 
// 	if( this->HaveBlockName ( strBlockName , Block_Id ) )
// 	{
// 		bReturn = TRUE ;
// 	}
// 	else
// 	{
// 		AcDbDatabase * pTemp_Db = new AcDbDatabase(false);
// 		if( this->Verify_FileName( strFilePath , FALSE ) )
// 		{	
// 			Acad::ErrorStatus itt = pTemp_Db->readDwgFile( strFilePath ) ;
// 
// 			if( Acad::eOk == itt )
// 			{
// 				Acad::ErrorStatus eInsert = m_pDb->insert( Block_Id , strBlockName  , pTemp_Db ) ;
// 				if( Acad::eOk == eInsert )
// 				{
// 					bReturn = TRUE ;
// 				}				
// 			}
// 		}
// 		delete pTemp_Db ;
// 	}
// 	return bReturn ;
// }

// 查找替换块 ;
// nByBlockName=0;使用块名替换
// nByBlockName=1;使用文件替换
// ////////////////////////////////////////////////////////////////////////
// 
// int CUserSystem::FindReplaceBlock(CString strOldBlockName, CString strNewBlockFile, int nByBlockName, BOOL bIsGlobal)
// {		
// 	AcDbBlockReference * pRef = NULL ;
// 	int iBlockNum = 0 ;
// 
// 	//CStringArray strArrayVal ;
// 
// 	//全图转换
// 	if (bIsGlobal) 
// 	{		
// 		AcDbBlockTable * pBlkTbl = NULL ;
// 		m_pDb->getBlockTable(pBlkTbl,AcDb::kForWrite);
// 
// 		AcDbBlockTableRecord * pBlkTblRec = NULL ;
// 		pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
// 		//pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForWrite )  ;
// 		pBlkTbl->close() ;
// 		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
// 
// 		pBlkTblRec->newIterator( pBlkTblRecIter );
// 		pBlkTblRec->close();
// 
// 		AcDbEntity * pEnt = NULL ;
// 		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
// 		{
// 			//遍历并打开实体 ;		
// 
// 			if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
// 			{
// 				AcDbBlockReference * pBlockRef = NULL ;
// 
// 				//仅处理块 ；
// 				if (pEnt->isKindOf(AcDbBlockReference::desc()))
// 				{
// 					pBlockRef = (AcDbBlockReference *)pEnt ;
// 
// 					AcDbObjectId blockid ;
// 					blockid = pBlockRef->blockTableRecord() ;
// 
// 					AcDbBlockTableRecord *pBlkDefRecord ;
// 
// 					acdbOpenObject(pBlkDefRecord, blockid, AcDb::kForWrite);
// 
// 					TCHAR *name;
// 					pBlkDefRecord->getName(name) ;
// 					_tcsupr(name) ;
// 					//strupr(name) ;
// 					if (_tcscmp(name,strOldBlockName) == 0)
// 					{		
// 						AcGePoint3d blockPoint = pBlockRef->position() ;					    
// 						/*
// 						AcDbObjectIterator * pAttiteror = pBlockRef->attributeIterator() ;						
// 						strArrayVal.RemoveAll() ;					
// 						for(pAttiteror->start(); !pAttiteror->done(); pAttiteror->step())
// 						{
// 						AcDbObjectId attObjId ;
// 						attObjId = pAttiteror->objectId() ;							
// 						AcDbAttribute *pAtt = NULL ;
// 						if (Acad::eOk != acdbOpenObject(pAtt, attObjId, AcDb::kForRead))
// 						{
// 						delete pAttiteror ;
// 						continue ;
// 						}
// 						CString strAtt = pAtt->textString() ;
// 						strArrayVal.Add(strAtt) ;
// 						pAtt->close() ;							
// 						}
// 						delete pAttiteror ;
// 						pAttiteror = NULL ;
// 						*/
// 
// 						if (this->InsertBlock(pRef, strNewBlockFile, blockPoint, FALSE, nByBlockName)) 
// 						{
// 							pBlockRef->erase() ;
// 
// 							pRef->close() ;
// 						}
// 						iBlockNum++ ;
// 					}		
// 					pBlkDefRecord->close() ;
// 				}
// 				pEnt->close() ;
// 			}			
// 
// 		}
// 		delete pBlkTblRecIter ;
// 		pBlkTblRecIter = NULL ;
// 
// 	}
// 	else
// 	{
// 		ads_name sum_name  , temp_name  ;
// 		struct resbuf eb ;
// 		TCHAR chSbuf[10] ;
// 		const CString strDxf = _T("INSERT") ;
// 		CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
// 		eb.restype = 0 ;
// 		eb.resval .rstring = chSbuf ;
// 		eb.rbnext = NULL ;
// 		ads_printf ( _T("\n选择实体: ")) ;
// 
// 		if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
// 		{
// 			long lLength = 0 ;
// 			ads_sslength(sum_name, &lLength) ;
// 			for(int i = 0; i<lLength; i++)
// 			{
// 				ads_ssname(sum_name, i, temp_name) ;		
// 
// 				AcDbEntity * pEnt = NULL ;	
// 
// 				if (OpenAcDbEntity(temp_name, pEnt))
// 				{	
// 					AcDbBlockReference * pBlockRef = NULL ;				
// 					if (pEnt->isKindOf(AcDbBlockReference::desc()))
// 					{	
// 						pBlockRef = (AcDbBlockReference *)pEnt ;
// 						AcDbObjectId blockid ;
// 						blockid = pBlockRef->blockTableRecord() ;
// 
// 						AcDbBlockTableRecord *pBlkDefRecord ;
// 
// 						acdbOpenObject(pBlkDefRecord, blockid, AcDb::kForWrite);
// 						TCHAR *name;
// 						pBlkDefRecord->getName(name) ;
// 						_tcsupr(name) ;
// 						if (_tcscmp(name, strOldBlockName) == 0)
// 						{
// 							AcGePoint3d blockPoint = pBlockRef->position() ;
// 
// 							if (this->InsertBlock(pRef, strNewBlockFile, blockPoint, FALSE, nByBlockName)) 
// 							{
// 								pBlockRef->erase() ;
// 								pRef->close() ;
// 							}					
// 
// 							iBlockNum++ ;
// 						}
// 						pBlkDefRecord->close() ;		
// 					}
// 				}
// 				pEnt->close() ;
// 			}
// 			ads_ssfree(sum_name) ;
// 		}
// 	}
// 
// 	return iBlockNum ;
// }

// BOOL CUserSystem::InsertDwgFileByBlock(CString strBlockName, AcDbObjectId &Block_Id)
// {
// 	BOOL bReturn = FALSE ;
// 
// 	if( this->HaveBlockName ( strBlockName , Block_Id ) )
// 	{
// 		bReturn = TRUE ;
// 	}
// 	else
// 	{
// 		AcDbDatabase * pTemp_Db = new AcDbDatabase(false);
// 		CString strDwgFileName ;
// 		strDwgFileName.Format( _T("%s.dwg") , strBlockName ) ;		
// 		TCHAR chDwgPath[MAX_PATH] ;
// 		if (RTNORM == acedFindFile(strDwgFileName,chDwgPath))
// 		{
// 			Acad::ErrorStatus itt = pTemp_Db->readDwgFile( chDwgPath ) ;
// 
// 			if( Acad::eOk == itt )
// 			{
// 				if( Acad::eOk == m_pDb->insert( Block_Id , strBlockName  , pTemp_Db ) )
// 				{
// 					bReturn = TRUE ;
// 				}				
// 			}
// 		}					
// 
// 		delete pTemp_Db ;
// 	}
// 	return bReturn ;
// }

AcDbObjectId CUserSystem::Set_Layer(CString strStatus, CString strType)
{
	AcDbObjectId lay_Id ;
	CString strLayerName;
	strLayerName = strStatus + strType;
	AcDbLayerTable * pLayTbl;
	m_pDb->getLayerTable(pLayTbl,AcDb::kForWrite);
	if (Acad::eOk==pLayTbl->getAt(strLayerName,lay_Id, Adesk::kFalse))
	{

	}
	else
	{
		int iColorIndex = 7 ;
		AcDbLayerTableRecord *pLayTblRec = new AcDbLayerTableRecord;
		AcCmColor color;
		if( strStatus == _T("新建") )
		{
			iColorIndex = 3 ;
		}
		else if(strStatus == _T("原有") )
		{
			iColorIndex = 7 ;
		}
		else if(strStatus == _T("更换") )
		{
			iColorIndex = 6 ;
		}
		else if(strStatus == _T("拆除") )
		{
			iColorIndex = 2 ;
		}
		else 
		{
			iColorIndex = 7 ;
		}
		color.setColorIndex(iColorIndex);
		pLayTblRec->setColor(color);
		pLayTblRec->setName(strLayerName);
		pLayTbl->add(lay_Id,pLayTblRec);	
		pLayTblRec->close() ;			
	}

	pLayTbl->close();
	m_pDb->setClayer(lay_Id);	

	return lay_Id ;
}

AcDbObjectId CUserSystem::SetCLayer(const CString&  strLayer , int iColorIndex)
{
	AcDbObjectId objId = this->CreateLayer(strLayer, iColorIndex) ;
	m_pDb->setClayer(objId);	

	return objId ;
}

// 设置实体到指定的层
int CUserSystem::SetLayer(long nHandle, const CString& strLayer)
{
	AcDbObjectId objId = this->CreateLayer(strLayer) ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt))
	{
		pEnt->setLayer(objId) ;
		pEnt->close() ;
	}

	return 0;
}

// 创建图层
AcDbObjectId CUserSystem::CreateLayer(const CString& strLayer, int iColorIndex)
{
	AcDbObjectId objId ;
	AcDbLayerTable * pLayerTbl = NULL ;
	m_pDb->getLayerTable(pLayerTbl, AcDb::kForWrite);
	if (Acad::eOk!=pLayerTbl->getAt(strLayer, objId, Adesk::kFalse))
	{
		AcDbLayerTableRecord *pLayerTblRec = new AcDbLayerTableRecord;
		AcCmColor color;
		color.setColorIndex(iColorIndex);
		pLayerTblRec->setColor(color);
		pLayerTblRec->setName(strLayer);
		pLayerTbl->add(objId, pLayerTblRec);	
		pLayerTblRec->close() ;	
	}
	pLayerTbl->close();

	return objId ;
}

// BOOL CUserSystem::SetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, short iVal)
// {
// 	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
// 	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //不设为workingdatabase的话，acdbRegApp始终返回-5001错误，setXdata报错，notfoundAppID
// 
// 	BOOL bRet = FALSE ;
// 	struct resbuf * pRb;
// 	pRb = pEnt->xData(chAppName);
// 	if(pRb != NULL)
// 	{
// 		ads_relrb(pRb);
// 		pRb = NULL ;
// 	}
// 	ads_regapp(chAppName);
// 	pRb = acutBuildList(AcDb::kDxfRegAppName, chAppName, AcDb::kDxfXdInteger16, iVal, 0) ;
// 	Acad::ErrorStatus es = Acad::eOk  ;
// 	Adesk::Boolean bIsWriteAble = pEnt->isWriteEnabled() ;
// 	if(Adesk::kFalse==bIsWriteAble)
// 	{
// 		es = pEnt->upgradeOpen() ;
// 	}
// 	if(Acad::eOk == es)
// 	{
// 		if (Acad::eOk == pEnt->setXData(pRb))
// 		{
// 			bRet = TRUE ;
// 		}
// 	}
// 	//恢复读写权限
// 	if (Adesk::kFalse==bIsWriteAble)
// 	{
// 		pEnt->downgradeOpen() ;
// 	}	
// 	ads_relrb(pRb);
// 
// 	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;
// 
// 	return bRet;
// }
// BOOL CUserSystem::SetXdata(AcDbObjectId objId, const TCHAR *chAppName, short iVal)
// {
// 	BOOL bRet = FALSE ;
// 
// 	AcDbEntity *pEnt = NULL ;
// 	if (this->OpenAcDbEntity(objId, pEnt, AcDb::kForRead))
// 	{
// 		bRet = SetXdata(pEnt, chAppName, iVal) ;
// 		pEnt->close() ;
// 	}
// 
// 	return bRet ;
// }

BOOL CUserSystem::SetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, int iVal)
{
	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //不设为workingdatabase的话，acdbRegApp始终返回-5001错误，setXdata报错，notfoundAppID

	BOOL bRet = FALSE ;
	struct resbuf * pRb;
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		ads_relrb(pRb);
		pRb = NULL ;
	}
	ads_regapp(chAppName);
	pRb = acutBuildList(AcDb::kDxfRegAppName, chAppName, AcDb::kDxfXdInteger32, iVal, 0) ;
	Acad::ErrorStatus es = Acad::eOk  ;
	Adesk::Boolean bIsWriteAble = pEnt->isWriteEnabled() ;
	if(Adesk::kFalse==bIsWriteAble)
	{
		es = pEnt->upgradeOpen() ;
	}
	if(Acad::eOk == es)
	{
		if (Acad::eOk == pEnt->setXData(pRb))
		{
			bRet = TRUE ;
		}
	}
	//恢复读写权限
	if (Adesk::kFalse==bIsWriteAble)
	{
		pEnt->downgradeOpen() ;
	}
	ads_relrb(pRb);

	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;

	return bRet;
}

BOOL CUserSystem::SetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, double dVal)
{
	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //不设为workingdatabase的话，acdbRegApp始终返回-5001错误，setXdata报错，notfoundAppID

	BOOL bRet = FALSE ;
	struct resbuf * pRb;
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		ads_relrb(pRb);
		pRb = NULL ;
	}
	ads_regapp(chAppName);
	pRb = acutBuildList(AcDb::kDxfRegAppName, chAppName, AcDb::kDxfXdReal, dVal, 0) ;
	Acad::ErrorStatus es = Acad::eOk  ;
	Adesk::Boolean bIsWriteAble = pEnt->isWriteEnabled() ;
	if(Adesk::kFalse==bIsWriteAble)
	{
		es = pEnt->upgradeOpen() ;
	}
	if(Acad::eOk == es)
	{
		if (Acad::eOk == pEnt->setXData(pRb))
		{
			bRet = TRUE ;
		}
	}
	//恢复读写权限
	if (Adesk::kFalse==bIsWriteAble)
	{
		pEnt->downgradeOpen() ;
	}
	ads_relrb(pRb);

	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;

	return bRet;
}

BOOL CUserSystem::SetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, long iVal)
{
	return SetXdata(pEnt, chAppName, (int)iVal) ;
}

BOOL CUserSystem::SetXdata(AcDbObjectId objId, const TCHAR *chAppName, int iVal)
{
	BOOL bRet = FALSE ;

	AcDbEntity *pEnt = NULL ;
	if (this->OpenAcDbEntity(objId, pEnt, AcDb::kForRead))
	{
		bRet = SetXdata(pEnt, chAppName, iVal) ;
		pEnt->close() ;
	}

	return bRet ;
}
BOOL CUserSystem::SetXdata(long nHandle, const TCHAR *chAppName, long iVal)
{
	BOOL bRet = FALSE ;

	AcDbEntity *pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		bRet = SetXdata(pEnt, chAppName, iVal) ;
		pEnt->close() ;
	}

	return bRet ;
}
BOOL CUserSystem::SetXdata(long nHandle, const TCHAR *chAppName, const TCHAR *chVar)
{
	BOOL bRet = FALSE ;

	AcDbEntity *pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		bRet = SetXdata(pEnt, chAppName, chVar) ;
		pEnt->close() ;
	}

	return bRet ;
}

int CUserSystem::RegisterAppName(const TCHAR *appname) 
{ 
	int ret = -1; // error code 
	AcDbRegAppTable *pRegAppTable = NULL; 
	m_pDb->getSymbolTable(pRegAppTable, AcDb::kForRead); 
	if (pRegAppTable) 
	{ 
		if (!pRegAppTable->has(appname)) 
		{ 
			AcDbRegAppTableRecord *pRegAppTableRecord = new AcDbRegAppTableRecord(); 
			pRegAppTableRecord->setName(appname); 
			pRegAppTable->upgradeOpen(); 
			pRegAppTable->add(pRegAppTableRecord); 
			pRegAppTableRecord->close();
			pRegAppTableRecord = NULL ;
			ret = 1; 
			// if added 
		} 
		else 
		{ 
			ret = 0; // If already exist appname 
		} 
		pRegAppTable->close(); 
		pRegAppTable = NULL ;
	}

	return ret ;
} 
 
BOOL CUserSystem::SetXdata(AcDbEntity *pEnt, const TCHAR *chAppName, const TCHAR *chVar)
{
	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //不设为workingdatabase的话，acdbRegApp始终返回-5001错误，setXdata报错，notfoundAppID

	BOOL bRet = FALSE ;
	struct resbuf * pRb;
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		ads_relrb(pRb);
		pRb = NULL ;
	}
	int iRt = acdbRegApp(chAppName);

	pRb = acutBuildList(AcDb::kDxfRegAppName, chAppName, AcDb::kDxfXdAsciiString, chVar, 0) ;
	Acad::ErrorStatus es = Acad::eOk  ;
	Adesk::Boolean bIsWriteAble = pEnt->isWriteEnabled() ;
	if(Adesk::kFalse==bIsWriteAble)
	{
		es = pEnt->upgradeOpen() ;
	}
	if(Acad::eOk == es)
	{
		es = pEnt->setXData(pRb) ; 
		if (Acad::eOk == es)
		{
			bRet = TRUE ;
		}
	}
	//恢复读写权限
	if (Adesk::kFalse==bIsWriteAble)
	{
		pEnt->downgradeOpen() ;
	}	
	ads_relrb(pRb);

	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;
	return bRet;
}

BOOL CUserSystem::SetXdata(AcDbObjectId objId, const TCHAR *chAppName, const TCHAR *chVar)
{
	BOOL bRet = FALSE ;

	AcDbEntity *pEnt = NULL ;
	if (this->OpenAcDbEntity(objId, pEnt, AcDb::kForRead))
	{
		bRet = SetXdata(pEnt, chAppName, chVar) ;
		pEnt->close() ;
	}

	return bRet ;
}
BOOL CUserSystem::DeleteXdata(AcDbEntity *pEnt, const TCHAR *chAppName)
{
	BOOL bRet = TRUE ;

	struct resbuf *pRb=NULL; 
	pRb = pEnt->xData(chAppName);
	if(pRb != NULL)
	{
		acutRelRb(pRb) ;
		pRb = NULL ;

		pRb = acutBuildList(AcDb::kDxfRegAppName, chAppName, 0) ;

		Acad::ErrorStatus es = Acad::eOk  ;
		Adesk::Boolean bIsWriteAble = pEnt->isWriteEnabled() ;
		if(Adesk::kFalse==bIsWriteAble)
		{
			es = pEnt->upgradeOpen() ;
		}
		if(Acad::eOk == es)
		{
			Acad::ErrorStatus es2 = pEnt->setXData(pRb) ;
			if (Acad::eOk != es2)
			{
				bRet = FALSE ;
			}
		}
		//恢复读写权限
		if (Adesk::kFalse==bIsWriteAble)
		{
			pEnt->downgradeOpen() ;
		}

        acutRelRb(pRb);
		pRb = NULL ;
	}

	return bRet ;

}


//删除选定的文本的属性
void CUserSystem::DelSelXdata()
{
	ads_name sum_name , temp_name ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("TEXT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf( _T("\n请选择要删除属性的文本\n ") ) ;
	if( RTNORM == ads_ssget( NULL , NULL , NULL , &eb , sum_name ) )
	{
		long lLength = 0 ;

		ads_sslength(sum_name, &lLength) ;
		for(int i = 0; i<lLength; i++)
		{
			ads_ssname(sum_name, i, temp_name) ;
			AcDbEntity * pEnt = NULL ;
			if (OpenAcDbEntity(temp_name, pEnt))
			{
				if (pEnt->isKindOf(AcDbText::desc()))
				{
					struct resbuf *pRb, *pTmp;
					pRb = pEnt->xData(NULL);
					if (pRb != NULL) 
					{
						ads_printf(_T("\n")) ;
						for(pTmp=pRb; pTmp!=NULL; pTmp = pTmp->rbnext)
						{
							switch(pTmp->restype) 
							{
							case 1001:
								{
									CString strAppName ;
									strAppName.Format(_T("%s"), pTmp->resval.rstring) ;
									DeleteXdata(pEnt, strAppName) ;

									ads_printf(_T("删除属性:%s\n"),strAppName) ;

								}							
								break;
							default:
								//ads_printf("unknown type\n") ;
								break;
							}							
						}
						ads_relrb(pRb) ;
						//acutRelRb(pRb);
					}
					else
					{
						ads_printf(_T("\nNo xdata for this appname"));
					}
				}


				pEnt->close() ;
				pEnt = NULL ;

			}
		}
		ads_ssfree(sum_name) ;
	}


}

// AcDbObjectId CUserSystem::PostToModelSpace(AcDbEntity *pEnt)
// {
// 	AcDbBlockTable *pBlockTable; 
// 	acdbHostApplicationServices()->workingDatabase() 
// 		->getBlockTable(pBlockTable, AcDb::kForRead);   
// 	
// 	AcDbBlockTableRecord *pBlockTableRecord; 
//     pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, 
//         AcDb::kForWrite);     
// 	
// 	AcDbObjectId entId; 
// 	pBlockTableRecord->appendAcDbEntity(entId, pEnt);  
// 	
// 	pBlockTable->close(); 
// 	pBlockTableRecord->close(); 
// 	pEnt->close();  
// 	
// 	return entId;
// }

//获取CAD系统变量
void CUserSystem::GetSysVar(CString strVarName,CString & strVal)
{
	struct resbuf rb;
	acedGetVar(strVarName,&rb);
	if (rb.restype==RTSTR)
	{
		strVal = rb.resval.rstring;
	}
	return;
}
void CUserSystem::GetSysVar(CString strVarName,int & iVar)
{
	struct resbuf rb;
	acedGetVar(strVarName ,&rb);
	if (rb.restype==RTSHORT)
	{
		iVar=rb.resval.rint;
	}
	return ;
}

//设置CAD系统变量
int CUserSystem::SetSysVar(const CString strVarName, const CString strVal)
{
	struct resbuf rb  ; 
	rb.restype = RTSTR; 
	_tcscpy(rb.resval.rstring, strVal) ;
	if (rb.resval.rstring!=_T("")) 
		return acedSetVar(strVarName, &rb) ;	
	else
		return -1 ;
}
int CUserSystem::SetSysVar(const CString strVarName, int  iVar)
{
	struct resbuf rb; 
	rb.restype = RTSHORT; 
	rb.resval.rint = iVar; 
	return acedSetVar(strVarName, &rb) ;
}

// 取得当前图纸中最大的ID值（xdata设置的appname为ID的）
long CUserSystem::GetMaxId(void)
{
	long nRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

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
		Acad::ErrorStatus es = pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead) ;

		if (Acad::eOk == es)
		{
			int nId = 0 ;
			if (this->GetXdata(pEnt, _T("ID"), nId))
			{
				if (nId>nRet)
				{
					nRet = nId ;
				}
			}				
			
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return nRet ;
}

// 显示或隐藏块指定的属性
BOOL CUserSystem::ShowAtt(AcDbBlockReference * pRef, const TCHAR * chTag, bool bShow)
{
	BOOL bRet = FALSE ;

	AcDbAttribute * pAtt = NULL ;
	if (this->FindAtt(pRef, chTag, pAtt)==TRUE)
	{
		pAtt->upgradeOpen() ;
		if (pAtt->setInvisible(bShow)==Acad::eOk)
		{
			bRet = TRUE ;
		}
		pAtt->downgradeOpen() ;
		pAtt->close() ; //一定不要忘了关闭此对象
	}
	
	return bRet;
}



// 取得当前图纸上实体占用空间的最左下点和最右上点
int CUserSystem::GetCurExtents(AcGePoint3d & pt_3d_min, AcGePoint3d & pt_3d_max)
{	
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	double dMinX = 0.000, dMaxX = 0.000, dMinY = 0.000, dMaxY = 0.000 ;
	
	BOOL bFirst = TRUE ;

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;		
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			AcDbExtents exts ;
			pEnt->getGeomExtents(exts) ;
			pt_3d_max = exts.maxPoint() ;				
			pt_3d_min = exts.minPoint() ;
			if (bFirst)
			{
				dMinX = pt_3d_min.x ;
				dMaxX = pt_3d_max.x ;
				dMinY = pt_3d_min.y ;
				dMaxY = pt_3d_max.y ;
				bFirst = FALSE ;
			}
			else
			{
				if (pt_3d_min.x<dMinX)
				{
					dMinX = pt_3d_min.x ;
				}
				if (pt_3d_max.x>dMaxX)
				{
					dMaxX = pt_3d_max.x ;
				}
				if (pt_3d_min.y<dMinY)
				{
					dMinY = pt_3d_min.y ;
				}
				if (pt_3d_max.y>dMaxY)
				{
					dMaxY = pt_3d_max.y ;
				}
			}

			pEnt->close() ;
		}			

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	pt_3d_min.x = dMinX ;
	pt_3d_min.y = dMinY ;
	pt_3d_max.x = dMaxX ;
	pt_3d_max.y = dMaxY ;

	return 0;
}

// 查找指定区域的实体
//iOption:默认=0,表示包含边界(只要实体任何部分在区域内即算实体在该区域);其他=1,表示不包含边界,只包含边界内的实体; iEntType默认为0,表示所有实体, 1 文本, 2 块
int CUserSystem::FindEntInArea(const AcGePoint3d & pt_3d_bottomleft, const AcGePoint3d & pt_3d_topright, CLongArray & iArrayEntHandle, int iOption, int iEntType)
{
	int iRet = 0 ;
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

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
			//过滤不想查找的
			if ( (iEntType==1&&!pEnt->isKindOf(AcDbText::desc())) || (iEntType==2&&!pEnt->isKindOf(AcDbBlockReference::desc())) )
			{
				pEnt->close() ;
				continue;
			}
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||this->IsOnOffLayer(pEnt))
			{
                pEnt->close() ;
				continue;
			}


			//AcDb::Visibility bVis = pEnt->visibility()  ; //是否隐藏
			//acutPrintf(_T("\nthis is %s-%d"), pEnt->isA()->name(), bVis) ;
			//if (bVis==AcDb::kInvisible)//只处理非隐藏的
			{
				AcDbExtents entExtents ;
				pEnt->getGeomExtents(entExtents) ;

				AcGePoint3d pt_3d_ext_min(0,0,0), pt_3d_ext_max(0,0,0) ;
				pt_3d_ext_min = entExtents.minPoint() ;
				pt_3d_ext_max = entExtents.maxPoint() ;

				//假定矩形是用一对点表达的(minx, miny) (maxx, maxy)，那么两个矩形
				//rect1{(minx1, miny1)(maxx1, maxy1)}
				//rect2{(minx2, miny2)(maxx2, maxy2)}  
				//相交的结果一定是个矩形，构成这个相交矩形rect{(minx, miny) (maxx, maxy)}的点对坐标是：  
				//	minx   =   max(minx1,   minx2)  
				//	miny   =   max(miny1,   miny2)  
				//	maxx   =   min(maxx1,   maxx2)  
				//	maxy   =   min(maxy1,   maxy2)
				//	如果两个矩形不相交，那么计算得到的点对坐标必然满足：  
				//	（ minx  >  maxx ） 或者 （ miny  >  maxy ）
				//判定是否相交，以及相交矩形是什么都可以用这个方法一体计算完成。
				//	从这个算法的结果上，我们还可以简单的生成出下面的两个内容：
				//	㈠ 相交矩形：  (minx, miny) (maxx, maxy)
				//	㈡ 面积： 面积的计算可以和判定一起进行
				//	if ( minx>maxx ) return 0;
				//  if ( miny>maxy ) return 0;
				//  return (maxx-minx)*(maxy-miny)

				AcGePoint3d pt_3d_cross_min(0,0,0), pt_3d_cross_max(0,0,0) ;
				pt_3d_cross_min.x = max(pt_3d_bottomleft.x, pt_3d_ext_min.x) ;
				pt_3d_cross_min.y = max(pt_3d_bottomleft.y, pt_3d_ext_min.y) ;
				pt_3d_cross_max.x = min(pt_3d_topright.x, pt_3d_ext_max.x) ;
				pt_3d_cross_max.y = min(pt_3d_topright.y, pt_3d_ext_max.y) ;
				//只要实体entExtents任何部分在区域内即算实体在该区域，类似acedSSGet(_T("C")....)
				//if ((pt_3d_min.x>pt_3d_bottomleft.x && pt_3d_min.x<pt_3d_topright.x && (pt_3d_min.y>pt_3d_bottomleft.y && pt_3d_min.y<pt_3d_topright.y||pt_3d_max.y>pt_3d_bottomleft.y && pt_3d_max.y<pt_3d_topright.y)) ||(pt_3d_max.x>pt_3d_bottomleft.x && pt_3d_max.x<pt_3d_topright.x && (pt_3d_max.y>pt_3d_bottomleft.y && pt_3d_max.y<pt_3d_topright.y||pt_3d_min.y>pt_3d_bottomleft.y && pt_3d_min.y<pt_3d_topright.y)) )
				if (!(pt_3d_cross_min.x>pt_3d_cross_max.x||pt_3d_cross_min.y>pt_3d_cross_max.y))
				{
					if (iOption==1)
					{
						if (pt_3d_cross_min.x>pt_3d_bottomleft.x&&pt_3d_cross_min.y>pt_3d_bottomleft.y&&pt_3d_cross_max.x<pt_3d_topright.x&&pt_3d_cross_max.y<pt_3d_topright.y)
						{
							UINT iHandle = 0 ;
							iHandle = this->GetHandle(pEnt) ;
							iArrayEntHandle.Add(iHandle) ;
							iRet++ ;
						}
					}
					else
					{
						UINT iHandle = 0 ;
						iHandle = this->GetHandle(pEnt) ;
						iArrayEntHandle.Add(iHandle) ;
						iRet++ ;
					}
				}
			}		
	
			pEnt->close() ;
		}

	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;	
}

// 修改块属性的字体大小
void CUserSystem::SetAttHeight(AcDbBlockReference *& pRef, double dHeight, double dWidthFactor, const CString & strTag, int iAttIndexToSet)
{	
// 	BOOL bReturn = FALSE ;
// 	AcDbAttribute * pAtt = NULL ;
// 	if( this->FindAtt ( pRef, strTag, pAtt ) )
// 	{
// 		pAtt->setColorIndex(nColorIndex) ;
// 		pAtt->close() ;
// 		bReturn = TRUE ;
// 	}
// 
// 	return bReturn ;

	AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;

	acdbHostApplicationServices()->setWorkingDatabase(this->m_pDb) ; //解决后台数据库写入的文本，对齐方式问题，记住，文本写入数据库后，要把当前数据库设置为原始状态


	AcDbObjectIterator* pAttItor = pRef->attributeIterator() ;
	int iAttIndexCur = 0 ;
	for(pAttItor->start(); !pAttItor->done(); pAttItor->step())
	{
		AcDbEntity *pEnt = NULL ;
		AcDbObjectId objId = pAttItor->objectId() ; //注意区别 pAttItor->entity()
		if (OpenAcDbEntity(objId, pEnt, AcDb::kForWrite))
		{
			AcDbAttribute *pAtt = AcDbAttribute::cast(pEnt) ;
			if (!strTag.IsEmpty())
			{
				if (_tcscmp(strTag,pAtt->tag())==0)
				{
					if (dHeight!=0)
					{
						pAtt->setHeight(dHeight) ;
					}
					if (dWidthFactor!=0)
					{
						pAtt->setWidthFactor(dWidthFactor) ;
					}

					pEnt->close() ;
					break;
				}
			}
			else
			{
				if (iAttIndexCur==iAttIndexToSet)
				{
					if (dHeight!=0)
					{
						pAtt->setHeight(dHeight) ;
					}
					if (dWidthFactor!=0)
					{
						pAtt->setWidthFactor(dWidthFactor) ;
					}
					pEnt->close() ;
					break;
				}
			}
			
			pEnt->close() ;
		}

		iAttIndexCur++ ;
	}

	acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;
}

// 旋转图块（块参照类的setRotation函数不会旋转其带的属性）
Acad::ErrorStatus CUserSystem::RotationEnt(AcDbEntity* pEnt, double dAngle, const AcGePoint3d& center)
{
	AcGeMatrix3d xform; 	
	xform.setToRotation(dAngle, AcGeVector3d::kZAxis, center) ;
	Acad::ErrorStatus es = pEnt->transformBy(xform); 

	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
// 		double dAngleBlock = pRef->rotation() ;

		AcDbObjectIterator* pAttItor = pRef->attributeIterator() ;
		for(pAttItor->start(); !pAttItor->done(); pAttItor->step())
		{
			AcDbEntity *pEntAtt = NULL ;
			AcDbObjectId objId = pAttItor->objectId() ; //注意区别 pAttItor->entity()
			if (OpenAcDbEntity(objId, pEntAtt, AcDb::kForWrite))
			{
				AcDbAttribute *pAtt = AcDbAttribute::cast(pEntAtt) ;
				double dAngleAtt = pAtt->rotation() ;
				AcDbExtents ext ;
				pAtt->getGeomExtents(ext) ;
				AcGePoint3d pt_3d_min = ext.minPoint() ;
				AcGePoint3d pt_3d_max = ext.maxPoint() ;
				AcGeLineSeg3d lineseg(pt_3d_min, pt_3d_max) ;
				AcGePoint3d pt_3d_mid = lineseg.midPoint() ;	
				//double dAngleBlockTmp = fmodf(dAngleBlock,PI) ;
				if (dAngleAtt>(PI/2)&&dAngleAtt<(PI*3/2))
				{
					AcGeMatrix3d xform2 ;				
					xform2.setToRotation(PI, AcGeVector3d::kZAxis, pt_3d_mid) ;
					es = pAtt->transformBy(xform2) ;
				}

				pEntAtt->close() ;
			}
		}
	}
	return es ;
}
Acad::ErrorStatus CUserSystem::RotationEnt(AcDbObjectId objId, double dAngle, const AcGePoint3d& center)
{
	Acad::ErrorStatus es = eOk ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(objId, pEnt))
	{
		es = this->RotationEnt(pEnt, dAngle, center) ;
		pEnt->close() ;
	}
	return es ;
}

AcGePoint3d CUserSystem::ucsToWcs(const AcGePoint3d& pt)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

	AcGeMatrix3d m;

	getUcsToWcsMatrix(m, db);
	return m * pt;
}
void CUserSystem::getUcsToWcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db)
{
	ASSERT(db != NULL);

	if (!acdbUcsMatrix(m, db)) {
		m.setToIdentity();
		ASSERT(0);
	}
}
// 镜像
Acad::ErrorStatus CUserSystem::MirrEnt(AcDbEntity* pEnt, const AcGePoint3d& pt_3d_1, const AcGePoint3d& pt_3d_2)
{   
	Acad::ErrorStatus es ;
	AcGeMatrix3d xform; 
	int iVarOld = -1 ;
	this->GetSysVar(_T("MIRRTEXT"), iVarOld) ;
	this->SetSysVar(_T("MIRRTEXT"), 0) ;

// 	if (pEnt->isKindOf(AcDbText::desc()))
// 	{
// 		xform.setToIdentity() ;
// 
// 		AcDbText* pText = AcDbText::cast(pEnt) ;
// 		AcGeVector3d norm ;
// 		norm = pText->normal() ;
// 		xform.setToMirroring(AcGeLine3d(pt_3d_1, pt_3d_2)) ;
// 		es = pText->transformBy(xform) ;
// 		pText->setNormal(norm) ;
// 	}
// 	else
// 	{
		AcGeVector3d uAxis, vAxis ;
		uAxis = pt_3d_2-pt_3d_1;
		vAxis = AcGeVector3d::kZAxis ;
		AcGePlane plane(pt_3d_1, uAxis, vAxis) ;
		xform.setToMirroring(plane) ;
		es = pEnt->transformBy(xform);  //对应属性文本对齐方式为中间的，属性没有镜像（奇怪的现象，还未找到原因）
// 	}
	
this->SetSysVar(_T("MIRRTEXT"), iVarOld) ;
// 		if(pEnt->isKindOf(AcDbBlockReference::desc()))
// 		{
// 			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
// 			AcDbObjectIterator* pAttItor = pRef->attributeIterator() ;
// 			for(pAttItor->start(); !pAttItor->done(); pAttItor->step())
// 			{
// 				AcDbEntity *pEntAtt = NULL ;
// 				AcDbObjectId objId = pAttItor->objectId() ; //注意区别 pAttItor->entity()
// 				if (OpenAcDbEntity(objId, pEntAtt, AcDb::kForWrite))
// 				{
// 					AcDbAttribute *pAtt = AcDbAttribute::cast(pEntAtt) ;
// 					AcDbExtents ext ;
// 					pAtt->getGeomExtents(ext) ;
// 					AcGePoint3d pt_3d_min = ext.minPoint() ;
// 					AcGePoint3d pt_3d_max = ext.maxPoint() ;
// 					AcGeLineSeg3d lineseg(pt_3d_min, pt_3d_max) ;
// 					AcGePoint3d pt_3d_mid = lineseg.midPoint() ;
// 					AcGePlane planeTmp(pt_3d_mid, uAxis, vAxis) ;			
// 					AcGeMatrix3d xform2 ;
// 					xform2.setToMirroring(planeTmp) ;
// 					es = pAtt->transformBy(xform2) ;
// 
// 					pEntAtt->close() ;
// 				}
// 			}	
// 		}
// 		else if(pEnt->isKindOf(AcDbText::desc()))
// 		{
// 
// 		}

	return es ;
}

// 通过handle读取AcdbText的文本
BOOL CUserSystem::GetTextString(long nHandle, CString& strText)
{
	BOOL bRet = FALSE ;
	AcDbEntity *pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText *pText = AcDbText::cast(pEnt) ;
			strText = pText->textString() ;
			bRet = TRUE ;
		}
		pEnt->close() ;
		pEnt = NULL ;

	}
	return bRet ;
}

//两点连线的中点
AcGePoint3d CUserSystem::GetMidPoint(const AcGePoint3d& pt_3d_1, const AcGePoint3d& pt_3d_2)
{
	AcGePoint3d pt_3d_ret(0,0,0) ;
	pt_3d_ret.x = (pt_3d_1.x+pt_3d_2.x)/2 ;
	pt_3d_ret.y = (pt_3d_1.y+pt_3d_2.y)/2 ;
	return pt_3d_ret;
}

//设置实体颜色
Acad::ErrorStatus CUserSystem::SetEntColorIndex(AcDbObjectId objId, Adesk::UInt16 iColorIndex)
{
	Acad::ErrorStatus es ;
	AcDbEntity *pEnt = NULL ;
	if(this->OpenAcDbEntity(objId, pEnt, AcDb::kForWrite))
	{			
		es = pEnt->setColorIndex(iColorIndex) ;
		pEnt->close() ;
	}
	return es ;
}
Acad::ErrorStatus CUserSystem::SetEntColorIndex(ads_name en, Adesk::UInt16 iColorIndex)
{
	AcDbObjectId objId ;
	if(acdbGetObjectId(objId, en)==eOk)
	{
		return SetEntColorIndex(objId, iColorIndex) ;
	}
	else
	{
		return eUnknownHandle ;
	}
}
Acad::ErrorStatus CUserSystem::SetEntColorIndex(long nHandle, Adesk::UInt16 iColorIndex)
{
	AcDbObjectId objId ;
	if(this->GetObjectId(objId, nHandle)==eOk)
	{
		return SetEntColorIndex(objId, iColorIndex) ;
	}
	else
	{
		return eUnknownHandle  ;
	}
}

// 闪烁实体
void CUserSystem::FlashEnt(AcDbObjectId objId, DWORD dwCountLast, DWORD dwDelayCount)
{
	AcDbEntity *pEnt = NULL ;
	if(this->OpenAcDbEntity(objId, pEnt, AcDb::kForRead))
	{			
		DWORD dwCount=::GetTickCount();
		//while(!acedUsrBrk())
		while(TRUE)
		{
			pEnt->highlight();
			::acedUpdateDisplay();
			if (dwDelayCount!=0)
			{
				PauseApp(dwDelayCount);
			}

			pEnt->unhighlight();
			::acedUpdateDisplay();
			if (dwDelayCount!=0)
			{
				PauseApp(dwDelayCount);
			}

			if(::GetTickCount()-dwCount>=dwCountLast)
				break;
		}
		pEnt->close() ;
	}

}
void CUserSystem::FlashEnt(ads_name en, DWORD dwCountLast, DWORD dwDelayCount)
{
	AcDbObjectId objId ;
	acdbGetObjectId(objId, en) ;
	FlashEnt(objId, dwCountLast, dwDelayCount) ;
}
void CUserSystem::FlashEnt(long nHandle, DWORD dwCountLast, DWORD dwDelayCount)
{
	AcDbObjectId objId ;
	this->GetObjectId(objId, nHandle) ;
	FlashEnt(objId, dwCountLast, dwDelayCount) ;
}

// 取得实体的objid
Acad::ErrorStatus CUserSystem::GetObjectId(AcDbObjectId& objId, long nHandle)
{
	AcDbHandle objHandle(nHandle,0) ;
	return m_pDb->getAcDbObjectId(objId,false, objHandle) ;	
}

void CUserSystem::PauseApp(DWORD dwCountLast)
{
	DWORD dwCount=::GetTickCount();
	while(TRUE)
	{
		if(::GetTickCount()-dwCount>=dwCountLast)
			break;
	}
}

// 当前视图中心点
AcGePoint2d CUserSystem::GetViewCenterPoint(void)
{
	struct resbuf rb;
	struct resbuf ucs, dcs; // 转换坐标时使用的坐标系统标记

	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// 当前视口中视图的中心点（UCS坐标）
	acedGetVar(_T("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	return AcGePoint2d( rb.resval.rpoint[0], rb.resval.rpoint[1]);
}

// 当前视图高度
double CUserSystem::GetViewHeight(void)
{
	struct resbuf rb;
	// 当前视口的视图高度（图形单位）
	acedGetVar(_T("VIEWSIZE"), &rb);	
	return rb.resval.rreal;
}

// 当前视图宽度
double CUserSystem::GetViewWidth(void)
{
	struct resbuf rb;
	double dHeight = GetViewHeight() ;
	// 以像素为单位的当前视口的大小（X 和 Y 值）
	acedGetVar(_T("SCREENSIZE"), &rb);	
	return rb.resval.rpoint[X] / rb.resval.rpoint[Y] * dHeight;
}

// 高亮实体
void CUserSystem::Highlight(long nHandle)
{
	AcDbEntity *pEnt = NULL ;
	if(this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		pEnt->highlight();
		pEnt->close() ;
	}
}

// 取消高亮实体
void CUserSystem::UnHighlight(long nHandle)
{
	AcDbEntity *pEnt = NULL ;
	if(this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		pEnt->unhighlight();
		pEnt->close() ;
	}
}

// 取得显示视图的范围
void CUserSystem::GetViewExtents(CViewExtents& viewExt)
{
	AcGePoint2d pt_2d_cen = this->GetViewCenterPoint() ;					
	this->ConvertPoint(pt_2d_cen, viewExt.m_pt3dCen) ;
	viewExt.m_dHeight =this->GetViewHeight() ;
	viewExt.m_dWidth = this->GetViewWidth() ;
}

// 设置视图范围
void CUserSystem::SetViewExtents(const CViewExtents& viewExt)
{
	this->ZoomView(viewExt.m_pt3dCen, viewExt.m_dHeight, viewExt.m_dWidth) ;
}

// 取得所有块表名称(默认包含*Model_Space和*Paper_Space等)
int CUserSystem::GetAllBlockTable(CStringArray& strArrayName, int iMode)
{
	int iRet = 0 ;
	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableIterator* pBlkTblIter = NULL ;
	pBlkTbl->newIterator(pBlkTblIter) ;
	pBlkTbl->close() ;
	for(pBlkTblIter->start();!pBlkTblIter->done();pBlkTblIter->step())
	{
		TCHAR * pName = NULL ;
		AcDbBlockTableRecord* pSymTblRec = NULL ;
		pBlkTblIter->getRecord(pSymTblRec, AcDb::kForRead) ;
		pSymTblRec->getName(pName) ;
		pSymTblRec->close() ;

		CString strName ;
		strName.Format(_T("%s"), pName) ;
		if (!(iMode!=0&&strName.Left(1)==_T("*")||strName==_T("")))
		{
			strArrayName.Add(strName) ;
			iRet++ ;    
		}    
	}
	delete pBlkTblIter ;
	pBlkTblIter = NULL ;

	return iRet ;
}

// 查找具有指定app扩展数据的实体
int CUserSystem::FindEnt(const CString& strAppName, const long nXdata, CLongArray& nArrayHandle)
{
	int iRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			long nXdataTmp = 0 ;
			this->GetXdata(pEnt, strAppName, nXdataTmp) ;
			if (nXdataTmp==nXdata)
			{
				long nHandle = this->GetHandle(pEnt) ;
				nArrayHandle.Add(nHandle) ;
				iRet++ ;
			}
			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

// 取得所有和指定圆相交的直线或多线
//dOffset:允许一定偏差
//iIntersectNum:默认一个交点
int CUserSystem::GetConnectLine(const AcGePoint3d& pt3dCenter, double dRadius,  CLongArray& nArrayHandle, double dOffset, int iIntersectNum)
{
	int iRet = 0 ;

	AcDbCircle* pCircle = new AcDbCircle(pt3dCenter, AcGeVector3d::kZAxis, dRadius+dOffset) ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||this->IsOnOffLayer(pEnt))
			{
				pEnt->close() ;
				continue;
			}

			AcGePoint3dArray pt3dArray ;
			if (pEnt->isKindOf(AcDbLine::desc())||pEnt->isKindOf(AcDbPolyline::desc()))
			{
				if (Acad::eOk== pCircle->intersectWith(pEnt, AcDb::kOnBothOperands, pt3dArray) )
				{
					if (pt3dArray.length()==iIntersectNum)
					{
						nArrayHandle.Add(GetHandle(pEnt)) ;
						iRet++ ;
					}
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	pCircle->close() ;
	delete pCircle ;

	return iRet ;
}

// 取得所有和指定矩形相交的直线或多线
int CUserSystem::GetConnectLine(const AcGePoint3d& pt_3d_min, const AcGePoint3d& pt_3d_max,  CLongArray& nArrayHandle, double dOffset, int iIntersectNum)
{
	int iRet = 0 ;

	AcDbPolyline* pPolyline= new AcDbPolyline(4) ;
	for (int i=0; i<4; i++)
	{
		AcGePoint2d pt2dTmp  ;
		pt2dTmp.x = (i%3==0?pt_3d_min.x:pt_3d_max.x)+dOffset*(i%3==0?-1:1) ;
		pt2dTmp.y = (i/2==1?pt_3d_min.y:pt_3d_max.y)+dOffset*(i/2==1?-1:1) ;
		pPolyline->addVertexAt(i, pt2dTmp) ;
	}
	pPolyline->setClosed(true) ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||this->IsOnOffLayer(pEnt))
			{
				pEnt->close() ;
				continue;
			}

			AcGePoint3dArray pt3dArray ;
			if (pEnt->isKindOf(AcDbLine::desc())||pEnt->isKindOf(AcDbPolyline::desc()))
			{
				if (Acad::eOk== pPolyline->intersectWith(pEnt, AcDb::kOnBothOperands, pt3dArray) )
				{
					if (pt3dArray.length()==iIntersectNum)
					{
						nArrayHandle.Add(GetHandle(pEnt)) ;
						iRet++;
					}
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	pPolyline->close() ;
	delete pPolyline ;

	return iRet ;
}

// 取得要绘制文本的长度(in CAD)
double CUserSystem::GetTextLen(LPCTSTR strText, double dHeight, double dWithFactor)
{
	if(_tcslen(strText) == 0) return(0.) ;  

	CString  strStyleName ;  
	this->GetCurTextStyle(strStyleName);  

	struct resbuf *list = NULL ;  
	list = acutBuildList(RTDXF0, _T("TEXT"), 1, strText, kDxfTextStyleName, strStyleName, 40, dHeight, 41, dWithFactor, 51, 0., NULL);  

	ads_point pt_bottomleft = {0,0,0}, pt_righttop={0,0,0} ;  
	acedTextBox(list,pt_bottomleft,pt_righttop);  
	acutRelRb(list);  

	return fabs(pt_bottomleft[0]-pt_righttop[0]);   
}

//取得当前textstyle
void CUserSystem::GetCurTextStyle(CString& strStyleName)
{
	AcDbObjectId objId = m_pDb->textstyle();
	AcDbTextStyleTableRecord* pStyleRecord;
	if(acdbOpenObject((AcDbObject *&)pStyleRecord, objId, AcDb::kForRead) == Acad::eOk) 
	{
		const TCHAR *pChName;
		pStyleRecord->getName(pChName);
		strStyleName.Format(_T("%s"), pChName) ;
		pStyleRecord->close();
	}
	else 
	{
		strStyleName = _T("STANDARD");
	}
} 

// 反序直线或多段线的顶点,默认iForward=0，左向右，iForward=1，右向左
int CUserSystem::ReverseLinePoint(AcDbEntity*& pEnt, int iForward, int iIndexOfColor)
{
	int iRet = 0 ;

	Adesk::Boolean bIsWriteAble = pEnt->isWriteEnabled() ;
	if(Adesk::kFalse==bIsWriteAble)
	{
		pEnt->upgradeOpen() ;
	}

	if (pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;
		AcGePoint2d pt_2d_1, pt_2d_2 ;
		pPolyLine->getPointAt(0, pt_2d_1) ;
		pPolyLine->getPointAt(pPolyLine->numVerts()-1, pt_2d_2) ;
		if ((pt_2d_2.x<pt_2d_1.x&&iForward==0)||(pt_2d_2.x>pt_2d_1.x&&iForward==1))
		{
			iRet++ ;
			int iNumVert = pPolyLine->numVerts() ;
			for (int i=0; i<(iNumVert/2); i++)
			{
				pPolyLine->getPointAt(i, pt_2d_1) ;
				pPolyLine->getPointAt(iNumVert-i-1, pt_2d_2) ;
				pPolyLine->setPointAt(i, pt_2d_2) ;
				pPolyLine->setPointAt(iNumVert-i-1, pt_2d_1) ;
			}
			if (iIndexOfColor>-1)
			{
				pPolyLine->setColorIndex(3) ;
			}
		}
	}
	else if (pEnt->isKindOf(AcDbLine::desc()))
	{
		AcDbLine* pLine = AcDbLine::cast(pEnt) ;
		AcGePoint3d pt_3d_1, pt_3d_2 ;
		pLine->getStartPoint(pt_3d_1) ;
		pLine->getEndPoint(pt_3d_2) ;
		if ((pt_3d_2.x<pt_3d_1.x&&iForward==0)||(pt_3d_2.x>pt_3d_1.x&&iForward==1))
		{
			iRet++ ;
			pLine->setStartPoint(pt_3d_2) ;
			pLine->setEndPoint(pt_3d_1) ;
			if (iIndexOfColor>-1)
			{
				pLine->setColorIndex(3) ;
			}
		}
	}

	if(Adesk::kFalse==bIsWriteAble)
	{
		pEnt->downgradeOpen() ;
	}

	return iRet ;
}

// 查找起点或终点在指定点的线
int CUserSystem::GetLineOfStartOrEndInPt(const AcGePoint3d& pt_3d_in, const CLongArray& nArrayHandleToFind, CLongArray nArrayHandleLine[2], double dTol)
{
	int iRet = 0 ;

	AcDbEntity* pEnt = NULL ;
	for (int i=0; i<nArrayHandleToFind.GetCount(); i++)
	{
		long nHandleCur = nArrayHandleToFind.GetAt(i) ;
		if (this->OpenAcDbEntity(nHandleCur, pEnt, AcDb::kForRead))
		{
			AcGePoint3d pt_3d_start, pt_3d_end ;
			BOOL bIsLine = FALSE ;
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				AcDbLine* pLine = AcDbLine::cast(pEnt) ;			
				pLine->getStartPoint(pt_3d_start) ;
				pLine->getEndPoint(pt_3d_end) ;
				bIsLine = TRUE ;
			}
			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;				
				pPolyLine->getStartPoint(pt_3d_start) ;
				pPolyLine->getEndPoint(pt_3d_end) ;
				bIsLine = TRUE ;
			}

			if (bIsLine==TRUE)
			{
				AcGeTol tol ;
				tol.setEqualPoint(dTol) ;
				if (pt_3d_in.isEqualTo(pt_3d_start, tol))
				{
					nArrayHandleLine[0].Add(nHandleCur) ;
					iRet++ ;
				}
				else if (pt_3d_in.isEqualTo(pt_3d_end, tol))
				{
					nArrayHandleLine[1].Add(nHandleCur) ;
					iRet++ ;
				}
			}
			pEnt->close() ;
		}
	}

	return iRet ;
}

// 取得指定块的旋转角度
double CUserSystem::GetAngle(long nHandleBlk)
{
	double dAngle = 0.00012345 ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandleBlk, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			dAngle = pRef->rotation() ;
		}
		pEnt->close() ;
	}

	return dAngle;
}

BOOL CUserSystem::Erase(long nHandle)
{
	BOOL bRet = FALSE ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForWrite))
	{
		if (pEnt->erase()==Acad::eOk)
		{
			bRet = TRUE ;
		}
		pEnt->close() ;
	}

	return bRet ;
}
int CUserSystem::Erase(CLongArray& nArrayHandle)
{
	int iRet = 0 ;
	for (int i=0; i<nArrayHandle.GetCount(); i++)
	{
		long nHandle = nArrayHandle.GetAt(i) ;
		if (this->Erase(nHandle))
		{
			iRet++ ;
		}
	}
	
	return iRet ;
}

// 查找和指定多段线连接的盒子等实体
int CUserSystem::FindConnectEnt(const AcDbPolyline* pPolyLine, int iVertIndex, CLongArray& nArrayHandleFind, double dOffset)
{
	int iRet = 0 ;

	AcGePoint3d pt_3d_vert ;
	pPolyLine->getPointAt(iVertIndex, pt_3d_vert) ;
// 	long nHandleSrc = this->GetHandle(pPolyLine) ;

	//以pt_3d_vert为中点创造1根水平和1根垂直线，判断和此2线相交的实体(排除pPolyline）
	AcGePoint3d pt_3d_start, pt_3d_end ;
	for (int i=0; i<2; i++)
	{
		if (i==0)
		{
			pt_3d_start.x = pt_3d_vert.x-dOffset/2 ;
			pt_3d_start.y = pt_3d_vert.y ;
			pt_3d_end.x = pt_3d_vert.x+dOffset/2 ;
			pt_3d_end.y = pt_3d_vert.y ;
		}
		else 
		{
			pt_3d_start.x = pt_3d_vert.x ;
			pt_3d_start.y = pt_3d_vert.y-dOffset/2 ;
			pt_3d_end.x = pt_3d_vert.x ;
			pt_3d_end.y = pt_3d_vert.y+dOffset/2 ;
		}
		AcDbLine* pLine = new AcDbLine(pt_3d_start, pt_3d_end) ;

		AcDbBlockTable * pBlkTbl = NULL ;
		m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
		pBlkTblRec->newIterator( pBlkTblRecIter );
		pBlkTblRec->close();

		AcDbEntity * pEnt = NULL ;
		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
		{
			if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
			{
				if(!pEnt->isEqualTo(pPolyLine))
				{
					AcGePoint3dArray pt3dArray ;

					if (Acad::eOk== pLine->intersectWith(pEnt, AcDb::kOnBothOperands, pt3dArray) )
					{
						if (pt3dArray.length()>0)
						{
							long nHandleCur = GetHandle(pEnt) ;
							if(CBlkUtility::FindLongInArray(nArrayHandleFind, nHandleCur)==-1)
							{
								nArrayHandleFind.Add(nHandleCur) ;								
								acutPrintf(_T("\n%d:%s-%d"), i, pEnt->isA()->name(), nHandleCur) ;
								iRet++ ;
							}
						}
					}
				}		

				pEnt->close() ;
			}	
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;
		pLine->close() ;
		delete pLine ;
	}

	return iRet ;
}

int CUserSystem::FindConnectEnt(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset)
{
	int iRet = 0 ;

	//以pt_3d_vert为中点创造1根水平和1根垂直线，判断和此2线相交的实体(排除pPolyline）
	AcGePoint3d pt_3d_start, pt_3d_end ;
	for (int i=0; i<2; i++)
	{
		if (i==0)
		{
			pt_3d_start.x = pt_3d_base.x-dOffset/2 ;
			pt_3d_start.y = pt_3d_base.y ;
			pt_3d_end.x = pt_3d_base.x+dOffset/2 ;
			pt_3d_end.y = pt_3d_base.y ;
		}
		else 
		{
			pt_3d_start.x = pt_3d_base.x ;
			pt_3d_start.y = pt_3d_base.y-dOffset/2 ;
			pt_3d_end.x = pt_3d_base.x ;
			pt_3d_end.y = pt_3d_base.y+dOffset/2 ;
		}
		AcDbLine* pLine = new AcDbLine(pt_3d_start, pt_3d_end) ;

		AcDbBlockTable * pBlkTbl = NULL ;
		m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
		pBlkTblRec->newIterator( pBlkTblRecIter );
		pBlkTblRec->close();

		AcDbEntity * pEnt = NULL ;
		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
		{
			if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
			{
				AcGePoint3dArray pt3dArray ;

				if (Acad::eOk== pLine->intersectWith(pEnt, AcDb::kOnBothOperands, pt3dArray) )
				{
					if (pt3dArray.length()>0)
					{
						long nHandleCur = GetHandle(pEnt) ;
						if(CBlkUtility::FindLongInArray(nArrayHandleFind, nHandleCur)==-1)
						{
							nArrayHandleFind.Add(nHandleCur) ;								
							acutPrintf(_T("\n%d:%s-%d"), i, pEnt->isA()->name(), nHandleCur) ;
							iRet++ ;
						}
					}
				}	

				pEnt->close() ;
			}	
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;
		pLine->close() ;
		delete pLine ;
	}

	return iRet ;
}

// 设置实体的显示、隐藏
int CUserSystem::setVisibility(AcDbEntity* pEnt, AcDb::Visibility newVal)
{
	Adesk::Boolean bIsCanW = pEnt->isWriteEnabled() ;
	if (!bIsCanW)
	{
		pEnt->upgradeOpen() ;
	}
	pEnt->setVisibility(newVal);
	if (!bIsCanW)
	{
		pEnt->downgradeOpen() ;
	}
	
	return 0;
}

// 设置实体的显示、隐藏
int CUserSystem::setVisibility(long nHandle, AcDb::Visibility newVal)
{
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt))
	{
		this->setVisibility(pEnt, newVal) ;
		pEnt->close() ;
	}

	return 0;
}

// 设置实体的显示、隐藏
int CUserSystem::setVisibility(ads_name en, AcDb::Visibility newVal)
{
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(en, pEnt))
	{
		this->setVisibility(pEnt, newVal) ;
		pEnt->close() ;
	}

	return 0;
}

// 绘制连接点（多段线）
AcDbObjectId CUserSystem::DrawConNode(AcGePoint3d pt_3d_cen, double dRadius)
{
	AcDbObjectId ojbId ;
	AcGePoint3d pt_3d_vertex[2] = {pt_3d_cen, pt_3d_cen} ;
	pt_3d_vertex[0].x-=dRadius/2 ;
	pt_3d_vertex[1].x+=dRadius/2 ;
	ojbId = this->AddAcDbPolyLine(pt_3d_vertex[0], pt_3d_vertex[1], dRadius, PI,  256, TRUE) ;
	return ojbId;
}

int CUserSystem::SetTextString(long nHandle, const CString& strText)
{
	AcDbEntity* pEntText = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEntText, AcDb::kForRead))
	{
		if (pEntText->isKindOf(AcDbText::desc()))
		{
			AcDbText * pText = AcDbText::cast(pEntText) ;
			pEntText->upgradeOpen() ;
			pText->setTextString(strText) ;
			pEntText->downgradeOpen() ;
		}
		pEntText->close() ;
	}

	return 0;
}

// 实体是否在关闭的off的层上
bool CUserSystem::IsOnOffLayer(AcDbEntity* pEnt)
{
	AcDbObject* pObj = NULL ;
	AcDbLayerTableRecord* pLayerTbRec = NULL ;
	bool bIsOff = false ;
	if (acdbOpenAcDbObject(pObj, pEnt->layerId(), AcDb::kForRead)==Acad::eOk)
	{
		pLayerTbRec = AcDbLayerTableRecord::cast(pObj) ;
		bIsOff = pLayerTbRec->isOff() ;
		pObj->close() ;
	}
	return bIsOff;
}

// 设置多段线宽
int CUserSystem::SetPLineWidth(long nHandle, double dWidth)
{
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline * pPline = AcDbPolyline::cast(pEnt) ;
			pEnt->upgradeOpen() ;
			int iNumVert = pPline->numVerts() ;
			for (int i=0; i<iNumVert; i++)
			{
				pPline->setWidthsAt(i, dWidth, dWidth) ;
			}
			pEnt->downgradeOpen() ;
		}
		pEnt->close() ;
	}
	return 0;
}

// 取得指定handle的块或文本的插入点
BOOL CUserSystem::GetEntPosition(long nHandle, AcGePoint3d& pt_3d_base)
{
	BOOL bRet = FALSE ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbText::desc()))
		{
			AcDbText * pText = AcDbText::cast(pEnt) ;
			pt_3d_base = pText->position() ;
			bRet = TRUE ;
		}
		else if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
			pt_3d_base = pRef->position() ;
			bRet = TRUE ;
		}
		pEnt->close() ;
	}
	return 0;
}

// 设置多段线宽
int CUserSystem::GetPLineStartWidth(long nHandle, double &dWidth)
{
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline * pPline = AcDbPolyline::cast(pEnt) ;
			pEnt->upgradeOpen() ;
			int iNumVert = pPline->numVerts() ;
			for (int i=0; i<iNumVert; i++)
			{
				//pPline->setWidthsAt(i, dWidth, dWidth) ;
				pPline->getWidthsAt(0,dWidth,dWidth);
			}
			pEnt->downgradeOpen() ;
		}
		pEnt->close() ;
	}
	return 0;
}
