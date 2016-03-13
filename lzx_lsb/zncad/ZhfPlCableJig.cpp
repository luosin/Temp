#include "StdAfx.h"
#include "ZhfPlCableJig.h"
#include "ZnUtility.h"

CZhfPlCableJig::CZhfPlCableJig()
: m_nHandleBoxStart(0)
, m_nHandleBoxEnd(0)
, m_iType(0)
, m_iMode(0)
{
	m_pPolyLine = new AcDbPolyline() ;
}

CZhfPlCableJig::CZhfPlCableJig(const AcGePoint2dArray& pt2dArray)
: m_nHandleBoxStart(0)
, m_nHandleBoxEnd(0)
, m_iType(0)
, m_iMode(1)
{
	m_pPolyLine = new AcDbPolyline() ;
	int iNum = pt2dArray.length() ;
	for (int i=0; i<iNum; i++)
	{
		AcGePoint2d pt_2d_tmp = pt2dArray.at(i) ;
		m_pPolyLine->addVertexAt(i, pt_2d_tmp) ;
		if (i==iNum-1)
		{
			m_pt3dCur.x = pt_2d_tmp.x ;
			m_pt3dCur.y = pt_2d_tmp.y ;
		}
	}
}

CZhfPlCableJig::~CZhfPlCableJig(void)
{
}

// -----------------------------------------------------------------------------
AcDbEntity * CZhfPlCableJig::entity(void) const
{
	//AcDbEntity * retCode =AcEdJig::entity () ;
	return m_pPolyLine ;
}

// -----------------------------------------------------------------------------
AcEdJig::DragStatus CZhfPlCableJig::sampler(void)
{
	AcEdJig::DragStatus stat ;
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		|AcEdJig::kGovernedByOrthoMode
		| AcEdJig::kNoNegativeResponseAccepted
		|AcEdJig::kAnyBlankTerminatesInput
		 | AcEdJig::kNullResponseAccepted
		| AcEdJig::kNoZeroResponseAccepted));

	static AcGePoint3d tempPoint;   

	stat = acquirePoint(m_pt3dCur);   

	if (tempPoint != m_pt3dCur)   
		tempPoint = m_pt3dCur;   
	else if (stat == AcEdJig::kNormal)   
		return AcEdJig::kNoChange; 

	return stat ;
}

// -----------------------------------------------------------------------------
Adesk::Boolean CZhfPlCableJig::update(void)
{
	//Adesk::Boolean retCode =AcEdJig::update () ;
	int iNumVert = m_pPolyLine->numVerts() ;
	if (iNumVert!=0)
	{
		m_pPolyLine->setPointAt(iNumVert-1, m_pt3dCur.convert2d(AcGePlane::kXYPlane)) ;
	}
	
	return (TRUE) ;
}

