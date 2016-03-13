// CUserSystem.h: interface for the CSystem class.
//
//AUTHOR :周海锋
//EMAIL:zhfeng@crscd.com.cn blksun@sina.com

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEM_H__1BBA4DD8_9883_4F9E_A32D_48744EF6E037__INCLUDED_)
#define AFX_SYSTEM_H__1BBA4DD8_9883_4F9E_A32D_48744EF6E037__INCLUDED_

#ifdef ZHFDLL_EXPORT
#define ZHFDLL __declspec(dllexport)
#else
#define ZHFDLL __declspec(dllimport)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "relay.h"
//#include "aced.h"
//#include <adslib.h>
#include <afxtempl.h>
#include <dbents.h>
#include <dbpl.h>
#include "ViewExtents.h"

typedef CArray<long, long> CLongArray;

// struct ZhfAcDb
// {
// 	enum TextMode          
// 	{ 
// 		kDTextLeft            = 0, //左对齐
// 		kDTextCenter          = 1, //
// 		kDTextRight           = 2,
// 		kDTextAligned         = 3,
// 		kDTextMiddle		  = 4,
// 		kDTextFit			  = 5,
// 		kDTextTopLeft		  = 6,
// 		kDTextTopCenter	      = 7,
// 		kDTextTopRight		  = 8,
// 		kDTextMiddleLeft	  = 9,
// 		kDTextMiddleCenter	  = 10,
// 		kDTextMiddleRight	  = 11,
// 		kDTextBottomLeft	  = 12,
// 		kDTextBottomCenter	  = 13,
// 		kDTextBottomRight	  = 14
// 
// 	};
// };


class ZHFDLL CUserSystem  
{
public:
	CUserSystem();
	virtual ~CUserSystem();

public:
	//int m_iColorIndex  ;
	double m_dFontScale;
	double m_dFontSize;
	AcDbDatabase * m_pDb;	

public:
	CString m_strBasePath;

	int RegisterAppName(const TCHAR *appname) ;
	
	//_WP方式选择区域ent
	int SelectEntByWP(const AcGePoint3dArray& ptArray, const resbuf* rbFilter, ads_name &ss_name) ;
	resbuf* ptArrayToResbuf(const AcGePoint3dArray& ptArray) ;
	void CopyAdsPt(ads_point ptDis, const ads_point ptSrc) ;

	//替换一个实体块
	BOOL ReplaceBlock(AcDbBlockReference *pBlkRef, const CMapStringToString *pMapAtt, const CString *strNewBlkName, BOOL bWithAtt=TRUE) ;

	//炸开图块
	int Explode(AcDbEntity * pEnt) ;

	//取得块的名称
	BOOL GetBlockName(const AcDbBlockReference * pRef, CString &strBlockName) ;
	BOOL GetBlockName(long nHandle, CString &strBlockName) ;

	//返回点到直线的距离
	double GetDistance(const AcGeLineSeg2d linSeg_2d, const AcGePoint2d pt) ;
	double GetDistance(const AcGeLineSeg3d linSeg_3d, const AcGePoint3d pt) ;
	double GetDistance(const AcGeLine2d lin_2d, const AcGePoint2d pt);
	double GetDistance(const AcGeLine3d lin_3d, const AcGePoint3d pt);
	double GetDistance(const AcGePoint3d pt1, const AcGePoint3d pt2, const AcGePoint3d pt);
	double GetDistance(const AcGePoint2d pt1, const AcGePoint2d pt2, const AcGePoint2d pt);

	//查找区域内的块
	int FindBlkInArea(const AcGePoint3d &pt_topleft, const AcGePoint3d &pt_bottomright, CLongArray &nArrayBlkHandle) ;
	//查找名为strBlkName的块
	int FindBlk(const CString &strBlkName, CLongArray &nArrayBlkHandle) ;
	//int FindBlk(const CString &strBlkName, CArray<AcDbBlockReference *, AcDbBlockReference *> &arrayBlk) ;

	//查找区域内的文本
	int FindTxtInArea(const AcGePoint3d &pt_topleft, const AcGePoint3d &pt_bottomright, CStringArray &strArrayFound) ;
	int FindTxtInArea(const AcGePoint3d &pt_topleft, const AcGePoint3d &pt_bottomright, CArray<AcDbText*, AcDbText *> &arrayText) ;
	//销毁FindTxtInArea函数中arrayText参数中保存的堆变量
	void DestroyEntInArray(CArray<AcDbText *, AcDbText *> &arrayObj) ;
	void DelSelXdata();

