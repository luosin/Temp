// TextUtil.h: interface for the CTextUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTUTIL_H__F0E02D5A_2083_4575_8BFB_F9C7716F9226__INCLUDED_)
#define AFX_TEXTUTIL_H__F0E02D5A_2083_4575_8BFB_F9C7716F9226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTextUtil  
{
public:
	CTextUtil();
	virtual ~CTextUtil();

	// 创建单行文字
	static AcDbObjectId AddText(const AcGePoint3d& ptInsert, 
		const TCHAR* text, double widthfactor =1,AcDbObjectId style = AcDbObjectId::kNull, 
		double height = 2.5, double rotation = 0);

	// 创建多行文字
	static AcDbObjectId AddMText(const AcGePoint3d& ptInsert, 
		const TCHAR* text, double widthfactor=0.5,AcDbObjectId style = AcDbObjectId::kNull, 
		double height = 2.5, double width = 10);
	static void WriteText(AcGePoint3d base_Pt, CString cstrText, double dAngle_PI, int iAlignMode, double dFontSize) /*iAlignMode为文本的对齐模式?*/;
	static void WriteText(AcGePoint3d base_Pt, CString cstrText, double dAngle_PI , AcDb::TextHorzMode iHorMode, double dTextHeight =2.5,double dTextWidth =10 );
	static void WriteText(AcGePoint3d base_Pt, CString cstrText, double dAngle_PI , AcDb::TextHorzMode iHorMode, double dTextHeight ,double dTextWidth ,AcCmColor color);
	static void WriteText(AcGePoint3d base_Pt, CString cstrText, AcDbObjectId &objId, double dHeight, double dWidthFactor, AcDb::TextHorzMode iHorMode, AcDb::TextVertMode iVerMode, int iColorIndex, AcDbObjectId textStyle, double dAngle );
	static void M_WriteText(AcGePoint3d base_Pt, AcGePoint3d AlignmentPoint_3d, CString cstrText, double dAngle_PI , int iHaveVertOrHorzFlag, AcDb::TextVertMode iVerMode, AcDb::TextHorzMode iHorMode, AcDbObjectId textStyle, double dTextHeight =2.5,double dTextWidth =10, int iColorIndex =4);
	static void WriteMText(AcGePoint3d base_Pt, CString cstrText, AcDbObjectId textStyleId, AcDbMText::AttachmentPoint iAttachment, double dTextHeight=2.5,double dTextWidth =10, int icolor =5 );
	static	AcDbObjectId AddText_Center( const AcGePoint3d& ptInsert, const AcGePoint3d& ptAlignpoint,const TCHAR* text, AcDbObjectId style = AcDbObjectId::kNull, 
		double height = 2.5, double width = 10 );

};

#endif // !defined(AFX_TEXTUTIL_H__F0E02D5A_2083_4575_8BFB_F9C7716F9226__INCLUDED_)
