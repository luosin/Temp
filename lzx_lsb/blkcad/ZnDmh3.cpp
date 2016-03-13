#include "stdafx.h"
#include "zndmh.h"

#include "DBUtility.h"

#include "ConNode.h"

extern BOOL bFirst ;

// 端子关联
BOOL CZnDmh::SetEntConnect(AcDbEntity *pEnt, long nSecondId , long nConCableId, int iMode)
{
	BOOL bReturn = FALSE ;
	//读取连接;
	long nConId = 0 ;
	AcDbText * pText = NULL ;

	AcCmColor color ;
	color.setColorIndex(1) ;

	if(!g_cSystem.GetXdata(pEnt, _T("CON_1_ID") , nConId))
	{
		g_cSystem.SetXdata(pEnt , _T("CON_1_ID") , nSecondId) ;
		if(nConCableId > 0)
		{
			g_cSystem.SetXdata (pEnt , _T("CON_1_CABLE_ID") , nConCableId) ;
		}
		pEnt->setColor(color) ;
		bReturn = TRUE ;
	}
	else if (iMode==1)
	{
		g_cSystem.SetXdata(pEnt , _T("CON_2_ID") , nSecondId) ;
		if(nConCableId > 0)
		{
			g_cSystem.SetXdata (pEnt , _T("CON_2_CABLE_ID") , nConCableId) ;
		}
		pEnt->setColor(color) ;
		bReturn = TRUE ;
	}
	else
	{
		CConNode CConNode ;
		if(pEnt->isKindOf(AcDbText::desc()))
		{
			pText = AcDbText::cast(pEnt) ;
			CConNode.m_strNode = pText->textString() ;
		}

		AcDbEntity * pEntCon = NULL ; //与实体一连接的实体;
		CConNode.m_strPrompt1 = _T("重新建立连接") ;
		if(GetEntFromId(pEntCon , nConId))
		{
			if(pEntCon->isKindOf(AcDbText::desc()))
			{
				pText = AcDbText::cast(pEntCon) ;
				CConNode.m_strPrompt1.Format(_T("替换%s与%s的连接") , CConNode.m_strNode , pText->textString());
			}
			pEntCon->close() ;
		}
		if(!g_cSystem.GetXdata(pEnt , _T("CON_2_ID") , nConId))
		{	
			CConNode.m_strPrompt2 = _T("增加一个节点连接") ;
		}
		else
		{

			CConNode.m_strPrompt2 = _T("重新建立连接") ;
			if(GetEntFromId(pEntCon , nConId))
			{
				if(pEntCon->isKindOf(AcDbText::desc()))
				{
					pText = AcDbText::cast(pEntCon) ;
					CConNode.m_strPrompt2.Format(_T("替换%s与%s的连接") , CConNode.m_strNode , pText->textString());
				}
				pEntCon->close() ;
			}
		}
		if(IDOK == CConNode.DoModal())
		{
			if(CConNode.m_iVal == 0)
			{
				g_cSystem.SetXdata(pEnt , _T("CON_1_ID") , nSecondId) ;
				if(nConCableId > 0)
				{
					g_cSystem.SetXdata (pEnt , _T("CON_1_CABLE_ID") , nConCableId) ;
				}
			}
			else
			{
				g_cSystem.SetXdata(pEnt , _T("CON_2_ID") , nSecondId) ;
				if(nConCableId > 0)
				{
					g_cSystem.SetXdata (pEnt , _T("CON_2_CABLE_ID") , nConCableId) ;
				}
			}
			pEnt->setColor(color) ;
			bReturn = TRUE ;
		}
		//
	}
	return bReturn ;
}

BOOL CZnDmh::GetEntFromId(AcDbEntity *& pEnt, long nEntId)
{
	BOOL bReturn = FALSE ;
	AcDbBlockTable * pBlkTbl = NULL ;
	g_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	long nGetVal = 0 ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbText::desc()))
			{
				if(g_cSystem.GetXdata (pEnt , _T("ID") , nGetVal))
				{
					if(nGetVal == nEntId)
					{
						bReturn = TRUE ;
						break ;
					}
				}
			}
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
	return bReturn ;

}

