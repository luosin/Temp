#include "stdafx.h"
#include "zndmh.h"

#include "DBUtility.h"

#include "ConNode.h"

extern BOOL bFirst ;

// 电化局组合柜，无综合柜
void CZnDmh::DrawZD(const CString &strSavePath)
{
	AcDbDatabase *pDb = new AcDbDatabase() ;
	g_cSystem.m_pDb = pDb ;
	CString strSaveFile ;
	strSaveFile.Format(_T("%s\\组合柜布置图.dwg"), strSavePath) ;
	CBlkUtility::GetBasePath(m_strSysBasePath) ;//系统路径

	double dMainTbWidth = 188.000 ; //组合柜主框宽度
	double dMainTbRowHeight = 20.000 ; //每层高度
	double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 38.000 ; //左侧序号、组合类型等列所占宽度（从框架左边框到组合单元的距离）
	double dCellWidth = 15.000 ; //组合单元格宽度
	double dCellWidthR = 10.000 ; //ZDTR单元格宽度
	double dCellWidthZBPU = 18.750 ; //ZBPU单元格宽度
	double dCellWidthHLP = 9.375 ;//150/16,若单元格变化此值随着变
	double dBetweenMainTb = 5.000 ; //Z1和Z2之间的距离
	double dFrameBaseX = -30.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -40.000 ; //左Frame的插入基点y

	try
	{
		//将数据读入array
		CADORecordset cSet ;
		CString strSql ;

		int iZIndex = 0 ; //组合架序号
		int iZLocRow = 10 ; //组合架内层号，10~1
		//int iZCellIndex = 1 ; //每个组合内继电器位置序号，从1~10

		AcDbBlockReference *pRef = NULL ;
		CString strBlockName = _T("") ;
		AcGePoint3d pt_3d_insert(0,0,0) ;
		CStringArray strArrayVal ; //一般用于块属性值设置
		CStringArray strArrayRet ; //一般用于查询结果保存
		CLongArray nArrayRet ; //一般用于查询结果保存

		//先插入一个组合架
		strBlockName = _T("dmh_box_zzh_main") ;
		pt_3d_insert.x = pt_3d_insert.y = 0 ;		
		strArrayVal.RemoveAll() ;
		CString strZIndex ;
		strZIndex.Format(_T("%s%d"),m_strPreZ, iZIndex+1) ;
		strArrayVal.Add(strZIndex) ;
		strArrayVal.Add(_T("站内组合柜")) ;			
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		//在第一个组合架下面插入注释:"+"号表示受电端
		strBlockName = _T("dmh_circuit_note_0002_0") ;
		pt_3d_insert.y -= 9  ;		
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		//begin FBJ
		BOOL bHasSetPlusFBJ = FALSE ; //是否设置过了+1FBJ
		for (int i=0; i<2; i++)
		{
			strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where instr(dmh_import_line,'%s行')>0 and instr(dmh_import_line,'半自动接近')=0 and dmh_import_table=0 order by dmh_import_lineindex"), i==0?_T("上"):_T("下")) ; //先上行线
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iSLineNum = cSet.GetRecordCount() ; //有几条上行线（从table 0读取的都是正线）就有几条FB组合
				if (iSLineNum>0)
				{
					cSet.MoveFirst() ;
					while (!cSet.IsEOF())
					{
						CString strLine ;
						cSet.GetFieldValue(_T("dmh_import_line"), strLine) ;
						CString strJZ ;
						strArrayRet.RemoveAll() ;
						this->GetLineInfo(strLine, strJZ, strArrayRet) ;

						CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
						strBlockName = _T("dmh_boxd_z_fb") ;
						pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
						pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
						strArrayVal.RemoveAll() ;
						CString strJZTmp ;
						strJZTmp.Format(_T("(%s)"), strJZ) ;
						strArrayVal.Add(strJZTmp) ;												
						if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}

						//插入组合继电器单元
						strBlockName = _T("dmh_boxd_z_cell") ;
						if (strArrayRet.GetCount()==2) //上行线只有SJM SFM/XFJM,下行线只有XJM XFM/SFJM
						{
							CString strCell ;
							for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
							{	
								CString strCell ;
								if (i==1&&iCellIndex==0&&bHasSetPlusFBJ==FALSE)//+1FBJ写在X行
								{
									bHasSetPlusFBJ = TRUE ;
									strCell = _T("+1") ;
								}
								else if (iCellIndex==0)
								{
									continue;
								}
								
								if (iCellIndex>0)
								{
									if (iCellIndex<3)
									{
										strCell = strArrayRet.GetAt(0) ;
									}
									else if (iCellIndex<5)
									{
										strCell = strArrayRet.GetAt(1) ;
									}
									else
									{
										strCell = strJZ ;
									}
								}

								pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
								pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
								strArrayVal.RemoveAll() ;								
								strArrayVal.Add(strCell) ;								
								if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
									pRef->close() ;
									pRef = NULL ;
								}
							}
						}

						iZLocRow-- ;

						cSet.MoveNext() ;
					}
				}

				cSet.Close() ;
			}
		}
		//end FBJ

		//begin BM
		for (int i=0; i<2; i++)
		{
			strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where instr(dmh_import_line,'%s行')>0 and instr(dmh_import_line,'半自动接近')=0 and dmh_import_table=0 order by dmh_import_lineindex"), i==0?_T("上"):_T("下")) ; //先上行线
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				int iSLineNum = cSet.GetRecordCount() ; //有几条上行线（从table 0读取的都是正线）就有几条BM组合
				if (iSLineNum>0)
				{
					cSet.MoveFirst() ;
					while (!cSet.IsEOF())
					{
						CString strLine ;
						cSet.GetFieldValue(_T("dmh_import_line"), strLine) ;
						CString strJZ, strFCZ, strCZ, strFJZ ;
						this->GetLineInfo(strLine, strJZ, strFCZ, strCZ, strFJZ) ;

						CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
						strBlockName = _T("dmh_boxd_z_bm") ;
						pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
						pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
						strArrayVal.RemoveAll() ;
						CString strJZTmp ;
						strJZTmp.Format(_T("(%s)"), strJZ) ;
						strArrayVal.Add(strJZTmp) ;														
						if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
						{
							pRef->close() ;
							pRef = NULL ;
						}
						//插入组合继电器单元
						strBlockName = _T("dmh_boxd_z_cell") ;
						CString strCell ;
						for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
						{
							if (iCellIndex<3)
							{
								strCell = strJZ ;
							}
							else if (iCellIndex<6)
							{
								strCell = strFJZ ;
							}
							else if (iCellIndex<8)
							{
								strCell = strCZ ;
							}
							else	
							{
								strCell = strFCZ ;
							}

							pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
							strArrayVal.RemoveAll() ;								
							strArrayVal.Add(strCell) ;								
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
								pRef->close() ;
								pRef = NULL ;
							}
						}

						iZLocRow-- ;

						cSet.MoveNext() ;
					}
				}

				cSet.Close() ;
			}
		}

		//end BM
		
		//begin GC
		CStringArray strArrayGuiDao, strArrayGCJName ; //这2个数值是对应的，例：strArraGuiDao[0]存储的IBG，其对应的就是strArrayGCJName[0]存储的SF1GCJ
		int iRetSum = 0 ;
		//SF XF
		strArrayRet.RemoveAll() ;
		strSql.Format(_T("select distinct dmh_import_type, dmh_import_line, dmh_import_lineindex from dmh_import where dmh_import_table=0 and dmh_import_type='反接' order by dmh_import_lineindex asc")) ;
		iRetSum = CDBUtility::GetRecordValue(strSql, _T("dmh_import_line"), strArrayRet) ;
		for (int i=0; i<iRetSum; i++)
		{
			//查询该进路所有轨道
			strSql.Format(_T("select * from dmh_import where dmh_import_table=0 and dmh_import_type='反接' and dmh_import_line='%s' and dmh_import_cellrow<17 order by dmh_import_cellrow desc"), strArrayRet.GetAt(i)) ;
			int iGuiDaoCountTmp = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuiDao) ;
			
			CString strJZ, strFCZ, strCZ, strFJZ ;
			this->GetLineInfo(strArrayRet.GetAt(i), strJZ, strFCZ, strCZ, strFJZ) ;
			CString strGCJ ;
			for (int iGCJIndex=0; iGCJIndex<iGuiDaoCountTmp; iGCJIndex++)
			{
				strGCJ.Format(_T("%s%dGCJ"), strFJZ, iGCJIndex+1) ;
				strArrayGCJName.Add(strGCJ) ;
			}
		}
		//S(S->XF包括SJM SFM整条线所有轨道) X
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where instr(dmh_import_line,'半自动接近')=0 and dmh_import_table=0  order by dmh_import_lineindex desc")) ;
		strArrayRet.RemoveAll() ;
		iRetSum = CDBUtility::GetRecordValue(strSql, _T("dmh_import_line"), strArrayRet) ;
		for (int i=0; i<iRetSum; i++)
		{
			//查询该进路所有轨道
			strSql.Format(_T("select * from dmh_import where dmh_import_table=0 and dmh_import_type in('正接','正发') and dmh_import_line='%s' and dmh_import_cellrow<17 order by dmh_import_cellcol asc ,dmh_import_cellrow desc"), strArrayRet.GetAt(i)) ;
			int iGuiDaoCountTmp = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuiDao) ;

			CString strJZ, strFCZ, strCZ, strFJZ ;
			this->GetLineInfo(strArrayRet.GetAt(i), strJZ, strFCZ, strCZ, strFJZ) ;
			CString strGCJ ;
			for (int iGCJIndex=0; iGCJIndex<iGuiDaoCountTmp; iGCJIndex++)
			{
				strGCJ.Format(_T("%s%dGCJ"), strJZ, iGCJIndex+1) ;
				strArrayGCJName.Add(strGCJ) ;
			}
		}
		//绘制GC组合
		int iGuiDaoCount = strArrayGuiDao.GetCount() ;
		int iNeedRow_GC = iGuiDaoCount/10+(iGuiDaoCount%10==0?0:1) ;
		for (int i=0; i<iNeedRow_GC; i++)
		{
			//插GC
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_gc") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_boxd_z_cell2") ;
			for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight/2 ;
				strArrayVal.RemoveAll() ;
				CString strCell, strGCJName ;
				if ((i*10+iCellIndex)<iGuiDaoCount)
				{
					strCell = strArrayGuiDao.GetAt(i*10+iCellIndex) ;
					strGCJName = strArrayGCJName.GetAt(i*10+iCellIndex) ;
					strArrayVal.Add(strCell) ;	
					strArrayVal.Add(strGCJName) ;
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						g_cSystem.SetAttHeight(pRef, 2, 0.6, _T("JDQNAME")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
			iZLocRow-- ;
		}
		//end GC

		//begin GJF
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where dmh_import_table=0 and instr(dmh_import_line,'半自动接近')=0 order by dmh_import_lineindex desc")) ;
		strArrayRet.RemoveAll() ;
		iRetSum = CDBUtility::GetRecordValue(strSql, _T("dmh_import_line"), strArrayRet) ;
		for (int i=0; i<iRetSum; i++)
		{
			CString strJZ, strFCZ, strCZ, strFJZ ;
			this->GetLineInfo(strArrayRet.GetAt(i), strJZ, strFCZ, strCZ, strFJZ) ;

			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_gjf") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			strArrayVal.RemoveAll() ;
			CString strJZTmp ;
			strJZTmp.Format(_T("(%s)"), strJZ) ;
			strArrayVal.Add(strJZTmp) ;													
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//查询该进路所有轨道
			strSql.Format(_T("select * from dmh_import where dmh_import_table=0 and dmh_import_type in('正接','正发') and dmh_import_line='%s' and dmh_import_cellrow<17 order by dmh_import_cellcol asc ,dmh_import_cellrow desc"), strArrayRet.GetAt(i)) ;
			strArrayGuiDao.RemoveAll() ;
			iGuiDaoCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuiDao) ;
			//插入cell
			strBlockName = _T("dmh_boxd_z_cell") ;
			BOOL bPassGuDao = FALSE ; //股道前轨道均后移一位（第一位被股道占了），股道后的轨道保持不变（股道跑第一位去了,各轨道又前移一位）
			int iGuiDaoCountTmp = (iGuiDaoCount>10?10:iGuiDaoCount) ;
			for (int iCellIndex=0; iCellIndex<iGuiDaoCountTmp; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2+(bPassGuDao?0:dCellWidth) ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;
				strCell = strArrayGuiDao.GetAt(iCellIndex) ;
				if (strCell.Left(1)==_T("*")) //股道写在第一个cell,此处有个可能，股道不在前10个
				{
					bPassGuDao = TRUE ;
					pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth/2 ;
				}
				strArrayVal.Add(strCell) ;							
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
					pRef->close() ;
					pRef = NULL ;
				}
			}
			iZLocRow-- ;

			//若轨道区段数iGuiDaoCount>10,需要单独再增加一个GJF组合(20131206:杨明、梅梅定)
			if (iGuiDaoCount>10)
			{
				CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
				strBlockName = _T("dmh_boxd_z_gjf") ;
				pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
				strArrayVal.RemoveAll() ;				
				strArrayVal.Add(strJZTmp) ;													
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					pRef->close() ;
					pRef = NULL ;
				}

				//插入cell
				strBlockName = _T("dmh_boxd_z_cell") ;
				for (int iCellIndex=0; iCellIndex<(iGuiDaoCount-10); iCellIndex++)
				{							
					pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
					pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
					strArrayVal.RemoveAll() ;
					CString strCell ;
					strCell = strArrayGuiDao.GetAt(10+iCellIndex) ; 
					strArrayVal.Add(strCell) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
				iZLocRow-- ;
			}

		}


		//end GJF

		//begin ZDTR
		//XJM XFM
		strArrayRet.RemoveAll() ;
		strArrayGuiDao.RemoveAll() ;
		strSql.Format(_T("select distinct dmh_import_jinluname,dmh_import_table,dmh_import_cellcol from dmh_import order by dmh_import_table asc, dmh_import_cellcol asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					CString strJinLuName ;	
					int iTableIndex = 0 ;
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					cSet.GetFieldValue(_T("dmh_import_table"), iTableIndex) ;

					strSql.Format(_T("select distinct dmh_import_guidao, dmh_import_cellrow from dmh_import where  dmh_import_jinluname='%s' and dmh_import_cellrow<%d order by dmh_import_cellrow desc"), strJinLuName, iTableIndex==0?17:16) ;
					CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuiDao) ;

					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
		//绘制ZDTR组合
		iGuiDaoCount = strArrayGuiDao.GetCount() ;
		int iNeedRow_ZDTR = iGuiDaoCount/15+(iGuiDaoCount%15==0?0:1) ;
		for (int i=0; i<iNeedRow_ZDTR; i++)
		{
			//插R
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_zdtr") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_boxd_z_cell") ;
			for (int iCellIndex=0; iCellIndex<15; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidthR*iCellIndex+dCellWidthR/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;
				if ((i*15+iCellIndex)<iGuiDaoCount)
				{
					strCell = strArrayGuiDao.GetAt(i*15+iCellIndex) ;
					strArrayVal.Add(strCell) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
			iZLocRow-- ;
		}
		//end ZDTR

		//begin ZC
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where dmh_import_table=1 and dmh_import_line='中岔'   order by dmh_import_lineindex asc")) ;
		nArrayRet.RemoveAll() ;
		iRetSum = CDBUtility::GetRecordValue(strSql, _T("dmh_import_lineindex"), nArrayRet) ;
		for (int i=0; i<iRetSum; i++)
		{	
			//插入一个ZC组合(LXJF FBJ LXJF FBJ GJF2 GJF2 GJF2)
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_zc") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}

			//插入信号机 X3 X3 S3 S3 
			strSql.Format(_T("select distinct dmh_import_xhj, dmh_import_line, dmh_import_lineindex, dmh_import_cellcol from dmh_import where dmh_import_table=1 and dmh_import_line='中岔' and dmh_import_lineindex=%d order by dmh_import_cellcol desc"), nArrayRet.GetAt(i)) ;
			strArrayRet.RemoveAll() ;
			int iXhjCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_xhj"), strArrayRet) ;
			for (int iXhjIndex=0; iXhjIndex<iXhjCount; iXhjIndex++)
			{
				for (int iCellIndex=0; iCellIndex<2; iCellIndex++)
				{
					pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*2*iXhjIndex+dCellWidth*iCellIndex+dCellWidth/2 ;
					pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
					strBlockName = _T("dmh_boxd_z_cell") ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strArrayRet.GetAt(iXhjIndex)) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
			//插入轨道 3G 32-34DG 3BG
			strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_lineindex=%d and dmh_import_xhj='%s' and dmh_import_cellrow<16 order by dmh_import_cellrow desc"), nArrayRet.GetAt(i), strArrayRet.GetAt(0)) ;
			strArrayRet.RemoveAll() ;
			int iGuiDaoCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayRet) ;
			for (int iGuiDaoIndex=0; iGuiDaoIndex<iGuiDaoCount; iGuiDaoIndex++)
			{
				CString strGuiDao ;
				strGuiDao = strArrayRet.GetAt(iGuiDaoIndex) ;
				strGuiDao.TrimRight(_T('+')) ;
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*(4+iGuiDaoIndex)+dCellWidth/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strBlockName = _T("dmh_boxd_z_cell") ;
				strArrayVal.RemoveAll() ;			
				strArrayVal.Add(strGuiDao) ;							
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
					pRef->close() ;
					pRef = NULL ;
				}
			}

			iZLocRow-- ;
		}
		//end ZC
		
		//begin GD
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where dmh_import_table=1 and dmh_import_line='同一股道' order by dmh_import_lineindex asc")) ;
		nArrayRet.RemoveAll() ;
		iRetSum = CDBUtility::GetRecordValue(strSql, _T("dmh_import_lineindex"), nArrayRet) ;
		for (int i=0; i<iRetSum; i++)
		{
			//一个GD组合放2个股道（X1 X1 S1 S1 1G）
			if (i%2==0)   //遇双放置一个GD组合
			{
				//插R
				CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
				strBlockName = _T("dmh_boxd_z_gd") ;
				pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
				{
					pRef->close() ;
					pRef = NULL ;
				}
				//iZLocRow-- ; //挪到添加cell后面,以免影响cell的y坐标
			}

			CString strCZ_X, strCZ_S, strGD ;
			strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_lineindex=%d and dmh_import_line='同一股道' order by dmh_import_cellcol desc"), nArrayRet.GetAt(i)) ;
			strArrayRet.RemoveAll() ;
			int iXhjCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_xhj"), strArrayRet) ;
			if (iXhjCount==2)
			{
				strCZ_X = strArrayRet.GetAt(0) ;
				strCZ_S = strArrayRet.GetAt(1) ;

				strArrayRet.RemoveAll() ;
				int iGuiDaoCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayRet) ;
				if (iGuiDaoCount>0)
				{
					strGD = strArrayRet.GetAt(0) ;
					strGD.Trim(_T("+")) ;
					CString strCells[5] = {strCZ_X, strCZ_X, strCZ_S, strCZ_S, strGD} ;
					for (int iCellIndex=0; iCellIndex<5; iCellIndex++)
					{
						pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*(iCellIndex+(i%2==0?0:5))+dCellWidthR/2 ;
						pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
						strBlockName = _T("dmh_boxd_z_cell") ;
						strArrayVal.RemoveAll() ;
						strArrayVal.Add(strCells[iCellIndex]) ;							
						if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
						{
							g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
							pRef->close() ;
							pRef = NULL ;
						}
					}
				}

			}
			if (i%2==1||((i==iRetSum-1)&&(i%2==0))) //每次在一行的第二个GD绘制完成后iZLocRow自减1,例外是最后一个GD刚好是在一行的第一个
			{
				iZLocRow-- ;
			}

		}
		//end GD

		//begin djg gd1
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex from dmh_import where dmh_import_table=1 and dmh_import_line='调车接近轨' order by dmh_import_lineindex asc")) ;
		nArrayRet.RemoveAll() ;
		int iRet1 = CDBUtility::GetRecordValue(strSql, _T("dmh_import_lineindex"), nArrayRet) ;
		//第三线的股道也放置在GD1组合上
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex, dmh_import_jfm from dmh_import where dmh_import_table=1 and instr(dmh_import_line,'半自动接近')>0 and dmh_import_jfm='FC' order by dmh_import_lineindex asc")) ;		
		int iRet2 = CDBUtility::GetRecordValue(strSql, _T("dmh_import_lineindex"), nArrayRet) ;
		iRetSum = iRet1+iRet2 ;
		int iNeedRow_DJG = (int)(iRetSum/3)+(iRetSum%3==0?0:1) ;		
		for (int i=0; i<iNeedRow_DJG; i++)
		{
			//插DJG
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_gd1") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_boxd_z_cell") ;
			for (int iCellIndex=0; iCellIndex<3; iCellIndex++) //此处iCellIndex是每层组合的第几个调车接近轨
			{							
				if ((i*3+iCellIndex)<iRetSum)
				{
					strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_lineindex=%d and dmh_import_cellrow<16"), nArrayRet.GetAt(i*3+iCellIndex)) ;
					CString strXhjDc, strGuiDaoDc ;
					strArrayRet.RemoveAll() ;
					int iRetSumTmp = CDBUtility::GetRecordValue(strSql, _T("dmh_import_xhj"), strArrayRet) ;
					if (iRetSumTmp==1)
					{
						strXhjDc = strArrayRet.GetAt(0) ;

						strArrayRet.RemoveAll() ;
						CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayRet) ;
						strGuiDaoDc = strArrayRet.GetAt(0) ;
						for (int iJdqIndex=0; iJdqIndex<3; iJdqIndex++)
						{
							pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*3*iCellIndex+dCellWidth*iJdqIndex+dCellWidth/2 ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
							strArrayVal.RemoveAll() ;
							CString strCell ;
							if (iJdqIndex==0)
							{
								strCell = strXhjDc ;
							}
							else
							{
								strCell = strGuiDaoDc ;
							}							
							strArrayVal.Add(strCell) ;							
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
								pRef->close() ;
								pRef = NULL ;
							}

						}
					}
				}
			}
			iZLocRow-- ;
		}
		//end djg

		//begin 第三线半自动接近 JG、GC/GJF
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_jfm from dmh_import where dmh_import_table=1 and instr(dmh_import_line,'半自动接近')>0 and dmh_import_jfm='JG' order by dmh_import_line")) ;
		strArrayRet.RemoveAll() ;
		iRetSum = CDBUtility::GetRecordValue(strSql, _T("dmh_import_line"), strArrayRet) ;
		int iNeedRow_BJG = iRetSum ;		
		for (int i=0; i<iNeedRow_BJG; i++)
		{
			//插JG
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_jg") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			strArrayVal.RemoveAll() ;
			//JZ
			strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_line='%s' and dmh_import_jfm='JG' "), strArrayRet.GetAt(i)) ;
			CString strJZ ;
			 CDBUtility::GetRecordValue(strSql, _T("dmh_import_xhj"), strJZ) ;
			 CString strJZTmp  ;
			 strJZTmp.Format(_T("(%s)"), strJZ) ;
			 strArrayVal.Add(strJZTmp) ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入前5个cell（接近轨）
			strBlockName = _T("dmh_boxd_z_cell") ;
			for (int iCellIndex=0; iCellIndex<5; iCellIndex++) 
			{	
				CString strCell = strJZ ;
				if (iCellIndex==1||iCellIndex==2)
				{
					strCell.Format(_T("%sJG"), strJZ) ;
				}
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
				strArrayVal.RemoveAll() ;
				strArrayVal.Add(strCell) ;							
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
					pRef->close() ;
					pRef = NULL ;
				}
			}

			//查询有无相应的接车进路
			strSql.Format(_T("select top 1 *  from dmh_import where dmh_import_table=0 and instr(dmh_import_line,'%s')>0 "), strArrayRet.GetAt(i) ) ;
			CString strStartEndXhj, strCZ;
			CDBUtility::GetRecordValue(strSql, _T("dmh_import_xhj"), strStartEndXhj) ;
			if (!strStartEndXhj.IsEmpty())
			{
				int iLocFind = strStartEndXhj.Find(_T("→")) ;
				int iLen = strStartEndXhj.GetLength() ;
				if (iLocFind!=-1&&iLen-iLocFind>=2)
				{
					strCZ = strStartEndXhj.Right(iLen-iLocFind-2) ;
				}				
				//插入后4个cell（接码）
				for (int iCellIndex=5; iCellIndex<9; iCellIndex++) 
				{	
					CString strCell = strJZ ;
					if (iCellIndex==8)
					{
						strCell = strCZ ;
					}
					else
					{
						strCell.Format(_T("%sJM"), strJZ) ;
					}
					pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
					pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strCell) ;							
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}

			iZLocRow-- ;

			//增加相应的GC/GJF(若有)
			if (!strStartEndXhj.IsEmpty())
			{
				strSql.Format(_T("select * from dmh_import where dmh_import_table=0 and dmh_import_cellrow<17 and instr(dmh_import_line,'%s')>0  order by dmh_import_cellrow desc "), strArrayRet.GetAt(i) ) ;
				CStringArray strArrayGuiDaoJM;
				int iGuiDaoJMCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuiDaoJM) ;
				int iNeedRow_JM = (int)(iGuiDaoJMCount/5)+(iGuiDaoJMCount%5==0?0:1) ;	

				//////////////////////////////////////////////////////////////////////////
				for (int iJMRowIndex=0; iJMRowIndex<iNeedRow_JM; iJMRowIndex++)
				{
					//插JG
					CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
					strBlockName = _T("dmh_boxd_z_gc-gjf") ;
					pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
					pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
					strArrayVal.RemoveAll() ;
					strArrayVal.Add(strJZTmp) ;
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					//填充GC-GJF cell
					//////////////////////////////////////////////////////////////////////////
					strBlockName = _T("dmh_boxd_z_cell2") ;
					for (int iCellIndex=0; iCellIndex<5; iCellIndex++)
					{							
						pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidth*iCellIndex+dCellWidth/2 ;
						pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight/2 ;
						strArrayVal.RemoveAll() ;
						CString strCell ;
						if ((iJMRowIndex*5+iCellIndex)<iGuiDaoJMCount)
						{
							strCell = strArrayGuiDaoJM.GetAt(iJMRowIndex*5+iCellIndex) ;
							strArrayVal.Add(strCell) ;	
							CString strCellAtt2 ; 
							strCellAtt2.Format(_T("%s%dGCJ"), strJZ, iJMRowIndex*5+iCellIndex+1) ;
							strArrayVal.Add(strCellAtt2) ;							
							if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
							{
								g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
								pRef->close() ;
								pRef = NULL ;
							}
							//后5个cell
							pt_3d_insert.x +=(dCellWidth*5) ;
							pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight*5/8 ;
							strArrayVal.RemoveAll() ;
							strArrayVal.Add(strCell) ;
							if (g_cSystem.InsertBlock(pRef, _T("dmh_boxd_z_cell") , pt_3d_insert, &strArrayVal, FALSE))
							{
								g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
								pRef->close() ;
								pRef = NULL ;
							}
						}
					}
					//////////////////////////////////////////////////////////////////////////

					iZLocRow-- ;
				}

				//////////////////////////////////////////////////////////////////////////

			}


		}
		//end 第三线半自动接近 JG、GC/GJF

		//begin ZBPU-2B
		CStringArray strArrayJinLu0, strArrayJinLu1 ;
		//正线进路表
		strSql.Format(_T("select distinct dmh_import_jfm, dmh_import_cellcol,dmh_import_jinluname from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				CString strHasFinished = _T("") ; //记录已经处理过的进路的列
				while (!cSet.IsEOF())
				{
					CString strCellCol ;
					cSet.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ;
					//处理过的进路不再处理
					CString strToFind ;
					strToFind.Format(_T("%s,"), strCellCol) ;
					if (strHasFinished.Find(strToFind)!=-1)
					{
						cSet.MoveNext() ;
						continue;
					}

					strHasFinished.AppendFormat(_T("%s,"), strCellCol) ;

					CString strJinLuName, strJFM ;
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					cSet.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;

					//begin 查询和当前进路共用发送器的进路
					BOOL bFoundShare = FALSE ;
					CString strJinLuName2, strJFM2, strCellCol2 ;
					if (strJFM.Right(1)==_T(")"))
					{
						int iFindMarkLoc = strJFM.ReverseFind(_T('(')) ; //例：JM(1)，左括号的位置
						if (iFindMarkLoc!=-1)
						{
							int iMarkLen = strJFM.GetLength()-iFindMarkLoc ; //例：JM(1)，最后（1）的长度
							CString strMark = strJFM.Mid(iFindMarkLoc) ; //例：JM(1)中（1）
							CADORecordset cSetFindShare ;
							strSql.Format(_T("select * from (select distinct dmh_import_jfm, dmh_import_cellcol,dmh_import_jinluname from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc) where dmh_import_cellcol>'%s' and right(dmh_import_jfm,%d)='%s'"), strCellCol, iMarkLen, strMark) ;
							if (cSetFindShare.IsOpen())
							{
								cSetFindShare.Close() ;
							}
							if (cSetFindShare.Open(g_PtrCon, strSql))
							{
								int iRsCountFindShare = cSetFindShare.GetRecordCount() ;
								if (iRsCountFindShare==1)
								{
									cSetFindShare.MoveFirst() ;
									cSetFindShare.GetFieldValue(_T("dmh_import_cellcol"), strCellCol2) ;
									cSetFindShare.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName2) ;
									cSetFindShare.GetFieldValue(_T("dmh_import_jfm"), strJFM2) ;

									bFoundShare = TRUE ;
									strHasFinished.AppendFormat(_T("%s,"), strCellCol2) ;
								}
								else if(iRsCountFindShare>1)
								{
									CString strMsg ;
									strMsg.Format(_T("下面查询结果大于1：\n%s"), strSql) ;	
									AfxMessageBox(strMsg) ;
								}
								cSetFindShare.Close() ;
							}
						}
					}
					//end 查询和当前进路共用发送器的进路

					if (!strJinLuName2.IsEmpty())
					{
						strJinLuName.AppendFormat(_T("/%s"), strJinLuName2) ;
					}

					strArrayJinLu0.Add(strJinLuName) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
		//侧线股道表
		strSql.Format(_T("select distinct dmh_import_type, dmh_import_freq, dmh_import_xhj,dmh_import_jinluname, dmh_import_lineindex from dmh_import where dmh_import_table=1 order by dmh_import_lineindex asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					CString strJinLuName ;				
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					strArrayJinLu1.Add(strJinLuName) ;
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
		//绘制ZBPU组合
		int iJinLuCount0 = strArrayJinLu0.GetCount() ;
		int iJinLuCount1 = strArrayJinLu1.GetCount() ;
		int iNeedRow_ZBPU = (iJinLuCount0*2+iJinLuCount1)/8+((iJinLuCount0*2+iJinLuCount1)%8==0?0:1) ;
		for (int i=0; i<iNeedRow_ZBPU; i++)
		{
			//插ZBPU
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_boxd_z_zbpu-2b") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			for (int iCellIndex=0; iCellIndex<8; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidthZBPU*iCellIndex+dCellWidthZBPU/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight/2 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;
				if ((i*8+iCellIndex)<(iJinLuCount0*2+iJinLuCount1))
				{
					if ((i*8+iCellIndex)<(iJinLuCount0*2))
					{
						strCell = strArrayJinLu0.GetAt((i*8+iCellIndex)/2) ;
						strBlockName = _T("dmh_boxd_z_cell2") ;
						strArrayVal.Add(strCell) ;	
						CString strIndex ;
						strIndex.Format(_T("(%d)"), iCellIndex%2==0?1:2) ;
						strArrayVal.Add(strIndex) ;
					}
					else
					{
						strCell = strArrayJinLu1.GetAt(i*8+iCellIndex-iJinLuCount0*2) ;
						strBlockName = _T("dmh_boxd_z_cell") ;
						strArrayVal.Add(strCell) ;	
					}											
					if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
						pRef->close() ;
						pRef = NULL ;
					}
				}
			}
			iZLocRow-- ;
		}
		//end ZBPU-2B

		//插入汇流排
		if (m_bUseHLP==TRUE)
		{
			CheckToAddZ(iZLocRow, iZIndex) ; //每次插入一行时要先检查是否要新添一个组合架（即是否要换行）
			strBlockName = _T("dmh_box_z_hlp") ;
			pt_3d_insert.x = iZIndex*(dMainTbWidth+dBetweenMainTb) ;
			pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight ;
			strArrayVal.RemoveAll() ;
			strArrayVal.Add(_T("H1")) ;
			if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
			{
				pRef->close() ;
				pRef = NULL ;
			}
			//插入cell
			strBlockName = _T("dmh_boxd_z_cell") ;
			CString strHLPNames[14] = {_T("H-F2"),_T("H-F3"),_T("H-F5"),_T("H-F9"),_T("H-F10"),_T("H-F11"),_T("H-F12"),_T("H-F14"),_T("H-F15"),_T("H-F16"),_T("H-F17"),_T("H-F18"),_T("H-1700"),_T("H-2000")} ;
			for (int iCellIndex=0; iCellIndex<14; iCellIndex++)
			{							
				pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*iZIndex+dLeftWidth+dCellWidthHLP*iCellIndex+dCellWidthHLP/2 ;
				pt_3d_insert.y = (iZLocRow-1)*dMainTbRowHeight + dMainTbRowHeight/2 ;
				strArrayVal.RemoveAll() ;
				CString strCell ;	
				strCell = strHLPNames[iCellIndex] ;
				strArrayVal.Add(strCell) ;							
				if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
				{
					g_cSystem.SetAttHeight(pRef, 2.5, 0.6, _T("CELL_CONTENT")) ;
					pRef->close() ;
					pRef = NULL ;
				}				
			}
			iZLocRow-- ;
		}

		
        //begin frame
		strBlockName = _T("frame_left") ;
		pt_3d_insert.x = dFrameBaseX ;
		pt_3d_insert.y = dFrameBaseY ;
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		//内边框
		AcGePoint3dArray ptArrayFrame ;
		ptArrayFrame.append(AcGePoint3d(-5, dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1), dFrameBaseY+5, 0)) ;
		ptArrayFrame.append(AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1), dFrameBaseY+297-5, 0)) ;
		ptArrayFrame.append(AcGePoint3d(-5, dFrameBaseY+297-5, 0)) ;
		g_cSystem.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
		//外边框
		ptArrayFrame.setAt(0,AcGePoint3d(dFrameBaseX, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(1,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1)+5, dFrameBaseY, 0)) ;
		ptArrayFrame.setAt(2,AcGePoint3d((dMainTbWidth+dBetweenMainTb)*(iZIndex+1)+5, dFrameBaseY+297, 0)) ;
		ptArrayFrame.setAt(3,AcGePoint3d(dFrameBaseX, dFrameBaseY+297, 0)) ;
		g_cSystem.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	
		//签字栏
		strBlockName = _T("SIGN_3") ;
		pt_3d_insert.x = (dMainTbWidth+dBetweenMainTb)*(iZIndex+1) ;
		pt_3d_insert.y = dFrameBaseY+5 ;
		strArrayVal.RemoveAll() ;
		strArrayVal.Add(m_strStation) ;
		strArrayVal.Add(m_strDwgSn) ;		
		strArrayVal.Add(CBlkUtility::GetDateStr()) ;
		strArrayVal.Add(_T("站内电码化组合柜布置图")) ;
		strArrayVal.Add(m_strProjectName) ;
		if (g_cSystem.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		g_cSystem.ModifyFontStyle(_T("standard"), _T("hztxt2.shx")) ;
		pDb->saveAs(strSaveFile) ;
		delete pDb ;
		pDb = NULL ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in DrawZD!"));			
	}
}

