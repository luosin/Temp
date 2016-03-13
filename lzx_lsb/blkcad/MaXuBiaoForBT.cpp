#include "stdafx.h"
#include "maxubiaoforbt.h"

#include "DBUtility.h"

extern BOOL bFirst ;

CMaXuBiaoForBT::CMaXuBiaoForBT(void)
: m_pCurDb(NULL)
, m_strStation(_T(""))
, m_strStation2(_T(""))
, m_iOCPGuiDaoId(0)
, m_strSavePath(_T(""))
{
	CBlkUtility::GetBasePath(m_strSysBasePath) ;
	CString strMaXu[4] = {_T("R"),_T("RY"),_T("Y"),_T("G")} ;
	CString strMaXuRecForLoopline[3] = {_T("Y2"),_T("YY2"),_T("G")} ;
	CString strMaXuRecForMainline[3] = {_T("Y"),_T("YY"),_T("G")} ;
	CString strListMaXuRecForThrough[1] = {_T("G")} ;
	
	for (int i=0; i<4; i++)
	{
		m_strListMaXu.AddTail(strMaXu[i]) ;
	}
	for (int i2=0; i2<3; i2++)
	{
		m_strListMaXuRecForLoop.AddTail(strMaXuRecForLoopline[i2]) ;
	}
	for (int i3=0; i3<3; i3++)
	{
		m_strListMaXuRecForMain.AddTail(strMaXuRecForMainline[i3]) ;
	}
	m_strListMaXuRecForThrough.AddTail(strListMaXuRecForThrough[0]) ;

}

CMaXuBiaoForBT::~CMaXuBiaoForBT(void)
{
}

