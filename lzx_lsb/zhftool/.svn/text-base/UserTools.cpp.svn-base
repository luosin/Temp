#include "StdAfx.h"
#include "usertools.h"
#include "SBDestination.h"
#include "TextDlg.h"
#include "InputTextDlg.h"
#include "BatchDTextDlg.h"
#include "FindReplaceDlg.h"
#include "ConvertTextDlg.h"
#include "ModifyTextStyleDlg.h"
#include "STextSet.h"
#include "../blkuserdll/BlkExcelTool.h"
#include "XlsToDwgDlg.h"
#include "BatchFindReplaceDlg.h"
#include "ChangeFileNameDlg.h"
#include "ShowBlkAttDlg.h"
#include "GetDianlanSumDlg.h"
#include "GetHeightWidthDlg.h"
#include "SetBlkAttDlg.h"
#include "SetPlWidthDlg.h"
#include "BlockUtil.h"
#include "ConvertUtil.h"
#include "PolylineUtil.h"



CUserTools::CUserTools(void)
{

}

CUserTools::~CUserTools(void)
{

}

int CUserTools::ZhfTools(void)
{
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	int iInput = 0 ;
	if (acedGetInt( _T("\n本系列命令供开发人者使用 1)显示文本对齐方式 2)显示数据库所有ent......:"), &iInput)==RTNORM)
	{
		if (iInput==1)
		{
			ads_name en ;
			ads_point pt ;
			if (RTNORM==acedEntSel(_T("\nselect a text"), en, pt))
			{
				AcDbEntity* pEnt = NULL ;
				if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
				{
					if (pEnt->isKindOf(AcDbText::desc()))
					{
						AcDbText* pText = AcDbText::cast(pEnt) ;
						AcDb::TextHorzMode horz = pText->horizontalMode() ;
						AcDb::TextVertMode vert = pText->verticalMode() ;
						
						acutPrintf(_T("\nHorz:%d, Vert:%d"), horz, vert) ;
						acutPrintf(_T("\nTextHorzMode Name Value Description \nkTextLeft 0 TH_LEFT \nkTextCenter 1 TH_CENT \nkTextRight 2 TH_RIGHT \nkTextAlign 3 THV_ALIGN \nkTextMid 4 THV_MID\nkTextFit 5 THV_FIT ")) ;
						acutPrintf(_T("\nTextVertMode \nName Value Description \nkTextBase 0 TV_BASE \nkTextBottom 1 TV_BOT\nkTextVertMid 2 TV_MID \nkTextTop 3 TV_TOP")) ;
					}

					pEnt->close() ;
				}
			}
		}
		else if (iInput==2) //显示数据库所有实体
		{
			DWORD dwCount1=::GetTickCount();

			AcDbDatabase* pCurDb = new AcDbDatabase ;

			if (Acad::eOk== pCurDb->readDwgFile(_T("D:\\逻辑检查\\1.dwg")))
			{
				AcDbBlockTable *pBlkTbl = NULL;
				pCurDb->getBlockTable(pBlkTbl, AcDb::kForRead);

				AcDbBlockTableRecord *pBlkTblRcd = NULL;
				pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
				pBlkTbl->close();

				AcDbBlockTableRecordIterator *it = NULL;
				pBlkTblRcd->newIterator(it);
				AcDbEntity *pEnt = NULL;
				for (it->start(); !it->done(); it->step())
				{
					if (it->getEntity(pEnt, AcDb::kForRead) == Acad::eOk)
					{
						// 					CString strMsg ;
						// 					strMsg.Format(_T("\n%s"), pEnt->isA()->name()) ;
						// 					acutPrintf(strMsg) ;
						if (pEnt->isKindOf(AcDbText::desc()))
						{
							pEnt->upgradeOpen() ;
							//pEnt->setVisibility(AcDb::kVisible) ;
							AcDbText* pText = AcDbText::cast(pEnt) ;
							pText->setColorIndex(3) ;

							pEnt->downgradeOpen() ;
						}

						pEnt->close();
					}
				}
				delete it;
				pBlkTblRcd->close();
			}
			delete pCurDb ;
			pCurDb = NULL ;

			DWORD dwCount2=::GetTickCount();

			acutPrintf(_T("\niter耗时:%d"), dwCount2-dwCount1) ;

		}
		else if (iInput==3)
		{
			DWORD dwCount1=::GetTickCount();

			AcDbDatabase* pCurDb = new AcDbDatabase ;

			if (Acad::eOk== pCurDb->readDwgFile(_T("D:\\逻辑检查\\1.dwg")))
			{
				AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
				acdbHostApplicationServices()->setWorkingDatabase(pCurDb) ; 
				mysys.m_pDb = pCurDb ;

				struct resbuf* pRb ;
				long nLen = 0 ;
				CLongArray nArrayHandle;
				pRb = acutBuildList(RTDXF0, _T("TEXT"), 0) ;
				nLen = mysys.GetSSEntHandle(_T("X"), NULL, NULL, pRb, nArrayHandle) ;
				acutRelRb(pRb) ;
				for (int i=0; i<nLen; i++)
				{
					AcDbEntity* pEnt = NULL ;
					if (mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt))
					{
						AcDbText* pText = AcDbText::cast(pEnt) ;
						pText->setColorIndex(3) ;
						pEnt->close() ;
					}
				}

				acdbHostApplicationServices()->setWorkingDatabase(pOldDb) ;
			}
			DWORD dwCount2=::GetTickCount();
            
			acutPrintf(_T("\nssget耗时:%d"), dwCount2-dwCount1) ;
		}
		else if (iInput==4)
		{
			AcGePoint3dArray ptArrayIntersect ;

			ads_name en1 ;
			ads_point pt1 ;
			if(acedEntSel(_T("\nselect ent1:"), en1, pt1)==RTNORM)
			{
				AcDbEntity* pEnt1= NULL ;
				if(mysys.OpenAcDbEntity(en1, pEnt1, AcDb::kForRead) )
				{
					ads_name en2 ;
					ads_point pt2 ;
					if(acedEntSel(_T("\nselect ent2:"), en2, pt2)==RTNORM)
					{
						AcDbEntity* pEnt2= NULL ;
						if(mysys.OpenAcDbEntity(en2, pEnt2, AcDb::kForRead) )
						{							
							pEnt1->intersectWith(pEnt2, AcDb::kOnBothOperands, AcGePlane::kXYPlane, ptArrayIntersect) ;
							pEnt2->close() ;
						}
					}

// 					if (pEnt1->isKindOf(AcDbPolyline::desc()))
// 					{
// 						AcDbPolyline* pPolyline = AcDbPolyline::cast(pEnt1) ;
// 						CLongArray nArray ;
// 						mysys.FindConnectEnt(pPolyline, 0, nArray) ;
// 					}

					pEnt1->close() ;
				}
			}
			CString strMsg ;
			for (int i=0; i<ptArrayIntersect.length(); i++)
			{
				strMsg.AppendFormat(_T("%.3f,%.3f-"), ptArrayIntersect.at(i).x, ptArrayIntersect.at(i).y) ;
			}
			acutPrintf(_T("\nTotal Intersect Point %d, %s"), ptArrayIntersect.length(), strMsg) ;
		}
		else if (iInput==5)
		{
			AcGePoint3d pt_3d_cen(0,0,0);
			double dRadius = 10 ; 

			CLongArray nArrayHandle ;	

			ads_name en2 ;
			ads_point pt2 ;
			if(acedEntSel(_T("\nselect ent2:"), en2, pt2)==RTNORM)
			{
				AcDbEntity* pEnt2= NULL ;
				if(mysys.OpenAcDbEntity(en2, pEnt2, AcDb::kForRead) )
				{
					if (pEnt2->isKindOf(AcDbBlockReference::desc()))
					{
						AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt2) ;
						CString strBlkName ;
						mysys.GetBlockName(pRef, strBlkName) ;
						AcGePoint3d pt_3d_blk ;
						pt_3d_blk = pRef->position() ;

						AcGePoint3d pt_3d_minmax[2]  ;
						double dBoxWidth = 4 ; //默认是送受电盒子的长度
						if (strBlkName.Left(6)==_T("BOX_XB"))
						{
							dBoxWidth = 5 ;
							for (int i=0; i<2; i++)
							{
								pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
								pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*2 ;
							}
							mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandle, 0.05) ;
						}
						else if (strBlkName.Left(7)==_T("BOX_FSD"))
						{
							dBoxWidth = 4 ;
							for (int i=0; i<2; i++)
							{
								pt_3d_minmax[i].x = pt_3d_blk.x+(i==0?-1:1)*dBoxWidth/2 ;
								pt_3d_minmax[i].y = pt_3d_blk.y+(i==0?-1:1)*1.5 ;
							}
							mysys.GetConnectLine(pt_3d_minmax[0], pt_3d_minmax[1], nArrayHandle, 0.05) ;
						}
						else if (strBlkName.Left(6)==_T("BOX_HF"))
						{						
							dBoxWidth = 6 ;
							mysys.GetConnectLine(pt_3d_blk, 3, nArrayHandle, 0.05) ;
						}
						else if (strBlkName.Left(6)==_T("BOX_HZ"))
						{						
							dBoxWidth = 3 ;
							mysys.GetConnectLine(pt_3d_blk, 1.5, nArrayHandle, 0.05) ;
						}	

					}
					pEnt2->close() ;
				}
			}
			CString strMsg ;
			for (int i=0; i<nArrayHandle.GetCount(); i++)
			{
				strMsg.AppendFormat(_T("%d-"), nArrayHandle.GetAt(i)) ;
			}
			acutPrintf(_T("\nTotal Intersect ent %d, %s"), nArrayHandle.GetCount(), strMsg) ;

		}
		else if (iInput==6)
		{
			ads_name en ;
			ads_point pt ;
			if(acedEntSel(_T("\nselect an ent:"), en, pt)==RTNORM)
			{
				AcDbEntity *pEnt = NULL ;
				if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
				{
					if (pEnt->isKindOf(AcDbBlockReference::desc()))
					{
						AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
						double dAngleBlk = pRef->rotation() ;
						AcGeScale3d scale3d = pRef->scaleFactors() ;
						double dSX = scale3d.sx ;
						AcDbBlockTableRecord * pBlkTblRec = NULL ;

						//得到块的属性
						AcDbObjectIterator * pObjIter ;
						pObjIter = pRef->attributeIterator() ;
						for(pObjIter->start(); !pObjIter->done(); pObjIter->step()) 
						{
							AcDbObjectId attId ;
							attId = pObjIter->objectId() ;
							AcDbAttribute * pAttrib ;
							pRef->openAttribute(pAttrib, attId, AcDb::kForRead, Adesk::kFalse) ;

							//for test;
							//////////////////////////////////////////////////////////////////////////

							TCHAR * pText ;
							TCHAR * pTag ;
							AcGePoint3d attPoint(0,0,0) ;
							attPoint = pAttrib->position() ;
							pTag = pAttrib->tag() ;
							pText = pAttrib->textString() ;	
							AcDbExtents ext ;
							pAttrib->getGeomExtents(ext) ;
							AcGePoint3d pt_3d_bottomleft = ext.minPoint() ;
							AcGePoint3d pt_3d_topright = ext.maxPoint() ;
							double dWidth = pAttrib->widthFactor() ;
							double dHeight = pAttrib->height() ;
							bool b1 =  pAttrib->isMirroredInX() ;
							bool b2 =  pAttrib->isMirroredInY() ;
							if (dSX==-1)
							{
								pAttrib->upgradeOpen() ;
								if (dAngleBlk>(PI/2) && dAngleBlk<(PI*3/2))
								{
									pAttrib->mirrorInY(!b2) ;
									pAttrib->setVerticalMode(AcDb::kTextTop) ;
								}
								else
								{
									pAttrib->mirrorInX(!b1) ;
								}
								pAttrib->downgradeOpen() ;
							}
							else
							{
								pAttrib->upgradeOpen() ;
								if (dAngleBlk>(PI/2) && dAngleBlk<(PI*3/2))
								{
									pAttrib->setRotation(pAttrib->rotation()-PI) ;
									pAttrib->setVerticalMode(AcDb::kTextTop) ;
								}
								pAttrib->downgradeOpen() ;
							}
							acutPrintf(_T("\n********************\n属性文本\ntag:%s\ncontent:%s\npositon:%f-%f\nangle:%.3f\nmirroedin:x:%d, y:%d\nbottomleft:%.3f,%.3f, topright: %.3f,%.3f\nwidth:%.3f height:%.3f\n********************"), pTag, pText, attPoint.x, attPoint.y, pAttrib->rotation(), b1, b2, pt_3d_bottomleft.x, pt_3d_bottomleft.y, pt_3d_topright.x, pt_3d_topright.y,dWidth, dHeight);

							//////////////////////////////////////////////////////////////////////////		

							pAttrib->close() ;	
						}

						AcDbObjectId blkTblObjId ;
						blkTblObjId = pRef->blockTableRecord() ;
						acdbOpenObject(pBlkTblRec, blkTblObjId, AcDb::kForRead) ;

						//遍历块表记录
						AcDbBlockTableRecordIterator * pBlkTblRecIterator ;
						pBlkTblRec->newIterator(pBlkTblRecIterator) ;
						for(; !pBlkTblRecIterator->done(); pBlkTblRecIterator->step())
						{
							AcDbEntity *pEnt1;
							if(Acad::eOk==pBlkTblRecIterator->getEntity(pEnt1, AcDb::kForRead))
							{
								acutPrintf(_T("\n%s"), pEnt1->isA()->name()) ;
								if (pEnt1->isKindOf(AcDbAttributeDefinition::desc()))
								{
									AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt1) ;

									double dAngle = pAttDef->rotation() ;
									bool b1 = pAttDef->isMirroredInX() ;
									bool b2 = pAttDef->isMirroredInY() ;
									acutPrintf(_T("\n#################\n%s:%.3f %d %d %.3f-%.3f\n###################"), pAttDef->tag(), dAngle, b1, b2, pAttDef->position().x, pAttDef->position().y) ;
								}
								else if (pEnt1->isKindOf(AcDbText::desc()))
								{
									AcDbText *pText = AcDbText::cast(pEnt1) ;
									acutPrintf(_T("\n%s:%.3f %.3f-%.3f"), pText->textString(), pText->rotation(), pText->position().x, pText->position().y) ;
								}
								pEnt1->close() ;
							}
						}
						delete pBlkTblRecIterator ;
						pBlkTblRec->close() ;
					}

					pEnt->close() ;
				}

			}
		}
		else if (iInput==7)
		{
			ads_name ss;
			if (RTNORM != acedSSGet( NULL, NULL, NULL, NULL, ss ))
			{
				acutPrintf( _T("acedSSGet cancelled\n") );
				return 0;
			}

			long length;
			if (RTNORM != acedSSLength( ss, &length ) || 0 == length)
			{
				acutPrintf( _T("empty selection set\n") );
				acedSSFree( ss );
				return 0;
			}

			ads_name          ent;
			AcDbObjectId      id;
			AcDbObjectIdArray list;
			AcDbIdMapping     pIdMap;
			long i = 0 ;
			for (i = 0; i < length; i++) {
				if (RTNORM != acedSSName( ss, i, ent ))
					continue;
				if (Acad::eOk != acdbGetObjectId( id, ent ))
					continue;

				list.append( id );
			}
			acedSSFree( ss );

			if (list.isEmpty()) {
				acedAlert( _T("No entities found in model space of DWG" ));
				return 0;
			}

			AcApDocManager *pDocMgr = acDocManager ;
			//acDocManager->executeInApplicationContext(CreateDoc, (void *)pData); 

			int iDocCount = pDocMgr->documentCount() ;
			AcApDocument *pDocCur = pDocMgr->curDocument() ;

			// 				AcApDocumentIterator *pIterDoc = pDocMgr->newAcApDocumentIterator() ;
			// 				int i = 0 ;
			// 				AcApDocument *pDocToAct = NULL ;
			// 				for (;!pIterDoc->done(); pIterDoc->step(), i++)
			// 				{
			// 					AcApDocument *pDoc = pIterDoc->document() ;
			// 					CString strDoc= pDoc->fileName() ;
			// 					if (strDoc.Find(_T("Drawing2"))!=-1)
			// 					{
			// 						pDocToAct = pDoc ;
			// 					}	
			// 
			// 					acutPrintf(_T("\n%d:%s-%s"), i, pDoc->docTitle(), strDoc) ;
			// 				}


			//AcDbDatabase * pDbTo = pDocCur->database() ;
			AcDbDatabase *pDbNew = new AcDbDatabase ;


			//pDocMgr->setCurDocument(pDocCur) ;
			//pDocMgr->lockDocument(pDocCur) ;

			acutPrintf(_T("\n%d:%s-%s"), i, pDocCur->docTitle()) ;

			mysys.Wblock(list, pDbNew) ;
			TCHAR strFileName[] = _T("c:\\111.dwg") ;
			pDbNew->saveAs(strFileName) ;
			delete pDbNew ;
			pDbNew = NULL ;
			pDocMgr->executeInApplicationContext(CUserSystem::OpenDocument,(void *)strFileName) ;
			//OpenDoc(strFileName) ;

			//pDocMgr->unlockDocument(pDocCur) ;
			//pDocMgr->setCurDocument(pDocCur) ;
			//pDocMgr->activateDocument(pDocToAct) ;
			//zhf:20140804:此段多文档代码老崩溃，原因待查
			//zhf:20140805:执行完后将curdoc设置回原执行时的doc好像就不崩溃了。

		}
		else if (iInput==8)
		{
// 			CSBDestination sb(NULL, IDS_BFF_TITLE) ;
// 			sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
// 			if (sb.SelectFolder())
// 			{
// 				CString strPath = _T("") ;
// 				strPath = sb.GetSelectedFolder() ;
// 				if (strPath.IsEmpty())
// 				{
// 					return 0  ;
// 				}
// 				CStringArray strArrayFile ;
// 				CBlkUtility::FindFile(strPath, _T("dwg"), strArrayFile) ;
// 
// 				AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;
// 			
// 				for (int i=0; i<strArrayFile.GetCount(); i++)
// 				{
// 					CString strFile = strArrayFile.GetAt(i) ;
// 					AcApDocManager *pDocMgr = acDocManager ;
// 					TCHAR * chFileName = NULL ;
// 					chFileName = (LPTSTR)(LPCTSTR)strFile ;
// 					pDocMgr->executeInApplicationContext(RefreshDwg,(void *)chFileName) ;
// 				}
// 			}

		}
		else if (iInput==9) //将信号机图块几点的小竖线长度变为0.01
		{
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
				if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
				{		
					//处理单线 AcDbLine ;
					//const char * pName = pEnt->isA()->name() ;
					if (pEnt->isKindOf(AcDbLine::desc()))
					{
						AcDbLine *pLine = AcDbLine::cast(pEnt) ;
						AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0) ;
						pLine->getStartPoint(pt_3d_start) ;
						pLine->getEndPoint(pt_3d_end) ;
						if (pt_3d_start.x==0.00&&pt_3d_end.x==0.00)
						{
							double d1 = fabs(pt_3d_start.y) ;
							double d2 = fabs(pt_3d_end.y) ;
							if (d1==0.01 && d2==0.01)
							{
								pt_3d_start.y = pt_3d_start.y/10 ;
								pt_3d_end.y = pt_3d_end.y/10 ;
								pLine->setStartPoint(pt_3d_start) ;
								pLine->setEndPoint(pt_3d_end) ;
							}
						}				
					}
					pEnt->close() ;
				}		
			}
			delete pBlkTblRecIter ;
			pBlkTblRecIter = NULL ;
		}
		else if(iInput==10)
		{
			AcDbBlockTable * pBlkTbl = NULL ;
			pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
			acutPrintf(_T("\n%s"),  pBlkTbl->isA()->name()) ;
			AcDbBlockTableIterator* pSymTblIter = NULL ;
			pBlkTbl->newIterator(pSymTblIter) ;
			pBlkTbl->close() ;
			int i = 0 ;
			for(pSymTblIter->start();!pSymTblIter->done();pSymTblIter->step())
			{
				AcDbObjectId id ;
				pSymTblIter->getRecordId(id) ;
				AcDbObject* pObj = NULL ;
				acdbOpenAcDbObject(pObj, id, AcDb::kForRead) ;
				acutPrintf(_T("\n%d:%s"), i, pObj->isA()->name()) ;

				TCHAR * pNameTbl = NULL ;
				AcDbBlockTableRecord* pSymTblRec = NULL ;
				pSymTblIter->getRecord(pSymTblRec, AcDb::kForRead) ;
				pSymTblRec->getName(pNameTbl) ;
				acutPrintf(_T("\n%d:%s"), i, pNameTbl) ;
				pSymTblRec->close() ;

				i++ ;
			}
			delete pSymTblIter ;
			pSymTblIter = NULL ;

// 			AcDbSymbolTableRecord * pBlkTblRec = NULL ;
// 			pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
// 			pBlkTbl->close() ;
// 			AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
// 
// 			pBlkTblRec->newIterator( pBlkTblRecIter );
// 			pBlkTblRec->close();

// 			AcDbEntity * pEnt = NULL ;
// 			int i = 0 ;
// 			for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
// 			{
// 				TCHAR * pNameRec = NULL ;
// 				pBlkTblRecIter->blockTableRecord()->getName(pNameRec) ;
// 				acutPrintf(_T("\n%d:%s"), i, pNameRec) ;
// 				//遍历并打开实体 ;
// 				if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
// 				{		
// 					const char * pName = pEnt->isA()->name() ;
// 					acutPrintf(_T("\n%d:%s"), i, pName) ;
// 					pEnt->close() ;
// 					
// 				}	
// 				i++ ;
// 			}
// 			delete pBlkTblRecIter ;
// 			pBlkTblRecIter = NULL ;
		}
		else if (iInput==11)
		{
			struct resbuf *rb ;
			rb = acutNewRb(RTSHORT) ;
			acedGetVar(_T("INSUNITS"), rb)  ;
			acutPrintf(_T("\ndragmode:%d"), rb->resval.rint) ;
			rb->resval.rint = 4 ;
			acedSetVar(_T("INSUNITS"), rb) ;
			acutRelRb(rb) ;
		}
	}

	return 0;
}

