#include "stdafx.h"
#include "UserSystem.h"
#include "../blkdll/BlkUtitily.h"
//#include "aced.h"
//#include <adslib.h>
// #include <iostream>
// #include <string>
// #include "../mygreta/greta/regexpr2.h"
// using namespace std;
// using namespace regex;
// 
// 
// #ifndef _tstring
// #	ifndef UNICODE
// typedef string _tstring;
// #	else
// typedef wstring _tstring;
// #	endif
// #endif
// 
// #ifndef _tcout
// #	ifndef UNICODE
// #		define _tcout cout
// #	else
// #		define _tcout wcout
// #	endif
// #endif


//对给定的文本实体数组进行排序，排序规则是：
//先按照x坐标从左往右，相同的x坐标，y越小，越后排
//////////////////////////////////////////////////////////////////////////
int CUserSystem::SortText(CArray<AcDbText *, AcDbText *> &arrayent)
{
	//first sort by x;
	//////////////////////////////////////////////////////////////////////////	
	BOOL bSorted = FALSE ;
	int iPass = 0 ;
	while (!bSorted)
	{
		iPass ++ ;
		bSorted = TRUE ;
		for(int i=0; i<arrayent.GetSize()-iPass; i++)
		{
			AcDbText * pTexti = arrayent.GetAt(i) ;
			AcDbText * pTexti1 = arrayent.GetAt(i+1) ;
			AcGePoint3d ptTexti = pTexti->position() ;
			AcGePoint3d ptTexti1 = pTexti1->position() ;
			if (ptTexti.x > ptTexti1.x )
			{
				AcDbText *pTextTmp = pTexti ;
				arrayent.SetAt(i, pTexti1) ;
				arrayent.SetAt(i+1, pTextTmp) ;
				bSorted = FALSE ;				
			}
		}
	}

	//end first sort ;
	//////////////////////////////////////////////////////////////////////////

	//second sort by y ;
	//////////////////////////////////////////////////////////////////////////	
	CArray<int , int > arrayNum ;
	AcDbText *pTextTmp = arrayent.GetAt(0) ;
	double dX0 = (pTextTmp->position()).x ;
	int iNumTmp = 0 ;
	//将按x排序的数组，统计出相同x的个数；
	//如数组：(1 2 2 3 3 3 4 5 5) ，统计结果为:1(1个) 2(2个) 3(三个) 4(1个) 5(2个);
	for(int i=0; i<arrayent.GetSize(); i++)
	{
		pTextTmp = arrayent.GetAt(i) ;
		double dX = (pTextTmp->position()).x ;
		if (dX == dX0)
		{
			iNumTmp ++ ;			
		}
		else
		{
			arrayNum.Add(iNumTmp) ;
			dX0 = dX ;
			iNumTmp = 1 ;
		}		
		if (i==(arrayent.GetSize()-1))
		{
			arrayNum.Add(iNumTmp) ;
		}		
	}

	/*
	int itttt = arrayNum.GetSize() ;
	CString strMsg ;
	strMsg.Format(_T("%d"), itttt) ;
	AfxMessageBox(strMsg) ;
	*/

	//将x坐标相同的数组内容再按y坐标来排序;
	int iStep = 0 ;
	for(int iNum=0; iNum<arrayNum.GetSize(); iNum++)
	{
		if (arrayNum.GetAt(iNum) == 1)
		{
			iStep ++ ;
			continue ;
		}
		else
		{			
			//////////////////////////////////////////////////////////////////////////			
			BOOL bSortedNum = FALSE ;
			int iPassNum = 0 ;
			while (!bSortedNum)
			{
				iPassNum ++ ;
				bSortedNum = TRUE ;
				for(int iNumSort=iStep; iNumSort<iStep+arrayNum.GetAt(iNum)-iPassNum; iNumSort++)
				{
					AcDbText * pTextNumi = arrayent.GetAt(iNumSort) ;
					AcDbText * pTextNumi1 = arrayent.GetAt(iNumSort+1) ;
					AcGePoint3d ptTextNumi = pTextNumi->position() ;
					AcGePoint3d ptTextNumi1 = pTextNumi1->position() ;
					if (ptTextNumi.y < ptTextNumi1.y )
					{
						AcDbText *pTextTmpNum = pTextNumi ;
						arrayent.SetAt(iNumSort, pTextNumi1) ;
						arrayent.SetAt(iNumSort+1, pTextTmpNum) ;
						bSortedNum = FALSE ;				
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			iStep = iStep + arrayNum.GetAt(iNum) ;			
		}
	}
	//end second sort ;
	//////////////////////////////////////////////////////////////////////////

	return 0 ;
}

int CUserSystem::SortEnt(CArray<AcDbText *, AcDbText *> &arrayent, int iMode)
{
	if (iMode==0 || iMode==1)
	{
		//sort by x;
		BOOL bSorted = FALSE ;
		int iPass = 0 ;
		while (!bSorted)
		{
			iPass ++ ;
			bSorted = TRUE ;
			for(int i=0; i<arrayent.GetSize()-iPass; i++)
			{
				AcDbText * pTexti = arrayent.GetAt(i) ;
				AcDbText * pTexti1 = arrayent.GetAt(i+1) ;
				AcGePoint3d ptTexti = pTexti->position() ;
				AcGePoint3d ptTexti1 = pTexti1->position() ;
				if (ptTexti.x > ptTexti1.x )
				{
					AcDbText *pTextTmp = pTexti ;
					arrayent.SetAt(i, pTexti1) ;
					arrayent.SetAt(i+1, pTextTmp) ;
					bSorted = FALSE ;				
				}
			}
		}	

	}
	if (iMode==0 || iMode==2)
	{
		//sort by x;
		BOOL bSorted = FALSE ;
		int iPass = 0 ;
		while (!bSorted)
		{
			iPass ++ ;
			bSorted = TRUE ;
			for(int i=0; i<arrayent.GetSize()-iPass; i++)
			{
				AcDbText * pTexti = arrayent.GetAt(i) ;
				AcDbText * pTexti1 = arrayent.GetAt(i+1) ;
				AcGePoint3d ptTexti = pTexti->position() ;
				AcGePoint3d ptTexti1 = pTexti1->position() ;
				if (ptTexti.y < ptTexti1.y )
				{
					AcDbText *pTextTmp = pTexti ;
					arrayent.SetAt(i, pTexti1) ;
					arrayent.SetAt(i+1, pTextTmp) ;
					bSorted = FALSE ;				
				}
			}
		}	

	}
	return 0 ;

}

int CUserSystem::SortEnt(CArray<AcDbBlockReference *, AcDbBlockReference *> &arrayBlk, int iMode)
{
	if (iMode==0 || iMode==1)
	{
		//sort by x;
		BOOL bSorted = FALSE ;
		int iPass = 0 ;
		while (!bSorted)
		{
			iPass ++ ;
			bSorted = TRUE ;
			for(int i=0; i<arrayBlk.GetSize()-iPass; i++)
			{
				AcDbBlockReference * pBlki = arrayBlk.GetAt(i) ;
				AcDbBlockReference * pBlki1 = arrayBlk.GetAt(i+1) ;
				AcGePoint3d ptBlki = pBlki->position() ;
				AcGePoint3d ptBlki1 = pBlki1->position() ;
				if (ptBlki.x > ptBlki1.x )
				{
					AcDbBlockReference *pBlkTmp = pBlki ;
					arrayBlk.SetAt(i, pBlki1) ;
					arrayBlk.SetAt(i+1, pBlkTmp) ;
					bSorted = FALSE ;				
				}
			}
		}	

	}
	if (iMode==0 || iMode==2)
	{
		//sort by x;
		BOOL bSorted = FALSE ;
		int iPass = 0 ;
		while (!bSorted)
		{
			iPass ++ ;
			bSorted = TRUE ;
			for(int i=0; i<arrayBlk.GetSize()-iPass; i++)
			{
				AcDbBlockReference * pBlki = arrayBlk.GetAt(i) ;
				AcDbBlockReference * pBlki1 = arrayBlk.GetAt(i+1) ;
				AcGePoint3d ptBlki = pBlki->position() ;
				AcGePoint3d ptBlki1 = pBlki1->position() ;
				if (ptBlki.y < ptBlki1.y )
				{
					AcDbBlockReference *pBlkTmp = pBlki ;
					arrayBlk.SetAt(i, pBlki1) ;
					arrayBlk.SetAt(i+1, pBlkTmp) ;
					bSorted = FALSE ;				
				}
			}
		}	

	}
	return 0 ;
}

//改写sortent,iMode=0(xy),1(x),2(y), iEntType默认为0：表示排序的是块，1 文本，其他再添加
int CUserSystem::SortEnt(CLongArray &nArrayHandle, int iMode, int iEntType)
{	
	if (iMode==0 || iMode==1)
	{
		//sort by x;
		BOOL bSorted = FALSE ;
		int iPass = 0 ;
		while (!bSorted)
		{
			iPass ++ ;
			bSorted = TRUE ;
			for(int i=0; i<nArrayHandle.GetSize()-iPass; i++)
			{
				AcGePoint3d pt_3d_blk1(0,0,0) ;
				AcGePoint3d pt_3d_blk2(0,0,0) ;

				AcDbEntity *pEnt = NULL ;				
				if (this->OpenAcDbEntity(nArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
				{
					if (iEntType==0)
					{
						AcDbBlockReference *pRef1 = AcDbBlockReference::cast(pEnt) ;
						pt_3d_blk1 = pRef1->position() ;
					}
					else if (iEntType==1)
					{
						AcDbText *pText1 = AcDbText::cast(pEnt) ;
						pt_3d_blk1 = pText1->position() ;
					}
					pEnt->close() ;
				}
				if (this->OpenAcDbEntity(nArrayHandle.GetAt(i+1),pEnt,AcDb::kForRead))
				{
					if (iEntType==0)
					{
						AcDbBlockReference *pRef2 = AcDbBlockReference::cast(pEnt) ;
						pt_3d_blk2 = pRef2->position() ;
					}
					else if (iEntType==1)
					{
						AcDbText *pText2 = AcDbText::cast(pEnt) ;
						pt_3d_blk2 = pText2->position() ;
					}
					pEnt->close() ;
				}

				if (pt_3d_blk1.x > pt_3d_blk2.x )
				{
					UINT nHandleTmp = nArrayHandle.GetAt(i) ;
					nArrayHandle.SetAt(i, nArrayHandle.GetAt(i+1)) ;
					nArrayHandle.SetAt(i+1, nHandleTmp) ;
					bSorted = FALSE ;				
				}
			}
		}
	}
	if (iMode==0 || iMode==2)
	{
		//sort by x;
		BOOL bSorted = FALSE ;
		int iPass = 0 ;
		while (!bSorted)
		{
			iPass ++ ;
			bSorted = TRUE ;
			for(int i=0; i<nArrayHandle.GetSize()-iPass; i++)
			{
				AcGePoint3d pt_3d_blk1(0,0,0) ;
				AcGePoint3d pt_3d_blk2(0,0,0) ;

				AcDbEntity *pEnt = NULL ;				
				if (this->OpenAcDbEntity(nArrayHandle.GetAt(i),pEnt,AcDb::kForRead))
				{
					if (iEntType==0)
					{
						AcDbBlockReference *pRef1 = AcDbBlockReference::cast(pEnt) ;
						pt_3d_blk1 = pRef1->position() ;
					}
					else if (iEntType==1)
					{
						AcDbText *pText1 = AcDbText::cast(pEnt) ;
						pt_3d_blk1 = pText1->position() ;
					}
					pEnt->close() ;
				}
				if (this->OpenAcDbEntity(nArrayHandle.GetAt(i+1),pEnt,AcDb::kForRead))
				{
					if (iEntType==0)
					{
						AcDbBlockReference *pRef2 = AcDbBlockReference::cast(pEnt) ;
						pt_3d_blk2 = pRef2->position() ;
					}
					else if (iEntType==1)
					{
						AcDbText *pText2 = AcDbText::cast(pEnt) ;
						pt_3d_blk2 = pText2->position() ;
					}
					pEnt->close() ;
				}

				if (pt_3d_blk1.y < pt_3d_blk2.y )
				{
					UINT nHandleTmp = nArrayHandle.GetAt(i) ;
					nArrayHandle.SetAt(i, nArrayHandle.GetAt(i+1)) ;
					nArrayHandle.SetAt(i+1, nHandleTmp) ;
					bSorted = FALSE ;				
				}
			}
		}
	}
		
	return 0 ;
}

void CUserSystem::WriteToRec(CString strDitName,CString strRecName,CString  strVal)
{
	AcDbDictionary * pNamedObj;
	AcDbDictionary * pDit;
	AcDbObjectId DicId,BlockRecId;
	m_pDb->getNamedObjectsDictionary(pNamedObj,AcDb::kForWrite);
	if(pNamedObj->getAt(strDitName,(AcDbObject*&) pDit,AcDb::kForWrite)
		== Acad::eKeyNotFound)
	{
		pDit = new AcDbDictionary;
		pNamedObj->setAt(strDitName,pDit,DicId);
	}
	pNamedObj->close();
	AcDbXrecord * pXBlockRec;
	if(pDit->getAt(strRecName,(AcDbObject*&)pXBlockRec,AcDb::kForWrite)
		== Acad::eOk)
	{
		pXBlockRec->erase(Adesk::kTrue);
		pDit->remove(strRecName);
		pXBlockRec->close();
	}
	pXBlockRec = new AcDbXrecord;
	pDit->setAt(strRecName,pXBlockRec,BlockRecId);
	pDit->close();
	struct resbuf * pHead;
	pHead = ads_buildlist(AcDb::kDxfText,strVal,0);
	pXBlockRec->setFromRbChain(* pHead);
	ads_relrb(pHead);
	pXBlockRec->close();
}
void CUserSystem::WriteToRec(CString strDitName,CString strRecName,int iScale)
{
	AcDbDictionary * pNamedObj;
	AcDbDictionary * pDit;
	AcDbObjectId DicId,BlockRecId;
	m_pDb->getNamedObjectsDictionary(pNamedObj,AcDb::kForWrite);
	if(pNamedObj->getAt(strDitName,(AcDbObject*&) pDit,AcDb::kForWrite)
		== Acad::eKeyNotFound)
	{
		pDit = new AcDbDictionary;
		pNamedObj->setAt(strDitName,pDit,DicId);
	}
	pNamedObj->close();
	AcDbXrecord * pXBlockRec;
	if(pDit->getAt(strRecName,(AcDbObject*&)pXBlockRec,AcDb::kForWrite)
		== Acad::eOk)
	{
		pXBlockRec->erase(Adesk::kTrue);
		pDit->remove(strRecName);
		pXBlockRec->close();
	}
	pXBlockRec = new AcDbXrecord;
	pDit->setAt(strRecName,pXBlockRec,BlockRecId);
	pDit->close();
	struct resbuf * pHead;
	pHead = ads_buildlist(AcDb::kDxfInt16,iScale,0);
	pXBlockRec->setFromRbChain(* pHead);
	ads_relrb(pHead);
	pXBlockRec->close();
}

bool CUserSystem::ReadFromRec(CString strDitName , CString strRecName , int & iVal)
{
	AcDbDictionary * pNamedObj;
	AcDbDictionary * pDit;
	AcDbObjectId DicId,BlockRecId;
	m_pDb->getNamedObjectsDictionary(pNamedObj,AcDb::kForWrite);
	if(pNamedObj->getAt(strDitName,(AcDbObject*&) pDit,AcDb::kForWrite)
		== Acad::eKeyNotFound)
	{
		pNamedObj->close();
		return false;
	}
	pNamedObj->close();
	AcDbXrecord * pXBlockRec ;
	if(Acad::eKeyNotFound ==(pDit->getAt(strRecName,(AcDbObject*&)pXBlockRec,AcDb::kForRead)))
	{
		pDit->close();
		return false;
	}
	struct resbuf * pHead;
	pXBlockRec ->rbChain(&pHead);
	iVal = pHead->resval.rint;
	pXBlockRec ->close();
	ads_relrb(pHead);
	pDit->close();
	return true;
}
bool CUserSystem::ReadFromRec(CString strDitName,CString strRecName,CString  & strVal)
{
	AcDbDictionary * pNamedObj;
	AcDbDictionary * pDit;
	AcDbObjectId DicId,BlockRecId;
	m_pDb->getNamedObjectsDictionary(pNamedObj,AcDb::kForWrite);
	if(pNamedObj->getAt(strDitName,(AcDbObject*&) pDit,AcDb::kForWrite)
		== Acad::eKeyNotFound)
	{
		pNamedObj->close();
		return false;
	}
	pNamedObj->close();
	AcDbXrecord * pXBlockRec ;
	if(Acad::eKeyNotFound ==(pDit->getAt(strRecName,(AcDbObject*&)pXBlockRec,AcDb::kForRead)))
	{
		pDit->close();
		return false;
	}
	struct resbuf * pHead;
	pXBlockRec ->rbChain(&pHead);
	strVal = pHead->resval.rstring;
	pXBlockRec ->close();
	ads_relrb(pHead);
	pDit->close();
	return true;
}


//遍历一个块，返回所有的实体(此处仅处理AcdbLine AcDbPolyline AcDbText AcDbMText AcDbAttribute)id ;
void CUserSystem::GetAllEntIdFromBlock(AcDbBlockReference *pBlockRef, AcGePoint3d insertP,AcDbObjectIdArray &objIdArray)
{
	//得到块的属性
	AcDbObjectIterator * pObjIter ;
	pObjIter = pBlockRef->attributeIterator() ;
	for(pObjIter->start(); !pObjIter->done(); pObjIter->step()) 
	{
		AcDbObjectId attId ;
		attId = pObjIter->objectId() ;
		AcDbAttribute * pAttrib ;
		pBlockRef->openAttribute(pAttrib, attId, AcDb::kForRead, Adesk::kFalse) ;
		objIdArray.append(attId) ;

		//for test;
		//////////////////////////////////////////////////////////////////////////
		/*
		char * pText ;
		char * pTag ;
		AcGePoint3d attPoint(0,0,0) ;
		attPoint = pAttrib->position() ;
		pTag = pAttrib->tag() ;
		pText = pAttrib->textString() ;		
		acutPrintf("属性文本 Object Id %lx,tag:%s,content:%s,--%f-%f\n",attId,pTag,pText, attPoint.x, attPoint.y);
		*/
		//////////////////////////////////////////////////////////////////////////		

		pAttrib->close() ;	
	}


	//打开块表记录
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	AcDbObjectId blkTblObjId ;
	blkTblObjId = pBlockRef->blockTableRecord() ;
	acdbOpenObject(pBlkTblRec, blkTblObjId, AcDb::kForRead) ;

	//遍历块表记录
	AcDbBlockTableRecordIterator * pBlkTblRecIterator ;
	pBlkTblRec->newIterator(pBlkTblRecIterator) ;
	for(; !pBlkTblRecIterator->done(); pBlkTblRecIterator->step())
	{
		AcDbEntity *pEntity;
		pBlkTblRecIterator->getEntity(pEntity, AcDb::kForRead);

		//for test;
		//////////////////////////////////////////////////////////////////////////
		/*
		AcDbHandle objHandle;
		pEntity->getAcDbHandle(objHandle);
		char handleStr[20];
		objHandle.getIntoAsciiBuffer(handleStr);
		*/
		//////////////////////////////////////////////////////////////////////////		

		const TCHAR *pCname = pEntity->isA()->name();
		//if (strcmp(pCname, "AcDbText")==0 || strcmp(pCname, "AcDbMText")==0 || strcmp(pCname, "AcDbLine")==0 || strcmp(pCname, "AcDbPolyline") == 0)
		//{

		//	objIdArray.append(pEntity->objectId()) ;
		//}

		//////////////////////////////////////////////////////////////////////////

		if (_tcscmp(pCname, _T("AcDbText")) == 0)
		{
			/*
			AcDbText * pText = NULL ;
			pText = (AcDbText *)pEntity ;
			char const * pchText ;
			pchText = pText->textString() ;
			acutPrintf("Object Id %lx, handle %s, class %s, content %s.\n",
			pEntity->objectId(), handleStr, pCname, pchText);
			*/

		}
		else if (_tccmp(pCname, _T("AcDbMText")) == 0) 
		{/*
		 AcDbMText * pMtext = NULL ;
		 pMtext = (AcDbMText *)pEntity ;
		 char const *pchMtext ;
		 pchMtext = pMtext->contents() ;
		 acutPrintf("Object Id %lx, handle %s, class %s, content %s.\n",
		 pEntity->objectId(), handleStr, pCname, pchMtext);		
		 */
		}
		else if (_tcscmp(pCname, _T("AcDbLine")) == 0)
		{
			AcDbLine * pLineTest = NULL ;
			pLineTest = (AcDbLine *)pEntity ;

			//acutPrintf("id:%d,(%f,%f)\n", pLineTest->objectId(), pLineTest->startPoint().x+insertP.x, pLineTest->startPoint().y+insertP.y) ;

		}

		else if (_tcscmp(pCname, _T("AcDbPolyline")) == 0)
		{
			AcDbPolyline *pPolyline = NULL ;
			pPolyline = (AcDbPolyline *) pEntity ;
			AcDbVoidPtrArray pVoidArray = NULL ;

			pPolyline->explode(pVoidArray) ;

			for(int iLineNum=0; iLineNum<pVoidArray.length(); iLineNum++)
			{
				AcDbLine * pEntLine = NULL ;
				pEntLine = (AcDbLine *)(pVoidArray.at(iLineNum)) ;
				objIdArray.append(pEntLine->objectId()) ;
				//acutPrintf("id:%d,(%f,%f)\n", pEntLine->objectId(), pEntLine->startPoint().x+insertP.x, pEntLine->startPoint().y+insertP.y) ;
				pEntLine->close() ;
			}	

		}
		//////////////////////////////////////////////////////////////////////////


		else if (_tcscmp(pCname, _T("AcDbBlockReference")) == 0) 
		{
			AcDbBlockReference * pBlkRef = NULL ;
			pBlkRef = (AcDbBlockReference *)pEntity ;
			AcGePoint3d insertP2(0,0,0) ;
			insertP2.x = pBlkRef->position().x + insertP.x ;
			insertP2.y = pBlkRef->position().y + insertP.y ;
			GetAllEntIdFromBlock(pBlkRef, insertP2, objIdArray) ;
		}

		pEntity->close() ;

	}
	delete pBlkTblRecIterator ;
	pBlkTblRec->close() ;
	//acutPrintf("\n") ;	

}


//遍历一个块，返回所有的实体(此处仅处理AcdbLine AcDbPolyline AcDbText AcDbMText AcDbAttribute)id ;
void CUserSystem::GetAllEntIdFromBlock(AcDbBlockReference *pBlockRef, AcGePoint3d insertP,CArray<AcDbLine*, AcDbLine *> &arrayLine, CArray<AcDbText*, AcDbText *> &arrayText)
{

	//打开块表记录
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	AcDbObjectId blkTblObjId ;
	blkTblObjId = pBlockRef->blockTableRecord() ;
	acdbOpenObject(pBlkTblRec, blkTblObjId, AcDb::kForRead) ;

	//遍历块表记录
	AcDbBlockTableRecordIterator * pBlkTblRecIterator ;
	pBlkTblRec->newIterator(pBlkTblRecIterator) ;
	for(pBlkTblRecIterator->start(); !pBlkTblRecIterator->done(); pBlkTblRecIterator->step())
	{
		AcDbEntity *pEntity = NULL ;
		pBlkTblRecIterator->getEntity(pEntity, AcDb::kForRead);	

		const TCHAR *pCname = pEntity->isA()->name();

		if (AcDb::kVisible==pEntity->visibility())
		{		
			if (_tcscmp(pCname, _T("AcDbText")) == 0)
			{		
				AcDbText * pText = NULL ;
				pText = (AcDbText *)pEntity ;
				AcGePoint3d textp_3d(0,0,0) ;

				AcDbText * pTextTmp = new AcDbText ;
				pTextTmp->setTextString(pText->textString()) ;
				textp_3d.x = pText->position().x + insertP.x ;
				textp_3d.y = pText->position().y + insertP.y ;
				pTextTmp->setPosition(textp_3d) ;
				arrayText.Add(pTextTmp) ;

				pText->close() ;
			}
			else if (_tcscmp(pCname, _T("AcDbMText")) == 0) 
			{
				AcDbMText * pMtext = NULL ;
				pMtext = (AcDbMText *)pEntity ;
				AcGePoint3d textp_3d(0,0,0) ;			

				AcDbText * pTextTmp = new AcDbText ;
				pTextTmp->setTextString(pMtext->contents()) ;
				textp_3d.x = pMtext->location().x + insertP.x ;
				textp_3d.y = pMtext->location().y + insertP.y ;
				pTextTmp->setPosition(textp_3d) ;
				arrayText.Add(pTextTmp) ;

				pMtext->close() ;

			}
			else if (_tcscmp(pCname, _T("AcDbLine")) == 0)
			{
				AcDbLine * pLine = NULL ;
				pLine = (AcDbLine *)pEntity ;
				AcGePoint3d startP_3d(0,0,0) ;
				AcGePoint3d endP_3d(0,0,0) ;

				startP_3d.x = pLine->startPoint().x + insertP.x ;
				startP_3d.y = pLine->startPoint().y + insertP.y ;
				endP_3d.x = pLine->endPoint().x + insertP.x ;
				endP_3d.y = pLine->endPoint().y + insertP.y ;

				//和x轴y轴平行
				if (startP_3d.x==endP_3d.x || startP_3d.y==endP_3d.y)
				{
					AcDbLine * pLineTmp = new AcDbLine ;
					pLineTmp->setStartPoint(startP_3d) ;
					pLineTmp->setEndPoint(endP_3d) ;

					//acutPrintf("(%d,%d)-(%d,%d)", startP_3d.x, startP_3d.y, endP_3d.x, endP_3d.y) ;

					arrayLine.Add(pLineTmp) ;		
				}
				else
				{
					//直线斜率
					double dTanArc = (endP_3d.y-startP_3d.y)/(endP_3d.x-startP_3d.x) ;
					//和x轴夹角小于1度的
					if (fabs(dTanArc)<0.017455) 
					{
						AcDbLine * pLineTmp = new AcDbLine ;

						double dMid_y = (endP_3d.y+startP_3d.y)/2 ;
						startP_3d.y = dMid_y ;
						endP_3d.y = dMid_y ;	
						pLineTmp->setStartPoint(startP_3d) ;
						pLineTmp->setEndPoint(endP_3d) ;

						//acutPrintf("(%d,%d)-(%d,%d)", startP_3d.x, startP_3d.y, endP_3d.x, endP_3d.y) ;					

						arrayLine.Add(pLineTmp) ;				
					}
					//和y轴夹角小于1度的
					else if (fabs(dTanArc)>57.289962) 
					{
						AcDbLine * pLineTmp = new AcDbLine ;

						double dMid_x = (endP_3d.x+startP_3d.x)/2 ;
						startP_3d.x = dMid_x ;
						endP_3d.x = dMid_x ;
						pLineTmp->setStartPoint(startP_3d) ;
						pLineTmp->setEndPoint(endP_3d) ;

						//acutPrintf("(%d,%d)-(%d,%d)", startP_3d.x, startP_3d.y, endP_3d.x, endP_3d.y) ;					

						arrayLine.Add(pLineTmp) ;				
					}
				}

				pLine->close() ;			
			}

			else if (_tcscmp(pCname, _T("AcDbPolyline")) == 0)
			{
				AcDbPolyline *pPolyline = NULL ;
				pPolyline = (AcDbPolyline *) pEntity ;


				for(int iPolyLineVert=0; iPolyLineVert<(pPolyline->numVerts()-1); iPolyLineVert++)
				{		
					AcGePoint3d startP_3d(0,0,0) ;
					AcGePoint3d endP_3d(0,0,0) ;
					pPolyline->getPointAt(iPolyLineVert, startP_3d) ;
					pPolyline->getPointAt(iPolyLineVert+1, endP_3d) ;
					startP_3d.x = startP_3d.x + insertP.x ;
					startP_3d.y = startP_3d.y + insertP.y ;
					endP_3d.x = endP_3d.x + insertP.x ;
					endP_3d.y = endP_3d.y +insertP.y ;	

					//和x轴y轴平行
					if (startP_3d.x==endP_3d.x || startP_3d.y==endP_3d.y)
					{
						AcDbLine * pLineTmp = new AcDbLine ;
						pLineTmp->setStartPoint(startP_3d) ;
						pLineTmp->setEndPoint(endP_3d) ;

						arrayLine.Add(pLineTmp) ;		
					}
					else
					{					
						//直线斜率
						double dTanArc = (endP_3d.y-startP_3d.y)/(endP_3d.x-startP_3d.x) ;

						//和x轴夹角小于1度的
						if (fabs(dTanArc)<0.017455) 
						{
							AcDbLine * pLineTmp = new AcDbLine ;

							double dMid_y = (endP_3d.y+startP_3d.y)/2 ;
							startP_3d.y = dMid_y ;
							endP_3d.y = dMid_y ;	
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;

							arrayLine.Add(pLineTmp) ;				
						}
						//和y轴夹角小于1度的
						else if (fabs(dTanArc)>57.289962) 
						{
							AcDbLine * pLineTmp = new AcDbLine ;

							double dMid_x = (endP_3d.x+startP_3d.x)/2 ;
							startP_3d.x = dMid_x ;
							endP_3d.x = dMid_x ;
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;

							arrayLine.Add(pLineTmp) ;				
						}
					}

				}
				if (Adesk::kTrue==pPolyline->isClosed())
				{
					AcGePoint3d startP_3d(0,0,0) ;
					AcGePoint3d endP_3d(0,0,0) ;
					pPolyline->getPointAt(0, startP_3d) ;
					pPolyline->getPointAt(pPolyline->numVerts()-1, endP_3d) ;
					startP_3d.x = startP_3d.x + insertP.x ;
					startP_3d.y = startP_3d.y + insertP.y ;
					endP_3d.x = endP_3d.x + insertP.x ;
					endP_3d.y = endP_3d.y +insertP.y ;	

					//和x轴y轴平行
					if (startP_3d.x==endP_3d.x || startP_3d.y==endP_3d.y)
					{
						AcDbLine * pLineTmp = new AcDbLine ;
						pLineTmp->setStartPoint(startP_3d) ;
						pLineTmp->setEndPoint(endP_3d) ;

						arrayLine.Add(pLineTmp) ;		
					}
					else
					{					
						//直线斜率
						double dTanArc = (endP_3d.y-startP_3d.y)/(endP_3d.x-startP_3d.x) ;

						//和x轴夹角小于1度的
						if (fabs(dTanArc)<0.017455) 
						{
							AcDbLine * pLineTmp = new AcDbLine ;

							double dMid_y = (endP_3d.y+startP_3d.y)/2 ;
							startP_3d.y = dMid_y ;
							endP_3d.y = dMid_y ;	
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;

							arrayLine.Add(pLineTmp) ;				
						}
						//和y轴夹角小于1度的
						else if (fabs(dTanArc)>57.289962) 
						{
							AcDbLine * pLineTmp = new AcDbLine ;

							double dMid_x = (endP_3d.x+startP_3d.x)/2 ;
							startP_3d.x = dMid_x ;
							endP_3d.x = dMid_x ;
							pLineTmp->setStartPoint(startP_3d) ;
							pLineTmp->setEndPoint(endP_3d) ;

							arrayLine.Add(pLineTmp) ;				
						}
					}


				}
				pPolyline->close() ;			
			}
			else if (_tcscmp(pCname, _T("AcDbBlockReference")) == 0) 
			{
				AcDbBlockReference * pBlkRef = NULL ;
				pBlkRef = (AcDbBlockReference *)pEntity ;

				//得到块的属性
				AcDbObjectIterator * pObjIter ;
				pObjIter = pBlkRef->attributeIterator() ;
				for(pObjIter->start(); !pObjIter->done(); pObjIter->step()) 
				{
					AcDbObjectId attId ;
					attId = pObjIter->objectId() ;
					AcDbAttribute * pAttrib ;
					pBlkRef->openAttribute(pAttrib, attId, AcDb::kForRead, Adesk::kFalse) ;

					AcDbText * pTextAtt = new AcDbText ;
					pTextAtt->setTextString(pAttrib->textString()) ;
					AcGePoint3d attP_3d(0,0,0) ;
					attP_3d.x = pAttrib->position().x + insertP.x ;
					attP_3d.y = pAttrib->position().y + insertP.y ;
					pTextAtt->setPosition(attP_3d) ;
					arrayText.Add(pTextAtt) ;

					pAttrib->close() ;	
				}

				delete pObjIter ;
				pObjIter = NULL ;

				AcGePoint3d insertP2(0,0,0) ;
				insertP2.x = pBlkRef->position().x + insertP.x ;
				insertP2.y = pBlkRef->position().y + insertP.y ;			
				//递归获取进一步块信息
				GetAllEntIdFromBlock(pBlkRef, insertP2,arrayLine, arrayText) ;

				pBlkRef->close() ;
			}

		}
		pEntity->close() ;

	}
	delete pBlkTblRecIterator ;
	pBlkTblRec->close() ;

}

//从线数组中将竖线的X坐标，横线的Y坐标读取出来，并且不要重复，从小到大排序x，从大到小排序y
void CUserSystem::GetXY(CArray<AcDbLine *, AcDbLine *> &arrayLine, CArray<double, double> &arrayX, CArray<double, double> &arrayY) 
{
	//arrayX.RemoveAll() ;
	//arrayY.RemoveAll() ;

	int iarrayLineLen = arrayLine.GetSize() ;

	for(int iLineIndex=0; iLineIndex<iarrayLineLen; iLineIndex++)
	{
		AcDbLine *pLineTmp = NULL ;
		pLineTmp = arrayLine.GetAt(iLineIndex) ;

		double dStartPx = 0, dStartPy = 0, dEndPx = 0, dEndPy = 0 ;
		AcGePoint3d startP(0,0,0) ;
		AcGePoint3d endP(0,0,0) ;

		pLineTmp->getStartPoint(startP) ;
		pLineTmp->getEndPoint(endP) ;

		dStartPx = startP.x ;
		dStartPy = startP.y ;
		dEndPx = endP.x ;
		dEndPy = endP.y ;

		int iarrayXLen = arrayX.GetSize() ;
		int iarrayYLen = arrayY.GetSize() ;


		//vertical, store into arrayX
		if(dStartPx == dEndPx)
		{
			if (iarrayXLen==0)
			{
				arrayX.Add(dStartPx) ;
			}
			else
			{
				for(int iXIndex=0; iXIndex<iarrayXLen; iXIndex++)
				{
					if (dStartPx==arrayX.GetAt(iXIndex))
					{
						break ;
					}
					else if (dStartPx<arrayX.GetAt(iXIndex))
					{
						arrayX.InsertAt(iXIndex, dStartPx) ;
						break ;
					}
					else if (dStartPx>arrayX.GetAt(iXIndex))
					{
						if (iXIndex==(iarrayXLen-1))
						{
							arrayX.Add(dStartPx) ;
							break ;
						}
						continue ;
					}
				}

			}
		}
		//horizontal, store into arrayY
		else if (dStartPy == dEndPy )
		{
			if (iarrayYLen==0)
			{
				arrayY.Add(dStartPy) ;
			}
			else
			{
				for(int iYIndex=0; iYIndex<iarrayYLen; iYIndex++)
				{
					if (dStartPy==arrayY.GetAt(iYIndex))
					{
						break ;
					}
					else if (dStartPy>arrayY.GetAt(iYIndex))
					{	
						arrayY.InsertAt(iYIndex, dStartPy) ;
						break ;
					}
					else if (dStartPy<arrayY.GetAt(iYIndex))
					{
						if (iYIndex==(iarrayYLen-1)) 
						{
							arrayY.Add(dStartPy) ;
							break ;
						}
						continue ;
					}
				}

			}


		}
		//pLineTmp->close() ;
	}
}

//判断一个点是否在某一条直线上
BOOL CUserSystem::IsPointInLine(const AcGePoint3d &pt_3d, const AcDbLine *pLine) 
{
	BOOL bRet = FALSE ;

	AcGePoint3d startP_3d(0,0,0) ;
	AcGePoint3d endP_3d(0,0,0) ;

	pLine->getStartPoint(startP_3d) ;
	pLine->getEndPoint(endP_3d) ;

	AcGeLineSeg3d LineSeg3d(startP_3d, endP_3d) ;		
	if (Adesk::kTrue==LineSeg3d.isOn(pt_3d)) 
	{
		bRet = TRUE ;			
	}

	return bRet ;
}

//判断一个点是否在提供的直线数组中的某一条直线上
BOOL CUserSystem::IsPointInAnyLine(const AcGePoint3d &pt_3d, const CArray<AcDbLine *, AcDbLine *> &arrayLine) 
{
	BOOL bReturn = FALSE ;

	int iarraylineLen = arrayLine.GetSize() ;
	for(int iLineIndex=0; iLineIndex<iarraylineLen; iLineIndex++)
	{
		AcDbLine *pLine = NULL ;
		pLine = arrayLine.GetAt(iLineIndex) ;
		if (IsPointInLine(pt_3d,pLine)) 
		{
			bReturn = TRUE ;
			break ;
		}
	}
	return bReturn ;			
}

//查询点pt_2d_on是否在多段线pPolyLine上，不在，返回结果-1。若在，返回值<1000，表示此点在某个顶点；返回值=1000，表示此点不在某个顶点。nVertexIndex返回查询点在第几段（base 0）
int CUserSystem::IsPointOnPolyLine(const AcDbPolyline *pPolyLine, const AcGePoint2d &pt_2d_on, UINT &nVertexIndex, double dTol)
{
	int iRet = -1 ;

	AcGeTol tol ;
	tol.setEqualPoint(dTol) ;

	UINT nNumVerts = pPolyLine->numVerts() ;
	for (int i=0; i<nNumVerts-1; i++)
	{
		AcGeLineSeg2d lineSeg2d ;
		pPolyLine->getLineSegAt(i, lineSeg2d) ;
		if (lineSeg2d.isOn(pt_2d_on,tol))
		{
			nVertexIndex = i ;
			iRet = 1000 ;
			break;
		}
	}
	if (iRet==1000)
	{
		AcGePoint2d pt_2d_vert(0,0) ;
		for (int i=0; i<nNumVerts; i++)
		{
			pPolyLine->getPointAt(i, pt_2d_vert) ;
			//if (pt_2d_vert.distanceTo(pt_2d_on)<=dTol)
			if (pt_2d_on.isEqualTo(pt_2d_vert, tol))
			{
// 				double dd = pt_2d_vert.distanceTo(pt_2d_on) ;
// 				acutPrintf(_T("\n顶点%.3f,%.3f 距离 查询点 %.3f, %.3f:%.3f"), pt_2d_vert.x, pt_2d_vert.y, pt_2d_on.x, pt_2d_on.y, dd) ;
				iRet = i ;
				nVertexIndex = i ;
				break;
			}
		}
	}

	return iRet ;
}
int CUserSystem::IsPointOnPolyLine(const AcDbPolyline *pPolyLine, const AcGePoint3d &pt_3d_on, UINT &nVertexIndex, double dTol)
{
	AcGePoint2d pt_2d_on(0,0) ;
	this->ConvertPoint(pt_3d_on, pt_2d_on) ;
	return IsPointOnPolyLine(pPolyLine, pt_2d_on, nVertexIndex, dTol) ;
}

//取得直线长度
double CUserSystem::GetLineLen(const AcDbLine *pLine)
{
	double dLen = 0 ;
	pLine->getDistAtPoint(pLine->endPoint(), dLen) ;
	return dLen ;
}

//取得多段线长度
double CUserSystem::GetLineLen(const AcDbPolyline *pPLine)
{
	double dLen = 0 ;
	int iNumVert = pPLine->numVerts() ;
	for (int i=0; i<iNumVert-1; i++)
	{
		AcGeLineSeg2d lineSeg2d ;
		pPLine->getLineSegAt(i, lineSeg2d) ;
		dLen+=lineSeg2d.length() ;		
	}

	return dLen ;
}

//转换文本对齐方式为序号（在下拉对话框中的序号）
BOOL CUserSystem::ConvertTextModeToIndex(AcDb::TextHorzMode iHorMode, AcDb::TextVertMode iVerMode, int &iIndex)
{
	BOOL bRet = FALSE ;

	switch(iVerMode) {
	case kTextBase:
		{
			switch(iHorMode) 
			{
			case kTextLeft:
				iIndex = 0 ;
				break;
			case kTextCenter:
				iIndex = 1 ;
				break;
			case kTextRight:
				iIndex = 2 ;
				break;
			case kTextAlign:
				iIndex = 3 ;
				break;
			case kTextMid:
				iIndex = 4 ;
				break;
			case kTextFit:
				iIndex = 5 ;
				break;
			default:
				break;
			}
		}

		break;
	case kTextTop:
		{
			switch(iHorMode) 
			{
			case kTextLeft:
				iIndex = 6 ;
				break;
			case kTextCenter:
				iIndex = 7 ;
				break;
			case kTextRight:
				iIndex = 8 ;
				break;			
			default:
				break;
			}
		}
		break;
	case kTextVertMid:
		{
			switch(iHorMode) 
			{
			case kTextLeft:
				iIndex = 9 ;
				break;
			case kTextCenter:
				iIndex = 10 ;
				break;
			case kTextRight:
				iIndex = 11 ;
				break;			
			default:
				break;
			}
		}
		break;
	case kTextBottom:
		{
			switch(iHorMode) 
			{
			case kTextLeft:
				iIndex = 12 ;
				break;
			case kTextCenter:
				iIndex = 13 ;
				break;
			case kTextRight:
				iIndex = 14 ;
				break;			
			default:
				break;
			}
		}
		break;

	default:
		break;
	}

	if (iIndex>-1) 
	{
		bRet = TRUE ;
	}

	return bRet ;
}


//序号（在cad对齐列表框中的顺序）转换为对齐方式
BOOL CUserSystem::ConvertIndexToTextMode(int iIndex, AcDb::TextHorzMode &iHorMode, AcDb::TextVertMode &iVerMode)
{
	BOOL bRet = FALSE ;

	//int textMode[15][2] = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{0,3},{1,3},{2,3},{0,2},{1,2},{2,2},{0,1},{1,1},{2,1}} ;
	int textMode[15][2] = {{AcDb::kTextLeft,AcDb::kTextBase},{AcDb::kTextCenter,AcDb::kTextBase},{AcDb::kTextRight,AcDb::kTextBase},{AcDb::kTextAlign,AcDb::kTextBase},{AcDb::kTextMid,AcDb::kTextBase},{AcDb::kTextFit,AcDb::kTextBase},{AcDb::kTextLeft,AcDb::kTextTop},{AcDb::kTextCenter,AcDb::kTextTop},{AcDb::kTextRight,AcDb::kTextTop},{AcDb::kTextLeft,AcDb::kTextVertMid},{AcDb::kTextCenter,AcDb::kTextVertMid},{AcDb::kTextRight,AcDb::kTextVertMid},{AcDb::kTextLeft,AcDb::kTextBottom},{AcDb::kTextCenter,AcDb::kTextBottom},{AcDb::kTextRight,AcDb::kTextBottom}} ;

	iHorMode = (AcDb::TextHorzMode)textMode[iIndex][0] ;
	iVerMode = (AcDb::TextVertMode)textMode[iIndex][1] ;

	if (iHorMode!=-1&&iVerMode!=-1)
	{
		bRet = TRUE ;
	}

	return bRet ;
}


//转换文本的对齐模式，但不移动文本
BOOL CUserSystem::ConvertTextMode(AcDbText *&pText, int iTextModeNew, int iColorIndex )
{
	BOOL bRet = FALSE ;

	AcDbObjectId objId = NULL ;
	objId = pText->objectId() ;
	ads_name temp_name ;
	acdbGetAdsName(temp_name, objId) ;

	AcDbEntity * pEntTmp = NULL ;

	AcGePoint3d textP_3d(0, 0, 0) ;
	AcGePoint3d textP_3d2(0, 0, 0) ;

	AcGePoint3d alignPt_3d(0, 0, 0) ;
	AcGePoint3d positionPt_3d(0, 0, 0) ;
	double dHeight = 0.0 ;
	double dRotation = 0.0 ;
	double dWidthFactor = 0.0 ;

	dHeight = pText->height() ;
	dRotation = pText->rotation() ;
	dWidthFactor = pText->widthFactor() ;


	AcDb::TextVertMode iVerMode = pText->verticalMode() ;
	AcDb::TextHorzMode iHorMode = pText->horizontalMode() ;

	int iTextModeOld = -1 ;
	this->ConvertTextModeToIndex(iHorMode, iVerMode, iTextModeOld) ;

	if (iTextModeOld!=0)
	{	
		alignPt_3d = pText->alignmentPoint() ;						
	}
	positionPt_3d = pText->position() ;							

	switch(iTextModeOld)  
	{
	case 0:	
		{
			switch(iTextModeNew) 
			{
			case 0:										
				break;																
			case 1:	
				//pText->setVerticalMode(kTextBase) ;
				pText->setHorizontalMode(kTextCenter) ;	
				break;
			case 2:
				//pText->setVerticalMode(kTextBase) ;
				pText->setHorizontalMode(kTextRight) ;	
				break;									
			case 3:	
				//pText->setVerticalMode(kTextBase) ;
				pText->setHorizontalMode(kTextRight) ;	
				break;
			case 4:
				//pText->setVerticalMode(kTextBase) ;
				pText->setHorizontalMode(kTextMid) ;	
				break;
			case 5:	
				//pText->setVerticalMode(kTextBase) ;
				pText->setHorizontalMode(kTextRight) ;	
				break;
			case 6:
				pText->setVerticalMode(kTextTop) ;
				pText->setHorizontalMode(kTextLeft) ;	
				break;
			case 7:
				pText->setVerticalMode(kTextTop) ;
				pText->setHorizontalMode(kTextCenter) ;	
				break;
			case 8:
				pText->setVerticalMode(kTextTop) ;
				pText->setHorizontalMode(kTextRight) ;	
				break;
			case 9:
				pText->setVerticalMode(kTextVertMid) ;
				pText->setHorizontalMode(kTextLeft) ;	
				break;
			case 10:
				pText->setVerticalMode(kTextVertMid) ;
				pText->setHorizontalMode(kTextCenter) ;	
				break;
			case 11:
				pText->setVerticalMode(kTextVertMid) ;
				pText->setHorizontalMode(kTextRight) ;	
				break;
			case 12:
				pText->setVerticalMode(kTextBottom) ;
				pText->setHorizontalMode(kTextLeft) ;	
				break;
			case 13:
				pText->setVerticalMode(kTextBottom) ;
				pText->setHorizontalMode(kTextCenter) ;	
				break;
			case 14:
				pText->setVerticalMode(kTextBottom) ;
				pText->setHorizontalMode(kTextRight) ;
				break;
			default:
				break;
			}

			if (iTextModeNew!=0)
			{
				CString strMsg ;
				strMsg.Format(_T("aold.x:%f-pold.x:%f\n"), alignPt_3d.x, positionPt_3d.x) ;
				ads_printf(strMsg) ;								
				pText->setAlignmentPoint(positionPt_3d) ;										
				pText->close() ;				
				pText = NULL ;		


				if (OpenAcDbEntity(temp_name, pEntTmp))
				{											
					pText = (AcDbText * )pEntTmp ;
					AcGePoint3d alignPt_3dTmp(0, 0, 0) ;
					AcGePoint3d positionPt_3dTmp(0, 0, 0) ;
					alignPt_3dTmp = pText->alignmentPoint() ;
					positionPt_3dTmp = pText->position() ;

					positionPt_3d.x = positionPt_3d.x + (alignPt_3dTmp.x-positionPt_3dTmp.x) ;
					positionPt_3d.y = positionPt_3d.y + (alignPt_3dTmp.y-positionPt_3dTmp.y) ;
					if (iTextModeNew==3)
					{
						pText->setHorizontalMode(kTextAlign) ;

					}
					else if (iTextModeNew==5)
					{
						pText->setHorizontalMode(kTextFit) ;

					}
					pText->setAlignmentPoint(positionPt_3d) ;									
					pText->setPosition(alignPt_3dTmp) ;
					if (iColorIndex!=-1)
					{
						pText->setColorIndex(iColorIndex) ;
					}
					strMsg.Format(_T("anew.x:%f-pnew.x:%f\n"), alignPt_3dTmp.x, positionPt_3dTmp.x) ;
					ads_printf(strMsg) ;	
				}
				pEntTmp->close() ;
				pEntTmp = NULL ;
			}

		}
		break;
	case 1:
	case 2:	
	case 3:	
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		{
			if (iTextModeOld != iTextModeNew) 
			{									
				switch(iTextModeNew) 
				{
				case 0:	
					pText->setVerticalMode(kTextBase) ;
					pText->setHorizontalMode(kTextLeft) ;	
					break;																
				case 1:	
					pText->setVerticalMode(kTextBase) ;
					pText->setHorizontalMode(kTextCenter) ;	
					break;
				case 2:
					pText->setVerticalMode(kTextBase) ;
					pText->setHorizontalMode(kTextRight) ;	
					break;									
				case 3:																		 
					{
						CString strMsg ;
						strMsg.Format(_T("aold.x:%f-pold.x:%f\n"), alignPt_3d.x, positionPt_3d.x) ;
						ads_printf(strMsg) ;

						pText->setHorizontalMode(kTextRight) ;
						pText->close() ;							
						pText = NULL ;

						if (OpenAcDbEntity(temp_name, pEntTmp))
						{											
							pText = (AcDbText * )pEntTmp ;
							AcGePoint3d alignPt_3dTmp(0, 0, 0) ;
							AcGePoint3d positionPt_3dTmp(0, 0, 0) ;
							AcGePoint3d positionPt_3dTmp2(0, 0, 0) ;

							positionPt_3dTmp2.x = positionPt_3d.x ;
							positionPt_3dTmp2.y = positionPt_3d.y ;

							alignPt_3dTmp = pText->alignmentPoint() ;
							positionPt_3dTmp = pText->position() ;

							positionPt_3d.x = positionPt_3d.x + (alignPt_3dTmp.x-positionPt_3dTmp.x) ;
							//positionPt_3d.y = positionPt_3d.y + (alignPt_3dTmp.y-positionPt_3dTmp.y) ;					

							pText->setVerticalMode(kTextBase) ;	
							pText->setHorizontalMode(kTextAlign) ;
							pText->setAlignmentPoint(positionPt_3d) ;									
							pText->setPosition(positionPt_3dTmp2) ;
							if (iColorIndex!=-1)
							{
								pText->setColorIndex(iColorIndex) ;
							}
							strMsg.Format(_T("anew.x:%f-pnew.x:%f\n"), alignPt_3dTmp.x, positionPt_3dTmp.x) ;
							ads_printf(strMsg) ;	
						}
						pEntTmp->close() ;
						pEntTmp = NULL ;
					}
					break;
				case 4:
					pText->setVerticalMode(kTextBase) ;
					pText->setHorizontalMode(kTextMid) ;	
					break;
				case 5:											
					{
						CString strMsg ;
						strMsg.Format(_T("aold.x:%f-pold.x:%f\n"), alignPt_3d.x, positionPt_3d.x) ;
						ads_printf(strMsg) ;

						pText->setHorizontalMode(kTextRight) ;
						pText->close() ;
						//pEnt = NULL ;
						pText = NULL ;

						if (OpenAcDbEntity(temp_name, pEntTmp))
						{											
							pText = (AcDbText * )pEntTmp ;
							AcGePoint3d alignPt_3dTmp(0, 0, 0) ;
							AcGePoint3d positionPt_3dTmp(0, 0, 0) ;
							AcGePoint3d positionPt_3dTmp2(0, 0, 0) ;

							positionPt_3dTmp2.x = positionPt_3d.x ;
							positionPt_3dTmp2.y = positionPt_3d.y ;

							alignPt_3dTmp = pText->alignmentPoint() ;
							positionPt_3dTmp = pText->position() ;

							positionPt_3d.x = positionPt_3d.x + (alignPt_3dTmp.x-positionPt_3dTmp.x) ;
							//positionPt_3d.y = positionPt_3d.y + (alignPt_3dTmp.y-positionPt_3dTmp.y) ;					

							pText->setVerticalMode(kTextBase) ;
							pText->setHorizontalMode(kTextFit) ;
							pText->setAlignmentPoint(positionPt_3d) ;									
							pText->setPosition(positionPt_3dTmp2) ;
							if (iColorIndex!=-1)
							{
								pText->setColorIndex(iColorIndex) ;
							}
							strMsg.Format(_T("anew.x:%f-pnew.x:%f\n"), alignPt_3dTmp.x, positionPt_3dTmp.x) ;
							ads_printf(strMsg) ;	
						}
						pEntTmp->close() ;
						pEntTmp = NULL ;
					}
					break;
				case 6:
					pText->setVerticalMode(kTextTop) ;
					pText->setHorizontalMode(kTextLeft) ;	
					break;
				case 7:
					pText->setVerticalMode(kTextTop) ;
					pText->setHorizontalMode(kTextCenter) ;	
					break;
				case 8:
					pText->setVerticalMode(kTextTop) ;
					pText->setHorizontalMode(kTextRight) ;	
					break;
				case 9:
					pText->setVerticalMode(kTextVertMid) ;
					pText->setHorizontalMode(kTextLeft) ;	
					break;
				case 10:
					pText->setVerticalMode(kTextVertMid) ;
					pText->setHorizontalMode(kTextCenter) ;	
					break;
				case 11:
					pText->setVerticalMode(kTextVertMid) ;
					pText->setHorizontalMode(kTextRight) ;	
					break;
				case 12:
					pText->setVerticalMode(kTextBottom) ;
					pText->setHorizontalMode(kTextLeft) ;	
					break;
				case 13:
					pText->setVerticalMode(kTextBottom) ;
					pText->setHorizontalMode(kTextCenter) ;	
					break;
				case 14:
					pText->setVerticalMode(kTextBottom) ;
					pText->setHorizontalMode(kTextRight) ;
					break;
				default:
					break;
				}

				if (iTextModeNew==0)
				{
					pText->setPosition(positionPt_3d) ;
				}
				else if(iTextModeNew!=3 && iTextModeNew!=5)
				{
					CString strMsg ;
					strMsg.Format(_T("aold.x:%f-pold.x:%f\n"), alignPt_3d.x, positionPt_3d.x) ;
					ads_printf(strMsg) ;								

					pText->setAlignmentPoint(positionPt_3d) ;										
					pText->close() ;
					//pEnt = NULL ;
					pText = NULL ;

					if (OpenAcDbEntity(temp_name, pEntTmp))
					{											
						pText = (AcDbText * )pEntTmp ;
						AcGePoint3d alignPt_3dTmp(0, 0, 0) ;
						AcGePoint3d positionPt_3dTmp(0, 0, 0) ;
						AcGePoint3d positionPt_3dTmp2(0, 0, 0) ;

						positionPt_3dTmp2.x = positionPt_3d.x ;
						positionPt_3dTmp2.y = positionPt_3d.y ;

						alignPt_3dTmp = pText->alignmentPoint() ;
						positionPt_3dTmp = pText->position() ;

						positionPt_3d.x = positionPt_3d.x + (alignPt_3dTmp.x-positionPt_3dTmp.x) ;
						positionPt_3d.y = positionPt_3d.y + (alignPt_3dTmp.y-positionPt_3dTmp.y) ;					

						pText->setAlignmentPoint(positionPt_3d) ;									
						pText->setPosition(positionPt_3dTmp2) ;
						if (iColorIndex!=-1)
						{
							pText->setColorIndex(iColorIndex) ;
						}
						strMsg.Format(_T("anew.x:%f-pnew.x:%f\n"), alignPt_3dTmp.x, positionPt_3dTmp.x) ;
						ads_printf(strMsg) ;	
					}
					pEntTmp->close() ;
					pEntTmp = NULL ;
				}
			}	
		}			
		break;
	default:
		break;
	}

	bRet = TRUE ;
	return bRet ;
}

BOOL CUserSystem::GetTextStyleArray(CStringArray &strArrayTextStyle) 
{
	BOOL bRet = FALSE ;
	AcDbTextStyleTable * pTextStyleTable = NULL ;
	AcDbTextStyleTableRecord * pTextStyleTableRecord = NULL ;
	AcDbTextStyleTableIterator * pTextStyleTableIterator = NULL ;

	if(Acad::eOk == m_pDb->getTextStyleTable(pTextStyleTable, AcDb::kForRead))
	{
		if (Acad::eOk == pTextStyleTable->newIterator(pTextStyleTableIterator)) 
		{
			pTextStyleTable->close() ;

			for(pTextStyleTableIterator->start(); !pTextStyleTableIterator->done(); pTextStyleTableIterator->step())
			{
				if (Acad::eOk == pTextStyleTableIterator->getRecord(pTextStyleTableRecord,AcDb::kForRead))
				{
					const TCHAR * pTextStyleName ;
					pTextStyleTableRecord->getName(pTextStyleName) ;

					pTextStyleTableRecord->close() ;

					strArrayTextStyle.Add(pTextStyleName) ;
				}
			}
		}
		delete pTextStyleTableIterator ;
		pTextStyleTableIterator = NULL ;
	}
	if (strArrayTextStyle.GetSize()>0)
	{
		bRet = TRUE ;
	}
	return  bRet ;
}


//替换实体中的文本
int CUserSystem::ReplaceEntText(const AcDbEntity *pEnt, CString strFindText, CString strReplaceText, BOOL bIsAllFit, BOOL bCaseSensitive, BOOL bReg, int iColorIndex, int iFindEntType)
{
	//ads_printf("\n into ReplaceEntText:findtext:%s - replace:%s - casesens:%s - color:%d", strFindText, strReplaceText,bCaseSensitive?"TRUE":"FALSE", iColorIndex) ;

	int iRet = 0;
	//AcDbText
	if (pEnt->isKindOf(AcDbText::desc()) && iFindEntType==0)
	{
		AcDbText * pText = AcDbText::cast(pEnt) ;
		CString strText ;
		strText = pText->textString() ;
		CString strRet = strText ;

		if (bIsAllFit)//全字匹配
		{			
			if (bReg==TRUE) //正则模式
			{
				if (CBlkUtility::IsStrMatch(strFindText, strText))
				{					
					CBlkUtility::RegReplace(strFindText, strReplaceText, strText, strRet) ;
					pText->setTextString(strRet) ;
					acutPrintf(_T("\n%d:[%s] to [%s]"), iRet, strText, strRet) ;
					if (iColorIndex!=-1) 
					{                           
						pText->setColorIndex(iColorIndex) ;
					} 
					iRet++ ;
				}
			}
			else
			{
				if (strText==strFindText)
				{
					strRet = strReplaceText ;
					pText->setTextString(strRet) ;
					acutPrintf(_T("\n%d:[%s] to [%s]"), iRet, strText, strRet) ;
					if (iColorIndex!=-1) 
					{                           
						pText->setColorIndex(iColorIndex) ;
					} 
					iRet++ ;
				} 
			}
		}
		else //部分匹配
		{
			if (bReg==TRUE)
			{
				if (CBlkUtility::IsStrMatch(strFindText, strText))
				{					
					CBlkUtility::RegReplace(strFindText, strReplaceText, strText, strRet) ;
					pText->setTextString(strRet) ;
					acutPrintf(_T("\n%d:[%s] to [%s]"), iRet, strText, strRet) ;
					if (iColorIndex!=-1) 
					{                           
						pText->setColorIndex(iColorIndex) ;
					}
					iRet++ ;
				}
			}
			else
			{
				if (strText.Find(strFindText)!=-1 ) 
				{
					//ads_printf("\n before ReplaceTxt:text:%s - findtext:%s - replace:%s - casesens:%s - color:%d", strText, strFindText, strReplaceText,bCaseSensitive?"TRUE":"FALSE", iColorIndex) ;
					CBlkUtility::ReplaceTxt(strRet, strFindText, strReplaceText, bCaseSensitive) ;
					pText->setTextString(strRet) ;
					acutPrintf(_T("\n%d:[%s] to [%s]"), iRet, strText, strRet) ;
					if (iColorIndex!=-1) 
					{                           
						pText->setColorIndex(iColorIndex) ;
					}
					iRet++ ;
				}
			}
		}
	}
	//AcDbMtext
	else if (pEnt->isKindOf(AcDbMText::desc()) && iFindEntType==1)
	{
		AcDbMText * pMText = AcDbMText::cast(pEnt) ;
		CString strText ;
		strText = pMText->contents() ;

		if (bIsAllFit)
		{
			if (strText==strFindText)
			{
				pMText->setContents(strReplaceText) ;
				acutPrintf(_T("\n%d:[%s] to [%s]"), iRet, strText, strReplaceText) ;
				if (iColorIndex!=-1) 
				{                           
					pMText->setColorIndex(iColorIndex) ;
				} 
				iRet++ ;
			}   
		}
		else 
		{
			if (strText.Find(strFindText)!=-1 ) 
			{
				CString strTextTmp = strText ;
				//ads_printf("\n before ReplaceTxt:text:%s - findtext:%s - replace:%s - casesens:%s - color:%d", strText, strFindText, strReplaceText,bCaseSensitive?"TRUE":"FALSE", iColorIndex) ;
				int iRetTmp = CBlkUtility::ReplaceTxt(strTextTmp, strFindText, strReplaceText, bCaseSensitive) ;
				//ads_printf("\n after ReplaceTxt:text:%s - findtext:%s - replace:%s - casesens:%s - color:%d", strText, strFindText, strReplaceText,bCaseSensitive?"TRUE":"FALSE", iColorIndex) ;
				pMText->setContents(strTextTmp) ;
				acutPrintf(_T("\n%d:[%s] to [%s]"), iRet, strText, strTextTmp) ;
				//ads_printf("\nlinebreak:%s", pMText->lineBreak()) ;
				if (iColorIndex!=-1) 
				{                           
					pMText->setColorIndex(iColorIndex) ;
				}
				iRet++ ;
			}
		}
	}
	//AcDbAttributeDefinition
	else if (pEnt->isKindOf(AcDbAttributeDefinition::desc()) && iFindEntType==2)
	{
	}
	//AcDbBlockReference 
	else if (pEnt->isKindOf(AcDbBlockReference::desc()) && iFindEntType==3)
	{
	}

	return iRet ;
}

//查找替换文本
int CUserSystem::FindAndReplaceText(CString strFindText, CString strReplaceText, BOOL bIsAllFit, BOOL bCaseSensitive, BOOL bReg,  int iRange, int iColorIndex, int iFindEntType)
{
	int iRet = 0 ;

	//全图替换
	if (iRange==1)
	{
		AcDbBlockTable * pBlkTbl = NULL ;
		this->m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) ;

		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRec, AcDb::kForRead) ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator * pBlkTblRecIter = NULL ;

		pBlkTblRec->newIterator(pBlkTblRecIter) ;
		pBlkTblRec->close() ;

		AcDbEntity *pEnt = NULL ;
		for(pBlkTblRecIter->start(); !pBlkTblRecIter->done(); pBlkTblRecIter->step())
		{
			if (Acad::eOk==pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite)) 
			{

				int iRetTmp = this->ReplaceEntText(pEnt, strFindText, strReplaceText, bIsAllFit, bCaseSensitive, bReg, iColorIndex, iFindEntType) ;
				iRet += iRetTmp ;

				pEnt->close() ;
			}
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;

	}
	//选择性替换
	else if(iRange==0)
	{
		ads_name sum_name  , temp_name  ;		

		struct resbuf *pRb ;
		switch (iFindEntType)
		{
		case 0:
			pRb = acutBuildList(RTDXF0, _T("TEXT"), 0) ;
			break;
		case 1:
			pRb = acutBuildList(RTDXF0, _T("MTEXT"), 0) ;
			break;
		case 2:
			pRb = acutBuildList(RTDXF0, _T("ATTDEF"), 0) ;
			break;
		case 3:
			pRb = acutBuildList(RTDXF0, _T("INSERT"), 0) ;
			break;
		default:
			pRb = acutBuildList(RTDXF0, _T("TEXT"), 0) ;
			break;
		}

		ads_printf ( _T("\n选择实体: ") ) ;
		if( RTNORM == ads_ssget ( NULL , NULL , NULL , pRb , sum_name ) )
		{
			long lLength = 0 ;
			ads_sslength(sum_name, &lLength) ;
			for(int i = 0; i<lLength; i++)
			{
				ads_ssname(sum_name, i, temp_name) ;
				AcDbEntity * pEnt = NULL ;
				if (OpenAcDbEntity(temp_name, pEnt))
				{
					int iRetTmp = this->ReplaceEntText(pEnt, strFindText, strReplaceText, bIsAllFit, bCaseSensitive, bReg, iColorIndex, iFindEntType) ;
					iRet += iRetTmp ;

					pEnt->close() ;
				}
			}
			ads_ssfree(sum_name) ;
		}
		acutRelRb(pRb) ;

	}

	return iRet ;
}

AcDbObjectId CUserSystem::CreateTextStyleByFileName(CString strStyleName, CString strFileName, CString strBigFontFileName, double dSize, double dxScale)
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
		//pTextStyle->setFont(strFontName, 0, 0, 134, 1) ;
		pTextStyle->setFileName(strFileName) ;
		if (strBigFontFileName!=_T(""))
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

int CUserSystem::SelectAcDbEnt(const TCHAR *chPrompt, const TCHAR *chEntType, ads_name &ent_name)
{
	int iRet = 0 ;
	ads_point pt = {0, 0, 0} ;
	while (iRet == 0) 
	{
		int iReturn = ads_entsel(chPrompt, ent_name, pt) ;
		if(iReturn == RTNORM)
		{			
			AcDbEntity * pEnt = NULL ;
			if(this->OpenAcDbEntity(ent_name, pEnt))
			{
				if( _tcscmp(pEnt->isA()->name(),chEntType) == 0)
				{					
					iRet = 1 ;					
				}
				else
				{
					CString strMsg ;
					strMsg.Format(_T("选择的实体不是%s"), chEntType) ;
					AfxMessageBox(strMsg) ;
					iRet = 0 ;
				}
			}
			pEnt->close() ;	
		}
		else if (iReturn == RTERROR) 
		{
			iRet = 0 ;
		}	
		else if (iReturn == RTCAN)
		{
			iRet = -1 ;
		}
	}

	return iRet ;

}

//删除文本所有的xdata
void CUserSystem::DelAllXdata()
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
			struct resbuf *pRb, *pTmp;
			pRb = pEnt->xData(NULL);
			if (pRb != NULL) 
			{
// 				pRb->rbnext = NULL ;
// 				pEnt->setXData(pRb) ;
// 				acutRelRb(pRb) ;
				ads_printf(_T("\n")) ;
				for(pTmp=pRb; pTmp!=NULL; pTmp = pTmp->rbnext)
				{
					switch(pTmp->restype) 
					{
					case 1001:
						{
							CString strAppName ;
							strAppName.Format(_T("%s"), pTmp->resval.rstring) ;
							this->DeleteXdata(pEnt, strAppName) ;	
							ads_printf(_T("删除属性:%s\n"),strAppName) ;								
						}							
						break;
					default:
						//acutPrintf("unknown type\n") ;
						break;
					}
				}	
				//end for
				acutRelRb(pRb);
				pRb = NULL ;

				pEnt->setColorIndex(7) ;

			} //end if
			else
			{
				//ads_printf(_T("\n没有xdata"));
			}			

			pEnt->close() ;
			pEnt = NULL ;
		}

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

}



AcDbObjectId CUserSystem::SetFontStyle()
{
	CString strFontName =  _T("文字标注") ;
	CString m_strFontName  = _T("宋体") ;
	double m_dFont_Size =  2.5 ;

	AcDbObjectId fontStyle_Id ;
	AcDbTextStyleTable * pTextStyleTable;
	m_pDb->getTextStyleTable(pTextStyleTable,AcDb::kForWrite);
	if(Acad::eOk == pTextStyleTable->getAt(strFontName,fontStyle_Id,AcDb::kForWrite))
	{
		pTextStyleTable->close();
		m_pDb->setTextstyle(fontStyle_Id);
	}
	else
	{
		AcDbTextStyleTableRecord * pTextStyle=new AcDbTextStyleTableRecord;
		pTextStyle->setName(strFontName);
		pTextStyle->setFont(m_strFontName , 0 , 0 ,  134 , 1 ); //注意　；
		pTextStyle->setTextSize( m_dFont_Size ) ;
		pTextStyleTable->add(fontStyle_Id,pTextStyle);
		pTextStyle->close();
		pTextStyleTable->close();
		m_pDb->setTextstyle(fontStyle_Id);
	}
	return fontStyle_Id ;

}

AcDbObjectId CUserSystem::SetFontStyle(CString strStyleName, CString strFont_Name)
{
	AcDbObjectId fontStyle_Id ;
	AcDbTextStyleTable * pTextStyleTable;
	m_pDb->getTextStyleTable(pTextStyleTable,AcDb::kForWrite);
	if(Acad::eOk == pTextStyleTable->getAt(strStyleName,fontStyle_Id,AcDb::kForWrite))
	{
		pTextStyleTable->close();
		m_pDb->setTextstyle(fontStyle_Id);
	}
	else
	{
		AcDbTextStyleTableRecord * pTextStyle=new AcDbTextStyleTableRecord;
		pTextStyle->setName(strStyleName);
		pTextStyle->setFont(strFont_Name  , 0 , 0 ,  134 , 1 ); //注意　；
		pTextStyle->setTextSize( 2.5 ) ;
		pTextStyleTable->add(fontStyle_Id,pTextStyle);
		pTextStyle->close();
		pTextStyleTable->close();
		m_pDb->setTextstyle(fontStyle_Id);
	}
	return fontStyle_Id ;
}

Acad::ErrorStatus CUserSystem::ModifyFontStyle(CString strStyleName, CString strFontName, BOOL bUseBigFont)
{
	Acad::ErrorStatus erro ;

	AcDbTextStyleTable * pTextStyleTb = NULL ;
	m_pDb->getTextStyleTable(pTextStyleTb, AcDb::kForRead) ;
	AcDbTextStyleTableRecord *pTxtStyRec = NULL ;
	pTextStyleTb->getAt(strStyleName, pTxtStyRec, AcDb::kForWrite) ;	
	pTextStyleTb->close() ; 
	if (bUseBigFont)
	{
		erro = pTxtStyRec->setBigFontFileName(strFontName) ;
	}
	else 
	{
		erro = pTxtStyRec->setFont(strFontName, 0, 0, 134, 1) ;
	}
	pTxtStyRec->close() ;

	return erro ;
}


/*
void CUserSystem::AddAttValue(AcDbBlockReference *pRef, const TCHAR *chTag, const TCHAR *chVal)
{
	AcDbAttribute * pAtt=new AcDbAttribute();
	AcGePoint3d base_3d(0,0,0) ;
	base_3d = pRef->position() ;
	pAtt->setVerticalMode(AcDb::kTextVertMid);
	pAtt->setRotation( 0 );

	pAtt->setTextString( chVal );

	pAtt->setHorizontalMode(AcDb::kTextCenter);

	pAtt->setAlignmentPoint( base_3d );
	pAtt->setPosition( base_3d );
	pAtt->setInvisible( TRUE );
	pAtt->setTag( chTag);
	AcDbObjectId fontStyle_Id;
	fontStyle_Id = SetFontStyle(  ); //字体设置 ；
	pAtt->setTextStyle( fontStyle_Id );
	AcCmColor color;
	color.setColorIndex( 7 ); //设置颜色 ；
	pAtt->setColor ( color ) ;
	AcDbObjectId attId;
	pRef->appendAttribute(attId,pAtt);
	pAtt->close();

}
*/

BOOL CUserSystem::GetAttValue(AcDbBlockReference * pRef , const TCHAR * chTag , TCHAR * chVal )
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef , chTag , pAtt ) )
	{
		chVal = pAtt->textString () ;
		bReturn = TRUE ;
		pAtt->close () ;
	}
	return bReturn ;
}