//nMode=0：输入连接电缆类型
//nMode=1：选择连接电缆类型
int CZnDmh::ConTie(int nMode)
{
	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;
	AcDbObjectId layer_Id ;
	layer_Id = g_cSystem.Set_Layer(_T("") , _T("tie")) ;
	acutPrintf(_T("\n在两个节点之间建立关联")) ;

	long nMaxId = g_cSystem.GetMaxId() ;

	ads_name first_name  , second_name , third_name ;
	int iSelect_Val = 0 ;
	iSelect_Val = SelectAcDbText(_T("\n选取相连的第一个接点(对应的文本)") , first_name) ;
	if(iSelect_Val == -1)
	{
		return -1;
	}
	AcDbEntity * pEnt_1 = NULL ;
	AcDbEntity * pEnt_2 = NULL ;
	AcDbEntity * pEnt_3 = NULL ;
	if(g_cSystem.OpenAcDbEntity(first_name , pEnt_1))
	{
		pEnt_1->highlight() ;
	}
	else
	{
		return -1;
	}
	while (TRUE)
	{
		iSelect_Val = SelectAcDbText(_T("\n选取相连的第二个接点(对应的文本)") , second_name) ;
		if(iSelect_Val == -1)
		{
			pEnt_1->unhighlight() ;
			pEnt_1->close() ;
			return -1;
		}
		else
		{
			if(g_cSystem.OpenAcDbEntity(second_name , pEnt_2))
			{
				pEnt_2->highlight() ;
			}
			break ;	
		}
	}


	// 选取连接电缆类型文本 ;
	AcDbBlockReference *pRef = NULL ;

	AcGePoint3d pt_3d_insert(0,0,0) ;  
	AcGePoint3d pt_3d_1 = (AcDbText::cast(pEnt_1))->position() ;
	AcGePoint3d pt_3d_2 = (AcDbText::cast(pEnt_2))->position() ;
	pt_3d_insert.x = pt_3d_1.x+(pt_3d_2.x-pt_3d_1.x)/2 ;
	pt_3d_insert.y = pt_3d_1.y+(pt_3d_2.y-pt_3d_1.y)/2 ;

	while (TRUE)
	{
		if (nMode==0)
		{
			iSelect_Val = InputCableType(_T("\n电缆类型(没有不输)[输入0,表示类型为※,输入1,表示类型为※1]:") , _T("conti") , pt_3d_insert, pRef, third_name) ;
			pEnt_1->unhighlight() ;
			pEnt_2->unhighlight() ;
			if(iSelect_Val == 1)
			{	
				//pRef->setVisibility(AcDb::kInvisible) ;
				pEnt_3 = (AcDbEntity *) pRef ;			
			}
		}
		else
		{
			iSelect_Val = SelectAcDbBlockReference(_T("\n选取相连的电缆(对应的图块)") , _T("conti") , third_name) ;
			pEnt_1->unhighlight() ;
			pEnt_2->unhighlight() ;
			if(iSelect_Val == 1)
			{
				g_cSystem.OpenAcDbEntity(third_name , pEnt_3) ;
			}
		}

		break ;


	}

	long nFirst_Id = 0 , nSecond_Id = 0 ;
	if(!g_cSystem.GetXdata(pEnt_1 , _T("ID") , nFirst_Id))
	{
		nFirst_Id = nMaxId+1 ;
		nMaxId++ ;
		g_cSystem.SetXdata(pEnt_1 , _T("ID") , nFirst_Id) ;
	}
	if(!g_cSystem.GetXdata(pEnt_2 , _T("ID") , nSecond_Id))
	{
		nSecond_Id = nMaxId+1 ;
		nMaxId++ ;
		g_cSystem.SetXdata(pEnt_2 , _T("ID") , nSecond_Id) ;
	}
	long nCon_Cable_Id = 0 ;
	if(pEnt_3 != NULL)
	{
		if(!g_cSystem.GetXdata(pEnt_3 , _T("ID") , nCon_Cable_Id))
		{
			nCon_Cable_Id = nMaxId+1 ;
			nMaxId++ ;
			g_cSystem.SetXdata(pEnt_3 , _T("ID") , nCon_Cable_Id) ;
		}
	}
	if(SetEntConnect(pEnt_1 , nSecond_Id , nCon_Cable_Id))
	{
		SetEntConnect(pEnt_2 , nFirst_Id , nCon_Cable_Id) ;
	}

	pEnt_1->setLayer(layer_Id) ;
	pEnt_2->setLayer(layer_Id) ;


	if(pRef != NULL)
	{
		pRef->close() ;
	}

	if(pEnt_3 != NULL)
	{
		pEnt_3->close() ;
	}

	pEnt_1->close() ;
	pEnt_2->close() ;

	return 0;
}

int CZnDmh::SelectAcDbText(const TCHAR *chPrompt, ads_name &ent_name)
{
	int iReturn = 0 ;
	ads_point pt = { 0,0,0 } ;
	while (TRUE)
	{
		int iGet_Sel = 0 ;
		iGet_Sel = ads_entsel (chPrompt , ent_name , pt) ;
		if(RTNORM == iGet_Sel)
		{
			AcDbEntity * pEnt = NULL ;
			if(g_cSystem.OpenAcDbEntity(ent_name , pEnt))
			{
				if (pEnt->isKindOf(AcDbText::desc()))
				{
					iReturn = 1 ;
					pEnt->close() ;
					break ;
				}
				else
				{
					acutPrintf(_T("\n选取的不是文本，重新选择")) ;
				}
				pEnt->close() ;
			}
			else
			{
				acutPrintf(_T("\n选取实体错误，重新选择")) ;
			}
		}
		else if (RTERROR == iGet_Sel)
		{
			int iError_Val = 0 ;
			g_cSystem.GetSysVar(_T("ERRNO") , iError_Val) ;
			if(iError_Val == 52)
			{
				iReturn = -1 ;
				break ;
			}
			else
			{
				acutPrintf(_T("\n选取错误，重新选择")) ;
			}
		}
		else // == RTCAN ;
		{
			iReturn = -1 ;
			break ;
		}
	}
	return iReturn ;
}