// 镜像实体
void CUserTools::MirrorEnt(void)
{
	AcDbDatabase * pDb  = acdbHostApplicationServices()->workingDatabase() ;

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	ads_name ssname, temp_name ;
	acutPrintf(_T("\n选择要镜像的实体(仅用解决块,镜像后文字问题)")) ;

	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		ads_point pt1 ;
		if (RTNORM==acedGetPoint(NULL, _T("\n指定镜像线的第一点:"), pt1))
		{
			ads_point pt2 ;
			if (RTNORM==acedGetPoint(NULL, _T("\n指定镜像线的第二点:"), pt2))
			{
				long lLength = 0 ;
				acedSSLength(ssname, &lLength) ;
				for (int i=0; i<lLength; i++)
				{
					acedSSName(ssname, i, temp_name) ;
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(temp_name, pEnt))
					{
						AcGePoint3d pt_3d_1, pt_3d_2 ;
						pt_3d_1 = asPnt3d(pt1) ;
						pt_3d_2 = asPnt3d(pt2) ;
						mysys.MirrEnt(pEnt, pt_3d_1, pt_3d_2) ;
						pEnt->close() ;
						pEnt = NULL ;
					}
				}
			}
		}

		acedSSFree(ssname) ;
	}
}

// 旋转块，解决文字颠倒问题
void CUserTools::RotationEnt(void)
{
	AcDbDatabase * pDb  = acdbHostApplicationServices()->workingDatabase() ;

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	ads_name ssname, temp_name ;
	acutPrintf(_T("\n选择要旋转的实体(仅用解决块,旋转后文字问题)")) ;

	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		ads_real dAngle = 0 ;
		if (RTNORM==acedGetReal(_T("\n输入旋转角度:"), &dAngle))
		{
			long lLength = 0 ;
			acedSSLength(ssname, &lLength) ;
			for (int i=0; i<lLength; i++)
			{
				acedSSName(ssname, i, temp_name) ;
				AcDbEntity *pEnt = NULL ;
				if (mysys.OpenAcDbEntity(temp_name, pEnt))
				{
					AcGePoint3d pt_3d_base(0,0,0) ;
					if (pEnt->isKindOf(AcDbBlockReference::desc()))
					{
						AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
						pt_3d_base = pRef->position() ;

						mysys.RotationEnt(pEnt, dAngle*PI/180, pt_3d_base) ;
					}

					pEnt->close() ;
					pEnt = NULL ;
				}
			}
		}

		acedSSFree(ssname) ;
	}
}

// 调整多段线和起始点相邻的顶点，使其和起始点平行或垂直（微调）
void CUserTools::AdjustPlineVertex(void)
{
	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"),0) ;	

	long iLen = 0 ;
	ads_name sum_name ;
	if (Acad::eNormal==acedSSGet(NULL, NULL, NULL, pRb, sum_name))
	{	
		acedSSLength(sum_name, &iLen) ;
		for (int i=0; i<iLen; i++)
		{
			acutPrintf(_T("\n%d:"), i) ;

			ads_name en ;
			acedSSName(sum_name, i, en) ;
			AcDbEntity *pEnt = NULL ;
			AcDbObjectId ent_id ;
			acdbGetObjectId(ent_id, en) ;
			if(Acad::eOk == acdbOpenAcDbEntity(pEnt,ent_id, AcDb::kForRead))
			{
				AcDbPolyline *pPolyLine = AcDbPolyline::cast(pEnt) ;
				int iNumVert = pPolyLine->numVerts() ;
				if (iNumVert>1)
				{
					AcGePoint2d pt_2d_start, pt_2d_nearstart, pt_2d_end, pt_2d_nearend ;
					pPolyLine->getPointAt(0, pt_2d_start) ;
					pPolyLine->getPointAt(iNumVert-1, pt_2d_end) ;
					pPolyLine->getPointAt(1, pt_2d_nearstart) ;
					pPolyLine->getPointAt(iNumVert-2, pt_2d_nearend) ;

					//5度认为靠近
					//靠近x轴，即接近平行：0~5,175~185,355~360
					double dDegreeOf5 = PI*5/180 ; //(5°=弧度0.0872664625997165)
					
					if (iNumVert==2)
					{
						int iInput = 0 ;
						if(RTNORM==acedGetInt(_T("\n所选多段线只有2个顶点，请指定以起点<0>还是终点<1>对齐:"), &iInput) )
						{
							AcGeVector2d vec1 = pt_2d_end-pt_2d_start ;
							double d1 = vec1.angle() ;
							if (d1<dDegreeOf5||d1>(PI*2-dDegreeOf5)||d1>(PI-dDegreeOf5)&&d1<(PI+dDegreeOf5))
							{
								if (iInput==0)
								{
									pt_2d_nearstart.y = pt_2d_start.y ;
								}
								else
								{
									pt_2d_nearend.y = pt_2d_end.y ;
								}								
							}
							else if (d1>(PI/2-dDegreeOf5)&&d1<(PI/2+dDegreeOf5)||d1>(PI*3/2-dDegreeOf5)&&d1<(PI*3/2+dDegreeOf5))
							{
								if (iInput==0)
								{
									pt_2d_nearstart.x = pt_2d_start.x ;
								}
								else
								{
									pt_2d_nearend.x = pt_2d_end.x ;
								}
							}

						}
					}
					else
					{
						//起点
						AcGeVector2d vec1 = pt_2d_nearstart-pt_2d_start ;
						double d1 = vec1.angle() ;
						if (d1<dDegreeOf5||d1>(PI*2-dDegreeOf5)||d1>(PI-dDegreeOf5)&&d1<(PI+dDegreeOf5))
						{
							pt_2d_nearstart.y = pt_2d_start.y ;
						}
						else if (d1>(PI/2-dDegreeOf5)&&d1<(PI/2+dDegreeOf5)||d1>(PI*3/2-dDegreeOf5)&&d1<(PI*3/2+dDegreeOf5))
						{
							pt_2d_nearstart.x = pt_2d_start.x ;
						}
						//终点
						AcGeVector2d vec2 = pt_2d_nearend-pt_2d_end ;
						double d2 = vec2.angle() ;
						if (d2<dDegreeOf5||d2>(PI*2-dDegreeOf5)||d2>(PI-dDegreeOf5)&&d2<(PI+dDegreeOf5))
						{
							pt_2d_nearend.y = pt_2d_end.y ;
						}
						else if (d2>(PI/2-dDegreeOf5)&&d2<(PI/2+dDegreeOf5)||d2>(PI*3/2-dDegreeOf5)&&d2<(PI*3/2+dDegreeOf5))
						{
							pt_2d_nearend.x = pt_2d_end.x ;
						}
					}

					pEnt->upgradeOpen() ;
					pPolyLine->setPointAt(1, pt_2d_nearstart) ;
					pPolyLine->setPointAt(iNumVert-2, pt_2d_nearend) ;
					pEnt->downgradeOpen() ;
				}
				else 
				{

				}				

				pEnt->close() ;
			}

		}

		acedSSFree(sum_name) ;
	}
	acutRelRb(pRb) ;
}
void CUserTools::RefreshDwg(void* pData)
{
	CString strFile ;
	strFile.Format(_T("%s"), (TCHAR*)pData) ;

	AcApDocument* pDoc = acDocManager->curDocument();
	if (acDocManager->isApplicationContext()) 
	{
		acDocManager->appContextOpenDocument((const TCHAR *)pData);
		//acDocManager->activateDocument(pDoc) ;
		AcApDocumentIterator *pIterDoc = acDocManager->newAcApDocumentIterator() ;
		for (;!pIterDoc->done(); pIterDoc->step())
		{
			AcApDocument *pDoc = pIterDoc->document() ;
			CString strDoc= pDoc->fileName() ;
			if (strDoc.Compare(strFile)==0)
			{
				//acDocManager->sendStringToExecute(pDoc, _T("-purge a \r n \r \003\003")) ;
				//begin 刷新居中保存
				acDocManager->sendStringToExecute(pDoc, _T("zoom e \r \003\003")) ;
				acDocManager->sendStringToExecute(pDoc, _T("_qsave  \r \003\003")) ;	
				acDocManager->sendStringToExecute(pDoc, _T("close \n")) ;	
				//end 刷新居中保存
				//pDoc->database()->saveAs(strFile) ;
				//acDocManager->closeDocument(pDoc) ;

// 				//begin 单位
// 				acDocManager->sendStringToExecute(pDoc, _T("insunits 4 \r \003\003")) ;
// 				acDocManager->sendStringToExecute(pDoc, _T("_qsave  \r \003\003")) ;	
// 				acDocManager->sendStringToExecute(pDoc, _T("close \n")) ;	
// 				//end 单位

			}	
		}
	} 
	else
	{
		acutPrintf(_T("\nERROR: in Document context : %s\n"),pDoc->fileName());
	}
}