BOOL CUserSystem::GetAttValue(AcDbBlockReference * pRef , const TCHAR * chTag , CString & strVal )
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef , chTag , pAtt ) )
	{
		TCHAR * chVal;
		chVal = pAtt->textString () ;
		strVal = chVal ;
		bReturn = TRUE ;
		pAtt->close () ;
	}
	return bReturn ;
}
BOOL CUserSystem::GetAttValue(AcDbBlockReference * pRef , const TCHAR * chTag , int & iVal )
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef , chTag , pAtt ) )
	{
		TCHAR * chVal;
		chVal = pAtt->textString () ;
		iVal = _tstoi( chVal ) ;
		bReturn = TRUE ;
		pAtt->close () ;
	}
	return bReturn ;
}
BOOL CUserSystem::GetAttValue(AcDbBlockReference * pRef , const TCHAR * chTag , double & dVal )
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef , chTag , pAtt ) )
	{
		TCHAR * chVal;
		chVal = pAtt->textString () ;
		dVal = _tstof( chVal ) ;
		bReturn = TRUE ;
		pAtt->close () ;
	}
	return bReturn ;
}

BOOL CUserSystem::GetAttValue(long nHandle, const TCHAR * chTag , TCHAR * chVal )
{
	BOOL bReturn = FALSE ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			bReturn = GetAttValue(pRef, chTag, chVal) ;
		}
		pEnt->close() ;
	}
	return bReturn ;
}