	//查找替换文本
	//iFindEntType=0,单行文本:AcDbText
	//iFindEntType=1,多行文本:AcDbMtext
	//iFindEntType=2,属性文本:AcDbAttributeDefinition
	//iFindEntType=3,块中文本:AcDbBlockReference
	//iRange=0,选择替换
	//iRange=1,全图替换
	//iRange=2,文件夹指定
	int FindAndReplaceText(CString strFindText, CString strReplaceText, BOOL bIsAllFit=TRUE, BOOL bCaseSensitive=FALSE, BOOL bReg=FALSE, int iRange=0, int iColorIndex=-1, int iFindEntType=0);
	int ReplaceEntText(const AcDbEntity *pEnt, CString strFindText, CString strReplaceText, BOOL bIsAllFit=TRUE, BOOL bCaseSensitive=FALSE, BOOL bReg=FALSE, int iColorIndex=-1, int iFindEntType=0);

	//转换文本的对齐模式，但不移动文本
	BOOL ConvertTextMode(AcDbText *&pText, int iTextModeNew = AcDb::kTextMid, int iColorIndex=-1);

	//文本对齐方式表示互转
	BOOL ConvertTextModeToIndex(AcDb::TextHorzMode iHorMode, AcDb::TextVertMode iVerMode, int &iIndex) ;
	BOOL ConvertIndexToTextMode(int iIndex, AcDb::TextHorzMode &iHorMode, AcDb::TextVertMode &iVerMode) ;

	//取得直线长度
	double GetLineLen(const AcDbLine *pLine) ;
	//取得多段线长度
	double GetLineLen(const AcDbPolyline *pPLine) ;

	//判断一个点是否在一条直线上
	BOOL IsPointInLine(const AcGePoint3d &pt_3d, const AcDbLine *pLine) ;
	//判断一个点是否在提供的直线数组中的某一条直线上
	BOOL IsPointInAnyLine(const AcGePoint3d &pt_3d, const CArray<AcDbLine *, AcDbLine *> &arrayLine) ;
	//查询点pt_2d_on是否在多段线pPolyLine上，不在，返回结果-1。若在，返回值<1000，表示此点在某个顶点；返回值=1000，表示此点不在某个顶点。nVertexIndex返回查询点在第几段（base 0）,dTol允许误差
	int IsPointOnPolyLine(const AcDbPolyline  *pPolyLine, const AcGePoint2d &pt_2d_on, UINT &nVertexIndex, double dTol=0.001) ;
	int IsPointOnPolyLine(const AcDbPolyline  *pPolyLine, const AcGePoint3d &pt_3d_on, UINT &nVertexIndex, double dTol=0.001) ;
	//从线数组中将竖线的X坐标，横线的Y坐标读取出来，并且不要重复，从小到大排序x，从大到小排序y
	void GetXY(CArray<AcDbLine *, AcDbLine *> &arrayLine, CArray<double, double> &arrayX, CArray<double, double> &arrayY) ;
	//从块中读出实体ID
	void GetAllEntIdFromBlock(AcDbBlockReference *pBlockRef, AcGePoint3d insertP,CArray<AcDbLine*, AcDbLine *> &arrayLine, CArray<AcDbText*, AcDbText *> &arrayText) ;
	void GetAllEntIdFromBlock(AcDbBlockReference *pBlockRef, AcGePoint3d insertP, AcDbObjectIdArray & objIdArray);

	//对给定的文本实体数组进行排序，排序规则是：
	//先按照x坐标从左往右，相同的x坐标，y越小，越后排
	int SortText(CArray<AcDbText *, AcDbText *> &arrayent);
	//改写sortent,iMode=0(xy),1(x),2(y)
	int SortEnt(CArray<AcDbText *, AcDbText *> &arrayent, int iMode=0) ;
	int SortEnt(CArray<AcDbBlockReference *, AcDbBlockReference *> &arrayBlk, int iMode=0) ;
	//改写sortent,iMode=0(xy),1(x),2(y), iEntType默认为0：表示排序的是块，1 文本，其他再添加
	int SortEnt(CLongArray &nArrayHandle, int iMode=0, int iEntType=0) ;
	//得到排序后的文本
	int GetHaveSortedText(const CArray<AcDbText *, AcDbText *> &arrayent, CStringArray & strArrayText) ;

	//将excel里面的颜色转换为cad里面的颜色
	static int ConvExcelCorlorToDwg(int iColorIndex) ;