int CUserTools::ReplaceEnt()
{
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;	
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	AcDbObjectId objIdClayer = pDb->clayer() ;
	mysys.Set_Layer(_T(""), _T("CRSCD_XTS_PMT_REPLACED")) ;
	mysys.SetLayerStatus(_T("CRSCD_XTS_PMT_REPLACED"), false, false, false, 3) ;

	int iOldOsMode = 0 ;
	mysys.GetSysVar(_T("OSMODE"), iOldOsMode) ; //当期对象捕捉设置值

	DWORD iType = 0 ;
	CBlkUtility::QueryValueReg(_T("PMTRBSET"),  _T("ToRbType"), iType) ;
	acutPrintf(_T("\nCur Select is %d"),  iType) ;

	if (iType==0) //信号机
	{
		mysys.SetSysVar(_T("OSMODE"), 32) ; //交点捕捉

		acutPrintf(_T("\n替换信号机")) ;

		CString strXhjBlkPath ;
		CBlkUtility::QueryValueReg(_T("PMTRBSET"), _T("XhjBlkPath"), strXhjBlkPath) ;
		if(strXhjBlkPath.Find(_T("XHJ_DT_"))!=-1)
		{
			mysys.SetSysVar(_T("OSMODE"), 544) ; //最近点捕捉(512)10+交点捕捉(32)6
		}
		CString strBlkName ;
		CBlkUtility::GetFileName(strXhjBlkPath, strBlkName) ;
		if (strBlkName.Left(3)==_T("XHJ"))
		{
			CString strXhjType = strBlkName.Mid(4,2) ;
			ads_point pt_insert  ;
			if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert)==RTNORM )
			{
				CString strName = _T("") ;
				double dAngle = 0 ;
				CString strBlkNameOld ;

				ads_name ssname, temp_name ;
				acutPrintf(_T("\n选择要替换的实体")) ;

				mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
				if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
				{
					long lLength = 0 ;
					acedSSLength(ssname, &lLength) ;
					for (int i=0; i<lLength; i++)
					{
						acedSSName(ssname, i, temp_name) ;
						AcDbEntity *pEnt = NULL ;
						if (mysys.OpenAcDbEntity(temp_name, pEnt))
						{
							if (pEnt->isKindOf(AcDbText::desc()) )
							{
								AcDbText *pText = AcDbText::cast(pEnt) ;
								strName = pText->textString() ;
							}
							else if (pEnt->isKindOf(AcDbBlockReference::desc()))
							{
								AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
								mysys.GetBlockName(pRef, strBlkNameOld) ;
								strBlkNameOld.MakeUpper() ;
								if (strBlkNameOld.Find(strXhjType)!=-1)
								{
									dAngle = pRef->rotation() ;
								}
							}
							pEnt->erase() ;	
							pEnt->close() ;
							pEnt = NULL ;
						}
					}
					acedSSFree(ssname) ;
				}

				AcDbBlockReference *pRef = NULL ;
				CStringArray strArrayVal ;	
				AcGePoint3d pt_3d_insert(0,0,0) ;
				mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
				strArrayVal.Add(strName) ;	
				if (mysys.InsertBlock(pRef, strXhjBlkPath, pt_3d_insert, &strArrayVal, FALSE, 1))
				{
					mysys.RotationEnt(pRef, dAngle, pt_3d_insert) ;
					pRef->close() ;
					pRef = NULL ;
				}
				if (g_bAddJyjWhenInsertXhj)
				{
					if (mysys.InsertBlock(pRef, _T("JYJ_PT_0"), pt_3d_insert))
					{
						mysys.RotationEnt(pRef, dAngle, pt_3d_insert) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
		}
		else
		{
			AfxMessageBox(_T("请运行pmtrbset设置要替换的信号机块！（注意选取的块文件名必须是XHJ_开头的）")) ;
		}
	}
	else if (iType==1) //道岔
	{
		mysys.SetSysVar(_T("OSMODE"), 33) ; //端点、交点捕捉

		acutPrintf(_T("\n替换道岔")) ;
		ads_point pt_insert  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert)==RTNORM )
		{
			pt_insert[2] = 0 ;
			ads_point pt_direct ;
			mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
			if (acedGetPoint(NULL, _T("\n指定一个道岔朝向点"), pt_direct)==RTNORM )
			{
				pt_direct[2] = 0 ;
				CString strName = _T("xxx") ;	
				//删除以pt_3d1为中心一定范围的实体
				CLongArray iArrayHandle ;
				ads_point pt1 = {pt_insert[0]-5, pt_insert[1]-5, 0}, pt2 = {pt_insert[0]+5, pt_insert[1]+5, 0} ;
				mysys.GetSSEntHandle(_T("W"), &pt1, &pt2, NULL, iArrayHandle) ; 
				for (int i=0; i<iArrayHandle.GetCount(); i++)
				{
					AcDbEntity* pEnt = NULL ;
					if(mysys.OpenAcDbEntity(iArrayHandle.GetAt(i), pEnt))
					{
						if (pEnt->isKindOf(AcDbText::desc()) )
						{
							AcDbText *pText = AcDbText::cast(pEnt) ;
							strName = pText->textString() ;
						}

						pEnt->erase() ;
						pEnt->close() ;
					}
				}

				AcDbBlockReference *pRef = NULL ;
				CStringArray strArrayVal ;	
				AcGePoint3d pt_3d_insert(0,0,0) ;
				mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
				strArrayVal.Add(strName) ;	
				if (mysys.InsertBlock(pRef, _T("DC_00"), pt_3d_insert, &strArrayVal, FALSE))
				{				
					//道岔朝向
					if (pt_insert[0]>pt_direct[0]&&pt_insert[1]<pt_direct[1] ) //左上，基本型
					{
					}
					else if (pt_insert[0]<pt_direct[0]&&pt_insert[1]<pt_direct[1]) //右上
					{
						AcGePoint3d pt_3d_insert2(pt_3d_insert.x, pt_3d_insert.y+1, 0) ;
						mysys.MirrEnt(pRef, pt_3d_insert, pt_3d_insert2) ;
					}
					else if (pt_insert[0]>pt_direct[0]&&pt_insert[1]>pt_direct[1]) //左下
					{
						AcGePoint3d pt_3d_insert2(pt_3d_insert.x+1, pt_3d_insert.y, 0) ;
						mysys.MirrEnt(pRef, pt_3d_insert, pt_3d_insert2) ;
					}
					else if (pt_insert[0]<pt_direct[0]&&pt_insert[1]>pt_direct[1])//右下
					{
						AcGePoint3d pt_3d_insert2(pt_3d_insert.x, pt_3d_insert.y+100, 0) ;
						mysys.MirrEnt(pRef, pt_3d_insert, pt_3d_insert2) ;
						pt_3d_insert2.x = pt_3d_insert.x+100 ;
						pt_3d_insert2.y = pt_3d_insert.y ;
						mysys.MirrEnt(pRef, pt_3d_insert, pt_3d_insert2) ;
					}
					pRef->close() ;
					pRef = NULL ;
				}
			}
		}
	}
	else if (iType==2) //绝缘节
	{
		mysys.SetSysVar(_T("OSMODE"), 32) ; //交点捕捉

		int iJyjIndex = 0 ;
		CBlkUtility::QueryValueReg(_T("PMTRBSET"),  _T("JyjIndex"), iJyjIndex) ;

		int iJyjWithDist = 0 ;
		CBlkUtility::QueryValueReg(_T("PMTRBSET"),  _T("JyjWithDist"), iJyjWithDist) ;

		CString strBlkName ;
		CString strJyjTypes[4] = {_T("PT"), _T("DT"),  _T("JX"), _T("CX") } ;
		strBlkName.Format(_T("JYJ_%s_%d"), strJyjTypes[iJyjIndex], iJyjWithDist) ;

		acutPrintf(_T("\n替换绝缘节")) ;
		acutPrintf(_T("\n选择2个点确定绝缘节朝向")) ;
		ads_point pt_insert1  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert1)==RTNORM )
		{
			ads_point pt_insert2  ;
			mysys.SetSysVar(_T("OSMODE"), 1) ; //端点捕捉
			if (acedGetPoint(NULL, _T("\n选择第二点（确定绝缘节朝向）"), pt_insert2)==RTNORM )
			{
				AcGePoint3d pt_3d_1(0,0,0), pt_3d_2(0,0,0) ;
				mysys.ConvertPtAdsTo3D(pt_insert1, pt_3d_1) ;
				mysys.ConvertPtAdsTo3D(pt_insert2, pt_3d_2) ;
				AcGeVector3d vec = pt_3d_2-pt_3d_1 ;
				double dAngle = vec.angleOnPlane(AcGePlane::kXYPlane) ;

				//删除以pt_3d1为中心一定范围的实体
				CLongArray iArrayHandle ;
				AcGePoint3d pt_3d_min(pt_3d_1.x-3, pt_3d_1.y-3,0) ;
				AcGePoint3d pt_3d_max(pt_3d_1.x+3, pt_3d_1.y+3,0) ;
				ads_point pt1, pt2 ;
				mysys.ConvertPt3DToAds(pt_3d_min, pt1) ;
				mysys.ConvertPt3DToAds(pt_3d_max, pt2) ;
				mysys.GetSSEntHandle(_T("W"), &pt1, &pt2, NULL, iArrayHandle) ; //FindEntInArea遍历整个数据库，效率太低了
				for (int i=0; i<iArrayHandle.GetCount(); i++)
				{
					AcDbEntity* pEnt = NULL ;
					if(mysys.OpenAcDbEntity(iArrayHandle.GetAt(i), pEnt))
					{
						pEnt->erase() ;
						pEnt->close() ;
					}
				}				

				AcDbBlockReference *pRef = NULL ;
				BOOL bInsert = FALSE ;
				if (iJyjWithDist==1)
				{
					CStringArray strArray ;
					strArray.Add(_T("XXX")) ;
					bInsert = mysys.InsertBlock(pRef, strBlkName, pt_3d_1, &strArray, FALSE) ;
				}
				else
				{
					bInsert = mysys.InsertBlock(pRef, strBlkName, pt_3d_1, FALSE) ;
				}
				if (bInsert)
				{
					pRef->setRotation(dAngle-PI/2) ;
					pRef->close() ;
					pRef = NULL ;
				}
			}
		}
	}
	else if(iType==3) //警冲标
	{
		mysys.SetSysVar(_T("OSMODE"), 4) ; //圆心捕捉

		acutPrintf(_T("\n替换警冲标")) ;
		ads_point pt_insert  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点（小圆点）"), pt_insert)==RTNORM )
		{
			ads_name ssname, temp_name ;
			CString strName = _T("") ;
			AcGePoint3d pt_3d_text(0,0,0) ; //以此文本位置和选择基点对比确定警冲标方向（不绝对）
			acutPrintf(_T("\n选择要替换的实体")) ;

			mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
			if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
			{
				long lLength = 0 ;
				acedSSLength(ssname, &lLength) ;
				for (int i=0; i<lLength; i++)
				{
					acedSSName(ssname, i, temp_name) ;
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(temp_name, pEnt))
					{
						if (pEnt->isKindOf(AcDbText::desc()) )
						{
							AcDbText *pText = AcDbText::cast(pEnt) ;
							strName = pText->textString() ;
							pt_3d_text = pText->position() ;
						}
						pEnt->erase() ;	
						pEnt->close() ;
						pEnt = NULL ;
					}
				}
				acedSSFree(ssname) ;
			}

			AcDbBlockReference *pRef = NULL ;
			CStringArray strArrayVal ;	
			AcGePoint3d pt_3d_insert(0,0,0) ;
			mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
			strArrayVal.Add(strName) ;	
			if (mysys.InsertBlock(pRef, _T("PMT_JCB"), pt_3d_insert, &strArrayVal, FALSE))
			{
				if (pt_3d_text.x<=pt_3d_insert.x)
				{
					AcGePoint3d pt_3d_insert2(pt_3d_insert.x, pt_3d_insert.y+1, 0) ;
					mysys.MirrEnt(pRef, pt_3d_insert, pt_3d_insert2) ;
				}					
				pRef->close() ;
				pRef = NULL ;
			}			
		}
	}
	else if(iType>=4&&iType<=6) //地铁：无线接入点（AP）
	{
		mysys.SetSysVar(_T("OSMODE"), 3) ; //端点1，中点2
		mysys.Set_Layer(_T(""), _T("P_无线接入点")) ;
		mysys.SetLayerStatus(_T("P_无线接入点"), false, false, false, 3) ;

		acutPrintf(_T("\n替换无线AP")) ;
		ads_point pt_insert  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert)==RTNORM )
		{
			ads_name ssname, temp_name ;
			CString strName = _T("") ;
			acutPrintf(_T("\n选择要替换的实体")) ;

			mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
			if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
			{
				long lLength = 0 ;
				acedSSLength(ssname, &lLength) ;
				for (int i=0; i<lLength; i++)
				{
					acedSSName(ssname, i, temp_name) ;
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(temp_name, pEnt))
					{
						if (pEnt->isKindOf(AcDbText::desc()) )
						{
							AcDbText *pText = AcDbText::cast(pEnt) ;
							strName = pText->textString() ;
						}
						pEnt->erase() ;	
						pEnt->close() ;
						pEnt = NULL ;
					}
				}
				acedSSFree(ssname) ;
			}

			AcDbBlockReference *pRef = NULL ;
			CStringArray strArrayVal ;	
			AcGePoint3d pt_3d_insert(0,0,0) ;
			mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
			strArrayVal.Add(strName) ;	
			CString strBlkName ;
			strBlkName.Format(_T("DT_AP_%d00"), iType-4) ;
			if (mysys.InsertBlock(pRef, strBlkName, pt_3d_insert, &strArrayVal, FALSE))
			{				
				pRef->close() ;
				pRef = NULL ;
			}			
		}
	}
	else if(iType>=7&&iType<=9) //地铁：应答器
	{
		mysys.SetSysVar(_T("OSMODE"), 544) ; //交点6，最近点10
		mysys.Set_Layer(_T(""), _T("P_应答器")) ;
		mysys.SetLayerStatus(_T("P_应答器"), false, false, false, 4) ;

		acutPrintf(_T("\n替换应答器")) ;
		ads_point pt_insert  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert)==RTNORM )
		{
			ads_name ssname, temp_name ;
			CString strName = _T("") ;
			acutPrintf(_T("\n选择要替换的实体")) ;

			mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
			if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
			{
				long lLength = 0 ;
				acedSSLength(ssname, &lLength) ;
				for (int i=0; i<lLength; i++)
				{
					acedSSName(ssname, i, temp_name) ;
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(temp_name, pEnt))
					{
						if (pEnt->isKindOf(AcDbText::desc()) )
						{
							AcDbText *pText = AcDbText::cast(pEnt) ;
							strName = pText->textString() ;
						}
						pEnt->erase() ;	
						pEnt->close() ;
						pEnt = NULL ;
					}
				}
				acedSSFree(ssname) ;
			}

			AcDbBlockReference *pRef = NULL ;
			CStringArray strArrayVal ;	
			AcGePoint3d pt_3d_insert(0,0,0) ;
			mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
			strArrayVal.Add(strName) ;	
			CString strBlkName ;
			strBlkName.Format(_T("DT_YDQ_%d00"), iType-7) ;
			if (mysys.InsertBlock(pRef, strBlkName, pt_3d_insert, &strArrayVal, FALSE))
			{				
				pRef->close() ;
				pRef = NULL ;
			}			
		}
	}
	else if(iType>=10&&iType<=12) //地铁：计轴
	{
		mysys.SetSysVar(_T("OSMODE"), 512) ; //最近点10
		mysys.Set_Layer(_T(""), _T("P_计轴")) ;
		mysys.SetLayerStatus(_T("P_计轴"), false, false, false, 6) ;

		acutPrintf(_T("\n替换应答器")) ;
		ads_point pt_insert  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert)==RTNORM )
		{
			ads_name ssname, temp_name ;
			CString strName = _T("") ;
			acutPrintf(_T("\n选择要替换的实体")) ;

			mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
			if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
			{
				long lLength = 0 ;
				acedSSLength(ssname, &lLength) ;
				for (int i=0; i<lLength; i++)
				{
					acedSSName(ssname, i, temp_name) ;
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(temp_name, pEnt))
					{
						if (pEnt->isKindOf(AcDbText::desc()) )
						{
							AcDbText *pText = AcDbText::cast(pEnt) ;
							strName = pText->textString() ;
						}
						pEnt->erase() ;	
						pEnt->close() ;
						pEnt = NULL ;
					}
				}
				acedSSFree(ssname) ;
			}

			AcDbBlockReference *pRef = NULL ;
			CStringArray strArrayVal ;	
			AcGePoint3d pt_3d_insert(0,0,0) ;
			mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
			strArrayVal.Add(strName) ;	
			CString strBlkName ;
			strBlkName.Format(_T("DT_JZ_%d00"), iType-10) ;
			if (mysys.InsertBlock(pRef, strBlkName, pt_3d_insert, &strArrayVal, FALSE))
			{				
				pRef->close() ;
				pRef = NULL ;
			}			
		}
	}
	else if(iType==13) //地铁：短环线
	{
		mysys.SetSysVar(_T("OSMODE"), 544) ; //最近点10
		mysys.Set_Layer(_T(""), _T("P_短环线")) ;
		mysys.SetLayerStatus(_T("P_短环线"), false, false, false, 3) ;

		acutPrintf(_T("\n替换短环线")) ;
		ads_point pt_insert  ;
		if (acedGetPoint(NULL, _T("\n选择插入基点"), pt_insert)==RTNORM )
		{
			ads_name ssname, temp_name ;
			CString strName = _T("") ;
			acutPrintf(_T("\n选择要替换的实体")) ;

			mysys.SetSysVar(_T("OSMODE"), 0) ; //无捕捉
			if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
			{
				long lLength = 0 ;
				acedSSLength(ssname, &lLength) ;
				for (int i=0; i<lLength; i++)
				{
					acedSSName(ssname, i, temp_name) ;
					AcDbEntity *pEnt = NULL ;
					if (mysys.OpenAcDbEntity(temp_name, pEnt))
					{
						if (pEnt->isKindOf(AcDbText::desc()) )
						{
							AcDbText *pText = AcDbText::cast(pEnt) ;
							strName = pText->textString() ;
						}
						pEnt->erase() ;	
						pEnt->close() ;
						pEnt = NULL ;
					}
				}
				acedSSFree(ssname) ;
			}

			AcDbBlockReference *pRef = NULL ;
			CStringArray strArrayVal ;	
			AcGePoint3d pt_3d_insert(0,0,0) ;
			mysys.ConvertPtAdsTo3D(pt_insert, pt_3d_insert) ;
			strArrayVal.Add(strName) ;	
			CString strBlkName ;
			strBlkName.Format(_T("DT_DHX_000")) ;
			if (mysys.InsertBlock(pRef, strBlkName, pt_3d_insert, &strArrayVal, FALSE))
			{				
				pRef->close() ;
				pRef = NULL ;
			}			
		}
	}


	pDb->setClayer(objIdClayer) ;
	mysys.SetSysVar(_T("OSMODE"), iOldOsMode) ;

	return 0;
}

int CUserTools::SetBlkAtt(int iMode)
{
	int iNum = 0 ;

	TCHAR chBlkName[100] ;
	TCHAR chTagName[100] ;
	TCHAR chAttValue[100] ;
	CString strBlkName, strTagName, strAttValue ;

	if (RTNORM == acedGetString(0, _T("\n块名:"), chBlkName))
	{
		strBlkName.Format(_T("%s"), chBlkName) ;
		if (RTNORM == acedGetString(0, _T("\n要修改的属性Tag:"), chTagName))
		{
			strTagName.Format(_T("%s"),  chTagName) ;
			if (RTNORM == acedGetString(0, _T("\n属性Value:"), chAttValue))
			{
				strAttValue.Format(_T("%s"), chAttValue) ;				
				//////////////////////////////////////////////////////////////////////////
				CUserSystem mysys ;

				if (iMode==0)
				{
					AcDbDatabase *pCurDb = acdbCurDwg() ;
					AcDbBlockTable * pBlkTbl = NULL ;
					pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);

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
							if (pEnt->isKindOf(AcDbBlockReference::desc()))
							{
								AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;

								//得到块名
								CString strBlockName ;
								mysys.GetBlockName(pRef, strBlockName) ;
								if (strBlockName==strBlkName)
								{
									if(mysys.SetAttValue(pRef, strTagName, strAttValue)==TRUE)
									{
										iNum++ ;
									}
								}								
							}
							pEnt->close() ;
						}						
					}
					delete pBlkTblRecIter ;
					pBlkTblRecIter = NULL ;											
				}
				else if (iMode==1)
				{
					ads_name sum_name  , temp_name  ;
					struct resbuf eb ;
					TCHAR chSbuf[10] ;
					const CString strDxf = _T("INSERT") ;
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
								if (pEnt->isKindOf(AcDbBlockReference::desc()))
								{
									AcDbBlockReference * pRef = (AcDbBlockReference *)pEnt ;

									//得到块名
									CString strBlockName ;
									mysys.GetBlockName(pRef, strBlockName) ;
									if (strBlockName==strBlkName)
									{
										if(mysys.SetAttValue(pRef, strTagName, strAttValue)==TRUE)
										{
											iNum++ ;
										}										
									}									
								}
								pEnt->close() ;				
							}							
						}
						ads_ssfree(sum_name) ;
					}					
				}				
				//////////////////////////////////////////////////////////////////////////			
			}		
		}
	}
	acutPrintf(_T("\n处理个数：%d"), iNum) ;

	return iNum ;
}

// 将起点x坐标大于终点x坐标的线（包括多段线）逆向反置一下（顺正）
void CUserTools::ReverseLinePoint(void)
{
	acutPrintf(_T("\n将起点在终点右侧的线顺成起点在左，终点在右。")) ;
	CUserSystem mysys ;

	struct resbuf *pRb ;
	pRb = acutBuildList(-4,_T("<OR"),RTDXF0,_T("LINE"),RTDXF0,_T("LWPOLYLINE"), -4,_T("OR>"), 0) ;
	long iLen = 0 ;
	ads_name sum_name ;
	if (Acad::eNormal==acedSSGet(NULL, NULL, NULL, pRb, sum_name))
	{	
		acedSSLength(sum_name, &iLen) ;
		for (int i=0; i<iLen; i++)
		{
			acutPrintf(_T("\n%d:"), i) ;
			ads_name en ;
			acedSSName(sum_name, i, en) ;
			AcDbEntity *pEnt = NULL ;			
			if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
			{
				mysys.ReverseLinePoint(pEnt, 0) ;

				pEnt->close() ;
			}
		}
		acedSSFree(sum_name) ;
	}
	acutRelRb(pRb) ;
}