BOOL CUserSystem::GetAttValue(long nHandle, const TCHAR * chTag , CString & strVal )
{
	BOOL bReturn = FALSE ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			bReturn = GetAttValue(pRef, chTag, strVal) ;
		}
		pEnt->close() ;
	}
	return bReturn ;
}
BOOL CUserSystem::GetAttValue(long nHandle, const TCHAR * chTag , int & iVal )
{
	BOOL bReturn = FALSE ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			bReturn = GetAttValue(pRef, chTag, iVal) ;
		}
		pEnt->close() ;
	}
	return bReturn ;
}
BOOL CUserSystem::GetAttValue(long nHandle, const TCHAR * chTag , double & dVal )
{
	BOOL bReturn = FALSE ;
	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			bReturn = GetAttValue(pRef, chTag, dVal) ;
		}
		pEnt->close() ;
	}
	return bReturn ;
}

BOOL CUserSystem::SetAttValue(AcDbBlockReference * pRef , CString strTag, CString strValue, BOOL bInVisible)
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef , strTag , pAtt ) )
	{
		pAtt->upgradeOpen() ;
		pAtt->setTextString ( strValue ) ;
		pAtt->close() ;
		bReturn = TRUE ;
	}
	else
	{
		this->AddAttValue(pRef, strTag, strValue, pRef->position(), 0, bInVisible) ;
	}
	return bReturn ;
}
BOOL CUserSystem::SetAttValue(AcDbBlockReference * pRef, CString strTag, double dValue, BOOL bInVisible)
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	CString strValue ;
	strValue.Format( _T("%.3f") , dValue ) ;
	if( this->FindAtt ( pRef , strTag , pAtt ) )
	{
		pAtt->upgradeOpen() ;
		pAtt->setTextString ( strValue ) ;
		pAtt->close() ;
		bReturn = TRUE ;
	}
	else
	{
		this->AddAttValue(pRef, strTag, strValue, pRef->position(), 0, bInVisible) ;
	}
	return bReturn ;
}