// 电化局组合柜 FB组合 取得一条线的进站信号机和FBJ名
void CZnDmh::GetLineInfo(const CString & strLine, CString & strJZ, CStringArray & strArrayFBJ)
{
	CString strFCZ, strCZ, strFJZ ;
	this->GetLineInfo(strLine, strJZ, strFCZ, strCZ, strFJZ) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select distinct dmh_import_line,dmh_import_jfm, dmh_import_cellcol,dmh_import_jinluname from dmh_import where dmh_import_table=0 and dmh_import_line='%s' order by dmh_import_cellcol asc"), strLine) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				CString strHasFinished = _T("") ; //记录已经处理过的进路的列
				while (!cSet.IsEOF())
				{
					CString strCellCol ;
					cSet.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ;
					//处理过的进路不再处理
					CString strToFind ;
					strToFind.Format(_T("%s,"), strCellCol) ;
					if (strHasFinished.Find(strToFind)!=-1)
					{
						cSet.MoveNext() ;
						continue;
					}

					strHasFinished.AppendFormat(_T("%s,"), strCellCol) ;

					CString strJinLuName, strJFM ;
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;
					cSet.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;

					//begin 查询和当前进路共用发送器的进路
					BOOL bFoundShare = FALSE ;
					CString strJinLuName2, strJFM2, strCellCol2 ;
					if (strJFM.Right(1)==_T(")"))
					{
						int iFindMarkLoc = strJFM.ReverseFind(_T('(')) ; //例：JM(1)，左括号的位置
						if (iFindMarkLoc!=-1)
						{
							int iMarkLen = strJFM.GetLength()-iFindMarkLoc ; //例：JM(1)，最后（1）的长度
							CString strMark = strJFM.Mid(iFindMarkLoc) ; //例：JM(1)中（1）
							CADORecordset cSetFindShare ;
							strSql.Format(_T("select * from (select distinct dmh_import_line,dmh_import_jfm, dmh_import_cellcol,dmh_import_jinluname from dmh_import where dmh_import_table=0 and dmh_import_line='%s' order by dmh_import_cellcol asc) where dmh_import_cellcol>'%s' and right(dmh_import_jfm,%d)='%s'"), strLine, strCellCol, iMarkLen, strMark) ;
							if (cSetFindShare.IsOpen())
							{
								cSetFindShare.Close() ;
							}
							if (cSetFindShare.Open(g_PtrCon, strSql))
							{
								int iRsCountFindShare = cSetFindShare.GetRecordCount() ;
								if (iRsCountFindShare==1)
								{
									cSetFindShare.MoveFirst() ;
									cSetFindShare.GetFieldValue(_T("dmh_import_cellcol"), strCellCol2) ;
									cSetFindShare.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName2) ;
									cSetFindShare.GetFieldValue(_T("dmh_import_jfm"), strJFM2) ;

									bFoundShare = TRUE ;
									strHasFinished.AppendFormat(_T("%s,"), strCellCol2) ;
								}
								else if(iRsCountFindShare>1)
								{
									CString strMsg ;
									strMsg.Format(_T("下面查询结果大于1：\n%s"), strSql) ;	
									AfxMessageBox(strMsg) ;
								}
								cSetFindShare.Close() ;
							}
						}
					}
					//end 查询和当前进路共用发送器的进路

					if (!strJinLuName2.IsEmpty())
					{
						strJinLuName.AppendFormat(_T("/%s"), strJinLuName2) ;
					}

					strArrayFBJ.Add(strJinLuName) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetLineInfo")) ;
	} 
}