	BOOL ExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt, double dHeightRate=0.328, double dWidthRate=2.41);
   
	BOOL GetMTextBasePt(AcDbMText::AttachmentPoint iAttachment, ads_point base_pt, double dWidth, double dHight, ads_point &real_pt) ;
	BOOL ChangeExcelAlignToMText(long lVerAlign, long lHorAlign, AcDbMText::AttachmentPoint &iAttachment) ; 

	//AcDbObjectId createTextStyle(CString fontName,CString bigFontName,CString textStyleName);

	AcDbObjectId AddEntityToDb(AcDbEntity * pEnt);
	void ConvertMTexttoTextAll();
	//
// 	void ConvertAlignToInt(AcDb::TextVertMode iVerMode, AcDb::TextHorzMode iHorMode, int &iResult);

	//转换所有的属性文本为普通文本
	void ConvertAttrtoTextAll();

	//转换point类型
	void ConvertPtAdsTo3D(ads_point point_Pt, AcGePoint3d & point_3d);
	void ConvertPt3DToAds(const AcGePoint3d pt_3d_src, ads_point &pt_dist) ;
	void ConvertPoint(const AcGePoint3d &pt_3d_src, AcGePoint2d &pt_2d_to) ;
	void ConvertPoint(const AcGePoint2d &pt_2d_src, AcGePoint3d &pt_3d_to) ;

	//打开实体
	BOOL OpenAcDbEntity(const AcDbObjectId objId, AcDbEntity *& pEnt, AcDb::OpenMode iMode=AcDb::kForWrite);
	BOOL OpenAcDbEntity(ads_name ent_name, AcDbEntity *& pEnt, AcDb::OpenMode iMode=AcDb::kForWrite);
	BOOL OpenAcDbEntity(const UINT iHandle, AcDbEntity *& pEnt, AcDb::OpenMode iMode=AcDb::kForWrite);
	//选择实体
	int SelectAcDbEnt(const TCHAR *chPrompt, const TCHAR *chEntType, ads_name &ent_name) ;
	//清除指定范围的实体
	void ClearEntity(double dStartX, double dStartY, double dEndX, double dEndY);

	//图层操作
	AcDbObjectId Set_Layer(CString  strStatus , CString strType);
	AcDbObjectId SetCLayer(const CString&  strLayer , int iColorIndex=7);
	BOOL DeleteLayer(CString strLayerName);
	BOOL SetLayerStatus(CString strLayerName, bool bIsOff=true, bool bIsFrozen=false, bool bIsLocked=false, int iColorIndex=7);
	// 设置实体到指定的层
	int SetLayer(long nHandle, const CString& strLayer);
	// 创建图层
	AcDbObjectId CreateLayer(const CString& strLayer, int iColorIndex=7);

	//style操作
	AcDbObjectId SetFontStyle( CString strStyleName,CString strFont_Name);
	AcDbObjectId SetFontStyle();
	AcDbObjectId CreateTextStyle(CString strStyleName,CString strFontName,CString strBigFontFileName,double dSize,double dxScale) ;
	AcDbObjectId CreateTextStyleByFileName(CString strStyleName,CString strFileName,CString strBigFontFileName,double dSize,double dxScale);
	BOOL GetTextStyleArray(CStringArray &strArrayTextStyle) ;
	// 是否存在以strTextStyleName为名称的字体Style
	BOOL HasTextStyle(const TCHAR * strTextStyleName, AcDbObjectId & objId);
	//通过id取得style的name
	BOOL GetTextStyle(AcDbObjectId objId, CString & strTextStyleName) ;
	//取得text实体的style的name
	BOOL GetTextStyle(const AcDbText *pText, CString & strTextStyleName) ;
	//
	Acad::ErrorStatus ModifyFontStyle(CString strStyleName, CString strFontName, BOOL bUseBigFont=TRUE) ;

	//添加扩展数据 int 和 long 都一样，都是kDxfXdInteger32
	BOOL SetXdata(AcDbEntity * pEnt , const TCHAR *chAppName, const TCHAR *chVar);
	BOOL SetXdata(AcDbEntity * pEnt, const TCHAR *chAppName, int iVal);
 	BOOL SetXdata(AcDbEntity * pEnt, const TCHAR *chAppName, long lVal);
	BOOL SetXdata(AcDbEntity * pEnt, const TCHAR *chAppName, double dVal);
	BOOL SetXdata(AcDbObjectId objId, const TCHAR *chAppName, const TCHAR *chVar);
	BOOL SetXdata(AcDbObjectId objId, const TCHAR *chAppName, int iVal);
 	BOOL SetXdata(long nHandle, const TCHAR *chAppName, long lVal);
	BOOL SetXdata(long nHandle, const TCHAR *chAppName, const TCHAR *chVar);
	//删除扩展数据
	BOOL DeleteXdata(AcDbEntity * pEnt, const TCHAR *chAppName );
	void DelAllXdata() ;

	//获取扩展数据