BOOL CUserSystem::SetAttColor(AcDbBlockReference * pRef, CString strTag, Adesk::UInt16 nColorIndex)
{
	BOOL bReturn = FALSE ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef, strTag, pAtt ) )
	{
		pAtt->upgradeOpen() ;
		pAtt->setColorIndex(nColorIndex) ;
		pAtt->close() ;
		bReturn = TRUE ;
	}

	return bReturn ;
}
/*
void CUserSystem::EditValue(AcDbBlockReference * pRef, CString strTag, CString strValue)
{
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt (pRef , strTag, pAtt))
	{
		pAtt->setTextString (strValue) ;
		pAtt->close() ;
	}
	else
	{
		AcGePoint3d base_3d( 0,0,0 ) ;
		base_3d = pRef->position() ;
		pAtt = new AcDbAttribute;
		pAtt->setTag( strTag );
		pAtt->setTextString( strValue );
		pAtt->setPosition( base_3d );
		pAtt->setInvisible(TRUE);
		pRef->appendAttribute(pAtt);
		pAtt->close() ; //20091102: add,新建对象一定要关闭
	}
}
void CUserSystem::EditValue(AcDbBlockReference * pRef, CString strTag, double dValue)
{
	CString strValue ;
	strValue.Format (_T("%.3f") , dValue ) ;
	AcDbAttribute * pAtt = NULL ;
	if( this->FindAtt ( pRef , strTag , pAtt ) )
	{
		pAtt->setTextString ( strValue ) ;
		pAtt->close() ;
	}
	else
	{
		AcGePoint3d base_3d( 0,0,0 ) ;
		base_3d = pRef->position() ;
		pAtt = new AcDbAttribute;
		pAtt->setTag( strTag );
		pAtt->setTextString( strValue );
		pAtt->setPosition( base_3d );
		pAtt->setInvisible(TRUE);
		pRef->appendAttribute(pAtt);
		pAtt->close() ;
	}
}
*/
//增加属性,iAlignMode:0~14,对应CAD中的对齐下拉框
void CUserSystem::AddAttValue(AcDbBlockReference * pRef,const TCHAR *chTag,	const TCHAR *chVal,
							  AcGePoint3d pt_3d_att,
							  int iAlign,
							  BOOL bInVisible,		
							  double dHeight ,
							  double dWidthFactor,
							  double dAngle,
							  Adesk::UInt16 nColorIndex,
							  AcDbObjectId objIdTextStyle)
{
	AcDbAttribute * pAtt = new AcDbAttribute();

	pAtt->setTag( chTag);
	pAtt->setTextString(chVal);

	AcDb::TextHorzMode textHMode ;
	AcDb::TextVertMode textVMode ;
	this->ConvertIndexToTextMode(iAlign, textHMode, textVMode) ;
	pAtt->setHorizontalMode(textHMode);
	pAtt->setVerticalMode(textVMode);

	if (pt_3d_att.x==0&&pt_3d_att.y==0&&pt_3d_att.z==0&&bInVisible==TRUE)
	{
		pt_3d_att = pRef->position() ;
	}

	if (iAlign==0||iAlign==3||iAlign==5)
	{
		pAtt->setPosition(pt_3d_att) ;
	}
	else
	{
		pAtt->setAlignmentPoint(pt_3d_att);
	}

	pAtt->setInvisible(bInVisible);

	pAtt->setHeight (dHeight) ;
	pAtt->setWidthFactor(dWidthFactor) ;

	if(dAngle>0.5001*PI && dAngle<1.5001*PI)
	{
		pAtt->setRotation(dAngle+PI);
	}
	else
	{
		pAtt->setRotation(dAngle);
	}

	pAtt->setColorIndex(nColorIndex) ;

	if (objIdTextStyle!=NULL)
	{
		pAtt->setTextStyle(objIdTextStyle) ;
	}

	AcDbObjectId objId;
	pRef->appendAttribute(objId, pAtt);
	pAtt->close();

}