// 导入用以绘制码序表示意图的轨道等信息表格
int CMaXuBiaoForBT::ImportGdTable(const CString strXlsFileName)
{
	int iGdNum = 0 ;


	_ApplicationPtr m_pExcel = NULL ;
	WorkbooksPtr pWorkBooks = NULL ;
	SheetsPtr pWorksheets = NULL ;
	RangePtr pRange = NULL ;
	_WorksheetPtr pSheet = NULL ;

	CString strExec, strMsg ;

	//清除old数据
	CDBUtility::ClearTable(_T("bt_mxb_import")) ;
	CUserSystem mysys ;

	try
	{
		if( bFirst)
		{
			::AfxOleInit() ;
			bFirst = FALSE ;
		}
		HRESULT hr = m_pExcel.CreateInstance (_T("Excel.Application")  ) ;
		if(FAILED(hr))
		{
			AfxMessageBox( _T("初始化Excel失败,可能是没有安装Excel") ) ;
			return -1 ;
		}

		pWorkBooks = m_pExcel->GetWorkbooks () ;

		pWorkBooks->Open( (_bstr_t)strXlsFileName ) ;

		VARIANT vt_index ;
		vt_index.vt = VT_I2 ;
		vt_index.iVal = 1 ;

		pWorksheets  = m_pExcel->GetSheets( ) ;
		pSheet = pWorksheets ->GetItem( vt_index ) ;

		CString strCheckCell[2] = {_T("C3"), _T("J3")} ;
		BOOL bCheckOk = TRUE ;
		_variant_t range_var ;
		for (int iCheckIndex=0; iCheckIndex<2; iCheckIndex++)
		{
			pRange = NULL ;
			pRange = pSheet->GetRange((_variant_t)strCheckCell[iCheckIndex]) ;
			range_var = pRange->GetText() ;
			pRange = NULL ;
			CString strVar ;
			strVar = (CString)range_var.bstrVal ;
			strVar.TrimLeft() ;
			strVar.TrimRight() ;
			if (strVar!=_T("轨道名称"))
			{
				bCheckOk = FALSE ;
				break ;
			}
		}
		if (bCheckOk)
		{
			CString strColName[2][6] = {{_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G")},	{_T("I"),_T("J"),_T("K"),_T("L"),_T("M"),_T("N")}} ;

			for (int i=0; i<2; i++)
			{
				CString strXhjName = _T(""), strGdName = _T(""), strBjLoc = _T(""), strDkLoc = _T(""), strFreq = _T(""), strXhjType = _T("") ;
				long iBjLoc = 0 ;
				long iDkLoc = 0 ;
				int iUpOrDown = 0 ;

				iUpOrDown = i ;			

				int iCurRow = 4 ;
				BOOL bNext = TRUE ;
				while (bNext)
				{
					for (int iColIndex=0; iColIndex<6; iColIndex++)
					{
						CString strCellName = _T(""), strCellText = _T("") ;
						strCellName.Format(_T("%s%d"), strColName[i][iColIndex], iCurRow) ;						
						pRange = NULL ;
						pRange = pSheet->GetRange((_variant_t)strCellName) ;
						strCellText = (CString)(pRange->GetText().bstrVal) ;
						strCellText.TrimLeft() ;
						strCellText.TrimRight() ;
						acutPrintf(_T("\n%s-%s"), strCellName, strCellText) ;

						if (iColIndex==1)
						{
							if (strCellText==_T(""))
							{
								bNext = FALSE ;
								break ;
							}
						}

						switch (iColIndex)
						{
						case 0:
							{
								strXhjName = strCellText ;
							}						
							break;
						case 1:
							{
								strGdName = strCellText ;
							}
							break;
						case 2:
							{
								strBjLoc = strCellText ;
							}
							break;
						case 3:
							{
								strDkLoc = strCellText ;
							}
						case 4:
							{
								strFreq = strCellText ;
							}
							break;
						case 5:
							{
								strXhjType = strCellText ;
							}
							break;

						}
					}
					if (bNext)
					{

						strExec.Format(_T("insert into bt_mxb_import(bt_mxb_import_guidao, bt_mxb_import_xhj, bt_mxb_import_bj, bt_mxb_import_lx, bt_mxb_import_upordown, bt_mxb_import_freq, bt_mxb_import_xhjtype)\
										  values('%s','%s','%s','%s', %d, '%s', '%s')"), strGdName, strXhjName, strBjLoc, strDkLoc,  iUpOrDown, strFreq, strXhjType) ;
						ExecSqlString(strExec) ;					
					}
					iCurRow++ ;
				}

			}


		}

		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!"));		
		pWorkBooks->Close() ;
		pWorkBooks = NULL ;
		m_pExcel->Quit() ;
		m_pExcel = NULL ;		
	}

	return 0;
}

// 绘制巴铁码序表平面图
void CMaXuBiaoForBT::DrawBtMxbPmt(void)
{
	AcDbDatabase *pCurDb = acdbCurDwg() ;
	m_pCurDb = pCurDb ;
	CUserSystem mysys ;
	mysys.m_pDb = pCurDb ;

	AcDbObjectId objIdTextStyle ;

	objIdTextStyle = mysys.CreateTextStyle(_T("CHINESE"),_T("Times New Roman"), _T(""), 2, 0.7) ;
	pCurDb->setTextstyle(objIdTextStyle) ;

	double dBetweenUpOrDown = 12.000 ;
	double dBetweenGd = 18.000 ;

	try
	{
		CADORecordset cSet ;
		CString strSql ;
		CString strOrder[2] = {_T("asc"),_T("desc")} ;

		for (int i=0; i<2; i++)
		{
			//
			strSql.Format(_T("SELECT * FROM bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), i, strOrder[i]) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if(cSet.Open(g_PtrCon, strSql))
			{
				int iGdSum = cSet.GetRecordCount() ;
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}

				int iPlus = i==0?1:-1 ;//上下行X对应不同的增量（下行增加，上行减少）
				int iCurGdIndex = 0 ;
				double dStartX = i==0?0:dBetweenGd*iGdSum ;
				double dGdY = iPlus*dBetweenUpOrDown/2 ;
				double dEndX = i==1?0:dBetweenGd*iGdSum ;

				//绘制2条轨道直线
				AcGePoint3d pt_3d_start(dStartX, dGdY,0), pt_3d_end(dEndX, dGdY,0) ;
				mysys.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.4) ;

				CString strBlockFile ;				

				BOOL bPassAss = FALSE ;
				while (!cSet.IsEOF())
				{
					double dCurStartX = dStartX+iPlus*iCurGdIndex*dBetweenGd ;  //当前区段起始点

					CString strGdName, strXhjName, strBjLoc, strDkLoc ;
					cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strGdName) ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjName) ;
					cSet.GetFieldValue(_T("bt_mxb_import_bj"), strBjLoc) ;
					cSet.GetFieldValue(_T("bt_mxb_import_lx"), strDkLoc) ;
					CString strJyjName = _T("bt_mxb_jxjyj") ;
					if (!bPassAss)
					{
						if (strXhjName.Left(3)==_T("ASS"))
						{
							bPassAss = TRUE ;
						}
					}
					else if (strXhjName.Left(3)!=_T("OHS"))
					{
						strJyjName = _T("bt_mxb_dqjyj") ;
					}
					AcDbBlockReference *pRef = NULL ;
					CStringArray strArrayAtt ;
					AcGePoint3d pt_3d_insert(dCurStartX, dGdY, 0) ;

					//绝缘节
					strBlockFile.Format(_T("%s\\support\\bt\\%s.dwg"), m_strSysBasePath, strJyjName) ;
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
					{
						pRef->close() ;
						pRef = NULL ;
					}
					//最后一个轨道绘制末端绝缘节
					if (iGdSum==(iCurGdIndex+1))
					{
						pt_3d_insert.x = dCurStartX+dBetweenGd*iPlus ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
						{
							pRef->close() ;
							pRef = NULL ;
						}	
					}

					//轨道名称
					pt_3d_insert.x = dCurStartX+iPlus*6.3 ;
					strBlockFile.Format(_T("bt_mxb_gd_%s_0"), i==0?_T("down"):_T("up") ) ;
					strArrayAtt.Add(strGdName) ;
					strArrayAtt.Add(_T("")) ;
					if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					//信号机
					if (strXhjName!=_T(""))
					{
						pt_3d_insert.x = dCurStartX+(-1)*iPlus ;
						if (strXhjName.Left(3)==_T("ASS"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_ass_%s_0"), i==0?_T("down"):_T("up") ) ;
						}
						else if (strXhjName.Left(3)==_T("OHS"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_ohs_%s_0"), i==0?_T("down"):_T("up")) ;
						}
						else if (strXhjName.Left(2)==_T("AD"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a_%s_0"), i==0?_T("down"):_T("up")) ;
						}
						else if (strXhjName.Left(2)==_T("AD"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a_down_0")) ;
						}
						else if (strXhjName.Left(2)==_T("AU"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a_up_0")) ;
						}
						else if (strXhjName.Left(3)==_T("LXD"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a-lx_down_0")) ;
						}
						else if (strXhjName.Left(3)==_T("LXU"))
						{
							strBlockFile.Format(_T("bt_mxb_xhj_a-lx_up_0")) ;
						}
						strArrayAtt.RemoveAll() ;
						strArrayAtt.Add(strXhjName) ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}
					}

					//报警
					if (strBjLoc!=_T(""))
					{
						strArrayAtt.RemoveAll() ;
						CBlkUtility::DivideString(strBjLoc, _T(","), strArrayAtt) ;
						int iLxNum = strArrayAtt.GetCount() ;						
						strBlockFile.Format(_T("bt_mxb_bjq_%s_%d"), i==0?_T("down"):_T("up"), iLxNum) ;
						AcDbObjectId objIdBjq ;
						if (!mysys.HaveBlockName(strBlockFile, objIdBjq))
						{
							this->CreateBjqBlock(iLxNum, i) ;
						}
						pt_3d_insert.x = dCurStartX ;
						pt_3d_insert.y = i==0? dGdY+12:dGdY-12 ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}


					}

					//道口
					if (strDkLoc!=_T(""))
					{
						pt_3d_insert.x = dCurStartX+14*iPlus ;
						pt_3d_insert.y = 0 ;
						strArrayAtt.RemoveAll() ;
						strArrayAtt.Add(strDkLoc) ;
						strBlockFile = _T("bt_mxb_lx_0") ;
						if(mysys.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}

					}


					iCurGdIndex++ ;

					cSet.MoveNext() ;
				}


				cSet.Close() ;

			}


		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawBtMxbPmt")) ;
	} 
}

// 根据输入的参数创建报警器的块
BOOL CMaXuBiaoForBT::CreateBjqBlock(int iLxNum, int iUpOrDown)
{
	// 获得当前图形数据库的块表
	AcDbBlockTable *pBlkTbl;
	g_pCurDb->getBlockTable(pBlkTbl, AcDb::kForWrite);
	// 创建新的块表记录
	AcDbBlockTableRecord *pBlkTblRec;
	pBlkTblRec = new AcDbBlockTableRecord();
	CString strBlkName ;
	strBlkName.Format(_T("bt_mxb_bjq_%s_%d"), iUpOrDown==0?_T("down"):_T("up"), iLxNum) ;
	pBlkTblRec->setName(strBlkName);
	// 将块表记录添加到块表中
	AcDbObjectId blkDefId;
	pBlkTbl->add(blkDefId, pBlkTblRec);
	pBlkTbl->close();

	// 向块表记录中添加实体
	AcDbObjectId entId;
	AcGePoint3d pt_3d_ent(-13, 0, 0) ;
	for (int i=0; i<iLxNum; i++)
	{
		if (iUpOrDown==0)
		{
			pt_3d_ent.x = -13 ;
			pt_3d_ent.y = (iLxNum-i-1)*2+0.25 ;
		}
		else
		{
			pt_3d_ent.x = 0.7 ;
			pt_3d_ent.y = 0-(i+1)*2+0.25-1-2*2.5 ;
		}
		CString strAtt ;
		strAtt.Format(_T("LX%d"),i+1) ;
		AcDbAttributeDefinition *pAttDef = new AcDbAttributeDefinition(pt_3d_ent, strAtt, strAtt, strAtt);
		pAttDef->setHeight(1.5) ;
		pAttDef->setWidthFactor(0.7) ;
		pBlkTblRec->appendAcDbEntity(entId, pAttDef);
		pAttDef->close();
	}
	CString strText[2] = {_T("ANNUNCIATOR"), _T("APPROACH")} ;
	for (int i2=0; i2<2; i2++)
	{
		if (iUpOrDown==0)
		{
			pt_3d_ent.x = -13 ;
			pt_3d_ent.y = 2*iLxNum+i2*2.5+0.45 ;
		}
		else
		{
			pt_3d_ent.x = 0.7 ;
			pt_3d_ent.y = 0-1-(i2+1)*2.5+0.45 ;
		}
		AcDbText *pText = new AcDbText(pt_3d_ent, strText[i2]) ;
		pText->setHeight(1.6) ;
		pText->setWidthFactor(0.7) ;
		pBlkTblRec->appendAcDbEntity(entId, pText);
		pText->close() ;
	}

	AcGePoint3d pt_3d_solid_0(0,2*iLxNum+2*2.5+0.5,0), pt_3d_solid_1(-3,2*iLxNum+2*2.5,0), pt_3d_solid_2(-3,2*iLxNum+2*2.5+1,0) ;
	if (iUpOrDown==1)
	{
		pt_3d_solid_1.x = pt_3d_solid_2.x = 3 ;
		pt_3d_solid_0.y = -0.5 ;
		pt_3d_solid_1.y = -1 ;
		pt_3d_solid_2.y = 0 ;
	}
	AcDbSolid *pSolid = new AcDbSolid(pt_3d_solid_0, pt_3d_solid_1, pt_3d_solid_2) ;

	AcGePoint3d pt_3d_start(-13, pt_3d_solid_0.y, 0), pt_3d_end(0, pt_3d_solid_0.y, 0);
	if (iUpOrDown==1)
	{
		pt_3d_start.x = 13 ;
		pt_3d_start.y = pt_3d_end.y = -0.5 ;
	}
	AcDbLine *pLine1 = new AcDbLine(pt_3d_start, pt_3d_end); // 创建一条直线
	pt_3d_start.set(0, 0, 0);
	pt_3d_end.set(0, pt_3d_solid_2.y, 0);
	if (iUpOrDown==1)
	{
		pt_3d_end.set(0, 0-1-2*2.5-iLxNum*2, 0);
	}
	AcDbLine *pLine2 = new AcDbLine(pt_3d_start, pt_3d_end); // 创建一条直线

	pBlkTblRec->appendAcDbEntity(entId, pSolid);
	pBlkTblRec->appendAcDbEntity(entId, pLine1);
	pBlkTblRec->appendAcDbEntity(entId, pLine2);

	// 关闭实体和块表记录
	pSolid->close();
	pLine1->close();
	pLine2->close();	
	pBlkTblRec->close();

	return 0;
}

// 在数据库中填充bt_mxb_import_belongxhj字段
int CMaXuBiaoForBT::SetBelongXhj(void)
{
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		CString strOrder[2] = {_T("asc"),_T("desc")} ;

		for (int i=0; i<2; i++)
		{		
			strSql.Format(_T("SELECT * FROM bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), i, strOrder[i]) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if(cSet.Open(g_PtrCon, strSql))
			{
				int iGdSum = cSet.GetRecordCount() ;
				if (!cSet.IsBOF())
				{
					cSet.MoveFirst() ;
				}

				CString strPreXhj = _T("") ;				

				
				while (!cSet.IsEOF())
				{
					CString strGdName, strXhjName, strBjLoc, strDkLoc ;
					cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strGdName) ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjName) ;
					cSet.GetFieldValue(_T("bt_mxb_import_bj"), strBjLoc) ;
					cSet.GetFieldValue(_T("bt_mxb_import_lx"), strDkLoc) ;

					if (strXhjName!=_T(""))
					{
						strPreXhj = strXhjName ;
					}

					cSet.Edit() ;
					cSet.SetFieldValue(_T("bt_mxb_import_belongxhj"), strPreXhj) ;
					cSet.Update() ;
					
					cSet.MoveNext() ;
				}

				cSet.Close() ;

			}

		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in SetLxBelongSection")) ;
	} 


	return 0;
}

// 区间编码
void CMaXuBiaoForBT::JinLuBianMa_QJ(int iUpOrDown)
{	
	CArray<CStringArray *, CStringArray *> arrayBianMa, arrayBianMa_1, arrayBianMa_2, arrayBianMa_3, arrayBianMa2 ;
	CStringArray strArrayDescribe, strArrayDescribe_1, strArrayDescribe_2, strArrayDescribe_3, strArrayDescribe2 ;
	CStringArray strArrayOCPGuiDao, strArrayOCPGuiDao_1, strArrayOCPGuiDao_2, strArrayOCPGuiDao_3, strArrayOCPGuiDao2 ;

	CString strRetBianMa = _T("") ;	

	try
	{
		CADORecordset cSet ;
		CString strSql ;

		int iGuiDaoIdLeftStart = 0 ; //剩余轨道的开始 id（处理完被报警的区段后剩余的区段）
		int iGuiDaoIdLeftEnd = 0 ; //剩余轨道的结束 id

		//清除basecode 和code字段
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_basecode='', bt_mxb_import_code='' where id>0")) ;
		ExecSqlString(strSql) ;
		

		//begin no route is set

		//设置道口状态 bt_mxb_import_lxstatus=0 表示道口不能行车（能走人）
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=0 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d"), iUpOrDown) ;
		ExecSqlString(strSql) ;

		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveLast() ;
				cSet.GetFieldValue(_T("id"),iGuiDaoIdLeftStart) ;
				cSet.MoveFirst() ;
				cSet.GetFieldValue(_T("id"),iGuiDaoIdLeftEnd) ;
			}
			strRetBianMa = _T("") ;
			while(!cSet.IsEOF())
			{
				int iGuiDaoId ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;
				this->GetGuiDaoBianMa_QJ(iGuiDaoId, strRetBianMa, 0, iUpOrDown) ;
				cSet.Edit() ;
				cSet.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMa) ;
				cSet.SetFieldValue(_T("bt_mxb_import_basecode"), strRetBianMa) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		
		
		//将码从数据库拷入数组中
		CStringArray *strArrayBianMa = new CStringArray ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				CString strBianMaTmp ;				
				cSet.GetFieldValue(_T("bt_mxb_import_basecode"), strBianMaTmp) ;
				//ASS之前的轨道（站内的）发NC(R)码
				if (iUpOrDown==0)
				{
					if (iCodeIndex==0)
					{
						strBianMaTmp = _T("NC(R)") ;
					}					
				}
				else
				{
					if (iCodeIndex==iRsCount-1)
					{
						strBianMaTmp = _T("NC(R)") ;
					}
				}
				strArrayBianMa->Add(strBianMaTmp) ;
				
				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		arrayBianMa.Add(strArrayBianMa) ;
		strArrayDescribe.Add(_T("No Route is Set")) ;
		strArrayOCPGuiDao.Add(_T("")) ;

		//end no route is set

		//begin 左侧 set dispatch route， 右侧 no route
		//begin 查找所有站内通知的道口（只能通过排除法：没有approach的道口均视为站内通知过的）
		//CStringArray strArrayLxZnBj ;
		CString strLxZnBj = _T("") ;
		CString strAllBj = _T("") ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_bj<>''"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			while(!cSet.IsEOF())
			{
				CString strBjTmp ;
				cSet.GetFieldValue(_T("bt_mxb_import_bj"), strBjTmp) ;
				if (strAllBj!=_T(""))
				{
					strAllBj.Append(_T(";")) ;							
				}
				strAllBj.Append(strBjTmp) ;

				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_lx<>''"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iFindCount = cSet.GetRecordCount() ;
			if (iFindCount>0)
			{
				cSet.MoveFirst() ;
				while(!cSet.IsEOF())
				{
					CString strLxTmp ;
					cSet.GetFieldValue(_T("bt_mxb_import_lx"), strLxTmp) ;
					if (strAllBj.Find(strLxTmp)==-1)
					{
						if (strLxZnBj!=_T(""))
						{
							strLxZnBj.Append(_T(",")) ;							
						}
						strLxZnBj.Append(strLxTmp) ;
					}

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		if (strLxZnBj!=_T(""))
		{
			strLxZnBj.Replace(_T(","), _T("','")) ;
			strLxZnBj.Insert(0, _T("'")) ;
			strLxZnBj.Append(_T("'")) ;
		}

		//end 查找所有站内通知的道口（只能通过排除法：没有approach的道口均视为站内通知过的）

		//遍历所有的轨道被占用的情况
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;	
		//strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id=10"), iUpOrDown) ;	//for test
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iRsCount = cSet.GetRecordCount() ;
			int iOCPIndex = 0 ;
			while(!cSet.IsEOF())
			{
				if (iOCPIndex==iRsCount-1)
				{
					break ; //最后一个占用情况不要
				}
				//////////////////////////////////////////////////////////////////////////
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iGuiDaoIdOCP = 0 ;
				CString strBelongXhjOCP ;
				CString strCurGuiDao ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_belongxhj"), strBelongXhjOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurGuiDao) ;

				iGuiDaoIdLeftStart = iGuiDaoIdOCP ;

				//////////////////////////////////////////////////////////////////////////
				
				this->GetGuiDaoBianMaOCP(iGuiDaoIdOCP, strBelongXhjOCP, strLxZnBj, iGuiDaoIdLeftStart, iGuiDaoIdLeftEnd, 0, iUpOrDown) ;

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMaTmp = new CStringArray ;
				CADORecordset cSetTmp ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					int iRsCount = cSetTmp.GetRecordCount() ;
					if (!cSetTmp.IsBOF())
					{
						cSetTmp.MoveFirst() ;
					}
					int iCodeIndex = 0 ;
					while(!cSetTmp.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;	
						//车越过ASS后，ASS后的区段发NC(R)码，车越过ASS防护的第一个轨道，则该轨道发R码
						if (iUpOrDown==0)
						{
							if (iOCPIndex>0)
							{
								if (iCodeIndex==0)
								{
									strBianMaTmp = _T("NC(R)") ;
								}
								if (iOCPIndex>1)
								{
									if (iCodeIndex==1)
									{
										strBianMaTmp = _T("R") ;
									}
								}

							}
						}
						else
						{
							if (iOCPIndex>0)
							{
								if (iCodeIndex==iRsCount-1)
								{
									strBianMaTmp = _T("NC(R)") ;
								}
								if (iOCPIndex>1)
								{
									if (iCodeIndex==iRsCount-2)
									{
										strBianMaTmp = _T("R") ;
									}
								}

							}
						}


						strArrayBianMaTmp->Add(strBianMaTmp) ;

						iCodeIndex++ ;
						cSetTmp.MoveNext() ;
					}
					cSetTmp.Close() ;
				}
				arrayBianMa.Add(strArrayBianMaTmp) ;
				CString strCurDescribe ;
				strCurDescribe.Format(_T("%s:Set Dispatch Route\n%s:No Route"), m_strStation, m_strStation2) ;
				strArrayDescribe.Add(strCurDescribe) ;
				strArrayOCPGuiDao.Add(strCurGuiDao) ;

				iOCPIndex++ ;
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 左侧 set dispatch route， 右侧 no route

		//begin 左侧 set Reception route For Loopline(接车进侧线)
		//遍历轨道被占用的情况
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj in (select top 3 bt_mxb_import_xhj from bt_mxb_import where bt_mxb_import_upordown=%d and  bt_mxb_import_xhj<>'' order by id %s) order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc"), iUpOrDown==0?_T("asc"):_T("desc")) ;	
		//strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id=10"), iUpOrDown) ;	//for test
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			while(!cSet.IsEOF())
			{
				//////////////////////////////////////////////////////////////////////////
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iGuiDaoIdOCP = 0 ;
				CString strBelongXhjOCP ;
				CString strCurGuiDao ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_belongxhj"), strBelongXhjOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurGuiDao) ;

				iGuiDaoIdLeftStart = iGuiDaoIdOCP ;

				//////////////////////////////////////////////////////////////////////////

				this->GetGuiDaoBianMaOCP(iGuiDaoIdOCP, strBelongXhjOCP, strLxZnBj, iGuiDaoIdLeftStart, iGuiDaoIdLeftEnd, 1, iUpOrDown) ;

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMaTmp = new CStringArray ;
				CADORecordset cSetTmp ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					int iRsCount = cSetTmp.GetRecordCount() ;
					if (!cSetTmp.IsBOF())
					{
						cSetTmp.MoveFirst() ;
					}
					int iCodeIndex = 0 ;
					while(!cSetTmp.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;	

						//车越过ASS后，ASS后的区段发NC(R)码，车越过ASS防护的第一个轨道，则该轨道发R码
						if (iUpOrDown==0)
						{
							if (iCodeIndex==0)
							{
								strBianMaTmp = _T("NC(R)") ;
							}
							if (iCodeIndex==1)
							{
								strBianMaTmp = _T("R") ;
							}
						}
						else
						{
							if (iCodeIndex==iRsCount-1)
							{
								strBianMaTmp = _T("NC(R)") ;
							}
							if (iCodeIndex==iRsCount-2)
							{
								strBianMaTmp = _T("R") ;
							}
						}

						strArrayBianMaTmp->Add(strBianMaTmp) ;

						iCodeIndex++ ;
						cSetTmp.MoveNext() ;
					}
					cSetTmp.Close() ;
				}
				arrayBianMa_1.Add(strArrayBianMaTmp) ;
				CString strCurDescribe ;
				strCurDescribe.Format(_T("%s:Set Reception Route\nFor Loopline"), m_strStation) ;
				strArrayDescribe_1.Add(strCurDescribe) ;
				strArrayOCPGuiDao_1.Add(strCurGuiDao) ;
				
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 左侧 set Reception route For Loopline(接车进侧线)
		
		//begin 左侧 set Reception route For Mainline(接车进正线)
		//遍历轨道被占用的情况
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj in (select top 2 bt_mxb_import_xhj from bt_mxb_import where bt_mxb_import_upordown=%d and  bt_mxb_import_xhj<>'' order by id %s) order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc"), iUpOrDown==0?_T("asc"):_T("desc")) ;	
		//strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id=10"), iUpOrDown) ;	//for test
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			
			while(!cSet.IsEOF())
			{
				//////////////////////////////////////////////////////////////////////////
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iGuiDaoIdOCP = 0 ;
				CString strBelongXhjOCP ;
				CString strCurGuiDao ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_belongxhj"), strBelongXhjOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurGuiDao) ;

				iGuiDaoIdLeftStart = iGuiDaoIdOCP ;

				//////////////////////////////////////////////////////////////////////////

				this->GetGuiDaoBianMaOCP(iGuiDaoIdOCP, strBelongXhjOCP, strLxZnBj, iGuiDaoIdLeftStart, iGuiDaoIdLeftEnd, 2, iUpOrDown) ;

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMaTmp = new CStringArray ;
				CADORecordset cSetTmp ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					int iRsCount = cSetTmp.GetRecordCount() ;
					if (!cSetTmp.IsBOF())
					{
						cSetTmp.MoveFirst() ;
					}
					int iCodeIndex = 0 ;
					while(!cSetTmp.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;

						//车越过ASS后，ASS后的区段发NC(R)码，车越过ASS防护的第一个轨道，则该轨道发R码
						if (iUpOrDown==0)
						{
							if (iCodeIndex==0)
							{
								strBianMaTmp = _T("NC(R)") ;
							}
							if (iCodeIndex==1)
							{
								strBianMaTmp = _T("R") ;
							}
						}
						else
						{
							if (iCodeIndex==iRsCount-1)
							{
								strBianMaTmp = _T("NC(R)") ;
							}
							if (iCodeIndex==iRsCount-2)
							{
								strBianMaTmp = _T("R") ;
							}
						}

						strArrayBianMaTmp->Add(strBianMaTmp) ;

						iCodeIndex++ ;
						cSetTmp.MoveNext() ;
					}
					cSetTmp.Close() ;
				}
				arrayBianMa_2.Add(strArrayBianMaTmp) ;
				CString strCurDescribe ;
				strCurDescribe.Format(_T("%s:Set Reception Route\nFor Mainline"), m_strStation) ;
				strArrayDescribe_2.Add(strCurDescribe) ;
				strArrayOCPGuiDao_2.Add(strCurGuiDao) ;
				
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 左侧 set Reception route For Mainline(接车进正线)
		
		//begin 左侧 set Run Through Route(通过)
		//遍历轨道被占用的情况
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj in (select top 2 bt_mxb_import_xhj from bt_mxb_import where bt_mxb_import_upordown=%d and  bt_mxb_import_xhj<>'' order by id %s) order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc"), iUpOrDown==0?_T("asc"):_T("desc")) ;	
		//strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id=10"), iUpOrDown) ;	//for test
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}

			while(!cSet.IsEOF())
			{
				//////////////////////////////////////////////////////////////////////////
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iGuiDaoIdOCP = 0 ;
				CString strBelongXhjOCP ;
				CString strCurGuiDao ;
				cSet.GetFieldValue(_T("id"), iGuiDaoIdOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_belongxhj"), strBelongXhjOCP) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurGuiDao) ;

				iGuiDaoIdLeftStart = iGuiDaoIdOCP ;

				//////////////////////////////////////////////////////////////////////////

				this->GetGuiDaoBianMaOCP(iGuiDaoIdOCP, strBelongXhjOCP, strLxZnBj, iGuiDaoIdLeftStart, iGuiDaoIdLeftEnd, 3, iUpOrDown) ;

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMaTmp = new CStringArray ;
				CADORecordset cSetTmp ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					int iRsCount = cSetTmp.GetRecordCount() ;
					if (!cSetTmp.IsBOF())
					{
						cSetTmp.MoveFirst() ;
					}
					int iCodeIndex = 0 ;
					while(!cSetTmp.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;	

						//车越过ASS后，ASS后的区段发NC(R)码，车越过ASS防护的第一个轨道，则该轨道发R码
						if (iUpOrDown==0)
						{
							if (iCodeIndex==0)
							{
								strBianMaTmp = _T("NC(R)") ;
							}
							if (iCodeIndex==1)
							{
								strBianMaTmp = _T("R") ;
							}
						}
						else
						{
							if (iCodeIndex==iRsCount-1)
							{
								strBianMaTmp = _T("NC(R)") ;
							}
							if (iCodeIndex==iRsCount-2)
							{
								strBianMaTmp = _T("R") ;
							}
						}

						strArrayBianMaTmp->Add(strBianMaTmp) ;

						iCodeIndex++ ;
						cSetTmp.MoveNext() ;
					}
					cSetTmp.Close() ;
				}
				arrayBianMa_3.Add(strArrayBianMaTmp) ;
				CString strCurDescribe ;
				strCurDescribe.Format(_T("%s:Set Run Through Route"), m_strStation) ;
				strArrayDescribe_3.Add(strCurDescribe) ;
				strArrayOCPGuiDao_3.Add(strCurGuiDao) ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 左侧 set Run Through Route(通过)


		g_pCurDb = new AcDbDatabase ;
		g_cSystem.m_pDb = g_pCurDb ;
		CArray<CStringArray *, CStringArray *> arrayBianMaTmp ;
		CStringArray strArrayOCPGuiDaoTmp, strArrayDescribeTmp ;		
		arrayBianMaTmp.Append(arrayBianMa) ;
		arrayBianMaTmp.Append(arrayBianMa_1) ;
		arrayBianMaTmp.Append(arrayBianMa_2) ;
		arrayBianMaTmp.Append(arrayBianMa_3) ;
		strArrayOCPGuiDaoTmp.Append(strArrayOCPGuiDao) ;
		strArrayOCPGuiDaoTmp.Append(strArrayOCPGuiDao_1) ;
		strArrayOCPGuiDaoTmp.Append(strArrayOCPGuiDao_2) ;
		strArrayOCPGuiDaoTmp.Append(strArrayOCPGuiDao_3) ;
		strArrayDescribeTmp.Append(strArrayDescribe) ;
		strArrayDescribeTmp.Append(strArrayDescribe_1) ;
		strArrayDescribeTmp.Append(strArrayDescribe_2) ;
		strArrayDescribeTmp.Append(strArrayDescribe_3) ;
		CString strFileName ;
		strFileName.Format(_T("%s\\%s-%s-%s-normal.dwg"), m_strSavePath, m_strStation, m_strStation2, iUpOrDown==0?_T("down"):_T("up")) ;
		//绘制码序表
		this->DrawBianMaTable(arrayBianMaTmp, strArrayOCPGuiDaoTmp, strArrayDescribeTmp,iUpOrDown) ;	
		g_pCurDb->saveAs(strFileName) ;
		delete g_pCurDb ;
		g_pCurDb = NULL ;


		//begin 带限速的情况
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_xhj<>'' and bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iIndex = 0 ;
			int iPreXhjId = 0 ; //记录上次
			//set TSR Between strFrontXhj and strBackXhj
			while(!cSet.IsEOF())
			{
				int iFrontXhjId = 0 ;
				CString strFrontXhj = _T("") ;
				cSet.GetFieldValue(_T("id"), iFrontXhjId) ;
				cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strFrontXhj) ;
				cSet.MoveNext() ;
				if (!cSet.IsEOF())
				{
					int iBackXhjId = 0 ;
					CString strBackXhj = _T("") ;
					cSet.GetFieldValue(_T("id"), iBackXhjId) ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strBackXhj) ;

					//遍历占用区段（限速分区及前一个闭塞分区）
					if (iIndex==0)
					{
						strSql.Format(_T("select * from bt_mxb_import where id%s%d and bt_mxb_import_upordown=%d order by id %s"), iUpOrDown==0?_T("<"):_T(">"), iBackXhjId, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
					}
					else
					{
						strSql.Format(_T("select * from bt_mxb_import where id%s%d  and id %s%d and bt_mxb_import_upordown=%d order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iPreXhjId, iUpOrDown==0?_T("<"):_T(">"), iBackXhjId, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
					}
					CADORecordset cSetOCP ;
					if (cSetOCP.IsOpen())
					{
						cSetOCP.Close() ;
					}
					if (cSetOCP.Open(g_PtrCon, strSql))
					{
						//20120308：here
						if (!cSetOCP.IsBOF())
						{
							cSetOCP.MoveFirst() ;
						}
						while (!cSetOCP.IsEOF())
						{
							int iOCPGuiDaoId = 0 ;
							CString strOCPGuiDao =_T("") ;
							cSetOCP.GetFieldValue(_T("id"), iOCPGuiDaoId) ;
							cSetOCP.GetFieldValue(_T("bt_mxb_import_guidao"), strOCPGuiDao) ;
							int iFind = 0 ;
							iFind = CBlkUtility::FindStrInArray(strArrayOCPGuiDao, strOCPGuiDao) ;
							if (iFind!=-1)
							{
								CStringArray *pstrArrayCurBianMa ;
								pstrArrayCurBianMa = arrayBianMa.GetAt(iFind) ;							

								//将码从数组中导入到数据库，便于修改
								for (int iBianMa=0; iBianMa<pstrArrayCurBianMa->GetCount(); iBianMa++)
								{
									strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='%s' where id=(select top 1 id from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc)+%d"), pstrArrayCurBianMa->GetAt(iBianMa), iUpOrDown, iBianMa) ;
									ExecSqlString(strSql) ;
								}

								//begin 在无限速基础上修改限速区段和前一个区段的编码

								//begin 取得限速区下一个闭塞分区第一个轨道编码
								CString strFirstBianMaNew = _T("") ;
								CStringArray strArrayFirstBianMa ;

								CADORecordset cSetNextSectionBianMa ;
								strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj='%s' order by id %s"), strBackXhj, iUpOrDown==0?_T("asc"):_T("desc")) ;
								if (cSetNextSectionBianMa.IsOpen())
								{
									cSetNextSectionBianMa.Close() ;
								}
								if (cSetNextSectionBianMa.Open(g_PtrCon, strSql))
								{
									if (!cSetNextSectionBianMa.IsBOF())
									{
										cSetNextSectionBianMa.MoveFirst() ;
									}
									//20120309:here
									CString strFirstBianMa = _T("") ; //闭塞分区的第一个轨道编码
									int iNextBianMaIndex = 0 ;
									while(!cSetNextSectionBianMa.IsEOF())
									{									
										CString strBianMaTmp = _T("") ;									
										cSetNextSectionBianMa.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;

										if (iNextBianMaIndex==0)
										{
											strFirstBianMa = strBianMaTmp ;
											if (iIndex==iRsCount-2) //最后一个限速区段时，将OHS后的轨道编码固定认为是G/Y/Y2/R(20120314：张劼，改为R，因为三接近由3个继电器控制发码，只能发8种)
											{
												strFirstBianMaNew = _T("R") ;
												cSetNextSectionBianMa.Edit() ;
												cSetNextSectionBianMa.SetFieldValue(_T("bt_mxb_import_code"), strFirstBianMaNew) ;
												cSetNextSectionBianMa.Update() ;
												CBlkUtility::DivideString(strFirstBianMaNew, _T("/"), strArrayFirstBianMa) ;
												break;
											}
											//处理该编码，如果该编码已经含有R，则不处理，否则，增加R的情况										
											CBlkUtility::DivideString(strBianMaTmp, _T("/"), strArrayFirstBianMa) ;
											if (CBlkUtility::FindStrInArray(strArrayFirstBianMa, _T("R"))==-1)
											{
												strArrayFirstBianMa.Add(_T("R")) ;
												strFirstBianMaNew.Format(_T("%s/R"), strFirstBianMa) ;
												cSetNextSectionBianMa.Edit() ;
												cSetNextSectionBianMa.SetFieldValue(_T("bt_mxb_import_code"), strFirstBianMaNew) ;
												cSetNextSectionBianMa.Update() ;
											}
											else
											{
												strFirstBianMaNew = strFirstBianMa ;
												break;
											}

										}
										else
										{
											if (strBianMaTmp!=strFirstBianMa)
											{
												break;
											}
											else
											{
												cSetNextSectionBianMa.Edit() ;
												cSetNextSectionBianMa.SetFieldValue(_T("bt_mxb_import_code"), strFirstBianMaNew) ;
												cSetNextSectionBianMa.Update() ;
											}										
										}									

										iNextBianMaIndex++ ;
										cSetNextSectionBianMa.MoveNext() ;
									}


									cSetNextSectionBianMa.Close() ;
								}
								//end 取得限速区下一个闭塞分区第一个轨道编码

								CString strXsSectionBianMa = _T("") ;//限速区段的编码
								if (CBlkUtility::FindStrInArray(strArrayFirstBianMa, _T("R"))!=-1)
								{
									strXsSectionBianMa.Append(_T("R40")) ;
								}
								if (CBlkUtility::FindStrInArray(strArrayFirstBianMa, _T("RY"))!=-1 || CBlkUtility::FindStrInArray(strArrayFirstBianMa, _T("Y"))!=-1 || CBlkUtility::FindStrInArray(strArrayFirstBianMa, _T("Y2"))!=-1)
								{
									strXsSectionBianMa.Insert(0, _T("40/")) ;
								}
								if (CBlkUtility::FindStrInArray(strArrayFirstBianMa, _T("G"))!=-1)
								{
									strXsSectionBianMa.Insert(0, _T("40/")) ;
								}

								strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='%s' where id%s%d and id%s%d"), strXsSectionBianMa, iUpOrDown==0?_T(">="):_T("<="), iOCPGuiDaoId, iUpOrDown==0?_T("<"):_T(">"), iBackXhjId) ;
								ExecSqlString(strSql) ;
								strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='A40' where id%s%d and id%s%d"), iUpOrDown==0?_T(">="):_T("<="), iOCPGuiDaoId, iUpOrDown==0?_T("<"):_T(">"), iFrontXhjId) ;
								ExecSqlString(strSql) ;	

								//end 在无限速基础上修改限速区段和前一个区段的编码

								//将码从数据库拷入数组中
								CStringArray *strArrayBianMaTmp2 = new CStringArray ;
								CADORecordset cSetDbToArray ;
								strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
								if (cSetDbToArray.IsOpen())
								{
									cSetDbToArray.Close() ;
								}
								if (cSetDbToArray.Open(g_PtrCon, strSql))
								{
									if (!cSetDbToArray.IsBOF())
									{
										cSetDbToArray.MoveFirst() ;
									}			
									while(!cSetDbToArray.IsEOF())
									{
										CString strBianMaTmp = _T("") ;				
										cSetDbToArray.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;	
										strArrayBianMaTmp2->Add(strBianMaTmp) ;
										cSetDbToArray.MoveNext() ;
									}
									cSetDbToArray.Close() ;
								}
								arrayBianMa2.Add(strArrayBianMaTmp2) ;
								CString strCurDescribe2 ;
								strCurDescribe2.Format(_T("Set TSR Between\n%s and %s"), strFrontXhj, strBackXhj) ;
								strArrayDescribe2.Add(strCurDescribe2) ;
								strArrayOCPGuiDao2.Add(strOCPGuiDao) ;

							}


							cSetOCP.MoveNext() ;
						}

						cSetOCP.Close() ;
					}

                    
					iPreXhjId = iFrontXhjId ;
					iIndex++ ;
				}
			}

			cSet.Close() ;
		}

		//end 带限速的情况

		g_pCurDb = new AcDbDatabase ;
		g_cSystem.m_pDb = g_pCurDb ;
		strFileName.Format(_T("%s\\%s-%s-%s-tsr.dwg"),m_strSavePath, m_strStation, m_strStation2, iUpOrDown==0?_T("down"):_T("up")) ;
		//绘制码序表
		this->DrawBianMaTable(arrayBianMa2, strArrayOCPGuiDao2, strArrayDescribe2, iUpOrDown) ;	
		g_pCurDb->saveAs(strFileName) ;
		delete g_pCurDb ;
		g_pCurDb = NULL ;


		//清理new对象
		if (arrayBianMa.GetSize()>0)
		{
			for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
			{
				delete arrayBianMa[iBianMa] ;
				arrayBianMa[iBianMa] = NULL ;
			}
		}
		if (arrayBianMa_1.GetSize()>0)
		{
			for(int iBianMa=0; iBianMa<arrayBianMa_1.GetSize(); iBianMa++)
			{
				delete arrayBianMa_1[iBianMa] ;
				arrayBianMa_1[iBianMa] = NULL ;
			}
		}
		if (arrayBianMa_2.GetSize()>0)
		{
			for(int iBianMa=0; iBianMa<arrayBianMa_2.GetSize(); iBianMa++)
			{
				delete arrayBianMa_2[iBianMa] ;
				arrayBianMa_2[iBianMa] = NULL ;
			}
		}
		if (arrayBianMa_3.GetSize()>0)
		{
			for(int iBianMa=0; iBianMa<arrayBianMa_3.GetSize(); iBianMa++)
			{
				delete arrayBianMa_3[iBianMa] ;
				arrayBianMa_3[iBianMa] = NULL ;
			}
		}
		
		if (arrayBianMa2.GetSize()>0)
		{
			for(int iBianMa2=0; iBianMa2<arrayBianMa2.GetSize(); iBianMa2++)
			{
				delete arrayBianMa2[iBianMa2] ;
				arrayBianMa2[iBianMa2] = NULL ;
			}
		}


	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-JinLuBianMa_QJ"),sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error JinLuBianMa_QJ")) ;
	}

}

BOOL CMaXuBiaoForBT::GetGuiDaoBianMa_QJ(int iGuiDaoId, CString & strRetBianMa, int iCodeType, int iUpOrDown)
{
	CStringList *strListMaXu = NULL;
	switch (iCodeType)
	{
	case 0:
		strListMaXu = &m_strListMaXu ;
		break;
	case 1:
		strListMaXu = &m_strListMaXuRecForLoop ;
		break;
	case 2:
		strListMaXu = &m_strListMaXuRecForMain ;
		break;
	case 3:
		strListMaXu = &m_strListMaXuRecForThrough ;
		break;
	default:
		strListMaXu = &m_strListMaXu ;
		break;
	}

	if (strRetBianMa==_T(""))
	{
		strRetBianMa = strListMaXu->GetHead() ;
	}
	else
	{
		POSITION posCur, posEnd ;
		posCur = strListMaXu->Find(strRetBianMa) ;
		posEnd = strListMaXu->GetTailPosition() ;
		
		try
		{
			CString strSql ;
			CADORecordset cSet ;		

			CString strPreBelongXhj = _T("") ; //前方（下一个）信号机
			CString strCurBelongXhj = _T("")  ; //当前区段所在的防护信号机
			CString strCurGuidaoLx = _T("")  ; //当前轨道区段内的道口
			int iCurGuidaoLxStatus = 0 ;//当前轨道区段内的道口状态


			strSql.Format(_T("select * from bt_mxb_import where id=%d"), iGuiDaoId) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()>0)
				{
					cSet.MoveFirst() ;
					
					cSet.GetFieldValue(_T("bt_mxb_import_lx"), strCurGuidaoLx) ;
					cSet.GetFieldValue(_T("bt_mxb_import_lxstatus"), iCurGuidaoLxStatus) ;
					cSet.GetFieldValue(_T("bt_mxb_import_belongxhj"), strCurBelongXhj) ;
				}                

				cSet.Close() ;
			}

			//begin 前一个轨道是否是信号机防护的第一区段（跨过信号机则编码变化）
			strSql.Format(_T("select * from bt_mxb_import where id=%d"), iUpOrDown==0?iGuiDaoId+1:iGuiDaoId-1) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()>0)
				{
					cSet.MoveFirst() ;
					int iPreGuiDaoId = 0 ;
					CString strPreGuidaoXhj ;
					cSet.GetFieldValue(_T("id"), iPreGuiDaoId) ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strPreGuidaoXhj) ;
					if (!strPreGuidaoXhj.IsEmpty())
					{
						if (posEnd!=posCur) //当前已经达到G，后续编码都为G
						{
							strListMaXu->GetNext(posCur) ;
							strRetBianMa = strListMaXu->GetAt(posCur) ;

							//begin 前后信号机是否都关闭了（两架连续的信号机都不开放，则中间所有轨道发R码）
							CADORecordset cSetTmp ;
							int iPreSectionLxStatus = 1 ;//下一架信号机防护的道口状态	
							int iCurSectionLxStatus = 1 ;//当前架信号机防护的道口状态
							int iPreXhjStatus = 1 ;//下一架信号机状态	
							int iCurXhjStatus = 1 ;//当前架信号机状态

							//strSql.Format(_T("select * from bt_mxb_import where id%s%d and bt_mxb_import_belongxhj<>'%s' and bt_mxb_import_upordown=%d order by id %s"), iUpOrDown==0?_T(">"):_T("<"), iGuiDaoId, strCurBelongXhj, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
							//当前防护区段的道口状态
							strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj='%s' and bt_mxb_import_lx<>'' order by id %s"), strCurBelongXhj, iUpOrDown==0?_T("asc"):_T("desc")) ;
							if (cSetTmp.IsOpen())
							{
								cSetTmp.Close() ;
							}
							if (cSetTmp.Open(g_PtrCon, strSql))
							{
								if (cSetTmp.GetRecordCount()>0)
								{
									cSetTmp.MoveFirst() ;
									cSetTmp.GetFieldValue(_T("bt_mxb_import_lxstatus"),iCurSectionLxStatus) ;

								}
								cSetTmp.Close() ;
							}
							//当前防护区段的第一个轨道的编码
							int iCurGuiDaoId = 0 ;
							CString strCurXhjType = _T("") ;
							//CString strCurXhjFirstGuiDaoBianMa = _T("") ;
							strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj='%s' and bt_mxb_import_xhj<>'' order by id %s"), strCurBelongXhj, iUpOrDown==0?_T("asc"):_T("desc")) ;
							if (cSetTmp.IsOpen())
							{
								cSetTmp.Close() ;
							}
							if (cSetTmp.Open(g_PtrCon, strSql))
							{
								if (cSetTmp.GetRecordCount()>0)
								{
									cSetTmp.MoveFirst() ;
									cSetTmp.GetFieldValue(_T("id"),iCurGuiDaoId) ;
									cSetTmp.GetFieldValue(_T("bt_mxb_import_xhjtype"),strCurXhjType) ;
									//cSetTmp.GetFieldValue(_T("bt_mxb_import_code"),strCurXhjFirstGuiDaoBianMa) ;

								}
								cSetTmp.Close() ;
							}
							if (iCurSectionLxStatus==0)
							{
								iCurXhjStatus = 0 ;
							}
							else if (strCurXhjType==_T("ASS"))
							{
								iCurXhjStatus = 0 ;
							}
// 							else
// 							{
// 								//如果当前架信号机关闭（信号机防护第一个区段R码）
// 								if (strCurXhjFirstGuiDaoBianMa==_T("R"))
// 								{
// 									iCurXhjStatus = 0 ;
// 								}
// 							}
							//下一防护区段道口状态
							strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_belongxhj='%s' and bt_mxb_import_upordown=%d and bt_mxb_import_lx<>'' order by id %s"), strPreGuidaoXhj, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;							
							if (cSetTmp.IsOpen())
							{
								cSetTmp.Close() ;
							}
							if (cSetTmp.Open(g_PtrCon, strSql))
							{
								if (cSetTmp.GetRecordCount()>0)
								{
									cSetTmp.MoveFirst() ;
													
									cSetTmp.GetFieldValue(_T("bt_mxb_import_lxstatus"), iPreSectionLxStatus) ;
								}                

								cSetTmp.Close() ;
							}
							if (iPreSectionLxStatus==0)
							{
								iPreXhjStatus = 0 ;
							}
							else if(strPreGuidaoXhj.Left(3)==_T("OHS"))
							{
								iPreXhjStatus = 0 ;
							}
							else
							{
								//如果下一架信号机关闭（信号机防护第一个区段R码）
								CString strPreGuiDaoBianMa = _T("") ;
								cSet.GetFieldValue(_T("bt_mxb_import_code"), strPreGuiDaoBianMa) ;
								if (strPreGuiDaoBianMa==_T("R"))
								{
									iPreXhjStatus=0 ;
								}
							}
							if (iUpOrDown==0)
							{
								if (!(m_iOCPGuiDaoId>=iCurGuiDaoId && m_iOCPGuiDaoId<iPreGuiDaoId))
								{	
									if (iPreXhjStatus==0 && iCurXhjStatus==0 )
									{
										strRetBianMa = _T("R") ;
									}
								}
							}
							else
							{
								if (!(m_iOCPGuiDaoId<=iCurGuiDaoId && m_iOCPGuiDaoId>iPreGuiDaoId))
								{
									if (iPreXhjStatus==0 && iCurXhjStatus==0 )
									{
										strRetBianMa = _T("R") ;
									}
								}
							}


							//end 前后信号机是否都关闭了（两架连续的信号机都不开放，则中间所有轨道发R码）
						}
					}
				}                

				cSet.Close() ;
			}
			//end 前一个轨道是否是信号机防护的第一区段（跨过信号机则编码变化）

			//begin 位于当前轨道的道口开放否（道口不开则当前轨道发R码）
			if (!strCurGuidaoLx.IsEmpty() && iCurGuidaoLxStatus==0)
			{						
				strRetBianMa = strListMaXu->GetHead() ;
			}
			//end 位于当前轨道的道口开放否（道口不开则当前轨道发R码）



		}
		catch (_com_error& e)
		{
			CString sBuff = CBlkUtility::GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in GetGuiDaoBianMa_QJ")) ;
		}


	}

	return 0;
}