int CZnDmh::SelectAcDbBlockReference(const TCHAR *chPrompt, const TCHAR * chBlockPre , ads_name &ent_name)
{
	int iReturn = 0 ;
	ads_point pt = { 0,0,0 } ;
	while (TRUE)
	{
		int iGet_Sel = 0 ;
		iGet_Sel = ads_entsel (chPrompt , ent_name , pt) ;
		if(RTNORM == iGet_Sel)
		{
			AcDbEntity * pEnt = NULL ;
			if(g_cSystem.OpenAcDbEntity(ent_name , pEnt))
			{		
				if(pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
					CString strBlockName ;
					g_cSystem.GetBlockName(pRef , strBlockName) ;
					strBlockName.MakeLower () ;
					if(strBlockName.GetLength() > 5)
					{
						if(_tcscmp(strBlockName.Mid(0, 5) , chBlockPre)==0)
						{
							iReturn = 1 ;
							pEnt->close() ;
							break ;
						}
					}
					else
					{
						ads_printf(_T("\n选取的不是连接电缆图块，重新选择")) ;
					}

				}
				else
				{
					ads_printf(_T("\n选取的不是图块，重新选择")) ;
				}
				pEnt->close() ;
			}
			else
			{
				ads_printf(_T("\n选取实体错误，重新选择")) ;
			}
		}
		else if (RTERROR == iGet_Sel)
		{
			int iError_Val = 0 ;
			g_cSystem.GetSysVar("ERRNO" , iError_Val) ;
			if(iError_Val == 52)
			{
				iReturn = -1 ;
				break ;
			}
			else
			{
				ads_printf(_T("\n选取错误，重新选择")) ;
			}
		}
		else // == RTCAN ;
		{
			iReturn = -1 ;
			break ;
		}
	}
	return iReturn ;
}