// 给多段线增加顶点
void CUserTools::AddVertex(void)
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;
	ads_name en ;
	ads_point pt ;
	BOOL bReturn = false ;
	while (bReturn == false) 
	{
		int iReturn = acedEntSel(_T("\n选择要增加顶点的多段线"), en, pt) ;
		if(iReturn == RTNORM)
		{			
			AcDbEntity * pEnt = NULL ;
			if(mysys.OpenAcDbEntity(en, pEnt, AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbPolyline::desc()))
				{
					ads_point pt2 ;
					BOOL bReturn2 = false ;
					while (bReturn2 == false) 
					{
						int iReturn2 = acedGetPoint(pt, _T("\n输入要插入的顶点"), pt2) ;
						if (iReturn2==RTNORM)
						{
							AcDbPolyline* pPolyline = AcDbPolyline::cast(pEnt) ;
							pPolyline->upgradeOpen() ;
							AcGePoint2d pt_2d_start, pt_2d_end ;
							pPolyline->getPointAt(0, pt_2d_start) ;
							pPolyline->getPointAt(pPolyline->numVerts()-1, pt_2d_end) ;
							AcGePoint2d pt_2d_tmp(pt2[0], pt2[1]) ;
							UINT nVertexIndex = 0 ;
							if (mysys.IsPointOnPolyLine(pPolyline, pt_2d_tmp, nVertexIndex)!=-1)
							{
								pPolyline->addVertexAt(nVertexIndex+1, pt_2d_tmp) ;
							}
							else
							{
								if(pt_2d_tmp.distanceTo(pt_2d_start)<pt_2d_tmp.distanceTo(pt_2d_end))
								{							
									pPolyline->addVertexAt(0, pt_2d_tmp) ;
								}
								else
								{
									pPolyline->addVertexAt(pPolyline->numVerts(), pt_2d_tmp) ;
								}
							}
							pPolyline->downgradeOpen() ;		

							bReturn = TRUE ;
							bReturn2 = TRUE ;
						}	
						else if (iReturn2 == RTERROR) 
						{
							bReturn2 = false ;
						}	
						else if (iReturn2 == RTCAN)
						{
							bReturn2 = true ;
						}
					}
				}
				else 
				{
					acutPrintf(_T("\n选择的不是多段线")) ;
					bReturn = FALSE ;
				}
				pEnt->close() ;	
			}
		}
		else if (iReturn == RTERROR) 
		{
			bReturn = false ;
		}	
		else if (iReturn == RTCAN)
		{
			bReturn = true ;
		}
	}
}


// 炸开命名以A$开头的块
int CUserTools::ExplodeSpeBlk(void)
{
	AcDbDatabase *pCurDb = NULL ;
	pCurDb = acdbCurDwg() ;

	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	//炸开命名以A$开头的块

	AcDbBlockTable * pBlkTbl = NULL ;
	pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;

	int iExplodeNum = 0 ;

	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{		
			//处理图块 AcDbBlockReference ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				CString strBlkName ;
				AcDbBlockReference * pRef = (AcDbBlockReference*)pEnt ;
				mysys.GetBlockName(pRef, strBlkName) ;
				if (strBlkName.Left(2) == _T("A$"))
				{	
					mysys.Explode(pRef) ;										

					iExplodeNum++ ;
				}
			}
			pEnt->close() ;
		}
	}

	acutPrintf(_T("\n炸开以A$开头命名的块%d个"), iExplodeNum) ;
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;  

	return iExplodeNum;
}

// 增强版DDEDIT
void CUserTools::MyEdit(void)
{
	CAcModuleResourceOverride myResources ;	

	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	ads_name en ;
	ads_point pt ;
	BOOL bReturn = false ;

	while (bReturn == false) 
	{
		int iReturn = ads_entsel(_T("\n选择实体:"), en, pt) ;
		if(iReturn == RTNORM)
		{			
			AcDbEntity * pEnt = NULL ;
			if(mysys.OpenAcDbEntity(en, pEnt))
			{
				if(_tcscmp(pEnt->isA()->name(),_T("AcDbText")) == 0)
				{
					AcDbText * pText = NULL ;
					pText = (AcDbText *) pEnt ;
					CTextDlg dlg ;
					dlg.pEnt = pEnt ;

					dlg.pText = pText ;
					dlg.DoModal() ;	

					bReturn = true ;					
				}
				else
				{
					AfxMessageBox(_T("选择的不是文本！")) ;
					bReturn = false ;
				}
			}
			pEnt->close() ;	
		}
		else if (iReturn == RTERROR) 
		{
			bReturn = false ;
		}	
		else if (iReturn == RTCAN) {
			bReturn = true ;
		}
	}
}