//此函数使用过程中请注意：务必在调用结束后关闭pAtt，即pAtt->close()
BOOL CUserSystem::FindAtt(AcDbBlockReference * pRef ,const TCHAR * chTag , AcDbAttribute * & pAtt )
{
	BOOL bHaveAtt = FALSE ;
	AcDbObjectIterator * pObjIter;
	pObjIter = pRef->attributeIterator();
	AcDbObjectId attId;
	AcDbAttribute * pAtt_Temp;
	for(pObjIter->start();!pObjIter->done();pObjIter->step())
	{
		attId = pObjIter->objectId();
		pRef->openAttribute(pAtt_Temp,attId, AcDb::kForRead,Adesk::kFalse);
		const TCHAR * pTagTmp = pAtt_Temp->tag() ;
		if ( _tcscmp( pTagTmp, chTag ) == 0)
		{
			pAtt = pAtt_Temp ;
			bHaveAtt = TRUE ;
			break;
		}
		else
		{
			pAtt_Temp->close();
		}
	}
	delete pObjIter;
	return bHaveAtt ;
}

AcDbObjectId CUserSystem::AddAcDbCircle(AcGePoint3d pt_3d_center, double dRadius, int iColorIndex)
{
	AcGeVector3d normal(0.0, 0.0, 1.0) ;
	AcDbCircle *pCircle = new AcDbCircle(pt_3d_center, normal, dRadius);

	AcCmColor color ;
	if (!((iColorIndex>=0 && iColorIndex<=7) || iColorIndex==256))
	{
		iColorIndex = 0 ;
	}
	color.setColorIndex(iColorIndex) ;
	pCircle->setColor(color) ;


	AcDbBlockTable *pBlockTable;
	this->m_pDb->getBlockTable(pBlockTable,
		AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);
	pBlockTable->close();

	AcDbObjectId circleId;
	pBlockTableRecord->appendAcDbEntity(circleId, pCircle);

	pBlockTableRecord->close();
	pCircle->close();

	return circleId ;

}