int CZnDmh::InputCableType(const TCHAR *chPrompt, const TCHAR *chBlockPre, const AcGePoint3d pt_3d_insert, AcDbBlockReference *&pRef, ads_name &ent_name)
{
	int iReturn = 0 ;

	//g_cSystem.SetLayerStatus(_T("AppUse_CableType"),false) ;
	g_cSystem.Set_Layer(_T("") , _T("AppUse_CableType")) ;


	while (TRUE)
	{
		int iConType = 0 ;
		int iGet_Sel = ads_getint(chPrompt, &iConType);

		if(RTNORM == iGet_Sel)
		{
			if (iConType>=0)
			{
				CStringArray strArrayAtt ;
				CString strAtt ;
				strAtt.Format(_T("%d"), iConType) ;
				strArrayAtt.Add(strAtt) ;

				if (g_cSystem.InsertBlock(pRef, _T("conti_2"), pt_3d_insert, &strArrayAtt, FALSE, 0))
				{
					iReturn = 1 ;
					//pRef->close() ;
					g_cSystem.SetXdata(pRef, _T("TYPE"), _T("AppUse_CableType")) ;
					break;
				}				
			}
			else
			{
				acutPrintf(_T("请输入大于等于0的整数!")) ;
			}			
		}
		else if (RTERROR == iGet_Sel)
		{
			int iError_Val = 0 ;
			g_cSystem.GetSysVar(_T("ERRNO") , iError_Val) ;
			if(iError_Val == 52)
			{
				iReturn = -1 ;
				break ;
			}
			else
			{
				acutPrintf(_T("\n选取错误，重新选择")) ;
			}
		}
		else // == RTCAN ;
		{
			iReturn = -1 ;
			break ;
		}

	}
	g_cSystem.Set_Layer(_T("") , _T("0")) ;
	g_cSystem.SetLayerStatus(_T("AppUse_CableType"),true) ;

	return iReturn ;

}
int CZnDmh::SetPar(void)
{
	g_pCurDb = acdbCurDwg() ;
	g_cSystem.m_pDb = g_pCurDb ;

	acutPrintf(_T("\n在端子与柜子间建立关联")) ;
	ads_name sum_name, par_name ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0,_T("TEXT"), 0) ;		

	acutPrintf (_T("\n选取端子(对应文本) ")) ;

	if(RTNORM == ads_ssget (NULL , NULL , NULL , pRb , sum_name))
	{
		long nLength = 0 ;
		ads_sslength (sum_name , &nLength) ;
		if(nLength > 0)
		{
			while (TRUE )
			{
				int iSelect_Val = 0 ;
				iSelect_Val = SelectAcDbText(_T("\n选取柜子(对应的文本)"), par_name) ;
				if(iSelect_Val == -1)
				{
					return -1;
				}
				ads_name temp_name ;
				BOOL bIn_Ss = FALSE ;
				for (int i = 0 ; i < nLength ; i ++)
				{
					ads_ssname (sum_name , i , temp_name) ;
					if(ads_name_equal (temp_name , par_name))
					{
						bIn_Ss = TRUE ;
						break ;
					}
				}
				if(bIn_Ss)
				{
					acutPrintf (_T("\n端子和柜子是同一个实体，重新选取")) ;
					continue ;
				}
				else
				{
					break ;
				}
			}
			AcDbEntity * pTemp_Ent = NULL ;
			AcDbEntity * pPar_Ent = NULL ;
			if(g_cSystem.OpenAcDbEntity(par_name , pPar_Ent))
			{
				long nPar_Id = 0 ;
				long nMaxId = g_cSystem.GetMaxId() ;
				if(!g_cSystem.GetXdata(pPar_Ent , _T("ID") , nPar_Id))
				{
					nPar_Id = nMaxId+1 ;
					g_cSystem.SetXdata(pPar_Ent , _T("ID") , nPar_Id) ;
				}
				g_cSystem.SetXdata(pPar_Ent , _T("IS_PARENT") , 1) ; //记录是父级文本
				AcCmColor color ;
				color = pPar_Ent->color() ;
				if(color.colorIndex() == 5) 
				{
					color.setColorIndex(6) ;
				}
				else
				{
					color.setColorIndex(5) ;
				}
				pPar_Ent->setColor(color) ;
				pPar_Ent->close() ;
				for (int i = 0 ; i < nLength ; i ++)
				{
					ads_name temp_name ;
					ads_ssname (sum_name , i , temp_name) ;
					if(g_cSystem.OpenAcDbEntity(temp_name , pTemp_Ent  ))
					{
						g_cSystem.SetXdata(pTemp_Ent , _T("PARENT_ID") , nPar_Id) ;

						color.setColorIndex(3) ;
						pTemp_Ent->setColor(color) ;
						pTemp_Ent ->close() ;
					}
				}
				acutPrintf(_T("\n指定完成")) ;

			}
			else
			{
				acutPrintf(_T("\n指定失败")) ;
			}
		}
		ads_ssfree (sum_name) ;
	}
	else
	{
		AfxMessageBox(_T("没有选中任何实体")) ;
	}
	acutRelRb(pRb) ;

	return 0;
}