// 类cad中单行文本text命令
void CUserTools::MyText(void)
{
	AcDbDatabase * pCurDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	ads_point selectPt ;
	if (RTNORM == ads_getpoint(NULL, _T("\n选择一个输入点:"), selectPt))
	{
		CInputTextDlg inputtextdlg ;
		mysys.GetTextStyleArray(inputtextdlg.m_strArrayTextStyle) ;

		if (inputtextdlg.DoModal()==IDOK)
		{
			ads_point base_pt = {0, 0, 0} ;
			base_pt[0] = selectPt[0]+inputtextdlg.m_dXOffSet ;
			base_pt[1] = selectPt[1]+inputtextdlg.m_dYOffSet ;

			AcDbTextStyleTable * pTextStyleTb = NULL ;
			pCurDb->getTextStyleTable(pTextStyleTb, AcDb::kForRead) ;
			AcDbObjectId idTextStyle ;
			if (Acad::eOk!=pTextStyleTb->getAt(inputtextdlg.m_strTextStyle, idTextStyle))
			{
				idTextStyle = NULL ;
			}
			pTextStyleTb->close() ;			

			AcDbObjectId objId ;

			//mysys.WriteText(base_pt,inputtextdlg.m_strText, objId, inputtextdlg.m_dHeight, inputtextdlg.m_dWidth,AcDb::kTextLeft, AcDb::kTextBase, g_inputtext_iColorIndex, idTextStyle, inputtextdlg.m_dRotation) ;
			mysys.AddText(objId, inputtextdlg.m_strText, base_pt, base_pt, AcDb::kTextLeft, AcDb::kTextBase, inputtextdlg.m_dHeight, inputtextdlg.m_dWidth, inputtextdlg.m_dRotation, g_inputtext_iColorIndex, idTextStyle) ;

			AcDbEntity * pEnt = NULL ;
			if (Acad::eOk==acdbOpenObject(pEnt,objId,AcDb::kForWrite)) 
			{
				AcDbText * pText = NULL ;
				AcGePoint3d alignP_3d(0, 0, 0) ;
				alignP_3d.x = base_pt[0] ;
				alignP_3d.y = base_pt[1] ;
				pText = (AcDbText *)pEnt ;

				int iTextMode = g_inputtext_iJustifyIndex ;
				switch(iTextMode)
				{
				case 0 :
				case 1:						
					break;
				case 2:
					{
						pText->setHorizontalMode(AcDb::kTextCenter) ;
						//pText->setVerticalMode(AcDb::kTextVertMid) ;			
					}
					break;
				case 3:
					{
						pText->setHorizontalMode(AcDb::kTextRight) ;
						//pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 4:
					{
						pText->setHorizontalMode(AcDb::kTextAlign) ;
						//pText->setHeight(g_inputtext_dHeight) ;
						//pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 5:
					{
						pText->setHorizontalMode(AcDb::kTextMid) ;
						//pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 6:
					{
						pText->setHorizontalMode(AcDb::kTextFit) ;							
						//pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 7:
					{
						pText->setHorizontalMode(AcDb::kTextLeft) ;
						pText->setVerticalMode(AcDb::kTextTop) ;
					}
					break;
				case 8:
					{
						pText->setHorizontalMode(AcDb::kTextCenter) ;
						pText->setVerticalMode(AcDb::kTextTop) ;
					}
					break;
				case 9:
					{
						pText->setHorizontalMode(AcDb::kTextRight) ;
						pText->setVerticalMode(AcDb::kTextTop) ;
					}
					break;
				case 10:
					{
						pText->setHorizontalMode(AcDb::kTextLeft) ;
						pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 11:
					{
						pText->setHorizontalMode(AcDb::kTextCenter) ;
						pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 12:
					{
						pText->setHorizontalMode(AcDb::kTextRight) ;
						pText->setVerticalMode(AcDb::kTextVertMid) ;
					}
					break;
				case 13:
					{
						pText->setHorizontalMode(AcDb::kTextLeft) ;
						pText->setVerticalMode(AcDb::kTextBottom) ;
					}
					break;
				case 14:
					{
						pText->setHorizontalMode(AcDb::kTextCenter) ;
						pText->setVerticalMode(AcDb::kTextBottom) ;
					}
					break;
				case 15:
					{
						pText->setHorizontalMode(AcDb::kTextRight) ;
						pText->setVerticalMode(AcDb::kTextBottom) ;
					}
					break;
				default:
					break;
				}

				pText->setAlignmentPoint(alignP_3d) ;					

			} 
			pEnt->close() ;	

		}
	}
}

// 批量输入文本
void CUserTools::BText(void)
{
	CAcModuleResourceOverride myResources ;

	AcDbDatabase * pCurDb = acdbHostApplicationServices()->workingDatabase() ;

	ads_point pt1 , pt2 ;
	BOOL bReturn1 = false ;
	BOOL bReturn2 = false ;

	while (bReturn1 == false) 
	{
		int iReturn1 = ads_getpoint(NULL,_T("\n选择起始点"),pt1) ;

		if(iReturn1 == RTNORM)
		{
			bReturn1 = true ;

			while (bReturn2 == false)
			{
				int iReturn2 = ads_getpoint(NULL, _T("\n选择第二个点"), pt2) ;
				if (iReturn2 == RTNORM)
				{
					bReturn2 = true ;

					CBatchDTextDlg dlg ;
					//dlg.pDb = pCurDb ;
					dlg.startp_2d[0] = pt1[0] ;
					dlg.startp_2d[1] = pt1[1] ;
					dlg.endp_2d[0] = pt2[0] ;
					dlg.endp_2d[1] = pt2[1] ;

					dlg.DoModal() ;					

				}
				else if (iReturn2 == RTERROR)
				{
					bReturn2 = false ;
				}
				else if (iReturn2 == RTCAN)
				{
					bReturn2 = true ;
				}
			}

		}
		else if (iReturn1 == RTERROR) 
		{
			bReturn1 = false ;
		}	
		else if (iReturn1 == RTCAN) 
		{
			bReturn1 = true ;
		}		

	}
}

// cad命令find增强版
void CUserTools::MyFind(void)
{
	CAcModuleResourceOverride myResources ;	

	CFindReplaceDlg dlg ;
	int nRet = dlg.DoModal();

	if (nRet == IDOK)
	{
		if (dlg.m_iRange!=2)
		{
			AcDbDatabase * pDb ;
			pDb = acdbCurDwg() ;

			CUserSystem mySys ;
			mySys.m_pDb = pDb ;

			int iNum = 0 ;
			iNum = mySys.FindAndReplaceText(g_strFindTxt, g_strReplaceTxt, FALSE, g_bCaseSensitive, dlg.m_bReg,dlg.m_iRange, g_iColorSel, 0) ;
			ads_printf(_T("\n共处理%d个文本"), iNum) ;
		}
		else
		{
			CUserSystem mysys ;
			CStringArray strArrayFile ;
			int iNum = 0 ;

			CBlkUtility::FindFile(dlg.m_strFilePath,_T("dwg"),strArrayFile) ;

			int iFileNum = strArrayFile.GetSize() ;

			acedSetStatusBarProgressMeter(_T("开始查找替换......"), 0, iFileNum-1) ;	

			for(int i=0; i<iFileNum; i++)
			{
				acedSetStatusBarProgressMeterPos(i) ;

				CString strFilePath ;
				strFilePath = strArrayFile.GetAt(i) ;
				ads_printf(_T("\n正在处理图纸:%s"), strFilePath) ;
				AcDbDatabase *pDb = new AcDbDatabase(false) ;
				if (Acad::eOk==pDb->readDwgFile(strFilePath))
				{
					mysys.m_pDb = pDb ;
					int iNumCur = mysys.FindAndReplaceText(g_strFindTxt, g_strReplaceTxt, FALSE, g_bCaseSensitive, dlg.m_bReg,1 , g_iColorSel, 0) ;
					iNum += iNumCur ;
					ads_printf(_T("\n当前图纸共处理%d处文本"), iNumCur) ;
					if (iNumCur>0)
					{
						pDb->saveAs(strFilePath) ;
					}
				}

				delete pDb ;
				pDb = NULL ;
			}

			acedRestoreStatusBar() ;

			ads_printf(_T("\n共处理%d个文本"), iNum) ;
		}

	}
}

// 属性文本和多行文本转换为单行文本
void CUserTools::AtMtToTxt(void)
{
	CAcModuleResourceOverride myResources ;	

	CConvertTextDlg dlg ;
	dlg.DoModal() ;
}

// 批量修改文字大小、style、对齐方式
void CUserTools::MoText(void)
{
	CAcModuleResourceOverride myResources ;	

	AcDbDatabase * pDb ;
	pDb = acdbCurDwg() ;

	CUserSystem mySys ;
	mySys.m_pDb = pDb ;

	ads_name sum_name  , temp_name  ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("TEXT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf ( _T("\n选择文本:\n ") ) ;

	if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
	{
		CModifyTextStyleDlg modifytextdlg ;
		mySys.GetTextStyleArray(modifytextdlg.m_strArrayTextStyle) ;

		long lLength = 0 ;
		ads_sslength(sum_name, &lLength) ;

		//选择单个文本时将默认显示该文本的属性
		if (lLength==1)
		{
			ads_ssname(sum_name, 0, temp_name) ;
			AcDbEntity * pEnt = NULL ;			
			if (mySys.OpenAcDbEntity(temp_name, pEnt))
			{
				AcDbText * pText = AcDbText::cast(pEnt) ;
				modifytextdlg.m_dHeight = pText->height() ;
				modifytextdlg.m_dRotation = pText->rotation() ;
				modifytextdlg.m_dWidthFactor = pText->widthFactor() ;
				AcDb::TextHorzMode modeHor = pText->horizontalMode() ;
				AcDb::TextVertMode modeVert = pText->verticalMode() ;
				int iMode = 0 ;
				mySys.ConvertTextModeToIndex(modeHor, modeVert, iMode) ;
				modifytextdlg.m_iTextJustifyCurSel = iMode+1 ;

				CString strStyleName ;
				mySys.GetTextStyle(pText, strStyleName) ;
				int iStyleLoc = CBlkUtility::FindStrInArray(modifytextdlg.m_strArrayTextStyle, strStyleName) ;
				modifytextdlg.m_iTextStyleCurSel =iStyleLoc+1 ;

				pEnt->close() ;
			}
		}

		if (modifytextdlg.DoModal()==IDOK) 
		{
			//double dHeight = -1 ;
			//double dWidthFactor = -1 ;
			//double dRotation = -1 ;
			CString strTextStyle ;
			int iTextModeNew = -1 ;
			int iTextStyleIndex = -1 ;

			//justify and style index;
			iTextModeNew = modifytextdlg.m_iTextJustifyCurSel - 1 ;
			iTextStyleIndex = modifytextdlg.m_iTextStyleCurSel ;
			strTextStyle = modifytextdlg.m_strTextStyle ;		

			//style's record objectid;
			AcDbTextStyleTable * pTextStyleTb = NULL ;
			pDb->getTextStyleTable(pTextStyleTb, AcDb::kForRead) ;
			AcDbObjectId idTextStyle ;
			pTextStyleTb->getAt(strTextStyle,idTextStyle) ;
			pTextStyleTb->close() ;		

			//文字高、宽幅、转向;
			//dHeight = modifytextdlg.m_dHeight ;
			//dWidthFactor = modifytextdlg.m_dWidthFactor ;
			//dRotation = modifytextdlg.m_dRotation ;

			for(int i = 0; i<lLength; i++)
			{
				ads_ssname(sum_name, i, temp_name) ;
				AcDbEntity * pEnt = NULL ;			
				if (mySys.OpenAcDbEntity(temp_name, pEnt))
				{
					AcDbText * pText = NULL ;

					if (_tcscmp(pEnt->isA()->name(),_T("AcDbText")) == 0)
					{	
						pText = (AcDbText *)pEnt ;

						if (iTextStyleIndex>0) 
						{
							pText->setTextStyle(idTextStyle) ;
						}
						if (modifytextdlg.m_bSet)
						{
							pText->setHeight(modifytextdlg.m_dHeight) ;
							pText->setWidthFactor(modifytextdlg.m_dWidthFactor) ;
							pText->setRotation(modifytextdlg.m_dRotation) ;	
						}
						if (iTextModeNew>-1)
						{
							mySys.ConvertTextMode(pText, iTextModeNew) ;
						}
					}

				}
				pEnt->close() ;
				pEnt = NULL ;
			}
		} 

		ads_ssfree(sum_name) ;
	}
	else
	{
		AfxMessageBox(_T("没有选择任何文本!")) ;
	}
}

// 对齐文本
void CUserTools::AlignText(void)
{
	CAcModuleResourceOverride myResources ;

	AcDbDatabase * pDb ;
	pDb = acdbCurDwg() ;

	CUserSystem mySys ;
	mySys.m_pDb = pDb ;	

	ads_name sum_name  , temp_name  ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("TEXT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf ( _T("\n选择文本:\n ") ) ;

	if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
	{
		int iTextModeNew = 4 ;
		int iAligned = 0 ;
		ads_point selectPt = {0, 0, 0} ;
		if (RTNORM == ads_getpoint(NULL, _T("\n选择一个对齐点:"), selectPt))
		{	
			ads_getint(_T("\n对齐方式:0(Left)/1(Center)/2(Right)/3(Aligned)4(Middle默认)/5(Fit)/6(TopLeft)/7(TopCenter)/8(TopRight)/9(MiddleLeft)/10(MiddleCenter)/11(MiddleRight)/12(BottomLeft)/13(BottomCenter)/14(BottomRight):"), &iTextModeNew) ;
			ads_getint(_T("\n对齐参照:0(水平对齐)/1(垂直对齐):"), &iAligned) ;
			long lLength = 0 ;
			ads_sslength(sum_name, &lLength) ;			

			for(int i = 0; i<lLength; i++)
			{
				ads_ssname(sum_name, i, temp_name) ;
				AcDbEntity * pEnt = NULL ;			
				if (mySys.OpenAcDbEntity(temp_name, pEnt))
				{
					AcDbText * pText = NULL ;

					if (_tcscmp(pEnt->isA()->name(),_T("AcDbText")) == 0)
					{	
						pText = (AcDbText *)pEnt ;	
						//CString strP_3d ;
						mySys.ConvertTextMode(pText, iTextModeNew) ;
					}
				}
				pEnt->close() ;
				pEnt = NULL ;
			}

			for(int i2=0; i2<lLength; i2++)
			{
				ads_ssname(sum_name, i2, temp_name) ;
				AcDbEntity * pEnt = NULL ;
				if (mySys.OpenAcDbEntity(temp_name, pEnt)) 
				{
					AcDbText * pText = NULL ;
					if (_tcscmp(pEnt->isA()->name(),_T("AcDbText"))==0)
					{
						pText = (AcDbText *)pEnt ;
						AcGePoint3d textAlignPt_3d(0, 0, 0) ;
						AcGePoint3d textPositionPt_3d(0, 0, 0) ;

						if (iTextModeNew==0)
						{
							textPositionPt_3d = pText->position() ;
							if (iAligned==0)
							{
								textPositionPt_3d.y = selectPt[1] ;
							}
							else 
							{
								textPositionPt_3d.x = selectPt[0] ;
							}
							pText->setPosition(textPositionPt_3d) ;
						}
						else if (iTextModeNew==3||iTextModeNew==5) 
						{
							textAlignPt_3d = pText->alignmentPoint() ;
							textPositionPt_3d = pText->position() ;
							if (iAligned==0)
							{
								textAlignPt_3d.y = selectPt[1] ;
								textPositionPt_3d.y = selectPt[1] ;
							}
							else
							{
								textPositionPt_3d.x = selectPt[0]-(textAlignPt_3d.x-textPositionPt_3d.x) ;
								textAlignPt_3d.x = selectPt[0] ;

							}
							pText->setAlignmentPoint(textAlignPt_3d) ;
							pText->setPosition(textPositionPt_3d) ;
						}
						else
						{
							textAlignPt_3d = pText->alignmentPoint() ;
							if (iAligned==0)
							{
								textAlignPt_3d.y = selectPt[1] ;
							}
							else 
							{
								textAlignPt_3d.x = selectPt[0] ;
							}
							pText->setAlignmentPoint(textAlignPt_3d) ;
						}

					}
				}
				pEnt->close() ;
				pEnt = NULL ;

			}

			ads_ssfree(sum_name) ;

		}

	}
	else
	{
		AfxMessageBox(_T("没有选择任何文本!")) ;
	}
}

// 批量修改文本
void CUserTools::BEdit(void)
{
	CAcModuleResourceOverride myResources ;

	AcDbDatabase * pDb ;
	pDb = acdbCurDwg() ;

	CUserSystem mySys ;
	mySys.m_pDb = pDb ;

	int iNum = 0 ;	

	CArray<AcDbText *, AcDbText *> arrayent ;	

	ads_name sum_name  , temp_name  ;
	struct resbuf eb ;
	TCHAR chSbuf[10] ;
	const CString strDxf = _T("TEXT") ;
	CBlkUtility::tcscpy( chSbuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
	eb.restype = 0 ;
	eb.resval .rstring = chSbuf ;
	eb.rbnext = NULL ;
	ads_printf ( _T("\n选择实体: ") ) ;

	CSTextSet dlg ;
	if (dlg.DoModal()==IDOK)
	{
		//////////////////////////////////////////////////////////////////////////		
		//点选 ;
		if (g_iSTxtSetSelectMode==1)
		{
			if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
			{
				long lLength = 0 ;
				ads_sslength(sum_name, &lLength) ;			

				for(int i = 0; i<lLength; i++)
				{
					ads_ssname(sum_name, i, temp_name) ;
					AcDbEntity * pEnt = NULL ;			
					if (mySys.OpenAcDbEntity(temp_name, pEnt))
					{
						AcDbText * pText = NULL ;

						if (_tcscmp(pEnt->isA()->name(),_T("AcDbText")) == 0)
						{	
							pText = (AcDbText *)pEnt ;	
							CString strSet ;
							int iTmpNum = g_iSTxtSetStart + (i/g_iSTxtSetRepeat)*g_iSTxtSetIncrement ;
							strSet.Format(_T("%s%d%s"), g_strSTxtSetFront, iTmpNum, g_strSTxtSetBack) ;
							//arrayent.Add(pText) ;
							pText->setTextString(strSet) ;		
						}
					}
					pEnt->close() ;					
				}
				ads_ssfree(sum_name) ;
			}
			else
			{
				AfxMessageBox(_T("没有选择任何实体!")) ;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//框选 ;
		else if (g_iSTxtSetSelectMode==0)
		{
			if( RTNORM == ads_ssget ( NULL , NULL , NULL , &eb , sum_name ) )
			{
				long lLength = 0 ;
				ads_sslength(sum_name, &lLength) ;			

				for(int i = 0; i<lLength; i++)
				{
					ads_ssname(sum_name, i, temp_name) ;
					AcDbEntity * pEnt = NULL ;			
					if (mySys.OpenAcDbEntity(temp_name, pEnt))
					{
						AcDbText * pText = NULL ;						
						if (_tcscmp(pEnt->isA()->name(),_T("AcDbText")) == 0)
						{	
							pText = (AcDbText *)pEnt ;	

							arrayent.Add(pText) ;	
						}
					}
					pEnt->close() ;					
				}
				ads_ssfree(sum_name) ;
			}
			else
			{
				AfxMessageBox(_T("没有选择任何实体!")) ;
			}

			mySys.SortEnt(arrayent, dlg.m_iSortBy+1) ;

			CStringArray strArrayTest;

			for(int it=0; it<arrayent.GetSize(); it++)
			{

				CString strSet ;
				int iTmpNum = g_iSTxtSetStart + (it/g_iSTxtSetRepeat)*g_iSTxtSetIncrement ;
				strSet.Format(_T("%s%d%s"), g_strSTxtSetFront, iTmpNum, g_strSTxtSetBack) ;

				AcDbText * pTextTest = NULL ;
				AcDbEntity * pEnttest ;
				pTextTest = arrayent.GetAt(it) ;
				acdbOpenAcDbEntity(pEnttest, pTextTest->objectId(), AcDb::kForWrite) ;
				AcDbText * pTextTest1 = (AcDbText *)pEnttest ;
				pTextTest1->setTextString(strSet) ;
				pEnttest->close() ;
			}

		}
		//////////////////////////////////////////////////////////////////////////

	}
}

// 删除选中的文本的xdata
void CUserTools::DelSelXdata(void)
{
	CUserSystem mysys ;
	ads_name sum_name , temp_name ;

	ads_printf( _T("\n请选择要删除属性的文本\n ") ) ;
	if( RTNORM == acedSSGet( NULL , NULL , NULL , NULL , sum_name ) )
	{
		long lLength = 0 ;
		acedSSLength(sum_name, &lLength) ;
		for(int i = 0; i<lLength; i++)
		{
			ads_ssname(sum_name, i, temp_name) ;
			AcDbEntity * pEnt = NULL ;
			if (mysys.OpenAcDbEntity(temp_name, pEnt))
			{
				//if (_tcscmp(pEnt->isA()->name(),_T("AcDbText")) == 0)
				//{
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
								mysys.DeleteXdata(pEnt, strAppName) ;

								ads_printf(_T("删除属性:%s\n"),strAppName) ;

							}							
							break;
						default:
							//ads_printf("unknown type\n") ;
							break;
						}							
					}
					ads_relrb(pRb);
					pRb = NULL ;

					pEnt->setColorIndex(7) ;
				}
				else
				{
					ads_printf(_T("\nNo xdata for this appname"));
				}
				//}
				pEnt->close() ;
				pEnt = NULL ;
			}
		}
		ads_ssfree(sum_name) ;
	}

}

// excel表格转换成cad中的表格
void CUserTools::XlsToDwg(void)
{
	CAcModuleResourceOverride myResources ;	

	CBlkExcelTool exceltool ;
	CUserSystem mySys ;
	ads_point base_pt = {0,0,0};

	CXlsToDwgDlg dlg ;
	if (dlg.DoModal()==IDOK)
	{
		ads_printf(_T("%d"), dlg.m_iConvMode) ;
		int iConvMode = dlg.m_iConvMode ;
		CString strMark = dlg.m_strMark ;
		strMark.TrimLeft() ;
		strMark.TrimRight() ;

		ads_getpoint(NULL,_T("\n选择起始点"),base_pt) ;

		TCHAR cStartCol[2], cEndCol[2] ;			
		//_tcscpy_s(cStartCol, dlg.m_strStartCol) ;
		CBlkUtility::tcscpy( cStartCol , (dlg.m_strStartCol.GetLength()+1)*sizeof(TCHAR), dlg.m_strStartCol ) ;
		//_tcscpy_s(cEndCol, dlg.m_strEndCol) ;	
		CBlkUtility::tcscpy( cEndCol , (dlg.m_strEndCol.GetLength()+1)*sizeof(TCHAR), dlg.m_strEndCol ) ;

		//对指定范围内的所有单元格进行转换
		if (iConvMode==0)
		{			
			mySys.ExcelToDwg(dlg.m_strExcelFileLoc,dlg.m_iStartRow,dlg.m_iEndRow, cStartCol,cEndCol,base_pt) ;
		}		
		//查找标示表头部分，进行转换		
		else if(iConvMode==1 && strMark!=_T(""))
		{			
			CArray<CExcelRange*, CExcelRange*> arrayexcelrange ;
			CArray<double, double> arrayWidth ;
			int iRetNum = exceltool.GetExcelRange(dlg.m_strExcelFileLoc, strMark, arrayWidth,arrayexcelrange, dlg.m_iStartRow, dlg.m_iEndRow, cStartCol, cEndCol ) ;

			int iStartRowTmp = 0 ;
			int iEndRowTmp = 0 ;
			int iStartColTmp = NULL ;
			int iEndColTmp = NULL ;

			for(int i=0; i<iRetNum; i++)
			{

				iStartRowTmp = (arrayexcelrange.GetAt(i))->iStartRow ;
				iEndRowTmp = (arrayexcelrange.GetAt(i))->iEndRow ;
				iStartColTmp = (arrayexcelrange.GetAt(i))->iStartCol ;
				iEndColTmp = (arrayexcelrange.GetAt(i))->iEndCol ;

				CString  chStartColTmp  ;
				CString chEndColTmp  ;
				chStartColTmp = CBlkUtility::GetExcelFieldName(iStartColTmp) ;
				chEndColTmp = CBlkUtility::GetExcelFieldName(iEndColTmp) ;

				TCHAR cStartCol1[2], cEndCol1[2] ;
				//_tcscpy_s(cStartCol1, chStartColTmp) ;
				CBlkUtility::tcscpy( cStartCol1 , (chStartColTmp.GetLength()+1)*sizeof(TCHAR), chStartColTmp ) ;
				//_tcscpy_s(cEndCol1, chEndColTmp) ;	
				CBlkUtility::tcscpy( cEndCol1 , (chEndColTmp.GetLength()+1)*sizeof(TCHAR), chEndColTmp ) ;


				ads_printf(_T("\n开始处理第%d处表格\n"),i) ;
				mySys.ExcelToDwg(dlg.m_strExcelFileLoc, iStartRowTmp, iEndRowTmp, cStartCol1, cEndCol1, base_pt) ;

				base_pt[0]=base_pt[0] + arrayWidth.GetAt(i) + 5 ;				
			}

			ads_printf(_T("共处理了%d处表格"),iRetNum) ;

			for(int i2=0; i2<iRetNum; i2++)
			{
				delete arrayexcelrange[i2] ;
				arrayexcelrange[i2] = NULL ;
			}
		}
	} 
}

// 设置扩展数据xdata
void CUserTools::SetXdata(void)
{
	CUserSystem mysys ;
	AcDbDatabase *pDb = acdbCurDwg() ;
	mysys.m_pDb = pDb ;

	AcDbObjectId layer_id ;
	layer_id = mysys.Set_Layer(_T(""), _T("define")) ;

	ads_name ssname, temp_name ;
	ads_point pt ;

	acutPrintf(_T("\n选择实体")) ;
	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		int iMode = -1 ; //0:int 1:str 2:long	

		TCHAR chAppName[100] ;
		TCHAR chAppType[100] ;
		int iXdata = -1 ;
		long lXdata = 0 ;
		TCHAR chXdata[100]  ;
		if (RTNORM==acedGetString(1, _T("\n扩展数据名："), chAppName))
		{				
			if (RTNORM==acedGetString(1, _T("\n扩展数据类型[int(i)/str(s)long(l)]"), chAppType))
			{					
				BOOL bAddRet = FALSE ;

				CString strAppType ;
				strAppType.Format(_T("%s"), chAppType) ;
				strAppType.MakeLower() ;

				if (strAppType==_T("int") || strAppType==_T("i") )
				{
					if (RTNORM==acedGetInt(_T("\n输入扩展数据："), &iXdata))
						//if (RTNORM==acedGetString(1, _T("\n输入扩展数据："), chXdata))
					{
						//iXdata = _ttoi(chXdata) ;
						iMode = 0 ;
						//bAddRet = mysys.SetXdata(pTemEnt, chAppName, iXdata) ;
					}
				}	
				else if (strAppType==_T("str") || strAppType==_T("s"))
				{

					if (RTNORM==acedGetString(1, _T("\n输入扩展数据："), chXdata))
					{
						iMode = 1 ;
						//bAddRet = mysys.SetXdata(pTemEnt, chAppName, chXdata) ;
					}
				}
				if (strAppType==_T("long") || strAppType==_T("l") )
				{
					if (RTNORM==acedGetString(1, _T("\n输入扩展数据："), chXdata))
					{
						lXdata = _ttol(chXdata) ;
						iMode = 2 ;
						//bAddRet = mysys.SetXdata(pTemEnt, chAppName, iXdata) ;
					}
				}	
			}			
		}

		if (iMode>-1)
		{
			long lLength = 0 ;
			acedSSLength(ssname, &lLength) ;
			for (int i=0; i<lLength; i++)
			{
				acedSSName(ssname, i, temp_name) ;
				AcDbEntity *pEnt = NULL ;
				if (mysys.OpenAcDbEntity(temp_name, pEnt))
				{
					switch (iMode)
					{
					case 0:
						mysys.SetXdata(pEnt, chAppName, iXdata) ;
						break;
					case 1:
						mysys.SetXdata(pEnt, chAppName, chXdata) ;
						break ;
					case 2:
						mysys.SetXdata(pEnt, chAppName, lXdata) ;
						break ;
					}
					//pEnt->setColorIndex(3) ;
					pEnt->close() ;
					pEnt = NULL ;
				}
			}
		}
		acedSSFree(ssname) ;
	}
	else
	{
		AfxMessageBox(_T("没有选中任何实体")) ;
	}
}

// 显示扩展数据xdata
void CUserTools::ShowXdata(void)
{	
	//先删除以前的显示
	CUserTools::HideXdata() ;

	CUserSystem mysys ;
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	mysys.m_pDb = pCurDb ;

	Acad::ErrorStatus es;
	AcDbBlockTable * pBlkTbl = NULL ;
	AcDbBlockTableRecord * pBlkTblRec = NULL ;

	es = pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	assert(es==Acad::eOk) ;

	es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
	assert(es==Acad::eOk) ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;	

	AcDbObjectId lay_Id ;
	lay_Id = mysys.Set_Layer( _T("") , _T("AppUse_ShowXdata")) ;

	CArray<AcDbObjectId, AcDbObjectId&> arrayObjIdShowXdata ; //显示的xdata文本的objid
	arrayObjIdShowXdata.SetSize(200, 100) ;

	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{			
			CString strEntType ;
			strEntType.Format(_T("%s"), pEnt->isA()->name()) ;

			CString strXData = _T("") ;

			struct resbuf *pRb, *pTmp;
			pRb = pEnt->xData(NULL);
			if (pRb != NULL) 
			{
				for(pTmp=pRb; pTmp!=NULL; pTmp = pTmp->rbnext)
				{
					switch(pTmp->restype) 
					{
					case 1001:
						{
							CString strTmp ;
							strTmp.Format(_T("%s="), pTmp->resval.rstring) ;	
							strXData+=strTmp ;
						}							
						break;
					case 1000:
						{
							CString strTmp ;
							strTmp.Format(_T("%s;"), pTmp->resval.rstring) ;	
							strXData+=strTmp ;							
						}
						break;
					case 1070:
						{
							CString strTmp ;
							strTmp.Format(_T("%d;"), pTmp->resval.rint) ;	
							strXData+=strTmp ;							
						}
						break;
					case 1071:
						{
							CString strTmp ;
							strTmp.Format(_T("%d;"), pTmp->resval.rlong) ;	
							strXData+=strTmp ;							
						}
						break;
					default:
						//ads_printf("unknown type\n") ;
						break;
					}							
				}
				ads_relrb(pRb);

				if (_tcscmp(strEntType, _T("AcDbText"))==0)
				{
					AcDbText *pText = (AcDbText*)pEnt ;
					AcGePoint3d pt_3d_text = pText->position() ;

					ads_point pt_base = {0,0,0} ;
					pt_base[0] = pt_3d_text.x ;
					pt_base[1] = pt_3d_text.y+3 ;
					//mysys.SetXdata(pEnt, _T("TYPE"), _T("SHOW")) ;
					//mysys.WriteText(pt_base, strXData, 0, AcDb::kTextMid, 0.5, 0.45) ;
					AcDbObjectId objIdText ;
					mysys.AddText(objIdText, strXData, pt_base, pt_base, AcDb::kTextMid, AcDb::kTextBase, 0.5, 0.45, 0, 3) ;
					arrayObjIdShowXdata.Add(objIdText) ;
				}
			}
			else
			{
				//ads_printf("\n没有xdata");
			}

			if(!strXData.IsEmpty())
			{
				if (strEntType == _T("AcDbBlockReference"))
				{
					AcDbBlockReference * pRef = NULL ;
					pRef = AcDbBlockReference::cast(pEnt) ;
					AcGePoint3d pt_3d_blk(0, 0, 0) ;
					pt_3d_blk = pRef->position() ;
					acutPrintf(_T("\n:%s:%s(%.3f-%.3f)"), pEnt->isA()->name(), strXData, pt_3d_blk.x, pt_3d_blk.y) ;
				}
				else
				{
					acutPrintf(_T("\n:%s:%s"), pEnt->isA()->name(), strXData) ;
				}				
			}		

			pEnt->close() ;
		}
	}
	mysys.Set_Layer(_T(""), _T("0")) ;

	//显示xdata的文本打上标示，以便清除
	for (int i=0; i<arrayObjIdShowXdata.GetCount(); i++)
	{
		AcDbEntity *pEntText = NULL ;
		if (mysys.OpenAcDbEntity(arrayObjIdShowXdata.GetAt(i), pEntText, AcDb::kForWrite))
		{
			mysys.SetXdata(pEntText, _T("TYPE"), _T("AppUse_ShowXdata")) ;
			pEntText->close() ;
		}
	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

// 替换块
void CUserTools::ReplaceBlock(void)
{
	CAcModuleResourceOverride myResources ;	
	int iNumReplace = 0 ;

	CBatchFindReplaceDlg dlg ;
	if(dlg.DoModal()==IDOK)
	{
		CUserSystem mysys ;
		switch (dlg.m_iSelOption)
		{
		case 0:
			{		
				AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
				mysys.m_pDb = pDb ;
				TCHAR chBuf[10] ;
				const CString strDxf = _T("INSERT") ;
				CBlkUtility::tcscpy( chBuf , (strDxf.GetLength()+1)*sizeof(TCHAR), strDxf ) ;
				ads_name sum_name, temp_name ;
				struct resbuf eb ;
				eb.restype = 0 ;
				eb.resval.rstring = chBuf ;
				eb.rbnext = NULL ;
				acutPrintf(_T("\n请选择对象:")) ;
				if (RTNORM==acedSSGet(NULL, NULL, NULL, &eb, sum_name))
				{
					long iLen = 0 ;
					acedSSLength(sum_name, &iLen) ;
					for (int i=0; i<iLen; i++)
					{
						acedSSName(sum_name, i, temp_name) ;
						AcDbEntity *pEnt = NULL ;
						if (mysys.OpenAcDbEntity(temp_name, pEnt))
						{						
							if (pEnt->isKindOf(AcDbBlockReference::desc()))
							{
								AcDbBlockReference *pBlkRef = (AcDbBlockReference*)pEnt ;

								CString strBlkName ;
								mysys.GetBlockName(pBlkRef, strBlkName) ;
								if (strBlkName==dlg.m_strOldBlk)
								{
									if(mysys.ReplaceBlock(pBlkRef, &dlg.m_mapStrToStrAtt, &dlg.m_strNewBlk, dlg.m_bUseAtt))
									{
										iNumReplace++ ;
									}
								}
							}
							pEnt->close() ;
						}
					}
				}
			}
			break;
		case 1:
			{
				AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
				mysys.m_pDb = pDb ;
				CLongArray nArrayHandle ;
				int iNumFind = mysys.FindBlk(dlg.m_strOldBlk, nArrayHandle) ;
				for (int i=0; i<iNumFind; i++)
				{
					AcDbEntity * pEnt = NULL ;
					if (mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt, AcDb::kForWrite))
					{
						if (pEnt->isKindOf(AcDbBlockReference::desc()))
						{
							AcDbBlockReference * pBlkRef = (AcDbBlockReference *)pEnt ;
							if(mysys.ReplaceBlock(pBlkRef, &dlg.m_mapStrToStrAtt, &dlg.m_strNewBlk, dlg.m_bUseAtt) )
							{
								iNumReplace++ ;
							}
						}
						pEnt->close() ;
					}
				}
			}
			break;
		case 2:
			{			
				CStringArray strArrayFile ;
				int iNum = 0 ;		

				CBlkUtility::FindFile(dlg.m_strFold,_T("dwg"),strArrayFile) ;

				int iFileNum = strArrayFile.GetSize() ;

				for(int iIndexFile=0; iIndexFile<iFileNum; iIndexFile++)
				{

					CString strFilePath ;
					strFilePath = strArrayFile.GetAt(iIndexFile) ;
					ads_printf(_T("\n正在处理图纸:%s"), strFilePath) ;
					AcDbDatabase *pDb = new AcDbDatabase(false) ;
					if (Acad::eOk==pDb->readDwgFile(strFilePath))
					{
						mysys.m_pDb = pDb ;

						CLongArray nArrayHandle ;
						int iNumFind = mysys.FindBlk(dlg.m_strOldBlk, nArrayHandle) ;
						for (int i=0; i<iNumFind; i++)
						{
							AcDbEntity * pEnt = NULL ;
							if (mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt, AcDb::kForWrite))
							{
								if (pEnt->isKindOf(AcDbBlockReference::desc()))
								{
									AcDbBlockReference * pBlkRef = (AcDbBlockReference *)pEnt ;
									if(mysys.ReplaceBlock(pBlkRef, &dlg.m_mapStrToStrAtt, &dlg.m_strNewBlk, dlg.m_bUseAtt) )
									{
										iNumReplace++ ;
									}
								}
								pEnt->close() ;
							}
						}

						pDb->saveAs(strFilePath) ;
					}

					delete pDb ;
					pDb = NULL ;
				}
			}
			break;
		}
	}
	acutPrintf(_T("\n替换%d个块"), iNumReplace) ;
}

// 统计线段长度（所有AcDbCurve）
void CUserTools::SumLineLen(void)
{
	CAcModuleResourceOverride myResources ;	

	ads_name sset ;
	int err = ads_ssget(NULL, NULL, NULL, NULL, sset) ;
	if (err!=RTNORM)
	{
		ads_printf(_T("\nError selection set")) ;
		return ;
	}
	long lLength = -1 ;
	ads_sslength(sset, & lLength) ;
	ads_name en ;
	AcDbObjectId eId ;
	AcDbEntity * pEnt = NULL ;

	double dSumLen = 0 ;
	int iNumofCurve = 0 ;

	ads_printf(_T("\n")) ;
	for(int iEntIndex=0; iEntIndex<lLength; iEntIndex++)
	{
		ads_ssname(sset, iEntIndex, en) ;

		Acad::ErrorStatus es = acdbGetObjectId(eId, en) ;
		if (es==Acad::eOk)
		{
			if (Acad::eOk == acdbOpenAcDbEntity(pEnt, eId, AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbCurve::desc())) 
				{
					AcDbCurve * pCurve = AcDbCurve::cast(pEnt) ;
					double dLenTmp = 0 ;
					double dEndParam = 0 ;

					pCurve->getEndParam(dEndParam) ;
					pCurve->getDistAtParam(dEndParam, dLenTmp) ;
					dSumLen = dSumLen + dLenTmp ;			
					iNumofCurve++ ;
					ads_printf(_T("序号%d:长度%f, 实体类型:%s\n"), iNumofCurve, dLenTmp, pCurve->isA()->name()) ;
				}
				pEnt->close() ;				

			}
		}		

	}
	ads_ssfree(sset) ;
	ads_printf(_T("统计的实体个数:%d,总的长度:%f"), iNumofCurve, dSumLen) ;
}

//刷新图纸，主要是某些程序写入的文字没有明确指定对齐点，导致图形显示不正确，移动一下可解决。在单文档模式下有效
void CUserTools::RefreshDwg(void)
{
	CSBDestination sb(NULL, IDS_BFF_TITLE) ;
	sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
	if (sb.SelectFolder())
	{
		CString strPath = _T("") ;
		strPath = sb.GetSelectedFolder() ;
		if (strPath.IsEmpty())
		{
			return  ;
		}
		CStringArray strArrayFile ;
		CBlkUtility::FindFile(strPath, _T("dwg"), strArrayFile) ;

		AcDbDatabase *pOldDb = acdbHostApplicationServices()->workingDatabase() ;

		for (int i=0; i<strArrayFile.GetCount(); i++)
		{
			CString strFile = strArrayFile.GetAt(i) ;
			AcApDocManager *pDocMgr = acDocManager ;
			TCHAR * chFileName = NULL ;
			chFileName = (LPTSTR)(LPCTSTR)strFile ;
			pDocMgr->executeInApplicationContext(RefreshDwg,(void *)chFileName) ;
		}
	}
}

// 批量修改文件名称
void CUserTools::MoFileName(void)
{
	CAcModuleResourceOverride myResources ;	
	CChangeFileNameDlg dlg ;
	int iRet = 0 ;

	if (dlg.DoModal()==IDOK)
	{
		CUserSystem mysys ;
		CStringArray strArrayFile ;
		int iNum = 0 ;
		CString strFileExt = _T("") ;

		if (dlg.m_strFileExt!=_T("*.*"))
		{
			strFileExt = dlg.m_strFileExt.Mid(2) ;
		}

		CBlkUtility::FindFile(dlg.m_strFilePath, strFileExt, strArrayFile) ;

		int iFileNum = strArrayFile.GetSize() ;	

		for(int i=0; i<iFileNum; i++)
		{
			CString strFileFullName = strArrayFile.GetAt(i) ;
			CStringArray strArrayNamePart ;
			CBlkUtility::DivideString(strFileFullName, _T("\\"), strArrayNamePart) ;
			int iPartSum = strArrayNamePart.GetSize() ;
			ASSERT(iPartSum>0) ;
			CString strFileName = strArrayNamePart.GetAt(iPartSum-1) ;
			CString strFileNameNew = strFileName ;
			CBlkUtility::ReplaceTxt(strFileNameNew, dlg.m_strOld, dlg.m_strNew) ;

			if (strFileNameNew==strFileName)
			{
				continue ;
			}
			CString strFilePath = strFileFullName.Left(strFileFullName.GetLength()-strFileName.GetLength()) ;
			CString strFileFullNameNew ;
			strFileFullNameNew.Format(_T("%s\\%s"), strFilePath, strFileNameNew) ;
			BOOL bResult = FALSE ;
			bResult = ::MoveFile(strFileFullName, strFileFullNameNew) ;
			if (bResult==TRUE)
			{
				iRet++ ;
			}
		}	

		CString strMsg ;
		strMsg.Format(_T("共处理%d个文件"), iRet) ;	
		AfxMessageBox(strMsg) ;

	}
}


// 显示/隐藏块属性
void CUserTools::ShowBlkAtt(void)
{
	CShowBlkAttDlg dlg ;
	if (dlg.DoModal()==IDOK)
	{
		AcDbDatabase *pDb = acdbCurDwg() ;
		CUserSystem mysys ;
		mysys.m_pDb = pDb ;

		CStringArray strArrayAtt ;
		CBlkUtility::DivideString(dlg.m_strAtts, _T(","), strArrayAtt) ;
		AcDbBlockTable *pBlkTb = NULL ;
		pDb->getBlockTable(pBlkTb, AcDb::kForRead) ;
		AcDbBlockTableRecord *pBlkTbRec = NULL ;
		pBlkTb->getAt(ACDB_MODEL_SPACE, pBlkTbRec, AcDb::kForRead) ;
		pBlkTb->close() ;
		AcDbBlockTableRecordIterator *pTbRecIter = NULL ;
		pBlkTbRec->newIterator(pTbRecIter) ;
		pBlkTbRec->close() ;

		AcDbEntity *pEnt = NULL ;
		for (pTbRecIter->start(); !pTbRecIter->done(); pTbRecIter->step())
		{
			if (Acad::eOk==pTbRecIter->getEntity(pEnt, AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt) ;
					CString strBlkName ;
					mysys.GetBlockName(pBlkRef, strBlkName) ;
					if (strBlkName==dlg.m_strBlkName)
					{
						for (int i=0; i<strArrayAtt.GetCount(); i++)
						{
							mysys.ShowAtt(pBlkRef, strArrayAtt.GetAt(i), dlg.m_iShowOrHide) ;
						}
					}
				}

				pEnt->close() ;
			}
		}
		delete pTbRecIter ;
		pTbRecIter = NULL ;

	}

}

// 标出ID相同的实体
void CUserTools::CheckSameId(void)
{
	CUserTools::UnCheckSameId() ;

	AcDbDatabase *pDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	AcDbBlockTable *pBlkTb = NULL ;
	pDb->getBlockTable(pBlkTb, AcDb::kForRead) ;
	AcDbBlockTableRecord *pBlkTbRec = NULL ;
	pBlkTb->getAt(ACDB_MODEL_SPACE, pBlkTbRec, AcDb::kForRead) ;
	pBlkTb->close() ;
	AcDbBlockTableRecordIterator *pTbRecIter = NULL ;
	pBlkTbRec->newIterator(pTbRecIter) ;
	pBlkTbRec->close() ;

	AcDbEntity *pEnt = NULL ;
	for (pTbRecIter->start(); !pTbRecIter->done(); pTbRecIter->step())
	{
		if (Acad::eOk==pTbRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			int iIsCheck = 0 ;
			mysys.GetXdata(pEnt, _T("FindSameId_ISCHECK"), iIsCheck) ;
			long nID = 0 ;
			if (mysys.GetXdata(pEnt, _T("ID"), nID) && iIsCheck!=1)
			{
				CLongArray nArrayHandle ;
				//UINT nHandle = mysys.GetHandle(pEnt) ;
				if (mysys.FindSameIdEnt(nID, nArrayHandle)>1)
				{
					AcDbEntity *pEntTmp = NULL ;
					AcGePoint3d pt_3d_preent(0,0,0) ;
					mysys.Set_Layer( _T("") , _T("AppUse_ShowSameIdEnt")) ;

					CString strMsg = _T("文本 ") ;
					for (int i=0; i<nArrayHandle.GetSize(); i++)
					{
						AcGePoint3d pt_3d_curent(0,0,0) ;
						if(mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEntTmp, AcDb::kForRead))
						{
							if (pEntTmp->isKindOf(AcDbBlockReference::desc()))
							{
								AcDbBlockReference *pRefTmp = AcDbBlockReference::cast(pEntTmp) ;
								pt_3d_curent = pRefTmp->position() ;
							}
							else if (pEntTmp->isKindOf(AcDbText::desc()))
							{
								AcDbText *pTextTmp = AcDbText::cast(pEntTmp) ;
								pt_3d_curent = pTextTmp->position() ;
								strMsg.AppendFormat(_T("%s(%.3f,%.3f) "), pTextTmp->textString(), pt_3d_curent.x, pt_3d_curent.y) ;
							}
							AcDbObjectId objIdCircle = mysys.AddAcDbCircle(pt_3d_curent, 3, 1) ;
							AcDbEntity *pEntCircle = NULL ;
							if (mysys.OpenAcDbEntity(objIdCircle, pEntCircle, AcDb::kForRead))
							{
								mysys.SetXdata(pEntCircle, _T("TYPE"), _T("AppUse_ShowSameIdEnt")) ;
								pEntCircle->close() ;
							}

							if (i>0)
							{
								AcDbObjectId objIdLine = mysys.AddAcDbLine(pt_3d_preent, pt_3d_curent, 1) ;
								AcDbEntity *pEntLine = NULL ;
								if (mysys.OpenAcDbEntity(objIdLine, pEntLine, AcDb::kForRead))
								{
									mysys.SetXdata(pEntLine, _T("TYPE"), _T("AppUse_ShowSameIdEnt")) ;
									pEntLine->close() ;
								}
							}
							pt_3d_preent = pt_3d_curent ;

							pEntTmp->close() ;
							pEntTmp = NULL ;
						}
					}
					strMsg.Append(_T(" id相同")) ;
					acutPrintf(_T("\n%s"), strMsg) ;
					mysys.Set_Layer( _T("") , _T("0")) ;

				}
			}

			pEnt->close() ;
		}
	}

	for (pTbRecIter->start(); !pTbRecIter->done(); pTbRecIter->step())
	{
		if (Acad::eOk==pTbRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			mysys.DeleteXdata(pEnt, _T("FindSameId_ISCHECK")) ;

			pEnt->close() ;
		}
	}
	delete pTbRecIter ;
	pTbRecIter = NULL ;
}

// 取消标注ID相同实体
void CUserTools::UnCheckSameId(void)
{
	AcDbDatabase *pDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	AcDbBlockTable *pBlkTb = NULL ;
	pDb->getBlockTable(pBlkTb, AcDb::kForRead) ;
	AcDbBlockTableRecord *pBlkTbRec = NULL ;
	pBlkTb->getAt(ACDB_MODEL_SPACE, pBlkTbRec, AcDb::kForRead) ;
	pBlkTb->close() ;
	AcDbBlockTableRecordIterator *pTbRecIter = NULL ;
	pBlkTbRec->newIterator(pTbRecIter) ;
	pBlkTbRec->close() ;

	AcDbEntity *pEnt = NULL ;
	for (pTbRecIter->start(); !pTbRecIter->done(); pTbRecIter->step())
	{
		if (Acad::eOk==pTbRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			CString strType;
			mysys.GetXdata(pEnt, _T("TYPE"), strType) ;

			if (strType==_T("AppUse_ShowSameIdEnt"))
			{
				pEnt->erase(true) ;
			}

			pEnt->close() ;
		}
	}
	delete pTbRecIter ;
	pTbRecIter = NULL ;

	mysys.DeleteLayer(_T("AppUse_ShowSameIdEnt")) ; 
}

void CUserTools::HideXdata(void)
{
	AcDbDatabase *pDb = acdbCurDwg() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	AcDbBlockTable *pBlkTb = NULL ;
	pDb->getBlockTable(pBlkTb, AcDb::kForRead) ;
	AcDbBlockTableRecord *pBlkTbRec = NULL ;
	pBlkTb->getAt(ACDB_MODEL_SPACE, pBlkTbRec, AcDb::kForRead) ;
	pBlkTb->close() ;
	AcDbBlockTableRecordIterator *pTbRecIter = NULL ;
	pBlkTbRec->newIterator(pTbRecIter) ;
	pBlkTbRec->close() ;

	AcDbEntity *pEnt = NULL ;
	for (pTbRecIter->start(); !pTbRecIter->done(); pTbRecIter->step())
	{
		if (Acad::eOk==pTbRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			CString strType;
			mysys.GetXdata(pEnt, _T("TYPE"), strType) ;

			if (strType==_T("AppUse_ShowXdata"))
			{
				pEnt->erase(true) ;
			}

			pEnt->close() ;
		}
	}
	delete pTbRecIter ;
	pTbRecIter = NULL ;

	mysys.DeleteLayer(_T("AppUse_ShowXdata")) ; 
}

// 对选中的数值统一加减某数值
void CUserTools::AddNum(void)
{
	int iAddPlus = 0 ;

	if( acedGetInt(_T("\n请输入要加减的数值:"), &iAddPlus) == RTNORM)
	{
		struct resbuf *pRb ;
		pRb = acutBuildList(RTDXF0,_T("TEXT"),0) ;	

		long iLen = 0 ;
		ads_name sum_name ;
		if (Acad::eNormal==acedSSGet(NULL, NULL, NULL, pRb, sum_name))
		{	
			acedSSLength(sum_name, &iLen) ;
			for (int i=0; i<iLen; i++)
			{
				acutPrintf(_T("\n%d:"), i) ;

				ads_name en ;
				acedSSName(sum_name, i, en) ;
				AcDbEntity *pEnt = NULL ;
				AcDbObjectId ent_id ;
				acdbGetObjectId(ent_id, en) ;
				if(Acad::eOk == acdbOpenAcDbEntity(pEnt,ent_id, AcDb::kForWrite))
				{
					AcDbText *pText = AcDbText::cast(pEnt) ;
					CString strText = pText->textString() ;
					int iText = 0 ;
					BOOL bHasKH = FALSE ;
					if (strText.Left(1)==_T("(")&&strText.Right(1)==_T(")"))
					{
						bHasKH = TRUE ;
						iText = _tstoi(strText.Mid(1, strText.GetLength()-2)) ;
					}
					else
					{
						iText = _tstoi(strText) ;
					}
					iText+=iAddPlus ;
					CString strNewText ;
					strNewText.Format(_T("%s%d%s"), bHasKH==TRUE?_T("("):_T(""),iText,bHasKH==TRUE?_T(")"):_T("")) ;
					pText->setTextString(strNewText) ;
					pText->setColorIndex(3) ;
					acutPrintf(_T("%s+%d=%d\t Text %s(%.3f,%.3f)"), strText, iAddPlus, iText, strText, pText->position().x, pText->position().y ) ;

					pEnt->close() ;
				}

			}

			acedSSFree(sum_name) ;
		}
		acutRelRb(pRb) ;

	}
}

// 替换某个APP的值中的文本
void CUserTools::ReplaceAppValue(void)
{
	CUserSystem mysys ;
	//begin 批量修改xdata		
	ads_name ssname, temp_name ;
	acutPrintf(_T("\n选择实体")) ;
	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		TCHAR strInput0[50]  ;
		if(RTNORM == acedGetString(0,_T("\nAPPNAME:"), strInput0))
		{
			TCHAR strInput1[50]  ;
			if(RTNORM == acedGetString(0,_T("\n原字符:"), strInput1))
			{
				TCHAR strInput2[50]  ;
				if(RTNORM == acedGetString(0,_T("\n被替换成的字符:"), strInput2))
				{
					long nLength = 0 ;
					acedSSLength(ssname, &nLength) ;

					for (int i=0; i<nLength; i++)
					{
						acedSSName(ssname, i, temp_name) ;
						AcDbEntity *pEnt = NULL ;
						if (mysys.OpenAcDbEntity(temp_name, pEnt))
						{

							CString strXdata ;
							if (mysys.GetXdata(pEnt, strInput0, strXdata))
							{
								CString strXdataNew ;
								strXdataNew = strXdata ;								
								strXdataNew.Replace(strInput1, strInput2) ;
								if (strXdataNew.Compare(strXdata)!=0)
								{
									mysys.SetXdata(pEnt, strInput0, strXdataNew) ;
									acutPrintf(_T("\n%s---%s"), strXdata, strXdataNew) ;
								}						
							}

							pEnt->close() ;
							pEnt = NULL ;
						}
					}
				}
			}

		}

		acedSSFree(ssname) ;	

	}
	else
	{
		AfxMessageBox(_T("没有选中任何实体")) ;
	}
	//end 批量修改xdata
}

// 显示实体Handle
int CUserTools::ZList(void)
{
	CUserSystem mysys ;
	ads_name en ;
	ads_point pt ;		
	if(RTNORM == acedEntSel (_T("\n选择对象：") , en , pt) )
	{
		AcDbEntity * pEnt = NULL ;
		if(mysys.OpenAcDbEntity(en , pEnt, AcDb::kForRead))
		{
			UINT nHandle = mysys.GetHandle(pEnt) ;
			acutPrintf(_T("\n所选实体类型：%s\n句柄：%d"), pEnt->isA()->name(), nHandle ) ;
			pEnt->close() ;
		}
	}	

	return 0;
}

// 删除指定app
void CUserTools::DelOneApp(void)
{
	CUserSystem mysys ;
	ads_name ssname, temp_name ;
	acutPrintf(_T("\n选择实体")) ;
	if (RTNORM==acedSSGet(NULL, NULL, NULL, NULL, ssname))
	{
		TCHAR strInput[50]  ;
		if(RTNORM == acedGetString(0,_T("\nInput APP name to del:"), strInput))
		{
			long nLength = 0 ;
			acedSSLength(ssname, &nLength) ;

			for (int i=0; i<nLength; i++)
			{
				acedSSName(ssname, i, temp_name) ;
				AcDbEntity *pEnt = NULL ;
				if (mysys.OpenAcDbEntity(temp_name, pEnt))
				{
					mysys.DeleteXdata(pEnt, strInput) ;

					pEnt->close() ;
					pEnt = NULL ;
				}
			}
		}
		acedSSFree(ssname) ;	
	}
	else
	{
		AfxMessageBox(_T("没有选中任何实体")) ;
	}
}

// CAD中表格转换成excel表格
void CUserTools::DwgToXls(void)
{
	CAcModuleResourceOverride myResources ;
	CBlkExcelTool::DwgToExcel() ;
}

//在将excel转换成cad表格时计算对应height和width
void CUserTools::GetHW(void)
{
	CAcModuleResourceOverride myResources ;
	CGetHeightWidthDlg dlg ;
	dlg.DoModal() ;
}

//删除空文本
void CUserTools::DelNullText(void)
{
	CUserSystem mysys ;
	int iRet = mysys.DeleteNullText() ;
	acutPrintf(_T("\n删除了 %d 个空文本"), iRet) ;

}

// 删除扩展数据
void CUserTools::DelXdata(void)
{
	int i = 0 ;
	if (RTNORM == acedGetInt(_T("\n删除扩展数据[全图删除(0)/选择删除(1)]<0>:"), &i))
	{
		if (i==0)
		{
			CUserSystem mysys ;
			mysys.DelAllXdata() ;
		}
		else
		{
			DelSelXdata() ;
		}
	}
}

//统计电缆
void CUserTools::SumDianLan(void)
{
	CAcModuleResourceOverride myResources ;	
	CGetDianlanSumDlg dlg ;
	dlg.DoModal() ;
}

// 单线转换成多段线
int CUserTools::ConvertToPolyline(void)
{
	CUserSystem mysys ;

	acutPrintf(_T("\n框选线段:")) ;
	struct resbuf* pRb ;
	int nLen = 0 ;
	CLongArray nArrayHandle ;
	pRb = acutBuildList(-4,_T("<OR"),RTDXF0,_T("LINE"),RTDXF0,_T("LWPOLYLINE"), -4,_T("OR>"), 0) ;
	nLen = mysys.GetSSEntHandle(NULL, NULL, NULL, pRb, nArrayHandle) ;
	acutRelRb(pRb) ;
	//单线转多段线
	int iRet = 0 ;
	//CLongArray nArrayHandleNew/*, nArrayHandleToDel*/ ;
	for (int i=0; i<nLen; i++)
	{
		AcDbEntity* pEnt = NULL ;
		long nHandle = nArrayHandle.GetAt(i) ;
		if (mysys.OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				AcGePoint3d pt_3d_se[2] ;
				AcDbLine* pLine = AcDbLine::cast(pEnt) ;
				pLine->getStartPoint(pt_3d_se[0]) ;
				pLine->getEndPoint(pt_3d_se[1]) ;
				AcDbObjectId ojbId = mysys.AddAcDbPolyLine(pt_3d_se[0], pt_3d_se[1]) ;
				//long nHandleNew = mysys.GetHandle(ojbId) ;
				//nArrayHandleNew.Add(nHandleNew) ;
				pEnt->upgradeOpen() ;
				pEnt->erase() ; //删除单线
				iRet++ ;
			}
			// 			else if (pEnt->isKindOf(AcDbPolyline::desc()))
			// 			{
			// 				nArrayHandleNew.Add(nHandle) ;
			// 			}
			pEnt->close() ;
		}
	}
	acutPrintf(_T("\n转换直线到多段线%d条"), iRet) ;
	return 0;
}

