#include "stdafx.h"
#include "zndmh.h"

#include "DBUtility.h"

#include "ConNode.h"

extern BOOL bFirst ;

// ���Ƶ��뻯����
BOOL CZnDmh::DoAllPx(const CString& strPath)
{
	BOOL bReturn = TRUE ;
	CString strSql ;

	CStringArray strArrayStation ; //�洢strPathĿ¼�µĳ�վĿ¼��
	CBlkUtility::FindFile(strPath, _T(""), strArrayStation, 1, 1) ; //strPathĿ¼�µ��ļ��У����ݹ��ļ����µ��ļ��У�
	int iStationNum = 0 ;
	iStationNum = strArrayStation.GetCount() ;
	//�������г�վĿ¼
	for (int iStationIndex=0; iStationIndex<iStationNum; iStationIndex++)
	{
		CString strStationPath ;		
		strStationPath = strArrayStation.GetAt(iStationIndex) ;
		CString strStationName ;
		strStationName = strStationPath.Right(strStationPath.GetLength()-strStationPath.ReverseFind(_T('\\'))-1) ;//��վ��

		CArray<AcDbDatabase * , AcDbDatabase *> cArrayDwg_Px_Z ;
		CArray<AcDbDatabase  *, AcDbDatabase *> cArrayDwg_Px_ZY ;
		CArray<AcDbDatabase * , AcDbDatabase *> cArrayDwg_Px_ZZH ;
		CArray<AcDbDatabase * , AcDbDatabase *> cArrayDwg_Px_FXP ;
		CArray<AcDbDatabase * , AcDbDatabase *> cArrayDwg_Px_HLP ; //����������

		CString strZ_Error_Text, strZY_Error_Text, strZZH_Error_Text, strFXP_Error_Text, strHLP_Error_Text ;

		//������ݿ�
		CString strClearTableName[4] = { _T("dmh_have_con_tie_zy") , _T("dmh_have_con_tie_z") , _T("read_symbol_info"), _T("g_have_con_tie") } ;
		for (int i=0 ; i<4 ; i++)
		{
			CDBUtility::ClearTable(strClearTableName[i]) ;
		}		

		CStringArray strArrayDwg ; //�洢��վĿ¼�µ�ͼֽ��		
		CBlkUtility::FindFile(strStationPath, _T("dwg"), strArrayDwg, 1, 0) ; //strStationPathĿ¼�µ�dwg�ļ������ݹ��Ŀ¼�µ��ļ����ڵ�dwg�ļ���
		int iDwgNum = 0 ;
		iDwgNum = strArrayDwg.GetCount() ;
		//������
		acedSetStatusBarProgressMeter(_T("���뻯����"), 1, iDwgNum+5) ;
		for (int iDwgIndex=0; iDwgIndex<iDwgNum; iDwgIndex++)
		{
			acedSetStatusBarProgressMeterPos(iDwgIndex+1) ;

			CString strDwgPath ;
			strDwgPath = strArrayDwg.GetAt(iDwgIndex) ;
			CString strDwgName ;
			strDwgName = strDwgPath.Right(strDwgPath.GetLength()-strDwgPath.ReverseFind(_T('\\'))-1) ;//ͼֽ��

			acutPrintf(_T("\n%s"), strDwgName) ;

			BOOL bCircuit = FALSE ;  //�Ƿ�Ϊ��·ͼ ;
			g_pCurDb = new AcDbDatabase ;
			g_cSystem.m_pDb = g_pCurDb ;
			if(Acad::eOk == g_pCurDb->readDwgFile(strDwgPath))
			{
				CString strDrawType ;
				if(g_cSystem.ReadFromRec(_T("second_draw") , _T("draw_type") , strDrawType))
				{
					if(strDrawType == _T("CIRCUIT"))  //�ǵ�·ͼ 
					{	
						//�Ƚ�n+1����ת��ΪID
						if (strDwgName.Find(_T("n+1"))!=-1)
						{
							this->TransNAdd1SnToId(g_pCurDb) ;
						}

						this->ReadDwgTie() ;
						this->SetTieParentText() ;
						// 						//��"QZ1"����"QZ1-"
						strSql.Format (_T("Update tie_info set tie_parent_text = 'QZ1-' where tie_parent_text = 'QZ1'")) ;
						ExecSqlString (strSql) ;
						bCircuit = TRUE ;
					}
				}
			}
			delete g_pCurDb ;
			g_pCurDb = NULL ;

			if(bCircuit)
			{
				//��������ͼ��
				this->Draw_PX_Z(&cArrayDwg_Px_Z, strZ_Error_Text) ;
				this->Draw_PX_ZY(&cArrayDwg_Px_ZY, strZY_Error_Text) ;
				this->Draw_PX_ZZH(&cArrayDwg_Px_ZZH, strZZH_Error_Text) ;
				this->Draw_PX_FXP(&cArrayDwg_Px_FXP, strFXP_Error_Text) ;
				if (strDwgName.Find(_T("n+1"))!=-1)
				{
					//begin ��������ŵ�����
					this->UpdateDbForHLP() ;
					this->Draw_PX_Z(&cArrayDwg_Px_Z, strZ_Error_Text, 1) ;
					this->Draw_PX_ZY(&cArrayDwg_Px_ZY, strZY_Error_Text, 1) ;
					this->Draw_PX_ZZH(&cArrayDwg_Px_ZZH, strZZH_Error_Text, 1) ;
					this->Draw_PX_HLP(&cArrayDwg_Px_HLP, strHLP_Error_Text) ;
					//end ��������ŵ�����
				}
			}	
		}

		//��Ϲ�
		this->Save_PX_Drawing( strStationPath ,_T("Z") , &cArrayDwg_Px_Z ) ;
		acedSetStatusBarProgressMeterPos(iDwgNum+1) ;
		//��Ƶ��
		this->Save_PX_Drawing( strStationPath ,_T("ZY") , &cArrayDwg_Px_ZY ) ;
		acedSetStatusBarProgressMeterPos(iDwgNum+2) ;
		//����ӿڹ�zzh
		this->Save_PX_Drawing(strStationPath, _T("ZZH"), &cArrayDwg_Px_ZZH) ;
		acedSetStatusBarProgressMeterPos(iDwgNum+3) ;
		//fxp
		this->Save_PX_Drawing(strStationPath, _T("F"), &cArrayDwg_Px_FXP) ;
		acedSetStatusBarProgressMeterPos(iDwgNum+4) ;
		//HLP
		this->Save_PX_Drawing(strStationPath, _T("HLP"), &cArrayDwg_Px_HLP) ;
		acedSetStatusBarProgressMeterPos(iDwgNum+5) ;

		acedRestoreStatusBar() ;

	}

	return bReturn ;
}

void CZnDmh::ReadDwgTie(void)
{	
	CString strExec ;				

	CString strTableName[3] = {_T("tie_info") , _T("parent_info"), _T("tie_con_cable_info") } ;
	for (int i = 0 ; i < 3 ; i ++)
	{
		CDBUtility::ClearTable(strTableName[i]) ;
	}

	AcDbBlockTable * pBlkTbl = NULL ;
	g_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
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
			AcDbText * pText = NULL ;
			if(pEnt->isKindOf(AcDbText::desc())) 
			{
				pText = AcDbText::cast(pEnt) ;
				AcGePoint3d base_3d(0,0,0) ;
				base_3d = pText->position() ;			

				int lCon_1_Id = 0 , lCon_2_Id = 0 , lMySelf_Id = 0 , lParent_Id = 0 , lCon_1_Cable_Id = 0 , lCon_2_Cable_Id = 0, lCon_Gd_Id=0 ;
				int iTemp_Val = 0 ;
				CString strType ;
				CString strToCon ;
				if (g_cSystem.GetXdata(pEnt, _T("TOCON"), strToCon)&& m_bUseHLP)
				{	
						g_cSystem.GetXdata(pEnt , _T("ID") , lMySelf_Id) ;	
						if (lMySelf_Id==0)
						{
							lMySelf_Id=-1 ;
						}
						g_cSystem.GetXdata(pEnt , _T("PARENT_ID") , lParent_Id) ;
						CString strCon1Text ;
						int iLen = strToCon.GetLength() ;
						int iLoc = strToCon.ReverseFind(_T('_')) ;
						strCon1Text.Format(_T("H-%s"), strToCon.Right(iLen-iLoc-1)) ;
						CString strMySelf_Text ;
						strMySelf_Text = pText->textString() ;
						strExec.Format(_T("insert into tie_info (tie_id , tie_text , tie_parent_id ,   base_x , base_y ,  con_1_text) values \
										  (%d , '%s' , %d ,  %.1f , %.1f ,'%s') ") , lMySelf_Id , strMySelf_Text , lParent_Id ,  base_3d[X] , base_3d[Y], strCon1Text ) ;
						ExecSqlString (strExec) ;	
				}
				else if(g_cSystem.GetXdata(pEnt , _T("CON_1_ID") , lCon_1_Id))
				{
					g_cSystem.GetXdata(pEnt , _T("ID") , lMySelf_Id) ;
					g_cSystem.GetXdata(pEnt , _T("CON_2_ID") , lCon_2_Id) ;
					g_cSystem.GetXdata(pEnt , _T("PARENT_ID") , lParent_Id) ;
					g_cSystem.GetXdata (pEnt , _T("CON_1_CABLE_ID") , lCon_1_Cable_Id) ;
					g_cSystem.GetXdata (pEnt , _T("CON_2_CABLE_ID") , lCon_2_Cable_Id) ;							

					if (g_cSystem.GetXdata(pEnt , _T("TYPE") , strType))
					{
						g_cSystem.GetXdata (pEnt , _T("CON_GD_ID") , lCon_Gd_Id) ;		
					}

					CString strMySelf_Text ;
					strMySelf_Text = pText->textString() ;
					strExec.Format(_T("insert into tie_info (tie_id , tie_text , tie_parent_id , con_1_id , con_2_id , base_x , base_y , con_1_cable_id , con_2_cable_id, con_gd_id, tie_type) values \
									  (%d , '%s' , %d , %d , %d , %.1f , %.1f , %d , %d, %d, '%s') ") , lMySelf_Id , strMySelf_Text , lParent_Id , lCon_1_Id , lCon_2_Id , base_3d[X] , base_3d[Y] ,
									  lCon_1_Cable_Id , lCon_2_Cable_Id, lCon_Gd_Id, strType) ;
					ExecSqlString (strExec) ;
					
				}
				else if(g_cSystem.GetXdata(pEnt , _T("IS_PARENT") , iTemp_Val))
				{
					g_cSystem.GetXdata(pEnt , _T("ID") , lMySelf_Id) ;
					if (lMySelf_Id!=0)
					{
						CString strParent_Text ;
						strParent_Text = pText->textString() ;

						strExec.Format(_T("insert into parent_info (parent_id , parent_text) values (%d , '%s') ") , lMySelf_Id , strParent_Text) ;
						ExecSqlString (strExec) ;						
					}
				}
				else if (g_cSystem.GetXdata(pEnt , _T("TYPE") , strType))//�����̹����Ĺ����
				{
					if (strType==_T("GUIDAO"))
					{
						if(g_cSystem.GetXdata(pEnt , _T("ID") , lMySelf_Id))
						{
							CString strMySelf_Text ;
							strMySelf_Text = pText->textString() ;
							strExec.Format(_T("insert into tie_info (tie_id , tie_text ,  base_x , base_y ) values (%d , '%s' , %.1f , %.1f) ") , lMySelf_Id , strMySelf_Text, base_3d[X] , base_3d[Y]) ;
							ExecSqlString (strExec) ;
						}
					}
				}
			}
			else if (pEnt->isKindOf(AcDbBlockReference::desc()))  //��ͼ�� ;
			{
				long lMySelf_Id = 0 ;
				AcDbBlockReference * pRef = AcDbBlockReference::cast(pEnt) ;
				CString strBlockName ;
				g_cSystem.GetBlockName(pRef , strBlockName) ;
				strBlockName.MakeLower () ;
				if(strBlockName.GetLength()>5)
				{
					if(strBlockName.Mid (0, 5) == _T("conti"))
					{
						if(g_cSystem.GetXdata(pEnt , _T("ID") , lMySelf_Id))
						{
							CString strCon_Cable_Text ;
							g_cSystem.GetAttValue(pRef , _T("1") , strCon_Cable_Text) ;
							strExec.Format(_T("insert into tie_con_cable_info (con_cable_id , con_cable_text , blockname) values (%d , '%s' , '%s') "),	lMySelf_Id , strCon_Cable_Text , strBlockName) ;
							ExecSqlString (strExec) ;
						}
					}
				}
			}
			pEnt->close() ;
		}
	} //end for 

	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