// 读入组合架到数据库(for 电化局版)
void CZnDmh::ReadZD(const CString & strFileName)
{
	CString strMsg ;

	double dMainTbWidth = 188.000 ; //组合柜主框宽度
	double dMainTbRowHeight = 20.000 ; //每层高度
	double dRowTopBottomHeight = 5.000 ; //每层上边or下边单元条的高度（上边写单元号，下边写设备类型）
	double dLeftWidth = 38.000 ; //左侧序号、组合类型等列所占宽度（从框架左边框到组合单元的距离）
	double dCellWidth = 15.000 ; //组合单元格宽度	
	double dCellWidthR = 10.000 ; //ZDTR-1单元宽度
	double dCellWidthZBPU = 18.750 ; //ZBPU单元宽度
	double dCellWidthHLP = 9.375 ;//150/16,若单元格变化此值随着变
	double dBetweenMainTb = 5.000 ; //Z1和Z2之间的距离
	double dFrameBaseX = -30.000 ; //左Frame的插入基点x,frame宽25，间隙5
	double dFrameBaseY = -40.000 ; //左Frame的插入基点y

	m_pDb = new AcDbDatabase(false) ;
	//CUserSystem mysys ;
	g_cSystem.m_pDb = m_pDb ;
	CDBUtility::ClearTable(_T("dmh_in_zlayer")) ;
	CDBUtility::ClearTable(_T("dmh_in_zcell")) ;
	if(Acad::eOk==m_pDb->readDwgFile(strFileName))
	{
		AcDbBlockTable * pBlkTbl = NULL ;
		m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
		AcDbBlockTableRecord * pBlkTblRec = NULL ;
		pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
		pBlkTbl->close() ;
		AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

		pBlkTblRec->newIterator(pBlkTblRecIter);
		pBlkTblRec->close();

		AcDbEntity * pEnt = NULL ;
		for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
		{
			if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
			{
				if (pEnt->isKindOf(AcDbBlockReference::desc()))
				{
					AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;					
					CString strBlkName ;
					g_cSystem.GetBlockName(pRef, strBlkName) ;
					if (strBlkName==_T("dmh_box_zzh_main"))
					{
						CString strSql ;

						CString strZIndex ;
						g_cSystem.GetAttValue(pRef, _T("ZZH_INDEX"), strZIndex) ;
						int nZIndex = 0 ;
						//nZIndex = _tstoi(strZIndex.Right(1)) ; // 此处默认组合架序号应该都是个位数，有问题否？(20130909:有问题了，果然有ZY10),另如果只有一个组合架，要写成Z1，不能写Z
						nZIndex = _tstoi(strZIndex.Right(strZIndex.GetLength()-m_strPreZ.GetLength())) ; 
						if (nZIndex!=0)
						{
							AcGePoint3d pt_3d_z(0,0,0) ;
							pt_3d_z = pRef->position() ;
							//1~10层组合块插入点应该严格捕捉
							for (int iLayerIndex=0; iLayerIndex<10; iLayerIndex++)
							{
								AcGePoint3d pt_3d_layer(0,0,0) ;
								pt_3d_layer.x = pt_3d_z.x ;
								pt_3d_layer.y = pt_3d_z.y+iLayerIndex*20 ;
								//查找插入点在pt_3d_layer处的块
								CString strLayerBlkName, strShowType, strStartEnd1, strStartEnd2 ;
								CStringArray strArrayJdqName ;
								int iFindLayerNum = FindZDLayer(pt_3d_layer, strLayerBlkName, strArrayJdqName) ;
								if (iFindLayerNum==1)
								{
									//存组合layer信息
									strSql.Format(_T("insert into dmh_in_zlayer(dmh_in_zlayer_shelfnum, dmh_in_zlayer_layernum, dmh_in_zlayer_layerblock) values(%d, %d, '%s')"), nZIndex, iLayerIndex+1, strLayerBlkName) ;
									ExecSqlString(strSql) ;

									int nLayerId = 0 ;
									nLayerId = CDBUtility::GetTableMaxId(_T("dmh_in_zlayer")) ;

									if (strLayerBlkName==_T("dmh_boxd_z_zdtr"))
									{
										//查找本层1~15box
										for (int iCellIndex=0; iCellIndex<15; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dCellWidthR;
											pt_3d_cell_bottomright.y = pt_3d_layer.y ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidthR ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dMainTbRowHeight ;
											//
											CStringArray strArrayCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell_D(pt_3d_cell_topleft, pt_3d_cell_bottomright, strArrayCellContent) ;

											if (iFindCellNum==1)
											{
												int iCellContentCount = strArrayCellContent.GetCount() ; //==1表示是dmh_boxd_z_cell1,==2表示是dmh_boxd_z_cell2
												if (iCellContentCount==1)
												{
													CString strJdqName ;
													strJdqName.Format(_T("R%d"), iCellIndex+1) ; //此处strJdqName是指ZDTR-1电阻名														
													strSql.Format(_T("insert into dmh_in_zcell(dmh_in_zcell_layerid, dmh_in_zcell_shelfnum, dmh_in_zcell_layernum, dmh_in_zcell_boxnum, dmh_in_zcell_jdqname, dmh_in_zcell_cellcontent) values(%d, %d, %d, %d, '%s', '%s')"), nLayerId, nZIndex, iLayerIndex+1, iCellIndex+1, strJdqName, strArrayCellContent.GetAt(0)) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在Z%d-%d的第%d位置处找到不止一个z_cell块，请检查是否有重合！"), nZIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
									else if (strLayerBlkName==_T("dmh_boxd_z_zbpu-2b"))
									{
										//查找本层1~8box
										for (int iCellIndex=0; iCellIndex<8; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dCellWidthZBPU;
											pt_3d_cell_bottomright.y = pt_3d_layer.y ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidthZBPU ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dMainTbRowHeight ;
											//
											CStringArray strArrayCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell_D(pt_3d_cell_topleft, pt_3d_cell_bottomright, strArrayCellContent) ;

											if (iFindCellNum==1)
											{
												int iCellContentCount = strArrayCellContent.GetCount() ; //==1表示是dmh_boxd_z_cell1,==2表示是dmh_boxd_z_cell2
												if (iCellContentCount>0)
												{
													strSql.Format(_T("insert into dmh_in_zcell(dmh_in_zcell_layerid, dmh_in_zcell_shelfnum, dmh_in_zcell_layernum, dmh_in_zcell_boxnum, dmh_in_zcell_cellcontent) values(%d, %d, %d, %d, '%s')"), nLayerId, nZIndex, iLayerIndex+1, iCellIndex+1, strArrayCellContent.GetAt(0)) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在Z%d-%d的第%d位置处找到不止一个z_cell块，请检查是否有重合！"), nZIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
									else if (strLayerBlkName==_T("dmh_box_z_hlp"))
									{
										//查找本层1~16box
										for (int iCellIndex=0; iCellIndex<16; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dCellWidthHLP;
											pt_3d_cell_bottomright.y = pt_3d_layer.y ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidthHLP ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dMainTbRowHeight ;
											//
											CStringArray strArrayCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell_D(pt_3d_cell_topleft, pt_3d_cell_bottomright, strArrayCellContent) ;

											if (iFindCellNum==1)
											{
												int iCellContentCount = strArrayCellContent.GetCount() ; //==1表示是dmh_boxd_z_cell1,==2表示是dmh_boxd_z_cell2
												if (iCellContentCount==1)
												{
													strSql.Format(_T("insert into dmh_in_zcell(dmh_in_zcell_layerid, dmh_in_zcell_shelfnum, dmh_in_zcell_layernum, dmh_in_zcell_boxnum, dmh_in_zcell_cellcontent) values(%d, %d, %d, %d, '%s')"), nLayerId, nZIndex, iLayerIndex+1, iCellIndex+1, strArrayCellContent.GetAt(0)) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在Z%d-%d的第%d位置处找到不止一个z_cell块，请检查是否有重合！"), nZIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}
									else
									{
										//查找本层1~10继电器情况
										for (int iCellIndex=0; iCellIndex<10; iCellIndex++)
										{
											AcGePoint3d pt_3d_cell_topleft(0,0,0), pt_3d_cell_bottomright(0,0,0) ;
											pt_3d_cell_topleft.x = pt_3d_layer.x + dLeftWidth+iCellIndex*dCellWidth;
											pt_3d_cell_bottomright.y = pt_3d_layer.y ;
											pt_3d_cell_bottomright.x = pt_3d_cell_topleft.x+dCellWidth ;
											pt_3d_cell_topleft.y = pt_3d_cell_bottomright.y+dMainTbRowHeight ;
											//
											CStringArray strArrayCellContent ; //单元格内容，例：11DG、IG
											int iFindCellNum = FindCell_D(pt_3d_cell_topleft, pt_3d_cell_bottomright, strArrayCellContent) ;

											if (iFindCellNum==1)
											{
												int iCellContentCount = strArrayCellContent.GetCount() ; //==1表示是dmh_boxd_z_cell1,==2表示是dmh_boxd_z_cell2
												if (iCellContentCount>0)
												{
													CString strJdqName = strArrayJdqName.GetAt(iCellIndex) ;
													if (iCellContentCount==2)
													{
														strJdqName = strArrayCellContent.GetAt(1) ;
													}
													CString strCellContent = strArrayCellContent.GetAt(0) ;
													CString strCellContentTrim = strCellContent ;
													strCellContentTrim.TrimLeft(_T("*")) ;
													strCellContentTrim.TrimRight(_T("+")) ;
													strSql.Format(_T("insert into dmh_in_zcell(dmh_in_zcell_layerid, dmh_in_zcell_shelfnum, dmh_in_zcell_layernum, dmh_in_zcell_boxnum, dmh_in_zcell_jdqname, dmh_in_zcell_cellcontent, dmh_in_zcell_cellcontent_trim) values(%d, %d, %d, %d, '%s', '%s', '%s')"), nLayerId, nZIndex, iLayerIndex+1, iCellIndex+1, strJdqName, strCellContent, strCellContentTrim) ;
													ExecSqlString(strSql) ;
												}
											}
											else if (iFindCellNum>1)
											{
												strMsg.Format(_T("在Z%d-%d的第%d位置处找到不止一个z_cell块，请检查是否有重合！"), nZIndex, iLayerIndex, iCellIndex) ;
												AfxMessageBox(strMsg) ;
												continue;
											}
											else
											{
												//strMsg.Format(_T("在Z%d-%d的第%d位置处找到0个z_cell块！"), nZIndex, iLayerIndex, iCellIndex) ;
												//acutPrintf(strMsg) ;
												continue;
											}
										}
									}

								}
								else if (iFindLayerNum>1)
								{
									strMsg.Format(_T("在(%.3f,%.3f)处找到不止一个组合块，请检查是否有重合！"), pt_3d_layer.x, pt_3d_layer.y) ;
									AfxMessageBox(strMsg) ;
									continue;
								}
								else
								{
									strMsg.Format(_T("\n在(%.3f,%.3f)处找到0个组合块！"), pt_3d_layer.x, pt_3d_layer.y) ;
									acutPrintf(strMsg) ;
									continue;
								}
							}
						}
					}
				}
				pEnt->close() ;
			}
		}
		delete pBlkTblRecIter ;
		pBlkTblRecIter = NULL ;
	}

	delete m_pDb ;
	m_pDb = NULL ;
}

// 查找在指定点的组合块（layer）(for 电化局)
int CZnDmh::FindZDLayer(const AcGePoint3d & pt_3d_base, CString & strBlockName, CStringArray & strArrayJdqName)
{
	int nRet = 0 ;

	AcDbBlockTable * pBlkTbl = NULL ;
	m_pDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator(pBlkTblRecIter);
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if(Acad::eOk == pBlkTblRecIter->getEntity(pEnt ,AcDb::kForRead))
		{
			if (pEnt->isKindOf(AcDbBlockReference::desc()))
			{
				AcDbBlockReference *pRef = AcDbBlockReference::cast(pEnt) ;	
				AcGePoint3d pt_3d_layer(0,0,0) ;
				pt_3d_layer = pRef->position() ;
				if (pt_3d_layer==pt_3d_base)
				{
					CString strBlockNameTmp ;
					g_cSystem.GetBlockName(pRef, strBlockNameTmp) ;
					if (strBlockNameTmp!=_T("dmh_box_zzh_main"))
					{
						strBlockName = strBlockNameTmp ;
						this->GetJdqNames_D(strBlockName, strArrayJdqName) ;
// 						if (strBlockName==_T("dmh_boxd_z_gc"))
// 						{						
// 					
// 						}
						nRet++ ;
					}

				}
			}
			pEnt->close() ;
		}
	}
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;

	return nRet;
}

int CZnDmh::GetJdqNames_D(const CString & strBlkName, CStringArray & strArrayJdqNames)
{
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_def_z where dmh_def_z_blkname='%s' order by dmh_def_z_index"), strBlkName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					CString strJdqName = _T("") ;
					cSet.GetFieldValue(_T("dmh_def_z_jdqname"), strJdqName) ;
					strArrayJdqNames.Add(strJdqName) ;
					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);  		
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误 in GetJdqNames!"));			
	}
	return 0;
}

// 查找组合中1~10继电器位置放置的元素
int CZnDmh::FindCell_D(const AcGePoint3d& pt_3d_topleft, const AcGePoint3d& pt_3d_bottomright, CStringArray & strArrayCellContent)
{
	int nRet = 0 ;

	CLongArray nArrayBlkHandle ;
	int iFindBlkNum = g_cSystem.FindBlkInArea(pt_3d_topleft, pt_3d_bottomright, nArrayBlkHandle) ;
	if (iFindBlkNum>0)
	{	
		AcDbEntity *pEnt = NULL ;
		for (int i=0; i<iFindBlkNum; i++)
		{				
			if(g_cSystem.OpenAcDbEntity(nArrayBlkHandle.GetAt(i), pEnt, AcDb::kForRead))
			{
				AcDbBlockReference *pRef = NULL ;
				pRef = AcDbBlockReference::cast(pEnt) ;
				CString strBlkName ;
				g_cSystem.GetBlockName(pRef, strBlkName) ;
				if (strBlkName.Find(_T("dmh_boxd_z_cell"))!=-1)
				{
					CString strCellContentTmp ;
					g_cSystem.GetAttValue(pRef, _T("CELL_CONTENT"), strCellContentTmp) ;
					if (!strCellContentTmp.IsEmpty())
					{
						strArrayCellContent.Add(strCellContentTmp) ;
						if(g_cSystem.GetAttValue(pRef, _T("JDQNAME"), strCellContentTmp))
						{
							strArrayCellContent.Add(strCellContentTmp) ;
						}
					}
					nRet++ ;
				}

				pEnt->close() ;
			}
		}
	}

	return nRet ;
}

// 绘制电路图 for 电化局
void CZnDmh::DrawCircuitD(const CString &strSavePath)
{
	CString strMsg ;
	CUserSystem mysys ;

	double dWidth_NGL_Code = 65.000 ;
	double dWidth_NGL_NoCode = 56.000 ; 
	double dWidthZJC_0000 = 40.000 ;
	double dWidthGCJ_0000 = 65.000 ;
	double dWidthZJC_FMJ = 53.000 ;
	double dWidthFS = 253.000 ; 

	double dWidthZF_FMJ = 65.000 ;

	double dNGL_Y = 0.000 ;
	double dGCJ_Y = -120.000 ; //GCJ块插入点Y坐标

	double dOffsetX_NGL_GCJ = 0.000 ; //NGL和GCJ插入点x坐标不是对齐的，需要错开一点

	double dWidth_TFGZ = 60.000 ;
	double dOffsetY_FS = 26.000; // FS块插入时Y偏离dGCJ_Y的距离
	double dOffsetY_FSPLUS = 47.000 ;//+1FS插入时Y偏离dGCJ_Y的距离
	double dWidthGJFALL_0000 = 180.000 ;
	double dWidthGJFALL_0001 = 130.000 ;
	double dWidthGJFGJ_0000 = 165.000 ;
	double dWidthGJFGJ_0001 = 80.000 ;
	double dWidthZJC_0002 = 80.000 ;

	double dWidthZBPU_FBJ_FS_GD = 400.000 ;
	double dWidthBM_FMJ = 65.000 ; //ZF FJ中和GC相连的FMJ
	double dWidthZBPU = 77.0509 ; //发送调整ZBPU
	

	double dWidthZBJH = 33.000 ;
	double dWidthGJFGCJ = 28.000 ;
	double dWidthGCJGJF_JFMJ = 28.000 ;

	double dWidthLiCi_3000 = 230.000 ;

	double dWidth_ZLOC = 25.000 ; //组合位置图框的宽度
	double dHeight_ZLOC = 5.000 ;

	double dHeight_NGL = 75.000 ;
	double dHeight_ZJC = 71.000 ;
	double dHeight_GCJGJF_GJF = 28.000 ;
	double dHeight_GCJGJF_Frame = 62.000 ;

	double dBetween_NGL_ZJC = 9.000 ;

	//侧线股道等
	double dOffsetY_NGLGJF1 = 50.000 ; //侧线的NGL、GJF1相对正线时Y坐标偏移
	
	try
	{
		CString strSql ;
		CADORecordset cSet ;
		m_ctrlProgressAll->SetRange(1, 3) ;
		
		//先处理第一个table
		
		m_ctrlProgressAll->SetPos(1) ;	
		
		//::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)0, 0) ;	
		strSql.Format(_T("select distinct dmh_import_line, dmh_import_lineindex, dmh_import_type, dmh_import_xhj, dmh_import_cellcol, dmh_import_jinluname from dmh_import where dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int nRsCount = cSet.GetRecordCount() ;
			//acedSetStatusBarProgressMeter(_T("生成电码化电路"), 1, nRsCount) ;
			m_ctrlProgressCur->SetRange(1, nRsCount) ;
			
			if (nRsCount>0)
			{
				cSet.MoveFirst() ;

				int iGuDaoFenGe = 1 ; //股道是否分割，每条线都有正接，只在正接时判断是否分割，遇到正接就重新设置该值,1表示无分割，其他表示有分割
				CStringArray strArrayGuDao ;
				CString strJZXHJ,strFJZXHJ,strCZXHJ, strFCZXHJ ; //每条线假定必有正接，故只在一条线的正接进路中取一次进站、出站 、反出站、反进站
				CString strGuDao ; //股道名称
				int iMaxGCJ_ZJ = 0, iMaxGCJ_ZF = 0 ; //保存正接和正发的GCJ个数

				int iDwgIndex = 0 ;

				while (!cSet.IsEOF())
				{
					//acedSetStatusBarProgressMeterPos(iDwgIndex+1) ;
					m_ctrlProgressCur->SetPos(iDwgIndex+1) ;
					//::PostMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSPOS, (WPARAM)(iDwgIndex+1), 0) ;

					//一条记录就是一张图纸
					CString strSaveFileName ; //存储电码化图纸的文件名					
					AcDbDatabase *pSaveDb = new AcDbDatabase ; //存储数据库
					mysys.m_pDb = pSaveDb;	
					//记录扩展数据值
					CMapStringToString mapToReplace, mapStrToStrXdata, mapToAddXdata ;
					CString strCircuitTemplete ; //当前选择的底图模板
					AcGeMatrix3d xform;  
					AcGeVector3d vec(0,0,0) ;

					CString strLine, strJinLuType, strStartEndXhj, strCellCol, strJinLuName ;
					int iLineIndex = 0 ;
					cSet.GetFieldValue(_T("dmh_import_line"), strLine) ; //例：下行线1
					cSet.GetFieldValue(_T("dmh_import_lineindex"), iLineIndex) ; //例：下行线1为0
					cSet.GetFieldValue(_T("dmh_import_type"), strJinLuType) ; //例：正接
					cSet.GetFieldValue(_T("dmh_import_xhj"), strStartEndXhj) ; //例：X->XI
					cSet.GetFieldValue(_T("dmh_import_cellcol"), strCellCol) ; //例：B
					cSet.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ; //进路名，例：XJM

					strMsg.Format(_T("正线电码化-%s(%d/%d)"), strJinLuName, iDwgIndex+1, nRsCount) ;
					::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)(&strMsg), 0) ;

					int iMaxGCJ = 0 ; //当前进路GCJ的最大编号（个数）

					int iUpOrDown = 0 ; //0：下行，1：上行
					int iRunDirect = 0 ; //运行方向，决定图纸布局，0，从左往右，1，从右往左
					if (strLine.Find(_T("下行"))!=-1)
					{
						iUpOrDown = 0 ;
						if (strJinLuType.Find(_T("正"))!=-1)
						{
							iRunDirect = 0 ;
						}
						else
						{
							iRunDirect = 1 ;
						}
					}
					else
					{
						iUpOrDown = 1 ;
						if (strJinLuType.Find(_T("正"))!=-1)
						{
							iRunDirect = 1 ;
						}
						else
						{
							iRunDirect = 0 ;
						}
					}

					if (strJinLuType==_T("正接"))
					{ 
						//取得进出站信号机等信息,只在每条线的正接进路中执行此操作
						strJZXHJ = strFCZXHJ = strCZXHJ = strFJZXHJ = _T("") ;
						this->GetLineInfo(strLine, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ) ;

						//股道
						iGuDaoFenGe = 1 ;
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and left(dmh_import_guidao,1)='*' order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iGuDaoFenGe = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;
						if (iGuDaoFenGe==1)
						{
							strGuDao = strArrayGuDao.GetAt(0) ;
						}
						else if (iGuDaoFenGe>1)
						{
							CString strGuDaoTmp = strArrayGuDao.GetAt(0) ;
							strGuDao = strGuDaoTmp.Left(strGuDaoTmp.GetLength()-1) ;
						}						

						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;
						iMaxGCJ_ZJ = iMaxGCJ ;						

						strSaveFileName.Format(_T("%s\\%s%s正向接车进路电码化电路图.dwg"), strSavePath, m_strStation, strStartEndXhj) ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标
								double dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								double dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X	

                                int iGCJLayerPre = 0 ;
								int iZLocIndex = 1 ;
								CString strCD1Loc, strCD2Loc ; //记录CD-1的zloc和CD-2的zloc（若有）
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;									
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端

									CString strGCJName ;
									int iGCJShelf = 0, iGCJLayer = 0, iGCJBox = 0, iGCJIndex = 0  ;
									this->GetGCJInfo_D(strGuiDao, strGCJName, iGCJShelf, iGCJLayer, iGCJBox) ;
									if (nGuiDaoIndex==0)
									{
										iGCJLayerPre = iGCJLayer ;
										strCD1Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
									}
									else if (iGCJLayerPre!=iGCJLayer && iZLocIndex!=2)
									{
										iZLocIndex = 2 ;
										strCD2Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
                                        //记录1CDA-2图框起始位置
										dCDA2_StartX = dBaseX ;
									}									
									
									CString strGCJIndex ;
									CString strReg = _T("^.+(\\d+)GCJ$") ;
									if (CBlkUtility::IsStrMatch(strReg, strGCJName))
									{
										CBlkUtility::RegReplace(strReg, _T("$1"), strGCJName, strGCJIndex) ;
										iGCJIndex = _tstoi(strGCJIndex) ;
									}									
									
									this->FillXdataNGL_D(iGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_00%d%d_0.dwg"), m_strSysBasePath, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_1%d%d0_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,-20,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((iGuDaoFenGe==1&&nGuiDaoIndex==0)||(iGuDaoFenGe>1&&(nGuiDaoIndex==(iGuDaoFenGe-1))))
										{
											dBaseX-=dWidthZJC_0000 ; 
										}
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((nGuiDaoIndex==0&&iGuDaoFenGe==1)||(iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1))
										{
											dBaseX+=dWidthZJC_0000 ; 
										}
									}

									xformNGL.setToTranslation(vecNGL);	
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									this->FillXdataGCJ_D(nGuiDaoIndex, iGCJIndex, iGCJBox, iZLocIndex, strGCJName, mapToReplace, mapStrToStrXdata, mapToAddXdata, 0) ;
									vecNGL.x =vecNGL.x+dOffsetX_NGL_GCJ ;
									vecNGL.y = dGCJ_Y ;
									xformNGL.setTranslation(vecNGL) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcj_000%d_%d.dwg"), m_strSysBasePath, nGuiDaoIndex%2==0?1:2, iRunDirect) ;
									this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									if (nGuiDaoIndex==0)
									{
										//ZJC_JMJ
										this->FillXdataJMJ_0000_D(0, strJZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
										vecNGL.x = vecNGL.x+(iRunDirect==0?-dWidthZJC_0000:dWidthGCJ_0000) ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_jmj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}

									dCDA2_EndX = dBaseX ;

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数		

								//begin 绘制1CDA-1标签
								double dGCJZLoc1_X = 10.000 ;
								double dGCJZLoc1_Y = -192.000 ;
								CString strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								CString strZhShowName ;
								strZhShowName.Format(_T("GC(%s)"), strJZXHJ) ;

								this->FillXdataGCJZLoc_D(0, strZLayerBlkName, strZhShowName, strCD1Loc, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）

								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -192,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -120,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -120,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -192,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;

									this->FillXdataGCJZLoc_D(0, strZLayerBlkName, strZhShowName, strCD2Loc, mapStrToStrXdata, mapToAddXdata) ;

									double dGCJZLoc2_X = (iRunDirect==0?dCDA2_EndX:dCDA2_StartX) ;
									double dGCJZLoc2_Y =  -192 ;		
									vec.x = dGCJZLoc2_X ;
									vec.y = dGCJZLoc2_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）									
								
								//begin 绘制ZBPU-FBJ-FS块
								double dZBPU_FBJ_FS_Loc_X = dWidth_NGL_Code ;
								double dZBPU_FBJ_FS_Loc_Y = 0 ;
								if (iRunDirect==1)
								{
									dZBPU_FBJ_FS_Loc_X = 0 ;
								}
								this->FillXdataZBPU_FBJ_FS(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strJinLuName,mapToReplace,mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZBPU_FBJ_FS_Loc_X ;
								vec.y = dZBPU_FBJ_FS_Loc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_zbpu-fbj-fs_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZBPU-FBJ-FS块		

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*(iMaxGCJ+(iGuDaoFenGe==1?1:0)) + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dWidth_NGL_Code ; //都从右往左依次绘制1GJF~10GJF
								double dGJFGCJ_Y = dGCJZLoc1_Y-20 ;
								if (iRunDirect==1)
								{
									dGJFGCJStartX = dGJFGCJTotalWidth ; 
								}
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ	

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 股道无分割插入GCJGJF_0000、GCJGJF-JFMJ_0000块									
									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{
										//GCJGJF-GJF_0000										
										this->FillXdataGCJGJF_GJF_D(0, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGJFGCJ_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gjf_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ_D(0, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.y = dGJFGCJ_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gcj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ,mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//正接ZJC-JMJ:GCJGJF-JFMJ_0000
										this->FillXdataGCJGJF_JFMJ_D(0, strJZXHJ, strFJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.x =dBaseX-dWidthGJFGCJ ;
										vecGJFGCJ.y = dGJFGCJ_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-jmj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace,  mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX = dBaseX-dWidthGJFGCJ-dWidthGCJGJF_JFMJ ;
									}
									//end 股道无分割插入GCJGJF_0000、GCJGJF-JFMJ_0000块

									//begin 绘制GCJGJF

									if (!(iGuDaoFenGe==1&&nGuiDaoIndex==0) && iIsSendCode==1) //第一个是GCJ1，股道,不再1GCJ~10GCJ
									{									
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF_D(0, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGJFGCJ_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ_D(0, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.y = dGJFGCJ_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;										
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gcj_0001_0.dwg"), m_strSysBasePath) ;	
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;
									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZBJH或者GDFS(半自动口)

								strSql.Format(_T("select * from dmh_import where instr(dmh_import_line, '半自动接近')>0 and dmh_import_xhj='%s' and dmh_import_table=1 "), strJZXHJ) ;
								CStringArray strArrayRec ;
								if(CDBUtility::GetRecordValue(strSql, _T("id"), strArrayRec)==0)
								{
									this->FillXdataZBJH_JGJ_1LQ_D(0,  strJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
									vec.x = dBaseX ;
									vec.y = dGJFGCJ_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_zbjh-jgjlqj_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								else//非0说明以strJZ为进站信号机的该线为半自动接近
								{
									//this->FillXdataGDFS_GJF1(strJZXHJ, iMaxGCJ, mapStrToStrXdata, mapToAddXdata) ;
									vec.x = dBaseX ;
									vec.y = dGJFGCJ_Y-8 ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gdfs-gjf1_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}

								//begin 绘制JMJ/FMJ励磁 块
								double dLiCiLoc_X = 0-iMaxGCJ*dWidth_NGL_Code-dWidthZJC_0000-30 ;
								double dLiCiLoc_Y = dGJFGCJ_Y ;
								if (iRunDirect==1)
								{
									dLiCiLoc_X = (iMaxGCJ+1)*dWidth_NGL_Code+dWidthZJC_0000+30 ;
								}
								this->FillXdataLiCi_D(0, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dLiCiLoc_X ;
								vec.y = dLiCiLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_lici_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;	
								
								//绘制一根竖线
								AcGePoint3d pt_3d_line_start(dLiCiLoc_X,0,0), pt_3d_line_end(dLiCiLoc_X,-287,0) ;								
								mysys.AddAcDbPolyLine(pt_3d_line_start, pt_3d_line_end, 0.5) ;

								//除股道外的轨道GJF1
								cSetGuiDao.MoveLast() ;
								double dLiCi_GJFLoc_X = dLiCiLoc_X+(iRunDirect==1?10:-10) ;
								double dLiCi_GJFLoc_Y = dLiCiLoc_Y-31 ; //初始y，依次往上增加13
								int iLiCiGJF1GuiDaoIndex = 0 ;
								while (!cSetGuiDao.IsBOF())
								{
									CString strGuiDao ;										
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;	
									if (strGuiDao.Left(1)==_T("*"))
									{
										cSetGuiDao.MovePrevious() ; //跳过股道
										continue;
									}
									this->FillXdataLiCi_GJF1_FuSi_D(iLiCiGJF1GuiDaoIndex, strGuiDao, mapToReplace, mapStrToStrXdata, mapToAddXdata, 0) ;
									vec.x = dLiCi_GJFLoc_X ;
									vec.y = dLiCi_GJFLoc_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_lici-gjf_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;

									dLiCi_GJFLoc_Y+=12 ;
									iLiCiGJF1GuiDaoIndex++ ;

									cSetGuiDao.MovePrevious() ;
								}
								//绘制GJF1两侧端子连线
								pt_3d_line_start.x = dLiCi_GJFLoc_X ;
								pt_3d_line_start.y = dLiCiLoc_Y-31 ;
								pt_3d_line_end.x =pt_3d_line_start.x ;
								pt_3d_line_end.y = pt_3d_line_start.y+12*(iMaxGCJ-1) ;
								mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

								pt_3d_line_start.x  = pt_3d_line_start.x+(iRunDirect==1?21:-21) ;
								pt_3d_line_end.x =pt_3d_line_start.x ;								
								mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

								//end 绘制JMJ/FMJ励磁 块

								//begin 绘制 fusi
								double dFuSiLoc_X = dLiCiLoc_X-100 ;
								if (iRunDirect==1)
								{
									dFuSiLoc_X = dLiCiLoc_X ;
								}
								double dFuSiLoc_Y = 0 ;
								cSetGuiDao.MoveLast() ;
								while (!cSetGuiDao.IsBOF())
								{
									CString strGuiDao ;									
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;		
									this->FillXdataLiCi_GJF1_FuSi_D(-1, strGuiDao, mapToReplace, mapStrToStrXdata, mapToAddXdata, 0) ;
									vec.x = dFuSiLoc_X ;
									vec.y = dFuSiLoc_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_fusi_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;

									dFuSiLoc_Y-=25 ;									

									cSetGuiDao.MovePrevious() ;
								}								
								
								//end 绘制 fusi

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = -10 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = -11 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //出站信号机
									{
										strXhjName = strCZXHJ ;
									}
									else if (nGuiDaoIndex==iGuDaoFenGe) //反向出站信号机
									{
										strXhjName = strFCZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}
									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}					


									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制进站
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = -11 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strJZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}								

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,-11,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = -5 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//end 绘制示意图								
								
								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -297+10, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -297+10, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, 0, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, 0, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -297+5, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -297+5, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, 5, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, 5, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s进站正向接车进路电码化电路图"), strJZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏
							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段
					}
					else if (strJinLuType==_T("正发"))
					{
						
						strSaveFileName.Format(_T("%s\\%s%s正向发车进路电码化电路图.dwg"), strSavePath, m_strStation, strStartEndXhj) ;
						acutPrintf(_T("\n%s"), strSaveFileName) ;
						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;
						iMaxGCJ_ZF = iMaxGCJ ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标
								double dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								double dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X

								int iGCJLayerPre = 0 ;
								int iZLocIndex = 1 ;
								CString strCD1Loc, strCD2Loc ; //记录CD-1的zloc和CD-2的zloc（若有）
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端

									CString strGCJName ;
									int iGCJShelf = 0, iGCJLayer = 0, iGCJBox = 0, iGCJIndex = 0  ;
									this->GetGCJInfo_D(strGuiDao, strGCJName, iGCJShelf, iGCJLayer, iGCJBox) ;
									if (nGuiDaoIndex==0)
									{
										iGCJLayerPre = iGCJLayer ;
										strCD1Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
									}
									else if (iGCJLayerPre!=iGCJLayer && iZLocIndex!=2)
									{
										iZLocIndex = 2 ;
										strCD2Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
										//记录1CDA-2图框起始位置
										dCDA2_StartX = dBaseX ;
									}

									CString strGCJIndex ;
									CString strReg = _T("^.+(\\d+)GCJ$") ;
									if (CBlkUtility::IsStrMatch(strReg, strGCJName))
									{
										CBlkUtility::RegReplace(strReg, _T("$1"), strGCJName, strGCJIndex) ;
										iGCJIndex = _tstoi(strGCJIndex) ;
									}

									this->FillXdataNGL_D(iGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_00%d%d_0.dwg"), m_strSysBasePath, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_1%d%d0_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,-20,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
									}

									xformNGL.setToTranslation(vecNGL);												
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									//绘制GCJ
									if (iIsSendCode==1) 
									{
										this->FillXdataGCJ_D(nGuiDaoIndex, iGCJIndex, iGCJBox, iZLocIndex, strGCJName, mapToReplace, mapStrToStrXdata, mapToAddXdata, 2) ;
										double dGCJ_X = dBaseX+dOffsetX_NGL_GCJ+(iRunDirect==0?0:-dWidth_NGL_Code) ;
										vecNGL.x = dGCJ_X ;
										vecNGL.y = dGCJ_Y ;
										xformNGL.setTranslation(vecNGL) ;
										//strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_gcj_0%d%d%d_%d.dwg"), m_strSysBasePath, iGuDaoFenGe==1?0:1, nGCJIndex<10?0:1,nGCJIndex<10?nGCJIndex:0, iRunDirect) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcj_000%d_%d.dwg"), m_strSysBasePath, nGuiDaoIndex%2==0?2:1, iRunDirect) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									// end 绘制GCJ

									dCDA2_EndX = dBaseX ;
									
									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数

								//begin 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）
								double dGCJZLoc1_X = 10.000 ;
								double dGCJZLoc1_Y = -192.000 ;
								CString strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								CString strZhShowName ;
								strZhShowName.Format(_T("GC(%s)"), strJZXHJ) ;

								this->FillXdataGCJZLoc_D(0, strZLayerBlkName, strZhShowName, strCD1Loc, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）

								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -192,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -120,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -120,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -192,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;

									this->FillXdataGCJZLoc_D(0, strZLayerBlkName, strZhShowName, strCD2Loc, mapStrToStrXdata, mapToAddXdata) ;

									double dGCJZLoc2_X = (iRunDirect==0?dCDA2_EndX:dCDA2_StartX) ;
									double dGCJZLoc2_Y =  -192 ;		
									vec.x = dGCJZLoc2_X ;
									vec.y = dGCJZLoc2_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）

								//begin 绘制正发 ZF_FMJ
								double dZF_FMJ_X = dWidth_NGL_Code ;
								double dZF_FMJ_Y = -131.8248 ;
								if (iRunDirect==1)
								{
									dZF_FMJ_X = 0-dWidthZF_FMJ  ; 
								}
								this->FillXdataFMJ_D(strJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZF_FMJ_X ;
								vec.y = dZF_FMJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_fmj_2000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZF_FMJ	

								//begin 绘制ZBPU-FBJ-FS块
								double dZBPU_FBJ_FS_Loc_X = dWidth_NGL_Code+dWidthZF_FMJ ;
								double dZBPU_FBJ_FS_Loc_Y = 0 ;
								if (iRunDirect==1)
								{
									dZBPU_FBJ_FS_Loc_X = 0-dWidthZF_FMJ ;
								}
								this->FillXdataZBPU_FBJ_FS(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strJinLuName,mapToReplace,mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZBPU_FBJ_FS_Loc_X ;
								vec.y = dZBPU_FBJ_FS_Loc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_zbpu-fbj-fs_2000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZBPU-FBJ-FS块

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*iMaxGCJ + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dWidth_NGL_Code+dWidthBM_FMJ+dWidthZBPU ; //都从右往左依次绘制1GJF~10GJF
								double dGJFGCJ_Y = dGCJZLoc1_Y-20 ;
								if (iRunDirect==1)
								{
									dGJFGCJStartX = 0-dWidthBM_FMJ-dWidthZBPU+dGJFGCJTotalWidth ; 
								}
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndexs[2] ; //gcj继电器在组合中的序号（1~10）
									CString strZhNames[2] ;
									CString strJdqNames[2] ;
									this->GetGCJGJFJdqInfo(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuiDao, strZhNames, iJdqIndexs, strJdqNames) ;

									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									
									CString strGCJName ;
									int iGCJShelf = 0, iGCJLayer = 0, iGCJBox = 0, iGCJIndex = 0  ;
									this->GetGCJInfo_D(strGuiDao, strGCJName, iGCJShelf, iGCJLayer, iGCJBox) ;
									if (nGuiDaoIndex==0)
									{
										iGCJLayerPre = iGCJLayer ;
										strCD1Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
									}
									else if (iGCJLayerPre!=iGCJLayer && iZLocIndex!=2)
									{
										iZLocIndex = 2 ;
										strCD2Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
										//记录1CDA-2图框起始位置
										dCDA2_StartX = dBaseX ;
									}

// 									int iGCJNameStrLen = strGCJName.GetLength() ;
// 									int iJZXHJStrLen = strJZXHJ.GetLength() ;									
// 									iGCJIndex = _tstoi(strGCJName.Mid(iJZXHJStrLen,iGCJNameStrLen-iJZXHJStrLen-3)) ;								

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 绘制GCJGJF
									if (nGuiDaoIndex==0) //先插入一个ZF-FMJ块
									{										
										this->FillXdataGCJGJF_JFMJ_D(2, strJZXHJ, strFJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vec.x = dBaseX ;
										vec.y = dGJFGCJ_Y ;
										xform.setTranslation(vec) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-fmj_2000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										
										dBaseX-=dWidthGJFGCJ ;
									}

									if (iIsSendCode==1) 
									{
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF_D(2, nGuiDaoIndex, iMaxGCJ, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;										
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGJFGCJ_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ_D(2, nGuiDaoIndex, iMaxGCJ, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;										
										vecGJFGCJ.y = dGJFGCJ_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gcj_000%d_0.dwg"), m_strSysBasePath, nGuiDaoIndex==0?0:1) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;

									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZF-GJF1(和股道的链接)
								this->FillXdataGCJGJF_GDGJF1_D(strJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dBaseX ;
								vec.y = dGJFGCJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gdgjf1_2000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制GJF-GCJ
								//*********************************************************************************************************************

								//begin 绘制JMJ/FMJ励磁 块
								double dLiCiLoc_X = 0-(iMaxGCJ-1)*dWidth_NGL_Code-30 ;
								double dLiCiLoc_Y = dGJFGCJ_Y ;
								if (iRunDirect==1)
								{
									dLiCiLoc_X = iMaxGCJ*dWidth_NGL_Code+30 ;
								}
								this->FillXdataLiCi_D(2, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dLiCiLoc_X ;
								vec.y = dLiCiLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_lici_2000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;	

								//绘制一根竖线
								AcGePoint3d pt_3d_line_start(dLiCiLoc_X,0,0), pt_3d_line_end(dLiCiLoc_X,-287,0) ;								
								mysys.AddAcDbPolyLine(pt_3d_line_start, pt_3d_line_end, 0.5) ;

								//除股道外的轨道GJF1
								cSetGuiDao.MoveLast() ;
								double dLiCi_GJFLoc_X = dLiCiLoc_X+(iRunDirect==1?10:-10) ;
								double dLiCi_GJFLoc_Y = dLiCiLoc_Y-31 ; //初始y，依次往上增加13
								int iLiCiGJF1GuiDaoIndex = 0 ;
								while (!cSetGuiDao.IsBOF())
								{
									CString strGuiDao ;										
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;	
									if (strGuiDao.Left(1)==_T("*"))
									{
										cSetGuiDao.MovePrevious() ; //跳过股道
										continue;
									}
									this->FillXdataLiCi_GJF1_FuSi_D(iLiCiGJF1GuiDaoIndex, strGuiDao, mapToReplace, mapStrToStrXdata,  mapToAddXdata, 2) ;
									vec.x = dLiCi_GJFLoc_X ;
									vec.y = dLiCi_GJFLoc_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_lici-gjf_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;

									dLiCi_GJFLoc_Y+=12 ;	
									iLiCiGJF1GuiDaoIndex++ ;

									cSetGuiDao.MovePrevious() ;
								}
								//绘制GJF1两侧端子连线
								pt_3d_line_start.x = dLiCi_GJFLoc_X ;
								pt_3d_line_start.y = dLiCiLoc_Y-31 ;
								pt_3d_line_end.x =pt_3d_line_start.x ;
								pt_3d_line_end.y = pt_3d_line_start.y+12*(iMaxGCJ-1) ;
								mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

								pt_3d_line_start.x = pt_3d_line_start.x+(iRunDirect==1?21:-21) ;								
								pt_3d_line_end.x =pt_3d_line_start.x ;								
								mysys.AddAcDbLine(pt_3d_line_start, pt_3d_line_end) ;

								//end 绘制JMJ/FMJ励磁 块

								//begin 绘制 fusi
								double dFuSiLoc_X = dLiCiLoc_X-100 ;
								if (iRunDirect==1)
								{
									dFuSiLoc_X = dLiCiLoc_X ;
								}
								double dFuSiLoc_Y = 0 ;
								cSetGuiDao.MoveLast() ;
								while (!cSetGuiDao.IsBOF())
								{
									CString strGuiDao ;									
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;		
									this->FillXdataLiCi_GJF1_FuSi_D(-1, strGuiDao, mapToReplace, mapStrToStrXdata, mapToAddXdata, 2) ;
									vec.x = dFuSiLoc_X ;
									vec.y = dFuSiLoc_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_fusi_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;

									dFuSiLoc_Y-=25 ;									

									cSetGuiDao.MovePrevious() ;
								}								

								//end 绘制 fusi

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = -10 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = -11 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //反进站信号机
									{
										strXhjName = strFJZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0011_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}

									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									if (nGuiDaoIndex==0&&iRunDirect==0) //反进站信号机较长，故箭头要左移
									{
										pt_3d_insert_syt.x = dBaseX-13 ;
									}
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制CZ
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = -11 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strCZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,-11,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = -5 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制示意图

								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -297+10, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -297+10, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, 0, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, 0, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -297+5, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -297+5, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, 5, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, 5, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s出站正向发车进路电码化电路图"), strCZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏

							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段
					}
					else if (strJinLuType==_T("反接"))
					{
						//GCJ个数
						strArrayGuDao.RemoveAll() ;
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow>6 and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						iMaxGCJ = CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strArrayGuDao) ;

						strSaveFileName.Format(_T("%s\\%s%s反向接车进路电码化电路图.dwg"), strSavePath, m_strStation, strStartEndXhj) ;
						acutPrintf(_T("\n%s"), strSaveFileName) ;

						//begin 遍历该进路的轨道区段
						strSql.Format(_T("select * from dmh_import where dmh_import_line='%s' and dmh_import_type='%s' and dmh_import_cellrow<17 order by dmh_import_cellrow"), strLine, strJinLuType) ;
						CADORecordset cSetGuiDao ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							if (cSetGuiDao.GetRecordCount()>0)
							{
								cSetGuiDao.MoveFirst() ;
								int nGuiDaoIndex = 0 ;
								double dBaseX = 0 ; //当前NGL块插入点的x坐标	
								double dCDA2_StartX = 0 ; //1CDA-2图框的起始坐标X
								double dCDA2_EndX = 0 ; //1CDA-2图框的终止坐标X	
								int iGCJLayerPre = 0 ;
								int iZLocIndex = 1 ;
								CString strCD1Loc, strCD2Loc ; //记录CD-1的zloc和CD-2的zloc（若有）

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									CString strGCJName ;
									int iGCJShelf = 0, iGCJLayer = 0, iGCJBox = 0, iGCJIndex = 0  ;
									this->GetGCJInfo_D(strGuiDao, strGCJName, iGCJShelf, iGCJLayer, iGCJBox) ;
									if (nGuiDaoIndex==0)
									{
										iGCJLayerPre = iGCJLayer ;
										strCD1Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
									}
									else if (iGCJLayerPre!=iGCJLayer && iZLocIndex!=2)
									{
										iZLocIndex = 2 ;
										strCD2Loc.Format(_T("%s%d-%d"), m_strPreZ, iGCJShelf, iGCJLayer) ;
										//记录1CDA-2图框起始位置
										dCDA2_StartX = dBaseX ;
									}

									CString strGCJIndex ;
									CString strReg = _T("^.+(\\d+)GCJ$") ;
									if (CBlkUtility::IsStrMatch(strReg, strGCJName))
									{
										CBlkUtility::RegReplace(strReg, _T("$1"), strGCJName, strGCJIndex) ;
										iGCJIndex = _tstoi(strGCJIndex) ;
									}
								
									this->FillXdataNGL_D(iGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;

									//begin 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元
									//插入配线用电路图
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_00%d%d_0.dwg"), m_strSysBasePath, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_1%d%d0_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0) ;
									}
									AcGeMatrix3d xformNGL;  
									AcGeVector3d vecNGL(0,-20,0) ;

									if (iRunDirect==0)
									{
										if (nGuiDaoIndex==0)
										{
											dBaseX = 0 ;
										}
										else
										{
											dBaseX-=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										}										
										vecNGL.x = dBaseX ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((iGuDaoFenGe==1&&nGuiDaoIndex==0)||(iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1))
										{
											dBaseX-=dWidthZJC_0000 ; 
										}
									}
									else
									{
										vecNGL.x = dBaseX ;
										dBaseX+=(iIsSendCode==0?dWidth_NGL_NoCode:dWidth_NGL_Code) ;
										//无分割的股道放置在第一位,股道和非股道区段之间空出放置ZJC_0000的距离
										//股道分割时，股道和非股道区段之间空出放置ZJC_0000的距离
										if ((nGuiDaoIndex==0&&iGuDaoFenGe==1)||(iGuDaoFenGe>1&&nGuiDaoIndex==iGuDaoFenGe-1))
										{
											dBaseX+=dWidthZJC_0000 ; 
										}
									}

									xformNGL.setToTranslation(vecNGL);												
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata,  _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 数据准备完毕，向电码化图中添加一个单元，并用刚才收集的数据填充该单元

									//begin 股道无分割插入GCJ_0000（GCJ1）和FJ_JMJ块
									this->FillXdataGCJ_D(nGuiDaoIndex, iGCJIndex, iGCJBox, iZLocIndex, strGCJName, mapToReplace, mapStrToStrXdata, mapToAddXdata, 3) ;
									vecNGL.x =vecNGL.x+dOffsetX_NGL_GCJ ;
									vecNGL.y = dGCJ_Y ;
									xformNGL.setTranslation(vecNGL) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcj_000%d_%d.dwg"), m_strSysBasePath, nGuiDaoIndex%2==0?1:2, iRunDirect) ;
									this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformNGL, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{										
										//JMJ
										this->FillXdataJMJ_0000_D(3, strJZXHJ, strFJZXHJ, mapStrToStrXdata, mapToAddXdata) ;										
										vecNGL.x = vecNGL.x+(iRunDirect==0?-dWidthZJC_0000:dWidthGCJ_0000) ;
										xformNGL.setTranslation(vecNGL) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_jmj_3000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xformNGL, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 股道无分割插入ZJC块（GCJ1）

									dCDA2_EndX = dBaseX ;

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//int iGuiDaoSum = nGuiDaoIndex ; //轨道个数

								//begin 绘制1CDA-1标签
								double dGCJZLoc1_X = 10.000 ;
								double dGCJZLoc1_Y = -192.000 ;
								CString strZLayerBlkName = _T("dmh_box_z_1cda_1") ;
								CString strZhShowName ;
								strZhShowName.Format(_T("GC(%s)"), strFJZXHJ) ;

								this->FillXdataGCJZLoc_D(0, strZLayerBlkName, strZhShowName, strCD1Loc, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dGCJZLoc1_X ;
								vec.y = dGCJZLoc1_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制1CDA-1标签（股道无分割，GCJ个数大于5）/1CDA（股道无分割，GCJ个数小于于5）/1CDB（股道分割）

								//begin 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）
								if (dCDA2_StartX!=0)
								{
									AcGePoint3dArray pt_3d_array_1cda2frame ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -192,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_StartX, -120,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -120,0)) ;
									pt_3d_array_1cda2frame.append(AcGePoint3d(dCDA2_EndX, -192,0)) ;
									mysys.AddAcDb2dPolyline(pt_3d_array_1cda2frame, 0.5, 0) ;

									strZLayerBlkName = _T("dmh_box_z_1cda_2") ;

									this->FillXdataGCJZLoc_D(0, strZLayerBlkName, strZhShowName, strCD2Loc, mapStrToStrXdata, mapToAddXdata) ;

									double dGCJZLoc2_X = (iRunDirect==0?dCDA2_EndX:dCDA2_StartX) ;
									double dGCJZLoc2_Y =  -192 ;		
									vec.x = dGCJZLoc2_X ;
									vec.y = dGCJZLoc2_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_zloc_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								//end 若GCJ个数大于5，则要绘制1CDA-2(1CDA-1对应的)或者1CDA(1CDB对应的）		

								//begin 绘制反接 FJC_FMJ
								double dFJ_FMJ_X = dWidth_NGL_Code ;
								double dFJ_FMJ_Y = -131.8248 ;
								if (iRunDirect==1)
								{
									dFJ_FMJ_X = 0-dWidthZF_FMJ ;
								}
								this->FillXdataFMJ_D(strJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dFJ_FMJ_X ;
								vec.y = dFJ_FMJ_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_fmj_3000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制FJC_FMJ

								//begin 绘制ZBPU-FBJ-FS块
								double dZBPU_FBJ_FS_Loc_X = dWidth_NGL_Code+dWidthZF_FMJ ;
								double dZBPU_FBJ_FS_Loc_Y = 0 ;
								if (iRunDirect==1)
								{
									dZBPU_FBJ_FS_Loc_X = 0-dWidthZF_FMJ ;
								}
								this->FillXdataZBPU_FBJ_FS(3, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strJinLuName,mapToReplace,mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dZBPU_FBJ_FS_Loc_X ;
								vec.y = dZBPU_FBJ_FS_Loc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_zbpu-fbj-fs_3000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								//end 绘制ZBPU-FBJ-FS块

								//*********************************************************************************************************************
								//begin 绘制GJF-GCJ
								//计算绘制需要的宽度=ZBJH宽度*1+GCJGJF宽度*轨道个数+ZJC-JMJ(ZJC_0003)宽度*1，若股道无分割，需要多加一个GCJGJF0000的宽度
								double dGJFGCJTotalWidth = dWidthZBJH + dWidthGJFGCJ*(iMaxGCJ+(iGuDaoFenGe==1?1:0)) + dWidthGCJGJF_JFMJ ;
								double dGJFGCJStartX = dWidth_NGL_Code+dWidthBM_FMJ+dWidthZBPU ; //都从右往左依次绘制1GJF~10GJF
								double dGJFGCJ_Y = dGCJZLoc1_Y-20 ;
								if (iRunDirect==1)
								{
									dGJFGCJStartX = 0-dWidthZF_FMJ*2+dGJFGCJTotalWidth ; 
								}
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = dGJFGCJStartX ; //当前块插入点的x坐标
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									
									//输入excel表中轨道所处的行，范围7~16是发码端，17~21是不发码端
									int iIsSendCode = nCellRow>16?0:1 ; //是否发码端
									int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ	

									AcGeMatrix3d xformGJFGCJ;  
									AcGeVector3d vecGJFGCJ(0,0,0) ;

									//begin 股道无分割插入GCJGJF_0000、GCJGJF-JFMJ_0000块									
									if (nGuiDaoIndex==0&&iGuDaoFenGe==1)
									{
										//GCJGJF-GJF_0000										
										this->FillXdataGCJGJF_GJF_D(3, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGJFGCJ_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gjf_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ_D(3, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;										vecGJFGCJ.y = dGJFGCJ_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gcj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//正接ZJC-JMJ:GCJGJF-JFMJ_0000
										this->FillXdataGCJGJF_JFMJ_D(3, strJZXHJ, strFJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.x =dBaseX-dWidthGJFGCJ ;
										vecGJFGCJ.y = dGJFGCJ_Y ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-jmj_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX = dBaseX-dWidthGJFGCJ-dWidthGCJGJF_JFMJ ;
									}
									//end 股道无分割插入GCJGJF_0000、GCJGJF-JFMJ_0000块

									//begin 绘制GCJGJF

									if (!(iGuDaoFenGe==1&&nGuiDaoIndex==0) && iIsSendCode==1) //第一个是GCJ1，股道,不再1GCJ~10GCJ
									{									
										//GCJGJF-GJF_0000
										this->FillXdataGCJGJF_GJF_D(3, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.x =dBaseX ;
										vecGJFGCJ.y = dGJFGCJ_Y-8 ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gjf_0001_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//GCJGJF-GCJ_0000
										this->FillXdataGCJGJF_GCJ_D(3, nGuiDaoIndex, iMaxGCJ+1, strGuiDao, strJZXHJ, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vecGJFGCJ.y = dGJFGCJ_Y-dHeight_GCJGJF_GJF ;
										xformGJFGCJ.setTranslation(vecGJFGCJ) ;										
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gcjgjf-gcj_0001_0.dwg"), m_strSysBasePath) ;	
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xformGJFGCJ, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;

										dBaseX-=dWidthGJFGCJ ;
									}
									// end 绘制GCJGJF

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								//绘制ZBJH或者GDFS(半自动口)

								strSql.Format(_T("select * from dmh_import where instr(dmh_import_line, '半自动接近')>0 and dmh_import_xhj='%s' and dmh_import_table=1 "), strJZXHJ) ;
								CStringArray strArrayRec ;
								if(CDBUtility::GetRecordValue(strSql, _T("id"), strArrayRec)==0)
								{
									this->FillXdataZBJH_JGJ_1LQ_D(3, strJZXHJ, mapStrToStrXdata, mapToAddXdata) ;
									vec.x = dBaseX ;
									vec.y = dGJFGCJ_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_zbjh-jgjlqj_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}
								else//非0说明以strJZ为进站信号机的该线为半自动接近
								{
									//this->FillXdataGDFS_GJF1(strJZXHJ, iMaxGCJ, mapStrToStrXdata, mapToAddXdata) ;
									vec.x = dBaseX ;
									vec.y = dGJFGCJ_Y-8 ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gdfs-gjf1_0000_0.dwg"), m_strSysBasePath) ;
									this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
								}

								//begin lici
								double dLiCiLoc_X = dGJFGCJStartX-dGJFGCJTotalWidth-dWidthLiCi_3000-10 ;
								double dLiCiLoc_Y = dGJFGCJ_Y ;
								if (iRunDirect==1)
								{
									dLiCiLoc_X = dGJFGCJStartX+30 ; //0-dWidthZF_FMJ-dWidthLiCi_3000 ;
								}
								this->FillXdataLiCi_D(3, strJZXHJ, strFCZXHJ, strCZXHJ, strFJZXHJ, strGuDao, mapStrToStrXdata, mapToAddXdata) ;
								vec.x = dLiCiLoc_X ;
								vec.y = dLiCiLoc_Y ;
								xform.setTranslation(vec) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_lici_3000_0.dwg"), m_strSysBasePath) ;
								this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;	
								//end lici

								AcDbBlockReference *pRef = NULL ;
								CStringArray strArrayVal ;

								//begin 绘制示意图
								cSetGuiDao.MoveFirst() ;
								nGuiDaoIndex = 0 ;
								dBaseX = 0 ;
								AcGePoint3d pt_3d_insert_syt(0,0,0) ;

								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;
									if (nCellRow>16)
									{
										break; 
									}

									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									dBaseX = 0-nGuiDaoIndex*dWidth_NGL_Code+dWidth_NGL_Code ;
									if (iRunDirect==1)
									{
										dBaseX = nGuiDaoIndex*dWidth_NGL_Code+22 ;
									}

									//绘制轨道名称
									pt_3d_insert_syt.x = dBaseX+(dWidth_NGL_Code/2)*(iRunDirect==0?-1:1) ;
									pt_3d_insert_syt.y = -10 ;
									mysys.AddText(strGuiDao, pt_3d_insert_syt, pt_3d_insert_syt) ;

									//绘制信号机（按运行方向，该轨道的远方的那个）
									pt_3d_insert_syt.x = dBaseX ;
									pt_3d_insert_syt.y = -11 ;
									CString strXhjName = _T("");

									if (nGuiDaoIndex==0) //反出站信号机
									{
										strXhjName = strFCZXHJ ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect) ;
									}
									else if (nGuiDaoIndex==iGuDaoFenGe) //出站信号机
									{
										strXhjName = strCZXHJ ;	
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0000_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
									}
									strArrayVal.RemoveAll() ;
									strArrayVal.Add(strXhjName) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}
									//绘制标示接收/发送箭头
									pt_3d_insert_syt.x = dBaseX+(iRunDirect==0?-5:5) ;
									int iIsFOrR = 0 ;
									if (strGuiDao.Right(1)==_T("+"))
									{
										iIsFOrR = 1 ;
									}
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0000_%d.dwg"), m_strSysBasePath, iIsFOrR) ;
									if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
									{
										pRef->close() ;
										pRef = NULL ;
									}

									nGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}
								//绘制反进站
								pt_3d_insert_syt.x = dBaseX+dWidth_NGL_Code*(iRunDirect==0?-1:1) ;
								pt_3d_insert_syt.y = -11 ;			
								strArrayVal.RemoveAll() ;
								strArrayVal.Add(strFJZXHJ) ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-xhj_0011_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, &strArrayVal, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//绘制轨道直线
								AcGePoint3d pt_3d_start(0,-11,0), pt_3d_end(0,0,0) ;
								pt_3d_start.x = (iRunDirect==0?dWidth_NGL_Code:22) ;
								pt_3d_end = pt_3d_insert_syt ;
								mysys.AddAcDbLine(pt_3d_start, pt_3d_end) ;

								//绘制运行方向
								pt_3d_insert_syt.y = -5 ;
								strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuit_syt-jt_0001_%d.dwg"), m_strSysBasePath, iRunDirect) ;
								if(mysys.InsertBlock(pRef, strCircuitTemplete, pt_3d_insert_syt, FALSE, 1))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								//end 绘制示意图

								//begin 绘制整个电路图的边框和签字栏
								AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
								mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

								//内边框4个点
								AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -297+10, 0) ;
								AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -297+10, 0) ;
								AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, 0, 0) ;
								AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, 0, 0) ;
								//外边框4个点
								AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -297+5, 0) ;
								AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -297+5, 0) ;
								AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, 5, 0) ;
								AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, 5, 0) ;

								AcGePoint3d pt_3d_insert(0,0,0) ;

								CString strBlockName ;
								//左边frame
								strBlockName = _T("frame_left") ;
								pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
								pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
								{
									pRef->close() ;
									pRef = NULL ;
								}

								AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
								AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
								CLongArray iArrayHandle ;
								double dframe_sign_3_hor_width = 180.000 ;
								double dframe_sign_3_hor_height = 34.000 ;								

								//判断右下角是否为空，为空则绘制一个横向的frame于此
								pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
								pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
								pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
								pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

								iArrayHandle.RemoveAll() ;								
								int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
								if (iFindEntSum==0)
								{									
									strBlockName = _T("SIGN_3") ;
								}
								else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
								{
									pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
									pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
									pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
									pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
									iArrayHandle.RemoveAll() ;
									iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
									//其他情况只能往右延展绘制一个竖向的frame
									if (iFindEntSum!=0)
									{
										pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
										pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
									}

									strBlockName = _T("frame_sign_3_ver_new") ;
								}	

								//内边框
								AcGePoint3dArray ptArrayFrame ;
								ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
								ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
								ptArrayFrame.append(pt_3d_frame_in_topright) ;
								ptArrayFrame.append(pt_3d_frame_in_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
								//外边框
								ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
								ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
								ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
								ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
								mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

								//签字栏
								pt_3d_insert = pt_3d_frame_in_bottomright ;

								strArrayVal.RemoveAll() ;
								strArrayVal.Add(m_strStation) ;
								strArrayVal.Add(m_strDwgSn) ;		
								strArrayVal.Add(CBlkUtility::GetDateStr()) ;
								CString strDwgName ;
								strDwgName.Format(_T("%s进站反向接车进路电码化电路图"), strFJZXHJ) ;
								strArrayVal.Add(strDwgName) ;
								strArrayVal.Add(m_strProjectName) ;
								if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
								{
									pRef->close() ;
									pRef = NULL ;
								}
								//end 绘制整个电路图的边框和签字栏

							}
							cSetGuiDao.Close() ;
						}
						//end 遍历该进路的轨道区段					

					}



					//删除没用的端子
					this->DelNoUserEnt(pSaveDb) ;

					//将sn转换为ID
					this->TransMySnToId(pSaveDb) ;

					//删除过程sn等xdata
					//this->DelSn(pSaveDb) ;

					mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
					mysys.SetLayerStatus(_T("hide"), true) ;
					mysys.ModifyFontStyle(_T("Standard"), _T("hztxt.shx"), TRUE) ;
					//mysys.ModifyFontStyle(_T("HZ"), _T("Times New Roman"), FALSE) ;

					if (!strSaveFileName.IsEmpty())
					{
						pSaveDb->saveAs(strSaveFileName) ;
					}
					delete pSaveDb ;
					pSaveDb = NULL ;

					cSet.MoveNext() ;

					iDwgIndex++ ;
				}
			}
			//acedRestoreStatusBar() ;

			cSet.Close() ;
		}	
					

		//处理第二个table		
		m_ctrlProgressAll->SetPos(2) ;		
		
		//::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)1, 0) ;		
		double dOffsetY_CeXian = -30 ;//侧线各块Y坐标下沉60
		dGCJ_Y+=dOffsetY_CeXian ; 
		strSql.Format(_T("select distinct dmh_import_line,dmh_import_lineindex,dmh_import_jfm from dmh_import where dmh_import_table=1 order by dmh_import_lineindex")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int nRsCount = cSet.GetRecordCount() ;

			m_ctrlProgressCur->SetRange(1, nRsCount) ;

			cSet.MoveFirst() ;

			int iDwgIndex = 0 ;
			while (!cSet.IsEOF())
			{
				m_ctrlProgressCur->SetPos(iDwgIndex+1) ;

				//一条记录就是一张图纸
				CString strSaveFileName ; //存储电码化图纸的文件名					
				AcDbDatabase *pSaveDb = new AcDbDatabase ; //存储数据库
				mysys.m_pDb = pSaveDb;	
				//记录扩展数据值
				CMapStringToString mapToReplace, mapStrToStrXdata, mapToAddXdata ;
				CString strCircuitTemplete ; //当前选择的底图模板
				AcGeMatrix3d xform;  
				AcGeVector3d vec(0,0,0) ;

				CString strLine ; //线路名称，例：同一股道1、 调车接近轨
				CString strJFM ;
				int iLineIndex = 0 ;//线别序号
				cSet.GetFieldValue(_T("dmh_import_line"), strLine) ;
				cSet.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;
				cSet.GetFieldValue(_T("dmh_import_lineindex"), iLineIndex) ; //数据库Access初定义dmh_import_lineindex为整形，此处读不出，改成长整形ok
				//cSet.GetFieldValue(_T("dmh_import_lineindex"), iLineIndex) ;

				int iJinLuType = 4 ; //进路类型，4：侧线股道；5：调车接近轨；6：半自动接近轨
				if (strLine.Find(_T("调车接近"))!=-1)
				{
					iJinLuType = 5 ;
				}
				else if (strLine.Find(_T("半自动接近"))!=-1)
				{
					if (strJFM==_T("JG"))
					{
						iJinLuType = 6 ;
					}
					else if (strJFM==_T("FC"))
					{
						iJinLuType = 8 ;
					}
				}
				else if (strLine.Find(_T("中岔"))!=-1)
				{
					iJinLuType = 7 ;
				}

				strMsg.Format(_T("侧线电码化-%s(%d/%d)"), strLine, iDwgIndex+1, nRsCount) ;
				::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)(&strMsg), 0) ;

				CString strGuDao = _T("") ;
				//type	    jfm	 freq  xhj	cellcol
				//上行频率	FC		   S5	 E
				//下行频率	FC		   X5	 F
				CADORecordset cSetJinLu ;	
				strSql.Format(_T("select distinct dmh_import_type,dmh_import_jfm,dmh_import_freq, dmh_import_xhj,dmh_import_cellcol, dmh_import_jinluname from dmh_import where dmh_import_table=1 and  dmh_import_lineindex=%d order by dmh_import_cellcol"), iLineIndex) ;
				if (cSetJinLu.IsOpen())
				{
					cSetJinLu.Close() ;
				}
				if (cSetJinLu.Open(g_PtrCon, strSql))
				{
					int iJinLuNum = cSetJinLu.GetRecordCount() ;

					cSetJinLu.MoveFirst() ;

					while (!cSetJinLu.IsEOF())
					{
						CString strType, strJfm, strFreq, strXhj, strJinLuName ;
						cSetJinLu.GetFieldValue(_T("dmh_import_type"), strType) ; //例：
						cSetJinLu.GetFieldValue(_T("dmh_import_jfm"), strJfm) ;
						cSetJinLu.GetFieldValue(_T("dmh_import_freq"), strFreq) ;
						cSetJinLu.GetFieldValue(_T("dmh_import_xhj"), strXhj) ;
						cSetJinLu.GetFieldValue(_T("dmh_import_jinluname"), strJinLuName) ;					

						if (strGuDao.IsEmpty())
						{
							strGuDao.Format(_T("%sG"), strXhj.Right(strXhj.GetLength()-1)) ;
							if (strXhj.Left(1)==_T("D"))
							{
								strGuDao.Format(_T("%sG"), strXhj) ;
							}
						}

						int iUpOrDown = 0 ; //0：下行，1：上行
						int iRunDirect = 0 ; //运行方向，决定图纸布局，0，从左往右，1，从右往左
						if (strType.Find(_T("下行"))!=-1)
						{
							iUpOrDown = 0 ;
							iRunDirect = 1 ;								
						}
						else
						{
							iUpOrDown = 1 ;
							iRunDirect = 0 ;
						}

						//begin begin 绘制ZBPU-FBJ-FS块
						double dZBPU_FBJ_FS_Loc_X = dWidthZBPU_FBJ_FS_GD ;
						double dZBPU_FBJ_FS_Loc_Y = dOffsetY_CeXian ;
						if (iRunDirect==0)
						{
							dZBPU_FBJ_FS_Loc_X = 0-dWidthZBPU_FBJ_FS_GD ;
						}						
						this->FillXdataZBPU_FBJ_FS_CeXian(iJinLuType, strXhj, strJinLuName, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
						vec.x = dZBPU_FBJ_FS_Loc_X ;
						vec.y = dZBPU_FBJ_FS_Loc_Y ;
						xform.setTranslation(vec) ;								
						strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_zbpu-fbj-fs_%d000_%d.dwg"), m_strSysBasePath, iJinLuType, iRunDirect) ;
						this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
						//end begin 绘制ZBPU-FBJ-FS块

						//begin 绘制NGL、GJF1等
						CADORecordset cSetGuiDao ;
						strSql.Format(_T("select * from dmh_import where dmh_import_table=1 and dmh_import_jinluname='%s' order by dmh_import_cellrow"), strJinLuName) ;
						if (cSetGuiDao.IsOpen())
						{
							cSetGuiDao.Close() ;
						}
						if (cSetGuiDao.Open(g_PtrCon, strSql))
						{
							int iGuiDaoCount = cSetGuiDao.GetRecordCount() ;
							if (iGuiDaoCount>0)
							{
								cSetGuiDao.MoveFirst() ;
								int iGuiDaoIndex = 0 ;

								int iGCJLayerPre = 0 ;
								int iZLocIndex = 1 ;
								CString strCD1Loc, strCD2Loc ; //记录CD-1的zloc和CD-2的zloc（若有）
								while (!cSetGuiDao.IsEOF())
								{
									CString strGuiDao ;									
									int nCellRow = 0 ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;
									cSetGuiDao.GetFieldValue(_T("dmh_import_cellrow"), nCellRow) ;

									int iJdqIndex = 0 ; //GJF1继电器在GDFS组合中的序号（1~8）

									CString strIsROrF = _T("F") ;
									if(strGuiDao.Right(1)==_T("+"))
									{
										strIsROrF = _T("R") ;
									}
									//输入excel表中轨道所处的行，范围6~15是发码端，16~20是不发码端
									int iIsSendCode = nCellRow>15?0:1 ; //是否发码端
									//int nGCJIndex = nCellRow-6 ; //GCJ的序号，0表示股道无分割的情况GCJ1	,其他表示1GCJ~10GCJ
									
									//int iGDShelf = 0, iGDLayer = 0, iGDBox = 0 ;		
								
									//begin 绘制NGL
									double dNGLLoc_X = dZBPU_FBJ_FS_Loc_X+iGuiDaoIndex*dWidth_NGL_Code-dOffsetX_NGL_GCJ ; 
									double dNGLLoc_Y = -20+dOffsetY_CeXian ;
									if (iRunDirect==0)
									{
										dNGLLoc_X = dZBPU_FBJ_FS_Loc_X-(iGuiDaoIndex+1)*dWidth_NGL_Code-dOffsetX_NGL_GCJ ;
									}
									int iGCJIndex = iGuiDaoIndex+1+(iRunDirect==0?200:100) ;
									this->FillXdataNGL_D(iGCJIndex, strGuiDao, strIsROrF, iIsSendCode, mapStrToStrXdata, mapToAddXdata, strJinLuName) ;
									vec.x = dNGLLoc_X ;
									vec.y = dNGLLoc_Y ;
									xform.setTranslation(vec) ;
									strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_00%d%d_0.dwg"), m_strSysBasePath, strIsROrF==_T("F")?1:0, iIsSendCode) ;
									if (m_iTwoOrFourLine==4)
									{
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_ngl_1%d%d0_0.dwg"), m_strSysBasePath, m_bIsFLInZH?0:1, strIsROrF==_T("F")?1:0) ;
									}									
									if (!(m_iTwoOrFourLine==4&&iIsSendCode==0)) //四线制时，不发码的NGL不用绘制？
									{
										this->InsertDwgToDb(strCircuitTemplete, pSaveDb, xform, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
									}
									//end 绘制NGL

									if (iIsSendCode==1)
									{
										//begin 绘制GJF2
										this->FillXdataGJF2_CeXian(iJinLuType, strJinLuName, strGuiDao, iGCJIndex, mapToReplace, mapStrToStrXdata, mapToAddXdata) ;
										vec.x = vec.x+dOffsetX_NGL_GCJ ;
										vec.y = dGCJ_Y ;
										xform.setTranslation(vec) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_gjf2_0001_%d.dwg"), m_strSysBasePath, iRunDirect==0?1:0) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
										//end 绘制GJF2
									}

									iGuiDaoIndex++ ;
									cSetGuiDao.MoveNext() ;
								}

								if (iJinLuType==7&&iUpOrDown==1)
								{
									//begin 绘制 fusi
									double dFuSiLoc_X = -350 ;
									double dFuSiLoc_Y = -225 ;
									cSetGuiDao.MoveLast() ;
									while (!cSetGuiDao.IsBOF())
									{
										CString strGuiDao ;									
										cSetGuiDao.GetFieldValue(_T("dmh_import_guidao"), strGuiDao) ;		
										this->FillXdataLiCi_GJF1_FuSi_D(-1, strGuiDao, mapToReplace, mapStrToStrXdata,  mapToAddXdata, iJinLuType) ;
										vec.x = dFuSiLoc_X ;
										vec.y = dFuSiLoc_Y ;
										xform.setTranslation(vec) ;
										strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_circuitd_fusi_0000_0.dwg"), m_strSysBasePath) ;
										this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform, mapToReplace, mapStrToStrXdata, mapToAddXdata,_T("TYPE"), mysys.GetMaxId()) ;

										dFuSiLoc_X-=90 ;									

										cSetGuiDao.MovePrevious() ;
									}								

									//end 绘制 fusi
								}

							}

							cSetGuiDao.Close() ;
						}
						//end 绘制NGL、GJF1等

						cSetJinLu.MoveNext() ;
					}

					//begin 绘制整个电路图的边框和签字栏
					AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
					mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

					//内边框4个点
					AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x-5, -297+10, 0) ;
					AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -297+10, 0) ;
					AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, 0, 0) ;
					AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x-5, 0, 0) ;
					//外边框4个点
					AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25-5, -297+5, 0) ;
					AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -297+5, 0) ;
					AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, 5, 0) ;
					AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25-5, 5, 0) ;

					AcGePoint3d pt_3d_insert(0,0,0) ;

					CString strBlockName ;
					//左边frame
					strBlockName = _T("frame_left") ;
					pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
					pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
					AcDbBlockReference *pRef = NULL ;
					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
					{
						pRef->close() ;
						pRef = NULL ;
					}

					AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
					AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
					CLongArray iArrayHandle ;
					double dframe_sign_3_hor_width = 180.000 ;
					double dframe_sign_3_hor_height = 34.000 ;								

					//判断右下角是否为空，为空则绘制一个横向的frame于此
					pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
					pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
					pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
					pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

					iArrayHandle.RemoveAll() ;								
					int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
					if (iFindEntSum==0)
					{									
						strBlockName = _T("SIGN_3") ;
					}
					else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
					{
						pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
						pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
						pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
						pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
						iArrayHandle.RemoveAll() ;
						iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
						//其他情况只能往右延展绘制一个竖向的frame
						if (iFindEntSum!=0)
						{
							pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
							pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
							pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
							pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
						}

						strBlockName = _T("frame_sign_3_ver_new") ;
					}	

					//内边框
					AcGePoint3dArray ptArrayFrame ;
					ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
					ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
					ptArrayFrame.append(pt_3d_frame_in_topright) ;
					ptArrayFrame.append(pt_3d_frame_in_topleft) ;
					mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
					//外边框
					ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
					ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
					ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
					ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
					mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

					//签字栏
					CStringArray strArrayVal ;
					pt_3d_insert = pt_3d_frame_in_bottomright ;

					strArrayVal.RemoveAll() ;
					strArrayVal.Add(m_strStation) ;
					strArrayVal.Add(m_strDwgSn) ;		
					strArrayVal.Add(CBlkUtility::GetDateStr()) ;
					CString strDwgName ;
					strDwgName.Format(_T("股道电码化电路图(%s)"), strGuDao) ;
					strArrayVal.Add(strDwgName) ;
					strArrayVal.Add(m_strProjectName) ;
					if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
					{
						pRef->close() ;
						pRef = NULL ;
					}
					//end 绘制整个电路图的边框和签字栏

					cSetJinLu.Close() ;
				}

				strSaveFileName.Format(_T("%s\\%s侧线股道电码化电路图(%s).dwg"), strSavePath, m_strStation, strGuDao) ;

				//删除没用的端子
				this->DelNoUserEnt(pSaveDb) ;

				//将sn转换为ID
				this->TransMySnToId(pSaveDb) ;

				//删除过程sn等xdata
				//this->DelSn(pSaveDb) ;

				mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
				mysys.SetLayerStatus(_T("hide"), true) ;
				mysys.ModifyFontStyle(_T("Standard"), _T("hztxt.shx"), TRUE) ;

				pSaveDb->saveAs(strSaveFileName) ;

				delete pSaveDb ;
				pSaveDb = NULL ;

				cSet.MoveNext() ;
				iDwgIndex++ ;
			}

			cSet.Close() ;
		}
				
		//处理n+1
		m_ctrlProgressAll->SetPos(3) ;
		//::SendMessage(m_hWndToSendMsg, WM_USER_THREAD_PROGRESSRANGE, (WPARAM)1, 0) ;		
		//double dOffsetY_CeXian = -60.000 ;//侧线各块Y坐标下沉60
		//dGCJ_Y+=dOffsetY_CeXian ;
		CStringArray strArrrayJinLu ;
		//因为n+1图纸排序是按XJM,SJM,XFM,SFM,侧线,一个查询搞不定，故多个查询汇成数组
		//ZJ
		strSql.Format(_T("select distinct dmh_import_jinluname, dmh_import_cellcol from dmh_import where dmh_import_type='正接' and dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		CDBUtility::GetRecordValue(strSql, _T("dmh_import_jinluname"),  strArrrayJinLu) ;
		//ZF FJ
		strSql.Format(_T("select distinct dmh_import_jinluname, dmh_import_cellcol from dmh_import where dmh_import_type='正发' and dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		CStringArray strArrrayJinLuTmp ;
		int iZFCount = CDBUtility::GetRecordValue(strSql, _T("dmh_import_jinluname"),  strArrrayJinLuTmp) ;
		for (int iZFIndex=0; iZFIndex<iZFCount; iZFIndex++)
		{
			strSql.Format(_T("SELECT * FROM dmh_in_zcell where  dmh_in_zcell_jdqname='FBJ' and  dmh_in_zcell_cellcontent like '%%%s%%' and dmh_in_zcell_layerid in (select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_fb') "), strArrrayJinLuTmp.GetAt(iZFIndex)) ;
			CString strZF_FJJinLu ;
			CDBUtility::GetRecordValue(strSql, _T("dmh_in_zcell_cellcontent"),  strZF_FJJinLu) ;
			if (strZF_FJJinLu.IsEmpty())
			{
				strArrrayJinLu.Add(strArrrayJinLuTmp.GetAt(iZFIndex)) ;
			}
			else
			{
				strArrrayJinLu.Add(strZF_FJJinLu) ;
			}
		}
		//第三线接车
		strSql.Format(_T("select distinct dmh_import_jinluname, dmh_import_cellcol from dmh_import where dmh_import_type='接码' and dmh_import_table=0 order by dmh_import_cellcol asc")) ;
		CDBUtility::GetRecordValue(strSql, _T("dmh_import_jinluname"),  strArrrayJinLu) ;

		//侧线
		strSql.Format(_T("select distinct dmh_import_lineindex, dmh_import_cellcol, dmh_import_jinluname from dmh_import where  dmh_import_table=1 order by dmh_import_lineindex asc, dmh_import_cellcol desc")) ;
		CDBUtility::GetRecordValue(strSql, _T("dmh_import_jinluname"),  strArrrayJinLu) ;

		int nRsCount =strArrrayJinLu.GetCount() ;

		m_ctrlProgressCur->SetRange(1, nRsCount) ;

		CString strSaveFileName ; //存储n+1图纸的文件名	
		strSaveFileName.Format(_T("%s\\%sn+1电路图.dwg"), strSavePath, m_strStation) ;

		AcDbDatabase *pSaveDb = new AcDbDatabase ; //存储数据库
		mysys.m_pDb = pSaveDb;	
		//记录扩展数据值
		CMapStringToString mapToReplace, mapStrToStrXdata, mapToAddXdata ;
		CString strCircuitTemplete ; //当前选择的底图模板
		AcGeMatrix3d xform;  
		AcGeVector3d vec(0,0,0) ;

		int iFBJIndex = 0 ;
		double dBaseX = 0 ; //插入点
		double dBaseY = -257.000 ;
		double dBlockWidth = 0 ; //插入块的宽度

		AcDbBlockReference *pRef = NULL ;
		CStringArray strArrayVal ;

		CString strBlockName = _T("") ;
		AcGePoint3d pt_3d_insert(0,0,0) ;

		for (int iJinLuIndex=0; iJinLuIndex<nRsCount; iJinLuIndex++)
		{
			m_ctrlProgressCur->SetPos(iFBJIndex+1) ;

			dBlockWidth = 100.000 ;

			CString strJinLuName ;
			strJinLuName = strArrrayJinLu.GetAt(iJinLuIndex) ;		

			acutPrintf(_T("\n*********%s"), strJinLuName) ;

			int iFindMarkLoc = strJinLuName.Find(_T("/")) ; //有无共用fs的情况
			CString strJinLuNameTmp  = strJinLuName ;
			if (iFindMarkLoc!=-1)
			{
				strJinLuNameTmp = strJinLuName.Left(iFindMarkLoc) ;
			}

			//查询该进路相关信息
			CString strJZ, strFCZ, strCZ, strFJZ ; //取得进出站信号机以便填充扩展数据
			CString strLine, strJinLuType, strJFM ; 
			strSql.Format(_T("select * from dmh_import where dmh_import_jinluname='%s'"), strJinLuNameTmp) ;
			CADORecordset cSetFindOneJinLu ;
			if (cSetFindOneJinLu.IsOpen())
			{
				cSetFindOneJinLu.Close() ;
			}
			if (cSetFindOneJinLu.Open(g_PtrCon, strSql))
			{
				int iFindOneJinLuCount = cSetFindOneJinLu.GetRecordCount() ;
				if (iFindOneJinLuCount>0)
				{
					cSetFindOneJinLu.MoveFirst() ;

					cSetFindOneJinLu.GetFieldValue(_T("dmh_import_line"), strLine) ;
					cSetFindOneJinLu.GetFieldValue(_T("dmh_import_type"), strJinLuType) ;
					cSetFindOneJinLu.GetFieldValue(_T("dmh_import_jfm"), strJFM) ;
					int iTableIndex = 0 ;
					cSetFindOneJinLu.GetFieldValue(_T("dmh_import_table"), iTableIndex) ;
					if (iTableIndex==0)
					{
						this->GetLineInfo(strLine, strJZ, strFCZ, strCZ, strFJZ) ; //正线
					}
					else
					{
						CString strXhj ;
						cSetFindOneJinLu.GetFieldValue(_T("dmh_import_xhj"), strXhj) ; //侧线、调车等
						if (strLine.Find(_T("半自动接近"))!=-1)
						{
							if (strJFM==_T("JG"))
							{
								strJZ = strXhj ;
							}
							else if  (strJFM==_T("FC"))
							{
								strCZ = strXhj ;
							}
						}
						else	
						{
							strCZ = strXhj ;
						}							
					}
				}

				cSetFindOneJinLu.Close() ;
			}

			strBlockName = _T("") ;
			int iUpOrDown = 0 ; //0:下行，1：上行
			if (strLine.Find(_T("上行"))!=-1)
			{
				iUpOrDown = 1 ;
			}
			else if (strJinLuType.Find(_T("上行"))!=-1)
			{
				iUpOrDown = 1 ;
			}

			int iJinLuType = 0 ; //进路类型，-1:FS；4：侧线股道；5：调车接近轨；6：半自动接近轨

			if (strJinLuType==_T("正接"))
			{
				iJinLuType = 0 ;
				strBlockName.Format(_T("dmh_n+1d_zj_0000_%d"), iUpOrDown) ;
				dBlockWidth = 120.000 ;
			}
			else if (strJinLuType==_T("正发")||strJinLuType==_T("反接"))
			{
				iJinLuType = 2 ;
				strBlockName.Format(_T("dmh_n+1d_zf-fj_0000_%d"), iUpOrDown) ;
				dBlockWidth = 120.000 ;
			}
			else if (strJinLuType==_T("接码"))
			{
				iJinLuType = 9 ;
				strBlockName.Format(_T("dmh_n+1d_jg-jm_0000_%d"), iUpOrDown) ;
				dBlockWidth = 120.000 ;
			}
			else if (strLine.Find(_T("同一股道"))!=-1)
			{
				iJinLuType = 4 ;
				strBlockName.Format(_T("dmh_n+1d_gd_0000_%d"), iUpOrDown) ;
				dBlockWidth = 50.000 ;
			}
			else if (strLine.Find(_T("调车接近"))!=-1)
			{
				iJinLuType = 5 ;
				strBlockName.Format(_T("dmh_n+1d_dj_0000_%d"), iUpOrDown) ;
				dBlockWidth = 50.000 ;
			}
			else if (strLine.Find(_T("半自动接近"))!=-1)
			{
				if (strJFM==_T("JG"))
				{
					iJinLuType = 6 ;
					strBlockName.Format(_T("dmh_n+1d_jg-jj_0000_%d"), iUpOrDown) ;
					dBlockWidth = 100.000 ;
				}
				else if (strJFM==_T("FC"))
				{
					iJinLuType = 8 ;
					strBlockName.Format(_T("dmh_n+1d_jg-gd_0000_%d"), iUpOrDown) ;
					dBlockWidth = 50.000 ;
				}
			}
			else if (strLine.Find(_T("中岔"))!=-1)
			{
				iJinLuType = 7 ;
				strBlockName.Format(_T("dmh_n+1d_zc_0000_%d"), iUpOrDown) ;
				dBlockWidth = 50.000 ;
			}

			if (!strBlockName.IsEmpty())
			{
				if (iFBJIndex==0) //先绘制FS和+1FBJ
				{					
					this->FillXdataNAdd1_D(-1, _T("+1"), strJZ, strFCZ, strCZ, strFJZ, mapStrToStrXdata, mapToAddXdata, mapToReplace, iUpOrDown) ;

					vec.x = dBaseX ;
					vec.y = dBaseY ;
					xform.setTranslation(vec) ;
					strCircuitTemplete.Format(_T("%s\\support\\dmh\\dmh_n+1d_fs_000%d_0.dwg"), m_strSysBasePath, m_bUseHLP==TRUE?1:0) ;
					this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform,mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
					dBaseX+=140 ;
				}

				this->FillXdataNAdd1_D(iJinLuType, strJinLuName, strJZ, strFCZ, strCZ, strFJZ, mapStrToStrXdata, mapToAddXdata, mapToReplace, iUpOrDown) ;

				vec.x = dBaseX ;
				vec.y = dBaseY ;
				xform.setTranslation(vec) ;
				strCircuitTemplete.Format(_T("%s\\support\\dmh\\%s.dwg"), m_strSysBasePath, strBlockName) ;
				this->InsertDwgToDb1(strCircuitTemplete, pSaveDb, xform,mapToReplace, mapStrToStrXdata, mapToAddXdata, _T("TYPE"), mysys.GetMaxId()) ;
			}

			dBaseX+=dBlockWidth ;//即使strBlockName为空，也要讲dBaseX加dBlockWidth（默认100）,以便查漏
			iFBJIndex++ ;
			acutPrintf(_T("\n%d*********************"), iFBJIndex) ;

			//删除没用的端子
			//this->DelNoUserEnt(pSaveDb) ;				
		}

		//绘制“接本图”标记、说明文字（note）等

		pt_3d_insert.x = dBaseX ;
		pt_3d_insert.y = -145.4281 ;			
		strBlockName = _T("dmh_n+1_con_0") ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		pt_3d_insert.x = dBaseX ;
		pt_3d_insert.y = -35.4198 ;			
		strBlockName = _T("dmh_n+1d_con_1") ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		pt_3d_insert.x = dBaseX+5 ;
		pt_3d_insert.y = -70 ;			
		strBlockName = _T("dmh_circuit_note_0001_0") ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		//begin 绘制整个电路图的边框和签字栏
		AcGePoint3d pt_3d_min(0,0,0), pt_3d_max(0,0,0) ;
		mysys.GetCurExtents(pt_3d_min, pt_3d_max) ;

		//内边框4个点
		AcGePoint3d pt_3d_frame_in_bottomleft(pt_3d_min.x, -262+5, 0) ;
		AcGePoint3d pt_3d_frame_in_bottomright(pt_3d_max.x+5, -262+5, 0) ;
		AcGePoint3d pt_3d_frame_in_topright(pt_3d_max.x+5, -262+5+287, 0) ;
		AcGePoint3d pt_3d_frame_in_topleft(pt_3d_min.x, -262+5+287, 0) ;
		//外边框4个点
		AcGePoint3d pt_3d_frame_out_bottomleft(pt_3d_min.x-25, -262, 0) ;
		AcGePoint3d pt_3d_frame_out_bottomright(pt_3d_max.x+5+5, -262, 0) ;
		AcGePoint3d pt_3d_frame_out_topright(pt_3d_max.x+5+5, -262+297, 0) ;
		AcGePoint3d pt_3d_frame_out_topleft(pt_3d_min.x-25, -262+297, 0) ;

		//左边frame
		strBlockName = _T("frame_left") ;
		pt_3d_insert.x = pt_3d_frame_out_bottomleft.x ;
		pt_3d_insert.y = pt_3d_frame_out_bottomleft.y ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert))
		{
			pRef->close() ;
			pRef = NULL ;
		}

		AcGePoint3d pt_3d_bottomleft(0,0,0); //查找区域的左下角
		AcGePoint3d pt_3d_topright(0,0,0) ; //查找区域的右上角
		CLongArray iArrayHandle ;
		double dframe_sign_3_hor_width = 180.000 ;
		double dframe_sign_3_hor_height = 34.000 ;								

		//判断右下角是否为空，为空则绘制一个横向的frame于此
		pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
		pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_height+2 ;
		pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_width-2 ;
		pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;

		iArrayHandle.RemoveAll() ;								
		int iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;								
		if (iFindEntSum==0)
		{									
			strBlockName = _T("SIGN_3") ;
		}
		else  //右下角不为空，则判右边是否能垂直区域是否为空，为空则绘制一个竖向frame于此
		{
			pt_3d_topright.x = pt_3d_frame_in_bottomright.x-1 ;
			pt_3d_topright.y = pt_3d_frame_in_bottomright.y+dframe_sign_3_hor_width+2 ;
			pt_3d_bottomleft.x = pt_3d_frame_in_bottomright.x-dframe_sign_3_hor_height-2 ;
			pt_3d_bottomleft.y = pt_3d_frame_in_bottomright.y+1 ;
			iArrayHandle.RemoveAll() ;
			iFindEntSum = mysys.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayHandle) ;
			//其他情况只能往右延展绘制一个竖向的frame
			if (iFindEntSum!=0)
			{
				pt_3d_frame_in_bottomright.x+=dframe_sign_3_hor_height ;
				pt_3d_frame_in_topright.x+=dframe_sign_3_hor_height ;
				pt_3d_frame_out_bottomright.x+=dframe_sign_3_hor_height ;
				pt_3d_frame_out_topright.x+=dframe_sign_3_hor_height ;
			}

			strBlockName = _T("frame_sign_3_ver_new") ;
		}	

		//内边框
		AcGePoint3dArray ptArrayFrame ;
		ptArrayFrame.append(pt_3d_frame_in_bottomleft) ;
		ptArrayFrame.append(pt_3d_frame_in_bottomright) ;
		ptArrayFrame.append(pt_3d_frame_in_topright) ;
		ptArrayFrame.append(pt_3d_frame_in_topleft) ;
		mysys.AddAcDb2dPolyline(ptArrayFrame, 0.5, 0) ;
		//外边框
		ptArrayFrame.setAt(0,pt_3d_frame_out_bottomleft) ;
		ptArrayFrame.setAt(1,pt_3d_frame_out_bottomright) ;
		ptArrayFrame.setAt(2,pt_3d_frame_out_topright) ;
		ptArrayFrame.setAt(3,pt_3d_frame_out_topleft) ;
		mysys.AddAcDb2dPolyline(ptArrayFrame, 0.0, 0) ;	

		//签字栏
		// 			CStringArray strArrayVal ;
		pt_3d_insert = pt_3d_frame_in_bottomright ;

		strArrayVal.RemoveAll() ;
		strArrayVal.Add(m_strStation) ;
		strArrayVal.Add(m_strDwgSn) ;		
		strArrayVal.Add(CBlkUtility::GetDateStr()) ;
		CString strDwgName ;
		strDwgName.Format(_T("N+1电码化电路图")) ;
		strArrayVal.Add(strDwgName) ;
		strArrayVal.Add(m_strProjectName) ;
		if (mysys.InsertBlock(pRef, strBlockName, pt_3d_insert, &strArrayVal, FALSE))
		{
			pRef->close() ;
			pRef = NULL ;
		}
		//end 绘制整个电路图的边框和签字栏	

		//将sn转换为ID
		//this->TransNAdd1SnToId(pSaveDb) ;

		//删除过程sn等xdata
		//this->DelSn(pSaveDb) ;

		mysys.WriteToRec("second_draw", "draw_type", "CIRCUIT") ;
		mysys.SetLayerStatus(_T("hide"), true) ;
		mysys.ModifyFontStyle(_T("Standard"), _T("hztxt.shx"), TRUE) ;

		pSaveDb->saveAs(strSaveFileName) ;

		delete pSaveDb ;
		pSaveDb = NULL ;

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in DrawCircuit")) ;
	} 
}
// for 电化局
void CZnDmh::FillXdataNGL_D(int iGCJIndex, const CString & strGuiDao, const CString &strIsROrF, int iIsSendCode, CMapStringToString & mapStrToStrXdata, CMapStringToString &mapToAddXdata, const CString &strJinLuName)
{
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	//GUIDAO
	mapStrToStrXdata.SetAt(_T("GUIDAO"), strGuiDao) ;

	//读取strGuiDao在ZZH中的相关位置信息
	int iShelf=0, iLayer=0, iBox=0 ;
	CString strRName, strRSide1, strRSide2, strRSide3 ;
	this->GetZzhInfo_D(strGuiDao, iShelf, iLayer, iBox, strRName, strRSide1, strRSide2, strRSide3) ;

	CString strZhLoc ;
	strZhLoc.Format(_T("%s%d-%d"),m_strPreZ, iShelf, iLayer) ;

	mapStrToStrXdata.SetAt(_T("ZH_LOC"), strZhLoc) ;
	mapStrToStrXdata.SetAt(_T("R_NAME"), strRName) ;
	mapStrToStrXdata.SetAt(_T("NODE_A8"), strRSide1) ;
	mapStrToStrXdata.SetAt(_T("NODE_R_SIDE1"), strRSide2) ;
	mapStrToStrXdata.SetAt(_T("NODE_A18"), strRSide3) ;

	//FXP
	if (strIsROrF==_T("R")) //受电端
	{
		CString strCircuitType, strGJFPoint, strGJFSide, strGSide1, strGSide2, strFxpLoc1, strFxpLoc2 ;
		this->GetLsInfoR(strGuiDao, strCircuitType, strGJFPoint, strGJFSide, strGSide1, strGSide2, strFxpLoc1, strFxpLoc2) ;

		if (m_iFxpFrom==1) //程序计算分线盘端子，从m_strFxpStart开始顺
		{
			this->GetFxpDz(strFxpLoc1) ;
			this->GetFxpDz(strFxpLoc2) ;
		}
		mapStrToStrXdata.SetAt(_T("FXP_LOC1"), strFxpLoc1) ;
		mapStrToStrXdata.SetAt(_T("FXP_LOC2"), strFxpLoc2) ;	
	}
	else
	{
		CString strPower1, strPower2, strFxpLoc1, strFxpLoc2 ;
		this->GetLsInfoF(strGuiDao, strPower1, strPower2, strFxpLoc1, strFxpLoc2) ;

		if (m_iFxpFrom==1) //程序计算分线盘端子，从m_strFxpStart开始顺
		{
			this->GetFxpDz(strFxpLoc1) ;
			this->GetFxpDz(strFxpLoc2) ;
		}

		mapStrToStrXdata.SetAt(_T("FXP_LOC1"), strFxpLoc1) ;
		mapStrToStrXdata.SetAt(_T("FXP_LOC2"), strFxpLoc2) ;
	}


	if (iIsSendCode)
	{
		CString strAddXdata ;
		CString strFirstSnPre ;
		if (iGCJIndex<100) //正线
		{
			strFirstSnPre.Format(_T("CD_%dGCJ"), iGCJIndex) ;

			strAddXdata.Format(_T("MY_SN,NGL_NODE_A18_%d;FIRST_SN,%s_52"), iGCJIndex, strFirstSnPre) ;
			mapToAddXdata.SetAt(_T("NODE_A18"), strAddXdata) ;
			strAddXdata.Format(_T("MY_SN,NGL_NODE_A8_%d;FIRST_SN,%s_62"),iGCJIndex, strFirstSnPre) ;
			mapToAddXdata.SetAt(_T("NODE_A8"), strAddXdata) ;
		}
		else
		{
			int iGJFIndex = iGCJIndex%100 ; //传入iGCJIndex参数时:iGCJIndex+100(下行运行方向)/iGCJIndex+200(上行运行方向)
			int iUpOrDown = iGCJIndex/200 ; //0:下行,1:上行

			strAddXdata.Format(_T("MY_SN,%sNGL_NODE_A18_%d;FIRST_SN,%s_GJF2_%d3"), strJinLuName, iGJFIndex, strJinLuName, iUpOrDown==0?4:6) ;
			mapToAddXdata.SetAt(_T("NODE_A18"), strAddXdata) ;
			strAddXdata.Format(_T("MY_SN,%sNGL_NODE_A8_%d;FIRST_SN,%s_GJF2_%d3"), strJinLuName, iGJFIndex, strJinLuName, iUpOrDown==0?3:5) ;
			mapToAddXdata.SetAt(_T("NODE_A8"), strAddXdata) ;
		}
	}
}

// 读取strGuiDao在ZZH表中的信息
int CZnDmh::GetZzhInfo_D(const CString & strGuiDao, int & iShelf, int & iLayer, int & iBox, CString &strRName, CString &strRSide1, CString &strRSide2, CString &strRSide3)
{
	int iRet = -1 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		//ZH_LOC
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in (select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_zdtr')"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			iRet = cSet.GetRecordCount() ;

			if (iRet==1)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelf) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayer) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBox) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_jdqname"), strRName) ;
				strRSide3.Format(_T("03-%d"), iBox) ;
				strRSide1.Format(_T("0%d-%d"), (iBox-1)/8+1, (iBox>8?(iBox-8):iBox)*2-1) ;
				strRSide2.Format(_T("0%d-%d"), (iBox-1)/8+1, (iBox>8?(iBox-8):iBox)*2) ;

			}
			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetZzhInfo")) ;
	} 

	return iRet;
}

void CZnDmh::GetGCJInfo_D(const CString & strGuiDao, CString & strGCJName, int &iShelf, int &iLayer, int &iBox)
{
	try
	{
		CADORecordset cSet ;
		CString strSql ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in (select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_gc')"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRet = cSet.GetRecordCount() ;

			if (iRet==1)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelf) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayer) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBox) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_jdqname"), strGCJName) ;
			}
			else
			{
				CString strMsg ;
				strMsg.Format(_T("%s在组合架中GC组合中出现不是1次"), strGuiDao) ;
				AfxMessageBox(strMsg) ;
			}
			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetGCJInfo_D")) ;
	} 
}

//iZLocIndex：默认一张图的GC组合最多2行，开始iZLocIndex=1，若跨行了就为2
void CZnDmh::FillXdataGCJ_D(int iGuDaoIndex, int nGCJIndex, int nGCJJdqIndex, int iZLocIndex, const CString &strGCJName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iJinLuType)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	strAddXdata.Format(_T("%d"), nGCJIndex) ;
	mapToReplaceXdata.SetAt(_T("GCJINDEX"), strAddXdata) ;
	mapStrToStrXdata.SetAt(_T("GCJNAME"), strGCJName) ;

	CString strConStr, strParSn ;

	strParSn.Format(_T("PAR_SN,CD-%d_ZLOC"), iZLocIndex) ;

	strKey.Format(_T("CD_%dGCJ_52"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,NGL_NODE_A18_%d;FIRST_CON_SN,CON_%dGCJ_52;%s"), strKey, nGCJIndex,nGCJIndex, strParSn) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;
	strKey.Format(_T("CD_%dGCJ_62"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,%s;FIRST_SN,NGL_NODE_A8_%d;FIRST_CON_SN,CON_%dGCJ_62;%s"), strKey, nGCJIndex, nGCJIndex, strParSn) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;

	//连接电缆类型，增加MY_SN
	int iNodes[4] = {61, 62, 51, 52} ;
	for (int i=0; i<4; i++)
	{
		strKey.Format(_T("CON_%dGCJ_%d"), nGCJIndex, iNodes[i]) ;
		strAddXdata.Format(_T("MY_SN,%s"), strKey) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
	}

	//CD_nGCJ_61
	strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_61"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,CD_%dGCJ_63"), nGCJIndex, nGCJIndex+2) ;
	if (iGuDaoIndex==0)
	{		
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,ZBPU_NODE_U3"), nGCJIndex) ;
		}
		else
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,%s_FMJ_%d%d"), nGCJIndex, iJinLuType==2?_T("ZF"):_T("FJ"), iJinLuType==2?4:6, iJinLuType) ;
		}		
	}
	else if (iGuDaoIndex==1)
	{
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,ZJ_JMJ_42"), nGCJIndex) ;
		}		
		else if (iJinLuType==2)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,ZF_FMJ_62"), nGCJIndex) ;
		}
		else if (iJinLuType==3)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,FJ_JMJ_42"), nGCJIndex) ;
		}
	}
	else if (iGuDaoIndex==2)
	{
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,ZJ_JMJ_62"), nGCJIndex) ;
		}
		else if (iJinLuType==3)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_61;FIRST_SN,FJ_JMJ_62"), nGCJIndex) ;
		}
	}
	strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
	strKey.Format(_T("CD_%dGCJ_61"), nGCJIndex) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;

	//CD_nGCJ_51
	strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_51"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,CD_%dGCJ_53"), nGCJIndex, nGCJIndex+2) ;
	if (iGuDaoIndex==0)
	{		
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,ZBPU_NODE_U5"), nGCJIndex) ;
		}
		else
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,%s_FMJ_%d%d"), nGCJIndex, iJinLuType==2?_T("ZF"):_T("FJ"),   iJinLuType==2?3:5, iJinLuType) ;
		}		
	}
	else if (iGuDaoIndex==1)
	{
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,ZJ_JMJ_32"), nGCJIndex) ;
		}		
		else if (iJinLuType==2)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,ZF_FMJ_52"), nGCJIndex) ;
		}
		else if (iJinLuType==3)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,FJ_JMJ_32"), nGCJIndex) ;
		}
	}
	else if (iGuDaoIndex==2)
	{
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,ZJ_JMJ_52"), nGCJIndex) ;
		}
		else if (iJinLuType==3)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_51;FIRST_SN,FJ_JMJ_52"), nGCJIndex) ;
		}
	}
	strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
	strKey.Format(_T("CD_%dGCJ_51"), nGCJIndex) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;

	//CD_nGCJ_63
	strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_63"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,CD_%dGCJ_63;FIRST_SN,CD_%dGCJ_61"), nGCJIndex, nGCJIndex-2) ;
	if (iGuDaoIndex==0)
	{
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_63;FIRST_SN,ZJ_JMJ_61"), nGCJIndex) ;
		}
		else if (iJinLuType==3)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_63;FIRST_SN,FJ_JMJ_61"), nGCJIndex) ;
		}
	}
	strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
	strKey.Format(_T("CD_%dGCJ_63"), nGCJIndex) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;

	//CD_nGCJ_53
	strConStr.Format(_T("FIRST_CON_SN,CON_%dGCJ_53"), nGCJIndex) ;
	strAddXdata.Format(_T("MY_SN,CD_%dGCJ_53;FIRST_SN,CD_%dGCJ_51"), nGCJIndex, nGCJIndex-2) ;
	if (iGuDaoIndex==0)
	{
		if (iJinLuType==0)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_53;FIRST_SN,ZJ_JMJ_51"), nGCJIndex) ;
		}
		else if (iJinLuType==3)
		{
			strAddXdata.Format(_T("MY_SN,CD_%dGCJ_53;FIRST_SN,FJ_JMJ_51"), nGCJIndex) ;
		}
	}
	strAddXdata.AppendFormat(_T(";%s;%s"), strConStr, strParSn) ;
	strKey.Format(_T("CD_%dGCJ_53"), nGCJIndex) ;
	mapToAddXdata.SetAt(strKey, strAddXdata) ;	

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='GC' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0 "), nGCJJdqIndex) ;

		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;

					strKey.Format(_T("CD_%dGCJ_%d"), nGCJIndex, iJdqNode) ;

					mapStrToStrXdata.SetAt(strKey, strCon) ;

					cSet.MoveNext() ;
				}
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataGCJ_D")) ;
	}
}