// 读取组合内部到数据库, iFitFor=0 通号院, =1 电化局
int CZnDmh::ReadNB(const CString & strZhname, int iFitFor)
{
	double dJDQWidth = 40.000 ;
	double dJDQHeight = 6.000 ;
	double dJDQHeight2 = 7.000 ;
	int iNodes[2][14] = {{72,71,73,52,51,53,32,31,33,12,11,13,3,1},{82,81,83,62,61,63,42,41,43,22,21,23,4,2}} ;
	CString strSql ;
	CString strTable ;
	if (iFitFor==0)
	{
		strTable = _T("dmh_def_zhnb") ;
	}
	else if (iFitFor==1)
	{
		strTable = _T("dmh_def_zhnb_dhj") ;
	}

	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase() ;
	CUserSystem mysys ;
	mysys.m_pDb = pDb ;

	struct resbuf *pRb ;
	pRb = acutBuildList(RTDXF0,_T("INSERT"), 0) ;	
	long iLen = 0 ;
	CLongArray iArrayHandle ;
	acutPrintf(_T("\n选择当前组合的相关图块")) ;
	iLen = mysys.GetSSEntHandle(NULL, NULL, NULL, pRb, iArrayHandle) ;
	acutRelRb(pRb) ;
	acedSetStatusBarProgressMeter(_T("读入组合内部"), 1, iLen) ;
	for(int i=0; i<iLen; i++)
	{
		acedSetStatusBarProgressMeterPos(i) ;
		acutPrintf(_T("\n处理第%d/%d个块"), i, iLen) ;
		AcDbEntity *pEnt = NULL ;
		if (mysys.OpenAcDbEntity(iArrayHandle.GetAt(i),pEnt, AcDb::kForRead))
		{				
			AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;
			CString strBlkName ;
			mysys.GetBlockName(pRef, strBlkName) ;
			if (strBlkName==_T("ZHNB-JDQ"))
			{
				AcGePoint3d pt_3d_base(0,0,0) ;
				pt_3d_base = pRef->position() ;
				//jdq index 1~10
				AcGePoint3d pt_3d_bottomleft(pt_3d_base.x-dJDQWidth,pt_3d_base.y-dJDQHeight,0), pt_3d_topright(pt_3d_base) ;
				CLongArray iArrayHandle ;
				mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle, 1) ;
				if (iArrayHandle.GetCount()==1)
				{
					int iHandle = iArrayHandle.GetAt(0) ;
					AcDbEntity *pEntIndex = NULL ;
					if(mysys.OpenAcDbEntity(iHandle, pEntIndex, AcDb::kForRead))
					{
						if (pEntIndex->isKindOf(AcDbText::desc()))
						{
							AcDbText *pText = NULL ;
							pText = AcDbText::cast(pEntIndex) ;
							CString strJDQIndex = pText->textString() ;
							strJDQIndex.Trim() ;
							int iJDQIndex = 0 ;
							iJDQIndex = _tstoi(strJDQIndex) ;

							//jdq name
							CString strJDQName = _T("") ;
							pt_3d_bottomleft.y = pt_3d_base.y-dJDQHeight*2 ;
							pt_3d_topright.y = pt_3d_base.y-dJDQHeight ;
							pt_3d_topright.x = pt_3d_base.x-dJDQWidth/2 ;
							iArrayHandle.RemoveAll() ;
							mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle, 1, 1) ;
							int iFindJDQName = iArrayHandle.GetCount() ;
							if (iFindJDQName>0) //可能有些地方名称是 6GCJ (1GCJ),2个文本
							{
								if (iFindJDQName>1)
								{
									mysys.SortEnt(iArrayHandle, 0, 1) ;
								}
								for(int i=0; i<iFindJDQName; i++)
								{
									iHandle = iArrayHandle.GetAt(i) ;
									AcDbEntity *pEntName = NULL ;
									if (mysys.OpenAcDbEntity(iHandle, pEntName, AcDb::kForRead))
									{
										if (pEntName->isKindOf(AcDbText::desc()))
										{
											pText = AcDbText::cast(pEntName) ;
											CString strJDQNameTmp ;
											strJDQNameTmp = pText->textString() ;	
											strJDQNameTmp.Trim() ;
											strJDQName.Append(strJDQNameTmp) ;
										}
										pEntName->close() ;
									}
								}
							}

							//jdq type
							CString strJDQType = _T("") ;
							pt_3d_bottomleft.x = pt_3d_topright.x ;								
							pt_3d_topright.x = pt_3d_base.x ;
							iArrayHandle.RemoveAll() ;
							mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle, 1) ;
							if (iArrayHandle.GetCount()==1)
							{
								iHandle = iArrayHandle.GetAt(0) ;
								AcDbEntity *pEntType = NULL ;
								if (mysys.OpenAcDbEntity(iHandle, pEntType, AcDb::kForRead))
								{
									if (pEntType->isKindOf(AcDbText::desc()))
									{
										pText = AcDbText::cast(pEntType) ;
										strJDQType = pText->textString() ;
										strJDQType.Trim() ;
									}
									pEntType->close() ;
								}
							}

							for (int iCol=0; iCol<2; iCol++)
							{
								for (int iRow=0; iRow<14; iRow++)
								{
									pt_3d_bottomleft.x = pt_3d_base.x-(dJDQWidth/2)*(2-iCol)+5 ;
									pt_3d_bottomleft.y = pt_3d_base.y-(dJDQHeight*2+dJDQHeight2*(iRow+1)) ;
									pt_3d_topright.x = pt_3d_base.x-(dJDQWidth/2)*(1-iCol) ;
									pt_3d_topright.y = pt_3d_base.y-(dJDQHeight*2+dJDQHeight2*iRow) ;
									iArrayHandle.RemoveAll() ;
									int iNodeConSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle, 1) ;										
									if (iNodeConSum>0)
									{
										//mysys.SortEnt(iArrayHandle,2);
										AcDbEntity *pEntNodeCon = NULL ;
										CString strNodeCon = _T("") ;
										for (int i=0; i<iNodeConSum; i++)
										{
											iHandle = iArrayHandle.GetAt(i) ;
											if (mysys.OpenAcDbEntity(iHandle, pEntNodeCon, AcDb::kForRead))
											{
												if (pEntNodeCon->isKindOf(AcDbText::desc()))
												{
													pText = AcDbText::cast(pEntNodeCon) ;
													strNodeCon = pText->textString() ;

													strNodeCon.Trim() ;
													if (!strNodeCon.IsEmpty())
													{
														int iConType = 0 ;
														if (strNodeCon.Left(1)==_T("0")) //01-1
														{
															iConType = 0 ;
														}
														else if (strNodeCon.Find(_T("-"))!=-1) //6-11
														{
															iConType = 1 ;
														}
														else if (_tstoi(strNodeCon)!=0) //单个数字，例：3
														{
															iConType = 1 ;
														}
														else //KZ
														{
															iConType = 2 ;
														}

														strSql.Format(_T("insert into %s(dmh_def_zhnb_zhname, dmh_def_zhnb_jdqindex, dmh_def_zhnb_jdqtype, dmh_def_zhnb_jdqname, dmh_def_zhnb_jdqnode, dmh_def_zhnb_con, dmh_def_zhnb_conindex, dmh_def_zhnb_contype) values('%s', %d, '%s','%s',%d, '%s', %d, %d)"),strTable, strZhname, iJDQIndex, strJDQType, strJDQName, iNodes[iCol][iRow], strNodeCon, i,iConType) ;

														ExecSqlString(strSql) ;
													}
												}
												pEntNodeCon->close() ;
											}

										}
									}
								}
							}
						}
						pEntIndex->close() ;
					}			
				}
			}

			pEnt->close() ;
		}
	}
	acedRestoreStatusBar() ;

	return 0;
}

