// TextUtil.cpp: implementation of the CTextUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextUtil.h"
#include <dbents.h>
#include "DwgDatabaseUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextUtil::CTextUtil()
{

}

CTextUtil::~CTextUtil()
{

}

AcDbObjectId CTextUtil::AddText( const AcGePoint3d& ptInsert, const TCHAR* text, double width,
								AcDbObjectId style, double height, double rotation )
{
// 	AcDbText *pText = new AcDbText(ptInsert, text, style, height, rotation);
// 	//pText->setHorizontalMode(AcDb::kTextMid);
// 	return CDwgDatabaseUtil::PostToModelSpace(pText);
	AcDbText *pText = new AcDbText();
	//(ptInsert, text, style, height, rotation);
	//pText->setHorizontalMode(AcDb::kTextMid);
	pText->setTextString(text);
	pText->setPosition(ptInsert);
	pText->setTextStyle(style);
	pText->setWidthFactor(width);
	pText->setHeight(height);
	pText->setRotation(rotation);
	pText->setVerticalMode(AcDb::kTextVertMid) ;
	pText->setHorizontalMode(AcDb::kTextCenter) ;
	pText->setHorizontalMode(AcDb::kTextCenter);
	pText->setAlignmentPoint(ptInsert);

	return CDwgDatabaseUtil::PostToModelSpace(pText);
}
AcDbObjectId CTextUtil::AddText_Center( const AcGePoint3d& ptInsert, const AcGePoint3d& ptAlignpoint,const TCHAR* text, 
								AcDbObjectId style, double height, double rotation )
{
	AcDbText *pText = new AcDbText();
		//(ptInsert, text, style, height, rotation);
	//pText->setHorizontalMode(AcDb::kTextMid);
	pText->setTextString(text);
	pText->setPosition(ptInsert);
	pText->setVerticalMode(AcDb::kTextVertMid) ;
	pText->setHorizontalMode(AcDb::kTextCenter) ;
    pText->setHorizontalMode(AcDb::kTextCenter);
    pText->setAlignmentPoint(ptAlignpoint);
	
	return CDwgDatabaseUtil::PostToModelSpace(pText);
}

AcDbObjectId CTextUtil::AddMText( const AcGePoint3d& ptInsert, const TCHAR* text, double widthfactor,
								 AcDbObjectId style, double height,
								 double width )
{
	AcDbMText *pMText = new AcDbMText();
	
	// 设置多行文字的特性
	pMText->setTextStyle(style);
	pMText->setContents(text);
	//设置插入位置
	pMText->setLocation(ptInsert);
	//设置字体高度
	pMText->setTextHeight(height);
	//设置行距，建议0.8
	pMText->setLineSpacingFactor(widthfactor);
	//设置行宽
	pMText->setWidth(width);
	//pMText->setAttachment(AcDbMText::kBottomLeft);
	pMText->setAttachment(AcDbMText::kMiddleCenter);
	
	return CDwgDatabaseUtil::PostToModelSpace(pMText);
}
void CTextUtil::WriteText(AcGePoint3d base_Pt, CString cstrText, double dAngle_PI, int iAlignMode, double dFontSize)
//iAlignMode为文本的对齐模式；
{	
	//转换坐标：
	AcGePoint3d base_3d =base_Pt;
	AcDbDatabase * pDb;	
	pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText * pText= new AcDbText;
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
			pText->setTextString(cstrText);
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
			pText->setWidthFactor( dFontSize ) ;
			AcCmColor color ;
			color.setColorIndex(2) ;
			pText->setColor ( color ) ;
			pBlkTblRec->appendAcDbEntity(text_Id,pText);
			pBlkTblRec->close();
			pText->close();
			
		}
		pBlkTbl->close();
	}
}