void CZnDmh::FillXdataGCJZLoc_D(int iJinLuType, const CString & strLayerBlk, const CString & strZhShowName,const CString &strZLoc, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata, int iMode)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("CD_ZNAME"), strZhShowName) ;

	CString strKey ;
	CString strAddXdata ;

	CString strSql ;

	strKey = _T("GCJ_ZLOC") ;
	if (iJinLuType==0)
	{
		strAddXdata.Format(_T("MY_SN,CD-%d_ZLOC"), strLayerBlk==_T("dmh_box_z_1cda_2")?2:1) ;
	}
	else if (iJinLuType==1)
	{
		strAddXdata.Format(_T("MY_SN,%s_ZLOC"), strZhShowName) ;
	}
	else if (iJinLuType==2)
	{
		strAddXdata.Format(_T("MY_SN,CD-%d_ZLOC"), strLayerBlk==_T("dmh_box_z_1cda_2")?2:1) ;
	}
	else if (iJinLuType==3)
	{		
		strAddXdata.Format(_T("MY_SN,%s_ZLOC"), strZhShowName) ;
	}
	if (iMode==1)
	{
		strAddXdata.Format(_T("MY_SN,ZJ_FENGE_GUDAO_ZLOC")) ;
	}
	mapToAddXdata.SetAt(strKey, strAddXdata) ;	

	mapStrToStrXdata.SetAt(_T("GCJ_ZLOC"), strZLoc) ;

}

