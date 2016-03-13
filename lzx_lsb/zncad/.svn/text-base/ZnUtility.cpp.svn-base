#include "stdafx.h"
#include "znutility.h"
#include "SBDestination.h"
//#include "ZnSxt.h"
#include "XhlCableInfoDlg.h"

CZnUtility::CZnUtility(void)
{
}

CZnUtility::~CZnUtility(void)
{
}

//查找在pt_3d_base处的盒子，返回值：0，未找到;1，找到一个多段线（主电缆），>1，盒子
int CZnUtility::FindConBox(const AcGePoint3d& pt_3d_base, CLongArray& nArrayHandleFind, double dOffset)
{
	int iRet = 0 ;

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	CLongArray nArrayHandleXhlCable ; //
	//CLongArray nArrayHandleXhl ;

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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			//不可见的实体（自身隐藏或者所在层隐藏）放弃
			if (pEnt->visibility()==AcDb::kInvisible||mysys.IsOnOffLayer(pEnt))
			{
				pEnt->close() ;
				continue;
			}

			long nHandleCur = mysys.GetHandle(pEnt) ;
			
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;

				AcGePoint3d pt_3d_blk ;
				pt_3d_blk = pRef->position() ;
				if (pt_3d_blk.distanceTo(pt_3d_base)>10) //距离超过10 的就不考虑了
				{
					pEnt->close() ;
					continue;
				}

				CString strBlkName ;
				mysys.GetBlockName(pRef, strBlkName) ;

				if (strBlkName.Left(6)==_T("BOX_XB"))
				{
					if (IsPointOnRect(pt_3d_base.convert2d(AcGePlane::kXYPlane), pt_3d_blk.convert2d(AcGePlane::kXYPlane), 4, 5))
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 2 ;
					}
				}
				else if (strBlkName.Left(7)==_T("BOX_FSD"))
				{
					if (IsPointOnRect(pt_3d_base.convert2d(AcGePlane::kXYPlane), pt_3d_blk.convert2d(AcGePlane::kXYPlane), 3, 4))
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 3 ;
					}
				}
				else if (strBlkName.Left(7)==_T("PMT_XHL"))
				{
					if (IsPointOnRect(pt_3d_base.convert2d(AcGePlane::kXYPlane), pt_3d_blk.convert2d(AcGePlane::kXYPlane), 6, 10))
					{
						nArrayHandleFind.Add(nHandleCur) ;
						//nArrayHandleXhl.Add(nHandleCur) ;
						iRet = 4 ;
					}
				}
				else if (strBlkName.Left(6)==_T("BOX_HF"))
				{					
					if (abs(pt_3d_blk.distanceTo(pt_3d_base)-3)<dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 5 ;
					}
				}
				else if (strBlkName.Left(6)==_T("BOX_HZ"))
				{						
					if (abs(pt_3d_blk.distanceTo(pt_3d_base)-1.5)<dOffset)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						iRet = 6 ;
					}
				}
			}
			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				long iIsXhlCable = 0 ;
				if (mysys.GetXdata(pEnt, _T("IS_XHLCABLE"), iIsXhlCable)&&iIsXhlCable>0)
				{
					AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
					UINT nVertIndex = 0 ;					 
					if (mysys.IsPointOnPolyLine(pPolyLine, pt_3d_base, nVertIndex, dOffset)>-1)
					{
						nArrayHandleFind.Add(nHandleCur) ;
						nArrayHandleXhlCable.Add(nHandleCur) ;
						iRet = 1 ;
					}
				}
			}

			pEnt->close() ;
		}	
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	//int iNumXhl = nArrayHandleXhl.GetCount() ;
	int iNumXhlCable = nArrayHandleXhlCable.GetCount() ;