// 合并多段线
int CUserTools::JoinLine(void)
{
	BOOL bHasErro = FALSE ;
	CString strMsg ;
	CUserSystem mysys ;

	double dTolJolinLine = 0.001 ;
	CBlkUtility::QueryValueReg(_T("ZnOption"), _T("TolCheckGd"), dTolJolinLine) ;

	acutPrintf(_T("\n框选多段线：")) ;

	struct resbuf* pRb ;
	int nLen = 0 ;
	CLongArray nArrayHandle ;
	pRb = acutBuildList(RTDXF0,_T("LWPOLYLINE"),  0) ;
	nLen = mysys.GetSSEntHandle(NULL, NULL, NULL, pRb, nArrayHandle) ;
	acutRelRb(pRb) ;

	int iRet = 0 ;
	CString strHasFinishPt ;  //记录形如1111-0/1111-1（线的handle-起点0/终点1）
	CLongArray nArrayHandleToJoin[2] ;
	//查找需要首尾相连的线
	for (int i=0; i<nLen; i++)
	{
		AcGePoint3d pt_3d_se[2] ;

		long nHandleCur = nArrayHandle.GetAt(i) ;
		AcDbEntity* pEnt = NULL ;
		if (mysys.OpenAcDbEntity(nHandleCur, pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
				pPolyLine->getStartPoint(pt_3d_se[0]) ;
				pPolyLine->getEndPoint(pt_3d_se[1]) ;
			}
			pEnt->close() ;
		}
		for (int m=0; m<2; m++) //m=0：起点m=1：终点
		{
			CString strCurPt ; 
			strCurPt.Format(_T("%d-%d"), nHandleCur, m) ;
			if (strHasFinishPt.Find(strCurPt)!=-1)
			{
				continue; 
			}
			CLongArray nArrayHandleLine[2] ;
			mysys.GetLineOfStartOrEndInPt(pt_3d_se[m], nArrayHandle, nArrayHandleLine, dTolJolinLine) ; 
			if (m==0&&nArrayHandleLine[1].GetCount()==1) //在起点找到1个以此为终点的线
			{
				nArrayHandleToJoin[0].Add(nArrayHandleLine[1].GetAt(0)) ;
				nArrayHandleToJoin[1].Add(nHandleCur) ;
				strHasFinishPt.AppendFormat(_T("%s,"), strCurPt) ;
				strCurPt.Format(_T("%d-%d"), nArrayHandleLine[1].GetAt(0), 1) ;
				strHasFinishPt.AppendFormat(_T("%s,"), strCurPt) ;
			}
			else if (m==1&&nArrayHandleLine[0].GetCount()==1) //在起点找到1个以此为起点的线
			{
				nArrayHandleToJoin[0].Add(nHandleCur) ;
				nArrayHandleToJoin[1].Add(nArrayHandleLine[0].GetAt(0)) ;				
				strHasFinishPt.AppendFormat(_T("%s,"), strCurPt) ;
				strCurPt.Format(_T("%d-%d"), nArrayHandleLine[0].GetAt(0), 0) ;
				strHasFinishPt.AppendFormat(_T("%s,"), strCurPt) ;
			}
		}
	}
	//合并首尾相连的线
	int iNumJoin = nArrayHandleToJoin[0].GetCount() ;
	for (int i=0; i<iNumJoin; i++)
	{
		bHasErro = TRUE ;
		long nHandleLeft = nArrayHandleToJoin[0].GetAt(i) ;
		long nHandleRight = nArrayHandleToJoin[1].GetAt(i) ;
		AcDbEntity* pEnt = NULL ;
		if (mysys.OpenAcDbEntity(nHandleLeft, pEnt))
		{
			AcDbPolyline* pPolyLineLeft = AcDbPolyline::cast(pEnt) ;
			int iNumVert1 = pPolyLineLeft->numVerts() ;

			AcDbEntity* pEnt2 = NULL ;
			if (mysys.OpenAcDbEntity(nHandleRight, pEnt2, AcDb::kForRead))
			{
				AcDbPolyline* pPolyLineRight = AcDbPolyline::cast(pEnt2) ;

				int iNumVert2 = pPolyLineRight->numVerts() ;
				for (int j=0; j<iNumVert2-1; j++)
				{
					AcGePoint2d pt_2d_add ;
					pPolyLineRight->getPointAt(j+1, pt_2d_add) ;
					double dWidthStart = 0, dWidthEnd = 0 ;
					pPolyLineRight->getWidthsAt(j, dWidthStart, dWidthEnd) ;
					pPolyLineLeft->addVertexAt(iNumVert1+j, pt_2d_add) ;
					pPolyLineLeft->setWidthsAt(iNumVert1+j-1, dWidthStart, dWidthEnd) ;
				}

				pEnt2->upgradeOpen() ;
				pEnt2->erase() ;
				pEnt2->close() ;

				pPolyLineLeft->setColorIndex(3) ;
			}				

			pEnt->close() ;
		}
	}   

	if (bHasErro==TRUE)
	{
		strMsg.Format(_T("合并%d处多段线,已标示，请先确认后再运行此命令。"), iNumJoin) ;
		AfxMessageBox(strMsg) ;		
	}
	return 0;
}
//函数功能将xls转换生成标准的计算机联锁表
// 给块增加属性
void CUserTools::AddAtt(void)
{
	CSetBlkAttDlg dlg ;
	dlg.DoModal() ;
// 	CUserSystem mysys ;
// 	acutPrintf(_T("\n选择需要增加属性的块：")) ;
// 	struct resbuf* pRb ;
// 	int nLen = 0 ;
// 	CLongArray nArrayHandle ;
// 	pRb = acutBuildList(RTDXF0,_T("INSERT"),  0) ;
// 	nLen = mysys.GetSSEntHandle(NULL, NULL, NULL, pRb, nArrayHandle) ;
// 	acutRelRb(pRb) ;
// 	if (nLen>0)
// 	{
// 		if (acedGetString())
// 		{
// 		}
// 		for (int i=0; i<nLen; i++)
// 		{
// 			AcDbEntity* pEnt = NULL ;
// 			if (mysys.OpenAcDbEntity(nArrayHandle.GetAt(i), pEnt))
// 			{
// 				if (pEnt->isKindOf(AcDbBlockReference::desc()))
// 				{
// 					AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
// 					mysys.AddAttValue(pRef,chTag, chVal) ;
// 				}
// 				pEnt->close() ;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		acutPrintf(_T("\n没有选中任何块！")) ;
// 	}
}