void CZnDmh::FillXdataJMJ_0000_D(int iJinLuType, const CString &strJZ,const CString &strFJZ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey, strAddXdata ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	strAddXdata.Format(_T("BM(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("BM_NAME"), strAddXdata) ;

	if (iJinLuType==0)
	{
		strKey.Format(_T("ZJ_JMJ_31")) ;
		strAddXdata.Format(_T("FIRST_SN,ZBPU_NODE_D5;FIRST_CON_SN,CON_ZJ_JMJ_31")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
		strKey.Format(_T("ZJ_JMJ_41")) ;
		strAddXdata.Format(_T("FIRST_SN,ZBPU_NODE_D3;FIRST_CON_SN,CON_ZJ_JMJ_41")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
	}
	else if (iJinLuType==3)
	{
		strKey.Format(_T("FJ_JMJ_31")) ;
		strAddXdata.Format(_T("FIRST_SN,FJ_FMJ_33;FIRST_CON_SN,CON_FJ_JMJ_31")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
		strKey.Format(_T("FJ_JMJ_41")) ;
		strAddXdata.Format(_T("FIRST_SN,FJ_FMJ_43;FIRST_CON_SN,CON_FJ_JMJ_31")) ;
		mapToAddXdata.SetAt(strKey, strAddXdata) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_bm') and dmh_in_zcell_jdqname='JMJ' and dmh_in_zcell_cellcontent='%s'"), iJinLuType==0?strJZ:strFJZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				
				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				strKey.Format(_T("BM_ZLOC")) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("FillZJCFJC_0000下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataZJCFJC_0000")) ;
	}	
}

// 填充ZBPU_FBJ_FS块的扩展数据
void CZnDmh::FillXdataZBPU_FBJ_FS(int iJinLuType, const CString &strJZ, const CString &strFCZ, const CString &strCZ, const CString &strFJZ, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strJinLuName) ;

	strAddXdata.Format(_T("FB(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FB_NAME"), strAddXdata) ;

	strAddXdata.Format(_T("BM(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("BM_NAME"), strAddXdata) ;

	mapToReplaceXdata.SetAt(_T("FBJ_JINLUNAME"), strJinLuName) ;

	//CString strCurs[4] = {_T("ZPBU_NODE_U3"), _T("ZPBU_NODE_U5"), _T("ZPBU_NODE_D3"), _T("ZPBU_NODE_D5")} ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//ZBPU
		strSql.Format(_T("select * from dmh_in_zcell where (dmh_in_zcell_cellcontent like '%%%s' or dmh_in_zcell_cellcontent like '%s%%') and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_zbpu-2b') "), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				CString strZBPULoc ;
				strZBPULoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("ZBPU_ZLOC"), strZBPULoc) ;

				int iZBPU_RIndex = 0 ;
				while (!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
					//ZBPU_RINDEX1~2
					strAddXdata.Format(_T("R%d"), iBoxNum) ;
					strKey.Format(_T("ZBPU_RINDEX%d"), iZBPU_RIndex+1) ;
					mapStrToStrXdata.SetAt(strKey, strAddXdata) ;
					//ZBPU_NODE_U1~5 D1~5
					for (int iSide=0; iSide<4; iSide++)
					{
						strKey.Format(_T("ZBPU_NODE_%s%d"), iBoxNum%2==0?_T("D"):_T("U"), iSide==3?5:(iSide+1)) ;
						strAddXdata.Format(_T("0%d-%d"), iBoxNum%2==0?2:1, (iBoxNum-1)/2*4+iSide+1) ;
						mapStrToStrXdata.SetAt(strKey, strAddXdata) ;
					}
					iZBPU_RIndex++ ;
					cSet.MoveNext() ;
				}
			}
		}

		//FBJ FBJF
		strSql.Format(_T("select * from dmh_in_zcell where (dmh_in_zcell_cellcontent like '%%%s' or dmh_in_zcell_cellcontent like '%s%%') and dmh_in_zcell_jdqname='FBJ'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strFBJLoc ;
				strFBJLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("FBJ_ZLOC"), strFBJLoc) ;
				mapStrToStrXdata.SetAt(_T("FB_ZLOC"), strFBJLoc) ; //FBJ_ZLOC FB_ZLOC 回头同一修改成FB_ZLOC
				mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strCellContent) ;

				CString strJdqIndex_FBJ ;
				strJdqIndex_FBJ.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX_FBJ"), strJdqIndex_FBJ) ;

				//FBJ
				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='FB' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("FB_%dFBJ_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}

				CString strJdqIndex_FBJF ;
				strJdqIndex_FBJF.Format(_T("%d"), iBoxNum+1) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX_FBJF"), strJdqIndex_FBJF) ;

				//FBJF
				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='FB' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum+1) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("FB_%dFBJF_%d"), iBoxNum+1, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("FillXdataFBJ下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}

		//FS
		strSql.Format(_T("select * from dmh_in_zycell where dmh_in_zycell_jinluname like '%%%s' or dmh_in_zycell_jinluname like '%s%%'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShlefNum = 0, iBoxNum = 0 ;
				CString strJinLuNameAll, strFreq1, strFreq2 ;
				cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_freq1"), strFreq1) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_freq2"), strFreq2) ;

				strAddXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("ZY_LOC"), strAddXdata) ;

				strAddXdata.Format(_T("%dFS"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FS_INDEX"), strAddXdata) ;

				strAddXdata = strFreq1 ;
				if (!strFreq2.IsEmpty())
				{
					strAddXdata.AppendFormat(_T("/%s"), strFreq2) ;
				}
				mapStrToStrXdata.SetAt(_T("FREQ"), strAddXdata) ;

				strAddXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("DNUM"), strAddXdata) ;

				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
				mapStrToStrXdata.SetAt(_T("POWER_DNUM_KZ"), strAddXdata) ;
				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
				mapStrToStrXdata.SetAt(_T("POWER_DNUM_KF"), strAddXdata) ;
				strAddXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("POWER_RDNUM"), strAddXdata) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataZBPU_FS下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}
			cSet.Close() ;
		}
		//+1FS
		strSql.Format(_T("select * from dmh_in_zycell where dmh_in_zycell_jinluname like '%%+1%%'")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShlefNum = 0, iBoxNum = 0 ;
				CString strJinLuNameAll ;
				cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;

				strAddXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_ZYLOC"), strAddXdata) ;

				strAddXdata.Format(_T("%dFS"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_INDEX"), strAddXdata) ;

				strAddXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_DNUM"), strAddXdata) ;

				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_POWER_DNUM_KZ"), strAddXdata) ;
				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_POWER_DNUM_KF"), strAddXdata) ;
				strAddXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_POWER_RDNUM"), strAddXdata) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataZBPU_FS下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}
			cSet.Close() ;
		}

		//BM
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_bm') "), strJZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				CString strBMLoc ;
				strBMLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("BM_ZLOC"), strBMLoc) ;
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
		AfxMessageBox(_T("unknown error in FillXdataZBPU_FBJ")) ;
	}
}

// 填充ZBPU_FBJ_FS块的扩展数据
void CZnDmh::FillXdataZBPU_FBJ_FS_CeXian(int iJinLuType, const CString &strCZ, const CString &strJinLuName, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	CString strZhName = _T("GD") ;
	if(iJinLuType==5)
	{
		strZhName = _T("GD1") ;
	}
	else if (iJinLuType==7)
	{
		strZhName  = _T("ZC") ;
	}

	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;

	int iUpOrDown = 0 ;
	if (strCZ.Left(1)==_T("S"))
	{
		iUpOrDown = 1 ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//ZBPU
		strSql.Format(_T("select * from dmh_in_zcell where (dmh_in_zcell_cellcontent like '%%%s' or dmh_in_zcell_cellcontent like '%s%%') and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_zbpu-2b') "), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				CString strZBPULoc ;
				strZBPULoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("ZBPU_ZLOC"), strZBPULoc) ;

				int iZBPU_RIndex = 0 ;
				while (!cSet.IsEOF())
				{
					cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
					//ZBPU_RINDEX1~2
					strAddXdata.Format(_T("R%d"), iBoxNum) ;
					strKey.Format(_T("ZBPU_RINDEX%d"), iZBPU_RIndex+1) ;
					mapStrToStrXdata.SetAt(strKey, strAddXdata) ;
					//ZBPU_NODE_U1~5 D1~5
					for (int iSide=0; iSide<4; iSide++)
					{
						strKey.Format(_T("ZBPU_NODE_U%d_%d"), iSide==3?5:(iSide+1), iUpOrDown) ;
						strAddXdata.Format(_T("0%d-%d"), iBoxNum%2==0?2:1, (iBoxNum-1)/2*4+iSide+1) ;
						mapStrToStrXdata.SetAt(strKey, strAddXdata) ;
					}
					iZBPU_RIndex++ ;
					cSet.MoveNext() ;
				}
			}
		}

		//LXJF FBJ
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_jdqname='LXJF'"), strCZ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				strKey.Format(_T("%s_ZLOC"), strZhName) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;				
				
				CString strJdqIndex ;
				strJdqIndex.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX_LXJF"), strJdqIndex) ;
				
				//LXJF
				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("%s_%dLXJF_%d"), strZhName, iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}

				strJdqIndex.Format(_T("%d"), iBoxNum+1) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX_FBJ"), strJdqIndex) ;

				//FBJ
				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iBoxNum+1) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("%s_%dFBJ_%d"), strZhName, iBoxNum+1, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}

				if (iJinLuType<7)
				{
					int iJdqIndex_GJF2 = iBoxNum>5?10:5 ;
					if (iJinLuType==5)
					{
						iJdqIndex_GJF2 = iBoxNum+2 ;
					}
					strJdqIndex.Format(_T("%d"), iJdqIndex_GJF2) ;
					mapToReplaceXdata.SetAt(_T("JDQINDEX_GJF2"), strJdqIndex) ;

					//GUDAO
					strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_shelfnum=%d and dmh_in_zcell_layernum=%d and dmh_in_zcell_boxnum=%d"), iShelfNum, iLayerNum,iJdqIndex_GJF2) ;
					if(CDBUtility::GetRecordValue(strSql, _T("dmh_in_zcell_cellcontent"), strAddXdata)>0)
					{
						mapStrToStrXdata.SetAt(_T("GUDAO"), strAddXdata) ;
					}

					//GJF2
					strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iJdqIndex_GJF2) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					if (cSet.Open(g_PtrCon, strSql))
					{
						if (cSet.GetRecordCount()>0)
						{
							cSet.MoveFirst() ;

							while (!cSet.IsEOF())
							{
								int iJdqNode = 0 ;
								CString strCon ;
								cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
								cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

								CString strTypeName ;
								strTypeName.Format(_T("%s_%dGJF2_%d"), strZhName, iJdqIndex_GJF2, iJdqNode) ;
								mapStrToStrXdata.SetAt(strTypeName, strCon) ;

								cSet.MoveNext() ;
							}
						}
						cSet.Close() ;
					}	
				}

			}
			else
			{
				strErrMsg.Format(_T("FillXdataZBPU_CEXIAN下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}

		//FS
		strSql.Format(_T("select * from dmh_in_zycell where dmh_in_zycell_jinluname like '%%%s' or dmh_in_zycell_jinluname like '%s%%'"), strJinLuName, strJinLuName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShlefNum = 0, iBoxNum = 0 ;
				CString strJinLuNameAll, strFreq1, strFreq2 ;
				cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_freq1"), strFreq1) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_freq2"), strFreq2) ;

				strAddXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("ZY_LOC"), strAddXdata) ;

				strAddXdata.Format(_T("%dFS"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FS_INDEX"), strAddXdata) ;

				strAddXdata = strFreq1 ;
				if (!strFreq2.IsEmpty())
				{
					strAddXdata.AppendFormat(_T("/%s"), strFreq2) ;
				}
				mapStrToStrXdata.SetAt(_T("FREQ"), strAddXdata) ;

				strAddXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("DNUM"), strAddXdata) ;

				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
				mapStrToStrXdata.SetAt(_T("POWER_DNUM_KZ"), strAddXdata) ;
				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
				mapStrToStrXdata.SetAt(_T("POWER_DNUM_KF"), strAddXdata) ;
				strAddXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("POWER_RDNUM"), strAddXdata) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataZBPU_FS_CEXIAN下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}
			cSet.Close() ;
		}
		//+1FS
		strSql.Format(_T("select * from dmh_in_zycell where dmh_in_zycell_jinluname like '%%+1%%'")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShlefNum = 0, iBoxNum = 0 ;
				CString strJinLuNameAll ;
				cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;

				strAddXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_ZYLOC"), strAddXdata) ;

				strAddXdata.Format(_T("%dFS"), iBoxNum) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_INDEX"), strAddXdata) ;

				strAddXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_DNUM"), strAddXdata) ;

				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_POWER_DNUM_KZ"), strAddXdata) ;
				strAddXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_POWER_DNUM_KF"), strAddXdata) ;
				strAddXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
				mapStrToStrXdata.SetAt(_T("FSPLUS_POWER_RDNUM"), strAddXdata) ;
			}
			else
			{
				strErrMsg.Format(_T("FillXdataZBPU_FS_CEXIAN下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}
			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataZBPU_FBJ_CEXIAN")) ;
	}
}


void CZnDmh::FillXdataLiCi_GJF1_FuSi_D(int iLiCiGJF1GuiDaoIndex, const CString & strGuiDao, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata , int iJinLuType)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	CString strGuiDaoTmp = strGuiDao ;
	strGuiDaoTmp.TrimLeft(_T('*')) ;
	strGuiDaoTmp.TrimRight(_T('+')) ;

	mapStrToStrXdata.SetAt(_T("GUIDAO"), strGuiDaoTmp) ;
	strAddXdata = (iJinLuType==7?_T("JF2") :_T("JF1"));
	mapStrToStrXdata.SetAt(_T("GJFNAME"), strAddXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where %s='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_%s') "), iJinLuType==7?_T("dmh_in_zcell_cellcontent_trim"):_T("dmh_in_zcell_cellcontent"),strGuiDao, iJinLuType==7?_T("zc"):_T("gjf")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				CString strLoc ;
				strLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GJF_ZLOC"), strLoc) ;
				strAddXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX"), strAddXdata) ;

				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iJinLuType==7?_T("ZC"):_T("GJF"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;
						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
							strKey.Format(_T("GJF_%dGJF1_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;
							if (iJdqNode==11)
							{
								strAddXdata.Format(_T("MY_SN,GJF_%dGUIDAO-GJF1_11"),iLiCiGJF1GuiDaoIndex) ;
								if (iLiCiGJF1GuiDaoIndex==0)
								{
									strAddXdata.AppendFormat(_T(";FIRST_SN,BM_%s_12"), iJinLuType==0?_T("ZJMJ"):_T("FMJ")) ;
								}
								else
								{
									strAddXdata.AppendFormat(_T(";FIRST_SN,GJF_%dGUIDAO-GJF1_11"),iLiCiGJF1GuiDaoIndex-1) ;
								}
                                
								mapToAddXdata.SetAt(strKey, strAddXdata) ;
							}

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataLiCi_GJF1_D")) ;
	}

}


void CZnDmh::FillXdataGCJGJF_GJF_D(int iJinLuType, int iGuiDaoIndex, int iMaxGCJ, const CString &strGuiDao, const CString &strJZ,  CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata )
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	//GJF_NAME(组合name)
	strAddXdata.Format(_T("GJF(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("GJF_NAME"), strAddXdata) ;

	//GJFNAME(继电器name)
	CString strGuiDaoTmp = strGuiDao ;
	strGuiDaoTmp.TrimLeft(_T('*')) ;
	strGuiDaoTmp.TrimRight(_T('+')) ;
	strAddXdata.Format(_T("%sJF1"), strGuiDaoTmp) ;
	mapStrToStrXdata.SetAt(_T("GJFNAME"), strAddXdata) ;

	//JDQ_JD_GROUP
	int iJDQJDGROUP = 2 ;
	if (iJinLuType%2==0)
	{
		iJDQJDGROUP = 2 ;
	}
	else if (iJinLuType==3)
	{
		iJDQJDGROUP = 3 ;
		if (iGuiDaoIndex==0)
		{
			iJDQJDGROUP = 5 ;
		}
	}
	strAddXdata.Format(_T("%d"), iJDQJDGROUP) ;
	mapStrToStrXdata.SetAt(_T("JDQ_JD_GROUP"), strAddXdata) ;
	//replace JDQJDGROUP
	mapToReplaceXdata.SetAt(_T("JDQJDGROUP"), strAddXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent_trim='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_gjf')"), strGuiDaoTmp) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;				
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				CString strLoc ;
				strLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GJF_ZLOC"), strLoc) ;
				strAddXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX"), strAddXdata) ;

				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='GJF' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;	
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;

							strKey.Format(_T("GJF_%dGJF1_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;
							if (iJdqNode/10==iJDQJDGROUP)
							{
								strAddXdata = _T("") ;
								if (iJdqNode%10==1)
								{
									strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,GJF_%dGJF1_%d2"), iGuiDaoIndex, iJdqNode, iGuiDaoIndex-1, iJDQJDGROUP) ;
									if (iGuiDaoIndex==0)
									{
										if (iJinLuType==2)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,BM_2FMJ_22"), iGuiDaoIndex, iJdqNode ) ;
										}
										else
										{
											strAddXdata = _T("") ;		
										}
									}
									else if (iGuiDaoIndex==1)
									{
										if (iJinLuType==0)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,BM_1JMJ_22"), iGuiDaoIndex, iJdqNode ) ;
										}
										else if (iJinLuType==3)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,BM_4JMJ_22"), iGuiDaoIndex, iJdqNode ) ;
										}
									}
								}
								else if (iJdqNode%10==2)
								{
									strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,GJF_%dGJF1_%d1"), iGuiDaoIndex, iJdqNode, iGuiDaoIndex+1, iJDQJDGROUP) ;
									if (iGuiDaoIndex==0)
									{
										if (iJinLuType==0)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,BM_1JMJ_21"), iGuiDaoIndex, iJdqNode ) ;		
										}
										else if (iJinLuType==3)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,BM_4JMJ_21"), iGuiDaoIndex, iJdqNode ) ;
										}
									}
									else if (iGuiDaoIndex==(iMaxGCJ-1)) 
									{
										if (iJinLuType==0||iJinLuType==3)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,ZBJH_JGJ-LQJ_21"), iGuiDaoIndex, iJdqNode ) ;
										}
										else if (iJinLuType==2)
										{
											strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,ZF_GJF1_NODE_X1"), iGuiDaoIndex, iJdqNode ) ;
										}									
									}	
								}
								else if (iJdqNode%10==3)
								{
									strAddXdata.Format(_T("MY_SN,GJF_%dGJF1_%d;FIRST_SN,GC_%dGCJ_3"), iGuiDaoIndex, iJdqNode, iGuiDaoIndex ) ;
								}

								if (strAddXdata!=_T(""))
								{
									mapToAddXdata.SetAt(strKey, strAddXdata) ;
								}								
							}

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("%s执行结果不为1"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_GJF_D")) ;
	}

}