void CMaXuBiaoForBT::DrawBianMaTable(CArray<CStringArray *, CStringArray *> & arrayBianMa, const CStringArray &strArrayOCPGuiDao, const CStringArray &strArrayDescribe, int iUpOrDown)
{
	double dX = 0 ;
	double dY = 0 ;
	double dCellWidth = 15.000 ;
	double dDescribeWidth = 55.000 ;
	double dCellHeight = 7.000 ;
	double dCellOCPHeight = 3.000 ;
	double dLineWidth00 = 0.000 ;
	double dLineWidth02 = 0.200 ;
	double dLineWidth04 = 0.400 ;

	AcDbObjectId  idTextStyleTitle, idTextStyleContent, idTextStyleChs;
	idTextStyleTitle = g_cSystem.CreateTextStyleByFileName(_T("zhffontTitle"), _T("txt.shx"), _T("hztxt2.shx"), 4, 0.7) ;	
	idTextStyleContent = g_cSystem.CreateTextStyleByFileName(_T("zhffontContent"), _T("txt.shx"), _T("hztxt2.shx"), 3, 0.7) ;
	idTextStyleChs = g_cSystem.CreateTextStyle(_T("CHINESE"), _T("Times New Roman"), _T(""), 3, 0.9) ;

	int iBianMaCount = arrayBianMa.GetCount() ;
	int iGuiDaoCount = 0 ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		AcGePoint3d pt_3d_start(0,0,0), pt_3d_end(0,0,0), pt_3d_insert(0,0,0) ;
		ads_point pt_base = {0,0,0} ; 

		//begin 绘制示意图
		strSql.Format(_T("SELECT * FROM bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon, strSql))
		{
			int iGdSum = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}

			AcDbBlockReference *pRef = NULL ;
			CString strBlockFile = _T("") ;	

			int iPlus = iUpOrDown==0?1:-1 ;//上下行X对应不同的增量（下行增加，上行减少）
			int iCurGdIndex = 0 ;
			double dStartX = iUpOrDown==0?0:dCellWidth*iGdSum ;
			double dGuiDaoY = iUpOrDown==0?15:30 ;
			double dEndX = iUpOrDown==1?0:dCellWidth*iGdSum ;

			//绘制轨道直线
			pt_3d_start.x = dStartX ;
			pt_3d_start.y = pt_3d_end.y = dGuiDaoY ;
			pt_3d_end.x = dEndX ;			
			g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end, 0.4) ;

			//左右侧车站名称
			pt_base[0] = -5 ;
			pt_base[1] = dGuiDaoY ;
			//AcDbObjectId objId ;
			CString strStation = _T("") ;
			strStation.Format(_T("%s STATION"), m_strStation) ;
			//g_cSystem.WriteText(pt_base, strStation, objId, 2, 0.8, AcDb::kTextRight, AcDb::kTextVertMid, 0, idTextStyleChs,0) ;
			g_cSystem.AddText(strStation,pt_base,pt_base, AcDb::kTextRight, AcDb::kTextVertMid, 2, 0.8, 0, 0, idTextStyleChs) ;
			pt_base[0] = dCellWidth*iGdSum+5 ;
			strStation.Format(_T("%s STATION"), m_strStation2) ;
			//g_cSystem.WriteText(pt_base, strStation, objId, 2, 0.8, AcDb::kTextLeft, AcDb::kTextVertMid, 0, idTextStyleChs,0) ;
			g_cSystem.AddText(strStation,pt_base,pt_base, AcDb::kTextLeft, AcDb::kTextVertMid, 2, 0.8, 0, 0, idTextStyleChs) ;

			//arrow
			strBlockFile.Format(_T("%s\\support\\bt\\%s.dwg"), m_strSysBasePath, iUpOrDown==0?_T("bt_mxb_arrow_down_0"):_T("bt_mxb_arrow_up_0")) ;
			pt_3d_insert.x = iUpOrDown==0?-25:dCellWidth*iGdSum+10 ;
			pt_3d_insert.y = iUpOrDown==0?25:10 ;
			if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
			{
				pRef->close() ;
				pRef = NULL ;
			}			

			BOOL bPassAss = FALSE ;
			while (!cSet.IsEOF())
			{
				double dCurStartX = dStartX+iPlus*iCurGdIndex*dCellWidth ;  //当前区段起始点

				CString strGdName, strXhjName,strXhjType, strBjLoc, strDkLoc ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strGdName) ;
				cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjName) ;
				cSet.GetFieldValue(_T("bt_mxb_import_xhjtype"), strXhjType) ;
				cSet.GetFieldValue(_T("bt_mxb_import_bj"), strBjLoc) ;
				cSet.GetFieldValue(_T("bt_mxb_import_lx"), strDkLoc) ;
				CString strJyjName = _T("bt_mxb_jxjyj") ;
				if (bPassAss==FALSE)
				{
					if (strXhjType==_T("ASS"))
					{
						bPassAss = TRUE ;
					}
				}
				else if (strXhjType!=_T("OHS"))
				{
					strJyjName = _T("bt_mxb_dqjyj") ;
				}
				CStringArray strArrayAtt ;
				pt_3d_insert.x = dCurStartX ;
				pt_3d_insert.y = dGuiDaoY ;

				//绝缘节
				strBlockFile.Format(_T("%s\\support\\bt\\%s.dwg"), m_strSysBasePath, strJyjName) ;
				if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
				{
					pRef->close() ;
					pRef = NULL ;
				}
				//最后一个轨道绘制末端绝缘节
				if (iGdSum==(iCurGdIndex+1))
				{
					pt_3d_insert.x = dCurStartX+dCellWidth*iPlus ;
					if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert,0, 1) )
					{
						pRef->close() ;
						pRef = NULL ;
					}	
				}

				//轨道名称
				pt_3d_insert.x = dCurStartX+iPlus*6.3 ;
				strBlockFile.Format(_T("bt_mxb_gd_%s_0"), iUpOrDown==0?_T("down"):_T("up") ) ;
				strArrayAtt.Add(strGdName) ;
				strArrayAtt.Add(_T("")) ;
				if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				//信号机
				if (strXhjName!=_T(""))
				{
					pt_3d_insert.x = dCurStartX+(-1)*iPlus ;
					if (strXhjType==_T("ASS"))
					{
						strBlockFile.Format(_T("bt_mxb_xhj_ass_%s_0"), iUpOrDown==0?_T("down"):_T("up") ) ;
					}
					else if (strXhjType==_T("OHS"))
					{
						strBlockFile.Format(_T("bt_mxb_xhj_ohs_%s_0"), iUpOrDown==0?_T("down"):_T("up")) ;
					}
// 					else if (strXhjName.Left(2)==_T("AD"))
// 					{
// 						strBlockFile.Format(_T("bt_mxb_xhj_a_%s_0"), iUpOrDown==0?_T("down"):_T("up")) ;
// 					}
					else if (strXhjName.Left(2)==_T("AD"))
					{
						strBlockFile.Format(_T("bt_mxb_xhj_a_down_0")) ;
					}
					else if (strXhjName.Left(2)==_T("AU"))
					{
						strBlockFile.Format(_T("bt_mxb_xhj_a_up_0")) ;
					}
					else if (strXhjName.Left(3)==_T("LXD"))
					{
						strBlockFile.Format(_T("bt_mxb_xhj_a-lx_down_0")) ;
					}
					else if (strXhjName.Left(3)==_T("LXU"))
					{
						strBlockFile.Format(_T("bt_mxb_xhj_a-lx_up_0")) ;
					}
					strArrayAtt.RemoveAll() ;
					strArrayAtt.Add(strXhjName) ;
					if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}

				//报警
				if (strBjLoc!=_T(""))
				{
					strArrayAtt.RemoveAll() ;
					CBlkUtility::DivideString(strBjLoc, _T(","), strArrayAtt) ;
					int iLxNum = strArrayAtt.GetCount() ;						
					strBlockFile.Format(_T("bt_mxb_bjq_%s_%d"), iUpOrDown==0?_T("down"):_T("up"), iLxNum) ;
					AcDbObjectId objIdBjq ;
					if (!g_cSystem.HaveBlockName(strBlockFile, objIdBjq))
					{
						this->CreateBjqBlock(iLxNum, iUpOrDown) ;
					}
					pt_3d_insert.x = dCurStartX ;
					pt_3d_insert.y = iUpOrDown==0? dGuiDaoY+12:dGuiDaoY-12 ;
					if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}

				//道口
				if (strDkLoc!=_T(""))
				{
					pt_3d_insert.x = dCurStartX+12*iPlus ;
					pt_3d_insert.y = dGuiDaoY ;
					strArrayAtt.RemoveAll() ;
					strArrayAtt.Add(strDkLoc) ;
					strBlockFile = _T("bt_mxb_lx_0") ;
					if(g_cSystem.InsertBlock(pRef, strBlockFile, pt_3d_insert, &strArrayAtt,FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
				}

				iCurGdIndex++ ;

				cSet.MoveNext() ;
			}


			cSet.Close() ;

		}
		//end 绘制示意图

		//begin 绘制轨道名称和频率等
		pt_3d_start.x = pt_3d_end.x = pt_3d_start.y = pt_3d_end.y = 0 ;
		

		pt_base[0] = -dDescribeWidth-0.5 ;
		pt_base[1] = 3 ;
		//AcDbObjectId objId ;
		//g_cSystem.WriteText(pt_base, iUpOrDown==0?_T("DOWN"):_T("UP"), objId, 5, 0.9, AcDb::kTextLeft, AcDb::kTextBase, 0, idTextStyleChs) ;
		g_cSystem.AddText(iUpOrDown==0?_T("DOWN"):_T("UP"),pt_base,pt_base, AcDb::kTextLeft, AcDb::kTextBase, 5, 0.9, 0, 0, idTextStyleChs) ;

		CStringArray strArrayGuiDao ;

		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iGuiDaoCount = cSet.GetRecordCount() ;

			//绘制横线2根
			pt_3d_start.x = 0-dDescribeWidth ;
			pt_3d_end.x = dCellWidth*iGuiDaoCount ;
			g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth04, 0) ;
			pt_3d_start.y = pt_3d_end.y = 0-dCellHeight ;
			g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth02, 0) ;

			pt_base[0] = -dDescribeWidth/2 ;
			pt_base[1] = -dCellHeight/2 ;
			g_cSystem.AddMText(pt_base, _T("Track Section Name"), idTextStyleChs, AcDbMText::kMiddleCenter, 3, dDescribeWidth-1, 0) ;
			pt_base[1] = -dCellHeight*3/2 ;
			g_cSystem.AddMText(pt_base, _T("Frequency"), idTextStyleChs, AcDbMText::kMiddleCenter, 3, dDescribeWidth-1, 0) ;

			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}			
			int iColIndex = 0 ;
			pt_3d_start.y = 0 ;
			pt_3d_end.y = 0-dCellHeight*(iBianMaCount+2) ;
			while(!cSet.IsEOF())
			{
				CString strGuiDao, strFreq, strXhj ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strGuiDao) ;
				cSet.GetFieldValue(_T("bt_mxb_import_freq"), strFreq) ;
				cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strXhj) ;

				strArrayGuiDao.Add(strGuiDao) ;

				//绘制竖线
				pt_3d_start.x=pt_3d_end.x = dCellWidth*iColIndex ;
				g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,strXhj.IsEmpty()?dLineWidth00:dLineWidth04, 0) ;

				//绘制轨道名称				
				pt_base[0] = pt_3d_start.x+dCellWidth/2 ;
				pt_base[1] = 0-dCellHeight/2 ;
				g_cSystem.AddMText(pt_base, strGuiDao, idTextStyleTitle, AcDbMText::kMiddleCenter, 3, dCellWidth-1, 0) ;

				//绘制频率
				pt_base[1] = 0-3*dCellHeight/2 ;
				g_cSystem.AddMText(pt_base, strFreq, idTextStyleTitle, AcDbMText::kMiddleCenter, 3, dCellWidth-1, 0) ;

				iColIndex++ ;
				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}
		//end 绘制轨道名称和频率等

		//绘制编码
		pt_3d_start.x = 0 ;
		pt_3d_end.x = dCellWidth*iGuiDaoCount ;
		CString strPreDescribe = _T("") ;
		double dPreDescribeY = -2*dCellHeight ;
		
		for (int iRow=0; iRow<iBianMaCount; iRow++)
		{
			//describe
			CString strCurDescribe = _T("") ;
			strCurDescribe = strArrayDescribe.GetAt(iRow) ;

			//绘制行线和中间横线
			pt_3d_start.x = 0 ;
			pt_3d_start.y = pt_3d_end.y = 0-(iRow+2)*dCellHeight ;
			if (strPreDescribe!=strCurDescribe) //描述变化就绘制前一个描述
			{
				pt_3d_start.x = 0-dDescribeWidth ;
				if (strPreDescribe!=_T(""))
				{
					pt_base[0] = 0-dDescribeWidth/2 ;
					pt_base[1] = pt_3d_start.y-(pt_3d_start.y-dPreDescribeY)/2 ;
					g_cSystem.AddMText(pt_base, strPreDescribe, idTextStyleChs, AcDbMText::kMiddleCenter, 3, dDescribeWidth-1, 0) ;
					dPreDescribeY = pt_3d_start.y ;					
				}
				strPreDescribe = strCurDescribe ;
			}
			g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth02, 0) ;
			pt_3d_start.x = 0 ;
			pt_3d_start.y = pt_3d_end.y = 0-(iRow+2)*dCellHeight-dCellOCPHeight ;
			g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth00, 0) ;

			if (iRow==iBianMaCount-1) //最后一个描述和横线、左竖线
			{
				//最下面直线
				pt_3d_start.x = 0-dDescribeWidth ;
				pt_3d_start.y = pt_3d_end.y = 0-(iRow+3)*dCellHeight ;
				g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth02, 0) ;
				//最后的描述
				pt_base[0] = 0-dDescribeWidth/2 ;
				pt_base[1] = pt_3d_start.y-(pt_3d_start.y-dPreDescribeY)/2 ;
				g_cSystem.AddMText(pt_base, strPreDescribe, idTextStyleChs, AcDbMText::kMiddleCenter, 3, dDescribeWidth-1, 0) ;

				//左竖线
				pt_3d_end.x = pt_3d_start.x ;
				pt_3d_end.y = 0 ;
				g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth02, 0) ;
				//右竖线
				pt_3d_start.x = pt_3d_end.x = dCellWidth*iGuiDaoCount ;
				g_cSystem.AddAcDb2dPolyline(pt_3d_start, pt_3d_end,dLineWidth02, 0) ;

			}

			CStringArray *pStrArray = arrayBianMa.GetAt(iRow) ;
			for (int iCol=0; iCol<iGuiDaoCount; iCol++)
			{
				pt_base[0] = dCellWidth*iCol+dCellWidth/2 ;
				pt_base[1] = 0-(iRow+2)*dCellHeight-dCellOCPHeight-(dCellHeight-dCellOCPHeight)/2 ;
				g_cSystem.AddMText(pt_base, pStrArray->GetAt(iCol), idTextStyleContent, AcDbMText::kMiddleCenter, 2.5, dCellWidth-1, 0) ;

				if (strArrayGuiDao.GetAt(iCol)==strArrayOCPGuiDao.GetAt(iRow))
				{
					pt_base[1] = 0-(iRow+2)*dCellHeight-dCellOCPHeight+dCellOCPHeight/2 ;
					g_cSystem.AddMText(pt_base, _T("OCP"), idTextStyleChs, AcDbMText::kMiddleCenter, 2.5, dCellWidth-1, 0) ;
				}

// 				if ((iUpOrDown==0&&(iCol==iRow-1))||(iUpOrDown==1&&(iCol==iBianMaCount-iRow-1)))
// 				{
// 					pt_base[1] = pt_3d_start.y+dCellOCPHeight/2 ;
// 					g_cSystem.AddMText(pt_base, _T("OCP"), idTextStyleChs, AcDbMText::kMiddleCenter, 2.5, dCellWidth/2-1, 0) ;
// 				}
			}

		}



	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawBianMaTable")) ;
	}


}