// 	if (iNumXhl==1&&iNumXhlCable>0)
// 	{
// 		nArrayHandleFind.RemoveAll() ;
// 		nArrayHandleFind.Add(nArrayHandleXhl.GetAt(0)) ;
// 		iRet = 1 ;
// 	}
	if (iNumXhlCable>0)
	{
		nArrayHandleFind.RemoveAll() ;
		nArrayHandleFind.Add(nArrayHandleXhlCable.GetAt(0)) ;
		iRet = 1 ;
	}
	int iNumFind = nArrayHandleFind.GetCount() ;
	if (iNumFind>1)
	{
		acutPrintf(_T("\n在点（%.3f, %.3f）处发现多于1个盒子，请注意"), pt_3d_base.x, pt_3d_base.y) ;
	}

	return iRet ;
}
BOOL CZnUtility::IsPointOnRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dHeight, double dWidth, double dOffset)
{
	for (int i=0; i<4; i++)
	{
		AcGePoint2d pt_2d_tmp1 ;
		pt_2d_tmp1.x = pt_2d_cen.x+(i%3==0?-1:1)*dWidth/2 ;
		pt_2d_tmp1.y = pt_2d_cen.y+(i/2==0?1:-1)*dHeight/2 ;

		int j = (i==0?(i+3):(i-1)) ;
		AcGePoint2d pt_2d_tmp2 ;
		pt_2d_tmp2.x = pt_2d_cen.x+(j%3==0?-1:1)*dWidth/2 ;
		pt_2d_tmp2.y = pt_2d_cen.y+(j/2==0?1:-1)*dHeight/2 ;
		AcGeLineSeg2d lineseg2d(pt_2d_tmp2, pt_2d_tmp1) ;
		AcGeTol tol ;
		tol.setEqualPoint(dOffset) ;
		if (lineseg2d.isOn(pt_2d_base, tol))
		{
			return TRUE ;
		}
	}
	
	return FALSE ;
}


BOOL CZnUtility::IsIntersectWithRect(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_blk, double dHeight, double dWidth, double dOffset)
{
	AcGePoint2d pt_2d_start, pt_2d_end ; //创建2根组成十字的线和矩形4个边判断相交
	for (int i=0; i<2; i++)
	{
		if (i==0)
		{
			pt_2d_start.x = pt_2d_base.x-dOffset/2 ;
			pt_2d_start.y = pt_2d_base.y ;
			pt_2d_end.x = pt_2d_base.x+dOffset/2 ;
			pt_2d_end.y = pt_2d_base.y ;
		}
		else 
		{
			pt_2d_start.x = pt_2d_base.x ;
			pt_2d_start.y = pt_2d_base.y-dOffset/2 ;
			pt_2d_end.x = pt_2d_base.x ;
			pt_2d_end.y = pt_2d_base.y+dOffset/2 ;
		}
		AcGeLineSeg2d lineseg2d_1(pt_2d_start, pt_2d_end) ;
		for (int i=0; i<4; i++)
		{
			AcGePoint2d pt_2d_tmp1 ;
			pt_2d_tmp1.x = pt_2d_blk.x+(i%3==0?-1:1)*dWidth/2 ;
			pt_2d_tmp1.y = pt_2d_blk.y+(i/2==0?1:-1)*dHeight/2 ;

			int j = (i==0?(i+3):(i-1)) ;
			AcGePoint2d pt_2d_tmp2 ;
			pt_2d_tmp2.x = pt_2d_blk.x+(j%3==0?-1:1)*dWidth/2 ;
			pt_2d_tmp2.y = pt_2d_blk.y+(j/2==0?1:-1)*dHeight/2 ;
			AcGeLineSeg2d lineseg2d_2(pt_2d_tmp2, pt_2d_tmp1) ;
			AcGePoint2d pt2dIntersect ;
			if (lineseg2d_1.intersectWith(lineseg2d_2, pt2dIntersect))
			{
				return TRUE ;
			}
		}
	}
	return FALSE ;
}