void CZnDmh::FillXdataGCJGJF_GCJ_D(int iJinLuType, int iGuiDaoIndex, int iMaxGCJ, const CString &strGuiDao, const CString &strJZ,  CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_gc')"), strGuiDao) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;	
				CString strGCJName ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_jdqname"), strGCJName) ;
				mapStrToStrXdata.SetAt(_T("GCJNAME"), strGCJName) ;
				CString strLoc ;
				strLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GCJ_ZLOC"), strLoc) ;
				
				strAddXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX"), strAddXdata) ;

				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='GC' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;	
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;

							strKey.Format(_T("CD_%dGCJ_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;

							if (iJdqNode==3||iJdqNode==1)
							{
								strAddXdata.Format(_T("MY_SN,GC_%dGCJ_%d"), iGuiDaoIndex, iJdqNode) ;
								if (iJdqNode==1)
								{
									if (iGuiDaoIndex==(iMaxGCJ-1))
									{
										if (iJinLuType==0||iJinLuType==3)
										{
											strAddXdata.AppendFormat(_T(";FIRST_SN,ZBJH_JGJ-LQJ_23")) ;
										}
										else if (iJinLuType==2)
										{
											strAddXdata.AppendFormat(_T(";FIRST_SN,ZF_GJF1_NODE_X3")) ;
										}									
									}
									else
									{
										strAddXdata.AppendFormat(_T(";FIRST_SN,GC_%dGCJ_3"), iGuiDaoIndex+1) ;
									}
								}
								mapToAddXdata.SetAt(strKey, strAddXdata) ;
							}

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("%s执行结果不为1"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_GCJ_D")) ;
	}   

}

void CZnDmh::FillXdataGCJGJF_JFMJ_D(int iJinLuType, const CString &strJZ, const CString &strFJZ, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strAddXdata ;

	strAddXdata.Format(_T("BM(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("BM_NAME"), strAddXdata) ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), iJinLuType==3?strFJZ:strJZ) ;
	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_jdqname='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_bm')"),  iJinLuType==3?strFJZ:strJZ, iJinLuType==2?_T("FMJ"):_T("JMJ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;
				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;	
				
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;	
				
				CString strLoc ;
				strLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("BM_ZLOC"), strLoc) ;

				strAddXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX"), strAddXdata) ;

				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='BM' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;	
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;

							strKey.Format(_T("BM_%d%s_%d"), iBoxNum, iJinLuType==2?_T("FMJ"):_T("JMJ"), iJdqNode) ;
							mapStrToStrXdata.SetAt(strKey, strCon) ;

							cSet.MoveNext() ;
						}
					}
					cSet.Close() ;
				}
			}
			else
			{
				strErrMsg.Format(_T("%s执行结果不为1"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_JFMJ_D")) ;
	} 
}

// 填充zbjh-3jgj块的扩展数据
void CZnDmh::FillXdataZBJH_JGJ_1LQ_D(int iJinLuType,  const CString &strJZXHJ, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZXHJ) ;

	strXdata.Format(_T("FB(%s)"), strJZXHJ) ;
	mapStrToStrXdata.SetAt(_T("FB_NAME"), strXdata) ;

	if (iJinLuType==0) //正接
	{
		mapStrToStrXdata.SetAt(_T("JGJ_LQJ"), _T("3JGJ")) ;
	}
	else if (iJinLuType==3) //反接
	{
		mapStrToStrXdata.SetAt(_T("JGJ_LQJ"), _T("1LQJ")) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//3JGJ 1LQJ的21 23端子
		strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='FB' and dmh_def_zhnb_jdqname like '%%%s%%' and dmh_def_zhnb_contype=0"), iJinLuType==0?_T("3JGJ"):_T("1LQJ")) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()>0)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{
					int iJdqNode = 0 ;
					CString strCon ;	
					cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
					cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;
					strKey.Format(_T("ZBJH_JGJ-LQJ_%d"), iJdqNode) ;
					mapStrToStrXdata.SetAt(strKey, strCon) ;

					strXdata.Format(_T("MY_SN,%s"), strKey) ;
					mapToAddXdata.SetAt(strKey, strXdata) ;

					cSet.MoveNext() ;
				}
			}
			cSet.Close() ;
		}

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='3JGJ' and dmh_in_zcell_cellcontent='%s'"), strJZXHJ) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				while (!cSet.IsEOF())
				{					
					int iShelfNum = 0, iLayerNum = 0 ;	

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;

					mapStrToStrXdata.SetAt(_T("ZBJH_ZLOC"), strZLoc) ;

					cSet.MoveNext() ;
				}
			}
			else if(iRsCount>1)
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}


	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataZBJH_JGJ_1LQ_D")) ;
	}
}