//水平或者垂直方向有无对齐方式标志 ;
// Vert  Horz  iHaveVertOrHorzFlag
//  0     0        0
//  0     1        1
//  1     0        2
//  1     1        3
//
void CTextUtil::M_WriteText(AcGePoint3d base_Pt, AcGePoint3d AlignmentPoint_3d, CString cstrText, double dAngle_PI , int iHaveVertOrHorzFlag, AcDb::TextVertMode iVerMode, AcDb::TextHorzMode iHorMode, AcDbObjectId textStyle, double dTextHeight,double dTextWidth, int iColorIndex )
{
	//转换坐标：
	AcGePoint3d base_3d =base_Pt;
	//ads_point_To_AcGe3d( base_Pt , base_3d ) ;
	for( int i = 0 ; i < 3 ; i++ )
	{
		base_3d[i] =AlignmentPoint_3d[i] ;
	}
	AcDbDatabase * pDb;	
	pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
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
			pText->setTextString(cstrText);
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
			 	iColorIndex =2 ;
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
void CTextUtil::WriteText(AcGePoint3d base_Pt, CString cstrText, double dAngle_PI ,  AcDb::TextHorzMode iHorMode, double dTextHeight,double dTextWidth )
{
	//转换坐标：
	AcGePoint3d base_3d =base_Pt;
	//ads_point_To_AcGe3d( base_Pt , base_3d ) ;
	AcDbDatabase * pDb;	
	pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText * pText= new AcDbText ;	
			pText->setPosition(base_3d);	
	    	pText->setHorizontalMode(iHorMode) ;
			pText->setAlignmentPoint(base_3d);
			pText->setTextString(cstrText);
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



void CTextUtil::WriteText(AcGePoint3d base_Pt, CString cstrText, double dAngle_PI ,  AcDb::TextHorzMode iHorMode, double dTextHeight,double dTextWidth,AcCmColor color)
{
	//转换坐标：
	AcGePoint3d base_3d =base_Pt;
	AcDbDatabase * pDb;	
	pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec;
		if( Acad::eOk == pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec,AcDb::kForWrite) )
		{
			AcDbText * pText= new AcDbText ;
			
			pText->setPosition(base_3d);
			
			pText->setHorizontalMode(iHorMode) ;
			pText->setAlignmentPoint(base_3d);
			pText->setTextString(cstrText);
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

//
void CTextUtil::WriteText(AcGePoint3d base_Pt, CString cstrText, AcDbObjectId &objId, double dHeight, double dWidthFactor, AcDb::TextHorzMode iHorMode, AcDb::TextVertMode iVerMode, int iColorIndex, AcDbObjectId textStyle, double dAngle ) 
{
	AcGePoint3d base_3d = base_Pt ;
	//ads_point_To_AcGe3d(base_Pt, base_3d) ;
	AcDbDatabase * pDb;	
	pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable * pBlkTbl = NULL ;
	if (Acad::eOk==pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
	{
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		if (Acad::eOk==pBlkTbl->getAt(ACDB_MODEL_SPACE,pBlkTblRec, AcDb::kForWrite))
		{
			AcDbText * pText = new	AcDbText ;
			pText->setTextString(cstrText) ;
			
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
			pBlkTblRec->appendAcDbEntity(objId, pText) ;
			pBlkTblRec->close() ;
			pText->close() ;
			
		}
		pBlkTbl->close() ;
	}
}

void CTextUtil::WriteMText(AcGePoint3d base_Pt, CString cstrText, AcDbObjectId textStyleId, AcDbMText::AttachmentPoint iAttachment, double dTextHeight,double dTextWidth, int icolor ) 
{
	//转换坐标：
	AcGePoint3d base_3d =base_Pt;
	//ads_point_To_AcGe3d( base_Pt , base_3d ) ;
	AcDbDatabase * pDb;	
	pDb=acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable * pBlkTbl;
	if( Acad::eOk == pDb->getBlockTable(pBlkTbl,AcDb::kForRead) )
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
			pMText->setContents(cstrText) ;		
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