// 计算当某一轨道占用时所有轨道发码情况
int CMaXuBiaoForBT::GetGuiDaoBianMaOCP(int iGuiDaoIdOCP, const CString strBelongXhjOCP, const CString strLxZnBj, int &iGuiDaoIdLeftStart, int &iGuiDaoIdLeftEnd, int iType,  int iUpOrDown)
{
	try
	{
		CString strSql ;
		CADORecordset cSetTmp ;

		CString strRetBianMa ;

		m_iOCPGuiDaoId = iGuiDaoIdOCP ;

		//begin 占据区段后方（列车运行方向为向前方）
		//设置道口状态
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=0 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s%d "), iUpOrDown, iUpOrDown==0?_T("<"):_T(">"), iGuiDaoIdOCP) ;
		ExecSqlString(strSql) ;

		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("<"):_T(">"), iGuiDaoIdOCP, iUpOrDown==0?_T("desc"):_T("asc")) ;
		if (cSetTmp.IsOpen())
		{
			cSetTmp.Close() ;
		}
		if (cSetTmp.Open(g_PtrCon, strSql))
		{
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			strRetBianMa = _T("") ;
			while(!cSetTmp.IsEOF())
			{
				int iGuiDaoIdTmp = 0 ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
				this->GetGuiDaoBianMa_QJ(iGuiDaoIdTmp, strRetBianMa, 0, iUpOrDown) ;
				cSetTmp.Edit() ;
				cSetTmp.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMa) ;
				cSetTmp.Update() ;
				//2012-02-13 here

				cSetTmp.MoveNext() ;
			}

			cSetTmp.Close() ;
		}
		//end 占据区段后方（列车运行方向为向前方）


		//begin 占据区段前方（列车运行方向为向前方）
		//查找后方所有的报警通知的道口(已通知道口）
		CStringArray strArrayBj ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s%d and bt_mxb_import_bj<>'' order by id %s"), iUpOrDown, iUpOrDown==0?_T("<="):_T(">="), iGuiDaoIdOCP, iUpOrDown==0?_T("asc"):_T("desc")) ;
		if (cSetTmp.IsOpen())
		{
			cSetTmp.Close() ;
		}
		if (cSetTmp.Open(g_PtrCon, strSql))
		{
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			while(!cSetTmp.IsEOF())
			{
				CString strBjStr ;
				cSetTmp.GetFieldValue(_T("bt_mxb_import_bj"), strBjStr) ;
				CStringArray strArrayBjTmp ;
				CBlkUtility::DivideString(strBjStr, _T(","), strArrayBjTmp) ;
				strArrayBj.Append(strArrayBjTmp) ;

				cSetTmp.MoveNext() ;
			}
			cSetTmp.Close() ;
		}
		//查找报警通知的道口(已通知道口）处于哪些信号机防护
		CStringArray strArrayXhj ;
		CString strBjStr= _T("") ;
		if (strArrayBj.GetCount()>0)
		{
			CBlkUtility::ConvertArrayToString(strArrayBj, _T(","), strBjStr) ;
			strBjStr.Replace(_T(","), _T("','")) ;
			strBjStr.Insert(0, _T("'")) ;
			strBjStr.Append(_T("'")) ;
			strSql.Format(_T("select * from  bt_mxb_import where bt_mxb_import_xhj<>'' and  bt_mxb_import_belongxhj in( select DISTINCT bt_mxb_import_belongxhj  from bt_mxb_import where bt_mxb_import_upordown=%d and id%s%d and bt_mxb_import_belongxhj<>'%s' and bt_mxb_import_lx in (%s)) order by id %s"), iUpOrDown, iUpOrDown==0?_T(">"):_T("<"), iGuiDaoIdOCP, strBelongXhjOCP, strBjStr, iUpOrDown==0?_T("asc"):_T("desc") ) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			if (cSetTmp.Open(g_PtrCon, strSql))
			{
				if (!cSetTmp.IsBOF())
				{
					cSetTmp.MoveFirst() ;
				}
				while(!cSetTmp.IsEOF())
				{
					CString strXhjTmp ;
					cSetTmp.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjTmp) ;
					strArrayXhj.Add(strXhjTmp) ;

					cSetTmp.MoveNext() ;
				}
				cSetTmp.Close() ;
			}
		}

		//2012-02-20：here
		//判断最后一个“已通知道口所处的防护信号机”的下一个信号机是否有道口 或者是不是OHS
		BOOL bAddPlusXhj = FALSE ;
		int iXhjCount1 = strArrayXhj.GetCount() ;
		if (iXhjCount1>0)
		{
			CString strLastXhj = strArrayXhj.GetAt(iXhjCount1-1) ;

			//下架信号机是不是OHS
			strSql.Format(_T("SELECT top 1 * FROM bt_mxb_import WHERE id%s(SELECT id FROM bt_mxb_import WHERE bt_mxb_import_xhj='%s') and bt_mxb_import_xhj<>'' order by id %s"), iUpOrDown==0?_T(">"):_T("<"), strLastXhj, iUpOrDown==0?_T("asc"):_T("desc") ) ;
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			if (cSetTmp.Open(g_PtrCon, strSql))
			{
				if (cSetTmp.GetRecordCount()>0)
				{
					cSetTmp.MoveFirst() ;
					CString strXhjTmp ;
					cSetTmp.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjTmp) ;
					if(strXhjTmp.Left(3)==_T("OHS"))
					{
						strArrayXhj.Add(strXhjTmp) ;
						bAddPlusXhj = TRUE ;
					}
				}
				cSetTmp.Close() ;
			}
			//下一个信号机是否有道口
			if (bAddPlusXhj==FALSE)
			{
				strSql.Format(_T("SELECT * FROM bt_mxb_import WHERE bt_mxb_import_lx<>'' and bt_mxb_import_belongxhj=(select bt_mxb_import_xhj from (SELECT top 1 * FROM bt_mxb_import WHERE id%s(SELECT id FROM bt_mxb_import WHERE bt_mxb_import_xhj='%s') and bt_mxb_import_xhj<>'' order by id %s)) order by id %s"), iUpOrDown==0?_T(">"):_T("<"), strLastXhj, iUpOrDown==0?_T("asc"):_T("desc"), iUpOrDown==0?_T("asc"):_T("desc") ) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						cSetTmp.MoveFirst() ;
						CString strXhjTmp ;
						cSetTmp.GetFieldValue(_T("bt_mxb_import_xhj"), strXhjTmp) ;
						strArrayXhj.Add(strXhjTmp) ;
						bAddPlusXhj = TRUE ;
					}
					cSetTmp.Close() ;
				}
			}

		}
		//begin 遍历strArrayXhj中信号机防护区段
		// 				//设置当前ocp轨道所在的防护区段所有道口 bt_mxb_import_lxstatus=1 表示道口能行车（不能走人）
		// 				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_belongxhj=(select bt_mxb_import_belongxhj from bt_mxb_import where id=%d) "), iUpOrDown, iGuiDaoIdOCP) ;
		// 				ExecSqlString(strSql) ;

		int iXhjCount2 = strArrayXhj.GetCount() ;
		if (iType>0)//接车进正线、侧线、通过时，占用后的道口全部开放（能行车）
		{
			iXhjCount2 = 0 ;
		}
		for(int iLxSection=0; iLxSection<iXhjCount2; iLxSection++)
		{
			if ((iLxSection==iXhjCount2-2) && bAddPlusXhj==TRUE) 
			{
				//2012-02-22;here
				CString strLastXhj = strArrayXhj.GetAt(iXhjCount2-1) ;
				CString strViceLastXhj = strArrayXhj.GetAt(iLxSection) ;

				strRetBianMa = _T("R") ;
				//begin 最后一架xhj关闭，倒数第二架xhj关闭的情况
				//设置当前ocp轨道所在信号机至strViceLastXhj之间的所有道口 bt_mxb_import_lxstatus=1 表示道口能行车（不能走人）
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_xhj=(select bt_mxb_import_belongxhj from bt_mxb_import where id=%d )) and id%s(select id from bt_mxb_import where bt_mxb_import_xhj='%s')"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP, iUpOrDown==0?_T("<"):_T(">"), strViceLastXhj) ;
				ExecSqlString(strSql) ;
				//设置strViceLastXhj之后的所有道口 bt_mxb_import_lxstatus=0 表示道口不能能行车（能走人）
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=0 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_xhj='%s')"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), strViceLastXhj) ;
				ExecSqlString(strSql) ;

				//begin 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭
				if (strLxZnBj!=_T(""))
				{
					strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_lx in(%s) and id%s%d"), iUpOrDown, strLxZnBj, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP) ;
					ExecSqlString(strSql) ;
				}
				//end 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭

				strSql.Format(_T("select * from bt_mxb_import where id%s%d and id %s(select id from bt_mxb_import where bt_mxb_import_xhj='%s') order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP,iUpOrDown==0?_T("<"):_T(">"), strLastXhj, iUpOrDown==0?_T("desc"):_T("asc")) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						cSetTmp.MoveFirst() ;
						cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdLeftStart) ;
						if (iUpOrDown==0)
						{
							iGuiDaoIdLeftStart++ ;
						}
						else
						{
							iGuiDaoIdLeftStart-- ;
						}

					}							
					while(!cSetTmp.IsEOF())
					{
						int iGuiDaoId ;
						CString strCurBianMa,strBianMaOld ;
						cSetTmp.GetFieldValue(_T("id"), iGuiDaoId) ;
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaOld) ;

						this->GetGuiDaoBianMa_QJ(iGuiDaoId, strRetBianMa, 0, iUpOrDown) ;
						strCurBianMa = strRetBianMa ;
						if (!strBianMaOld.IsEmpty())
						{
							strCurBianMa.Append(_T("/")) ;
							strCurBianMa.Append(strBianMaOld) ;									
						}
						cSetTmp.Edit() ;
						cSetTmp.SetFieldValue(_T("bt_mxb_import_code"), strCurBianMa) ;
						cSetTmp.Update() ;

						cSetTmp.MoveNext() ;
					}

					cSetTmp.Close() ;
				}
				//end 最后一架xhj关闭，倒数第二架xhj关闭的情况

				//begin 最后一架xhj关闭，倒数第二架xhj开放的情况
				//设置当前strViceLastXhj防护的道口 bt_mxb_import_lxstatus=1 表示道口能行车（不能走人）
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_belongxhj='%s'"), iUpOrDown, strViceLastXhj) ;
				ExecSqlString(strSql) ;
				//begin 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭	
				if (strLxZnBj!=_T(""))
				{
					strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_lx in(%s) and id%s%d"), iUpOrDown, strLxZnBj, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP) ;
					ExecSqlString(strSql) ;
				}
				//end 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭

				strSql.Format(_T("select * from bt_mxb_import where id%s%d and id %s(select id from bt_mxb_import where bt_mxb_import_xhj='%s') order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP,iUpOrDown==0?_T("<"):_T(">"), strLastXhj, iUpOrDown==0?_T("desc"):_T("asc")) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (!cSetTmp.IsBOF())
					{
						cSetTmp.MoveFirst() ;
					}
					strRetBianMa = _T("R") ;
					while(!cSetTmp.IsEOF())
					{
						int iGuiDaoId ;
						CString strCurBianMa,strBianMaOld ;
						cSetTmp.GetFieldValue(_T("id"), iGuiDaoId) ;
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaOld) ;

						this->GetGuiDaoBianMa_QJ(iGuiDaoId, strRetBianMa, 0, iUpOrDown) ;
						strCurBianMa = strRetBianMa ;
						if (!strBianMaOld.IsEmpty())
						{
							strCurBianMa.Append(_T("/")) ;
							strCurBianMa.Append(strBianMaOld) ;									
						}
						cSetTmp.Edit() ;
						cSetTmp.SetFieldValue(_T("bt_mxb_import_code"), strCurBianMa) ;
						cSetTmp.Update() ;

						cSetTmp.MoveNext() ;
					}

					cSetTmp.Close() ;
				}

				//end 最后一架xhj关闭，倒数第二架xhj开放的情况

				break;
			}

			CString strCurXhj = strArrayXhj.GetAt(iLxSection) ;					
			//设置当前ocp轨道所在信号机至strCurXhj之间的所有道口 bt_mxb_import_lxstatus=1 表示道口能行车（不能走人）
			strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_xhj=(select bt_mxb_import_belongxhj from bt_mxb_import where id=%d )) and id%s(select id from bt_mxb_import where bt_mxb_import_xhj='%s')"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP, iUpOrDown==0?_T("<"):_T(">"), strCurXhj) ;
			ExecSqlString(strSql) ;
			//设置strCurXhj之后的所有道口 bt_mxb_import_lxstatus=0 表示道口不能能行车（能走人）
			strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=0 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_xhj='%s')"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), strCurXhj) ;
			ExecSqlString(strSql) ;
			//begin 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭	
			if (strLxZnBj!=_T(""))
			{
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_lx in(%s) and id%s%d"), iUpOrDown, strLxZnBj, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP) ;
				ExecSqlString(strSql) ;
			}
			//end 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭

			//begin 如果存在strArrayXhj.GetAt(iLxSection+1)信号机,并且该信号机即为紧挨strCurXhj下一架信号机,则从该信号机后方轨道开始排码
			BOOL bNextEquNearCur = FALSE ;
			if ((iLxSection+1)<iXhjCount2)
			{
				strSql.Format(_T("select * from bt_mxb_import where id%s(select id from bt_mxb_import where bt_mxb_import_xhj='%s') and bt_mxb_import_xhj<>'' and bt_mxb_import_upordown=%d order by id %s"),iUpOrDown==0?_T(">"):_T("<"), strCurXhj, iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc") ) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						cSetTmp.MoveFirst() ;
						CString strNextXhj ;
						int iNextXhjId = 0 ;
						cSetTmp.GetFieldValue(_T("bt_mxb_import_xhj"), strNextXhj) ;
						cSetTmp.GetFieldValue(_T("id"), iNextXhjId) ;
						if (strNextXhj==strArrayXhj.GetAt(iLxSection+1))
						{
							bNextEquNearCur = TRUE ;
							strSql.Format(_T("select * from bt_mxb_import where id%s%d and id %s%d order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP,iUpOrDown==0?_T("<"):_T(">"), iNextXhjId, iUpOrDown==0?_T("desc"):_T("asc")) ;
						}
					}
					cSetTmp.Close() ;
				}
			}
			//end 如果存在strArrayXhj.GetAt(iLxSection+1)信号机,并且该信号机即为紧挨strCurXhj下一架信号机,则从该信号机后方轨道开始排码

			if (bNextEquNearCur==FALSE)
			{
				strSql.Format(_T("select * from bt_mxb_import where id%s%d and id %s(select top 1 id from bt_mxb_import where bt_mxb_import_lx<>'' and bt_mxb_import_belongxhj='%s' order by id %s) order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP,iUpOrDown==0?_T("<="):_T(">="), strCurXhj, iUpOrDown==0?_T("desc"):_T("asc"), iUpOrDown==0?_T("desc"):_T("asc")) ;
			}
			if (cSetTmp.IsOpen())
			{
				cSetTmp.Close() ;
			}
			if (cSetTmp.Open(g_PtrCon, strSql))
			{
				if (!cSetTmp.IsBOF())
				{
					cSetTmp.MoveFirst() ;
				}

				strRetBianMa = _T("") ;
				while(!cSetTmp.IsEOF())
				{
					int iGuiDaoId ;
					CString strCurBianMa, strBianMaOld ;
					cSetTmp.GetFieldValue(_T("id"), iGuiDaoId) ;
					cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaOld) ;

					this->GetGuiDaoBianMa_QJ(iGuiDaoId, strRetBianMa, 0, iUpOrDown) ;
					strCurBianMa = strRetBianMa ;
					if (!strBianMaOld.IsEmpty())
					{
						strCurBianMa.Append(_T("/")) ;
						strCurBianMa.Append(strBianMaOld) ;									
					}
					cSetTmp.Edit() ;
					cSetTmp.SetFieldValue(_T("bt_mxb_import_code"), strCurBianMa) ;
					cSetTmp.Update() ;

					cSetTmp.MoveNext() ;
				}

				cSetTmp.Close() ;
			}

			//begin 如果是最后一个信号机，此时要考虑其开放的情况
			if (iLxSection==iXhjCount2-1) 
			{
				//设置strCurXhj之后的所有道口 bt_mxb_import_lxstatus=1 表示道口能能行车（不能走人）
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_xhj='%s')"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), strCurXhj) ;
				ExecSqlString(strSql) ;

				//begin 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭	
				if (strLxZnBj!=_T(""))
				{
					strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_lx in(%s) and id%s%d"), iUpOrDown, strLxZnBj, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP) ;
					ExecSqlString(strSql) ;
				}
				//end 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭

				//查找起始轨道前一个轨道的basecode
				strRetBianMa = _T("") ;
				strSql.Format(_T("select * from bt_mxb_import where id%s(select top 1 id from bt_mxb_import where bt_mxb_import_lx<>'' and bt_mxb_import_belongxhj='%s' order by id %s) order by id %s"), iUpOrDown==0?_T(">"):_T("<"), strCurXhj, iUpOrDown==0?_T("desc"):_T("asc"), iUpOrDown==0?_T("asc"):_T("desc")) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (cSetTmp.GetRecordCount()>0)
					{
						cSetTmp.MoveFirst() ;
						cSetTmp.GetFieldValue(_T("bt_mxb_import_basecode"), strRetBianMa) ;
						cSetTmp.GetFieldValue(_T("id"), iGuiDaoIdLeftStart) ;
					}
					cSetTmp.Close() ;
				}

				strSql.Format(_T("select * from bt_mxb_import where id%s%d and id %s(select top 1 id from bt_mxb_import where bt_mxb_import_lx<>'' and bt_mxb_import_belongxhj='%s' order by id %s) order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP,iUpOrDown==0?_T("<="):_T(">="), strCurXhj, iUpOrDown==0?_T("desc"):_T("asc"), iUpOrDown==0?_T("desc"):_T("asc")) ;
				if (cSetTmp.IsOpen())
				{
					cSetTmp.Close() ;
				}
				if (cSetTmp.Open(g_PtrCon, strSql))
				{
					if (!cSetTmp.IsBOF())
					{
						cSetTmp.MoveFirst() ;
					}							
					while(!cSetTmp.IsEOF())
					{
						int iGuiDaoId ;
						CString strCurBianMa,strBianMaOld ;
						cSetTmp.GetFieldValue(_T("id"), iGuiDaoId) ;
						cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaOld) ;

						this->GetGuiDaoBianMa_QJ(iGuiDaoId, strRetBianMa, 0, iUpOrDown) ;
						strCurBianMa = strRetBianMa ;
						if (!strBianMaOld.IsEmpty())
						{
							strCurBianMa.Append(_T("/")) ;
							strCurBianMa.Append(strBianMaOld) ;									
						}
						cSetTmp.Edit() ;
						cSetTmp.SetFieldValue(_T("bt_mxb_import_code"), strCurBianMa) ;
						cSetTmp.Update() ;

						cSetTmp.MoveNext() ;
					}

					cSetTmp.Close() ;
				}
			}
			//end 如果是最后一个信号机，此时要考虑其开放的情况


		}
		//end 遍历strArrayXhj中信号机防护区段

		//////////////////////////////////////////////////////////////////////////
		//begin 有报警通知的区段之后的区段

		//iGuiDaoIdLeftStart所属的信号机防护区内道口设为开放，之后的所有道口设为关闭（对列车关闭）
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=0 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s%d"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdLeftStart) ;
		ExecSqlString(strSql) ;
		//OCP区段的道口要开放（对列车开放）
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_belongxhj='%s'"), iUpOrDown, strBelongXhjOCP) ;
		ExecSqlString(strSql) ;

		//begin 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭	
		if (strLxZnBj!=_T(""))
		{
			strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and bt_mxb_import_lx in(%s) and id%s%d"), iUpOrDown, strLxZnBj, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP) ;
			ExecSqlString(strSql) ;
		}
		//end 设置站内通知道口状态，只要排列了进路，这些道口就对车开放，车压过该道口所在的闭塞分区才对车关闭

		if (iType>0) //接车进正线、侧线、通过，占用后的道口均已开放（能行车）
		{
			strSql.Format(_T("update bt_mxb_import set bt_mxb_import_lxstatus=1 where bt_mxb_import_lx<>'' and bt_mxb_import_upordown=%d and id%s%d"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdOCP) ;
			ExecSqlString(strSql) ;
		}

		strSql.Format(_T("select * from bt_mxb_import where id%s%d and id%s%d and bt_mxb_import_upordown=%d order by id %s"), iUpOrDown==0?_T(">="):_T("<="), iGuiDaoIdLeftStart,  iUpOrDown==0?_T("<="):_T(">="), iGuiDaoIdLeftEnd, iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc")) ;
		if (cSetTmp.IsOpen())
		{
			cSetTmp.Close() ;
		}
		if (cSetTmp.Open(g_PtrCon, strSql))
		{
			if (!cSetTmp.IsBOF())
			{
				cSetTmp.MoveFirst() ;
			}
			strRetBianMa = _T("") ;
			while(!cSetTmp.IsEOF())
			{
				int iGuiDaoId ;
				CString strCurBianMa,strBianMaOld ;
				cSetTmp.GetFieldValue(_T("id"), iGuiDaoId) ;
				cSetTmp.GetFieldValue(_T("bt_mxb_import_code"), strBianMaOld) ;

				this->GetGuiDaoBianMa_QJ(iGuiDaoId, strRetBianMa, iType, iUpOrDown) ;
				strCurBianMa = strRetBianMa ;
				if (!strBianMaOld.IsEmpty())
				{
					strCurBianMa.Append(_T("/")) ;
					strCurBianMa.Append(strBianMaOld) ;									
				}
				cSetTmp.Edit() ;
				cSetTmp.SetFieldValue(_T("bt_mxb_import_code"), strCurBianMa) ;
				cSetTmp.Update() ;

				cSetTmp.MoveNext() ;
			}

			cSetTmp.Close() ;
		}


		//end 有报警通知的区段之后的区段


		//end 占据区段前方（列车运行方向为向前方）


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGuiDaoBianMaOCP")) ;
	}


	return 0;
}