// 链接GCJ-GJF左端，类似ZBJH块
void CZnDmh::FillXdataGCJGJF_GDGJF1_D(const CString &strJZ,const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strGuiDaoShow = strGuDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuiDaoShow) ;

	strXdata.Format(_T("GJF(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("GJF_NAME"), strXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent_trim='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_gjf')"), strGuiDaoShow) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0 ;

				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				mapStrToStrXdata.SetAt(_T("GJF_ZLOC"), strZLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("%s执行结果不为1"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataGCJGJF_GDGJF1_D")) ;
	}
}

void CZnDmh::FillXdataLiCi_D(int iJinLuType, const CString &strJZ, const CString &strFCZ, const CString &strCZ, const CString &strFJZ,const CString &strGuDao, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strGuiDaoShow = strGuDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUDAO"), strGuiDaoShow) ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	strXdata.Format(_T("JZ(%s)"), iJinLuType==3?strFJZ:strJZ) ;
	mapStrToStrXdata.SetAt(_T("JZ_NAME"), strXdata) ;

	strXdata.Format(_T("GJF(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("GJF_NAME"), strXdata) ;
	strXdata.Format(_T("BM(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("BM_NAME"), strXdata) ;
	strXdata.Format(_T("FB(%s)"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FB_NAME"), strXdata) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		CString strZLocs[3] = {_T("GJF_ZLOC"), _T("BM_ZLOC"), _T("FB_ZLOC")}; 
		CString strSqls[3] = {_T(""), _T(""), _T("")} ;
		strSqls[0].Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent_trim='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_gjf')"), strGuiDaoShow) ;
		strSqls[1].Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_jdqname='JMJ' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_bm')"), strJZ) ;
		strSqls[2].Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_jdqname like '%%1LQJ%%' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_fb')"), strJZ) ;
		for (int i=0; i<3 ;i++)
		{
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSqls[i]))
			{
				if (cSet.GetRecordCount()==1)
				{
					cSet.MoveFirst() ;

					int iShelfNum = 0, iLayerNum = 0 ;

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					mapStrToStrXdata.SetAt(strZLocs[i], strZLoc) ;
				}
				else
				{
					strErrMsg.Format(_T("%s执行结果不为1"), strSqls[i]) ;
					AfxMessageBox(strErrMsg) ;
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
		AfxMessageBox(_T("unknown error in FillXdataLiCi_D")) ;
	}
}

void CZnDmh::FillXdataFMJ_D(const CString &strXhj, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	CString strErrMsg ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey, strAddXdata ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strXhj) ;

	strAddXdata.Format(_T("BM(%s)"), strXhj) ;
	mapStrToStrXdata.SetAt(_T("BM_NAME"), strAddXdata) ;	
	
	for (int i=0; i<4; i++)
	{
		CString strMySn, strFirstSn ;
		strMySn.Format(_T("ZF_FMJ_%d1"), 3+i) ;
		strFirstSn.Format(_T("FIRST_SN,ZBPU_NODE_%s%d"), i<2?_T("D"):_T("U"), i%2==0?5:3) ;
		mapToAddXdata.SetAt(strMySn, strFirstSn) ;
	}

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_bm') and dmh_in_zcell_jdqname='FMJ' and dmh_in_zcell_cellcontent='%s'"), strXhj) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0 ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				strKey.Format(_T("BM_ZLOC")) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;
			}
			else
			{
				strErrMsg.Format(_T("FillZJCFJC_0000下面语句执行结果不为1:\n%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataFMJ_D")) ;
	}	
}

void CZnDmh::GetCeXianGDInfo_D(int iJinLuType, const CString & strGuiDao, int &iShelf, int &iLayer, int &iBox)
{
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		CString strGuiDaoTmp = strGuiDao ;
		strGuiDaoTmp.TrimRight(_T("+")) ;

		CString strBlkName ;
		switch (iJinLuType)
		{
		case 4:
			{
				strBlkName = _T("dmh_boxd_z_gd") ;
			}
			break;
		case 5:
			{
				strBlkName = _T("dmh_boxd_z_gd1") ;
			}
			break;
// 		case 6:
// 			{
// 				strBlkName = _T("dmh_boxd_z_gd") ;
// 			}
// 			break;
		}


		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in (select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='%s')"), strGuiDaoTmp, strBlkName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRet = cSet.GetRecordCount() ;

			if (iRet==1)
			{
				cSet.MoveFirst() ;

				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelf) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayer) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBox) ;				
			}
			else
			{
				CString strMsg ;
				strMsg.Format(_T("%s在组合架中GD组合中出现不是1次"), strGuiDao) ;
				AfxMessageBox(strMsg) ;
			}
			cSet.Close() ;
		}

	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in GetCeXianGDInfo_D")) ;
	} 
}