// 	BOOL GetXdata(AcDbEntity * pEnt, const TCHAR *chAppName, short & siVal);
	BOOL GetXdata(AcDbEntity * pEnt, const TCHAR *chAppName, int & iVal);
 	BOOL GetXdata(AcDbEntity * pEnt, const TCHAR *chAppName, long & lVal);
	BOOL GetXdata(AcDbEntity * pEnt, const TCHAR *chAppName,double & dVal);
	BOOL GetXdata(AcDbEntity * pEnt , const TCHAR *chAppName, CString strFieldName ,CString & strVal);
	BOOL GetXdata(AcDbEntity * pEnt , const TCHAR *chAppName, CString & strVal);

	BOOL GetXdata(long nHandle, const TCHAR *chAppName, long & lVal);
	BOOL GetXdata(long nHandle, const TCHAR *chAppName, CString & strVal);
	BOOL GetXdata(long nHandle, const TCHAR *chAppName,double & dVal);

	//取得多义线的节点
	void collectVertices(const AcDb2dPolyline * pline, AcGePoint3dArray & pts, bool asWcsPts) ;
	void collectVertices(const AcDbPolyline * pline, AcGePoint3dArray & pts) ;

	//图块操作
	BOOL HaveBlockName( CString strBlockName , AcDbObjectId & Block_Id ) ;		
	BOOL Verify_FileName( const TCHAR * chFileName , BOOL bPropt ) ;	
	//nMode=1:strBlockName是fullpath,nMode=0:strBlockName是块名
	BOOL InsertBlock(AcDbBlockReference *& pRef , CString strBlockName  , AcGePoint3d pt_3d_insert , CStringArray * pStrArrayVal , BOOL bUniformFont, int nMode=0) ;
	//BOOL InsertBlock(AcDbBlockReference *& pRef , CString strBlockName  , AcGePoint3d pt_3d_insert ) ;		
	//插入块,不指定属性,采用默认值,本函数主要用来替换块时批量插入;nByBlkOrPath=1:strBlockName是fullpath,nByBlkOrPath=0:strBlockName是块名
	BOOL InsertBlock(AcDbBlockReference *& pRef , CString strBlockName  , AcGePoint3d pt_3d_insert , BOOL bUniformFont=FALSE, int nByBlkOrPath=0, BOOL bWithAtt=FALSE);
	//nMode:default=0,提供块名比如pmt_XXX,nMode=1,提供的是块所在dwg路径
	BOOL InsertDwgFile(CString strBlockName , AcDbObjectId  &Block_Id, int nMode=0) ;	
	
