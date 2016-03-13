// DwgDatabaseUtil.cpp: implementation of the CDwgDatabaseUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DwgDatabaseUtil.h"
#include <dbsymtb.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDwgDatabaseUtil::CDwgDatabaseUtil()
{

}

CDwgDatabaseUtil::~CDwgDatabaseUtil()
{

}

AcDbObjectId CDwgDatabaseUtil::PostToModelSpace( AcDbEntity *pEnt, AcDbDatabase *pDb )
{
	// 检查输入参数的有效性
	assert (pEnt);		// 等效于assert (pEnt != NULL);
	
	// 获得当前图形数据库的块表
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);
	
	// 获得模型空间对应的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForWrite);
	pBlkTbl->close();
	
	// 将实体添加到模型空间的块表记录
	AcDbObjectId entId;
	Acad::ErrorStatus es = pBlkTblRcd->appendAcDbEntity(entId, pEnt);
	if (es != Acad::eOk)
	{
		pBlkTblRcd->close();
		delete pEnt;	// 添加失败时，要delete
		pEnt = NULL;
		
		return AcDbObjectId::kNull;
	}
	
	// 关闭模型空间块表记录和实体
	pBlkTblRcd->close();
	pEnt->close();
	
	return entId;
}

AcDbObjectIdArray CDwgDatabaseUtil::GetAllEntityIds( const TCHAR* layerName, AcDbDatabase *pDb )
{
	AcDbObjectIdArray entIds;		// 满足条件的实体集合
	bool bFilterLayer = false;			// 是否需要过滤图层
	AcDbObjectId layerId;
	// 获得指定图层的对象ID
	if (layerName != NULL)
	{
		AcDbLayerTable *pLayerTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTbl, AcDb::kForRead);
		if (!pLayerTbl->has(layerName))
		{
			pLayerTbl->close();
			return entIds;
		}
		pLayerTbl->getAt(layerName, layerId);
		pLayerTbl->close();

		bFilterLayer = true;
	}
	
	// 获得块表
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);
	
	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();
	
	// 创建遍历器，依次访问模型空间的每一个实体
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	for (it->start(); !it->done(); it->step())
	{
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es = it->getEntity(pEnt, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (bFilterLayer)				// 过滤图层
			{
				if (pEnt->layerId() == layerId)
				{
					entIds.append(pEnt->objectId());
				}				
			}
			else
			{
				entIds.append(pEnt->objectId());
			}
			
			pEnt->close();
		}
		else
		{
			acutPrintf(TEXT("\nCDwgDatabaseUtil::GetAllEntityIds中打开实体失败(错误代码:%d)."), (int)es);
		}
	}
	delete it;
	pBlkTblRcd->close();
	
	return entIds;
}

AcDbExtents CDwgDatabaseUtil::GetModelSpaceExtent( AcDbDatabase *pDb )
{
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);
	
	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();
	
	AcDbExtents extent;
	Acad::ErrorStatus es = extent.addBlockExt(pBlkTblRcd);
	pBlkTblRcd->close();
	
	// 如果图形数据库不是当前的工作数据库，有时候直接获取模型空间的范围会失败
	if (es != Acad::eOk)
	{
		AcDbObjectIdArray allEnts = GetAllEntityIds(NULL, pDb);
		for (int i = 0; i < allEnts.length(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (acdbOpenObject(pEnt, allEnts[i], AcDb::kForRead) == Acad::eOk)
			{
				AcDbExtents ext;
				if (pEnt->getGeomExtents(ext) == Acad::eOk)
				{
					extent.addExt(ext);
				}
				
				pEnt->close();
			}
		}
	}
	
	return extent;
}