BOOL CZnUtility::IsIntersectWithCircle(const AcGePoint2d& pt_2d_base, const AcGePoint2d& pt_2d_cen, double dRadius, double dOffset)
{
	AcGePoint2d pt_2d_start, pt_2d_end ;
	for (int i=0; i<2; i++)
	{
		if (i==0)
		{
			pt_2d_start.x = pt_2d_base.x-dOffset/2 ;
			pt_2d_start.y = pt_2d_base.y ;
			pt_2d_end.x = pt_2d_base.x+dOffset/2 ;
			pt_2d_end.y = pt_2d_base.y ;
		}
		else 
		{
			pt_2d_start.x = pt_2d_base.x ;
			pt_2d_start.y = pt_2d_base.y-dOffset/2 ;
			pt_2d_end.x = pt_2d_base.x ;
			pt_2d_end.y = pt_2d_base.y+dOffset/2 ;
		}
		AcGeLineSeg2d lineseg2d(pt_2d_start, pt_2d_end) ;
		AcGeCircArc2d circle2d(pt_2d_cen, dRadius) ;
		int iNum = 0 ;
		AcGePoint2d pt_2d_1, pt_2d_2 ;
		if (circle2d.intersectWith(lineseg2d, iNum, pt_2d_1, pt_2d_2))
		{
			return TRUE ;
		}
	}

	return FALSE ;
}

BOOL CZnUtility::IsIntersectWithLine(const AcGePoint3d& pt_3d_base, const AcDbPolyline* pPolyLine, double dOffset)
{
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
		AcGePoint3dArray pt3dArray ;
		if (pPolyLine->intersectWith(pLine, AcDb::kOnBothOperands, pt3dArray)==eOk)
		{
			if (pt3dArray.length()>0)
			{
				pLine->close() ;
				delete pLine ;
				return TRUE ;
			}
		}

		pLine->close() ;
		delete pLine ;
	}

	return FALSE ;
}
// 取得多段线顶点数组
int CZnUtility::GetVertex(ads_name en, AcGePoint2dArray& pt2dArrayVertex)
{
	int iRet = 0 ;
	CUserSystem mysys ;
	AcDbEntity *pEnt = NULL ;
	if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
			iRet = GetVertex(pPolyLine, pt2dArrayVertex) ;
		}
		pEnt->close() ;
	}

	return iRet;
}

// 取得多段线顶点数组
int CZnUtility::GetVertex(const AcDbPolyline* pPolyLine, AcGePoint2dArray& pt2dArrayVertex)
{
	int iRet = pPolyLine->numVerts() ;
	for (int i=0; i<iRet; i++)
	{
		AcGePoint2d pt_2d_tmp ;
		pPolyLine->getPointAt(i, pt_2d_tmp) ;
		pt2dArrayVertex.append(pt_2d_tmp) ;
	}

	return iRet ;
}

// 实体所属类型,转辙机:0,信号机:1,轨道电路:2
void CZnUtility::GetEntBelongType(long nHandleBelong, int& iType)
{
	CUserSystem mysys ;
	AcDbEntity* pEnt = NULL ;
	if (mysys.OpenAcDbEntity(nHandleBelong, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			CString strBlkName ;
			mysys.GetBlockName(pRef, strBlkName) ;
			if (strBlkName.GetLength()>3)
			{
				CString strBlkPart = strBlkName.Left(3) ;
				if (strBlkPart==_T("XHJ"))
				{
					iType = 1 ;
				}
				else if (strBlkPart==_T("ZZJ"))
				{
					iType = 0 ;
				}
				else if (strBlkPart==_T("JYJ"))
				{
					iType = 2 ;
				}
			}
		}
		pEnt->close() ;
	}
	if (iType==-1)
	{
		long nHandleTmp = 0 ;
		if(mysys.GetXdata(nHandleBelong, _T("BELONG_ENT"), nHandleTmp) &&nHandleTmp>0)
		{
			GetEntBelongType(nHandleTmp, iType) ;
		}
	}
}