BOOL CUserSystem::SetLayerStatus(CString strLayerName, bool bIsOff, bool bIsFrozen, bool bIsLocked, int iColorIndex)
{
	BOOL bRet = FALSE ;
	AcDbLayerTable * pLayTbl;
	AcDbLayerTableRecord *pLayerTblRec ;
	m_pDb->getLayerTable(pLayTbl,AcDb::kForRead);
	Acad::ErrorStatus es = pLayTbl->getAt(strLayerName,pLayerTblRec,AcDb::kForWrite) ;
	if (Acad::eOk==es)
	{		
		pLayerTblRec->setIsOff(bIsOff) ;
		pLayerTblRec->setIsFrozen(bIsFrozen) ;
		pLayerTblRec->setIsLocked(bIsLocked) ;
		AcCmColor color ; 
		color.setColorIndex(iColorIndex) ;
		pLayerTblRec->setColor(color) ;
		
		pLayerTblRec->close() ;

		bRet = TRUE ;
	}
	pLayTbl->close();

	return bRet ;
}

BOOL CUserSystem::DeleteLayer(CString strLayerName)
{
	BOOL bRet = FALSE ;

	AcDbLayerTable * pLayTbl;
	AcDbLayerTableRecord *pLayerTblRec ;
	m_pDb->getLayerTable(pLayTbl,AcDb::kForRead);
	Acad::ErrorStatus es = pLayTbl->getAt(strLayerName,pLayerTblRec,AcDb::kForWrite) ;
	if (Acad::eOk==es)
	{		
		pLayerTblRec->erase() ;
		pLayerTblRec->close() ;

		bRet = TRUE ;
	}
	pLayTbl->close();

	return bRet ;

}
int CUserSystem::FindBlkInArea(const AcGePoint3d &pt_topleft, const AcGePoint3d &pt_bottomright, CLongArray &nArrayBlkHandle) 
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbBlockReference * pRef = NULL ;

			//处理blockref ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				pRef = AcDbBlockReference::cast(pEnt) ;
				AcGePoint3d pt_3d_ref(0,0,0) ;
				pt_3d_ref = pRef->position() ;
				//只处理规定区域的；
				if((pt_3d_ref.x>pt_topleft.x && pt_3d_ref.x<pt_bottomright.x) && (pt_3d_ref.y>pt_bottomright.y && pt_3d_ref.y<pt_topleft.y))
				{
					AcDbHandle handletmp ;
					Adesk::UInt32 iHandleLow = 0 ;
					pRef->getAcDbHandle(handletmp) ;
					iHandleLow = handletmp.low() ;
					nArrayBlkHandle.Add(iHandleLow) ;
					iRet++ ;	
				}				
			}
			pEnt->close() ;
		}

	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}

//查找名为strBlkName的块
int CUserSystem::FindBlk(const CString &strBlkName, CLongArray &nArrayBlkHandle) 
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
			AcDbBlockReference * pRef = NULL ;

			//处理blockref ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				pRef = AcDbBlockReference::cast(pEnt) ;
				CString strCurBlkName ;
				this->GetBlockName(pRef, strCurBlkName) ;
				if (strCurBlkName.Compare(strBlkName)==0)
				{					
					nArrayBlkHandle.Add(this->GetHandle(pRef)) ;
					iRet++ ;
				}				
			}
			pEnt->close() ;
		}
	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}
/*
int CUserSystem::FindBlk(const CString &strBlkName, CArray<AcDbBlockReference *, AcDbBlockReference *> &arrayBlk)
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
			AcDbBlockReference * pRef = NULL ;

			//处理blockref ;
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				pRef = AcDbBlockReference::cast(pEnt) ;
				CString strCurBlkName ;
				this->GetBlockName(pRef, strCurBlkName) ;
				if (strCurBlkName.Compare(strBlkName)==0)
				{
					AcDbBlockReference *pNewRef = new AcDbBlockReference()

					iRet++ ;
				}				
			}
			pEnt->close() ;
		}
	}

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return iRet ;
}
*/