AcDbObjectIdArray CDwgDatabaseUtil::GetAll_Polyline_EntityIds( const TCHAR* layerName /*= NULL*/, AcDbDatabase *pDb /*= acdbHostApplicationServices()->workingDatabase()*/ )
{
	AcDbObjectIdArray destIds;
	AcDbObjectIdArray allEntIds =CDwgDatabaseUtil::GetAllEntityIds();
	int cnt_blk=0;
	int cnt_dc=0;
	int cnt_cz=0;
	int cnt_pline=0;
	int cnt_line=0;
	int cnt_3dline=0;
	acutPrintf(_T("\n准备打开多段线！"));
	for(int i=0;i<allEntIds.length();i++)
	{
		//AcDbPolyline *Pline =NULL;
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,allEntIds[i],AcDb::kForWrite)==Acad::eOk)
		{
			if(pEnt->isKindOf(AcDbPolyline::desc()))
			{
				destIds.append(allEntIds[i]);
				//Pline =pEnt::cast();
				AcDbPolyline *Pline= AcDbPolyline::cast(pEnt);
				//Pline->upgradeOpen() ;
				cnt_pline++;
				if(cnt_pline>255) 
					cnt_pline -=255;
			    Pline->setColorIndex(cnt_pline+2);
				//Pline->downgradeOpen() ;
		    	acutPrintf(_T("\n打开二维多段线成功！"));
			   Pline->close();
			}
			else if(pEnt->isKindOf(AcDb3dPolyline::desc()))
			{
				destIds.append(allEntIds[i]);
				//Pline =pEnt::cast();
				AcDb3dPolyline *Pline= AcDb3dPolyline::cast(pEnt);
				//Pline->upgradeOpen() ;
				cnt_3dline++;
				if(cnt_3dline>255) 
					cnt_3dline -=255;
				Pline->setColorIndex(cnt_3dline+2);
				//Pline->setColorIndex(2);
				//Pline->downgradeOpen() ;
				acutPrintf(_T("\n打开3维多段线成功！"));
				Pline->close();
			}
			else if(pEnt->isKindOf(AcDbLine::desc()))
			{
				//destIds.append(allEntIds[i]);
				//Pline =pEnt::cast();
				AcDbLine *Pline= AcDbLine::cast(pEnt);
				//Pline->upgradeOpen() ;
				cnt_line++;
				if(cnt_line>255) 
					cnt_line -=255;
				Pline->setColorIndex(cnt_line+2);
				//Pline->setColorIndex(3);
				//Pline->downgradeOpen() ;
				acutPrintf(_T("\n打开直线成功！"));
				Pline->close();
			}
			else if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				acutPrintf(_T("\n识别块！"));
				cnt_blk++;
				AcDbBlockReference *pRef =AcDbBlockReference::cast(pEnt);
				AcDbBlockTableRecord *pBlkDefRcd =NULL;
				if(acdbOpenObject(pBlkDefRcd,pRef->blockTableRecord(),AcDb::kForRead)==Acad::eOk)
				{
					TCHAR * blkname=NULL;
					 Acad::ErrorStatus  es=pBlkDefRcd->getName(blkname);
					 acutPrintf("\n blkname =%s",blkname);
				      if(es==Acad::eOk)		
					  {
						if(strcmp(blkname,_T("DC"))==0)
						{
				            acutPrintf("\n 遇到调车信号机");
							cnt_dc++;
						}
						else if(strcmp(blkname,_T("CZ_A-LFUHB1"))==0)
						{
							acutPrintf("\n 遇到出站信号机");
							cnt_cz++;
						}
						else if(strcmp(blkname,_T("JCB"))==0)
						{
							acutPrintf("\n 遇到警冲标");
       
						}
						else if(strcmp(blkname,_T("JZ"))==0)
						{

						}
						else if(strcmp(blkname,_T("XNJZXHJ"))==0)
						{

						}
						else
						{

						}

					}
				}
				else
				{
						acutPrintf("\n 块打开失败");
				}
				pBlkDefRcd->close();
				pRef->close();
				

			}
			else
			{

			}
			  pEnt->close() ;
		}	
		acutPrintf(_T("\n打开多段线结束！"));
	  ///  acutPrintf(_T("\n本图含有%d个块！"),cnt_blk);
	}
	acutPrintf(_T("\n本图含有%d个块！"),cnt_blk);
	return destIds;
}