// 将图纸中width为dFrom的转换为dTo
void CUserTools::SetPlineWidth(AcDbDatabase* pDb, double dFrom, double dTo, BOOL bIncludeBlk)
{
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
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				pEnt->upgradeOpen() ;
				AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
				int iNumVerts = pPolyLine->numVerts() ;
				for (int i=0; i<iNumVerts; i++)
				{
					double dStartWidth=0, dEndWidth=0 ;
					pPolyLine->getWidthsAt(i,dStartWidth, dEndWidth) ;
					if (dStartWidth==dFrom&&dEndWidth==dFrom)
					{										
						pPolyLine->setWidthsAt(i, dTo, dTo) ;
					}
				}
				pEnt->downgradeOpen() ;
			}
			else if (bIncludeBlk==TRUE&&pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRefTmp = AcDbBlockReference::cast(pEnt) ;
				SetPlineWidth(pRefTmp, dFrom, dTo) ;
			}

			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

void CUserTools::SetPlineWidth(AcDbBlockReference* pRef, double dFrom, double dTo)
{
	//打开块表记录
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	AcDbObjectId blkTblObjId ;
	blkTblObjId = pRef->blockTableRecord() ;
	acdbOpenObject(pBlkTblRec, blkTblObjId, AcDb::kForRead) ;

	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbPolyline::desc()))
			{
				pEnt->upgradeOpen() ;
				AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
				int iNumVerts = pPolyLine->numVerts() ;
				for (int i=0; i<iNumVerts; i++)
				{
					double dStartWidth=0, dEndWidth=0 ;
					pPolyLine->getWidthsAt(i,dStartWidth, dEndWidth) ;
					if (dStartWidth==dFrom&&dEndWidth==dFrom)
					{										
						pPolyLine->setWidthsAt(i, dTo, dTo) ;
					}
				}
				pEnt->downgradeOpen() ;
			}
			else if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference* pRefTmp = AcDbBlockReference::cast(pEnt) ;
				SetPlineWidth(pRefTmp, dFrom, dTo) ;
			}

			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}