int CUserSystem::FindTxtInArea(const AcGePoint3d &pt_topleft, const AcGePoint3d &pt_bottomright, CStringArray &strArrayFound)
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbText * pText = NULL ;

			//处理文本 AcDbText ;
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				pText = (AcDbText *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pText->position() ;
				//只处理规定区域的文本；
				if((textp_3d.x>pt_topleft.x && textp_3d.x<pt_bottomright.x) && (textp_3d.y>pt_bottomright.y && textp_3d.y<pt_topleft.y))
				{
					CString strText ;					
					strText = pText->textString() ;
					strText.TrimLeft();
					strText.TrimRight() ;
					if (!strText.IsEmpty())
					{
						strArrayFound.Add(strText) ;
						iRet++ ;
						//bReturn = true ;
						/*
						CString strTt ;
						strTt.Format(_T("(%f,%f)--%s"),textp_3d.x,textp_3d.y, strText) ;
						AfxMessageBox(strTt) ;
						*/
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
int CUserSystem::FindTxtInArea(const AcGePoint3d &pt_topleft, const AcGePoint3d &pt_bottomright, CArray<AcDbText *, AcDbText *> &arrayText)
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
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			AcDbText * pText = NULL ;

			//处理文本 AcDbText ;
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				pText = (AcDbText *)pEnt ;
				AcGePoint3d textp_3d(0,0,0) ;
				textp_3d = pText->position() ;
				//只处理规定区域的文本；
				if((textp_3d.x>pt_topleft.x && textp_3d.x<pt_bottomright.x) && (textp_3d.y>pt_bottomright.y && textp_3d.y<pt_topleft.y))
				{
					CString strText ;					
					strText = pText->textString() ;
					strText.TrimLeft();
					strText.TrimRight() ;
					if (!strText.IsEmpty())
					{
						AcGePoint3d textp_3d(0,0,0) ;
						AcDbText * pTextTmp = new AcDbText ;
						pTextTmp->setTextString(strText) ;
						textp_3d.x = pText->position().x  ;
						textp_3d.y = pText->position().y  ;
						pTextTmp->setPosition(textp_3d) ;
						arrayText.Add(pTextTmp) ;

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

int CUserSystem::GetHaveSortedText(const CArray<AcDbText *, AcDbText *> &arrayent, CStringArray & strArrayText)
{
	int iRet = arrayent.GetSize() ;
	for (int i=0; i<iRet; i++)
	{
		AcDbText * pText = arrayent.GetAt(i) ;
		strArrayText.Add(pText->textString()) ;
	}
	return iRet ;
} 

void CUserSystem::DestroyEntInArray(CArray<AcDbText *, AcDbText *> &arrayObj)
{
	while (arrayObj.GetSize()>0)
	{
		delete arrayObj[0] ;
		arrayObj.RemoveAt(0) ;
	}

}

//返回点到直线的距离
//pt1, 已知直线上的一点坐标
//pt2, 已知直线上的另一点坐标
//pt, 用户输入的直线外一点
double CUserSystem::GetDistance(const AcGePoint2d pt1, const AcGePoint2d pt2, const AcGePoint2d pt)
{
	AcGeTol tol;
	double  len;
	AcGeLine2d  lin(pt1, pt2);
	len = lin.distanceTo(pt, tol);
	return len;
}

double CUserSystem::GetDistance(const AcGePoint3d pt1, const AcGePoint3d pt2, const AcGePoint3d pt)
{
	AcGeTol tol;
	double  len;
	AcGeLine3d  lin(pt1, pt2);
	len = lin.distanceTo(pt, tol);
	return len;
}
double CUserSystem::GetDistance(const AcGeLine2d lin_2d, const AcGePoint2d pt)
{
	AcGeTol tol;
	double  len;
	len = lin_2d.distanceTo(pt, tol);
	return len;
}
double CUserSystem::GetDistance(const AcGeLine3d lin_3d, const AcGePoint3d pt)
{
	AcGeTol tol;
	double  len;
	len = lin_3d.distanceTo(pt, tol);
	return len;
}
double CUserSystem::GetDistance(const AcGeLineSeg2d linSeg_2d, const AcGePoint2d pt)
{
	AcGeTol tol;
	double  len;
	len = linSeg_2d.distanceTo(pt, tol);
	return len;
}
double CUserSystem::GetDistance(const AcGeLineSeg3d linSeg_3d, const AcGePoint3d pt)
{
	AcGeTol tol;
	double  len;
	len = linSeg_3d.distanceTo(pt, tol);
	return len;
}

//得到块名
BOOL CUserSystem::GetBlockName(const AcDbBlockReference * pRef, CString &strBlockName) 
{
	BOOL bRet = FALSE ;

	AcDbBlockTableRecord * pBlkTblRecTmp = NULL ;
	AcDbObjectId blkTblObjId ;
	blkTblObjId = pRef->blockTableRecord() ;
	if(Acad::eOk == acdbOpenObject(pBlkTblRecTmp, blkTblObjId, AcDb::kForRead))
	{
		TCHAR * chBlockName = NULL ;
		if (Acad::eOk==pBlkTblRecTmp->getName(chBlockName))
		{
			strBlockName.Format(_T("%s"), chBlockName) ;		
			bRet = TRUE ;
		}	
	}
	pBlkTblRecTmp->close() ;

	return bRet ;
}

//得到块名
BOOL CUserSystem::GetBlockName(long nHandle, CString &strBlockName) 
{
	BOOL bRet = FALSE ;

	AcDbEntity* pEnt = NULL ;
	if (this->OpenAcDbEntity(nHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbBlockReference::desc()))
		{
			AcDbBlockReference* pRef = AcDbBlockReference::cast(pEnt) ;
			bRet= GetBlockName(pRef, strBlockName) ;
		}
		pEnt->close() ;
	}

	return bRet ;
}

int CUserSystem::Explode(AcDbEntity * pEnt)
{
	//炸开后的实体集合 ；
	AcDbVoidPtrArray entSet ;	
	pEnt->explode(entSet) ;				

	// 
	int iEntLength = entSet.length() ;

	for(int i=0; i<iEntLength; i++)
	{
		AcDbEntity *pExpEnt = (AcDbEntity*)entSet[i] ;
		this->AddEntityToDb(pExpEnt) ;
		pExpEnt->close() ;
	}
	pEnt->erase(Adesk::kTrue) ;

	return iEntLength ;
}

//替换实体块
BOOL CUserSystem::ReplaceBlock(AcDbBlockReference *pBlkRef, const CMapStringToString *pMapAtt, const CString *pStrNewBlkName, BOOL bWithAtt) 
{
	BOOL bRet = FALSE ;

	AcDbBlockReference *pRefTmp = NULL ; 
	int nInsertMode = 0 ;
	if (pStrNewBlkName->Find(_T("\\"))!=-1)
	{
		nInsertMode = 1 ;
	}
	if (this->InsertBlock(pRefTmp, *pStrNewBlkName, pBlkRef->position(), FALSE, nInsertMode, bWithAtt))
	{
		if (bWithAtt)
		{
			CMapStringToString mapOldAttToStr ;

			POSITION ipos = pMapAtt->GetStartPosition() ;
			while (ipos!=NULL)
			{
				CString strOldAtt, strNewAtt, strOldAttStr ;
				pMapAtt->GetNextAssoc(ipos, strOldAtt, strNewAtt) ;
				this->GetAttValue(pBlkRef, strOldAtt, strOldAttStr) ;
				mapOldAttToStr.SetAt(strOldAtt, strOldAttStr) ;
			}

			POSITION ipos2 = mapOldAttToStr.GetStartPosition() ;
			while (ipos2!=NULL)
			{
				CString strOldAtt,  strOldAttStr, strNewAtt ;
				mapOldAttToStr.GetNextAssoc(ipos2, strOldAtt, strOldAttStr) ;

				pMapAtt->Lookup(strOldAtt, strNewAtt) ;                                     
				this->SetAttValue(pRefTmp, strNewAtt, strOldAttStr) ;                                       
			}
		}

		pRefTmp->close() ;
		pRefTmp = NULL ;
		pBlkRef->erase(TRUE) ;

		bRet = TRUE ;
	}

	return bRet ;
}

//_WP方式选择区域ent
int CUserSystem::SelectEntByWP(const AcGePoint3dArray& ptArray, const resbuf* rbFilter, ads_name &ss_name)
{
	resbuf* ptList = ptArrayToResbuf(ptArray);
	if (ptList == NULL) 
	{		
		return Acad::eNone;
	}

	int result = acedSSGet(_T("_WP"), ptList, NULL, rbFilter, ss_name);
	acutRelRb(ptList);
	return result;
}

resbuf* CUserSystem::ptArrayToResbuf(const AcGePoint3dArray& ptArray)
{
	resbuf* ptList = NULL;        // overall list
	resbuf* lastRb = NULL;        // place holder to end of list
	resbuf* rb;
	int len = ptArray.length();
	for (int i=0;i<len;i++) 
	{
		if ((rb = acutNewRb(RT3DPOINT)) == NULL) 
		{
			acutRelRb(ptList);
			return NULL;
		}
		CopyAdsPt(rb->resval.rpoint, asDblArray(ptArray.at(i)));
		if (ptList == NULL) 
		{
			ptList = rb;
			lastRb = rb;
		}
		else 
		{
			lastRb->rbnext = rb;
			lastRb = rb;
		}
	}
	return ptList;
}

void CUserSystem::CopyAdsPt(ads_point ptDis, const ads_point ptSrc)
{
	ptDis[0] = ptSrc[0];
	ptDis[1] = ptSrc[1];
	ptDis[2] = ptSrc[2];

}

void CUserSystem::ConvertPt3DToAds(const AcGePoint3d pt_3d_src, ads_point &pt_dist)
{
	pt_dist[0] = pt_3d_src.x ;
	pt_dist[1] = pt_3d_src.y ;
	pt_dist[2] = pt_3d_src.z ;

}

// 取得2点之间向量的夹角
double CUserSystem::GetAngle(const AcGePoint3d pt_3d_start, const AcGePoint3d pt_3d_end)
{
	AcGeVector2d vet(pt_3d_end.x-pt_3d_start.x,pt_3d_end.y-pt_3d_start.y) ;
	double dAngle = vet.angle() ;
	//acutPrintf(_T("\nangle:%.3f"), dAngle*180/PI) ;
	return dAngle;
}

// 取得到pt_3d_src的距离为dDisFromSrcPt，角度（pt_src->pt_to的向量）为dAngle的点
void CUserSystem::GetPoint(const AcGePoint3d pt_3d_src, double dDisFromSrcPt, double dAngle, AcGePoint3d & pt_3d_to)
{
	pt_3d_to.x = pt_3d_src.x+dDisFromSrcPt*cos(dAngle) ;
	pt_3d_to.y = pt_3d_src.y+dDisFromSrcPt*sin(dAngle) ;
}


// 取得直线上/下一定距离范围内的字符串
//dExtend:直线延长或缩短数
int CUserSystem::GetStringOnLine(const AcDbLine * pLine, double dDistFromLine, CStringArray & strArrayFindText, const TCHAR * chSSGetMode, double dExtend, int iUpOrDown)
{
	int iRet = 0 ;

	if (pLine==NULL)
	{
		return -1 ;
	}
	AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0),pt_3d_start1(0,0,0), pt_3d_end1(0,0,0) ;
	pt_3d_start = pLine->startPoint() ;
	pt_3d_end = pLine->endPoint() ;
	double dAngle = this->GetAngle(pt_3d_start, pt_3d_end) ;
	AcGePoint3dArray ptArray ;
	if ((dAngle>=0&&dAngle<=(90*PI/180)) || (dAngle>(270*PI/180)&&dAngle<=(360*PI/180)) ) //1、4象限
	{		
		if (1==iUpOrDown) //搜索线的上方
		{
			dDistFromLine = fabs(dDistFromLine) ;
		}
		else if (-1==iUpOrDown)
		{
			dDistFromLine = -fabs(dDistFromLine) ;
		}
	}
	else
	{
		if (1==iUpOrDown) //搜索线的上方
		{
			dDistFromLine = -fabs(dDistFromLine) ;
		}
		else if (-1==iUpOrDown)
		{
			dDistFromLine = fabs(dDistFromLine) ;
		}
	}

	//将直线pLine延长或者缩短，数值是fabs(dEntend)
	pt_3d_start.x = pt_3d_start.x-dExtend*cos(dAngle) ;
	pt_3d_end.x = pt_3d_end.x+dExtend*cos(dAngle) ;
	pt_3d_start.y = pt_3d_start.y-dExtend*sin(dAngle) ;
	pt_3d_end.y = pt_3d_end.y+dExtend*sin(dAngle) ;

	this->GetPoint(pt_3d_start, dDistFromLine, dAngle+PI/2, pt_3d_start1) ;
	this->GetPoint(pt_3d_end, dDistFromLine, dAngle+PI/2, pt_3d_end1) ;
	ptArray.append(pt_3d_start) ;
	ptArray.append(pt_3d_start1) ;
	ptArray.append(pt_3d_end1) ;
	ptArray.append(pt_3d_end) ;	

// 	this->AddAcDb2dLine(pt_3d_start, pt_3d_end,1) ;
// 	this->AddAcDb2dLine(pt_3d_start1, pt_3d_end1,1) ;	

	struct resbuf *pRb, *ptList ;
	pRb = acutBuildList(RTDXF0, _T("TEXT"), 0) ;
	ptList = this->ptArrayToResbuf(ptArray) ;
	CLongArray iArrayHandle ;
	long iLen = 0 ;
	iLen = this->GetSSEntHandle(chSSGetMode, ptList, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;
	acutRelRb(ptList) ;

	for (int i=0; i<iLen; i++)
	{
		AcDbEntity *pEnt = NULL ;
		if (OpenAcDbEntity(iArrayHandle.GetAt(i), pEnt, AcDb::kForRead))
		{
			AcDbText *pText = AcDbText::cast(pEnt) ;
			CString strText = pText->textString() ;
			BOOL bExist = FALSE ;
			int iLen2 = strArrayFindText.GetCount() ;
			for (int i2=0; i2<iLen2; i2++)
			{
				if (strText==strArrayFindText.GetAt(i2))
				{
					bExist = TRUE ;
					break;
				}
			}
			if (!bExist)
			{
				strArrayFindText.Add(strText) ;
				iRet++ ;
			}				

			pEnt->close() ;
		}
	}

	return iRet;
}

// 取得实体的handle（只针对32位系统，故只取handle_low）
unsigned long CUserSystem::GetHandle(const AcDbEntity * pEnt)
{
	AcDbHandle hEnt;
	Adesk::UInt32 iHandle = 0 ;
	pEnt->getAcDbHandle(hEnt) ;
	iHandle = hEnt.low() ;

	return iHandle;
}

unsigned long CUserSystem::GetHandle(const AcDbObjectId &objId)
{
	Adesk::UInt32 iHandle = 0 ;

	AcDbEntity* pEnt = NULL ;
	if(Acad::eOk == acdbOpenAcDbEntity(pEnt,objId, AcDb::kForRead))
	{
		iHandle = GetHandle(pEnt) ;
		pEnt->close() ;
	}

	return iHandle;
}

// 删除空字符文字或者trim后为空的文字
int CUserSystem::DeleteNullText(void)
{
	int iRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	this->m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead) ;

	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRec, AcDb::kForRead) ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator * pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator(pBlkTblRecIter) ;
	pBlkTblRec->close() ;

	AcDbEntity *pEnt = NULL ;
	for(pBlkTblRecIter->start(); !pBlkTblRecIter->done(); pBlkTblRecIter->step())
	{
		if (Acad::eOk==pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite)) 
		{
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				AcDbText *pText = AcDbText::cast(pEnt) ;
				CString strText = pText->textString() ;
				strText.Trim() ;
				if (strText.GetLength()==0)
				{
					pText->erase(true) ;
					iRet++ ;
				}
			}
			
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;


	return iRet;
}

// 取得选择集实体的handle
int CUserSystem::GetSSEntHandle(const TCHAR * strSSMode, const void * pt1, const void * pt2, const struct resbuf * filter, CLongArray & iArrayHandle)
{
	long iLen = 0 ;
	ads_name sum_name ;
	int iRet = acedSSGet(strSSMode, pt1, pt2, filter, sum_name) ;
	if (Acad::eNormal==iRet)
	{	
		acedSSLength(sum_name, &iLen) ;
		iArrayHandle.SetSize(iLen) ;
		for (int i=0; i<iLen; i++)
		{
			ads_name en ;
			acedSSName(sum_name, i, en) ;
			AcDbEntity *pEnt = NULL ;
			if (OpenAcDbEntity(en, pEnt, AcDb::kForRead))
			{
				//acutPrintf(_T("\n%s"), pEnt->isA()->name()) ;
				UINT iHandle = 0 ;
				iHandle = GetHandle(pEnt) ;
				iArrayHandle.SetAt(i,iHandle) ;
				pEnt->close() ;
			}
		}

		acedSSFree(sum_name) ;
	}
	
	return iLen;
}

//打开实体
BOOL CUserSystem::OpenAcDbEntity(const AcDbObjectId objId, AcDbEntity *& pEnt, AcDb::OpenMode iMode)
{
	BOOL bReturn = false ;	
	Acad::ErrorStatus es =acdbOpenAcDbEntity(pEnt,objId, iMode) ;
	if(Acad::eOk != es)
	{
		bReturn = false ;		
	}
	else
	{
		bReturn = true ;
	}
	return bReturn ;	
}
BOOL CUserSystem::OpenAcDbEntity(ads_name ent_name, AcDbEntity *&pEnt, AcDb::OpenMode iMode)
{
	AcDbObjectId objId ;
	acdbGetObjectId(objId, ent_name) ;
	return OpenAcDbEntity(objId, pEnt, iMode) ;
}
BOOL CUserSystem::OpenAcDbEntity(const UINT iHandle, AcDbEntity *& pEnt, AcDb::OpenMode iMode)
{
	AcDbObjectId objId ;
	if (this->GetObjectId(objId, iHandle)==eOk)
	{
		return OpenAcDbEntity(objId, pEnt, iMode) ;
	}
	else
	{
		return FALSE ;
	}	
}

// 缩放视图
BOOL CUserSystem::ZoomView(const AcGePoint3d & pt_3d_center, double dHeight, double dWidth)
{
	// get the extents of the drawing
	AcDbViewTableRecord view;
	// now set the view center point
	AcGePoint2d pt_2d_center(pt_3d_center.x,pt_3d_center.y) ;
	view.setCenterPoint (pt_2d_center);
	// now height and width of view
	view.setHeight(dHeight);
	view.setWidth(dWidth);
	// set the view
	acedSetCurrentView (&view, NULL);
	// updates the extents
	//m_pDb->updateExt(TRUE);

	return 0;
}

// 缩放视图
BOOL CUserSystem::ZoomView()
{
	// get the extents of the drawing
	AcDbViewTableRecord view;
	AcGePoint3d max = m_pDb->extmax(),min = m_pDb->extmin();

	AcGePoint2d max_2d (max[X], max[Y]);
	AcGePoint2d min_2d (min[X], min[Y]);
	// now set the view center point
	view.setCenterPoint (min_2d + (max_2d - min_2d) / 2.0);
	// now height and width of view
	view.setHeight(max_2d[Y] - min_2d[Y]);
	view.setWidth(max_2d[X] - min_2d[X]);
	// set the view
	acedSetCurrentView (&view, NULL);
	// updates the extents
	//pDb->updateExt(TRUE);

	return 0;
}

// 是否存在以strTextStyleName为名称的字体Style
BOOL CUserSystem::HasTextStyle(const TCHAR * strTextStyleName, AcDbObjectId & objId)
{
	BOOL bRet = FALSE ;
	AcDbTextStyleTable * pTextStyleTable = NULL ;	

	if(Acad::eOk == m_pDb->getTextStyleTable(pTextStyleTable, AcDb::kForRead))
	{
		//bRet = pTextStyleTable->has(strTextStyleName) ;
		if (Acad::eOk == pTextStyleTable->getAt(strTextStyleName, objId))
		{
			bRet = TRUE ;
		}
		pTextStyleTable->close() ;
	}

	return  bRet ;
}

//通过id取得style的name
BOOL CUserSystem::GetTextStyle(AcDbObjectId objId, CString & strTextStyleName)
{
	BOOL bRet = FALSE ;

	AcDbTextStyleTableRecord *pStyleTbRec = NULL ;
	if (acdbOpenAcDbObject((AcDbObject*&)pStyleTbRec, objId,AcDb::kForRead)==Acad::eOk)
	{
		TCHAR *pStyleName = NULL ;

		if (pStyleTbRec->getName(pStyleName)==Acad::eOk)
		{
			strTextStyleName.Format(_T("%s"), pStyleName) ;
			bRet = TRUE ;
		}				
		pStyleTbRec->close() ;
		pStyleTbRec = NULL ;
	}
    
	return bRet ;
}

//取得text实体的style的name
BOOL CUserSystem::GetTextStyle(const AcDbText *pText, CString & strTextStyleName)
{
	AcDbObjectId objId = NULL ;
	objId = pText->textStyle() ;
	return this->GetTextStyle(objId, strTextStyleName) ;
}

// 计算一条线的平行线,默认返回上方的平行线
void CUserSystem::GetParallelLine(const AcGeLineSeg2d & lineSeg2dSrc, AcGeLineSeg2d & lineSeg2dParallel, double dDist, BOOL bRetUpLine)
{
	lineSeg2dParallel =  lineSeg2dSrc ;
	AcGeVector2d vecTo(dDist, 0) ;
	AcGeVector2d vecSrc = lineSeg2dSrc.endPoint()-lineSeg2dSrc.startPoint() ;
	vecTo.rotateBy(vecSrc.angle()+(bRetUpLine==TRUE?PI/2:-PI/2)) ;
	lineSeg2dParallel.translateBy(vecTo) ;	
}

// 计算两线段交点（AcGeLineSeg2d本身具有求交点的函数，但是没有AcDb::Intersect intType这种选项）
int CUserSystem::IntersectWith(const AcGeLineSeg2d* pLineSeg2dFrom, const AcGeLineSeg2d* pLineSeg2dTo, AcGePoint2d& pt_2d_ret, AcDb::Intersect intType)
{
	int iRet = 0 ;

	AcGePoint3d pt_3d_start1, pt_3d_end1, pt_3d_start2, pt_3d_end2 ;
	this->ConvertPoint(pLineSeg2dFrom->startPoint(), pt_3d_start1) ;
	this->ConvertPoint(pLineSeg2dFrom->endPoint(), pt_3d_end1) ;
	this->ConvertPoint(pLineSeg2dTo->startPoint(), pt_3d_start2) ;
	this->ConvertPoint(pLineSeg2dTo->endPoint(), pt_3d_end2) ;
	AcDbLine * pLineFrom = new AcDbLine(pt_3d_start1, pt_3d_end1) ;
	AcDbLine * pLineTo = new AcDbLine(pt_3d_start2, pt_3d_end2) ;

	AcGePoint3dArray pt3dArrayIntersect ;
	Acad::ErrorStatus es = pLineFrom->intersectWith(pLineTo, intType, pt3dArrayIntersect) ;
	if (es==Acad::eOk)
	{
		if (pt3dArrayIntersect.length()==1)
		{
			this->ConvertPoint(pt3dArrayIntersect.at(0), pt_2d_ret) ;
			iRet = 1 ;
		}
	}
	
	pLineFrom->close() ;
	pLineTo->close() ;

	return iRet ;
}

//当前视图 (2014.11.14:调用有未知错误)
AcDbViewTableRecord CUserSystem::GetCurrentView()
{
	AcDbViewTableRecord view;
	struct resbuf rb;
	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记

	wcs.restype = RTSHORT;
	wcs.resval.rint = 0;
	ucs.restype = RTSHORT;
	ucs.resval.rint = 1;
	dcs.restype = RTSHORT;
	dcs.resval.rint = 2;

	// 获得当前视口的“查看”模式
	acedGetVar(_T("VIEWMODE"), &rb);
	view.setPerspectiveEnabled(rb.resval.rint & 1);
	view.setFrontClipEnabled(rb.resval.rint & 2);
	view.setBackClipEnabled(rb.resval.rint & 4);
	view.setFrontClipAtEye(!(rb.resval.rint & 16));

	// 当前视口中视图的中心点（UCS坐标）
	acedGetVar(_T("VIEWCTR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], rb.resval.rpoint[Y]));

	// 当前视口透视图中的镜头焦距长度（单位为毫米）
	acedGetVar(_T("LENSLENGTH"), &rb);
	view.setLensLength(rb.resval.rreal);

	// 当前视口中目标点的位置（以 UCS 坐标表示）
	acedGetVar(_T("TARGET"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
	view.setTarget(AcGePoint3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的观察方向（UCS）
	acedGetVar(_T("VIEWDIR"), &rb);
	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));

	// 当前视口的视图高度（图形单位）
	acedGetVar(_T("VIEWSIZE"), &rb);
	view.setHeight(rb.resval.rreal);
	double height = rb.resval.rreal;

	// 以像素为单位的当前视口的大小（X 和 Y 值）
	acedGetVar(_T("SCREENSIZE"), &rb);
	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);

	// 当前视口的视图扭转角
	acedGetVar(_T("VIEWTWIST"), &rb);
	view.setViewTwist(rb.resval.rreal);

	// 将模型选项卡或最后一个布局选项卡置为当前
	acedGetVar(_T("TILEMODE"), &rb);
	int tileMode = rb.resval.rint;

	// 设置当前视口的标识码
	acedGetVar(_T("CVPORT"), &rb);
	int cvport = rb.resval.rint;

	// 是否是模型空间的视图
	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
	view.setIsPaperspaceView(paperspace);
	if (!paperspace)
	{
		// 当前视口中前向剪裁平面到目标平面的偏移量
		acedGetVar(_T("FRONTZ"), &rb);
		view.setFrontClipDistance(rb.resval.rreal);

		// 获得当前视口后向剪裁平面到目标平面的偏移值
		acedGetVar(_T("BACKZ"), &rb);
		view.setBackClipDistance(rb.resval.rreal);
	}
	else
	{
		view.setFrontClipDistance(0.0);
		view.setBackClipDistance(0.0);
	}

	return view;
}

// 找出相同ID的实体
int CUserSystem::FindSameIdEnt(UINT nID, CLongArray & nArrayHandle)
{
	int iRet = 0 ;

	AcDbBlockTable *pBlkTb = NULL ;
	m_pDb->getBlockTable(pBlkTb, AcDb::kForRead) ;
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
			this->GetXdata(pEnt, _T("FindSameId_ISCHECK"), iIsCheck) ;

			int nCurID ;
			if (this->GetXdata(pEnt, _T("ID"), nCurID) && iIsCheck!=1)
			{
				if (nCurID==nID)
				{
					UINT nHandle = this->GetHandle(pEnt) ;
					nArrayHandle.Add(nHandle) ;
					iRet++ ;
					this->SetXdata(pEnt, _T("FindSameId_ISCHECK"), 1) ;
				}
			}

			pEnt->close() ;
		}
	}
	delete pTbRecIter ;
	pTbRecIter = NULL ;

	return iRet;
}
// 取得指定多段线指定顶点的线宽
Acad::ErrorStatus CUserSystem::GetPlineWidthAt( const UINT nPlineHandle, unsigned int nIndex, double& dStartWidth, double& dEndWidth) 
{
	Acad::ErrorStatus es ; 
	AcDbEntity *pEnt = NULL ;
	if (OpenAcDbEntity(nPlineHandle, pEnt, AcDb::kForRead))
	{
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pLine = AcDbPolyline::cast(pEnt) ;
			es = pLine->getWidthsAt(nIndex, dStartWidth, dEndWidth) ;
		}
		pEnt->close() ;
	}

	return es ;
}

// 复制实体
Acad::ErrorStatus  CUserSystem::Wblock(const AcDbObjectIdArray& objIdsOut,AcDbDatabase*& pDbTo, /*const AcGePoint3d& pt_3d_base,*/ const AcGeMatrix3d & xform)
{
	Acad::ErrorStatus es ;

	for (int i=0; i<objIdsOut.length(); i++)
	{
		AcDbObjectId objId = objIdsOut.at(i);
		AcDbEntity *pEnt = NULL ;
		if(Acad::eOk==acdbOpenAcDbEntity(pEnt,objId, AcDb::kForRead))
		{
			AcDbObjectId objIdDic ;
			if ((objIdDic = pEnt->extensionDictionary())!= AcDbObjectId::kNull)
			{
				AcDbDictionary *pDict = NULL;				
				if (Acad::eOk==acdbOpenObject(pDict, objIdDic, AcDb::kForWrite))
				{
					pDict->setTreatElementsAsHard(Adesk::kTrue);
					pDict->close();
				}
			}
			pEnt->close() ;
		}
	}

	AcDbDatabase *pTempDb ;
	es = m_pDb->wblock( pTempDb, objIdsOut, AcGePoint3d::kOrigin ) ;
	if (Acad::eOk != es )
	{
		acedAlert( _T("wblock failed!") );
		return es ;
	}
	es = pDbTo->insert( xform, pTempDb ) ;
	if (Acad::eOk != es)
		acedAlert( _T("insert failed!" ));
	delete pTempDb;

	return es ;
}

Acad::ErrorStatus CUserSystem::Wblock(AcDbDatabase*& pDbFrom, AcDbDatabase*& pDbTo, /*const AcGePoint3d& pt_3d_base,*/ const AcGeMatrix3d & xform)
{
	Acad::ErrorStatus es ;

	AcDbObjectIdArray objIdsOut;

	AcDbBlockTable* pBT = NULL ;
	pDbFrom->getBlockTable(pBT, AcDb::kForRead);
	AcDbBlockTableRecord* pBTR = NULL;
	es = pBT->getAt(ACDB_MODEL_SPACE, pBTR, AcDb::kForRead);
	pBT->close();

	AcDbBlockTableRecordIterator* pIT;
	es = pBTR->newIterator(pIT) ;
	for (; !pIT->done(); pIT->step()) 
	{
		AcDbObjectId objId;
		if (Acad::eOk == pIT->getEntityId(objId))
		{
			objIdsOut.append(objId);
		}
	}

	delete pIT;
	pBTR->close();

	if (!objIdsOut.isEmpty()) 
	{
		es = Wblock(objIdsOut, pDbTo, xform) ;
	}
	else
	{
		acutPrintf(_T("\nNo entities found in model space of DWG"));
	}

	return es ;
}

// 新建一个CAD文档，等同于在cad中new一个文档
void CUserSystem::CreateDocument(void * pData)
{
	AcApDocument* pDoc = acDocManager->curDocument() ;
	if (acDocManager->isApplicationContext()) 
	{
		acDocManager->appContextNewDocument((const TCHAR *)pData) ;
	} 
	else
	{
		acutPrintf(_T("\nERROR To Create Document!\n"));
	}
}

// 打开CAD文档，等同于CAD中Open一个文档
void CUserSystem::OpenDocument(void * pData)
{
	AcApDocument* pDoc = acDocManager->curDocument();
	if (acDocManager->isApplicationContext())
	{
		acDocManager->appContextOpenDocument((const TCHAR *)pData);
	} 
	else
	{
		acutPrintf(_T("\nERROR To Open Document!\n"));
	}
}