// 填充侧线GJF2块的扩展数据
void CZnDmh::FillXdataGJF2_CeXian(int iJinLuType, const CString &strJinLuName, const CString &strGuiDao, int iGCJIndex, CMapStringToString & mapToReplaceXdata, CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata)
{
	mapToReplaceXdata.RemoveAll() ;
	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;

	CString strKey ;
	CString strXdata ;

	//int iGJFIndex = iGCJIndex%100 ; //传入iGCJIndex参数时:iGCJIndex+100(下行运行方向)/iGCJIndex+200(上行运行方向)
	int iUpOrDown = iGCJIndex/200 ; //0:下行,1:上行

	CString strGuiDaoShow = strGuiDao ;
	strGuiDaoShow.TrimLeft(_T("*")) ;
	strGuiDaoShow.TrimRight(_T("+")) ;
	mapStrToStrXdata.SetAt(_T("GUIDAO"), strGuiDaoShow) ;

	mapToReplaceXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;

	CString strZhName = _T("GD") ;
	if (iJinLuType==5)
	{
		strZhName = _T("GD1") ;
	}
	else if (iJinLuType==7)
	{
		strZhName = _T("ZC") ;
	}

	mapStrToStrXdata.SetAt(_T("ZH_NAME"), strZhName) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;
		CString strGuiDaoTmp = strGuiDao ;
		strGuiDaoTmp.TrimRight(_T("+")) ;

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent_trim='%s' and dmh_in_zcell_jdqname='GJF2' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_%s')"), strGuiDaoShow, strZhName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			int iRsCount = cSet.GetRecordCount() ;
			if (iRsCount==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				strKey.Format(_T("GD_ZLOC")) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;

				strXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX"), strXdata) ;

				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iBoxNum) ;

				CADORecordset cSetJdqNode ;
				if (cSetJdqNode.IsOpen())
				{
					cSetJdqNode.Close() ;
				}
				if (cSetJdqNode.Open(g_PtrCon, strSql))
				{
					if (cSetJdqNode.GetRecordCount()>0)
					{
						cSetJdqNode.MoveFirst() ;

						while (!cSetJdqNode.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSetJdqNode.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("GD_%dGJF2_%d"), iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							strXdata = _T("") ;
							if (iJdqNode==51||iJdqNode==31)
							{
								strXdata.Format(_T("MY_SN,%s_GJF2_%d;FIRST_SN,ZBPU_NODE_U3_%d"), strJinLuName, iJdqNode, iUpOrDown) ;								
							}
							else if (iJdqNode==61||iJdqNode==41)
							{
								strXdata.Format(_T("MY_SN,%s_GJF2_%d;FIRST_SN,ZBPU_NODE_U5_%d"), strJinLuName, iJdqNode, iUpOrDown) ;								
							}
							else if (iJdqNode==53||iJdqNode==33)
							{
								strXdata.Format(_T("MY_SN,%s_GJF2_%d;FIRST_SN,%sNGL_NODE_A8_1;FIRST_CON_SN,CON_%s_GJF2_%d"), strJinLuName, iJdqNode, strJinLuName, strJinLuName, iJdqNode) ;								
							}
							else if (iJdqNode==63||iJdqNode==43)
							{
								strXdata.Format(_T("MY_SN,%s_GJF2_%d;FIRST_SN,%sNGL_NODE_A18_1;FIRST_CON_SN,CON_%s_GJF2_%d"), strJinLuName, iJdqNode, strJinLuName, strJinLuName, iJdqNode) ;								
							}
							if (!strXdata.IsEmpty())
							{
								mapToAddXdata.SetAt(strTypeName, strXdata) ;
							}

							cSetJdqNode.MoveNext() ;
						}
					}
					cSetJdqNode.Close() ;
				}
			}
			else
			{
				acutPrintf(_T("\n%s执行结果不为1:%d"), strSql, iRsCount) ;

			}

			cSet.Close() ;
		}
	}
	catch (_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e) ;
		AfxMessageBox(sBuff) ;  
	}
	catch (...)
	{
		AfxMessageBox(_T("unknown error in FillXdataGJF2_CeXian")) ;
	}
}

// 填充N+1模块数据
void CZnDmh::FillXdataNAdd1_D(int iJinLuType, const CString &strJinLuName, const CString &strJZ,const CString &strFCZ,const CString &strCZ,const CString &strFJZ,  CMapStringToString & mapStrToStrXdata, CMapStringToString & mapToAddXdata,CMapStringToString & mapToReplaceXdata, int iUpOrDown)
{
	CString strErrMsg ;
	CString strKey, strXdata ;

	mapStrToStrXdata.RemoveAll() ;
	mapToAddXdata.RemoveAll() ;
	mapToReplaceXdata.RemoveAll() ;

	mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZ) ;
	mapStrToStrXdata.SetAt(_T("FCZ_XHJ"), strFCZ) ;
	mapStrToStrXdata.SetAt(_T("CZ_XHJ"), strCZ) ;
	mapStrToStrXdata.SetAt(_T("FJZ_XHJ"), strFJZ) ;

	mapToReplaceXdata.SetAt(_T("JINLUNAME"), strJinLuName) ;

	mapStrToStrXdata.SetAt(_T("FBJ_JINLUNAME"), strJinLuName) ;

	CString strZh_Names[] = {_T("BM"), _T("FB"), _T("JG")} ;
	for(int i=0; i<3; i++)
	{
		strKey.Format(_T("%s_NAME"), strZh_Names[i]) ;
		strXdata.Format(_T("%s(%s)"), strZh_Names[i], strJZ) ;
		mapStrToStrXdata.SetAt(strKey, strXdata) ;
	}

	//XORS
	CString strXorS ;
	strXorS = strJZ.Left(1) ;
	if (iJinLuType==5) //调车接近时不能从信号机名判断出上下行方向，增加一个有默认值的参数放最后，原函数调用不用理会该参数
	{
		strXorS = (iUpOrDown==0?_T("X"):_T("S")) ;
	}
	mapToReplaceXdata.SetAt(_T("XORS"), strXorS) ;

	CString strZhName = _T("FB") ;
	if (iJinLuType==4)
	{
		strZhName = _T("GD") ;
	}
	else if (iJinLuType==5||iJinLuType==8)
	{
		strZhName = _T("GD1") ;
	}
	else if (iJinLuType==7)
	{
		strZhName = _T("ZC") ;
	}
	else if (iJinLuType==6||iJinLuType==9)
	{
		strZhName = _T("JG") ;
	}
	
	mapToReplaceXdata.SetAt(_T("ZH_NAME"), strZhName) ;
	mapStrToStrXdata.SetAt(_T("ZH_NAME"), strZhName) ;

	try
	{
		CString strSql ;
		CADORecordset cSet ;

		//如果是调车接近轨或三线的股道，需要知道轨道名
		CString strDJG ;
		strSql.Format(_T("select * from dmh_import where dmh_import_jinluname='%s' and dmh_import_cellrow<16"), strJinLuName) ;
		CDBUtility::GetRecordValue(strSql, _T("dmh_import_guidao"), strDJG) ;
		CString strCellContentTmp = (iJinLuType>3?strCZ:strJinLuName) ;
		if (iJinLuType<4||iJinLuType==6||iJinLuType==9)
		{
			strCellContentTmp = strJinLuName ;
		}
		else if (iJinLuType==4||iJinLuType==7)
		{
			strCellContentTmp = strCZ ;
		}
		else if (iJinLuType==5||iJinLuType==8)
		{
			strCellContentTmp = strDJG ;
		}

		strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_jdqname='FBJ' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_%s')"), strCellContentTmp, strZhName) ;
		if (cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if (cSet.Open(g_PtrCon, strSql))
		{
			if (cSet.GetRecordCount()==1)
			{
				cSet.MoveFirst() ;

				int iShelfNum = 0, iLayerNum = 0, iBoxNum = 0 ;
				CString	strCellContent ;
				cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
				cSet.GetFieldValue(_T("dmh_in_zcell_boxnum"), iBoxNum) ;
				//cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strCellContent) ;
				CString strZLoc ;
				strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
				strKey.Format(_T("%s_ZLOC"), strZhName) ;
				mapStrToStrXdata.SetAt(strKey, strZLoc) ;

				//FBJ
				strXdata.Format(_T("%d"), iBoxNum) ;
				mapToReplaceXdata.SetAt(_T("JDQINDEX_FBJ"), strXdata) ;
				
				strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iBoxNum) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()>0)
					{
						cSet.MoveFirst() ;

						while (!cSet.IsEOF())
						{
							int iJdqNode = 0 ;
							CString strCon ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
							cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

							CString strTypeName ;
							strTypeName.Format(_T("%s_%dFBJ_%d"), strZhName, iBoxNum, iJdqNode) ;
							mapStrToStrXdata.SetAt(strTypeName, strCon) ;

							strXdata.Format(_T("MY_SN,%s_%dFBJ_%d"), strCellContentTmp, iBoxNum, iJdqNode) ;
							mapToAddXdata.SetAt(strTypeName, strXdata) ;

							cSet.MoveNext() ;
						}
					}

					cSet.Close() ;
				}

				//FBJF
				if (iJinLuType>-1&&(iJinLuType<4||iJinLuType==9)) //正线和三线的JM都用了FBJF
				{
					strXdata.Format(_T("%d"), iBoxNum+1) ;
					mapToReplaceXdata.SetAt(_T("JDQINDEX_FBJF"), strXdata) ;

					strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iBoxNum+1) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					if (cSet.Open(g_PtrCon, strSql))
					{
						if (cSet.GetRecordCount()>0)
						{
							cSet.MoveFirst() ;

							while (!cSet.IsEOF())
							{
								int iJdqNode = 0 ;
								CString strCon ;
								cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
								cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

								CString strTypeName ;
								strTypeName.Format(_T("%s_%dFBJF_%d"), strZhName, iBoxNum+1, iJdqNode) ;
								mapStrToStrXdata.SetAt(strTypeName, strCon) ;

								strXdata.Format(_T("MY_SN,%s_%dFBJF_%d"), strCellContentTmp, iBoxNum+1, iJdqNode) ;
								mapToAddXdata.SetAt(strTypeName, strXdata) ;

								cSet.MoveNext() ;
							}
						}

						cSet.Close() ;
					}
				}

				//LXJF
				if (iJinLuType>3) //正线、三线(除股道，和调车接近共用GD1)的LXJF的端子固定了
				{
					strXdata.Format(_T("%d"), iBoxNum-1) ;
					mapToReplaceXdata.SetAt(_T("JDQINDEX_LXJF"), strXdata) ;

					strSql.Format(_T("select * from dmh_def_zhnb_dhj where dmh_def_zhnb_zhname='%s' and dmh_def_zhnb_jdqindex=%d and dmh_def_zhnb_contype=0"), strZhName, iBoxNum-1) ;
					if (cSet.IsOpen())
					{
						cSet.Close() ;
					}
					if (cSet.Open(g_PtrCon, strSql))
					{
						if (cSet.GetRecordCount()>0)
						{
							cSet.MoveFirst() ;

							while (!cSet.IsEOF())
							{
								int iJdqNode = 0 ;
								CString strCon ;
								cSet.GetFieldValue(_T("dmh_def_zhnb_jdqnode"), iJdqNode) ;
								cSet.GetFieldValue(_T("dmh_def_zhnb_con"), strCon) ;							

								CString strTypeName ;
								strTypeName.Format(_T("%s_%dLXJF_%d"), strZhName, iBoxNum-1, iJdqNode) ;
								mapStrToStrXdata.SetAt(strTypeName, strCon) ;

								cSet.MoveNext() ;
							}
						}

						cSet.Close() ;
					}
				}

				//如果是第三线的接近轨，需要出站信号机（LXJF）
				if (iJinLuType==6)
				{
					strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_shelfnum=%d and dmh_in_zcell_layernum=%d and dmh_in_zcell_boxnum=9"), iShelfNum, iLayerNum) ;
					CString strCZTmp ;
					if (CDBUtility::GetRecordValue(strSql, _T("dmh_in_zcell_cellcontent"), strCZTmp)>0)
					{
						mapStrToStrXdata.SetAt(_T("CZ_XHJ"),  strCZTmp) ;
					}
				}
			}
			else
			{
				strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
				AfxMessageBox(strErrMsg) ;
			}

			cSet.Close() ;
		}

		if (iJinLuType<4)
		{
			//beging BM_ZLOC		
			strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_jdqname='JMJ' and dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_bm')"), strJZ) ;

			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()==1)
				{
					cSet.MoveFirst() ;

					int iShelfNum = 0, iLayerNum = 0 ;

					cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
					cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
					//cSet.GetFieldValue(_T("dmh_in_zcell_cellcontent"), strXhj) ;
					CString strZLoc ;
					strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
					mapStrToStrXdata.SetAt(_T("BM_ZLOC"), strZLoc) ;
				}
				else
				{
					strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}
				cSet.Close() ;
			}
			//end BM_ZLOC
		}

		//begin 第三线的n+1股道部分jg-gd要用到JG组合信息 ZXJF
		if (iJinLuType==8)
		{
			strSql.Format(_T("	select * from dmh_import where dmh_import_line in (SELECT dmh_import_line from dmh_import where dmh_import_jinluname='%s') and dmh_import_jfm='JG' "),  strJinLuName) ;
			CString strJZTmp ;
			if(CDBUtility::GetRecordValue(strSql, _T("dmh_import_xhj"), strJZTmp) >0)
			{
				mapStrToStrXdata.SetAt(_T("JZ_XHJ"), strJZTmp) ;

				strXdata.Format(_T("JG(%s)"),  strJZTmp) ;
				mapStrToStrXdata.SetAt(_T("JG_NAME"), strXdata) ;

				strSql.Format(_T("select * from dmh_in_zcell where dmh_in_zcell_cellcontent='%s' and dmh_in_zcell_jdqname='ZXJF' and dmh_in_zcell_layerid in(select id from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_boxd_z_jg')"), strJZTmp) ;

				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()==1)
					{
						cSet.MoveFirst() ;

						int iShelfNum = 0, iLayerNum = 0 ;

						cSet.GetFieldValue(_T("dmh_in_zcell_shelfnum"), iShelfNum) ;
						cSet.GetFieldValue(_T("dmh_in_zcell_layernum"), iLayerNum) ;
						CString strZLoc ;
						strZLoc.Format(_T("%s%d-%d"), m_strPreZ, iShelfNum, iLayerNum) ;
						mapStrToStrXdata.SetAt(_T("JG_ZLOC"), strZLoc) ;
					}
					else
					{
						strErrMsg.Format(_T("下面sql语句执行结果<>1：%s"), strSql) ;
						AfxMessageBox(strErrMsg) ;
					}
					cSet.Close() ;
				}
			}
		}
		//end 第三线的n+1股道部分jg-gd要用到JG组合信息

		//for FS
		if (iJinLuType==-1)
		{
			strSql.Format(_T("select * from dmh_in_zycell where instr(dmh_in_zycell_jinluname, '+1')>0")) ;
			if (cSet.IsOpen())
			{
				cSet.Close() ;
			}
			if (cSet.Open(g_PtrCon, strSql))
			{
				if (cSet.GetRecordCount()==1)
				{
					cSet.MoveFirst() ;
					int iShlefNum = 0, iBoxNum = 0 ;
					CString strJinLuNameAll, strFreq1, strFreq2 ;
					cSet.GetFieldValue(_T("dmh_in_zycell_shelfnum"), iShlefNum) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_boxnum"), iBoxNum) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_jinluname"), strJinLuNameAll) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_freq1"), strFreq1) ;
					cSet.GetFieldValue(_T("dmh_in_zycell_freq2"), strFreq2) ;

					strXdata.Format(_T("%s%d-%d"), m_strPreZY, iShlefNum, iBoxNum) ;
					mapStrToStrXdata.SetAt(_T("ZY_LOC"), strXdata) ;

					strXdata.Format(_T("%dFS"), iBoxNum) ;
					mapStrToStrXdata.SetAt(_T("FS_INDEX"), strXdata) ;

					strXdata = strFreq1 ;
					if (!strFreq2.IsEmpty())
					{
						strXdata.AppendFormat(_T("/%s"), strFreq2) ;
					}
					mapStrToStrXdata.SetAt(_T("FREQ"), strXdata) ;

					strXdata.Format(_T("(%d)"), (iBoxNum+1)/2) ;
					mapStrToStrXdata.SetAt(_T("DNUM"), strXdata) ;

					strXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?1:3) ;
					mapStrToStrXdata.SetAt(_T("POWER_DNUM_KZ"), strXdata) ;
					strXdata.Format(_T("D%d-%d"), (iBoxNum+1)/2, iBoxNum%2==1?2:4) ;
					mapStrToStrXdata.SetAt(_T("POWER_DNUM_KF"), strXdata) ;
					strXdata.Format(_T("RD%d"), (iBoxNum+1)/2) ;
					mapStrToStrXdata.SetAt(_T("POWER_RDNUM"), strXdata) ;
				}
				else
				{
					strErrMsg.Format(_T("FillXdataFS下面语句执行结果不为1:\n%s"), strSql) ;
					AfxMessageBox(strErrMsg) ;
				}
				cSet.Close() ;
			}

			//汇流排zloc
			if (m_bUseHLP==TRUE)
			{
				strSql.Format(_T("select * from dmh_in_zlayer where dmh_in_zlayer_layerblock='dmh_box_z_hlp'")) ;
				if (cSet.IsOpen())
				{
					cSet.Close() ;
				}
				if (cSet.Open(g_PtrCon, strSql))
				{
					if (cSet.GetRecordCount()==1)
					{
						cSet.MoveFirst() ;
						int iShlefNum = 0, iLayerNum = 0 ;

						cSet.GetFieldValue(_T("dmh_in_zlayer_shelfnum"), iShlefNum) ;
						cSet.GetFieldValue(_T("dmh_in_zlayer_layernum"), iLayerNum) ;

						strXdata.Format(_T("%s%d-%d"), m_strPreZ, iShlefNum, iLayerNum) ;
						mapStrToStrXdata.SetAt(_T("HLP_ZLOC"), strXdata) ;
					}
					cSet.Close() ;
				}

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
		AfxMessageBox(_T("unknown error in FillXdataNAdd1_D")) ;
	}
}