AcDbObjectIdArray CDwgDatabaseUtil::GetAll_Block_EntityIds( const TCHAR* layerName /*= NULL*/, AcDbDatabase *pDb /*= acdbHostApplicationServices()->workingDatabase()*/ )
{
	AcDbObjectIdArray destIds;
	AcDbObjectIdArray allEntIds =CDwgDatabaseUtil::GetAllEntityIds();
	int blk_cnt=0;
	for(int i=0;i<allEntIds.length();i++)
	{
		//AcDbPolyline *Pline =NULL;
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,allEntIds[i],AcDb::kForWrite)==Acad::eOk)
		{
			//只处理块
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{	
				AcDbBlockReference *pRef =AcDbBlockReference::cast(pEnt);
				AcDbBlockTableRecord *pBlkDefRcd =NULL;
				if(acdbOpenObject(pBlkDefRcd,pRef->blockTableRecord(),AcDb::kForRead)==Acad::eOk)
				{
					TCHAR * blkname=NULL;
					Acad::ErrorStatus  es=pBlkDefRcd->getName(blkname);
					acutPrintf("\n blkname =%s",blkname);
					blk_cnt++;
					//获取Id
					if(es==Acad::eOk)		
					{
						destIds.append(allEntIds[i]);
						if(strcmp(blkname,_T("DC"))==0)
						{
							acutPrintf("\n 遇到调车信号机");
							//cnt_dc++;
							destIds.append(allEntIds[i]);
						}
						else if(strcmp(blkname,_T("CZ_A-LFUHB1"))==0)
						{
							acutPrintf("\n 遇到出站信号机");
							//cnt_cz++;
						}
						else if(strcmp(blkname,_T("JCB"))==0)
						{
							acutPrintf("\n 遇到警冲标");

						}
						else if(strcmp(blkname,_T("JZ"))==0)
						{
                             acutPrintf("\n 遇到计轴");
						}
						else if(strcmp(blkname,_T("XNJZXHJ"))==0)
						{
                           acutPrintf("\n 遇到下逆进站信号机");  
						}
						else
						{

						}

					}
				}
				else
				{
					acutPrintf("\n 块打开失败");
				}
				pBlkDefRcd->close();
				pRef->close();
			}
			else
			{

			}
			pEnt->close() ;
		}	
	}
	acutPrintf(_T("\n本图含有%d个块！"),blk_cnt);
	return destIds;

}
//按名字获取块ID
AcDbObjectIdArray CDwgDatabaseUtil::GetBlockids_byName( const TCHAR* blockName,const TCHAR* layerName /*= NULL*/, AcDbDatabase *pDb /*= acdbHostApplicationServices()->workingDatabase()*/ )
{
	AcDbObjectIdArray destIds;
	AcDbObjectIdArray allEntIds =CDwgDatabaseUtil::GetAllEntityIds();
	int blk_cnt=0;
	for(int i=0;i<allEntIds.length();i++)
	{
		//AcDbPolyline *Pline =NULL;
		AcDbEntity *pEnt=NULL;
		if(acdbOpenObject(pEnt,allEntIds[i],AcDb::kForWrite)==Acad::eOk)
		{
			//只处理块
			if(pEnt->isKindOf(AcDbBlockReference::desc()))
			{	
				AcDbBlockReference *pRef =AcDbBlockReference::cast(pEnt);
				AcDbBlockTableRecord *pBlkDefRcd =NULL;
				if(acdbOpenObject(pBlkDefRcd,pRef->blockTableRecord(),AcDb::kForRead)==Acad::eOk)
				{
					TCHAR * blkname=NULL;
					Acad::ErrorStatus  es=pBlkDefRcd->getName(blkname);
					//acutPrintf("\n blkname =%s",blkname);
					//blk_cnt++;
					//获取Id
					if(es==Acad::eOk)		
					{
						//destIds.append(allEntIds[i]);
						if(strcmp(blkname,blockName)==0)
						{
							destIds.append(allEntIds[i]);
							blk_cnt++;
						}
					}
				}
				else
				{
					acutPrintf("\n 块打开失败");
				}
				pBlkDefRcd->close();
				pRef->close();
			}
			else
			{
			}
			pEnt->close() ;
		}	
	}
	acutPrintf(_T("\n本图含有%s块共%d个！"),blockName,blk_cnt);
	return destIds;
}
