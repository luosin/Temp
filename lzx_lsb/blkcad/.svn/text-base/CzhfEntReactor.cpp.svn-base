// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- CzhfEntReactor.cpp : Implementation of CzhfEntReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CzhfEntReactor.h"

//-----------------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(CzhfEntReactor, AcDbObjectReactor)

//-----------------------------------------------------------------------------
CzhfEntReactor::CzhfEntReactor () : AcDbObjectReactor() {
}

//-----------------------------------------------------------------------------
CzhfEntReactor::~CzhfEntReactor () {
}



// -----------------------------------------------------------------------------
void CzhfEntReactor::modified(const AcDbObject * dbObj)
{
	AcDbObjectReactor::modified (dbObj) ;
	AcDbBlockReference *pRef = AcDbBlockReference::cast(dbObj) ;
	AcGePoint3d pt_3d_ref = pRef->position() ;
	acutPrintf(_T("\nblock moved to  %.3f,%.3f"), pt_3d_ref.x, pt_3d_ref.y) ;	
	
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
	g_cSystem.m_pDb = pDb ;

	int iHandleBlk = 0 ;
	iHandleBlk = g_cSystem.GetHandle(pRef) ;

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
		if (eOk==pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead) )
		{
			if (pEnt->isKindOf(AcDbText::desc()))
			{				
				int iHandle = 0 ;
				g_cSystem.GetXdata(pEnt, _T("conblkhandle"), iHandle ) ;
				if (iHandle==iHandleBlk)
				{
					AcDbText *pText = AcDbText::cast(pEnt) ;
					AcGePoint3d pt_3d_txt = pText->position() ;
					pt_3d_txt.x = pt_3d_ref.x ;
					pText->upgradeOpen() ;
					pText->setPosition(pt_3d_txt) ;
					pText->downgradeOpen() ;
				}
			}
			pEnt->close() ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	

}