int CZhfPlCableJig::doIt(void)
{
	int iRet = 0 ;
	CUserSystem mysys ;

// 	m_pPolyLine = new AcDbPolyline() ;
	acutPrintf(_T("\n绘制箱盒连接线（电缆）.\n")) ;
	int iOldOsMode = 0 ;
	mysys.GetSysVar(_T("OSMODE"), iOldOsMode) ; //当期对象捕捉设置值
	mysys.SetSysVar(_T("OSMODE"), 147) ; //1（端点）、2（中点、5（象限点）、8（垂足）,10010011

	AcEdJig::DragStatus stat ;
	do 
	{
		int iNumVert = m_pPolyLine->numVerts() ;
		CString strMsg(_T("请输入连接线第1个点（在箱盒边上）:\n")) ;
		if (iNumVert>0)
		{
			strMsg.Format(_T("请输入第%d个点:\n"), iNumVert) ;
		}
		acutPrintf(strMsg) ;

		stat = drag() ;		

		if (stat==AcEdJig::kNormal)
		{
			if (iNumVert==0)
			{
				if (this->GetPickBox(m_pt3dCur, m_nHandleBoxStart)==1)
				{
					m_pPolyLine->addVertexAt(iNumVert, m_pt3dCur.convert2d(AcGePlane::kXYPlane)) ;
					m_pPolyLine->addVertexAt(iNumVert+1, m_pt3dCur.convert2d(AcGePlane::kXYPlane)) ;
				}
			}
			else
			{
				//判断该点和前点距离，太小的舍弃
				AcGePoint3d pt_3d_pre ;
				m_pPolyLine->getPointAt(iNumVert-2, pt_3d_pre) ;
				if (pt_3d_pre.distanceTo(m_pt3dCur)<0.2)
				{
					continue;
				}
				m_pPolyLine->setPointAt(iNumVert-1, m_pt3dCur.convert2d(AcGePlane::kXYPlane)) ;

				long nHandleBox = 0 ;
				if (this->GetPickBox(m_pt3dCur, m_nHandleBoxEnd)==1)
				{
					if (m_nHandleBoxEnd==m_nHandleBoxStart)
					{
						AfxMessageBox(_T("径路起点和终点不能选在同一盒子！")) ;
						m_nHandleBoxEnd = 0 ;
						continue;
					}
					acutPrintf(_T("\n已选择末端盒子或主电缆.")) ;
					break;
				}
				m_pPolyLine->addVertexAt(iNumVert, m_pt3dCur.convert2d(AcGePlane::kXYPlane)) ;
			}
		}
		else 
		{
			if(m_iType==0)
			{
				m_pPolyLine->reset(Adesk::kFalse, 0) ; //reset(Adesk::Boolean reuse,unsigned int numVerts);If reuse is Adesk::kFalse, then numVerts is ignored and all existing vertex information will be deleted.
			}
			else if (m_iType==1)
			{
				if (iNumVert>0)
				{
					m_pPolyLine->removeVertexAt(iNumVert-1) ;
				}
				break;
			}
		}
	} while (stat==AcEdJig::kNormal);

	int iNumVert2 = m_pPolyLine->numVerts() ;
	if (iNumVert2>1)
	{
		if (m_iMode==0)
		{
			append() ;
		}
		else if (m_iMode==1)
		{
			for (int i=0; i<iNumVert2; i++)
			{
				AcGePoint2d pt_2d_tmp ;
				m_pPolyLine->getPointAt(i, pt_2d_tmp) ;
				m_pt2dArray.append(pt_2d_tmp) ;
			}
			delete m_pPolyLine ;
		}
		iRet = 1 ;
	}
	else 
	{
		delete m_pPolyLine ;
	}

	mysys.SetSysVar(_T("OSMODE"), iOldOsMode) ;

	return iRet ;
}

// pick点所在的箱盒
int CZhfPlCableJig::GetPickBox(const AcGePoint3d& pt_3d_pick, long& nHandleBox)
{
	//AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	//mysys.m_pDb = pDb ;

	int iNumVert = m_pPolyLine->numVerts() ;

	long iLen = 0 ;
	CLongArray iArrayHandle ;
	CZnUtility::FindConBox(pt_3d_pick, iArrayHandle, 0.1) ;
	iLen = iArrayHandle.GetCount() ;

	int iRet = 0 ;
	if (iLen>0)
	{
		AcDbEntity* pEnt = NULL ;
		if (mysys.OpenAcDbEntity(iArrayHandle.GetAt(0), pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
				CString strBlkName ;
				mysys.GetBlockName(pRef, strBlkName) ;
				if (strBlkName.Left(3)==_T("BOX"))
				{
					nHandleBox = iArrayHandle.GetAt(0) ;
					iRet++ ;
					//计算此盒子容纳的电缆芯线(放在此处貌似不合适)
				}
				else if (strBlkName.Left(7)==_T("PMT_XHL")||strBlkName.Left(7)==_T("PMT_ZBS"))
				{
					if (iNumVert==0)
					{
						nHandleBox = iArrayHandle.GetAt(0) ;
						iRet++ ;
						m_iType = 1 ; //主电缆槽
					}
				}
			}
			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				if (iNumVert>0)
				{
					long iIsXhlCable = 0 ;
					if (mysys.GetXdata(pEnt, _T("IS_XHLCABLE"), iIsXhlCable)&&iIsXhlCable>0)
					{
						nHandleBox = iArrayHandle.GetAt(0) ;
						iRet++ ;
						m_iType = 2 ; //进槽电缆
					}
				}
			}

			pEnt->close() ;
		}
	}
	else if (iLen>1)
	{
		acutPrintf(_T("\n注意:在点(%.3f,%.3f)位置有不止1个箱盒(%d个)或者表示主电缆的多段线！"), pt_3d_pick.x, pt_3d_pick.y, iLen) ;
	}

	return iRet ;
}
