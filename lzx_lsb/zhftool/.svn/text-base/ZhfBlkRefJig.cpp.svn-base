#include "StdAfx.h"
#include "zhfblkrefjig.h"
#include "..\blkuserdll\UserSystem.h"

CZhfBlkRefJig::CZhfBlkRefJig(CGsPreviewCtrl* pCtrl)
: m_pBlkRef(NULL)
// , m_pt3dCur(0)
//, m_strBlkFile(_T(""))
{
	m_pCtrlGsPreview = pCtrl ;
	m_strBlkFile = pCtrl->m_strDwgFile ;
}

CZhfBlkRefJig::~CZhfBlkRefJig(void)
{
}


// -----------------------------------------------------------------------------
AcEdJig::DragStatus CZhfBlkRefJig::sampler(void)
{
	DragStatus stat;
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNoZeroResponseAccepted));

	static AcGePoint3d axisPointTemp;
	stat = acquirePoint(m_pt3dCur);
// 	if (m_pCtrlGsPreview!=g_pCtrlGsPreview)
// 	{
// 		int iii = 0 ;
// 		return AcEdJig::kCancel ;
// 	}
	if (axisPointTemp != m_pt3dCur)
		axisPointTemp = m_pt3dCur;
	else if (stat == AcEdJig::kNormal)
		return AcEdJig::kNoChange;

	return stat;
}

// -----------------------------------------------------------------------------
Adesk::Boolean CZhfBlkRefJig::update(void)
{
	m_pBlkRef->setPosition(m_pt3dCur) ;
	return Adesk::kTrue ;
}

// -----------------------------------------------------------------------------
AcDbObjectId CZhfBlkRefJig::append(void)
{
	AcDbBlockTableRecord *btr;
	acdbOpenObject(btr,m_pBlkRef->blockTableRecord(),kForRead);
	if(btr->hasAttributeDefinitions())
	{
		AcDbBlockTableRecordIterator *it;
		btr->newIterator(it);
		for(;!it->done();it->step())
		{
			AcDbEntity *pEnt;
			it->getEntity(pEnt, AcDb::kForRead);

			// Make sure the entity is an attribute definition
			// and not a constant.
			//
			AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);
			if (pAttDef != NULL && !pAttDef->isConstant())
			{
				// We have a non-constant attribute definition,
				// so build an attribute entity.

				AcDbAttribute * pAtt = new AcDbAttribute ;
				pAtt->setTag( pAttDef->tag() ) ;

				AcDb::TextHorzMode attHMode = pAttDef->horizontalMode() ;
				AcDb::TextVertMode attVMode = pAttDef->verticalMode() ;

				pAtt->setHorizontalMode( attHMode );
				pAtt->setVerticalMode ( attVMode ) ;

				AcGePoint3d pt_3d_position( 0,0,0 ), pt_3d_align(0,0,0), Att_Def_3d ( 0,0,0 ) ;
				Att_Def_3d = pAttDef->position() ;
				AcGePoint3d pt_3d_insert = m_pBlkRef->position() ;
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
				pAtt->setHeight( pAttDef->height() ) ;
				pAtt->setWidthFactor ( pAttDef->widthFactor() );		
				pAtt->setTextString( pAttDef->textString() ) ;
				pAtt->setInvisible ( pAttDef->isInvisible() ) ;
				pAtt->setColorIndex(pAttDef->colorIndex()) ;
				pAtt->setTextStyle(pAttDef->textStyle()) ;
				m_pBlkRef->appendAttribute(pAtt);
				pAtt->close() ;
			}
			pEnt->close();
		}

		delete it;
	}
	btr->close();

	AcDbObjectId retCode =AcEdJig::append () ;
	return (retCode) ;
}

// -----------------------------------------------------------------------------
AcDbEntity * CZhfBlkRefJig::entity(void) const
{	
	return m_pBlkRef ;
}

void CZhfBlkRefJig::DoIt(void)
{
	CUserSystem mysys ;
	mysys.m_pDb = acdbHostApplicationServices()->workingDatabase() ;
	AcDbObjectId objIdBlk;
	if (mysys.InsertDwgFile(m_strBlkFile, objIdBlk, 1))
	{
		m_pBlkRef = new AcDbBlockReference(AcGePoint3d::kOrigin,objIdBlk);
		CString strBlkName ;
		CBlkUtility::GetFileName(m_strBlkFile, strBlkName) ;
		setDispPrompt(_T("²åÈëÍ¼¿é:%s"), strBlkName);
		DragStatus es = drag();
		//acutPrintf(_T("\nint doit es:%d"), es) ;
		if (es==DragStatus::kNormal)
		{
			append();
			if (strBlkName.Left(3)==_T("XHJ")&&g_bAddJyjWhenInsertXhj)
			{
				AcDbBlockReference *pRef = NULL ;
				if(mysys.InsertBlock(pRef, _T("JYJ_PT_0"), m_pBlkRef->position()))
				{
					pRef->close() ;
					pRef = NULL ;
				}
			}
		}
		else if (m_pBlkRef!=NULL)
		{
			delete m_pBlkRef ;
		}
	}
}