// 返回指定的实体（信号机、道岔、轨道电路）
int CZnUtility::GetEnt(CLongArray& nArrayHandle, int iType)
{
// 	int nRet = 0 ;
// 
// 	CUserSystem mysys ;
// 
// 	struct resbuf* pRb ;
// 	int nLen = 0 ;
// 	CLongArray nArrayHandle ;
// 	//pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"), 1001, _T("CloneFrom"), 1070, nCloneFromHandle, 1001, _T("UPDOWN"), 1070, iUpOrDwon, 0) ;
// 	pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"), 1001, _T("ISGUIDAO"),  0) ;
// 	nLen = mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
// 	acutRelRb(pRb) ;
// 	
// 	for (int i=0; i<nLen; i++)
// 	{
// 		AcDbEntity* pEnt = NULL ;
// 		long nTemp = nArrayHandle.GetAt(i) ;
// 		if (mysys.OpenAcDbEntity(nTemp, pEnt, AcDb::kForRead))
// 		{		
// 			int iUPDOWN = 0 ;
// 			mysys.GetXdata(pEnt, _T("CloneFrom"), nCloneFrom) ;
// 			mysys.GetXdata(pEnt, _T("UPDOWN"), iUPDOWN) ;
// 
// 			pEnt->close() ;
// 		}
// 	}

	return 0;
}

int CZnUtility::GetJltType(const CString& strNameIntoXhl)
{
	int iTypeDljl = -1 ;
	if (strNameIntoXhl==_T("DC"))
	{
		iTypeDljl = 0 ;
	}
	else if (strNameIntoXhl==_T("XH"))
	{
		iTypeDljl = 1 ;
	}
	else if (strNameIntoXhl==_T("GD"))
	{
		iTypeDljl = 2 ;
	}
	else if (strNameIntoXhl==_T("DM"))
	{
		iTypeDljl = 3 ;
	}
	else
	{
		//acutPrintf(_T("\n连到主电缆的电缆名称 %s 不是以[DC,XH,GD,DM]开头，请规范名称！"), strNameIntoXhl) ;
	}
	return  iTypeDljl ;
}


// 取得电缆参数
int CZnUtility::GetCableParam(double& dCoeff, int& iIntoXhl, int& iPerGd, int& iPerBetweenGd, int& iPerBox, int& iPerBoxF, int& iPerBetweenBoxF)
{
	double d1 = 1.05 ;
	if (CBlkUtility::QueryValueReg(_T("ZnOption"), _T("CableCoeff"), d1) )
	{
		dCoeff = d1 ;
	}
	CString strAppNames[6] = {_T("CableIntoXhl"), _T("CablePerGd"), _T("CablePerBetweenGd"),  _T("CablePerBox"), _T("CablePerBoxF"), _T("CablePerBetweenBoxF")} ;
	int iData[6] = {50, 10, 10, 4, 4, 500} ;
	for (int i=0; i<6; i++)
	{
		if (CBlkUtility::QueryValueReg(_T("ZnOption"), strAppNames[i], iData[i]))
		{
			switch (i)
			{
			case 0:
				iIntoXhl = iData[i] ;
				break;
			case 1:
				iPerGd = iData[i] ;
				break;
			case 2:
				iPerBetweenGd = iData[i] ;
				break;
			case 3:
				iPerBox = iData[i] ;
				break;
			case 4:
				iPerBoxF = iData[i] ;
				break;
			case 5:
				iPerBetweenBoxF = iData[i] ;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

// 返回对象的类类型
// CString CZnUtility::GetClassType(CBoxNodeBase* pObject)
// {
// 	const char *szClassName = typeid(*pObject).name() ;
// 	CString strClassName ;
// // 	CBlkUtility::ConvMultiToWide(szClassName, strClassName) ;
// 	strClassName.TrimLeft(_T("class ")) ;
// 	return strClassName ;
// }