void CUserTools::SetPlineWidth(void)
{
	CSetPlWidthDlg dlg ;
	if (dlg.DoModal()==IDOK)
	{
		AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase() ;
		CUserTools::SetPlineWidth(pCurDb, dlg.m_dFrom, dlg.m_dTo, dlg.m_bIncludeBlk) ;
	}
}

// 批量修改图库（单线to多段线，或者反向操作）
int CUserTools::ReplaceTUKU()
{
	int iType = 0 ;
	if (acedGetInt(_T("\n请输入参数：[0，设置单线、圆为多线（老图库变新图库）/1，设置属性颜色随层，多段线颜色为白色/2，多线线宽变为0/3，多线线宽变为0.1] <0>:"), &iType) )
	{
		CSBDestination sb(NULL, IDS_BFF_TITLE) ;
		sb.SetFlags(BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT) ;
		if (sb.SelectFolder())
		{
			CString strPath = _T("") ;
			strPath = sb.GetSelectedFolder() ;
			if (strPath.IsEmpty())
			{
				return 0  ;
			}
			CStringArray strArrayFile ;
			CBlkUtility::FindFile(strPath, _T("dwg"), strArrayFile) ;

			AcGePoint3d pt_3d_rect(0, 10, 0) ;
			AcGePoint3dArray pt3dArray, pt3dArray1 ;
			pt3dArray.append(pt_3d_rect) ;
			pt_3d_rect.x = 2 ;
			pt3dArray.append(pt_3d_rect) ;
			pt_3d_rect.y = 2 ;
			pt3dArray.append(pt_3d_rect) ;
			pt_3d_rect.x = 0 ;
			pt3dArray.append(pt_3d_rect) ;
			pt_3d_rect.x = 2 ;
			pt_3d_rect.y = 10 ;
			pt3dArray1.append(pt_3d_rect) ;
			pt_3d_rect.x = 4 ;
			pt3dArray1.append(pt_3d_rect) ;
			pt_3d_rect.y = 2 ;
			pt3dArray1.append(pt_3d_rect) ;
			pt_3d_rect.x = 2 ;
			pt3dArray1.append(pt_3d_rect) ;

			for (int i=0; i<strArrayFile.GetCount(); i++)
			{
				CString strFile = strArrayFile.GetAt(i) ;
				AcDbDatabase* pDb = new AcDbDatabase(false) ;
				CUserSystem mysys ;
				mysys.m_pDb = pDb ;

				CStringArray strArrayNameStr ;
				CString strFileNameWithoutExt ;
				CBlkUtility::GetFileName(strFile, strFileNameWithoutExt) ;
				CBlkUtility::DivideString(strFileNameWithoutExt, _T("_"), strArrayNameStr) ;
				BOOL bIsHigh = FALSE ;//是否高柱
				int iNumShow = 0 ; //表示器个数
				if (strArrayNameStr.GetCount()==8)
				{
					if (strArrayNameStr.GetAt(6)==_T("G"))
					{
						bIsHigh = TRUE ;
					}
					iNumShow = _tstoi(strArrayNameStr.GetAt(3)) ;
				}

				if (Acad::eOk==pDb->readDwgFile(strFile))
				{
					AcDbBlockTable * pBlkTbl = NULL ;
					pDb->getBlockTable(pBlkTbl,AcDb::kForRead);

					AcDbBlockTableRecord * pBlkTblRec = NULL ;
					pBlkTbl->getAt ( ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead )  ;
					pBlkTbl->close() ;
					AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
					pBlkTblRec->newIterator( pBlkTblRecIter );
					pBlkTblRec->close();

					AcDbEntity * pEnt = NULL ;
					int i1 = 0 ;
					if (iType==0) //设置单线（包括圆）为多线
					{
						for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
						{
							if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
							{
								i1++ ;
								acutPrintf(_T("\n(%d) %s:%d:%s"), i, strFileNameWithoutExt, i1, pEnt->isA()->name()) ;

								if (pEnt->isKindOf(AcDbCircle::desc()))
								{
									pEnt->upgradeOpen() ;
									AcDbCircle* pCircle = AcDbCircle::cast(pEnt) ;
									double dRadius =  pCircle->radius() ;
									AcGePoint3d pt_3d_cen = pCircle->center() ;
									int iColorIndex = pCircle->colorIndex() ;
									mysys.AddAcDbPolyLine(AcGePoint3d(pt_3d_cen.x-dRadius, pt_3d_cen.y, 0) , PI, pt_3d_cen, 0.1, iColorIndex);		

									pCircle->erase() ;
								}
								else if (pEnt->isKindOf(AcDbLine::desc()))
								{
									AcDbLine* pLine = AcDbLine::cast(pEnt) ;
									AcGePoint3d pt_3d_start = pLine->startPoint() ;
									AcGePoint3d pt_3d_end = pLine->endPoint() ;
									AcGeVector3d vec = pt_3d_end-pt_3d_start ;
									double dAngle = vec.angleOnPlane(AcGePlane::kXYPlane) ;
									double dLen = 0 ;
									pLine->getDistAtPoint(pt_3d_end, dLen) ;
									int iColorIndex = pLine->colorIndex() ;
									if (dLen>0.1) 
									{
										pEnt->upgradeOpen() ;
										if (fabs(dLen-3.2)<0.001||fabs(dLen-4)<0.001||bIsHigh||(!bIsHigh&&iNumShow==0))
										{
											mysys.AddAcDbPolyLine(pt_3d_start, pt_3d_end, 0.1, iColorIndex) ;
											pLine->erase() ;
										}
										else
										{
											pLine->erase() ;
										}	
									}

								}
								pEnt->close() ;
							}
						}
						//若是矮型，且有表示器，需要边框
						if (!bIsHigh&&iNumShow>0)
						{
							mysys.AddAcDbPolyLine(pt3dArray, 0.1, 0, TRUE) ;
							if (iNumShow>3)
							{
								mysys.AddAcDbPolyLine(pt3dArray1, 0.1, 0, TRUE) ;
							}
						}	
					}
					else if (iType==1) //设置属性颜色随层，多段线颜色为白色
					{
						for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
						{
							if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
							{
								i1++ ;
								acutPrintf(_T("\n(%d) %s:%d:%s"), i, strFileNameWithoutExt, i1, pEnt->isA()->name()) ;

								if (pEnt->isKindOf(AcDbAttributeDefinition::desc()))
								{
									pEnt->upgradeOpen() ;
									pEnt->setColorIndex(256) ; //随层
								}
								else if (pEnt->isKindOf(AcDbPolyline::desc()))
								{
									pEnt->upgradeOpen() ;
									int iColorIndex = pEnt->colorIndex() ;
									if (iColorIndex==0||iColorIndex>7)
									{
										pEnt->setColorIndex(7) ;
									}
								}

								pEnt->close() ;
							}
						}
					}
					else if (iType==2) //多线线宽变为0（类似直线，为解决打印问题：0.1单位的多段线1:1打印线宽为默认线宽0.25*0.1=0.025，远小于0线默认打印线宽0.25，比直线还细）
					{
						for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
						{
							if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
							{
								i1++ ;
								acutPrintf(_T("\n(%d) %s:%d:%s"), i, strFileNameWithoutExt, i1, pEnt->isA()->name()) ;
								if (pEnt->isKindOf(AcDbPolyline::desc()))
								{
									pEnt->upgradeOpen() ;
									AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
									int iNumVerts = pPolyLine->numVerts() ;
									for (int i=0; i<iNumVerts; i++)
									{
										double dStartWidth=0, dEndWidth=0 ;
										pPolyLine->getWidthsAt(i,dStartWidth, dEndWidth) ;
										if (dStartWidth==0.1&&dEndWidth==0.1)
										{										
											pPolyLine->setWidthsAt(i, 0, 0) ;
										}
									}
									pEnt->downgradeOpen() ;
								}

								pEnt->close() ;
							}
						}
					}
					else if (iType==3) //多线线宽变为0.1(iType=2的反向操作)
					{
						for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
						{
							if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
							{
								i1++ ;
								acutPrintf(_T("\n(%d) %s:%d:%s"), i, strFileNameWithoutExt, i1, pEnt->isA()->name()) ;
								if (pEnt->isKindOf(AcDbPolyline::desc()))
								{
									pEnt->upgradeOpen() ;
									AcDbPolyline* pPolyLine = AcDbPolyline::cast(pEnt) ;
									int iNumVerts = pPolyLine->numVerts() ;
									for (int i=0; i<iNumVerts; i++)
									{
										double dStartWidth=0, dEndWidth=0 ;
										pPolyLine->getWidthsAt(i,dStartWidth, dEndWidth) ;
										if (dStartWidth==0&&dEndWidth==0)
										{										
											pPolyLine->setWidthsAt(i, 0.1, 0.1) ;
										}
									}
									pEnt->downgradeOpen() ;
								}

								pEnt->close() ;
							}
						}
					}

					delete pBlkTblRecIter ;
					pBlkTblRecIter = NULL ;

				}
				pDb->saveAs(strFile) ;
				delete pDb ;
				pDb = NULL ;
			}		
		}

	}

	return 0;
}
void CUserTools::XlsToLsb( void )
{
	CAcModuleResourceOverride myResources ;	

	CBlkExcelTool exceltool ;
	CUserSystem mySys ;
	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	_WorkbookPtr pWorkBook = NULL ;
	//WorksheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;
	SheetsPtr pWorksheets = NULL ;
	ads_point base_pt = {0,0,0};
	try
	{
			CXlsToDwgDlg dlg ;
			if (dlg.DoModal()==IDOK)
			{
			ads_printf(_T("%d"), dlg.m_iConvMode) ;
			int iConvMode = dlg.m_iConvMode ;
			CString strMark = dlg.m_strMark ;
			strMark.TrimLeft() ;
			strMark.TrimRight() ;
			BOOL bFirst=TRUE;
			if (bFirst) 
			{
				::AfxOleInit ( ) ;
				bFirst =FALSE ;
			}		
			HRESULT hr = m_pExcel.CreateInstance ( "Excel.Application" ) ;
			if(FAILED(hr))
			{
				AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
				return  ;
			}
			pWorkBooks = m_pExcel->GetWorkbooks () ;
			pWorkBook = pWorkBooks->Open( (_bstr_t)dlg.m_strExcelFileLoc) ;
			_variant_t vt_index ;
			//VARIANT vt_index ;
			vt_index.vt = VT_I2 ;
			vt_index.iVal = 1 ;

			pWorksheets  = m_pExcel->GetSheets() ; //m_pExcel->GetSheets( ) ;
			//获得sheet1的指针
			pSheet = pWorksheets->GetItem( vt_index ) ;

			ads_getpoint(NULL,_T("\n选择起始点"),base_pt) ;

			TCHAR cStartCol[2], cEndCol[2] ;			
			//_tcscpy_s(cStartCol, dlg.m_strStartCol) ;
			CBlkUtility::tcscpy( cStartCol , (dlg.m_strStartCol.GetLength()+1)*sizeof(TCHAR), dlg.m_strStartCol ) ;
			//_tcscpy_s(cEndCol, dlg.m_strEndCol) ;	
			CBlkUtility::tcscpy( cEndCol , (dlg.m_strEndCol.GetLength()+1)*sizeof(TCHAR), dlg.m_strEndCol ) ;
			int iStart_Row=dlg.m_iStartRow;
			int iEnd_Row=dlg.m_iEndRow;
			AcGePoint3d base_pt3d;
			//int i=iStart_Row;
			for(int i=iStart_Row;i<iEnd_Row+1;)
			{
				//取i行C列，判断是否为合并格
				CString strCellName ; 
				CString strCellText ;
				int iEnd_RowTemp=0;
				CString strColName = CBlkUtility::GetExcelFieldName(3) ;
				strCellName.Format(_T("%s%d"),strColName, i+37) ;
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
					//为合并格，则图幅截至上一个单元格
					iEnd_RowTemp=iMergeCellStartRow-1;
					//i=iEnd_RowTemp+1;
				}
				else
					iEnd_RowTemp=i+37;
				double d_witdh2=0;
				double d_rowhight=0;
				//只写到指定行，并且从第三行开始
				if(iEnd_RowTemp>iEnd_Row)
				{
					iEnd_RowTemp=iEnd_Row;
				}
				//写表头
				mySys.LSBExcelToDwg2(dlg.m_strExcelFileLoc,1,2, cStartCol,cEndCol,base_pt,d_witdh2,d_rowhight);
				ads_point base_pt1={base_pt[0],base_pt[1],base_pt[2]};
				base_pt1[1]-=d_rowhight;
				mySys.LSBExcelToDwg2(dlg.m_strExcelFileLoc,i,iEnd_RowTemp, cStartCol,cEndCol,base_pt1,d_witdh2,d_rowhight);
				i=iEnd_RowTemp+1;
				//加上联锁表格框
				//double d_witdh=0;
				CUserSystem my_User;
				my_User.ConvertPtAdsTo3D(base_pt,base_pt3d);
				//添加联锁表表框
				AddToLsb(base_pt3d,d_witdh2);
				base_pt[1]-=350;
				
			}
		}
		///关闭打开的各个资源
		pWorkBooks->Close( ) ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;		
		//m_pExcel->Release() ;
		m_pExcel = NULL ;

		}
catch(_com_error& e)
{
	//bReturn = FALSE ;
	CString sBuff = CBlkUtility::GetErrorDescription(e);
	AfxMessageBox(sBuff);  
	pWorkBooks->Close() ;
	pWorkBooks = NULL ;
	m_pExcel->Quit() ;
	m_pExcel = NULL ;		
}
catch(...)
{	
	//bReturn = FALSE ;
	AfxMessageBox(_T("未知的错误!"));		
	pWorkBooks->Close() ;
	pWorkBooks = NULL ;
	m_pExcel->Quit() ;
	m_pExcel = NULL ;		
}	

return  ;

}
void CUserTools::AddToLsb( AcGePoint3d start_pt_3d,float f_length_xls )
{
	//ads_point base_pt={0,0,0};
	//ads_getpoint(NULL,_T("\n选择起始点"),base_pt);
	AcGePoint3d base_pt3d=start_pt_3d;
	//CUserSystem my_User;
	//my_User.ConvertPtAdsTo3D(base_pt,base_pt3d);
	AcGePoint2d base_pt2d=CConvertUtil::ToPoint2d(base_pt3d);
	AcGePoint2d start_rectangle1_pt2d1=base_pt2d;
	start_rectangle1_pt2d1.x-=15;
	start_rectangle1_pt2d1.y+=9;
	float xls_len=f_length_xls;
	AcGePoint2d start_rectangle1_pt2d2=base_pt2d;
	start_rectangle1_pt2d2.x+=(15+xls_len);
	start_rectangle1_pt2d2.y+=(9-287);
	CPolylineUtil::AddRectangle(start_rectangle1_pt2d1,start_rectangle1_pt2d2,0.5);
	AcGePoint2d start_rectangle2_pt2d1=base_pt2d;
	start_rectangle2_pt2d1.x-=40;
	start_rectangle2_pt2d1.y+=14;
	AcGePoint2d start_rectangle2_pt2d2=base_pt2d;
	start_rectangle2_pt2d2.x+=(20+xls_len);
	start_rectangle2_pt2d2.y+=(14-297);
	CPolylineUtil::AddRectangle(start_rectangle2_pt2d1,start_rectangle2_pt2d2,0);
	//创建圆形
	AcGePoint2d ptCenter_2d=start_rectangle1_pt2d1;
	ptCenter_2d.x-=12.41;
	ptCenter_2d.y-=227;
	CPolylineUtil::AddPolyCircle(ptCenter_2d,7.4,0);
	AcGePoint3d sign_point1=CConvertUtil::ToPoint3d(start_rectangle1_pt2d2);
	sign_point1.x+=-180+63;
	sign_point1.y+=17;
	CString tuname;
	CString tuhao;
	CString riqi;
	CString program_name1;
	CString program_name2;
	//TCHAR*file_sign =_T("C:\\辅助开发\\my_sign.dwg");
	CString strFilePath2 ;
	strFilePath2.Format(_T("%s\\support\\sign\\my_sign.dwg"), g_strBasePath) ;
	TCHAR *file_sign=(TCHAR *)strFilePath2.GetBuffer(strFilePath2.GetLength());
	TCHAR *bsign=_T("my_sign");
	AcDbObjectId blockid = CBlockUtil::InsertDwgBlockDef(file_sign,bsign,1);
	//CBlockUtil::InsertBlockRef(blockid,ptStart);
	AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blockid,sign_point1);
	AcDbBlockReference *pBlkRef =NULL;
	if(acdbOpenObject(pBlkRef,blkRefId,AcDb::kForWrite)==Acad::eOk)
	{
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUNAME_001"),tuname.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("TUHAO_001"),tuhao.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("RIQI_001"),riqi.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME_001"),program_name1.GetString());
		CBlockUtil::SetBlockRefAttribute(pBlkRef,_T("PROGRAM_NAME2_001"),program_name2.GetString());
		pBlkRef->close();
	}
	//以块方式插入
	AcGePoint3d jungong_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
	CString strFilePath ;
	strFilePath.Format(_T("%s\\support\\sign\\xiugai.dwg"), g_strBasePath) ;
	TCHAR *filename =(TCHAR *)strFilePath.GetBuffer(strFilePath.GetLength());
	//TCHAR*filename =_T("C:\\辅助开发\\xiugai.dwg");
	TCHAR *bname =_T("jungong");
	AcDbObjectId blockid1 = CBlockUtil::CopyBlockDefFromOtherDwg(filename,bname);
	//CBlockUtil::InsertBlockRef(blockid,pt_Start);
	CBlockUtil::InsertBlockRef(blockid1,jungong_point);
	AcGePoint3d filename_point =CConvertUtil::ToPoint3d(start_rectangle1_pt2d1);
	filename_point.y-=287;
	TCHAR *bname2 =_T("filename");
	AcDbObjectId blockid2 = CBlockUtil::CopyBlockDefFromOtherDwg(filename,bname2);
	//CBlockUtil::InsertBlockRef(blockid,pt_Start);
	CBlockUtil::InsertBlockRef(blockid2,filename_point);

}