// 删除类似00-0端子
void CZnDmh::DelNoUserEnt(AcDbDatabase * & pDb)
{
	CStringArray strArrayTypeToDel ;

	CUserSystem mysys ;
	mysys.m_pDb = pDb ;
	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	int i1 = 0 ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			if (!pEnt->isErased() && pEnt->isKindOf(AcDbText::desc()))
			{
				int iVal = 0 ;
				if (mysys.GetXdata(pEnt, _T("CHECK_DELETE"), iVal))
				{
					AcDbText *pText = AcDbText::cast(pEnt) ;
					TCHAR *pStrText = pText->textString() ;

					if (_tcscmp(pStrText, _T("00-0"))==0)
					{				
						CString strType ;
						mysys.GetXdata(pEnt, _T("TYPE"), strType) ;
						CString strTieType ;
						CString strTmp ;
						strTmp =  strType.Right(strType.GetLength()-strType.Find(_T("_"))-1) ;
						strTieType.Format(_T("TIE_%s"), strTmp) ;
						strArrayTypeToDel.Add(strTieType) ;
						//acutPrintf(_T("\n%s-%.3f-%.3f-%s\n"), pStrText, pText->position().x, pText->position().y, strTieType) ;
						pText->erase() ;
					}
				}
			}

			pEnt->close() ;
		}
		i1++ ;
	}

	int i2 = 0 ;
	//acutPrintf(_T("\ntodelcount:%d\n"), strArrayTypeToDel.GetCount()) ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForWrite))
		{
			if (!pEnt->isErased() && pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				int iVal = 0 ;
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;

				if (mysys.GetXdata(pEnt, _T("CHECK_DELETE"), iVal))
				{
					CString strType ;
					if (mysys.GetXdata(pEnt, _T("TYPE"), strType))
					{
						if(CBlkUtility::FindStrInArray(strArrayTypeToDel, strType)!=-1)
						{
							//acutPrintf(_T("\n%s-%.3f-%.3f\n"), strType, pRef->position().x, pRef->position().y) ;
							pEnt->erase() ;
						}
					}

				}
			}	

			pEnt->close() ;
		}

		i2++ ;
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

}
// 删除生图过程中的扩展数据，主要是Sn等
void CZnDmh::DelSn(AcDbDatabase * & pDb)
{
	g_cSystem.m_pDb = pDb ;
	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	CString strToDelXdatas[5] = {_T("MY_SN"), _T("FIRST_SN"), _T("FIRST_CON_SN"), _T("PAR_SN"), _T("HASTRANS")} ;

	AcDbEntity * pEnt_1 = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		long nMaxId = g_cSystem.GetMaxId() ;
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt_1, AcDb::kForWrite))
		{
			for (int i=0; i<5; i++)
			{
				g_cSystem.DeleteXdata(pEnt_1, strToDelXdatas[i]) ;
			}

			pEnt_1->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

// 查找指定扩展数据的实体
int CZnDmh::GetTie(const CString & strToFindApp, const CString & strToFindXdata, UINT& iHandle)
{
	int iRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	g_cSystem.m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt, AcDb::kForRead))
		{
			CString strXdata ;
			if (g_cSystem.GetXdata(pEnt, strToFindApp, strXdata))
			{
				if (strXdata == strToFindXdata)
				{
					iHandle = g_cSystem.GetHandle(pEnt) ;
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


// 将图纸中的MySn等信息转换成Id，以便配线
void CZnDmh::TransMySnToId(AcDbDatabase* & pDb)
{
	g_cSystem.m_pDb = pDb ;
	g_pCurDb = pDb ;

	AcDbBlockTable * pBlkTbl = NULL ;
	pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;
	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt_1 = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		long nMaxId = g_cSystem.GetMaxId() ;
		//遍历并打开实体 ;
		if (Acad::eOk == pBlkTblRecIter->getEntity(pEnt_1, AcDb::kForWrite))
		{
			if (pEnt_1->isKindOf(AcDbText::desc()))
			{
				AcDbText *pText = AcDbText::cast(pEnt_1) ;
				CString strText = pText->textString() ;

				int iHasTrans = 0 ;
				if (g_cSystem.GetXdata(pEnt_1, _T("HASTRANS"), iHasTrans)==FALSE)
				{
					//begin 处理first_sn
					CString strFirstSn ;
					if (g_cSystem.GetXdata(pEnt_1, _T("FIRST_SN"), strFirstSn))
					{
						//查看firsts_sn是否已经有对应的con_1_id,有则不用处理了
						int iCon_1_Id = 0 ;
						if (!g_cSystem.GetXdata(pEnt_1, _T("CON_1_ID"), iCon_1_Id))
						{
							//先找出对方端子
							UINT iSecondHandle = 0 ;
							int iSecHandleSum = this->GetTie(_T("MY_SN"), strFirstSn, iSecondHandle) ;
							if (iSecHandleSum>0)
							{
								//先看本端子是否有关联电缆类型
								CString strFirstConSn = _T("") ;
								UINT iConTypeHandle = 0 ;								
								if(g_cSystem.GetXdata(pEnt_1, _T("FIRST_CON_SN"), strFirstConSn))
								{
									int iConTypeHandleSum = this->GetTie(_T("MY_SN"), strFirstConSn, iConTypeHandle) ;
									if (iConTypeHandleSum>1)
									{
										acutPrintf(_T("\n!!!!!!!!!!AppName为 MY_SN，值为%s的记录不止一条，请注意！"), strFirstConSn) ;
									}
								}

								AcDbEntity *pEnt_2 = NULL ; //连接的端子
								AcDbEntity *pEnt_3 = NULL ; //连接的电缆类型
								if(g_cSystem.OpenAcDbEntity(iSecondHandle, pEnt_2))
								{
									//若在第一个端子xdata中没有连接电缆类型sn，试着去第二个端子找找
									if (iConTypeHandle==0)
									{
										if(g_cSystem.GetXdata(pEnt_2, _T("FIRST_CON_SN"), strFirstConSn))
										{
											int iConTypeHandleSum2 = this->GetTie(_T("MY_SN"), strFirstConSn, iConTypeHandle) ;
											if (iConTypeHandleSum2>1)
											{
												acutPrintf(_T("\n!!!!!!!!!!AppName为 MY_SN，值为%s的记录不止一条，请注意！"), strFirstConSn) ;
											}
										}
									}
									if (iConTypeHandle!=0)
									{
										g_cSystem.OpenAcDbEntity(iConTypeHandle, pEnt_3) ;
									}

									//添加ID
									long nFirst_Id = 0 , nSecond_Id = 0 ;
									//本端子
									if(!g_cSystem.GetXdata(pEnt_1 , _T("ID") , nFirst_Id))
									{
										nFirst_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt_1 , _T("ID") , nFirst_Id) ;
									}
									//连接到的端子
									if(!g_cSystem.GetXdata(pEnt_2 , _T("ID") , nSecond_Id))
									{
										nSecond_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt_2 , _T("ID") , nSecond_Id) ;
									}
									//连接电缆类型（若有）
									long nCon_Cable_Id = 0 ;
									if(pEnt_3 != NULL)
									{
										if(!g_cSystem.GetXdata(pEnt_3 , _T("ID") , nCon_Cable_Id))
										{
											nCon_Cable_Id = nMaxId+1 ;
											nMaxId++ ;
											g_cSystem.SetXdata(pEnt_3 , _T("ID") , nCon_Cable_Id) ;
										}
									}

									//关联端子
									if(SetEntConnect(pEnt_1, nSecond_Id, nCon_Cable_Id,1))
									{
										SetEntConnect(pEnt_2, nFirst_Id, nCon_Cable_Id,1) ;
									}

									if(pEnt_3 != NULL)
									{
										pEnt_3->close() ;
									}

									pEnt_2->close() ;
								}

								if (iSecHandleSum>1)
								{
									acutPrintf(_T("\n!!!!!!!!!!AppName为 MY_SN，值为%s的记录不止一条，请注意！"), strFirstSn) ;
								}								
							}

						}
					}
					//end first_sn

					//begin 处理second_sn 
					CString strSecondSn ;
					if (g_cSystem.GetXdata(pEnt_1, _T("SECOND_SN"), strSecondSn))
					{
						//查看second_sn是否已经有对应的con_2_id,有则不用处理了
						int iCon_2_Id = 0 ;
						if (!g_cSystem.GetXdata(pEnt_1, _T("CON_2_ID"), iCon_2_Id))
						{
							//先找出对方端子
							UINT iOtherHandle = 0 ;
							int iOtherHandleSum = this->GetTie(_T("MY_SN"), strSecondSn, iOtherHandle) ;
							if (iOtherHandleSum>0)
							{
								//先看本端子是否有关联电缆类型
								CString strSecondConSn = _T("") ;
								UINT iConTypeHandle = 0 ;								
								if(g_cSystem.GetXdata(pEnt_1, _T("SECOND_CON_SN"), strSecondConSn))
								{
									int iConTypeHandleSum = this->GetTie(_T("MY_SN"), strSecondConSn, iConTypeHandle) ;
									if (iConTypeHandleSum>1)
									{
										acutPrintf(_T("\n!!!!!!!!!!AppName为 MY_SN，值为%s的记录不止一条，请注意！"), strSecondConSn) ;
									}
								}

								AcDbEntity *pEnt_2 = NULL ; //连接的端子
								AcDbEntity *pEnt_3 = NULL ; //连接的电缆类型
								if(g_cSystem.OpenAcDbEntity(iOtherHandle, pEnt_2))
								{
									//若在第一个端子xdata中没有连接电缆类型sn，试着去第二个端子找找(此处没意义)
									if (iConTypeHandle==0)
									{
										if(g_cSystem.GetXdata(pEnt_2, _T("SECOND_CON_SN"), strSecondConSn))
										{
											int iConTypeHandleSum2 = this->GetTie(_T("MY_SN"), strSecondConSn, iConTypeHandle) ;
											if (iConTypeHandleSum2>1)
											{
												acutPrintf(_T("\n!!!!!!!!!!AppName为 MY_SN，值为%s的记录不止一条，请注意！"), strSecondConSn) ;
											}
										}
									}
									if (iConTypeHandle!=0)
									{
										g_cSystem.OpenAcDbEntity(iConTypeHandle, pEnt_3) ;
									}

									//添加ID
									long nFirst_Id = 0 , nSecond_Id = 0 ;
									//本端子
									if(!g_cSystem.GetXdata(pEnt_1 , _T("ID") , nFirst_Id))
									{
										nFirst_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt_1 , _T("ID") , nFirst_Id) ;
									}
									//连接到的端子
									if(!g_cSystem.GetXdata(pEnt_2 , _T("ID") , nSecond_Id))
									{
										nSecond_Id = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEnt_2 , _T("ID") , nSecond_Id) ;
									}
									//连接电缆类型（若有）
									long nCon_Cable_Id = 0 ;
									if(pEnt_3 != NULL)
									{
										if(!g_cSystem.GetXdata(pEnt_3 , _T("ID") , nCon_Cable_Id))
										{
											nCon_Cable_Id = nMaxId+1 ;
											nMaxId++ ;
											g_cSystem.SetXdata(pEnt_3 , _T("ID") , nCon_Cable_Id) ;
										}
									}

									//关联端子
									if(SetEntConnect(pEnt_1, nSecond_Id, nCon_Cable_Id,1))
									{
										SetEntConnect(pEnt_2, nFirst_Id, nCon_Cable_Id,1) ;
									}

									if(pEnt_3 != NULL)
									{
										pEnt_3->close() ;
									}

									pEnt_2->close() ;
								}

								if (iOtherHandleSum>1)
								{
									acutPrintf(_T("\n!!!!!!!!!!AppName为 MY_SN，值为%s的记录不止一条，请注意！"), strSecondSn) ;
								}								
							}

						}
					}
					//end 处理second_sn 

					//begin 处理par_sn
					CString strFirstConSn ;
					if (g_cSystem.GetXdata(pEnt_1, _T("PAR_SN"), strFirstConSn)) 
					{
						long iParID = 0 ; 
						if (!g_cSystem.GetXdata(pEnt_1, _T("PARENT_ID"), iParID))
						{
							//先找出对方端子
							UINT iParHandle = 0 ;
							int iParHandleSum = this->GetTie(_T("MY_SN"), strFirstConSn, iParHandle) ;
							if (iParHandleSum>0)
							{
								AcDbEntity *pEntPar = NULL ; //位置								
								if(g_cSystem.OpenAcDbEntity(iParHandle, pEntPar))
								{
									long iID = 0 ;
									//父端子已有ID
									if (g_cSystem.GetXdata(pEntPar, _T("ID"), iID))
									{
										g_cSystem.SetXdata(pEnt_1, _T("PARENT_ID"), iID) ;
									}
									else
									{
										iParID = nMaxId+1 ;
										nMaxId++ ;
										g_cSystem.SetXdata(pEntPar, _T("ID"), iParID) ;
										g_cSystem.SetXdata(pEntPar, _T("IS_PARENT"), 1) ;
										g_cSystem.SetXdata(pEnt_1, _T("PARENT_ID"), iParID) ;
									}

									pEntPar->close() ;
								}
							}

						}
					}
					//end 处理par_sn


					//HASTRANS设为1，表示本端子处理过了
					g_cSystem.SetXdata(pEnt_1, _T("HASTRANS"), 1) ;
				}
			}

			pEnt_1->close() ;
		}	

	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}