// 	int FindReplaceBlock(CString strOldBlockName, CString strNewBlockFile, int nByBlockName, BOOL bIsGlobal=TRUE) ;

	//查找属性
	BOOL FindAtt(AcDbBlockReference * pRef ,const TCHAR * chTag , AcDbAttribute * & pAtt );	
	//获取指定图块 指定属性的值；
	BOOL GetAttValue(AcDbBlockReference* pRef , const TCHAR * chTag , TCHAR * chVal ); //获取某个属性的值；
	BOOL GetAttValue(AcDbBlockReference* pRef , const TCHAR * chTag , CString & strVal ); //获取某个属性的值；
	BOOL GetAttValue(AcDbBlockReference* pRef , const TCHAR * chTag , int  & iVal ); //获取某个属性的值；
	BOOL GetAttValue(AcDbBlockReference* pRef , const TCHAR * chTag , double  & dVal ); //获取某个属性的值；	
	BOOL GetAttValue(long nHandle, const TCHAR * chTag , TCHAR * chVal ); //获取某个属性的值；
	BOOL GetAttValue(long nHandle , const TCHAR * chTag , CString & strVal ); //获取某个属性的值；
	BOOL GetAttValue(long nHandle , const TCHAR * chTag , int  & iVal ); //获取某个属性的值；
	BOOL GetAttValue(long nHandle , const TCHAR * chTag , double  & dVal ); //获取某个属性的值；	

	//设置指定图块 指定属性的值；
	BOOL SetAttValue(AcDbBlockReference * pRef, CString strTag, double fValue, BOOL bInVisible=TRUE ) ;
	BOOL SetAttValue(AcDbBlockReference * pRef, CString strTag, CString strValue, BOOL bInVisible=TRUE) ;
	BOOL SetAttColor(AcDbBlockReference * pRef, CString strTag, Adesk::UInt16 nColorIndex=7) ;
	
	//增加属性,iAlignMode:0~14,对应CAD中的对齐下拉框
	void AddAttValue(AcDbBlockReference * pRef ,const TCHAR * chTag, const TCHAR * chVal,
		AcGePoint3d pt_3d_att=AcGePoint3d(0,0,0),
		int iAlign=0,
		BOOL bInVisible=TRUE,		
		double dHeight=2 ,
		double dWidthFactor=0.7,
		double dAngle=0.0,
		Adesk::UInt16 nColorIndex=7,
		AcDbObjectId objIdTextStyle=NULL);

	//绘制AcDbLine
	AcDbObjectId AddAcDbLine(AcGePoint3d pt_3d_start, AcGePoint3d pt_3d_end, int iColor=7);
	AcDbObjectId AddAcDbLine(ads_point pt_start, ads_point pt_end, int iColor=7);

	//绘制AcDb2dPolyline
	void AddAcDb2dPolyline(AcGePoint3dArray PlinePointArray, double dWidth, int iLine_Type);
	void AddAcDb2dPolyline(AcGePoint3d start_3d, AcGePoint3d end_3d, double dWidth , int iLine_Type=0, int iColor=7 );	

	//绘制多段线AcDbPolyline
	AcDbObjectId AddAcDbPolyLine(const AcGePoint2dArray &ptArray,  double dWidth, int iColor=7, BOOL bIsClosed=FALSE);
	AcDbObjectId AddAcDbPolyLine(const AcGePoint3dArray &ptArray,  double dWidth, int iColor=7, BOOL bIsClosed=FALSE);
	AcDbObjectId AddAcDbPolyLine(const AcGePoint2dArray &ptArray,  const AcArray<double>& dWidths, int iColor=7, BOOL bIsClosed=FALSE);
	//AcDbObjectId AddAcDbPolyLine(AcGePoint3d start_3d, AcGePoint3d end_3d, double dWidth, int iColor=7, BOOL bIsClosed=FALSE);
	//添加多段线绘制的圆弧
	//根据起点、圆弧角、半径、弦方向绘制圆弧。dAngle 圆弧弧度, dDirectOfChord,弦的方向角度
	AcDbObjectId AddAcDbPolyLine(AcGePoint3d pt_3d_start, double dAngle, double dRadius, double dDirectOfChord, double dWidth=0, int iColor=7, BOOL bIsClosed=TRUE);
	//根据起点、圆弧角、圆心绘制圆弧。dAngle 圆弧弧度, pt_3d_center 圆心
	AcDbObjectId AddAcDbPolyLine(AcGePoint3d pt_3d_start, double dAngle, AcGePoint3d pt_3d_center, double dWidth=0, int iColor=7, BOOL bIsClosed=TRUE);
	//根据起点、终点、圆弧角绘制圆弧。dAngle 圆弧弧度,=0时为直线
	AcDbObjectId AddAcDbPolyLine(AcGePoint3d pt_3d_start, AcGePoint3d pt_3d_second, double dWidth=0, double dAngle=0,  int iColor=7, BOOL bIsClosed=FALSE);

	//添加Circle
	AcDbObjectId AddAcDbCircle(AcGePoint3d pt_3d_center, double  dRadius, int iColorIndex=7);

	//写扩展记录
	void WriteToRec(CString strDitName,CString strRecName,int  strVal);
	void WriteToRec(CString strDitName,CString strRecName,CString  strVal);
	//读扩展记录
	bool ReadFromRec(CString strDitName,CString strRecName,int  & iVal) ;
	bool ReadFromRec(CString strDitName,CString strRecName,CString  & strVal) ;

	//绘制文本
	void AddText(AcDbObjectId &objIdText, const TCHAR *chText, AcGePoint3d & pt_3d_position, AcGePoint3d & pt_3d_align, AcDb::TextHorzMode textHMode=AcDb::kTextLeft, AcDb::TextVertMode textVMode=AcDb::kTextBase, double dHeight=2, double dWidthFactor=0.8, double dAngle=0,  Adesk::UInt16 nColorIndex=7, AcDbObjectId objIdTextStyle=NULL) ;
	void AddText(const TCHAR *chText, AcGePoint3d & pt_3d_position, AcGePoint3d & pt_3d_align, AcDb::TextHorzMode textHMode=AcDb::kTextLeft, AcDb::TextVertMode textVMode=AcDb::kTextBase, double dHeight=2, double dWidthFactor=0.8, double dAngle=0,  Adesk::UInt16 nColorIndex=7, AcDbObjectId objIdTextStyle=NULL) ;

	void AddText(AcDbObjectId &objIdText, const TCHAR *chText, AcGePoint2d & pt_2d_position, AcGePoint2d & pt_2d_align, AcDb::TextHorzMode textHMode=AcDb::kTextLeft, AcDb::TextVertMode textVMode=AcDb::kTextBase, double dHeight=2, double dWidthFactor=0.8, double dAngle=0,  Adesk::UInt16 nColorIndex=7, AcDbObjectId objIdTextStyle=NULL) ;
	void AddText(const TCHAR *chText, AcGePoint2d & pt_2d_position, AcGePoint2d & pt_2d_align, AcDb::TextHorzMode textHMode=AcDb::kTextLeft, AcDb::TextVertMode textVMode=AcDb::kTextBase, double dHeight=2, double dWidthFactor=0.8, double dAngle=0,  Adesk::UInt16 nColorIndex=7, AcDbObjectId objIdTextStyle=NULL) ;

	void AddText(AcDbObjectId &objIdText, const TCHAR *chText, ads_point & pt_position, ads_point & pt_align, AcDb::TextHorzMode textHMode=AcDb::kTextLeft, AcDb::TextVertMode textVMode=AcDb::kTextBase, double dHeight=2, double dWidthFactor=0.8, double dAngle=0,  Adesk::UInt16 nColorIndex=7, AcDbObjectId objIdTextStyle=NULL) ;
	void AddText(const TCHAR *chText, ads_point & pt_position, ads_point & pt_align, AcDb::TextHorzMode textHMode=AcDb::kTextLeft, AcDb::TextVertMode textVMode=AcDb::kTextBase, double dHeight=2, double dWidthFactor=0.8, double dAngle=0,  Adesk::UInt16 nColorIndex=7, AcDbObjectId objIdTextStyle=NULL) ;
	
	void AddMText(ads_point base_Pt, CString strText, AcDbObjectId textStyleId, AcDbMText::AttachmentPoint iAttachment, double dTextHeight,double dTextWidth, int icolor ) ;

	// 取得2点之间向量的夹角
	double GetAngle(const AcGePoint3d pt_3d_start, const AcGePoint3d pt_3d_end);
	// 取得到pt_3d_src的距离为dDisFromSrcPt，角度（pt_src->pt_to的向量）为dAngle的点
	void GetPoint(const AcGePoint3d pt_3d_src, double dDisFromSrcPt, double dAngle, AcGePoint3d & pt_3d_to);
	// 取得直线上/下一定距离范围内的字符串
	int GetStringOnLine(const AcDbLine * pLine, double dDistFromLine, CStringArray & strArrayFindText, const TCHAR * chSSGetMode=_T("_WP"), double dExtend=0, int iUpOrDown=1);

	// 取得实体的handle（只针对32位系统，故只取handle_low）
	unsigned long GetHandle(const AcDbEntity * pEnt);
	unsigned long GetHandle(const AcDbObjectId &objId);

	// 删除空字符文字或者trim后为空的文字
	int DeleteNullText(void);
	// 取得选择集实体的handle
	int GetSSEntHandle(const TCHAR * strSSMode, const void * pt1, const void * pt2, const struct resbuf * filter, CLongArray & iArrayHandle);

	// 缩放视图
	BOOL ZoomView(const AcGePoint3d & pt_3d_center, double dHeight, double dWidth);
	// 缩放视图
	BOOL ZoomView();
	
	//获取CAD系统变量
	void GetSysVar(const CString strVarName,CString & strVal);
	void GetSysVar(const CString strVarName,int & iVar);

	//设置CAD系统变量
	int SetSysVar(const CString strVarName, const CString strVal);
	int SetSysVar(const CString strVarName, int iVar);

	// 取得当前图纸中最大的ID值（xdata设置的appname为ID的）
	long GetMaxId(void);
	// 找出相同ID的实体
	int FindSameIdEnt(UINT nID, CLongArray & nArrayHandle);

	// 显示或隐藏块指定的属性
	BOOL ShowAtt(AcDbBlockReference * pRef, const TCHAR * chTag, bool bShow);

	// 取得当前图纸上实体占用空间的最左下点和最右上点
	int GetCurExtents(AcGePoint3d & pt_3d_min, AcGePoint3d & pt_3d_max);

	// 查找指定区域的实体
	//iOption:默认=0,表示包含边界(只要实体任何部分在区域内即算实体在该区域);其他=1,表示不包含边界,只包含边界内的实体; iEntType默认为0,表示所有实体, 1 文本, 2 块
	int FindEntInArea(const AcGePoint3d & pt_3d_bottomleft, const AcGePoint3d & pt_3d_topright, CLongArray & iArrayEntHandle, int iOption=0, int iEntType=0);

	// 修改块属性的字体大小
	void SetAttHeight(AcDbBlockReference *& pRef, double dHeight, double dWidthFactor, const CString & strTag, int iAttIndexToSet=0);

	// 计算一条线的平行线,默认返回上方的平行线
	void GetParallelLine(const AcGeLineSeg2d & lineSeg2dSrc, AcGeLineSeg2d & lineSeg2dParallel, double dDist, BOOL bRetUpLine=TRUE);

	// 计算两线段交点（AcGeLineSeg2d本身具有求交点的函数，但是没有AcDb::Intersect intType这种选项）
	int IntersectWith(const AcGeLineSeg2d* pLineSeg2dFrom, const AcGeLineSeg2d* pLineSeg2dTo, AcGePoint2d& pt_2d_ret, AcDb::Intersect intType=AcDb::kOnBothOperands);
	
	//当前视图
	AcDbViewTableRecord GetCurrentView() ;
	
	// 取得指定多段线指定顶点的线宽
	Acad::ErrorStatus GetPlineWidthAt(const UINT nPlineHandle, unsigned int nIndex, double& dStartWidth, double& dEndWidth) ;
	
	// 复制实体
	Acad::ErrorStatus Wblock(const AcDbObjectIdArray& objIdsOut,AcDbDatabase*& pDbTo,/*const AcGePoint3d& pt_3d_base=AcGePoint3d::kOrigin,*/ const AcGeMatrix3d & xform=AcGeMatrix3d::kIdentity);
	Acad::ErrorStatus Wblock(AcDbDatabase*& pDbFrom, AcDbDatabase*& pDbTo, /*const AcGePoint3d& pt_3d_base=AcGePoint3d::kOrigin,*/ const AcGeMatrix3d & xform=AcGeMatrix3d::kIdentity);
	
	// 新建一个CAD文档，等同于在cad中new一个文档
	static void CreateDocument(void * pData);
	// 打开CAD文档，等同于CAD中Open一个文档
	static void OpenDocument(void * pData);
	
	// 旋转图块（块参照类的setRotation函数不会旋转其带的属性）
	Acad::ErrorStatus RotationEnt(AcDbEntity* pEnt, double dAngle, const AcGePoint3d& center = AcGePoint3d::kOrigin);
	Acad::ErrorStatus RotationEnt(AcDbObjectId objId, double dAngle, const AcGePoint3d& center = AcGePoint3d::kOrigin);
	// 镜像
	Acad::ErrorStatus MirrEnt(AcDbEntity* pEnt, const AcGePoint3d& pt_3d_1, const AcGePoint3d& pt_3d_2) ;

	void getUcsToWcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db);
	AcGePoint3d ucsToWcs(const AcGePoint3d& pt) ;
	// 镜像