void CMaXuBiaoForBT::JinLuBianMa_ZN(int iUpOrDown)
{
	CArray<CStringArray *, CStringArray *> arrayBianMa ;
	CStringArray strArrayDescribe ;
	CStringArray strArrayOCPGuiDao ;

	CString strRetBianMa = _T("") ;	

	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//清除basecode 和code字段
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_basecode='', bt_mxb_import_code='' where id>0")) ;
		ExecSqlString(strSql) ;

		//OHS,HS SS, ASS 
		int iIdOHS = 0, iIdHS = 0, iIdSS = 0, iIdASS = 0 ;
		CString strAssXhj =_T("") ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_xhjtype in('OHS','HS','SS','ASS') "), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while(!cSet.IsEOF())
				{
					CString strXhjTypeTmp = _T("") ;
					int iIdTmp = 0 ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhjtype"), strXhjTypeTmp) ;
					cSet.GetFieldValue(_T("id"), iIdTmp) ;
					if (strXhjTypeTmp==_T("OHS"))
					{
						iIdOHS = iIdTmp ;
					}
					if (strXhjTypeTmp==_T("HS"))
					{
						iIdHS = iIdTmp ;
					}
					if (strXhjTypeTmp==_T("SS"))
					{
						iIdSS = iIdTmp ;
					}
					if (strXhjTypeTmp==_T("ASS"))
					{
						iIdASS = iIdTmp ;
						cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strAssXhj) ;
					}
					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}


		//begin no route is set

		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			strRetBianMa = _T("") ;
			CString strPreXhjType = _T("") ;
			while(!cSet.IsEOF())
			{
				int iGuiDaoId = 0;
				CString strCurXhjType = _T("") ;
				CString strRetBianMaTmp = _T("") ;
				CString strBelongXhj = _T("") ;
				cSet.GetFieldValue(_T("id"), iGuiDaoId) ;				
				cSet.GetFieldValue(_T("bt_mxb_import_xhjtype"), strCurXhjType) ;
				cSet.GetFieldValue(_T("bt_mxb_import_belongxhj"), strBelongXhj) ;

				if (strCurXhjType==_T("ASS"))
				{
					strRetBianMa = _T("R") ;
					strRetBianMaTmp = strRetBianMa ;	
				}
				else if (strPreXhjType==_T("ASS")||strPreXhjType==_T("SS")||strPreXhjType==_T("HS"))
				{					
					strRetBianMa = _T("R") ;
					strRetBianMaTmp = _T("NC(R)") ;					
				}
				else
				{
					if (strBelongXhj==_T(""))
					{
						this->GetGuiDaoBianMa_ZN(iGuiDaoId, strRetBianMa, 0, 0, iUpOrDown) ;
					}
					else
					{
						this->GetGuiDaoBianMa_ZN(iGuiDaoId, strRetBianMa, 0, 1, iUpOrDown) ;
					}

					strRetBianMaTmp = strRetBianMa ;	
				}
				strPreXhjType = strCurXhjType ;
				
				cSet.Edit() ;
				cSet.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMaTmp) ;
				cSet.SetFieldValue(_T("bt_mxb_import_basecode"), strRetBianMaTmp) ;
				cSet.Update() ;

				cSet.MoveNext() ;
			}

			cSet.Close() ;
		}


		//将码从数据库拷入数组中
		CStringArray *strArrayBianMa = new CStringArray ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				CString strBianMaTmp ;				
				cSet.GetFieldValue(_T("bt_mxb_import_basecode"), strBianMaTmp) ;
				strArrayBianMa->Add(strBianMaTmp) ;

				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		arrayBianMa.Add(strArrayBianMa) ;
		strArrayDescribe.Add(_T("No Route is Set")) ;
		strArrayOCPGuiDao.Add(_T("")) ;

		//end no route is set

		//begin set call-on route
		//清除code字段
		strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
		ExecSqlString(strSql) ;
		//OCP-200s
		CStringArray *strArrayBianMa_200s = new CStringArray ;
		CString strOCPGuiDao_200s = _T("") ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				CString strBianMaTmp = _T(""), strXhjTypeTmp = _T("") ;				
				cSet.GetFieldValue(_T("bt_mxb_import_basecode"), strBianMaTmp) ;
				cSet.GetFieldValue(_T("bt_mxb_import_xhjtype"), strXhjTypeTmp) ;
				if (iCodeIndex==0&&iUpOrDown==0 || iCodeIndex==iRsCount-1&&iUpOrDown==1)
				{
					strBianMaTmp = _T("RY-Y3") ;
					cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strOCPGuiDao_200s) ;
				}
				else if(strXhjTypeTmp==_T("OHS"))
				{
					strBianMaTmp = _T("R-RY") ;
				}
				strArrayBianMa_200s->Add(strBianMaTmp) ;

				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		arrayBianMa.Add(strArrayBianMa_200s) ;
		strArrayDescribe.Add(_T("Set Call-on Route")) ;
		strArrayOCPGuiDao.Add(strOCPGuiDao_200s) ;
		//OCP-30s
		CStringArray *strArrayBianMa_30s = new CStringArray ;
		CString strOCPGuiDao_30s = _T("") ;
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				CString strBianMaTmp = _T(""), strXhjTypeTmp = _T("") ;				
				cSet.GetFieldValue(_T("bt_mxb_import_basecode"), strBianMaTmp) ;
				cSet.GetFieldValue(_T("bt_mxb_import_xhjtype"), strXhjTypeTmp) ;
				if (iCodeIndex==0&&iUpOrDown==0 || iCodeIndex==iRsCount-1&&iUpOrDown==1)
				{
					strBianMaTmp= _T("R") ;					
				}
				else if(strXhjTypeTmp==_T("OHS"))
				{
					strBianMaTmp = _T("RY-RW") ;
					cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strOCPGuiDao_30s) ;
				}
				strArrayBianMa_30s->Add(strBianMaTmp) ;

				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		arrayBianMa.Add(strArrayBianMa_30s) ;
		strArrayDescribe.Add(_T("Set Call-on Route")) ;
		strArrayOCPGuiDao.Add(strOCPGuiDao_30s) ;

		//end set call-on route

		//begin Set Reception Route to Loopline
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_xhjtype='HS') order by id %s"), iUpOrDown, iUpOrDown==0?_T("<"):_T(">"), iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc") ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iOCPGuiDaoId = 0 ;	
				CString strCurOCPGuiDao = _T("") ;
				cSet.GetFieldValue(_T("id"), iOCPGuiDaoId) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurOCPGuiDao) ;

				CADORecordset cSetLoopline ;
				//OCP后方
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("<"):_T(">"), iOCPGuiDaoId, iUpOrDown==0?_T("desc"):_T("asc") ) ;
				if (cSetLoopline.IsOpen())
				{
					cSetLoopline.Close() ;
				}
				if (cSetLoopline.Open(g_PtrCon, strSql))
				{
					if (!cSetLoopline.IsBOF())
					{
						cSetLoopline.MoveFirst() ;
					}
					strRetBianMa = _T("") ;
					while (!cSetLoopline.IsEOF())
					{
						int iGuiDaoId = 0 ;
						cSetLoopline.GetFieldValue(_T("id"), iGuiDaoId) ;
						this->GetGuiDaoBianMa_ZN(iGuiDaoId, strRetBianMa, 0, 0, iUpOrDown) ;

						cSetLoopline.Edit() ;
						cSetLoopline.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMa) ;						
						cSetLoopline.Update() ;

						cSetLoopline.MoveNext() ;
					}
					cSetLoopline.Close() ;
				}
				//OCP-HS
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s%d and id%s(select id from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_xhjtype='HS') order by id %s"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iOCPGuiDaoId, iUpOrDown==0?_T("<"):_T(">"),iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc") ) ;
				if (cSetLoopline.IsOpen())
				{
					cSetLoopline.Close() ;
				}
				if (cSetLoopline.Open(g_PtrCon, strSql))
				{
					if (!cSetLoopline.IsBOF())
					{
						cSetLoopline.MoveFirst() ;
					}
					strRetBianMa = _T("") ;
					while (!cSetLoopline.IsEOF())
					{
						int iGuiDaoId = 0 ;
						cSetLoopline.GetFieldValue(_T("id"), iGuiDaoId) ;
						this->GetGuiDaoBianMa_ZN(iGuiDaoId, strRetBianMa, 1, 0, iUpOrDown) ;

						cSetLoopline.Edit() ;
						cSetLoopline.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMa) ;						
						cSetLoopline.Update() ;

						cSetLoopline.MoveNext() ;
					}
					cSetLoopline.Close() ;
				}
				//HS-ASS
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_xhjtype='HS') order by id %s"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc") ) ;
				if (cSetLoopline.IsOpen())
				{
					cSetLoopline.Close() ;
				}
				if (cSetLoopline.Open(g_PtrCon, strSql))
				{
					if (!cSetLoopline.IsBOF())
					{
						cSetLoopline.MoveFirst() ;
					}
					strRetBianMa = _T("") ;
					CString strPreXhjType = _T("") ;
					while (!cSetLoopline.IsEOF())
					{
						int iGuiDaoId = 0;
						CString strCurXhjType = _T("") ;
						CString strRetBianMaTmp = _T("") ;
						cSetLoopline.GetFieldValue(_T("id"), iGuiDaoId) ;				
						cSetLoopline.GetFieldValue(_T("bt_mxb_import_xhjtype"), strCurXhjType) ;

						if (strCurXhjType==_T("ASS"))
						{
							strRetBianMa = _T("R") ;
							strRetBianMaTmp = strRetBianMa ;	
						}
						else if (strPreXhjType==_T("ASS")||strPreXhjType==_T("SS")||strPreXhjType==_T("HS"))
						{					
							strRetBianMa = _T("R") ;
							strRetBianMaTmp = _T("NC(R)") ;					
						}
						else
						{
							this->GetGuiDaoBianMa_ZN(iGuiDaoId, strRetBianMa, 0, 1, iUpOrDown) ;
							strRetBianMaTmp = strRetBianMa ;	
						}
						strPreXhjType = strCurXhjType ;

						cSetLoopline.Edit() ;
						cSetLoopline.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMaTmp) ;						
						cSetLoopline.Update() ;

						cSetLoopline.MoveNext() ;
					}
					cSetLoopline.Close() ;
				}

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMa = new CStringArray ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetLoopline.IsOpen())
				{
					cSetLoopline.Close() ;
				}
				if (cSetLoopline.Open(g_PtrCon, strSql))
				{					
					if (!cSetLoopline.IsBOF())
					{
						cSetLoopline.MoveFirst() ;
					}
					
					while(!cSetLoopline.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetLoopline.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;
						strArrayBianMa->Add(strBianMaTmp) ;						
						cSetLoopline.MoveNext() ;
					}
					cSetLoopline.Close() ;
				}
				arrayBianMa.Add(strArrayBianMa) ;
				strArrayDescribe.Add(_T("Set Reception Route to Loopline")) ;
				strArrayOCPGuiDao.Add(strCurOCPGuiDao) ;

				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

		//end Set Reception Route to Loopline

		//begin Set Reception Route to Mainline
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s(select id from bt_mxb_import where bt_mxb_import_upordown=%d and bt_mxb_import_xhjtype='SS') order by id %s"), iUpOrDown, iUpOrDown==0?_T("<"):_T(">"), iUpOrDown, iUpOrDown==0?_T("asc"):_T("desc") ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iOCPGuiDaoId = 0 ;	
				CString strCurOCPGuiDao = _T("") ;
				cSet.GetFieldValue(_T("id"), iOCPGuiDaoId) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurOCPGuiDao) ;

				CADORecordset cSetMainline ;
		
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc") ) ;
				if (cSetMainline.IsOpen())
				{
					cSetMainline.Close() ;
				}
				if (cSetMainline.Open(g_PtrCon, strSql))
				{
					if (!cSetMainline.IsBOF())
					{
						cSetMainline.MoveFirst() ;
					}
					strRetBianMa = _T("") ;
					while (!cSetMainline.IsEOF())
					{
						int iGuiDaoIdTmp = 0 ;
						cSetMainline.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
						CString strRetBianMaTmp = _T("") ;
						
						if (iUpOrDown==0)
						{
							if(iGuiDaoIdTmp==iIdASS)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = strRetBianMa ;
							}
							else if (iGuiDaoIdTmp==iIdASS-1)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = _T("NC(R)") ;
							}
							else if (iGuiDaoIdTmp>=iIdHS &&iGuiDaoIdTmp>=iOCPGuiDaoId)
							{
								this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
								strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
							}
							else if (iGuiDaoIdTmp>=iIdHS && iGuiDaoIdTmp<iOCPGuiDaoId)
							{
								strRetBianMa = strRetBianMaTmp = _T("NC") ;
							}
							else if (iGuiDaoIdTmp==iIdHS-1)
							{
								if (iOCPGuiDaoId>=iIdHS)
								{
									strRetBianMa = _T("R") ;
								}
								else
								{
									strRetBianMa = _T("Y") ;
								}
								strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
							}
							else if (iGuiDaoIdTmp<iIdHS)
							{
								if (iOCPGuiDaoId>=iIdHS)
								{
									this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
								}
								else
								{
									this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 2, 0, iUpOrDown) ;
								}
								if(iGuiDaoIdTmp==iOCPGuiDaoId-1)
								{
									strRetBianMa = _T("R") ;
								}
								strRetBianMaTmp = strRetBianMa ;
								if (iGuiDaoIdTmp>=iIdOHS)
								{
									strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
								}
								
							}
						}
						else
						{
							if(iGuiDaoIdTmp==iIdASS)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = strRetBianMa ;
							}
							else if (iGuiDaoIdTmp==iIdASS+1)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = _T("NC(R)") ;
							}
							else if (iGuiDaoIdTmp<=iIdHS &&iGuiDaoIdTmp<=iOCPGuiDaoId)
							{
								this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
								strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
							}
							else if (iGuiDaoIdTmp<=iIdHS && iGuiDaoIdTmp>iOCPGuiDaoId)
							{
								strRetBianMa = strRetBianMaTmp = _T("NC") ;
							}
							else if (iGuiDaoIdTmp==iIdHS+1)
							{
								if (iOCPGuiDaoId<=iIdHS)
								{
									strRetBianMa = _T("R") ;
								}
								else
								{
									strRetBianMa = _T("Y") ;
								}
								strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
							}
							else if (iGuiDaoIdTmp>iIdHS)
							{
								if (iOCPGuiDaoId<=iIdHS)
								{
									this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
								}
								else
								{
									this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 2, 0, iUpOrDown) ;
								}
								if(iGuiDaoIdTmp==iOCPGuiDaoId+1)
								{
									strRetBianMa = _T("R") ;
								}
								strRetBianMaTmp = strRetBianMa ;
								if (iGuiDaoIdTmp<=iIdOHS)
								{
									strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
								}
							}
						}

						cSetMainline.Edit() ;
						cSetMainline.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMaTmp) ;						
						cSetMainline.Update() ;

						cSetMainline.MoveNext() ;
					}
					cSetMainline.Close() ;
				}

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMa = new CStringArray ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetMainline.IsOpen())
				{
					cSetMainline.Close() ;
				}
				if (cSetMainline.Open(g_PtrCon, strSql))
				{					
					if (!cSetMainline.IsBOF())
					{
						cSetMainline.MoveFirst() ;
					}

					while(!cSetMainline.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetMainline.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;
						strArrayBianMa->Add(strBianMaTmp) ;						
						cSetMainline.MoveNext() ;
					}
					cSetMainline.Close() ;
				}
				arrayBianMa.Add(strArrayBianMa) ;
				strArrayDescribe.Add(_T("Set Reception Route to Mainline")) ;
				strArrayOCPGuiDao.Add(strCurOCPGuiDao) ;

				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}
		//end Set Reception Route to Mainline

		//begin Stop at the Rear of ASS (ASS is Red)
		strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d and id%s%d and id%s%d order by id %s"), iUpOrDown, iUpOrDown==0?_T(">="):_T("<="), iUpOrDown==0?iIdSS-1:iIdSS+1,iUpOrDown==0?_T("<"):_T(">"), iIdASS, iUpOrDown==0?_T("asc"):_T("desc") ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (!cSet.IsBOF())
			{
				cSet.MoveFirst() ;
			}
			int iCodeIndex = 0 ;
			while(!cSet.IsEOF())
			{
				//清除code字段
				strSql.Format(_T("update bt_mxb_import set bt_mxb_import_code='' where id>0")) ;
				ExecSqlString(strSql) ;

				int iOCPGuiDaoId = 0 ;	
				CString strCurOCPGuiDao = _T("") ;
				cSet.GetFieldValue(_T("id"), iOCPGuiDaoId) ;
				cSet.GetFieldValue(_T("bt_mxb_import_guidao"), strCurOCPGuiDao) ;

				CADORecordset cSetOneRoute ;

				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id %s"), iUpOrDown, iUpOrDown==0?_T("desc"):_T("asc") ) ;
				if (cSetOneRoute.IsOpen())
				{
					cSetOneRoute.Close() ;
				}
				if (cSetOneRoute.Open(g_PtrCon, strSql))
				{
					if (!cSetOneRoute.IsBOF())
					{
						cSetOneRoute.MoveFirst() ;
					}
					strRetBianMa = _T("") ;
					while (!cSetOneRoute.IsEOF())
					{
						int iGuiDaoIdTmp = 0 ;
						cSetOneRoute.GetFieldValue(_T("id"), iGuiDaoIdTmp) ;
						CString strRetBianMaTmp = _T("") ;

						if (iUpOrDown==0)
						{
							if(iGuiDaoIdTmp==iIdASS)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = strRetBianMa ;
							}
							else if (iGuiDaoIdTmp>=iOCPGuiDaoId)
							{
								if (iGuiDaoIdTmp==iOCPGuiDaoId && iGuiDaoIdTmp==iIdSS-1)
								{
									strRetBianMa = _T("Y3") ;
									strRetBianMaTmp = _T("NC(Y3)") ;
								}
								else
								{
									this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
									strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
								}
							}
							else if (iGuiDaoIdTmp>=iIdHS &&  iGuiDaoIdTmp<iOCPGuiDaoId)
							{
								if (iGuiDaoIdTmp==iIdSS-1)
								{
									strRetBianMa = _T("R") ;
									strRetBianMaTmp = _T("NC(R)") ;
								}
								else 
								{
									strRetBianMa = strRetBianMaTmp = _T("NC") ;
								}
							}
							else if (iGuiDaoIdTmp==iIdHS-1)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = _T("NC(R)") ;
							}
							else if(iGuiDaoIdTmp<iIdHS)
							{
								this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
								strRetBianMaTmp = strRetBianMa ;
								if(iGuiDaoIdTmp>=iIdOHS)
								{
									strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
								}							
							}
						}
						else
						{
							if(iGuiDaoIdTmp==iIdASS)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = strRetBianMa ;
							}
							else if (iGuiDaoIdTmp<=iOCPGuiDaoId)
							{
								if (iGuiDaoIdTmp==iOCPGuiDaoId && iGuiDaoIdTmp==iIdSS+1)
								{
									strRetBianMa = _T("Y3") ;
									strRetBianMaTmp = _T("NC(Y3)") ;
								}
								else
								{
									this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
									strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
								}
							}
							else if (iGuiDaoIdTmp<=iIdHS && iGuiDaoIdTmp>iOCPGuiDaoId)
							{
								if (iGuiDaoIdTmp==iIdSS+1)
								{
									strRetBianMa = _T("R") ;
									strRetBianMaTmp = _T("NC(R)") ;
								}
								else 
								{
									strRetBianMa = strRetBianMaTmp = _T("NC") ;
								}
							}
							else if (iGuiDaoIdTmp==iIdHS+1)
							{
								strRetBianMa = _T("R") ;
								strRetBianMaTmp = _T("NC(R)") ;
							}
							else if(iGuiDaoIdTmp>iIdHS)
							{
								this->GetGuiDaoBianMa_ZN(iGuiDaoIdTmp, strRetBianMa, 0, 0, iUpOrDown) ;
								strRetBianMaTmp = strRetBianMa ;
								if(iGuiDaoIdTmp<=iIdOHS)
								{
									strRetBianMaTmp.Format(_T("NC(%s)"), strRetBianMa) ;
								}							
							}
						}

						cSetOneRoute.Edit() ;
						cSetOneRoute.SetFieldValue(_T("bt_mxb_import_code"), strRetBianMaTmp) ;						
						cSetOneRoute.Update() ;

						cSetOneRoute.MoveNext() ;
					}
					cSetOneRoute.Close() ;
				}

				//将码从数据库拷入数组中
				CStringArray *strArrayBianMa = new CStringArray ;
				strSql.Format(_T("select * from bt_mxb_import where bt_mxb_import_upordown=%d order by id asc"), iUpOrDown) ;
				if (cSetOneRoute.IsOpen())
				{
					cSetOneRoute.Close() ;
				}
				if (cSetOneRoute.Open(g_PtrCon, strSql))
				{					
					if (!cSetOneRoute.IsBOF())
					{
						cSetOneRoute.MoveFirst() ;
					}

					while(!cSetOneRoute.IsEOF())
					{
						CString strBianMaTmp ;				
						cSetOneRoute.GetFieldValue(_T("bt_mxb_import_code"), strBianMaTmp) ;
						strArrayBianMa->Add(strBianMaTmp) ;						
						cSetOneRoute.MoveNext() ;
					}
					cSetOneRoute.Close() ;
				}
				arrayBianMa.Add(strArrayBianMa) ;
				CString strDescribe ;
				strDescribe.Format(_T("Stop at the Rear of ASS (%s is Red)"), strAssXhj) ;
				strArrayDescribe.Add(strDescribe) ;
				strArrayOCPGuiDao.Add(strCurOCPGuiDao) ;

				iCodeIndex++ ;
				cSet.MoveNext() ;
			}
			cSet.Close() ;
		}

		//end Stop at the Rear of ASS (ASS is Red) 


		g_pCurDb = new AcDbDatabase ;
		g_cSystem.m_pDb = g_pCurDb ;
		CString strFileName ;
		strFileName.Format(_T("%s-%s--normal.dwg"), m_strStation, iUpOrDown==0?_T("down"):_T("up")) ;
		//绘制码序表
		this->DrawBianMaTable(arrayBianMa, strArrayOCPGuiDao, strArrayDescribe,iUpOrDown) ;	
		g_pCurDb->saveAs(strFileName) ;
		delete g_pCurDb ;
		g_pCurDb = NULL ;


		//清理new对象
		if (arrayBianMa.GetSize()>0)
		{
			for(int iBianMa=0; iBianMa<arrayBianMa.GetSize(); iBianMa++)
			{
				delete arrayBianMa[iBianMa] ;
				arrayBianMa[iBianMa] = NULL ;
			}
		}


	}	
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		sBuff.Format(_T("%s-JinLuBianMa_ZN"),sBuff) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error JinLuBianMa_ZN")) ;
	}
}