void CZnDmh::SetTieParentText(void)
{
	// ��parent���ı�д��ȥ ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		strSql.Format(_T("select * from parent_info order by ID")) ;
		if(cSet.IsOpen())
		{
			cSet.Close() ;
		}
		if(cSet.Open(g_PtrCon , strSql))
		{
			if(cSet.GetRecordCount() > 0)
			{
				cSet.MoveFirst() ;
				while (!cSet.IsEOF())
				{
					long lParent_Id = 0 ;
					CString strParentText ;
					cSet.GetFieldValue(_T("parent_id") , lParent_Id) ;
					cSet.GetFieldValue(_T("parent_text") ,strParentText) ;

					strSql.Format(_T("Update tie_info Set tie_parent_text = '%s' where tie_parent_id = %d"), strParentText ,lParent_Id) ;
					ExecSqlString (strSql) ;

					cSet.MoveNext();
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
		AfxMessageBox(_T("δ֪�Ĵ���!in SetTieParentText"));
	}
}

void CZnDmh::Draw_PX_Z(CArray < AcDbDatabase * , AcDbDatabase * >  *  pDbArray , CString & strError_Text, int iMode)
{		
	double dSpaceX_Table = 5.00 , dSpaceY_Table = 17.50 , 
		dExtendX = 2.00 , dExtendY = 2.00 ,
		dWidth_Table = 20.00 , dHeigth_Table = 7.00;
	//CString strErro_Info = _T("��ϼ����ߴ�����Ϣ\n") ;

	try
	{	
		//cTie_Set    ��'z'��ͷ�Ķ��ӵļ�¼����
		//cTie_Set2   ���еĶ��ӵļ�¼�� ��
		//cTie_Set3   �Է�'z'��ͷ�ģ�����21-203-4�Ķ��� ��
		CADORecordset cTie_Set , cTie_Set2 ;
		CString strSql ,strSql2 ;
		//
		strSql2.Format( _T("select * from tie_info order by ID") ) ;
		if( cTie_Set2.Open( g_PtrCon , strSql2 ) )
		{
			if(cTie_Set2.GetRecordCount() > 0 )
			{
				cTie_Set2.MoveFirst() ;
			}
		}

		//begin�ȴ�����'z'��ͷ�Ķ��ӣ�����z2-304-2��
		int iZ_Pre_Len = 0 ;
		iZ_Pre_Len = m_strPreZ.GetLength() ;
		int iZZH_Pre_Len = 0 ;
		iZZH_Pre_Len = m_strPreZZH.GetLength() ;
		int iZY_Pre_Len = 0 ;
		iZY_Pre_Len = m_strPreZY.GetLength() ;

		strSql.Format( _T("select * from tie_info where Left(tie_parent_text,%d) = '%s' and Left(tie_parent_text, %d ) <> '%s' and Left(tie_parent_text,%d) <> '%s' and Left(tie_parent_text,%d)<> '%sH' and Left(tie_parent_text,3) <> 'ZJC' and Left(tie_text, 1 ) <> 'D' %s order by ID") ,
			iZ_Pre_Len , m_strPreZ, iZZH_Pre_Len , m_strPreZZH, iZY_Pre_Len, m_strPreZY, iZ_Pre_Len+1, m_strPreZ, iMode==0?_T("and (isnull(con_1_text) or left(con_1_text,1)<>'H')"):_T("and left(con_1_text,1)='H'") ) ;

		//strSql.Format(_T("select * from tie_info where Left(tie_parent_text,1) = 'z' and Left(tie_parent_text,3) <> 'zzh' and Left(tie_parent_text,2) <> 'ZY' and Left(tie_parent_text,3)<> 'ZJC'and Left(tie_parent_text,2)<> 'ZH' and Left(tie_text, 1 ) <> 'D' order by ID")) ;
		if( cTie_Set.Open( g_PtrCon ,strSql ) )
		{
			if( cTie_Set.GetRecordCount() > 0 )
			{
				cTie_Set.MoveFirst() ;
				CString strMySelf_Text ;
				int iMySelfCol = 0 , iMySelfRow = 0 ;
				CString strZ_Loc ;
				int iMySelf_Block = 0 ; //�ڼ������ ;
				int iZ_No = 0 ; //�ڼ������� ;
				long lCon_1_Id = 0 ;//�����1 ;
				long lCon_2_Id = 0 ;//���ӵ�2 ;
				CString strTempPre , strTempPre2 ;

				ads_point base_Pt = { 0,0,0 } ;

				while ( !cTie_Set.IsEOF() )
				{
					cTie_Set.GetFieldValue( _T("tie_parent_text") , strZ_Loc ) ;

					//����z2-304-2������ ��

					//��������Compute_Pre_Bh()���ַ���tie-parent�ֽ� ;
					CBlkUtility::Compute_Pre_Bh( strZ_Loc , strTempPre , iMySelf_Block ) ; //��iMySelf_Block 3;

					strTempPre = this->CutStringBack( strTempPre ) ;
					CBlkUtility::Compute_Pre_Bh( strTempPre , strTempPre2 , iZ_No  ) ; //���Ӻ�iZ_No 2 ;
					if( iZ_No == 0 )
					{
						iZ_No = 1 ;
					}

					g_pCurDb = this->GetCurDatabase( pDbArray , iZ_No , _T("Z") ) ;
					g_cSystem.m_pDb = g_pCurDb ;

					cTie_Set.GetFieldValue( _T("tie_text") , strMySelf_Text ) ;
					cTie_Set.GetFieldValue( _T("con_1_Id") ,lCon_1_Id ) ;
					cTie_Set.GetFieldValue( _T("con_2_id") ,lCon_2_Id ) ;
					long lCon_Cable_Id[2] = { 0 , 0 } ;

					cTie_Set.GetFieldValue( _T("con_1_cable_id") , lCon_Cable_Id[0] ) ;
					cTie_Set.GetFieldValue( _T("con_2_cable_id") , lCon_Cable_Id[1] ) ;

					//��tie_text�ֽ�(����01-12�ֳ�1��12) ;
					CString strZ_MySelf_Text ; //�ϲ��Ժ���ı� ;
					strZ_MySelf_Text.Format( _T("%s%s") , strZ_Loc ,strMySelf_Text ) ;
					//strZ_MySelf_Text.Format(_T("%s%d-%d%s"), strTempPre2, iZ_No, iMySelf_Block, strMySelf_Text ) ;

					//��ѯ�͸ýӵ������Ľӵ�,���������� ;					
					CString strSql_Find , strResult = _T("") , strResult2 =_T("") ,
						strFind_Tie_Parent_Text , strFind_Tie_Text , 
						strFind_Tie_Parent_Text2 , strFind_Tie_Text2 ;

					long lCon_1_2_Id[2] ;
					lCon_1_2_Id[0] = lCon_1_Id ;
					lCon_1_2_Id[1] = lCon_2_Id ;

					//��һ�����Ӷ�Ӧ�ڵ� ;
					CString strFill_1_Text  ;
					CString strTemp_Error_Text ;
					for ( int i = 0 ; i < 2 ; i ++ )
					{
						strSql_Find.Format( _T("tie_id=%d") , lCon_1_2_Id[i]) ;

						if( cTie_Set2.FindFirst( strSql_Find ) )
						{
							int iTie_Front = 0 , iTie_Back = 0 ;
							cTie_Set2.GetFieldValue( _T("tie_parent_text") , strFind_Tie_Parent_Text ) ;
							cTie_Set2.GetFieldValue( _T("tie_text") , strFind_Tie_Text ) ;
							strFill_1_Text.Format( _T("%s%s") , strFind_Tie_Parent_Text , strFind_Tie_Text ) ;
							this->Fill_Z_Table( strZ_MySelf_Text , strFill_1_Text , strTemp_Error_Text , lCon_Cable_Id[i] );								
						}
					}
					cTie_Set.MoveNext() ;
				}
			}
			cTie_Set.Close() ;
		}
		//end������'z'��ͷ�Ķ��ӽ�����

		cTie_Set2.Close() ;		
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!Draw_PX_Z"));
	}
}

//iMode=1ʱ����ʾ���ƻ����ŵ�����
void CZnDmh::Draw_PX_ZY(CArray < AcDbDatabase *, AcDbDatabase * >  * pDbArray, CString &strError_Text, int iMode)
{
	CString strZYName=m_strPreZY;//������Ƶ�������
	CString strTemp;
	int iPaneWidth=20,iPaneHeight=8,iAllWidth=75;//ͼ�з���Ŀ�ȡ��߶ȡ�ÿ���󷽸�Ŀ�ȣ�������ࣩ
	try
	{
		CADORecordset cTie_Set ;
		CString strSql ;
		strSql.Format( _T("select * from tie_info where Left(tie_parent_text,%d) = '%s' %s order by ID"),m_strPreZY.GetLength() , strZYName, iMode==0?_T("and (isnull(con_1_text) or left(con_1_text,1)<>'H')"):_T("and left(con_1_text,1)='H'")) ;
		if( cTie_Set.Open( g_PtrCon ,strSql ) )
		{
			if( cTie_Set.GetRecordCount() > 0 )
			{
				CADORecordset cSet ;//ѡ��Ҫ���ߵĶ���
				CADORecordset cRS;//ѡ���ö��ӵĶ�Ӧ����
				cSet = cTie_Set ;
				cSet.MoveFirst() ;
				CString strMySelf_Text ;//����ͼ�е�����
				CString strZY_Loc ;
				//��дλ�õ���Ϣ
				int iMySelf_Block = 0 ; //�ڼ������ ;
				int iZY_No = 0 ; //�ڼ�����Ƶ�� ;
				int iMySelfCol = 0 , iMySelfRow = 0 ;//��д��λ�� �� ��
				//Ҫ��д���ݵ���Ϣ
				CString strInput;//������һ�ֻ��������
				int iInput_Block=0;//�ڼ������ ;
				int iInput_ZY_No=0;//�ڼ�����Ƶ�� ;
				int iInputCol=0,iInputRow=0;//���е�λ��

				int iCon_ID[2];//��ȡ���Ķ�����ض��ӵ�ID
				CString strTempPre , strTempPre2 , strTempPre1,strTempPre3 ;

				ads_point base_Pt = { 0,0,0 } ;
				while ( !cSet.IsEOF() )
				{
					cSet.GetFieldValue( _T("tie_parent_text") , strZY_Loc ) ;
					CBlkUtility::Compute_Pre_Bh( strZY_Loc , strTempPre , iMySelf_Block ) ;
					strTempPre = this->CutStringBack( strTempPre ) ;
					CBlkUtility::Compute_Pre_Bh( strTempPre , strTempPre2 , iZY_No  ) ;
					if( iZY_No == 0 )
					{
						iZY_No = 1 ;
					}

					g_pCurDb = this->GetCurDatabase( pDbArray , iZY_No , _T("ZY") ) ;
					g_cSystem.m_pDb = g_pCurDb ;

					//ȷ��Ҫ��д���к���
					cSet.GetFieldValue(_T("tie_text"),strTempPre);
					Divide_Text(strTempPre,iMySelfCol,iMySelfRow);
					strTemp.Format(_T("%s%d-%d0%d-%d"),strZYName,iZY_No,iMySelf_Block,iMySelfCol,iMySelfRow);//Ŀǰ����д�Ķ��ӵ�����
					if (iMySelfRow==0 || iMySelfCol==0 || iMySelfRow>18 || iMySelfCol>3)
					{
						//����ȡ�����к����д��ڲ��������֣�ֱ���˳�ִ����һ��ѭ��
						strError_Text=strError_Text + _T("����") + strTemp + _T("��������ķ�Χ\n");
						cSet.MoveNext();
						continue;
					}

					//ȡ������ѡ������صĶ��ӵ�ID
					cSet.GetFieldValue(_T("con_1_id"),iCon_ID[0]);
					cSet.GetFieldValue(_T("con_2_id"),iCon_ID[1]);
					CString strCon_Cable_FiledName [2] = { _T("con_1_cable_id") , _T("con_2_cable_id") } ;
					long lCon_Cable_Id = 0  ;
					for (int i=0;i<2;i++)
					{
						lCon_Cable_Id = 0  ;
						strSql.Format(_T("select * from tie_info where tie_id=%d"),iCon_ID[i]);
						if( cRS.Open( g_PtrCon ,strSql ) )
						{
							if (cRS.GetRecordCount()==1)
							{
								//ȡ��Ҫ��д���ݵ�ȫ��
								cRS.GetFieldValue(_T("tie_parent_text"),strTempPre);
								cRS.GetFieldValue(_T("tie_text"),strTempPre2);

								cRS.GetFieldValue( strCon_Cable_FiledName[i] , lCon_Cable_Id ) ;
								cRS.Close();
							}
							else
							{
								cRS.Close();
								continue;
							}
						}
						//ȡ����Ӧ����Ϣ
						CBlkUtility::Compute_Pre_Bh(strTempPre,strTempPre1,iInput_Block);
						strTempPre3=this->CutStringBack(strTempPre1);
						CBlkUtility::Compute_Pre_Bh(strTempPre3,strInput,iInput_ZY_No);
						Divide_Text(strTempPre2,iInputCol,iInputRow);
						if (iInput_ZY_No==0)
						{
							iInput_ZY_No = 1 ;
						}

						if (strInput != m_strPreZY )  //��������� ��Ƶ ����;
						{
							//�������QY�ϵĶ�����ֱ��д��
							strMySelf_Text=strTempPre+strTempPre2;
						}
						else
						{
							//��ZY�ϵĶ��ӣ����ж�
							if (iInput_ZY_No!=iZY_No)
							{	
								//����ͬһ������
								strMySelf_Text=strTempPre+strTempPre2;
							}
							else
							{
								if (iInput_Block!=iMySelf_Block)
								{
									//����ͬһ������
									strMySelf_Text.Format(_T("%d%s"),iInput_Block,strTempPre2);
								}
								else
								{
									if (iMySelfCol!=iInputCol)
									{
										//����ͬһ��
										strMySelf_Text=strTempPre2;
									}
									else
									{
										strMySelf_Text.Format(_T("%d"),iInputRow);
									}
								}
							}
						}
						//����Ӧ��д���λ��
						base_Pt[X]=(10-iMySelf_Block)*iAllWidth+(3-iMySelfCol)*iPaneWidth+1;
						base_Pt[Y]=(18-iMySelfRow)*iPaneHeight+iPaneHeight*3/4;
						//�ڽ��õ�������д��ͼ��֮ǰ�����жϸ�λ���Ƿ�д������д����д�������һ��������д��λ��
						//��ûд����������ͼ�е�����д�����ݿ�

						strSql.Format(_T("select * from dmh_have_con_tie_zy where shelf_no=%d and block_no=%d and col_no=%d and row_no=%d"),iZY_No,iMySelf_Block,iMySelfCol,iMySelfRow);
						int iTemp=0;
						if( cRS.IsOpen() )
						{
							cRS.Close() ;
						}
						if( cRS.Open( g_PtrCon ,strSql ) )
						{
							if (cRS.GetRecordCount()==0)
							{

								CString strExec ;
								strExec.Format( _T("insert into dmh_have_con_tie_zy (shelf_no , block_no , col_no , row_no , tie_number, tie_text ) \
												values ( %d , %d , %d , %d ,%d, '%s' )") , iZY_No , iMySelf_Block , iMySelfCol , iMySelfRow , 1, strMySelf_Text) ;
								ExecSqlString ( strExec ) ;
							}
							else
							{
								cRS.GetFieldValue(_T("tie_number"),iTemp); 
								iTemp = iTemp + 1 ;
								CString strHaveTie ;
								cRS.GetFieldValue(_T("tie_text"), strHaveTie) ;
								strHaveTie.AppendFormat(_T(",%s"), strMySelf_Text) ;
								cRS.Edit () ;
								cRS.SetFieldValue(_T("tie_number"),iTemp); 
								cRS.SetFieldValue(_T("tie_text"),strHaveTie);
								cRS.Update () ;
								base_Pt[Y]=base_Pt[Y]-4;
							}
							cRS.Close();
						}
						//��ʼ���ó�������д��ͼ��

						g_cSystem.AddText(strMySelf_Text, base_Pt, base_Pt, AcDb::kTextLeft, AcDb::kTextVertMid, 2, 0.7);
						if( lCon_Cable_Id != 0 )
						{
							this->Fill_Connect_Cable_Block ( base_Pt , lCon_Cable_Id ) ;
						}
					}
					cSet.MoveNext() ;
				}
			}
			cTie_Set.Close() ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!"));
	}
}

//begin����zzh��ͷ�����ߣ���Ϊ���߱����ϼ����߱�һ�������Դ�����������ϼ�����ģ�飻
void CZnDmh::Draw_PX_ZZH(CArray < AcDbDatabase * , AcDbDatabase * >  *  pDbArray  , CString & strError_Text, int iMode) 
{
	double dSpaceX_Table = 5.00 , dSpaceY_Table = 17.50 , 
		dExtendX = 2.00 , dExtendY = 2.00 ,
		dWidth_Table = 20.00 , dHeigth_Table = 7.00;
	//CString strErro_Info = _T("�ۺϼ����ߴ�����Ϣ\n") ;

	try
	{	
		//cTie_Set   ���еĶ��ӵļ�¼�� ��
		//cTie_Set3   �Է�'zzh'��ͷ�ģ�����zzh1-203-4�Ķ��� ��
		CADORecordset cTie_Set, cTie_Set3 ;
		CString strSql2 ,strSql3;
		//
		strSql2.Format(_T("select * from tie_info order by ID")) ;
		if(cTie_Set.Open(g_PtrCon , strSql2))
		{
			if(cTie_Set.GetRecordCount() > 0)
			{
				cTie_Set.MoveFirst() ;
			}
		}

		//begin������zzh��ͷ�Ķ��ӣ�
		//begin�ȴ�����'z'��ͷ�Ķ��ӣ�����z2-304-2��
		int iZ_Pre_Len = 0 ;
		iZ_Pre_Len = m_strPreZ.GetLength() ;
		int iZY_Pre_Len = 0 ;
		iZY_Pre_Len = m_strPreZY.GetLength() ;
		int iZZH_Pre_Len = 0 ;
		iZZH_Pre_Len = m_strPreZZH.GetLength() ;

		strSql3.Format(_T("select * from tie_info where Left(tie_parent_text,%d) = '%s' and Left(tie_parent_text,%d) <> '%s' and Left(tie_parent_text,3) <> 'ZJC' and Left(tie_text, 1) <> 'D' %s order by ID") ,
			iZZH_Pre_Len , m_strPreZZH, iZY_Pre_Len, m_strPreZY, iMode==0?_T("and (isnull(con_1_text) or left(con_1_text,1)<>'H')"):_T("and left(con_1_text,1)='H'")) ;

		//strSql3.Format(_T("select * from tie_info where Left(tie_parent_text,3) = 'zzh' and Left(tie_text, 1) <> 'D' order by ID")) ;
		if(cTie_Set3.Open(g_PtrCon ,strSql3))
		{			
			if(cTie_Set3.GetRecordCount() > 0)
			{
				cTie_Set3.MoveFirst() ;
				CString strMySelf_Text ;
				int iMySelfCol = 0 ; //
				int iMySelfRow = 0 ; //
				int iPxtDwgIndex = 0 ;
				CString strZ_Loc ;  //������ ��
				int iMySelf_Block = 0 ; //�ڼ��� ;
				int iZ_No = 0 ; //�ڼ������ ;
				long lCon_1_Id = 0 ;//�����1 ;
				long lCon_2_Id = 0 ;//���ӵ�2 ;
				CString strTempPre , strTempPre2 ;

				ads_point base_Pt = { 0,0,0 } ;

				while (!cTie_Set3.IsEOF())
				{
					cTie_Set3.GetFieldValue(_T("tie_parent_text") , strZ_Loc) ;

					//����zzh1-304-2������ ��

					//��������Compute_Pre_Bh()���ַ���tie-parent�ֽ� ;
					CBlkUtility::Compute_Pre_Bh(strZ_Loc , strTempPre , iMySelf_Block) ; //��iMySelf_Block 3;
					strTempPre = this->CutStringBack(strTempPre) ;

					//begin�õ����Ӻ� ��
					CBlkUtility::Compute_Pre_Bh(strTempPre , strTempPre2 , iZ_No ) ; 					
					//end�õ����Ӻ� ��

					//begin�õ�ͼ�����ݿ�ĺ�
					if(iZ_No == 0)
					{
						iZ_No = 1 ;
					}

					g_pCurDb = this->GetCurDatabase(pDbArray , iZ_No , _T("ZZH")) ;
					g_cSystem.m_pDb = g_pCurDb ;

					cTie_Set3.GetFieldValue(_T("tie_text") , strMySelf_Text) ;
					cTie_Set3.GetFieldValue(_T("con_1_Id") ,lCon_1_Id) ;
					cTie_Set3.GetFieldValue(_T("con_2_id") ,lCon_2_Id) ;
					long lCon_Cable_Id[2] = { 0 , 0 } ;

					cTie_Set3.GetFieldValue(_T("con_1_cable_id") , lCon_Cable_Id[0]) ;
					cTie_Set3.GetFieldValue(_T("con_2_cable_id") , lCon_Cable_Id[1]) ;

					//�ϲ��Ժ���������ı�  ;
					CString strZZH_MySelf_Text ; //�ϲ��Ժ���ı� ;
					strZZH_MySelf_Text.Format(_T("%s%s") , strZ_Loc ,strMySelf_Text) ;

					//��ѯ�͸ýӵ������Ľӵ�,���������� ;					
					CString strSql_Find , strResult = _T("") , strResult2 =_T("") ,
						strFind_Tie_Parent_Text , strFind_Tie_Text , 
						strFind_Tie_Parent_Text2 , strFind_Tie_Text2 ;

					long lCon_1_2_Id[2] ;
					lCon_1_2_Id[0] = lCon_1_Id ;
					lCon_1_2_Id[1] = lCon_2_Id ;

					//��һ�����Ӷ�Ӧ�ڵ� ;
					CString strFill_1_Text  ;
					CString strTemp_Error_Text ;
					for (int i = 0 ; i < 2 ; i ++)
					{
						strSql_Find.Format(_T("tie_id=%d") , lCon_1_2_Id[i]) ;
						if(cTie_Set.FindFirst(strSql_Find))
						{
							int iTie_Front = 0 , iTie_Back = 0 ;
							cTie_Set.GetFieldValue(_T("tie_parent_text") , strFind_Tie_Parent_Text) ;
							cTie_Set.GetFieldValue(_T("tie_text") , strFind_Tie_Text) ;
							strFill_1_Text.Format(_T("%s%s") , strFind_Tie_Parent_Text , strFind_Tie_Text) ;
							this->Fill_Z_Table(strZZH_MySelf_Text , strFill_1_Text , strTemp_Error_Text , lCon_Cable_Id[i], 0, 1);
						}
					}
					cTie_Set3.MoveNext() ;	
				}
			}
			cTie_Set3.Close() ;
			//cTie_Set.Close() ; 
		}

		//end������ZZH��ͷ�Ķ��ӽ�����

		cTie_Set.Close() ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!"));
	}
}

void CZnDmh::DrawAllPx( PXT_DATA * pData)
{
	//pData->strData = CBlkUtility::GetDateStr() ;
	//this->Fill_Circuit_Data_XXX ( pData ) ;
	int iTemp = 0 ; 

	AcDbBlockTable * pBlkTbl = NULL ;
	g_pCurDb->getBlockTable(pBlkTbl,AcDb::kForRead);
	AcDbBlockTableRecord * pBlkTblRec = NULL ;
	pBlkTbl->getAt (ACDB_MODEL_SPACE, pBlkTblRec,AcDb::kForRead)  ;
	pBlkTbl->close() ;
	AcDbBlockTableRecordIterator *pBlkTblRecIter = NULL ;

	pBlkTblRec->newIterator( pBlkTblRecIter );
	pBlkTblRec->close();

	AcDbEntity * pEnt = NULL ;
	for(pBlkTblRecIter->start();!pBlkTblRecIter->done();pBlkTblRecIter->step())
	{
		if(Acad::eOk == pBlkTblRecIter->getEntity( pEnt ,AcDb::kForWrite ) )
		{
			AcDbText * pText = NULL ;
			if(pEnt->isKindOf(AcDbText::desc())) 
			{
				pText  = AcDbText::cast(pEnt) ;
				CString strTemp ;
				int iType ;
				if( g_cSystem.GetXdata( pEnt , _T("TYPE") , iType ) )
				{
					switch ( iType )
					{
					case 32 :
					case 33 :
					case 50:
						{
							int iIndex = 0 ;
							if( g_cSystem.GetXdata( pEnt , _T("INDEX") , iIndex ) )
							{
								CString strPre ;
								int iNo = 0 ;
								CString strZHStartNo ;								

								if ( iType == 32 ) //����ӿڹ� ;
								{
									strZHStartNo.Format(_T("%s"), pData->chZZH_StartNo) ;
									CBlkUtility::Compute_Pre_Bh(strZHStartNo, strPre, iNo) ;	
								}
								else if ( iType == 33 ) //��ϼ�
								{
									strZHStartNo.Format(_T("%s"), pData->chZ_StartNo) ;
									CBlkUtility::Compute_Pre_Bh(strZHStartNo, strPre, iNo) ;	
								}
								else if (iType==50) //������,��F1-2
								{
									strZHStartNo.Format(_T("%s"), pData->chFXP_StartNo) ;
									CBlkUtility::Compute_Pre_Bh(strZHStartNo, strPre, iNo) ;	
								}

								strPre.MakeUpper() ;
								strTemp.Format( _T("%s%d") , strPre, iNo + iIndex - 1 ) ;
								pText->setTextString(strTemp) ;
								if( iType == 33 ) //��ϼ� ;��Ҫд G ������ ;
								{
									try
									{
										CADORecordset cSet ;
										CString strSql ;
										strSql.Format( _T("SELECT myself_G_name, circuit_type, myself_qz_loc FROM read_symbol_info WHERE myself_QZ_loc = '%s'") , strTemp ) ;

										if( cSet.Open( g_PtrCon , strSql ) )
										{
											if( cSet.GetRecordCount() > 0 )
											{
												cSet.MoveFirst() ;
												CString strMySelf_G_Name , strCircuit_Type ;
												cSet.GetFieldValue( _T("myself_G_name") , strMySelf_G_Name ) ;
												cSet.GetFieldValue( _T("circuit_type") , strCircuit_Type ) ;
												AcGePoint3d base_3d( 0,0,0 ) ;
												base_3d = pText->position() ;
												ads_point base_Pt = { 0,0,0 } ;
												g_cSystem.ConvertPt3DToAds(base_3d , base_Pt) ;
												base_Pt[X] = base_Pt[X] - 20 + 4 ;
												base_Pt[Y] = base_Pt[Y]  + 1.25 ;
												g_cSystem.AddText(strMySelf_G_Name , base_Pt, base_Pt, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5) ;

												g_cSystem.ConvertPt3DToAds( base_3d , base_Pt ) ;
												base_Pt[X] = base_Pt[X] + 20 + 4 ;
												base_Pt[Y] = base_Pt[Y]  + 1.25 ;												
												g_cSystem.AddText(strCircuit_Type , base_Pt, base_Pt, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5) ;
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
										AfxMessageBox(_T("δ֪�Ĵ���!"));
									}
								}
							}
							break ;
						}						

					case 27:
						{
							strTemp.Format(_T("%sվ"), m_strStation) ;
							pText->setTextString( strTemp ) ;
							break ;
						}
					case 28 :
						{							
							strTemp.Format(_T("%s"), pData->chDate) ;
							pText->setTextString(strTemp) ;
							break ;
						}
					case 29 :
						{	
							strTemp.Format(_T("%s"), pData->chDrawingNo) ;
							pText->setTextString(strTemp) ;
							break ;
						}
					case 31:
						{
							strTemp.Format(_T("%s"), pData->chDrawingName) ;
							pText->setTextString(strTemp) ;
							break;
						}					
					}
				}
			}
			pEnt->close() ;
		} // end if(Acad::eOk == pBlkTblRecIter->getEntity( pEnt ,AcDb::kForWrite ) )
	}//end for 
	delete pBlkTblRecIter ;
	pBlkTblRecIter = NULL ;
}

CString CZnDmh::CutStringBack(const CString &strVal)
{
	CString strReturn ;
	strReturn = strVal ;
	int iLength = 0 ;
	iLength = strVal.GetLength() ;
	if(iLength > 1)
	{
		if(strVal.Mid(iLength - 1) == _T("-")) 
		{
			strReturn = strVal.Mid(0 , iLength - 1) ;
		}
	}
	return strReturn ;

}

AcDbDatabase *  CZnDmh::GetCurDatabase(CArray < AcDbDatabase * , AcDbDatabase * >  *  pDbArray, int iCur_No , CString strDwgType)
{
	AcDbDatabase * pCurDb ;
	if(iCur_No <= 0)
	{
		iCur_No = 1 ;
	}
	int iDatabase_Number = 0 ;
	iDatabase_Number = pDbArray ->GetSize() ;
	if(iCur_No > iDatabase_Number ) 
	{
		for (int i = 0 ; i <  iCur_No - iDatabase_Number ; i ++)
		{
			AcDbDatabase * pTemp = new AcDbDatabase ;
			CString strFileName ;
			strFileName.Format (_T("%s\\support\\pxtdwt\\%s.dwg") ,g_strBasePath , strDwgType) ;
			pTemp->readDwgFile(strFileName) ;
			//����ͼֽ��ŵ���Ϣ
			pDbArray->Add(pTemp) ;
		}
	}
	pCurDb = pDbArray->GetAt(iCur_No - 1) ;
	return pCurDb ;
}

BOOL CZnDmh::Fill_F_Table(CString strMySelf_Text ,CString strFill_Text , CString &strErrorText , long lCon_Cable_Id , long lCon_Cable_No)
{
	BOOL bReturn = FALSE ;

	//ÿ��ŵİ����� ��
	int iBlockSum = 13 ; 

	double dBlock_Width = 30.000 , dBlock_Heigth = 108.000 ; //�˴��Ŀ�ָ����2���01-13��ģ��
	double dCell_Width = 25.000 , dCell_Height = 7.000 ;
	double dBetweenBlock =5.000 ; 
	double dBetweenLayer = 17.000 ; //����2����
	double dLeftBlockWidth = 5.000 ; //������������

	ads_point base_Pt = { 0,0,0 } ;

	CStringArray strArraySelf ;
	CBlkUtility::DivideString(strMySelf_Text, _T("-"), strArraySelf) ;
	//F1-908-5,�˴���F1�������ִ�1~n��9�����ִ�1~10������10�㲻�ã�08�����ִ�1~13,5�����ִ�1~6������ֱ���i4~i1��ʾ
	int i1 = 0, i2 = 0, i3 = 0, i4 = 0 ;					
	if (strArraySelf.GetCount()==3)
	{
		CString str1, str2, str3 ;
		str1 = strArraySelf.GetAt(2) ;
		str2 = strArraySelf.GetAt(1) ;
		str3 = strArraySelf.GetAt(0) ;

		i4 = _tstoi(str1) ;
		i3 = _tstoi(str2.Right(2)) ;
		i2 = _tstoi(str2.Left(str2.GetLength()-2)) ;
		i1 = _tstoi(str3.Right(str3.GetLength()-1)) ;
	}

	if(strArraySelf.GetCount()==3)
	{
		int iLayer = (i2+1)%2 ; //�����ǵ�1�㣬�����0��
        base_Pt[X] =  (i3-1) * (dBlock_Width+dBetweenBlock)+dLeftBlockWidth+dCell_Width/2-2 ;
		base_Pt[Y] = iLayer*(dBlock_Heigth+dBetweenLayer)-(i4-1)*dCell_Height*2-dCell_Height-dCell_Height/2 ;
		if (strFill_Text.Find(_T("FS"))!=-1||strFill_Text.Find(_T("JS"))!=-1) 
		{
			base_Pt[Y]+=dCell_Height ;
			if (strFill_Text.Find(_T("FSH"))!=-1||strFill_Text.Find(_T("JSH"))!=-1) //FS�������ظ�
			{
				//�������ƴ����ı�
				AcGePoint3d pt_3d_bottomleft(0,0,0), pt_3d_topright(0,0,0) ;
				ads_point pt_name = {0,0,0} ;
				pt_3d_bottomleft.x = (i3-1) * (dBlock_Width+dBetweenBlock)+10 ;
				pt_3d_topright.x = (i3-1) * (dBlock_Width+dBetweenBlock)+dBlock_Width ;
				pt_3d_bottomleft.y = iLayer*(dBlock_Heigth+dBetweenLayer)+8*2 ;
				pt_3d_topright.y = pt_3d_bottomleft.y+8 ;
				CLongArray iArrayEntHandle ;
				int iFindSum = g_cSystem.FindEntInArea(pt_3d_bottomleft, pt_3d_topright, iArrayEntHandle, 1, 1) ;
				CString strGuiDao ;
				strGuiDao = strFill_Text.Left(strFill_Text.GetLength()-3) ;
				if (iFindSum==0)
				{
					pt_name[X] =  pt_3d_bottomleft.x+(pt_3d_topright.x-pt_3d_bottomleft.x)/2 ;
					pt_name[Y] = pt_3d_bottomleft.y+(pt_3d_topright.y-pt_3d_bottomleft.y)/2 ;
					g_cSystem.AddText(strGuiDao, pt_name, pt_name, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8) ;
				}
				else if (iFindSum==1)
				{
					AcDbEntity *pEntTmp = NULL ;
					if (g_cSystem.OpenAcDbEntity(iArrayEntHandle.GetAt(0), pEntTmp, AcDb::kForWrite))
					{
						AcDbText *pText = AcDbText::cast(pEntTmp) ;
						CString strText = pText->textString() ;
						strText.AppendFormat(_T(",%s"), strGuiDao) ;
						pText->setTextString(strText) ;
						pEntTmp->close() ;
					}
				}
			}
		}
		g_cSystem.AddText(strFill_Text, base_Pt, base_Pt, AcDb::kTextCenter, AcDb::kTextVertMid, 2.5, 0.8) ;

		if(lCon_Cable_Id != 0)
		{
			ads_point base_pt_con = {base_Pt[0]-6, base_Pt[1],0} ;
			this->Fill_Connect_Cable_Block (base_pt_con , lCon_Cable_Id) ;
		}
		if(lCon_Cable_No != 0)
		{
			CStringArray strAry ;
			strAry.RemoveAll () ;
			CString strTemp ;
			strTemp.Format (_T("%d") , lCon_Cable_No) ;
			strAry.Add (strTemp) ;
			AcDbBlockReference * pRef  = NULL ;
			AcGePoint3d temp_3d (0,0,0);
			temp_3d[X] = base_Pt[X] + 9.5 ;
			temp_3d[Y] = base_Pt[Y] ;
			if(g_cSystem.InsertBlock (pRef, _T("CONTI_2") , temp_3d , &strAry, FALSE))
			{
				pRef->close () ;
			}
		}

		bReturn = TRUE ;
	}
	return bReturn ;
}

//��Ϲ���ۺϹ����߱�ṹһ�������Թ��ô˺�����iZorZZH=0Ĭ�ϱ�ʾ��Ϲ�
BOOL CZnDmh::Fill_Z_Table(CString strMySelf_Text ,CString strFill_Text , CString &strErrorText , long lCon_Cable_Id , long lCon_Cable_No, int iZorZZH)
{
	//ÿ��ŵİ��������˴���5�죬ԭ����3�� ��
	int iBlockSum = 5 ; 
	//ÿ����ӵ�����������6��ԭ����12 ��
	int iColSum = 6 ;

	BOOL bReturn = FALSE ;
	int iZ_Pre_Len = 0 ;
	iZ_Pre_Len = m_strPreZ.GetLength() ;

	//double dBlock_Width = 255 ,dBlock_Heigth = 143.5 ;
	double dBlock_Width = 137.27 , dBlock_Heigth = 142.79 ;
	double dCell_Width = 20 , dCell_Height = 7 ;
	//CString strErro_Info = _T("��ϼ����ߴ�����Ϣ\n") ;
	int iMySelfCol = 0 , iMySelfRow = 0 ;
	CString strZ_Loc ;
	int iMySelf_Block = 0 ; //�ڼ������ ;
	int iMySelf_Z_No = 0 ; //�ڼ������� ;
	CString strMySelf_Z_Pre ;
	CString strTempPre , strTempPre2 ;
	ads_point base_Pt = { 0,0,0 } ;
	if(this->Divide_Z_Text(strMySelf_Text , strMySelf_Z_Pre , iMySelf_Z_No , iMySelf_Block , iMySelfCol , iMySelfRow))
	{
		//ÿ��ͼֽ���������,����ڲ�ͬ����ʱ������ ;
		int iBlock_Row = 0 , iBlock_Col = 0 ;
		iBlock_Row =  (iMySelf_Block - 1)  / iBlockSum ;
		iBlock_Col =  (iMySelf_Block - 1) % iBlockSum ;
		base_Pt[X] =  iBlock_Col * dBlock_Width + (iColSum - iMySelfCol) * dCell_Width + 0.6 ;
		base_Pt[Y] = 0 - iBlock_Row * dBlock_Heigth - (iMySelfRow - 1) * dCell_Height - dCell_Height/4 ;

		//��ʼд ;
		int iFill_Qz_No = 0 , iFill_Block = 0 , iFill_Col = 0 , iFill_Row = 0 ;
		CString strFill_Qz_Pre ;
		CString strWrite_Text ;
		if(this->Divide_Z_Text(strFill_Text , strFill_Qz_Pre ,iFill_Qz_No , iFill_Block , iFill_Col , iFill_Row))
		{
			if(strFill_Qz_Pre == strMySelf_Z_Pre)
			{
				if(iFill_Qz_No == iMySelf_Z_No)
				{
					if(iFill_Block == iMySelf_Block)
					{
						if(iFill_Col == iMySelfCol)
						{
							strWrite_Text.Format(_T("%d") , iFill_Row) ;
						}
						else
						{
							if (iFill_Col < 10)
							{
								strWrite_Text.Format(_T("0%d-%d") , iFill_Col ,iFill_Row) ;
							}
							else
							{
								strWrite_Text.Format(_T("%d-%d") , iFill_Col ,iFill_Row) ;
							}
						}
					}
					else
					{
						if(iFill_Col < 10)
						{
							strWrite_Text.Format(_T("%d0%d-%d") , iFill_Block ,iFill_Col ,iFill_Row) ;
						}
						else
						{
							strWrite_Text.Format(_T("%d%d-%d") , iFill_Block ,iFill_Col ,iFill_Row) ;
						}
					}
				}
				else
				{
					strWrite_Text = strFill_Text ;
				}
			}
			else
			{
				strWrite_Text = strFill_Text ;
			}
		}
		else
		{
			strWrite_Text = strFill_Text ;
		}
		if(strFill_Text == _T("QKZ") || strFill_Text == _T("QKF") || strFill_Text == _T("XQZ") || strFill_Text == _T("XQF") ||
			strFill_Text == _T("SQZ") || strFill_Text == _T("SQF") || strFill_Text == _T("+24") || strFill_Text == _T("-24") || strFill_Text == _T("024") ||
			strFill_Text == _T("JZ") || strFill_Text == _T("JF") || strFill_Text == _T("GJZ220") || strFill_Text == _T("GJF220") || strFill_Text == _T("KZ") || strFill_Text == _T("KF"))
		{
			base_Pt[X] = base_Pt[X] + 10 ;
			base_Pt[Y]=base_Pt[Y] - 2.8  ;
		}
		else
		{
			CADORecordset cRS ;
			CString strSql ;
			try
			{
				strSql.Format(_T("select * from dmh_have_con_tie_z where shelf_no=%d and block_no=%d and col_no=%d and row_no=%d and z_or_zzh=%d"), iMySelf_Z_No,iMySelf_Block,iMySelfCol,iMySelfRow, iZorZZH);
				if(cRS.IsOpen())
				{
					cRS.Close() ;
				}				
				if(cRS.Open(g_PtrCon ,strSql))
				{
					if (cRS.GetRecordCount()==0)
					{
						CString strExec ;
						strExec.Format(_T("insert into dmh_have_con_tie_z (shelf_no, block_no, col_no , row_no, tie_number, tie_text, z_or_zzh) values  \
									   (%d, %d , %d , %d , %d, '%s', %d) ") , iMySelf_Z_No , iMySelf_Block , iMySelfCol , iMySelfRow , 1, strFill_Text, iZorZZH) ;
						ExecSqlString (strExec) ;
					}
					else
					{
						int iTemp = 0 ;
						cRS.GetFieldValue(_T("tie_number"),iTemp);
						CString strHaveTie ;
						cRS.GetFieldValue(_T("tie_text"), strHaveTie) ;
						strHaveTie.AppendFormat(_T(",%s"), strFill_Text) ;
						iTemp = iTemp + 1 ;
						cRS.Edit() ;
						cRS.SetFieldValue(_T("tie_number"),iTemp);
						cRS.SetFieldValue(_T("tie_text"),strHaveTie);
						cRS.Update() ;
						base_Pt[Y]=base_Pt[Y] - dCell_Height/2  ;
					}
					cRS.Close();
				}
			}
			catch(_com_error& e)
			{
				CString sBuff = CBlkUtility::GetErrorDescription(e);
				AfxMessageBox(sBuff);        
			}
			catch(...)
			{	
				AfxMessageBox(_T("δ֪�Ĵ���!"));
			}
		}
		strWrite_Text.MakeUpper() ;
		//g_cSystem.WriteText(base_Pt , strWrite_Text , 0 , 1) ;
		g_cSystem.AddText(strWrite_Text, base_Pt, base_Pt, AcDb::kTextLeft, AcDb::kTextVertMid, 2, 0.7) ;
		if(lCon_Cable_Id != 0)
		{
			this->Fill_Connect_Cable_Block (base_Pt , lCon_Cable_Id) ;
		}
		if(lCon_Cable_No != 0)
		{
			CStringArray strAry ;
			strAry.RemoveAll () ;
			CString strTemp ;
			strTemp.Format (_T("%d") , lCon_Cable_No) ;
			strAry.Add (strTemp) ;
			AcDbBlockReference * pRef  = NULL ;
			AcGePoint3d temp_3d (0,0,0);
			temp_3d[X] = base_Pt[X] + 15 ;
			temp_3d[Y] = base_Pt[Y] ;
			if(g_cSystem.InsertBlock (pRef, _T("CONTI_2") , temp_3d , &strAry, FALSE))
			{
				pRef->close () ;
			}
		}
		bReturn = TRUE ;
	}
	return bReturn ;
}

BOOL CZnDmh::Divide_Z_Text(CString strText ,CString & strZ_Pre , int & iZ_No , int & iBlock , int & iCol , int & iRow) 
{
	int iFirstMark = 0 ;
	int iSecondMark = 0 ;
	int iLength = 0 ;
	BOOL bReturn = FALSE ;
	int iLoc = 0 ;
	iLoc = strText.Find (_T("-") , 0) ;

	iFirstMark = iLoc ;
	iSecondMark = strText.Find(_T('-'),iLoc+1) ;
	iLength = iSecondMark - iFirstMark - 1 ;

	if(iLoc != -1)
	{
		CString strZ_Loc , strDivide_Text;
		if(strText.GetLength() > iLoc + 2)
		{
			if (iLength == 3) 
			{	
				strZ_Loc = strText.Mid (0 , iLoc + 2) ;
				strDivide_Text = strText.Mid (iLoc + 2) ;
			}
			else if(iLength == 4)
			{
				strZ_Loc = strText.Mid (0 , iLoc + 3) ;
				strDivide_Text = strText.Mid (iLoc + 3) ;
			}
			CString strPre    ;
			CBlkUtility::Compute_Pre_Bh(strZ_Loc , strPre , iBlock) ;
			strPre = this->CutStringBack(strPre) ;
			CBlkUtility::Compute_Pre_Bh(strPre , strZ_Pre , iZ_No) ;
			this->Divide_Text(strDivide_Text , iCol , iRow) ;
			//if(iZ_No > 0 && iBlock > 0 && iBlock <= 10 && iCol > 0 && iCol <= 6 && iRow >0 && iRow <= 18)
			if(iZ_No == 0)
			{
				iZ_No = 1 ;
			}
			if(iZ_No > 0 && iBlock > 0 && iBlock <= 10 && iCol > 0 && iCol <= 6 && iRow >0 && iRow <= 18)

			{
				bReturn = TRUE ;
			}
		}
	}
	return bReturn ;

}

BOOL CZnDmh::Divide_Text(CString strText, int &iFront, int &iBack)
{
	BOOL bReturn = FALSE ;

	int iLoc = -1 ;
	iLoc = strText.Find(_T("-") , 0) ;
	if( iLoc != -1)
	{
		iFront = _tstoi (strText.Mid(0 , iLoc)) ;
		iBack = _tstoi(strText.Mid(iLoc + 1));
		bReturn = TRUE ;
	}
	return bReturn ;
}

void CZnDmh::Fill_Connect_Cable_Block(ads_point base_Pt, long lId)
{
	try
	{
		CString strSql ;
		strSql.Format(_T("select * from tie_con_cable_info order by con_cable_id")) ;
		CADORecordset cSet ;
		if(cSet.IsOpen ())
		{
			cSet.Close () ;
		}
		if(cSet.Open(g_PtrCon ,strSql))
		{
			if(cSet.GetRecordCount() > 0)
			{
				cSet.MoveFirst () ;
				CString strFind ;
				strFind.Format (_T("con_cable_id = %d") , lId) ;
				if(cSet.FindFirst(strFind))
				{
					CString strBlockName , strCon_Cable_Text ;
					cSet.GetFieldValue(_T("blockname") , strBlockName) ;
					if(strBlockName != _T(""))
					{
						cSet.GetFieldValue(_T("con_cable_text") , strCon_Cable_Text ) ;
						CStringArray strAry ;
						strAry.RemoveAll() ;
						strAry.Add (strCon_Cable_Text) ;
						AcDbBlockReference * pRef = NULL ;
						AcGePoint3d base_3d (0,0,0) ;
						base_3d[X] = base_Pt[X] + 15.5  ;
						base_3d[Y] = base_Pt[Y] ;
						if(g_cSystem.InsertBlock(pRef , strBlockName , base_3d , &strAry, FALSE))
						{
							pRef->close () ;
						}
					}
				}
			}
			cSet.Close () ;
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!"));
	}
}

void CZnDmh::Save_PX_Drawing (CString strPath ,CString strType , CArray<AcDbDatabase  *, AcDbDatabase * >  *pDbArray)
{
	CString strFileName ;
	if ( strType == _T("Z") )
	{
		strFileName = _T("վ����Ϲ�������߱�") ;
	}
	else if ( strType == _T("ZY") )
	{
		strFileName = _T("վ����Ƶ��������߱�") ;
	}
	else if (strType == _T("ZZH")) 
	{
		strFileName = _T("վ���ۺϹ�������߱�") ;
	}
	else if (strType == _T("F")) 
	{
		strFileName = _T("���������߱�") ;
	}
	else if (strType == _T("HLP")) 
	{
		strFileName = _T("���������߱�") ;
	}
	// 	else if (strType == _T("QZN")) 
	// 	{
	// 		strFileName = _T("��ϼ����߱�(N)") ;
	// 	}
	// 	else if ( strType == _T("QZH") )
	// 	{
	// 		strFileName = _T("����ӿڹ����߱�") ;
	// 	}
	// 	else if(strType == _T("OTHER"))
	// 	{
	// 		strFileName = _T("�����������߱�") ;
	// 	}
	// 	else if(strType == _T("GLB"))
	// 	{
	// 		strFileName = _T("�����ѹ�����߱�") ;
	// 	}
	// 	else if(strType == _T("G"))
	// 	{
	// 		strFileName = _T("��������߱�") ;
	// 	}

	int iDrawing_Number = 0 ;
	iDrawing_Number = pDbArray->GetSize() ;
	for (int i=0 ; i<iDrawing_Number ; i++)
	{		
		CString strNumber ;
		strNumber = CBlkUtility::NumToCapsString(i+1) ;
		g_pCurDb = pDbArray->GetAt(i) ;
		g_cSystem.m_pDb = g_pCurDb ;
		//��ʼ��ͼ������ ;
		PXT_DATA pxtData ;		
		//TCHAR *pPxtTmp ;		
		//pPxtTmp =  (LPTSTR)(LPCTSTR)(CBlkUtility::GetDateStr()) ;
		//_tcscpy_s(pxtData.chDate, sizeof(pxtData.chDate)/sizeof(pxtData.chDate[0]), pPxtTmp) ;
		//_tcscpy_s(pxtData.chDate, pPxtTmp) ;
		CBlkUtility::tcscpy( pxtData.chDate , (CBlkUtility::GetDateStr().GetLength()+1)*sizeof(TCHAR), CBlkUtility::GetDateStr() ) ;
		//pPxtTmp =  (LPTSTR)(LPCTSTR)(m_strStation) ;
		//_tcscpy_s(pxtData.chStationName, pPxtTmp) ;
		CBlkUtility::tcscpy( pxtData.chStationName , (m_strStation.GetLength()+1)*sizeof(TCHAR), m_strStation ) ;
		//pPxtTmp =  (LPTSTR)(LPCTSTR)(m_strDwgSn) ;
		//_tcscpy_s(pxtData.chDrawingNo, pPxtTmp) ;
		CBlkUtility::tcscpy( pxtData.chDrawingNo , (m_strDwgSn.GetLength()+1)*sizeof(TCHAR), m_strDwgSn ) ;

		CString strNum ;
		strNum = CBlkUtility::NumToCapsString( i + 1 ) ;
		CString strDrawingName_Pre ;

		if ( strType == _T("Z")  )
		{
			strDrawingName_Pre = _T("վ����Ϲ�������߱�") ;
			CString strZStartNo ;
			strZStartNo.Format(_T("%s%d-1"), m_strPreZ, i+1) ;
			//pPxtTmp =  (LPTSTR)(LPCTSTR)(strZStartNo) ;
			//_tcscpy_s(pxtData.chZ_StartNo, pPxtTmp) ;
			CBlkUtility::tcscpy( pxtData.chZ_StartNo , (strZStartNo.GetLength()+1)*sizeof(TCHAR), strZStartNo ) ;

			/*cData.strQz_Start_No.Format(_T("%s%d-1"), m_strPreZ, i+1) ;		*/	
		}
		else if (strType == _T("ZY") )
		{
			strDrawingName_Pre = _T("վ����Ƶ��������߱�") ;
		}
		else if (strType == _T("ZZH"))
		{
			strDrawingName_Pre = _T("վ���ۺϹ�������߱�") ;
			CString strZZHStartNo ;
			if (iDrawing_Number==1)
			{
				//cData.strQzh_Start_No.Format(_T("%s-1"), m_strPreZZH) ;

				strZZHStartNo.Format(_T("%s-1"), m_strPreZZH) ;
			}
			else
			{
				//cData.strQzh_Start_No.Format(_T("%s%d-1"),m_strPreZZH, i+1) ;
				strZZHStartNo.Format(_T("%s%d-1"),m_strPreZZH, i+1) ;
			}
			//pPxtTmp =  (LPTSTR)(LPCTSTR)(strZZHStartNo) ;
			//_tcscpy_s(pxtData.chZZH_StartNo, pPxtTmp) ;
			CBlkUtility::tcscpy( pxtData.chZZH_StartNo , (strZZHStartNo.GetLength()+1)*sizeof(TCHAR), strZZHStartNo ) ;

		}
		else if (strType == _T("F") )
		{
			strDrawingName_Pre.Format(_T("����������ͼ-F%d(%s)"), (i/5)+1, CBlkUtility::DigitToCapsString(i%5+1)) ;			
			CString strFXPStartNo ;
			strFXPStartNo.Format(_T("F%d-%d"), (i/5)+1,i%5*2+1) ;
			//pPxtTmp =  (LPTSTR)(LPCTSTR)(strFXPStartNo) ;
			//_tcscpy_s(pxtData.chFXP_StartNo, pPxtTmp) ;
			CBlkUtility::tcscpy( pxtData.chFXP_StartNo , (strFXPStartNo.GetLength()+1)*sizeof(TCHAR), strFXPStartNo ) ;
		}
		else if (strType == _T("HLP") )
		{
			strDrawingName_Pre = _T("���������߱�") ;
		}

		CString strDrawingName ;
		if( iDrawing_Number != 1 )
		{
			strDrawingName.Format( _T("%s(%s)") , strDrawingName_Pre , strNum ) ;
		}
		else
		{
			strDrawingName.Format( _T("%s") , strDrawingName_Pre  ) ;
		}
		//pPxtTmp =  (LPTSTR)(LPCTSTR)(strDrawingName) ;
		//_tcscpy_s(pxtData.chDrawingName, pPxtTmp) ;
		CBlkUtility::tcscpy( pxtData.chDrawingName , (strDrawingName.GetLength()+1)*sizeof(TCHAR), strDrawingName ) ;

		this->DrawAllPx( &pxtData ) ;

		//������� ;

		//����ļ�;
		CString strFilePath ;
		if( iDrawing_Number != 1 )
		{
			strFilePath.Format( _T("%s\\%s(%s).dwg") , strPath , strFileName , strNumber ) ;
			if (strType==_T("F"))
			{
				strFilePath.Format( _T("%s\\%sF%d(%d-%d)(%s).dwg") , strPath , strFileName , i/5+1, i%5*2+1, i%5*2+2, strNumber ) ;
			}
		}
		else
		{
			strFilePath.Format( _T("%s\\%s.dwg") , strPath , strFileName ) ;
		}
		g_pCurDb->saveAs( strFilePath ) ;
		delete g_pCurDb ;
		g_pCurDb  = NULL ;

	}
}

void CZnDmh::Fill_Circuit_Data_XXX(CCircuit_Data * pData)
{
	CString strXXX_Val = _T("XXXX") ;
	if( pData->str1FsLoc == _T("") )
	{
		pData->str1FsLoc = strXXX_Val ;
	}
	if( pData->strBack_G_Name == _T("") )
	{
		pData->strBack_G_Name = strXXX_Val ;
	}
	if( pData->strBack_QZ_Loc == _T("") )
	{
		pData->strBack_QZ_Loc = strXXX_Val ;
	}
	if( pData->strBack_Symbol_Name == _T("") )
	{
		pData->strBack_Symbol_Name = strXXX_Val ;
	}
	if( pData->strCombine_Frequencry == _T("") )
	{
		pData->strCombine_Frequencry = strXXX_Val ;
	}
	if( pData->strCombine_G_Name == _T("") )
	{
		pData->strCombine_G_Name = strXXX_Val ;
	}
	if( pData->strCombine_Js_RD_Name == _T("") )
	{
		pData->strCombine_Js_RD_Name = strXXX_Val ;
	}
	if( pData->strCombine_Js_RD_Start_Tie == _T("") )
	{
		pData->strCombine_Js_RD_Start_Tie = strXXX_Val ;
	}
	if( pData->strCombine_QY_Loc == _T("") )
	{
		pData->strCombine_QY_Loc = strXXX_Val ;
	}
	if( pData->strData == _T("") )
	{
		pData->strData = strXXX_Val ;
	}
	if( pData->strDd_Start_Tie == _T("") )
	{
		pData->strDd_Start_Tie = strXXX_Val ;
	}
	if( pData->strDrawingName == _T("") )
	{
		pData->strDrawingName = strXXX_Val ;
	}
	if( pData->strDrawingNo == _T("") )
	{
		pData->strDrawingNo = strXXX_Val ;
	}
	if( pData->strFront_G_Name == _T("") )
	{
		pData->strFront_G_Name = strXXX_Val ;
	}
	if( pData->strFront_QZ_Loc == _T("") )
	{
		pData->strFront_QZ_Loc = strXXX_Val ;
	}
	if( pData->strFront_Symbol_Name == _T("") )
	{
		pData->strFront_Symbol_Name = strXXX_Val ;
	}
	if( pData->strFsJs_Start_Tie == _T("") )
	{
		pData->strFsJs_Start_Tie = strXXX_Val ;
	}
	if( pData->strFsJs_Face_Start_Tie == _T("") )
	{
		pData->strFsJs_Face_Start_Tie = strXXX_Val ;
	}
	if( pData->strGLB_RD_Start_Tie == _T("") )
	{
		pData->strGLB_RD_Start_Tie = strXXX_Val ;
	}
	if( pData->strMySelf_Frequencry == _T("") )
	{
		pData->strMySelf_Frequencry = strXXX_Val ;
	}
	if( pData->strMySelf_Fs_RD_Name == _T("") )
	{
		pData->strMySelf_Fs_RD_Name = strXXX_Val ;
	}
	if( pData->strMySelf_FsJs_RD_Start_Tie == _T("") )
	{
		pData->strMySelf_FsJs_RD_Start_Tie = strXXX_Val ;
	}
	if( pData->strMySelf_G_Name == _T("") )
	{
		pData->strMySelf_G_Name = strXXX_Val ;
	}
	if( pData->strMySelf_GLB_Loc == _T("") )
	{
		pData->strMySelf_GLB_Loc = strXXX_Val ;
	}
	if( pData->strMySelf_GLB_RD_Name == _T("") )
	{
		pData->strMySelf_GLB_RD_Name = strXXX_Val ;
	}
	if( pData->strMySelf_Js_RD_Name == _T("") )
	{
		pData->strMySelf_Js_RD_Name = strXXX_Val ;
	}
	if( pData->strStation == _T("") )
	{
		pData->strStation = strXXX_Val ;
	}
	if( pData->strQzh_Start_No == _T("") )
	{
		pData->strQzh_Start_No = strXXX_Val ;
	}
	if( pData->strQz_Start_No == _T("") )
	{
		pData->strQz_Start_No = strXXX_Val ;
	}
	if( pData->strMySelf_QZH_Loc == _T("") )
	{
		pData->strMySelf_QZH_Loc = strXXX_Val ;
	}
	if( pData->strMySelf_QZ_Loc == _T("") )
	{
		pData->strMySelf_QZ_Loc = strXXX_Val ;
	}
	if( pData->strMySelf_QY_Loc == _T("") )
	{
		pData->strMySelf_QY_Loc = strXXX_Val ;
	}
	if( pData->strMySelf_Name == _T("") )
	{
		pData->strMySelf_Name = strXXX_Val ;
	}
	if( pData->str1Lq_Qz_Loc == _T("") )
	{
		pData->str1Lq_Qz_Loc = strXXX_Val ;
	}
}

//begin����F��ͷ������
void CZnDmh::Draw_PX_FXP(CArray < AcDbDatabase * , AcDbDatabase * >  *  pDbArray  , CString & strError_Text) 
{
	double dSpaceX_Table = 5.00 , dSpaceY_Table = 17.50 , 
		dExtendX = 2.00 , dExtendY = 2.00 ,
		dWidth_Table = 20.00 , dHeigth_Table = 7.00;
	//CString strErro_Info = _T("�ۺϼ����ߴ�����Ϣ\n") ;

	try
	{	
		//cTie_Set   ���еĶ��ӵļ�¼�� ��
		//cTie_Set3   �Է�'zzh'��ͷ�ģ�����zzh1-203-4�Ķ��� ��
		CADORecordset cTie_Set, cTie_Set3 ;
		CString strSql2 ,strSql3;
		//
		strSql2.Format(_T("select * from tie_info order by ID")) ;
		if(cTie_Set.Open(g_PtrCon , strSql2))
		{
			if(cTie_Set.GetRecordCount() > 0)
			{
				cTie_Set.MoveFirst() ;
			}
		}

		//begin������F��ͷ�Ķ��ӣ�
		//begin�ȴ�����'z'��ͷ�Ķ��ӣ�����z2-304-2��
		strSql3.Format(_T("select * from tie_info where Left(tie_text,1) = 'F' order by ID")) ;
		if(cTie_Set3.Open(g_PtrCon ,strSql3))
		{			
			if(cTie_Set3.GetRecordCount() > 0)
			{
				cTie_Set3.MoveFirst() ;
				CString strMySelf_Text ; //F1-908-5
				int iMySelfCol = 0 ; //
				int iMySelfRow = 0 ; //
				int iPxtDwgIndex = 0 ;
				CString strZ_Loc ;  //������ ��
				int iMySelf_Block = 0 ; //�ڼ��� ;
				
				int iDwgNo = 0 ; //�ڼ���ͼֽ,���ƹ��Ӻ�
				long lCon_1_Id = 0 ;//�����1 ;
				long lCon_2_Id = 0 ;//���ӵ�2 ;
				CString strTempPre , strTempPre2 ;

				ads_point base_Pt = { 0,0,0 } ;

				while (!cTie_Set3.IsEOF())
				{					
					cTie_Set3.GetFieldValue(_T("tie_text") , strMySelf_Text) ;
					CStringArray strArrayTie ;
					CBlkUtility::DivideString(strMySelf_Text, _T("-"), strArrayTie) ;
					//F1-908-5,�˴���F1�������ִ�1~n��9�����ִ�1~10������10�㲻�ã�08�����ִ�1~13,5�����ִ�1~6������ֱ���i4~i1��ʾ
					int i1 = 0, i2 = 0, i3 = 0, i4 = 0 ;					
					if (strArrayTie.GetCount()==3)
					{
						CString str1, str2, str3 ;
						str1 = strArrayTie.GetAt(2) ;
						str2 = strArrayTie.GetAt(1) ;
						str3 = strArrayTie.GetAt(0) ;

						i4 = _tstoi(str1) ;
						i3 = _tstoi(str2.Right(2)) ;
						i2 = _tstoi(str2.Left(str2.GetLength()-2)) ;
						i1 = _tstoi(str3.Right(str3.GetLength()-1)) ;
					}
					//begin�õ�ͼ�����ݿ�ĺ�
					iDwgNo = (i1-1)*5+(i2+1)/2 ;

					g_pCurDb = this->GetCurDatabase(pDbArray , iDwgNo , _T("F")) ;
					g_cSystem.m_pDb = g_pCurDb ;

					cTie_Set3.GetFieldValue(_T("con_1_Id") ,lCon_1_Id) ;
					cTie_Set3.GetFieldValue(_T("con_2_id") ,lCon_2_Id) ;
					long lCon_Cable_Id[2] = { 0 , 0 } ;
					cTie_Set3.GetFieldValue(_T("con_1_cable_id") , lCon_Cable_Id[0]) ;
					cTie_Set3.GetFieldValue(_T("con_2_cable_id") , lCon_Cable_Id[1]) ;

					long lConGdId = 0 ; //���ӵĹ��id
					cTie_Set3.GetFieldValue(_T("con_gd_id") , lConGdId) ;
					CString strTieType ;
					cTie_Set3.GetFieldValue(_T("tie_type") , strTieType) ; //��FS����FSH

					//��ѯ�͸ýӵ������Ľӵ�,���������� ;					
					CString strSql_Find , strResult = _T("") , strResult2 =_T("") ,
						strFind_Tie_Parent_Text , strFind_Tie_Text , 
						strFind_Tie_Parent_Text2 , strFind_Tie_Text2 ;

					long lCon_1_2_Id[2] ;
					lCon_1_2_Id[0] = lCon_1_Id ;
					lCon_1_2_Id[1] = lCon_2_Id ;

					//��һ�����Ӷ�Ӧ�ڵ� ;
					CString strFill_1_Text  ;
					CString strTemp_Error_Text ;
					for (int i = 0 ; i < 2 ; i ++)
					{
						strSql_Find.Format(_T("tie_id=%d") , lCon_1_2_Id[i]) ;
						if(cTie_Set.FindFirst(strSql_Find))
						{
							int iTie_Front = 0 , iTie_Back = 0 ;
							cTie_Set.GetFieldValue(_T("tie_parent_text") , strFind_Tie_Parent_Text) ;
							cTie_Set.GetFieldValue(_T("tie_text") , strFind_Tie_Text) ;
							strFill_1_Text.Format(_T("%s%s") , strFind_Tie_Parent_Text , strFind_Tie_Text) ;
							this->Fill_F_Table(strMySelf_Text , strFill_1_Text , strTemp_Error_Text , lCon_Cable_Id[i], 0);
						}
					}

					//�������10DGFS��10DGFSH
					strSql_Find.Format(_T("tie_id=%d") , lConGdId) ;
					cTie_Set.MoveFirst() ;
					if(cTie_Set.FindFirst(strSql_Find))
					{						
						cTie_Set.GetFieldValue(_T("tie_text") , strFind_Tie_Text) ; //����*IG+
						strFind_Tie_Text.TrimLeft(_T("*")) ;
						CString strFsOrJs = _T("FS") ;
						if (strFind_Tie_Text.Right(1)==_T("+"))
						{
							strFind_Tie_Text.TrimRight(_T("+")) ;
							strFsOrJs = _T("JS") ;
						}
						
						strFill_1_Text.Format(_T("%s%s%s") , strFind_Tie_Text, strFsOrJs, strTieType.Right(1)==_T("1")?_T(""):_T("H")) ;
						this->Fill_F_Table(strMySelf_Text , strFill_1_Text , strTemp_Error_Text , 0, 0);
					}


					cTie_Set3.MoveNext() ;	
				}
			}
			cTie_Set3.Close() ;
			//cTie_Set.Close() ; 
		}

		//end������F��ͷ�Ķ��ӽ�����

		cTie_Set.Close() ;

	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!"));
	}
}


void CZnDmh::Draw_PX_HLP(CArray < AcDbDatabase * , AcDbDatabase * >  *  pDbArray  , CString & strError_Text)
{
	double dBoxWidth = 45.000 ; //����H1-1�����Ŀ�
	double dCellWidth = 20.000 ;
	double dCellHeight = 7.000 ;
	double dBetweenBoxH = 20.000 ;
	double dBoxLeftWidth = 5.000 ;
	double dBetweenBoxV = 129.000 ;
	try
	{
		CADORecordset cSet ;
		CString strSql ;
		strSql.Format( _T("select * from tie_info where Left(con_1_text,1) = 'H' order by ID")) ;
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
			ads_point base_Pt = { 0,0,0 } ;
			while (!cSet.IsEOF())
			{
				CString strTie, strConTie, strConTiePar ;
				cSet.GetFieldValue(_T("con_1_text"), strTie) ;
				cSet.GetFieldValue(_T("tie_text"), strConTie) ;
				cSet.GetFieldValue(_T("tie_parent_text"), strConTiePar) ;
				CString strWriteText ;
				strWriteText.Format(_T("%s%s"), strConTiePar, strConTie) ;

				g_pCurDb = this->GetCurDatabase( pDbArray , 1 , _T("HLP") ) ;
				g_cSystem.m_pDb = g_pCurDb ;

                // Hi1-i20i3-i4: H1-302-8
				CStringArray strArrayRet ;
				CBlkUtility::DivideString(strTie, _T("-"), strArrayRet) ;
				if (strArrayRet.GetCount()==3)
				{
					CString str2, str3 ;
					str2 = strArrayRet.GetAt(1) ;
					str3 = strArrayRet.GetAt(2) ;
					int i1 = 1 ;
					int i2 = _tstoi(str2.Left(str2.GetLength()-2)) ;
					int i3 = _tstoi(str2.Right(1)) ;
					int i4 = _tstoi(str3) ;
					base_Pt[0]=(i2-1)%8*(dBoxWidth+dBetweenBoxH) + i3/2*dCellWidth + dCellWidth/2 ;
					base_Pt[1]=0-(i2-1)/8*dBetweenBoxV - (i4-1)*dCellHeight  - dCellHeight/2 ;
					g_cSystem.AddText(strWriteText, base_Pt, base_Pt, AcDb::kTextCenter, AcDb::kTextVertMid, 2, 0.7);
				}

				cSet.MoveNext() ;
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
		AfxMessageBox(_T("δ֪�Ĵ��� px-HLP!"));
	}
}

void CZnDmh::UpdateDbForHLP(void)
{
	CString strToCons[14] = {_T("H-F2"), _T("H-F3"), _T("H-F5"), _T("H-F9"), _T("H-F10"), _T("H-F11"), _T("H-F12"), _T("H-F14"), _T("H-F15"), _T("H-F16"), _T("H-F17"), _T("H-F18"), _T("H-1700"), _T("H-2000")} ;
	int iCellNumPerBox = 2*9 ; //H1-1��9*2������

	try
	{	
		CADORecordset cSet ;
		CString strSql ;

		strSql.Format(_T("select max(tie_id) as max_tie_id from tie_info")) ;
		int nMaxTieId ;
		CDBUtility::GetRecordValue(strSql, _T("max_tie_id"), nMaxTieId) ;

		//��n+1ͼ�У�����F5���Ķ���01-5���������Ҳ಻���й������ӣ���û��Ϊ������id���ʴ���һ��tie_idΪ0�ļ�¼���ڲ�ʹ��HLPʱû���⣩
		//��ʹ��HLPʱ������Ҫ������tie_idΪ-1�ĸ�Ϊ��-1��
		strSql.Format(_T("select * from tie_info where tie_id=-1")) ;
		CLongArray nArrayRet ;
		CDBUtility::GetRecordValue(strSql, _T("id"), nArrayRet) ;
		for (int i=0; i<nArrayRet.GetCount(); i++)
		{
			strSql.Format(_T("update tie_info set tie_id=%d where id=%d"), ++nMaxTieId, nArrayRet.GetAt(i)) ;
			ExecSqlString(strSql) ;
		}

// 		int iTieIndex = 0 ; //��¼ȫ���������ӵ���ţ��Ա���������λ��
		for (int i=0; i<14; i++)
		{
			strSql.Format(_T("select * from tie_info where con_1_text='%s' order by base_x asc"), strToCons[i]) ;
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
				int iTieIndex = 0 ; //��¼ȫ���������ӵ���ţ��Ա���������λ��
				while (!cSet.IsEOF())
				{
					int nTieId = 0 ;
					CString strCon1Text ; 
					cSet.GetFieldValue(_T("tie_id"), nTieId) ;
					//Hi1-i20i3-i4 H1-202-8
					int i1 = 1 ;
					int i2 = i+1 ;//(iTieIndex/iCellNumPerBox)+1 ; 
					int i3 = iTieIndex/9+1 ;
					int i4 = iTieIndex%9+1 ;
					strCon1Text.Format(_T("H%d-%d0%d-%d"), i1, i2, i3, i4) ;
					cSet.Edit() ;
					cSet.SetFieldValue(_T("con_1_Id"), ++nMaxTieId) ;
					cSet.SetFieldValue(_T("con_1_text"), strCon1Text) ;
					cSet.Update() ;
					strSql.Format(_T("insert into tie_info(tie_id, tie_text,con_1_Id) values(%d, '%s', %d)"), nMaxTieId, strCon1Text, nTieId) ;
					ExecSqlString(strSql) ;

					iTieIndex++ ;

					cSet.MoveNext() ;
				}

				cSet.Close() ;
			}
		}
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);        
	}
	catch(...)
	{	
		AfxMessageBox(_T("δ֪�Ĵ���!"));
	}
}