//	Acad::ErrorStatus MirrEnt(AcDbEntity* pEnt, const AcGePoint3d& pt_3d_1, const AcGePoint3d& pt_3d_2) ;
	// 从文件全路径获取文件名称，默认不带后缀，iOption=1则带后缀
	static BOOL GetFileName(const CString& strFilePath, CString& strFileName, int iOption=0 );
	// 通过handle读取AcdbText的文本
	BOOL GetTextString(long nHandle, CString& strText);
	//两点连线的中点
	AcGePoint3d GetMidPoint(const AcGePoint3d& pt_3d_1, const AcGePoint3d& pt_3d_2);
	
	//设置实体颜色
	Acad::ErrorStatus SetEntColorIndex(AcDbObjectId objId, Adesk::UInt16 iColorIndex);
	Acad::ErrorStatus SetEntColorIndex(ads_name en, Adesk::UInt16 iColorIndex);
	Acad::ErrorStatus SetEntColorIndex(long nHandle, Adesk::UInt16 iColorIndex);
	
	// 闪烁实体
	void FlashEnt(AcDbObjectId objId, DWORD dwCountLast, DWORD dwDelayCount=0);
	void FlashEnt(ads_name en,  DWORD dwCountLast, DWORD dwDelayCount=0);
	void FlashEnt(long nHandle, DWORD dwCountLast, DWORD dwDelayCount=0);
	
	// 取得实体的objid
	Acad::ErrorStatus GetObjectId(AcDbObjectId& objId, long nHandle);
	
	void PauseApp(DWORD dwCountLast);
	
	// 当前视图中心点
	AcGePoint2d GetViewCenterPoint(void);
	// 当前视图高度
	double GetViewHeight(void);
	// 当前视图宽度
	double GetViewWidth(void);
	// 高亮实体
	void Highlight(long nHandle);
	// 取消高亮实体
	void UnHighlight(long nHandle);
	
	// 取得显示视图的范围
	void GetViewExtents(CViewExtents& viewExt);
	// 设置视图范围
	void SetViewExtents(const CViewExtents& viewExt);
	// 取得所有块表名称(默认包含*Model_Space和*Paper_Space等)
	int GetAllBlockTable(CStringArray& strArrayName, int iMode=0);
	
	// 查找具有指定app扩展数据的实体
	int FindEnt(const CString& strAppName, const long nXdata, CLongArray& nArrayHandle);
	
	// 取得所有和指定圆相交的直线或多线
	int GetConnectLine(const AcGePoint3d& pt3dCenter, double dRadius,  CLongArray& nArrayHandle, double dOffset=0, int iIntersectNum=1);
	// 取得所有和指定矩形相交的直线或多线
	int GetConnectLine(const AcGePoint3d& pt_3d_min, const AcGePoint3d& pt_3d_max, CLongArray& nArrayHandle, double dOffset=0, int iIntersectNum=1);
	
	// 取得要绘制文本的长度(in CAD)	
	double GetTextLen(LPCTSTR strText, double dHeight, double dWr);
	
	//取得当前textstyle
	void GetCurTextStyle(CString& strStyleName) ;
	
	// 反序直线或多段线的顶点,默认iForward=0，左向右，iForward=1，右向左
	int ReverseLinePoint(AcDbEntity*& pEnt, int iForward=0, int iIndexOfColor=3);
	
	// 查找起点或终点在指定点的线
	int GetLineOfStartOrEndInPt(const AcGePoint3d& pt_3d_in, const CLongArray& nArrayHandleToFind, CLongArray nArrayHandleLine[2], double dTol=0.001);
	
	// 取得指定块的旋转角度
	double GetAngle(long nHandleBlk);
	
	//删除实体
	BOOL Erase(long nHandle);	
	int Erase(CLongArray& nArrayHandle);
	
	// 查找和指定多段线连接的盒子等实体
	int FindConnectEnt(const AcDbPolyline* pPolyLine, int iVertIndex, CLongArray& nArrayHandleFind, double dOffset=0.2);
	// 查找和指定点连接的盒子等实体
	int FindConnectEnt(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset=0.2);

	// 设置实体的显示、隐藏
	int setVisibility(AcDbEntity* pEnt, AcDb::Visibility newVal);
	// 设置实体的显示、隐藏
	int setVisibility(long nHandle, AcDb::Visibility newVal);
	// 设置实体的显示、隐藏
	int setVisibility(ads_name en, AcDb::Visibility newVal);
	// 绘制连接点（多段线）
	AcDbObjectId DrawConNode(AcGePoint3d pt_3d_cen, double dRadius);

	int SetTextString(long nHandle, const CString& strText);
	// 实体是否在关闭的off的层上
	bool IsOnOffLayer(AcDbEntity* pEnt);
	// 设置多段线宽
	int SetPLineWidth(long nHandle, double dWidth=0.5);
	//BOOL LSBExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt, double dHeightRate, double dWidthRate);
	//BOOL LSBExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt, double dHeightRate, double dWidthRate,double &d_width);
	BOOL LSBExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt ,double &d_width, double dHeightRate=0.328, double dWidthRate=1.488);
	BOOL LSBExcelToDwg2(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt ,double &d_width,double &d_row_hight,double dHeightRate=0.328, double dWidthRate=1.488);
	BOOL GetEntPosition(long nHandle, AcGePoint3d& pt_3d_base);
	int GetPLineStartWidth(long nHandle, double &dWidth);
	//BOOL ExcelToDwg(CString strExcelFileName, int iStartRow, int iEndRow, TCHAR iStartCol[], TCHAR iEndCol[], ads_point base_pt, double dHeightRate=0.328, double dWidthRate=2.41);
};

#endif // !defined(AFX_SYSTEM_H__1BBA4DD8_9883_4F9E_A32D_48744EF6E037__INCLUDED_)