BOOL CMaXuBiaoForBT::GetGuiDaoBianMa_ZN(int iGuiDaoId, CString & strRetBianMa, int iCodeType, int iType,  int iUpOrDown)
{
	CStringList *strListMaXu = NULL;
	switch (iCodeType)
	{
	case 0:
		strListMaXu = &m_strListMaXu ;
		break;
	case 1:
		strListMaXu = &m_strListMaXuRecForLoop ;
		break;
	case 2:
		strListMaXu = &m_strListMaXuRecForMain ;
		break;
	case 3:
		strListMaXu = &m_strListMaXuRecForThrough ;
		break;
	default:
		strListMaXu = &m_strListMaXu ;
		break;
	}

	if (strRetBianMa==_T(""))
	{
		strRetBianMa = strListMaXu->GetHead() ;

	}
	else
	{
		POSITION posCur, posEnd ;
		posCur = strListMaXu->Find(strRetBianMa) ;
		posEnd = strListMaXu->GetTailPosition() ;

		try
		{
			CString strSql ;
			CADORecordset cSet ;	

			if (iType==1)
			{
				strRetBianMa = _T("NC") ;
			}

			//begin 前一个轨道是否是信号机防护的第一区段（跨过信号机则编码变化）
			strSql.Format(_T("select * from bt_mxb_import where id=%d"), iUpOrDown==0?iGuiDaoId+1:iGuiDaoId-1) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()>0)
				{
					cSet.MoveFirst() ;
					int iPreGuiDaoId = 0 ;
					CString strPreGuidaoXhj ;
					cSet.GetFieldValue(_T("id"), iPreGuiDaoId) ;
					cSet.GetFieldValue(_T("bt_mxb_import_xhj"), strPreGuidaoXhj) ;
					if (!strPreGuidaoXhj.IsEmpty())
					{
						if (posEnd!=posCur) //当前已经达到G，后续编码都为G
						{
							strListMaXu->GetNext(posCur) ;
							strRetBianMa = strListMaXu->GetAt(posCur) ;							
						}
					}
				}                

				cSet.Close() ;
			}
			//end 前一个轨道是否是信号机防护的第一区段（跨过信号机则编码变化）


	
		}
		catch (_com_error& e)
		{
			CString sBuff = CBlkUtility::GetErrorDescription(e) ;
			AfxMessageBox(sBuff) ;  
		}
		catch (...)
		{
			AfxMessageBox(_T("unknown error in GetGuiDaoBianMa_ZN")) ;
		}


	}

	return 0;
}
